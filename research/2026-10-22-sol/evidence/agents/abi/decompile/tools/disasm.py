#!/usr/bin/env python3
"""Bounded arm64 Mach-O inspection through libLLVM's public C disassembler API."""
import ctypes as C, sys, struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'scripts'))
from inventory import MachO
lib=C.CDLL('/lib/x86_64-linux-gnu/libLLVM.so.20.1')
for n in ['TargetInfo','Target','TargetMC','Disassembler']:
 getattr(lib,'LLVMInitializeAArch64'+n)()
lib.LLVMCreateDisasm.argtypes=[C.c_char_p,C.c_void_p,C.c_int,C.c_void_p,C.c_void_p]
lib.LLVMCreateDisasm.restype=C.c_void_p
lib.LLVMDisasmInstruction.argtypes=[C.c_void_p,C.c_void_p,C.c_uint64,C.c_uint64,C.c_char_p,C.c_size_t]
lib.LLVMDisasmInstruction.restype=C.c_size_t
ctx=lib.LLVMCreateDisasm(b'arm64-apple-macos',None,0,None,None)
assert ctx

def disasm(image,start,size):
 assert 0<size<=32768
 data=image.read(start,size)
 for i in range(0,len(data)-3,4):
  raw=data[i:i+4]; buf=C.create_string_buffer(512); inp=C.create_string_buffer(raw)
  n=lib.LLVMDisasmInstruction(ctx,inp,4,start+i,buf,len(buf))
  yield start+i,raw.hex(),buf.value.decode() if n else '<undecoded>'

if __name__=='__main__':
 im=MachO(Path(sys.argv[1]))
 for spec in sys.argv[2:]:
  addr,size=(int(x,0) for x in spec.split(':'))
  for a,r,s in disasm(im,addr,size):print(f'{a:#x}\t{r}\t{s}')
