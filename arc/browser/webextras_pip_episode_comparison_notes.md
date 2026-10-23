# Held typed PiP episode comparator increment

Proposal only: apply `webextras_pip_episode_comparison.patch` **after exact comparator24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf** (full normalization chain →87be boundary arguments →f2b3 callback correction). The patch changes `tests/recorder/diff.py` and adds `tests/recorder/pip_episode.py` in a private staged tree. Actual recorder files, policy, transcripts and runtime remain untouched. The designated omnibox owner integrates only after review and lead coordination.

This implements the bounded logical episode proof in `webextras_pip_session_comparison_requirements.md` (SHA4a3698bc617f1eb0b5595ad702d71950d3b7c8e28904b2f19a1932a0c1bf475a), under captured lead143529/current canonical-PiP allowlist. It does not recover native pointers or extend identity after Hide/reopen.

The mode is automatic with the existing CLI. There is no new flag, report key, output line, normalization mask or exit policy. Unsupported, malformed, ambiguous, non-PiP, v1 and fixture-image comparisons retain the original comparison. In supported captures, only the proved sole `ArcWebContentsMediaSession.pipController` normalized return slot may change. The existing caller-field predicate was extracted without changing its condition and is reused for proof; syntactically valid other caller images keep the existing admission policy.

Proof requires all of the following on both sides:

- Complete per-thread raw call/return signature and depth pairing, unique sequence counters increasing within each thread. Cross-thread write interleaving is preserved; records are never sorted or renumbered. Only a sole outer main-thread `ArcBrowserApplication.run` may remain open.
- One explicit browser creation and browser-created WebContents callback; all public WebContents/media-session calls resolve to that owner and one stable media session. The paired app delegate registration occurs inside the owner's creation callback.
- One exact delegate/owner controller-created callback, then one balanced Show, then one Hide. A lifecycle-shaped frame in any other direction/class/kind/caller lane rejects proof; any destroy-shaped frame rejects this limited scope. Hide is an eligibility boundary, not proof of destruction.
- The sole app `pipController` getter is a direct balanced child of Show, preceded there by the same owner's `mediaSession` getter. Public controller member calls use that returned wrapper inside Show. Missing/conflicting/extra-owner/replacement/unclear-ancestry evidence leaves baseline intact.
- A consistent raw public owner URL within each run, matching across runs through **existing loaded URL normalization**, and owner→SessionID→stable positive int32 identifier edges matching through **existing loaded bijective normalization**. Active-state roles must match. No `/a`, host, port, raw object number, native pointer or fixture-specific SessionID constant appears in the proof.

The transformation runs after existing load, Google-query normalization and omnibox receiver alignment. It reads the already-normalized controller-created argument and substitutes it only into the proved getter's normalized return. It never pre-seeds aliases or changes first-appearance numbering. Thus unrelated reference-shaped strings and every other return/argument retain their original normalized values. All calls, callback order/counts, getter payloads, scalar arguments and raw files remain present. Proof is all-or-nothing across both captures; rejection silently retains the baseline.

## Actual capture effect

Frozen gold `results/golden/arc-1.170-media-pip/launch-1/record.jsonl` and ours `results/webextras/media-pip-current-run1/launch-1/record.jsonl` have the explicit owner/delegate/create→Show/getter graph detailed in the requirements. Only this row is removed from the default report:

```json
{"method":"->engine -ArcWebContentsMediaSession pipController","call":0,"args":"[]","golden":"\"<ArcWebContentsMediaSessionPiPController#2>\"","ours":"\"<ArcWebContentsMediaSessionPiPController#1>\""}
```

Exact24437 default header:

```text
methods differing: 48 (missing 8, extra 1, returns 11, callback args 3, counts 25; internal-only 0, ours-internal 1)
```

Proposed header:

```text
methods differing: 47 (missing 8, extra 1, returns 10, callback args 3, counts 25; internal-only 0, ours-internal 1)
```

Both callback-sequence lines are byte-identical, with gold172/ours170 and first divergence1. Full first two lines, commands, exit codes and the exact removed row are retained in `logs/wp/webextras-pip-episode-comparison/receipt.json` plus `actual-default-{baseline,staged}.{stdout,stderr,json}`. These are current private24437 results, distinct from the historical50-difference current-run1 report.

With existing `--compare-boundary-arguments`,71→70 differences, again only this same return row disappears; all26 argument mismatches remain. Every other JSON field/list/row equals baseline in both invocations. Both exit0 behaviors remain the existing CLI's report semantics, not acceptance. Total callbacks172/170, feature-state callbacks92/72, source-title callbacks2/1 and currentTime0.322324/0.422503 remain unresolved. The elapsed-time evidence gap and source-cache timing question stay separate; no time/scalar normalization is added.

## Validation and limits

`python3 -B arc/browser/webextras_pip_episode_comparison_test.py`: **38 groups PASS**. Tests apply the actual patch privately and require byte-exact staged reconstruction, actual nonempty capture effect/default+argument mode, self-comparison equivalence, a different-host/path/IDs synthetic public graph, and byte-exact fallback for owner/delegate/pair/ancestry/extra-owner/replacement/Hide/destroy/URL/SID/caller/sequence-counter failures. Single-getter post-Hide and single-Hide pre-Show controls reject without relying on a count change. Reference-shaped title text, geometry/videoView payloads, scalar callback/setter arguments and callback/count/sequence changes remain different. Launch-slice, devtools and pdf self comparisons, v1/empty input, help, malformed options and existing error controls retain exact exit/stdout/stderr/JSON bytes.

Fresh review found an initial malformed-lifecycle lane gap; the initial evidence is preserved in the reviewer's distinct folder. Corrected guards inspect lifecycle selectors across all lanes before accepting exact callback shape, reject unknown method phases, and validate legal direction/kind enums. Regressions include the reviewer's legal internal-lane and bogus-lane Created/Destroyed reproductions. `test-corrected.log` records the final38-group run; earlier logs are historical development runs.

Actual freezerf605 automatically captures **eight** inputs including the new helper. Its capability and all four current frontend1a2c canaries pass; there are no manual extra-helper copies in the frozen snapshot. Durable evidence: `automatic-eight-input-snapshot/comparison-inputs.json` and `frontend-four-canaries.json`. This proves comparator/freezer compatibility, not app runtime or architecture acceptance.

The route intentionally covers one fully proved public owner and episode, not generic multiple-tab matching, replacement/reopen, absent lifecycle callbacks, arbitrary object aliases or native-pointer identity. Such captures retain baseline. No claimed implementation fix follows for timing, PiP event multiplicity, native destruction/cache behavior, DevTools or any other method. No new app/build/runtime, production source edit, lock, index operation or board write occurred.

Frozen input/source/patch/test/evidence hashes are recorded in `logs/wp/webextras-pip-episode-comparison/freeze.json`; they were verified before handoff. Fresh independent review is required before readiness and remains a separate artifact.
