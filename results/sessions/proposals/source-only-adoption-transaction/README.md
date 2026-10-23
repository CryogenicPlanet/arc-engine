# Parent source-only adoption transaction

**PRIVATE, never executed on production by this worker.** Parent alone invokes the reviewed transaction through `arc/tools/locked.sh`. The frozen script is `/tmp/arccore-sessions-adopt-source-only.py`, SHA256 `fb482e50efcd33fb7dcfacf353ecdcc17ce968815946801a89181ac51436484c`; `transaction.py` is byte-identical. Runtime runner v1 is pinned at `0df88d889d64f38d46dc06216cc0c2ba87a04167854b05c1f566649a1a73a8a0`.

The executor verifies the lock owner is an ancestor, then captures entry-time preimages of seven targets, MANIFEST, maintained patches and broad `arc/api`/`arc/wp` source pins plus `arc/BUILD.gn`. Generated `.inc` files are excluded. It requires sessions e973, accepts foreign shared-header additions, strictly applies the original six source-only nonheader sections plus reviewed host574, and inserts the exact two sessions hook blocks through a reversible semantic header union. Existing sessions symbols or nonheader conflicts abort without a live write. Foreign lines/includes/forwards are retained. No policy, accounting, logical root, private API or other Arc source changes are published.

Before live publication it runs actual Siso-derived VFS syntax on the three native translation units plus owned sessions. It adds the five native paths to 0010 only when unassigned, preserves any existing assignment and keeps arcmod_hooks.h in 0004. It regenerates maintained patches, then runs the official owned check and rejects actual warning/error diagnostics despite its masked exit behavior. Parent-reviewed runtime must return launch9/9 with accepted coverage, canonical validation no-earlier-regression with both original harness statuses retained, code0 and cleanupVerified true. Canonical harness1 matching the prior close failure is not acceptance.

The caller's frozen runtime pin file and explicit SHA protect reviewed tools/goldens/baselines across the lock wait. Only seven postapply sources, MANIFEST, current maintained patches and entry-frozen broad Arc sources are merged. The standalone `{runtime_pins}` argv placeholder becomes the generated map path. Unrelated runtime/tool changes are never adopted by reading their new hashes. Broad entry source changes before/after execution fail closed.

Signals INT/TERM/HUP defer until the current direct child drains. Runtime is a new session; its remaining same-group descendants drain as well. The separately reviewed v1 runner owns cleanup of its nested new-session commands, disposable app/helpers and original profile/cache identities. The executor does not signal apps or touch profile/cache directories. A runtime return without cleanupVerified holds the lock until a nonce-bound independent cleanup confirmation. An unexpected own target/MANIFEST/maintained-patch delta also holds rather than overwriting foreign work.

Failures after live publication restore only the seven entry preimages and MANIFEST, regenerate before returning, and verify maintained patch recovery. A newly unreferenced own0010 patch gets its original absent/content state back. Unexpected generated-patch edits hold; any regenerated patch delta relative to entry preimages is recorded exactly and requires nonce-bound reviewed confirmation rather than silently claiming an exact rollback. Regeneration/rollback failures retain the lock and retry. A successful transaction is marked APPLIED only after all gates; private mocks use MOCK_APPLIED.

Seven temporary-root mock gates passed: foreign-header union success, launch failure rollback, canonical validation failure rollback, regeneration failure rollback, SIGTERM while runtime child lives followed by drain/rollback, missing cleanup holding until nonce confirmation, and surviving same-group grandchild drainage. Mocks use fake lock/regen/syntax/runtime and launch no app/build. The earlier host-gating proposal separately passed actual four-TU VFS syntax. These checks are not an actual adoption/build/launch result. See `mock-results.json` and final independent `review.md`/`review.json`.

Example exact parent invocation, from the repository root (the two output directories must be fresh):

```sh
arc/tools/locked.sh python3 /tmp/arccore-sessions-adopt-source-only.py \
  --evidence-dir /Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/source-only-adoption-transaction/run-20261023T170400Z \
  --frozen-runtime-pins /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/pins.json \
  --frozen-runtime-pins-sha256 1324a413c955799e43fcd6054478e9ca645d5f11117d33025e992a5cee79505e \
  --runtime-command python3 /tmp/arccore-sessions-adoption-runtime-v1.py \
  --out /Users/cryogenicplanet/general/arc-engine/results/sessions/adoption-source-only-20261023T170400Z \
  --pins '{runtime_pins}' \
  --baseline-root /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/baseline \
  --full
```

Manual HOLD receipts, only after independent verification, are direct children of the chosen transaction evidence directory. Cleanup receipt `manual-cleanup-confirmation.json` requires transactionId, cleanupVerified:true, verifiedBy and evidence. Regenerated patch receipt `manual-rollback-patch-delta-confirmation.json` requires transactionId, exact acceptedCurrentPatchHashes, verifiedBy and evidence. Their presence is never synthesized by the executor. Never release the outer lock while a HOLD or runtime child remains.
