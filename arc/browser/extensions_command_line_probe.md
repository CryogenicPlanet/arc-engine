> Superseded by the combined `extensions_positive_probe.patch`, now applied to the owned helper sources. Do not apply this historical patch separately. Runtime verification of the combined version is pending.

# Deferred command-line fixture phase

Unapplied proposal: `extensions_command_line_probe.patch`. Only the patch and this document are written in the actual checkout. All proposed helper/runner source, compiled outputs, and syntax-check fixtures are under `/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-cli-proposal-ngbvcxj5`. No apps, shared builds, locks, actual fixture directories, or existing runtime artifacts were touched.

## Mode and ownership

The default remains `public-install`. Explicit `--fixture-mode=command-line` (also `--fixture-mode command-line`) is a separate fixture environment, not a Web Store or public-install acceptance claim.

Both engines receive the same canonical absolute nonce A/B paths in one `--load-extension=A,B` argument. Delimiter-containing command-line paths are rejected. Generated manifest/worker/popup/icon files are hashed; immutable fixture inputs and full launch intents are saved before each app launch, then bound to its exact PID. The existing engine/source/recorder/Mach-O provenance gates and PID-bound helper/native-window/CDP readiness gates remain.

The helper starts with separate startup-pending A/B ownership. `adopt-command-line` uses actual public list/getInfo results: exact nonce name, version `1.2.3`, real ID matching the lookup, and `fromWebstore=false`. It rejects duplicate IDs for a label. It emits no fabricated install callback/completion. Public A/B installation is forbidden in this mode, since it could replace the command-line location with ordinary unpacked policy.

A startup intent resolves only after the actual fixture appears in native info. Readiness, elapsed time, an empty registry, and process exit do not prove a missing command-line installation finished. Unresolved ownership remains in the journal and prevents cleanup certification. Cleanup retries native list reconciliation, verifies owned IDs, uninstalls only proven fixtures, and restores the baseline. Baseline pins are captured after adoption, excluding only the actual adopted IDs while preserving all other order. The original public-install cleanup body is retained behind a CLI-only preparation branch.

Every real callback entering before CLI adoption is independently journaled as `startup-observation`, including callbacks during incomplete discovery requests. Its actual operation attribution is preserved separately, so discarding unsuccessful discovery observations cannot remove lifecycle evidence from the paired startup stream. No install responses are invented. Other late/unassigned fixture diagnostics still fail the run.

## Operations and reload

Public-install operation order/assertions stay unchanged. CLI mode skips only public A/B installation. It retains malformed-manifest installation, explicit public enable attempts and strict enabled preconditions, commands and real effects, pin dedup/reordering, negative MV3 background JavaScript, disable/enable, popup attempts, reload, uninstall and cleanup.

All positive command/pin/popup operations occur before CLI reload. The CLI reload result must contain a typed Boolean `enabled`; that actual value is preserved exactly for paired comparison, with no asserted true/false default. Current Chromium can reload a fresh command-line extension through ordinary `UnpackedInstaller::Load` because unpacked-location prefs omit the cached manifest; it may therefore become disabled under developer-mode policy. There is no reinstall, policy override, or automatic repair. The public-install reload assertion remains unchanged.

The command listener uses the already-applied audited no-tab payload measurements; Action-B still requires its real extension ID and numeric tab ID/exact nonce URL. Reserved-command handling, positive legacy background JavaScript, and browser category pin APIs are not added.

## Evidence model with recorder v2

Recorder v2 excludes calls originating from the probe image. A successful helper operation writes its actual semantic observations and an explicit required-row sidecar. Required rows require actual fields with validated native JSON types (metadata, native list/info/commands, real completions/callbacks and completion/delegate interleaving, effects, pin/ownership state as applicable); they are not fabricated recorder events. Transport PID is independently checked, and only the already-documented transport/engine-label/fixture-scope exclusions apply to comparison.

`comparedOperationCount` counts valid paired helper observations with complete required rows even when standard recorder slices are empty. Exact semantic/required-row differences remain failures. Known mismatches take precedence even with zero valid observations; zero paired observations cannot exit successfully. `observedParity` concerns helper observations only and requires at least one valid pair.

Standard `diff.py` also drops non-app probe-image calls under v2 caller attribution. No verified paired fixture-call/return inventory or explicit fixture-image opt-in is available here. Every operation therefore reports method coverage unavailable, `boundaryMethodParity=null`, and standard/overall parity remains null unless there is a known failure (which remains false). Nonempty retained app/engine rows do not establish fixture-call coverage either. True caller labels are preserved in scoped recordings and retained-row inventory sidecars; no app attribution is fabricated. Startup callbacks have their own exact semantic sidecar. None of this establishes canonical scenario acceptance.

## Validation

All final temporary checks passed:

- `xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework AppKit` against the proposed `.m`, output only under the temporary directory.
- Existing local `tsgo --ignoreConfig --noEmit --strict --noUncheckedIndexedAccess --exactOptionalPropertyTypes --skipLibCheck --target ESNext --module ESNext --moduleResolution Bundler --types bun` against the temporary mirrored `.ts`.
- Bun bundle, generated worker `node --check`, existing runner `bash -n`, and unapplied `git apply --check`.
- Temporary schema/failure-precedence checks: Boolean rejection, missing required rows, malformed callback/completion evidence, known semantic/required/boundary mismatches at zero and one paired observation, zero-observation exit failure, and semantic-only evidence retaining unknown method parity. These execute no app callbacks or runtime requests.

Fresh read-only reviewer approved the final callback retention, false-precedence/zero-count handling, mode/adoption/cleanup behavior and explicit unknown fixture method coverage. No remaining blocking findings; runtime is unverified.

Freeze audit: actual `.m`, `.ts`, `.sh`, and native context content and mtimes remained unchanged across this task. The parent rebuilt the existing helper artifacts while validation5 was queued: the dylib retained its prior hash with a later mtime, and bundled JavaScript changed from `89e2e6ce...` to `b01f591b...`. This worker wrote temporary compile/bundle outputs only; no runtime artifact was overwritten by this proposal.
Patch bases:

- `.m`: `c36966c10fbac3a1eae4536261ae10afc7a15ca37745a2018848accdc11f9871`
- `.ts`: `3d5b2f22972d7fd0e7a828254796bb743678ff46d15dec2d7124ed2901657355`

Validation artifacts: `clang.log`, `tsgo.log`, `bun.log`, `worker.log`, `bash.log`, `apply-check.log`, `evidence-logic.log`, `frozen.json`, and the later `freeze-check.json` under the temporary directory above.

## Remaining runtime risks

The stock engine may ignore or reject command-line loading because of branding, Enhanced Protection or installation policy. No startup result is synthesized in that case, and missing ownership stays unresolved. Native popup presentation may remain unavailable under a locked desktop. Startup callback timing may differ between engines; exact observed differences are retained. Registry absence plus restored pins certifies the existing public-boundary cleanup scope, not asynchronous on-disk deletion completion. A startup failure before usable native services may require a later lead-directed cleanup and must not be reported clean.

## Optional fresh-profile follow-up, not implemented

A dirty shared disposable profile may prevent original app-window readiness. A later lead-owned phase can stage fresh owned data, but must preserve the existing `ArcModAgent-trace156`/`ours156` data rather than reset it. Before any move or copy, acquire the shared slot, verify exact disposable bundle identity and that its executable has no live PID, journal canonical data/backup paths and ownership, and keep recoverable backups until both engine cleanup and process exit are established. Chromium `--user-data-dir` alone may not isolate the native Arc app state used for readiness; that path must be verified separately before claiming a fresh profile. This proposal performs no profile move/reset and adds no fresh-profile flag.

Future invocation, after the extensions worker applies the reviewed patch,
rebuilds the helper, and acquires the shared runtime lock:

```sh
arc/tools/locked.sh arc/browser/extensions_service_probe.sh run \
  --out results/extensions-service-probe/cli-UNIQUE \
  --fixture-mode=command-line --agents trace156,ours156 --port 9458
```
