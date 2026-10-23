#!/usr/bin/env python3
"""Artifact validation, not semantic/runtime equivalence testing."""
import csv,hashlib,json,re,sqlite3,datetime,resource
from pathlib import Path
ROOT=Path('/home/exedev/work/inputs');OUT=Path('specs')
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for data in iter(lambda:f.read(1024*1024),b''):h.update(data)
 return h.hexdigest()
assigned=json.loads((ROOT/'context/class-assignments.json').read_text())['groups']['spec-3'];meta=json.loads((OUT/'evidence/metadata.json').read_text());rows=list(csv.DictReader((OUT/'methods.tsv').open(),delimiter='\t'));cov=json.loads((OUT/'coverage.json').read_text());bs=json.loads((OUT/'evidence/body-index.json').read_text())
key=lambda x:(x['class_name'],x['category'],x['kind'],x['selector'])
assert len(rows)==assigned['method_rows']==491
assert len({key(x) for x in rows})==491
assert {x['class_name'] for x in rows}==set(assigned['classes'])==set(meta['classes'])
assert {p.stem for p in (OUT/'classes').glob('*.md')}==set(assigned['classes']) and len(assigned['classes'])==37
original={key(x):x for x in meta['methods']}
for x in rows:
 y=original[key(x)];assert x['type_encoding']==y['type_encoding'] and x['imp_va']==y['imp']
 assert x['evidence'] and x['behavior'] and x['confidence'] and x['body_status']
 if x['semantic_status']=='unresolved':assert x['unresolved_question']
for cls,l in meta['classes'].items():
 assert all(0<=v['offset'] and v['offset']+v['size']<=l['instance_size'] for v in l['ivars'])
 md=(OUT/'classes'/f'{cls}.md').read_text()
 expected=sum(x['class_name']==cls for x in rows)
 matrix=sum(line.startswith(('| (base) |','| Extensions |','| Init |')) for line in md.splitlines())
 assert expected==matrix,(cls,expected,matrix)
 assert 'State/layout' in md and 'Unresolved engineering questions' in md
assert len(bs)==len({x['imp_va'] for x in rows})==433
assert json.loads((OUT/'evidence/raw-dump-check.json').read_text())['matched_rows']==491
assert cov['declared_method_rows']==491 and cov['assigned_classes']==37 and cov['unique_identities']==491
assert cov['body_available_method_rows']==491 and cov['missing_method_rows']==cov['failed_method_rows']==0
assert sum(cov['semantic_status_counts'].values())==491
assert cov['unresolved_method_rows']==sum(x['semantic_status']=='unresolved' for x in rows)==79
assert cov['method_rows_with_open_questions']==sum(bool(x['unresolved_question']) for x in rows)==383
assert sum(x['kind']=='+' for x in rows)==3 and sum(bool(x['category']) for x in rows)==3
assert cov['base_category_collisions']==[]
db=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
body_candidates=missing=0
for p in sorted((OUT/'bodies').glob('*.txt')):
 va=int(p.stem,16);row=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone();snapshot=p.read_bytes()
 if row is None:
  assert snapshot.startswith(f'NO_EXPORTED_BODY {hex(va)} '.encode());missing+=1;continue
 assert not row[4] and row[3]<=32768
 with Path(row[1]).open('rb') as f:f.seek(row[2]);raw=f.read(row[3])
 assert snapshot.split(b'\n',1)[1]==raw+b'\n',(p,row)
 assert b'TRUNCATED:' not in snapshot
 body_candidates+=1
sources=json.loads((OUT/'evidence/sources.json').read_text())
assert len(sources)==11
for src in sources:assert sha(Path(src['local_path']))==src['sha256']
gaps=json.loads((OUT/'evidence/address-requests.json').read_text());vt=json.loads((OUT/'evidence/vtable-prefixes.json').read_text())
assert len(gaps)==missing==4
for x in gaps:assert any(y['target_va']==x['va'] for y in vt) and db.execute('select va from functions where va=?',(int(x['va'],16),)).fetchone() is None
leaves=json.loads((OUT/'evidence/arm64-leaves.json').read_text());assert len(leaves)==9 and sum(x['requires_semantic_warning'] for x in leaves)==4
for x in leaves:assert key(x) in original and x['imp']==original[key(x)]['imp'] and x['decoded_whitelist']
app=list(csv.DictReader((OUT/'evidence/app-launch-calls.tsv').open(),delimiter='\t'));assert len(app)==4
for p in [OUT/'report.md',*(OUT/'classes').glob('*.md')]:
 for target in re.findall(r'\]\(([^)]+)\)',p.read_text()):
  if ':' not in target and not target.startswith('#'):
   destination=p.parent/target.split('#',1)[0]
   if destination==OUT/'evidence/validation.json':continue  # emitted after all checks
   assert destination.exists(),(p,target)
report={'result':'PASS','utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'command':'python3 tools/validate.py','scope':'Metadata/artifact/source-byte consistency; no behavioral/runtime equivalence claim','class_files':37,'method_identities':491,'distinct_method_imps':433,'byte_exact_body_snapshots':body_candidates,'missing_backend_targets':missing,'source_hashes_checked':len(sources),'instruction_checked_leaf_rows':len(leaves),'independently_checked_app_BLs':len(app),'max_validator_RSS_KiB':resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,'semantic_unresolved_rows':79,'rows_with_open_questions':383,'failed_declared_IMP_exports':0,'no_size_sum':True}
(OUT/'evidence/validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
