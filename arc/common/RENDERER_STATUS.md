# Renderer work package checkpoint, 2026-10-23

Acceptance verification is in progress. No commits or pushes. This supersedes
run1 implementation gaps; the final run2 report must record the completed
fresh runtime results rather than treating this checkpoint as acceptance.

## Implemented, by file

- `arc/api/arc_renderer_values.mm`, `arc/common/renderer_values.h`: ArcHSLAMatrix
  aggregate and twenty float getters; ArcMetaTagEntry/Selector; all
  ArcHtmlToMarkdownResult constructor/getters with native copy/nil semantics.
- `arc/api/arc_renderer_effects.mm`, `arc/common/effects_browser.h`: immutable
  profile service identity with weak lifetime; exact-host ordered Boost CSS/JS
  config, identifier replacement/removal quirks; native HostZoomMap factors.
- `arc/api/arc_renderer_ad_block.mm`, `arc/common/ad_block_browser.h`: async raw
  EasyList compilation to Chromium indexed rulesets, parsed protobuf badfilter
  removal across lists, exclusions, feature/config publication, explicit clear.
- `arc/api/arc_renderer_web_contents.mm`: per-WebContents/per-frame associated
  Mojo bridge; direct CSS, profile config distribution, frame-disconnect safety,
  real disallowed-load count updates and native primary-page reset; enabled
  debug queries through live same-profile agents. Core owns the count property
  and weak top-band delegate slot.
- `arc/renderer/ad_block_filter.{h,cc}`: Blink subresource filter backed by the
  indexed matcher, document/generic activation exceptions, native internal-URL
  exclusions and request-destination mapping. Real ReportDisallowedLoad drives
  browser counts. Clearing replaces any installed filter with a non-null
  allow-all adapter; Blink does not permit null filter removal.
- `arc/renderer/render_frame_agent.{h,cc}`: user-origin direct CSS replacement
  and removal; Boost CSS/shared keys and isolated-world-6 JS at document start;
  cosmetic selectors, activation exceptions, exact debug formatting and
  process-shared success cache with configuration-generation invalidation.
- `arc/api/arc_renderer_extraction.mm`, `arc/common/extraction_values.h`:
  ArcBrowserContextPageContentExtraction, ArcExtractedPageContent, ArcContentNode
  using Chromium's AnnotatedPageContent proto; exact node/text/form enums,
  optional string presence, redaction, title/URL/wall-clock metadata, weak UI
  lifetime and frame-identity checks. Reuses UI-owned form/control value types.
- `arc/common/renderer.mojom`, common/renderer build targets, `renderer.gni`:
  internal associated IPC, browser/renderer dependencies and source ownership.
- `arc/renderer/probe.{m,ts,sh}`, `probe_diff.py`, `compare_extraction.py`,
  `compare_features.py`, `value_probe.{mm,sh}`: disposable-app-only genuine
  original/ours bounded ABI, rendering and feature probes. Raw transcripts,
  callbacks, metadata and visual snapshots are retained separately from
  accepted scenario goldens. No actual Arc profile is read or changed.
- `renderer_contract_evidence.{md,py}`, `RENDERER_INTEGRATION.md`,
  `RENDERER_VISUAL_GAPS.md`: bounded original binary evidence and exact pending
  shared changes, including recovered Page scheme precedence and visual gaps.

## Verified before the pending combined fresh run

- `results/renderer-values`: original/ours 36 public methods and property
  encodings match, matrix aggregate/getters/copies/nil tested, zero selected
  boundary differences. An older recorder's C-array NSInvocation trap was
  isolated; recorder worker fixed skipping unsupported array arguments.
- `results/renderer-direct-run2`: 14 original/ours computed-style snapshots
  match, plus ten explicit stylesheet calls with zero selected boundary diff.
  Includes same-key replacement, independent keys, missing removal, document
  navigation and existing/new iframe scope. Actual screenshot was inspected.
- Original `results/renderer-effects-original-run2`: profile CSS/JS do not
  apply immediately; navigation applies main-frame user CSS and isolated JS;
  unregister does not alter current document but clears the next document.
- Original simple/rich extraction outputs under `results/renderer-extraction-*
  -run2`: asynchronous main-thread one-object callback; detailed proto tree,
  redacted password, form options and Unix extraction time recovered. Paired
  current-engine and repeated-mutation cache behavior remain to be verified.
- `results/renderer-adblock-enabled-rules-original-run2`: real script block,
  count one and cosmetic hide observed. Initial publication stayed NoBlockList
  beyond the first 15 seconds, so the new probe waits up to 60 seconds for
  observed readiness. Exclusion receiver has no native ruleset-pointer write;
  no synthetic exclusion gate was inferred from this delayed publication.
- Final production .mm and renderer C++ real-flag syntax checks passed, including
  review fixes for activation exceptions, empty-list clear, internal schemes,
  destination fallback and whitespace around badfilter. Combined build passed
  10:04:19 UTC and signed package passed. `results/renderer-combined-run2` has
  exact ten CSS boundary calls and all 14 rendered snapshots, including site
  !important cascade. Rich extraction and immediate mutation repeat both match
  exactly: two complete trees, public metadata, main-thread async shape, bounded
  timestamps. This is explicitly subset validation because the broader run failed
  in unrelated later adblock phases; raw errors remain in extraction-normalized.
- Combined overall result is NOT parity: original stayed NoBlockList for the full
  60-second readiness window; native publication path is under audit. Ours
  crashed on second publication: replacing base::File closed the old descriptor
  on UI and triggered AssertBlockingAllowed. Owned fix moves old/rejected file
  destruction to MayBlock pool. Boost differs at missing document-start hook;
  new-document cosmetic application is also missing for that reason. Feature
  toggle filter updates lag debug until shared setter refresh hook is applied.

## Canonical launch-slice retry2

Full current build/package/run completed 10:17:54 UTC, preserved immutably in
`results/renderer-launch-slice-run2-retry2`. Launch/navigation/storage/relaunch
steps pass; both quits time out and need SIGTERM. Renderer arguments/returns
match; launch1 has count differences (CSS 8/3, count getter4/2, delegate2/1)
from the differing second-content path. Launch2 renderer subset is exact.

`diff-launch-1.txt`, first two lines verbatim:

```text
methods differing: 174 (missing 13, extra 36, returns 13, callback args 3, counts 109; internal-only 9)
callback sequence: {"golden": 106, "ours": 117, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

`diff-launch-2.txt`, first two lines verbatim:

```text
methods differing: 82 (missing 14, extra 33, returns 11, callback args 3, counts 21; internal-only 9)
callback sequence: {"golden": 46, "ours": 64, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

## Pending shared work / uncovered behavior

See `RENDERER_INTEGRATION.md`: native document-start hook, native runtime-feature
lookup/refresh, Blink Page color scheme, Page/paint HSLA filter and top-band
tracker. The latter visual APIs are intentionally not approximated. The current census (`logs/wp/renderer.golden-census.run2.json`) covers 23 final
launch transcripts, excluding candidates: 23 empty exclusions calls, 33 count
setters, 54 count getters, 27 top-band delegate setters, 59 stylesheet inserts.
There are no Boost, extraction, block-list/debug, filter/scheme or actual sampled
color calls. Broader feature goldens are requested from scenarios.

Our document filter does not cover worker requests or navigation throttles;
native uses URLLoaderThrottle with custom -90 network errors, whereas Blink's
subresource rejection uses its upstream error path. Off-UI early debug callback
synchrony is also not claimed: native access must remain on UI per architecture.
Feature11 renderer gating needs the newly added genuine flag-toggle probe;
only browser/debug gating is established by current static evidence.

## Contradictions / corrected research

- The brief says insertStylesheet:, but metadata/native use
  insertStylesheet:stylesheet: (identifier, CSS).
- Specs previously lacked or guessed extraction callback arity, enums, timing,
  host grammar and script world; native evidence and real original probes now
  pin those contracts. Extraction is annotated content, not arbitrary DOM.
- Native traps/early returns on absent backend conflict with architecture's
  safe lifetime handling. Implementation uses weak native ownership and UI
  dispatch; unavailable UI extraction returns one nil result.
- AI selectorsApplied discrepancy is in native C-to-Mojo conversion, which
  drops those ABI fields; public DTO initializer correctly copies its argument.
  AI worker was told to preserve that distinction (Chirp #134587).

## Original publication diagnostic completed

`results/renderer-native-checkpoints-run2/trace156` completed all ten adblock
observations under exact-PID bounded LLDB checkpoints. Eight pinned addresses
produced25 hits across three compilation/publication chains. Initial compiler
execution took14ms, readiness708ms. Process and debugger exited normally. No
native state, private profile or security settings were changed. Attachment
perturbs scheduling; the earlier uninstrumented60s timeout is still unexplained.

Native feature11=false continues blocking/counting and keeps cosmetics; only
browser debug is disabled. Reenable republishes exclusions and clears debug
cache. Native debug chooses the earliest surviving bound renderer-process agent
by registration counter; this can differ from the fixture's renderer. Process
ownership/routing refactor is being planned; current per-frame selection and
renderer feature gate do not yet satisfy this observed contract.

PiP flags requested by core are process-global (skipfalse/ignoretrue defaults),
with distinct video/document activation semantics. A host-gated shared Blink
proposal is ready in renderer_pip_blink_proposal.patch; core keeps public
selectors and renderer will supply the native common/IPC bridge. No Chromium
source was edited. Proposal compile validation is in progress with temporary
VFS overlays.

## Process ownership refactor checkpoint

New `arc/api/arc_renderer_process.mm` plus `arc/renderer/render_process_agent.h`
and `.cc` implement the associated process binder, RPH lifetime/incarnation
registry, per-profile retained file snapshots, native bind-order debug selection,
distinct rule/exclusion updates and bounded first-frame retry. Global pair
absence is distinct from engaged empty lists. Cosmetic availability is distinct
from a network-only ruleset (native debug NoBlockList for no cosmetic region).
Explicit exclusions work while disabled. Rebind replays an incarnation's own
pending snapshot rather than incorrectly inheriting current profile rules.

All three changed .mm and both renderer .cc files pass actual flags, and fresh
review found no remaining concrete source blocker. Evidence:
`logs/wp/renderer.process-refactor.checks.log`. Legacy frame config/debug Mojo
methods remain unused temporarily so shared incremental builds stay compiling.
Fresh full build/package/paired runtime is queued in
`logs/wp/renderer.process-parity.run2.log`, output `results/renderer-process-parity-run2`.
No process-refactor runtime parity is claimed yet. Exact lifecycle and shared
feature/document-start hooks are still required as listed in INTEGRATION.
