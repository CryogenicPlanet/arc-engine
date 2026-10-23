#!/usr/bin/env python3
"""Bounded, segment-aware reads of the copied ArcCore; uses installed LLVM only."""
import csv, ctypes as C, hashlib, json, struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
IMAGE=Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64')
class Image:
 def __init__(self,p):
  self.f=p.open('rb');self.segments=[]
  assert self.read(0,4)==b'\xcf\xfa\xed\xfe'
  pos=32
  for _ in range(struct.unpack('<I',self.read(16,4))[0]):
   cmd,n=struct.unpack('<II',self.read(pos,8));b=self.read(pos,n)
   if cmd==0x19:
    va,vs,off,fs=struct.unpack_from('<QQQQ',b,24)
    self.segments.append((va,fs,off))
   pos+=n
 def read(self,o,n):
  assert 0<n<=32768
  self.f.seek(o);b=self.f.read(n);assert len(b)==n;return b
 def offset(self,a,n=1):
  for va,fs,off in self.segments:
   if va<=a and a+n<=va+fs:return off+a-va
  raise ValueError(hex(a))
 def v(self,a,n):return self.read(self.offset(a,n),n)
lib=C.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM.so.20.1')
for n in ['TargetInfo','Target','TargetMC','Disassembler']:getattr(lib,'LLVMInitializeAArch64'+n)()
lib.LLVMCreateDisasmCPUFeatures.argtypes=[C.c_char_p,C.c_char_p,C.c_char_p,C.c_void_p,C.c_int,C.c_void_p,C.c_void_p]
lib.LLVMCreateDisasmCPUFeatures.restype=C.c_void_p
lib.LLVMDisasmInstruction.argtypes=[C.c_void_p,C.c_void_p,C.c_uint64,C.c_uint64,C.c_char_p,C.c_size_t]
lib.LLVMDisasmInstruction.restype=C.c_size_t
ctx=lib.LLVMCreateDisasmCPUFeatures(b'arm64-apple-macos',b'generic',b'+lse,+rcpc',None,0,None,None);assert ctx
im=Image(IMAGE)
ranges=[(0x43ed9a8,1116),(0x40439c4,628),(0x443ff88,832),(0x432bf9c,404),(0x43eca0c,248),(0x4041fec,432),(0xc87c40,208)]
out=[]
for start,size in ranges:
 data=im.v(start,size)
 for i in range(0,size,4):
  a=start+i;raw=data[i:i+4];b=C.create_string_buffer(512);p=C.create_string_buffer(raw)
  n=lib.LLVMDisasmInstruction(ctx,p,4,a,b,len(b));s=b.value.decode().strip() if n else '<undecoded>'
  w=int.from_bytes(raw,'little');target=''
  if w>>26 in (0b000101,0b100101):
   k=w&0x3ffffff;k=k-(1<<26) if k&(1<<25) else k;target=hex(a+4*k)
  elif (w&0x7e000000)==0x34000000 or (w&0xff000010)==0x54000000:
   k=(w>>5)&0x7ffff;k=k-(1<<19) if k&(1<<18) else k;target=hex(a+4*k)
  out.append(dict(image='ArcCore 1.170.0 arm64',address_space='unslid VA',function_va=hex(start),va=hex(a),file_offset=hex(im.offset(a,4)),bytes=raw.hex(),instruction=s,branch_target=target))
with (ROOT/'revision-evidence/instructions.tsv').open('w') as f:
 w=csv.DictWriter(f,list(out[0]),delimiter='\t');w.writeheader();w.writerows(out)
# Recheck relevant cross-report bytes and segment translation, not just prose.
prior=Path('/home/exedev/work/agents/abi/decompile/binary-instructions.tsv')
checks=[]
for r in csv.DictReader(prior.open(),delimiter='\t'):
 if r['range_va'] not in ('0x43ed9a8','0x4041fec'):continue
 a=int(r['instruction_va'],16) if 'instruction_va' in r else int(r['va'],16)
 raw=r.get('bytes',r.get('raw_bytes'))
 assert im.v(a,4).hex()==raw
 assert im.offset(a,4)==int(r['file_offset'],16)
 checks.append(a)
(ROOT/'revision-evidence/binary-check.json').write_text(json.dumps(dict(image=str(IMAGE),identity='ArcCore 1.170.0 arm64; unslid VAs',ranges=[dict(va=hex(a),bytes=n,sha256=hashlib.sha256(im.v(a,n)).hexdigest()) for a,n in ranges],instructions=len(out),cross_report_byte_checks=len(checks),cross_report=str(prior),disassembler='installed LLVM 20 public C API; generic arm64 +lse,+rcpc'),indent=2)+'\n')
