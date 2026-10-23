import csv,json,re
from pathlib import Path
from collections import Counter
OUT=Path('/home/exedev/work/agents/spec-2/revision-reviews/v1');REV=Path('/home/exedev/work/agents/spec-2/revisions/v1');PRIOR=Path('/home/exedev/work/agents/spec-1/classreview')
def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def write(name,fields,data):
 with (OUT/name).open('w') as f:
  w=csv.DictWriter(f,fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(data)
methods=rows(REV/'methods.tsv');findings=rows(PRIOR/'findings.tsv');ledger=rows(REV/'corrections.tsv')
observations={
1:'Object encoding @24@0:8^@16 and NSMutableIndexSet construction/copy agree; native unsigned values >=4 are skipped; failure converts NSError and returns nil; engagement trap retained.',
2:'NSMutableDictionary receives converted element key/value pairs and is copied after iteration, including the no-entry path.',
3:'Absent optional returns the empty NSArray singleton; engaged native string entries are converted/added and the array is copied; invalid nonzero engagement traps.',
4:'Engagement guard, temporary suggestion vector, UTF-16 element conversion and returned NSMutableArray are visible; no copy message is sent.',
5:'Successful store of saved x2 at 0x43ecb90 adopts the incoming owner; zero at 0x43ecb9c targets old owner, and 0x43ecbe4 belongs to failed-init cleanup. No caller move guarantee is retained.',
6:'0x43ece30 directly calls 0x24e39c0 with reason zero; 0x43ece40 resets saved callable state and 0x43ece58 indirectly transfers to saved cleanup. Native-state guards are retained.',
7:'Pseudocode cleanup/reset precedes apparent reuse and constructor mapping has bool-cast objects/in_stack values. Allocations establish intent only. Related aggregate getter/copy bodies and bounded instructions do not recover a safe contents/lifetime contract; revision explicitly leaves it unresolved.',
8:'Input param_10 is enumerated and each untyped element receives kind/value; this path contains no concrete ArcMetaTagSelector allocation/class test. Revision labels that DTO a compatible candidate.',
9:'DTO initializer stores kind and explicitly copies value, supporting a compatible shape; conversion enumeration does not prove concrete receiver identity. Both relationship prose and annotation are qualified.',
10:'Encoding separates native aggregate and ObjC owner. Raw 0x441a1dc/0x441a1e0 save x3/x2 separately; loads through saved x2 populate handle slots, saved x3 reaches weak-store at 0x441a268. Unused-param_4/aggregate-owner artifact is now explicit.',
11:'Raw 0x441c3d8/0x441c3dc save x3/x2 separately; loads through saved x2 populate handle slots, saved x3 reaches weak-store at 0x441c464. Encoding and explicit signature artifact agree; caller ABI remains qualified.',
12:'Exact body contains Type propagation algorithm not settling and displayed extra argument; row, class prose and methods.tsv all flag type/argument semantics as untrusted while retaining available export status.',
13:'Exact body independently contains Type propagation algorithm not settling and displayed extra argument; row, class prose and methods.tsv all flag type/argument semantics as untrusted while retaining available export status.'}
dispositions=[]
for i,(old,c) in enumerate(zip(findings,ledger),1):
 r=next(r for r in methods if (r['class_name'],r['selector'])==(old['class'],old['selector']))
 ev=f"ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48, unslid VA {r['imp_va']}; {r['body_path']}@{r['body_offset']}+{r['body_length']}; evidence/corrected-bodies.txt. {observations[i]} Revised classes/{r['class_name']}.md, methods.tsv and corrections.tsv data row {i} agree."
 if i in [1,5,6,7,10,11]:ev+=' evidence/instructions.txt (fresh copied-image reads; selected ranges, not function-boundary certification).'
 if i==7:ev+=' evidence/analogous-bodies.txt: native getter/copy helper/input comparison.'
 dispositions.append(dict(finding_row=i,status='resolved' if i<=4 else 'resolved_with_limits',evidence=ev,remaining_unknown=c['remaining_unknown']))
write('finding-dispositions.tsv',['finding_row','status','evidence','remaining_unknown'],dispositions)
# Exact per-class local sample and a fresh, deliberately bounded conclusion.
info={
'ArcAIInferenceConfiguration':('0x443a318','Initializer stores int32 token limit and narrows streaming interval to int16, then engages inline storage.','Token ranges, service consumer behavior and borrowed-pointer lifetime remain unknown.'),
'ArcAIInferenceModel':('0x4439f8c','Initializer stores discriminator and converts directory descriptor; this sample is a value adapter.','Loader validation, model identity and resource lifetime remain unknown.'),
'ArcAIService':('0x43d9ab8 0x43d9fd4','Both ranking operations enumerate examples and capture completion; unsettled-type warnings and extra-argument limitations are explicit. Original findings 12/13 corrected.','All 20 Extensions operations and callback adapters were not freshly traced transitively; result/error/token, queue and cancellation remain unknown.'),
'ArcAIServiceConfiguration':('0x43dcbdc','Type -1 produces NSError and nil; path/string conversion, retry-byte narrowing and timeout store agree with class description.','Valid type/range policy and nil NSError-out use remain unknown; displayed ABI is not certified.'),
'ArcAnnotatedVisit':('0x43ff558 0x43ff438','relatedSearches now describes copied array/empty fallback; analogous categories getter allocates wrappers and copies its array. Finding 3 corrected.','Most of 63 getter/setter semantics were not freshly audited statement by statement; producer mapping, conversion edge cases and freshness remain unknown.'),
'ArcAutocompleteResult':('0x43df204','Initializer stores the two ObjC collections after NSObject init; revision preserves ownership-fixup caveat.','Element classes, ordering, collection mutability and copy/retain details remain unknown.'),
'ArcBrowserContextAttributes':('0x443b028 0x443b0f4','Getter lazily caches ArcColor at self+8; setter forwards self+0x10. Fresh helper 0x441e0a0 changes native fields without writing cache slot.','No app-use, profile persistence or cache invalidation policy beyond this assignment path is established.'),
'ArcBrowserContextAutocomplete':('0x43ecb50 0x43ece08','Body and instructions support adoption without successful-source-zeroing, direct stop helper and separate callable cleanup. Findings 5/6 corrected.','Caller move/owner ABI, null-owner validity, exact upstream identity, completion ordering and queue remain unknown.'),
'ArcBrowserContextID':('0x43ed454','Engagement checked; isIncognito compares stored int32 discriminator with one.','Discriminator meanings beyond one, basename policy and profile creation behavior remain unknown.'),
'ArcColor':('0x4401b58','Init category preserves four input bytes in local aggregate passed to initWithColor:. Category identity preserved.','Consumer packing, premultiplication and complete aggregate-return ABI remain outside this sample.'),
'ArcCommandBarRankingEvaluation':('0x4434190','Engaged getter builds dictionary from converted names and boxed numeric values; collection return is described.','Boxed FP argument ABI, metric meanings and service result production remain unverified.'),
'ArcCommandBarRankingExample':('0x4401f34 0x4401ca0 0x443366c 0x4433528','candidates is explicitly semantically unresolved; native-copy helper and analogous input show conversion intent without settling temporary lifetime. Finding 7 is resolved by honest limitation.','Candidate contents/order, temporary ownership, constructor argument recovery and selectedIndex validation remain unknown; no engine bug or always-empty result is established.'),
'ArcContentConversionService':('0x43de074','Conversion enumerates untyped objects and dispatches kind/value. Fresh 0x42a75b4 shows unavailable completion branch, callback/request storage and Mojo interface string. Finding 8 corrected.','Concrete element producer/class, algorithm/library, callback outputs/queue and service ownership remain unknown.'),
'ArcContextMenuElementInfo':('0x443f654 0x443f58c','Suggestions getter now returns mutable array of converted UTF-16 elements; analogous misspelledWord returns scalar string. Finding 4 corrected.','Suggestion producer/order, coordinate space, enum names and mutation expectations remain unverified.'),
'ArcDisplayNameOrOrigin':('0x441cefc','Getter selects short/long native string representation and invokes UTF-16 conversion helper.','Formatting, escaping, origin/trust policy and producer semantics remain unknown.'),
'ArcExtensionInfo':('0x442f330','Engaged permissions getter converts native UTF-16 elements into NSMutableArray, consistent with its collection annotation.','Extension execution, service producer, permission localization, icon ownership and snapshot freshness remain unknown.'),
'ArcExtensionToolbarAction':('0x443bd08','Absent optional returns nil; present branch calls NSNumber integer factory. Missing displayed numeric argument remains a decompiler limit.','Actual numeric passing/index semantics, producer/action path and snapshot lifetime remain unverified.'),
'ArcGPULogMessage':('0x443ab84','Header getter converts short/long native byte-string storage; metadata assigns shared implementation to this class.','Severity domain, GPU-feed source and ordering remain unknown; shared IMP name is not receiver identity.'),
'ArcHistogramSamples':('0x44032d8','Destructor clears 37 object slots; full declaration inventory and matrix contain 39 rows.','Fresh semantic sample is destructor only; the 37 initializer-source branches/units were not re-proven individually or dynamically.'),
'ArcHistoryDeletion':('0x442e88c','Engaged getter constructs array of boxed native visit IDs; reading the notification payload does not itself prove a deletion action.','Deletion producer/timing, cached items, restricted URL relation and absent-range meaning remain unknown.'),
'ArcHistoryQueryOptions':('0x4403a34','Duplicate-policy getter accepts native unsigned values 0..2 and traps otherwise; source QueryOptions is a domain comparison.','Signed-edge setters, date defaults/NaN handling and executed query behavior were not exhaustively re-audited.'),
'ArcImportSource':('0x4403e9c','Initializer sign-extends source ID, gates source type with bitmask/range and reads translation table.','Enum labels and backend importer identity remain unknown; no import execution established.'),
'ArcKeyEventData':('0x43def08','Initializer explicitly copies code/key strings and stores event type/keycode scalars.','Event consumer, layout/modifier policy and dispatch timing remain unknown.'),
'ArcLoginCredentials':('0x8374f3c','Password getter checks engagement, calls helper and converts temporary native UTF-16 storage.','No secure-storage, encryption, erasure or complete helper contract inferred; producer/origin/lifetime remain unknown.'),
'ArcMetaTagSelector':('0x43dd458','Initializer stores scalar kind and copies value; conversion relationship now limited to compatible shape. Finding 9 corrected.','Concrete conversion receiver dataflow, kind enum and matching policy remain unknown.'),
'ArcNavigationAction':('0x4408554 0x4408564','Type values above four normalize to one; disposition requires unsigned value below twelve or traps.','Enum names, actual navigation policy and producer timing remain unknown.'),
'ArcNetworkActivityMonitor':('0x4408898 0x432c834','isLogging forwards through engaged handle; string predicate accepts INITIALIZING/STARTING_LOG/LOGGING, differing from pinned upstream STOPPING_LOG inclusion.','Exact upstream function identity, stop/timer/finalization and sequence/thread contract remain unverified.'),
'ArcNetworkLoggingConfiguration':('0x443c3e4','Default initializer resets native path storage and shows no duration assignment.','Allocation default duration, numeric/path validation, overwrite and finalization policy remain unknown.'),
'ArcNotificationItem':('0x43eae2c','Shared setter writes self+8; metadata assigns title setter despite foreign Ghidra class name. Ownership-fixup limits retained.','Exact setter copy/retain behavior and notification producer/delivery remain unverified.'),
'ArcRedirectEntry':('0x442ce90','Engaged aggregate getter copies two string representations and scalar tail into return storage.','Redirect enum, producer and pre/post-redirect timing remain unknown; displayed aggregate layout is not certified ABI.'),
'ArcSiteEngagementDetails':('0x442dc40','Engaged score getter loads double at self+0x20; upstream score class is cited only as domain context.','Score algorithm/range/units, refresh timing and profile service identity remain unknown.'),
'ArcTabGroupProperties':('0x443d9ec','Absent title returns nil; present title copies native string and converts to ObjC string. Source comparison distinguishes concrete upstream values.','Mutation consumer, independent field interaction, nil-vs-empty usage and color mapping remain unknown.'),
'ArcURLLoadConfiguration':('0x440d9f8 0x440db0c','Headers now returns copied dictionary, including empty case. timeout has empty decompilation, retained as signature artifact. Finding 2 corrected.','Actual loader, conversion edge cases, payload/file precedence, timeout and policy semantics remain unknown.'),
'ArcWebContentsDevTools':('0x4415b60 0x435c8fc 0x435ca2c','Extra correction supported: x3 error-out saved/forwarded; task copies callable state and later invokes it. Pinned CEF transport/Alloy constraints remain appropriately limited.','Executor identity, main-thread delivery, native pane ownership, reentrancy and queued-work suppression/draining remain unverified.'),
'ArcWebContentsMediaSession':('0x441a848 0x441a1c4 0x441aa94','Object index-set return differs from analogous BOOL pause action; constructor register map corroborates signature artifact. Findings 1/10 corrected.','Media enum names, complete producer/error behavior, caller ABI, volume, PiP lifetime and callback queue remain unknown.'),
'ArcWebContentsThumbnailsGenerator':('0x441c3c0 0x436df2c','Constructor registers corroborate aggregate/owner artifact; fresh key pointer read resolves ThumbnailTabHelper literal and backend lookup. Finding 11 corrected.','Caller ABI, capture policy, image ownership, observer timing and teardown remain unknown; no CEF lifecycle equivalence established.'),
'ArcWebLoginDialog':('0x441d5b4','Cancel indirectly calls the callable at self+0x58 with state self+0x48; revision retains distinct close/accept contracts and unknown ordering.','Callable producers, reentrancy/one-shot semantics, trust policy and presentation thread remain unknown.')}
assert set(info)=={r['class_name'] for r in methods}
bodyindex={}
for filename in ['corrected-bodies.txt','analogous-bodies.txt','unchanged-samples.txt','followup-bodies.txt','monitor-imp.txt']:
 for match in re.finditer(r'^INDEX (0x[0-9a-f]+) (.*?) path=(.*?) offset=(\d+) C_bytes=(\d+) failed=(\d+)$',(OUT/'evidence'/filename).read_text(),re.M):
  va,name,path,pos,n,failed=match.groups();bodyindex[va]=f'{va} {name}; {path}@{pos}+{n}; evidence/{filename}'
classrows=[]
for cls in sorted(info):
 addrs,observation,limit=info[cls];rs=[r for r in methods if r['class_name']==cls]
 ev=f'{len(rs)} exact class/category/kind/selector declarations and matrix rows; binary class/category membership, type/IMP/record, layout and body provenance checked in evidence/audit.py. {observation} ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48 unslid VAs: '+ ' | '.join(bodyindex[a] for a in addrs.split())
 classrows.append(dict(class_name=cls,status='reviewed_with_explicit_semantic_gap' if cls=='ArcCommandBarRankingExample' else 'reviewed_with_limits',evidence=ev,limits=limit+' Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.'))
write('class-review.tsv',['class_name','status','evidence','limits'],classrows)
write('findings.tsv',['severity','class_name','selector','claim','evidence','required_correction'],[])
print('Wrote 13 finding dispositions, 37 class reviews, header-only findings.tsv; exact exported semantic samples:',len(bodyindex))
