"""Bounded seek/read Mach-O base adapted from /home/exedev/work/agents/abi/scripts/inventory.py. Extension parser is audit.py."""
import struct
from pathlib import Path
class MachO:
    def __init__(self, path):
        self.path = path; self.f = path.open('rb'); self.segments=[]; self.sections={}; self.section_info={}; self.commands=[]
        h = struct.unpack('<8I', self.f.read(32)); assert h[0] == 0xfeedfacf
        for _ in range(h[4]):
            cmd, size = struct.unpack('<II', self.f.read(8)); d = self.f.read(size-8)
            self.commands.append((cmd,d))
            if cmd == 0x19:
                sn, va, vs, fo, fs, _, _, ns, _ = struct.unpack_from('<16sQQQQiiII', d)
                sn=sn.rstrip(b'\0').decode()
                self.segments.append(dict(name=sn, va=va, vmsize=vs, fileoff=fo, filesize=fs))
                for i in range(ns):
                    a=struct.unpack_from('<16s16sQQIIIIIIII',d,64+i*80)
                    self.sections[(sn,a[0].rstrip(b'\0').decode())] = (a[2], a[3], a[4])
                    self.section_info[(sn,a[0].rstrip(b'\0').decode())]=dict(flags=a[8],reserved1=a[9],reserved2=a[10])
        self.pointer_formats={}
        self.image_base=next(s['va'] for s in self.segments if s['name']=='__TEXT')
        for cmd,d in self.commands:
            if cmd!=0x80000034:continue
            off,sz=struct.unpack('<II',d);self.f.seek(off);blob=self.f.read(sz)
            so=struct.unpack_from('<I',blob,4)[0];n=struct.unpack_from('<I',blob,so)[0]
            for i in range(n):
                rel=struct.unpack_from('<I',blob,so+4+i*4)[0]
                if not rel:continue
                fmt=struct.unpack_from('<H',blob,so+rel+6)[0]
                assert fmt in [2,6], f'unsupported chained pointer format {fmt}'
                self.pointer_formats[self.segments[i]['name']]=fmt
    def offset(self, va, size=1):
        for s in self.segments:
            if s['va'] <= va and va+size <= s['va']+s['filesize']:
                return s['fileoff']+va-s['va']
        raise ValueError(f'unbacked VA {va:#x} size {size}')
    def read(self, va, n):
        self.f.seek(self.offset(va,n)); b=self.f.read(n); assert len(b)==n; return b
    def u64(self, va): return struct.unpack('<Q', self.read(va,8))[0]
    def pointer(self, va):
        raw=self.u64(va)
        if raw==0:return 0
        # DYLD_CHAINED_PTR_64 (2) / DYLD_CHAINED_PTR_64_OFFSET (6).
        # The chained 'next' bits are not part of a pointer. Refuse binds.
        segment=next(s for s in self.segments if s['va']<=va<s['va']+s['filesize'])
        fmt=self.pointer_formats.get(segment['name'])
        if fmt:
            assert raw>>63==0, f'bound pointer at {va:#x}'
            target=(raw & ((1<<36)-1)) | (((raw>>36)&255)<<56)
            return target+(self.image_base if fmt==6 else 0)
        return raw
    def string(self, va):
        if not va: return ''
        end=next((s['va']+s['filesize'] for s in self.segments if s['va']<=va<s['va']+s['filesize']),None)
        if end is None:raise ValueError(f'unbacked string {va:#x}')
        chunks=[]
        for pos in range(va,min(end,va+1024*1024),4096):
            b=self.read(pos,min(4096,end-pos))
            chunks.append(b.split(b'\0',1)[0])
            if b'\0' in b:return b''.join(chunks).decode('utf8',errors='replace')
        raise ValueError(f'unterminated {va:#x}')
    def symbols(self):
        for cmd,d in self.commands:
            if cmd!=2: continue
            so,n,st,sz=struct.unpack('<4I',d)
            self.f.seek(st); strings=self.f.read(sz)
            self.f.seek(so); raw=self.f.read(n*16)
            for idx in range(n):
                sx,typ,sect,desc,val=struct.unpack_from('<IBBHQ',raw,idx*16)
                yield strings[sx:strings.find(b'\0',sx)].decode('utf8',errors='replace'),typ,sect,desc,val
    def import_stubs(self):
        symbols=list(self.symbols());indirect=[]
        for cmd,d in self.commands:
            if cmd!=0xb:continue
            fields=struct.unpack('<18I',d);off,n=fields[12:14]
            self.f.seek(off);indirect=list(struct.unpack('<'+str(n)+'I',self.f.read(n*4)))
        result={}
        for key,(va,sz,fo) in self.sections.items():
            info=self.section_info[key]
            if info['flags']&255!=8:continue
            step=info['reserved2'];assert step>0
            for i in range(sz//step):
                idx=indirect[info['reserved1']+i]
                if idx&0xc0000000:continue
                result[va+i*step]=symbols[idx][0]
        return result
