# Current-source composition verification

Both complete frozen alternatives apply to the CURRENT owned source, retaining both newer fixes. Both final owned translation units pass the actual Siso compiler command redirected to staged source/header overlays with output flags removed and `-fsyntax-only` added. Fresh independent read-only review: PASS.

This is compatibility evidence, not a new proposal, adoption, native compilation, link, runtime, or parity claim. No frozen patch/generator/stage, production/shared/golden source, app, object, shared build lock, board or credential was changed. New source stages/scripts are `/tmp/arccore-sessions-current-composition-*`; this evidence directory contains only manifests/check logs/README/reproduction script.

## Verified current baseline

- `arc/api/arc_sessions_restore.mm`: `b7c5c201a2395ecf9cd1bd736958432ed42076f2b970cdbaf627fd18e45e5206`
- `arc/api/arc_internal.h`: `70495f85414c996dc97262cc9670a9464451e9e215600c934cc71e46269a62a1`
- Current native baseline files copied directly from `chromium/src`, including current `arcmod_hooks.h`, restoration helper/factory headers and sources. Complete per-file hashes are in `manifest.json`.
- Frozen factory/TAB manifests checked before composition. All source/native patch hashes recorded; all patches and current baseline files rechecked unchanged after compiler checks.

## Alternative A: source-only

Applied in order to `/tmp/arccore-sessions-current-composition-source-only-stage`:

1. `/tmp/arccore-sessions-native-source-hooks.patch`
2. `/tmp/arccore-sessions-source-registration.patch`
3. `/tmp/arccore-sessions-factory-native.patch`
4. `/tmp/arccore-sessions-factory-owned.patch`
5. `/tmp/arccore-sessions-tab-factory-native.patch`
6. `/tmp/arccore-sessions-tab-factory-owned.patch`

Six `git apply --check` and six actual `git apply`: exit0. Final owned SHA256 `b7be71ed3837f2ca425f12cafbf8383829355f7aaa58093072133f728c2177e4`. Owned actual Siso syntax exit0, empty `source-only-owned.syntax.log`. Source-only header overlay correctly has source and factory hooks; no startup-policy declaration was added.

## Alternative B: combined source+policy

Applied in order to `/tmp/arccore-sessions-current-composition-source-policy-stage`:

1. `/tmp/arccore-sessions-native-source-policy.patch`
2. `/tmp/arccore-sessions-source-policy-registration.patch`
3. `/tmp/arccore-sessions-factory-native-after-source-policy.patch`
4. `/tmp/arccore-sessions-factory-owned-after-source-policy.patch`
5. `/tmp/arccore-sessions-tab-factory-native.patch`
6. `/tmp/arccore-sessions-tab-factory-owned.patch`

Six `git apply --check` and six actual `git apply`: exit0. Final owned SHA256 `83a1e9bfc05d4b38d8b0c4fe2d9439561b81301907de420a7700237853db681d`. Owned actual Siso syntax exit0, empty `source-policy-owned.syntax.log`. Its staged CURRENT `arc_internal.h` accepts the policy declaration hunk; staged native hooks include policy, source and factory declarations from this whole chain.

Do not mix the primary WINDOW factory header patch with the source-policy prerequisite; the explicitly named after-source-policy variant remains required. The same incremental TAB patches apply to either complete WINDOW alternative.

## New owned guards preserved

Guard checks ran BEFORE and AFTER every individual patch, not by starting from old frozen owned stages. Every check passed:

- `CHECK_GT(navigations.count, 0u);` remains exactly once.
- `QueueRestorationCallback` retains its Arc wrapper and tests current `[wrapper webContents]` at delivery, including after rebind.
- No old `nativeContents` WeakPtr or `contents->GetWeakPtr()` capture was reintroduced into that callback.

Final combined-source lines are navigation guard616 and current-binding comment/guard781/784; final source-only locations are available in its temp source. Native source-marker userdata WeakPtrs elsewhere are expected and do not replace this current-binding guard.

## Compiler verification and limits

`check.py` records the exact reproduction procedure. It obtains the real owned command via read-only `siso query commands`, selects only the arc_sessions_restore.mm compiler line (query emits a large dependency command graph), replaces its source with each final staged owned file, prepends that stage's include directory, strips `-c`, `-o`, dependency outputs, and redirects crash diagnostics to a new temp directory. It retains real include/config/ARC/plugin/`-Werror` flags and adds `-fsyntax-only`. Both diagnostics logs are empty; no object/dependency output is requested. Original and redirected argv arrays and removed flags are recorded in `manifest.json` / `owned-original-compiler-command.json`.

No context drift, compiler failure, guard loss, or frozen-input mutation was observed. Native translation units, linking, runtime factory behavior, and existing cancellation/group/partial-route/canonical-identity limitations were NOT revalidated here. All frozen proposal safety deviations and lead adoption prerequisites remain exactly as documented in their original READMEs. This evidence does not authorize activating hooks or consuming an old frozen owned stage over current source.
