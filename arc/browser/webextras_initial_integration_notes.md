# HELD initial webextras integration executor

This is private preparation, not an integration or queue receipt. No SEALED
configuration is supplied. `template` emits `status: UNSEALED`; `validate` and
`run` reject it before acquiring a lock or publishing a source. The reviewed
host gate, nested frontend regeneration tool/ACK, preserved current shared-header
bytes, final patch chain and all exact images must be sealed and freshly reviewed
by the parent before any real invocation. Existing wrappers are unchanged.

Only `webextras_initial_integration.py`, its test, these notes and distinct
preparation logs were written. Tests use temporary checkouts, mocked runtime
commands, real private `git init/apply`, and extracted execution/gate functions.
No real build, app, profile/cache, source publication, shared lock or board write
was performed. These tests are not runtime acceptance.

## Sealed configuration

The parent supplies a JSON file and its exact SHA256. There is no automatic
sealing, rebasing, configuration discovery, tool upgrade or adoption command.
Required fields are:

- `schema: 1`, `status: SEALED`, exact `repo`, and nonempty `acknowledgments`
  references for `hostGate` and `regenerationInterface`.
- `pins`: immutable source/tool/proposal/private-image/harness/golden dependencies.
  Ordinary keys are safe checkout-relative paths. Read-only absolute pins may
  point into this checkout or the sibling Arc checkout, for the existing native
  harness helper. This does not grant any sibling write authority.
- `inventory`: exact BEFORE production file lists for `arc/api`, `arc/browser`
  and `arc/wp` (`.mm/.cc/.h/.gn/.gni`). Every listed member has an immutable pin or
  exact mutable image. Only exact newly owned targets are excluded from the
  stable list; image checks enforce their phase-specific absence/presence.
- Ordered `patches`: full path, SHA256 and checkout-relative working directory.
  Their bytes are pinned. Private reconstruction must produce every exact target
  postimage before publication. Each private working directory is initialized as
  a Git repository so `git apply` cannot discover the live checkout above it.
- `targets`: each source's `before` hash or null (original absence), `after` hash
  and pinned private postimage `blob`. `manifest` has the same shape and is
  journalled separately. No source deletion is currently supported.
- `patchOutputs`: every existing `patches/*.patch`, every pre/post MANIFEST owner,
  plus prospective `0010-webextras.patch`, with exact `before/after` hashes or
  absence. Full output inventories are checked at phase boundaries. Ownership
  must remain unique; existing assignments are preserved; newly owned native
  files use `0010-webextras.patch`.
- `gitRepos`: `path`, actual `index` path, raw index SHA256, `head`, sealed
  `environmentKey` and exact `intentToAdd` new-owned relative paths. Each actual
  index path is independently discovered. Index copies are private. Only those
  copies can receive `git add -N`, and they reset to the guarded entry image
  before rollback regeneration. The real indexes are never written/restored.
- `regeneration`: exact reviewed `argv`, `cwd`, `env`. Alternate-index environment
  names are explicit configuration, not guessed frontend tool interfaces.
  The pending helper must correctly scope outer versus nested repository index
  handling. An inherited outer `GIT_INDEX_FILE` is not a valid nested index.
- `baselineFiles`: independently frozen current-run1 artifacts for each scenario;
  at minimum steps, coverage, runner summary, recovery, input manifest and both
  binary identity records. These baselines used different engine binaries and
  must not be described as one source/binary snapshot.
- `runNumbers`: distinct increasing integers above1 for launch-slice, devtools,
  media, pdf, media-pip, then `recovery-launch-slice`. All result paths must be
  fresh. Each run intentionally uses the existing scenario runner and rebuilds/
  packages; there is no new reuse-build mode.

After current other-owner integrations finish, strict patch reconstruction and
full postimages must be refreshed in a private stage and reviewed. Strict hunk
application alone does not authorize overwriting a current header with an older
whole-file image. The sealed before/after hashes must preserve foreign additions.

## Execution and rollback contract

`run` starts a new protected session. `_protected` ignores INT/TERM/HUP BEFORE
execing `locked.sh`; the shell lock owner inherits those dispositions. The
guardian and worker require that exact owner in their ancestry, with stable lock
directory/PID-file identity. They defer cancellation through a durable request;
handlers do not raise or terminate children. Cancellation stops the next
candidate phase after the current phase drains. A late cancellation prevents
guardian acceptance and HOLDs for parent reconciliation.

Every command gets a fresh process group. After spawn, even a receipt-write
failure drains the leader and remaining non-zombie group members. Group census
failure does not authorize release. No process is killed automatically. Exact
disposable app/framework helper processes must be absent before mutation,
rollback and release. Parent retains all reaping/reconciliation authority.

The guardian does not trust worker exit alone: a missing completion, unexpected
worker exit, changed source/index/config, live phase/app/helper or un-restored
profile/cache root identity HOLDs the protected lock. SIGKILL, reboot and arbitrary
descendants escaping into unrelated new sessions are outside this protection.

Entry preimages include absent state and file modes. Publication is journalled
BEFORE the first atomic source replacement. Phase receipts freeze image hashes
and immutable input pins. All MANIFEST native inputs are guarded. Before each
build the pinned canonical runner's actual dependency closure must be declared
and match the sealed source/tool inputs, including sibling native helper files.

Raw index byte drift always fails closed. A bounded read-only `ls-files
--stage -v --debug` diagnostic is retained where detected, with optional Git locks
disabled. Suspected metadata-only refresh is not silently blessed. After source
publication, index drift blocks rollback and HOLDs; the executor never restores
the real index or overwrites actual staging.

On candidate failure, the executor first requires quiet command/app/helper
processes and confirmed original-profile restoration for every started scenario.
Only exact journalled before/after source and patch bytes can be restored. Unknown
or foreign edits cause HOLD. Sources/MANIFEST are restored, private indexes reset,
patches regenerated, and an originally absent now-unreferenced own patch removed
only if its bytes are the sealed candidate image. Baseline package recovery then
calls the existing runner: rebuild, package, and exact accepted launch9/9. A failed
or ambiguous recovery HOLDs rather than releasing a stale candidate package.

## Independent runtime gates

Runner exit0 is not boundary parity; runner exit1 can mean an ordinary dirty
global comparator report. Both are preserved. Operational errors, source/input
drift, missing raw evidence, profile recovery uncertainty or fatal app markers
cannot be waived by a known-failing baseline.

- Launch requires the exact nine canonical labels/order, all passing assertions,
  harness0, runtimeAccepted true, accepted coverage and exactly two launches.
- Media/PDF/PiP require exact accepted-golden labels/order, all pass, harness0,
  accepted coverage and exactly one launch.
- DevTools allows either its exact frozen six-row baseline with both unchanged
  native-control inconclusive details, harness1 and unaccepted coverage, or a
  complete improvement to the accepted golden's SEVEN real assertion rows,
  harness0 and accepted coverage. Partial/new failure shapes are rejected.
- All runs require correct ours156 app/recorded receipts, exact ordered unique
  launch inventory, natural Browser.close receipts, nonempty hooked raw traces,
  complete raw default comparator JSON/text, immutable source phase receipts,
  actual packaged executable/helper/recorder identities, and verified profile
  root restoration. Profile/cache dev+ino roots must match transaction entry;
  cache contents are not restored or claimed isolated. An unexpected new cache
  root HOLDs for parent preservation/recovery instead of deleting it.
- Fatal markers include standard Chromium FATAL/DFATAL prefixes, Check/Assertion
  failure, EXC_BAD_ACCESS/INSTRUCTION, SIGABRT/SIGSEGV, relevant Received signal,
  crash-thread and fatal-error markers. This is an app-log audit, not a complete
  OS crash census.

All full raw diffs remain at their original paths. No mismatch normalization,
caller relabelling, count padding or comparison suppression is added. The result
is explicitly **no earlier canonical step regression**, not callback/argument/
receiver/order parity. A dirty report still needs the parent's owned-boundary
assessment. Natural quit receipts plus helper absence do not capture or prove a
normal OS exit status.

## Private checks and remaining review

The unit suite covers successful private reconstruction, alternate-index-only
regeneration, absent-file/orphan-patch rollback, baseline package-recovery phase,
partial publication, regeneration failure, cancellation, child/group drainage
after bookkeeping failure, worker-death guardian HOLD, unknown profile/helper
recovery, foreign source/patch/index preservation, output and new-header
inventory, strict unsealed/ACK/pin/run-number/CLI refusal, known DevTools failure
and genuine seven-row improvement, dirty global diff handling, missing or changed
runtime evidence, actual binary drift, duplicate coverage, and eleven fatal
marker negatives. Runtime commands and the Git host index are mocked; no real
engine/tool interface has been executed.

No actual integration configuration is ready. Fresh independent source/test
review, host-gate review, corrected regeneration helper/ACK, current-base staging
and exact config sealing remain mandatory before the parent queues anything.
