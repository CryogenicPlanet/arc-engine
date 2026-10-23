#!/usr/bin/env python3
"""Validate review completeness, snapshot stability and selected body bytes."""
import csv,collections,hashlib,json,sqlite3
from pathlib import Path
O=Path(__file__).resolve().parents[1];I=Path('/home/exedev/work/inputs')
def read(p):return list(csv.DictReader(open(O/p),delimiter='\t'))
classes=set(json.load(open(I/'context/class-assignments.json'))['groups']['spec-4']['classes'])
reviews=read('class-review.tsv');findings=read('findings.tsv');semantic=read('semantic-review.tsv');checks=read('coverage-checks.tsv')
assert len(reviews)==len(classes)==38 and {r['class_name'] for r in reviews}==classes
assert len(semantic)==38 and sum(int(x['declared_method_rows']) for x in semantic)==491
assert len(checks)==38 and sum(int(x['method_rows']) for x in checks)==491
assert {r['class_name'] for r in reviews if r['result']=='requires_correction'}=={r['class'] for r in findings}
for r in read('reviewed-spec-snapshot.tsv'):assert hashlib.sha256(Path(r['path']).read_bytes()).hexdigest()==r['sha256'],r['path']
db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True);selected={r['va']:r for r in map(json.loads,open(O/'body-read-log.jsonl'))};out=[]
for va,r in selected.items():
 name,path,off,length,failed=db.execute('select name,path,offset,length,failed from functions where va=?',(int(va,16),)).fetchone()
 assert not failed and length==r['length'] and off==r['offset'] and path==r['path']
 with open(path,'rb') as f:f.seek(off);body=f.read(length)
 assert body==(O/'bodies'/f'{int(va,16):x}.c').read_bytes()
 out.append(dict(va=va,name=name,path=path,offset=off,length=length,sha256=hashlib.sha256(body).hexdigest()))
with open(O/'selected-body-validation.tsv','w') as f:
 w=csv.DictWriter(f,list(out[0]),delimiter='\t');w.writeheader();w.writerows(out)
v=json.load(open(O/'validation.json'));assert v['method_rows']==491 and v['all_layout_and_ivar_tables_match']
assert {r['va'] for r in v['unmapped_citations']}=={'0x443f1968','0x443f21a8'}
summary=dict(classes=38,method_rows=491,findings=len(findings),classes_requiring_correction=len({r['class'] for r in findings}),selected_bodies_validated=len(out),snapshot_files_unchanged=len(read('reviewed-spec-snapshot.tsv')),result='PASS inventory/provenance/review structure; semantic corrections remain publication gates')
(O/'review-validation.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
