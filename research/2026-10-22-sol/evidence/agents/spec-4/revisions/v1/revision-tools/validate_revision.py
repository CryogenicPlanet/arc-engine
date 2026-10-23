#!/usr/bin/env python3
"""Read-only input checks; all output confined to this revision. No old runner."""
import collections,csv,hashlib,json,re,sqlite3,struct,sys
from pathlib import Path
R=Path(__file__).resolve().parents[1]
assert R==Path('/home/exedev/work/agents/spec-4/revisions/v1')
O=Path('/home/exedev/work/agents/spec-4/specs');V=Path('/home/exedev/work/agents/spec-3/classreview');I=Path('/home/exedev/work/inputs')
sys.path.insert(0,str(R/'scripts'))
from metadata_reader import MachO

def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
# Confirm the frozen writer baseline still matches the completed review.
snapshot=rows(V/'reviewed-spec-snapshot.tsv')
for x in snapshot:assert sha(Path(x['path']))==x['sha256']
old=rows(O/'methods.tsv');new=rows(R/'methods.tsv');decl=rows(R/'evidence/declared-methods.tsv')
identity=['class_name','category','kind','selector','type_encoding','imp_va']
key=lambda r:tuple(r[k] for k in identity)
assert len(new)==491 and [key(r) for r in new]==[key(r) for r in old]
assert len(set(tuple(r[k] for k in identity[:4]) for r in new))==491
for a,b in zip(old,new):assert a['machine_record']==b['machine_record']
assert [tuple(r[k] for k in identity[:5])+ (r['imp_va'],) for r in new]==[tuple(r[k] for k in identity[:5])+ (r['imp'],) for r in decl]
assigned=set(json.loads((I/'context/class-assignments.json').read_text())['groups']['spec-4']['classes'])
assert len(assigned)==38 and {p.stem for p in (R/'classes').glob('*.md')}==assigned
cov=json.loads((R/'coverage.json').read_text());oldcov=json.loads((O/'coverage.json').read_text())
assert cov['assigned_class_count']==cov['actual_class_specs']==38
assert cov['assigned_method_rows']==cov['actual_method_rows']==cov['unique_method_identity_count']==491
assert cov['unique_imp_count']==len({r['imp_va'] for r in new})==418
assert cov['category_overrides']==oldcov['category_overrides'] and cov['by_class']==oldcov['by_class']
for field,col in [('body_status_counts','body_status'),('semantic_status_counts','semantic_status')]:assert cov[field]==dict(collections.Counter(r[col] for r in new))
assert cov['body_status_counts']=={'available':482,'available_prototype_malformed':4,'available_pseudocode_incorrect_raw_verified':4,'available_semantics_suspect':1}
# Preserved snapshots, checked byte-for-byte with streaming hashes.
preserved=[]
for d in ['bodies','evidence','sources','scripts']:
 op={p.relative_to(O) for p in (O/d).rglob('*') if p.is_file() and not (d=='scripts' and '__pycache__' in p.parts)};rp={p.relative_to(R) for p in (R/d).rglob('*') if p.is_file()}
 assert op==rp,(d,'file sets differ')
 for rel in sorted(op):
  a=sha(O/rel);assert sha(R/rel)==a,str(rel);preserved.append(dict(path=str(rel),sha256=a))
assert (R/'original-report.md').read_bytes()==(O/'report.md').read_bytes()
# Current method matrices are exact multisets, with matching annotation text.
for cls in assigned:
 doc=(R/'classes'/f'{cls}.md').read_text();matrix=[]
 for line in doc.splitlines():
  if not line.startswith('| ') or '| M `' not in line:continue
  p=[x.strip() for x in line.split('|')[1:-1]]
  assert len(p)==8,(cls,p)
  kindsel=p[1].strip('`');enc=p[2].strip('`');imp=re.match(r'\[(0x[0-9a-f]+)\]',p[3])[1]
  matrix.append(('' if p[0]=='(base)' else p[0],kindsel[0],kindsel[1:],enc,imp,p[4]))
 expected=[(r['category'],r['kind'],r['selector'],r['type_encoding'],r['imp_va'],r['behavior']) for r in new if r['class_name']==cls]
 assert collections.Counter(matrix)==collections.Counter(expected),(cls,'matrix mismatch')
 for r in (x for x in new if x['class_name']==cls):
  line=next(x for x in doc.splitlines() if f"`{r['kind']}{r['selector']}`" in x and f"[{r['imp_va']}]" in x and f"| {r['category'] or '(base)'} |" in x)
  assert f"; {r['body_status']}; {r['semantic_status']} |" in line
 for link in re.findall(r'\]\(([^)#]+)(?:#[^)]*)?\)',doc):
  if not re.match(r'\w+://',link):assert ((R/'classes')/link).exists(),(cls,link)
# Fresh selected class/metaclass/category lists from copied Mach-O; no full corpus scan.
im=MachO(I/'binaries/ArcCore-1.170.0.arm64');layout=json.loads((R/'evidence/layout.json').read_text());binary=[]
for cls,v in layout.items():
 a=int(v['metadata_va'],16);ro=im.ptr(a+32)&~7
 assert im.string(im.ptr(ro+24))==cls
 assert struct.unpack('<II',im.vread(ro+4,8))==(v['instance_start'],v['instance_size'])
 for iv in v['ivars']:
  rec=int(iv['record_va'],16)
  assert im.string(im.ptr(rec+8))==iv['name'] and im.string(im.ptr(rec+16))==iv['type']
  assert struct.unpack('<I',im.vread(im.ptr(rec),4))[0]==iv['offset']
  assert struct.unpack('<I',im.vread(rec+28,4))[0]==iv['size']
 for a,k in [(a,'-'),(im.ptr(a),'+')]:
  ro=im.ptr(a+32)&~7
  for m in im.method_list(im.ptr(ro+32)):binary.append((cls,'',k,m))
for v in cov['metadata_reconciliation']['categories']:
 a=int(v['metadata_va'],16);cls=v['class_name'];cat=im.string(im.ptr(a))
 assert cat==v['category'];cl=im.ptr(a+8);ro=im.ptr(cl+32)&~7;assert im.string(im.ptr(ro+24))==cls
 for off,k in [(16,'-'),(24,'+')]:
  for m in im.method_list(im.ptr(a+off)):binary.append((cls,cat,k,m))
assert collections.Counter((c,ct,k,m['selector'],m['type_encoding'],m['imp']) for c,ct,k,m in binary)==collections.Counter(map(key,new))
lookup={(c,ct,k,m['selector']):m for c,ct,k,m in binary}
for r in new:
 m=lookup[tuple(r[k] for k in identity[:4])];mr=json.loads(r['machine_record'])
 assert mr['method_record_va']==m['method_record_va']
 assert int(mr['imp_file_offset'],16)==im.offset(int(r['imp_va'],16))
# Exact corpus comparisons, using indexed reads only.
db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
index={**json.loads((R/'evidence/body-index.json').read_text()),**json.loads((R/'evidence/bridges.json').read_text())}
for a in index:
 name,path,off,n,failed=db.execute('select name,path,offset,length,failed from functions where va=?',(int(a,16),)).fetchone()
 assert not failed and n<=32768
 with open(path,'rb') as f:f.seek(off);data=f.read(n)
 assert data==(R/'bodies'/f'{int(a,16):x}.c').read_bytes()
# Verify newly retained read-function output matches exact index bytes/provenance.
transcript_bodies=[]
for file in ['rechecked-original-bodies.txt','nearby-original-bodies.txt','boostide-boundary-bodies.txt']:
 text=(R/'revision-evidence'/file).read_text();matches=list(re.finditer(r'^INDEX (0x[0-9a-f]+) (.*?) path=(.*?) offset=(\d+) C_bytes=(\d+) failed=(\d+)\n',text,re.M))
 assert matches and 'TRUNCATED:' not in text
 for j,m in enumerate(matches):
  a,name,path,off,n,failed=m.groups();assert failed=='0'
  rec=db.execute('select name,path,offset,length,failed from functions where va=?',(int(a,16),)).fetchone();assert rec==(name,path,int(off),int(n),0)
  with open(path,'rb') as f:f.seek(int(off));data=f.read(int(n))
  end=matches[j+1].start() if j+1<len(matches) else len(text)
  assert text[m.end():end]==data.decode()+'\n'
  transcript_bodies.append(a)
# Instruction and selected-source provenance checks.
instructions=rows(R/'revision-evidence/instructions.tsv')
for x in instructions:
 a=int(x['va'],16);assert x['image']=='ArcCore 1.170.0 arm64' and x['address_space']=='unslid VA'
 assert im.offset(a)==int(x['file_offset'],16) and im.vread(a,4).hex()==x['bytes']
for x in rows(R/'revision-evidence/primary-sources.tsv'):assert sha(Path(x['local_path']))==x['sha256']
assert sha(I/'binaries/ArcCore-1.170.0.arm64')==cov['metadata_reconciliation']['inputs_sha256'][str(I/'binaries/ArcCore-1.170.0.arm64')]
# Every original finding has exactly one row, identified by DATA ROW, without sorting/dedup.
finding=rows(V/'findings.tsv');correction=rows(R/'corrections.tsv')
assert len(finding)==len(correction)==9
assert list(correction[0])==['finding_row','class_name','selector','disposition','before','after','evidence','remaining_unknown']
for i,(f,c) in enumerate(zip(finding,correction),1):
 assert c['finding_row']==str(i) and c['class_name']==f['class'] and c['selector']==f['selector']
 assert c['before']==f['claim'] and c['disposition']=='corrected' and all(c.values())
 assert (R/'classes'/f"{c['class_name']}.md").read_bytes()!=(O/'classes'/f"{c['class_name']}.md").read_bytes()
changed_classes=sorted(p.stem for p in (R/'classes').glob('*.md') if p.read_bytes()!=(O/'classes'/p.name).read_bytes())
assert len(changed_classes)==8
annotation_changes=sum(a!=b for a,b in zip(old,new));assert annotation_changes==cov['revision']['annotated_method_rows']==12
for cls in ['ArcBrowserContext','ArcBrowserContextHistory','ArcMenuItemModel']:
 assert 'nil clears stored bridge' not in (R/'classes'/f'{cls}.md').read_text()
assert 'parsed values' not in (R/'classes/ArcWebContentsBoostIDE.md').read_text()
assert 'session_types.h:203' not in (R/'classes/ArcSessionID.md').read_text()
assert not any('0x443f1968' in (R/'classes'/f'{x}.md').read_text() or '0x443f21a8' in (R/'classes'/f'{x}.md').read_text() for x in assigned)
with (R/'revision-evidence/preserved-files.tsv').open('w') as f:
 w=csv.DictWriter(f,['path','sha256'],delimiter='\t');w.writeheader();w.writerows(preserved)
result=dict(result='PASS',classes=38,method_rows=491,unique_method_identities=491,unique_imps=418,categories_and_kinds_and_order_unchanged=True,binary_method_records_checked=len(binary),class_layouts_checked=len(layout),finding_data_rows=len(correction),changed_class_files=changed_classes,annotated_method_rows=annotation_changes,body_status_counts=cov['body_status_counts'],semantic_status_counts=cov['semantic_status_counts'],copied_snapshot_files_byte_preserved=len(preserved),original_report_byte_preserved=True,reviewed_original_snapshot_hashes_checked=len(snapshot),exact_indexed_copied_body_comparisons=len(index),new_transcript_body_comparisons=len(transcript_bodies),new_transcript_unique_bodies=len(set(transcript_bodies)),instruction_words_byte_checked=len(instructions),undecoded_instruction_words=sum(x['instruction']=='<undecoded>' for x in instructions),source_hashes_checked=4,validation_scope='Static identity, layout, matrix/annotation, correction cardinality and provenance; no runtime compatibility or complete behavior verification')
(R/'revision-evidence/validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
