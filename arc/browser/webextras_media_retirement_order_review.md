# Media retirement order delta review — held normal path

Fresh read-only review, 2026-10-23 UTC. Only the separate retirement delta and
its evidence were reviewed; this does not repeat or promote the base Media
candidate. No source, frozen evidence, test, build, app, runtime, or lock was
changed or executed. Only this distinct review artifact was written.

Reviewed freeze `logs/wp/webextras-media-retirement-held/freeze.json` SHA256
`86f2fae7c6b7980d8af34fec07f60e9fc00c2b7fa8512625fd7f52e52786c2d7`.

| Artifact | SHA256 |
|---|---|
| Retirement patch | `c49d22f26e53c291cd2bf53e72803b96fec1ec54ed25063491e3099801a236fb` |
| Corrected Media source | `f5b7aa007e597dddf2adf15e78b2ccab088c2be0a86c736a34f66ee6c6d98396` |
| Combined counter/consumer source | `e38db1cddc3a2ec948d052218f733ac67d081aa78e2907ad91da20caa9dc4b82` |
| Retirement test | `d41debc45ebb9e8db77e69cadd58c070c29f8d3a18ca5e26a557ebd3fd355b58` |
| Native bounded attachment evidence | `2c95e580b2fb8c726b786901c0dcf1d032d11de1f13e07e2b50cba0bb4be7792` |

## Result

Clear within the stated unchanged-attachment normal-path scope. No new blocker
was found in this small order correction. It remains HELD, and does not close
CORE early-public-binding, whole logical-root/sibling lifetime, qualified
discard, repeated retirement, recursive cutover, or runtime gates.

The exact source delta moves PiPDestroyed before observer Detach and Source
weak-binding reset. It adds no Retiring receipt filter, expected-old check,
generation comparison, rollback, synthetic event, or state/cache reset. Existing
selected-bridge delivery, native value snapshots and accounting are unchanged.

Read actual native control flow, not instruction-address order: valid old weak
binding at435f9f0 branches to435fb30. After native retirement/binding/userdata gates,
435fb78 invokes the PiP peer while Media receiver, old userdata and physical weak
binding still exist. After callback return,435fb7c reloads userdata peer;
435fb98 clears it;435fb9c branches BACK to435f9f4. Receiver reset435f9f8 and weak
physical storage clear435fa00/18 then occur. No postcall expected-old identity
revalidation is present on this inspected helper tail. This supports the local
order change, not invented safe behavior under arbitrary recursive replacement.

My earlier detach-first interpretation in the frozen delivery audit was wrong.
The old ad88d9a2 note and revision2 prose/test expectation remain preserved,
but are superseded on this point by the corrected raw control-flow reading and
this held delta. Source receiver/binding observations are not proof that CORE's
public owner.webContents binding remains old during the app callback.

## Tests and checks inspected, without rerunning

Verified the freeze hash, all three owned hashes and all 30 frozen evidence
hashes. Compared corrected source against frozen revision2: only the ordering
and explanatory comment changed. Verified the old delivery note/manifest,
revision2 freeze and accounting freeze hashes remain unchanged.

The new test extracts actual Source retirement/PiP/cache/dispatch plus the actual
Info observer forwarder. Its mock transport delivers a nested Info receipt
during the PiP app callout, requiring receiver still attached and Source physical
binding uncleared. Real extracted cache/queue logic processes state1/camera2/
microphone1 and suppresses a later equal receipt. A task accepted before callback
return remains eligible afterward; only then are Detach and reset observed.
No production callback is synthesized by the patch; the test's mock receipt is
explicitly a test transport. This does not exercise genuine Chromium Mojo.

The same control compiled against immutable old revision2 returns deliberate
exit 71 with its expected old-order failure message, not an abort/crash. Corrected
control returns 0. The derived full delivery suite changes ONLY the obsolete
in-call detach expectation 1 to 0 and returns 0; its postcall expectation remains 1.
The composed actual-primitive accounting suite returns 0. None is described as
public binding, root lifetime, or recursive attachment coverage.

Recorded strict validation has 38 apply/check commands: baseline reconstruction
plus all six retirement/counter/consumer permutations, with byte equality asserted
across composed outputs. Two additional actual out/Base whole-Media TU checks,
baseline and combined, return 0 without diagnostics. Read validation script/result
and exact commands/overlays; no command was rerun during this review.

These results are sufficient to clear this bounded staged correction. Promotion
still requires a genuine early CORE retirement endpoint, correct authoritative
public binding and association lifetime, logical receipt locks and accepted
root/sibling retention, ownership/reentry decisions, and authorized integrated
validation. The late WebContentsDestroyed fallback is not made source-equivalent
by moving these two statements. No public Close activation is approved.
