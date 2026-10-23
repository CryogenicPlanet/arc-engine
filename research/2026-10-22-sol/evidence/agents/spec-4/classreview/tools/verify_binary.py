#!/usr/bin/env python3
import csv,json,struct
from pathlib import Path
from macho import Image
O=Path(__file__).resolve().parents[1];I=Path('/home/exedev/work/inputs');m=Image(I/'binaries/ArcCore-1.170.0.arm64');rows=[]
def add(a,interpret,n=8):rows.append(dict(image='ArcCore-1.170.0.arm64',va=hex(a),file_offset=hex(m.off(a)),bytes=m.read(a,n).hex(),interpretation=interpret))
# Independently decode all twenty float getters plus the repaired bool.
vs=[0x43dee04,0x43dee0c,0xc7fdb4]+list(range(0x43dee14,0x43dee9c,8))
assert len(vs)==20
for j,a in enumerate(vs):
 w,r=struct.unpack('<II',m.read(a,8));assert r==0xd65f03c0 and w&0xffc003ff==0xbd400000
 off=((w>>10)&4095)*4;assert off==8+4*j
 add(a,f'LDR S0,[X0,#{off}]; RET')
a=0x40b1230;w,r=struct.unpack('<II',m.read(a,8));assert w==0x39402000 and r==0xd65f03c0;add(a,'LDRB W0,[X0,#8]; RET')
# Execute only the constrained SIMD moves present in the matrix getter.
regs={};result={};steps=[]
for a in range(0x43dee9c,0x43deec0,4):
 w=m.u32(a)
 if w==0xd65f03c0:steps.append('RET');continue
 if w&0xffe00c00==0x3cc00000:
  imm=(w>>12)&511;imm=imm-512 if imm&256 else imm;rt=w&31;rn=(w>>5)&31;assert rn==0
  regs[rt]=list(range(imm,imm+16));s=f'LDUR Q{rt},[X0,#{imm}]'
 elif w&0xffc00000==0x3d800000:
  imm=((w>>10)&4095)*16;rt=w&31;rn=(w>>5)&31;assert rn==8
  for j,x in enumerate(regs[rt]):result[imm+j]=x
  s=f'STR Q{rt},[X8,#{imm}]'
 elif w&0xffc00000==0xad000000:
  imm=(w>>15)&127;imm=(imm-128 if imm&64 else imm)*16;rt=w&31;rt2=(w>>10)&31;rn=(w>>5)&31;assert rn==8
  for j,x in enumerate(regs[rt]+regs[rt2]):result[imm+j]=x
  s=f'STP Q{rt},Q{rt2},[X8,#{imm}]'
 else:raise AssertionError(hex(w))
 steps.append(s)
assert [result[i] for i in range(80)]==list(range(8,88));add(0x43dee9c,'; '.join(steps),36)
# The no-index entry is a direct B veneer. Decode signed imm26 without assuming file offsets.
a=0x4410bb4;w=m.u32(a);assert w>>26==5;imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=a+4*imm;assert target==0x47390dc;add(a,f'B {hex(target)}',4)
# TextColor format constant is metadata, not a guessed decompiler label.
a,z,_=m.sections['__cfstring'];formats=[]
for c in range(a,a+z,32):
 n=struct.unpack('<Q',m.read(c+24,8))[0]
 if 3<=n<=10:
  p=m.ptr(c+16)
  if p:
   t=m.read(p,n)
   if b'08X' in t:formats.append(dict(record_va=hex(c),string_va=hex(p),format=t.decode()))
(O/'evidence/textcolor-formats.json').write_text(json.dumps(formats,indent=2)+'\n')
# Recover textColor's format address and stack vararg from the instructions omitted by Ghidra.
a=0x4430c0c;q=m.u32(a);assert q&0x9f00001f==0x90000002
imm=(((q>>5)&0x7ffff)<<2)|((q>>29)&3);imm=imm-(1<<21) if imm&(1<<20) else imm
page=(a&~4095)+(imm<<12);q=m.u32(a+4);assert q&0xffc003ff==0x91000042
fmt=page+((q>>10)&4095);assert fmt==0xe244ab8
assert m.u32(0x4430be4)==0xb9406008 # LDR W8,[X0,#0x60], zero extended to X8
assert m.u32(0x4430c08)==0xf90003e8 # STR X8,[SP], variadic argument slot
assert formats==[dict(record_va=hex(fmt),string_va='0xd398912',format='#%08X')]
add(0x4430bdc,'LDR W8,[X0,#0x60]; zero -> nil; format # percent 08X at 0xe244ab8; color vararg STR X8,[SP]',88)
(O/'evidence/data-checks.json').write_text(json.dumps(dict(image='ArcCore-1.170.0.arm64',site_setting_table_va='0xc9d8188',file_offset=hex(m.off(0xc9d8188)),bytes=list(m.read(0xc9d8188,6))),indent=2)+'\n')
# App branches: verify selector stubs from instructions and selrefs.
app=Image.__new__(Image)
# App uses the same on-disk rebase layout; permit its independently checked pointer format.
# Clone constructor with exact expected pointer format 6 instead of the engine's format 2.
import types
src=(O/'tools/macho.py').read_text().replace('assert self.formats=={2},self.formats','assert self.formats=={6},self.formats')
ns={};exec(src,ns);app=ns['Image'](I/'binaries/Arc-1.168.0.arm64')
# App's preferred base is 0x100000000; format 6 encodes offsets relative to it.
def appptr(a):
 raw=struct.unpack('<Q',app.read(a,8))[0];assert not raw>>63;return (raw&((1<<36)-1))+0x100000000
app_rows=[]
for a,kind,sel in [(0x1005426b4,'BL','createPaneHostView'),(0x1005426d4,'B','attachToPaneHostView:'),(0x1005426e8,'B','detachFromPaneHostView:')]:
 w=app.u32(a);assert w>>26==(0x25 if kind=='BL' else 5);imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=a+4*imm
 words=struct.unpack('<8I',app.read(target,32));q=words[0];assert q&0x9f00001f==0x90000001
 imm=(((q>>5)&0x7ffff)<<2)|((q>>29)&3);imm=imm-(1<<21) if imm&(1<<20) else imm
 page=(target&~4095)+(imm<<12);q=words[1];assert q&0xffc003ff==0xf9400021
 ref=page+((q>>10)&4095)*8;actual=app.string(appptr(ref));assert actual==sel
 app_rows.append(dict(image='Arc-1.168.0.arm64',va=hex(a),file_offset=hex(app.off(a)),branch=kind,word=hex(w),target_va=hex(target),stub_bytes=app.read(target,32).hex(),selector_ref_va=hex(ref),selector=actual,receiver='not proven by selector or branch'))
for path,data in [('binary-checks.tsv',rows),('app-branches.tsv',app_rows)]:
 with (O/'evidence'/path).open('w') as f:
  w=csv.DictWriter(f,list(data[0]),delimiter='\t');w.writeheader();w.writerows(data)
print(f'PASS: {len(rows)} engine instruction checks; {len(app_rows)} old-app selector branches; format records {formats}')
