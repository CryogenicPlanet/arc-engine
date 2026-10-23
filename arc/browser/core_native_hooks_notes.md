# Core native hook proposals (unapplied)

Production Chromium and shared files remain untouched. The lead owns integration.

## Current proposed pair

1. `core_navigation_chromium.patch`: add the leaf public `content/public/browser/host_app_hooks.h` and callback slots defined in existing WebContentsImpl. Forward actual NavigationStateChanged flags before Chrome's delegate, and UpdateTargetURL/ClearTargetURL after it. Guard delegate/callback destruction with weak native identity. No WebContentsObserver vtable change or broad content header rebuild.
2. `core_navigation_registration.patch`: core-owned companion, generated against current production ArcWebContents. Register slots; remove guessed Ready/start/finish URL-ID pairs; preserve lead services/renderer/sessions integration. Exact flag order URL, TITLE, LOAD, TAB, AUDIO; URL's old counter is published then incremented, beginning at zero; committed URL takes precedence, fallback visible URL for initial navigation. Real queued snapshots retain event-time payloads. Internal direct UI delegate access avoids exposing synthetic internal getter calls; public getter remains.

Recovered native evidence: NavigationStateChanged producer0x4385538; URL slot0x60 ->0x44137b0; TITLE slot0x58 ->0x441371c; LOAD slot0x28 ->0x44130fc; TAB slot0x18 ->0x4412fe0; AUDIO slot0x20 ->0x4413050. Target producer0x4388630 forwards underlying Chrome before formatted status; both target and target-clear source paths must be wired. Default target width400/font is recovered; nondefault font/width remains uncovered. Special URL rewrite rules and numeric audio traits remain uncovered.

Scheduling audit corrected the earlier policy-buffer design. Native flag branches enqueue immediately in URL, TITLE, LOAD, LOAD-status, TAB, AUDIO order, each a separate zero-delay UI task. URL advances its counter at receipt; LOAD captures loading but queries back and forward separately during delivery. All navigation lifecycle callbacks are also posted per event-time registration token; remove/readd expires an old registration. Target status remains synchronous after Chrome. No policy barrier or pending-entry pin exists in the original: normal same-stack policy naturally precedes posted delivery, while async beforeunload/nested loops may allow early flags first.

The current companion stages WebContents; navigation async registration already landed independently. It removes all artificial policy queues, captures native flag snapshots, uses separate UI tasks, and models UI delegate bridge lifetime: every nonnil assignment replaces the stored bridge; nil unregisters future delivery but retains the stored bridge so already queued old status may survive. Public delegate getters and lead integration remain. Actual compilers for staged WC, staged navigation, and native WC exit0 in logs/wp/core.navigation.proposal.check.log. Fresh review P1 is CLOSED: sync zoom KVO now has the prior native-liveness/generation guard before controller receipt. Navigation async registration delivery has independently landed in owned production; the remaining companion therefore changes WebContents only. The display companion below is also ready for integration with the native hook patch. All sources remain private staged until the lead applies the bundle.

Explicit remaining gaps: AUDIO trait producer mapping; dynamic nonempty target text font/width; exact cancellation lifetime of native DelegateWrapper versus weak ObjC owner. Runtime validation remains necessary after native patch + companion are applied together.

Latest source/proposal corrections supersede the older trait-gap statement:
core_traits_publication_notes.md resolves cached alert/ICE mapping and native
first-audio/PiP ordering. Current generic/observer drafts publish traits once for
TAB|AUDIO after favicon, retain a native vector at receipt, and materialize a
fresh IndexSet per delivery before weak ObjC owner load. Nonempty provider remains
unconnected; core_tab_traits_proposal.patch is compiled/reviewed but HOLD for
creation lifetime/rebind/caller integration. Current four-part apply/compile
manifest is logs/wp/core.bundle-current.20261023T133616Z/manifest.json; current
generic hashbb79407a and observer7e1cca4e. Earlier hashes are superseded.

Hosted-empty proposal135062 is also superseded: current95c46cc5 adds explicit
force-skip exceptions in both idle guards, fixing a reviewed forced-close P1 in
the old unapplied draft. Native Browser/Unload compilers0/apply0/fresh review
clean; core_hosted_empty_browser.md and logs/wp/core.hosted-empty.latest.json.
Ready UNAPPLIED139280; source/compile success is not runtime replacement parity.

## Independent requests

- `core_hosted_empty_browser.patch` (135062): Browser::TabStripEmpty/UnloadController idle transition must keep an empty hosted browser for replacement tab; explicit close remains normal. This fixes missing second wrapper/navigation path in launch-slice. Applycheck passes; runtime unapplied.
- App135358 owns native nested-runloop/keepalive shutdown patch; core must not work around quit with forced termination.
- Renderer135545 owns process-global skip-user-activation/ignore-disable-PiP controls and shared declarations. No per-tab caching/replay.
- ArcColor4f134662 needs the lead-owned value implementation for real metadata payloads.

## Held/rejected artifacts

`core_precommit_chromium.patch` and `core_precommit_registration.patch` are RETIRED: broad original goldens prove generic native flag publication, not an initial precommit URL replay. Do not apply.

`core_close_chromium.patch` and `core_close_registration.patch` remain HOLD even though private overlay3TU compile0: Close() bypasses ClosePage, CloseAllTabs bypasses CloseWebContents, and generic native callback context is not WebContentsObserver context. Enabling synchronous public close requires complete guards for those paths before deleting blanket deferral; partial guards are unsafe. Source-only preserved for review, not ready integration.


## New proven display hook (same native proposal)

The current native patch also adds HostFrameTokenCallback and calls it at the end of RWHViewMac::OnFrameTokenChanged, after frame-token queue processing, with a weak-view guard. RWHMac and nativeWC actual compilers0 (`logs/wp/core.render-token.proposal.check.log`), applycheckPASS. Original3a88eb0 ->25e0eb8 writes native readiness+0x174 and posts observer0x88; paint callback independentlyno-op. This hook alone is NOT yet a complete readiness implementation: native CompositorObserver ack and CopyFromSurface also publishtrue after receiving a frame; their exact sender/registration semantics remain under audit. Do not replace readiness with metadataAfterActivation: original metadataBefore callback3649040 ->36490a0 is proven, originalAfter4168ec0 only caches background; frames can activate with unchangedmetadata.


## Reviewable integration bundle (ready, unapplied)

Apply all three together:
- `core_navigation_chromium.patch`: leaf callback header + existing nativeWC getter storage + RWHMac frame/copy/occlusion sites + BrowserCompositor ownattach/detach/destructor sites. Actual nativeWC, RWHMac, BrowserCompositor compilers0, applycheck --whitespace=error PASS.
- `core_navigation_registration.patch`: WC flag snapshot/queue and UI bridge token companion; removes unsupported direct DidFirstVisuallyNonEmptyPaint readiness. Current production navigation token queue is already landed, not repeated in this patch. Actual stagedWC/navigation/nativeWC compilers0; applycheck --whitespace=error PASS.
- `core_display_registration.patch`: helper only, posted metadata/ready with weak ObjC owner; per-widget receivedframe flag, own-compositor ACK observation, no copy/fallback flag creation, unparented false without clearingflag. Actual helper compiler0/no diagnostics, applycheckPASS; two fresh read-only reviews including corrected weak-owner/no-native-cancellation task semantics.

View hook consumers only update observations or enqueue UI work; no synchronous app/KVO calls in native frame/copy/unparented or own-compositor hooks. Native callbacks preserve original WasOccluded hostIsHidden early return. Ownobserver attach beforeUnsuspend; null detach before recycling; destructor null event after weak invalidation removes registry even if alreadydetached. Original disableCompositorRecycling flag0x1d defaultsfalse; override is not modeled and remains uncovered, as does native occlusion field+0x4f0. Other WC progress/history/security scheduling audit is continuing separately; these gaps should not be mistaken for complete core acceptance.

## Observer scheduling follow-on (ready, unapplied; request136683)

`core_observer_registration.patch` must follow the three-part bundle136490. It is now WebContents-only; its navigation/header changes landed independently. Its WebContents base is the staged navigation companion. Actual staged WC/navigation compilers exit0 (`logs/wp/core.observer.proposal.check.log`), staged-base applycheck with whitespace errors rejected passes, fresh read-only review reports no P1/P2.

The history observer is a separate native WCO (`da7b748`), with exactly four non-no-op callbacks: committed+0x170, pruned+0x178, deleted+0x180, changed+0x188. It updates a shared cached native model, retains that same model in posted delivery, then materializes Objective-C arrays at delivery. Changed compares cached title AND VirtualURL, updates and posts only if either differs; invalid cached index traps. Original title update itself stores title then sends TitleWasSet+0x208 (NOOP for this observer) and TITLE invalidation. Chromium156 has the same signal shape: do not invent a title/history producer to chase counts. Source evidence `/tmp/core-history-title-{sender2,upstream,callers}.txt` and `/tmp/core-history-changed-{sender,dispatch}.txt`.

Progress and theme capture payloads before posting. Security snapshots the mapped final security level and captures the active UI bridge at receipt; context construction occurs at delivery. Focus gained first posts through weak native identity, then posts the weak-owner property; focus lost directly posts the property. Visibility posts its own enum and does not synthesize state from IsCrashed.

Correct native state provenance supersedes preliminary crash/readiness guesses: state1 comes from discarded WebContents replacement (OnTabStripModelChanged4397de8); state2 from a freeze routine4374f14 after SetPageFrozen(true); state0 from genuine WebContentsHostView::viewDidMoveToWindow473a9d8 with a nonnil window, observer+0x10→4398404→43981ac, native/browser/helper guards and unfreeze. That attachment path is still being audited, not approximated by every bounds or visibility change.

## Independently landed owned follow-ons

Production ArcBrowser now implements tryToCloseSkipBeforeUnloadHandlers:callback: (136887): cancellation BOOL, FIFO coalescing/escalation, inline NO before native Close, posted YES after unload reset, native teardown draining and weak liveness rechecks. core_browser_close.patch is an ALREADY APPLIED review artifact; do not reapply. Actual staged compiler0, production check-file no diagnostics, fresh review clean; runtime beforeunload/cancellation still pending.

The navigation/header portion of observerpatch136683 and all core_navigation_values.patch have independently LANDED in owned production. The observer artifact is now WC-ONLY; its base remains the staged generic navigation companion. The shared history model is structurally in place; existing production snapshotForWebContents still materializes immediately until the WC observer companion starts capturing it for queued delivery. Navigation values now retain shared URL/status/redirect fields and snapshot headers per wrapper construction; callbacks construct wrappers at delivery. No native handle is retained by a queued callback. Nav-values patch is an ALREADY APPLIED review artifact; do not reapply. Finish-load FIFO/URL update, command preallocation, actual cross-tab true producer and exact process-address hash remain uncovered. ArcNavigation native init: has C++ aggregate ABI and is excluded by the binding architecture; no invented ObjC initializer was added.

## Latest current-base verification and owned fixes

`logs/wp/core.bundle-current.20261023T122644Z/manifest.json` records strict sequential
apply checks for native, generic WebContents, display and observer patches against
current production. Actual combined WebContents and render helper compilers both
exit0 with no diagnostics. It preserves newer existing-only diagnostic lookup,
canonical-marker query, popup count/getters, historical -1 fallback and deferred
Close target. Apply incremental patches; copying an older staged WebContents file
would lose independently landed work. Native hook header remains absent.

Browser type/active correction and diagnostic lookup artifacts are ALREADY APPLIED:
`core_browser_type.patch`, `core_diagnostic_lookup.patch`. Browser Chrome raw
0/1/4/5 maps to public0/1/2/3; popup/app-popup force insertion active, otherwise
current active identity is captured before callouts. Existing-only diagnostic
lookup never creates a wrapper or allocates an ID; shared declaration is pending.
Public tab close still defers, but its final target is WebContents::Close rather
than ClosePage. `core_close_accounting_plan.md` maps a coordinated accounting
change and its remaining native prerequisites; independent review found no P1/P2.
Do not activate the held partial synchronous-close patch.

`core_navigation_followon_notes.md` corrects older stage notes: native436a748
finishes,436ade4 fails; both pop committed values FIFO and update URL from frame
callback GURL. Command redirected-to-other-tab flag compares post-Navigate
source_contents, not destination output or HTTP redirect. FIFO overlap, public
command return identity and BFCache adaptation remain uncovered/unimplemented;
no current golden establishes those changes as a runtime fix.
