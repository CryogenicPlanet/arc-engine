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
ownership/routing is now implemented in the process registry/agent below;
its runtime validation exposed a startup ordering bug being corrected.

PiP flags requested by core are process-global (skipfalse/ignoretrue defaults),
with distinct video/document activation semantics. A host-gated shared Blink
proposal is ready in renderer_pip_blink_proposal.patch; core keeps public
selectors and renderer will supply the native common/IPC bridge. No Chromium
source was edited. All three affected Blink translation units pass actual Base
flags using temporary VFS overlays, zero diagnostics. This is not link/runtime proof.

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
Fresh full build/package passed 10:44 UTC. Paired runtime in
`logs/wp/renderer.process-parity.run2.log`, output `results/renderer-process-parity-run2`,
FAILED before any ours CSS observations: browser requested the process associated
interface before the first-frame fallback registered it; both renderers hit
unknown-interface NOTREACHED and aborted, followed by the browser sad-tab check.
This is an owned startup bug, not measured feature mismatches. The original
independently timed out cosmetic publication after 60 seconds. Corrective work
defers actual binding until the frame acknowledgment unless paired startup hooks
are guaranteed; registration order/configuration capture remains at launch.
No process-refactor runtime parity is claimed yet. Exact lifecycle and shared
feature/document-start hooks are still required as listed in INTEGRATION.

A fresh review also found that document-start Boost JS can destroy the frame.
The helper must report liveness and Chrome must skip extensions after deletion.
The owned hooks proposal now includes that guard; source correction is in progress.


## Safe fallback runtime verified

Fresh full build passed 2026-10-23 10:58:32 UTC, package/deep strict signing
passed. Framework SHA256 is preserved in
`logs/wp/renderer.framework.safe.run2.sha256`.
`results/renderer-safe-parity-run2` completes both engines with 14 stylesheet
states each and exact computed-style parity. Selected CSS boundary diff:

```text
methods differing: 0 (missing 0, extra 0, returns 0, callback args 0, counts 0; internal-only 0)
callback sequence: {"golden": 0, "ours": 0, "first_divergence": null, "golden_from": [], "ours_from": []}
```

The strict extraction comparator also passes both rich extractions, including
a changed text node: exact recursive trees, public runtime metadata, asynchronous
main-thread callback shape, and bounded Unix wall-clock timestamps. Both actual
styled screenshots were inspected. No unknown-interface, fatal or signal marker
occurred. This verifies the safe process fallback removed the prior startup crash.

The separate `results/renderer-safe-adblock-run2` is INCOMPLETE: its BEST_EFFORT
compiler request never became visible to debug within 60,588ms. There was no
crash; feature toggles were not reached. The original also repeatedly stalls in
cold uninstrumented runs, but shared cause has not been proved. Preserve the
prior successful bounded native checkpoint run as separate evidence. A new
paired idle-before-request diagnostic is queued in
`logs/wp/renderer.followup.run2.log`; it also records native filter pixels and
all eight classes' public runtime surface. Its outcome is pending.

Refreshed `arc/renderer/coverage_report.py` snapshot now covers 24 accepted launch
transcripts: 24 empty-exclusions calls, 34 zero-count setters, 56 getters,
28 top-band delegate setters and 60 stylesheet inserts. No active blocking,
Boost, extraction, scheme/filter or top-band sample callbacks occur there.
Shared current diffs still contain count/path differences and a stale
pre-fix tabs-navigation failure; provenance/hashes and exact first two diff lines
are captured in `logs/wp/renderer.coverage.run2.json`. These are shared mutable
artifacts, distinct from the immutable owned paired probes and launch snapshot.

The visual effects proposal is compiler checked in nine translation units,
including six codec tests under two security modes (compiled, not executed).
`arc/common/renderer_effects_blink_proposal.patch` remains proposal-only;
`logs/wp/renderer-effects-proposal/REVIEW.md` lists transport, runtime and native
image-path gaps. Owned transport work is being prepared as a separate unapplied
proposal, keeping production source stable for runtime validation.


## Native network and visual follow-up

Native error -90 is now pinned to `ERR_BLOCKED_BY_CONTENT_BLOCKER` through the
numeric ErrorToShortString jump table, with an independent -20/CLIENT
cross-check. The exact shared enum request is
`NET_ERROR(BLOCKED_BY_CONTENT_BLOCKER, -90)`; stock Chromium lacks it.
Native `URLLoaderThrottle` evaluates only WillStartRequest; redirect and later
hooks are no-ops. Clone shares a thread-safe dealer; each throttle captures an
exclusion snapshot but reads the current ruleset under the dealer lock at start.
The full renderer/browser network replacement is being prepared as an unapplied
proposal. It must replace fallback filtering only when the paired provider hook
is installed. Browser ResourceLoadComplete(error -90) owns native counts.

The extra native ResourceRequest final optional Origin (+1840/+1920) is now
proven to come from Blink ResourceRequestHead::TopFrameOrigin (+208), through
PopulateResourceRequest0x1f83a94. Independent getter/setter and frame/worker
producer evidence is in renderer_extra_origin_evidence.md. Earlier Mojo Read
identification was wrong and is explicitly corrected there. This is distinct
from isolated_world_origin and trusted isolation-info top-frame origin. A full
minimal request/Mojo/converter plus throttle proposal revision is being checked;
it remains unapplied, so this bypass is not production/runtime verified. Native count equality suppression is now proven at
0x438eba4-0x438ebac, so the existing nonzero PrimaryPageChanged reset guard is
correct; independent bulk wrapper initialization can still emit setter calls.

The owned filter/scheme transport proposal is now concrete and compiler checked
in five staged TUs with generated Mojo isolated under logs:
`renderer_effects_transport_proposal.patch`. It requires the shared Blink APIs
and remains unapplied. Eight public encodings match exactly; independent review
caught/fixed low-32-bit enum aliasing and missing peer guards. No compatibility
no-op branch was introduced. Production remains stable for current validation.

TopBandColorTracker recovery continues. Its native producer hit-tests layout
objects and analyzes CSS candidates, including a ytd-masthead heuristic. A root
background/screenshot approximation cannot establish that behavior.


## Follow-up build-only failure

The identity-fix follow-up acquired 11:17:07 UTC and failed both build attempts
before packaging/runtime in foreign `arc_omnibox_controller.mm:388` (-Wshadow
of matches declared373). Current owner source already uses provider_values;
app monitor confirmed that on Chirp136318. Exact attempts are preserved in
`logs/wp/renderer.followup-first-build-failure/`. No new metadata, pixel or adblock
outcome exists from this attempt. A single retry is planned after the COMMON
few-minute wait; no foreign source was changed.


## Follow-up retry and canonical baseline

After the required few-minute wait, a single retry was queued11:23:43UTC
(waiter80059, toolsession35162), logs/wp/renderer.followup-retry1.run2.sh.
It includes eight-class public surface comparison, newly accepted
renderer-public-api canonical original/ours diff, then the paired45s-idle
adblock/native-filter diagnostic. It was paused only while confirmed unstarted
under foreign lock95985 to insert the new canonical scenario, then resumed.
No new runtime result is claimed yet. Canonical sources/driver/script hashes
are frozen in logs/wp/renderer.canonical.source.freeze.run2.json.

PiP owned transport proposal is complete (threefiles, fourrealflagchecks0),
with an alternative18file combined effects/network/PiP owned patch,11checks0.
Both apply-check0/fresh review clean and remain unapplied. Core forwarding
code/dependency/overlap details are in
logs/wp/renderer-pip-transport-proposal/REVIEW.md.

The same unstarted retry waiter was briefly paused under foreignowner75168 to
add diagnostic-only CDP Startup.* histogram snapshots beforeidle/beforesetlists
and at ready/timeout. Sourcefreeze refreshed; standaloneObjC+Bunbuild0. These
read state and retain errors without changing task priority or readiness rules.
Existing runs show uninstrumented native publication can succeed, while mixed
startup ages rule out a universal60s threshold. No fence-cause claim is made.


The shared recorder/diff upgrade can omit injected-image calls when both files
have caller attribution. probe_diff.py now additionally compares all10raw
fixture calls/pairedreturns exactly (arguments, return values, order, mainthread
and single receiver), independent of that filter. Preserved safe-pair records
pass; a copied-data caller-filtered positive passes and a deliberate CSS-argument
negative exits1. No raw runtime transcript changed. Shared diff was recomputed
with tool/output hashes in logs/wp/renderer.safe-css.recomputed.run2.json;
its zero count is separate from the exact ten-call comparison.


## TopFrameOrigin proposal revision ready

The revised15owned network and10shared hooks/request/Mojo/converter/test
patches are published, still unapplied. Revised18owned combined transport
changes only the network broker relative to its prior effects/network/PiP
version.19network/shared/test and11combined actualflags compilerchecks exit0;
fresh review clean. Fixtures are compiled, not linked/executed. Exact native
field spelling/Mojo ordinal remain unknown; top_frame_origin is our descriptive
name for the independently proven Blink value. Browser-initiated population
policy is not invented. Details/hashes/repro in
logs/wp/renderer-network-top-origin-proposal/REVIEW.md.

One all-integration private composition is being prepared to merge the shared
Chrome startup overlap and current owned patches. App pending integration
already contains exact duplicate nativefeature/Refresh/AppFlags hunks; adoption
must deduplicate those. No actual shared/production source changed.

Retry1 acquired at11:30:40UTC; build attempt1 began11:30:43. Own frozen source hashes all still match; packaging/canonical/runtime outcomes pending.


## Canonical renderer scenario run2

Fresh11:31:37UTC fullbuild/package strictsignPASS, identityfix nowlinked.
results/renderer-surface-run2 matches136publicrows/eightclasses exactly.
results/renderer-canonical-run2 passes14CSSstates, twoexactrich extraction
trees, asyncmainonce completions andrawrequiredrowcounts. Solefunctional
scenario failure is Browser.close15stimeout thenSIGTERM/browser+7helpersexit.
Fullstandarddiff retains69differences (goldenv1nocaller/oursv2); ownentries
are themeCSS3native/5ours (extrasbeforefixture), internalextractionconstructors
2/1 andextraone-pointerconstructor. Oursconstructorsarefrom:engine; newnative
caller-aware recording willdisambiguate. Exact10fixtureCSScalls/pairedreturns
match independently in css-boundary/. These retainedcountdifferences are not
allowlisted andfullruntimeacceptance is notclaimed.

Native filterfixtures nowexist in renderer-idle-diagnostic-run2/trace156.
Observed CSSred/text grayscaleandidentity255→254, whilecanvas,PNGIMG,SVGIMG
remainred. Remove restoresbaseline. Parentinspectedactualgrayscalescreenshot;
separatevisual/nativepathreviewinprogress. This narrowscoverage toexactfixtures.


New runtime-only paired diagnostic queued11:40UTC, waiter23677/toolsession8502,
logs/wp/renderer.long-diagnostic.run2.sh. It uses240s readiness tospanstartup
3minfallback, keeps60sdefaultandnativeBEST_EFFORTunchanged, recordsperiodic
Startup.* snapshots andactualrequestdeadline. BothappsreceiveoneCOPYFILE_EXCL
frozenrecordersnapshotwithimmediatesource/hashcheck; perappartifactsstillchecked.
Threeopaque-redgeneratedgradientfixturesaddedoptionally. ObjC+Bunchecks0 and
freshread-onlyreviewclean. Nooutcomeyet; baseCSSsummaryparitydoesnotimply
filter/adblockparity. Framework/hashatactualrunwillcomefromprovenance.


Provenancecorrection: the11:31standardrenderer-public-api scenario batch at
results/renderer-canonical-run2 is SUPPLEMENTAL. Itpreserveddisposableprofile
state; officialdiff-runwrapper resetsit. Scenariosinformed136769; canonical
results/ours/renderer-public-api remainsmissing. Fixturefunctionality/rawcall
checksretainmeaning, butthisisnotcanonicalcompletion. Officialrunwillfollow
currentwarmdiagnosticorbehandledbyconfirmedscenariosintegrator, avoidingduplicate
waiters.

FullTopBand producer/browser contracts nowpersisted in renderer_contract_evidence
(mainsection1038+, browser1528+). Nativebrowserstate is DocumentUserData onroot
mainRFHdocument, notunkeyedWebContents/wholeRFHlifetime; BFCachepersistsand
sameRFHcrossdocresets. Missingview/width<1 retains pending, failedcopy cachesnil.
Separatefaithfulportproposalwillpreserve these. Arc36relay/liveupdatesremain
unproven; bothnativebasefeaturedefaultsDISABLED.
