#!/usr/bin/env python3
"""Artifact integrity and coverage checks, not ABI/behavior verification."""
import collections,csv,hashlib,json,pathlib
from inventory import ROOT
def tsv(name):return list(csv.DictReader((ROOT/name).open(),delimiter='\t'))
inv=tsv('inventory.tsv');mapping=tsv('cef-mapping.tsv');requests=tsv('decompile-requests.tsv')
key=lambda r:(r['class_name'],r.get('objc_category',r.get('category')),r['kind'],r['selector'])
assert len(inv)==len(mapping)==834
assert len({key(r) for r in mapping})==834
assert {key(r) for r in inv}=={key(r) for r in mapping}
assert len({r['class_name'] for r in mapping})==63
assert sum(r['selector'].startswith('.cxx_') for r in mapping)==105
assert sum(bool(r['objc_category']) for r in mapping)==48
counts=collections.Counter(r['category'] for r in mapping)
assert {r['category']:int(r['count']) for r in tsv('category-counts.tsv')}=={
    c:counts[c] for c in ('direct_cef_public','partial_cef_plus_bridge','chrome_internal_dependency','arc_specific_behavior','unresolved')}
rev=json.loads((ROOT/'sources/cef-target.json').read_text())['sha']
assert '156.0.8078.48' in (ROOT/'sources/cef/CHROMIUM_BUILD_COMPATIBILITY.txt').read_text()
for r in mapping:
    assert r['evidence_revision']==rev and r['confidence'] and r['rationale']
    assert r['engine170_imp_va'] and r['type_encoding'] and r['ghidra_qualified_name']
    if r['category'] in ('direct_cef_public','partial_cef_plus_bridge'):
        assert r['cef_api'] and f'/blob/{rev}/include/' in r['evidence_url']
assert len(requests)==49
allmethods=tsv('evidence/engine-objc-all.tsv')
look={(r['class_name'],r['category'],r['kind'],r['selector']):r for r in allmethods}
for r in requests:
    assert r['engine170_imp_va']==look[key(r)]['engine170_imp_va']
    assert r['ghidra_qualified_name'] and r['question'] and r['followup']
for source in ('cef','chromium'):
    for r in json.loads((ROOT/f'sources/{source}-manifest.json').read_text()):
        if r.get('status','ok')!='ok':continue
        file=ROOT/f'sources/{source}'/r['path']
        assert hashlib.sha256(file.read_bytes()).hexdigest()==r['sha256'],str(file)
calls=tsv('evidence/app-direct-selector-calls.tsv')
observed=collections.Counter(r['selector'] for r in calls)
for r in inv:assert int(r['app_selector_bl_calls'])==observed[r['selector']]
arrived=tsv('evidence/arrived-requested-functions.tsv') if (ROOT/'evidence/arrived-requested-functions.tsv').exists() else []
followups=tsv('decompile-followups.tsv') if (ROOT/'decompile-followups.tsv').exists() else []
if arrived:
    assert len(arrived)==49 and sum(r['status']=='extracted' for r in arrived)==46
    assert sum(r['status']=='extracted' for r in followups)==14
    for shard in json.loads((ROOT/'evidence/arrived-shards.json').read_text()):
        with pathlib.Path(shard['file']).open('rb') as stream:
            assert hashlib.file_digest(stream,'sha256').hexdigest()==shard['sha256']
summary=dict(status='PASS',inventory_rows=len(inv),classes=63,mapping_rows=len(mapping),
             categories=dict(counts),decompile_requests=len(requests),
             verified_app_selector_BL_instructions=len(calls),cef_revision=rev,
             chromium_match='156.0.8078.48',
             engine_decompile_present=(pathlib.Path('/home/exedev/work/inputs/decompile').is_dir()),
             arrived_requested_functions_extracted=sum(r['status']=='extracted' for r in arrived),
             arrived_followup_functions_extracted=sum(r['status']=='extracted' for r in followups),
             omitted_thunk_resolved_by_binary_branch=bool(arrived),
             warnings=['Integrity checks do not establish Arc behavioral equivalence, CEF build success, callback ownership, or receiver-class dispatch.'])
(ROOT/'evidence/final-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
