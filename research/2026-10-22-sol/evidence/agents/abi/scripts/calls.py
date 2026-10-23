#!/usr/bin/env python3
"""Stream app pseudocode; resolve ObjC stub selectors against app binary.

Rows prove app dispatch of a selector, not a particular engine IMP/receiver.
Exact BL VAs are independently read from pristine app executable. Caller
attribution uses a matching pseudocode function plus its saved min/end range;
min/end ranges are not treated as contiguous Ghidra body-byte maps.
"""
import bisect, collections, csv, json, re, struct
from inventory import ROOT,OUT,DATA,MachO,write_tsv

HEADER=re.compile(r'^// ==== (0x[0-9a-f]+)  (.+)  \((\d+) bytes\)')
CALL=re.compile(r'\b([A-Za-z_][A-Za-z_0-9]*)::([A-Za-z_][A-Za-z_0-9]*)\s*\(')

def functions(path):
    h=None; body=[]
    for ln,line in enumerate(path.open(),1):
        m=HEADER.match(line)
        if m:
            if h:yield h,''.join(body)
            h=dict(va=m[1],name=m[2],body_byte_count=int(m[3]),header_line=ln,path=str(path));body=[]
        elif h:body.append(line)
    if h:yield h,''.join(body)

def stub_selector(app,va):
    """Decode the first ADRP x1 / LDR x1 pair of an arm64 ObjC selector stub."""
    a,b=struct.unpack('<II',app.read(va,8))
    assert a&0x9f00001f==0x90000001,(hex(va),hex(a))
    imm=((a>>29)&3)|(((a>>5)&0x7ffff)<<2)
    if imm&(1<<20):imm-=1<<21
    page=(va&~4095)+(imm<<12)
    assert b&0xffc003ff==0xf9400021,(hex(va),hex(b))
    selref=page+(((b>>10)&0xfff)*8)
    return app.string(app.pointer(selref)),selref

def engine_label(r):
    return r['kind']+'['+r['class_name']+('('+r['category']+')' if r['category'] else '')+' '+r['selector']+']@'+r['imp']

def main():
    app=MachO(ROOT/'binaries/Arc-1.168.0.arm64')
    methods=list(csv.DictReader((OUT/'all-methods-170.tsv').open(),delimiter='\t'))
    selmap=collections.defaultdict(list)
    for r in methods:
        if r['class_name'].startswith('Arc'):selmap[r['selector']].append(r)
    engine_classes={r['class_name'] for r in methods if r['class_name'].startswith('Arc')}
    contract=list(csv.DictReader((OUT/'contract.tsv').open(),delimiter='\t'))
    imported={r['class_name'] for r in contract}
    bounds={}
    for line in (ROOT/'app168/ghidra/functions.tsv').open():
        p=line.rstrip('\n').split('\\t')
        if len(p)!=4:p=line.rstrip('\n').split('\t')
        bounds[int(p[0],16)]=(int(p[1],16),int(p[2]),p[3])
    paths=sorted((ROOT/'app168/ghidra/decomp').glob('*.c'))
    # ObjC stubs fall in 0x1035*.c in this input; don't assume all text is a stub.
    stubs={}; selector_only=[]
    for path in paths:
        if path.name not in ['0x1035.c']:continue
        for h,body in functions(path):
            if not h['name'].startswith('objc_stub::'):continue
            alias=CALL.search(body)
            if not alias:continue
            quoted=re.findall(r'"([^"\n]+)"',body)
            if len(quoted)!=1:continue
            sel=quoted[0]
            if sel not in selmap:continue
            decoded,ref=stub_selector(app,int(h['va'],16));assert decoded==sel,(h,decoded,sel)
            stubs[alias[2]]=dict(h,selector=sel,selector_ref=hex(ref),binary_selector_verified=True)
    used=[]; seen_selectors=set(); counts=collections.Counter(); unmatched=[]
    imported_c=[x.strip() for x in (DATA/'app168-imports.txt').open() if x.strip() and not x.startswith('_OBJC_CLASS_$_')]
    # Named external C stubs have exact VAs in supplied stubs.txt.
    cstubs={}
    for line in (ROOT/'app168/dump/stubs.txt').open():
        a,s=line.split(maxsplit=1);s=s.strip()
        if s in imported_c:cstubs[s]=int(a,16)
    for path in paths:
        for h,body in functions(path):
            if h['name'].startswith('objc_stub::') or '__stub' in h['name']:continue
            eligible=[m for m in CALL.finditer(body) if m[2] in stubs and (m[1]=='objc_stub' or m[1] in engine_classes)]
            found=collections.Counter(m[2] for m in eligible)
            directc={s:len(re.findall(r'(?<![A-Za-z_0-9])'+re.escape(s)+r'\s*\(',body)) for s in imported_c}
            if not found and not any(directc.values()):continue
            start=int(h['va'],16)
            if start not in bounds:unmatched.append(h);continue
            end,bytecount,_=bounds[start]
            assert end>=start and end-start<16*1024*1024
            code=app.read(start,end-start)
            targets=collections.defaultdict(list);tails=collections.defaultdict(list)
            # Scan only saved bounds for functions already confirmed in source.
            # Gaps may exist: source evidence and matching target are required.
            for i in range(0,len(code)-3,4):
                ins=struct.unpack_from('<I',code,i)[0]
                if ins&0xfc000000 not in [0x94000000,0x14000000]:continue
                off=ins&0x3ffffff
                if off&0x2000000:off-=1<<26
                (targets if ins&0xfc000000==0x94000000 else tails)[start+i+off*4].append(hex(start+i))
            for alias,n in found.items():
                stub=stubs[alias];sel=stub['selector'];vas=targets[int(stub['va'],16)];bvas=tails[int(stub['va'],16)]
                first=next(m for m in eligible if m[2]==alias)
                line=h['header_line']+1+body[:first.start()].count('\n')
                candidates=selmap[sel]
                # Provide context, but do not infer a receiver from a caller name.
                context=body[max(0,body.rfind('\n',0,first.start()-1)-500):min(len(body),first.end()+250)]
                row=dict(evidence_kind='app_dispatch_binary+app_pseudocode' if vas or bvas else 'app_pseudocode_dispatch_only',
                         app_version='1.168.0',app_caller_va=h['va'],app_caller_name=h['name'],
                         app_caller_min=hex(start),app_caller_end=hex(end),app_caller_body_bytes=bytecount,
                         caller_bounds_caveat='min/end_may_include_gaps;matching_source_caller_required',
                         app_file=h['path'],app_line=line,selector=sel,app_stub_va=stub['va'],app_stub_alias=alias,
                         app_selector_ref=stub['selector_ref'],binary_bl_vas=';'.join(vas),binary_tail_b_vas=';'.join(bvas),source_namespace=first[1],
                         source_call_count=n,binary_bl_count=len(vas),engine_candidates=';'.join(engine_label(r) for r in candidates),
                         engine_candidate_count=len(candidates),imported_class_candidates=';'.join(sorted({r['class_name'] for r in candidates if r['class_name'] in imported})),
                         receiver_status='unresolved_dynamic_dispatch',engine_implementation_called='not_proven',
                         context_json=json.dumps(context),stub_source=f"{stub['path']}:{stub['header_line']}")
                used.append(row);seen_selectors.add(sel);counts[sel]+=1
            for s,n in directc.items():
                if not n:continue
                pos=re.search(r'(?<![A-Za-z_0-9])'+re.escape(s)+r'\s*\(',body).start()
                used.append(dict(evidence_kind='app_C_import_call_binary+app_pseudocode' if targets[cstubs.get(s,-1)] else 'app_C_import_call_pseudocode_only',
                     app_version='1.168.0',app_caller_va=h['va'],app_caller_name=h['name'],app_caller_min=hex(start),app_caller_end=hex(end),
                     app_caller_body_bytes=bytecount,caller_bounds_caveat='min/end_may_include_gaps;matching_source_caller_required',
                     app_file=h['path'],app_line=h['header_line']+1+body[:pos].count('\n'),selector=s,app_stub_va=hex(cstubs[s]) if s in cstubs else '',
                     app_stub_alias=s,app_selector_ref='',binary_bl_vas=';'.join(targets[cstubs.get(s,-1)]),binary_tail_b_vas=';'.join(tails[cstubs.get(s,-1)]),source_namespace='',source_call_count=n,
                     binary_bl_count=len(targets[cstubs.get(s,-1)]),engine_candidates='C_export:'+s,engine_candidate_count=1,
                     imported_class_candidates='',receiver_status='C_function_symbol',engine_implementation_called='import_symbol_confirmed;runtime_binding_not_observed',
                     context_json=json.dumps(body[max(0,pos-150):pos+250]),stub_source='app168/dump/stubs.txt'))
    for alias,s in stubs.items():
        selector_only.append(dict(selector=s['selector'],app_stub_va=s['va'],app_stub_alias=alias,
             app_selector_ref=s['selector_ref'],binary_selector_verified=True,app_source=s['path'],app_line=s['header_line'],
             caller_rows=counts[s['selector']],status='app_caller_evidence' if counts[s['selector']] else 'selector_stub_only_no_caller_found',
             engine_candidates=';'.join(engine_label(r) for r in selmap[s['selector']])))
    bysel=collections.defaultdict(list)
    for r in used:bysel[r['selector']].append(r)
    for r in contract:
        calls=bysel[r['selector']];r['call_evidence_count']=len(calls)
        r['usage_status']='app_dispatch_selector_seen_receiver_unresolved' if calls else ('selector_stub_only' if r['selector'] in {x['selector'] for x in stubs.values()} else 'declared_only')
        r['receiver_proof']='unresolved;selector_match_is_not_class_usage' if calls else 'none'
    write_tsv(OUT/'contract.tsv',contract)
    write_tsv(OUT/'used-calls.tsv',used)
    write_tsv(OUT/'app-selector-stubs.tsv',selector_only)
    summary=dict(app_pseudocode_shards=len(paths),app_engine_selector_stubs=len(stubs),
       unique_objc_selectors_with_callers=len(seen_selectors),used_call_rows=len(used),
       evidence_counts=dict(collections.Counter(r['evidence_kind'] for r in used)),
       contract_status_counts=dict(collections.Counter(r['usage_status'] for r in contract)),
       contract_methods_with_selector_callers=sum(bool(bysel[r['selector']]) for r in contract),
       distinct_imported_classes_with_selector_candidates=len({r['class_name'] for r in contract if bysel[r['selector']]}),
       unmatched_function_bounds=unmatched,nonclass_import_stubs={k:hex(v) for k,v in cstubs.items()})
    (OUT/'calls-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))

if __name__=='__main__':main()
