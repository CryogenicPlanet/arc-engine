# Sync UI registration eligibility correction — isolated stage, HOLD

2026-10-23. Only `/tmp/core-ui-sync-scope-stage/` was written. No repo/production/native/shared source edits, app/run/attach, full build, build graph regeneration, lock, board or credential actions. Existing Siso was queried for its WebContents compile command; staged WC was syntax-checked only.

## Exact base/artifacts

The full 16-file stage named by `logs/wp/core.beforeunload-transport/manifest.json` was copied to `stage/`. This includes the unchanged composed full11 accounting and empty-default native transports. Base WC SHA256 `b6a66bacc567827d9507904f5058d916201cbe1a0f5a968c090559fec70032c1` matches the original eligibility audit input.

- `incremental.patch` SHA256 `dfe84311c2359de2acd2acd096fd6d84ffa904ebe080c130c20acfc59f4ad826`.
- Changed ONLY staged `arc/api/arc_web_contents.mm`, SHA256 `3b1c8e899f70131190e8468ff1a59ce60a5848d699fe24aa1b2218f70e920fd5`.
- `manifest.json` records every base/stage file hash and exact compiler/evidence hashes. All other15 files are unchanged.

## Three synchronous changes only

1. `NativeTargetURLChanged`: retains existing precise wrapper local and formatting before active bridge selection. Captures active CoreUIDelegateBridge into a strong `objc_precise_lifetime` local, then only a nonnil bridge enters the UI ScopedCoreCloseInvocation and existing selected inner delivery. Formatting remains outside that eligibility branch. Original Chrome-forwarding path and native hook behavior are untouched.
2. `nativeContentsClosing`: preserves `_closing` check/set exactly. Selects and precise-holds active bridge before creating UI scope; inside uses `bridge.delegate` instead of reading fresh `_uiDelegate`. SessionID construction and existing selector invocation remain in their original inner condition.
3. `nativeContentsDestroyed`: preserves quit-state snapshot, restoration ID, existing imminent call, `_contents.reset()` and all current phase/owner behavior. ONLY inside the existing nonquit Closed branch selects/precise-holds active bridge, then conditionally enters UI scope and selected inner callback. Quit continues to skip that branch without changing cleanup/imminent behavior.

At all three sites the selected lease is declared BEFORE the invocation scope and outside its nested block, so the selected bridge survives app callback, C++ scope destruction and restoration of the previous busy flag. A live registration enters busy even if its weak ObjC delegate is nil or does not support the selector. Delegate resolution/respondsToSelector is INNER work, never the eligibility gate. Replacement/nil-unregister during a running callback cannot prematurely destroy the selected bridge lease.

No blanket method early return was added. No async producer/helper, Navigation scope, provider/source gate, dispatcher guard, renderer file, UI delegate setter, physical cleanup phase or Close implementation was edited. The existing inaccurate stored-bridge comment was also deliberately left untouched to keep this increment to the requested three call sites.

## Source basis

Read both `logs/wp/core.ui-registration/core-ui-registration-audit.md` and `core-ui-registration-lifetime.md` completely. Original TargetURL438881c–8854 selects a live registration before busy4388878–8880 and restores43888c4. Generic InvokeSync4383be4–3c18 acquires selected registration, then saves/sets busy4383c3c/44, invokes4383c74, restores4383c8c and releases selected owner4383cb0. Imminent43834a0 and nonquit Closed438a23c use that shared InvokeSync. No delegate/selector predicate precedes native busy acquisition. This patch reflects that bounded eligibility and selected-owner lifetime order, not the still-missing native dispatcher machinery.

## Verification

- `git apply --check`: PASS against separate copy of the exact pinned base.
- Applied to `reconstructed/`: all16 reconstructed file hashes exactly match the staged16-file tree. Only WC differs from base.
- Actual Siso `obj/arc/arc_core/arc_web_contents.o` command saved as `original-command.txt`. Preserved compiler, defines, real includes, ARM64/SDK, ARC, warnings/plugins and C++ flags; prepended stage include overlay, removed original source/compile/object/dependency outputs and added staged source plus `-fsyntax-only`. Exact argv is `compiler-command.json`, SHA256 `41d4e8adbd66bd8ec014b5301c09b1d12d4e7f7b60e6abca8657b93673592414`.
- Staged WC syntax: **PASS exit0 with EMPTY stdout/stderr** (`compiler.log`,0 bytes). No object/dependency output requested. No fixture, full link/build or runtime comparison was run.

## HOLD and remaining gaps

Unapplied corrective increment only. Functional logical-self ownership, logical invocation-wrapper dispatcher guard/pre-lock bookkeeping and other producer eligibility remain incomplete. In particular LOAD/status and visible-security async acquisition, accepted/no-registration queued ownership and outer guard semantics are untouched. CoreCloseState or an ObjC bridge lease alone does not reproduce the original functional logical lifetime/dispatcher.

This does not establish runtime parity, acceptance, native Close safety, corrected Closed source phase, or permission to activate a partial accounting replacement. Parent will perform fresh review and archive the artifact; production/app freeze remains intact.

Fresh independent review CLEAN in logs/wp/core.ui-sync-scope/review.md. Parent archived exact candidate+command+raw evidence; this remains an incremental proposal AFTER full11 target, never an actual production patch.
