#!/usr/bin/env python3
"""Verify selected APP 1.168 selector stubs and BL calls; receivers stay unproven."""
import bisect, csv, hashlib, json, re, struct, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'initial/tools'))
from inventory import MachO
INP=Path('/home/exedev/work/inputs'); OUT=ROOT/'decompile/evidence'
selectors={
 'createBrowserContext:isHidden:type:completion:error:',
 'createBrowser:withDelegate:forWindow:', 'createAutocompleteControllerWithProviders:',
 'autocomplete:cursorPosition:preventInlineAutocomplete:completion:',
 'autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:',
 'installExtensionFromWebstore:unattended:completion:',
 'setPinnedExtensionOrder:', 'attachToPaneHostView:', 'createPaneHostView',
 'save','neverAskAgain','additionalData',
}
stubs={}; address=0; active=False
path=INP/'app168/ghidra/decomp/0x1035.c'
for no,line in enumerate(path.open(),1):
 m=re.match(r'// ==== (0x[0-9a-f]+)\s+(.*?)\s+\((\d+) bytes\)',line)
 if m: address=int(m[1],16);active=m[2].startswith('objc_stub::')
 if active:
  for sel in re.findall(r'"([^"\n]+)"',line):
   if sel in selectors: stubs[address]=(sel,str(path),no)
app=MachO(INP/'binaries/Arc-1.168.0.arm64')
for va,(sel,_,_) in stubs.items():
 adrp,ldr,_,_,branch=struct.unpack_from('<5I',app.mem,app.offset(va))
 assert adrp&0x9f00001f==0x90000001 and ldr&0xffc003ff==0xf9400021 and branch==0xd61f0200
 imm=((adrp>>29)&3)|(((adrp>>5)&0x7ffff)<<2)
 if imm&(1<<20):imm-=1<<21
 ref=(va&~0xfff)+(imm<<12)+(((ldr>>10)&0xfff)*8)
 assert app.string(app.ptr(ref))==sel
base,length=app.sections['__text']; off=app.offset(base);calls=[]
for i in range(0,length-3,4):
 w=struct.unpack_from('<I',app.mem,off+i)[0]
 if w&0xfc000000!=0x94000000:continue
 imm=w&0x3ffffff
 if imm&0x2000000:imm-=0x4000000
 target=base+i+imm*4
 if target in stubs:
  sel,source,no=stubs[target]
  calls.append(dict(selector=sel,app_call_va=hex(base+i),app_file_offset=hex(off+i),stub_va=hex(target),
                    stub_source=source,stub_line=no,receiver_status='unproven',app_body_source='',app_body_offset='',app_excerpt=''))
# Obtain containing saved APP body by address bucket/header, without treating reported sizes as contiguous.
for bucket in sorted({int(r['app_call_va'],16)>>20 for r in calls}):
 path=INP/f'app168/ghidra/decomp/0x{bucket:x}.c'
 if not path.exists():continue
 headers=[];pos=0
 with path.open('rb') as f:
  for line in f:
   m=re.match(rb'// ==== (0x[0-9a-f]+)\s+(.*?)\s+\((\d+) bytes\)',line)
   if m:headers.append((int(m[1],16),pos,m[2].decode()))
   pos+=len(line)
 addresses=[r[0] for r in headers]
 for r in calls:
  va=int(r['app_call_va'],16)
  if va>>20!=bucket:continue
  j=bisect.bisect_right(addresses,va)-1
  if j<0:continue
  start,offset,name=headers[j];end=headers[j+1][1] if j+1<len(headers) else pos
  r.update(app_body_source=str(path),app_body_offset=offset,app_excerpt=f'evidence/app-{start:x}.c')
  p=ROOT/'decompile'/r['app_excerpt']
  if not p.exists():
   with path.open('rb') as f:f.seek(offset);b=f.read(min(end-offset,65536))
   p.write_bytes(b)
with (OUT/'app-calls.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(calls[0]),delimiter='\t');w.writeheader();w.writerows(calls)
summary={sel:dict(stubs=sum(s[0]==sel for s in stubs.values()),binary_BL_calls=sum(r['selector']==sel for r in calls)) for sel in sorted(selectors)}
(OUT/'app-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
