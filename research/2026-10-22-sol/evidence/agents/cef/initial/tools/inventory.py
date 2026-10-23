#!/usr/bin/env python3
"""Read Mach-O ObjC metadata using segment mappings; cross-check saved dumps.

No row-order pairing. Keys are class, category, method kind and selector.
Ghidra records are matched by exact IMP start only; end is not body coverage.
"""
import collections, csv, hashlib, json, mmap, pathlib, re, struct

ROOT = pathlib.Path(__file__).resolve().parents[1]
INPUT = pathlib.Path('/home/exedev/work/inputs')
DATA = INPUT/'context/analysis/data'

class MachO:
    def __init__(self, path):
        self.file = path.open('rb')
        self.mem = mmap.mmap(self.file.fileno(), 0, access=mmap.ACCESS_READ)
        assert self.u32(0) == 0xfeedfacf
        self.segments, self.sections = [], {}
        self.pointer_format=None
        off = 32
        for _ in range(self.u32(16)):
            cmd, size = struct.unpack_from('<II', self.mem, off)
            if cmd == 0x19:
                name, va, vs, fo, fs, _, _, ns, _ = struct.unpack_from('<16sQQQQiiII', self.mem, off+8)
                self.segments.append((va, vs, fo, fs, name.rstrip(b'\0').decode()))
                for n in range(ns):
                    p = off+72+80*n
                    sec, seg, addr, length = struct.unpack_from('<16s16sQQ', self.mem, p)
                    self.sections[sec.rstrip(b'\0').decode()] = (addr, length)
            if cmd == 0x80000034:  # LC_DYLD_CHAINED_FIXUPS
                dataoff,datasize=struct.unpack_from('<II',self.mem,off+8)
                starts=dataoff+self.u32(dataoff+4)
                formats=set()
                for index in range(self.u32(starts)):
                    rel=self.u32(starts+4+index*4)
                    if rel:formats.add(struct.unpack_from('<H',self.mem,starts+rel+6)[0])
                assert len(formats)==1 and formats <= {2,6}, formats
                self.pointer_format=next(iter(formats))
            off += size
        self.image_base=min(va for va,_,fo,fs,_ in self.segments if fo==0 and fs)
    def offset(self, va):
        for start, _, off, size, _ in self.segments:
            if start <= va < start+size:
                return off+va-start
        raise ValueError(f'unmapped/file-unbacked VA {va:#x}')
    def u32(self, off): return struct.unpack_from('<I', self.mem, off)[0]
    def ptr(self, va):
        raw = struct.unpack_from('<Q', self.mem, self.offset(va))[0]
        # The engine uses DYLD_CHAINED_PTR_64 (2); the app uses 64_OFFSET (6).
        # Read the documented Mach-O chained-fixup format from load commands.
        # Chained pointers' next
        # chain-link bits must not be treated as part of the virtual address.
        # A bound external symbol has no in-image VA.
        if not self.pointer_format:return raw
        if not raw or raw >> 63: return 0
        target=(raw & ((1<<36)-1))
        if self.pointer_format==6:return self.image_base+target
        return target | (((raw>>36)&255)<<56)
    def s32(self, va): return struct.unpack_from('<i', self.mem, self.offset(va))[0]
    def string(self, va):
        off = self.offset(va)
        return self.mem[off:self.mem.find(b'\0', off)].decode('utf-8')
    def pointers(self, section):
        va, size = self.sections.get(section, (0,0))
        return [self.ptr(a) for a in range(va, va+size, 8)]
    def classinfo(self, va):
        ro = self.ptr(va+32)&~7
        return self.string(self.ptr(ro+24)), self.ptr(ro+32)
    def methods(self, va, cls, category, kind):
        if not va: return []
        p = self.offset(va)
        flags, count = struct.unpack_from('<II', self.mem, p)
        small, direct = bool(flags&0x80000000), bool(flags&0x40000000)
        size = flags&0xffff
        assert size in (12,24), (cls,category,hex(flags))
        result = []
        for i in range(count):
            a = va+8+i*size
            if small:
                sn = a+self.s32(a)
                selector = self.string(sn if direct else self.ptr(sn))
                enc = self.string(a+4+self.s32(a+4))
                imp = a+8+self.s32(a+8)
            else:
                selector, enc, imp = self.string(self.ptr(a)), self.string(self.ptr(a+8)), self.ptr(a+16)
            result.append(dict(class_name=cls,category=category,kind=kind,selector=selector,
                               type_encoding=enc,engine170_imp_va=hex(imp),
                               engine170_imp_file_offset=hex(self.offset(imp))))
        return result
    def inventory(self):
        result=[]
        for a in self.pointers('__objc_classlist'):
            cls, methods = self.classinfo(a)
            result += self.methods(methods,cls,'','-')
            _, methods = self.classinfo(self.ptr(a))
            result += self.methods(methods,cls,'','+')
        for a in self.pointers('__objc_catlist'):
            cat = self.string(self.ptr(a))
            clsaddr = self.ptr(a+8)
            if not clsaddr: continue  # external category; cannot be one of defined imported Arc classes
            cls, _ = self.classinfo(clsaddr)
            result += self.methods(self.ptr(a+16),cls,cat,'-')
            result += self.methods(self.ptr(a+24),cls,cat,'+')
        return result

def write_tsv(path, rows, fields=None):
    with path.open('w') as out:
        w=csv.DictWriter(out, fieldnames=fields or list(rows[0]),delimiter='\t',lineterminator='\n')
        w.writeheader(); w.writerows(rows)

def run():
    imported=set(re.findall(r'_OBJC_CLASS_\$_(\w+)',(DATA/'app168-imports.txt').read_text()))
    engine=MachO(INPUT/'binaries/ArcCore-1.170.0.arm64')
    allrows=engine.inventory()
    rows=sorted([dict(r) for r in allrows if r['class_name'] in imported],
                key=lambda r:(r['class_name'],r['category'],r['kind'],r['selector']))
    assert len(rows)==834 and len({r['class_name'] for r in rows})==63
    keys=[(r['class_name'],r['category'],r['kind'],r['selector']) for r in rows]
    assert len(keys)==len(set(keys))
    dumpkeys={}
    for version in ('168','170'):
        d={}
        category=''
        for lineno,line in enumerate((DATA/f'dobjc{version}.txt').open(),1):
            interface=re.search(r'@interface\s+(\w+)(?:\(([^)]+)\))?',line)
            if interface: category=interface[2] or ''
            m=re.search(r'0x([0-9a-fA-F]+)\s+([-+])\[([^ ]+) ([^\]]+)\]',line)
            if m and m[3] in imported:
                key=(m[3],category,m[2],m[4])
                assert key not in d, key
                d[key]=(int(m[1],16),lineno)
        dumpkeys[version]=d
    assert set(dumpkeys['168'])==set(dumpkeys['170'])
    selected={int(r['engine170_imp_va'],16) for r in rows}
    gh={}
    with (INPUT/'metadata/decomp/arccore-1.170.0-functions.tsv').open() as f:
        for line in f:
            a,b,n,name=line.rstrip('\n').split('\t',3)
            if int(a,16) in selected: gh[int(a,16)]=(b,n,name)
    for r in rows:
        key=r['class_name'],r['category'],r['kind'],r['selector']
        assert int(r['engine170_imp_va'],16)==dumpkeys['170'][key][0], key
        r['engine168_imp_va']=hex(dumpkeys['168'][key][0])
        r['dump168_line']=dumpkeys['168'][key][1]
        r['dump170_line']=dumpkeys['170'][key][1]
        g=gh.get(int(r['engine170_imp_va'],16),('','',''))
        r.update(ghidra_end_bound_va=g[0],ghidra_body_bytes=g[1],ghidra_qualified_name=g[2])
        r['app_direct_call_status']='not_established'
    write_tsv(ROOT/'inventory.tsv',rows)
    write_tsv(ROOT/'evidence/engine-objc-all.tsv',allrows)
    summary=dict(imported_classes=len(imported),declared_methods=len(rows),
                 cxx_lifecycle=sum(r['selector'].startswith('.cxx_') for r in rows),
                 category_methods=sum(bool(r['category']) for r in rows),
                 exact_ghidra_start_matches=sum(bool(r['ghidra_qualified_name']) for r in rows),
                 same_selector_sets_168_170=True,engine_segments=engine.segments,
                 binary_sha256=hashlib.file_digest(engine.file,'sha256').hexdigest())
    (ROOT/'evidence/inventory-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
    print(json.dumps(summary,indent=2))

if __name__=='__main__':run()
