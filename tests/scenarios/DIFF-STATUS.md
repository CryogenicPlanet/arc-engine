# Scenarios engine differential validation

Each scenario was attempted once through arc/tools/diff-run.sh; command exit alone is not acceptance. Steps and transcript differences are reported separately.

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

Current external build blocker: GN rejects `//arc:arc_core` depending on `//extensions/browser:browser_sources`; that target is visible only to `//extensions/browser/*`. Source: `logs/gn-gen.log`. The navigation compile errors printed by diff-run were aggregated older log entries, not this attempt's failing stage. Retry pending after a few minutes.

Attempt 2 after a few minutes: exit 1 before launch. GN visibility failure resolved. Build attempt 1 failed in external `arc_webextras_media.mm:274` (missing explicit property synthesis) and `:299` (`ui::Layer` abstract/no `SetColor`); automatic build attempt 2 failed in external `arc_sessions_restore.mm:347` (copy of noncopyable `base::ScopedObservation`). No windows steps or diff exists.

## downloads

Attempt 1: `arc/tools/diff-run.sh downloads`; exit: 1 before launch. No scenario steps or diff. External GN visibility blocker: `//arc:arc_core` cannot depend on `//ui/accessibility/platform:platform`. Allowed visibility lists Chrome accessibility targets, `//ui/accessibility:accessibility`, `//ui/views:views`. Source: `logs/gn-gen.log`. Retry pending after delay.
