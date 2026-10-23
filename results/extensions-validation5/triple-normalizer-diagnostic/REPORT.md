# Historical validation5 recomparison with the triple comparator

No new runtime was executed. This compares current attributed launch-slice goldens with unchanged validation5 recordings. It establishes neither product improvement nor package acceptance. Only this previously absent diagnostic directory was created; no app, build, lock, board, product source, helper or shared comparator was used for mutation.

The copied comparator SHA256 is `63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08`. Its tool root includes exact snapshots of the finalized proposal allowlist, runtime metadata and callback table. `provenance.json` gives source/copy SHA256 for every input; all were reverified after comparison. No fixture-image flag is used. Both launch-slice sides have caller attribution on every call/return. The exact raw inputs are copied beside each diff.

## Global results, separate from extension ownership

### launch-slice/launch-1

Exact first two CLI diff lines (the callback line is truncated by the comparator itself):

```text
methods differing: 123 (missing 5, extra 5, returns 13, callback args 2, counts 98; internal-only 0, ours-internal 1)
callback sequence: {"golden": 106, "ours": 128, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

Global report categories: {"arg_mismatch": 2, "count_mismatch": 98, "extra": 5, "internal": 0, "internal_ours": 1, "missing": 5, "return_mismatch": 13}.
Global callbacks: 106 golden / 128 ours; first divergence index 1.

### launch-slice/launch-2

Exact first two CLI diff lines (the callback line is truncated by the comparator itself):

```text
methods differing: 56 (missing 6, extra 2, returns 14, callback args 2, counts 32; internal-only 0, ours-internal 1)
callback sequence: {"golden": 47, "ours": 63, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

Global report categories: {"arg_mismatch": 2, "count_mismatch": 32, "extra": 2, "internal": 0, "internal_ours": 1, "missing": 6, "return_mismatch": 14}.
Global callbacks: 47 golden / 63 ours; first divergence index 1.

## Extension-owned observations

Only four extension-owned methods/callbacks are recorded after caller selection in either launch. `extension-owned-method-inventory.json` retains every normalized occurrence, while `extension-owned-callback-sequences.json` retains raw callback call arguments and sequence positions. All report sections, including internal classification, were inspected.

| Method | Launch 1 golden/ours | Launch 2 golden/ours | Observed result |
|---|---:|---:|---|
| `-[ArcBrowserContextExtensions listExtensions]` | 3/3 | 3/3 | All returns are empty arrays. |
| `-[ArcBrowserContextExtensions setDelegate:]` | 1/1 | 1/1 | All returns are null; engine-bound arguments have the limitation below. |
| `browser:didUpdateExtensionToolbarActions:` | 1/1 | 1/1 | Empty action arrays, same normalized browser argument, null returns. |
| `browser:extensionGroupIdForTabWithSessionId:` | **13/10** | 3/3 | Every callback returns -1; common normalized arguments match. Launch 1 retains a count difference. |

The sole extension-owned public report residual is launch 1 `engine->app -ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:` count **13 golden / 10 ours**. There are no other extension-owned missing, extra, return, callback-argument, count or internal-only entries. Launch 1 has 14/11 extension callbacks in total; their filtered method sequence differs because three group-ID callbacks are absent. Launch 2 has 4/4 extension callbacks with matching filtered method order. The full global callback sequences still diverge in both launches.

The raw extension-class audit also inspected excluded rows. In each launch native records twelve engine-only `ArcExtensionInfo initWithExtensionInfo:` calls while ours records none. Both record one engine-only context native-pointer factory; ours additionally records one engine-only local `initWithArcBrowserContext:` helper. These are excluded by the unchanged caller policy, not removed by the SessionID or epoch normalization. `extension-class-raw-audit.json` retains their complete raw rows/callers.

Unreported argument limitation: `setDelegate:` supplies lifecycle-manager recorder labels `#97` golden versus `#78` ours in launch 1, and `#90` versus `#78` in launch 2. The existing comparator does not compare arguments of calls into the engine, and its object-token regex does not renumber those dotted class labels. These labels are visible in the saved inventory; no delegate-identity equivalence claim follows from the empty report row. This task does not change that existing policy.

No lifecycle callback, permission prompt, installed-extension DTO/action, pin/unpin/move/action invocation, popup open/resize/close, command/omnibox action, or asynchronous tabGroups get/create/query/update/move/ungroup delegate request is recorded in the selected launch-slice extension scope. Empty startup surfaces do not establish those behaviors. Core-owned accessors/SessionID/TabGroup DTOs are separate from extension-owned methods.

## SessionID counterexample retained by the comparison

`session-id-evidence.json` records every selected target scalar, raw sequence position and the shared per-transcript alias. In launch 1, native getter IDs map to `[1,1,1,1,2,3,3,3]`, versus `[1,1,1,1,2]` in ours. Thus the core-owned `-[ArcSessionID identifier]` retains a separate **8/5 count mismatch**, and native records three distinct selected IDs while ours records two.

The first ten group-ID callback arguments use alias 1 on both sides, with return -1. Native then records three callbacks using alias 3, raw ID `1064772388`, at raw seq **6274, 6284, 6404**; ours has no corresponding tail. Native alias-3 getter returns occur at seq **5968, 5972, 6026**. Ours reaches only IDs `69453245` (alias 1) and `69453246` (alias 2), with its final selected getter at seq **5676**. These missing occurrences remain count differences. The comparator zips common calls, so the absent alias-3 tail does not generate an additional callback-argument or return mismatch. No alias collapse/split appears in the paired prefix; no cause for the missing tail is inferred from these recordings.

Launch 2 records one distinct target ID per side: native `1064772440` versus ours `69453300`. Both have three getter returns and three group-ID callbacks, all alias 1; callback returns are -1. This is numerical alias agreement within this historical recording, not proof of native tab routing or timing.

## Page-actions coverage limit

NOT COMPARED: current page-actions golden is v1-or-mixed; v1 fallback cannot provide paired caller-attributed coverage.

Current golden: 5597 boundary rows, 0 with caller attribution. Validation5 ours: 5281 boundary rows, all attributed. Both are snapshotted with provenance, but no page-actions diff was run. Falling back to v1 would disable the paired caller-selection policy; this diagnostic does not claim equivalent coverage.

## Reproduction and retained evidence

`capture.py` records source snapshots and the comparison command. It refuses to overwrite an existing tool root or provenance file. Initial directory creation used `mkdir(exist_ok=False)`; an existing diagnostic destination would have been refused. `diff.txt`, `diff.json`, `first-two-lines.txt`, the ownership inventory/callback sequence, and SessionID evidence are retained per launch. The source recordings, finalized tool inputs and copied inputs all hash-check unchanged.

No shared-file request or architecture/spec contradiction was established. The lead can inspect the launch-1 count mismatch alongside the separate core tab/session differences; these recordings alone do not identify its implementation cause.

Fresh independent read-only review found no defects, verified all ten source/copy hashes and confirmed that the only changes from the previous attributed diagnostic are the intended SessionID return and group-callback argument entries (`fresh-review.md`). This is a comparison-policy change on historical recordings, not a runtime improvement.
