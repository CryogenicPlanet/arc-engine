#!/usr/bin/env python3
"""Independent output consistency and exact on-disk address checks."""
import argparse
import collections
import csv
import json
import struct
from pathlib import Path
from inventory import MachO, digest


def rows(path):return list(csv.DictReader(open(path),delimiter='\t'))


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--inputs',type=Path,default=Path('/home/exedev/work/inputs'))
    ap.add_argument('--out',type=Path,default=Path('initial'))
    a=ap.parse_args();o=a.out
    counts=json.loads((o/'counts.json').read_text())
    m=MachO(a.inputs/'binaries/ArcCore-1.170.0.arm64')
    for p,h in counts['sha256'].items():assert digest(a.inputs/p)==h,p
    ns=rows(o/'arc-functions.tsv');strings=rows(o/'arc-strings.tsv');xs=rows(o/'string-xrefs.tsv')
    requests=rows(o/'decompile-requests.tsv')
    requested={r['address'] for r in requests}
    named={r['va']:r for r in ns}
    table={}
    all_sizes=0;all_spans=0;all_size_mismatch=0;all_count=0
    for l in open(a.inputs/'metadata/decomp/arccore-1.170.0-functions.tsv'):
        va,end,size,name=l.rstrip('\n').split('\t',3)
        all_count+=1;all_sizes+=int(size);all_spans+=int(end,16)-int(va,16)
        all_size_mismatch+=int(size)!=int(end,16)-int(va,16)
        if va in named:
            r=named[va]
            assert r['end_bound']==end and int(r['reported_body_bytes'])==int(size) and r['name']==name
        if va in requested:table[va]=(name,end)
    assert all_count==counts['function_table']['functions']
    assert all_sizes==all_spans==counts['function_table']['reported_body_bytes']
    assert not all_size_mismatch
    for r in ns:
        v=int(r['va'],16)
        assert int(r['file_offset'],16)==m.offset(v)
        assert r['expected_shard']==f'0x{v>>20:x}.c'
    for r in strings:
        va=int(r['va'],16);off=int(r['file_offset'],16);b=r['text'].encode()
        assert m.offset(va)==off and m.read(off,len(b))==b,r
    for r in xs:
        pc=int(r['instruction_va'],16);w,w2=struct.unpack('<II',m.vread(pc,8))
        assert int(r['instruction_file_offset'],16)==m.offset(pc)
        assert hex(w)==r['adrp_word'] and hex(w2)==r['add_word']
        imm=((w>>29)&3)|(((w>>5)&0x7ffff)<<2)
        if imm&(1<<20):imm-=1<<21
        dst=(pc&~4095)+(imm<<12)+(((w2>>10)&4095)<<(12 if w2&(1<<22) else 0))
        assert dst==int(r['string_va'],16)
    for r in requests:
        va=int(r['address'],16)
        assert r['address'] in table,r
        assert r['expected_shard']==f'0x{va>>20:x}.c' and int(r['file_offset'],16)==m.offset(va)
    imps=rows(o/'objc-arc-imp-anchors.tsv')
    assert len(imps)==1566
    assert sum(r['arc_named_anchor']=='True' for r in imps)==1520
    assert len(ns)==1531 and len({r['name'] for r in ns})==1520
    assert sum(bool(r['instruction_thunk_pattern']) for r in ns)==12
    assert sum(r['primary_arc_objc_imp']=='False' for r in ns)==11
    subs=rows(o/'subsystems.tsv')
    assert sum(int(r['named_function_entries']) for r in subs)==len(ns)
    assert sum(int(r['named_reported_bytes']) for r in subs)==288288
    assert sum(int(r['arc_source_paths']) for r in subs)==111
    app=MachO(a.inputs/'binaries/Arc-1.168.0.arm64')
    calls=rows(o/'app168-launch-calls.tsv')
    for r in calls:
        pc=int(r['call_va'],16);w=struct.unpack('<I',app.vread(pc,4))[0]
        imm=w&0x3ffffff
        if imm&(1<<25):imm-=1<<26
        assert w&0xfc000000==0x94000000
        assert pc+(imm<<2)==int(r['stub_va'],16)
        assert int(r['call_file_offset'],16)==app.offset(pc)
        assert app.string(app.ptr(int(r['selector_ref_va'],16)))==r['selector']
    result=dict(status='PASS',named_function_rows=len(ns),string_occurrence_rows=len(strings),
                exact_instruction_xrefs=len(xs),decompile_request_rows=len(requests),app_direct_call_sites=len(calls),
                arc_primary_imp_anchors=len(imps),explicit_dyld_binary_method_tuple_matches=counts['objc']['dyld_binary_key_match'])
    (o/'validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))


if __name__=='__main__':main()
