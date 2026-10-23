#!/usr/bin/env python3
import sys,json,re,sqlite3,struct,csv,collections
from pathlib import Path
sys.dont_write_bytecode=True
from macho_metadata import MachO
R=Path('/home/exedev/work/inputs');out=Path('specs/evidence');mach=MachO(R/'binaries/ArcCore-1.170.0.arm64')
con=sqlite3.connect('file:'+str(R/'corpus-index.sqlite')+'?mode=ro',uri=True)
# Manually selected bridge edges, not a claimed exhaustive closure.
vas=[0x4357078,0x4357738,0x4357aa0,0x4357b40,0x4357be0,0x441e0a0,0x443a40c,0x443a21c,0x42c35e4,0x43eceb0,0x432c950,0x432c834,0x436df2c,0x436d904,0x436de50,0x435b114,0x4355df8,0x4360b58,0x43310bc,0x429fa38,0x42a6200,0x42a75b4,0x43abd34,0x43abdf8,0x1c391bc,0x1d9fb90,0x1b356b8,0x4402548,0x44025f4,0x4356fdc,0x4357fd0,0x43584f8,0x436051c,0x4332f08,0x43458dc,0x436d69c,0x435dd94,0x42a64c0]
vtables={}
for va in [0xda88250,0xda88368,0xda88460]:
 ps=[mach.ptr(va+8*i) for i in range(6)];vtables[hex(va)]=[hex(p) for p in ps]
 vas.extend(p for p in ps if con.execute('select 1 from functions where va=?',(p,)).fetchone())
json.dump(vtables,open(out/'bridge-vtables.json','w'),indent=2)
rows=[]
for va in sorted(set(vas)):
 row=con.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 if not row:rows.append(dict(va=hex(va),status='missing'));continue
 name,path,off,n,fail=row
 with open(path,'rb') as f:f.seek(off);body=f.read(min(n,65536)).decode('utf-8','replace')
 rows.append(dict(va=hex(va),name=name,path=path,offset=off,length=n,failed=fail,truncated=n>65536,body=body))
json.dump(rows,open(out/'bridges.json','w'),indent=2)
# A deliberately tiny decoder: only two-instruction leaf unsigned-immediate scalar loads + RET.
leaf=[]
for m in json.load(open(out/'methods-bodies.json')):
 va=int(m['imp'],16);words=struct.unpack('<8I',mach.vread(va,32));w=words[0]
 d=dict(imp=m['imp'],class_name=m['class_name'],selector=m['selector'],words=[f'{x:08x}' for x in words])
 if (w&0x3b000000)==0x39000000 and words[1]==0xd65f03c0 and ((w>>22)&3)==1:
  size=w>>30;fp=(w>>26)&1;rt=w&31;rn=(w>>5)&31;off=((w>>10)&4095)*(1<<size)
  d['leaf_load']=dict(register=rt,base_register=rn,offset=off,width_bits=8<<size,fp=bool(fp))
 if w&0xfc000000==0x14000000:
  imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;d['direct_branch_target']=hex(va+4*imm)
 leaf.append(d)
json.dump(leaf,open(out/'imp-prefixes.json','w'),indent=2)
json.dump([dict(va=hex(va),file_offset=hex(mach.offset(va)),bytes=list(mach.vread(va,n))) for va,n in [(0xc9d8158,15),(0xc9d8190,3),(0xc9d8198,9),(0xc9d81a8,7)]],open(out/'enum-tables.json','w'),indent=2)
print('bounded bridge bodies',len(rows),'exact load+RET method identities',sum('leaf_load'in x for x in leaf),'branch thunks',sum('direct_branch_target'in x for x in leaf))
for r in rows:
 b=r.get('body','')
 print(r['va'],r.get('name',''),r.get('length'), ' | '.join(re.findall(r'"[^"\n]+"',b))[:550])
