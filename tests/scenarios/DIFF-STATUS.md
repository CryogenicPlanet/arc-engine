# Scenarios engine differential validation

Snapshot refreshed: **2026-10-23 09:52:42 UTC**. This update ran exactly one fresh `arc/tools/diff-run.sh tabs-navigation`; no other builds, launches, or golden retries were initiated by this worker. Only this report was edited.

## Latest requested validation attempt

Command: `arc/tools/diff-run.sh tabs-navigation`; **exit 1 before packaging or scenario launch**. The build wrapper exhausted its automatic attempts; final failure reported at **2026-10-23 09:51:47 UTC**:

```
../../arc/api/arc_web_contents.mm:450:21: error: unused variable 'wrapper' [-Werror,-Wunused-variable]
```

This is outside the scenarios package. No retry was made. Existing tabs-navigation artifacts below predate this attempt and were not produced by it. Historical build errors elsewhere in this report are retained as observations of earlier tree states.

## Native automation environment

Parent verified that foreground PID 173 is macOS `loginwindow` and that AXWindows reports AXApplication roles in the locked desktop state; earlier accepted golden captures exposed true AXWindow roles. This blocks fresh native action validation and can explain missing native controls in current shared runs. Parent canceled waiting golden batch PID 33563 before it acquired the lock, preserving the 18 accepted goldens. Coordination evidence: Chirp message **134458**. This worker did not unlock the desktop, retry native goldens, or modify another job's lock lifetime.

## Current accepted canonical goldens

18 canonical goldens have `coverage.json` with `accepted=true` at this snapshot. Command exit, available transcript diffs, and scenario acceptance are separate evidence.

| Accepted scenario | Current shared artifacts |
| --- | --- |
| bookmarks-import | No transcript diff or steps available |
| command-bar-autocomplete | 1 launch diff(s); steps pass=1, fail=1, inconclusive=1 |
| context-menus | No transcript diff or steps available |
| cookies-storage-site-settings | No transcript diff or steps available |
| devtools | 2 launch diff(s); steps pass=2, fail=2, inconclusive=2 |
| dialogs | No transcript diff or steps available |
| dialogs-auth-cancel | No transcript diff or steps available |
| dialogs-beforeunload | No transcript diff or steps available |
| downloads | No transcript diff or steps available |
| fullscreen | No transcript diff or steps available |
| launch-slice-regression | No transcript diff or steps available |
| media | No transcript diff or steps available |
| media-pip | No transcript diff or steps available |
| native-action-isolation | No transcript diff or steps available |
| pdf | No transcript diff or steps available |
| permissions-deny | No transcript diff or steps available |
| tabs-navigation | 1 launch diff(s); steps pass=0, fail=1, inconclusive=1 |
| windows | No transcript diff or steps available |

## All current scenario diff artifacts

Producer and command exit status are unknown/shared for these existing results. The requested latest build did not replace them. Every current `results/ours/*/diff-launch-*.txt` is included below, including the default launch-slice scenario, with its exact first two lines and current steps.

### command-bar-autocomplete — shared producer unknown

Steps source: `results/ours/command-bar-autocomplete/steps.json`; modified 2026-10-23 09:50:23 UTC.

- launch: **pass** — native ready, CDP target list available
- scenario actions: **inconclusive** — Error: command bar field not exposed
- quit: **fail** — SIGTERM (Browser.close did not exit in 15 s); browser + 7 helpers exited

`results/ours/command-bar-autocomplete/diff-launch-1.txt`; modified 2026-10-23 09:50:23 UTC. Exact first two lines:

```
methods differing: 125 (missing 35, extra 32, returns 13, callback args 1, counts 44; internal-only 12)
callback sequence: {"golden": 47, "ours": 74, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

### devtools — shared producer unknown

Steps source: `results/ours/devtools/steps.json`; modified 2026-10-23 09:46:29 UTC.

- launch: **pass** — native ready, CDP target list available
- scenario actions: **inconclusive** — Error: expected one native control; observed 0
- quit: **fail** — SIGTERM (Browser.close did not exit in 15 s); browser + 7 helpers exited
- launch: **pass** — native ready, CDP target list available
- scenario actions: **inconclusive** — Error: expected one native control; observed 0
- quit: **fail** — SIGTERM (Browser.close did not exit in 15 s); browser + 6 helpers exited

`results/ours/devtools/diff-launch-1.txt`; modified 2026-10-23 09:46:29 UTC. Exact first two lines:

```
methods differing: 133 (missing 30, extra 32, returns 15, callback args 1, counts 55; internal-only 11)
callback sequence: {"golden": 56, "ours": 1032, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

`results/ours/devtools/diff-launch-2.txt`; modified 2026-10-23 09:46:29 UTC. Exact first two lines:

```
methods differing: 144 (missing 33, extra 31, returns 19, callback args 1, counts 60; internal-only 10)
callback sequence: {"golden": 58, "ours": 1017, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

### launch-slice — shared producer unknown

Steps source: `results/ours/launch-slice/steps.json`; modified 2026-10-23 09:44:24 UTC.

- S1 launch: **pass** — native ready, CDP target list available
- S2 open A: **pass** — title=launch-slice A
- S3 navigate: **pass** — B=true (71 ms) frag=true back=true title=launch-slice B href=http://127.0.0.1:60161/b
- S4 store: **pass** — ok
- S5 close + new tab: **pass**
- S6 quit: **fail** — SIGTERM (Browser.close did not exit in 15 s); browser + 8 helpers exited
- S7 relaunch: **pass** — native ready, CDP target list available
- S7 readback: **pass** — {"cookie":"ls_cookie=c0ffee","local":"v-41","idb":"idb-93"}
- S8 quit: **fail** — SIGTERM (Browser.close did not exit in 15 s); browser + 6 helpers exited

`results/ours/launch-slice/diff-launch-1.txt`; modified 2026-10-23 09:44:25 UTC. Exact first two lines:

```
methods differing: 172 (missing 21, extra 36, returns 17, callback args 1, counts 97; internal-only 10)
callback sequence: {"golden": 106, "ours": 3598, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObse
```

`results/ours/launch-slice/diff-launch-2.txt`; modified 2026-10-23 09:44:25 UTC. Exact first two lines:

```
methods differing: 95 (missing 22, extra 33, returns 16, callback args 1, counts 23; internal-only 10)
callback sequence: {"golden": 46, "ours": 1034, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

### tabs-navigation — shared producer unknown

Steps source: `results/ours/tabs-navigation/steps.json`; modified 2026-10-23 09:42:19 UTC.

- T1 launch: **inconclusive** — no readiness record for this pid within 60 s
- T11 quit: **fail** — SIGTERM (Browser.close failed); browser + 0 helpers exited

`results/ours/tabs-navigation/diff-launch-1.txt`; modified 2026-10-23 09:42:19 UTC. Exact first two lines:

```
methods differing: 190 (missing 139, extra 21, returns 6, callback args 1, counts 23; internal-only 19)
callback sequence: {"golden": 163, "ours": 88, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserv
```

Startup failure in `results/ours/tabs-navigation/launch-1/app.log`:

```
[22819:3539750:1023/024124.518425:FATAL:base/observer_list.h:192] Check failed: !check_reentrancy || reentrancy != ObserverListReentrancyPolicy::kDisallowReentrancy || list_.IsOnlyRemainingNode().
```

## Historical attempts owned by this worker

The following observations were captured from this worker's original commands. Their underlying tool-generated output paths may now contain newer shared results.

## tabs-navigation

Command: `arc/tools/diff-run.sh tabs-navigation`; exit: 0.

Steps: T1 launch=pass, T2 three tabs=pass, T3 document navigation=pass, T4 back=pass, T5 forward=pass, T6 reload=pass, T7 same document=pass, T8 redirect=pass, T9 close exact tab=pass, T10 open replacement tab=pass, T11 quit=fail.

Failure: SIGTERM (Browser.close did not exit in 15 s); browser + 8 helpers exited

results/ours/tabs-navigation/diff-launch-1.txt

```
methods differing: 191 (missing 61, extra 39, returns 28, callback args 5, counts 58; internal-only 16)
callback sequence: {"golden": 163, "ours": 33, "first_divergence": 0, "golden_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.ADKApplication didConnectToNetworkProcess", "ADK.BrowserContextDelegateBridge browserContextPreferencesDidUpdateSearchEngines:", "ADK.ChromiumBrowser browser:didCreateNewWebContents:isActiv
```

Engine log: `ArcCore unimplemented: -[ArcBrowserApplication tryToTerminate:]` immediately after the scenario actions.

## windows

Attempt 1: `arc/tools/diff-run.sh windows`; exit: 1. No scenario launch or transcript diff.

Historical external build blocker: GN rejects `//arc:arc_core` depending on `//extensions/browser:browser_sources`; that target is visible only to `//extensions/browser/*`. Source: `logs/gn-gen.log`. The navigation compile errors printed by diff-run were aggregated older log entries, not this attempt's failing stage.

Attempt 2 after a few minutes: exit 1 before launch. GN visibility failure resolved. Build attempt 1 failed in external `arc_webextras_media.mm:274` (missing explicit property synthesis) and `:299` (`ui::Layer` abstract/no `SetColor`); automatic build attempt 2 failed in external `arc_sessions_restore.mm:347` (copy of noncopyable `base::ScopedObservation`). No windows steps or diff exists.

## downloads

Attempt 1: `arc/tools/diff-run.sh downloads`; exit: 1 before launch. No scenario steps or diff. External GN visibility blocker: `//arc:arc_core` cannot depend on `//ui/accessibility/platform:platform`. Allowed visibility lists Chrome accessibility targets, `//ui/accessibility:accessibility`, `//ui/views:views`. Source: `logs/gn-gen.log`.

Attempt 2 after delay: exit 1 before launch. GN visibility failure resolved, but the build wrapper exhausted its automatic attempts; the final compile failed in external `arc/api/arc_browser.mm:146:22`: no visible `@interface` for `ArcBrowser` declares selector `isInitialized`. No downloads steps or transcript diff exists. Stopped further build attempts as instructed after repeated outside-package build failures.

## Outcome and remaining coverage

Current shared launch-slice steps establish page navigation/storage/readback success, followed by failed graceful shutdown on both launches. Current devtools and command-bar-autocomplete launches pass readiness but native actions remain inconclusive; graceful shutdown also fails. Current tabs-navigation artifacts show an older startup reentrancy crash and no scenario actions. None of these diffs or nonpassing steps establishes engine parity.

Other accepted scenarios currently have no comparison artifacts as listed above. downloads-prompt has no accepted canonical golden at this snapshot. No further builds were attempted after the latest outside-package compile failure. No real Arc data or VMs were touched.
