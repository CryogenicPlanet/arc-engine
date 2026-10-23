import csv,hashlib,json,struct,uuid,sqlite3,difflib,html,re
from pathlib import Path
from collections import Counter
OUT=Path('/home/exedev/work/agents/spec-2/revision-reviews/v1')
OLD=Path('/home/exedev/work/agents/spec-2/specs'); REV=Path('/home/exedev/work/agents/spec-2/revisions/v1'); PRIOR=Path('/home/exedev/work/agents/spec-1/classreview')
def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(65536),b''):h.update(b)
 return h.hexdigest()
roots=[OLD,REV,PRIOR]
manifest={str(p):sha(p) for root in roots for p in root.rglob('*') if p.is_file()}
base=OUT/'evidence/start-hashes.json'
if base.exists():assert json.loads(base.read_text())==manifest,'read-only artifact changed during review'
else:base.write_text(json.dumps(manifest,indent=2)+'\n')
for r in rows(PRIOR/'evidence/reviewed-spec-hashes.tsv'):assert sha(Path(r['path']))==r['sha256']
old=rows(OLD/'methods.tsv');new=rows(REV/'methods.tsv')
assigned=json.loads(Path('/home/exedev/work/inputs/context/class-assignments.json').read_text())['groups']['spec-2']
assert len(new)==len(old)==assigned['method_rows']==491
assert sorted({r['class_name'] for r in new})==sorted(assigned['classes'])
assert {p.stem for p in (REV/'classes').glob('*.md')}==set(assigned['classes'])
key=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
assert len({key(r) for r in new})==491
annotations={'body_status','behavior','evidence','confidence','unresolved_question','semantic_status'}
changed=[]
for a,b in zip(old,new):
 assert all(a[k]==b[k] for k in a if k not in annotations)
 if a!=b:changed.append({'identity':key(b),'imp_va':b['imp_va'],'changes':{k:[a[k],b[k]] for k in a if a[k]!=b[k]}})
(OUT/'evidence/annotation-diff.json').write_text(json.dumps(changed,indent=2)+'\n')
diffs=[]
for p in sorted((OLD/'classes').glob('*.md')):
 q=REV/'classes'/p.name
 diffs.extend(difflib.unified_diff(p.read_text().splitlines(True),q.read_text().splitlines(True),fromfile=str(p),tofile=str(q)))
(OUT/'evidence/class-diff.patch').write_text(''.join(diffs))
# Independently translate copied Mach-O load commands and decode method records.
f=Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64').open('rb')
def read(off,n):f.seek(off);b=f.read(n);assert len(b)==n;return b
h=read(0,32);assert struct.unpack_from('<I',h)[0]==0xfeedfacf
segments=[];sections=[];off=32
for _ in range(struct.unpack_from('<I',h,16)[0]):
 cmd,size=struct.unpack('<II',read(off,8));b=read(off,size)
 if cmd==0x19:
  name,va,vs,fo,fs=struct.unpack_from('<16sQQQQ',b,8);segments.append((va,fs,fo))
  for j in range(struct.unpack_from('<I',b,64)[0]):
   sn,sg,sv,sz,so=struct.unpack_from('<16s16sQQI',b,72+80*j);sections.append((sn.rstrip(b'\0').decode(),sv,sz))
 if cmd==0x80000034:
  fix=struct.unpack_from('<I',b,8)[0];starts=fix+struct.unpack_from('<I',read(fix,28),4)[0]
  formats=set()
  for j in range(struct.unpack('<I',read(starts,4))[0]):
   rel=struct.unpack('<I',read(starts+4+j*4,4))[0]
   if rel:formats.add(struct.unpack('<H',read(starts+rel+6,2))[0])
  assert formats=={2}
 if cmd==0x1b:uid=str(uuid.UUID(bytes=b[8:24]))
 off+=size
def vread(va,n):
 for start,size,fo in segments:
  if start<=va and va+n<=start+size:return read(fo+va-start,n)
 raise ValueError(hex(va))
def string(va):return vread(va,4096).split(b'\0')[0].decode()
def pointer(va):
 p=struct.unpack('<Q',vread(va,8))[0]
 if p>>63:return 0 # imported/bound pointers are not local class-list entries
 return (p&0xfffffffff)|(((p>>36)&255)<<56)
def listptr(name):
 for sn,va,n in sections:
  if sn==name:
   for off in range(0,n,8):yield pointer(va+off)
def methodlist(va,cls,cat,kind):
 if not va:return []
 flags,n=struct.unpack('<II',vread(va,8));assert flags&0x80000000 and flags&0xffff==12
 out=[]
 for i in range(n):
  a=va+8+i*12;sr,tr,ir=struct.unpack('<iii',vread(a,12))
  sel=a+sr if flags&0x40000000 else pointer(a+sr)
  out.append(dict(class_name=cls,category=cat,kind=kind,selector=string(sel),type_encoding=string(a+4+tr),imp_va=hex(a+8+ir),method_record_va=hex(a)))
 return out
actual=[];class_names={};layouts={}
for va in listptr('__objc_classlist'):
 ro=pointer(va+32)&~7;cls=string(pointer(ro+24));class_names[va]=cls
 if cls not in assigned['classes']:continue
 layouts[cls]=(va,ro,*struct.unpack('<II',vread(ro+4,8)))
 actual+=methodlist(pointer(ro+32),cls,'','-')
 meta=pointer(pointer(va)+32)&~7
 actual+=methodlist(pointer(meta+32),cls,'','+')
for va in listptr('__objc_catlist'):
 cls=class_names.get(pointer(va+8))
 if cls not in assigned['classes']:continue
 cat=string(pointer(va))
 actual+=methodlist(pointer(va+16),cls,cat,'-')
 actual+=methodlist(pointer(va+24),cls,cat,'+')
assert Counter(key(r) for r in actual)==Counter(key(r) for r in new)
byidentity={key(r):r for r in new}
for a in actual:
 for field in ('type_encoding','imp_va','method_record_va'):assert a[field]==byidentity[key(a)][field]
for l in json.loads((REV/'evidence/layouts.json').read_text()):
 va,ro,start,size=layouts[l['class_name']]
 assert (hex(va),hex(ro),start,size)==(l['metadata_va'],l['ro_va'],l['instance_start'],l['instance_size'])
 iv=pointer(ro+48);ivars=[]
 if iv:
  stride,n=struct.unpack('<II',vread(iv,8))
  for i in range(n):
   a=iv+8+stride*i
   ivars.append(dict(record_va=hex(a),offset=struct.unpack('<I',vread(pointer(a),4))[0],name=string(pointer(a+8)),type=string(pointer(a+16)),alignment=struct.unpack('<I',vread(a+24,4))[0],size=struct.unpack('<I',vread(a+28,4))[0]))
 assert ivars==l['ivars']
(OUT/'evidence/binary-declarations.json').write_text(json.dumps(actual,indent=2)+'\n')
for r in new:
 va=int(r['method_record_va'],16);sr,tr,ir=struct.unpack('<iii',vread(va,12))
 assert string(pointer(va+sr))==r['selector']
 assert string(va+4+tr)==r['type_encoding']
 assert va+8+ir==int(r['imp_va'],16)
# Exact original corpus/index reconciliation: bounded reads only.
db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
bodies=json.loads((REV/'evidence/methods-bodies.json').read_text());bykey={key(r):r for r in bodies}
for r in new:
 b=bykey[key(r)];name,path,pos,n,failed=db.execute('select name,path,offset,length,failed from functions where va=?',(int(r['imp_va'],16),)).fetchone()
 assert (path,pos,n,failed)==(r['body_path'],int(r['body_offset']),int(r['body_length']),0)
 assert n<32768
 with Path(path).open('rb') as inp:inp.seek(pos);raw=inp.read(n)
 assert raw.decode()==b['body']
# Matrices are complete, unique, and annotations exactly agree.
count=0
for cls in assigned['classes']:
 found=[]
 for line in (REV/'classes'/f'{cls}.md').read_text().splitlines():
  cells=[c.strip() for c in line.split('|')[1:-1]]
  if len(cells)!=8 or not cells[1].startswith(('- ','+ ')):continue
  ident=(cls,'' if cells[0]=='base' else cells[0],cells[1][0],cells[1][2:]);found.append(ident)
  r=next(x for x in new if key(x)==ident)
  assert cells[2]=='`'+r['type_encoding']+'`' and cells[3]==r['imp_va']
  assert html.unescape(cells[4])==r['behavior']
  assert cells[7]==f"{r['confidence']} / {r['body_status']} / {r['semantic_status']}"
  count+=1
 assert Counter(found)==Counter(key(r) for r in new if r['class_name']==cls)
# Original findings numbered by DATA row; no filtering/reordering.
finding=rows(PRIOR/'findings.tsv');ledger=rows(REV/'corrections.tsv')
assert len(finding)==len(ledger)==13
for i,(a,b) in enumerate(zip(finding,ledger),1):
 assert b['finding_row']==str(i) and (a['class'],a['selector'])==(b['class_name'],b['selector'])
 before=next(r for r in old if (r['class_name'],r['selector'])==(a['class'],a['selector']))
 assert a['claim'].strip() in b['before'] and 'Original annotation: '+before['behavior'] in b['before']
 m=next(r for r in new if (r['class_name'],r['selector'])==(b['class_name'],b['selector']))
 assert b['after']==m['behavior'] and b['remaining_unknown']==m['unresolved_question']
# Original raw evidence and all copied source files preserved byte for byte.
rawfiles=[]
for folder in ['evidence','sources']:
 paths=sorted(p.relative_to(OLD) for p in (OLD/folder).rglob('*') if p.is_file())
 assert paths==sorted(p.relative_to(REV) for p in (REV/folder).rglob('*') if p.is_file())
 for rel in paths:assert sha(OLD/rel)==sha(REV/rel);rawfiles.append(str(rel))
assert sha(OLD/'report.md')==sha(REV/'original-report.md')
sources=[]
for r in rows(REV/'primary-sources.tsv'):
 p=Path(r['local_path']) if r['project']=='chromium' else REV/'sources/cef'/Path(r['source_path']).name
 assert sha(p)==r['sha256'];sources.append({**r,'checked_path':str(p)})
(OUT/'evidence/source-checks.json').write_text(json.dumps(sources,indent=2)+'\n')
cov=json.loads((REV/'coverage.json').read_text())
assert cov['body_status_counts']==dict(Counter(r['body_status'] for r in new))
assert cov['semantic_status_counts']==dict(Counter(r['semantic_status'] for r in new))
assert cov['unresolved_rows']==sum(bool(r['unresolved_question']) for r in new)
result=dict(status='PASS',classes=len(assigned['classes']),declarations=len(new),unique_imps=len({r['imp_va'] for r in new}),matrix_rows=count,original_finding_rows=len(finding),changed_annotations=len(changed),changed_classes=len({x['identity'][0] for x in changed}),original_body_copies_verified=len(new),raw_evidence_sources_preserved=len(rawfiles),primary_source_hashes=len(sources),original_review_recorded_hashes=39,image='ArcCore-1.170.0.arm64',LC_UUID=uid,address_basis='unslid Mach-O VA; segment-translated file reads',body_status_counts=cov['body_status_counts'],semantic_status_counts=cov['semantic_status_counts'],historical_and_revision_files_unchanged_since_start=len(manifest),limits='Structural/provenance checks, not semantic or ABI certification. No full function-TSV scan.')
(OUT/'evidence/validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
