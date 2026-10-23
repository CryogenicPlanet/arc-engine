#!/usr/bin/env python3
"""Human review decisions for the completed fresh audit; local outputs only."""
import csv,json
from pathlib import Path
HERE=Path(__file__).resolve().parent
REV=Path('/home/exedev/work/agents/spec-3/revisions/v1')
PRIOR=Path('/home/exedev/work/agents/spec-2/classreview')
def read(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def write(name,fields,rs):
 with (HERE/name).open('w') as f:
  w=csv.DictWriter(f,fieldnames=fields,delimiter='\t');w.writeheader();w.writerows(rs)
rows=read(REV/'methods.tsv');changes=json.loads((HERE/'evidence/annotation-changes.json').read_text())
# Every changed row was read; below are additional explicitly read declaration samples.
spots={
'ArcAIDistilBertClassifier':['0x443a824'],
'ArcAccessibilityActionData':['0x43dec58'],
'ArcAccessibilityTreeNode':['0x443ac38'],
'ArcAutocompleteMatch':['0x4439ba8'],
'ArcBackForwardList':['0x43dfaf0'],
'ArcBookmarkTreeNode':['0x43e05dc'],
'ArcBrowserApplicationDebugProperty':['0x43e6408'],
'ArcBrowserContextDevTools':['0x43f8f28'],
'ArcBrowserCreateParams':['0x4401138'],
'ArcCommandBarRankingTensor':['0x4432208'],
'ArcConfirmInfobar':['0x443b434'],
'ArcDevToolsConnectionDialog':['0x4402950'],
'ArcDownloadItem':['0x442fdd8'],
'ArcFreezingVoteToken':['0x443eb94','0x283ebf0'],
'ArcHistoryItem':['0x442e408'],
'ArcKeyboardAccelerator':['0x4405fc8'],
'ArcMemoryDumpProvider':['0x4407a04'],
'ArcMenuModel':['0x444075c'],
'ArcNomicEmbedTextItem':['0x442d568'],
'ArcNotificationButton':['0x4408ac4'],
'ArcOverscrollBehavior':['0x440a7b4'],
'ArcOverscrollParams':['0x43eae2c'],
'ArcPasteAndGoClassification':['0x442d13c'],
'ArcSelectOption':['0x442bf94'],
'ArcSessionTab':['0x443cd44'],
'ArcWebContentsAIAgent':['0x4440c34'],
'ArcWebContentsConfiguration':['0x4414cb0'],
'ArcWebContentsNavigationController':['0x43fe218'],
}
key=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
changed={key(x['after']) for x in changes};sample=[]
for r in rows:
 if key(r) in changed or r['imp_va'] in spots.get(r['class_name'],[]):
  sample.append(dict(image='ArcCore-1.170.0.arm64',address_basis='unslid Mach-O VA',**{k:r[k] for k in ('class_name','category','kind','selector','imp_va')},body='evidence/bodies/'+f"{int(r['imp_va'],16):08x}.txt",reason='changed annotation reviewed in full' if key(r) in changed else 'analogous/unmodified declaration spot-check'))
write('evidence/manual-method-sample.tsv',list(sample[0]),sample)
notes={
'ArcAIDistilBertClassifier':('Signed short numClasses and engagement guard agree.','Model loading/inference, native copy helpers and borrowed storage invalidation remain outside this sample.'),
'ArcAccessibilityActionData':('Optional value checks collapse absent and empty strings to nil, unlike SelectOption.','Action names, aggregate calling convention and consumer effects remain unknown.'),
'ArcAccessibilityTreeNode':('children boxes contiguous 32-bit IDs; no recursive wrapper construction is claimed.','Snapshot producer, copy-helper closure and update subscription/sequence are not recovered here.'),
'ArcAutocompleteMatch':('associatedKeyword returns cached wrapper or wraps a native result; source comparison remains provisional.','Providers, mutation/cache invalidation and underlying native match lifetime are not established.'),
'ArcBackForwardList':('Native setter replaces optional shared state without refreshing ObjC arrays in the shown body.','Backend ordering, snapshot consistency and navigation identity remain open.'),
'ArcBookmarkTreeNode':('liveChildren wraps native children and returns a copied collection; absent state can return nil.','Bookmark model lifetime and deletion races remain unknown; collection copy is not a deep-copy guarantee.'),
'ArcBrowser':('Finding 1 corrected; direct completion and pending-vector storage are separate branches. browserCreateParams preserves runtime return handling.','Live completion timing, exactly-once behavior, queue, before-unload decisions and Chrome Browser shutdown lifetime remain unknown.'),
'ArcBrowserApplication':('Finding 2 corrected; only first initialized termination request stores callback state. Wrapper return guard accepts 0..2.','Public return enum names, repeated-completion fate, first delivery and global shutdown ordering remain unknown.'),
'ArcBrowserApplicationDebugProperty':('Native setter has separate assignments for both inline C++ strings.','Producer key vocabulary, aggregate ABI, live binding and producer lifetime are not resolved.'),
'ArcBrowserContextDevTools':('Extensions equality compares native optional identity; six base plus two category declarations remain distinct.','Invalid close(int) recovery remains labeled; frontend ownership, close completion and register ABI remain unknown.'),
'ArcBrowserCreateParams':('pipSize engagement guards and the unusual second native field offset are visible and correctly qualified.','PiP layout/calling convention, platform window/view ownership and capability consumers remain open.'),
'ArcColor4f':('Finding 5 corrected; constructor and void destructor share single-ret code, with no initialization or cleanup stores.','Float aggregate input/return ABI and color-space/range semantics remain unverified.'),
'ArcCommandBarRankingTensor':('shape boxes int32 entries in order after optional engagement check.','Shape/data consistency, native copy helper, inference consumer and mutable-storage lifetime remain unknown.'),
'ArcConfirmInfobar':('accept retains engagement and indirect-dispatch observations while flagging the false socket prototype.','Actual receiver/register ABI, action consumption, dismissal and exactly-once semantics remain unverified.'),
'ArcDevToolsConnectionDialog':('accept checks cancellation before indirect invocation; false socket prototype stays qualified.','Mutual exclusion/consumption across actions, persisted disable policy and executor remain unknown.'),
'ArcDownloadItem':('cancel checks engagement/live backend and forwards its bool argument through a virtual slot.','Source API comparison does not resolve the binary vtable, native item lifetime or shutdown behavior.'),
'ArcErrorStatus':('Finding 6 corrected; shared single-ret constructor returns self unchanged and void destructor performs no cleanup.','Producer enum meanings, initial value before explicit initialization and aggregate ABI remain open.'),
'ArcFreezingVoteToken':('clearVote requires engagement and non-null token; native accessor returns wrapper storage address, not an ObjC weak load.','Unregistration helper closure, graph shutdown, borrowed pointer invalidation and required sequence remain open.'),
'ArcHistoryItem':('lastVisitTime offset and special-value conversion agree with the pinned Windows-epoch comparison.','This is an epoch inference, not a binary type match; persistence/producer and native lifetime remain unverified.'),
'ArcHtmlToMarkdownResult':('All five changed object getter rows preserve retain/autorelease annotations and class-specific field identity.','Return handling establishes no independent copy; extraction producer, deep element copying and threading remain outside scope.'),
'ArcImage':('Changed full decode row agrees with wrapper, adapter and helper: early native completion and input copy before task dispatch.','Ultimate executor/decoder closure, malformed-image behavior, resource limits and teardown lifetime remain unknown.'),
'ArcKeyboardAccelerator':('Initializer widens key and sign-extends the modifiers field as described.','Key/modifier enum meanings and native event/menu consumers are not established.'),
'ArcMemoryDumpProvider':('dealloc clears wrapper/native pointer before unregistering and destroying provider, then calls super.','Registration identity remains a candidate; outstanding completion and teardown races are not established.'),
'ArcMenuModel':('items lazily builds and caches wrappers; source MenuModel lifecycle comparison is scoped as inference.','Vtable identity, dynamic invalidation and show/close ordering are not established.'),
'ArcNomicEmbedTextItem':('Text/task initializer substitutes empty native strings for nil and engages storage.','Task vocabulary, native copy helper closure, service behavior and mutable pointer lifetime remain open.'),
'ArcNotification':('Finding 3 corrected; tagged-result parser followed by direct dispatch, nil click input and initial close fallback agree.','Intended text-input/fallback behavior, full platform schema, native callback lifetime and delivery thread remain unknown.'),
'ArcNotificationButton':('placeholder setter warning identifies objc_storeStrong; this differs from tab-group copy setters.','Platform response producer and caller mutation/lifetime policies are not established by field accessors.'),
'ArcOverscrollBehavior':('Initializer stores zero in both directional scalar fields after super init.','Enum meaning and renderer/native consumer behavior remain unknown.'),
'ArcOverscrollParams':('Setter annotation identifies objc_storeStrong for supplied behavior object; no copy is established.','Post-application mutation effects and renderer policy/sequence remain unresolved.'),
'ArcPasteAndGoClassification':('destinationURL checks optional engagement then forwards the second native string to conversion.','Classification producer, URL policy and native string copy-helper closure are not reconstructed.'),
'ArcSelectOption':('value distinguishes absent optional string from present string, including empty conversion; wrapper must be engaged.','Aggregate ABI, native copy helper and option producer/control action remain open.'),
'ArcSessionTab':('lastActiveTime uses the same epoch offset/special-value family as HistoryItem, consistent with pinned source comparison.','Session type identification is provisional; persistence, current-index/cache invalidation and native lifetime remain open.'),
'ArcTabGroupInfo':('Three changed setter rows retain nonatomic-copy annotations; class prose and individual rows now agree.','No deep element copy is established; enum meanings, producer defaults and session-ID ordering remain open.'),
'ArcTabGroupResult':('Two changed setter rows retain nonatomic-copy annotations independently of shared IMP aliases.','Error vocabulary, group-ID producer and command-result semantics remain unproved.'),
'ArcWebContentsAIAgent':('sendKeyPress checks optional engagement and forwards to native helper; no enforced UI-thread dispatch is asserted.','Deeper key/IME/focus behavior, weak native view transport and lifetime remain outside this fresh sample.'),
'ArcWebContentsConfiguration':('Changed void destructor is single ret; analogous constructor actually writes defaults, including initiallyHidden=true.','Packed aggregate ABI, NaN/rounding of forced-size conversion and consumer enforcement remain unresolved.'),
'ArcWebContentsNavigationController':('Finding 4 corrected; binary/runtime-symbol resolution corroborates weak retained load and autoreleased return; setter is weak.','Caller retention, concurrent weak changes, teardown and four missing exact observer bodies remain unresolved.'),
}
classrows=[]
for cl in sorted(notes):
 rr=[r for r in rows if r['class_name']==cl];ss=[r for r in sample if r['class_name']==cl];assert ss
 vas=','.join(dict.fromkeys(r['imp_va'] for r in ss))
 evidence=f"ArcCore-1.170.0.arm64 unslid Mach-O VAs {vas}; {len(rr)} declarations, metadata/layout/body provenance pass (evidence/decoded-methods.tsv, evidence/layout-checks.tsv, evidence/body-checks.tsv); fresh sample: evidence/manual-method-sample.tsv. {notes[cl][0]}"
 classrows.append(dict(class_name=cl,status='pass_with_limits',evidence=evidence,limits=notes[cl][1]))
write('class-review.tsv',['class_name','status','evidence','limits'],classrows)
engine='ArcCore-1.170.0.arm64, unslid Mach-O VA; '
dispositions=[
(1,'resolved_with_limits',engine+'0x43e1190 -> 0x42fa5c8; evidence/bodies/043e1190.txt and 042fa5c8.txt; evidence/instructions.tsv 0x42fa6d0..0x42fa6f0 direct invoke, 0x42fa7a4..0x42fa7f8 pending-vector copy; adapter 0x43e3394..0x43e33c0 reaches block invoke directly. Revised class/matrix/corrections row 1 agree.','Live-path timing, queue, exactly-once delivery, before-unload cancellation and teardown lifetime remain unknown; adapter lacks exact exported C header.'),
(2,'resolved_with_limits',engine+'0x42b2430 stores callback at native +0x150..+0x168 only on first initialized request; repeated path logs/returns 2. Wrapper 0x43e6ee0 checks 0..2 and destroys local transport. evidence/bodies/042b2430.txt, 043e6ee0.txt, 04044e80.txt, 04044ec0.txt; mapped instructions/runtime symbols and callable table checked. Revised class/matrix/corrections row 2 agree.','Enum names, first completion timing, repeated completion fate outside these bodies and shutdown/executor behavior remain unknown.'),
(3,'resolved_with_limits',engine+'0x440a1f8 -> 0x473f138; evidence/bodies/0440a1f8.txt and 0473f138.txt. Mapped instructions 0x440a220/224/228 use x8 result and x0 response; 0x440a260 sets x3=nil. Parser tags/direct dispatch and initial close fallback agree with revised class/matrix/corrections row 3.','Whether nil text input/unknown-action close are intended, full schema, platform delivery/thread and native callback lifetime remain unknown; displayed C is not a callable ABI.'),
(4,'resolved_with_limits',engine+'0x43fe200 calls weak-retained load at 0x43fe20c and autoreleased return at 0x43fe214; runtime veneers and owner property 0xe2a97b0 independently resolved. evidence/bodies/043fe200.txt, evidence/instructions.tsv and evidence/layout-checks.tsv. Revised class/matrix/corrections row 4 agree.','Caller retention, returned-object lifetime across teardown and concurrent changes remain unproved. Zeroing-weak declaration/runtime operation is distinct from a naked borrowed load.'),
(5,'resolved',engine+'0x15ac0 is word d65f03c0 (ret), with unchanged x0 and no stores; ArcColor4f .cxx_construct encoding @16@0:8 and distinct void destructor verified from binary. evidence/bodies/00015ac0.txt, evidence/instructions.tsv, evidence/decoded-methods.tsv. Revised class/matrix/corrections row 5 agree.','No initialization stores occur in this IMP; allocation/explicit initialization and float aggregate ABI are separate, unverified questions.'),
(6,'resolved',engine+'0x15ac0 is word d65f03c0 (ret), with unchanged x0 and no stores; separate ArcErrorStatus .cxx_construct @16@0:8 and void destructor identities verified. evidence/bodies/00015ac0.txt, evidence/instructions.tsv, evidence/decoded-methods.tsv. Revised class/matrix/corrections row 6 agree.','Pre-explicit-init field values, producer status/error enums and aggregate ABI remain outside this no-op observation.'),
]
write('finding-dispositions.tsv',['finding_row','status','evidence','remaining_unknown'],[dict(zip(('finding_row','status','evidence','remaining_unknown'),x)) for x in dispositions])
write('findings.tsv',['severity','class_name','selector','claim','evidence','required_correction'],[])
assert len(notes)==len(classrows)==37 and len(changed)==21
print(json.dumps(dict(classes=len(classrows),changed_rows=len(changed),manual_declaration_sample=len(sample),sample_distinct_imps=len({r['imp_va'] for r in sample}),dispositions=len(dispositions),new_or_remaining_findings=0),indent=2))
