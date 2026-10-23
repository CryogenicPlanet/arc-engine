#!/usr/bin/env python3
import collections,csv,hashlib,json,sqlite3
from pathlib import Path
HERE=Path(__file__).resolve().parents[1]
assignment=json.load(open('/home/exedev/work/inputs/context/class-assignments.json'))['groups']['spec-2']['classes']
reviews=list(csv.DictReader(open(HERE/'class-review.tsv'),delimiter='\t'));findings=list(csv.DictReader(open(HERE/'findings.tsv'),delimiter='\t'))
assert len(reviews)==37 and {r['class_name'] for r in reviews}==set(assignment)
assert len(findings)==13 and {r['class'] for r in findings}<set(assignment)
assert len({r['class'] for r in findings})==10
db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True);db.row_factory=sqlite3.Row
samples=list(csv.DictReader(open(HERE/'evidence/semantic-review.tsv'),delimiter='\t'))
vas={v for r in samples for v in r['reviewed_body_vas'].split()};assert len(vas)==118
for va in vas:
    e=db.execute('select * from functions where va=?',(int(va,16),)).fetchone()
    with open(e['path'],'rb') as f:f.seek(e['offset']);raw=f.read(e['length'])
    assert raw==(HERE/'evidence/body-views'/(va+'.c')).read_bytes()
    assert not e['failed']
for r in csv.DictReader(open(HERE/'evidence/reviewed-spec-hashes.tsv'),delimiter='\t'):
    assert hashlib.sha256(Path(r['path']).read_bytes()).hexdigest()==r['sha256']
coverage=json.load(open(HERE/'evidence/coverage-check.json'))
assert coverage['classes']==37 and coverage['method_rows']==491 and coverage['body_snapshot_matches']==491
assert coverage['span_count_differences']==16
assert len(json.load(open(HERE/'evidence/leaf-load-checks.json')))==59
assert (HERE/'report.md').exists()
result=dict(result='PASS',classes=37,method_rows=491,unique_imps=453,findings=len(findings),classes_with_findings=10,selected_body_snapshots_rechecked=len(vas),severity_counts=dict(collections.Counter(r['severity'] for r in findings)),limits='Validation checks structure/provenance, not semantic correctness or runtime equivalence.')
(HERE/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
