#!/usr/bin/env python3
"""Validate evidence provenance and explicit observations, not runtime semantics."""
import collections,csv,hashlib,json,pathlib,sqlite3
O=pathlib.Path(__file__).resolve().parents[1];I=pathlib.Path('/home/exedev/work/inputs')
def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
s=json.loads((O/'audit-summary.json').read_text());assert s['handoff']['rows']==5846
assert s['corpus']['unique_shards']==199 and s['corpus']['headers']==567959
c=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
assert c.execute('select count(*) from functions where failed=1').fetchone()[0]==79
bodies={r['va']:r for r in rows(O/'reviewed-body-index.tsv')}
for va,r in bodies.items():
 b=(O/r['copy']).read_bytes();assert len(b)==int(r['C_bytes']) and hashlib.sha256(b).hexdigest()==r['body_sha256']
 q=c.execute('select path,offset,length from functions where va=?',(int(va,16),)).fetchone();assert q==(r['path'],int(r['offset']),len(b))
 with open(r['path'],'rb') as f:f.seek(int(r['offset']));assert f.read(len(b))==b
claims=json.loads((O/'claims.json').read_text());checks=0
for claim in claims:
 for va in claim['engine']:assert va in bodies,(claim['id'],va)
 for va,tokens in claim['anchors'].items():
  b=(O/bodies[va]['copy']).read_text()
  for token in tokens:assert token in b,(claim['id'],va,token);checks+=1
for r in rows(O/'source-backed-identities.tsv'):assert digest(pathlib.Path(r['path']))==r['sha256']
for r in rows(O/'unresolved-callees.tsv'):
 q=c.execute('select failed from functions where va=?',(int(r['engine_va'],16),)).fetchone();assert q==(0,),r
assert len(rows(O/'app168-essential-sends.tsv'))==8
assert sum(s['audited_syntax']['closure_depths'].values())==5793
assert not s['audited_syntax']['missing_exported_bodies']
assert len(rows(O/'merged-metadata-regions.tsv'))==10
assert len(rows(O/'malformed-body-review.tsv'))==6
availability=collections.Counter(r['status'] for r in rows(O/'initial-request-availability.tsv'));assert availability=={'present':138,'no_exact_exported_body':2}
# Hash pins: the exact images used in new binary checks and the source artifacts read.
pins=[]
for p in [I/'binaries/ArcCore-1.170.0.arm64',I/'binaries/Arc-1.168.0.arm64',I/'metadata/decomp/arccore-1.170.0-functions.tsv',I/'claude-handoff/scripts/arc_layer.py',I/'claude-handoff/results/arc-layer-functions.tsv',I/'context/ENGINE.md',I/'context/lead-review.md',I/'app168/ghidra/decomp/0x1004.c',I/'app168/ghidra/decomp/0x1005.c']:
 pins.append(dict(path=str(p),sha256=digest(p)))
with (O/'input-pins.tsv').open('w') as f:w=csv.DictWriter(f,['path','sha256'],delimiter='\t');w.writeheader();w.writerows(pins)
result=dict(status='PASS',corpus_headers=567959,corpus_failure_placeholders=79,claims_checked=len(claims),literal_anchor_checks=checks,exact_body_copies=len(bodies),app_binary_sends=8,source_identity_comparisons=4,initial_request_availability=dict(availability),missing_entry_or_instruction_requests=len(rows(O/'additional-shards.tsv')),scope='Provenance and static evidence validation only; not execution, compatibility, source equivalence, ABI or code-size validation.')
(O/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
