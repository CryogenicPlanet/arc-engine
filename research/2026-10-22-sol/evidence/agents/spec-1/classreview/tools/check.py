#!/usr/bin/env python3
"""Independent bounded spec-2 inventory/body/source checks. Stdlib, read-only inputs."""
import collections, csv, hashlib, html, json, re, sqlite3, struct, sys
from pathlib import Path

HERE=Path(__file__).resolve().parents[1]
SRC=HERE.parent.parent/'spec-2/specs'
IN=Path('/home/exedev/work/inputs')
bounded='--bounded-only' in sys.argv
OUT=HERE/'evidence'/('bounded' if bounded else '')
OUT.mkdir(parents=True,exist_ok=True)
OUT.mkdir(exist_ok=True)
def sha(b): return hashlib.sha256(b).hexdigest()
def digest(p):
    h=hashlib.sha256()
    with open(p,'rb') as f:
        for b in iter(lambda:f.read(1048576),b''): h.update(b)
    return h.hexdigest()
def write(name, rows, fields=None):
    rows=list(rows)
    with open(OUT/name,'w') as f:
        w=csv.DictWriter(f,fields or list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)

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

assignment=json.load(open(IN/'context/class-assignments.json'))['groups']['spec-2']
classes=assignment['classes'];assert len(classes)==37
rows=list(csv.DictReader(open(SRC/'methods.tsv'),delimiter='\t'))
key=lambda r:tuple(r[x] for x in ('class_name','category','kind','selector'))
bykey={key(r):r for r in rows};assert len(bykey)==len(rows)==491
assert set(r['class_name'] for r in rows)==set(classes)
image=Image(IN/'binaries/ArcCore-1.170.0.arm64')
actual=[];layouts={};class_names={}
for va in image.pointers('__objc_classlist'):
    ro=image.ptr(va+32)&~7;name=image.string(image.ptr(ro+24));class_names[va]=name
    if name not in classes:continue
    layouts[name]=(va,ro,*struct.unpack('<II',image.v(ro+4,8)))
    for kind,rva in [('-',ro),('+',image.ptr(image.ptr(va)+32)&~7)]:
        for r in image.methods(image.ptr(rva+32)):actual.append(dict(class_name=name,category='',kind=kind,**r))
for va in image.pointers('__objc_catlist'):
    cls=class_names.get(image.ptr(va+8))
    if cls not in classes:continue
    cat=image.string(image.ptr(va))
    for kind,off in [('-',16),('+',24)]:
        for r in image.methods(image.ptr(va+off)):actual.append(dict(class_name=cls,category=cat,kind=kind,**r))
assert len(actual)==491 and set(map(key,actual))==set(bykey)
for a in actual:
    for field in ('type_encoding','imp_va','method_record_va'):assert a[field]==bykey[key(a)][field],(a,field)
layout_checks=[]
for l in json.load(open(SRC/'evidence/layouts.json')):
    cls=l['class_name'];va,ro,start,size=layouts[cls]
    assert (int(l['metadata_va'],16),int(l['ro_va'],16),l['instance_start'],l['instance_size'])==(va,ro,start,size)
    iv=image.ptr(ro+48);ivars=[]
    if iv:
        stride,n=struct.unpack('<II',image.v(iv,8))
        for i in range(n):
            a=iv+8+stride*i
            ivars.append(dict(record_va=hex(a),offset=struct.unpack('<I',image.v(image.ptr(a),4))[0],name=image.string(image.ptr(a+8)),type=image.string(image.ptr(a+16)),alignment=struct.unpack('<I',image.v(a+24,4))[0],size=struct.unpack('<I',image.v(a+28,4))[0]))
    assert ivars==l['ivars']
    layout_checks.append(dict(class_name=cls,class_va=hex(va),ro_va=hex(ro),instance_start=start,instance_size=size,ivars=ivars))
(OUT/'layout-verification.json').write_text(json.dumps(layout_checks,indent=2)+'\n')

# Independently parse explicit dyld class/category context; no row-order pairing.
dyld=[];cls=cat=None
for lineno,line in enumerate(open(IN/'context/analysis/data/dobjc170.txt'),1):
    m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
    if m:cls,cat=m[1],m[2] or ''
    m=re.search(r'(0x[\da-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
    if m and cls in classes:
        assert m[3]==cls
        dyld.append((cls,cat,m[2],m[4],hex(int(m[1],16)),lineno))
assert collections.Counter(x[:5] for x in dyld)==collections.Counter((*key(a),a['imp_va']) for a in actual)
for d in dyld:assert int(bykey[d[:4]]['dyld_line'])==d[5]

db=sqlite3.connect('file:'+str(IN/'corpus-index.sqlite')+'?mode=ro',uri=True)
db.row_factory=sqlite3.Row
bodies={};entries={};checks=[]
snapshots=json.load(open(SRC/'evidence/methods-bodies.json'))
snap={key(r):r for r in snapshots}
for r in rows:
    va=int(r['imp_va'],16)
    e=dict(db.execute('select * from functions where va=?',(va,)).fetchone());entries[va]=e
    if va not in bodies:
        assert e['length']<200000
        with open(e['path'],'rb') as f:f.seek(e['offset']);bodies[va]=f.read(e['length'])
    b=bodies[va];s=b.decode();assert not e['failed'] and s.startswith('// ==== '+hex(va)+' ')
    assert str(e['path'])==r['body_path'] and str(e['offset'])==r['body_offset'] and str(e['length'])==r['body_length']
    assert snap[key(r)]['body']==s and not snap[key(r)]['truncated']
    warnings='; '.join(re.findall(r'/\* WARNING: (.*?)\*/',s,re.S))
    checks.append(dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],imp_va=r['imp_va'],binary_identity='match',dyld_identity='match',body_sha256=sha(b),body_name=e['name'],body_path=e['path'],body_offset=e['offset'],body_length=e['length'],header_count=e['reported_size'],warnings=' '.join(warnings.split())))
write('method-verification.tsv',checks)

# Raw otool cited lines must identify the exact type and implementation pair.
needed={int(r[k]) for r in rows for k in ('otool_type_line','otool_imp_line')};otool={}
for i,line in enumerate(open(IN/'context/analysis/data/objc170.txt'),1):
    if i in needed:otool[i]=line.strip()
for r in rows:
    assert otool[int(r['otool_type_line'])].endswith(' '+r['type_encoding'])
    # otool prints the encoded signed displacement first, then resolved VA.
    assert int(re.findall(r'0x[\da-fA-F]+',otool[int(r['otool_imp_line'])])[-1],16)==int(r['imp_va'],16)

# Stream TSV; enclosing spans are not function body membership maps.
tsv={}
if not bounded:
    for line in open(IN/'metadata/decomp/arccore-1.170.0-functions.tsv'):
        a,end,size,name=line.rstrip('\n').split('\t',3);v=int(a,16)
        if v in entries:tsv[v]=(int(end,16)-v,name,int(size))
    assert len(tsv)==len(entries)
    write('span-triage.tsv', [dict(va=hex(v),name=tsv[v][1],enclosing_span=tsv[v][0],header_addresses=entries[v]['reported_size'],different=int(tsv[v][0]!=entries[v]['reported_size'])) for v in sorted(entries)])

matrices=[];citations=[]
for cls in classes:
    text=(SRC/'classes'/f'{cls}.md').read_text(); ms=[]
    assert f'# {cls}\n' in text
    assert 'ArcCore **1.170.0 arm64**' in text
    va,ro,start,size=layouts[cls]
    assert f'metadata class `{hex(va)}`, class_ro `{hex(ro)}`' in text
    assert f'instanceStart {start}, instanceSize {size} bytes' in text
    for ln,line in enumerate(text.splitlines(),1):
        if re.match(r'\| (base|\w+) \| [-+] ',line):
            cells=line.split(' | ');cat=cells[0][2:];kind,sel=cells[1].split(' ',1)
            r=bykey[(cls,'' if cat=='base' else cat,kind,sel)]
            assert cells[2]==f"`{r['type_encoding']}`" and cells[3]==r['imp_va']
            assert r['behavior']==html.unescape(cells[4]),(cls,sel,r['behavior'],cells[4])
            ms.append(key(r))
        for a in re.findall(r'0x[\da-fA-F]+',line):
            v=int(a,16)
            if v<0x10000:continue
            sec=next((s[0] for s in image.sections if s[1]<=v<s[1]+s[2]),'unmapped')
            en=db.execute('select name,path,offset,length,failed from functions where va=?',(v,)).fetchone() if v<2**63 else None
            kind='constant' if v in (0x08000100,0x14000100,0x295e9648864000,0xffffffffffffffff) else 'function-entry' if en else 'metadata-or-data'
            assert not(sec=='__text' and not en and kind!='constant'),(cls,ln,a)
            citations.append(dict(class_name=cls,line=ln,va=hex(v),image='ArcCore-1.170.0.arm64' if kind!='constant' else '',value_kind=kind,section=sec,exact_body_name=en['name'] if en else '',body_path=en['path'] if en else '',body_offset=en['offset'] if en else '',body_length=en['length'] if en else '',failed=en['failed'] if en else '',context=line[:500]))
    assert collections.Counter(ms)==collections.Counter(key(r) for r in rows if r['class_name']==cls)
    matrices.append(dict(class_name=cls,matrix_rows=len(ms),unique_imps=len({bykey[k]['imp_va'] for k in ms}),category_counts=json.dumps(dict(collections.Counter(k[1] or 'base' for k in ms)))))
write('class-counts.tsv',matrices);write('citations.tsv',citations)

bridges=[]
for b in json.load(open(SRC/'evidence/bridges.json')):
    va=int(b['va'],16);en=db.execute('select * from functions where va=?',(va,)).fetchone()
    with open(en['path'],'rb') as f:f.seek(en['offset']);raw=f.read(en['length'])
    assert raw.decode()==b['body'] and not b['truncated'] and not en['failed']
    bridges.append(dict(va=hex(va),name=en['name'],path=en['path'],offset=en['offset'],length=en['length'],sha256=sha(raw)))
write('bridge-verification.tsv',bridges)
sources=[]
for s in csv.DictReader(open(SRC/'primary-sources.tsv'),delimiter='\t'):
    p=Path(s['local_path'])
    if not p.is_absolute():p=SRC.parent/p
    h=digest(p);assert h==s['sha256'];sources.append(dict(**s,reviewed_local_path=str(p),independent_sha256=h))
write('source-verification.tsv',sources)
cov=json.load(open(SRC/'coverage.json'))
for field,value in [('actual_classes',37),('actual_method_rows',491),('unique_metadata_identities',491),('unique_imps',453),('instance_methods',491),('class_methods',0),('body_available',491),('body_failed',0),('body_missing',0),('body_truncated',0)]:assert cov[field]==value
assert cov['semantic_status_counts']==dict(collections.Counter(r['semantic_status'] for r in rows))
assert cov['body_status_counts']==dict(collections.Counter(r['body_status'] for r in rows))
assert cov['unresolved_rows']==sum(bool(r['unresolved_question']) for r in rows)
summary=dict(classes=len(classes),method_rows=len(rows),unique_imps=len(entries),binary_dyld_method_matches=len(actual),body_snapshot_matches=len(rows),bridge_snapshot_matches=len(bridges),sources=len(sources),matrix_identity_matches=sum(x['matrix_rows'] for x in matrices),semantic_status_counts=dict(collections.Counter(r['semantic_status'] for r in rows)),body_status_counts=dict(collections.Counter(r['body_status'] for r in rows)),declared_calls_not_proven_calls=True,span_count_differences=None if bounded else sum(tsv[v][0]!=entries[v]['reported_size'] for v in entries),max_indexed_body_text=max(e['length'] for e in entries.values()),writer_coverage=cov)
(OUT/'coverage-check.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k!='writer_coverage'},indent=2))
