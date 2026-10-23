# Browser pin/unpin/move probe extension

**APPLIED after validation9 completed with changed-paths=[].** The parent applied the reviewed helper-only patch and independently passed strict ObjC syntax and TypeScript checks; logs are `logs/wp/extensions.service.browser-pins.{clang,tsgo}.log`. Runtime validation is pending. The historical proposal checks below wrote only this patch/document, `logs/wp/extensions-browser-pins-probe-proposal/`, and temporary files under `/tmp/arc-extensions-browser-pins-probe-v1/`; no actual helper/product/runtime or app state changed during that preparation.

This adds an optional **`--browser-pins`** phase to the supplemental runner. It requires explicit `--fixture-mode command-line`; default operation order and Chromium startup switches are unchanged. The helper separately requires `ARC_EXTENSIONS_PROBE_BROWSER_PINS=1` plus command-line mode before any new selector invocation. The runner records the flag in provenance and explicitly sets that environment value to 1/0 for its owned child. This is a separate probe environment, not a policy bypass or proof of public-install enable behavior.

After rebuilding the helper, the supplemental parent-locked runner uses:

```text
arc/browser/extensions_service_probe.sh run --out results/extensions-service-probe/UNIQUE --fixture-mode command-line --browser-pins
```

Runtime is **not yet executed**. `extensions_browser_pins_verify.sh` checks product hashes against compiled validation9, rebuilds the helper, checks current built/package identity through the service runner, and freezes before/after inputs. It does not rerun canonical scenarios or claim a new product build. No native browser pin/move result, callback timing or product parity is claimed here.

## Exact source bases and patch

The historical repository-root patch changed only the two helper files below. These exact source hashes and application were checked after validation9 ended; the proposed column is now applied.

| File | Frozen actual SHA256 | Proposed SHA256 |
| --- | --- | --- |
| `arc/browser/extensions_service_probe.m` | `5eeb5657a5cfc49067f9d72c34ec6bad38357dedaffd284423c83357f9f9b6ec` | `810558c095dafc67ff25399524199a11b2940e9a8e0ebeae86f171daa9012b25` |
| `arc/browser/extensions_service_probe.ts` | `d7adfe863abb9b7ea3c63310ebf4ac34a56a847faf88290612d11dd7b9dedcca` | `777b98336819a1dad8eb386ed4cade864acbea4ddc78b2a3784691b23d0360ba` |

Patch SHA256: `b79322deb7ac266d7f065c6f678e894bc90c6cd93a7be0120ecea655dd210499`.

## Safety and real boundary calls

For every added request the helper re-runs the existing browser mapping: public extension-service equality in both directions plus matching hash, captured real browser/delegate entries, exactly one matching browser, and equality with the request's selected browser/service. Transient context-wrapper pointers are not used to select another window. The existing outer request guard still requires the actual Default non-incognito context. No app activation, native pointer lookup, unrelated window, context menu, arbitrary extension ID or OS input is used.

Both A/B must be independently nonce/name/ID/version proven through the actual service, enabled and action-bearing. All startup ownership must be adopted, no install may be outstanding, and baseline pins must already exist. Input accepts only A/B labels; raw `extensionID` and `targetIndex` fields are rejected. A move's anchor must be a currently pinned A/B label; its absolute index is read from the actual visible pin order immediately before dispatch. All new messages invoke the browser's real selectors via objc_msgSend; no result or callback replacement is installed.

Runtime method signatures are checked before invocation: pin/unpin void with one object argument; move void with object plus **`Q`** and 64-bit NSUInteger. The move cast is explicitly `void (*)(id,SEL,id,NSUInteger)`. Actual selector, complete method encoding, label, real arguments including derived absolute index, full ordered `pinsBefore`/`pinsAfter`, existing real delegate events, and identity/baseline assertions remain in operation evidence. No synthetic ArcEngineRecorder rows are created. Callback forwarding and existing operation-entry attribution remain unchanged.

Before mutation, non-fixture pin projection must exactly match the captured ordered baseline and visible order must have unique string IDs. After settling, the helper checks baseline preservation again and the runner independently compares it with the registry's baseline. Moves also assert the full-array remove/insert result at the actual index, including unchanged unrelated order. Before the first valid move, actual toolbar callbacks must contain both fixture action IDs, proving the native cache prerequisite in observed data rather than assuming model readiness.

## Bounded native index/ordering evidence

- `arc/tools/arccore-1.170.0-objc.tsv` and `specs/methods.tsv:1075` give `moveExtension:targetIndex:` exactly `v32@0:8@16Q24`. IMP `43e2714` accepts an unsigned_long_long and forwards it.
- Native `43b606c` requires `targetIndex < visiblePinnedCount`, a nonnull cached action and an already pinned ID before calling `7cd9f40`.
- Native `7cd9f40` returns for a force-pinned action or unchanged index, otherwise rotates only the chosen action in stored ordering, preserving unrelated relative order. Current Chromium `ToolbarActionsModel::MovePinnedAction` uses the same visible-index/neighbor semantics.
- `43b6c18` only pins when absent and unpins when present. `7cd94c0` appends/removes the ID in pinned prefs. Forced entries may occupy a suffix outside ordinary stored ordering, so the runner never assumes baseline.length is a valid fixture move index.

Exact input paths/hashes are retained in `validation.json`; `metadata-rows.txt` preserves the three runtime metadata rows. Sources include the retained original decompiles at `research/2026-10-22-sol/evidence/agents/spec-2/classreview/backend/{043b606c,043b6c18}.c` and `agents/cef/decompile/functions/{7cd9f40,7cd94c0}.c`. This is bounded native/source proof for safe owned valid indices, not an assertion that every native precondition or producer is runtime covered. No arbitrary invalid index or raw ID is sent to the browser.

## Optional sequence and default preservation

The phase is inserted after the existing context `pins-A` step, before command/action/lifecycle/error exercise. Both engines execute the same 11 operations:

| Step | Real browser operation | Expected fixture projection |
| --- | --- | --- |
| browser-unpin-A-start | unpin A; save actual full entry pins | [] |
| browser-pin-A | pin A | [A] |
| browser-pin-A-again | pin A again | [A] |
| browser-pin-B | pin B; require adjacent A/B and actual toolbar snapshots | [A,B] |
| browser-move-B-left | move B to A's actual current index | [B,A] |
| browser-move-B-same | move B to B's actual current index | [B,A] |
| browser-move-B-right | move B to A's actual current index | [A,B] |
| browser-unpin-A | unpin A | [B] |
| browser-unpin-A-again | unpin A again | [B] |
| browser-unpin-B | unpin B | [] |
| browser-pin-A-restore | pin A; require exact full entry-order restoration | [A] |

All original operations and cleanup remain outside the optional block. A static comparison confirms all 21 original `await action(...)` call expressions are byte-identical after removing only the new block; this preserves the existing full CLI 19-operation path and existing public mode ordering/cleanup, not merely a selected shortened flow. On failure, normal existing error cleanup still runs under outstanding-install/nonce ownership rules. Partial optional coverage is explicitly labelled incomplete and actual prior rows remain; completed coverage is marked only after every phase assertion and full entry-order restoration succeeds.

Required-row checks include the new before/after/ABI/argument/identity fields. Semantic comparison preserves actual argument values and ordered fixture before/after projections, selectors, ABI and guard Booleans; baseline is independently checked per engine. Existing known-false semantic/boundary differences stay false, and `boundaryMethodParity` remains null without a verified v2 fixture-call inventory. Enabling the phase does not promote empty recorder diffs to API parity. OTR, disabled/no-action, force-pinned and invalid-ID/index execution remain uncovered.

## Validation and fresh review

- Temporary proposed `.m` passes strict ARC syntax and dynamic-library compile with `-Wall -Wextra -Werror`; final outputs have no diagnostics. Output dylib is temporary and was not injected, signed into an app, or launched.
- Temporary `.ts` passes strict tsgo with noUncheckedIndexedAccess/exactOptionalPropertyTypes and Bun bundling. Existing sibling agent-cdp imports resolve through a temporary symlink to the same real source, not a substituted stub.
- `git apply --check --whitespace=error` passes against both frozen bases. Applying only in a temporary tree yields byte-identical proposed copies. Actual helper hashes were rechecked unchanged.
- A standalone **mock-only** Objective-C executable passes 27 guard/ABI/order assertions: opt-in off/public mode, missing/mismatched browser/delegate, ambiguity, outstanding/startup ownership, absent baseline, foreign or disabled/no-action fixture, raw ID/index injection, duplicated/reordered baseline, unpinned/foreign anchor, extra anchor, signed-q ABI, valid left/right/same-index and pin/unpin calls. Its helper constructor installs no hooks (`gInstalled` remains false); its classes are local mocks, not real Arc apps. This is rejection/call-shape proof, not native fixture evidence.
- An isolated extraction of required-row/semantic functions passes 28 assertions for missing fields, invalid numeric/null Booleans, malformed/unsafe indices, nonstring pins, exact argument preservation and ordered before/after projection. The real runner is not imported or executed.
- Fresh independent read-only review `/root/extension_review/pin_probe_review` found no actionable defects in the temporary proposal. It confirmed CLI/env gating, actual service/browser/fixture guards, Q ABI, baseline/full-splice checks, preserved default sequence/cleanup and explicit parity limits. Final wording marks partial optional coverage incomplete; final restoration asserts exact phase-entry full order.

`logs/wp/extensions-browser-pins-probe-proposal/validation.json` retains hashes, commands, checks, preserved operation expressions and native source evidence; before/after helper copies, mock tests, isolated tests and compiler/test output are alongside it. Temporary source/header compile mistakes in the test harness were corrected before these final checks; no product compiler or runtime acceptance result was substituted. Parent integration and real paired runtime remain pending after validation9.
