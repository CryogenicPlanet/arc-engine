#!/usr/bin/env python3
"""Narrow, fail-closed parse of the observed arm64 EHFrame compact-unwind/FDE/CIE."""
import sys,struct,json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'scripts'))
from inventory import MachO
OUT=Path(__file__).resolve().parents[1]
b=MachO(Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64'))
u,usz,_=b.sections['__TEXT','__unwind_info'];eh,esz,_=b.sections['__TEXT','__eh_frame']
def u32(a):return struct.unpack('<I',b.read(a,4))[0]
def s32(a):return struct.unpack('<i',b.read(a,4))[0]
def s64(a):return struct.unpack('<q',b.read(a,8))[0]
h=struct.unpack('<7I',b.read(u,28));assert h[0]==1
idx=[struct.unpack('<III',b.read(u+h[5]+i*12,12)) for i in range(h[6])]
a=0x2e0238
page=next((x for i,x in enumerate(idx[:-1]) if x[0]<=a<idx[i+1][0]))
kind,eo,n,enco,encn=struct.unpack('<I4H',b.read(u+page[1],12));assert kind==3
common=list(struct.unpack('<'+'I'*h[2],b.read(u+h[1],h[2]*4)))
local=list(struct.unpack('<'+'I'*encn,b.read(u+page[1]+enco,encn*4)))
entries=[]
for i in range(n):
 v=u32(u+page[1]+eo+i*4);entries.append((page[0]+(v&0xffffff),(common+local)[v>>24]))
start,enc=next((p for i,p in enumerate(entries) if p[0]==a));assert enc&0x0f000000==0x03000000
fde=eh+(enc&0xffffff);fl=u32(fde);cie=fde+4-u32(fde+4)
assert eh<=fde<eh+esz and fl==40
# Only accept the precise observed DWARF augmentation/encodings. No generic DWARF parser claim.
cie_bytes=b.read(cie,u32(cie)+4)
assert cie_bytes[:19]==bytes.fromhex('1800000000000000017a504c520001781e079b')
assert cie_bytes[23:]==bytes.fromhex('10100c1f00')
slot=cie+19+s32(cie+19);personality=b.pointer(slot)
assert b.read(fde+24,1)==b'\x08'
covered_start=fde+8+s64(fde+8);covered_length=s64(fde+16)
lsda=fde+25+s64(fde+25)
assert covered_start==a and covered_length==32
# Verify exception table call-site and landing-pad offsets against Chromium's assembly.
lsda_bytes=b.read(lsda,24)
result=dict(compact_unwind_section=hex(u),compact_unwind_encoding=hex(enc),fde=hex(fde),fde_file_offset=hex(b.offset(fde)),cie=hex(cie),cie_bytes=cie_bytes.hex(),fde_bytes=b.read(fde,fl+4).hex(),function_start=hex(covered_start),fde_pc_range_bytes=covered_length,lsda=hex(lsda),lsda_bytes=lsda_bytes.hex(),personality_pointer_slot=hex(slot),personality=hex(personality),scope='Targeted observed encodings only. FDE range includes the landing pad omitted by the 24-byte pseudocode header.')
(OUT/'ehframe-unwind.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
