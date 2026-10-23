# Original thumbnail pixels: native audit and host-only proposal

Proposal: `arc/browser/webextras_thumbnail_pixels.patch`. The lead owns applying
it to Chromium and the maintained patch set. Only this proposal and these new
notes were written. No Chromium/shared source, shared test, maintained patch,
application, build graph, lock, or existing notes were changed.

## Native evidence

All addresses below are unslid arm64 VAs in
`/tmp/arccore-webextras.arm64`. Recovery used bounded
`xcrun llvm-objdump --disassemble --no-show-raw-insn --start-address=...
--stop-address=...` plus direct reads of Mach-O chained pointers/selectors.

### Source rectangle and target scaling

The tab-switch capture equivalent starts at `0x731d500`:

- `0x731d57c–0x731d59c` obtains the view bounds and rejects an empty size.
- `0x731d5b8` obtains the device scale factor; `0x731d5d4–0x731d5d8` calls
  capture-info helper `0x731d680` with the source-size address and that scale.
- `0x731d648–0x731d658` passes the resulting copy rectangle at offset `0x18`
  and target size at offset `0x28` into the view's copy-from-surface call.

Capture-info helper `0x731d680` preserves the complete source rectangle:

- `0x731d6c0–0x731d6c4` stores source width/height unchanged.
- `0x731d6c8–0x731d6d0` uses `max(device_scale, 1.5f)`.
- `0x731d6d8` calls preview-size getter `0x7e086ac`, whose complete body returns
  `{256, 144}`. `0x731d6e0–0x731d6ec` combines that with constant `{175, 175}`
  (decoded at `0xc7faeec`) using componentwise maximum helper `0x4484400`.
  The resulting minimum is therefore `{256, 175}`, matching current Chromium.
  `0x731d6f8–0x731d710` computes the scaled minimum dimension used for deciding
  whether scrollbar insets fit.
- `0x731d768–0x731d798` computes rounded-up scaled scrollbar width and stores
  right/bottom insets when subtracting them would still exceed that minimum.
- Crucially, `0x731d79c–0x731d7a0` stores `{0, 0, source_width, source_height}`
  directly as `copy_rect`. There is no inset operation or include-scrollbars
  branch between that store and target calculation.
- `0x731d7b0–0x731d7f4` computes target sizing from the full source aspect:
  take the smaller width/height ratio to scaled minimum sizing; preserve the
  source size when the ratio is at most 1; otherwise scale by its reciprocal
  and round dimensions upward. The called scale helper `0x1952230` contains
  `frintp.2s` at `0x1952264`, confirming ceiling. Minimum target sizing uses
  helper `0x1f5e478`, which reaches `frintm.2s` at `0x4484f84`, confirming floor.

The video-start equivalent `0x731da44` uses the same capture-info helper at
`0x731dae4`, then passes that information to the background capturer at
`0x731db04`. Thus both paths use the full source rectangle.

### Background frame pixels

The native background capturer's vtable is at `0xd968408`, with Start
`0x731b8e4`, Stop `0x1af12e4`, and frame callback `0x41c5614`. The secondary
consumer thunk at `0x731bb1c` adjusts `this` and branches to `0x41c5614`.

- `0x731b990–0x731b99c` sets equal minimum/maximum resolution constraints
  from capture-info target size (`this + 0x48`) with fixed-aspect flag false.
- `0x41c5798–0x41c57a8` adds content rectangle origin and extent to construct
  bitmap dimensions `content_rect.right()` and `content_rect.bottom()`.
- `0x41c57ac–0x41c5844` builds the bitmap over the mapped ARGB frame with
  its coded row stride and a frame-pinning release callback.
- `0x41c5850–0x41c5854` marks the bitmap immutable.
- `0x41c5858–0x41c5864` sends that entire bitmap to the frame callback.
  The complete bounded callback `0x41c5614–0x41c59a8` contains no scrollbar
  subtraction or `extractSubset` step. It does not crop the content origin.

This differs from current Chromium, which subtracts scaled scrollbar insets
and publishes an extracted subset of the compositor bitmap.

### NSImage conversion

The original observer bridge at `0x441cccc` calls `0x2a02ccc` at `0x441cd10`
to obtain an NSImage from its gfx image. The ImageSkia conversion reaches
`0x2a02ef4`:

- `0x2a02f60–0x2a02f80` converts bitmap representations and invokes
  `addRepresentation:` (selector reference `0xe28f2f0`).
- `0x2a02fa0–0x2a02fb8` reads ImageSkia logical width/height at storage offset
  `0x30`, converts them to doubles, and invokes `setSize:` (selector reference
  `0xe28f2c8`).
- The bridge then invokes `webContents:thumbnailReady:` at `0x441cd34`
  (selector reference `0xe291018`).

These selectors were decoded directly from the Mach-O selector-reference
pointers. There is no added 15-pixel margin or compensating resize here.
Current `gfx::NSImageFromImageSkia` performs the same representation conversion
and logical-size assignment. The package's wrapper conversion should stay as
it is.

## Proposed host-only behavior

The established predicate is
`base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")`, already
used by maintained `patches/0004-arcmod-hooks.patch` and current
`read_anything_side_panel_controller.cc:250`. No new switch or platform-wide
behavior is introduced.

- In `ThumbnailTabHelper::GetInitialCaptureInfo`, skip copy-rectangle scrollbar
  insetting in Arc host mode. Target sizing consequently uses the full source,
  matching the original routine for tab-switch capture. Outside host mode,
  the existing include-scrollbars argument retains its previous behavior.
- In `BackgroundThumbnailVideoCapturer::OnFrameCaptured`, publish the complete
  immutable bitmap in Arc host mode and return before extracting a subset.
  This preserves its pixels, original row stride, mapping/pinner lifetime,
  and any compositor-origin area. Outside host mode, retain the existing
  scrollbar/content-rectangle crop.

For an input bitmap of 512x352 with 15-pixel right/bottom insets at scale ratio
1, current Chromium's crop produces exactly 497x337. The original full-bitmap
path produces 512x352 from that same input. This establishes the mechanism and
the native behavior to restore. It does **not** establish that the original
and replacement runtime view bounds or scale factors were identical; a new
comparison must check actual results after application. No fixed dimensions,
15-pixel padding, or wrapper resizing are proposed.

## Regression coverage and validation

The proposal adds `ThumbnailPixelCaptureTest` to the existing thumbnails
`unit_tests` source set. Two private fixture friendships expose existing
capture setup only to this fixture; no production getter/testing API is added.

The test invokes the real `OnFrameCaptured` consumer with mapped ARGB memory
and a live but unserviced capture endpoint, without renderer/GPU/window work.
It checks exact dimensions and colored edge pixels for:

1. Full host-mode tab-switch rectangle and matching full-source target size.
2. Existing non-host tab-switch insetting.
3. Host-mode preservation of right/bottom compositor pixels.
4. Host-mode preservation when the compositor content has a nonzero origin.
5. Existing non-host 512x352-to-497x337 clipping and remaining pixel color.

The original cropped implementation fails the host pixel/dimension checks;
resizing a cropped bitmap would still lose the red edge pixels and fail.

- `git -C chromium/src apply --check ../../arc/browser/webextras_thumbnail_pixels.patch`
  passes against current source.
- The proposed tab helper and background capturer pass `-fsyntax-only` with
  their real `out/Base` compiler commands, including warning/error/plugins:
  exit 0, no diagnostics.
- The new test passes the same syntax check with the existing thumbnails
  `unit_tests` compile flags, changing only its source input to the temporary
  new test file: exit 0, no diagnostics. VFS overlays provide the proposed
  source/header changes; object/dependency output flags are removed.
- Tests were not executed and the new GN source was not regenerated or linked.
  The lead should run `ThumbnailPixelCaptureTest.*`, then repeat the package's
  media/cookies comparison and inspect callback dimensions/pixels. Runtime
  geometry and remaining scaling differences remain unverified until that run.
