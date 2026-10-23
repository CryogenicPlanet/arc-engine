"""Bounded independent copied-image instruction/import checks; no writer imports."""
import ctypes as C
import json, struct
from audit import Image, EV, REV, IN, digest

im=Image()
lib=C.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM-20.so')
for n in ('TargetInfo','Target','TargetMC','Disassembler'):
    getattr(lib,'LLVMInitializeAArch64'+n)()
lib.LLVMCreateDisasmCPUFeatures.argtypes=[C.c_char_p,C.c_char_p,C.c_char_p,C.c_void_p,C.c_int,C.c_void_p,C.c_void_p]
lib.LLVMCreateDisasmCPUFeatures.restype=C.c_void_p
dc=lib.LLVMCreateDisasmCPUFeatures(b'aarch64-apple-darwin',b'generic',b'+lse',None,0,None,None)
assert dc
lib.LLVMDisasmInstruction.argtypes=[C.c_void_p,C.c_void_p,C.c_uint64,C.c_uint64,C.c_char_p,C.c_size_t]
lib.LLVMDisasmInstruction.restype=C.c_size_t

def decode(a,n):
    assert n<=2048 and n%4==0
    data=im.read(a,n); out=[]
    for i in range(0,n,4):
        msg=C.create_string_buffer(512); word=C.create_string_buffer(data[i:i+4])
        assert lib.LLVMDisasmInstruction(dc,word,4,a+i,msg,512)==4
        inst=msg.value.decode().strip()
        w=struct.unpack_from('<I',data,i)[0]
        if w>>26 in (5,0x25):
            d=w&0x3ffffff
            if d&(1<<25):d-=1<<26
            inst+=' ; target='+hex(a+i+4*d)
        out.append(dict(unslid_va=hex(a+i),file_offset=hex(im.off(a+i,4)),bytes=data[i:i+4].hex(),instruction=inst))
    return out

def adrp(a):
    w=im.u32(a); d=(((w>>5)&0x7ffff)<<2)|((w>>29)&3)
    if d&(1<<20):d-=1<<21
    return (a&~4095)+(d<<12)

windows={
    0x4443ee0:60,0x4430bdc:88,0x4166068:12,0x425b6d8:12,
    0x8c7df80:12,0xc4a6dc8:12,0x4410bb4:4,
    0x43df53c:60,0x43fe184:60,0x440a2ac:60,
    0x44309d4:20,0x4430bb4:20,0x4430bc8:20,
    0x43dd5a4:52,0x40b1230:8,0x43dee04:8,0x43dee9c:36,
    0x4042028:72,0x4445a90:76,0x473cad8:828,
}
rows=[row for a,n in windows.items() for row in decode(a,n)]
assert struct.unpack('<I',im.read(0x4410bb4,4))[0]>>26==5
w=im.u32(0x4410bb4); delta=w&0x3ffffff
if delta&(1<<25):delta-=1<<26
assert 0x4410bb4+4*delta==0x47390dc
assert im.ptr(0xe244ab8+16)==0xd398912 and im.string(0xd398912)=='#%08X'
assert adrp(0x4430c0c)+((im.u32(0x4430c10)>>10)&4095)==0xe244ab8
selref=adrp(0x4430c00)+(((im.u32(0x4430c04)>>10)&4095)<<3)
assert im.string(im.ptr(selref))=='stringWithFormat:'
assert adrp(0x8c7df80)+((im.u32(0x8c7df84)>>10)&4095)==0xc4a6dc8
assert adrp(0xc4a6dc8)+(((im.u32(0xc4a6dcc)>>10)&4095)<<3)==0xd825018
raw=struct.unpack('<Q',im.read(0xd825018,8))[0]; assert raw>>63
ordinal=raw&0xffffff
version,starts,imports,symbols,count,fmt,symbol_fmt=im.fix_header
assert fmt==1 and symbol_fmt==0 and ordinal<count
entry=struct.unpack('<I',im.at(im.fix+imports+ordinal*4,4))[0]
name=im.at(im.fix+symbols+(entry>>9),256).split(b'\0',1)[0].decode()
assert name=='_objc_storeStrong'
claims=[]
for filename in ('finding-instructions.json','analog-cross-instructions.json','runtime-host-instructions.json'):
    d=json.loads((REV/'revision-evidence'/filename).read_text())
    # All writer raw-instruction records independently reread through segments.
    def walk(x):
        if isinstance(x,dict):
            if all(k in x for k in ('unslid_va','bytes','file_offset')):
                a=int(x['unslid_va'],16); b=bytes.fromhex(x['bytes'])
                assert im.read(a,len(b))==b and im.off(a,len(b))==int(x['file_offset'],16)
                claims.append(dict(file=filename,unslid_va=hex(a),bytes=b.hex()))
            for value in x.values():walk(value)
        elif isinstance(x,list):
            for value in x:walk(value)
    walk(d)
assert claims
image_sha=digest(IN/'binaries/ArcCore-1.170.0.arm64')
assert image_sha=='69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd'
out=dict(image='ArcCore 1.170.0 arm64',chromium_version='156.0.8078.48',sha256=image_sha,address_space='unslid virtual addresses',segments=im.segments,instructions=rows,format=dict(record_va='0xe244ab8',string_va='0xd398912',value='#%08X'),import_check=dict(got_va='0xd825018',raw=hex(raw),ordinal=ordinal,symbol=name),writer_instruction_records_verified=len(claims),result='pass')
(EV/'instructions.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:v for k,v in out.items() if k not in ('segments','instructions')},indent=2))
