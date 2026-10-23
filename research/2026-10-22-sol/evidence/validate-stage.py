#!/usr/bin/env python3
"""Deterministic completion coverage and prompt/input identity checks on research VM."""
import csv
import hashlib
import json
from pathlib import Path
import re
import sqlite3
import sys

root=Path('/home/exedev/work')
role,stage,action=sys.argv[1:]
job=role+'-'+stage
directory=root/'agents'/role/stage

def sha(path):
    digest=hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda:stream.read(1048576),b''):digest.update(block)
    return digest.hexdigest()
def rows(path):
    assert path.is_file() and path.stat().st_size>0,path
    with path.open(newline='') as stream:return list(csv.DictReader(stream,delimiter='\t'))
def identity():
    paths=[root/'prompts'/(job+'.md')]
    for name in ('FULL-CORPUS-READY','INDEX-READY','CHROMIUM-READY','context/class-assignments.json','context/lead-review.md','context/ENGINE.md'):
        path=root/'inputs'/name
        if path.exists():paths.append(path)
    paths+=sorted((root/'inputs').glob('decompile*/**/MANIFEST.json'))
    if role=='protocols':
        paths+=[root/'inputs/app168/dump/objc-classes.h']
        paths+=[root/'inputs/binaries'/name for name in ('Arc-1.168.0.arm64','ArcCore-1.168.0.arm64','ArcCore-1.170.0.arm64')]
    if stage=='synthesis':
        paths+=[root/'CLASS-REVISIONS-READY']
        for name in ('acceptance-evidence-review.md','body-evidence-review.md',
                     'protocol-evidence-review.md','parent-encoding-check.tsv',
                     'parent-metadata-comparison.json'):
            paths.append(root/'inputs/context'/name)
        # Completed role evidence is frozen, not just its summaries. The large
        # distribution catalog is documentary discovery data omitted by the
        # compact package, so it is not a synthesis input identity dependency.
        for role_name in ('abi','cef','arc-layer','matcher','quality','protocols'):
            for path in (root/'agents'/role_name).rglob('*'):
                if path.is_file() and '__pycache__' not in path.parts and '.sqlite' not in path.name \
                        and path != root/'agents/cef/initial/sources/cef-build-index.json':
                    paths.append(path)
        final=json.loads((root/'CLASS-REVISIONS-READY').read_text())
        assert final['classes']==146 and final['declared_methods']==1964
        assert len(final['selected'])==4
        classes=[c for item in final['selected'] for c in item['classes']]
        assert len(classes)==146 and len(set(classes))==146
        for item in final['selected']:
            for field in ('spec_sha256','review_sha256'):
                for name,expected in item[field].items():
                    path=(root/name).resolve()
                    assert path.is_relative_to(root) and sha(path)==expected,'Published revision changed: '+name
                    paths.append(path)
    return {str(path.relative_to(root)):sha(path) for path in dict.fromkeys(paths)}

if action=='record-identity':
    path=root/(job+'.run.json')
    # An existing record may represent a failed attempt; do not overwrite it.
    if path.exists():assert json.loads(path.read_text())==identity(),'Changed input/prompt; use explicit follow-up job'
    else:path.write_text(json.dumps(identity(),indent=2)+'\n')
elif action=='check-identity':
    assert json.loads((root/(job+'.run.json')).read_text())==identity(),'Completed scope differs; use explicit follow-up job'
elif action=='record-completion':
    assert stage=='synthesis','Other stages use their existing completion/recovery records'
    before=json.loads((root/(job+'.run.json')).read_text())
    assert before==identity(),'Input changed before completion proof'
    terminal=None
    with (root/'logs'/(job+'.jsonl')).open() as stream:
        for line in stream:
            try:event=json.loads(line)
            except ValueError:continue
            if event.get('type') in ('turn.completed','turn.failed','error'):terminal=event
    assert terminal and terminal['type']=='turn.completed','No final successful CLI event'
    outputs={str(path.relative_to(root)):sha(path) for path in sorted(directory.rglob('*'))
             if path.is_file() and '__pycache__' not in path.parts and '.sqlite' not in path.name}
    final_output=root/'out'/(job+'.md')
    assert final_output.stat().st_size>0
    proof={'inputs':before,'outputs':outputs,'terminal_event':terminal,
           'final_output':{'path':str(final_output.relative_to(root)),'sha256':sha(final_output)}}
    target=root/(job+'.completion.json')
    assert not target.exists(),'Never overwrite completion proof'
    temp=root/('.'+job+'.completion.tmp')
    with temp.open('x') as stream:stream.write(json.dumps(proof,indent=2)+'\n')
    temp.replace(target)
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
    elif role=='protocols':
        dump=(root/'inputs/app168/dump/objc-classes.h').read_text()
        blocks=re.findall(r'(?ms)^@protocol (Arc\w+)[^\n]*\n(.*?)^@end',dump)
        names={name for name,_ in blocks}
        inventory=rows(directory/'protocols.tsv')
        actual=[r['protocol_name'] for r in inventory]
        assert len(actual)==len(set(actual)) and set(actual)==names,'Protocol inventory coverage mismatch'
        expected={(name,kind,selector) for name,body in blocks
                  for kind,selector in re.findall(r'^([+-])\[\S+ (.*?)\];',body,re.M)}
        callbacks=rows(directory/'callbacks.tsv')
        actual=[(r['protocol_name'],r['kind'],r['selector']) for r in callbacks]
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Dump protocol declaration coverage mismatch'
        assert {p.stem for p in (directory/'protocol-specs').glob('*.md') if p.stat().st_size>0}==names
        assert isinstance(json.loads((directory/'coverage.json').read_text()),dict)
        contract=rows(root/'agents/abi/initial/contract.tsv')
        expected={(r['class_name'],r['category'],r['kind'],r['selector']) for r in contract if not r['encoding_168']}
        audit=rows(directory/'older-encoding-audit.tsv')
        actual=[(r.get('class_name',r.get('class')),r.get('category',''),r.get('kind'),r.get('selector')) for r in audit]
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Six older encoding identities missing/duplicated'
    elif stage=='synthesis':
        assert (directory/'engine-feasibility.md').stat().st_size>0
        actual=[r['class_name'] for r in rows(directory/'spec-index.tsv')]
        expected={c for g in assignments['groups'].values() for c in g['classes']}
        assert len(actual)==len(set(actual)) and set(actual)==expected,'Synthesis class index mismatch'
    print(job,'artifact/coverage validation passed')
else:raise SystemExit('Unknown action')
