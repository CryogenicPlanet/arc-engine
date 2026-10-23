#!/usr/bin/env python3
"""Small raw-byte corroboration; no function matching or external disassembler."""
import csv,json,struct
from pathlib import Path
from image import Image
HERE=Path(__file__).resolve().parents[1];SRC=HERE.parent.parent/'spec-2/specs'
m=Image(Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64'))
checks=[]
for p in json.load(open(SRC/'evidence/imp-prefixes.json')):
    va=int(p['imp'],16);words=[f'{w:08x}' for w in struct.unpack('<8I',m.v(va,32))]
    assert words==p['words'];checks.append(dict(va=hex(va),kind='instruction-prefix',result='match',file_offset=hex(m.offset(va)),bytes=m.v(va,32).hex()))
leaves=[]
for r in csv.DictReader(open(SRC/'methods.tsv'),delimiter='\t'):
    w,ret=struct.unpack('<II',m.v(int(r['imp_va'],16),8))
    if ret==0xd65f03c0 and (w&0x3b000000)==0x39000000 and ((w>>22)&3)==1 and ((w>>5)&31)==0 and (w&31)==0:
        leaves.append({k:r[k] for k in ('class_name','selector','imp_va','type_encoding')})
assert len(leaves)==59
(HERE/'evidence/leaf-load-checks.json').write_text(json.dumps(leaves,indent=2)+'\n')
for p in json.load(open(SRC/'evidence/enum-tables.json')):
    va=int(p['va'],16);data=m.v(va,len(p['bytes']));assert list(data)==p['bytes']
    checks.append(dict(va=hex(va),kind='enum-table',result='match',file_offset=hex(m.offset(va)),bytes=data.hex()))
for a,pointers in json.load(open(SRC/'evidence/bridge-vtables.json')).items():
    va=int(a,16);actual=[hex(m.ptr(va+i*8)) for i in range(len(pointers))];assert actual==pointers
    checks.append(dict(va=hex(va),kind='vtable',result='match',file_offset=hex(m.offset(va)),bytes=m.v(va,8*len(pointers)).hex()))
keyva=0xdf14bf0;ptr=m.ptr(keyva);value=m.string(ptr)
checks.append(dict(va=hex(keyva),kind='thumbnail-user-data-key',result=value,file_offset=hex(m.offset(keyva)),bytes=hex(ptr)))
with open(HERE/'evidence/raw-checks.tsv','w') as f:
    w=csv.DictWriter(f,checks[0].keys(),delimiter='\t');w.writeheader();w.writerows(checks)
# Narrow AArch64 load/store decoding for constructor ownership triage.
# Record unrecognized instructions explicitly; this is not a full disassembler.
triage=[]
for va,n in [(0x43ecb50,176),(0x40b1230,8),(0x440db0c,8),(0x443b1b0,12),(0x441d464,12)]:
    for i in range(0,n,4):
        word=struct.unpack('<I',m.v(va+i,4))[0];desc='uninterpreted'
        if (word&0x3b000000)==0x39000000:
            size=1<<(word>>30);opc=(word>>22)&3;rn=(word>>5)&31;rt=word&31;imm=((word>>10)&4095)*size
            desc=f'unsigned-offset memory op: opc={opc}, bytes={size}, base=x{rn}, rt={rt}, offset={imm}; vector={(word>>26)&1}'
        elif word==0xd65f03c0:desc='RET'
        triage.append(dict(va=hex(va+i),word=f'{word:08x}',description=desc))
with open(HERE/'evidence/raw-triage.tsv','w') as f:
    w=csv.DictWriter(f,triage[0].keys(),delimiter='\t');w.writeheader();w.writerows(triage)
print(json.dumps(dict(prefix_checks=sum(c['kind']=='instruction-prefix' for c in checks),enum_tables=4,vtables=3,thumbnail_key=value,raw_instructions=len(triage)),indent=2))
