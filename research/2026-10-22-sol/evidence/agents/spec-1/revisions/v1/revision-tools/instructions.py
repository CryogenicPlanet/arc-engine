#!/usr/bin/env python3
"""Read bounded instruction windows from copied Mach-O images; no input writes."""
import ctypes as C
import json
import os
from pathlib import Path
import struct
import sys

os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
ROOT = Path(__file__).resolve().parents[1]
INPUT = Path('/home/exedev/work/inputs/binaries')
IMAGE = 'ArcCore-1.170.0.arm64'

class MachO:
    def __init__(self, name):
        self.name = name
        self.f = (INPUT / name).open('rb')
        h = self.at(0, 32)
        assert struct.unpack_from('<I', h)[0] == 0xfeedfacf
        self.segments = []
        self.sections = {}
        self.commands = {}
        p = 32
        for _ in range(struct.unpack_from('<I', h, 16)[0]):
            cmd, size = struct.unpack('<II', self.at(p, 8))
            d = self.at(p, size)
            self.commands[cmd] = d
            if cmd == 0x19:
                n, va, vs, off, fs, _, _, ns, _ = struct.unpack_from('<16sQQQQiiII', d, 8)
                self.segments.append((n.rstrip(b'\0').decode(), va, vs, off, fs))
                for j in range(ns):
                    n, sg, a, z, off = struct.unpack_from('<16s16sQQI', d, 72+80*j)
                    self.sections[n.rstrip(b'\0').decode()] = (a, z, off)
            p += size
    def at(self, off, size):
        self.f.seek(off)
        b = self.f.read(size)
        assert len(b) == size
        return b
    def off(self, va, size=1):
        for _, start, _, off, fs in self.segments:
            if start <= va and va+size <= start+fs:
                return off+va-start
        raise ValueError(hex(va))
    def read(self, va, size):
        return self.at(self.off(va, size), size)
    def ptr(self, va):
        raw = struct.unpack('<Q', self.read(va, 8))[0]
        assert not raw >> 63, 'bound import, not rebase'
        return (raw & ((1 << 36)-1)) | (((raw >> 36) & 255) << 56)
    def cstr(self, va, limit=256):
        return self.read(va, limit).split(b'\0', 1)[0].decode()

def disassembler():
    lib = C.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM-20.so')
    for n in ['TargetInfo', 'Target', 'TargetMC', 'Disassembler']:
        getattr(lib, 'LLVMInitializeAArch64'+n)()
    lib.LLVMCreateDisasmCPUFeatures.argtypes = [C.c_char_p, C.c_char_p, C.c_char_p, C.c_void_p, C.c_int, C.c_void_p, C.c_void_p]
    lib.LLVMCreateDisasmCPUFeatures.restype = C.c_void_p
    dc = lib.LLVMCreateDisasmCPUFeatures(b'aarch64-apple-darwin', b'generic', b'+lse', None, 0, None, None)
    assert dc
    lib.LLVMDisasmInstruction.argtypes = [C.c_void_p, C.c_void_p, C.c_uint64, C.c_uint64, C.c_char_p, C.c_size_t]
    lib.LLVMDisasmInstruction.restype = C.c_size_t
    return lib, dc

def decode(img, va, size):
    assert 0 < size <= 4096 and size % 4 == 0
    lib, dc = disassembler()
    data = img.read(va, size)
    rows = []
    for j in range(0, size, 4):
        out = C.create_string_buffer(512)
        b = C.create_string_buffer(data[j:j+4])
        n = lib.LLVMDisasmInstruction(dc, b, 4, va+j, out, len(out))
        assert n == 4, (hex(va+j), data[j:j+4].hex())
        word = struct.unpack_from('<I', data, j)[0]
        s = out.value.decode().strip()
        if word >> 26 in (5, 0x25):
            imm = word & 0x3ffffff
            if imm & (1 << 25): imm -= 1 << 26
            s += ' ; unslid target '+hex(va+j+4*imm)
        rows.append({'image': img.name, 'unslid_va': hex(va+j), 'file_offset': hex(img.off(va+j)),
                     'bytes': data[j:j+4].hex(), 'instruction': s})
    return rows

if __name__ == '__main__':
    img = MachO(IMAGE)
    rows = []
    for arg in sys.argv[1:]:
        a, n = arg.split(':')
        rows.extend(decode(img, int(a, 0), int(n, 0)))
    print(json.dumps({'image': IMAGE, 'address_space': 'unslid virtual addresses',
                      'segments': img.segments, 'instructions': rows}, indent=2))
