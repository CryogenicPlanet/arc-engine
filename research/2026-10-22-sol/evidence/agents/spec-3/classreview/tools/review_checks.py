#!/usr/bin/env python3
"""Independent, read-only class inventory/body/provenance checks. Stdlib only."""
import csv, collections, hashlib, json, re, sqlite3, struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]; OUT=ROOT/'classreview'; W=ROOT.parent/'spec-4/specs'; I=Path('/home/exedev/work/inputs')
def rows(p): return list(csv.DictReader(open(p),delimiter='\t'))
def save(name,fields,data):
 with open(OUT/name,'w') as f:
  w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(data)
def digest(p):
 h=hashlib.sha256()
 with open(p,'rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
class Image:
 def __init__(self,p):
  self.f=open(p,'rb');self.segments=[];self.vm_ranges=[];self.sections={};assert self.read(0,4)==b'\xcf\xfa\xed\xfe'
  n=struct.unpack('<I',self.read(16,4))[0];pos=32
  for _ in range(n):
   cmd,size=struct.unpack('<II',self.read(pos,8));b=self.read(pos,size)
   if cmd==0x19:
    va,vs,off,fs=struct.unpack_from('<QQQQ',b,24);self.segments.append((va,fs,off));self.vm_ranges.append((va,vs))
    for j in range(struct.unpack_from('<I',b,64)[0]):
     x=72+j*80;name=b[x:x+16].split(b'\0')[0].decode();a,sz,fo=struct.unpack_from('<QQI',b,x+32);self.sections[name]=(a,sz,fo)
   pos+=size
 def read(self,o,n):self.f.seek(o);b=self.f.read(n);assert len(b)==n;return b
 def off(self,a):
  for va,fs,o in self.segments:
   if va<=a<va+fs:return o+a-va
  raise ValueError(hex(a))
 def v(self,a,n):return self.read(self.off(a),n)
 def ptr(self,a):
  x=struct.unpack('<Q',self.v(a,8))[0]
  assert not x>>63,('unexpected bind',hex(a))
  return (x&((1<<36)-1))|((x>>36&255)<<56)
 def string(self,a):return self.v(a,4096).split(b'\0')[0].decode()
 def array(self,s):
  a,n,o=self.sections[s]
  return [self.ptr(a+j) for j in range(0,n,8)]
 def name(self,a):return self.string(self.ptr((self.ptr(a+32)&~7)+24))
 def methods(self,a):
  if not a:return []
  flags,n=struct.unpack('<II',self.v(a,8));stride=flags&65535;out=[]
  for j in range(n):
   r=a+8+j*stride
   if flags&0x80000000:
    sn,ty,im=struct.unpack('<iii',self.v(r,12));s=r+sn
    if not flags&0x40000000:s=self.ptr(s)
    ty=r+4+ty;im=r+8+im
   else:s,ty,im=[self.ptr(r+k) for k in (0,8,16)]
   out.append((self.string(s),self.string(ty),hex(im),hex(r)))
  return out
image=Image(I/'binaries/ArcCore-1.170.0.arm64');actual=[];class_layout={};ivars={};category_vas={}
for a in image.array('__objc_classlist'):
 name=image.name(a);ro=image.ptr(a+32)&~7
 if not name.startswith('Arc'):continue
 class_layout[name]=dict(class_va=hex(a),ro_va=hex(ro),instance_start=struct.unpack('<I',image.v(ro+4,4))[0],instance_size=struct.unpack('<I',image.v(ro+8,4))[0])
 ivars[name]=[];iv=image.ptr(ro+48)
 if iv:
  stride,num=struct.unpack('<II',image.v(iv,8))
  for j in range(num):
   rec=iv+8+j*stride;offset=struct.unpack('<I',image.v(image.ptr(rec),4))[0]
   ivars[name].append((image.string(image.ptr(rec+8)),str(offset),str(struct.unpack('<I',image.v(rec+28,4))[0]),image.string(image.ptr(rec+16))))
 for c,k in [(a,'-'),(image.ptr(a),'+')]:
  r=image.ptr(c+32)&~7
  for sel,ty,imp,record in image.methods(image.ptr(r+32)):actual.append((name,'',k,sel,ty,imp,record))
for a in image.array('__objc_catlist'):
 try:name=image.name(image.ptr(a+8))
 except (AssertionError,ValueError):continue
 if not name.startswith('Arc'):continue
 cat=image.string(image.ptr(a))
 category_vas[hex(a)]=(name,cat)
 for off,k in [(16,'-'),(24,'+')]:
  for sel,ty,imp,record in image.methods(image.ptr(a+off)):actual.append((name,cat,k,sel,ty,imp,record))
methods=rows(W/'methods.tsv');classes=set(json.load(open(I/'context/class-assignments.json'))['groups']['spec-4']['classes']);assert len(classes)==38
key=lambda r:tuple(r[x] for x in ('class_name','category','kind','selector','type_encoding','imp_va'))
expected=[x for x in actual if x[0] in classes]
assert collections.Counter(x[:6] for x in expected)==collections.Counter(map(key,methods))
assert all(n==1 for n in collections.Counter(x[:4] for x in expected).values())
assert set(p.stem for p in (W/'classes').glob('*.md'))==classes
# Independently parse complete raw dyld declaration identity; no selector/type row pairing.
raw=[];raw_lines={};owner=cat=''
for ln,line in enumerate(open(I/'context/analysis/data/dobjc170.txt'),1):
 m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
 if m:owner,cat=m[1],m[2] or ''
 m=re.search(r'(0x[0-9a-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
 if m and m[3] in classes:
  assert owner==m[3];identity=(m[3],cat,m[2],m[4],hex(int(m[1],16)));raw.append(identity);raw_lines[identity]=ln
assert collections.Counter(raw)==collections.Counter((x[0],x[1],x[2],x[3],x[5]) for x in expected)
quality=rows(ROOT.parent/'quality/initial/method-coverage.tsv')
assert collections.Counter((r['class'],r['category'],r['kind'],r['selector'],r['engine170_va']) for r in quality if r['spec_group']=='spec-4')==collections.Counter(raw)
# Reconcile raw otool types/IMPs by explicit owner, category and kind; selectors
# come from binary records above, never ordinal pairing between the dumps.
lines=(I/'context/analysis/data/objc170.txt').read_text().splitlines();starts=[]
for j,line in enumerate(lines):
 m=re.match(r'^([0-9a-f]{16}) (0x[0-9a-f]+)(.*)$',line)
 if m:starts.append((j,m))
otool=[];seen=set()
for z,(j,h) in enumerate(starts):
 end=starts[z+1][0] if z+1<len(starts) else len(lines);m=re.search(r'_OBJC_CLASS_\$_(\w+)',h[3]);c=m[1] if m else '';cat='';is_cat=h[2] in category_vas
 if is_cat:c,cat=category_vas[h[2]]
 elif c in classes and any(x.startswith('        instanceSize') for x in lines[j:end]) and c not in seen:seen.add(c)
 else:continue
 if c not in classes:continue
 kind='-';active=False;ty=None
 for line in lines[j+1:end]:
  if is_cat:
   if re.match(r'^    instanceMethods ',line):kind='-';active=True;continue
   if re.match(r'^    classMethods ',line):kind='+';active=True;continue
   if active and re.match(r'^    (protocols|instanceProperties) ',line):active=False
  else:
   if line=='Meta Class':kind='+';active=False
   if re.match(r'^        baseMethods ',line):active=True;continue
   if active and re.match(r'^        baseProtocols ',line):active=False
  if not active:continue
  m=re.search(r'\btypes\s+0x[0-9a-f]+\s+\(0x[0-9a-f]+\) (.*)$',line)
  if m:ty=m[1]
  m=re.search(r'\bimp\s+0x[0-9a-f]+\s+\((0x[0-9a-f]+)\)',line)
  if m:assert ty is not None;otool.append((c,cat,kind,ty,m[1]));ty=None
assert collections.Counter(otool)==collections.Counter((x[0],x[1],x[2],x[4],x[5]) for x in expected)
records={x[:6]:x[6] for x in expected}
for r in methods:
 machine=json.loads(r['machine_record']);assert machine['method_record_va']==records[key(r)]
 assert machine['imp_file_offset']==hex(image.off(int(r['imp_va'],16)))
 assert machine['dyld_line']==raw_lines[(r['class_name'],r['category'],r['kind'],r['selector'],r['imp_va'])]
db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True);body_evidence=[]
indices={**json.load(open(W/'evidence/body-index.json')),**json.load(open(W/'evidence/bridges.json'))}
for va in indices:
 name,path,offset,length,failed=db.execute('select name,path,offset,length,failed from functions where va=?',(int(va,16),)).fetchone();assert not failed
 with open(path,'rb') as f:f.seek(offset);body=f.read(length)
 assert body==(W/'bodies'/f'{int(va,16):x}.c').read_bytes()
 body_evidence.append(dict(va=va,name=name,path=path,offset=offset,length=length,sha256=hashlib.sha256(body).hexdigest()))
save('body-validation.tsv',list(body_evidence[0]),body_evidence)
matrix=[];references=[];writer_layout=json.load(open(W/'evidence/layout.json'))
for c in sorted(classes):
 doc=(W/'classes'/f'{c}.md').read_text();pr=[]
 size=re.search(r'instanceStart (\d+), instanceSize (\d+) bytes',doc);assert (int(size[1]),int(size[2]))==(class_layout[c]['instance_start'],class_layout[c]['instance_size'])
 displayed_ivars=[]
 for line in doc.splitlines():
  if not line.startswith('| '):continue
  p=[x.strip() for x in line.split('|')[1:-1]]
  if len(p)==4 and p[0].startswith('`') and p[1].isdigit():displayed_ivars.append((p[0].strip('`'),p[1],p[2],p[3].strip('`')))
  if len(p)!=8 or not re.fullmatch(r'`[-+].+`',p[1]):continue
  kind,sel=p[1][1],p[1][2:-1];cat=p[0] if p[0]!='(base)' else '';imp=re.search(r'\[(0x[0-9a-f]+)\]',p[3])[1];ty=p[2].strip('`')
  pr.append((c,cat,kind,sel,ty,imp))
  assert f"M `{records[pr[-1]]}`" in p[6]
  assert f"D {raw_lines[(c,cat,kind,sel,imp)]}" in p[6]
 want=[key(r) for r in methods if r['class_name']==c];assert collections.Counter(pr)==collections.Counter(want),(c,pr,want)
 full_ivars=[(x['name'],str(x['offset']),str(x['size']),x['type']) for x in writer_layout[c]['ivars']]
 assert collections.Counter(full_ivars)==collections.Counter(ivars[c])
 assert collections.Counter(x[:3] for x in displayed_ivars)==collections.Counter(x[:3] for x in ivars[c])
 types={x[0]:x[3] for x in ivars[c]}
 for name,off,sz,ty in displayed_ivars:
  if '… (full encoding in layout.json)' in ty:assert types[name].startswith(ty.split('…')[0])
  else:assert ty==types[name],(c,name,ty,types[name])
 for a in set(re.findall(r'0x[0-9a-fA-F]{5,}',doc)):
  n=int(a,16)
  if n>=0x100000000:status='app_VA_separate_image'
  elif a=='0x3f800000' and 'self+0x58=0x3f800000' in doc:status='scalar_literal_not_VA'
  else:
   found=db.execute('select name from functions where va=?',(n,)).fetchone()
   try:mapped=hex(image.off(n))
   except ValueError:mapped='unmapped'
   virtual=any(v<=n<v+sz for v,sz in image.vm_ranges)
   status='indexed_function' if found else ('mapped_nonentry_or_metadata' if mapped!='unmapped' else ('virtual_zero_fill' if virtual else 'UNMAPPED'))
  references.append(dict(class_name=c,va=a,status=status))
 matrix.append(dict(class_name=c,method_rows=len(want),unique_imps=len(set(x[-1] for x in want)),binary_identity_match=True,dyld_match=True,matrix_exact_once=True,**class_layout[c]))
save('coverage-checks.tsv',list(matrix[0]),matrix);save('citation-address-checks.tsv',['class_name','va','status'],references)
sources=[]
for r in rows(W/'evidence/primary-sources.tsv'):
 h=digest(Path(r['local_path']));assert h==r['sha256'];sources.append({**r,'verified':True})
for p in ['base/values.h','base/values.cc']:
 sources.append(dict(commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',source_path=p,local_path=str(I/'chromium-156'/p),sha256=digest(I/'chromium-156'/p),verified=True))
save('source-validation.tsv',list(sources[0]),sources)
loads=[]
for r in methods:
 b=image.v(int(r['imp_va'],16),8);w,ret=struct.unpack('<II',b)
 if ret==0xd65f03c0 and w&0x3fc003ff==0x39400000:
  size=1<<(w>>30);offset=((w>>10)&4095)*size
  loads.append(dict(class_name=r['class_name'],category=r['category'],selector=r['selector'],imp_va=r['imp_va'],raw_8=b.hex(),load_width=size,self_offset=offset))
  m=re.search(r'Loads (\d+) byte\(s\) from self\+(0x[0-9a-f]+)',r['behavior'])
  if m:assert (size,offset)==(int(m[1]),int(m[2],16))
save('raw-accessor-checks.tsv',list(loads[0]),loads)
app=Image(I/'binaries/Arc-1.168.0.arm64');call=0x1004ad718;word=struct.unpack('<I',app.v(call,4))[0];assert word>>26==0x25
imm=word&0x3ffffff;imm-=0x4000000 if imm&0x2000000 else 0;target=call+4*imm;assert target==0x1035beda0
assert any(l.startswith('0x1004ad314\t') for l in open(I/'app168/ghidra/functions.tsv'))
(OUT/'app-call-check.json').write_text(json.dumps(dict(image='Arc 1.168.0 arm64 app',call_va=hex(call),call_file_offset=hex(app.off(call)),word=hex(word),target=hex(target),function_entry='0x1004ad314',limit='BL target and app function metadata only; selector stub name uses existing writer/app context and receiver class remains an inference.'),indent=2))
closure=[l.rstrip('\n').split('\t') for l in open(I/'claude-handoff/results/arc-layer-functions.tsv')];names={r[0] for r in closure}
closure_hits={g:sum(v['name'] in names for v in json.load(open(W/'evidence'/p)).values()) for g,p in [('assigned_imp','body-index.json'),('selected_bridge','bridges.json')]}
(OUT/'closure-check.json').write_text(json.dumps(dict(rows=len(closure),exact_name_hits=closure_hits,limit='Name-keyed two-level closure only; second column is depth; absent names do not imply absent bodies or exclusive ownership.'),indent=2))
imps=set(r['imp_va'] for r in methods);metadata=[]
for line in open(I/'metadata/decomp/arccore-1.170.0-functions.tsv'):
 p=line.rstrip('\n').split('\t',3)
 if p[0] in imps:
  a,b,size,name=p;body=(W/'bodies'/f'{int(a,16):x}.c').read_text();m=re.search(r'\((\d+) bytes\)',body)
  metadata.append(dict(va=a,end=b,enclosing_span=size,name=name,header_count=m[1] if m else '',different=bool(m and int(m[1])!=int(size))))
assert len(metadata)==len(imps);save('function-metadata.tsv',list(metadata[0]),metadata)
cov=json.load(open(W/'coverage.json'))
for field in ['body_status','semantic_status']:
 assert dict(collections.Counter(r[field] for r in methods))==cov[field+'_counts']
summary=dict(classes=len(classes),method_rows=len(methods),unique_imps=len(imps),exact_body_byte_checks=len(body_evidence),source_hash_checks=len(sources),method_identity_binary_and_dyld=True,otool_type_imp_rows=len(otool),all_layout_and_ivar_tables_match=True,all_matrix_identities_exactly_once=True,raw_simple_accessor_rows=len(loads),app_bl_target_verified=True,reported_span_header_differences=sum(r['different'] for r in metadata),unmapped_citations=[x for x in references if x['status']=='UNMAPPED'],body_status_counts=cov['body_status_counts'],semantic_status_counts=cov['semantic_status_counts'],limits='Provenance and inventory checks are exhaustive; behavioral review scope is separately recorded in report.md and semantic-review.tsv.')
(OUT/'validation.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
