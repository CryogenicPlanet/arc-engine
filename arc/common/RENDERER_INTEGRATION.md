# Renderer integration, 2026-10-23

CURRENT RELEASE: corrected UNAPPLIED TopBandd106f59e replaces withdrawn88.
Dependency order ALL e07 → gradient661 → TopBandd106.19actual-flags checks0/empty;
38fixtures compiled NOT executed; native-contract and canonical replay reviews pass.
Native geometry and active-pending/no-area findings are closed. Historical88 is
archived; dependent723 remains HELD while privately rebasing. Count-only discard21a
remains independently HELD for core global phases. Production is unchanged.
Public TopBand flag is31 (36 is Snapshot); startup relay is proved but not integrated.
Native live propagation, commit refresh activation and private ABI remain open.


## Current handoff, 2026-10-23 13:36UTC

Production remains unchanged since the verified exclusion/compiler-sequence fixes.
Published UNAPPLIED order: ALL `e07d1b39` → gradient `66179056` → corrected
TopBand `d106f59e` → optional startup `746ac8cc`. TopBand19real-flags checks0/empty,
38fixtures compiled NOT executed; canonical replay/native-contract reviews clean.
Geometry admits edge/fullbleed; strict interior needs coalescing+all3quarters.
Active timer/pending/no-area stops+empty; missingView returnsquiet.88iswithdrawn.

Startup746 supplies the pre-Chrome installer/shared declaration/weakdefault and
hosted renderer child callback reading public31. Native name lookup proves31TopBand,
36Snapshot,43HEIF. No live IPC was invented. Real GN/link/weak-provider resolution,
replacement runtime and conflicting feature-override behavior remain unverified.
Commit refresh remains unwired; native receipt order is under a final placement
review. Ordinary nonnested refresh precedes posted ObjC commit delivery; absolute
nested/reentrant delivery order remains unproved. Private C++handle ABI is in recon.

HELD count-only discard21a or full-stack723 (ALTERNATIVES, never both): retained
native peer count read before teardown, seeded before new publication, KVO synclast.
723transfers CURRENT weak TopBand payload includingnil, clearsoldmetadata, and copies
no document sampler/cache/token/RFH state. Startup746 and723 compose BOTHorders
byte-identically across77files. Core canonical mapping/global old-retirement→new-
attachment phases and target-owner reconciliation still block activation.

Native next-controls batch87533/tool22999 completed3×13stages and drained0.125audit
checks pass; all restorations/natural exits and eight23-input guards pass.
Public31-only/base/coalesced callbacks11/11/8; actualnil2/2/2; crossfadetrue9/9/6.
Same-document default inset→fullbleed→inset giveswhite→red→white; coalesced insetred.
Actual hardgradient/decodedSVG nils have browser surface-copy completion stackproof.
No injected TopBand switch in public31 mode; fresh switched descendants observed,
CDPtarget→PID remainsunproved. AllcontrolStatusINCONCLUSIVE/paritynull;2RAFs are
opportunities, not presentation receipts. Parent viewed original inset/gradient/IMG
PNGs; those3images byte-identical acrossallnative controls. Audit/provenance:
logs/wp/renderer-topband-next-native-audit/ and visual-review.run2.json. Nativeonly.

Peer UI37/37 fresh-profile renderer replay/natural quit remains scoped evidence.
10strict CSScalls each match preservedv1 and current attributednative. Current-native
vs historical UI global57 is diagnostic; old84 remains pinned tov1. Twoextra theme
calls are app-origin, upstream schedulingunproved, no renderer workaround. Official
results/ours/renderer-public-api remains absent. Timestamped census13:35:58UTC:
24canonical/29launches/12v1archivesexcluded/21shared diff snapshots; all27renderer
rows remain visible. See run2report/annex for literal first2headers. Acceptance
is in progress; no own app/build/waiter is active.

## Current adoption artifact

Use the consolidated renderer_all_integration_proposal.patch INSTEAD of its
component proposals below. Current SHA256 is
e07d1b39dd68beb59af325ab90446c760cc731911a2caf422d766ac20c0612bf,
rebased onto the applied false-feature exclusion and serialized-compiler corrections. Follow it with
renderer_gradient_followup_proposal.patch, SHA256
66179056ee5ae3a5c76f226e805ca86a8652279ea9f8fd15ac7ec799bf129e80.
The follow-up requires that exact ALL version. See the respective REVIEW.md
files under logs/wp/renderer-{all-integration,gradient-followup}-proposal for
composition, app overlap/dedup, checks and remaining real GN/link/runtime work.
All are unapplied. A separate discard attachment proposal remains HELD.

The sections below explain mechanisms and individual inputs for review; do
not apply their patches again alongside ALL. The serialized-compiler correction is applied, checked and included in this
rebase; component/ALL/gradient checks total74successful syntax invocations.

## Mechanism and landed hooks

Browser-owned `RendererBridge` follows each WebContents/frame for direct CSS,
Boosts and real blocked-count endpoints. A profile-aware process registry binds
an associated `RenderProcessAgent` per renderer process; the process owns rule
mapping, exclusions and cosmetic cache shared by every frame. Debug selects the
oldest surviving process binding, independent of requested URL. Internal Mojo
is our transport, not a copy of proprietary binary messages.

Patch 0005 registers frame agents from `RenderFrameCreated`, host-gated; patch
0004 forwards the host switch. Core initializes the bridge when creating an
ArcWebContents wrapper. Those hooks are landed and rendered CSS is verified.
Owned first-frame/first-use fallbacks support the new process agent while the
exact startup/launch/profile hooks below remain pending.

## Pending shared changes requested from lead

### Native document-start hook

At the start of `ChromeContentRendererClient::RunScriptsAtDocumentStart`, before
extension dispatch (which can destroy the frame), call:

```cpp
if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app") &&
    !arc::RunEffectsAtDocumentStart(render_frame)) {
  return;
}
```

Declaration/definition exist in `arc/renderer/render_frame_agent.{h,cc}`.
The receiver caches profile configuration without applying it to the current
document. Document-start applies exact-host main-frame Boost CSS and isolated
world 6 JavaScript, then cosmetics for all surviving frames. False reports that
Boost JavaScript destroyed the frame, so Chrome must skip extension dispatch.
An observer callback after
document creation does not establish the recovered native timing contract.

### Process and profile lifecycle

The complete apply-checkable proposal is `renderer_pending_hooks.patch`.
No shared source was changed by the renderer worker.

- Include `arc/renderer/render_process_agent.h` and call
  `arc::EnsureRenderProcessAgent()` in hosted `RenderThreadStarted`.
- Include `arc/common/ad_block_browser.h` and call
  `arc::RendererProcessWillLaunch(host, true)` after existing Chrome setup in hosted
  `ChromeContentBrowserClient::RenderProcessWillLaunch`. Native registration
  sequence is reserved here, before readiness/acknowledgment. The true argument
  is valid only when the paired renderer-thread startup hook is included in
  the same build. The default false argument safely delays actual binding
  until a frame acknowledgment proves the process binder is registered.
- Declare/call `arc::InitializeRendererProfile(profile)` when Profile is created
  or adopted. Lead-owned `arc_browser_context.mm::initWithProfile:` is the
  earliest wrapper adoption point in the proposal. Actual earlier Profile
  birth is preferable if it can predate wrapper adoption and feature changes.

Global manager rules are optional and differ from retained per-profile rules.
Disable clears the global pair without sending updates; profile/process rules
remain. Enabled registration inherits profile snapshots; disabled registration
receives no initial rules/exclusions. Reenable applies an engaged global pair
(if any), then exclusions; it does not substitute profile caches into already
bound false-born processes. Disabled explicit exclusions update only the global cache. Enabled profile
updates enqueue weak broadcasts; each checks live feature11 and reads the
latest profile origins. Re-enabling publishes the latest global exclusions.
Installed renderer matching remains active while disabled. See
renderer_exclusion_feature_gate_evidence.md for the corrected native proof.

### Runtime feature lookup and refresh

App worker implemented `bool ArcRuntimeFeatureEnabled(NSUInteger)` and made
its public getter delegate to that same table/ID45 special case. Lead must
still declare it in `arc_internal.h`; renderer will then switch its temporary
public-getter fallback to avoid extra native boundary calls.

App feature11 setter must call `arc::RefreshRendererAdBlockFeature()` outside
the feature lock, with a weak no-op caller fallback if renderer may be absent.
The proposal contains this change. Refresh updates the global manager/profile
state; renderer filtering itself is not gated by feature11. Calling refresh
inside the feature lock deadlocks when the native helper reads the table.

### Blink visual hooks

See `RENDERER_VISUAL_GAPS.md` for the recovered Page-scoped optional color scheme
(higher precedence than media/preference overrides), Page/paint HSLA filter,
and native top-band tracker. These require lead-owned Blink/cc patches. No
WebSettings, CSS/SVG, or root-background approximation was added.

## Golden coverage

Accepted scenarios cover direct stylesheet insertion/removal, top-band delegate
registration, zero blocked counts and empty excluded domains. The accepted
renderer-public-api baseline additionally covers14computed-style states and two
rich recursive extractions. Scenarios owns its official fresh-profile replacement
run; our earlier standard-scenario batch preserved disposable profile state and
is supplemental. Exact raw fixture-call and payload gates are required because
the shared default caller filter can omit injected-image calls.

No accepted scenario yet exercises active Boosts, active ad blocking/debug,
color filters/schemes or actual top-band callbacks. Renderer-owned supplemental
native/replacement fixtures retain genuine calls and raw outputs for those paths.
They have exposed real mismatches and are not labelled canonical diff-clean.

The brief's `insertStylesheet:` is a typo: metadata and original calls require
`insertStylesheet:stylesheet:` (identifier, CSS string).

### PiP process policies requested by core

`renderer_pip_blink_proposal.patch` preserves exact native process-global flags
and three consumer locations. It passes apply-check and all three modified
translation units pass actual Base flags using temporary VFS overlays; no
Chromium files were changed. See `logs/wp/renderer-pip-proposal/REVIEW.md`.
Core keeps public selectors; renderer will supply common/IPC forwarding after
the policy hooks land. Shared declarations requested:

```cpp
namespace arc {
void SetRendererSkipUserActivationForPiP(content::WebContents*, bool);
void SetRendererIgnoreDisablePiPAttribute(content::WebContents*, bool);
}
```

Native defaults are skip=false, ignore=true. Ignore affects eligibility only,
not AttributeChanged exit. Video skip bypasses activation/token consumption;
document PiP consumes/checks first and only then includes the skip flag.
No per-tab storage or navigation replay should be added.


## TopBand incremental proposal

After exact ALL e07d1b39dd68beb59af325ab90446c760cc731911a2caf422d766ac20c0612bf
and gradient66179056ee5ae3a5c76f226e805ca86a8652279ea9f8fd15ac7ec799bf129e80,
review renderer_top_band_integration_proposal.patch, SHA256
88adc051e7c8f8ed6646396215482b20eb04042b7a8b77aab0eae3cca3eb37ab.
It contains29files (20Blink/shared,6owned transport,3lead hooks), and is an
alternative to its3disjoint components. All19actual-flags syntax checks pass;
25focused cases compile but have not run. Fresh independent composition and
native-contract recheck pass; withdrawn359must not be adopted.

This proposal remains incomplete: committed-navigation refresh is implemented
but unwired pending native observer ordering; Arc36 relay/live updates are
unproved and absent; private shared-handle ABI remains uncovered. Default base
feature is disabled. Do not call static compile/review evidence runtime parity.
Exact dependencies/hashes/checks/limitations: logs/wp/renderer-top-band-proposal/.

The first native4mode callback diagnostic preserved all four incomplete runs
under results/renderer-topband-native-run2: a background RAF wait timed out
before any full stage. Base/both produced one genuine red callback; default/Arc
were silent, not nil. A compiled/reviewed isolated retry removes that RAF wait,
records bounded host/capture timing, and adds a high-contrast gradient opportunity
for a real nullable callback. No foreground UI or engine flag workaround.
