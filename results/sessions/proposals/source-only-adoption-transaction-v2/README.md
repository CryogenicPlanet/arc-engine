# Executor v2: canonical runtime pin identity

**PRIVATE fix only; no executor invocation, lock, live write, queue or runtime.** The parent reported v1 job4709 reaped with code1 / ABORTED_NO_WRITES under lock owner77162 at17:36:45, because the runner pin was incorrectly considered missing. The original frozen v1 script/evidence and actual failure were not modified.

V2 `/tmp/arccore-sessions-adopt-source-only-v2.py` SHA256 is `d6dea94745a287eb94b10be203b18793b6224edc53d55252c9d8f6529a5b7f88`; `transaction.py` is byte-identical. Its only difference from frozen v1 `fb482e50` is:

```diff
-self.pin_path(n)==RUNTIME_RUNNER
+self.pin_path(n)==RUNTIME_RUNNER.resolve()
```

On this Mac the frozen `/tmp/...runtime-v1.py` key resolves to `/private/tmp/...runtime-v1.py`. `pin_path` already resolves map keys; v2 resolves the expected path as well. The runtime runner and its required SHA `0df88d88...` remain unchanged. Hash validation, source/tool freeze, transaction/rollback logic and runtime argv are untouched.

Other path identity checks were inspected for this alias mistake: mutable path sets and replacement map keys already resolve both sides; the real production root is already canonical on this machine. The argv membership check intentionally requires the documented literal `/tmp/...runtime-v1.py` command spelling and remains unchanged; it does not compare a resolved pin key to a lexical path.

Eleven pure controls pass. They extract only the exact boolean pin guard from AST and evaluate it against a local pure Path helper, without importing, constructing or invoking the executor: actual frozen32 v1 reproduces false, actual frozen32 v2 succeeds, `/tmp` and `/private/tmp` forms succeed, wrong hash/wrong existing path/missing/mixed wrong hashes fail, original v1 and v2 copies retain exact bytes, and the production root is canonical. See `pin-identity-results.json`. No transaction/rollback mock or app run was repeated.

Parent invocation from repository root, with fresh output directories:

```sh
arc/tools/locked.sh python3 /tmp/arccore-sessions-adopt-source-only-v2.py \
  --evidence-dir /Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/source-only-adoption-transaction-v2/run-parent-v2 \
  --frozen-runtime-pins /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/pins.json \
  --frozen-runtime-pins-sha256 1324a413c955799e43fcd6054478e9ca645d5f11117d33025e992a5cee79505e \
  --runtime-command python3 /tmp/arccore-sessions-adoption-runtime-v1.py \
  --out /Users/cryogenicplanet/general/arc-engine/results/sessions/adoption-source-only-parent-v2 \
  --pins '{runtime_pins}' \
  --baseline-root /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/baseline \
  --full
```

Use the explicit v2 evidence directory; the single-line executor fix deliberately leaves its inherited default path untouched. Review and publication pins accompany this note. No live integration or acceptance is claimed.
