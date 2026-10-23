# Progress residual audit, run6 (read only)

No edits to production/native/shared source, build, app, board or new runtime.
Frozen batch results/core-runtime/20261023T125302Z is used throughout.
Exact first-WC events preserved in /tmp/core-progress-run6-streams.json. Caller
filter is the comparator's app calls + engine setters; no engine getter is
counted. Initial0 is constructor publication, not proof of a native0 receipt.

## Proven original receipt and delivery

Original ArcNative WCO vtable da81b40+d0 contains438a184, a thunk subtracting
Impl0xa8 into producer4389a3c. Producer requires managedWC130, stores input d0
in cachedprogress340 at4389a68, rereads d8 at4389ab4, then captures that same double
into EACH queued observer task+48 at4389fa4. Posting dispatch is438a06c-438a080 (ordinary zero-delay
UI task via TLS dispatcher). It posts a separate trailing batch finalizer at
4389ca0. There is no policy eligibility or navigation-generation check here.
Task4365e70 reloads payload d0 at4365eb4 and dispatches observer member; ObjC bridge
4413540 weak-loads wrapper at4413558, forwards captured double to setLoadProgress:
at441356c. It does not re-query live progress at delivery or quantize/synthesize.

Evidence /tmp/core-wco-native-vtable.txt, /tmp/core-wco-producers-audit2.txt,
/tmp/core-progress-original-delivery.txt, /tmp/core-wco-bridge-selectors.txt.

## Native coalescing exists in original AND current Chromium

Found original native source by string xrefs in the actual binary:
- d1dddea `WebContentsImpl::DidChangeLoadProgressForMainFrame`, xref27af864/868;
  actual function27af430 (thunk27af428).
- d1dd6b7 `WebContentsImpl::SendChangeLoadProgress`, xref27afdc8/dcc;
  actual function27af910.

At27af548-58c original reads last-update timestamp+9a0 and interval+9a8, compares
elapsed time, and immediately sends on first-update, progress0, progress1 or
elapsed interval. Otherwise27af590-5a4 checks for an outstanding weak task and
returns when already scheduled;27af5a8-660 posts delayed SendChangeLoadProgress
with nativeWC weak pointer and interval, not a copied intermediate double.
27af674-6c8 invalidates prior native weak tasks before immediate send27af6d0;
progress1 then resets page progress and timestamp at27af6e8-778. Send27af910 reads
CURRENT native page progress (d8) and invokes synchronous native WCO notification.
Therefore intermediate values may be replaced/coalesced before ARC receives them.
The original stored interval's DEFAULT value was not independently recovered;
this audit does not call that default100ms based only on the newer source.

Current pinned native source has the same mechanism:
- content/browser/web_contents/web_contents_impl.cc:10691-10750: eligible primary
  frame, immediate0/1/first/elapsed, weak-task coalescing, delayed task reads current
  progress. Interval DEFAULT100ms at web_contents_impl.h:2474-2476.
- SendChangeLoadProgress:9453-9460 records timestamp and synchronously notifies
  WCO with GetLoadProgress(); ResetLoadProgressState:9463-9467 sets native page0
  and invalidates pending source tasks.
- Blink progress_tracker.cc:185-225 calculates intermediate progress from parsing,
  paint and bytes, with value-delta0.02 or elapsed0.1s notification threshold.
  SendFinalProgress:124-129 emits1 unless its progress value is already1.
- FrameTreeNode::DidStopLoading:674-684 separately sends final1; RFH::DidChange-
  LoadProgress:8962-8973 accepts equal values (only rejects smaller), and native
  progress reset after each1 permits repeated1 notifications.

This proves native intermediate-event counts CAN vary with timing in both
engines. It does NOT prove timing is the actual cause of any particular run6
extra.7 or the missing forward1. Terminal1 bypasses native timer coalescing IF it
arrives at the native WC handler; Blink/frame-tree lifecycle determines whether
one or two such source events exist. BFCache/native path differences remain a
possible explanation, not an observed fact in these boundary-only records.
Evidence /tmp/core-progress-native-source-map.txt,
/tmp/core-progress-original-native-gates.txt,
/tmp/core-progress-original-native-coalescing.txt.

## Current owned policy buffer has real conditional loss paths

arc/api/arc_web_contents.mm:
-396-403: native progress is appended when awaiting nav start/policy, otherwise
  published synchronously. Outside init649 (constant0), ALL assignments403/494
  use a received double; refreshNativeState720-753 does not publish progress.
  Thus ours extra.7 setters are not fabricated by a core calculation.
-367-376: DidStartLoading clears existing pending events when !awaiting_policy.
  Native WCO contract explicitly permits consecutive DidStartLoading callbacks
  (web_contents_observer.h:413-424). A buffered progress event between those
  callbacks can therefore be discarded. This is a conditional source defect;
  this audit did NOT observe that exact receipt sequence in run6.
-295: superseding primary start while awaiting policy clears pending events.
-338-340: matching finish while awaiting policy clears pending events.
-260-264: CompleteNavigationPolicy moves events then discards them on denial,
  deadnative or generation mismatch;266-269 can also return before flushing after
  a synchronous URL/KVO callout changes native/generation.
-487-495: PublishPendingState checks native and generation before EACH element.
  Loading/progress KVO can change the generation; remaining already-received
  progress is then dropped. This differs from original receipt-time independent
  task snapshots when original weak wrapper/registration remains alive.

These are genuine code paths capable of losing received progress, NOT proof that
one specifically lost the forward1 in this run. No native receipt/enqueue/drop
log was collected in run6, and no simulation was used to label its cause.

The four-part generic+observer companion removes ALL policy buffers and these
clear/generation filters. Its staged WC:400-405 captures the scalar and enqueues
one PostCoreUI per native receipt; delivery weak-loads owner and applies original
value without requiring nativeWC or generation. This removes the demonstrated
owned conditional-drop behavior, while leaving native source coalescing intact.
That is the already-reviewed actionable owned fix. No extra isolated patch or
constant/progress synthesis is supported before integration and receipt evidence.

## Exact run6 boundary streams (first WC)

Tabs goldenWC115 / oursWC99. Entries value@rawseq. Golden path is
 goldens/tabs-navigation/launch-1/record.jsonl, ours tabs-navigation/launch-1/record.jsonl.

operation | golden | ours
initial/a | 0@2917,.7@3872,1@3886,1@3892 | 0@3124,.7@3972,1@3986,1@3992
a->b | .1@5924,1@6194,1@6200 | .1@6100,.7@6362,1@6376,1@6382
back | .1@6424,1@6690,1@6696 | .1@6562,1@6790,1@6876
forward | .1@6888,1@7136,1@7142 | .1@6958,1@7184
reload | .1@7232,1@7366,1@7372 | .1@7424,.7@7570,1@7576,1@7582
hash | .1@7574,1@7816,1@7822 | .1@7786,1@8056,1@8062
redirect->b | .1@8000,1@8300,1@8306 | .1@8230,.7@8540,1@8554,1@8560

Forward: golden commit7094,finish7134,1@7136,1@7142,stop(false)7148.
Ours1@7184,stop7196,commit7232,finish7272; no further1 until next reload.1@7424.
Back is NOT missing a1: ours1@6790,stop6802,second1@6876,commit6882,finish6922.
Nav callbacks are posted but progress/loading mostly synchronous in current code,
so boundary commit/finish order cannot identify native receipt ordering.

Tabs firstWC22->24 =3extra.7 minus1missing1; three othertabs each add1extra.7,
so complete scenario31->36. Every setter fromengine; no caller-stack/address or
receipt stage in transcript. No policy-denial or generation-drop event is logged.

Launchslice L1 firstWC118/102: both initial0,.7,1,1; a->b golden.1@4094,1@4364,
1@4370 vsours.1@4364,.7@4582,1@4596,1@4602; hash andsame-docback both.1,1,1.
L1 firstWC13->14. WholeL1 16->14 additionally lacks replacement's3 events.
Launch2 WC97/78: golden0@2523,1@3298,1@3304 vsours0@2762,.7@3566,1@3580,
1@3586. No terminal1 is missing in either launchslice operation.

## Prospective paired receipt probe (NOT performed)

Use the same disposable scenario/fixture and record three stages independently:
1. Native coalescer decision/send (original27af430/27af910; current native
   DidChangeLoadProgressForMainFrame/SendChangeLoadProgress): timestamp, native
   WC existing identity, input/current page progress, elapsed/interval, direct vs
   pending/coalesced branch. Do not call public getters or create wrappers.
2. Arc WCO receipt/enqueue (original4389a3c and copied task payload; ours
   LoadProgressChanged): per-WC receiptsequence,value, native/owner existing
   identity, navigation ID/served-from-BFCache flags at native finish, and current
   pending generation/state at receipt. For old current code, record each clear/
   generation-return with the actual discarded receiptsequence(s).
3. ObjC application (original4413540 vs ours queued weak-owner setter): receipt
   sequence/value, owner/registration presence. Keep raw boundary recorder intact
   and use a separate diagnostic channel; no additional setters/probe callbacks.

Then compare receipt->delivery conservation perWC, before comparing counts across
runs. Equal native receipts but lost owned delivery proves adapter loss; absent
native receipts plus recorded native coalescer/lifecycle branch proves source
variation. Boundary-only missing1 cannot choose between those cases. A probe on
the integrated four-part bundle is more useful than new simulation of retired
policy buffers. No allowlist or normalization change is justified by this audit.
