# Private discard nil-owner bridge-policy candidate

HOLD: not applied and not compiled. This is a separate one-line candidate on production178091; it is not composed with the held global-close v2 proposal.

Original selected bridge4446b10 weak-loads the delegate into a strong local, weak-loads owner, then sends finalize with that owner and BOOL without an owner-nil branch. Delegate remains pinned through the send. The sole candidate removes `contents &&` from our callback guard so a surviving selected delegate may receive a nil owner. The delegate presence and optional-selector checks remain under ARCH.

This is a local bridge-policy correction, not proof of owner-expired delivery reachability. Captures, post-PM native-binding check, asynchronous scheduling, registration, pending decrement and all Root/lifetime behavior are unchanged. Full discard parity and canonical close/restart acceptance remain unmet. Raw proof and independent review are being completed in `results/sessions/discard-nil-owner-review/`.

No source, shared file, tool, GNI, Git, compiler, query, runtime, app or queue mutation. Production remains178091. Any future adoption needs current input reconciliation, real syntax check and required locked launch-slice9/9/package nonregression; no new runtime nil-owner probe is proposed.
