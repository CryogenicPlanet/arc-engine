# Batch6 independent evidence review

Scope: supplemental public Objective-C API and CDP fixture evidence. **No canonical/default-diff parity claim.** No apps, builds, network, credential access, or production edits during review. The only writes are this directory's JSON/Markdown files.

## Findings and confirmed differences

No P1/P2 false-positive defect found in the v6 fixture for its stated two-individually-closed-tab restart contract. Two observable parity differences remain:

- **P1, full-window callback contract:** replacement `full-ours-run2/result.json` reports one created callback followed by finished with `insideAction=true`, zero captured didRestore callbacks and zero restored wrappers. Original run3 reports two main-thread asynchronous didRestore callbacks using source live IDs, then asynchronous finished. Replacement therefore fails the strict existing full-window fixture before history verification. The artifacts do not establish that no native tabs/history were created elsewhere; no root cause is inferred here.
- **P2, individual close callback depth:** original restart run1 emits both close callbacks with `insideAction=true`; replacement emits both with `insideAction=false`. Both are main-thread, exactly once, and supply genuine historical IDs. The v6 contract checks close ID/main/count, but not close depth. Its PASS is valid for the documented restart contract and does not prove close callback timing parity.

Full-window finished wrapper pointer identity differs: original `finishedSameBrowser=false`, replacement true; both public sessionId/windowIdentifier/context identity and isEqual match. The fixture correctly accepts public identity. This is an observed wrapper difference, not a reported defect.

## Paired result

| Probe | Original | Replacement |
|---|---|---|
| Build/package | batch preflight succeeded | build/package succeeded |
| Individual closed-tab restart | PASS | PASS |
| Process boundary | 91869 → 92333 | 92581 → 93154 |
| Restored tab1 IDs: source / historical / new live | 1508786195 / 1508786197 / 1508786250 | 1554261910 / 1554261912 / 1554261968 |
| Restored tab2 IDs: source / historical / new live | 1508786196 / 1508786198 / 1508786251 | 1554261911 / 1554261913 / 1554261969 |
| Restart restore callback IDs | historical IDs, once, main, outside action | same relations |
| Final restart wrapper lifetime | both initialized simultaneously, distinct live IDs | same |
| Restart history | exact A/B/C URLs and titles, current index 1 (B), both tabs | same |
| Full-window | PASS, callback/history contract | FAIL before restored-history phase |
| Full created / restored / finished counts | 1 / 2 / 1 | 1 / 0 / 1 |
| Full finished action depth | outside action | inside action |

Typed role normalization preserves raw IDs in [paired-comparison.json](paired-comparison.json). It maps source-live, historical, restored-live, and window IDs separately, replaces only fixture base URLs, and retains event order/depth. The restart projections differ only in the two close callback depth flags. No offset/arithmetic ID normalization or callback sorting was used.

## Why restart PASS is supported

The native fixture captures historical IDs directly from exact retained source-wrapper close callbacks (`/tmp/arccore-sessions-tab-restart-v6.mm:32`). Before quit, both old wrappers are uninitialized, both historical IDs are eligible, and neither has been restored. Relaunch reconstructs only public ArcSessionID objects (`:75`), and post-relaunch public eligibility plus restore callbacks must use those exact historical IDs. Callback matching by ID is independently cross-checked against initialized returned-wrapper URLs, new positive live IDs, and exact CDP histories (`/tmp/arccore-sessions-tab-restart-v6.ts:148`). Final state rechecks both wrappers simultaneously (`:153`). The fixture does not navigate fixture pages after relaunch to recreate their histories.

Both AppKit shutdowns for both agents have acknowledgment and verified process exit; second-launch PIDs differ. No emergency cleanup fallback/error is recorded. `cleanShutdown` requires both acknowledgment and exit (`v6.ts:84`). Requests/readiness/results use fresh nonce and exact PID, executable, bundle, and lock-ancestry checks; the two agents use isolated profiles restored by the outer wrapper. Both result.json files equal their observations.json files.

## Manifests and limits

All 10 temporary fixture source/driver/dylib/storage-helper hashes match the copied batch fixture manifest. Replacement framework plus owned sessions/browser source hashes match before and after batch. Raw manifest lines and hash comparisons are retained in [review.json](review.json). This pins the reviewed batch, not later production state or source-to-binary reproducibility.

Storage snapshots show an opaque encrypted version-5 Tabs file plus a version-3 file before and after quit. No payload parsing/decryption was performed. Those snapshots prove file presence/hash/header only; the public post-relaunch historical-ID behavior is the persistence oracle, not a claim that a selected file contains a particular fixture.

Coverage excludes full-window persistence/restart, automatic startup restoration policy, groups/splits/app windows, shutdown/reentrancy cancellation, serialized referrer/transition metadata, and arbitrarily late duplicate notifications. Source fixtures constructed A→B→C→B; observations verify the resulting full history and selected B, not post-restore physical back/forward traversal. Full fixtures differ in supplemental close-delegate instrumentation and cleanup transport; missing replacement `closedTabCallbacks` is not evidence of missing production callbacks.
