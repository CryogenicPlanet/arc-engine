#!/usr/bin/env python3
"""Metadata-only independent audit; bounded Mach-O reads, no older code reads.

Pairs selectors/types inside each actual method entry, never across dump order.
protocol_t offsets follow objc4 fb265098298302243cd7eeaa1f63f0ba7786dd9a.
No protocol method's third word is interpreted as an implementation.
"""
import collections,csv,hashlib,json,re,struct
from pathlib import Path
from macho import MachO
BASE=Path(__file__).resolve().parents[1]
INPUT=Path('/home/exedev/work/inputs')
def tsv(path,rows,fields=None):
    rows=list(rows);fields=fields or list(dict.fromkeys(k for r in rows for k in r))
    with path.open('w') as f:
        w=csv.DictWriter(f,fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
def load(path):return list(csv.DictReader(Path(path).open(),delimiter='\t'))
def sha(path):
    with path.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def guide():
    ps={};rows={};raw=0;p=None;required='required'
    for ln,line in enumerate((INPUT/'app168/dump/objc-classes.h').open(),1):
        m=re.match(r'@protocol (Arc\w+)(?: <([^>]+)>)?',line)
        if m:
            p=m[1];ps[p]=dict(protocol_name=p,inherited_guide=m[2] or '',guide_line=ln);continue
        if line.startswith('@end'):p=None
        if not p:continue
        if line.startswith('@required'):required='required'
        if line.startswith('@optional'):required='optional'
        m=re.match(r'([-+])\[('+p+r') (.+)\];',line)
        if m:
            raw+=1;k=(p,m[1],m[3]);r=rows.setdefault(k,dict(protocol_name=p,kind=m[1],selector=m[3],required_optional=required,guide_lines=[]))
            assert r['required_optional']==required
            r['guide_lines'].append(ln)
    assert len(ps)==26 and raw==235 and len(rows)==200 and len({k[2] for k in rows})==198
    return ps,rows,raw

class Metadata:
    def __init__(self,path,label,guide_names):
        self.b=MachO(path);self.label=label;self.protocols={};self.methods=[];self.classes={};self.adopters=[];self.impls=[];self.layouts={};self.errors=[];self.imports=[]
        b=self.b
        for cmd,d in b.commands:
            if cmd!=0x80000034:continue
            off,size=struct.unpack('<II',d);b.f.seek(off);blob=b.f.read(size)
            _,_,io,so,n,fmt,sfmt=struct.unpack_from('<7I',blob)
            assert sfmt==0
            for i in range(n):
                if fmt in (1,2):nameoff=struct.unpack_from('<I',blob,io+i*(4 if fmt==1 else 8))[0]>>9
                elif fmt==3:nameoff=struct.unpack_from('<Q',blob,io+i*16)[0]>>32
                else:raise ValueError(fmt)
                pos=so+nameoff;self.imports.append(blob[pos:blob.index(b'\0',pos)].decode())
        for p in self.section('__objc_protolist'):self.proto(p)
        # Include referenced, locally backed protocols even if not in protolist.
        for p in self.section('__objc_protorefs'):
            if p:self.proto(p)
        for p in self.section('__objc_classlist'):
            ro=b.pointer(p+32)&~7;name=b.string(b.pointer(ro+24));self.classes[p]=(name,ro)
        class_arcs={}
        for p,(name,ro) in self.classes.items():
            refs=self.refs(b.pointer(ro+40));names=self.flatten(refs)
            arc=sorted(x for x in names if x.startswith('Arc'))
            class_arcs[p]=arc
            if arc or name.startswith('Arc'):self.layouts[name]=self.layout(p,name,ro)
            if arc:
                self.adopters.append(dict(image=label,class_name=name,category='',class_va=hex(p),ro_va=hex(ro),direct_protocols=';'.join(self.proto(q)['name'] for q in refs),all_arc_protocols=';'.join(arc)))
            if arc or name.startswith('Arc'):
                for kind,cr in [('-',ro),('+',b.pointer(b.pointer(p)+32)&~7)]:
                    self.add_impl(b.pointer(cr+32),name,'',kind,p,arc)
        for p in self.section('__objc_catlist'):
            cat=b.string(b.pointer(p));cls=b.pointer(p+8) if not self.bound(p+8) else 0
            name=self.classes[cls][0] if cls in self.classes else self.bound(p+8).removeprefix('_OBJC_CLASS_$_')
            refs=self.refs(b.pointer(p+32));arc=sorted(x for x in self.flatten(refs) if x.startswith('Arc'))
            if arc:self.adopters.append(dict(image=label,class_name=name,category=cat,class_va=hex(cls),ro_va='',category_va=hex(p),direct_protocols=';'.join(self.proto(q)['name'] for q in refs),all_arc_protocols=';'.join(arc)))
            effective_arc=sorted(set(arc)|set(class_arcs.get(cls,[])))
            if effective_arc or name.startswith('Arc'):
                for off,kind in [(16,'-'),(24,'+')]:self.add_impl(b.pointer(p+off),name,cat,kind,cls,effective_arc)
        self.methods=[dict(image=label,protocol_name=p['name'],protocol_va=hex(va),**m) for va,p in self.protocols.items() if p['name'].startswith('Arc') for m in p['methods']]
    def section(self,name):
        for (_,sn),(va,sz,_) in self.b.sections.items():
            if sn==name:
                for a in range(va,va+sz,8):
                    if not self.bound(a):yield self.b.pointer(a)
    def bound(self,va):
        raw=self.b.u64(va)
        seg=next(s for s in self.b.segments if s['va']<=va<s['va']+s['filesize'])
        if self.b.pointer_formats.get(seg['name']) and raw>>63:
            return self.imports[raw&0xffffff]
        return ''
    def refs(self,va):
        if not va:return []
        n=self.b.u64(va);assert n<10000
        return [self.b.pointer(va+8+8*i) for i in range(n)]
    def flatten(self,refs,seen=None):
        seen=set() if seen is None else seen
        for p in refs:
            if p in seen:continue
            seen.add(p);q=self.proto(p);self.flatten(q['refs'],seen)
        return {self.proto(p)['name'] for p in seen}
    def read_methods(self,va,protocol=False):
        if not va:return []
        b=self.b;flags,n=struct.unpack('<II',b.read(va,8));size=flags&0xffff
        assert size in (12,24) and n<100000,(hex(va),hex(flags),n)
        rows=[]
        for i in range(n):
            e=va+8+i*size
            if flags&0x80000000:
                sn,ty,im=struct.unpack('<iii',b.read(e,12));ref=e+sn;sv=ref if flags&0x40000000 else b.pointer(ref);tv=e+4+ty;imp=e+8+im
            else:sv=b.pointer(e);tv=b.pointer(e+8);imp=0 if protocol else b.pointer(e+16)
            rows.append(dict(selector=b.string(sv),type_encoding=b.string(tv),entry_va=hex(e),list_va=hex(va),selector_va=hex(sv),type_va=hex(tv),imp='' if protocol else hex(imp)))
        return rows
    def proto(self,p):
        if p in self.protocols:return self.protocols[p]
        b=self.b;name=b.string(b.pointer(p+8));q=dict(name=name,va=hex(p),refs=[],inherited=[],methods=[]);self.protocols[p]=q
        q['refs']=self.refs(b.pointer(p+16));q['inherited']=[self.proto(r)['name'] for r in q['refs']]
        size,flags=struct.unpack('<II',b.read(p+64,8));q.update(size=size,flags=hex(flags))
        # Pair within this on-disk protocol using its actual list prefix and
        # entry index, as getExtendedTypesIndexesForMethod specifies. This is
        # not an order join across dumps. Runtime may later sort lists/arrays.
        ext=b.pointer(p+72) if size>=80 else 0;q['extended_types_va']=hex(ext)
        for off,kind,req in [(24,'-','required'),(32,'+','required'),(40,'-','optional'),(48,'+','optional')]:
            for m in self.read_methods(b.pointer(p+off),True):q['methods'].append(dict(kind=kind,required_optional=req,**m))
        if name.startswith('Arc') and ext:
            q['extended_types_array']=[dict(slot_va=hex(ext+i*8),type_va=hex(b.pointer(ext+i*8)),encoding=b.string(b.pointer(ext+i*8))) for i in range(len(q['methods']))]
            for m,e in zip(q['methods'],q['extended_types_array']):
                # Independently check that adding object/block annotations is
                # the only difference from that method entry's raw encoding.
                plain=re.sub(r'"[^"]*"','',e['encoding'])
                while '<' in plain:
                    simplified=re.sub(r'<[^<>]*>','',plain)
                    assert simplified!=plain
                    plain=simplified
                assert plain==m['type_encoding'],(name,m,e)
                m.update(extended_type_encoding=e['encoding'],extended_type_slot_va=e['slot_va'],extended_type_va=e['type_va'],extended_pairing='same_protocol_list_prefix+entry_index;objc4:4885-4924;raw_erasure_verified')
        return q
    def add_impl(self,va,name,cat,kind,cls,arc):
        for m in self.read_methods(va):self.impls.append(dict(image=self.label,class_name=name,category=cat,kind=kind,class_va=hex(cls),protocols=';'.join(arc),**m))
    def ivar_offset(self,op):
        if not op:return 'null_offset_pointer'
        try:return struct.unpack('<I',self.b.read(op,4))[0]
        except ValueError:
            if any(s['va']+s['filesize']<=op and op+4<=s['va']+s['vmsize'] for s in self.b.segments):return 'zerofill_initial_0;runtime_fixup_unresolved'
            raise
    def layout(self,p,name,ro):
        b=self.b;flags,start,size=struct.unpack('<III',b.read(ro,12));sup=self.bound(p+8)
        if not sup:
            sv=b.pointer(p+8);sup=self.classes[sv][0] if sv in self.classes else hex(sv)
        iv=b.pointer(ro+48);ivars=[]
        if iv:
            stride,n=struct.unpack('<II',b.read(iv,8));assert stride>=32 and n<10000
            for i in range(n):
                e=iv+8+i*stride;op=b.pointer(e);align,isz=struct.unpack('<II',b.read(e+24,8))
                ivars.append(dict(name=b.string(b.pointer(e+8)),type=b.string(b.pointer(e+16)),offset=self.ivar_offset(op),alignment=align,size=isz,entry_va=hex(e)))
        def mapbytes(off):
            v=b.pointer(ro+off)
            if not v:return ''
            data=bytearray()
            for j in range(256):
                x=b.read(v+j,1)[0];data.append(x)
                if not x:return data.hex()
            raise ValueError('unterminated layout')
        return dict(image=self.label,class_va=hex(p),ro_va=hex(ro),superclass=sup,flags=hex(flags),instance_start=start,instance_size=size,ivars=ivars,strong_layout=mapbytes(16),weak_layout=mapbytes(56))

def main():
    ps,guide_rows,raw=guide();tsv(BASE/'evidence/guide-identities.tsv',[{**r,'guide_lines':';'.join(map(str,r['guide_lines']))} for r in guide_rows.values()])
    images={};pins=[]
    for label,file,expected in [('app168','Arc-1.168.0.arm64','4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682'),('engine168','ArcCore-1.168.0.arm64','ca439a53a40f633cde10182533408b2220d8c918c04d22a12a6ba25ae2563c53'),('engine170','ArcCore-1.170.0.arm64','69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd')]:
        path=INPUT/'binaries'/file;digest=sha(path);assert digest==expected
        pins.append(dict(image=label,path=str(path),sha256=digest));im=Metadata(path,label,ps);images[label]=im
        selected=[dict(name=q['name'],**{k:v for k,v in q.items() if k not in ['name','refs']}) for q in im.protocols.values() if q['name'].startswith('Arc')]
        (BASE/f'evidence/{label}-protocols.json').write_text(json.dumps(selected,indent=2)+'\n')
        tsv(BASE/f'evidence/{label}-methods.tsv',im.methods)
        tsv(BASE/f'evidence/{label}-protocol-inventory.tsv',[dict(image=label,protocol_name=q['name'],protocol_va=hex(va),inherited=';'.join(q['inherited']),method_rows=len(q['methods'])) for va,q in im.protocols.items()])
        tsv(BASE/f'evidence/{label}-adopters.tsv',im.adopters)
        tsv(BASE/f'evidence/{label}-implementations.tsv',im.impls)
        (BASE/f'evidence/{label}-layouts.json').write_text(json.dumps({k:v for k,v in im.layouts.items() if k.startswith('Arc') or any(a['class_name']==k for a in im.adopters)},indent=2)+'\n')
        print(label,len(im.protocols),len(im.methods),len(im.adopters),len(im.impls),flush=True)
    tsv(BASE/'evidence/input-pins.tsv',pins)
    extra=[dict(image=label,protocol_name=q['name'],protocol_va=hex(va),**m) for label,im in images.items() for va,q in im.protocols.items() for m in q['methods'] if (q['name'],m['kind'],m['selector']) not in guide_rows]
    tsv(BASE/'actual-metadata-only.tsv',extra)
    contract=load('/home/exedev/work/agents/abi/initial/contract.tsv');unknown=[r for r in contract if r['compatibility']=='unknown_encoding'];assert len(unknown)==6
    audits=[]
    for r in unknown:
        key=tuple(r[x] for x in ['class_name','category','kind','selector']);found=[]
        for label in ['engine168','engine170']:
            match=[m for m in images[label].impls if tuple(m[x] for x in ['class_name','category','kind','selector'])==key];assert len(match)==1,(label,key,len(match));found.append(match[0])
        old,new=found;assert new['type_encoding']==r['type_encoding'] and new['imp']==r['imp'] and old['imp']==r['imp_168']
        ol,nl=(images[x].layouts[r['class_name']] for x in ['engine168','engine170'])
        layout_keys=['superclass','instance_start','instance_size','strong_layout','weak_layout']
        semantic_ivar=lambda l:[{k:v for k,v in i.items() if k!='entry_va'} for i in l['ivars']]
        stable=all(ol[k]==nl[k] for k in layout_keys) and semantic_ivar(ol)==semantic_ivar(nl)
        audits.append(dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],encoding_168=old['type_encoding'],encoding_170=new['type_encoding'],imp_168=old['imp'],imp_170=new['imp'],method_entry_168=old['entry_va'],method_entry_170=new['entry_va'],list_168=old['list_va'],list_170=new['list_va'],type_va_168=old['type_va'],type_va_170=new['type_va'],pairing='independent_binary_class/category/kind/selector;selector+encoding_read_same_entry',raw_encoding_comparison='equal' if old['type_encoding']==new['type_encoding'] else 'different',superclass_168=ol['superclass'],superclass_170=nl['superclass'],instance_start_168=ol['instance_start'],instance_start_170=nl['instance_start'],instance_size_168=ol['instance_size'],instance_size_170=nl['instance_size'],relevant_layout_equal=stable,layout_evidence=f"evidence/engine168-layouts.json:{r['class_name']};evidence/engine170-layouts.json:{r['class_name']}",resolution='older_encoding_resolved',limits='Metadata only; no older body analysis; equal raw encoding and ObjC layout do not prove native C++ ABI, ownership or behavior.'))
    tsv(BASE/'older-encoding-audit.tsv',audits)
    summary=dict(guide_protocols=len(ps),guide_raw_methods=raw,guide_identities=len(guide_rows),guide_selectors=len({x[2] for x in guide_rows}),images={label:dict(arc_protocols=len({m['protocol_name'] for m in im.methods}),arc_method_rows=len(im.methods),adopter_rows=len(im.adopters),classes_scanned=len(im.classes)) for label,im in images.items()},additional_metadata_rows=len(extra),older_unknowns_resolved=len(audits))
    (BASE/'evidence/metadata-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
if __name__=='__main__':main()
