# Distinct resource-aware native runner proposal

HELD / NOT INVOKED. This adds separate entrypoints and preserves every frozen
Native12 wrapper/helper and evidence byte. No engine build, app run, shared lock, profile,
shared/proposal source or index action was performed while preparing it.

New owned files:

- `webextras_native_resources_current_run.sh`: copy of the existing one-slot
  wrapper, adding resource capture after build, built-to-package proof after
  packaging, the new probe entrypoint, expanded source guards and final proof
  verification. The CLI/modes and runtime options are unchanged.
- `webextras_native_resources_probe.sh` / `.ts`: distinct copies using a separate
  probe build directory. The TS adds resources to the actual per-agent
  `binary-identity.json`, freezes the checker, binds ours to the held-slot package
  proof, and verifies each resource journal after existing process cleanup.
- `webextras_resource_identity.py`: stdlib file/inventory checker only. It does
  not decode PAKs, infer JavaScript dependencies, select different callbacks or
  change comparison/normalization policy.
- `webextras_resource_identity_test.py`: private filesystem and extracted actual
  TS/wrapper checks; no engine, app or shared lock is used.

## Resource contract

The checker inventories every regular file and directory below the direct
`Versions/A/Resources` directory, in sorted relative-path order. It records
SHA-256, size and stable device/inode/mtime/ctime identity and rejects nested
links and special entries. It rechecks tree inventory and metadata after
hashing. Nanosecond fields are decimal strings so the Python → TS journal →
Python round trip preserves them exactly.

Only built/ours roles require a nonempty `resources.pak`. The two scale packs
are recorded when present and covered by complete inventory equality and
unchanged checks; they are not claimed dependencies of the initial three
DevTools JS modules. `trace156` is literal inventory, with no imposed Chromium
PAK layout or built-C156 equality. Its resources may differ from ours.

The built snapshot is captured immediately after build and before packaging.
The checker then verifies that snapshot remains unchanged and compares its
complete relative file/directory inventory, hashes and sizes to the packaged
ours tree. Copy inode/timestamp differences are allowed in this content check.
The package proof records and verifies the same ancestor lock owner. This is
cooperative one-slot evidence, not a filesystem security boundary.

The new TS captures each actual packaged Resources tree before launch. Ours
must exactly match the packaged identity in that verified proof; original has
its own identity without cross-engine equality. Resources and checker/proof
identities are added directly to the new binary journal. After the existing
cleanup, the journal's resources must still equal the original in-memory
launch identity before and after checker execution. A rewritten journal cannot
authorize changed resources. Failures append to existing run errors and do not
erase earlier measurement failures or skip the remaining cleanup evidence.

Post-cleanup resource verification is new; post-cleanup binary revalidation is
not added. The original binary fields keep their existing launch-only meaning.
Full resource equality proves captured built bytes reached the packaged tree,
not that every byte was loaded or that a missing callback was repaired. The
existing separate reviewed PAK/JS check may be used by the parent in its held
slot; no redundant decoder is included here.

## Source scope and integration

The frontend-mode wrapper guard grows from15 selected inputs to21: it retains
the original inputs, adds the four new runtime source files and the two selected
packaging entry sources (`package-engine.sh`, `../arc/tools/build-mod.ts`).
This includes the resource-copy commands, not the complete transitive Arc/Chromium
build graph. The checker is copied read-only into the fresh run output and its
source/frozen identities are checked whenever it executes. Existing freezer
f605, frontend comparator helper1a2c, projection, recorder and comparison policy
are unchanged. Current legacy4 comparison remains pending; if the separately
reviewed comparator chain is adopted, the existing freezer discovers its local
closure and capability canaries must still verify it.

Prospective invocation, **only after material source adoption, fresh review and
parent authorization**:

```sh
bash arc/browser/webextras_native_resources_current_run.sh --run-number 13 --devtools-frontend-events
```

It uses the same four genuine inspect-on/off and right/bottom dock/restore
stimuli and the same5000/100/200ms deadlines. It self-acquires one shared slot
and performs build/package/probe there. The parent may instead invoke it with
`--locked` through a NEW reviewed Native13 reference-pin command. The Native12
pin script must not be reused: it writes Native12 paths and hardcodes12.

Fresh `results/webextras/native-run13` and the complete
`logs/wp/webextras.native.run13.*` namespace are required, including the new
`resources-built.json` and `resources-package.json`. Existing wrapper refusal
of prior output/source-before and exclusive manifest writes prevent overwriting
earlier results. New approval must bind the adopted production/helper/resource
inputs, this expanded wrapper closure and its final hashes. No run has been
queued and no resource bytes from a future build have been assumed.

## Verification and limits

The resource suite exercises real private file hashing, copy/content equality,
original-role layout separation, missing/empty main PAK, optional scale/nested
inventory mutations, links/FIFO, restored-mtime and inode replacement, concurrent
hash mutation, actual CLI/no-overwrite, extracted wrapper parser/source guards,
and actual TS journal/checker execution. The TS control preserves a preexisting
measurement error, detects a rewritten disk journal and a mutated frozen checker.
Private PID files reference the test's real ancestor PID solely to exercise the
ancestry function; no shared lock is acquired or altered.

The existing actual frontend17 and clock18 test bodies are also executed against
the new TS copy to verify their preserved producer, quiet, callback, default and
clock behavior. Bash syntax and Bun parsing are checked. The initial extracted
TS clean-journal test exposed nanosecond JSON precision loss; the final checker
uses strings and that real cross-language control passes. A read-only child
review found the mutable-journal gap; the binding checks and regression control
close it. Parent fresh review is still required before invocation.

Exact original input pins, baseline copies, narrow copy diffs, final checks and
freeze manifest are in `logs/wp/webextras-native-resources-proposal/`.
