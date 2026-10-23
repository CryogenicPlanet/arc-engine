#!/usr/bin/env python3
"""Create precise engine-decompile requests and reviewed app receiver examples."""
import collections,csv,json,re,struct
from inventory import ROOT,OUT,DATA,MachO,write_tsv

def table(name):return list(csv.DictReader((OUT/name).open(),delimiter='\t'))

def main():
    methods=table('all-methods-170.tsv'); calls=table('used-calls.tsv');contract=table('contract.tsv')
    details={x['class_name']:x for x in json.load((OUT/'arc-class-details-170.json').open())}
    reviewed=[
      ('0x1004aa890','init:','ArcBrowserApplication','explicit_class_allocation','Explicit &_OBJC_CLASS___ArcBrowserApplication followed by allocWithZone and init: stub; Ghidra omits argument recovery.'),
      ('0x1004ad314','initialize:','ArcBrowserApplication','typed_field_and_getter_context','Receiver loaded from ADKApplication lazy arcBrowserApplication storage after getter; APP Swift metadata names ArcBrowserApplication.'),
      ('0x1004ae240','run','ArcBrowserApplication','typed_field_and_getter_context','ADKApplication obtains its engine application and invokes run.'),
      ('0x1004ae4a0','tryToTerminate:','ArcBrowserApplication','typed_field_and_getter_context','Application termination wrapper passes a block; determine engine copy/lifetime and callback sequencing.'),
      ('0x1004ad0a0','createBrowser:withDelegate:forWindow:','ArcBrowserApplication','getter_context','Getter precedes dispatch; returned object is passed to ChromiumBrowser initializer. Register recovery is imperfect.'),
      ('0x1004af380','createBrowserContext:isHidden:type:completion:error:','ArcBrowserApplication','getter_context','Getter precedes dispatch; copied completion block is released after call; failure converts NSError.'),
      ('0x1004aefe4','downloads','ArcBrowserContext','typed_dictionary_and_registration_context','Context is stored in ADKApplication.contexts, then preferences/history/downloads delegates are installed; Swift context dictionary value is ArcBrowserContext.'),
      ('0x100525e6c','createWebContents:','ArcBrowser','typed_field','Receiver is ChromiumBrowser.browser; Swift metadata declares ArcBrowser. Returned object wraps in WebContentChromium.'),
      ('0x100525fec','takeOwnershipOfWebContents:','ArcBrowser','typed_fields','ChromiumBrowser.browser receives WebContentChromium.webView. Exact tail B to selector stub is recorded.'),
      ('0x10052604c','tryToCloseSkipBeforeUnloadHandlers:callback:','ArcBrowser','typed_field','ChromiumBrowser.browser receives copied close block; app releases its copied block after call.'),
      ('0x10052cf00','cancel:','ArcDownloadItem','typed_field','Receiver is ChromiumDownload.item; Swift metadata declares ArcDownloadItem.'),
      ('0x10052d0ec','pause','ArcDownloadItem','typed_field','Receiver is ChromiumDownload.item; no engine behavior inferred from app state updates.'),
      ('0x10052d2c8','resume','ArcDownloadItem','typed_field','Receiver is ChromiumDownload.item.'),
      ('0x10053b870','setNavigationDelegate:','ArcWebContents','typed_field','WebContentChromium.webView receives navigation-delegate registration; Swift metadata declares ArcWebContents.'),
      ('0x10054683c','setUIDelegate:','ArcWebContents','typed_field','WebContentChromium initializer registers self as UIDelegate on its webView; binary class protocol metadata independently records ArcWebContentsUIDelegate.'),
      ('0x10053fb24','devTools','ArcWebContents','typed_field','Receiver is WebContentChromium.webView; checks devTools result for nil. Getter return encoding alone is only @.'),
      ('0x1004f285c','openProtocolConnection:error:','ArcWebContentsDevTools','selector_and_wrapper_context_only','Copied incoming-message block, error-out handling, and connection object capture; receiver in unaff_x20 requires engine/static flow validation.'),
    ]
    examples=[]
    for va,sel,cls,proof,note in reviewed:
        found=[r for r in calls if r['app_caller_va']==va and r['selector']==sel];assert len(found)==1,(va,sel)
        r=found[0]
        cand=[m for m in methods if m['class_name']==cls and m['selector']==sel];assert len(cand)==1,(cls,sel)
        m=cand[0]
        examples.append(dict(app_caller_va=va,selector=sel,candidate_engine_class=cls,engine_imp=m['imp'],kind=m['kind'],category=m['category'],
            receiver_evidence=proof,engine_invocation_status='static_app_evidence;runtime_dispatch_and_engine_behavior_unobserved',
            app_file=r['app_file'],app_line=r['app_line'],binary_bl_vas=r['binary_bl_vas'],binary_tail_b_vas=r['binary_tail_b_vas'],note=note))
    write_tsv(OUT/'reviewed-app-boundaries.tsv',examples)
    for r in contract:
        d=details[r['class_name']]
        r.update(instance_start=d['instanceStart'],instance_size=d['instanceSize'],layout_evidence=f"arc-class-details-170.json:{r['class_name']}",
                 engine_pseudocode_status='absent_at_initial_stage',reviewed_app_receiver_evidence=';'.join(x['app_caller_va']+':'+x['receiver_evidence'] for x in examples if x['candidate_engine_class']==r['class_name'] and x['selector']==r['selector']))
    write_tsv(OUT/'contract.tsv',contract)
    plan=[]
    def add(cls,selectors,priority,topic,question,category=None):
        for sel in selectors:
            found=[m for m in methods if m['class_name']==cls and m['selector']==sel and (category is None or m['category']==category)]
            assert len(found)==1,(cls,sel,found)
            m=found[0]
            plan.append(dict(priority=priority,topic=topic,class_name=cls,category=m['category'],kind=m['kind'],selector=sel,engine_va=m['imp'],type_encoding=m['type_encoding'],question=question,
                app_evidence=';'.join(x['app_caller_va'] for x in examples if x['candidate_engine_class']==cls and x['selector']==sel),evidence_status='declared_method;call_evidence_separate'))
    add('ArcBrowserApplication',['init:','initialize:','run','tryToTerminate:','createBrowser:withDelegate:forWindow:','createPopupBrowser:withDelegate:','createBrowserContext:isHidden:type:completion:error:'],0,'bootstrap/lifecycle',
        'Identify Chromium entry points, initialization order, configuration translation, run-loop/thread requirements, delegate retention, synchronous returns vs completion, and shutdown ordering.')
    add('ArcBrowserApplication',['setNotificationCenter:','setHistogramSamplesDelegate:','setBreadcrumbCallback:','setDumpWithoutCrashingCallback:','setAccessibilityObserver:','.cxx_destruct'],1,'callbacks/ownership',
        'Does engine retain/copy or weakly reference argument? Which thread invokes callbacks? What cancels callbacks during teardown? Block signature and exactly-once behavior are unknown.')
    add('ArcBrowser',['initWithBrowser:','setDelegate:','setWindow:','createWebContents:','takeOwnershipOfWebContents:','tryToCloseSkipBeforeUnloadHandlers:callback:','.cxx_destruct'],0,'browser/window/ownership',
        'Map native Browser/window objects and ownership transfer, WebContents attachment/detachment, close veto ordering, delegate lifetime and callback thread. Trace bridges called by registration.')
    add('ArcBrowserContext',['initWithBrowserContext:','downloads','setDelegate:','createWebContents:','loadUrl:withDelegate:','cancelUrlLoad:','.cxx_destruct'],0,'context/lifetime',
        'Map BrowserContext/Profile lifetime, context deletion vs outstanding loads, returned wrapper identity, delegate ownership and callback cancellation; trace shared_ptr bridges.')
    add('ArcBrowserContextDownloads',['initWithBrowserContextDownloads:forBrowserContext:','setDelegate:','startDownload:error:','.cxx_destruct'],1,'downloads/delegates',
        'Trace DownloadManager observer registration, initial-list/create/update/remove ordering, filename completion timeout, delegate ownership, context teardown, and callback executor.')
    add('ArcDownloadItem',['pause','cancel:','resume','.cxx_destruct'],1,'download/lifetime',
        'Check invalidated DownloadItem behavior, cancel flag meaning, progress/update callbacks and whether wrappers outlive native items.')
    add('ArcWebContents',['initWithWebContents:','setNavigationDelegate:','setUIDelegate:','setActionsDelegate:','setTopBandColorDelegate:','executeJavascript:completion:','executeJavascript:reason:completion:','closeWithForegroundCommitTiming:','close','devTools','.cxx_destruct'],0,'webcontents/callbacks',
        'Trace underlying WebContents and delegate bridges, synchronous navigation-policy return, block/error shape, thread/actor expectations, close ordering and pending callback invalidation.')
    add('ArcWebContentsDevTools',['init:','setDelegate:','open','close','execute:completion:error:','subscribe:autoEnable:notification:','unsubscribe:','openProtocolConnection:error:','sendProtocolMessage:connectionID:error:','closeProtocolConnection:','.cxx_destruct'],1,'devtools/contracts',
        'Trace protocol connection/subscription lifetime, synchronous NSError vs async error, incoming-message and completion block signatures, retain cycles, detach/close behavior and callback thread.')
    add('ArcBrowserContextDevTools',['initWithBrowserContextDevTools:','close','webContents'],2,'context-devtools',
        'Distinguish context debugging from WebContents DevTools, wrapper ownership and endpoint shutdown.')
    engine=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
    for sym,typ,sect,desc,val in engine.symbols():
        if sym not in ['_CallWithEHFrame','_emitTraceEvent','_arcCategoryEnabledFlag','_ArcErrorDomain']:continue
        plan.append(dict(priority=0 if sym=='_CallWithEHFrame' else 2,topic='C-export/data-contract',class_name='',category='',kind='data' if sym=='_ArcErrorDomain' else 'C',selector=sym,engine_va=hex(val),type_encoding='',question='Recover actual C ABI, argument widths, exception/unwind behavior, callback/block ownership and threading; for ArcErrorDomain inspect constant value and storage ABI. App import alone does not give a prototype.',app_evidence=';'.join(sorted({r['app_caller_va'] for r in calls if r['selector']==sym})),evidence_status='binary_export+app_import'))
    eh=next(r for r in plan if r['selector']=='_CallWithEHFrame')
    val=int(eh['engine_va'],16);ins=struct.unpack('<I',engine.read(val,4))[0]
    assert ins&0xfc000000==0x14000000
    imm=ins&0x3ffffff
    if imm&0x2000000:imm-=1<<26
    plan.append(dict(priority=0,topic='C-export-tail-target',class_name='',category='',kind='C-helper',selector='_CallWithEHFrame tail target',engine_va=hex(val+4*imm),type_encoding='',question='Export is a four-byte direct B to this 24-byte helper; obtain missing 0x2.c function and verify callback invoke/unwind/exception behavior from instructions.',app_evidence='0x441d770:B',evidence_status='binary_direct_branch_target'))
    needed={int(r['engine_va'],16) for r in plan};gh={}
    for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
        p=line.rstrip('\n').split('\\t')
        if len(p)!=4:p=line.rstrip('\n').split('\t')
        if int(p[0],16) in needed:gh[int(p[0],16)]=p
    for r in plan:
        va=int(r['engine_va'],16);p=gh.get(va)
        r.update(image='ArcCore-1.170.0.arm64',address_basis='unslid_engine_MachO_VA',
            engine_file_offset=hex(engine.offset(va)),ghidra_start=p[0] if p else '',ghidra_end=p[1] if p else '',
            ghidra_body_byte_count=p[2] if p else '',ghidra_qualified_name=p[3] if p else '',
            ghidra_match='exact_start' if p else ('data_export_not_function' if r['kind']=='data' else 'no_exact_function_start'),
            address_bucket_1MiB=f'{va&~0xfffff:#x}-{(va&~0xfffff)+0x100000:#x}',
            requested_shard='engine shard containing exact start;shard naming unavailable',
            coverage_limit='Ghidra min/end not a contiguous body map;also request direct bridge callees and destructor/callback targets')
    write_tsv(OUT/'decompile-requests.tsv',sorted(plan,key=lambda r:(r['priority'],int(r['engine_va'],16),r['selector'])))
    # Broader Arc method delta protects against the imported-class lower bound.
    old={tuple(r[k] for k in ['class_name','category','kind','selector']):r for r in table('all-methods-168.tsv')}
    arcchanges=[]
    for r in methods:
        if not r['class_name'].startswith('Arc'):continue
        prev=old.get(tuple(r[k] for k in ['class_name','category','kind','selector']))
        if not prev or prev['type_encoding'] and prev['type_encoding']!=r['type_encoding']:
            arcchanges.append(dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],type168=prev['type_encoding'] if prev else '',type170=r['type_encoding']))
    write_tsv(OUT/'arc-method-changes.tsv',arcchanges,['class_name','category','kind','selector','type168','type170'])
    exportsets={v:set((DATA/f'core{v}-exports.txt').read_text().splitlines()) for v in ['168','169','170']}
    summary=dict(reviewed_app_boundaries=len(examples),decompile_requests=len(plan),request_function_exact_starts=sum(r['ghidra_match']=='exact_start' for r in plan),
        request_missing_function_starts=[r['selector'] for r in plan if r['ghidra_match']=='no_exact_function_start'],
        arc_methods_170=sum(r['class_name'].startswith('Arc') for r in methods),arc_known_method_changes=len(arcchanges),
        arc_unknown_168_encodings=sum(r['class_name'].startswith('Arc') and not r['type_encoding'] for r in old.values()),
        exports={v:len(s) for v,s in exportsets.items()},export_names_equal=exportsets['168']==exportsets['169']==exportsets['170'])
    (OUT/'requests-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))

if __name__=='__main__':main()
