# Fresh review: CLEAR for corrected direct proposal 303f8e07

**CLEAR for the private reviewed tool proposal**, within its documented normal-index and caller-lock contract. This is not production adoption or runtime acceptance.

Reviewed patch: `webextras_nested_regeneration_lead_rebase_reviewed.patch` SHA256 `303f8e07495ad3c763108bb22bfda115bcfcbb3299797b38dec6427283693227`; freeze: `47f0dfc2950aba4732354244b615ee5df6172a20a856b8758d64a275b76b00c9`. The direct prerequisite is actual `regen-patches.sh` SHA256 `dabcc5255e59c45d64555f8f1adea81e6fc2f329a22349fe2738513919f5b151`, with the helper absent. Only two shared-file results are proposed:

| Path | Reviewed SHA-256 |
|---|---|
| `arc/tools/regen-patches.sh` | `4663d76ce54a06658172ee527896499ec448dc558b029cbea8e169f90ecd578c` |
| `arc/tools/regen-chromium-patches.py` | `c77d0180e6f0d64f28bfbe4537cee532387d3e9fdfa3c23c425ac4515ea908f9` |

The independent final suite passes **52 tests in 37.504 seconds** (`final52.log`). The actual proposal reconstructs the copied staged tools exactly. The suite covers all five frontend files, native binding, private registration of a genuinely unindexed manifest leaf, foreign C++ raw text, unchanged original-sync forward/reverse replay, native tools-only byte/metadata oracle, poisoned context, HEAD/index/ownership checks, source-mode drift and rollback conflicts. The commands run copied fixture tools only. Original `sync-chromium.sh` is unchanged; no applier or handwritten patch parser is introduced.

Three additional independent controls pass (`extra-controls.json`):

1. A nested checkout reached through a `.git` indirection file generates/replays byte exactly while retaining both real fixture indices' complete physical identity.
2. Refusing a real nested split index preserves all index/shared-file bytes, modes, device/inode, size, mtime and ctime, plus source and patch contents.
3. A foreign write immediately after the validated postimage read, before that record is stored, is rejected by the final fingerprint. The foreign bytes, mode and physical identity survive unchanged; the uncontested other output is restored and fixture indices remain unchanged.

The previously reproduced blockers are addressed. Shared-index entries are conservatively refused before index-reading Git, including stale entries; no real shared-index read probe remains. Source snapshots include regular-file mode and physical metadata. Generated output bytes/mode and prepared inode identity are checked before publication. A single stable post-publication record must match prepared digest/dev/inode/size/mode/mtime before entering the expected fingerprint. Rollback checks prepared ownership, preserves foreign bytes/mode/inode/missing/symlink conflicts and reports explicit HOLD. It does not adopt arbitrary postscan contents.

All **61 current freeze/input/owned/artifact pins** remain unchanged after tests; the actual helper is still absent. The writer's 79 historical05976/c6 preservation pins were independently rechecked unchanged. The prior HOLD review and all 96 hashes it references, plus its own manifest, were also rechecked unchanged. The old HOLD remains valid for its old bytes. Initial all-zero probe failure, shared-index freshening, source-mode acceptance, blind rollback and the unfrozen524de postscan adoption prototype are historical evidence, not results for this cleared variant. This directory retains the exact524de helper and positive reproduction; the first noninjected `/var` alias attempt is retained and explicitly distinguished.

The support limits are appropriate and explicit: any sibling `sharedindex.*` refuses, even when stale; unsupported state is preserved for owner reconciliation. Per-file atomic replacement is not a crash-safe multi-file transaction, and portable check/replace cannot guarantee conditional updates against an uncooperative racing writer. The caller integration lock remains required. These limitations do not invalidate the proved copied-fixture behavior.

Exact owner ACK for the two hashes above and the parent's actual production all-native artifact oracle remain outstanding. The private three-native-leaf/two-patch oracle does not establish that production gate. Adopt only as the separately authorized persistent tool baseline; do not recover an engine journal by restoring a historical generator. Frontend manifest/build/package/launch/scenario gates remain separate.

No production generator, lock or diff-run was invoked. No actual tools/source/MANIFEST/maintained patch/index/build/app/runtime/board state was modified by this review. The review manifest pins current inputs, historical protection and owned evidence.
