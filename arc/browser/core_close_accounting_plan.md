Close accounting integration plan — source audit only
====================================================

Recommendation: land accounting, all proven producers, and the native-close-source
route as one coordinated change before making public idle close synchronous.
Changing only -close or wrapping only direct Objective-C delegate calls leaves
queued callbacks and native bulk-close paths uncovered. No implementation patch,
production edits, compiler/build, app run, or board action was made for this plan.

Evidence and exact domains
--------------------------
Original Close 437854c first returns when managed contents is absent, then returns
while the weak native-close-source token at Impl+160/+168 is live. Only after that
check does it examine these five busy domains:

* UI delegate: +358 synchronous invocation flag, +360 pending async invocations.
* Navigation delegate: +3b8 synchronous flag, +3c0 pending async invocations.
* WC observer list: +470 outstanding notification batches.
* NavigationController observer list: *(Impl+2d8)+b8 outstanding batches (list
  begins +88, list-local pending count +30; notify 436b480/436bbc0).
* Media delegate: *(Impl+2c8)+50 synchronous flag, +58 pending async invocations.

Busy Close posts a weak native retry (4378788..8848); idle Close reaches real
WebContents::Close at 4378894..8898. InvokeSync must remain busy through callback
return; queued invocation pending count must be acquired at enqueue, not delivery.
The async wrapper decrements after invocation returns, including the expired-target
branch (42c08f4..08fc /42c0968..0970). Observer batches use a SEPARATE trailing UI
task for the decrement (42db8a8 for WC observer; 40c5ad0 -> passed final function
for nav-controller observer). Therefore per-callback RAII alone is insufficient.
Source: /tmp/core-close-{busy-audit,delegate-dispatch,observer-finalize,
media-complete,nav-finaltask}.txt; parent confirmed the five field assignments.

The native-close-source token is narrowly evidenced: NativeCloseContents 43836a4
installs it from the input WC WeakPtr; matching continuation 4383da8 clears/resumes.
It is NOT established to be a generic native-observer-stack or all-removals guard.
Do not set it around every WCO callback or arbitrary detach/destructor operation.

Ownership and callsite map
--------------------------
Paths without /tmp are CURRENT production; /tmp paths are UNAPPLIED companions.
Line numbers were read from current files; later edits require refreshing them.

CORE: synchronous navigation delegate and WC observer publication
* arc/api/arc_web_contents.mm:232 — policy delegate invocation: NavigationDelegate
  sync scope; include nested calls and the selector invocation's return.
* :275,325,351,403,406,409,432,435,436,471,473,475,735..752 — current property/KVO
  producer paths; they currently run on native observer stacks. Intended queued
  adapters below replace this timing; do not make close synchronous before that.
* :426,479,745 — current security/status UI delegate calls: UI sync scope now;
  queued adapters must instead acquire pending at their producer enqueue.
* :695,711 — imminent/didClose app callbacks: UI invocation accounting plus the
  independently resolved native-close-source route. Preserve IDs/order/liveness.
* :835..849 — final public Close decision/retry/idle contents->Close route.

CORE: pending generic/observer companion
* /tmp/core-observer-stage/arc/api/arc_web_contents.mm:228 PostCoreUI must become
  explicitly typed, not one undifferentiated counter for every posted task.
* :326 URL (both setters one callback), :334 TITLE, :340 LOAD (all three setters
  one callback), :364 TAB, :370 AUDIO — WCObserver batch producers.
* :354 status and :422 security — UI pending-invocation producers; captured weak
  bridge stays the receipt bridge. Do not change delegate/nil/replacement semantics.
* :385 TargetURL — UI sync scope, preserving synchronous delivery after Chrome.
* :402 progress, :410 visibility, :436 theme, :502 focus bool, :515 backForwardList
  — WCObserver batches. backForwardList is a WC observer publication, NOT a new
  NavigationController batch counter merely because its payload contains history.
* :447 gained-focus first hop carries only weak native identity and creates the
  actual observer event at :502; do not count it as an app invocation prematurely.
* /tmp/core-display-stage/core_render_state.mm:253 ready and :305 copied metadata
  (viewport then root under one owner) — WCObserver batches. They must remain
  eligible after native/helper teardown while their weak ObjC owner survives.
* arc/api/arc_navigation.mm:455..491 deliverSelector — NavigationController batch:
  increment ONCE per notify, post tasks per existing weak registration in order,
  post final decrement task after ALL registration tasks, including their inert
  delivery branches. Preserve remove/re-add invalidation and fresh wrapper timing.

UI package (arc/api/arc_ui_*.mm): use shared UI channel
* javascript.mm:74/77/80,101,167/168 — sync app selector invocations. Complete
  :144..157 and native callbacks :118/:128 are later app->native work, not a reason
  to hold the original invocation busy for the entire user dialog lifetime.
* permissions.mm:143 enqueue Show -> :189 selector — acquire UI pending at enqueue,
  release after selector returns even on weak-target/missing-selector paths.
  :152 close selector needs UI sync. :191 Resolve enqueue is app->native decision;
  track any NEW app callout it produces, not the whole outstanding permission.
* auth.mm:85 Show -> :131 and :107 autofill -> :120 — UI pending at enqueue through
  invocation return. :98 close selector needs UI sync. :55/:65/:75 marshal app
  response into native; do not extend original pending-invocation lifetime to reply.
* menus.mm:477 willShowContextMenu — UI sync (return value preserved). Menu model
  :340/:349 delegates are not proven members of the per-WC UI wrapper domain;
  owner/package must resolve whether to associate them before adding busy counts.
* Notifications are profile/global, not proven per-WC close domains. Do not
  blanket-count arc_ui_notifications.mm tasks under an arbitrary source WC.

WEBEXTRAS: proven media and UI callback domains
* arc/api/arc_webextras_media.mm:246/253 PiP create/destroy, :261/:266/:271 info,
  :281 actions, :286 position — Media sync invocation scopes in current producer
  timing. :296..299 are native source adapters; shared state must be captured before
  app callout and stay valid if media observer/session is reset during callback.
* Same file :350 destructor notification; :356/:362/:367 show/hide/close;
  :376 resolution; :380/:383 creation+playback; :401 position; :406 title;
  :412 favicons; :428 feature — Media scopes. If webextras restores native async
  delivery, acquire pending at enqueue and preserve the proven task ordering,
  rather than just guarding its later selector. Destructor-origin callback cannot
  derive its accounting from a native object already being torn down.
* arc/api/arc_webextras_capture.mm:269 -> :281/:283 PDF-ready setter/UI callback:
  acquire at producer, with property publication in WCObserver batch domain and
  didFinishPDFRendering in UI domain. Exact original combined/split enqueue and
  resulting domain timing needs webextras/source confirmation before conversion.
* capture.mm:65..75/:159..162 thumbnail registrations; devtools.mm:75/:83 protocol
  notifications, :253/:265 DevTools UI; webextras_ui.mm:173 Boost; :63/:107..112
  Browser side-panel callbacks: these EXIST, but the five Close predicates do not
  independently prove inclusion. Keep their lifetime defenses; obtain producer
  proof instead of broadening native Close's busy policy by category name.

Minimal shared API/design (lead-owned contract)
----------------------------------------------
Place common declarations through arc/api/arc_internal.h:119 cross-package
section or a lead-approved private C++ header included from it. Core implements a
per-logical-Arc-WC shared state; no exported Arc-prefixed helper ObjC class.
UI/webextras must use the same core state, not separate per-file counters. A native
WeakPtr plus weak ObjC owner links state without retaining app delegates/wrappers.
Every task decrements the exact captured state, never a new lookup by native
address. The original Impl lifetime across discard/rebind and whether counters
carry into a replacement native need explicit policy; do not reset counts or
introduce generation cancellation based on pointer replacement alone.

Suggested small surface (names illustrative):
* ExistingCloseState(owner): obtains initialized core shared state, no wrapper creation.
* ScopedCloseInvocation(state, UI|NavigationDelegate|Media): reentrant depth;
  nested guards restore prior busy state. Scope spans invocation RETURN/cleanup.
* PostCloseInvocation(state, channel, block): pending++ BEFORE PostTask; shared
  token survives bridge/native death, task invokes conditionally, then pending--.
  Early return/expired registration must still execute invocation finalization.
  Destruction of a canceled task does not perform the native decrement. Rejected
  PostTask policy is unproven; do not invent rollback or forced delivery. UI
  sequence only; off-UI app responses marshal before mutation.
* BeginObserverBatch(state, WCObserver|NavigationController) + explicit Finish:
  batches++; enqueue ordinary weak-target deliveries; enqueue ONE trailing UI task
  which decrements. No last-registration destructor or first-callback RAII shortcut.
  Preserve empty-batch finalization where the native producer increments for it.
* HasLiveNativeCloseSource(state), Set/ClearNativeCloseSource(source weak identity,
  matching continuation identity): separate from Busy(), with exact source route
  confirmed first. A live source makes public Close return without new retry.
* Busy(state): any sync depth, delegate pending, or observer batch nonzero.

Public Close then checks native identity, native-close-source, then Busy; busy posts
same weak retry with original close arguments, idle calls contents->Close(). Do not
invoke Close from a counter guard destructor on a native callback stack. Do not
silently coalesce retries or replace native weak retry scheduling with inline drain
unless original scheduling is separately proved. Existing foreground timing argument
and flags still need original mapping; do not discard them in the shared helper.

Native routes / required lead decisions
--------------------------------------
* content/.../web_contents_impl.cc:10402..10420 is final contents->Close route. On
  Mac it asks CloseTabAfterEventTrackingIfNeeded first, then delegate CloseContents.
  web_contents_view_mac.mm:539..550 uses a DEFAULT-MODE Cocoa run-loop block;
  comments explicitly reject substituting merely a NonNestable Chromium task.
* chrome/browser/ui/browser_web_contents_delegate/browser_web_contents_delegate.cc:
  772..775 checks CanCloseContents then chrome::CloseWebContents. Lead must resolve
  the original NativeCloseContents source/continuation equivalent HERE or an
  existing host hook. Do not fake its token from OnTabStripModelChanged removal.
* arc/browser/arc_browser_window.mm:154..158 calls UnloadController::OnWindowClosing;
  unload_controller.cc:210..213 directly calls strip->CloseAllTabs. That path does
  not necessarily pass each ArcWebContents public Close/busy decision.
* tab_strip_model.cc:1261..1275 performs a bulk reentrancy-guarded close with
  historical-tab flag. Preserve this native bulk operation, ordering/restoration
  and unload policy. Shared producer counters alone do not make bulk native close
  wait: deciding the host-gated busy gate/continuation for that route requires
  native source evidence and lead coordination, not N calls to Arc close instead.
* Current wrapper-owned contents branch uses DeleteSoon (:839). Requested idle
  target is contents->Close; native Close is no-op without delegate. The attached
  native delegate/ownership behavior of unattached wrapper-owned contents needs
  confirmation before removing disposal or inventing a fallback delete.

Suggested coordinated landing and verification
---------------------------------------------
1. Lead fixes shared contract and native source/continuation/bulk-close design.
2. Core wires accounting into finalized queued WC/display/nav companions; UI and
   webextras wire only proven channels. Keep current public Close deferred meanwhile.
3. Compile affected owned TUs, then inspect coverage of all five domains, registration
   removal/re-add, nil/replaced bridges, nested sync, native/wrapper teardown and
   native association replacement. Counts must drain on inert delivery branches.
4. Switch public idle Close only after integrated paths are complete. Targeted
   runtime comparisons: close from nested UI/Nav/media callback; close between an
   observer callback and its trailing task; pending removed registration; native
   close-source continuation; JS beforeunload; whole-window CloseAllTabs; event
   tracking; unattached owner; existing launch/tabs/windows regression. No runtime
   validation was performed for this source-only plan.

Fresh independent source review
-------------------------------
core_close_plan_review: no concrete P1/P2 findings. Predicates, native retry/idle
route, enqueue-to-return accounting, separate observer finalization and narrow
source-token semantics agree with cited evidence. Current/staged callsites match
ownership. Package-specific producer enqueue timing still requires confirmation;
generic wrapper evidence does not prove every individual producer. Native source/
continuation/bulk-close, discard counter lifetime, unproven callback domains, PDF
enqueue grouping and unattached owned contents remain gated. No runtime validation.

Parent contract correction after review: removed the earlier overbroad guarantee
for failed posting. Original invocation completion decrements after return, but
task destruction does not. Rejected-post behavior remains unproven. This matches
the staged primitive and the correction requested of UI notes in message137863.

Native-source follow-on (see core_close_source_notes.md)
------------------------------------------------------
The separate source token is now proven to belong to the timing-zero foreground
continuation; ordinary public close forwards timing1 (4410bac and accepted
sessions golden6137), bypassing that special predicate. Do not place this token
around all native closures. Exact clear-before-match-current/reentry and the
imminent-notification flag are documented in the follow-on. The remaining
timing-zero keyed controller/mode identity and whole-browser native transaction
coordination remain bounded; no synchronous public activation is claimed.
The default forwarding correction has independently landed while preserving the
existing deferred native operation (core_close_forwarding.patch ALREADY APPLIED).
