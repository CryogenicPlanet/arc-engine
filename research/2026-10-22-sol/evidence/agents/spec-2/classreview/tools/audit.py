#!/usr/bin/env python3
"""Bounded read-only class-spec audit. Writes only this role's classreview/."""
import collections,csv,hashlib,json,re,sqlite3,struct
from pathlib import Path
ROOT=Path('/home/exedev/work/inputs'); W=Path('../spec-3/specs'); O=Path('classreview')
def table(path,rows,fields=None):
 rows=list(rows); fields=fields or list(rows[0])
 with path.open('w') as f:
  w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(rows)
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
class Image:
 def __init__(self,p):
  self.f=p.open('rb');self.segs=[];self.secs={};self.formats=set()
  hdr=self.read(0,32);assert struct.unpack_from('<I',hdr)[0]==0xfeedfacf
  pos=32
  for _ in range(struct.unpack_from('<I',hdr,16)[0]):
   cmd,n=struct.unpack('<II',self.read(pos,8));b=self.read(pos,n)
   if cmd==0x19:
    va,vs,fo,fs=struct.unpack_from('<QQQQ',b,24);self.segs.append((va,fs,fo))
    for j in range(struct.unpack_from('<I',b,64)[0]):
     z=72+j*80;name=b[z:z+16].split(b'\0')[0].decode();a,size=struct.unpack_from('<QQ',b,z+32);self.secs[name]=(a,size)
   if cmd==0x80000034:
    fixoff=struct.unpack_from('<I',b,8)[0];st=struct.unpack_from('<I',self.read(fixoff,28),4)[0];base=fixoff+st
    count=struct.unpack('<I',self.read(base,4))[0]
    for z in range(count):
     rel=struct.unpack('<I',self.read(base+4+z*4,4))[0]
     if rel:self.formats.add(struct.unpack('<H',self.read(base+rel+6,2))[0])
   pos+=n
  assert self.formats in ({2},{6}),self.formats
  self.imagebase=min(va for va,fs,fo in self.segs if fs)
 def read(self,o,n):
  self.f.seek(o);b=self.f.read(n);assert len(b)==n;return b
 def off(self,a):
  for va,fs,fo in self.segs:
   if va<=a<va+fs:return fo+a-va
  raise ValueError(hex(a))
 def data(self,a,n):return self.read(self.off(a),n)
 def ptr(self,a):
  x=struct.unpack('<Q',self.data(a,8))[0]
  if not x or x>>63:return 0
  target=x&0xfffffffff
  if self.formats=={6}:target+=self.imagebase
  return target|(((x>>36)&255)<<56)
 def string(self,a):return self.data(a,8192).split(b'\0')[0].decode()
 def ptrs(self,sec):
  a,n=self.secs[sec];return [self.ptr(x) for x in range(a,a+n,8)]
 def name(self,a):return self.string(self.ptr((self.ptr(a+32)&~7)+24))
 def methods(self,a,cl,cat,kind):
  if not a:return []
  flags,n=struct.unpack('<II',self.data(a,8));stride=flags&65535;out=[]
  for z in range(n):
   p=a+8+z*stride
   if flags&0x80000000:
    nm,ty,im=struct.unpack('<iii',self.data(p,12));s=p+nm
    if not flags&0x40000000:s=self.ptr(s)
    t=p+4+ty;imp=p+8+im
   else:s,t,imp=[self.ptr(p+x) for x in (0,8,16)]
   out.append(dict(class_name=cl,category=cat,kind=kind,selector=self.string(s),type_encoding=self.string(t),imp_va=hex(imp),record_va=hex(p)))
  return out
im=Image(ROOT/'binaries/ArcCore-1.170.0.arm64');assert im.formats=={2}
assignment=json.loads((ROOT/'context/class-assignments.json').read_text())['groups']['spec-3'];classes=set(assignment['classes'])
rows=list(csv.DictReader((W/'methods.tsv').open(),delimiter='\t'));key=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
actual=[];classvas={}
for a in im.ptrs('__objc_classlist'):
 name=im.name(a)
 if name not in classes:continue
 classvas[name]=a
 for c,k in ((a,'-'),(im.ptr(a),'+')):
  ro=im.ptr(c+32)&~7;actual+=im.methods(im.ptr(ro+32),name,'',k)
for a in im.ptrs('__objc_catlist'):
 c=im.ptr(a+8)
 if not c:continue
 name=im.name(c)
 if name in classes:
  cat=im.string(im.ptr(a))
  for off,k in ((16,'-'),(24,'+')):actual+=im.methods(im.ptr(a+off),name,cat,k)
assert len(actual)==491 and set(classvas)==classes
assert collections.Counter(map(key,rows))==collections.Counter(map(key,actual))
assert len(set(map(key,rows)))==len(rows)==491
bykey={key(r):r for r in actual}
for r in rows:
 a=bykey[key(r)];assert (r['imp_va'],r['type_encoding'])==(a['imp_va'],a['type_encoding'])
# Independent dyld class/category identities: no selector/type row-order pairing.
dyld=[];cl=cat=''
for line in (ROOT/'context/analysis/data/dobjc170.txt').open():
 m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
 if m:cl,cat=m[1],m[2] or ''
 m=re.search(r'(0x[\da-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
 if m and cl in classes:
  assert cl==m[3];dyld.append((cl,cat,m[2],m[4],hex(int(m[1],16))))
assert set(dyld)=={(*key(r),r['imp_va']) for r in rows}
db=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
bodyrows=[];bodies={}
for va in sorted({int(r['imp_va'],16) for r in rows}):
 b=db.execute('select name,path,offset,length,failed,reported_size from functions where va=?',(va,)).fetchone();assert b and not b[4]
 name,path,off,n,failed,size=b
 with open(path,'rb') as f:f.seek(off);data=f.read(n)
 assert re.match(rb'// ==== 0x'+f'{va:x}'.encode()+rb'\s',data)
 snap=(W/'bodies'/f'{va:08x}.txt').read_bytes();assert data in snap
 (O/'bodies'/f'{va:08x}.c').write_bytes(data);bodies[va]=data.decode()
 bodyrows.append(dict(va=hex(va),name=name,path=path,byte_offset=off,byte_length=n,sha256=hashlib.sha256(data).hexdigest(),header_address_count=size,file_offset=hex(im.off(va)),warnings=' | '.join(re.findall(r'/\* WARNING:.*?\*/',data.decode(),re.S))))
# Function metadata streamed; retain only assigned exact starts.
meta={}
for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
 start,end,n,name=line.rstrip('\n').split('\t',3);a=int(start,16)
 if a in bodies:meta[a]=(int(end,16),int(n),name)
assert len(meta)==len(bodies)
for b in bodyrows:
 end,span,name=meta[int(b['va'],16)];b.update(tsv_end=hex(end),tsv_enclosing_span=span,tsv_name=name)
table(O/'evidence/body-checks.tsv',bodyrows)
checks=[]
for cl in sorted(classes):
 rr=[r for r in rows if r['class_name']==cl];text=(W/'classes'/f'{cl}.md').read_text();matrix=[]
 for line in text.splitlines():
  m=re.match(r'\| ([^|]+) \| `([-+])([^`]+)` \| `([^`]+)` \| \[(0x[\da-f]+)\]',line)
  if m:matrix.append((cl,'' if m[1]=='(base)' else m[1],m[2],m[3],m[4],m[5]))
 assert collections.Counter(matrix)==collections.Counter((*key(r),r['type_encoding'],r['imp_va']) for r in rr),cl
 checks.append(dict(class_name=cl,class_metadata_va=hex(classvas[cl]),declarations=len(rr),distinct_imps=len({r['imp_va'] for r in rr}),category_rows=sum(bool(r['category']) for r in rr),matrix_rows=len(matrix),result='PASS exact binary/dyld/methods/matrix reconciliation',spec_sha256=sha(W/'classes'/f'{cl}.md')))
table(O/'evidence/class-coverage.tsv',checks)
table(O/'evidence/decoded-methods.tsv',actual)
# Pin verification plus exact line windows; source comparisons do not name anonymous binary functions.
sources=json.loads((W/'evidence/sources.json').read_text());sourcechecks=[]
with (O/'evidence/source-excerpts.txt').open('w') as f:
 for s in sources:
  p=Path(s['local_path']);p=p if p.is_absolute() else W.parent/p
  digest=sha(p);assert digest==s['sha256']
  sourcechecks.append(dict(repository=s['repository'],commit=s['commit'],file=s['file'],local_path=str(p.resolve()),sha256=digest))
  lines=p.read_text().splitlines();f.write(f"\n{s['repository']} {s['commit']} {s['file']}\n")
  for lo,hi in s['line_windows']:
   for i in range(lo-1,hi):f.write(f'{i+1}: {lines[i]}\n')
table(O/'evidence/source-checks.tsv',sourcechecks)
leafchecks=[]
for r in json.loads((W/'evidence/arm64-leaves.json').read_text()):
 a=int(r['imp'],16);words=[f'{x:08x}' for x in struct.unpack('<II',im.data(a,8))];assert words==r['words']
 leafchecks.append(dict(class_name=r['class_name'],selector=r['selector'],va=hex(a),translated_file_offset=hex(im.off(a)),words=' '.join(words),writer_decode=r['decoded_whitelist']))
table(O/'evidence/leaf-instructions.tsv',leafchecks)
coverage=json.loads((W/'coverage.json').read_text());assert coverage['assigned_classes']==len(classes) and coverage['declared_method_rows']==len(rows) and coverage['distinct_imps']==len(bodies)
summary=dict(classes=len(classes),declarations=len(rows),distinct_imps=len(bodies),method_kinds=dict(collections.Counter(r['kind'] for r in rows)),categories=dict(collections.Counter(r['category'] or '(base)' for r in rows)),all_bodies_exact=True,all_matrix_identities_exact=True,body_bytes_read=sum(b['byte_length'] for b in bodyrows),sources_verified=len(sourcechecks),leaf_instruction_rows=len(leafchecks),runtime_verification=False)
(O/'evidence/coverage.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
