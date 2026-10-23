#!/usr/bin/env python3
"""Index newly delivered engine shards and corroborate limited runtime calls.

No recovered C prototype is promoted into the ABI contract. Pseudocode can
omit retain/store operations via Ghidra injections, so runtime calls are also
decoded from the engine binary and LC_DYSYMTAB import-stub mapping.
"""
import collections,csv,hashlib,json,re,struct
from inventory import ROOT,OUT,MachO,write_tsv
from calls import functions

def table(n):return list(csv.DictReader((OUT/n).open(),delimiter='\t'))

def main():
    paths=sorted((ROOT/'decompile').glob('*.c'))
    if not paths:
        (OUT/'engine-evidence-summary.json').write_text(json.dumps(dict(shards_seen=0,status='absent_no_wait'),indent=2)+'\n');return
    b=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64');stubs=b.import_stubs()
    requests=table('decompile-requests.tsv');contract=table('contract.tsv');callbacks=table('app-callbacks.tsv')
    proto=table('app-arc-protocol-methods.tsv');psels={r['selector'] for r in proto}
    bysel=collections.defaultdict(list)
    for r in proto:bysel[r['selector']].append(r)
    appby=collections.defaultdict(list)
    for r in callbacks:appby[r['selector']].append(r)
    helpers={hex(x) for x in [0x42b0850,0x42b17e8,0x42b1ba8,0x42b2430,0x43ec404,0x4332148,0x433260c,0x437b014]}
    wanted={r['imp'] for r in contract}|{r['engine_va'] for r in requests}|helpers
    found={};cbrows=[];signals=[];runtime_rows=[];scanned=0
    bounds={}
    for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
        p=line.rstrip('\n').split('\\t')
        if len(p)!=4:p=line.rstrip('\n').split('\t')
        bounds[int(p[0],16)]=(int(p[1],16),int(p[2]))
    def binary_runtime(h):
        start=int(h['va'],16)
        if start not in bounds:return []
        end,n=bounds[start];code=b.read(start,end-start);result=[]
        for i in range(0,len(code)-3,4):
            ins=struct.unpack_from('<I',code,i)[0]
            op=ins&0xfc000000
            if op not in [0x94000000,0x14000000]:continue
            off=ins&0x3ffffff
            if off&0x2000000:off-=1<<26
            target=start+i+4*off
            # lld arm64 range-extension islands are ADRP x16; ADD x16,x16;
            # BR x16. Follow only that exact form (or a direct B), bounded.
            chain=[];resolved=target
            for _ in range(4):
                if resolved in stubs:break
                try:a,c,d=struct.unpack('<III',b.read(resolved,12))
                except ValueError:break
                if a&0xfc000000==0x14000000:
                    imm=a&0x3ffffff
                    if imm&0x2000000:imm-=1<<26
                    chain.append(hex(resolved));resolved+=imm*4;continue
                if a&0x9f00001f!=0x90000010 or c&0xffc003ff!=0x91000210 or d!=0xd61f0200:break
                imm=((a>>29)&3)|(((a>>5)&0x7ffff)<<2)
                if imm&(1<<20):imm-=1<<21
                chain.append(hex(resolved));resolved=(resolved&~4095)+(imm<<12)+((c>>10)&0xfff)
            if resolved in stubs:
                result.append(dict(engine_caller_va=h['va'],engine_caller_name=h['name'],
                   instruction_va=hex(start+i),branch='BL' if op==0x94000000 else 'B',branch_target_va=hex(target),range_extension_islands=';'.join(chain),stub_va=hex(resolved),import_symbol=stubs[resolved],
                   attribution_limit='caller_min/end_may_include_gaps;source_function_also_required'))
        return result
    SEND=re.compile(r'\b_objc_msgSend\s*\(\s*([^,;]+),\s*"([^"\n]+)"')
    selected_path=OUT/'engine-selected.c'
    with selected_path.open('w') as excerpts:
        for path in paths:
            for h,body in functions(path):
                scanned+=1
                matches=[m for m in SEND.finditer(body) if m[2] in psels]
                if h['va'] not in wanted and not matches:continue
                runtime=binary_runtime(h)
                if h['va'] in wanted:
                    found[h['va']]=dict(h,body_sha256=hashlib.sha256(body.encode()).hexdigest())
                    excerpts.write(f"// SOURCE {path}:{h['header_line']}\n// ==== {h['va']}  {h['name']}  ({h['body_byte_count']} bytes)\n{body}\n")
                    runtime_rows.extend(runtime)
                    counts=collections.Counter(r['import_symbol'] for r in runtime)
                    signals.append(dict(engine_va=h['va'],engine_name=h['name'],engine_file=str(path),engine_line=h['header_line'],
                        binary_import_call_counts_json=json.dumps(dict(counts),sort_keys=True),
                        source_has_weak_runtime=bool(re.search(r'_objc_(initWeak|copyWeak|storeWeak|loadWeak|destroyWeak)',body)),
                        ghidra_injection_warning_count=body.count('replaced with injection'),
                        caution='availability_and_runtime_imports_only;prototype_ownership_and_callback_semantics_require_review'))
                for m in matches:
                    sel=m[2]
                    cbrows.append(dict(engine_version='1.170.0',app_version='1.168.0',selector=sel,
                        engine_caller_va=h['va'],engine_caller_name=h['name'],engine_file=str(path),
                        engine_line=h['header_line']+1+body[:m.start()].count('\n'),
                        evidence='engine_pseudocode_objc_msgSend_second_argument_matches_app_protocol',
                        binary_objc_msgSend_branch_vas=';'.join(r['instruction_va'] for r in runtime if r['import_symbol']=='_objc_msgSend'),
                        binary_attribution='branches_in_same_caller;individual_selector_to_instruction_not_assigned',
                        app_protocols=';'.join(sorted({r['protocol'] for r in bysel[sel]})),
                        app_callback_imps=';'.join(sorted({r['imp'] for r in appby[sel]})),
                        receiver_status='dynamic_receiver_and_execution_unobserved',
                        context_json=json.dumps(body[max(0,m.start()-150):min(len(body),m.end()+250)])))
    for r in requests:
        h=found.get(r['engine_va'])
        r.update(engine_pseudocode_status='available_indexed;questions_still_require_analysis' if h else ('data_export_not_pseudocode' if r['kind']=='data' else 'missing_from_delivered_shards'),
                 engine_pseudocode_file=h['path'] if h else '',engine_pseudocode_line=h['header_line'] if h else '')
        if h:r['requested_shard']=h['path']
    signal_byva={s['engine_va']:s for s in signals}
    for r in contract:
        h=found.get(r['imp'])
        r.update(engine_pseudocode_status='available_indexed_not_semantically_verified' if h else 'not_in_delivered_shards',
                 engine_pseudocode_file=h['path'] if h else '',engine_pseudocode_line=h['header_line'] if h else '')
        s=signal_byva.get(r['imp']);counts=json.loads(s['binary_import_call_counts_json']) if s else {}
        r['engine_runtime_import_counts_json']=json.dumps(counts,sort_keys=True)
        weak=any(counts.get(k,0) for k in ['_objc_initWeak','_objc_storeWeak','_objc_copyWeak'])
        block=bool(counts.get('_objc_retainBlock',0))
        r['ownership_threading']=('weak_runtime_operations_corroborated;' if weak else '')+('block_retain_operation_corroborated;' if block else '')+'full_lifetime_callback_ownership_and_threading_unresolved'
    write_tsv(OUT/'missing-engine-bodies.tsv',[dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],engine_va=r['imp'],expected_address_bucket=f"{int(r['imp'],16)>>20:#x}.c",status='no_header_in_delivered_engine_shards') for r in contract if not r['engine_pseudocode_file']],['class_name','category','kind','selector','engine_va','expected_address_bucket','status'])
    write_tsv(OUT/'decompile-requests.tsv',requests);write_tsv(OUT/'contract.tsv',contract)
    write_tsv(OUT/'engine-callback-selector-evidence.tsv',cbrows)
    write_tsv(OUT/'engine-runtime-calls.tsv',runtime_rows)
    write_tsv(OUT/'engine-request-signals.tsv',signals)
    write_tsv(OUT/'engine-pseudocode-index.tsv',list(found.values()))
    summary=dict(shards_seen=len(paths),engine_function_headers_scanned=scanned,requested_function_bodies_available=sum(r['engine_pseudocode_file']!='' for r in requests),
        contract_declarations_with_body_available=sum(bool(r['engine_pseudocode_file']) for r in contract),
        callback_dispatch_source_rows=len(cbrows),callback_selectors_in_engine_sends=len({r['selector'] for r in cbrows}),
        matched_app_protocol_selectors=len(psels),selected_engine_functions=len(found),runtime_import_calls_validated=len(runtime_rows),
        engine_source_files=[str(p) for p in paths])
    (OUT/'engine-evidence-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps({k:v for k,v in summary.items() if k!='engine_source_files'},indent=2))

if __name__=='__main__':main()
