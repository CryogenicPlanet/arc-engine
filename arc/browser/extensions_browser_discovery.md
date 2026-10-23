> Superseded by the combined `extensions_positive_probe.patch`, now applied to the owned helper sources. Do not apply this historical patch separately. Runtime verification of the combined version is pending.

# Deferred public-service browser discovery

Unapplied proposal: `extensions_browser_discovery.patch`, based only on the current actual `arc/browser/extensions_service_probe.m`. Actual helper/product sources and runtime artifacts are unchanged. Proposed source and compiled outputs are under `/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-browser-discovery-gjf1v2rv`.

## Problem and change

Validation5 stock discovery reported `browserMatches=0`. The parent verified that `ArcBrowser.context` returned fresh wrapper objects while the real extension service delegate used its canonical owner. The former weak context-keyed browser map could disappear with the temporary wrapper even though the browser and app delegate remained alive.

The proposal records actual `ArcBrowser.setDelegate:` captures in a global map with weak browser keys, pointer key personality, and weak real app delegate values. Nil removes the entry; every forwarded original setter/callback remains unchanged. No browser, delegate, context or service lifetime is extended beyond scoped request locals.

For each request, including registry discovery, it obtains each live captured browser's public `context` and that wrapper's public `extensions`. It selects only candidates whose service has the audited identity ABI, compares equal to the actual captured extension service in both directions, and has the same public hash. Exactly one match supplies the action browser; zero or multiple matches yield no selected browser, and the existing strict registry assertion rejects the precondition. No app activation, target guessing, fabricated browser/delegate, profile mutation, or basename/pointer-context routing is added.

The actual response includes `browserCandidates`: candidate class/delegate presence, native identity ABI, both equality results, both hashes/hash agreement, match result, and target/candidate exposed context-ID fields. `contextIDFieldsEqual` compares only `directoryBasename` plus `isIncognito` and presence; it is explicitly diagnostic and does not select the browser. This raw inventory is retained in actual operation responses, not normalized into native app demand. Existing Action-B tab-ID/exact nonce-URL assertions still establish the selected tab before an action success claim.

## Identity audit

- `arc/tools/arccore-1.170.0-objc.tsv:490-491`: `isEqual:` is `B24@0:8@16`, `hash` is `Q16@0:8`.
- `specs/ArcBrowserContextExtensions.md:40-41`: equality has an object-identity fast path and compares native optional engagement/service pointer; hash requires an engaged native service and hashes that pointer.
- Examined exact stock bodies `research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43fca30.c` and `0x43fcba0.c`. The shared hash body has an unrelated decompiler class heading, but the method metadata and IMP address establish its use here.
- `ArcBrowserContextID` declares no value `isEqual:` override. The proposal therefore reports exposed ID-field agreement honestly, without treating object equality as native ID equivalence.

The request's target service comes from the real app delegate capture with a live owner; candidates come from real live browser contexts. The proposal does not construct detached services or call private native optional-storage selectors.

## Validation

Temporary strict `xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework AppKit` compilation passed. `git apply --check arc/browser/extensions_browser_discovery.patch` passed without applying it. Logs and `frozen.json`/`freeze-check.json` are in the temporary directory. All frozen actual helper/product source and runtime artifact hashes, sizes and mtimes remained unchanged. No TypeScript/shell source changes are proposed, so their prior checks are unaffected.

Fresh read-only review approved with no concrete blockers after checking equality/hash ABI, native wrapper getters, weak ownership, unique matching and callback attribution. No app runs, builds, lock acquisition, runtime artifact overwrites or profile changes occurred.

## Integration and uncovered behavior

This patch and the separate `extensions_command_line_probe.patch` share a `.m` base and will require a deliberate merge/rebase after the current batch. Neither is applied here. The current malformed-manifest timeout/outstanding-install state remains visible; this proposal adds no dialog suppression, callback substitution or forged completion and does not certify unresolved cleanup.

Runtime success is unverified. Public native service `hash` requires an engaged native optional; detached/closed-service behavior remains unvalidated, and the proposal stays scoped to captured live services in the launch sequence. A genuine second browser for the same extension service still fails the ambiguity precondition. No scenario diff was run and no boundary parity is claimed; recorder-v2 fixture-call coverage remains a separate evidence limitation.
