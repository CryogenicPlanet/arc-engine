#!/usr/bin/env python3
"""Deterministic completion coverage and prompt/input identity checks on research VM."""
import csv
import hashlib
import json
from pathlib import Path
import sqlite3
import sys

root=Path('/home/exedev/work')
role,stage,action=sys.argv[1:]
job=role+'-'+stage
directory=root/'agents'/role/stage

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def rows(path):
    assert path.is_file() and path.stat().st_size>0,path
    with path.open(newline='') as stream:return list(csv.DictReader(stream,delimiter='\t'))
def identity():
    paths=[root/'prompts'/(job+'.md')]
    for name in ('FULL-CORPUS-READY','INDEX-READY','CHROMIUM-READY','context/class-assignments.json','context/lead-review.md','context/ENGINE.md'):
        path=root/'inputs'/name
        if path.exists():paths.append(path)
    paths+=sorted((root/'inputs').glob('decompile*/**/MANIFEST.json'))
    return {str(path.relative_to(root)):sha(path) for path in paths}

if action=='record-identity':
    path=root/(job+'.run.json')
    # An existing record may represent a failed attempt; do not overwrite it.
    if path.exists():assert json.loads(path.read_text())==identity(),'Changed input/prompt; use explicit follow-up job'
    else:path.write_text(json.dumps(identity(),indent=2)+'\n')
elif action=='check-identity':
    assert json.loads((root/(job+'.run.json')).read_text())==identity(),'Completed scope differs; use explicit follow-up job'
elif action=='artifacts':
    assignments=json.loads((root/'inputs/context/class-assignments.json').read_text())
    if stage=='specs':
        classes=set(assignments['groups'][role]['classes'])
        actual={p.stem for p in (directory/'classes').glob('*.md') if p.stat().st_size>0}
        assert actual==classes,('Missing/extra class specs',classes-actual,actual-classes)
        metadata=rows(root/'agents/arc-layer/initial/objc-methods.tsv')
        expected={(r['class_name'],r['category'],r['kind'],r['selector'],int(r['imp'],0))
                  for r in metadata if r['class_name'] in classes and not r['protocol']}
        methods=rows(directory/'methods.tsv')
        actual=[(r['class_name'],r['category'],r['kind'],r['selector'],int(r['imp_va'],0)) for r in methods]
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Method identities missing/duplicated'
        assert isinstance(json.loads((directory/'coverage.json').read_text()),dict)
    elif stage=='classreview':
        other='spec-'+str(int(role[-1])%4+1)
        expected=set(assignments['groups'][other]['classes'])
        review=rows(directory/'class-review.tsv')
        actual=[r['class_name'] for r in review]
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Review class coverage mismatch'
        assert (directory/'findings.tsv').is_file()
    elif role=='quality':
        assert isinstance(json.loads((directory/'coverage.json').read_text()),dict)
        failed=rows(directory/'failures.tsv')
        db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
        expected={r[0] for r in db.execute('SELECT va FROM functions WHERE failed=1')}
        actual=[int(r['va'],0) for r in failed]
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Failure inventory mismatch'
        assert (directory/'quality-gates.tsv').is_file()
    elif stage=='synthesis':
        assert (directory/'engine-feasibility.md').stat().st_size>0
        actual=[r['class_name'] for r in rows(directory/'spec-index.tsv')]
        expected={c for g in assignments['groups'].values() for c in g['classes']}
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Synthesis class index mismatch'
    print(job,'artifact/coverage validation passed')
else:raise SystemExit('Unknown action')
