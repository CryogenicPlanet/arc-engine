#!/usr/bin/env python3
"""Validate exact assigned metadata identities, artifacts and bounded corpus snapshots."""
import csv,json,re,sqlite3,collections,hashlib
from pathlib import Path
R=Path('/home/exedev/work/inputs');O=Path('specs');E=O/'evidence'
a=json.load(open(R/'context/class-assignments.json'))['groups']['spec-2'];ms=json.load(open(E/'methods-bodies.json'));rows=list(csv.DictReader(open(O/'methods.tsv'),delimiter='\t'));coverage=json.load(open(O/'coverage.json'))
key=lambda m:(m['class_name'],m['category'],m['kind'],m['selector'])
assert len(rows)==len(ms)==a['method_rows']==491
assert len({key(r) for r in rows})==491
assert set(r['class_name'] for r in rows)==set(a['classes'])
assert {p.stem for p in (O/'classes').glob('*.md')}==set(a['classes'])
assert collections.Counter(key(r) for r in rows)==collections.Counter(key(m) for m in ms)
raw={key(m):m for m in ms};db=sqlite3.connect('file:'+str(R/'corpus-index.sqlite')+'?mode=ro',uri=True)
for r in rows:
 m=raw[key(r)]
 for x,y in [('imp_va','imp'),('type_encoding','type_encoding'),('method_record_va','method_record_va')]:assert r[x]==m[y]
 assert m['imp'] in r['evidence'] and m['method_record_va'] in r['evidence']
 assert all(r[x] for x in ['body_status','behavior','confidence','semantic_status','backend_callees'])
 idx=db.execute('select path,offset,length,failed from functions where va=?',(int(m['imp'],16),)).fetchone()
 assert idx==(m['body_path'],m['body_offset'],m['body_length'],m['failed'])
 with open(m['body_path'],'rb') as f:f.seek(m['body_offset']);body=f.read(m['body_length']).decode('utf-8','replace')
 assert body==m['body'];assert int(re.search(r'// ==== (0x[0-9a-f]+)',body)[1],16)==int(m['imp'],16)
 assert not m['failed'] and not m['truncated']
for c in a['classes']:
 text=(O/'classes'/f'{c}.md').read_text();rr=[r for r in rows if r['class_name']==c]
 matrix=[line for line in text.splitlines() if line.startswith('| ') and ('| - ' in line or '| + ' in line)]
 assert len(matrix)==len(rr),(c,len(matrix),len(rr))
 for r in rr:
  assert r['imp_va'] in text and r['type_encoding'].replace('|','&#124;') in text
 for link in re.findall(r'\]\((\.\./[^)]+)\)',text):assert ((O/'classes')/link).exists(),link
for l in json.load(open(E/'layouts.json')):
 assert all(v['offset']+v['size']<=l['instance_size'] for v in l['ivars'])
for b in json.load(open(E/'bridges.json')):
 assert not b.get('failed') and not b.get('truncated') and b.get('body')
 assert b['va']!='0x421f778'
assert coverage['body_available']==491 and coverage['body_failed']==coverage['body_missing']==0
assert sum(coverage['semantic_status_counts'].values())==491
assert coverage['unresolved_rows']==sum(bool(r['unresolved_question']) for r in rows)
assert coverage['base_category_selector_collisions']==[]
for s in csv.DictReader(open(O/'primary-sources.tsv'),delimiter='\t'):
 assert hashlib.sha256(Path(s['local_path']).read_bytes()).hexdigest()==s['sha256']
# Known misleading decompiler signatures must not silently be treated as complete semantics.
for c,s in [('ArcColor','r'),('ArcColor','color'),('ArcURLLoadConfiguration','timeout'),('ArcAIServiceConfiguration','timeout'),('ArcWebLoginDialog','isProxyLogin'),('ArcWebLoginDialog','close'),('ArcWebContentsMediaSession','setVolume:error:'),('ArcAnnotatedVisit','toAnnotatedVisit')]:
 assert next(r for r in rows if r['class_name']==c and r['selector']==s)['body_status']=='available_signature_artifact'
result=dict(status='PASS',class_specs=37,method_rows=491,unique_imps=453,exact_body_snapshots_checked=491,bridge_snapshots_checked=coverage['selected_bridge_bodies'],metadata_identity_check='binary/dyld/initial reconciled by collect.py; otool encoding/IMP corroborated by otool_check.py',semantic_claim_limit='static local observations and bounded bridge review, no runtime equivalence test')
json.dump(result,open(O/'validation.json','w'),indent=2);print(json.dumps(result,indent=2))
