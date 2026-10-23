# Native filter and top-band integration gaps

Read-only audit of the pristine ArcCore 1.170.0 arm64 image, 2026-10-23.
These paths are uncovered by the existing accepted golden transcripts. They
must not be replaced by guessed CSS filters or root/background-color reads.

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
paths using DarkModeFlags and the native rectangle path are proven; image-
specific inclusion/exclusion remains unestablished.

Profile `registerColorFilter` backend `0x42e25ec` stores the optional matrix
at host configuration +48/presence +72 and posts an update. Document-start
application `0x43a9290` forwards it to the same renderer endpoint.

Request to lead: add serializable cc::ColorFilter HSLA and composition
variants; Page filter setter/getter with local-frame paint invalidation;
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
uses chosen-state equality suppression, optional color, frame token and
crossfade state. A 100ms timer appears at `0x4684a40`. Mojo serializer
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
- Cache by frame token and suppress equal optional/color results
  (`0x40c0124`). Scroll begin/end input types 11/12 affect capture deferral
  (`0x40c0ce4`); this is not simply a periodic screenshot.

Current `RenderWidgetHostView::CopyFromSurface` can provide fallback pixels.
It cannot provide the custom tracker's selected-color path, chosen-state
transitions, frame-token gating or crossfade decisions.

Request to lead: recover/port the Blink tracker and expose its browser
notification hook. A screenshot-only producer cannot establish full parity.
Core owns the weak delegate slot; renderer must not duplicate its setter.

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
