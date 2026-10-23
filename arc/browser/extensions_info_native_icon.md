# Deferred coordinated native action-icon follow-up

## V2 validation7 current refresh

Current unapplied patch SHA256: `2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`. The proposal
was refreshed against the exact validation7 content snapshot below. Existing
proofs are preserved; earlier sections/tables describe their historical bases.
Only hunk coordinates/context were refreshed: changed semantic lines match the
prior reviewed proposal. No actual product/shared/helper/runtime content was changed by this refresh.

| Current source | Actual SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/extensions/browser/extension_action.cc` | `cb6943b27b7ce823e5746cf4948d4c684c952ceebf6aa2b2afa124813975c5eb` | `bc80d6285ba1ef4359d957876c202d31b5d3404bd1a655de30fd63cedc842782` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `3cfccf64a77926987287cb2091a292acb566785a088a03723565a4212599747c` |
| `chromium/src/extensions/browser/icon_util.cc` | `55e084a3ced904b67980002807f76eb03cd20660f77468bd1862c543b708e2ac` | `e81743acb5e6a1243f0e0c9202c92dda1c2927c76dd04470a602beda8d6fc20e` |

Current evidence: `logs/wp/extensions-info-native-icon-proposal/v2-current-refresh`.
The previous patch and document were preserved there before refresh. Shared
native original and proposed bytes match their prior strict compiler/negative
overlay evidence exactly; `shared-evidence-reuse.json` records those checks.
Shared native compilation was reused, not rerun or claimed under new flags.

Both refreshed proposals apply to copied current sources in either order:
pin then native-icon, or native-icon then pin. All five resulting source files
are byte-identical across orders. Combined context SHA256:
`73ba0b1d7bd9d78e7a60963570e01d6214d2c9c3a6cc3dd23def3418fa05218f`.
`composition.json` retains exact application commands, trees and all hashes.
This proves source composition only, not runtime behavior or native prerequisites.

Fresh exact-command checks cover pin context, pin browser, native-icon
context and the combined context. Every source keeps its own actual TU path
and warning/error/plugin flags through the symlink-correct VFS; only output
options are removed. Compiler commands/results and independent per-copy negative
sentinels are retained. All four compiler exits were zero with empty diagnostics;
all four negative overlays produced their unique expected error and exit1.

| Refreshed owned TU/copy | Syntax / diagnostics | Negative overlay |
|---|---|---|
| Pin context | 0 / empty | `ARC_PIN_V4_REFRESH_NEGATIVE_CONTEXT` detected |
| Pin browser | 0 / empty | `ARC_PIN_V4_REFRESH_NEGATIVE_BROWSER` detected |
| Native-icon context | 0 / empty | `ARC_ICON_V2_REFRESH_NEGATIVE_CONTEXT` detected |
| Combined context | 0 / empty | `ARC_COMPOSED_V7_NEGATIVE_CONTEXT` detected |

Fresh read-only review approved hashes, application/composition, exact compiler
evidence and updated documentation with no remaining blockers. All ten recorded
actual source/helper/runtime files retained their content hashes and sizes. The
final audit records mtime-only changes to the context and browser TUs, consistent
with the parent's announced validation7 force-compile touch. These are recorded
separately and are not reported as content mutations.

Content hashes and sizes, rather than mtimes alone, establish snapshot
stability here. The parent may deliberately touch owned TUs during validation7
to force a compile; `freeze-check.json` records before/after mtimes separately
and does not misreport an expected touch as a content mutation. These compiler
checks never build, package, run apps, take the shared lock or post to the board.

The shared Chromium hunks remain prerequisites: fold them into maintained
patches and apply them before the owned companions, or integrate them together.
Either copied application order above is a review route, not permission to run
an owned companion against unchanged native debug/size behavior. Runtime
callbacks, persistence, pixels and asynchronous loading remain unexecuted for
these proposals. Canonical scenario acceptance is unchanged.

**Unapplied proposal only.** `extensions_info_native_icon.patch` contains two
shared Chromium companions plus one owned context companion. The lead must
first fold the shared hunks into a maintained `patches/*.patch` and land/apply
them, then apply the owned companion, or integrate all three together. Do not
land the owned default-image change alone: current Chromium otherwise exposes
16px action images instead of the observed native 32px shape. A fresh process is
required; this is not a migration of already-created action image caches.

The base is the current actual context after the parent's validation6 corrections
(`475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d`).
No actual product/shared/helper/runtime sources or artifacts are changed here.
The pin-semantics proposal is refreshed separately against the same snapshot;
both copied application orders and their combined context are verified above.

## Concrete change

- `extensions/browser/extension_action.cc`: `ActionIconSize()` returns native32
  (`EXTENSION_ICON_SMALL`) only when command-line state is initialized and the
  exact `arcmod-host-app` switch is present. Existing ordinary Chromium desktop16
  and Android branches remain unchanged outside that mode, including calls made
  before command-line initialization.
- `extensions/browser/icon_util.cc`: the canvas icon parser obtains its DIP size
  from `ExtensionAction::ActionIconSize()` once per parse. Its scale and existing
  10-times bitmap-width limit use the same size. This preserves the explicit
  `LINT.IfChange/ThenChange` size invariant shared by the parser and action
  defaults/placeholders. Ordinary Chromium retains its original values; hosted
  parsing uses 32 and its corresponding 320px maximum. The parser's native branch
  itself has not been independently disassembled or runtime-compared; this hunk
  is the required consistency companion to the shared action size correction.
- `arc/api/arc_extensions_context.mm`: snapshots copy the real manager action's
  `GetDefaultIconImage()` into `arc::Image`. Remove the custom profile userdata
  cache/key/helper and its unused map, userdata, IconImage and IconsInfo headers;
  include the actual ExtensionAction declaration. Existing real-manager
  availability, false/null result when absent, and separate enabled query stay
  unchanged. No image resampling, fabricated pixels, explicit load forcing,
  callback substitution, persistent borrowed pointer or new cache is added.

The shared size method also serves other action-icon consumers in hosted mode;
its scope is explicitly action size throughout that mode, not snapshot-only
resizing. It does not change non-hosted Chromium behavior. There is no GN or
new dependency change: the existing browser target already links the action
implementation and base command-line service.

## Exact native evidence and source semantics

Evidence remains in `logs/wp/extensions-info-action-proposal/native`, pinned to
stock SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.
Those bounded arm64 fragments preserve original addresses and instruction bytes;
no native code was executed to extract them.

| Native address | Consequential evidence |
|---|---|
| `0x433513c`, especially `0x43352a0–0x4335328` | Info producer obtains actual manager/action; null builds an empty gfx image, otherwise calls `0x6ae40ec` before Arc image conversion |
| `0x43353b0–0x43353dc` | `hasAction` is action-pointer presence, separately stored from enabled state |
| `0x6ae40ec` | Existing default-icon-image branch, otherwise cached/generated placeholder; this matches `GetDefaultIconImage()` |
| `0x258fa84`, especially `0x258fb78–0x258fbc0` | Cache-first action lookup/creation creates its IconImage using the action default icon and native size helper |
| `0x4e12a78–0x4e12a7c` | Size helper is exactly `mov w0,#0x20; ret`, literal 32 |

Current Chromium `ExtensionAction::Populate` gives action-specific default icons
priority, then product icons. Its manager owns each action and optional
`IconImage`; unloading erases the cached action. `GetDefaultIconImage()` returns
the existing manager image, or the action's cached first-letter placeholder
when no icon exists. Image-loader failure fallback still follows native
Chromium's IconImage/action fallback path. The snapshot holds a gfx image copy,
with image loading/storage owned by the action/image implementation rather than
an independent profile-keyed product-icon cache.

The removed helper always chose product icons, supplied a generic fallback and
forced a 1x representation. It could not represent action-specific icon
precedence, the native letter placeholder, or action-owned cache/lifetime.
This proposal follows the proven native API instead of resizing that helper.

Paired `validation6-public` proves the availability mismatch on identical actual
fixture IDs: stock disabled false/null versus prior current hasAction=true and
32/64 images. Stock `validation6-cli` proves positive logical 32/representations 32
and 64, false/null after disable/reload, and restoration after enable. These are
helper observations and stored producer values, not canonical app-demand or
pixel-equivalence proof. Their captured state/hash excerpts are retained in this
proposal's `observed-state-excerpts.json`.

## Compiler, overlay and frozen-file evidence

Evidence directory: `logs/wp/extensions-info-native-icon-proposal`. Initial v1
original/proposed bytes, full hashes, exact stripped compiler argv and removed
output options are retained there. The current v2 paths and checks are listed above. `initial-two-tu.patch` and metadata preserve
the review version before adding the required parser companion.

All three proposed translation units are checked with their own exact current
`out/Base` compile command, preserving warning/error and Clang plugin flags,
using `-fsyntax-only`. Object/dependency/serialized-diagnostic outputs are
removed. VFS uses `use-external-names:false` and maps the canonical owned path
plus Chromium's `arc` symlink spelling; shared native files retain their own
source spelling. Distinct negative `#error` overlays prove routing for each TU.
No build graph, build/package, app, shared lock, profile or board operation runs.

Initial v1 checks passed for all three then-current TUs: compiler exit0 with empty
stdout/stderr; each unique negative overlay produced its expected sentinel and
exit1. `git apply --check --whitespace=error` passed, and applying the complete
patch only to copied originals reproduced all three proposed files byte for
byte. Frozen actual source/helper/runtime hashes, sizes and mtimes stayed
unchanged for all ten recorded files. Fresh read-only review approved the
complete three-hunk version with no remaining blockers, including parser size
consistency, existing dependencies, exact command evidence and documented limits.

| Source | Actual base SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/extensions/browser/extension_action.cc` | `cb6943b27b7ce823e5746cf4948d4c684c952ceebf6aa2b2afa124813975c5eb` | `bc80d6285ba1ef4359d957876c202d31b5d3404bd1a655de30fd63cedc842782` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `3cfccf64a77926987287cb2091a292acb566785a088a03723565a4212599747c` |
| `chromium/src/extensions/browser/icon_util.cc` | `55e084a3ced904b67980002807f76eb03cd20660f77468bd1862c543b708e2ac` | `e81743acb5e6a1243f0e0c9202c92dda1c2927c76dd04470a602beda8d6fc20e` |

Current patch SHA256: `2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`.

## Unexecuted runtime and required fixtures

No scenario or runtime validation of this proposal has run. Native 32/64 shape is
proven; current proposed pixel content, resource selection at each scale,
placeholder appearance, canvas parsing and asynchronous-load timing remain
untested. There is no clean boundary-diff or acceptance claim.

A future paired fixture phase needs distinct product and action icons (including
multiple sizes), missing icons, invalid/unloadable icon resources, canvas
`action.setIcon` at 1x/2x, and delayed loading. Observe exact native logical and
representation sizes, actual source/pixels, placeholder behavior, update timing,
disable/enable/reload cache replacement and retained snapshot lifetime. Use
proven owned fixture IDs and strict cleanup. Preserve true caller attribution;
recorder-v2 omission of probe calls still limits method-boundary proof.
