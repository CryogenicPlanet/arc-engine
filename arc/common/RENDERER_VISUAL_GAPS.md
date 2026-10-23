# Native filter and top-band integration gaps

CURRENT RELEASE: corrected UNAPPLIED TopBandd106f59e replaces withdrawn88.
Dependency order ALL e07 → gradient661 → TopBandd106.19actual-flags checks0/empty;
38fixtures compiled NOT executed; native-contract and canonical replay reviews pass.
Native geometry and active-pending/no-area findings are closed. Historical88 is
archived; dependent723 remains HELD while privately rebasing. Count-only discard21a
remains independently HELD for core global phases. Production is unchanged.
Public TopBand flag is31 (36 is Snapshot); startup relay is proved but not integrated.
Native live propagation, commit refresh activation and private ABI remain open.


## Current handoff, 2026-10-23

Production remains unchanged since the checked exclusion/compiler-sequence fixes.
Published unapplied order is ALL `e07d1b39` → gradient `66179056` → TopBand
`d106f59e`. TopBand canonical manifest `6a5aefce` pins19real-flags checks and
38compile-only fixtures, canonical29-file replay and fresh native-contract review.
Geometry now admits edge/fullbleed directly, while strict interior needs coalescing
and all three quarter points. Active timer/pending/no-area entersmode3+empty;
missingView staysquiet. Earlier88 is withdrawn in held-88/.

Native public flag31 is TopBand;36 is Snapshot. A reviewed native startup hook reads
31 and adds enable-blink-features=TopBandColorSampling for renderer children. That
startup integration is privately preparing; live propagation remains unproved.
Commit refresh remains unwired. Native attachment proof establishes refresh before
queued ObjC commit on ordinary NONNESTED UI dispatch; unconditional nested order
is unproved. Shared adoption/GN/link/runtime and private C++ABI remain pending.

Count-only discard `21a83075` is HELD: SAME Impl/backend count survives replacement,
old count read before teardown, seeded before new publication, wrapper KVO sync last.
Core must implement canonical ownership/global retirement→attachment phases.
Full-stack current weak TopBand-holder transfer is privately rebasing onto d106;
no old document sampler/cache/capture migration, no setter replay/activation.

Native retry1:4×14stages, integrity/restoration/natural quit pass; callbacks0/9/0/9
(default/base/index36/both), all18non-nil/main/crossfadefalse. Callbacks precede
hostobservation by21–31ms and screenshots by1.53s; not a final-painted-color oracle.
Those index36 controls tested Snapshot, not TopBand relay. Parent inspected actual
interior/splitgradient/fixedblue screenshots. Next private probe covers31/new renderer,
same-document geometry and genuine nullable/crossfade opportunities; not queued.

Peer UIrun4 fresh-profile37/37steps/natural quit pass.10strict fixture CSScalls each
match both preservedv1 and current attributednative. Current-native vs historical
UI global57 is a diagnostic, old84 remains pinned tooldv1. Twoextra theme calls are
app-origin invocations; upstream scheduling unproved, no renderer hack. Official
results/ours/renderer-public-api remains absent. Acceptance is in progress.

## HSLA filters

Direct apply/remove use browser backends `0x4379218`/`0x43793ac`, proxy
`0x442a24c`, renderer endpoint `0x43aa048`. An engaged optional contains
exactly twenty floats; absent clears the native filter.

The renderer builds `SkColorFilters::HSLAMatrix` (`0x2c4a754`, flags 1,1),
then composes an outer HSLA filter with an inner TableARGB filter. Alpha's
256-byte table is identity. Each RGB table maps 0..254 identically and
255 to 254 (`0x43aa11c`..`0x43aa17c`). Composition is `0xfa1548`.
The native setter `0x465bb00` replaces retained `cc::ColorFilter` at Page
+1320 and invalidates descendant painting through `0xa8ca1f0`. Both renderer
filter and scheme endpoints resolve exactly the same peer (frame -> virtual+56
-> peer+56); independent scheme-consumer evidence proves that peer is Page.

Source comparison: `third_party/skia/include/core/SkColorFilter.h` and
`src/effects/colorfilters/SkMatrixColorFilter.cpp` provide the HSLA recipe.
Upstream `cc/paint/color_filter.{h,cc}` lacks HSLA/composition serialization;
its RGBA MakeMatrix is a different operation. Blink's public WebView and
WebLocalFrame interfaces lack the native filter setter.

The paint consumer is now recovered: `LocalFrameView::PaintTree` (`0x1282dec`)
constructs GraphicsContext, resolves frame->Page, reads Page+1320 at `0x1283208`,
and sets the context's retained filter at `0x128321c` -> `0x1283ae0` (context
+216), before frame/overlay painting. `GraphicsContext::DarkModeFlags`
(`0xa49078`) checks that filter first, copies input PaintFlags, composes outer
context filter with inner existing flags filter (`0xa49154`), then returns the
copy without auto-dark processing. Upstream `graphics_context.cc:190` is the
corresponding constructor and exact insertion point. A second native inlined
instance exists in rectangular color drawing at `0xfa1040`..`0xfa11d8`.

This is paint-flags filtering, not a root compositor layer effect. Drawing
paths using DarkModeFlags and the native rectangle path are proven. Bounded native
fixtures leave ordinary canvas/PNG/SVG images unchanged while filtering generated
linear/radial/conic gradients; removal restores all9swatch histograms exactly.
ImageDrawOptions/Gradient fallback/cache propagation is source-mapped in
renderer_filter_image_evidence.md. Arbitrary image-path parity remains uncovered.

Profile `registerColorFilter` backend `0x42e25ec` stores the optional matrix
at host configuration +48/presence +72 and posts an update. Document-start
application `0x43a9290` forwards it to the same renderer endpoint.

Request to lead: adopt the concrete unapplied renderer ALL and incremental
gradient proposals (current exact dependency hashes in RENDERER_INTEGRATION.md).
They add serializable cc::ColorFilter HSLA and composition variants; Page filter setter/getter with local-frame paint invalidation;
WebView forwarding setter; retained GraphicsContext filter; initialize it
from Page immediately after constructing the context in PaintTree; prepend
the native copy/compose/return branch to DarkModeFlags. Worker ownership
excludes Chromium and patches. Do not substitute RGBA, CSS/SVG or a root
compositor layer filter. Null clears the Page filter.

## Top-band color

Exact delegate callback:

```objc
- (void)webContents:(ArcWebContents*)contents
    didSampleTopBandColor:(ArcColor*)nullableColor
         shouldCrossfade:(BOOL)crossfade;
```

`setTopBandColorDelegate:` (`0x44417ac`) creates a weak wrapper/delegate
bridge. Native registration `0x4345e80` -> `0x4376364` stores a weak native
reference at +632/+640. Callback `0x4445afc` converts an optional uint32
to ArcColor; absence is nil, not transparent black.

The producer is a custom Blink `arc::TopBandColorTracker`, proven by literals
at `0xd5d31fa` and `0xd5d323b`:
`../../arc/blink/core/top_band_color_tracker.cc` and
`EmitChosenState(blink::LocalFrameView*, ChosenState)`. Emitter `0x46874c0`
uses chosen-state equality suppression, optional color, an opaque sampling/cache
token and crossfade state. This token is not a proven Blink LocalFrameToken. A 100ms timer appears at `0x4684a40`. Mojo serializer
`0x418c740` sends message `0x597900bf`; browser dispatch `0x1e3ad9c` ->
`0x4150da4` -> `0x40c0094` reaches the per-main-frame sampler.

The browser fallback is separately recovered:

- Capture `(0, 0, view_width, 8 DIP)`, empty output size, via surface copy
  (`0x40c0230`..`0x40c03a4`). Use display device scale factor.
- Sample y=0 and y=`min(max(1, round(8*scale)), bitmap.height)-1`.
- Five x positions are clamped integer values of
  `(i/4.0f)*max(bitmap.width-round(24*scale),0)`, i=0..4.
- Compare normalized RGB Euclidean distances across top-row pairs and the
  corresponding second-row endpoints. Maximum >0.5 yields no color;
  zero yields the first top-row color; otherwise round the per-channel
  mean of the five top-row pixels, including alpha (`0x40c05c0`).
- Cache by sampling token and suppress equal optional/color results
  (`0x40c0124`). Scroll begin/end input types 11/12 affect capture deferral
  (`0x40c0ce4`); this is not simply a periodic screenshot.

Current `RenderWidgetHostView::CopyFromSurface` can provide fallback pixels.
It cannot provide the custom tracker's selected-color path, chosen-state
transitions, sampling-token gating or crossfade decisions.

The complete native producer/browser contract has now been recovered in
renderer_contract_evidence.md:1038 onward, including later refinements. A separate
29-file incremental port is staged under logs/wp/renderer-top-band-proposal,
with19successful actual-flags syntax checks and compile-only fixtures; final
composition review is finishing. It does not establish runtime parity.
Core owns the weak delegate slot and proposed selector forwarding. Committed-
navigation refresh remains unwired pending observer-order proof, and Arc36 relay/
live updates plus the private C++ shared-handle ABI remain uncovered.

## Color scheme

Public enum 0 is Light (native 1); public 1 is Dark (native 0). Browser
registration `0x42e324c` stores an engaged optional at host config +80/+84.
Direct apply `0x4379534` and remove `0x4379638` send the same optional to
renderer `0x43aa828`, which writes Page +1328/+1332 and invalidates local
documents via `0xa8ca278` -> `Document::ColorSchemeChanged` (`0xac2d840`).
Profile document-start `0x43a9290` uses the same Page fields; absent host
configuration clears the optional.

`StyleEngine::UpdateColorScheme` (`0xfaab90`) evaluates base Settings first,
then media overrides, then preference overrides. At `0xfaad70` it reads the
Page optional and, if engaged, overwrites `preferred_color_scheme_`; printing
forces Light afterward. Upstream `core/css/style_engine.cc:4461-4530` has
the lower layers and printing but lacks this optional.

Request to lead: expose a Page-scoped optional setter through WebView,
invalidate each local document, and consume the optional after preference
overrides and before printing. `WebSettings::SetPreferredColorScheme` is
not equivalent because it changes the lowest layer. Removal disengages the
optional and reevaluates the live lower layers; it must not restore a saved
base preference. No production approximation was added.


### Top-band producer audit update

New bounded native evidence is in `renderer_contract_evidence.md:1033` onward
and `logs/wp/renderer-topband-producer.native.log`. It supersedes the early
frame-token interpretation above. LocalFrame owns the tracker only for main
frames; post-lifecycle steps perform detection and a100ms timer schedules
animation. LCP updates mark readiness/crossfade state and clear candidate caches.
Parsing-finished and first-paint may arrive in either order; both feed the custom
readiness transition. The emitter compares full Blink Color/chosen state while
the browser independently suppresses equal optional ARGB32 values.

The producer performs layout hit-testing and CSS-background candidate analysis,
including actual image-pixel analysis through the recovered sampler. Canvas and
video candidates are explicitly excluded; IMG has a separate path. A CSS class
`ytd-masthead` candidate is skipped only on `youtube.com` and `.youtube.com`
hosts. It is not a tag-name rule. Root background voting is separate from
element admission: style/size-qualified edge/fullbleed elements admit directly;
strict interiors require enabled coalescing and all three quarter points.
Geometry uses LayoutView.GetScrollableArea.VisibleContentRect(kExcludeScrollbars)
integer width. Coordinate policy is selected once per pass from the first eligible
list hit: fixed/nonzero-sticky uses raw points, otherwise DocumentToFrame. The
full hit traversal, background/image admission,1.5s cache and state transitions
are now source-mapped; current evidence supersedes the earlier unknown-getter note.

Remaining gaps are integration/coverage: committed-navigation refresh order,
public Arc36 relay/live updates, private shared-handle ABI and runtime validation
after actual adoption. The separate native default/base/Arc/both public callback
diagnostic is queued, waiter52933/tool64062; silence is not treated as nil/parity.
No production TopBand producer is active from this worker's unapplied proposal.
