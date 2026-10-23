"""Validate this revision without invoking the historical output-writing harness."""
import csv
import hashlib
import html
import json
import re
import runpy
import struct
import subprocess
from collections import Counter
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
ORIGINAL=Path('/home/exedev/work/agents/spec-2/specs')
REVIEW=Path('/home/exedev/work/agents/spec-1/classreview')
assert ROOT==Path('/home/exedev/work/agents/spec-2/revisions/v1')
def table(p):
    return list(csv.DictReader(p.open(),delimiter='\t'))
def digest(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(65536),b''):h.update(b)
    return h.hexdigest()
old=table(ORIGINAL/'methods.tsv');new=table(ROOT/'methods.tsv')
assert len(new)==len(old)==491
annotations={'body_status','behavior','evidence','confidence','unresolved_question','semantic_status'}
assert set(old[0])==set(new[0])
changes=[]
for a,b in zip(old,new):
    assert {k:a[k] for k in a if k not in annotations}=={k:b[k] for k in b if k not in annotations}
    if a!=b:changes.append({k:b[k] for k in ['class_name','category','kind','selector','imp_va']})
assert len(changes)==14
key=lambda r:(r['class_name'],r['category'],r['kind'],r['selector'])
assert len({key(r) for r in new})==491
assert len({r['imp_va'] for r in new})==453
assert Counter(r['kind'] for r in new)=={'-':491}
classes=sorted({r['class_name'] for r in new})
assert len(classes)==37
assert {p.name for p in (ROOT/'classes').glob('*.md')}=={p.name for p in (ORIGINAL/'classes').glob('*.md')}
matrix_count=0
links=0
for cls in classes:
    text=(ROOT/'classes'/f'{cls}.md').read_text()
    matched=[]
    for line in text.splitlines():
        if not line.startswith('| '):continue
        cells=[c.strip() for c in line.split('|')[1:-1]]
        if len(cells)!=8 or not cells[1].startswith(('- ','+ ')):continue
        category='' if cells[0]=='base' else cells[0]
        kind,selector=cells[1][0],cells[1][2:]
        r=next(r for r in new if key(r)==(cls,category,kind,selector))
        assert cells[2]=='`'+r['type_encoding']+'`'
        assert cells[3]==r['imp_va']
        assert html.unescape(cells[4])==r['behavior'],(cls,selector)
        assert cells[5]==r['backend_callees'] or (not r['backend_callees'] and cells[5]=='local state / indirect operation; see body')
        assert cells[7]==f"{r['confidence']} / {r['body_status']} / {r['semantic_status']}"
        matched.append(key(r));matrix_count+=1
    assert Counter(matched)==Counter(key(r) for r in new if r['class_name']==cls)
    for target in re.findall(r'\]\(([^)]+)\)',text):
        if '://' in target:continue
        assert (ROOT/'classes'/target.split('#')[0]).exists(),(cls,target)
        links+=1
assert matrix_count==491

findings=table(REVIEW/'findings.tsv');ledger=table(ROOT/'corrections.tsv')
assert len(ledger)==len(findings)==13
assert list(ledger[0])==['finding_row','class_name','selector','disposition','before','after','evidence','remaining_unknown']
for number,(f,c) in enumerate(zip(findings,ledger),1):
    assert int(c['finding_row'])==number
    assert (c['class_name'],c['selector'])==(f['class'],f['selector'])
    assert all(c.values())
    r=next(r for r in new if (r['class_name'],r['selector'])==(f['class'],f['selector']))
    assert c['after']==r['behavior'] and c['remaining_unknown']==r['unresolved_question']
    assert f['exact evidence'] in c['evidence']

preserved={}
for directory in ['evidence','sources']:
    expected={str(p.relative_to(ORIGINAL)) for p in (ORIGINAL/directory).rglob('*') if p.is_file()}
    actual={str(p.relative_to(ROOT)) for p in (ROOT/directory).rglob('*') if p.is_file()}
    assert expected==actual
    for rel in sorted(expected):
        assert digest(ORIGINAL/rel)==digest(ROOT/rel),rel
        preserved[rel]=digest(ROOT/rel)
assert digest(ROOT/'original-report.md')==digest(ORIGINAL/'report.md')
for name in ['callbacks.tsv','next-requests.tsv','primary-sources.tsv','validation-run.log','validation-resource.log']:
    assert digest(ROOT/name)==digest(ORIGINAL/name),name

# Independently decode the bounded binary method records for every declaration.
image=runpy.run_path(str(ROOT/'revision-evidence/disassemble.py'))
for r in new:
    va=int(r['method_record_va'],16)
    sr,tr,ir=struct.unpack('<iii',image['vread'](va,12))
    assert image['string'](va+4+tr)==r['type_encoding']
    assert va+8+ir==int(r['imp_va'],16)
    assert image['string'](image['ptr'](va+sr))==r['selector']

coverage=json.loads((ROOT/'coverage.json').read_text())
assert coverage['body_status_counts']==dict(Counter(r['body_status'] for r in new))
assert coverage['semantic_status_counts']==dict(Counter(r['semantic_status'] for r in new))
assert coverage['unresolved_rows']==sum(bool(r['unresolved_question']) for r in new)==403
assert coverage['explicitly_semantically_unresolved_rows']==1
assert coverage['signature_or_type_artifact_rows']==13
assert coverage['body_quality_flagged_rows']==14
assert coverage['body_incoherent_semantics']==1
assert coverage['body_malformed_established']==0
for cls,p in coverage['per_class'].items():
    rs=[r for r in new if r['class_name']==cls]
    assert p['method_rows']==p['body_available']==len(rs)
    assert p['semantic_status_counts']==dict(Counter(r['semantic_status'] for r in rs))
    assert p['body_status_counts']==dict(Counter(r['body_status'] for r in rs))

# Reproduce only the bounded read-helper outputs; do not run original writer/review tools.
sets={
 'rechecked-bodies.txt':(16384,['0x441a848','0x440d9f8','0x43ff558','0x443f654','0x43ecb50','0x43ece08','0x4401f34','0x43de074','0x43dd458','0x441a1c4','0x441c3c0','0x43d9ab8','0x43d9fd4']),
 'related-bodies.txt':(16384,['0x443366c','0x4433528','0x43dad44','0x4401ca0','0x4415b74','0x435c8fc','0x435ca2c','0x432c834','0x436df2c','0x436d69c']),
 'crosscheck-bodies.txt':(12000,['0x4415b60','0x24e39c0'])}
for name,(bound,addresses) in sets.items():
    actual=subprocess.check_output(['python3','/home/exedev/work/read-function.py',*addresses,'--max-bytes',str(bound)])
    assert actual==(ROOT/'revision-evidence'/name).read_bytes()
    assert b'TRUNCATED:' not in actual
    # The interior 0x4415b74 negative query is intentionally retained, not counted as an IMP gap.
assert subprocess.check_output(['python3',str(ROOT/'revision-evidence/disassemble.py')])==(ROOT/'revision-evidence/instructions.txt').read_bytes()

source_hashes=[]
for r in table(ROOT/'primary-sources.tsv'):
    p=Path(r['local_path']) if r['project']=='chromium' else ROOT/'sources/cef'/Path(r['source_path']).name
    assert digest(p)==r['sha256']
    source_hashes.append(dict(project=r['project'],revision=r['revision'],path=str(p),sha256=digest(p)))

inputs=[REVIEW/n for n in ['report.md','findings.tsv','class-review.tsv']]
inputs += [Path('/home/exedev/work/inputs/context')/n for n in ['lead-review.md','body-evidence-review.md','acceptance-evidence-review.md','ENGINE.md']]
provenance=dict(image=str(image['IMAGE']),image_version='ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48',
                image_LC_UUID=image['image_uuid'],address_basis='unslid Mach-O VA; LC_SEGMENT_64 translation',
                body_reader='/home/exedev/work/read-function.py',body_reader_sha256=digest(Path('/home/exedev/work/read-function.py')),
                selected_reads={name:dict(addresses=addresses,max_bytes_per_function=bound,sha256=digest(ROOT/'revision-evidence'/name)) for name,(bound,addresses) in sets.items()},
                disassembler='existing /lib/x86_64-linux-gnu/libLLVM.so.20.1, AArch64 via ctypes; unrecognized instructions explicitly retained',
                original_review_and_context=[dict(path=str(p),sha256=digest(p)) for p in inputs],
                preserved_original_evidence_and_sources=preserved,primary_sources=source_hashes,
                changed_method_annotations=changes,
                scope='bounded static correction; no product/runtime/build/remote access or complete callee recovery')
(ROOT/'revision-evidence/provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
result=dict(status='PASS',classes=37,method_identities=491,unique_imps=453,instance_methods=491,
            method_identity_type_imp_record_and_evidence_location_preserved=491,
            raw_binary_method_records_rechecked=491,class_matrix_rows=491,class_relative_links=links,
            finding_data_rows=13,correction_rows=13,changed_method_annotation_rows=len(changes),
            copied_raw_evidence_and_source_files_byte_identical=len(preserved),
            original_report_byte_identical=True,source_hashes_verified=len(source_hashes),
            bounded_original_body_reads=24,interior_address_negative_query=1,
            body_status_counts=coverage['body_status_counts'],semantic_status_counts=coverage['semantic_status_counts'],
            unresolved_question_rows=403,
            limitation='Structural/provenance consistency and bounded static evidence only; no runtime compatibility or full callee/ABI/thread/teardown contract established.')
for p in [ROOT/'validation.json',ROOT/'revision-evidence/validation.json']:
    p.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
