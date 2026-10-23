#!/usr/bin/env python3
"""Read-only Mach-O image identity across ArcMod's ad-hoc re-signing.

Hash every pre-signature byte, normalizing only LC_CODE_SIGNATURE's locator and
__LINKEDIT's two signature-dependent size fields. Never modify either binary.
The signature must be the file tail, inside non-executable __LINKEDIT, after
every other segment. Symbol/binding metadata and all machine code remain hashed.
"""
import hashlib
import json
import mmap
from pathlib import Path
import struct
import sys


def fingerprint(path):
    path = Path(path)
    before = path.stat()
    with path.open("rb") as stream, mmap.mmap(
        stream.fileno(), 0, access=mmap.ACCESS_READ
    ) as data:
        if len(data) < 32:
            raise ValueError("truncated Mach-O")
        magic, cpu, _, _, count, command_bytes, _, _ = struct.unpack_from(
            "<8I", data
        )
        if magic != 0xFEEDFACF or cpu != 0x0100000C:
            raise ValueError("expected thin little-endian arm64 Mach-O")
        command_end = 32 + command_bytes
        if command_end > len(data):
            raise ValueError("load commands outside file")
        signatures = []
        links = []
        segments = []
        offset = 32
        for _ in range(count):
            if offset + 8 > command_end:
                raise ValueError("truncated load command")
            kind, size = struct.unpack_from("<II", data, offset)
            if size < 8 or size % 8 or offset + size > command_end:
                raise ValueError("invalid load command size")
            if kind == 0x1D:  # LC_CODE_SIGNATURE
                if size != 16:
                    raise ValueError("invalid signature command")
                signatures.append((offset, *struct.unpack_from("<II", data, offset + 8)))
            if kind == 0x19:  # LC_SEGMENT_64
                if size < 72:
                    raise ValueError("truncated segment command")
                name = data[offset + 8:offset + 24].rstrip(b"\0")
                file_offset, file_size = struct.unpack_from("<QQ", data, offset + 40)
                maximum, initial = struct.unpack_from("<ii", data, offset + 56)
                if file_offset + file_size > len(data):
                    raise ValueError("segment outside file")
                segment = (offset, name, file_offset, file_size, maximum, initial)
                segments.append(segment)
                if name == b"__LINKEDIT":
                    links.append(segment)
            offset += size
        if offset != command_end or len(signatures) != 1 or len(links) != 1:
            raise ValueError("expected complete commands and one signature/linkedit")
        signature_command, signature_offset, signature_size = signatures[0]
        link_command, _, link_offset, link_size, maximum, initial = links[0]
        link_vm_size = struct.unpack_from("<Q", data, link_command + 32)[0]
        if (
            not signature_size
            or signature_offset < command_end
            or signature_offset + signature_size != len(data)
            or link_offset > signature_offset
            or link_offset + link_size != len(data)
            or link_vm_size < link_size
            or (maximum | initial) & 4  # VM_PROT_EXECUTE
        ):
            raise ValueError("signature is not a non-executable linkedit tail")
        for _, name, start, size, _, _ in segments:
            if name != b"__LINKEDIT" and size and start + size > signature_offset:
                raise ValueError("signature overlaps another segment")
        unsigned_link_size = signature_offset - link_offset
        patches = [
            (signature_command + 8, b"\0" * 8),
            (link_command + 32, b"\0" * 8),
            (link_command + 48, struct.pack("<Q", unsigned_link_size)),
        ]
        digest = hashlib.sha256()
        start = 0
        for location, replacement in sorted(patches):
            digest.update(memoryview(data)[start:location])
            digest.update(replacement)
            start = location + len(replacement)
        digest.update(memoryview(data)[start:signature_offset])
        result = {
            "rawSHA256": hashlib.sha256(data).hexdigest(),
            "imageSHA256": digest.hexdigest(),
            "signatureOffset": signature_offset,
            "signatureSize": signature_size,
            "normalizedFields": {
                "LC_CODE_SIGNATURE.dataoff_datasize": signature_command + 8,
                "__LINKEDIT.vmsize": link_command + 32,
                "__LINKEDIT.filesize": link_command + 48,
            },
            "scope": "All bytes before trailing code signature, excluding only its locator and signature-dependent linkedit sizes",
        }
    after = path.stat()
    if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns):
        raise ValueError("binary changed during fingerprint")
    return result


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("usage: extensions_macho_fingerprint.py ARM64_MACHO")
    print(json.dumps(fingerprint(sys.argv[1]), sort_keys=True))
