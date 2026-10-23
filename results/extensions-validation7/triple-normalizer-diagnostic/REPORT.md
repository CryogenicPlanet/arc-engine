# Validation7 diagnostic with the exact triple comparator

**Private recomparison only.** No new runtime, product improvement or acceptance
claim. Official validation7 results remain **116 / 57 / 54**. This diagnostic
compares only the six unchanged validation7 raw snapshots copied into this
exclusive new directory, including validation7's already-copied v2 page golden.
It does not use later shared goldens, product changes or the helper flag fix
made after validation7.

The private comparator is exactly
`63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08`.
Its proposed allowlist, class metadata and callback table are snapshotted under
`tool/`; actual shared-tool/allowlist/metadata/table bytes are independently
snapshotted under `actual-shared-tool-snapshot/`. All 22 primary source/copy
hashes were reverified after comparison. No fixture image is opted in, no caller
label is rewritten, no raw input is edited, and shared tools are unchanged.

All three pairs have caller attribution on every call and return. The private
root supplies the exact metadata/callback inputs used by `boundary_methods`.
`capture.py` uses an exclusive guard before reading/copying inputs, and the
initial directory creation used `mkdir(exist_ok=False)`. A rerun refused before
changing any existing capture file (`rerun-refusal.txt`).

## Exact global first two lines

The callback lines below are copied exactly, including truncation performed by
the comparator itself. Full JSON callback reports remain in each `diff.json`.

### launch-slice/launch-1

Official methods differing: **116**. Private diagnostic:

```text
methods differing: 114 (missing 5, extra 3, returns 13, callback args 3, counts 90; internal-only 0, ours-internal 1)
callback sequence: {"golden": 106, "ours": 126, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

Attributed call/return rows: 7409 golden / 6863 ours.

### launch-slice/launch-2

Official methods differing: **57**. Private diagnostic:

```text
methods differing: 55 (missing 6, extra 0, returns 11, callback args 3, counts 35; internal-only 0, ours-internal 1)
callback sequence: {"golden": 47, "ours": 62, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

Attributed call/return rows: 4343 golden / 4613 ours.

### extensions-page-actions/launch-1

Official methods differing: **54**. Private diagnostic:

```text
methods differing: 52 (missing 5, extra 2, returns 10, callback args 2, counts 33; internal-only 0, ours-internal 1)
callback sequence: {"golden": 70, "ours": 84, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

Attributed call/return rows: 5535 golden / 5993 ours.

## What the comparator changed and retained

Private totals are **114 / 55 / 52**. `policy-delta.json` checks every report
category against the copied official JSON. On each pair, the only removed
entries are the exact `ArcSessionID.identifier` return mismatch and
`browser:extensionGroupIdForTabWithSessionId:` argument mismatch. The shared
per-transcript SessionID aliases preserve distinct/repeated identities; no
count, method, return sentinel or callback-order difference is erased.

Every count-mismatch list and the entire global callback-sequence report is
identical to the official report. Counts remain 90 / 35 / 33 count-mismatch rows, with
callbacks 106/126, 47/62 and 70/84, respectively; first divergence remains index 1
for all three. No epoch-only mismatch is removed in these captures. Fixture
admission is not enabled, so this is not injected-fixture method coverage.

## Extension-owned residuals after aliases

All report categories, including internal classification, were inspected.
`extension-owned-report.json`, method inventories, raw class audits, complete
owned callback method sequences and SessionID traces are retained per pair.

| Pair | Owned residuals |
|---|---|
| Launch 1 | Group-ID callback count **13 golden / 10 ours** only. Core-owned SessionID getter count 8/5 also remains globally. |
| Launch 2 | No owned report residual. This does not make the global scenario clean. |
| Page | Two toolbar value rows and eleven count rows, detailed below. |

Launch 1 still has 14/11 owned callbacks with different filtered method order;
native retains three final group callbacks for alias 3 while ours has no such
tail. The first ten callback aliases agree and all group callback returns are
-1. Native getter aliases are `[1,1,1,1,2,3,3,3]`, ours `[1,1,1,1,2]`.
Launch 2 has 4/4 owned callbacks with equal filtered method order and matching
alias 1 observations. Page has 13/14 owned callbacks and unequal filtered method
order. Aliases do not establish native tab routing, callback timing or a cause
for the missing launch1 tail.

### Page values and downstream enumeration

Two value differences remain literal:

- `ArcExtensionToolbarAction.pinnedPosition`: native null versus ours 0 at the
  first compared read. Raw audit also confirms native null versus ours 0 at the
  second read; the comparator reports only the first divergent return per method.
- `ArcExtensionToolbarAction.tooltipText`, second read: native
  `ArcCore fixture action\nHas access to this site` versus ours
  `ArcCore fixture action\nAllowed to read & change this site`.
  The corresponding accessibleName values agree on the legacy access suffix.

The eleven owned count rows are listExtensions 6/7; nine Info getter selectors
(enabled, fromWebstore, hasAction, icon, name, permissions, url and version 4/5,
id 8/10); and didChangePinnedExtensionOrder 1/2. No new Info field-value mismatch
was found: **all emitted getter values of all four native and five ours Info
objects match exactly**, including fixture ID, name, URL, permission strings,
image(32x32), enabled/hasAction/fromWebstore and version. This compares recorder
values, not image pixels or uncalled DTO fields.

The raw extra ours pin callback is seq 3252–3319, with the actual fixture ID
`nokhgadimgeidcniagoklokbibfpogjb`; native's only pin callback 2926–2991 and ours'
first 3116–3183 both carry empty arrays. The extra filled callback encloses one
additional listExtensions call at seq 3295 and ten Info getter return rows at
depth 2. `page-state-audit.json` retains the full callback/list/getter rows and
exact canonical values. Thus the extra Info/list demand is concretely linked
to the additional pin notification and one identical fixture snapshot, rather
than a new DTO producer value difference. The extra callback and its count/order
consequences remain in the reports; none is normalized away.

The known feature-configuration explanation remains supported by primary source:
ToolbarActionsModel's install path gates default pinning on the real feature,
preference and policy, then calls SetActionVisibility; ExtensionActionViewModel's
GetTooltip chooses the newer menu string only on its access-control feature
branch. The existing feature-state audit identifies generated Mac testing-config
overrides for ExtensionsPinnedByDefault and ExtensionsMenuAccessControl while
native defaults are disabled. Exact copies/hashes of that audit and both source
gates are retained under `feature-audit-sources/`. These raw values and nesting
are consistent with that pending configuration integration. **This diagnostic
alone does not isolate feature state as the exclusive cause**: it performs no
new disabled-feature run and does not replace the pending paired verification.
No additional extension-owned value defect is demonstrated by the new page
capture, and no count difference is dismissed as harmless.

## Limits and retained evidence

The existing comparator does not compare arguments on calls into the engine;
those arguments remain visible in the saved inventories. Object-token/string
normalization is unchanged except the selected exact SessionID aliases; no
normalization of pinnedPosition, tooltip text, pin arrays or Info contents is
introduced. Empty surfaces and unrecorded methods establish no coverage for
commands, native-icon pixels, popup lifecycle or asynchronous group operations.
The supplemental service probe is separate evidence and was not run here.

`provenance.json`, `input-verification.json`, `commands.json`, copied raw data,
first-two-lines files, global/owned reports, selection inventories, SessionID
traces, page state audit and policy delta preserve the reproduction inputs.
The later product/helper edits do not change these historical raw observations.
No product/shared/helper/runtime artifact, app, build, lock or board was changed.

Fresh independent read-only review approved all hashes, exact first-two-line
blocks, report policy delta, owned residuals and raw page nesting with no
blockers. Runtime and canonical acceptance remain unverified.
