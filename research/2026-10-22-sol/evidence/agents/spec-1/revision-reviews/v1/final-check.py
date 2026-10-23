"""Final schemas, coverage, links and frozen-file immutability checks."""
import collections,json,re
from audit import OUT, EV, IN, PRIOR, REV, digest, readtsv, snap

baseline=json.loads((EV/'baseline-sha256.json').read_text())
assert baseline==snap()
assignments=json.loads((IN/'context/class-assignments.json').read_text())['groups']['spec-1']['classes']
dispositions=readtsv(OUT/'finding-dispositions.tsv')
assert list(dispositions[0])==['finding_row','status','evidence','remaining_unknown']
assert [r['finding_row'] for r in dispositions]==[str(i) for i in range(1,len(readtsv(PRIOR/'findings.tsv'))+1)]
assert all(r['status'] in ('resolved','resolved_with_limits','unresolved_claim') and r['evidence'] for r in dispositions)
classes=readtsv(OUT/'class-review.tsv')
assert list(classes[0])==['class_name','status','evidence','limits']
assert collections.Counter(r['class_name'] for r in classes)==collections.Counter(assignments)
assert all(r['status'] and r['evidence'] and r['limits'] for r in classes)
expected='severity\tclass_name\tselector\tclaim\tevidence\trequired_correction\n'
assert (OUT/'findings.tsv').read_text()==expected
links=re.findall(r'\]\(([^)]+)\)',(OUT/'report.md').read_text())
for target in links: assert (OUT/target).is_file(),target
for p in EV.glob('group-*-original-bodies.txt'):
    assert 'TRUNCATED:' not in p.read_text() and 'NO_EXPORTED_BODY' not in p.read_text()
summary=dict(result='pass',finding_data_rows=len(dispositions),dispositions=dict(collections.Counter(r['status'] for r in dispositions)),assigned_class_verdicts=len(classes),remaining_findings=0,report_links=len(links),unchanged_frozen_files=len(baseline),original_review_finding_sha256=digest(PRIOR/'findings.tsv'))
(EV/'final-check.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
