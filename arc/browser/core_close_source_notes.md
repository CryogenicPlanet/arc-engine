# Native close-source token audit (2026-10-23, read-only)

No production/native/shared source, build, app execution, or board action.
Original arm64 binary: ../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore.
All addresses unslid.

## Critical default-timing correction

Original -close at4410ba4 loads closeWithForegroundCommitTiming: and sets w2=1
at4410bac. It does NOT forward0 as specs/ArcWebContents.md currently states.
Accepted golden results/golden/arc-1.170-sessions-public-api/launch-1/record.jsonl
seq6136 close ->6137 closeWithForegroundCommitTiming:[1] ->6138 didClose callback
->6488 timing-selector return ->6489 close return proves synchronous nesting.
The driver calls the ordinary public close selector; it does not replace it.
Evidence /tmp/core-close-default-timing.txt; tests/scenarios/sessions-public-api-driver.m:207-211.

Public native Close437854c first checks managedWC Impl+130 and live source token
Impl+160/+168. Live token returns immediately. Otherwise it sets Impl+159 to
(timing low32 ==0), clears Impl+15a, checks the five previously audited busy
accounting domains, then posts retry or calls actual content WC::Close679dfc4.
The retry reconstructs timing as !Impl159, so it preserves the zero/nonzero
classification. No enum-name claim is made.

The new ours run results/sessions/canonical-public-api-run1 fails the inline
callback assertion at tests/scenarios/sessions-public-api.ts:130: its close return
seq6381 precedes didClose6428. Current production arc_web_contents.mm:835-849
always DeleteSoon/posts and ignores supplied timing. This is a demonstrated
remaining gap, but NOT authorization to remove deferral before accounting/native
close paths are coordinated.

## Exact NativeCloseContents decision and token scope

Actual function entry is4383378;43836a4 is an internal deferred branch.
Input x1 is captured as sourceWC x21 at4383394. Entry checks live token160/168
and returns without further work while its effective pointer is nonnull.
It then clears byte158 (meaning not recovered), reads current managedWC130 and
its main RFH8d8, and does:

1. If159 && !15a, call4378aa0(current Impl). False -> deferred branch438369c.
2. Otherwise, when main RFH exists && !15a && RFH IsPageReadyToBeClosed31953e8,
   InvokeSync OnClosingImminent (4383488-34a0), passing current WC's SessionTabHelper
   ID or -1. Native UI delegate slot68 maps through da89030+68 to4446054,
   which sends webContentsClosingImminent:withSessionID: at44460b8.
3. Continue synchronously into ownership/proxy close. If owned binding+128 exists
   and embedding browser weak58/60 is absent/expired, clear+128 and delete its
   owned native pointer, then release owning helper120 (4383534-3560). Otherwise
   call WebContentsDelegateProxy::CloseContents4350ee4 with ORIGINAL sourceWC
   at438358c-3594. That proxy weak-loads original delegate30/38 and invokes its
   CloseContents vtable+40 with source (4350f14-0f70).

Therefore normal timing1 does not use the special deferred token branch.
OnClosingImminent is conditional, not a universal tab-removal/destruction event.
Do not force it on ordinary synchronous close merely to implement the token.

Deferred branch438369c:
- Sets15a=true, obtains INPUT sourceWC's native WeakPtr pair from+1298/+12a0.
- Installs it at Impl160/168 (43836a4-36dc), before any app callback.
- Weak-locks Arc owner and InvokeSync OnClosingImminent43837e4-37fc.
- After callback, requires captured source still alive and effective stored token
  pointer == captured source (4383828-385c).
- Enqueues zero-delay task43838c0-3928 through normal UI TLS runner. The task owns
  weakImpl from Impl610/618 plus captured source WeakPtr; it owns neither WC nor
  native Impl strongly. Bound target is4383da8. Task dispatcher439f740 checks
  weakImpl alive, transfers source pair into target arguments. Its destructor
  439f7a0 only releases weak captures; it does not resume close.

Continuation4383da8:
- Requires live captured source and effective token pointer==captured source.
- Clears token160/168 FIRST (4383e20-3e3c).
- Checks current managedWC130 == captured source (4383e40-3e54).
- Only then reenters4383378 with that source (4383e58-3e64).
- 15a remains true, so reentry skips deferred/imminent paths and reaches native
  ownership/proxy continuation. Mismatched token, expired source/Impl or rebound
  current WC drops this continuation; no alternate source is closed.
- Cleanup4372b6c independently clears160/168 at4372c28-2c44.

A source WeakPtr is thus transaction identity/coalescing across a specific
foreground-timing deferred continuation. It is NOT a generic observer-invocation
flag, arbitrary stack-depth guard, or reason to repost every native close.

BeforeUnloadFired producer4383ea4 is related but distinct: when proceed &&159 &&
!15a it sets15a=true and InvokeSync OnClosingImminent4383f68-3f80 before forwarding
beforeunload to the original delegate. This path does not itself install160/168.
Evidence /tmp/core-close-source-entry-task.txt, /tmp/core-close-source-token.txt,
/tmp/core-close-source-readiness-proxy.txt, /tmp/core-close-actual-imminent-bridge.txt.

## Predicate4378aa0 (bounded semantic recovery)

Exact control flow:
- No managedWC130 ->false.
- Finds containing browser via global lookup18efdf0, then bridge lookup31dab4;
  missing bridge or expired/empty weak bridge28/30 ->false.
- Calls WC NeedToFireBeforeUnloadOrUnloadEvents3195130; true ->true.
- Otherwise bridge vtable+18 obtains owning native object;4969fc with keydf54710
  gets keyed object; its byte+a8 true ->false; false ->tail7d6176c(object,WC).

3195130's FrameTree traversal and31953e8 frame readiness match current content
WebContentsImpl::NeedToFireBeforeUnloadOrUnloadEvents (web_contents_impl.cc:3356)
and RFH::IsPageReadyToBeClosed (render_frame_host_impl.cc:7854): disassembly
/tmp/core-close-readiness-close-native.txt and /tmp/core-close-imminent-bridge.txt.
7d6176c matches current UnloadController::ShouldRunUnloadEventsHelper exactly:
loops handlers, skip-beforeunload short-circuits false, ORs custom confirmation,
then checks DevToolsWindow instance and returns combined bool. Current source
unload_controller.cc:309-325. Its original body is
/tmp/core-close-readiness-final-calls.txt. This is stronger than guessing a
constant based on page loading/visibility. Timing1's proven native path bypasses
this predicate.

Follow-on identity audit resolves the earlier key/byte gap. Raw chained pointer
df54710 decodes to d32676a, literal UnloadController::kDataKeyImpl. Constructor
341952c registers that key at34195f0..9600 and clears byteA8 at3419648. Shutdown
producer31bb0f8 sets it for shutdown types3/4 before window close; consumer
35ea6d4 permits close immediately when set. The constructor/producer/consumer
chain matches current force_skip_warning_user_on_close_, including
BrowserCloseManager::CloseBrowsers and ShouldIgnoreUnloadHandlers, independently
of a field-layout guess. Current ShouldRunUnloadListenerBeforeClosing at
unload_controller.cc:237..240 supplies the exact named operation:
!force_skip_warning_user_on_close_ && ShouldRunUnloadEventsHelper(contents).

After the original containing-browser/bridge guards, the recovered predicate is
contents->NeedToFireBeforeUnloadOrUnloadEvents() OR
unload->ShouldRunUnloadListenerBeforeClosing(contents). Evidence is durable in
logs/wp/core.unload-controller-identity/native.txt; fresh read-only reviewer
proved the mapping and parent independently captured these native bodies. This
resolves predicate semantics, not the remaining native close transport, all-five
busy domains, destruction timing, or bulk transaction coordination. Old partial
close patches remain HOLD; no synchronous close/timing0 activation.

## didClose, detach and disposal

Original native WebContentsDestroyed438a18c (named by its own trace string):
- Selects restorationID from optional338/33c, or -1 when absent.
- Unless a global condition byte suppresses it (meaning not decoded here),
  InvokeSync UI delegate Closed at438a224-23c, slot20 ->4445cf0 ->
  webContents:didCloseWithSessionID:4445d54.
- Only AFTER app callback return calls Observe(nullptr)4353534 and full cleanup
  4372b6c (438a278-288).
- Cleanup clears token early, resets subservices, and zeros native binding128/130
  together at4373054-3058.
- Original isInitialized4440d78 reads nativeImpl130!=nullptr. Hence original
  callback can observe initialized==true during Closed; it becomes false only
  afterward. Current nativeContentsDestroyed resets _contents BEFORE didClose
  at arc_web_contents.mm:708-711. This is another source-proven timing gap;
  accepted sessions probe currently checks after public close, not inside callback.
  Do not retain a dangling native pointer to reproduce a boolean query.

Closed is tied to actual native destruction, not simply model removal/imminent.
Current Chrome detach transaction retains WC while emitting model-removal, then
resets it afterward. Public synchronous close can therefore deliver Closed inline
if its native operation synchronously runs the whole transaction.

Current source route evidence (helper audit):
- WC::Close web_contents_impl.cc:10402 applies Mac eventtracking check10413 then
  delegate CloseContents10419; bypasses ClosePage.
- BrowserWebContentsDelegate::CloseContents:772 calls CanCloseContents773 FIRST
  (mutates unload state), then chrome::CloseWebContents774.
- chrome::CloseWebContents browser_tabstrip.cc:130 restores stored flags141-144,
  adds history147, enters model150.
- TabStripModel::CloseWebContentses:4118 calls WillDetach4130, may pause at unload
  RunUnloadListener4184 (stores flags4185).
- DetachTabImpl:919 emits preparation, historical entry942, removes model945,
  and OnRemovedFromModel948; it does not yet destroy retained WC.
- SendDetachWebContentsNotifications:955 orders decreasing original indices,
  notifies REMOVED994, then dt->tab.reset1004 destroys WC.
- Browser native removal clears WC delegate (browser.cc:687 and922). A delayed
  continuation using only detachedWC->Close() can do nothing. Capture/replay the
  actual pre-detach operation; do not convert arbitrary removal to public Close.
- Our ArcBrowser removed branch nativeContentsClosingWithRestorationID at
  arc_browser.mm:217 runs during model-removal, before destruction. Current WC
  observer437-443 ->nativeContentsDestroyed703 sends Closed.

## Whole-browser relationship and safe coordination boundary

Current whole-window path is independent: UnloadController::OnWindowClosing
unload_controller.cc:213 ->TabStripModel::CloseAllTabs:1261 ->one bulk CloseTabs
transaction:3995. It bypasses both WC::Close and delegate CloseContents. It
captures reverse-order tabs and preserves bulk unload/history/notification state.
The original whole-browser bulk producer was not independently fully mapped in
this audit; no claim that every bulk tab installs the individual source token.
Original per-WC destruction callback still invokes Closed above. Its observed thunk438a43c subtracts Impl-interface offset0x70; this audit does not relabel that view-owner interface as the separate Impl+0xa8 WebContentsObserver vtable or claim the exact current-Chromium destructor callback phase is already equivalent.

Coordinated design requirements justified by this evidence:
1. Preserve ordinary close default timing1; only busy accounting causes its
   public retry. This directly addresses the new accepted sync assertion once
   every busy producer is integrated.
2. A native delegate interception must be BEFORE CanCloseContents mutation and
   must expose resumable original CloseContents(source), without detaching first.
   Represent source with weak exact native identity; an unrelated/new source or
   rebind must not be closed by an old continuation.
3. The special timing0 two-phase branch has its predicate/controller key resolved
   above; expose only that narrow source token. Completion clear-before-
   resume and 15a state prevent duplicate imminent notifications.
4. Single/model-native and whole-browser bulk transaction entries require their
   OWN busy/retry coordination, because they bypass public Close. Preserve flags,
   reverse/bulk order, and unload continuation; do not replay bulk as N singles.
5. Keep actual destruction as Closed producer and preserve optional historical
   ID rules. Callback delivery and native cleanup ordering need an explicit
   destroying-state design; do not equate imminent/removal with disposal.
6. Do not remove existing public deferral or land held ClosePage-only patch from
   this read-only evidence alone. Timing1 is proven, but native reentrant/bulk
   closure and five-domain accounting must be integrated/reviewed together.

No runtime/test success is claimed for a proposed implementation.
