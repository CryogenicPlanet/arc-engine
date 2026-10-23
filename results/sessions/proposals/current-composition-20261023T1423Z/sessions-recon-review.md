# Fresh read-only composition review

Result: **bounded PASS; no actionable composition finding**.

This review reads the completed manifest, generator, frozen patch inputs, current baseline, staged outputs, typed header prerequisites, and recorded compiler commands/logs. It does not rerun a compiler or apply patches, change production/shared files, launch an app, or claim native compilation, linking, runtime parity, cancellation parity, or adoption approval.

## Input and chain verification

- Current owned input is SHA-256 `a6475f82e58de755fbf87578cc0dcb4144c0aeb685ce0073e707af442a7e37f0`; current `arc_internal.h` is `70495f85414c996dc97262cc9670a9464451e9e215600c934cc71e46269a62a1`. Both match the manifest. All nine baseline files, all eleven frozen patch files, and both prerequisite header files currently match their recorded hashes. Every staged-file hash matches the corresponding final manifest record.
- Both chains contain exactly seven patches in the declared order: source-only hooks/registration or combined source-policy hooks/registration, then the matching WINDOW native/owned pair, the common ungrouped ROOT TAB native/owned pair, and incremental close accounting. The manifest records all fourteen apply checks and fourteen applications as exit zero; every apply log remains empty.
- All before/after guard records are true for all fourteen steps. Both final setter regions are also byte-identical to current a647: strong array publication precedes enumeration; there is no incoming `navigations.count` callout; `CHECK_GT(entries.size(), 0u)` precedes vector assignment. Both final `QueueRestorationCallback` regions are byte-identical to current a647, preserving the current `[wrapper webContents]` delivery guard without capturing an old native WebContents weak pointer.
- Final owned hashes are `7a370597afd2479bb1ed84f3fbd9becea2b341bf495e76d68d2283a265950033` (source-only) and `0aa0f890d7b831e8612d8384403ef73a90e8f1e53e7914b2d0765c933d2dd12d` (source-policy).

## Explicit prerequisites and compiler scope

The stages intentionally supply core's typed accounting declarations from `logs/wp/core.accounting-adapters/stage/arc/browser/`. Both copies match their recorded source hashes: `core_close_adapters.h` is `a4f884eb10c6bb261735f3c633f51bc35c8f80315223480ed727d7a106dd2eec` and `core_close_state.h` is `1f0fa0fecfdaa8bd8933ca88115e5b4fa34235c14248bfa36aba6ca1fa19ac1f`. These declarations provide `CoreCloseStateForWebContents`, `CoreCloseDelegateChannel::kUI`, and `MakeCoreCloseAsyncInvocation` with the handle/closure types used by the accounting patch. This is a staged core prerequisite, not evidence that the definitions or caller integration have been installed or linked in production. The source-policy chain additionally carries the policy declaration in its staged current `arc_internal.h`; source-only correctly does not add it.

Each recorded compiler argv reconstructs exactly from the saved actual Siso command with only the documented source/include changes, removal of object/dependency/crash-output flags, and addition of syntax-only controls. `-Werror`, ARC, and the original compiler/plugin flags remain. The staged include directory is first. No `-c`, `-o`, dependency-output switch, or object-output path remains in the reconstructed command. The crash diagnostics directory is redirected to the corresponding temporary path. Both recorded owned syntax results are exit zero with empty logs whose hashes still match the manifest. These are owned-file syntax checks against the complete staged header chains, not native translation-unit or linker checks.

## Remaining limits

All previously held semantic limits remain: grouped TAB/partial-entry routing and created-callback cancellation safety/native-parity decisions are not validated by successful composition. This review does not relax the frozen proposal scope or the need for core accounting definitions and exact native hook integration. The historical b7 and failed 1421 compositions are not the basis for this pass.

Independent read-only second review (`/root/sessions_recon/full_probe_review`) also reports PASS with no concrete composition/command bug, and confirms the same bounded syntax-only scope.
