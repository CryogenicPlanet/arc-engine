# Independent transaction review

**PASS; no unresolved P1/P2 findings.** Reviewed frozen `/tmp/arccore-sessions-adopt-source-only.py` SHA256 `fb482e50efcd33fb7dcfacf353ecdcc17ce968815946801a89181ac51436484c`, byte-identical to `transaction.py`. Static AST parse succeeds. This review did not execute/import the transaction or mock driver, launch apps, build, take the shared lock, use Chirp, or change production.

## Resolved findings

- **R1 (P1) resolved:** verify_preimages and pre-regeneration checks compare maintained patch preimages; generated outputs are recorded before deferred signal delivery; final validation requires their exact bytes; rollback HOLD protects unexpected edits, orphan0010 is restored only from known images, and regen deltas need nonce-bound explicit review.
- **R2 (P1) resolved:** Runtime leader is waited, then a ps census drains every live non-zombie member remaining in its isolated PGID before returning. Trusted pinned v1 runner independently checks exact app/helper absence and restored profile/cache roots.
- **R3 (P2) resolved:** Popen plus child-pid event are inside try/finally; direct child wait and runtime group drain occur even when the postspawn event raises. Failure-path persistence is best effort and rollback failures retain/retry under the lock.

## Confirmed protections

- Frozen script and evidence transaction.py byte-identical; SHA256 checked; ast.parse succeeds without importing/executing executor.
- Lock owner PID must be a live ancestor at capture/publish/postimage validation.
- Exact e973 owned precondition, prior-hook rejection, proposal hash pins, target allowlist, six original nonheader patch stages plus exact host574.
- Reversible semantic header insertion is hash-pinned and removes back to all original current foreign bytes.
- Seven targets/MANIFEST/all referenced maintained patches are snapshotted before mutation.
- Four actual-Siso VFS syntax commands precede publication; official owned check follows regeneration; this review does not claim those production commands executed.
- Missing five native paths only receive0010; existing ownership is retained; header remains0004; duplicate paths rejected.
- Frozen runtime pin file has explicit SHA; immutable tools/goldens are reverified before/after waits. Only reviewed entry-source pins and postapply source/MANIFEST/generated patches are merged. Pinned v1 runner and standalone runtime_pins placeholder required.
- TERM/INT/HUP defer; all started direct children drain; runtime PGID drains; unknown cleanup needs nonce-bound manual confirmation; unexpected target/MANIFEST/patch changes HOLD instead of overwriting.
- Success requires code0, cleanupVerifiedtrue, launch9/9+accepted, canonical validation0/noEarlierRegression and explicit separate harness counts, preserving known canonical failures.
- Rollback restores only seven source preimages/MANIFEST, handles orphan0010, regenerates and checks hashes; unexpected maintained patch bytes or regenerated foreign deltas require explicit HOLD reconciliation.

## Existing mock evidence independently checked

All seven recorded private mock cases pass: foreign-header preservation, launch failure rollback, canonical validation failure rollback, regeneration failure rollback, SIGTERM child-drain rollback, cleanup HOLD plus nonce confirmation, and surviving same-group grandchild drainage. I verified script/result hashes, all seven log hashes, mock-root identities, transaction statuses, actual target postimage hashes, and exact MANIFEST/maintained-patch preimages for all five rollback cases. I read the mock driver and did not rerun it.

## Limits

- Same-PGID drainage does not prove absence of children that start independent sessions. The pinned trusted runner starts nested commands in new sessions and supplies their direct-child drainage and exact-agent/profile/cache cleanup contract. No blanket process-tree or OS-normal-exit claim.
- Seven mocks use private temporary roots and fake syntax/regen/runtime/lock ancestry; they do not prove actual compilation, integration authority, lock-wrapper signal behavior, runtime parity, or sessions acceptance.
- Foreign maintained-patch mutation/HOLD, target/MANIFEST drift, invalid nonce, INT/HUP, rollback-regeneration retry, wrong e973/alreadyhook, immutable-tool drift, and postspawn bookkeeping-exception cases were statically reviewed but are not among the seven executed mocks.
- Global regen may expose preexisting maintained-patch/source disagreement. The transaction holds for nonce-bound manual review of exact regenerated deltas rather than blindly restoring foreign patch bytes.

This is review approval for the bounded transaction implementation. It is not a production application, build/link/runtime result, or canonical sessions acceptance claim.
