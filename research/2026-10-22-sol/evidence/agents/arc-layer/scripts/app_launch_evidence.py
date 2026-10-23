#!/usr/bin/env python3
"""Verify a small set of app launch sends using actual BLs and selector stubs.

Receiver association comes from explicitly identified old app pseudocode; the
binary verifies call sites and selectors, not runtime type or callback semantics.
"""
import argparse
import csv
import json
import struct
from pathlib import Path
from inventory import MachO, digest, tsv


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--inputs',type=Path,default=Path('/home/exedev/work/inputs'))
    ap.add_argument('--out',type=Path,default=Path('initial'))
    args=ap.parse_args()
    binary=args.inputs/'binaries/Arc-1.168.0.arm64'
    m=MachO(binary)
    sec=next(s for s in m.sections if s['section']=='__objc_stubs')
    wanted={
        'init:', 'init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:',
        'setNotificationCenter:', 'setHistogramSamplesDelegate:', 'initialize:',
        'setArcPerformanceObserver:', 'setArcCrashKeysObserver:',
    }
    stubs={}
    for va in range(sec['va'],sec['va']+sec['size'],32):
        w,w2=struct.unpack('<II',m.vread(va,8))
        if w & 0x9f00001f != 0x90000001 or w2 & 0xffc003ff != 0xf9400021:
            continue
        imm=((w>>29)&3)|(((w>>5)&0x7ffff)<<2)
        if imm & (1<<20):imm-=1<<21
        selref=(va&~4095)+(imm<<12)+(((w2>>10)&0xfff)*8)
        selector=m.string(m.ptr(selref))
        if selector in wanted:
            stubs[va]=(selector,selref)
    # Pinned old app function table; no guessed function boundaries.
    target={0x1004aa890,0x1004ad314}
    functions=[]
    tab=args.inputs/'app168/ghidra/functions.tsv'
    for line in open(tab):
        a,b,size,name=line.rstrip('\n').split('\t',3)
        if int(a,16) in target:
            functions.append((int(a,16),int(b,16),name))
    rows=[]
    for start,end,name in functions:
        for i,(w,) in enumerate(struct.iter_unpack('<I',m.vread(start,end-start))):
            if w & 0xfc000000 != 0x94000000:
                continue
            pc=start+i*4
            imm=w&0x3ffffff
            if imm&(1<<25):imm-=1<<26
            dest=pc+(imm<<2)
            if dest not in stubs:continue
            selector,selref=stubs[dest]
            cls=('ArcBrowserApplicationConfiguration' if selector.startswith('init:argv:') else
                 'ArcPerformanceController' if selector=='setArcPerformanceObserver:' else
                 'ArcCrashKeysReporter' if selector=='setArcCrashKeysObserver:' else 'ArcBrowserApplication')
            rows.append(dict(image=binary.name,app_function_va=hex(start),app_function=name,
                             call_va=hex(pc),call_file_offset=hex(m.offset(pc)),bl_word=hex(w),
                             stub_va=hex(dest),stub_file_offset=hex(m.offset(dest)),selector_ref_va=hex(selref),
                             selector=selector,receiver_association=cls,
                             association_basis='old_app_pseudocode_receiver_flow_not_runtime_type_proof',
                             pseudocode='app168/ghidra/decomp/0x1004.c',
                             pseudocode_context_line=126553 if start==0x1004aa890 else 128994))
    assert {r['selector'] for r in rows}==wanted, {r['selector'] for r in rows}
    tsv(args.out/'app168-launch-calls.tsv',list(rows[0]),rows)
    stats=dict(app_sha256=digest(binary),app_function_table_sha256=digest(tab),
               app_pseudocode_sha256=digest(args.inputs/'app168/ghidra/decomp/0x1004.c'),
               verified_bl_call_sites=len(rows),distinct_selectors=len({r['selector'] for r in rows}),
               pointer_formats=sorted(m.pointer_formats))
    (args.out/'app-launch-counts.json').write_text(json.dumps(stats,indent=2)+'\n')
    print(json.dumps(stats,indent=2))


if __name__=='__main__':main()
