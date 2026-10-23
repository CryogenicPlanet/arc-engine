# Held fixture driver wiring proposal

**Owned driver changes ADOPTED after native4 finished; shared comparator UNAPPLIED.**
`webextras_native_probe.ts` and `webextras_values_probe.sh` now freeze comparison
inputs and validate actual public ancestry/projection. The original patch is a
historical adoption artifact: do not apply it again. Fresh independent review
confirmed unsupported fixture capability still permits both engines' actual
measurements, then forces pending status, complete=false/parity=false and exit1.
The unchanged shared comparator is never used as zero-diff fixture evidence.
Lead adoption of explicit fixture contract137084 is still required for accepted
comparison. No runtime job was active during this owned-only adoption.

## Adoption contents

- `webextras_fixture_driver_wiring.patch`: changes only the two owned drivers after application.
- `webextras_fixture_inputs.py`: freezes/verifies comparator inputs, proves fixture capability and checks actual retained coverage.
- `webextras_fixture_projection.ts`: projects the already validated native action trace onto public events.
- `webextras_fixture_driver_wiring_test.py`: recognizes the already adopted owned changes and applies the shared proposal only in a temporary repository and exercises the pure comparison path. It never starts an app, loads an engine, takes a lock or edits the actual drivers/shared comparator.

The active driver imports/invokes the owned helper and projector files alongside it. The native driver's existing recorder freeze remains independent and unchanged.

## Frozen input and capability contract

Before either agent launch/value-engine load, freeze `tests/recorder/diff.py`, `arc/tools/arccore-1.170.0-objc.tsv`, `tests/recorder/app_callbacks.inc`, and the Python helper's own bytes into one per-pair mirrored repository tree. Each source is hashed externally with `/usr/bin/shasum`, copied exclusively, made read-only, rehashed, and checked for stable source realpath/device/inode/size/mtime/ctime across the copy. Manifest records source/frozen SHA256 and identities. The native run/summary include this manifest; values saves `comparison-identity.json` and retains its unique comparator snapshot directory.

Both sides use that frozen tree. Verify every frozen input before each launch/load and before/after every comparison. Changes to shared source files after freezing do not change the comparison. Changed frozen bytes or metadata fail. The helper is invoked from the frozen tree after freezing, rather than loading a potentially changed source helper for each comparison.

Capability is verified once against the frozen comparator: two synthetic known-public `ArcMediaPosition.playbackRate` fixture calls return1 versus2. Require exactly the expected return-mismatch entry and exact `fixture_comparison` metadata (`mode=exact-image`, chosen image, one call+return per input), with no other mismatch sections. An old comparator that silently accepts an unknown flag and reports zero is unsupported. Without this proof, drivers retain pending fixture-comparison status and cannot report parity; they do not treat a default filtered diff as evidence.

Only after capability succeeds does actual comparison use `--fixture-image WebextrasNativeProbe.dylib` or `--fixture-image probe`. The helper validates caller presence on every measured call/return, exact admitted pair identity and completeness, nonzero evidence and exact raw/report fixture counts. The native driver additionally validates every call/return in the full raw recording for caller presence and matching call/return attribution. Its per-action expected selector counts are1 each except subscription3, totaling exactly13 calls+13 returns per input. Values retains its exact selector/cardinality/nesting validation and requires exactly72 calls+72 returns, all from `probe`. Missing/mixed attribution or zero/incomplete evidence fails.

## Native public trace projection

Full raw parsing, ambient-stack derivation, nesting/return matching, public invocation offsets/counts, forwarded app callback checks and timelines run **before** projection. For callback timing, test whether the measured public call is an actual raw-stack ancestor, rather than requiring an exact raw depth that internal helpers can increase. Keep offset/forwarding/receiver checks. Projection changes no raw record bytes or labels.

For `->engine`, select only frozen-metadata public methods from the existing official caller policy: `app`, the exact chosen fixture image, or `engine` with a `set*` selector. Unknown helper selectors are excluded using metadata; `initWith*` calls whose frozen encoding declares a pointer argument and whose trace carries `<ptr>` are treated as internal native constructors. An NSString argument containing the literal text `<ptr>` remains public. Keep all `engine->app` events. The original constructor/helper classification in the shared comparator remains unchanged.

Compute depth by counting selected ancestors inside each validated action, rather than counting skipped helpers or the ambient `ArcBrowserApplication.run`. Normalize receiver IDs only over selected events, using one identity context per agent across all actions. Preserve thread/direction/class/kind/selector/order/from and the projected depth. A legitimate helper/getter interleaving can differ without changing the compared public shape; an extra/missing public call, changed public order, cross-action receiver replacement or attribution change remains visible.

The projected shape keeps the prior identity/order fields only. It introduces **no argument/return normalization**. The frozen official comparator detects public return differences using its own existing normalization, including any separately adopted epoch/SessionID proposal. Existing action results, raw block JSON payload spelling, NSError payloads, nonce checks and callback timing/forwarding validations remain mandatory. Shared comparison still compares input arguments only for engine-to-app callbacks; it alone proves neither fixture input arguments nor full order/receiver/timing parity.

The native validator's no-projection fallback is retained only for replaying archived v1 evidence. Live runs always instantiate the frozen-metadata projection, demand v2 caller fields and require the capability gate. Legacy replay success is not v2 fixture acceptance.

## Verification and scope

```sh
python3 -B arc/browser/webextras_fixture_driver_wiring_test.py
```

Ten tests cover adopted/unapplied/partial/drift state checks and unsupported-capability measurement that still cannot claim acceptance, plus the ignored-old-flag failure mode, exact137084 canary metadata,13/72 counts, differing public returns, missing/mixed/zero/incomplete fixture evidence, each frozen input's tamper rejection, source replacement isolation, patched Bun parse and `bash -n`, all11 saved original native3 v1 action raw/outcome replays, and synthetic public projection. The synthetic projection tests permit helper/getter interleaving while preserving public depths, and reject changed public method order/receiver/missing calls plus malformed raw nesting. No production runtime acceptance is claimed.

Concurrent shared normalization remains lead-owned. Freeze the **adopted composite comparator** once; do not merge or duplicate normalization into these helpers. The explicit fixture metadata schema must remain the137084 contract or this capability probe fails closed. The lead should rebase this held driver patch if queued work or shared-owner changes alter the target files before adoption.
