#!/usr/bin/env python3
"""Render one row per guide identity and one declarative spec per protocol."""
import collections,json,re
from pathlib import Path
from audit import BASE,INPUT,load,tsv,guide
from encoding_types import declaration
from notes import PURPOSE,NOTES,DEFAULT_LIMIT,NEXT
def key(r):return r['protocol_name'],r['kind'],r['selector']
def esc(s):return str(s).replace('|','&#124;').replace('\n',' ')
def main():
    ps,identities,raw=guide();proto=json.loads((BASE/'evidence/app168-protocols.json').read_text());byproto={p['name']:p for p in proto};assert set(ps)==set(byproto)==set(PURPOSE)
    metadata=collections.defaultdict(list)
    for r in load(BASE/'evidence/app168-methods.tsv'):metadata[key(r)].append(r)
    impls=load(BASE/'evidence/app168-implementations.tsv');adopters=load(BASE/'evidence/app168-adopters.tsv');im=collections.defaultdict(list)
    for r in impls:
        for p in r['protocols'].split(';'):
            k=p,r['kind'],r['selector']
            if k in identities:im[k].append(r)
    source=collections.defaultdict(list);binary=collections.defaultdict(list)
    for r in load(BASE/'evidence/engine-source-sends.tsv'):source[r['selector']].append(r)
    for r in load(BASE/'evidence/engine-binary-sends.tsv'):binary[r['selector']].append(r)
    # Independently decoded headerless filename selector is a focus gate, even
    # if the conservative general binary recognizer fails to recover it.
    focus=load(BASE/'evidence/focus-selectors.tsv');fsel=next(r for r in focus if r['selector'].startswith('browserContext:onDeterminingFilename:'))
    sel=fsel['selector']
    if not any(r['send_va']=='0x404281c' for r in binary[sel]):binary[sel].append(dict(engine_va='',send_va='0x404281c',selector_ref_va=fsel['selector_ref_va'],evidence='focus_independent_selector_decode;vtable0xdd2bfe0->0x4042728'))
    toolbox=next(r for r in focus if r['selector']=='webContents:updateDevToolsToolbox:')
    binary[toolbox['selector']].append(dict(engine_va='0x4447e88',send_va='0x4447edc',selector_ref_va=toolbox['selector_ref_va'],evidence='focus_independent_selector_decode;d1_write_does_not_clobber_x1'))
    tsv(BASE/'evidence/engine-all-static-sends.tsv',[dict(selector=sel,**r) if 'selector' not in r else r for sel,rr in binary.items() for r in rr])
    apps={r['va']:r for r in load(BASE/'evidence/app-body-index.tsv')};callbacks=[];joins=[];typed=[]
    for k,g in identities.items():
        p,kind,sel=k;mm=metadata[k];assert mm and {m['required_optional'] for m in mm}=={g['required_optional']}
        encs=sorted({m['type_encoding'] for m in mm});ext=sorted({m['extended_type_encoding'] for m in mm});assert len(encs)==len(ext)==1
        enc,extended=encs[0],ext[0];decl=declaration(kind,sel,extended);value=PURPOSE[p][0]=='non_callback_value_protocol'
        typed.append(dict(protocol_name=p,kind=kind,selector=sel,type_encoding=enc,extended_type_encoding=extended,typed_declaration=decl,protocol_va=mm[0]['protocol_va'],method_entry_vas=';'.join(m['entry_va'] for m in mm),extended_type_slots=';'.join(m['extended_type_slot_va'] for m in mm),pairing=mm[0]['extended_pairing'],limits='No nullability, generics, enum names, ownership or queue inferred.'))
        implementations=[]
        for m in im[k]:
            body=apps.get(m['imp']);implementations.append(m['class_name']+(f"({m['category']})" if m['category'] else '')+'::'+kind+sel+' IMP='+m['imp']+' entry='+m['entry_va']+(f" body={body['path']}@{body['offset']}+{body['length']}" if body and body.get('path') else ';'+(body['status'] if body else 'no_exact_app_body')))
            erased=re.sub(r'"[^"]*"','',m['type_encoding'])
            while '<' in erased:erased=re.sub(r'<[^<>]*>','',erased)
            joins.append(dict(protocol_name=p,kind=kind,selector=sel,protocol_method_entries=';'.join(x['entry_va'] for x in mm),protocol_encoding=enc,app_class=m['class_name'],app_category=m['category'],app_class_va=m['class_va'],app_method_entry=m['entry_va'],app_imp=m['imp'],app_encoding=m['type_encoding'],raw_equal=m['type_encoding']==enc,app_erased_encoding=erased,erasure_equal=erased==enc,app_body_status=body['status'] if body else 'missing'))
        bs=binary[sel];ss=source[sel];arc_ss=[r for r in ss if 0x4000000<=int(r['engine_va'],16)<0x4500000 and r['boundary_limit']!='known_merged_reject']
        sends=';'.join(f"send={r['send_va']} selref={r['selector_ref_va']}" for r in bs)
        inv=('NON_CALLBACK_VALUE_GETTER; ' if value else '')+(('binary_static_selector_sends: '+sends) if bs else 'no_binary_send_corroborated_in_bounded_scan')
        inv+=f'; full_corpus_pseudocode_candidates={len(ss)}; Arc_area_candidates={len(arc_ss)}; see evidence/engine-source-sends.tsv by selector; dynamic receiver/execution unobserved'
        own='Not established for this method; typed declaration does not establish lifetime, thread, reentrancy or cardinality.'
        if p=='ArcWebContentsNavigationDelegate':own='0x4445a58/0x4445abc immediate synchronous return; unsigned 0..11 accepted; weak delegate/owner; setNavigationDelegate:nil clears native pair at 0x4375b40. Callback thread/queued work unknown.'
        elif sel.startswith('browserContext:onDeterminingFilename:'):own='Engine weak target/context; double from uint32; app0x1004e9748 copies completion; helper0x1004e9ac4 default-filename fallback; completion NSString*; timeout units/exactly-once/late result/thread unknown.'
        elif p=='ArcBrowserContextDownloadsDelegate':own='0x4041fec installs weak target/context; nil does not clear registration. Created/updated/removed bridges weak-load their targets. Snapshot/delta ordering, teardown and thread unknown.'
        elif sel=='didInitializeBrowserProcess':own='Native0x42b1200->0x42b13c4 names InvokeAsync; ObjC bridge0x43eb374 weak-loads target; full task-to-bridge linkage/queue and global cardinality unresolved.'
        elif p=='ArcWebContentsUIDelegate':own='0x444112c registers shared bridge with weak target/owner; native nil unregister0x4375cdc. See protocol spec for alert/confirm block retain and permission normalization; per-method cancellation/cardinality/thread unknown.'
        elif p=='ArcWebContentsThumbnailsGeneratorObserver':own='0x441c680 weak observer/owner registration;0x441cccc may deliver nil thumbnail. Coalescing, image validity, removal and thread unresolved.'
        elif p=='ArcWebContentsMediaSessionDelegate' and sel=='webContents:didChangeMediaPosition:':own='0x441c08c constructs ArcMediaPosition and weak-loads receiver/owner before send; units, retained lifetime, ordering and thread unknown.'
        elif value:own='Readonly NSString value getters; this is not event notification. Transfer/serialization lifetime and concurrent access unresolved.'
        question=NEXT.get(p,'Which payload/enum semantics, ownership, ordering, teardown/cancellation and callback sequence must be preserved?')
        if '@?' in enc:question+=' Completion shape is declared; verify nil/late/repeated completion, retention and execution sequence.'
        if not bs and not value:question+=' Establish an exact engine send and receiver path; absence in this bounded recognizer is not absence of use.'
        if value:question='Verify descriptor consumption and value/JSON/error semantics; do not implement these getters as event delivery.'
        callbacks.append(dict(protocol_name=p,kind=kind,selector=sel,required_optional=g['required_optional'],type_encoding=enc,metadata_status=f"app_binary_protocol_present;raw_rows={len(mm)};duplicate_encodings_agree;engine168_and170_no_Arc_descriptor_in_parsed_lists",app_implementation_evidence=';'.join(implementations) or 'no_static_implementation_found',engine_invocation_evidence=inv,ownership_thread_evidence=own,confidence='metadata_high;'+('non_callback_value_purpose_supported' if value else ('static_send_corroborated;receiver_route_and_full_semantics_partial' if bs else 'implementation_present;engine_invocation_unresolved')),unresolved_question=question,classification=PURPOSE[p][0],extended_type_encoding=extended,typed_declaration=decl,protocol_method_entry_vas=';'.join(m['entry_va'] for m in mm)))
    tsv(BASE/'callbacks.tsv',callbacks);tsv(BASE/'evidence/app-contract-joins.tsv',joins);tsv(BASE/'evidence/typed-declarations.tsv',typed)
    prows=[]
    for p in ps:
        q=byproto[p];cb=[r for r in callbacks if r['protocol_name']==p];ad=[r for r in adopters if p in r['all_arc_protocols'].split(';')];bs={r['send_va'] for m in cb for r in binary[m['selector']]};value=PURPOSE[p][0]=='non_callback_value_protocol'
        inherited=';'.join(q['inherited']);assert inherited==ps[p]['inherited_guide']
        prows.append(dict(protocol_name=p,purpose_classification=PURPOSE[p][0]+': '+PURPOSE[p][1],inherited_protocols=inherited,app_binary_corroboration=f"app168 protocol={q['va']};raw_rows={len(q['methods'])};unique_identities={len(cb)};all guide identities corroborated",engine_binary_corroboration=f"engine168/170 parsed protocol lists each contain 69 non-Arc descriptors and no Arc descriptor;new-engine static selector sends={len(bs)}; see evidence/engine-binary-sends.tsv",adopter_classes=';'.join(r['class_name']+(f"({r['category']})" if r.get('category') else '')+f" class={r['class_va']} ro={r['ro_va']}" for r in ad),evidence=f"evidence/app168-protocols.json:{p};evidence/app168-adopters.tsv;protocol-specs/{p}.md;guide_line={ps[p]['guide_line']}",unresolved_limits=('Value protocol, not engine-to-app notifications. ' if value else '')+DEFAULT_LIMIT))
        lines=[f'# {p}', '',PURPOSE[p][1]+'. Classification: `'+PURPOSE[p][0]+'`.', '',f"Arc app 1.168.0 arm64 protocol descriptor `{q['va']}`; inherits `{inherited or '(none)'}`. Pristine binary corroborates all {len(cb)} unique guide identities ({len(q['methods'])} physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.", '', 'Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.', '', '## Actual adopters', '']
        lines += [f"- `{r['class_name']}`: class `{r['class_va']}`, class_ro `{r['ro_va']}`, direct protocols `{r['direct_protocols']}`." for r in ad]
        lines+=['','Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.','','## High-value behavior and lifecycle','','\n\n'.join(NOTES.get(p,[])) or NEXT.get(p,DEFAULT_LIMIT),'',DEFAULT_LIMIT,'','## Typed declaration surface','', 'Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry\'s raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.','','| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |','|---|---|---|---|---|']
        for r in cb:
            appimps='; '.join(f"`{m['imp']}` (entry `{m['entry_va']}`)" for m in im[(p,r['kind'],r['selector'])]);ss='; '.join(f"`{m['send_va']}`" for m in binary[r['selector']]) or 'Uncorroborated; source candidates only' if source[r['selector']] else 'Uncorroborated in bounded scan'
            # Avoid expression-precedence ambiguity for binary-only headerless callbacks.
            if binary[r['selector']]:ss='; '.join(f"`{m['send_va']}`" for m in binary[r['selector']])
            lines.append('| '+' | '.join(esc(x) for x in [r['required_optional'],'`'+r['typed_declaration']+'`','`'+r['type_encoding']+'`',appimps,ss])+' |')
        lines+=['','Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.','','## Remaining questions','',NEXT.get(p,'Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.'),'','Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.','']
        (BASE/'protocol-specs'/f'{p}.md').write_text('\n'.join(lines))
    tsv(BASE/'protocols.tsv',prows)
    refs=[]
    for label in ['app168','engine168','engine170']:
        for cls,layout in json.loads((BASE/f'evidence/{label}-layouts.json').read_text()).items():
            for ivar in layout['ivars']:
                for name in re.findall(r'@"<(Arc\w+)>"',ivar['type']):
                    refs.append(dict(image=label,class_name=cls,protocol_name_reference=name,ivar_name=ivar['name'],ivar_metadata_entry=ivar['entry_va'],raw_type=ivar['type'],in_guide=name in ps,status='type_name_reference_only;no_protocol_descriptor_or_method_claim'))
    tsv(BASE/'evidence/protocol-type-references.tsv',refs)
    value=[r for r in callbacks if r['classification']=='non_callback_value_protocol'];candidate=[r for r in callbacks if r not in value]
    coverage=json.loads((BASE/'evidence/metadata-summary.json').read_text())
    coverage.update(protocol_specs=26,callback_rows=len(callbacks),guide_identity_unique=len({key(r) for r in callbacks}),non_callback_value_protocols=2,non_callback_value_identities=len(value),candidate_callback_or_app_service_protocols=24,candidate_callback_or_app_service_identities=len(candidate),identities_with_app_implementation=sum(bool(im[key(r)]) for r in callbacks),app_implementation_rows=len(joins),unique_app_callback_imps=len({r['app_imp'] for r in joins}),identities_with_binary_selector_send=sum(bool(binary[r['selector']]) for r in callbacks),candidate_callback_identities_with_binary_selector_send=sum(bool(binary[r['selector']]) for r in candidate),candidate_callback_identities_with_source_send=sum(bool(source[r['selector']]) for r in candidate),candidate_identities_without_binary_send=[dict(protocol_name=r['protocol_name'],selector=r['selector']) for r in candidate if not binary[r['selector']]],raw_app_implementation_encoding_differences=sum(r['raw_equal'] is False for r in joins),app_implementation_encodings_equal_after_annotation_erasure=all(r['erasure_equal'] for r in joins),semantically_complete_callbacks=0,runtime_callbacks_executed=0,full_abi_or_behavior_equivalence_established=False,scope='Guide-bounded static audit; no older code, dynamic execution, full reachability or maintenance cost estimate')
    (BASE/'coverage.json').write_text(json.dumps(coverage,indent=2)+'\n');print(json.dumps(coverage,indent=2))
if __name__=='__main__':main()
