# Executor v4: select Siso command bytes before UTF-8 decoding

**PRIVATE narrow fix only; no executor/query/compiler/live/index/lock/runtime/queue action.** Parent reported v3 retry61528 reaped1 / ABORTED_NO_WRITES at17:44:17. All six+host stages/header union succeeded, but decoding the entire recursive Siso log failed on unrelated invalid UTF-8 before compiler or live write. V3 script, failure and raw log remain unchanged.

V4 `/tmp/arccore-sessions-adopt-source-only-v4.py` SHA256 is `8701261ae1af7e765bad2207741df67f590a91ae2920118701e0f4652fc2e4fd`; `transaction.py` is byte-identical. It adds a pure bytes selector: identify a clang line with exactly one `-c` requested-source token and exactly one `-o` exact requested-target token, require a unique matching line, strictly UTF-8 decode only that line, then shlex split and verify those argv tokens again. Unrelated bytes are not decoded. Invalid UTF-8 in the selected argv is rejected, never ignored or replaced. Raw query logs and their hashes remain retained; the compiler record also includes queryLogSHA256.

All exception state serialization sites now use type(error).__name__ plus str(error)[:4096], avoiding repr(UnicodeDecodeError) embedding the full raw graph. Failure messages are bounded; raw evidence remains in its separate hashed log. V3 private-index/official-tool-pin/alias/transaction/runtime/rollback behavior is untouched.

Eleven pure controls pass against the preserved actual helper-query log (20,858,643 bytes; SHA256 `2ec2cf5e16b6c6d544dec727ef5b91ae3fb33c591db36d98fe1e065c3a157158`). The whole graph fails strict UTF-8, while the unique selected line is valid. Controls accept unrelated invalid bytes and quoted exact source/target tokens; reject selected invalid UTF-8, missing/ambiguous commands, output-prefix lookalikes and wrong source; verify bounded large error messages and unchanged original log. Only the two pure helper functions are extracted from AST, without importing, constructing or invoking the executor. See command-selection-results.json and fresh narrow review.

Parent invocation from repository root; both outputs must be fresh:

```sh
arc/tools/locked.sh python3 /tmp/arccore-sessions-adopt-source-only-v4.py \
  --evidence-dir /Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/source-only-adoption-transaction-v4/run-parent-v4 \
  --frozen-runtime-pins /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/pins.json \
  --frozen-runtime-pins-sha256 1324a413c955799e43fcd6054478e9ca645d5f11117d33025e992a5cee79505e \
  --runtime-command python3 /tmp/arccore-sessions-adoption-runtime-v1.py \
  --out /Users/cryogenicplanet/general/arc-engine/results/sessions/adoption-source-only-parent-v4 \
  --pins '{runtime_pins}' \
  --baseline-root /Users/cryogenicplanet/general/arc-engine/results/sessions/source-only-runtime-inputs-20261023T170004Z/baseline \
  --full
```

This proves only the narrow parsing/error-formatting fix. No live adoption or sessions acceptance is claimed. Explicit v4 evidence output avoids the inherited default path.
