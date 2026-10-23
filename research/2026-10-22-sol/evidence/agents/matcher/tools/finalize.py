#!/usr/bin/env python3
"""Consolidate reproducible checks and prioritize exact engine shard requests."""
import argparse
import csv
import json
from pathlib import Path
import sqlite3

from engine_match import write_tsv
from macho import sha256_file


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',required=True)
    args=parser.parse_args()
    out=Path(args.out)
    validation=json.loads((out/'validation.json').read_text())
    db=sqlite3.connect(f'file:{out}/evidence.sqlite?mode=ro',uri=True)
    db.execute('PRAGMA cache_size=-8192')
    # The original evidence remains available after request prioritization.
    original=out/'anchor-decompile-requests.tsv'
    if not original.exists():original.write_bytes((out/'decompile-requests.tsv').read_bytes())
    anchor_rows=list(csv.DictReader(original.open(),delimiter='\t'))
    fields=list(anchor_rows[0])
    requests=[]
    for start,end,body,name in db.execute('SELECT start,end,body,name FROM arc WHERE body>1000000 ORDER BY body DESC'):
        requests.append(dict(priority=0,image='ArcCore-1.170.0.arm64',address_basis='unslid engine VA',
            arc_start=hex(start),arc_end_exclusive=hex(end),body_bytes=body,arc_name=name,
            stock_start='',stock_names='[]',reason='Overlapping megabyte TSV span prevents reliable body attribution; counts equal span in this TSV',
            request='First export actual Ghidra AddressSet ranges and getNumAddresses separately for this exact entry; report thunk status and entry namespace. Decompile only actual body blocks, never the entire min/end span.'))
    preferred=['-[BrowserCrApplication sendEvent:]','-[BrowserCrApplication init]',
        '-[AppController tryToTerminateApplication]',
        '-[RenderWidgetHostViewCocoa initWithHost:withHostHelper:]',
        '-[AXPlatformNodeCocoa accessibilityValue]',
        '-[BrowserAccessibilityCocoa accessibilityIsAttributeSettable:]']
    entries={r['identity']:r for r in csv.DictReader((out/'entry-pairs.tsv').open(),delimiter='\t')}
    for name in preferred:
        if name not in entries:continue
        row=entries[name]
        requests.append(dict(priority=0,image='ArcCore-1.170.0.arm64',address_basis='unslid engine VA',
            arc_start=row['arc_va'],arc_end_exclusive=row['arc_tsv_end_exclusive'],body_bytes=row['arc_tsv_body_bytes'],
            arc_name=row['ghidra_name'],stock_start=row['stock_va'],stock_names=json.dumps([name]),
            reason=f"Independent same ObjC identity; {row['window_bytes']}-byte entry window normalized_equal={row['normalized_equal']}; body attribution blocked by wider overlapping spans",
            request='Engine shard with this exact method, actual body ranges, callees and callers; compare same stock method semantics and source. For the 64-byte signature false-positive example, include both the correct and wrong stock methods listed in entry-validation.json.'))
    for row in validation['named_controls']:
        start=int(row['arc_va'],16)
        end,body,name=db.execute('SELECT end,body,name FROM arc WHERE start=?',(start,)).fetchone()
        requests.append(dict(priority=0,image='ArcCore-1.170.0.arm64',address_basis='unslid engine VA',
            arc_start=hex(start),arc_end_exclusive=hex(end),body_bytes=body,arc_name=name,
            stock_start=row['stock_va'],stock_names=json.dumps([row['identity']]),
            reason='Same exported Chromium identity but differing bytes and masked instruction layout; not proof of an Arc edit',
            request='Engine shard for this exact entry and direct callees; compare launch semantics against supplied stock symbol and pinned Chromium source once build provenance is available.'))
    requests.extend(anchor_rows)
    for i,row in enumerate(requests,1):row['priority']=i
    write_tsv(out/'decompile-requests.tsv',fields,requests)
    used=[]
    for row in csv.DictReader((out/'matches.tsv').open(),delimiter='\t'):
        reasons=json.loads(row['reasons'])
        if row['scope']=='unnamed_anchored_sample' and reasons.get('normalized_equal'):
            used.append(dict(arc_va=row['arc_va'],stock_va=row['stock_va'],stock_names=json.loads(row['stock_names']),
                long_anchor_count=len(reasons['long_unique_anchor_ids']),
                import_context_agrees=reasons['import_context_agrees'],status=row['status']))
    entry=json.loads((out/'entry-validation.json').read_text())
    validation['supplemental_entry_window_validation']={
        k:entry[k] for k in ['identity_pairs','full_window_raw_equal','full_window_normalized_equal','full_window_normalized_different']}
    validation['supplemental_entry_window_validation']['fixed_width_results']={
        width:{k:v[k] for k in ['eligible_identity_pairs','counts','rotated_wrong_identity_equal_prefixes','closed_set_unique_identity_precision']}
        for width,v in entry['fixed_width_closed_set_retrieval'].items()}
    validation['supplemental_entry_window_validation']['details_file']='entry-validation.json'
    validation['unit_tests']=json.loads((out/'unit-validation.json').read_text())
    validation['checked_ambiguous_equal_signatures']=used
    validation['decompile_request_count']=len(requests)
    # Verify expected version string in each image, without searching arbitrary
    # binary bytes or assuming a version string proves complete build provenance.
    validation['version_string_locations']=[dict(image=image,va=hex(va),value=value.decode())
        for image,va,value in db.execute('SELECT l.image,l.va,s.value FROM locations l JOIN strings s ON s.id=l.string_id WHERE s.value=?',(b'156.0.8078.48',))]
    rows=list(csv.DictReader((out/'matches.tsv').open(),delimiter='\t'))
    known_scopes={'unnamed_anchored_sample','named_identity_control'}
    checks=dict(matches_rows=len(rows),expected_rows=validation['unnamed_sample_pairs']+validation['named_control_count'],
        all_rows_have_required_fields=all(r['scope'] in known_scopes and r['method'] and r['confidence'] and r['reasons'] for r in rows),
        sqlite_integrity_check=db.execute('PRAGMA integrity_check').fetchone()[0],
        request_addresses_present_in_TSV=all(db.execute('SELECT 1 FROM arc WHERE start=?',(int(r['arc_start'],16),)).fetchone() for r in requests))
    if checks['matches_rows']!=checks['expected_rows'] or not checks['all_rows_have_required_fields'] or checks['sqlite_integrity_check']!='ok' or not checks['request_addresses_present_in_TSV']:
        raise ValueError(checks)
    validation['artifact_checks']=checks
    validation['tool_hashes']={str(f):sha256_file(f) for f in sorted(Path('tools').iterdir()) if f.suffix in ('.py','.sh')}
    validation['primary_sources']=[
        dict(revision='xnu-11417.121.6',commit='a1e26a70f38d1d7daa7b49b258e2f8538ad81650',path='EXTERNAL_HEADERS/mach-o/loader.h',
             url='https://github.com/apple-oss-distributions/xnu/blob/a1e26a70f38d1d7daa7b49b258e2f8538ad81650/EXTERNAL_HEADERS/mach-o/loader.h',local='sources/loader.h',sha256=sha256_file('sources/loader.h')),
        dict(revision='xnu-11417.121.6',commit='a1e26a70f38d1d7daa7b49b258e2f8538ad81650',path='EXTERNAL_HEADERS/mach-o/nlist.h',
             url='https://github.com/apple-oss-distributions/xnu/blob/a1e26a70f38d1d7daa7b49b258e2f8538ad81650/EXTERNAL_HEADERS/mach-o/nlist.h',local='sources/nlist.h',sha256=sha256_file('sources/nlist.h')),
        dict(revision='llvmorg-20.1.8',commit='87f0227cb60147a26a1eeb4fb06e3b505e9c7261',path='llvm/test/MC/AArch64/basic-a64-instructions.s',
             url='https://github.com/llvm/llvm-project/blob/87f0227cb60147a26a1eeb4fb06e3b505e9c7261/llvm/test/MC/AArch64/basic-a64-instructions.s',local='sources/basic-a64-instructions.s',sha256=sha256_file('sources/basic-a64-instructions.s'))]
    (out/'validation.json').write_text(json.dumps(validation,indent=2,sort_keys=True)+'\n')
    print(json.dumps(checks,sort_keys=True))


if __name__=='__main__':main()
