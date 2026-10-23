#!/usr/bin/env python3
"""Decode only whitelisted two-instruction arm64 leaves. Not a general disassembler.
Accept unsigned-immediate load into return register from x0 plus RET, or RET.
This resolves return-value erasure in Ghidra's wrong void prototypes.
"""
import sys
sys.dont_write_bytecode=True
import importlib.util,json,struct,re
from pathlib import Path
s=importlib.util.spec_from_file_location('inv','../arc-layer/scripts/inventory.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
mach=m.MachO(Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64'))
meta=json.loads(Path('specs/evidence/metadata.json').read_text());out=[]
for x in meta['methods']:
 raw=Path(f"specs/bodies/{int(x['imp'],16):08x}.txt").read_text()
 a=raw.find('\n{');b=raw.rfind('}');c=raw[a+2:b].strip()
 if c!='return;':continue
 va=int(x['imp'],16);w,w2=struct.unpack('<II',mach.vread(va,8));decoded=None
 if w==0xd65f03c0:decoded='ret (x0 unchanged)'
 elif w2==0xd65f03c0 and (w&0x3ff)==0:
  form=w&0xffc00000
  forms={0x39400000:('ldrb w0',1),0x79400000:('ldrh w0',2),0xb9400000:('ldr w0',4),0xf9400000:('ldr x0',8),0xbd400000:('ldr s0',4),0xfd400000:('ldr d0',8)}
  if form in forms:
   name,size=forms[form];offset=((w>>10)&0xfff)*size;decoded=f'{name}, [x0, #{offset}]; ret'
 out.append({**{k:x[k] for k in ['class_name','category','kind','selector','type_encoding','imp']},'file_offset':hex(mach.offset(va)),'words':[f'{w:08x}',f'{w2:08x}'],'decoded_whitelist':decoded,'pseudocode':'return; with no value','requires_semantic_warning':x['type_encoding'][0]!='v' and x['selector']!='.cxx_construct'})
 assert decoded is not None,(x,hex(w),hex(w2))
Path('specs/evidence/arm64-leaves.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out,indent=2))
