#!/usr/bin/env python3
"""Recheck six cited relative method entries in the two copied engine images.

This bounded check verifies selector/type/IMP bytes at the cited entries. Class
and category attribution remains the separate full metadata parser's evidence.
It does not compare implementation behavior or native C++ ABI.
"""
import argparse
import csv
from pathlib import Path
import struct
import sys


class Image:
    def __init__(self, path):
        self.stream = path.open('rb')
        header = struct.unpack('<8I', self.stream.read(32))
        assert header[0] == 0xfeedfacf
        self.segments = []
        chains = None
        for _ in range(header[4]):
            cmd, size = struct.unpack('<II', self.stream.read(8))
            payload = self.stream.read(size - 8)
            if cmd == 0x19:
                name, va, _, offset, length = struct.unpack_from('<16sQQQQ', payload)
                self.segments.append((name.rstrip(b'\0'), va, offset, length))
            elif cmd == 0x80000034:
                chains = struct.unpack('<II', payload)
        self.formats = {}
        self.base = next(s[1] for s in self.segments if s[0] == b'__TEXT')
        assert chains is not None
        self.stream.seek(chains[0])
        blob = self.stream.read(chains[1])
        starts = struct.unpack_from('<I', blob, 4)[0]
        count = struct.unpack_from('<I', blob, starts)[0]
        assert count <= len(self.segments)
        for i in range(count):
            relative = struct.unpack_from('<I', blob, starts + 4 + 4 * i)[0]
            if relative:
                fmt = struct.unpack_from('<H', blob, starts + relative + 6)[0]
                assert fmt in (2, 6), fmt
                self.formats[i] = fmt

    def location(self, va, size):
        return next((i, s) for i, s in enumerate(self.segments)
                    if s[1] <= va and va + size <= s[1] + s[3])

    def read(self, va, size):
        _, segment = self.location(va, size)
        self.stream.seek(segment[2] + va - segment[1])
        data = self.stream.read(size)
        assert len(data) == size
        return data

    def string(self, va):
        _, segment = self.location(va, 1)
        data = self.read(va, min(512, segment[1] + segment[3] - va))
        assert b'\0' in data
        return data.split(b'\0', 1)[0].decode()

    def pointer(self, va):
        raw = struct.unpack('<Q', self.read(va, 8))[0]
        i, _ = self.location(va, 8)
        fmt = self.formats.get(i)
        if not fmt:
            return raw
        assert not raw >> 63, 'Bound pointers require import resolution'
        target = (raw & ((1 << 36) - 1)) | (((raw >> 36) & 255) << 56)
        return target + (self.base if fmt == 6 else 0)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work', type=Path, required=True)
    args = parser.parse_args()
    audit = args.work / 'agents/protocols/initial/older-encoding-audit.tsv'
    with audit.open(newline='') as stream:
        rows = list(csv.DictReader(stream, delimiter='\t'))
    identities = {(r['class_name'], r['category'], r['kind'], r['selector']) for r in rows}
    expected = {
        ('ArcBrowserContext', 'Extensions', '-', selector) for selector in (
            'chatClosed:withReason:', 'productConnectionClosed:withReason:',
            'getChangedContentSettingsForOrigin:', 'getChangedPermissionsForOrigin:')}
    expected |= {('ArcNotification', '', '-', 'browserContextQualifiedID'),
                 ('ArcSessionID', '', '-', 'identifier')}
    assert len(rows) == 6 and identities == expected
    writer = csv.writer(sys.stdout, delimiter='\t', lineterminator='\n')
    writer.writerow(['image', 'class_name', 'category', 'kind', 'selector',
                     'method_entry', 'selector_va', 'type_va', 'encoding', 'imp', 'result'])
    for version in ('168', '170'):
        image_name = f'ArcCore-1.{version}.0.arm64'
        image = Image(args.work / 'inputs/binaries' / image_name)
        for row in rows:
            entry = int(row['method_entry_' + version], 0)
            method_list = int(row['list_' + version], 0)
            flags, count = struct.unpack('<II', image.read(method_list, 8))
            assert flags & 0x80000000 and flags & 0xffff == 12
            delta = entry - method_list - 8
            assert delta >= 0 and delta % 12 == 0 and delta // 12 < count
            name_delta, type_delta, imp_delta = struct.unpack('<iii', image.read(entry, 12))
            selector_va = entry + name_delta
            if not flags & 0x40000000:
                selector_va = image.pointer(selector_va)
            type_va, imp = entry + 4 + type_delta, entry + 8 + imp_delta
            assert image.string(selector_va) == row['selector']
            assert type_va == int(row['type_va_' + version], 0)
            assert imp == int(row['imp_' + version], 0)
            encoding = image.string(type_va)
            assert encoding == row['encoding_' + version]
            writer.writerow([image_name, row['class_name'], row['category'], row['kind'],
                             row['selector'], hex(entry), hex(selector_va), hex(type_va),
                             encoding, hex(imp), 'PASS'])


if __name__ == '__main__':
    main()
