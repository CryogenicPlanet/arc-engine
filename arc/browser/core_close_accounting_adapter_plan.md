Core accounting adapter plan review — UNAPPLIED composition only
================================================================

Review outcome
--------------
The proposed ownership and five-domain split fit the reviewed primitive. They
can be implemented as an unapplied composition on the four queued companions
and core_close_accounting_primitive.patch. This review does not authorize
synchronous public Close, native-close-source activation, or a partial landing.
Only this review document was written; no source, build, app, or board mutation.

Required additions/clarifications
--------------------------------
1. Include NativeTargetURLChanged's DIRECT status delegate invocation in the UI
   synchronous channel. Observer-stage WC378–389 explicitly preserves original
   4388630 InvokeSync. Merely splitting PostCoreUI and guarding policy/imminent/
   didClose leaves this proven synchronous producer uncovered.
2. Capture the exact shared state BEFORE posting the gained-focus first hop,
   without incrementing any count. Pass that captured handle to the second-hop
   PostFocus producer if its existing native WeakPtr survives. Do not recover
   state from the weak ObjC owner at delivery. This preserves the no-count first
   hop while keeping state identity independent of owner/native lookup timing.
3. Give a retained navigation controller an exact state handle when initialized,
   or otherwise establish its receipt-time association before weak owner loss.
   deliverSelector must not obtain state from a registration or a weak owner
   inside each task. A controller/queued task may retain the STATE, never the WC.
4. Follow-on source resolves initial setters as direct/uncounted before bridge
   registration. Preserve that timing without synthetic accounting. Subsequent
   zoom requires a genuine queued WCObserver batch+trailer: implemented in the
   separate UNAPPLIED core_zoom_observer.patch after this composition. State
   association alone does not repair that producer; no per-commit setter remains
   in the composed zoom stage.

State association / shared interface
------------------------------------
- Add one CoreCloseStateHandle ivar on ArcWebContents, initialized with
  arc::CreateCoreCloseState() immediately after successful super init and before
  the first initializer setter (observer-stage WC636–646).
- Lead-owned declaration may expose arc::GetCoreCloseState(ArcWebContents*) via
  arc_internal.h or an approved private header. Implement using a private direct
  accessor; no wrapper creation, native-address table, public getter dispatch,
  lazy replacement state, or retention of wrapper/delegate/native in the state.
- Define nil/uninitialized owner handling. Primitive operations CHECK nonnull
  handles; do not pass null or silently create an unrelated state. All live
  canonical wrappers must already have a state before consumers can run.
- Capture handle by value at each proven producer. Completion/finalizer uses
  that exact handle even after bridge replacement, removal, native close or
  helper destruction. No delivery-time native/generation cancellation is added.
- A future discard/rebind must preserve this handle only through the SAME
  canonical wrapper. No implication that current discard migration is complete.

Concrete primitive use
-----------------------
ScopedCoreCloseInvocation(state, kNavigation/kUI/kMedia) is for each synchronous
native delegate invocation. Construct before the app-facing selector/bridge
query and argument-wrapper construction; retain the state through return and
local callback cleanup. Its LIFO saved flags cover nested same-channel calls.
Do not stretch this scope across later app replies or asynchronous dialogs.

For a UI pending invocation, capture state at enqueue and post the closure
returned by MakeCoreCloseAsyncInvocation(state, kUI, existing_conditional_work)
using the existing runner/location/delay/nesting policy. Keep weak bridge and
other cancellation tests INSIDE that closure, so inert delivery still completes
accounting. No extra UI sync scope is required merely to duplicate its pending
count during delivery. Native asynchronous media producers use kMedia similarly.

For one observer notification:
  CoreCloseObserverBatch batch(state, channel);
  post every existing per-registration delivery unchanged on the original runner;
  post the separate finalizer using batch.PostFinalizer(FROM_HERE, same_original_runner);
The finalizer is a separate ordinary zero-delay task. Post it immediately after
that notification's deliveries, even for an empty list; do not defer all trailers
to the end of a larger native event. Consume the primitive's nodiscard return
explicitly, without synthesizing a rollback decrement on rejection or task
destruction. The audited original producers ignore the post result, as detailed
below. Do not yield/reenter app code while enumerating registrations/posting a
batch.

Bounded post-rejection policy resolved from original code
---------------------------------------------------------
The audited original helpers increment BEFORE posting and do not inspect the
post return value or perform a producer-side counter rollback:
- Delegate InvokeAsync4378120 increments pending+18 at4378194–819c; its post at
  4378308 is followed immediately by control-block reference cleanup437830c–8318
  and the epilogue. The alternate post4378414 tests the pre-existing w23 flag,
  not returned w0. Its counted invocation target42c0840 decrements only on run.
- Navigation notify436bbc0 increments list+30 at436bbe8–bbf0. Its registration
  post436bfdc ignores returned w0 and proceeds to captured-reference cleanup.
  Finalizer post436bdac is followed directly by register restoration and return
  at436bdb0–bdcc, without a result test or list+30 store.
- WC notify4387098 increments list+30 at4387114–711c. Its separate finalizer
  post4387304 is followed by vector/shared-reference cleanup4387308–7350,
  without a returned-w0 test or list+30 store. The supplied final42db8a8 is the
  decrement; ordinary closure destruction is not its invocation.

Evidence read: /tmp/core-close-busy-layout.txt and
/tmp/core-nav-dispatch-audit.txt, alongside the prior task-destructor audit.
This resolves the producer policy for these audited forms: ignore rejection;
no rollback or destructor repair. If an incremented state survives and its
completion task never runs, that counter remains nonzero. This is a conditional
source consequence, not a claim that current UI runners reject live tasks or
that shutdown has been tested. It does not prove every package-specific producer
uses these exact helper forms. Remaining activation gates below still apply.

Core callsite map (line numbers are reviewed-stage anchors; refresh before edit)
-------------------------------------------------------------------------------
Observer WC = /tmp/core-observer-stage/arc/api/arc_web_contents.mm.
Display = /tmp/core-display-stage/core_render_state.mm.
Nav values = /tmp/core-navigation-values-stage/arc_navigation.mm.

| Producer | Required accounting / grouping |
| --- | --- |
| WC250–261 policy selector | Navigation delegate sync; scope includes action construction and invocation return. |
| WC385–389 TargetURL status | UI sync, preserving synchronous delivery after native Chrome forwarding. |
| WC690–698 imminent and704–713 didClose | UI sync per actual delegate invocation; capture state without relying on live _contents, which didClose resets. Do not widen native-close-source token scope. |
| WC327 URL | One WCObserver batch; url and navigationID setters remain in ONE delivery. |
| WC335 TITLE | One WCObserver batch. |
| WC341 LOAD | One WCObserver batch; loading/back/forward remain in ONE delivery with existing delivery-time native queries. |
| WC355 LOAD status | Separate UI pending invocation, after the LOAD observer task/trailer, preserving receipt weak bridge semantics. |
| WC365 favicon | One WCObserver batch for TAB. |
| WC372 traits | Separate WCObserver batch after favicon for TAB OR AUDIO, exactly once for combined flags. Preserve copied receipt payload and delivery materialization policy. |
| WC404 progress,412 visibility,438 theme | Each native notification is one WCObserver batch, with existing copied scalar/optional payload and weak owner. |
| WC424 security | UI pending invocation, preserving captured level/context fields and receipt-time weak active UI bridge; construct the context at delivery inside the accounting closure. |
| WC449 gained-focus first task | Internal native hop: capture state but NO count yet; existing native WeakPtr gate decides whether PostFocus produces a notification. |
| WC502–507 PostFocus | One WCObserver batch per produced gained/lost boolean notification. |
| WC514–520 PostHistory | One WCObserver batch per actual list publication, including shared-model delivery-time list construction. It is NOT a NavigationController-domain event. |
| Display251–261 QueueReady | One WCObserver batch for each produced true/false ready notification. Frame/ACK/copy/unparent source guards remain unchanged. |
| Display291–317 metadata | ONE WCObserver batch and ONE delivery for viewport then root color; not one batch per setter. |
| Nav455–492 deliverSelector | ONE NavigationController batch per notify, before enumerating _registrations; all per-registration tasks unchanged, then one finalizer on that same runner, even empty. |

Why generic PostCoreUI cannot define batch boundaries
----------------------------------------------------
The native unit is NotifyObserversImpl, not a selector, KVO callback, C++
observer callback, or arbitrary UI task. One notify can fan out to multiple
registrations and one ObjC bridge can perform several setters. The one-bridge
architecture happens to use a single ordinary task for many individual WC
notifications, but this is not a general one-task-equals-one-batch rule.

Original 4387098 increments list+30 at4387114–711c, snapshots registrations,
posts their callbacks, then constructs/posts a distinct final function at
43872c0–72e4 targeting42db8a8. Its empty payload branch438716c returns to the
same increment. Native 43a0660 provides other WC notify forms. Final42db8a8
only decrements list+30; it does not call public Close. Navigation notify436b480/
436bbc0 similarly own one notification and a separate final task (40c5ad0
routes the supplied member function). These are the proven batch shapes.

Consequences:
- Do not count individual URL or LOAD setters separately.
- Do not merge all URL/TITLE/LOAD/TAB/AUDIO branches into one batch.
- Do not merge favicon and traits, or metadata and adjacent readiness events.
- Do not count each navigation registration as a separate batch, or use callback
  RAII for a navigation batch. All observers removed or no observers still get
  the native notify's trailing finalizer.
- Do not count policy (Navigation delegate) or status/security (UI delegate) in
  NavigationController or WCObserver merely because they originate in a WCO.
- Retain a raw/internal posting helper for the focus first hop; replace public
  property/delegate producers with explicitly typed helpers, without a default
  channel that can silently classify future callers.

Other-package and activation gates
-----------------------------------
UI permission/auth show/autofill currently post weak-method callbacks directly
(permissions143–144; auth85–86,107–109). Their owners must wrap at enqueue with
captured state; guarding only Show/DeliverAutofill is too late, and passing the
existing weak-method closure directly as the outer task can cancel the decrement.
Dialog close and JavaScript/menu synchronous selectors need UI sync scopes.
App-to-native completion replies are not the original app invocation's lifetime.

Media callback sites currently execute synchronously in webextras_media.mm;
its owner must use the SAME state and kMedia, including destructor-origin
notifications captured before native teardown. Any restored native async timing
must acquire at producer enqueue, not just guard the later selector. Do not
relabel unrelated thumbnails/DevTools/Boost/browser callbacks as one of the five
proven domains. Original PDFReady is now proved ONE UI pending invocation with
direct pdfController store and didFinishPDFRendering in that same delivery; no
extra WCObserver batch (board140257).

Public timing1 still requires same-state UI/media/realzoom producer integration,
exact ordinary forwarding/conditional imminent, source-proven Closed/query phase,
relevant native identity/lifetime handling and validation. Initial setters are
uncounted. Timing0 continuation, unattached ownership and original bulk semantics
are separate bounded scopes, not evidence for a native/bulk five-domain Busy gate.
The resolved UnloadController predicate does not remove these real prerequisites.
No guard destructor may run Close, drain retries inline, reset counts on native
replacement, or hold wrappers/delegates to keep callbacks deliverable.

Suggested validation for the later staged implementation
--------------------------------------------------------
Real-flag compile composed WC/Nav/display and primitive with lead header overlay;
strict sequential apply and preserved-hook/default-close/tail checks; fresh
source review of every typed producer. Then runtime nested UI/navigation/media
close, removed/re-added registration, empty/multiple-observer notification,
close during a grouped setter, between callback and trailer, and native teardown
with queued weak deliveries. No such implementation/runtime check was performed
for this plan review, and no acceptance claim follows from this document.
