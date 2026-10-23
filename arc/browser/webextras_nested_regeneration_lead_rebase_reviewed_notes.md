# Held corrected regeneration proposal after lead144766

**Sole current adoption candidate, still private:** `webextras_nested_regeneration_lead_rebase_reviewed.patch`, SHA256`303f8e07495ad3c763108bb22bfda115bcfcbb3299797b38dec6427283693227`. It applies DIRECTLY after actual `arc/tools/regen-patches.sh` SHA256`dabcc5255e59c45d64555f8f1adea81e6fc2f329a22349fe2738513919f5b151`, with the helper absent. It does not apply after any older proposal.

Owner ACK must name exactly these two shared-file results:

| Path | Proposed SHA-256 |
|---|---|
| `arc/tools/regen-patches.sh` | `4663d76ce54a06658172ee527896499ec448dc558b029cbea8e169f90ecd578c` |
| `arc/tools/regen-chromium-patches.py` | `c77d0180e6f0d64f28bfbe4537cee532387d3e9fdfa3c23c425ac4515ea908f9` |

Frozen05976 and c6f87b82 are **historical HOLD**, mutually exclusive with this direct variant. Their source, proposals, tests, notes, freeze and rejection evidence remain unchanged. The earlier05976 review correctly found that passing40 tests did not establish safe shared-index refusal or conflict-preserving rollback. The initial unfrozen corrected prototype also had a reproduced postimage-rescan gap; its proposal and the independent reproduction are retained. No older candidate should be adopted or restored as an engine-transaction recovery tool.

## Bounded contract

The shell preserves lead144766's context cleanup and real-checkout root guard, using physical paths. Both wrapper and helper ignore inherited `GIT_*` context; Git discovery cannot select a caller's `GIT_INDEX_FILE`, `GIT_DIR`, `GIT_WORK_TREE`, object/common directory, namespace, config or trace context. Helper root comes from its own copied file location; no caller-root/index option exists. Git optional index refreshes are disabled.

Each owning repository's real index is discovered first, then separately byte-copied into an internal temporary directory. Real HEAD, real-index bytes/path/dev/inode/size/mtime/ctime and index entry/flag state are checked; no real-index restoration is attempted. Exact manifest-assigned regular text leaves absent from both index and owning HEAD receive add-N only in that private copy. A HEAD-tracked leaf omitted from the real index is refused. Current real ITA entries are supported; no global untracked allowance exists. The known frontend gitlink's outer HEAD/index commit and independent nested HEAD must agree.

Only `third_party/devtools-frontend/src` is supported as a nested checkout. Manifest ownership remains exact-once outer-relative paths. Generation uses trusted Git diff **HEAD → final worktree** (staged plus unstaged contents), full outer prefixes for nested hunks, and no external diff/textconv/renames/color. HEAD-tracked-never-new checks use owning Git name/status and HEAD metadata, not hunk text. Git's own numstat validates generated text/path completeness. No arbitrary patch API, handwritten diff parser or sync change is added. `sync-chromium.sh` remains SHA256`5973cb9d53fde94f969362b033fbb645fe840e63283a1983829e4621db720923`.

## Corrections verified in this variant

1. **Shared-index metadata:** immediately after the non-index-reading Git path discovery, the helper refuses any `sharedindex.*` entry beside that real index, BEFORE index-reading Git commands. It performs no shared-index read probe, deletion or restoration. Even stale shared files in an ordinary unsplit checkout conservatively refuse; this is an explicit support limit. Missing shared links fail closed. Private commands disable split-index creation. Tests snapshot bytes, mode, device/inode, size, mtime and ctime for real fixture indices/shared files and verify preservation for outer, nested and wrapper refusals. This addresses the observed Git2.50.1 probe freshening; no universal filesystem-hermetic claim is made.
2. **Source-mode drift:** source fingerprints now use stable regular-file lstat identity, including mode/dev/inode/size/mtime/ctime plus content hash. An already-modified file changing644→755 during generation is rejected even when its bytes and porcelain status remain unchanged. The external source change is left alone; outputs are not published. The historical05976 acceptance of that exact change is retained in `historical-mode-probe.json`.
3. **Foreign rollback conflicts:** initial destination identities and generated→prepared bytes/mode are checked, then prepared identity and the old destination are rechecked before publication. Expected postimages originate from our prepared inode/hash/mode, not a later foreign scan. A single stable post-publication read must match prepared digest/dev/inode/size/mode/mtime before its record can enter the final expected fingerprint; rename-related ctime is captured only in that validated record. Rollback restores/deletes only destinations still matching our prepared postimage. Foreign bytes, mode changes, replacement inode, absence or symlink are preserved and produce explicit **HOLD: rollback conflict preserved**; uncontested published outputs can still be restored atomically. The reproduced ownership-check→blind-rescan adoption gap now rejects and preserves the foreign update.

All changed outputs are prepared before publication and replaced atomically per file. Identical patches are not replaced, preserving their bytes and metadata. Ordinary preparation failure leaves outputs untouched; ordinary later failure restores only uncontested published files through atomic replacement. This is not a power-loss multi-file transaction or a portable atomic conditional filesystem update. The caller's integration lock is still required; check→replace races against an uncooperative writer cannot be eliminated by these checks. A conflict HOLD requires owner reconciliation, not forced restoration/replay.

## Evidence and limits

`WEBEXTRAS_REGEN_TEST_EVIDENCE=<owned receipt directory> python3 -B arc/browser/webextras_nested_regeneration_lead_rebase_reviewed_test.py`: **52 groups PASS**, recorded in `logs/wp/webextras-nested-regeneration-lead-rebase-reviewed/test-corrected.log`. Test receipts default to disposable space. Fixture Git starts with sanitized context, and poison tests use only an explicit disposable context map. Every executed script/helper is copied; no production generator, lock or diff-run is invoked.

The actual direct patch is privately applied and must reconstruct the staged files exactly. The suite retains the40 native/5-frontend/full-prefix/foreign-text/original-sync/forward-reverse/index/poison/HEAD/ownership/binary/color/new-leaf/mode/ordinary-I/O controls, then adds full shared-file metadata, missing/stale shared state, source-mode drift, foreign byte/mode/inode/missing/symlink preservation, prepared-inode provenance and the exact late-postscan regression. The original sync loop replays all five frontend files plus the native binding, genuinely unindexed hook and independent foreign changes byte exactly; real fixture indices remain unchanged. Earlier failed development logs are historical, not readiness evidence.

The separate **private** native-only tools-replacement oracle preserves all three fixture native leaves/two patch outputs, real indices, sync bytes and unchanged patch inode/mtime/ctime. It does not qualify the actual production all-native artifact set. Fresh independent final review and its extra controls are separate from the writer's52-group result. `freeze.json` pins inputs, owned files and evidence; preservation receipts verify the historical05976/c6 artifacts.

## Standalone adoption request

Request exact owner ACK for the two hashes above as a **standalone persistent shared-tool baseline**, before a later engine journal. Do not include the tool as an engine target whose rollback restores dabc and invokes its real-index add-N/unstaged-only/nonnested route. No actual adoption or lock operation was performed here.

The authorized standalone adoption must pass the parent's pre/post **actual all-native artifact oracle** apart from these two tool files, preserving the existing manifest, maintained patches, sources, real indices/HEAD and build/binary guards. If final-worktree semantics or the conservative shared-file guard expose disagreement, HOLD for the tool owner; do not silently rewrite native artifacts or delete shared-index files. That actual oracle and owner ACK remain outstanding. Later frontend integration separately requires manifest registration, resource build/package, launch-slice9/9 and own-scenario no regression.

No actual source/tool/MANIFEST/maintained patch/index/lock/build/app/runtime/board mutation was performed. The proposal addresses nested serialization and Core144797's privately copied real-index request only; no architecture change or scenario acceptance is claimed.
