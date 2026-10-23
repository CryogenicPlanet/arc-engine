#!/usr/bin/env python3
"""Check reviewer deliverable links/counts/provenance; writes only own directory."""
import csv
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def rows(p):
    with p.open(newline='') as f:
        r = list(csv.DictReader(f, delimiter='\t'))
    assert all(None not in x for x in r), p
    return r

def sha(p):
    h = hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda: f.read(1048576), b''):
            h.update(b)
    return h.hexdigest()

required = ['report.md', 'engine-feasibility.md', 'findings.tsv', 'spec-index.tsv',
            'coverage-appendix.md', 'protocol-index.tsv']
for n in required:
    assert (ROOT / n).is_file()
audit = json.loads((ROOT / 'evidence/audit.json').read_text())
spot = json.loads((ROOT / 'evidence/spot-checks.json').read_text())
assert audit['status'] == spot['status'] == 'PASS'
s = rows(ROOT / 'spec-index.tsv')
assert len(s) == len({r['class_name'] for r in s}) == 146
assert sum(int(r['declared_methods']) for r in s) == 1964
for r in s:
    assert '/revisions/v1/classes/' in r['path']
    for field in ['path', 'review_path', 'review_report', 'correction_ledger', 'selection_manifest']:
        assert Path(r[field]).is_file(), (r['class_name'], field)
    assert sha(Path(r['path'])) == r['spec_sha256']
    assert sha(Path(r['review_path'])) == r['review_sha256']
    assert r['revision'] == 'v1'
    assert r['writer'] and r['reviewer'] and r['original_reviewer']
    assert 'not_complete_implementation_contract' in r['synthesis_status']
protocols = rows(ROOT / 'protocol-index.tsv')
assert len(protocols) == 26 and sum(int(p['identities']) for p in protocols) == 200
assert sum(int(p['static_send_supported_identities']) for p in protocols) == 194
for r in protocols:
    for k in ['path', 'callbacks_path', 'combined_send_evidence']:
        assert Path(r[k]).is_file(), (r['protocol_name'], k)
findings = rows(ROOT / 'findings.tsv')
assert len(findings) == len({r['id'] for r in findings}) == 22
assert all(r[k] for r in findings for k in ['severity', 'evidence', 'correction', 'parent_action'])
assert len(rows(ROOT / 'evidence/correction-dispositions.tsv')) == 32
links = []
for name in ['report.md', 'engine-feasibility.md', 'coverage-appendix.md']:
    text = (ROOT / name).read_text()
    for target in re.findall(r'\]\(([^\n)]+)\)', text):
        if target.startswith(('https://', 'http://')):
            continue
        base = re.sub(r':\d+$', '', target).split('#')[0]
        p = Path(base) if base.startswith('/') else ROOT / base
        # Outputs produced below may be linked before this check creates them.
        if p in [ROOT / 'artifacts.sha256', ROOT / 'evidence/output-validation.json']:
            continue
        assert p.exists(), (name, target)
        links.append((name, target))
    assert 'ArcCommandBarTrainingResult' not in text
main = (ROOT / 'engine-feasibility.md').read_text()
for term in ['5,859', '5,847', 'Eight', 'One', '1,670', '97', '195', '194',
             'Aperitif', 'metaclass layouts', 'synchronously', 'return **2**',
             'replacement readiness', '156.0.8078.48', 'No whole-rewrite']:
    assert term in main, term

result = dict(status='PASS', selected_classes=len(s), declaration_rows=1964,
              protocols=len(protocols), protocol_identities=200,
              static_send_supported_protocol_identities=194,
              synthesis_findings=len(findings), historical_finding_dispositions=32,
              checked_local_markdown_links=len(links),
              selected_spec_and_review_hashes=292,
              limitations='Deliverable consistency and provenance, not semantic/runtime acceptance.')
(ROOT / 'evidence/output-validation.json').write_text(json.dumps(result, indent=2) + '\n')
hashes = []
for p in sorted(ROOT.rglob('*')):
    if not p.is_file() or p.name == 'artifacts.sha256' or '__pycache__' in p.parts:
        continue
    hashes.append(f'{sha(p)}  {p.relative_to(ROOT)}')
(ROOT / 'artifacts.sha256').write_text('\n'.join(hashes) + '\n')
print(json.dumps(result, indent=2))
