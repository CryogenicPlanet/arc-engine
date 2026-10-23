# Current core run: bounded session audit

The archived core run reports launch-slice and tabs-navigation functional PASS, fresh build/package completion, and stable sealed inputs. It does **not** establish canonical sessions acceptance or a new session restore/discard behavior. No new actionable owned implementation bug is supported by this collection; the previously observed extra global close boundary remains.

## Inputs and packaging

Run: `results/core-runtime/20261023T142843Z-window-binding/`.

- `arc-build-inputs-before.json` and `arc-build-inputs-after.json` seal all 120 Arc inputs identically; `arc-build-inputs.changed.json` is empty. Both record `arc/api/arc_sessions_restore.mm` SHA-256 `a6475f82e58de755fbf87578cc0dcb4144c0aeb685ce0073e707af442a7e37f0`, also matching the current source during this read. Core's three 12-file source seals and the two 25-file runtime-input seals agree. This proves the recorded seal identity/stability, not causal isolation of all native/shared inputs.
- `build.log:3` records BUILD DONE on attempt 1. `package.log:7` identifies the built ArcCore framework; `package.log:49` records the packaged app. The pre-package framework hash is `a08c0042e6d3c5efd93919c7706cf3dc87264b02975b9e92517918faad833bfe`; the recorded packaged and packaged-current hashes both equal `5f732c9ab855fc041b77267a84d94e1f554251112ac4d51069f49cf60add1b03`. Do not equate signed packaged bytes with the pre-package bytes.
- Both harness exit files are 0. `completion.json` reports collection complete, scenarios pass, stable runtime/core inputs, and app/helpers exited. Launch steps include cookie/localStorage/IndexedDB relaunch readback and natural Browser.close exit; tabs steps include navigation/back/forward/reload/redirect/one exact tab close. This relaunch is not a closed-tab/window session restoration test.

## Exact session-related rows

| Transcript | ArcSessionID identifier first return (golden / ours) | Getter call count (golden / ours) | Global didCloseBrowser count (golden / ours) |
|---|---:|---:|---:|
| launch-slice launch-1 | 1064772386 / 610189578 | 8 / 5 | 0 / 1 |
| launch-slice launch-2 | 1064772440 / 610189633 | 3 / 3 | 0 / 0 |
| tabs-navigation launch-1 | 805422707 / 1684758739 | 14 / 14 | 0 / 0 |

The three default diff JSON files report the first ID value mismatch; only launch-1 reports the ID count mismatch. The app-origin `ArcWebContents sessionId` getter counts are launch-1 6/3, launch-2 3/3, and tabs 12/12; these return ArcSessionID objects, while the subsequent identifier getters and close-callback ID getters produce the numeric counts above. These different run-assigned numbers are observations, not independently a broken-ID or persistence conclusion. ArcSessionID itself belongs to core. The `extensionGroupIdForTabWithSessionId:` numeric argument rows track those first live IDs; launch-1 callback counts are 13/10, tabs 26/30, with no demonstrated grouping/restore operation.

The extra global close is `launch-slice/launch-1/record.jsonl:6033` (return6078), main, with args `[F1676741-7766-4A6B-9056-1874E689529C, <ArcBrowserContextID#33>, 0, null]`. The default diff retains it as an extra callback. It is absent in golden launch-1 and absent on both sides of launch-2 and tabs. This matches the historical `results/sessions/launch-slice-v2` shape: extra close with nil restoration ID and 8/5 identifier calls. The current archive alone does not identify why the app creates/closes a different browser path.

Individual close callbacks are present on both sides. Launch-1 original imminent live ID1064772386 is followed by didClose historical ID1064772387 (lines5281/5323, nested ID returns5283/5325); ours610189578→610189579 (5557/5599, nested returns5559/5601). Tabs original805422709→805422710 (8403/8445), ours1684758741→1684758742 (8697/8739). This is bounded value evidence from ordinary closes; it does not verify session restoration eligibility, reentry timing, or restart restoration.

## Raw gating and exclusions

The first two raw rows for each new transcript are directly available at:

- `launch-slice/launch-1/record.jsonl:1` and `:2`;
- `launch-slice/launch-2/record.jsonl:1` and `:2`;
- `tabs-navigation/launch-1/record.jsonl:1` and `:2`.

Each first row is `hooked` with 340 engine classes. Each second row is a main-thread app call to the full public configuration initializer, with the exact ours156 executable/profile paths and sessionRestorationPolicy argument0 (argument index5); its paired return is line3. Both golden and ours initializers receive policy0. Original raw policy getter call25/return26 is from engine and returns0; ours has no raw policy getter call. The default diff deliberately excludes that original engine-internal getter, so it is not a default-diff missing selector or a policy-value mismatch. This provides no policy1/startup consumer coverage.

The archived `tools/tests/recorder/diff.py:38` caller filter includes app→engine calls, engine→app callbacks, and engine setters, while excluding engine/internal and other non-app getters. New raw streams contain app, engine, and Combine callers; there are no `from:driver` calls in these three streams. We have not promoted any excluded driver/helper call into a default boundary-diff success claim. All three raw golden/ours pairs contain zero calls to public canRestore/restoreWebContents/restoreBrowser/restoreForeignTab, didRestore/finalizeDiscard, discard, or the four session DTO classes. Successful launch/tabs therefore cannot resolve known held native restore factory/source, policy, canonical identity, close timing, or discard rebind/accounting work. `coverage.json` accepted=true in each scenario means all scenario steps pass, transcripts exist, hooks are nonzero and calls exist (`tools/tests/scenarios/coverage.ts:12–33`); it does not require these missing session selectors or a zero default diff.

The golden directories are archived original transcripts with explicit recorder-upgrade provenance, not fresh paired original executions in this core job. The local audit manifest preserves input hashes, first-two rows, exact selected raw events, and session-related default-diff rows. No source changes, builds, app launches, runtime probes, board access, or credential access were performed by this audit.

Independent read-only second review (`/root/sessions_recon/full_probe_review`) confirms the selected diff rows, policy/getter exclusion, absent restore/DTO/discard calls, and the absence of direct driver caller labels.
