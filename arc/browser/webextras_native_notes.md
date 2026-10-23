# Webextras native evidence and remaining bridges

Verified locally against ArcCore 1.170 arm64 on 2026-10-23. These are bounded static findings; runtime equivalence still requires the scenario diffs. No VM access was used. Binary: `../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`; temporary arm64 extraction: `/tmp/arccore-webextras.arm64`.

## Exact findings

- `ArcWebContentsMediaSession seekTo:error:` and `scrubTo:error:` accept signed **whole seconds**, not microseconds. Native helpers `0x436121c` and `0x43612fc` multiply x1 by 1,000,000 with signed saturation before invoking the Chromium seek/scrub implementation. Implementation uses `base::Seconds(int64_t)`.
- Media action delegate bridge `0x441bfcc..0x441c088` iterates uint32 native actions, requires each index `< 0x15`, and adds the unchanged index to an NSMutableIndexSet. Original supports indices 0 through 20. Chromium156's added SaveVideoFrame index21 is not exposed.
- PDF `availablePages:error:` callback thunk `0x443e774` invokes its block with **int32 page count**, default0 when optional native result is absent. It is not an NSArray/NSIndexSet callback.
- PDF `visiblePages:error:` callback thunk `0x443e790` converts an optional vector of32-bit page indices to **NSMutableArray<NSNumber*>**, or nil when the optional is absent. A single most-visible page is not an equivalent substitute.
- PDF `getBookmarks:error:` and `getAvailableTextOn:callback:error:` share callback thunk `0x43f88dc`: optional native UTF8 string to **NSString or nil**. Bookmark JSON structure remains unrecovered.
- Media/thumbnail native failure constructor calls use tagged status0x25. Status9 maps to NSError code6 under the recovered native error table. PDF detached state also0x25/code6; missing glue/controller uses0x35/status13/code10. Error domain is `company.thebrowser.Arc`.
- Original volume helper `0x4148644` iterates a native map of MediaSessionController peers, forwarding the double to `0x654fc88`, which looks up the MediaPlayerRemote and sends a double-valued Mojo command. Upstream equivalent is `MediaSessionController::OnSetVolumeMultiplier` -> `media::mojom::MediaPlayer::SetVolumeMultiplier`; `SetDuckingVolumeMultiplier` is not equivalent (only changes the future ducking amount).
- PiP feature indices, inferred from original callback ordering against the matching controller Show/Info/Actions sequence: playPause4, skipAd6, nextTrack3, previousTrack5, microphoneMuted2, toggleMicrophone8, cameraOn0, toggleCamera7, hangUp1, nextSlide9, previousSlide10. Original has no matching callback for Chromium156's new hide-PiP/mute control methods.

## Runtime coverage currently available

- Final media golden: baseline mediaSession setter/types + thumbnails observer registration only.
- Final pdf golden: same baseline; **zero ArcWebContentsPDFController methods and zero didFinishPDFRendering callbacks**.
- Final media-pip golden: real PiP creation/show/hide, view/bounds getters, playYES, position wrappers/callbacks, state/actions/features/title/favicon. PiP video native resolution64x64 is shown in view128x128. Surface must stretch and use the window backing scale.
- Final launch-slice includes `webContentsPiPControllerDestroyed:` on tab destruction even if no actual PiP overlay was created. Guarded media observer teardown preserves this behavior.

## Remaining bridge gaps (explicit errors, not false success)

- General setVolume requires exposing a Chromium player-volume operation through a lead-owned patch; current wrapper returns code6 unavailable after checking managed contents only; no active-session precondition remains.
- PDF hasBookmarks/getBookmarks require renderer PDF bookmark queries; getBookmarks callback string schema still needs recovery/golden coverage.
- PDF visiblePages needs every visible page; upstream browser PDF helper only exposes GetMostVisiblePageIndex. Current wrapper returns unavailable instead of substituting one page.
- PDF page count and page text use actual PDFDocumentHelper methods. PDF helper creation/load observer only exposes full-page PDFs, invalidates the controller on committed non-same-document navigation, and calls optional uiDelegate didFinishPDFRendering after setting the cached controller. Existing final goldens do not cover these paths.

## Lifetime audit

- Media WebContentsDestroyed detaches its native observer before calling the app; app callbacks may setDelegate:nil and delete the observer.
- PiP creation callback occurs on the first SetPlaybackState, after the native controller owns the overlay, with a weak check after app reentry. Overlay resizing updates compositor state first and calls UpdateLayerBounds last.
- PDF readiness is validated/deduplicated at receipt, then a weak-observer UI task delivers it without a navigation-generation/helper recheck. Owner/delegate/controller are strong locals captured before the setter; no observer/helper access follows KVO. Native bridge retains the original delegate and continues its callback tail even if KVO navigates or replaces the delegate. Exact first-ready trigger and registration-replacement invalidation are not fully recovered.
- Thumbnail subscription removal is now immediate on the last registration: app delivery runs in separate Chromium UI tasks, outside ThumbnailImage callbacks. Member state clears before native unsubscribe to permit reentry. Ordered registration tokens hold weak observer/owner references. Emission queues one Chromium UI task per weak token, copying image data; removal cancels queued delivery without retaining the observer.

## Why the accepted pdf golden has no controller callback

`tests/scenarios/pdf.ts:17-33` waits only for `document.contentType === "application/pdf"`, asserts the MIME iframe, captures a screenshot, and immediately closes the target. It does not wait for the document to finish loading/rendering. The golden `pdf-dom.json` records the MIME iframe with `src="about:blank"`; visual inspection of `pdf.png` shows the PDF toolbar/sidebar shell and a blank document pane without the fixture page/text. Therefore this accepted scenario covers MIME hosting, not the controller-ready boundary. The native observer waits for PDFDocumentHelper's actual document-load completion rather than suppressing callbacks by scenario name. A scenario that waits for a rendered fixture page/text is needed to establish the original ready callback condition and API results.

## Auto-PiP and state validation

Original `notifyAutoPipTabActivationChanged:error:` helper `0x4360814` looks up a WebContents AutoPictureInPictureTabHelper and forwards the BOOL unchanged to `0x7b050bc`. Its body matches current public `AutoPictureInPictureTabHelper::OnTabActivatedChanged(bool)`: occlusion guard; active -> OnTabBecameActive; inactive -> close active helper's auto-PiP, then attempt eligibility/entry. No active MediaSession precondition exists. Missing helper error string at0xd3815c5 is exactly `no auto picture-in-picture tab helper`, tag0x25/code6. The wrapper now uses that exact public API.

Original state bridge `0x441bf5c` requires native state<4 and forwards unchanged; camera/microphone bridges `0x441c118`/`0x441c188` require native state<3 and forward unchanged. Current Chromium's SessionState (active0, ducking1, suspended2, inactive3), CameraState and MicrophoneState enum ranges agree. Bridge-side enum labels are inferred from upstream; no remapping was observed.

The overlay caches the associated media-session wrapper weakly. Internal native state/control callbacks use objc_direct helpers rather than repeatedly invoking the public `-[ArcWebContents mediaSession]` getter, preserving the app-observed getter counts.

## Architecture clarification

The broad one-wrapper-per-native-object identity rule conflicts with the original nested `pipController` getter: media-pip golden creates wrapper#150 for the creation callback and wrapper#152 through the subsequent getter, both for the same active overlay. The class spec independently observes allocating a new wrapper. Parent selected the architecture rule: the current getter returns the canonical cached wrapper constructed for the creation callback and clears it on destruction. The reduced engine-internal constructor count is an intentional identity/internal-constructor difference to report. Top-level media/thumbnails service wrappers remain cached once per ArcWebContents. Immutable ArcMediaPosition snapshots remain per-getter values.

Original `exitPictureInPicture:error:` helper `0x4360680` rejects a missing active PiP video before checking MediaSession. Error string0xd381591 is exactly `webcontents has no active picture in picture videos`, tagged0x25/code6. Wrapper reproduces that error and forwards the supplied pause BOOL to the actual controller when active. Original also has a rarer unsupported-session fallback that closes its existing native controller with false; this distinction is not exercised by current goldens and needs a dedicated unsupported-PiP-session case before claiming full fallback equivalence.

## DevTools boundary findings (parent worker)

- Final `devtools/launch-1` golden seq4211 onward disproves the spec's target wording: `ArcWebContentsDevTools.webContents` initializes/returns the frontend, title `DevTools`, URL `devtools://devtools/bundled/devtools_app.html?...panel=console`, then app obtains its native view. Implemented frontend return, with canonical wrapper creation deferred until getter rather than at Show.
- Raw protocol callback ABI is `(NSString* message, NSError* nil)` or `(NSString* nil, NSError* error)`, per bounded ABI report and original adapter0x4416690. Both directions have8MiB guards. Incoming oversized messages schedule detach rather than detaching synchronously in dispatch. Explicit close queues a copied callback with `DevTools connection closed`; already queued messages retain callable storage.
- Original status-to-NSError table was read at VA0xc9d8148 through the local original Mach-O segment mapping. Native statuses1..12 map to `[0,1,2,10,3,4,5,10,6,7,8,9]`; other statuses map10. Tagged constructor argument is not NSError code: tag0x25 means native9→NSError6; tag0x39 means native14→NSError10; tag0xd means native3→NSError2; tag0x15 means native5→NSError3.
- Raw `openProtocolConnection`0x4355df8 checks runtime feature44 before owner lookup; disabled literal is `DevTools protocol connections are disabled`, NSError6. Integration pending shared C++ runtime-feature accessor requested in Chirp133987; do not read feature state via a new public getter invocation, since original native lookup produces no extra ObjC boundary call.
- Final devtools launch2 observes `inspectModeDidChange:1` after opening inspect mode. Lead hook requested at real frontend protocol dispatch for Overlay.setInspectMode rather than inferring success solely from openInspectView. Focus request hook also pending. The callback adapter0x441604c passes native1..4 unchanged and maps others0; only searchForNode→1 is currently runtime-proven.
- Top-level subobjects are cached by core's serviceNamed using `initWithArcWebContents:`; SidePanel uses `initWithArcBrowser:`. These new runtime-callable owner initializers are implementation plumbing absent original traces. Their diff policy remains a lead decision, not locally hidden by changing recorder behavior.

## Bounded value acceptance

`arc/browser/webextras_values_probe.sh` compares original and ours without initializing a browser/profile. AccessibilityActionData, default AccessibilityTreeNode and SidePanelEntryInfo match all observed values; the expanded probe also verifies public MediaPosition, SidePanel and DevTools metadata, totaling29method encodings and22property attributes; recorded and nonrecorded results equal. `results/webextras-values/diff.txt` reports zero missing/extra/returns/arguments/counts/internal differences. It does not test live accessibility production or native-pointer input compatibility.

## DevTools commands/subscriptions bounded native audit (10:03 UTC)

Fresh read-only review of original arm64 established and production code now implements:
- currentPanel/execute completion adapter0x44162ac: (NSString*,NSError*); subscription adapter0x4416474: (NSNumber* subscriptionID,NSString* paramsJSON,NSError*).
- Event router435aca4 serializes only a dictionary params; absent/non-dictionary params yields NSError3 `Missing the `params``. Matching-event records are an insertion-ordered vector (435bc2c..bc9c append;435ad40/435ae10 iterate). Queue copied subscriber callbacks, so unsubscribe cannot invalidate an already queued event.
- Response router435aaa8..ab18/435afa0..b08c prefers dictionary result, then dictionary error. Error is code7 with full serialized error dictionary as description. Neither dictionary yields code1 `Missing the `result` or `error` field`. Dictionary helper1a29dc8..dd0 checks tag6. Chromium JSONReader/WriteJson preserve native parsing and serialization rather than Foundation string formatting.
- Separate zero-initialized processglobal subscription and command counters at e3bae50/e3bae54 preincrement, firstID1. Raw connection IDs remain per DevTools service.
- Internal command completions dispatch directly at435ed7c..ed88; subscriber notifications post copied tasks at435ae5c..adc4. Only raw transport applies explicit8MiB guards; internal send435dd94..e5b8/receive435ea30..eedc do not.
- Host close435d880 feeds pending transport callbacks empty dictionaries; outer router ignores them, retains pending completions, and does not reconnect. Later execute returns immediate code10 `DevTools agent host closed`.
- Every autoEnable=true registration sends domain.enable. Removing the final subscriber for a particular domain+event key sends domain.disable even if other events in that domain remain or autoEnable was false (435bca4..bcfc and435c00c..c16c).
- Non-string currentPanel result yields code3 `panelName not found` at4357ed8..fa0.
These remain bounded static evidence until the paired native probe exercises them; existing accepted goldens cover none of these higher-level command/subscription methods.

Raw sender follow-up: open unavailable literal omits final period (`Associated WebContents is not available`,4356384/stringd3810eb); attach failure is `Unable to attach DevTools client` (4356240..625c). Both code10. RFC parse options0/depth200 at4356600..6608, dictionary+FindInt32 id+FindString method. Unknown connection3, closed10, oversize strictly>8388608 bytes10, malformed2; exact existing strings verified. Validation order ID lookup,closed,size,parse,fields,dispatch. Unknown close no-op; known close queues single close error. Production corrected parsing and two literals; check-file clean.

## Thumbnail lifecycle follow-up (actual media-pip diff)

The extra thumbnailReady callback without a public request is NOT proof it should be suppressed. Original addObserver436d904 invokes436d69c at436db80: creates subscription, installs436dff4 callback, and requests existing image via731c7a8. remove436de50 clears subscription when observers reach0. Public request436df2c never creates a missing subscription: zero observers/no subscription =>YES; detached/helpermissing=>code6; VISIBLE(enum2) calls CaptureThumbnailOnTabBackgrounded731d500; other visibility requests existing image. Production now deduplicates observer before native work, requests stored image on new observer registration, and follows visible capture/no-subscription semantics. Syntax clean. Actual callback differences may reflect capture readiness/visibility/locked desktop; do not add scenario-based suppression.

## Raw close lifetime follow-up
Original incoming oversize Dispatch435c51c compares strictly>0x800000 at435c540, marksclosing435c574, postsClosehelper435c74c at435c608. That helper clearsnativehost beforedetach and emits ONLY generic `DevTools connection closed` (d3813a8,435c830-84c), native14/tag0x39->NSError10. There is no incoming size-specific error or double error. Original435c890-8b8 clears stored notification after queuing its copy. Production now clears callback_ afterNotify in both Close/AgentHostClosed, keepsclientaliveacrossAgentHostClosed reset, and explicitclose removesmapentry then callsClose throughlocalshared_ptr instead of relyingondestructor. Thus postedoversizetask cannotdeferexplicitdetach or duplicateclose; queuedmessages retain copiedcallbacks. Officialcheck0 andfreshreadonlylifetimereviewclean. Runtime rawboundary stilluncoveredpendingfeaturegateaccessor.

## Ordered thumbnail delivery acceptance
Original emitter436dff4 explicitly expands arc/common/utils/observer_list.h::NotifyObserversImpl, scanning native weak registrations by ascending16-byte index andposting pertask at436e80c (line268). Delivery42db6b8 weak-locks; removing B during A cancels B evenifalreadyqueued. Production ordered weak registration tokens replace NSHashTable snapshot; each copied image is converted per delivery, on Chromium UI PostTask. Newobserver during callback is excluded from currentemission; remove/readd invalidates oldtoken. Seven focused tests compile the ACTUAL marked production helper with a standalone queueadapter: order/async, AremovesB, addCnextbatch, observerexpiry, remove/readd, ownerexpiry, collectiondestruction. AllPASS with Wall/Wextra/Werror; native Chromium queue wiring is compile-only, full browser runtime pending. Explicit browser_thread.h +GetUIThreadTaskRunner ensure native scheduling class. Official andindependent actualcompile0.

Nested delivery clarification: native42db6d4-6fc acquires a shared strong lock, holds it through callback42db708-710, then decrements at734-73c. Therefore in-flight delivery deliberately keeps its old registration alive across remove/readd, and a nested oldtask maydeliver; oncein-flight lockunwinds, lateroldtasks cancel. Production stronglocalcurrent models this. Eighth regression usesboundednestableFIFO+actualhelper, provesnestedoldA2 delivered, oldA3cancelledafterunwind, newtokenreceivesnextemission. All8casesPASS, no stronger inventedcancellation. Fresh reviewfoundnoprovenmismatch; actualChromiumqueueintegrationstillcompile-only.

Subscription teardown follow-up: original436de80-90 clears storedsubscription on lastobserver removal, then directly destroys it at436df18->731bd18. Native ThumbnailImage forbidsunsubscribe onlywhile its nativecallbacks iterate; productionnativecallback nowonly posts tasks/noappcode, so laterappremove is outside iteration. Immediate teardown matchesnative observedfalse/observedtrue remove/readd transitions. Moveoldsubscription tolocal, clear_thumbnail, thendestroyold; subscription itself holdsimagealive andclearedmembers protectreentrantnewsubscription. Officialcheck0.

## PDF load-emitter reentry (11:52 UTC)
Chromium156 PDFDocumentHelper::OnDocumentLoadComplete503-514 invokes its member callback vector, then clears it and calls client_->OnDocumentLoadComplete using the same helper. App delivery from our callback can destroy that emitter through KVO or didFinishPDFRendering. The helper callback now only validates/deduplicates the readiness receipt and posts DeliverPDF to the UI runner. No app/KVO callback executes inside the helper's load callback iteration. Navigation generation is checked at receipt only; an already queued notification is not canceled by later navigation. Realflag syntax check0. Bounded native proof below establishes original posting BEFORE KVO/app; exact first-ready trigger/registration invalidation/runtime behavior remains uncovered.

## DevTools inspected-contents reentry (11:51 UTC)
OpenDevToolsWindow can synchronously invoke the display delegate; owner contents may die before it returns. openWithAction now snapshots/checks the contents WeakPtr before computing the remaining opening state. GetInstanceForInspectedWebContents currently compares identity without dereferencing its argument, so this is stale/address-reused identity protection rather than a claimed direct dereference UAF. Realflag syntax check0, fresh readonly review clean.

## Original PDF-ready scheduling: bounded native proof

Read-only disassembly of `/tmp/arccore-webextras.arm64` establishes an asynchronous boundary **before** `setPdfController:` and the app callback. This is stronger than the earlier selector-send evidence; it does not establish a rendered-PDF runtime pass.

- Native producer `0x439c028` is the corpus closure anchor for `InvokeAsync`, `AsyncInvocation`, `../../arc/common/utils/delegate_wrapper.h` and `arc::WebContentsUIDelegate::OnPDFRenderingFinished`. Binary strings at `0xd37e47c`, `0xd37e488` and `0xd382eec` independently match those names. The producer copies member-pointer data from `0xd734ba0` at `0x439c1e8..1f0`; its two words are exactly `(0x1d8, 1)`.
- Its live-delegate path constructs inner callable `0x43a3570` (`0x439c1c4..204`), wraps it in a guarded async invocation, and posts that outer callable at `0x439c308`. The call supplies source location `delegate_wrapper.h:118` (`0x439c238..250`), a task object (`x2`) and zero delay (`x3=0` at `0x439c304`), through the runner's first virtual task-post method. There is no live-delegate inline invocation branch in this producer. The runner is obtained through thread-local storage at `0xe379858` (`__tlv_bootstrap`), then dereferenced at `0x439c218..234`; this audit proves posting to that native sequence, not the TLS runner's exact symbolic name or UI identity independently of its caller.
- Delivery goes through guard thunk `0x42c1740`, `0x42c0840` (weak registration/control lock at `0x42c0890..8bc`, inner callable invocation at `0x42c08c4..8d0`), then `0x43a3570`. The latter decodes the member pointer at `0x43a3588..5a4` and dispatches the virtual member at `0x43a35bc`. The shared UI bridge address point is `0xda89030`; offset `0x1d8` is slot `0xda89208`, which resolves to `0x4447ff0`. Thus the async closure is tied to the exact PDF bridge, not merely a neighboring delegate notification.
- Bridge `0x4447ff0` itself is synchronous. It loads retained weak app delegate and owner (`0x4448008..801c`), allocates `ArcWebContentsPDFController` (`0x4448020..8028`), sends `initWithWebContentsPDFController:` at `0x4448038`, sends owner `setPdfController:` at `0x4448058`, then sends `webContents:didFinishPDFRendering:` at `0x4448070`, and releases retained objects. Import resolution confirms `_objc_loadWeakRetained`, `_objc_alloc`, `_objc_msgSend` and `_objc_release`. There is no additional task post between the property setter/KVO opportunity and the app callback. In particular, the original snapshots the retained delegate before invoking the setter and has no post-setter generation/liveness branch inside this bridge.

The upstream helper-to-producer chain remains partial: `0x439c028` occurs in the native implementation vtable at `0xda81120`; a secondary interface vtable at `0xda81a38` has destructor slots and callback thunk `0x439c778` at `0xda81a48`. That thunk subtracts `0x78` from its receiver and branches to `0x439c028`. The stripped secondary interface name and the original PDF helper method that invokes it were not recovered, so this audit does not equate it with Chromium156's current PDFDocumentHelper callback vector or prove its first-load/first-paint timing. Current C156 `OnDocumentLoadComplete` still uses its members after registered callbacks; posting KVO/app delivery outside that iteration is consistent with the proven original delegate scheduling boundary. Exact runtime PDF readiness, queue/thread identity and destructive callback behavior remain unexercised by the accepted PDF golden.

### Receipt snapshot and navigation follow-up
Original439c11c/120 loads the PDFController shared pair from nativeImpl+308/+310, increments its strong count, and439c204 stores the pair into the queued innerclosure+48/+50.43a35a8..5b8 passes exactly that saved pair to the bridge, without reloading owner/helper. Constructor43717dc zeros+308/+310;439c04c/050 skips allocation/writes when+308 is nonnull; only nonzero direct/overlapping stores found in4370000..43a5000 are null-branch439c078/080. Destructor4372104/2114 releases it. This bounded store scan supports a lazy persistent per-Impl peer, not proof against generic alias writes or exact cross-document helper semantics.

Guard storage Impl+368/+370 is initialized by4371828, invalidated/cleared in4372070/2080. Delegate registration4375cdc replaces the weakdelegate pair+348/+350. Delivery42c1740/42c0840 checks the wrapper/cancellation lifetime and weak-locks the captured registration; there is no recovered navigation-generation comparison. Therefore production now validates/deduplicates in synchronous PDFReady receipt (no app/KVO) and queues weakobserver DeliverPDF without a late generation/helper/readiness check. readiness_posted_ resets on navigation and is separate from has_controller_, which tracks actual setter delivery; an old queued delivery cannot suppress the new document's receipt or cause a premature nil setter. Strong local delegate/owner/controller survive KVO; no observer/helper access follows setter. Official compile0 +fresh design review. Native first-ready eligibility and UIdelegate replacement-token behavior remain partially uncovered; exact runtime parity is not claimed.

## High-level DevTools close and reentry (12:47 UTC)
Original high-level close4357fd0 calls native DevToolsWindow::CloseWindow atc107c50 before synchronous DidClose. Current C156 delegate public base permits virtual dispatch to its private CloseWindow override (docked beforeunload/closing path; undocked browser close), rather than skipping directly to frontend ClosePage. Production uses that native call when a window exists and only falls back to ClosePage for a frontend with no window. A pending native window can be located from the inspected contents before the show hook. Hosted undocked-with-no-browser behavior remains unverified.

Reset/disarm the old frontend observer BEFORE native/app callbacks; retain old toolbox/pending blocks until the operation ends. Native close and DidClose can reopen F2. No cleanup follows those calls, so delayed F1 destruction cannot erase F2. Standalone webextras_devtools_close_regression.py extracts actual production struct/observer/reset/notify/frontendClosed/close;9cases PASS atO2/Wall/Wextra/Werror, source excerpt SHA833a200f34666c99a80fffa0874c37277035fe7d722c8508e6a0eaf202032a3d. Independent negative controls reject missing disarm, retired-state lifetime, or native close. This proves wrapper ordering/reentry/ARC lifetime/public-base virtual dispatch, not real native window behavior.

## Context DevTools uncovered identity (12:47 UTC)
ArcBrowserContextDevTools factory remains unwired. Native Extensions isEqual43f8f28 compares native shared Impl identity after self/nil/type/optional checks, not frontend WebContents identity; native hash43f91fc requires engaged Impl and hashes its pointer. Current shell has no such native peer, so isEqual/hash remain generated stubs; do not substitute ObjC/weak WC identity. Native context-close4437324 calls c107cd4 Close(reason3), distinct from the high-level service CloseWindow entry. Current context shell ClosePage behavior is unverified and must not be reported implemented parity. Existing goldens do not exercise these methods.

## DevTools display scheduling and modes (recorded native-run4 follow-up)

`results/webextras/native-run4/trace156/record.jsonl` records
`openConsoleView` call/return 3572/3573, then genuine engine-to-app
`webContentsDevTools:displayAs:` call/return 3574/3717, main, depth1,
mode3. The corresponding `observations.json` explicitly records display
`beforePublicReturn:false`. Ours records first display3863..4012 nested
inside open3858..4015, mode4, with `beforePublicReturn:true`; its full raw also
has another display4097..4136. The second display is outside that first action's
accepted callback span and must not be hidden by inspecting its JSON timeline
alone. This audit establishes differences, not a passing comparison.

Bounded original arm64 evidence proves the display adapter itself always posts;
the original after-return event is not explained solely by frontend load timing:

- Native display producer4359388 decodes dock-side text into modes:
  undocked1 (43593d0..43593f4), left2 (4359430..4359450), right3
  (4359470..4359494 via4359458), bottom4 (43593fc..4359428), unknown0.
  At43594ec..4359508 it passes label d38120a
  `arc::WebContents::DevTools::Delegate::DisplayDevToolsAs`, virtual-member
  slot0x10, and the decoded scalar to4359648. Delegate bridge vtable
  da88250+0x10=da88260 points to4415fe0. Its4416010..4416018 bounds mapping
  preserves1..4 and maps other native values to0. A Boolean docked?3:4 cannot
  represent this contract:4 means bottom, not undocked.
- 4359648 locks the current delegate bridge weak_ptr from native Impl+78/+80.
  It builds an inner closure with invoke435d2a0, copies and retains the bridge
  shared pair at inner+38/+40 (4359730..4359748), captures the bridge pointer
  at+30, and copies the mode into+48 (435974c). These are receipt-time captures.
- At4359770..4359788 the source location is the literal `InvokeAsync`
  (d37e47c), `../../arc/common/utils/delegate_wrapper.h` (d37e488), line118.
  43597c8..4359814 builds an outer task with invoke42c1740, guarded dispatch
  42c0840, the native wrapper weak flag/anchor from Impl+98/+a0, and the inner
  closure. 4359818..4359830 invokes the selected runner's post entry with
  zero delay. There is no direct-success dispatch branch in4359648; the
  missing/dead-bridge branch4359864..435993c also posts housekeeping, not a
  synchronous app notification. Close wrapper4358240 instead uses literal
  `InvokeSync` d37e54d / delegate_wrapper.h line61, corroborating that these
  source-location names distinguish scheduling paths.
- 42c1740 checks captured weak flag+4 and anchor before guarded dispatch;
  42c0840 locks the captured bridge and invokes the inner callable.
  435d2a0 loads captured mode+48 and calls the stored virtual member with
  the captured bridge pair. There is no frontend pointer, current-window lookup,
  or frontend generation in these closures. 4415fe0 weak-loads app delegate
  bridge+8 and service bridge+10 at delivery, holds the successful loads across
  the direct ObjC send441602c, then releases them. Do not strongly retain the
  ObjC service/delegate from receipt merely to deliver this task.

The native delegate wrapper's flag at Impl+98 is initialized in43569e8..4356a14
and invalidated during Impl destruction at4356c00..4356c10. Examined high-level
close4357fd0 and frontend-closed43586ac do not invalidate that flag; they retire
window/toolbox state without a display-generation check. Thus a received display
notification is not locally cancelled merely because that frontend closes or a
new frontend opens before task delivery. Queue the notification after immediate
native frontend state association; do not queue/replay the whole state setup or
let an old task overwrite a replacement frontend. No state should be touched
after the app callback unless independently protected against its reentry.

ObjC setDelegate4414f60 updates the existing bridge's weak delegate for a nonnil
replacement, but nil clears its owning shared pair. A previously posted display
retains that old bridge pair; a later new bridge is a separate registration.
That distinction matters if approximating cancellation through registration
tokens. A late current-delegate read alone does not reproduce old-registration
behavior. Weak delegate/service loads still decide whether app objects exist at
delivery. Full alias-write coverage of all native guard mutations was not
proved by this bounded audit; the inspected close paths establish no local
close cancellation, not an absolute claim about every possible native teardown.

The runner is fetched through TLS e379858; its exact named sequence identity was
not recovered here. The actual run4 callbacks are main-thread. C156's hosted
frontend source separately clears can_dock without a Chrome docking controller,
then ScheduleShow can synchronously Show the undocked frontend before load.
Original opening43572f4 explicitly requests can_dock1 before creationc1057c0;
that is an input proof, not blanket proof that every original creation keeps it.
Preserving correct hosted docking/load semantics and posting the proven display
adapter are separate required changes. No production source or runtime was
changed by this audit.

NIL-setter instruction check:4414fb4 loads the old control block,4414fb8 clears
only the service's bridge pair with `stp xzr,xzr,[self,#0x10]`, and4414fc0..4fcc
decrements its strong count and returns directly when a queued closure still
retains it. There is no weak-delegate store on this branch. By contrast,
nonnil-existing4414f88..4414f98 calls the weak-store routine on bridge+8.
Consequently nil then new registration preserves a queued old bridge with its
old weak delegate, while direct nonnil replacement updates the bridge seen by
already queued delivery. A registration's validity token must not be invalidated
by nil to approximate this behavior; capture that bridge strongly while keeping
its ObjC service/delegate fields weak.

4415fe0 has no explicit nil-service check after its weak load. If the app delegate
still exists while the weak service load yields nil, the direct send passes nil
as the service argument; a nil delegate naturally receives no message. An
implementation that cancels solely on weak ObjC service disappearance adds a
condition absent from this bridge. Whether a native Impl can outlive that ObjC
service in each owner lifecycle is not established here; its outer lifetime
guard must not be confused with the bridge's weak ObjC fields.

## DevTools frontend getter fresh wrapping: native-run4

Original `webContents` getter44151a0 always allocates an ArcWebContents when
DevTools Impl+68 is nonnull:44151b8..44151bc reads the frontend peer box,
44151c4..44151cc allocates, and44151d8..44151e8 reloads that same Impl field
and passes it to `initWithWebContents:` (selector slot e28f8e8). There is no
wrapper-cache check, current-window lookup, or native WebContents creation in
this getter. The toolbox getter4415204 is different: it uses Impl+70 and
compares its cached wrapper's native peer before reusing/rebuilding that cache.

Run4 original raw `webContents`3615..3676 returns wrapper127 and later
3760..3821 returns wrapper136, each with its own initialization boundary
sequence. Ours3904..3967 and4090..4093 returns canonical wrapper111 both times.
Unconditional fresh wrapping of the same native peer **field** explains those
different ObjC identities without requiring a native frontend replacement.
The serialized `<ptr>` arguments do not establish equality of the field's
contents across the two calls. The bounded owned-Impl scan finds +68 written
at creation4357490 and cleared at close4358084/4358744/destruction4356c98;
it is not exhaustive alias-write or runtime native-identity proof. Preserve
the canonical-wrapper architecture pending an explicit lead decision about
this native fresh-wrapper behavior; no allocation workaround was made here.

Further registration detail: only the new-bridge setter branch calls native
SetDelegate, at44150bc ->4332f08 ->42ede24. 42ede50 writes the native weak bridge
pair to Impl+78/+80. Neither the nil branch nor existing-nonnil weak-field update
does that native call. Therefore nil leaves the native weak registration alias
pointing at the old bridge. If an earlier queued notification still retains that
bridge, **a new native display receipt after nil can still lock and queue it**;
after all strong captures drain, that weak registration expires. A new nonnil
registration installs a new alias; existing queued captures keep their prior
bridge. The owned approximation needs both current strong registration and a
separate native-registration weak alias, not merely `_delegateBridge` at receipt.

## Inspect-mode trigger: original embedder-message route

A bounded native chain establishes a direct **embedder message** producer,
separate from the frontend CDP transport. It does not establish an
`Overlay.setInspectMode` packet interceptor:

1. Original DevToolsUIBindings constructor calls frontend dispatcher creation
   c0eb6fc at c0f7774 with the bindings object. Its primary address point is
   dd00b80 (installed c0f7538..c0f755c).
2. Dispatcher creation registers literal `inspectModeChanged` at d701069 via
   c0ebaa0..c0ebae0, passing virtual-member slot50 and flag1 to c0ed650.
   That registrar stores the member pair/receiver and installs parser
   c0ef394 (c0ed680..c0ed6c4). The separate literal
   `dispatchProtocolMessage` d7011e4 is registered at c0ec5a8..c0ec5e8 with
   member slot160, not slot50.
3. The inspect handler requires **exactly one argument**: c0ef3d8..c0ef3e0
   compares the supplied list size to1. c0ef528..c0ef588 checks its first value
   via c483aa0 ->170dd80; that helper returns nonnull only for value type4
   (string). Zero/extra arguments or a nonstring returns false without invoking
   the selected method. c0ef510..c0ef518 invokes the bound handler only after
   successful parsing. Missing/malformed outer embedder-envelope behavior was
   not separately recovered by this bounded parser audit.
4. Bindings vtable dd00b80+50=dd00bd0 points to4718fd0. This thunk loads
   bindings delegate+58 and invokes its slot50. The DevToolsWindow delegate
   address point dd01bf0+50=dd01c40 is471a128 (the same table's+20 is native
   CloseWindow c107c50, corroborating its identity).
5. 471a128..471a168 requires window inspectedWC+50, looks up userdata key
   c9d7108, requires userdata+10 native peer, then tail-calls4359a74 with the
   parsed string. That reaches the recovered string-to-mode conversion and
   asynchronous slot18 delegate bridge described above. Any string is accepted
   by this local parser: recognized mode names map1..4; all others, including
   `none`, reach mode0. A missing inspected WC/userdata/peer produces no callback.

No agent-host attachment test, target/session lookup, protocol-method comparison,
or CDP success result occurs in this direct route. That statement is local to
the recovered path; it does not prove no alternative producer exists. The
original frontend JS emission site and the generic outer embedder dispatcher
validation/session provenance remain unmeasured here. Current C156 has no
`inspectModeChanged` frontend embedder handler or bindings/window delegate
method. Its outgoing `dispatchProtocolMessage` hook is a different route; any
attempt to infer these app notifications from Overlay packets needs an explicit
equivalence decision or separate native evidence, including duplicate prevention
if an embedder emission is later restored. This finding does not activate a hook
or fabricate an inspect-success callback.

Adjacent display-trigger evidence: original literal `requestDockPosition`
(d701055) is registered at c0eba34..c0eba78 with member slot48 and the same
one-string handler c0ed650. Bindings dd00b80+48=4718fc0 loads its delegate+58
and invokes slot48; window table dd01bf0+48=471a0d8 requires inspectedWC+50,
userdata c9d7108 and native peer+10, then tail-calls display producer4359388
at471a118. Thus an original display callback can be requested explicitly by
this frontend embedder message. The current Show-hook/GetDockSide adaptation
does not, by itself, establish full equivalence for every explicit dock-position
request or message multiplicity. Normal-open runtime measurement remains useful;
the async adapter and mode mapping proof above does not depend on this trigger
equivalence. No activation or production change follows from this finding.

## Original packaged frontend emission proof

The disposable original app's framework `Versions/A/Resources/resources.pak`
was read without launching an app. SHA256
`56734b34502ff1e68d72a9d36b19715a8e55fd3b910a8ce7839304cbd0fe07bc`,
size24824865, mtimeNs1792616318000000000, stable before/after extraction.
Correct extraction uses `chromium/src/tools/grit/pak_util.py extract` with
**explicit** `--brotli /opt/homebrew/bin/brotli`. An initial search with the
tool's default shell-based Brotli invocation left frontend payloads incorrectly
decoded; that no-match result is superseded and supplies no absence evidence.
Clean explicit extraction is byte-identical to the recovered JS below.

The original Mach-O resource-name table de7b7f8..de82378 has860 records, stride32,
mapping IDs52161..53020 to bundled frontend paths. Mapping source
`/tmp/arccore-webextras.arm64` SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
Extracted artifacts, table JSON, byte-offset fragments and hashes are at
`/tmp/webextras-frontend-emission-_mseb9oi/frontend-emission-evidence.json` and
`resources-correct/<resourceID>`; no application/private profile was read.

| Resource ID and path | Decoded SHA256 | Relevant byte offsets |
| --- | --- | --- |
| 52490 `devtools_compatibility.js` | `5155e65c618a6776d555abd85bcafdabe1d4fc606aaeeaa6292cea1eb3c6217c` | requestDockPosition45044/45121; inspectModeChanged45170/45245 |
| 52853 `panels/elements/elements.js` | `2616bfe4be3a3ce9eab925f250452f2f1e21398c763cb1a0c790787092e32702` | inspectModeChanged575760 |
| 53015 `ui/legacy/legacy.js` | `723be0b9811a6686918ea2889e72a69466d07197fcb3313804f86fa86884853e` | requestDockPosition30936/31298 |
| 52476 `core/host/host.js` | `7c9cb7cc1b64d651ec04cab8704414c504ecdbdee23a3129505e521ef58583a0` | empty nonembedded stub methods23522/23546 |

### Inspect controller emitter

Actual original `InspectElementModeController.setMode(mode)` first checks
`!TargetManager.instance().allTargetsSuspended()`. Inside that guard it sets
the global mode, loops over **scoped** OverlayModels and calls each model's
`setInspectMode(mode, tooltipSetting)` without awaiting, updates the search
action toggled state, and finally calls the host's `inspectModeChanged(mode)`
**once after the loop**. There is no equality/deduplication test for repeated
same-mode calls. The host notification is independent of target count and
backend command completion; it is not one notification per Overlay target.
A normal model's asynchronous setInspectMode waits for requestDocument before
calling its highlighter, so this controller send is not a backend-success ack.

Other original controller paths fix the remaining repeat/suspend distinctions:

- Search-toggle chooses None versus SearchForNode/SearchForUAShadowDOM and
  calls setMode; capture-area screenshot also calls setMode.
- Escape while active calls setMode(None). The scoped EXITED_INSPECT_MODE
  listener also calls setMode(None), subject to the same suspension guard.
- Tooltip-setting change calls setMode(this.mode), including repeated values;
  it emits again when not suspended.
- modelAdded applies an already active mode to the newly added OverlayModel,
  but does **not** call the host notification.
- suspendStateChanged directly writes mode=None and search-action false when
  all targets are suspended, with **no** host notification. A suspended setMode
  call skips the entire body. Do not add an inferred suspension-reset callback.
- Constructor initializes None without sending. The action delegate refuses
  the shared-worker query-param case. No original controller action for
  showDistances was found in this recovered body; the native receiver's support
  for that string does not authorize inventing such an action.

The compatibility layer's `inspectModeChanged(mode)` directly calls
`DevToolsAPI.sendMessageToEmbedder('inspectModeChanged', [mode], null)`.
sendMessageToEmbedder increments an ID, stores a completion only when a callback
is truthy, serializes `{id, method, params: [mode]}`, and calls DevToolsHost.
There is no callback promise or session/target envelope on this notification.
This directly corroborates the one-string native handler recovered above.

### Dock controller emitter

Actual original DockController constructor stores canDock; when false it sets
its local side to undocked, hides close UI and returns before registering the
dock-setting change listener. initialize() in that case explicitly sends
requestDockPosition('undocked') and returns. With docking enabled initialize()
calls dockSideChanged(), which feeds the current setting to setDockSide().

setDockSide validates against `['right','bottom','left','undocked']`, falling
back to right for an invalid input, and returns early if the requested side
already equals the local side. Otherwise it calls requestDockPosition(side)
**before** updating body CSS, last-side/current-side settings, dispatching
BeforeDockSideChanged, or calling setIsDocked(side!='undocked', callback).
After that call it updates close UI and dispatches DockSideChanged. The later
setIsDocked callback dispatches AfterDockSideChanged and announces location.
Thus display-request emission is not gated on the docking callback/ack.

Compatibility requestDockPosition(side) uses the same direct embedder sender
with `[side]` and no completion callback. The native message receives the
requested side itself; reconstructing that value later from profile settings
does not establish identical request-time ordering. The current Show adapter
still requires runtime comparison for its narrower ordinary-open behavior.
These recovered original emitters support a dedicated controller/compatibility/
one-string browser-handler integration proposal. They disprove treating generic
outgoing Overlay packets as an exact substitute. No hook/source activation or
runtime acceptance was performed for this resource audit.

Durable original-frontend evidence JSON/excerpts, fullresource table and hashes are also preserved in logs/wp/webextras-original-frontend-emission/; parent rehashed the original PAK and all four correctly decoded resources before preservation. Full decoded resources remain in the cited temporary extraction, reproducible from the unchanged original PAK with the explicit extractor above.
