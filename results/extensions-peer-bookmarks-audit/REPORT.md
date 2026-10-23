# Bookmark peer replay: bounded extension audit

This audits the completed peer `bookmarks-public-api` replay only. Both copied
step files contain40/40 PASS and accepted coverage metadata. Golden completion
state records `2026-10-23T13:10:10.594Z`; ours raw record mtime is
`2026-10-23T13:18:51.220838Z`. Those are existing peer results, not a new
extension-source-frozen runtime run or acceptance claim. No capture, golden,
product/helper source, app, runtime, lock or board state was changed.

## Exact first two diff lines

Official stored report (53), copied without editing:

```text
methods differing: 53 (missing 5, extra 0, returns 11, callback args 4, counts 33; internal-only 0, ours-internal 1)
callback sequence: {"golden": 44, "ours": 60, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

Private final comparator63c0055e (51), on copied public app/engine scopes:

```text
methods differing: 51 (missing 5, extra 0, returns 10, callback args 3, counts 33; internal-only 0, ours-internal 1)
callback sequence: {"golden": 44, "ours": 60, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

The comparator itself truncates the callback report text line at600 characters;
these are its exact lines, not reconstructed JSON. Exact text also lives in
`official-first-two-lines.txt` and `private-first-two-lines.txt`. Structured
callback reports remain in the corresponding diff JSON.

The private comparator is SHA256
`63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08`,
copied with its policy/metadata dependencies from the validation7 private tool
tree. No fixture-image admission was supplied. Both raw inputs are fully caller
attributed: golden5035/5035 and ours5229/5229 boundary rows. The81 driver-origin
calls per run remain outside the selected comparison; their existing raw gate
separately records74 bookmark calls,81 paired driver returns and a genuine
creation callback. This audit does not broaden their admission.

The current shared comparator snapshot is SHA256
`f8df579e6ea23b4eb531f357fdfe0460fa57b563daf44f9d6858c73abf84b184`.
Running this copied comparator on the copied inputs reproduces the stored
**entire official JSON and both first lines exactly**. This establishes a
reproducible policy comparison; the original historical generation command is
not independently recorded in the stored report.

Private versus official changes exactly two mismatch entries: the return of
`ArcSessionID identifier`, and args[1] of
`browser:extensionGroupIdForTabWithSessionId:`. All other report entries,
all33 count mismatches and the global callback sequence44/60 are unchanged.
The private51 total remains a global nonzero diff, not a passing scenario.

## Extension selector counts and observed values

Selected ownership covers the six extension classes, package-owned ArcBrowser
selectors, extension/lifecycle/error/pin/toolbar/popup and tab-group callbacks.
There are8/8 selected owned calls, across exactly these four selectors:

| Selector | Golden / ours calls | Observed values |
|---|---:|---|
| `ArcBrowserContextExtensions listExtensions` |3 /3| Every argument and return is `[]`. |
| `ArcBrowserContextExtensions setDelegate:` |1 /1| Returns null; raw targets are `<ADKClientLive.BrowserExtensionsLifeCycleManager#94>` / `#81`. |
| `browser:didUpdateExtensionToolbarActions:` |1 /1| Empty action array, null return. |
| `browser:extensionGroupIdForTabWithSessionId:` |3 /3| Every callback returns `-1`; positive SessionID1720849009 /2040340652. |

Total owned extension callbacks are **4/4**: one toolbar update plus **3/3** group
lookups. Callback-only method order is exactly toolbar, group, group, group.
The sole official extension-scope residual is the numeric group callback input;
the private extension-scope report has no missing, extra, return, argument or
count entries. SessionID normalization explains that narrow result.

Full selected owned call order is **different**, despite equal per-method counts:

```text
golden: setDelegate, list, toolbar, list, list, group, group, group
ours:   setDelegate, list, toolbar, list, group, group, group, list
```

Golden's third empty list demand is seq2956, before group callbacks3584/3590/3592.
Ours' third empty list demand is seq4080, after group callbacks3782/3876/3900.
Per-method comparison and callback-only sequence checking do not establish
cross-method app-demand ordering parity. Exact raw calls are preserved in
`extension-call-order.json` and `extension-raw-scope.json`.

The dotted delegate class label is not matched by the comparator's object regex;
it remains#94 versus#81 in the method inventory. The comparator does not compare
`->engine` argument mismatches. Thus the entire normalized method inventory is
correctly marked unequal; no raw-value equality claim hides this difference.
Browser object handles are renumbered by the existing comparator policy;
raw toolbar/group callback browser handles are#86 versus#76.

Raw extension-class calls are17 golden versus6 ours, before caller filtering:
golden includes12 engine-origin ArcExtensionInfo copy initializers and one native
context initializer; ours includes two engine-origin context initializers. The
selected four public context calls per run remain identical in count/return
shape. There are no app-origin Info/Command/Error/Frame/ToolbarAction getter
calls, extension browser actions, positive toolbar-action payloads, lifecycle,
error, pin setter or popup events here. These unexercised areas are not verified
by the empty-list/empty-toolbar observation.

## SessionID scope

Each run has exactly one positive32-bit native ID. Golden1720849009 appears in
all three paired `ArcSessionID identifier` returns and all three group callback
args[1]. Ours2040340652 has the same six-occurrence shape. Pairing has no pending
identifier calls or unpaired returns. The copied comparator consistently maps
each run's sole ID to native_session_id1 across both exact fields. Group lookup
return `-1`, counts and order are not normalized away. No other integer or
selector is added to that policy. `session-id-evidence.json` retains each raw
value, sequence, receiver and caller.

## Provenance and compiler-policy limits

The run provenance hashes authenticate ancestor combined-profile fixture files:
`probe.mm`, `scenario.ts` and `coverage.py`. The copied current canonical
`bookmarks-public-api.ts` instead compiles
`tests/scenarios/bookmarks-public-api-driver.m`, while intentionally writing
those ancestor references. The recorded provenance therefore does **not** pin
the exact canonical helper/scenario sources historically compiled for this run.

Both run compile logs are empty and their steps record strict-warning compilation
passing. Current canonical source observation shows
`xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror`, Foundation/AppKit and the
canonical driver source. That source observation, its raw gate source and both
run helper binaries were copied and hashed. Exact historical compiler identity
and argv were not recorded. The ancestor build.py/compile.log are frozen as
ancestor metadata and are not attributed to the canonical replay.

No captured provenance freezes the extension-owned TUs, their build commands or
the engine framework/source epoch. Do not infer that this peer replay used the
same binaries/source as extension validation9/10. This audit is a peer observation
of emitted empty extension state and narrow SessionID correspondence. Renderer
canonical v2 ours remains absent/unreplayed; this task neither runs nor promotes
that scenario. Existing accepted goldens were read and never promoted/replaced.

## Frozen inputs, integrity and review

Raw golden SHA256: `78eeab6de976e7822f6262216ebd63ee2035bb7e8969bdc03c58b542c7e69cef`.
Raw ours SHA256: `481bb697bf61dc6547bd3aace0ccc57ea5fd8c2b0d105e1b66ec7e6bdd75cfec`.

Exclusive directory/capture and supplement guards prevent recapture into this
result. `provenance.json`, `compiler-policy-supplement.json` and
`input-verification.json` record hashes, sizes and source timestamps, with source
and copy checks before/after. Final verification also covers the three prior
profile proposal artifacts, whose final hashes remain unchanged. Shared policy,
original diff, metadata, source observations and logs are copied into this private
tree. All comparator output is new private diagnostic output.

Fresh read-only review independently verified the original36 copied input hashes,
the eight supplementary hashes, both policy outputs, SessionID correspondence,
selector counts, callback-only order and differing full selected call order.
It found no concrete audit defect and confirmed the ancestor-source qualification.
No reviewer edits or runtime checks were performed. No new engine/spec
contradiction is established; missing provenance and uncovered behavior remain
explicit limits.
