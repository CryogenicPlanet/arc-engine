# Executor v3: private Chromium index for official regeneration

**PRIVATE preparation only. No live executor/index/queue/build/runtime operation.** V2 and earlier proofs remain unchanged. V3 `/tmp/arccore-sessions-adopt-source-only-v3.py` SHA256: `b3627435ecb682d2221a7f64fc85df50c2373abb0bc9f381f90b938ad57391c0`; `transaction.py` is byte-identical.

The minimal additive change protects a foreign assigned but actually untracked header from official `regen-patches.sh` intent-to-add changes in the real Chromium index. Immediately before its first official regeneration, while the outer lock is verified, v3 discovers and copies the actual entry index once into the transaction evidence. Only apply and rollback regeneration receive local `GIT_INDEX_FILE=<copied-index>` and `GIT_OPTIONAL_LOCKS=0`. No global environment changes; stage git operations keep their own repository/index. Official regeneration can naturally `git add -N` assigned untracked files in that private index. The same copy is reused on rollback.

Each regeneration records actual index hashes immediately before/after that invocation and rejects a difference. This is bounded to the invocation, not a whole-runtime exact-byte gate. Differences from the entry index already present before a later invocation are recorded separately. The executor never restores or writes the actual index or staging. Existing failure/rollback/HOLD logic remains.

The current official regen tool is pinned at `d90b45cd225c14324df1fdf5661a38db338662a1efa0ff9da2c7bb4e0acf94be`, verified during capture and again before source publication. A changed queued tool aborts before live writes. No shared tool is edited. Runtime v1, frozen32 pins, source patches, header union, runtime/signal/cleanup and ownership rules are unchanged.

Ten private tiny-Git controls pass. The exact two regen methods are extracted from AST and evaluated against a private test facade and tiny repository; the copied official script generates the expected new-file patch for an assigned foreign UNTRACKED header and expected owned apply/rollback diffs. The real tiny-repo index remains byte-exact across both pathways; its header remains untracked, while the private index contains the add-N entry. Copy occurs once and ambient environment stays unchanged. Tool hash positive/negative comparison is included. No production executor import/construction/entrypoint, actual shared lock or live index is involved. See `private-index-results.json` and narrow independent review.

Parent invocation from repository root, requiring fresh output directories:

```sh
arc/tools/locked.sh python3 /tmp/arccore-sessions-adopt-source-only-v3.py \
  --evidence-dir /Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/source-only-adoption-transaction-v3/run-parent-v3 \
  --frozen-runtime-pins /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/pins.json \
  --frozen-runtime-pins-sha256 1324a413c955799e43fcd6054478e9ca645d5f11117d33025e992a5cee79505e \
  --runtime-command python3 /tmp/arccore-sessions-adoption-runtime-v1.py \
  --out /Users/cryogenicplanet/general/arc-engine/results/sessions/adoption-source-only-parent-v3 \
  --pins '{runtime_pins}' \
  --baseline-root /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/baseline \
  --full
```

This v3 proof concerns index isolation and the new tool pin only; it is not live integration or sessions acceptance. Explicit v3 evidence output avoids the inherited default evidence location.
