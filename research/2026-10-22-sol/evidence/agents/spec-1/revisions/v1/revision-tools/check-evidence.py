#!/usr/bin/env python3
"""Recheck the four finding bodies and their binary anchors without input mutation."""
import csv
import hashlib
import json
from pathlib import Path
import sqlite3
import struct
import sys

sys.dont_write_bytecode = True
from instructions import MachO

ROOT = Path(__file__).resolve().parents[1]
I = Path('/home/exedev/work/inputs')
m = MachO('ArcCore-1.170.0.arm64')
def word(a): return struct.unpack('<I', m.read(a, 4))[0]
def adrp(a):
    w = word(a)
    assert w & 0x9f000000 == 0x90000000
    i = (((w >> 5) & 0x7ffff) << 2) | ((w >> 29) & 3)
    if i & (1 << 20): i -= 1 << 21
    return (a & ~4095) + (i << 12)
def branch(a):
    w = word(a)
    assert w >> 26 in (5, 0x25)
    i = w & 0x3ffffff
    if i & (1 << 25): i -= 1 << 26
    return a + 4*i

assert word(0x4443ee0) == 0x3900011f  # Clear returned discriminator/storage byte.
assert word(0x4443ee4) == 0x3900411f
assert word(0x4443eec) == 0x34000149  # Empty source -> RET, not BRK.
assert word(0x4443ef0) == 0xb4000148  # Engaged + null X8 -> BRK.
assert word(0x4443f08) == 0xf82a0129  # LDADD +1 at control+8.
assert word(0x4443f14) == 0xd65f03c0
assert word(0x4443f18) == 0xd4200020
assert word(0x4430be4) == 0xb9406008  # Unsigned color -> W8 (zero extended).
assert word(0x4430c08) == 0xf90003e8  # Color vararg -> [SP].
fmt = adrp(0x4430c0c) + ((word(0x4430c10) >> 10) & 4095)
assert fmt == 0xe244ab8
string = m.ptr(fmt+16)
length = struct.unpack('<Q', m.read(fmt+24, 8))[0]
assert string == 0xd398912 and m.read(string, length) == b'#%08X'
assert word(0x4430c28) == 0xd2800000  # Zero color -> nil.
assert word(0x4430c30) == 0xd4200020

# Resolve the two destructors' common veneer through its actual chained import.
assert word(0x4166068) == 0x91002000
assert word(0x425b6d8) == 0x91004000
for a in (0x416606c, 0x425b6dc): assert word(a) == 0xd2800001
assert branch(0x4166070) == branch(0x425b6e0) == 0x8c7df80
stub = adrp(0x8c7df80) + ((word(0x8c7df84) >> 10) & 4095)
assert stub == 0xc4a6dc8
got = adrp(stub) + ((word(stub+4) >> 10) & 4095)*8
raw = struct.unpack('<Q', m.read(got, 8))[0]
assert raw >> 63 == 1
fixoff, fixsize = struct.unpack_from('<II', m.commands[0x80000034], 8)
ver, starts, imports, syms, count, impfmt, symfmt = struct.unpack('<7I', m.at(fixoff, 28))
assert ver == symfmt == 0 and impfmt in (1, 2)
ordinal = raw & 0xffffff
assert ordinal < count
entry = struct.unpack('<I', m.at(fixoff+imports+ordinal*(4 if impfmt == 1 else 8), 4))[0]
symbol = m.at(fixoff+syms+(entry >> 9), 256).split(b'\0')[0].decode()
assert symbol == '_objc_storeStrong'

db = sqlite3.connect('file:'+str(I/'corpus-index.sqlite')+'?mode=ro', uri=True)
bodies = []
for va in (0x4443ee0, 0x4430bdc, 0x4166068, 0x425b6d8):
    name, path, off, size, failed = db.execute('SELECT name,path,offset,length,failed FROM functions WHERE va=?', (va,)).fetchone()
    assert not failed and size <= 10000
    with open(path, 'rb') as f:
        f.seek(off); data = f.read(size)
    assert data == (ROOT/'bodies'/f'{hex(va)}.c').read_bytes()
    bodies.append(dict(unslid_va=hex(va), name=name, original_path=path, byte_offset=off,
                       c_text_bytes=size, sha256=hashlib.sha256(data).hexdigest()))

sources = []
for path, pin in [
    (I/'chromium-156/content/browser/renderer_host/render_widget_host_view_mac.mm', 'd0c02bfad08ec92a6ffcf989e53cd995e6381a4c'),
    (ROOT/'sources/cef/include/internal/cef_types_mac.h', '426920c33ef22551e0b8f48cf343d623ac25642d')]:
    sources.append(dict(path=str(path), pin=pin, sha256=hashlib.sha256(path.read_bytes()).hexdigest()))

result = dict(image=m.name, address_space='unslid virtual addresses', body_checks=bodies,
              format=dict(record_va=hex(fmt), string_va=hex(string), value='#%08X'),
              destructor_call=dict(veneer_va='0x8c7df80', stub_va=hex(stub), got_va=hex(got),
                                   got_raw=hex(raw), import_ordinal=ordinal, import_format=impfmt, symbol=symbol),
              sources=sources, result='pass')
(ROOT/'revision-evidence/finding-checks.json').write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps(result, indent=2))
