#!/usr/bin/env python3
"""Rebuild Arc ObjC surface from dumps; never pair rows by list position.

Python standard library only. Binary access is seek/read through LC_SEGMENT_64.
The 168 selector join is explicitly weaker: unique class/category/kind/IMP in
the dyld dump, with selector-reference propagation only when unambiguous.
"""
import argparse, collections, csv, hashlib, json, re, struct
from pathlib import Path

ROOT = Path('/home/exedev/work/inputs')
OUT = Path(__file__).resolve().parents[1] / 'initial'
DATA = ROOT / 'context/analysis/data'

def write_tsv(path, rows, fields=None):
    rows = list(rows)
    fields = fields or (list(rows[0]) if rows else [])
    with path.open('w') as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter='\t', extrasaction='ignore')
        w.writeheader(); w.writerows(rows)

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

def dyld(version):
    rows=[]; cls={}; ctx=None
    p=DATA/f'dobjc{version}.txt'
    for ln,line in enumerate(p.open(),1):
        m=re.search(r'@interface ([\w]+)(?:\(([^)]+)\))?(?:\s*:\s*(\S+)\s*(.*))?',line)
        if m:
            name,cat,sup,pro=m.groups(); ctx=(name,cat or '')
            if not cat:
                cls[name]=dict(class_name=name,superclass_dyld=sup or '',protocols_dyld=(pro or '').strip(),dyld_line=ln)
        elif '@end' in line or '@protocol' in line: ctx=None
        m=re.search(r'(0x[0-9A-Fa-f]+)\s+([-+])\[([^ ]+) (.+)\]',line)
        if m and ctx:
            imp,kind,name,sel=m.groups(); assert name==ctx[0]
            rows.append(dict(class_name=name,category=ctx[1],kind=kind,selector=sel,imp=hex(int(imp,16)),dyld_line=ln))
    return rows,cls

def dump_blocks(version, section):
    p=DATA/f'objc{version}.txt'; active=False; block=[]
    for ln,line in enumerate(p.open(),1):
        if line.startswith('Contents of'):
            if active and block: yield block
            active=section in line; block=[]; continue
        if not active:continue
        if re.match(r'^[0-9a-f]{16} ',line):
            if block:yield block
            block=[]
        block.append((ln,line.rstrip('\n')))
    if active and block:yield block

def fields_at(block,indent):
    result={}
    for ln,line in block:
        if len(line)-len(line.lstrip())!=indent:continue
        m=re.match(r'\s*(\w+)\s+(.*)',line)
        if m:result[m[1]]=(m[2],ln)
    return result

def value_text(v):
    return re.sub(r'^0x[0-9a-f]+\s*','',v).strip()

def method_lists(block, field, indent, context):
    """Only direct list fields at a known indentation (ignore nested protocols)."""
    rows=[]; active=False; row=None; count=None; start=None; listva=None; index=0; entsize=0
    for ln,line in block:
        n=len(line)-len(line.lstrip()); s=line.strip()
        if n==indent:
            if active and row:raise ValueError('incomplete method')
            active=s.startswith(field+' ')
            if active: start=ln; listva=int(s.split()[1],16); index=0
            continue
        if not active or n!=indent+4:continue
        if s.startswith('entsize '):entsize=int(s.split()[1]);continue
        if s.startswith('count '):count=int(s.split()[1]);continue
        m=re.match(r'name\s+(0x[0-9a-f]+)(?:\s+\((0x[0-9a-f]+)\))?(?:\s+(.*))?',s)
        if m:
            row=dict(context,otool_line=ln,list_line=start,list_count=count,method_entry_va=hex(listva+8+index*entsize),
                     selector_ref=m[2] or m[1],selector_literal=m[3] or '')
        m=re.match(r'types\s+(0x[0-9a-f]+)(?:\s+\((0x[0-9a-f]+)\))?\s+(.+)',s)
        if m and row:row.update(type_encoding=m[3],type_va=m[2] or m[1])
        m=re.match(r'imp\s+(0x[0-9a-f]+)(?:\s+\((0x[0-9a-f]+)\))?',s)
        if m and row:
            row['imp']=hex(int(m[2] or m[1],16)); rows.append(row); row=None; index+=1
    counts=collections.Counter(r['list_line'] for r in rows)
    assert all(counts[r['list_line']]==r['list_count'] for r in rows), 'method-list count mismatch'
    return rows

def otool(version):
    rows=[]; layouts=[]
    for block in dump_blocks(version,'__objc_classlist)'):
        split=next((i for i,(_,l) in enumerate(block) if l=='Meta Class'),len(block))
        base=block[:split]; f=fields_at(base,8)
        if 'name' not in f:continue
        name=value_text(f['name'][0]); top=fields_at(base,4)
        lay=dict(class_name=name,otool_line=block[0][0],class_va=block[0][1].split()[1],
                 superclass=top.get('superclass',('',0))[0])
        for k in ['instanceStart','instanceSize','ivarLayout','weakIvarLayout','flags']:
            lay[k]=f.get(k,('',0))[0]
        # Preserve direct ivars, layout maps, properties, and protocol names as raw
        # source-located text; nested method declarations are not class methods.
        lay['metadata_json']=json.dumps([{'line':ln,'text':line} for ln,line in base if
            ('baseProtocols' in line or 'layout map' in line or 'ivars ' in line or 'offset ' in line or
             'size ' in line or 'alignment ' in line or 'attributes ' in line or
             (re.match(r'\s+name ',line) and len(line)-len(line.lstrip())>8))],separators=(',',':'))
        layouts.append(lay)
        rows+=method_lists(base,'baseMethods',8,dict(class_name=name,category='',kind='-'))
        rows+=method_lists(block[split:],'baseMethods',8,dict(class_name=name,category='',kind='+'))
    class_names_by_va={x['class_va']:x['class_name'] for x in layouts}
    for block in dump_blocks(version,'__objc_catlist)'):
        f=fields_at(block,4)
        if 'name' not in f or 'cls' not in f:continue
        cat=value_text(f['name'][0]); cv=f['cls'][0]
        m=re.search(r'_OBJC_CLASS_\$_(\w+)',cv)
        name=m[1] if m else class_names_by_va.get(cv.split()[0],'')
        assert name, (version,block[0])
        for field,kind in [('instanceMethods','-'),('classMethods','+')]:
            rows+=method_lists(block,field,4,dict(class_name=name,category=cat,kind=kind))
    return rows,layouts

def join(version,binary=None):
    d,classes=dyld(version); o,layouts=otool(version)
    byimp=collections.defaultdict(list)
    for r in d:byimp[tuple(r[x] for x in ['class_name','category','kind','imp'])].append(r)
    resolved={}; pending=[]; result=[]; failures=[]
    for r in o:
        key=tuple(r[x] for x in ['class_name','category','kind','imp']); cand=byimp[key]
        if binary:
            entry=int(r['method_entry_va'],16)
            rel=struct.unpack('<iii',binary.read(entry,12))
            assert entry+rel[0]==int(r['selector_ref'],16)
            assert entry+4+rel[1]==int(r['type_va'],16)
            assert entry+8+rel[2]==int(r['imp'],16)
            assert binary.string(int(r['type_va'],16))==r['type_encoding']
            try:sel=r['selector_literal'] or binary.string(binary.pointer(int(r['selector_ref'],16)))
            except (ValueError,AssertionError):sel=''
            exact=[x for x in cand if x['selector']==sel]
            if len(exact)==1:
                r.update(selector=sel,pairing='binary_selector+class/category/kind/IMP',dyld_line=exact[0]['dyld_line'])
            else:failures.append(dict(r,candidates=cand,decoded_selector=sel));continue
        elif len(cand)==1:
            r.update(selector=cand[0]['selector'],pairing='unique_class/category/kind/IMP_cross_dump',dyld_line=cand[0]['dyld_line'])
        else:pending.append((r,cand));continue
        resolved.setdefault(r['selector_ref'],set()).add(r['selector']); result.append(r)
    for r,cand in pending:
        exact=[x for x in cand if x['selector'] in resolved.get(r['selector_ref'],set())]
        if len(exact)==1:
            r.update(selector=exact[0]['selector'],pairing='unambiguous_selector_ref_propagation+class/category/kind/IMP',dyld_line=exact[0]['dyld_line']);result.append(r)
        else:failures.append(dict(r,candidates=cand))
    # Every dyld declaration must be accounted for once, even when types unknown.
    keyed={tuple(r[x] for x in ['class_name','category','kind','selector']):r for r in result}
    assert len(keyed)==len(result), 'duplicate method key'
    for r in d:
        key=tuple(r[x] for x in ['class_name','category','kind','selector'])
        if key not in keyed:
            result.append(dict(r,type_encoding='',type_va='',selector_ref='',otool_line='',list_line='',list_count='',method_entry_va='',pairing='unknown_no_unambiguous_otool_pair'))
    return result,classes,layouts,dict(dyld_methods=len(d),otool_methods=len(o),joined=len(keyed),unknown=len(d)-len(keyed),failures=failures)

def main():
    OUT.mkdir(exist_ok=True)
    binary=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
    app=MachO(ROOT/'binaries/Arc-1.168.0.arm64')
    imports=[l.strip() for l in (DATA/'app168-imports.txt').open() if l.strip()]
    imported={x.split('$_',1)[1] for x in imports if x.startswith('_OBJC_CLASS_$_')}
    actual={s for s,t,_,_,_ in app.symbols() if t&0x0e==0 and s.startswith('_OBJC_CLASS_$_Arc')}
    assert actual=={x for x in imports if x.startswith('_OBJC_CLASS_$_')},(actual,imports)
    rows170,c170,lay170,j170=join('170',binary); rows168,c168,lay168,j168=join('168')
    key=lambda r:tuple(r[x] for x in ['class_name','category','kind','selector'])
    old={key(r):r for r in rows168}; new={key(r):r for r in rows170}
    # The requested contract has one row per 170 declaration on an imported
    # class. Removed 168 methods are preserved separately as compatibility.tsv.
    contract=[]; changes=[]
    for r in rows170:
        if r['class_name'] not in imported:continue
        prev=old.get(key(r)); status='added' if prev is None else ('unchanged_encoding' if prev['type_encoding'] and prev['type_encoding']==r['type_encoding'] else ('unknown_encoding' if not prev['type_encoding'] or not r['type_encoding'] else 'encoding_changed'))
        row=dict(engine_version='1.170.0',app_version='1.168.0',address_basis='engine_unslid_MachO_VA',**r,
                 encoding_168=prev['type_encoding'] if prev else '',imp_168=prev['imp'] if prev else '',pairing_168=prev['pairing'] if prev else '',
                 compatibility=status,usage_status='declared_only_pending_call_scan',call_evidence_count=0,
                 receiver_proof='none',ownership_threading='unknown_metadata_does_not_specify',
                 superclass=c170[r['class_name']]['superclass_dyld'],protocols=c170[r['class_name']]['protocols_dyld'])
        row.pop('selector_literal',None);contract.append(row)
        if status!='unchanged_encoding':changes.append(dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],status=status,encoding_168=row['encoding_168'],encoding_170=r['type_encoding'],imp_168=row['imp_168'],imp_170=r['imp']))
    for r in rows168:
        if r['class_name'] in imported and key(r) not in new:
            changes.append(dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],status='removed',encoding_168=r['type_encoding'],encoding_170='',imp_168=r['imp'],imp_170=''))
    write_tsv(OUT/'contract.tsv',sorted(contract,key=key))
    write_tsv(OUT/'compatibility.tsv',changes,['class_name','category','kind','selector','status','encoding_168','encoding_170','imp_168','imp_170'])
    for v,rows,lay in [('170',rows170,lay170),('168',rows168,lay168)]:
        write_tsv(OUT/f'all-methods-{v}.tsv',rows)
        write_tsv(OUT/f'class-layout-{v}.tsv',[dict(x,imported_by_app168=x['class_name'] in imported) for x in lay])
    layout_changes=[]; l168={x['class_name']:x for x in lay168}
    for r in lay170:
        if r['class_name'] not in imported:continue
        prev=l168[r['class_name']]
        # Pointer-valued layout references differ by image; compare actual map
        # bytes and ivar/property records later, never addresses as ABI changes.
        for k in ['instanceStart','instanceSize','flags']:
            if prev[k]!=r[k]:layout_changes.append(dict(class_name=r['class_name'],field=k,value168=prev[k],value170=r[k]))
        if c168[r['class_name']]['superclass_dyld']!=c170[r['class_name']]['superclass_dyld'] or c168[r['class_name']]['protocols_dyld']!=c170[r['class_name']]['protocols_dyld']:
            layout_changes.append(dict(class_name=r['class_name'],field='superclass/protocols',value168=json.dumps(c168[r['class_name']]),value170=json.dumps(c170[r['class_name']])))
    write_tsv(OUT/'layout-changes.tsv',layout_changes,['class_name','field','value168','value170'])
    summary=dict(imported_classes=len(imported),imported_classes_binary_verified=True,imported_class_names=sorted(imported),
                 nonclass_imports=[x for x in imports if not x.startswith('_OBJC_CLASS_$_')],
                 surface_170=len(contract),surface_168=sum(r['class_name'] in imported for r in rows168),
                 surface_170_base=sum(not r['category'] for r in contract),surface_170_category=sum(bool(r['category']) for r in contract),
                 surface_170_instance=sum(r['kind']=='-' for r in contract),surface_170_class=sum(r['kind']=='+' for r in contract),
                 pairing_170=j170,pairing_168=j168,compatibility=dict(collections.Counter(r['compatibility'] for r in contract)),
                 removed=sum(r['status']=='removed' for r in changes),layout_changes=layout_changes,
                 segments_engine=binary.segments,segments_app=app.segments)
    (OUT/'inventory-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
    print(json.dumps({k:v for k,v in summary.items() if k not in ['imported_class_names','segments_engine','segments_app','pairing_168','pairing_170']},indent=2))
    print('pairing 170', {k:v for k,v in j170.items() if k!='failures'}, '168', {k:v for k,v in j168.items() if k!='failures'})

if __name__=='__main__':main()
