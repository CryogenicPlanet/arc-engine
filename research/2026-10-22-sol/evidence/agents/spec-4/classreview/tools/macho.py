import struct
class Image:
 def __init__(self,path):
  self.f=path.open('rb'); h=self.at(0,32);assert struct.unpack_from('<I',h)[0]==0xfeedfacf
  self.segs=[];self.sections={};pos=32;self.formats=set()
  for _ in range(struct.unpack_from('<I',h,16)[0]):
   cmd,n=struct.unpack('<II',self.at(pos,8)); d=self.at(pos,n)
   if cmd==0x19:
    name,va,vs,off,fs,_,_,ns,_=struct.unpack_from('<16sQQQQiiII',d,8);self.segs.append((va,fs,off,name.rstrip(b'\0').decode()))
    for j in range(ns):
     sec,sg,a,z,o=struct.unpack_from('<16s16sQQI',d,72+80*j);self.sections[sec.rstrip(b'\0').decode()]=(a,z,o)
   if cmd==0x80000034:
    fix,_=struct.unpack_from('<II',d,8);v,st,*_=struct.unpack('<7I',self.at(fix,28));assert v==0
    nseg=struct.unpack('<I',self.at(fix+st,4))[0]
    for j in range(nseg):
     rel=struct.unpack('<I',self.at(fix+st+4+4*j,4))[0]
     if rel:self.formats.add(struct.unpack('<H',self.at(fix+st+rel+6,2))[0])
   pos+=n
  assert self.formats=={2},self.formats
 def at(self,o,n):
  self.f.seek(o);d=self.f.read(n);assert len(d)==n;return d
 def off(self,a):
  for va,n,o,_ in self.segs:
   if va<=a<va+n:return o+a-va
  raise ValueError(hex(a))
 def read(self,a,n):return self.at(self.off(a),n)
 def u32(self,a):return struct.unpack('<I',self.read(a,4))[0]
 def ptr(self,a):
  raw=struct.unpack('<Q',self.read(a,8))[0]
  return 0 if raw>>63 else (raw&((1<<36)-1))|(((raw>>36)&255)<<56)
 def string(self,a):return self.read(a,4096).split(b'\0')[0].decode()
 def section(self,n):
  a,z,_=self.sections[n];return [self.ptr(a+i) for i in range(0,z,8)]
 def name(self,c):return self.string(self.ptr((self.ptr(c+32)&~7)+24)) if c else ''
 def methods(self,a,cls,cat,kind):
  if not a:return []
  flags,n=struct.unpack('<II',self.read(a,8)); stride=flags&65535; out=[]
  assert n<10000 and stride in (12,24)
  for j in range(n):
   r=a+8+j*stride
   if flags&0x80000000:
    x,y,z=struct.unpack('<iii',self.read(r,12));sel=r+x
    if not flags&0x40000000:sel=self.ptr(sel)
    ty,imp=r+4+y,r+8+z
   else:sel,ty,imp=[self.ptr(r+k) for k in (0,8,16)]
   out.append(dict(class_name=cls,category=cat,kind=kind,selector=self.string(sel),type_encoding=self.string(ty),imp_va=hex(imp),method_record_va=hex(r)))
  return out
