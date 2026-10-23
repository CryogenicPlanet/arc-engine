#!/usr/bin/env python3
"""Read bounded arm64 disassembly from the pristine local ArcCore image.

No process launch, runtime injection, or image writes. Usage:
  python3 arc/common/renderer_contract_evidence.py 0x43f9e60:0x120
  python3 arc/common/renderer_contract_evidence.py --string 0xd37e3a8
"""
import argparse
import ctypes as C
from pathlib import Path
import struct


class Image:
    def __init__(self, path):
        self.f = path.open("rb")
        h = self.f.read(256)
        self.fat = 0
        if h[:4] == bytes.fromhex("cafebabe"):
            for j in range(struct.unpack_from(">I", h, 4)[0]):
                cpu, _, off, _, _ = struct.unpack_from(">5I", h, 8 + j * 20)
                if cpu == 0x100000C:
                    self.fat = off
                    break
            else:
                raise ValueError("No arm64 slice")
        self.f.seek(self.fat)
        h = self.f.read(32)
        assert struct.unpack_from("<I", h)[0] == 0xFEEDFACF
        self.segs = []
        self.sections = {}
        pos = self.fat + 32
        for _ in range(struct.unpack_from("<I", h, 16)[0]):
            self.f.seek(pos)
            cmd, n = struct.unpack("<II", self.f.read(8))
            self.f.seek(pos)
            d = self.f.read(n)
            if cmd == 0x19:
                _, va, _, off, size = struct.unpack_from("<16sQQQQ", d, 8)
                self.segs.append((va, size, self.fat + off))
                nsects = struct.unpack_from("<I", d, 64)[0]
                for k in range(nsects):
                    sec, _, addr, length, fileoff = struct.unpack_from(
                        "<16s16sQQI", d, 72 + 80 * k)
                    self.sections[sec.rstrip(b"\0").decode()] = (
                        addr, length, self.fat + fileoff)
            pos += n

    def read(self, a, n):
        for va, size, off in self.segs:
            if va <= a and a + n <= va + size:
                self.f.seek(off + a - va)
                return self.f.read(n)
        raise ValueError(hex(a))

    def pointer(self, a):
        raw = struct.unpack("<Q", self.read(a, 8))[0]
        return 0 if raw >> 63 else (raw & ((1 << 36) - 1)) | (((raw >> 36) & 255) << 56)

    def string(self, a):
        return self.read(a, 512).split(b"\0")[0].decode(errors="replace")


def branch_target(address, word):
    """Decode direct PC-relative branch destinations from instruction bits."""
    if word & 0xFC000000 in (0x14000000, 0x94000000):
        displacement, bits = word & 0x3FFFFFF, 26
    elif (word & 0xFF000010 == 0x54000000 or
          word & 0x7E000000 == 0x34000000):
        displacement, bits = (word >> 5) & 0x7FFFF, 19
    elif word & 0x7E000000 == 0x36000000:
        displacement, bits = (word >> 5) & 0x3FFF, 14
    else:
        return None
    if displacement & (1 << (bits - 1)):
        displacement -= 1 << bits
    return address + 4 * displacement


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("ranges", nargs="*")
    p.add_argument("--string", action="append", default=[])
    p.add_argument("--pointer-string", action="append", default=[])
    p.add_argument("--calls-to", action="append", default=[],
                   help="Find direct arm64 B/BL xrefs in __text")
    p.add_argument("--image", type=Path, default=Path(__file__).resolve().parents[3] / "arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore")
    args = p.parse_args()
    im = Image(args.image)
    if args.calls_to:
        targets = {int(v, 0) for v in args.calls_to}
        va, length, off = im.sections["__text"]
        im.f.seek(off)
        data = im.f.read(length)
        for j, (word,) in enumerate(struct.iter_unpack("<I", data)):
            if word & 0xFC000000 in (0x14000000, 0x94000000):
                disp = word & 0x3FFFFFF
                if disp & (1 << 25):
                    disp -= 1 << 26
                addr = va + 4 * j
                target = addr + 4 * disp
                if target in targets:
                    print(f"{addr:#x} {'BL' if word & 0x80000000 else 'B'} -> {target:#x}")
    for s in args.string:
        a = int(s, 0)
        print(f"{a:#x}: {im.string(a)}")
    for s in args.pointer_string:
        a = int(s, 0)
        v = im.pointer(a)
        print(f"{a:#x} -> {v:#x}: {im.string(v)}")
    lib = C.CDLL("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libLTO.dylib")
    lib.lto_initialize_disassembler()
    lib.LLVMCreateDisasm.argtypes = [C.c_char_p, C.c_void_p, C.c_int, C.c_void_p, C.c_void_p]
    lib.LLVMCreateDisasm.restype = C.c_void_p
    lib.LLVMDisasmInstruction.argtypes = [C.c_void_p, C.c_void_p, C.c_uint64, C.c_uint64, C.c_char_p, C.c_size_t]
    lib.LLVMDisasmInstruction.restype = C.c_size_t
    ctx = lib.LLVMCreateDisasm(b"arm64-apple-macos", None, 0, None, None)
    assert ctx
    for r in args.ranges:
        start, size = (int(v, 0) for v in r.split(":"))
        assert 0 < size <= 16384 and start % 4 == 0 and size % 4 == 0
        print(f"RANGE {start:#x}:{size:#x}")
        for a in range(start, start + size, 4):
            raw = im.read(a, 4)
            buf = C.create_string_buffer(300)
            inp = C.create_string_buffer(raw)
            n = lib.LLVMDisasmInstruction(ctx, inp, 4, a, buf, 300)
            s = buf.value.decode() if n else "<undecoded>"
            word = struct.unpack("<I", raw)[0]
            target = branch_target(a, word)
            if target is not None:
                s += f" -> {target:#x}"
            print(f"{a:#x} {raw.hex()} {s}")


if __name__ == "__main__":
    main()
