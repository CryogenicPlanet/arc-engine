"""Bounded, segment-aware reads of the copied ArcCore image using installed LLVM.
Writes only stdout; ranges are selected manually, never inferred function spans.
"""
import ctypes as C
import struct
import uuid
from pathlib import Path

IMAGE = Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64')
f = IMAGE.open('rb')
def read(off, n):
    f.seek(off)
    b = f.read(n)
    assert len(b) == n
    return b
hdr = read(0, 32)
assert struct.unpack_from('<I', hdr)[0] == 0xfeedfacf
segments = []
off = 32
image_uuid = None
for _ in range(struct.unpack_from('<I', hdr, 16)[0]):
    cmd, size = struct.unpack('<II', read(off, 8))
    b = read(off, size)
    if cmd == 0x19:
        name, va, vs, fo, fs = struct.unpack_from('<16sQQQQ', b, 8)
        segments.append((va, fs, fo, name.rstrip(b'\0').decode()))
    if cmd == 0x1b:
        image_uuid = str(uuid.UUID(bytes=b[8:24]))
    off += size
def offset(va, n):
    for start, size, fo, name in segments:
        if start <= va and va + n <= start + size:
            return fo + va - start
    raise ValueError(hex(va))
def vread(va, n):
    return read(offset(va, n), n)
def ptr(va):
    x = struct.unpack('<Q', vread(va, 8))[0]
    assert not x >> 63, 'bound pointer requires import resolution'
    return (x & 0xfffffffff) | (((x >> 36) & 255) << 56)
def string(va):
    chunks = []
    for i in range(0, 16384, 256):
        b = vread(va+i, 256)
        chunks.append(b.split(b'\0')[0])
        if b'\0' in b:
            return b''.join(chunks).decode()
    raise ValueError('unterminated string exceeds 16 KiB bound')

if __name__ == '__main__':
    lib = C.CDLL('/lib/x86_64-linux-gnu/libLLVM.so.20.1')
    for name in ['LLVMInitializeAArch64TargetInfo', 'LLVMInitializeAArch64Target',
                 'LLVMInitializeAArch64TargetMC', 'LLVMInitializeAArch64Disassembler']:
        getattr(lib, name)()
    lib.LLVMCreateDisasm.argtypes = [C.c_char_p, C.c_void_p, C.c_int, C.c_void_p, C.c_void_p]
    lib.LLVMCreateDisasm.restype = C.c_void_p
    ctx = lib.LLVMCreateDisasm(b'aarch64-apple-darwin', None, 0, None, None)
    assert ctx
    lib.LLVMDisasmInstruction.argtypes = [C.c_void_p, C.c_void_p, C.c_uint64,
                                        C.c_uint64, C.c_char_p, C.c_size_t]
    lib.LLVMDisasmInstruction.restype = C.c_size_t
    print(f'Image: {IMAGE}; LC_UUID {image_uuid}; arm64; all VAs unslid')
    ranges = [(0x43ecb50,176), (0x43ece08,96), (0x441a1c4,336),
              (0x441c3c0,336), (0x441a848,344), (0x4401f34,932),
              (0x4415b74,224), (0x435c8fc,304), (0x435ca2c,160)]
    for va, n in ranges:
        assert n <= 1024 and n % 4 == 0
        print(f'\nRANGE VA {va:#x} +{n} file offset {offset(va,n):#x}')
        b = vread(va, n)
        for i in range(0,n,4):
            raw = C.create_string_buffer(b[i:i+4])
            out = C.create_string_buffer(256)
            count = lib.LLVMDisasmInstruction(ctx, raw, 4, va+i, out, 256)
            word = struct.unpack('<I', b[i:i+4])[0]
            desc = out.value.decode().strip() if count == 4 else 'unrecognized by configured LLVM (raw word retained)'
            if word & 0x7c000000 == 0x14000000:
                imm = word & 0x3ffffff
                if imm & 0x2000000: imm -= 0x4000000
                desc += f'; target {va+i+imm*4:#x}'
            print(f'{va+i:#x}\t{b[i:i+4].hex()}\t{desc}')
    print('\nSelected metadata, relative method-record decoding:')
    for record in [0xd77a710, 0xd77a6bc, 0xd77a894]:
        sr, tr, ir = struct.unpack('<iii', vread(record,12))
        print(f'record {record:#x}: selector-ref {record+sr:#x}; encoding {string(record+4+tr)}; IMP {record+8+ir:#x}')
    key = 0xdf14bf0
    p = ptr(key)
    print(f'\nThumbnail key pointer {key:#x} -> {p:#x}: {string(p)}')
