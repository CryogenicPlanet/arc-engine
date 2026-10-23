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

- General setVolume requires exposing a Chromium player-volume operation through a lead-owned patch; current wrapper returns code6 unavailable after checking native session.
- PDF hasBookmarks/getBookmarks require renderer PDF bookmark queries; getBookmarks callback string schema still needs recovery/golden coverage.
- PDF visiblePages needs every visible page; upstream browser PDF helper only exposes GetMostVisiblePageIndex. Current wrapper returns unavailable instead of substituting one page.
- PDF page count and page text use actual PDFDocumentHelper methods. PDF helper creation/load observer only exposes full-page PDFs, invalidates the controller on committed non-same-document navigation, and calls optional uiDelegate didFinishPDFRendering after setting the cached controller. Existing final goldens do not cover these paths.

## Lifetime audit

- Media WebContentsDestroyed detaches its native observer before calling the app; app callbacks may setDelegate:nil and delete the observer.
- PiP creation callback occurs on the first SetPlaybackState, after the native controller owns the overlay, with a weak check after app reentry. Overlay resizing updates compositor state first and calls UpdateLayerBounds last.
- PDF callbacks use weak observer pointers and navigation generations. Controller state is marked before calling the ObjC setter; a weak/generation check precedes the app PDF callback.
- Thumbnail subscription removal is posted to the main queue because ThumbnailImage forbids unsubscribe during its image callback. Observers are weak/deduplicated by pointer identity; callback enumeration takes a stable strong snapshot.

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

`arc/browser/webextras_values_probe.sh` compares original and ours without initializing a browser/profile. Three owned classes (AccessibilityActionData, default AccessibilityTreeNode, SidePanelEntryInfo) match all observed values,21method encodings,15property attributes; recorded and nonrecorded results equal. `results/webextras-values/diff.txt` reports zero missing/extra/returns/arguments/counts/internal differences. It does not test live accessibility production or native-pointer input compatibility.

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
