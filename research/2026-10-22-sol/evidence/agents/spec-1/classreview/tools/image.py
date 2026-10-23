import struct

class Image:
    def __init__(self,p):
        self.f=p.open('rb'); self.segs=[];self.sections=[]
        h=self.read(0,32);assert struct.unpack_from('<I',h)[0]==0xfeedfacf
        off=32
        for i in range(struct.unpack_from('<I',h,16)[0]):
            cmd,size=struct.unpack('<II',self.read(off,8));b=self.read(off,size)
            if cmd==0x19:
                name,va,vs,fo,fs=struct.unpack_from('<16sQQQQ',b,8)
                self.segs.append((va,fs,fo,name.rstrip(b'\0').decode()))
                for j in range(struct.unpack_from('<I',b,64)[0]):
                    sn,sg,sv,sz,so=struct.unpack_from('<16s16sQQI',b,72+j*80)
                    self.sections.append((sn.rstrip(b'\0').decode(),sv,sz,so))
            if cmd==0x80000034:
                fix=struct.unpack_from('<I',b,8)[0];starts=fix+struct.unpack_from('<I',self.read(fix,28),4)[0]
                n=struct.unpack('<I',self.read(starts,4))[0];formats=set()
                for j in range(n):
                    rel=struct.unpack('<I',self.read(starts+4+j*4,4))[0]
                    if rel:formats.add(struct.unpack('<H',self.read(starts+rel+6,2))[0])
                assert formats=={2}, formats
            off+=size
    def read(self,off,n):
        self.f.seek(off);b=self.f.read(n);assert len(b)==n;return b
    def offset(self,va):
        for start,size,off,name in self.segs:
            if start<=va<start+size:return off+va-start
        raise ValueError(hex(va))
    def v(self,va,n):return self.read(self.offset(va),n)
    def ptr(self,va):
        x=struct.unpack('<Q',self.v(va,8))[0]
        return 0 if x>>63 else (x&0xfffffffff)|(((x>>36)&255)<<56)
    def string(self,va):return self.v(va,8192).split(b'\0',1)[0].decode()
    def pointers(self,sec):
        for name,va,sz,off in self.sections:
            if name==sec:
                for a in range(va,va+sz,8):yield self.ptr(a)
    def methods(self,va):
        if not va:return []
        flags,n=struct.unpack('<II',self.v(va,8));stride=flags&0xffff
        assert flags&0x80000000 and stride==12
        result=[]
        for i in range(n):
            a=va+8+i*stride;sr,tr,ir=struct.unpack('<iii',self.v(a,12))
            sel=a+sr
            if not flags&0x40000000:sel=self.ptr(sel)
            result.append(dict(selector=self.string(sel),type_encoding=self.string(a+4+tr),imp_va=hex(a+8+ir),method_record_va=hex(a)))
        return result
