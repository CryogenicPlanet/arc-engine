# Optional genuine browser delegate repetition probe — UNAPPLIED

This proposal changes only temporary copies of `extensions_service_probe.m` and `.ts`. The actual helpers remain unchanged. No Arc app, actual engine API, runtime fixture, shared build/lock, or board operation was performed. Parent integration and a paired original/ours run are still required; the checks below establish harness behavior, not engine parity.

## Patch and exact bases

Apply only after confirming both actual helper base hashes:

| File | Actual/base SHA-256 | Proposed SHA-256 |
| --- | --- | --- |
| `arc/browser/extensions_service_probe.m` | `c715415d56337dad54b0ecebfb34809de2a7d4610855afb5a22fdd59b47eea9b` | `bc3adca6b91dd92ac65046e63956109c0b2faff75db7f82be0685a30bb9c519c` |
| `arc/browser/extensions_service_probe.ts` | `a506bd8516a7b6df15d5a5faa6f330747ab149db4e0cf00eef305433c3ad774c` | `ac5e55c85b27758222e69e25866340c6eae53f965408cb4f42a47a174d4f6eb9` |

Patch: [`extensions_browser_delegate_repeat_probe.patch`](extensions_browser_delegate_repeat_probe.patch), SHA-256 `a53f0a3b189c36f4d13f7d4fdcd289b82e702bc60a98b41f0a21bdc82401f530`.

The parent separately applied the product cache-preservation fix. The product source observed while preparing this proposal was `arc/api/arc_extensions_browser.mm` SHA-256 `03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e`; this patch does not change it or establish its runtime result.

## Native basis and scope

The independently reviewed [native lifecycle proof](../../logs/wp/extensions-action-command-lifecycle.md) has SHA-256 `41e25a1bdb8d3de00503274587221652ad41b4d453749cf5dc9b1afca5eca66c`; its `native-evidence.json` has SHA-256 `7cca45c364e1e7092f9b7d7d3a7247d791b56cadead45305cd14d2e5f4f8c448`. The pinned original engine whole-file SHA-256 is `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.

Public `ArcBrowser setDelegate:` metadata is `v24@0:8@16`. Its original implementation is bounded by `0x43e1284..0x43e149c` (536 bytes, SHA-256 `43c339b8d590a64f106d3cb2a196046dcfd33c137a82add08768c33ed5d86416`). With an existing bridge and nonnil incoming target, the branch loads the bridge at `0x43e12ac`, addresses its weak slot at `0x43e12b4`, calls `objc_storeWeak` at `0x43e12bc`, then releases the incoming argument and returns. That branch has no popup/cache rebuild or toolbar producer call. This supports a genuine same-nonnil-target repetition test only; nil, replacement, weak expiry and first-install behavior remain outside this probe.

## Optional operation and ownership guards

`--browser-delegate-repeat` requires explicit `--fixture-mode command-line`; the child additionally requires `ARC_EXTENSIONS_PROBE_BROWSER_DELEGATE_REPEAT=1`. The selected flag is recorded in provenance. The default 19-operation CLI sequence is unchanged. Selecting only this option adds one helper operation after the ordinary A popup, containing two genuine public calls: `executeExtensionAction:` for owned A, immediately followed on the same UI stack by `setDelegate:` once with the exact previously captured app delegate. It never constructs or substitutes a delegate and never calls a delegate callback directly.

The native helper verifies the uniquely selected browser and extension service in both directions, actual initialization, exact bound loopback nonce URL, distinct owned A/B IDs and expected fixture names/version/source, and actual enabled/action state. It rejects outstanding installation/startup work and uncertain Web Store state. It requires prior observed nonnil setter capture for this browser and retains the genuine weak-map target strongly through this operation. Runtime method encodings are checked before dispatch. It accepts the actual installed capture topology either directly or through the recorder's `_objc_msgForward` plus verified `__arcrec_setDelegate:` alias; it calls only the public selector.

The action is marked pending before dispatch to prevent nested polling from finishing a live native stack. Before the setter, it rechecks work/browser/context/liveness, pointer identity and the prior non-probe setter count. Any intervening setter, including nil/another delegate followed by restoration, aborts the repetition. The hook must observe exactly one same-object setter. Exceptions and finish paths clear the stage and strong retention. A failed guard is a real run failure; no fallback call, input, callback or success record is invented.

The runner requires the prior normal popup's three real HTTP effects and actual close callback before starting this phase. PID-bound CDP checks require the sole page to remain the exact nonce target before and after; the runner neither activates nor removes tabs/windows to create this precondition. The existing fixture generation lines and bodies remain byte-identical, including resize at 300 ms and close request at 1400 ms. The operation uses the existing three-second settle interval.

## Runtime evidence required before a pass

The proposal requires all of the following actual observations in each engine:

- Ordered, balanced raw public action/setter call and void-return pairs on main, with the same browser, the helper image as caller, owned A action ID, and a setter argument matching the latest prior genuine app-origin setter on that browser. That prior setter must also have a paired return.
- Exactly one real popup open and close callback to that same genuine app delegate and browser, owned A ID, a newly opened raw `ArcWebContents` identity and an actual URL getter returning A's `chrome-extension://.../popup.html`. Open occurs after the action starts; close occurs after the setter returns.
- Actual resize callbacks of 320×160 then 400×220 between open and close, unchanged full pin order, and no full-toolbar callback for that browser in the bounded phase. Absence is a bounded observation, not a count-only causal claim about unrelated startup events.
- Exactly three newly arriving HTTP effects in order (`popup-loaded`, `popup-resized`, `popup-close-requested`), with the owned A ID and popup URL. A final post-exit audit rejects late duplicates or missing reports.

The HTTP effects are associated by arrival after the completed prior popup. The unchanged fixture does not include a per-document nonce, so these reports alone cannot prove document identity. The new raw `ArcWebContents` identity, actual URL and real callback lifecycle provide that authority. This limitation is explicit and parent-approved; no stronger HTTP identity is inferred.

The full observations retain raw CDP target IDs, prior capture count/offset and real callback data. A separate `.raw-identity-proof.json` retains the actual raw rows and IDs used for the paired-call, delegate/browser and new-popup checks. The semantic projection compares actual URL/nonce, within-run same-target facts and a prior-real-nonnil-setter-observed fact. It deliberately does not compare cross-engine transport/object ID allocation or startup setter offsets/counts as this operation's behavior. This is a diagnostic projection with independently retained raw proof, not full boundary parity. `boundaryMethodParity` remains `null`; no new full method/fixture inventory claim is made.

## Verification performed without launching Arc

Exact commands, stdout/stderr, input copies, test sources and results are retained in [`logs/wp/extensions-browser-delegate-repeat-probe-proposal`](../../logs/wp/extensions-browser-delegate-repeat-probe-proposal/). `commands.json` records argv/cwd/exit and `validation.json` records source/proof hashes and scope.

- Strict Objective-C syntax and standalone dynamic-library compilation: exit 0 (`-fobjc-arc -Wall -Wextra -Werror`, AppKit linked for the library).
- Strict TypeScript (`tsgo` with strict, unchecked indexed access and exact optional properties) and Bun bundle: exit 0.
- Standalone local mock executable: 40 assertions passed, including direct/recorder-alias capture, same genuine pointer, guarded ownership/URL/ABI, mutation/restoration rejection, pending reentrancy and exception cleanup. These are mock objects, not app effects.
- Isolated TypeScript validators: 68 assertions passed, including required typed fields, actual lifecycle/effect shape, balanced raw identity predicates, corrupt/missing evidence rejection and late duplicate rejection. The runner was not imported or executed; test rows are local mock input only.
- `git apply --check --whitespace=error`: exit 0. Application to newly created temporary base copies produced bytes identical to both proposed files. Both actual helper base hashes remained unchanged.
- All 25 pre-existing static `await action(...)` expressions are byte-identical after removing the new optional block; all fixture generation lines are byte-identical. Existing optional pins, omnibox and malformed Web Store phases, cleanup and startup defaults are preserved.

Fresh independent review found and closed two draft issues: recorder forwarding topology and reentrant delegate mutation/restoration. The focused re-review found no further actionable issue; its record is `fresh-review.txt`. Runtime completion, actual raw proof, popup continuity, cleanup certification and original/ours parity remain unexecuted and must be assessed by the parent after integration.

## Parent integration status

Parent APPLIED the exact historical patcha53f0a3b189c36f4d13f7d4fdcd289b82e702bc60a98b41f0a21bdc82401f530 after final review. Actual helpers are Mbc3adca6b91dd92ac65046e63956109c0b2faff75db7f82be0685a30bb9c519c and TSac5e55c85b27758222e69e25866340c6eae53f965408cb4f42a47a174d4f6eb9. Actual strictM/TS checks exit0/empty in logs/wp/extensions.service.delegate-repeat.{clang,tsgo}.log. No runtime is queued at this note; fullvalidation13 planned. The patch is no longer additive to current helper source.
