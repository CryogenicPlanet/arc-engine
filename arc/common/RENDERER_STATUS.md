# Renderer work package status, 2026-10-23 09:01 UTC

Blocked; acceptance is not met. No commits or pushes.

## Implemented

- `arc/api/arc_renderer_values.mm`, `arc/common/renderer_values.h`: ArcHSLAMatrix initializer, aggregate getter/setter and twenty float getters; ArcMetaTagEntry and ArcMetaTagSelector constructors/getters; ArcHtmlToMarkdownResult constructor and all getters. Preserve copied input and nil values.
- `arc/api/arc_renderer_effects.mm`: weak profile ownership, native host zoom registration using log(factor)/log(1.2), default restoration on unregister. Other Boost operations remain logging stubs pending verified renderer semantics.
- `arc/api/arc_renderer_web_contents.mm`: ArcWebContents stylesheet insertion/removal categories. Native-owned associated remote follows the primary main frame and resets on frame deletion/process loss. Core owns blockedResourceCount and top-band delegate storage, so these are not duplicated.
- `arc/common/renderer.mojom`, `arc/common/BUILD.gn`: associated frame IPC.
- `arc/renderer/render_frame_agent.{h,cc}`, `arc/renderer/BUILD.gn`: frame observer, Blink injection/removal and keyed replacement. Fresh review caught that Blink appends even with a reused key; fixed by explicitly removing the old sheet first.
- `arc/wp/renderer.gni`: sources/dependencies.
- `arc/renderer/value_probe.mm`: cold-load ABI/copy probe; no profile creation or app launch.

## Verification

All three Objective-C implementation files pass syntax checks using real arc_core flags; renderer C++ passes its real target flags. Full combined build passed 09:00:26 UTC. Original and ours cold-load probes pass the same matrix/aggregate/all-float-getters/copy/nil tests; all 36 public method encodings match exactly. A final property-attribute adjustment (manual readonly getters to omit synthetic V attributes) is syntax-checked but occurred after that link; next shared build must pick it up before property parity is asserted.

## Boundary diff: launch-slice

Completed renderer diff-run at 09:01:29 UTC. Exact first two lines follow (the diff tool itself truncates the callback line):

### diff-launch-1.txt

```text
methods differing: 213 (missing 68, extra 27, returns 22, callback args 0, counts 96; internal-only 18)
callback sequence: {"golden": 106, "ours": 4, "first_divergence": 0, "golden_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.ADKApplication didConnectToNetworkProcess", "ADK.BrowserContextDelegateBridge browserContextPreferencesDidUpdateSearchEngines:", "ADK.ChromiumBrowser browser:didCreateNewWebContents:isActive
```

### diff-launch-2.txt

```text
methods differing: 162 (missing 58, extra 27, returns 18, callback args 0, counts 59; internal-only 17)
callback sequence: {"golden": 46, "ours": 4, "first_divergence": 0, "golden_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.ADKApplication didConnectToNetworkProcess", "ADK.BrowserContextDelegateBridge browserContextPreferencesDidUpdateSearchEngines:", "ADK.ChromiumBrowser browser:didCreateNewWebContents:isActive:
```

Renderer differences: launch 1 stylesheet calls 8 golden / 5 ours, top-band setter 2/1, blocked-count setters 2/1 and getters 4/2; the second content path was not reached after agent-cdp ECONNRESET. Launch 2 stylesheet calls 3/3, delegate setter 1/1, count setters 1/1 and getters 2/2. All observed blocked counts return zero. setExcludedDomains: is missing on ours (1/0 each launch). No sampled-color callbacks, Boost registration, matrices, zoom, block lists or extraction calls occur in these goldens. These observations do not establish actual CSS rendering: the Chromium hook is not yet present.

Harness outcomes: launch, fixture A, navigation and initial storage pass; second content inconclusive ECONNRESET; both quits fail and require SIGTERM; relaunch localStorage readback is null. Global callback/order differences remain extensive and are outside the renderer subset.

## Requests to lead

1. Apply exact gated renderer registration/build dependency/child-switch forwarding in `arc/common/RENDERER_INTEGRATION.md` (Chirp request #132932). Current Chromium source has no hook, so renderer IPC cannot execute.
2. Scenarios package: add accepted Boost CSS/JS register/update/remove, ad-blocking and extraction success/failure transcripts. Existing native automation cannot currently reach those features reliably (#132983).
3. Recover extraction trampoline 0x43f9e60 and node enums/redaction/cross-origin/time contract; ad-block list payload format/debug/count semantics; Boost host grammar, script world/timing, color scheme/filter semantics; top-band sampling/crossfade. These remain unimplemented rather than guessed.
4. Rebuild once after the final readonly-property metadata adjustment, then rerun value_probe against original and ours and compare PROPERTY rows as well as method rows.

## Contradictions and limits

- Renderer brief says insertStylesheet:, but metadata/golden require insertStylesheet:stylesheet: (identifier, CSS).
- Specs show traps for absent native optionals; architecture requires tolerating native destruction. The weak-owner zoom path follows architecture and returns when its profile is gone.
- Method/callback contract gaps in the specs are not permission to substitute an arbitrary DOM tree or an inferred extraction block signature. No such producer was implemented.
- Current stylesheet transport addresses the primary main frame, drops calls before a live frame and clears sheet identifiers on new documents. Frame scope/navigation/timing parity needs original-engine scenarios.

Coordination used the Chirp skill at `/Users/cryogenicplanet/.agents/skills/chirp/SKILL.md`; ownership and findings posted on projects/arc (#132884, #132932, #132964, #132969, #133017). No other external messages.
