# Attachment consumer follow-on audit, source only

Keep core_window_attachment.patch HOLD. No production/native edits or runtime
claim. Fresh read-only audit by core_async_review after run6.

Original normal path is synchronous:
473ad7c ->4398404 ->43981ac ->window lookup/bind ->unfreeze ->43741e8.
4374208 writes native state immediately. Observer member0x38 then posts delivery
to4413584, weak-loads ObjC owner and calls setState:. One queued app-delivery hop,
plus the observer batch's distinct trailing finalizer.

Held staged WC line561 posts native work, then line588 posts state. That extra
hop changes native/browser/window liveness checks and acceptance timing. There
is no source proof for replacing the original native work with a generic
NonNestableTask. The native transport forbids synchronous app/KVO calls, not all
native work. A strictly gated native-only normal path can remain synchronous.

Potential minimal subset must establish receipt-time facts:
- Authoritative native state0, independent of queued owner.state.
- Live native WC/current browser and live nonnil incoming window.
- Original global native-window match selects that current owner.
- Owner already bound to that same window (binding call43c8b18 is then a no-op).
- Independent Impl+270 branch at43981d0 known inactive or implemented.

That subset avoids tab transfer, reload and rebinding. It can perform native
unfreeze synchronously, followed by one queued observer notification. Current
helper has no producer of authoritative captured native state; current registry
filters only core browsers rather than original native collection; weak-window
rebinding differs from original once-set field semantics. Do not activate it.
Transfers additionally require independent reentrant rollback/lifetime handling.

New sender gate: nil-window detach does not enter4398404. Retained windowx22 is
tested at473ad18; nil uses interface+18 at473ace4, whereas attachment uses+10 at
473ad7c. Transport emits nil moves too, so a consumer must distinguish these
routes. Original source /tmp/core-state-{host,attach,host-senders}.txt and
/tmp/core-attach-*.txt; held stages /tmp/core-window-attachment-stage and
/tmp/core-window-stage. No approximate queue or native-state workaround applied.

## Independent fullscreen and native-state fields

Fresh source audit resolves Impl+0x270 as fullscreen-transition display
suppression, independent of native state+0x290 and freezing votes. Constructor
43717ac zeros it through an overlapping store at Impl+26f. Host-view fullscreen
notifications in vtable da81918 slots+30/+38/+40/+48 resolve to43988a4/4398900,
setting/clearing Impl270 and the RenderWidgetHostViewMac flag+641. That flag
gates CALayer delivery at41683dc. Current Chromium has no proven equivalent
flag/API. It is not safe to infer false from the public state or an ordinary
window styleMask.

The exact bypass at43981d0 is Impl270=false OR incoming window already has
NSWindowStyleMaskFullScreen. When the flag is true and the incoming window is
not fullscreen, native code clears the flag and calls RWH slot+1e0. A window
between fullscreen notifications can be ordinary while the flag is still true.

Authoritative state290 starts at0 at43717f8; setter4374208 writes it before
observer tasks. Confirmed writers are attachment43983bc->0, replacement43980d0
->1, freeze4374f14/4374f60->2, and event436c5a8->4374940 (frozen->2, else0,
subject to native lifecycle/identity gates). The event producer is now resolved
below. A model initialized0 without all those writers would
produce false attachment eligibility, rather than a faithful normal-path gate.

Owned emitFreezingVote: delegates native vote creation to4335f4c; the app-owned
ArcFreezingVoteToken owns revocation/destruction. Its lifecycle must participate
in any future complete state model. Current ArcContentsData stores identity;
the public init state0 and visibility/crash-derived state cannot establish the
native receipt-time facts. Consumer remains HOLD; no production change.

## Resolved PageNode lifecycle producer

Original436c434 is PageNodeObserver::OnPageLifecycleStateChanged. Constructor
436c0ec registers its vtable da80bb8, slot+80, with PM and contains literal
../../arc/browser/web_contents/page_node_observer.cc. Page getter9fcdec4 reads
node360, the same field received at436c5a4. FrozenFrameAggregator174e39c selects
active_frame_count!=0 && frozen_frame_count==active_frame_count; its Boolean is
stored at9fcef40, then observer+80 is notified at9fceffc. This independently
proves original value1 means frozen; no cross-version enum-ordinal assumption.

Exact receipt route/gates:
1. Weak event WC must survive, and event node must equal that WC's primary page.
2. Traverse to outermost WC and find its existing Arc native owner.
3. Owner's current native binding must equal the ORIGINAL event WC; state1 skips.
4. Primary main RFH must exist and IsRenderFrameLive() must succeed.
5. Select2 if frozen, otherwise0; invoke native state setter only if changed.

Original RFH1d926b0 has the identical process initialized/not-dead plus frame
created predicate, at vtable d8e2030+230. Its neighboring lifecycle getter+238
reads separate RFH1108. Do not substitute IsActive or a lifecycle enum. Outermost
getter415e89c follows WC1e0, matching native event traversal436c53c.

Current named counterparts are GetPrimaryPageNodeForWebContents,
GetOutermostWebContents, IsRenderFrameLive, and PageNodeObserver's lifecycle
callback. PerformanceManager APIs are main-thread and notify inline. Native
authority therefore updates at receipt without an extra task; only app/KVO
publication is queued, preserving observer-batch accounting.

This closes the event-source/gate proof but does not activate a partial model.
Logical state must survive replacement and receive all freeze/discard/attachment
writers; sessions already owns an ACTIVE discard category in
arc_sessions_restore.mm:100. Do not add a duplicate selector or treat it as a
stub. App-owned freezing-token destruction and independent fullscreen270 still
need coordination. Durable original binary hash, exact LLDB commands and traces:
logs/wp/core.page-lifecycle-identity/{manifest.json,native.txt}.
