**Proposal only: native release pin semantics. Product sources are not changed by this refresh.**

## V7 current omnibox range-guard browser refresh

This is the current **unapplied** pin proposal, based on the parent's confirmed
guard-applied actual browser SHA256
`ebc84f16a82b191e33f83da7b188cee175fe11ab73b71aedd595a96e31f6b35a`.
Only two browser hunk line numbers changed from V6 (257→258 and653→654).
All semantic added/deleted lines and all other patch lines are identical;
native/context patch sections remain literally unchanged. The guard patch is
already applied by the parent, SHA256
`72886519676cca645ff2a13ff8e300c8ea5b46773d37eb591be7917cc9fff2c6`.
This pin proposal neither adds nor changes its semantics.

Current pin patch SHA256:
`76961224c918c46c82c7e435f5586b739fb94caf1d8769c5d052bc6434100d44`.

| Source | Actual SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` | `91cf63c767fcbf4421875701c23d623bd1637deade57eea0ab4b9c5093d8d6c5` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `63e3c06b24b905a637087ac515883d50d1eff582802dfb2e3966c5b93efc7769` |
| `arc/api/arc_extensions_browser.mm` | `ebc84f16a82b191e33f83da7b188cee175fe11ab73b71aedd595a96e31f6b35a` | `9b49857b44afc3c4ca7170a62ffb589d24ad3faba84cd088e7399e1fd9306424` |

Evidence is retained under
`logs/wp/extensions-pin-semantics-proposal/v7-omnibox-range-refresh`.
The previous root patch/document and the entire V6 evidence tree are preserved;
all V6 file hashes were reverified. The three profile-observer proposal artifacts
also retain their final hashes. No product/helper source, native source, GNI,
runtime artifact, app, profile or board state was written by this worker.
Only this owned proposal patch/document and new V7 logs were written.

### Guard, popup and icon-route preservation

Applying only the parent's guard to the V6 actual browser produces the exact
V7 base. Applying only that guard to the V6 pin browser independently produces
the exact V7 pin browser. These byte comparisons preserve the popup keyboard
handler and all unrelated browser code. The omnibox method itself is byte-identical
between current actual and proposed browser: its unsigned `<12` CHECK remains
at entry before browser liveness. The check_op include occurs once, and the
removed RegisterIconUpdateObserver/OnIconUpdated route remains absent.
`guard-on-v6-before.json`, `guard-on-v6-pin.json` and
`guard-popup-icon-preservation.json` retain those checks.

Only the refreshed browser plus pin companion was freshly syntax checked with
its own current Base command, through canonical and Chromium arc-symlink VFS
paths. Original warning/error, optimization and Clang plugin flags were retained;
object/dependency output flags were stripped and syntax-only was added.
Compiler exit0 with empty diagnostics. The independent negative overlay found
`ARC_PIN_V7_OMNIBOX_RANGE_REFRESH_NEGATIVE_BROWSER`, expected compiler exit1.
No graph regeneration, engine build/package, app launch or runtime test ran.

Native/context base and pin-proposed bytes match V6 exactly, as do the three
native-icon base/proposed byte pairs. Earlier compiler/sentinel evidence is
reused; those TUs were not freshly compiled, and byte reuse does not establish
a current transitive-header/build-flag closure. The native-icon proposal is
unchanged at SHA256
`2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`.

Pin then icon and icon then pin pass strict whitespace/apply checks on exact
copied current sources and produce identical bytes across all five files.
Combined context SHA256 remains
`73ba0b1d7bd9d78e7a60963570e01d6214d2c9c3a6cc3dd23def3418fa05218f`.
`composition.json` retains commands and all final file hashes. Shared native
pin assertions and icon-size hunks remain prerequisites for the owned companions;
this refresh does not activate them.

### Concurrent validation11 artifact drift

All ten actual source/helper-script/icon-proposal inputs retained their hashes
and sizes. Source mtimes are recorded separately because the parent's runtime
wrapper may touch them while freezing its build. Two shared **derived** probe
artifacts changed during this refresh:

| Artifact | Initial fingerprint | Later fingerprint |
|---|---|---|
| `logs/wp/extensions-service-probe-build/ExtensionsServiceProbe.dylib` | `045b6a53620a0e9b36a5dc069a3a75ed30a52e84ae296b2bbc5c4ec1ca5bbdf0` (150656 bytes) | `d3ee51bedf4f8d7c4a6d45b5e7f5cc535c6b9a8854a75ee184db9b9e68070f05` (167728 bytes) |
| `logs/wp/extensions-service-probe-build/extensions_service_probe.js` | `51b0da848dce46c5ad8ca1cf2faaa2f007cd0f1b0398b5c4aa00e15b9d1ba7d9` (76868 bytes) | `54c03798c9812c33bb0e55208385b05d88ded02b7149f979fb620167c0714ec3` (89633 bytes) |

The parent confirmed these were expected validation11 helper rebuilds under its
own lock, before its runtime before-snapshot. This worker neither built nor
modified those artifacts. `freeze-check.json` explicitly records drift and does
**not** claim all12 input bytes stayed fixed or that this proposal shares a
runtime epoch. No rebuild/cancellation was attempted.

Fresh read-only V7 review found no concrete findings: only two hunk headers
change, all scoped semantic bytes are retained, current guard/popup/icon code
is preserved, and both five-file application orders agree. Reviewer executed
no build/runtime actions and made no edits. Source composition and bounded
compiler acceptance do not establish prefs, callback order/suppression, OTR
persistence, omnibox behavior or canonical scenario acceptance. Meaningful
pin/runtime validation remains **UNEXECUTED** for this proposal.

## Historical V6 popup-keyboard browser refresh

This is the current unapplied pin proposal. Only the browser integration base
and its hunk context changed from V5; semantic added/deleted lines are identical.
Native and context patch sections are literally unchanged. The actual browser
already contains the applied popup keyboard handler and the removal of the extra
icon observer route. Both remain intact in this proposal. No actual product,
shared native, helper, runtime, GNI or board state was changed by this refresh.
The icon proposal and popup application document were not edited.

Current pin patch SHA256:
`ec73a0ff215ff9388e341ef8b4d285220f6aec7185acbbfa7fde0f4941aa5aef`.

| Source | Actual SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` | `91cf63c767fcbf4421875701c23d623bd1637deade57eea0ab4b9c5093d8d6c5` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `63e3c06b24b905a637087ac515883d50d1eff582802dfb2e3966c5b93efc7769` |
| `arc/api/arc_extensions_browser.mm` | `84d532be49b5123068ed33ff340d741ef346a02234f3f58c43205b930f992b1e` | `63aec4c552fb66b507e44dc79a08aa822bdc30cc97c8fe780fa6e0032e1a4924` |

Evidence: `logs/wp/extensions-pin-semantics-proposal/v6-popup-keyboard-refresh`.
Prior proposal/document snapshots were preserved there before refresh. Strict
whitespace/application checks pass on exact source copies. Independently applying
the already applied popup patch to the V5 pin browser produces exactly the V6
pin browser bytes above (`popup-preservation.json`). Cached lookup and pin/unpin/
move semantic changes remain those reviewed previously; the popup handler is
preserved, and the removed `RegisterIconUpdateObserver`/`OnIconUpdated` route
remains absent.

Only the browser plus pin companion was freshly compiled, through canonical and
Chromium-symlink VFS paths using its own actual Base TU command. Warning/error and
Clang plugin flags were preserved; dependency/object outputs were stripped and
`-fsyntax-only -Wno-unused-command-line-argument` were added. Compiler exit0 had
empty diagnostics. The independent negative overlay found
`ARC_PIN_V6_POPUP_REFRESH_NEGATIVE_BROWSER` with expected exit1. Exact commands,
overlays and results are retained. No app was built, packaged or launched.

Native/context pin base and proposed bytes match V5 exactly. Native-icon base
and proposed bytes also match their recorded copies; the icon proposal remains
unchanged at SHA256
`2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`.
Their earlier syntax/sentinel evidence is reused, not freshly rerun. Byte reuse
does not establish a fresh transitive-header or build-flag closure check. Neither
native TU, context TU nor combined-context TU was recompiled in this refresh.

Pin then icon and icon then pin both apply to exact copied sources and produce
identical bytes across all five affected files (`composition.json`). Combined
context SHA256 remains
`73ba0b1d7bd9d78e7a60963570e01d6214d2c9c3a6cc3dd23def3418fa05218f`.
All twelve recorded frozen source/helper/runtime/icon-proposal inputs retained
their hashes and sizes; mtimes are recorded separately. These checks prove source
composition and bounded compiler acceptance, not link/runtime parity, canonical
acceptance, policy behavior or the cause/order of callbacks.

The shared native pin assertions and icon-size hunks remain prerequisites: fold
them into maintained patches and land before their owned companions, or integrate
together. Force-move no-op/hard checks, zero context setter callbacks
and cache-only browser moves retain the reviewed scope. Runtime callback, prefs,
OTR and keyboard behavior remain unexecuted for this proposal.

## Historical V5 browser-only refresh

This was the V5 pin proposal route. Earlier V4/V3 sections retain historical
bases/evidence. The adjacent patch is unapplied; its semantic changed lines are
identical to V4. Only hunk context/coordinates and the new browser integration
base have been refreshed. No actual product/shared/helper/runtime files were
changed by this refresh, and the native-icon proposal and historical APPLIED
info/popup documents were not edited.

Current pin patch SHA256:
`2cbf65c93c04ad19dc82478701cb82dbef6f39bbdbefcd79d2a58f3b5eed4cf6`.

| Current source | Actual SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` | `91cf63c767fcbf4421875701c23d623bd1637deade57eea0ab4b9c5093d8d6c5` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `63e3c06b24b905a637087ac515883d50d1eff582802dfb2e3966c5b93efc7769` |
| `arc/api/arc_extensions_browser.mm` | `08b4caeedfed9b9659498d950f45c71853751ae47e1ea32d19a83203597cf136` | `b88de1bf518b91030f8e2e011c7941373a9d6af0ccd84ada085d8f6e8c9ef227` |

Evidence: `logs/wp/extensions-pin-semantics-proposal/v5-browser-refresh`.
The prior patch/document were copied there before refresh. Copied current
application passes strict whitespace/apply checks and yields the recorded
proposed bytes. The current browser's applied icon-route removal remains
intact; the pin companion changes only its previously reviewed cached lookup
and pin/unpin/move methods. Those method bodies are unchanged from V4.

Only the current browser plus pin companion was recompiled, using its own exact
Base TU command, preserved warning/error/Clang plugin flags, and output-stripped
`-fsyntax-only`. The symlink-correct VFS maps canonical and Chromium `arc` paths.
Compiler exit0 with no diagnostics; the independent negative overlay detected
`ARC_PIN_V5_BROWSER_REFRESH_NEGATIVE_BROWSER` with expected exit1.

Context and native pin base/proposed bytes match V4 exactly, so their earlier
strict compiler/sentinel evidence is reused. All native-icon source and proposed
bytes match V2 exactly, including its context TU; that proposal is unchanged at
SHA256 `2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`.
No context, native pin, native-icon or combined-context TU was recompiled in
this browser-only refresh; `reused-evidence.json` records exact byte checks.

Both refreshed pin then unchanged icon, and unchanged icon then refreshed pin,
apply to copied current sources. All five result files are byte-identical across
orders. The combined context is unchanged from V4, SHA256
`73ba0b1d7bd9d78e7a60963570e01d6214d2c9c3a6cc3dd23def3418fa05218f`.
`composition.json` retains commands and all resulting hashes. This is source
composition proof, not an executed product/integration claim.

Fresh read-only review approved all hashes, preserved method bodies and icon
removal, both application orders, byte-reused evidence and documentation with
no blockers. Final `freeze-check.json` verifies all twelve recorded actual
source/helper/runtime/proposal inputs retained their hashes and sizes; mtimes
are recorded separately for any announced parent force-compile touch. Shared
native hunks remain prerequisites: fold them into maintained patches and land
before owned companions, or integrate together. Host-only native assertions,
force-move no-op/hard checks, suppressed zero context callbacks and cache-only
browser moves retain the reviewed scope. Runtime callbacks/prefs/OTR behavior
remain unexecuted for this proposal.

## Historical V4 validation7 refresh

Current unapplied patch SHA256: `19dcbcc70d47f3bf0261ce8935902bd3e25a9214de81289b97bc58c5377fe5bb`. The proposal
was refreshed against the exact validation7 content snapshot below. Existing
proofs are preserved; earlier sections/tables describe their historical bases.
Only hunk coordinates/context were refreshed: changed semantic lines match the
prior reviewed proposal. No actual product/shared/helper/runtime content was changed by this refresh.

| Current source | Actual SHA256 | Proposed SHA256 |
|---|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` | `91cf63c767fcbf4421875701c23d623bd1637deade57eea0ab4b9c5093d8d6c5` |
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `63e3c06b24b905a637087ac515883d50d1eff582802dfb2e3966c5b93efc7769` |
| `arc/api/arc_extensions_browser.mm` | `b1ed32a375b12d6afd346a46b5697d1237c9267147acccdf9e908ea2a771ef42` | `9d73a3354d87da6a41e2a4114f131eb7db895f0f026fb79a588183b7e42c57ca` |

Current evidence: `logs/wp/extensions-pin-semantics-proposal/v4-current-refresh`.
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

The adjacent `extensions_pin_semantics.patch` contains three repository-root
hunks: the Chromium `ToolbarActionsModel` change and two owned companions in
`arc_extensions_context.mm` and `arc_extensions_browser.mm`. No hunk has been
applied to the actual source tree. The lead must first fold the native hunk
into a maintained `patches/*.patch` and land/apply it. **Do not apply either
owned companion first:** their newly reachable native calls can trigger the
current Chromium debug restrictions.

The proposal changes neither service factories nor pref storage scope. Native
OTR persistence scope is unresolved; absence of an OTR guard does not establish
where Arc's original engine stores those writes.

The narrow native change is gated on the exact `arcmod-host-app` command-line
switch. Outside that mode, all original Chromium assertions still run.

| Native method | Host-only change | Preserved behavior |
|---|---|---|
| `SetActionVisibility` | Permit the release-engine paths rejected by the force-pinned, OTR, and stored-pref-membership debug assertions | The visible-state `DCHECK_NE`, real pref append/remove and write, filtered-state consistency `DCHECK`, and actual action dispatcher notification |
| `MovePinnedAction` | Permit OTR calls past its OTR debug assertion | The actual force-pinned early-return guard, nonempty/present-action/stored-action hard CHECKs, native order calculation and rotation, pref write, and filtered-state consistency `DCHECK` |

There is no direct replacement of prefs, manually constructed pinned list,
synthetic observer event, or fake app callback. Unknown IDs go through native
`SetActionVisibility`, including repeated writes when an ID is stored but
still absent from the visible projection. The native getter continues to hide
IDs without toolbar actions. The proposal intentionally does not add a stored
pref deduplication step: it would change the original release behavior.

The context companion preserves stable first-occurrence input deduplication,
the copied previous visible order, per-request pin/readback/gather order, model
reacquisition, and existing visibility safety rechecks. It removes only the
extra OTR early return and HasAction/force-pinned filters. A visibly pinned
forced item now reaches `MovePinnedAction` and consumes its sequential index,
while the native forced-move guard remains a no-op. The existing shared
`_orderSuppression` AutoReset remains around the entire mutation sequence,
including pref notifications and moves. `OnToolbarPinnedActionsChanged`
continues returning while suppressed; no final catch-up DidChange callback
is introduced. Mutation-specific `didChangePinnedExtensionOrder:` delivery
should therefore remain zero. Runtime callback behavior is still unexecuted.

The browser companion pins iff not visibly pinned and unpins iff visibly
pinned, removing extra HasAction/force/OTR filters. Its move path keeps
`index < visible_count` and `IsActionPinned`, and adds the original cache
requirement. `GetCachedActionForId` performs only `actions_.find` and returns
an existing nonnull value; it does not create an action, register observers,
or call the autovivifying `GetActionForId`. Missing cache entries, null entries,
unpinned IDs, and invalid indices remain no-ops. Forced moves reach the native
no-op guard. Existing nullable browser/model/bridge lifetime checks remain.

Retained original evidence, refined by the extensions worker's native scout:

| Original address/path | Consequential evidence |
|---|---|
| `42eb57c`, `research/2026-10-22-sol/evidence/agents/cef/decompile/functions/42eb57c.c` | Stable dedup; omitted visible IDs call unpin; each requested ID is checked/pinned/checked and immediately gathered in the same loop; then gathered IDs call Move at incrementing indices. Lines 195-215 establish that this is not a separate post-pin visibility scan. |
| `7cd94c0`, adjacent `7cd94c0.c` | Native visibility setter reads `extensions.pinned_extensions`, erases or appends without HasAction/force/OTR guards, writes prefs, and calls the real dispatcher. Hidden duplicate-pref writes are permitted by this release path. |
| `7cd9f40`, adjacent `7cd9f40.c` | Native force-pinned guard returns before hard list/action checks; nonforced moves preserve native ordering/pref operations. |
| `35d8bf4`, extensions-scout supplied getter evidence | Unknown IDs remain in prefs but are filtered from visible pinned actions. This address was supplied by the extensions scout; its body was not independently recovered by this worker. |
| `43e2454`, `43e2540`, `spec-3/revisions/v1/bodies/043e2454.txt` and `043e2540.txt` | Browser pin/unpin forward through the actual ExtensionsContainer. |
| `43b6c18`, `spec-2/classreview/backend/043b6c18.c` | Pin only when absent from visible pinned list; unpin only when present. No additional HasAction/force/OTR guard. |
| `43e2714` and `43b606c`, `spec-3/revisions/v1/bodies/043e2714.txt` and `spec-2/classreview/backend/043b606c.c` | Browser move requires model, in-range index, existing nonnull cached action, and currently pinned ID; otherwise no-op. |

All abbreviated `spec-*` paths above are under
`research/2026-10-22-sol/evidence/agents/`. Static decompilation establishes the
bounded paths above; callback timing, observer reentrancy, and complete OTR
persistence parity require actual paired execution. Existing weak-profile,
model-reacquisition, and visibility safety checks are retained rather than
claiming native parity for arbitrary reentrant mutations.

Validation performed without building or launching:

- `git apply --check --whitespace=error arc/browser/extensions_pin_semantics.patch`
  passed against the frozen actual files.
- The same complete patch was checked/applied to temporary copied originals.
  All three resulting files matched the proposed copies byte-for-byte.
- All three proposed translation units passed Clang `-fsyntax-only` with their
  own actual `out/Base` compile command, including the real warning/error/plugin
  flags. Sources were selected through a VFS overlay with
  `use-external-names: false`, mapping both canonical owned-file paths and the
  Chromium `arc` symlink spelling. No source argument was redirected to a
  differently configured translation unit.
- Object and dependency output options were stripped before invocation:
  `-o`, `-MF`, `-MT`, `-MQ`, `-MJ`, `-MD`, `-MMD`, `-MP`, `-MG`, and serialized
  diagnostic output options when present. No build graph command was executed.
- Separate negative VFS sentinels produced the expected unique `#error` for
  each original TU path. This verifies that successful checks actually read
  the overlays, including both owned ObjC++ files, rather than the unchanged
  real sources. These expected failures test overlay routing only.
- Original source SHA256 values were checked again after validation; all
  three actual product/shared source files were unchanged.

| Proposed TU | Actual Base target | Compiler exit | Diagnostics |
|---|---|---:|---|
| `../../chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `obj/chrome/browser/ui/toolbar/impl/toolbar_actions_model.o` | 0 | None |
| `../../arc/api/arc_extensions_context.mm` | `obj/arc/arc_core/arc_extensions_context.o` | 0 | None |
| `../../arc/api/arc_extensions_browser.mm` | `obj/arc/arc_core/arc_extensions_browser.o` | 0 | None |

Temporary copies, overlay, exact stripped argv, command hashes, compiler output,
and negative-overlay results are retained for review at:

`logs/wp/extensions-pin-semantics-proposal`

The original three-TU snapshot at that path is preserved. The refreshed current
context evidence is retained separately at:

`logs/wp/extensions-pin-semantics-proposal/v2-context-refresh`

This v2 refresh incorporates the parent's current native-correct list and
command-enumeration/shortcut implementation into the context base. The actual
pin-setter body was unchanged; only the proposal context hunk offsets and base
fingerprint changed. The complete refreshed patch passed apply-check and was
applied only to copied current sources. Only the combined current-context plus
pin-companion TU was recompiled with its exact Base command and stripped output
and dependency flags: exit 0, no diagnostics. Its new VFS path was independently
verified with the expected context-only negative sentinel.

The native and browser source AND proposed-copy SHA256 values match the
original checked snapshot. Their prior successful compilation and VFS evidence
are retained; neither TU was redundantly recompiled. The table above therefore
combines v1 native/browser evidence with v2 current-context evidence. Parent
wording and prior snapshot logs remain preserved. Actual product/shared sources,
runtime helpers, artifacts, and the source-frozen batch were not modified.

| Frozen original file | SHA256 |
|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` |
| `arc/api/arc_extensions_context.mm` | `ac3bc2f4048cbbc8c65621f37d7058c4255bc8b5bb4a8f6bf9250a67ff65c6fb` |
| `arc/api/arc_extensions_browser.mm` | `b3aabc4d7fd31bc0fb92c72632752205574f1f16efbb8dd621329a3696950b58` |

No synthetic tests mirroring the setter were added. They would not establish
pref propagation or actual app callback behavior. **Meaningful runtime remains
UNEXECUTED.** Before acceptance, the lead needs the same original/current
scenario with real visible fixtures, unknown/hidden IDs, repeated hidden-ID
writes, forced pins, cache-missing browser moves, and invalid move indices.
Measure actual prefs, real dispatcher effects, visible order, and exact zero
context DidChange callbacks during the setter. OTR tests require separately
proven ownership and actual persistence readback; this proposal supplies no
assumed storage destination.

Cleanup for unknown IDs cannot be certified through visible pinned order alone:
the original getter filters them, and omitting an already-hidden ID from a later
context setter does not remove its stored entry. Use an explicitly disposable
test-owned data scope with raw pref baseline/readback or independently verified
removal of that entire test-owned scope after exit. Preserve raw evidence and
never treat filtered getter equality as proof that hidden prefs were restored.

This is a proposed compatibility correction and compile evidence only. It does
not make the package acceptance-clean or validate the source-frozen batch.

A fresh read-only reviewer checked the native bodies, assertion scope, context
algorithm, cache-only browser guard, and documentation and reported no remaining
findings. The reviewer performed no edits or runtime checks.

## Historical V3 combined-TU refresh

The earlier sections and tables retain historical v1/v2 evidence. Their old
hashes and the statement that unchanged native/browser TUs were not redundantly
recompiled describe those earlier snapshots only. The then-current v3 integration patch
and all three combined translation units were freshly checked in that snapshot.
Prior patch/document bytes and evidence were preserved without overwriting v1/v2.

Current evidence: `logs/wp/extensions-pin-semantics-proposal/v3-current-refresh`.
The refreshed patch was applied only to copied current originals under that
path. `git apply --check --whitespace=error` passed before and after refresh.
No product/shared/helper source, helper/runtime artifact, app, build/package,
profile, lock or board state was changed. Actual source/artifact hashes, sizes
and mtimes remained unchanged throughout, as recorded in `freeze-check.json`.

| Current actual source | Original SHA256 | Proposed copy SHA256 |
|---|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` | `91cf63c767fcbf4421875701c23d623bd1637deade57eea0ab4b9c5093d8d6c5` |
| `arc/api/arc_extensions_context.mm` | `468bc9da0bdf22561ccb96e2e9af53b6ff5d9087c8b0bb21d1b584c6d62098e5` | `c8ce0190c7033cf20f93e8cad90f9150e84d4bdd44fbfde0463dd64867fecffd` |
| `arc/api/arc_extensions_browser.mm` | `6a55ee3efe5010cf616e42a82d35aee7dfc455657fe6587eac3a88759c19b727` | `354a90c3c0c122dcc78e1d1732dc5ef8255cd3ea130de3c93a1d82d62968369a` |

The refreshed patch SHA256 is
`2a9b12be1ce150dee8a431170ddf418b4bebc3478e10d2f75bcbdd9c71cc5696`.

Each TU was selected from its own current `siso query commands` compile line,
never another TU's flags. The original source argument, all warning/error and
Clang plugin flags were retained. Only object/dependency/serialized-diagnostic
output options were removed; `-fsyntax-only` and the overlay were added. No
build-graph command ran and no compiler output artifacts were emitted. Exact
original command hashes, stripped argv, removed options, stdout/stderr and exit
results are retained in the per-TU files.

The VFS overlay uses `use-external-names: false`, maps both canonical owned
paths and Chromium's `arc` symlink spelling, and preserves each source's original
include/compile context. Separate negative overlays replaced each exact TU with
its own unique `#error` sentinel; all three expected errors were observed.

| Current combined TU | Syntax exit / diagnostics | Negative overlay |
|---|---|---|
| ToolbarActionsModel native | 0 / empty | `ARC_PIN_V3_NEGATIVE_NATIVE`, expected exit 1 |
| ArcBrowserContextExtensions | 0 / empty | `ARC_PIN_V3_NEGATIVE_CONTEXT`, expected exit 1 |
| ArcBrowser Extensions category | 0 / empty | `ARC_PIN_V3_NEGATIVE_BROWSER`, expected exit 1 |

`scope-body-comparison.json` verifies byte-identical proposed bodies against v1
for native `SetActionVisibility`/`MovePinnedAction`, the context pin setter,
cache-only lookup, and browser pin/unpin/move. Thus current unrelated context
command/list behavior and browser integration changes are included in compile
evidence without broadening this proposal. Host-only assertion relaxation,
preserved native force-move no-op and hard checks, suppressed zero context
DidChange delivery, and cache-only moves retain the original reviewed scope.

Fresh read-only v3 review found no code blockers and confirmed identical semantic
patch lines, current source/proposed hashes, all three successful compiler
results and negative overlay routing. Runtime remains **UNEXECUTED**. This
refresh establishes compile/integration evidence only; it does not establish
pref propagation, callbacks, OTR persistence or scenario acceptance.
