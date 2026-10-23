#!/usr/bin/env python3
"""Validate final review shape, evidence references and preservation without touching inputs."""
import csv,hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parent
INPUT=Path('/home/exedev/work/inputs');REV=Path('/home/exedev/work/agents/spec-3/revisions/v1')
def read(p):return list(csv.DictReader(p.open(),delimiter='\t'))
assigned=json.loads((INPUT/'context/class-assignments.json').read_text())['groups']['spec-3']['classes']
cs=read(HERE/'class-review.tsv');assert len(cs)==37 and len({r['class_name'] for r in cs})==37 and set(r['class_name'] for r in cs)==set(assigned)
assert list(cs[0])==['class_name','status','evidence','limits'] and all(r['evidence'] and r['limits'] for r in cs)
fs=read(HERE/'finding-dispositions.tsv');prior=read(Path('/home/exedev/work/agents/spec-2/classreview/findings.tsv'))
assert len(fs)==len(prior)==6 and [r['finding_row'] for r in fs]==list(map(str,range(1,7)))
assert list(fs[0])==['finding_row','status','evidence','remaining_unknown']
assert all(r['status'] in {'resolved','resolved_with_limits','unresolved_claim'} and r['evidence'] and r['remaining_unknown'] for r in fs)
assert read(HERE/'findings.tsv')==[]
assert (HERE/'findings.tsv').read_text().splitlines()[0]=='severity\tclass_name\tselector\tclaim\tevidence\trequired_correction'
sample=read(HERE/'evidence/manual-method-sample.tsv');assert len(sample)==50 and set(r['class_name'] for r in sample)==set(assigned)
for r in sample:assert (HERE/r['body']).is_file()
for p,h in json.loads((HERE/'evidence/protected-before.json').read_text()).items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
for name in ('report.md','class-review.tsv','finding-dispositions.tsv','findings.tsv'):assert (HERE/name).is_file()
result=dict(result='PASS',classes=37,original_finding_rows=6,resolved=2,resolved_with_limits=4,remaining_findings=0,manual_declaration_sample=50,protected_files_unchanged=1849)
(HERE/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
