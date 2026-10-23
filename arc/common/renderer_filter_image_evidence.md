# Native Arc color-filter extent: image paths

This is a bounded native-code and fixture audit. It does not claim proposed-engine runtime equivalence. Addresses below are unslid arm64 addresses in the pristine original `../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`, decoded using `renderer_contract_evidence.py`. Disassembly logs are under `logs/wp/renderer-filter-image-audit/`. No production or Chromium source was changed by this audit.

## Native fixture observation

The genuine public `ArcHSLAMatrix` initializer and `ArcWebContents applyColorFilter:` were exercised with identity and saturation-zero matrices, followed by removal. Artifacts are `results/renderer-idle-diagnostic-run2/trace156/filter-{baseline,identity,grayscale,removed}.png`, `filter-observations.json`, and parent-generated `filter-pixels.json`. All four screenshots were visually inspected. Their geometry is stable: viewport 1042×756 CSS pixels, DPR 2; PNG 2084×1512. Each 48×48 CSS swatch is measured with a two-CSS-pixel inset, giving 7,744 pixels. Coordinates are recorded in the observation artifact.

| Swatch | Baseline RGBA | Identity RGBA | Saturation zero RGBA | Removed |
| --- | --- | --- | --- | --- |
| CSS red | 234,51,35,255 | 233,51,35,255 | 127,127,127,255 | baseline |
| CSS white | 255,255,255,255 | 254,254,254,255 | 254,254,254,255 | baseline |
| Canvas red | 234,51,35,255 | unchanged | unchanged | baseline |
| Canvas-produced raster PNG red | 234,51,35,255 | unchanged | unchanged | baseline |
| SVG image red | 234,51,35,255 | unchanged | unchanged | baseline |

Every entry above describes all 7,744 measured pixels. The red text's solid interior has 477 pixels: baseline 234,51,35; identity 233,51,35; grayscale 127,127,127. Baseline has 778 non-gray pixels including antialiasing; grayscale has none. All measured alpha values are 255. Removal restores the entire measured histogram of every swatch to baseline. These are screenshot values, not assumed source-color arithmetic. This fixture contains no generated gradient, tiled/pattern image, rounded image, filter combination, video, or cross-frame case.

## Proven native producers

The existing Page/context proof establishes the retained Arc `cc::ColorFilter` at GraphicsContext+216. These image methods copy that pointer into a retained field at ImageDrawOptions+8; they do not simply install it on every image's PaintFlags.

| Native method | Options address and retained copy | Dispatch |
| --- | --- | --- |
| GraphicsContext::DrawImage, `0xb64900` | options=sp+40; context+216 loaded/retained/stored to sp+48 at `0xb64a6c–0xb64a90` | Image::Draw vtable+256 at `0xb64ab4`, x5=sp+40 |
| GraphicsContext::DrawImageRRect, `0x928080` | options=sp+160; context+216 loaded/retained/stored to sp+168 at `0x928398–0x9283c0` | ApplyShader vtable+264 at `0x9284d0`; fallback Draw vtable+256 at `0x928588`, both receive these options |
| GraphicsContext::DrawImageTiled, `0xfa0ce0` | options=sp+8; context+216 loaded/retained/stored to sp+16 at `0xfa0ddc–0xfa0e00` | DrawPattern vtable+288 at `0xfa0e24`, x5=sp+8 |

The reference increments and corresponding decrements establish ownership; this is a retained wrapper, not an unowned SkColorFilter pointer. `0x928200` is a tail branch from the rounded method to DrawImage, **not** the rounded method entry. Current Chromium counterparts are `graphics_context.cc:519`, `:544`, and `:640`; `ImageDrawOptions` is defined in `graphics_context.h:93`.

## Consumers which exclude the options filter

These identifications use vtable slots, predicates and actual call arguments as well as trace-string references; they do not infer names from nearby strings.

- BitmapImage vtable `0xdc35d88`, Draw slot at `0xdc35e88` → `0xa328f0`. The actual `BitmapImage::draw` trace literal is referenced within this function and control flow rejoins its draw body. Its complete body through `0xa33048` reads options+0, +16, +40, +44, +48 and +56, but not +8. Canvas drawImageRect at `0xa32d38–0xa32d54` forwards the incoming flags. The separate auto-dark branch is not the Arc options filter. See `native-bitmap-draw-full.log`.
- Raw SVGImage vtable `0xdbeca10`, Draw slot → `0xa9ac258`. Its draw entry reads options+52 (auto dark), builds SVG DrawInfo and calls DrawInternal via `0xc492ab8`; it does not read options+8. See `native-svg-draw-entry.log`.
- SVGImageForContainer vtable `0xdbed060`, Draw → `0x1172c18`: reads options+52, constructs DrawInfo from container size/zoom/view data, calls DrawForContainer `0x1172ce8` at `0x1172cc8`, then DrawInternal `0x1172d50` at `0x1172d40`. Its options filter is not propagated. Pattern entry `0xa9acad4` likewise reads only auto-dark state before SVG pattern rendering. Counterpart: `svg_image_for_container.cc:110`.
- Accelerated StaticBitmapImage vtable `0xdc35a70`, Draw → `0xaaf8d40`, calls shared helper `0xab28fe0` at `0xaaf8e84`. Unaccelerated vtable `0xdc37820`, Draw → `0xab2d628`, calls that helper at `0xab2d684`. The full helper through `0xab292a4` reads options+16/+40/+44, never +8, and forwards original flags to canvas drawImageRect at `0xab291f8–0xab29214`. Counterpart: `static_bitmap_image.cc:84`.

This explains an actual image-filter exclusion mechanism consistent with the exact PNG/SVG/canvas fixture. It does not prove every possible canvas producer, SVG embedding, or image path bypasses filtering. The native canvas-element producer/foreign-layer route itself was not fully recovered here.

## Pattern images are a different path

Generic Image::DrawPattern is `0xab191c0` (BitmapImage, raw SVGImage and StaticBitmapImage vtables share this slot). It calls native GraphicsContext::DrawRect `0x3771d68` at `0xab1964c`. DrawRect calls DarkModeFlags `0xa49078` at `0x3771d94` before raw canvas drawRect. That helper applies the context filter even when auto dark is disabled. Therefore ordinary direct-image exclusions cannot be generalized to tiled/pattern draws. Current `image.cc:329` similarly uses `context.DrawRect(..., AutoDarkMode::Disabled())`.

GeneratedImage::DrawPattern is `0x469d2ac`. It copies options+0/+8 at `0x469d35c`, retains the copied filter and stores options+8 at `0x469d374`, calls CreateShader vtable+304, and applies DarkModeFlags at `0x469d464` before its outer canvas drawRect. Current `generated_image.cc:51` copies ImageDrawOptions, creates the inner shader, then uses destination-context DrawRect. Adding a retained field to ImageDrawOptions preserves the copy transport naturally. There is no native pattern-pixel fixture in this audit; do not claim how many effective filtering passes a particular pattern produces from the fixture above.

## Concrete uncovered generated-gradient mechanism

GradientGeneratedImage vtable `0xdc36f20` has Draw `0x3d246d8`, ApplyShader `0x24709a8`, and generated-pattern implementation `0x469d2ac`.

- Draw copies input PaintFlags at `0x3d247ac`, passes the gradient object (image+80), flags, transform and original options to `Gradient::ApplyToFlags` (`0x2470a00`) at `0x3d247c0`, then invokes raw canvas drawRect at `0x3d247d8`. There is no intervening GraphicsContext DarkModeFlags call.
- ApplyShader forwards options (x4→x3), gradient and flags to the same helper at `0x24709b8`.
- DrawTile entry `0x3d24814` builds fresh flags, calls the same helper at `0x3d24868`, and draws directly to the recording canvas. The generated-pattern options copy therefore also feeds this inner path.

Native Gradient::ApplyToFlags precisely implements **incoming PaintFlags color filter if present, otherwise ImageDrawOptions+8**:

```text
0x2470a88 mov  x24,x19             ; flags
0x2470a8c ldr  x8,[x24,#48]!      ; flags filter; x24 now its address
0x2470a90 add  x9,x20,#8          ; options filter address
0x2470a94 cmp  x8,#0
0x2470a98 csel x8,x9,x24,eq
0x2470a9c ldr  x20,[x8]           ; selected retained filter
```

It retains the selected pointer. The existing cached shader is Gradient+8. After the cache/matrix checks (`0x2470ab4–0x2470af4`), selected filter is compared to Gradient+16 at `0x2470af8–0x2470b00`. Equal cache, matrix and pointer branch directly to `0x2470e14`, skipping rebuild and flag clearing. **Only inside the rebuild branch**, `0x2470b14–0x2470b18` replaces Gradient+16, and `0x2470b40–0x2470b44` clears PaintFlags+48. Shader assignment and dither occur after this branch. Preserve this placement; do not introduce unconditional clearing.

The chosen filter actually affects gradient stops: `0x2470ed8` loads Gradient+16; non-null `0x2470edc` branches to `0x247149c`; the converted stop RGBA is passed with that filter to `0x8b2815c` at `0x24714c8`. That wrapper loads its underlying SkColorFilter at +16 and calls `0xc47f9e4`, with null color spaces; its null-underlying branch returns incoming RGBA. Current `gradient.cc:119`, `:146`, and `:193` already call `ColorFilter::FilterColor` when collecting stops.

Current Chromium `Gradient::ApplyToFlags` at `gradient.cc:347` has the same cache structure but uses only `flags.getColorFilter()` in its comparison/assignment. The reviewed effects Blink proposal and unchanged ALL candidate add no options filter field/fallback, so their direct generated-gradient path is uncovered even though ordinary image exclusions reconcile with the fixture.

The grounded incremental proposal is limited to:

1. Add default-null `sk_sp<cc::ColorFilter>` to ImageDrawOptions (`graphics_context.h`). Native field order need not be copied because this is internal C++ source, not an ABI bridge.
2. Populate it from the context Arc filter in the three GraphicsContext producers above.
3. In Gradient::ApplyToFlags, select incoming flags filter when non-null, otherwise the options filter; compare/assign the selected pointer within the existing cache logic, retaining the current rebuild-only flags clear.

No blanket bitmap/SVG/canvas filter should be added. This proposal still requires real compiler checks and runtime gradient/image comparisons; native static recovery is evidence for the mechanism, not executed-engine acceptance.

## Reproduction and limits

Bounded ranges were decoded with `PYTHONDONTWRITEBYTECODE=1 python3 arc/common/renderer_contract_evidence.py ADDRESS:SIZE ...`. Relevant logs include `native-image-call-candidates.log`, `native-generated-gradient-candidate.log`, `native-gradient-apply-flags-prefix.log`, `native-tiled-options-gradient-filter.log`, `native-pattern-drawrect-chain.log`, and `native-static-bitmap-draw-helper.log`. Direct branch islands were followed to real callees. Compact-unwind regions and generic vtable-call search hits were not treated as proof of function identity.

No app launch, build, lock, or production-source mutation was performed by this audit. The native fixture was run by the parent. The candidate source review is static; proposed implementation remains outside runtime acceptance until applied, built and compared by its authorized owner.

## Follow-up native gradient fixture

The parent's later native fixture is available under `results/renderer-long-readiness-run2/trace156/`, with the same four filter PNG filenames, `filter-observations.json`, and parent-generated `filter-pixels.json`. All four actual screenshots were inspected. In addition to the earlier controls, it contains opaque red linear, radial and conic CSS gradients at x=298, 354 and 410 CSS pixels respectively, y=18, each 48×48 CSS pixels. DPR remains 2, viewport 1042×756, and each measured inset contains 7,744 pixels. Geometry is identical across all four states.

All three gradient swatches have exactly the same measured histograms:

| State | Measured RGBA → pixel count |
| --- | --- |
| Baseline | (234,51,35,255) → 5,203; (234,51,36,255) → 1,936; (234,52,36,255) → 605 |
| Identity | (233,51,35,255) → 5,324; (233,50,35,255) → 1,331; (234,51,36,255) → 605; (233,51,36,255) → 484 |
| Saturation zero | (127,127,127,255) → 7,502; (128,127,127,255) → 121; (128,128,127,255) → 121 |
| Removed | Entire histogram exactly equals baseline |

Visually, all three gradients become gray while canvas, PNG and SVG remain red. The 242 gradient pixels with unequal channels after saturation zero differ by at most one channel value; this is consistent with the native gradient dither already proven at the end of ApplyToFlags. The exact histogram is reported instead of asserting every gradient pixel is neutral gray. All alpha values remain 255. The ordinary canvas/PNG/SVG controls remain exactly (234,51,35,255) at all 7,744 measured pixels through identity and grayscale. CSS red and white and text behave as in the earlier fixture. Independently comparing the JSON confirms **all nine removed-state swatch histograms exactly equal their baseline histograms**.

This provides native runtime confirmation that these three constant-color generated-gradient fixtures are affected, complementing the recovered ImageDrawOptions/Gradient mechanism. It does not establish multicolor interpolation behavior, rounded/tiled/pattern behavior, incoming-filter precedence at runtime, or proposed-engine parity. The proposed engine had not started for this comparison, and no ours gradient pixels were inspected. The follow-up incremental proposal's exact source review found no additional blocker: default-null retained options field, all three producer assignments, flags-first fallback, effective-pointer cache comparison/assignment and rebuild-only flags clearing match the native mechanism. That proposal's eight actual-flag syntax checks passed; its four focused test fixtures were compiled but not linked or executed.
