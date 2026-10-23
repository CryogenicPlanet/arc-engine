# HELD initial webextras integration executor

This is private preparation, not an integration or queue receipt. No SEALED
configuration is supplied. `template` emits `status: UNSEALED`; `validate` and
`run` reject it before acquiring a lock or publishing a source. The reviewed
host gate, nested frontend regeneration tool/ACK, preserved current shared-header
bytes, final patch chain and all exact images must be sealed and freshly reviewed
by the parent before any real invocation. Existing wrappers are unchanged.

Only `webextras_initial_integration.py`, its test, these notes, the DISTINCT held
`webextras_scenario_resources_held.patch` and its test, the distinct comparator
closure increment/aggregate observation patch and test, and distinct preparation
logs were written. The actual existing scenario runner is unchanged.
Tests use temporary checkouts, mocked runtime
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
  harness helper, or to the fixed resolved Node/Python interpreters. This does
  not grant any sibling write authority. Resource checker/test/notes, reviewed
  freeze/review receipts, actual native DataPack cc/h/encoding header, official
  Grit `data_pack.py`/`constants.py`, and interpreters are mandatory pins. The
  UNSEALED template seeds the exact reviewed corrected-helper policy hashes.
  The attributed App444e parser source, bounded validation classification and
  final runtime-derivative review/freeze are also exact mandatory pins. The
  executor copies only the pure function; it never imports an App driver.
- `inventory`: exact BEFORE production file lists for `arc/api`, `arc/browser`
  and `arc/wp` (`.mm/.cc/.h/.gn/.gni`). Every listed member has an immutable pin or
  exact mutable image. Only exact newly owned targets are excluded from the
  stable list; image checks enforce their phase-specific absence/presence.
- Ordered `patches`: full path, SHA256 and checkout-relative working directory.
  Their bytes are pinned. Private reconstruction must produce every exact target
  postimage before publication. Each private working directory is initialized as
  a Git repository so `git apply` cannot discover the live checkout above it.
- `targets`: each source's `before` hash or null (original absence), `after` hash
  and pinned private postimage `blob`, plus explicit `beforeMode/afterMode`.
  Absence has null beforeMode; every new file has a sealed permission mode.
  Existing-file modes must remain equal. Shared regeneration tools are immutable
  baseline inputs and cannot be engine targets.
  `manifest` has the same shape and is
  journalled separately. No source deletion is currently supported.
- `patchOutputs`: every existing `patches/*.patch`, every pre/post MANIFEST owner,
  plus prospective `0010-webextras.patch`, with exact `before/after` hashes or
  absence. Full output inventories are checked at phase boundaries. Ownership
  must remain unique; existing assignments are preserved; newly owned native
  files use `0010-webextras.patch`.
- `gitRepos`: `path`, independently discovered actual `index`, original raw
  `sha256`, immutable original `head`, `logicalSeal: [entryCount, logicalSHA256]`,
  and a pinned raw original `snapshot` path. Snapshot hash and parsed seal must
  match the config, before any worker exists. Root/outer/nested repositories
  each have their own original contract. The raw hash is provenance, not a
  refreshed expectation. No field can be rebound after entry; worker and
  guardian use the same sealed original inputs. Old environmentKey/ITA fields
  remain rejected. The executor never stages, repairs flags or restores indices.
- `regeneration`: exactly argv `./arc/tools/regen-patches.sh`, cwd `.`, empty env
  overrides and `toolModes` for the two already-adopted immutable tools:
  wrapper0755, helper0644. Both source hashes must be pins. No adoption command
  exists. The separately approved303f8e07 baseline is wrapper4663d76c/helperc77d0180;
  its owner grant/adoption oracle must precede config sealing. Both candidate
  and recovery use this same baseline. Engine targets AND actual engine patch
  changed paths cannot include either tool. Trusted Git metadata is read in
  disposable directories; rename/copy operations are refused because apply's
  numstat alone reports only their destination. No handwritten diff-body parser
  or actual checkout Git command is used for this check. The helper discovers
  sanitized actual owning indices, makes its own internal copies, and performs
  exact manifest-only add-N there. Caller-selected ALT/ITA behavior is removed.
- `observationTool`: distinct `before/after/blob/patch` for ONLY the owned
  `webextras_scenario_current_run.py` resource/comparator-observation update. It is not an
  engine target or generic persistent-tool privilege. The tool patch is strictly
  reconstructed and adopted under the protected lock BEFORE the engine journal.
  Apply/post-proof failure restores it before any engine publication. Once
  independently adopted, engine rollback retains its five-key observation proof;
  completion receipts explicitly say so. This does not authorize retaining any
  global regeneration/sync tool without the lead's separate exact decision.
- `resourceChecks`: separately sealed candidate/recovery `argv/cwd/expectHostGate`.
  The exact CLI is resolved pinned Python, `-B`, the pinned helper path,
  `--expect-host-gate present|absent --output {resource_output}`, with cwd `.`.
  No `-c`, duplicate flag or unrelated program can hide the helper as an argument.
  Initial candidate uses `present`, pre-integration recovery uses `absent`;
  validate enforces those exact initial role modes, never an inferred skip.
  A future already-host-gated batch needs a separate reviewed policy change.
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

After worker reap and known child/app/helper drainage, the SAME guardian validates
the worker's durable observation-tool handoff before source phase checks. Receipt
image fields must match sealed before/after/blob/patch values and actual runner
SHA; persistent adoption must agree with the completion retention bit and policy.
A refused-before-attempt completion may have no receipt only with retained=false
and the original runner bytes. Reverted refusal requires its exact durable receipt.
ADOPTING, unknown, missing adopted, mismatched or nonboolean evidence HOLDs. The
refresh changes only the validated tool flag; lock stamp, groups and config digest
are not reset by constructing a new guardian. This is a separate correction after
immutable09de/1b7, whose cached pre-worker false flag would otherwise falsely HOLD
successful persistent adoption. It does not relax foreign source/index guards.

Entry preimages include absent state and sealed file modes. Publication is journalled
BEFORE the first atomic source replacement. Phase receipts freeze image hashes
and immutable input pins. Phase checks and each publication/rollback compare-and-
swap verify BOTH source bytes and sealed modes for changed targets/MANIFEST.
Journalled rollback modes must agree with sealed beforeMode; publication uses
sealed afterMode, preserving existing executable bits. All MANIFEST native inputs
are guarded. Before each
build the pinned canonical runner's actual dependency closure must be declared
and match the sealed source/tool inputs, including sibling native helper files.
Inherited `GIT_*` routing/config-injection environment is stripped for Git
operations; global/system Git configuration is disabled, optional locks are off,
and no caller index is supplied. Local repo config and HEAD/index identity
remain guarded as part of the reviewed checkout. The generator owns its internal
private-copy isolation independently.

Logical validation uses the EXACT attributed pure App444e function, function
SHA14cf9aa8377da51031990cfb99868a7386c88aa2bbab5196f8bfb0c1a4b90024.
Source444e2c07, parent bounded-classificationec464fe3 and final review27fa4631 /
freezeddb03b30 are pinned. Historical validation was72 true cases plus the focused
split rejection, not a73-PASS whole batch. The function is unchanged; optimized
Python is rejected before execution so its assert checks remain active.

At a drained boundary, a stable current raw image must retain the ORIGINAL full
logical tuple and HEAD. The tuple protects every path, mode, OID, stage,
assume-valid, ITA and skip-worktree bit. Only stat fields, v2/v3/v4 encoding and
bounded known optional cache frames may differ. Cache payloads remain opaque;
this is not full Git cache validation. Unknown, checksum/framing-invalid,
split/sparse layouts and shared-index files refuse before index-reading Git.
Only non-index-reading rev-parse queries identify actual index path and HEAD.
There is no ls-files diagnostic probe; failures retain a bounded text reason.
Original raw snapshot/hash and logical expectations never refresh, and a later
semantic change rejects even after multiple equivalent refreshes.

Every ENTIRE owned regeneration opens a durable ledger before its command and
checks exact raw+physical identity/HEAD in finally, including nonzero return and
exception. Any drift/uncertain check permanently latches the output directory
before later regeneration, rollback, build, runtime or release can proceed. A
new logical-equivalent image cannot clear that latch or start a replacement
lease. The worker keeps an in-memory failure flag too; a separate guardian
requires absent latch and complete matching CHECKED ledger. If latch storage
fails, the still-OPEN ledger blocks a new guardian. Failure never restores a real
index. This is process-survival evidence, not a power-loss or adversarial atomic
filesystem transaction, and no HOLD-release IPC exists.

On candidate failure, the executor first requires quiet command/app/helper
processes and confirmed original-profile restoration for every started scenario.
Only exact journalled before/after source and patch bytes can be restored. Unknown
or foreign edits cause HOLD. Sources/MANIFEST are restored and patches regenerated
with the same immutable generator baseline, and an originally absent now-unreferenced own patch removed
only if its bytes are the sealed candidate image. Baseline package recovery then
calls the existing runner: rebuild, package, and exact accepted launch9/9. A failed
or ambiguous recovery HOLDs rather than releasing a stale candidate package.
The separately adopted owned observation tool stays installed; `ROLLED_BACK`
does not mean that every tool file was reverted to its pre-tool image.

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
  actual packaged executable/helper/recorder/**resources.pak** identities, and verified profile
  root restoration. Profile/cache dev+ino roots must match transaction entry;
  cache contents are not restored or claimed isolated. An unexpected new cache
  root HOLDs for parent preservation/recovery instead of deleting it.
- Every successful scenario gate, including recovery launch, is followed BEFORE
  the next scenario by the exact pinned resource checker. CLI0 alone is not a
  proof. Receipt mode and passed/match booleans, all three exact textual resource
  identities, native/Grit/decoder source pins, and built/package SHA must agree with
  BOTH runtime resource identities and current packaged bytes. Receipts and raw
  decoded/generated archives are retained. Historical run1 four-key identities
  remain historical; new candidate/recovery outputs must contain the fifth key.
  `nativePackSanity` must contain exactly intermediate/built/packaged records,
  each version5 with native-loader and writer-determinism checks true. This is a
  post-runtime proof before the next scenario, not a prelaunch interception.
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

The unit suite covers successful private reconstruction, guard-only-index
regeneration dispatch, absent-file/orphan-patch rollback, baseline package-recovery phase,
partial publication, regeneration failure, cancellation, child/group drainage
after bookkeeping failure, worker-death guardian HOLD, unknown profile/helper
recovery, foreign source/patch/index preservation, output and new-header
inventory, strict unsealed/ACK/pin/run-number/CLI refusal, known DevTools failure
and genuine seven-row improvement, dirty global diff handling, missing or changed
runtime evidence, actual binary drift, duplicate coverage, and eleven fatal
marker negatives. Runtime commands and basic fixture index guards are mocked.
A separate test executes only COPIED reviewed303 tools against disposable real
outer/nested repositories, including an immutable unindexed manifest leaf. It
proves candidate output, exact recovery outputs, unchanged actual fixture index
identities and unchanged baseline tool bytes/modes. No production tool or engine
runtime has been executed.

Four separate resource-observation tests extract the ACTUAL held runner's
identity functions into a private framework fixture. They prove stable five-key
identity, exact fixed PAK path, changed-PAK rejection, absent/symlink rejection,
and the old four-key false-negative control. The actual runner is hash-guarded
unchanged throughout. These are observation tests, not native PAK validity tests.

No actual integration configuration is ready. Fresh independent source/test
review, host-gate review, corrected regeneration helper/ACK, current-base staging
and exact config sealing remain mandatory before the parent queues anything.
Resource checker ca14402b remains historical HOLD: its permissive original Grit
reader alone does not prove PAK validity. The only supported helper is corrected
`webextras_frontend_resource_checked.py` aee01b90, freeze18c51bd6 and fresh review
2d22fc23. These exact reviewed policy/source pins are explicit; no automatic
version selection or actual generated-resource/PAK access was performed here.

The frozen resource-only patch d14b121b is unchanged. A distinct
`webextras_scenario_comparator_closure_held.patch` applies after it, and
`webextras_scenario_observation_held.patch` provides the byte-identical aggregate
for the one-patch persistent observation-tool interface. Both are proposals only.
The comparator increment source-loads reviewed f60537a3 freezer bytes, includes
its actual `comparison_files(ROOT)` closure in every phase inventory, and freezes
an isolated `canonical-comparator` root. The broader source archive contains
production headers and is never used as an executable comparator import root.
Every comparison verifies that closed snapshot before and after execution. Its
independent fixture canary can honestly be pending on a legacy comparator;
canonical comparisons always execute the unchanged DEFAULT policy, without
`--fixture-image`. This diagnostic is never runtime acceptance. The executor also
loads the sealed runner from checked source bytes, bypassing Python bytecode.

Nine private comparator-observation tests exercise exact aggregate composition,
legacy-four/current-seven input default reports, source dependency inventory,
missing helper, during-comparison tamper, unchecked source bytecode substitution,
and snapshot bytecode rejection. Reviewed freezer closure tests are replayed
separately. Actual existing runner/source bytes remain guarded unchanged.

The f39dc85e executor snapshot is historical: its default publication permission
0644 would have removed regen-patches.sh's0755 mode. The distinct mode correction
archives exact f39 source/test/notes before editing, adds explicit sealed image
modes and guards, and tests real temporary0755 publication/rollback and foreign
chmod refusal. No production tool/source mode was changed or repaired here.


The distinct post23dec no-write refusal increment does not validate a new
baseline. Before spawning the worker, the SAME guardian records sealed config,
launch/protected receipt identity, lock identity, genuine profile/cache root
identities (including absence), and the runner/trusted executor SHA, mode,
device/inode and file timestamps. Equal-byte replacement or chmod is uncertain.
The durable snapshot is also held in guardian memory and checked after reaping.

Only exact worker exit1, REFUSED_BEFORE_PUBLICATION, retained false, and an exact
false failure.sourcePublished may use this path. The entire output inventory
must contain only startup/failure/completion evidence and bounded read-only
index diagnostics; any observation attempt, phase input/child/log, publication,
journal, unknown file, directory or symlink blocks this exception. A reverted
observation receipt uses the existing strict handoff/source/index checks.
Matched worker PID/nonce, config, lock, trusted files and root identities and a
quiet owned worker/phase/app/helper census remain mandatory. Foreign source,
HEAD, staging and index bytes are not certified or overwritten by this path.

The release classification is REFUSED_WITHOUT_WRITES_NOT_CURRENT_BASELINE with
currentBaselineAsserted false, boundaryParity NOT ASSERTED and no normal OS exit
claim. Missing/ambiguous evidence retains the lock. This exception supplies no
runtime acceptance or authority to apply a stale config. Core integration and
regeneration proposals remain independently unsealed; the earlier c6/d90
regeneration proposal and stale11611 packet are historical HOLD after the lead
changed the accepted tool/HEAD epoch. Actual production/shared sources, tools,
real Git indices, apps and profiles were not modified during these private tests.


The distinct generator-baseline adaptation archives exact a80 source/test/notes,
its no-write increment and all review links before editing. It introduces no
shared-tool adoption path. The old current-config packets and earlier c6/d90
assumptions stay historical UNSEALED/HOLD; all11 entry and12 prospective output
images, current source/HEAD/index pins and exact owner grant must be independently
refreshed after older owners finish. Private tests model an already-adopted303
baseline; they neither claim actual adoption nor capture the current live epoch.
The generator's separate52-group private evidence remains pinned and was not
rerun as part of this adapter. Native all-artifact adoption proof remains outside
this executor increment.

The bbf1 alias-corrected baseline retained strict raw guards and is archived
unchanged. This separately authorized logical-index increment changes only the
boundary/owned-regeneration index contract described above. Source, mode, alias,
process, profile, guardian and early no-write refusal guards remain; neither
refusal nor logical equivalence supplies a baseline-validity/parity claim.


The frozen972f/368dc7b0 adapter is historical HOLD, not current eligibility.
Fresh review proved that the standard chromium/src/arc symlink can alias a
canonical immutable regeneration tool: lexical-only target/patch exclusions
allowed actual private prepare+publish and then pinned rollback refused it.
The distinct post972f correction preserves that source, tests, notes and patch
before adding one consistent engine_path guard. Every mutable path component
(including existing parents of absent future targets) must be nonsymlink, and
normalized resolved identity cannot name either canonical tool. Patch working
directories and EVERY Git-derived changed path receive the same check; checking
only targets would still admit an alias hidden in mixed patch metadata.

The guard is also rechecked by actual phase reads and prepare/publish/rollback
write boundaries. Canonical nonaliased nested checkout directories remain valid.
Read-only pin resolution is unchanged. This is not an atomic defense against an
uncooperative concurrent path replacement; the integration lock and existing
source/CAS guards remain required. Canonical copied303 candidate/recovery proof,
raw index strictness and guardian/no-write behavior are unchanged. New disposable
full prepare/publish negatives cover both wrapper and helper aliases, alongside
mixed metadata without alias targets, rename refusal, normalized spellings and
missing targets under aliased parents. No actual production tool/source/index or
runtime operation was performed, and no current config was refreshed or sealed.


The postbbf1 logical increment preserves exact source/test/notes, alias patch,
a02c freeze and5699517e review before editing. All current configs remain
UNSEALED/historical; actual source/HEAD/index epochs were neither captured nor
read. Shared303 adoption authority is still a separate prerequisite. Private
fixtures cover equivalent stat/cache/version refreshes, all logical fields and
HEADs, immutable original anchoring, unsupported formats before Git probes,
raw/physical drift during successful/failed/raising regeneration, durable latch
across worker/guardian instances and failed latch storage, plus the previous
mode/alias/guardian/no-write controls. Tests use only generated indices and
COPIED helpers in disposable repositories. No actual production tool, Git
index, lock, build, app/runtime or source epoch operation occurred.
