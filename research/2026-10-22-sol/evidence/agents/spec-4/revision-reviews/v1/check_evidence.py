#!/usr/bin/env python3
"""Independent bounded static checks. Writes only alongside this script."""
import collections, csv, ctypes as C, fcntl, hashlib, json, os, re, resource, sqlite3, struct
from pathlib import Path

HERE=Path(__file__).resolve().parent
O=Path('/home/exedev/work/agents/spec-4/specs')
R=Path('/home/exedev/work/agents/spec-4/revisions/v1')
V=Path('/home/exedev/work/agents/spec-3/classreview')
I=Path('/home/exedev/work/inputs')
os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
def rows(p):
    with p.open() as f:return list(csv.DictReader(f,delimiter='\t'))
def digest(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
def write(name, records, fields=None):
    with (HERE/name).open('w') as f:
        w=csv.DictWriter(f, fields or list(records[0]),delimiter='\t');w.writeheader();w.writerows(records)

class Binary:
    def __init__(self,p):
        self.f=p.open('rb');self.segments=[];self.sections={}
        h=self.read(0,32);assert struct.unpack_from('<II',h)==(0xfeedfacf,0x100000c)
        pos=32
        for _ in range(struct.unpack_from('<I',h,16)[0]):
            cmd,n=struct.unpack('<II',self.read(pos,8));b=self.read(pos,n)
            if cmd==0x19:
                sg,va,vs,off,fs,mx,ini,ns,fl=struct.unpack_from('<16sQQQQiiII',b,8)
                self.segments.append((va,fs,off))
                for j in range(ns):
                    sec,seg,a,size,fo=struct.unpack_from('<16s16sQQI',b,72+80*j)
                    self.sections[sec.rstrip(b'\0').decode()]=(a,size,fo)
            if cmd==0x80000034:
                fixoff=struct.unpack_from('<I',b,8)[0]
                starts=fixoff+struct.unpack('<I',self.read(fixoff+4,4))[0]
                count=struct.unpack('<I',self.read(starts,4))[0]
                formats=set()
                for j in range(count):
                    delta=struct.unpack('<I',self.read(starts+4+4*j,4))[0]
                    if delta:formats.add(struct.unpack('<H',self.read(starts+delta+6,2))[0])
                assert formats=={2},formats
            pos+=n
    def read(self,off,n):
        assert 0<=n<=32768
        self.f.seek(off);b=self.f.read(n);assert len(b)==n;return b
    def offset(self,a,n=1):
        for va,fs,off in self.segments:
            if va<=a and a+n<=va+fs:return off+a-va
        raise ValueError(hex(a))
    def data(self,a,n):return self.read(self.offset(a,n),n)
    def uint(self,a):return struct.unpack('<I',self.data(a,4))[0]
    def ptr(self,a):
        x=struct.unpack('<Q',self.data(a,8))[0]
        return 0 if x>>63 else (x&((1<<36)-1))|(((x>>36)&255)<<56)
    def string(self,a):return self.data(a,4096).split(b'\0')[0].decode()
    def section_ptrs(self,name):
        a,n,_=self.sections[name]
        return [self.ptr(x) for x in range(a,a+n,8)]
    def methods(self,a):
        if not a:return []
        flags,n=struct.unpack('<II',self.data(a,8));small=flags>>31;direct=flags&0x40000000;stride=flags&0xffff
        assert n<10000 and stride in (12,24)
        out=[]
        for j in range(n):
            m=a+8+j*stride
            if small:
                ds,dt,di=struct.unpack('<iii',self.data(m,12));s=m+ds
                if not direct:s=self.ptr(s)
                t=m+4+dt;imp=m+8+di
            else:s,t,imp=(self.ptr(m+k) for k in (0,8,16))
            out.append(dict(selector=self.string(s),type_encoding=self.string(t),imp_va=hex(imp),method_record_va=hex(m)))
        return out

old=rows(O/'methods.tsv');new=rows(R/'methods.tsv')
assigned=json.loads((I/'context/class-assignments.json').read_text())['groups']['spec-4']['classes']
identity=['class_name','category','kind','selector','type_encoding','imp_va']
key=lambda x:tuple(x[k] for k in identity)
assert len(new)==491 and [key(x) for x in new]==[key(x) for x in old]
assert len({key(x)[:4] for x in new})==491
assert {p.stem for p in (R/'classes').glob('*.md')}==set(assigned)
corrections=rows(R/'corrections.tsv');findings=rows(V/'findings.tsv')
assert len(corrections)==len(findings)==9
for j,(f,c) in enumerate(zip(findings,corrections),1):
    assert c['finding_row']==str(j) and c['class_name']==f['class'] and c['selector']==f['selector'] and c['before']==f['claim']
snap=rows(V/'reviewed-spec-snapshot.tsv')
for x in snap:assert digest(Path(x['path']))==x['sha256'],x['path']
preserved=[]
for d in ('bodies','evidence','sources','scripts'):
    orig={p.relative_to(O) for p in (O/d).rglob('*') if p.is_file() and '__pycache__' not in p.parts}
    current={p.relative_to(R) for p in (R/d).rglob('*') if p.is_file()}
    assert orig==current
    for p in sorted(orig):
        h=digest(O/p);assert h==digest(R/p);preserved.append(dict(path=str(p),sha256=h))
assert (O/'report.md').read_bytes()==(R/'original-report.md').read_bytes()
write('preserved-evidence-check.tsv',preserved)

image=I/'binaries/ArcCore-1.170.0.arm64'
# Only the full image hash is a heavy read; lock the existing file without writing it.
with Path('/home/exedev/work/heavy.lock').open('rb') as lock:
    fcntl.flock(lock,fcntl.LOCK_EX)
    image_hash=digest(image)
    fcntl.flock(lock,fcntl.LOCK_UN)
assert image_hash=='69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd'
b=Binary(image);decoded=[];layouts={}
for cl in b.section_ptrs('__objc_classlist'):
    ro=b.ptr(cl+32)&~7;name=b.string(b.ptr(ro+24))
    if name not in assigned:continue
    layouts[name]=(cl,ro)
    for a,kind in ((cl,'-'),(b.ptr(cl),'+')):
        r=b.ptr(a+32)&~7
        for m in b.methods(b.ptr(r+32)):decoded.append(dict(class_name=name,category='',kind=kind,**m))
categories=[]
for cat in b.section_ptrs('__objc_catlist'):
    cls=b.ptr(cat+8)
    if not cls:continue
    ro=b.ptr(cls+32)&~7;name=b.string(b.ptr(ro+24))
    if name not in assigned:continue
    catname=b.string(b.ptr(cat));categories.append((name,catname,hex(cat)))
    for off,kind in ((16,'-'),(24,'+')):
        for m in b.methods(b.ptr(cat+off)):decoded.append(dict(class_name=name,category=catname,kind=kind,**m))
assert set(layouts)==set(assigned)
assert collections.Counter(map(key,decoded))==collections.Counter(map(key,new))
lookup={key(x)[:4]:x for x in decoded}
for x in new:
    mr=json.loads(x['machine_record']);assert mr['method_record_va']==lookup[key(x)[:4]]['method_record_va']
    assert int(mr['imp_file_offset'],16)==b.offset(int(x['imp_va'],16))
write('binary-method-check.tsv',[dict(image='ArcCore 1.170.0 arm64',address_space='unslid VA',**x) for x in sorted(decoded,key=lambda x:key(x))])
layout=json.loads((R/'evidence/layout.json').read_text())
for name,(cl,ro) in layouts.items():
    v=layout[name]
    assert int(v['metadata_va'],16)==cl
    assert struct.unpack('<II',b.data(ro+4,8))==(v['instance_start'],v['instance_size'])
    va=b.ptr(ro+48);ivs=[]
    if va:
        stride,count=struct.unpack('<II',b.data(va,8))
        for j in range(count):
            a=va+8+j*stride
            ivs.append(dict(name=b.string(b.ptr(a+8)),type=b.string(b.ptr(a+16)),offset=b.uint(b.ptr(a)),size=b.uint(a+28),record_va=hex(a)))
    assert [{k:x[k] for k in ivs[0]} for x in v['ivars']]==ivs if ivs else not v['ivars']
dyld=[];name=cat=''
for line in (I/'context/analysis/data/dobjc170.txt').read_text().splitlines():
    m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
    if m:name,cat=m[1],m[2] or ''
    m=re.search(r'(0x[0-9a-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
    if m and m[3] in assigned:
        assert name==m[3];dyld.append((name,cat,m[2],m[4],hex(int(m[1],16))))
assert collections.Counter(dyld)==collections.Counter(tuple(x[k] for k in ('class_name','category','kind','selector','imp_va')) for x in new)
coverage=[]
for name in assigned:
    doc=(R/'classes'/f'{name}.md').read_text();matrix=[]
    for line in doc.splitlines():
        if not line.startswith('| ') or '| M `' not in line:continue
        p=[x.strip() for x in line.split('|')[1:-1]];assert len(p)==8
        ks=p[1].strip('`');imp=re.match(r'\[(0x[0-9a-f]+)\]',p[3])[1]
        matrix.append(('' if p[0]=='(base)' else p[0],ks[0],ks[1:],p[2].strip('`'),imp,p[4],p[7]))
    methods=[x for x in new if x['class_name']==name]
    expected=[tuple(x[k] for k in ('category','kind','selector','type_encoding','imp_va','behavior')) for x in methods]
    assert collections.Counter(x[:6] for x in matrix)==collections.Counter(expected)
    for x in methods:
        p=next(z for z in matrix if z[:5]==tuple(x[k] for k in ('category','kind','selector','type_encoding','imp_va')))
        assert p[6].endswith('; '+x['body_status']+'; '+x['semantic_status'])
    for target in re.findall(r'\]\(([^)#]+)(?:#[^)]*)?\)',doc):
        if '://' not in target:assert (R/'classes'/target).exists(),(name,target)
    coverage.append(dict(class_name=name,method_rows=len(methods),matrix_rows=len(matrix),unique_imps=len({x['imp_va'] for x in methods}),binary_identity_layout='PASS',matrix_annotations='PASS',changed=(R/'classes'/f'{name}.md').read_bytes()!=(O/'classes'/f'{name}.md').read_bytes()))
write('coverage-check.tsv',coverage)
cov=json.loads((R/'coverage.json').read_text())
for field,col in (('body_status_counts','body_status'),('semantic_status_counts','semantic_status')):
    assert cov[field]==dict(collections.Counter(x[col] for x in new))
assert cov['actual_class_specs']==len(assigned)==38 and cov['actual_method_rows']==len(new)==491
for name in assigned:assert cov['by_class'][name]['methods']==sum(x['class_name']==name for x in new)

db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
bodychecks=[]
for root in (R,V):
    for p in sorted((root/'bodies').glob('*.c')):
        a=int(p.stem,16);rec=db.execute('select name,path,offset,length,failed from functions where va=?',(a,)).fetchone()
        name,path,off,n,failed=rec;assert not failed and n<=32768
        with open(path,'rb') as f:f.seek(off);raw=f.read(n)
        assert raw==p.read_bytes()
        bodychecks.append(dict(image='ArcCore 1.170.0 arm64',address_space='unslid VA',snapshot=str(root),va=hex(a),name=name,index_path=path,index_offset=off,index_length=n,sha256=hashlib.sha256(raw).hexdigest()))
write('saved-body-check.tsv',bodychecks)
transcript=[]
files=list((R/'revision-evidence').glob('*bodies.txt'))+list(HERE.glob('*bodies.txt'))
for p in files:
    t=p.read_text();assert 'TRUNCATED:' not in t
    matches=list(re.finditer(r'^INDEX (0x[0-9a-f]+) (.*?) path=(.*?) offset=(\d+) C_bytes=(\d+) failed=(\d+)\n',t,re.M))
    for j,m in enumerate(matches):
        va,name,path,off,n,failed=m.groups();assert failed=='0'
        assert db.execute('select name,path,offset,length,failed from functions where va=?',(int(va,16),)).fetchone()==(name,path,int(off),int(n),0)
        with open(path,'rb') as f:f.seek(int(off));raw=f.read(int(n))
        end=matches[j+1].start() if j+1<len(matches) else len(t)
        assert t[m.end():end]==raw.decode()+'\n'
        transcript.append(dict(image='ArcCore 1.170.0 arm64',address_space='unslid VA',transcript=str(p),va=va,index_path=path,index_offset=off,index_length=n,sha256=hashlib.sha256(raw).hexdigest()))
write('transcript-check.tsv',transcript)

lib=C.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM.so.20.1')
for suffix in ('TargetInfo','Target','TargetMC','Disassembler'):getattr(lib,'LLVMInitializeAArch64'+suffix)()
lib.LLVMCreateDisasmCPUFeatures.argtypes=[C.c_char_p,C.c_char_p,C.c_char_p,C.c_void_p,C.c_int,C.c_void_p,C.c_void_p];lib.LLVMCreateDisasmCPUFeatures.restype=C.c_void_p
lib.LLVMDisasmInstruction.argtypes=[C.c_void_p,C.c_void_p,C.c_uint64,C.c_uint64,C.c_char_p,C.c_size_t];lib.LLVMDisasmInstruction.restype=C.c_size_t
ctx=lib.LLVMCreateDisasmCPUFeatures(b'arm64-apple-macos',b'generic',b'+lse,+rcpc',None,0,None,None);assert ctx
instructions=[]
for x in rows(R/'revision-evidence/instructions.tsv'):
    va=int(x['va'],16);raw=b.data(va,4);assert raw.hex()==x['bytes'] and b.offset(va,4)==int(x['file_offset'],16)
    out=C.create_string_buffer(512);buf=C.create_string_buffer(raw)
    n=lib.LLVMDisasmInstruction(ctx,buf,4,va,out,512);ins=out.value.decode().strip() if n else '<undecoded>'
    assert ins==x['instruction'],(x,ins)
    w=int.from_bytes(raw,'little');target=''
    if w>>26 in (5,37):
        v=w&0x3ffffff;v-=1<<26 if v&(1<<25) else 0;target=hex(va+4*v)
    elif (w&0x7e000000)==0x34000000 or (w&0xff000010)==0x54000000:
        v=(w>>5)&0x7ffff;v-=1<<19 if v&(1<<18) else 0;target=hex(va+4*v)
    assert target==x['branch_target']
    instructions.append(dict(image='ArcCore 1.170.0 arm64',address_space='unslid VA',function_va=x['function_va'],va=hex(va),file_offset=hex(b.offset(va,4)),bytes=raw.hex(),instruction=ins,branch_target=target))
write('instructions.tsv',instructions)
cross=0
for x in rows(Path('/home/exedev/work/agents/abi/decompile/binary-instructions.tsv')):
    if x['range_va'] not in ('0x43ed9a8','0x4041fec'):continue
    a=int(x['instruction_va'],16);assert b.data(a,4).hex()==x['bytes'] and b.offset(a,4)==int(x['file_offset'],16);cross+=1
accessors=rows(R/'evidence/accessor-instruction-checks.tsv')
for x in accessors:assert b.data(int(x['imp_va'],16),8).hex()==x['raw_8']
sources=rows(R/'revision-evidence/primary-sources.tsv')
for x in sources:
    assert x['commit']=='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c' and digest(Path(x['local_path']))==x['sha256']
all_sources={x['local_path']:{k:x[k] for k in ('commit','source_path','local_path','sha256')} for x in rows(V/'source-validation.tsv')+sources}
for x in all_sources.values():assert digest(Path(x['local_path']))==x['sha256']
write('source-check.tsv',list(all_sources.values()))
with (HERE/'source-excerpts.txt').open('w') as f:
    for x in sources:
        f.write(x['local_path']+' @ '+x['commit']+'\n')
        lines=Path(x['local_path']).read_text().splitlines()
        for j in range(int(x['excerpt_start']),int(x['excerpt_end'])+1):f.write(f'{j}: {lines[j-1]}\n')
        f.write('\n')
t=(R/'revision-evidence/source-excerpts.txt').read_text();path=None
for line in t.splitlines():
    if line.startswith('/home/'):
        path=Path(line.split(' @ ')[0]);lines=path.read_text().splitlines()
    elif re.match(r'^\d+: ',line):
        n,content=line.split(': ',1);assert lines[int(n)-1]==content
changed=[j for j,(a,z) in enumerate(zip(old,new),1) if a!=z];assert len(changed)==12
manifest=json.loads((R/'revision-evidence/method-annotation-changes.json').read_text())
assert len(manifest)==12
assert sorted(((x['before'],x['after']) for x in manifest),key=lambda x:key(x[1]))==sorted(((old[j-1],new[j-1]) for j in changed),key=lambda x:key(x[1]))
protected=json.loads((HERE/'protected-snapshot.json').read_text())
for x in protected:
    p=Path(x['path']);st=p.stat();assert st.st_size==x['size'] and st.st_mtime_ns==x['mtime_ns'] and digest(p)==x['sha256'],str(p)
result=dict(result='PASS',image=str(image),image_sha256=image_hash,address_identity='ArcCore 1.170.0 arm64; all engine addresses unslid VAs',classes=len(assigned),method_rows=len(new),unique_imps=len({x['imp_va'] for x in new}),binary_records=len(decoded),categories=categories,raw_dyld_rows=len(dyld),layouts=len(layouts),changed_classes=sum(x['changed'] for x in coverage),changed_method_rows=changed,original_findings=len(findings),original_frozen_hashes=len(snap),preserved_revision_evidence_files=len(preserved),protected_files_unchanged=len(protected),saved_bodies_by_snapshot=dict(collections.Counter(x['snapshot'] for x in bodychecks)),transcript_comparisons=len(transcript),fresh_read_unique_bodies=len({x['va'] for x in transcript if str(HERE) in x['transcript']}),instruction_words=len(instructions),undecoded_words=sum(x['instruction']=='<undecoded>' for x in instructions),cross_report_instruction_bytes=cross,raw_accessor_rows=len(accessors),revision_source_files=len(sources),all_source_files=len(all_sources),body_status_counts=cov['body_status_counts'],semantic_status_counts=cov['semantic_status_counts'],peak_rss_kib=resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,scope='Static inventory/provenance and selected semantic evidence only; no runtime tests or complete transitive reconstruction')
(HERE/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
