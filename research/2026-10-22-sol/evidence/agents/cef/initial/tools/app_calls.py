#!/usr/bin/env python3
"""Find binary BL instructions to selector-resolved ObjC app stubs.

This proves selector dispatch call sites, not dynamic receiver class. Saved
APP-layer pseudocode supplies the stub/selector association; binary BL targets
verify calls independently. Does not treat stub existence as a call.
"""
import collections, csv, json, pathlib, re, struct
from inventory import MachO, ROOT, INPUT, write_tsv

rows=list(csv.DictReader((ROOT/'inventory.tsv').open(),delimiter='\t'))
selectors={r['selector'] for r in rows}
stubs={}
for path in sorted((INPUT/'app168/ghidra/decomp').glob('*.c')):
    address=0; in_stub=False
    for line_no,line in enumerate(path.open(errors='replace'),1):
        m=re.match(r'// ==== (0x[0-9a-f]+)\s+(.*?)\s+\((\d+) bytes\)',line)
        if m:
            address=int(m[1],16); in_stub=m[2].startswith('objc_stub::')
        if in_stub:
            for sel in re.findall(r'"([^"\n]+)"',line):
                if sel in selectors:
                    stubs[address]=(sel,str(path),line_no)

app=MachO(INPUT/'binaries/Arc-1.168.0.arm64')
stub_start,stub_size=app.sections['__objc_stubs']
for address,(selector,_,_) in stubs.items():
    assert stub_start<=address<stub_start+stub_size
    adrp,ldr,_,_,branch=struct.unpack_from('<5I',app.mem,app.offset(address))
    assert adrp&0x9f00001f==0x90000001  # ADRP x1
    assert ldr&0xffc003ff==0xf9400021   # LDR x1,[x1,#unsigned_offset]
    assert branch==0xd61f0200           # BR x16
    imm=((adrp>>29)&3)|(((adrp>>5)&0x7ffff)<<2)
    if imm&(1<<20):imm-=1<<21
    selector_ref=(address&~0xfff)+(imm<<12)+(((ldr>>10)&0xfff)*8)
    assert app.string(app.ptr(selector_ref))==selector,(hex(address),selector)
base,length=app.sections['__text']; offset=app.offset(base)
calls=[]
for i in range(0,length-3,4):
    word=struct.unpack_from('<I',app.mem,offset+i)[0]
    if word&0xfc000000!=0x94000000: continue
    imm=word&0x3ffffff
    if imm&0x2000000:imm-=0x4000000
    target=base+i+imm*4
    if target not in stubs:continue
    sel,path,line_no=stubs[target]
    calls.append(dict(app_image='Arc-1.168.0.arm64',app_call_va=hex(base+i),
                      app_call_file_offset=hex(offset+i),app_stub_va=hex(target),
                      selector=sel,stub_source=path,stub_source_line=line_no,
                      receiver_class_status='unproven',instruction='BL'))
write_tsv(ROOT/'evidence/app-direct-selector-calls.tsv',calls)
counts=collections.Counter(r['selector'] for r in calls)
samples=collections.defaultdict(list)
for r in calls:
    if len(samples[r['selector']])<3:samples[r['selector']].append(r['app_call_va'])
for row in rows:
    sel=row['selector']
    row['app_selector_bl_calls']=counts[sel]
    row['app_selector_bl_samples']=','.join(samples[sel])
    row['app_direct_call_status']='selector_dispatch_receiver_unproven' if counts[sel] else 'not_established'
write_tsv(ROOT/'inventory.tsv',rows)
summary=dict(inventory_methods=len(rows),resolved_matching_stubs=len(stubs),
             binary_validated_stub_selectors=len(stubs),
             binary_bl_calls=len(calls),distinct_called_selectors=len(counts),
             declared_rows_sharing_called_selector=sum(bool(counts[r['selector']]) for r in rows),
             proven_class_selector_calls=0,
             limitation='No receiver-class recovery; no indirect dispatch or branch-thunk recovery. Shared selectors can target unrelated classes.')
(ROOT/'evidence/app-call-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
