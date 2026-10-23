# Isolated progress candidate — STAGED ONLY

Production base WC ff957456ece2014edd6a0dda7b1424438b0b2b903fccbf257aae5becb82ceed9. Candidate WC47534304f93ff381dbc00e1a080d7425caac808c6dd679825677f93c97201927. Source candidate is candidate/arc/api/arc_web_contents.mm. Parent is sole production writer; no source/app/build/lock/board/credential mutation here. No runtime or raw parity-row fix claimed.

## Exact semantics

core_progress_publication.patch changes ONLY LoadProgressChanged. Every received double is copied into an ordinary zero-delay UI task with an ObjC weak owner; delivery loads owner once with objc_precise_lifetime, conditionally invokes existing loadProgress setter, then returns with no native/observer access. No policy/generation/native-liveness gate, no value synthesis/coalescing change, no retained delegate/wrapper. The inline block->BindOnce->PostTask sequence is the same reviewed full-bundle PostCoreUI implementation. No shared header or GNI change. Existing navigation/loading/policy code, renderer/session/service integration, notifications and quit guard remain byte-for-byte unchanged. Native source receipt4389a3c→copied double4389fa4→UIpost438a06c–80→task4365eb4→weakowner4413558→setter441356c has no policy gate (core_progress_publication_notes.md). A KVO callout can reenter/delete native; no reads follow the setter.

Standalone leaves kProgress enum/double/else branch in the pending loading event structure; its sole producer has been replaced, so that branch becomes unreachable, without touching any loading/nav/policy semantics. The generic companion later removes it. No public Close activation, five-domain accounting or task cancellation/destructor decrement is added. Existing deferred Close remains. PostTask rejection/task cancellation has no invented alternate publication.

## Three alternative patches; do not apply both original and replacement companions

1. core_progress_publication.patch (0e8cd518...) against exact currentff957.
2. core_navigation_registration.after_progress.patch (ac1b3d7e...) REPLACES original generic companion when starting from progress candidate. It retains async progress, routes its block through generic's existing PostCoreUI helper, and otherwise targets the same original generic result.
3. core_observer_registration.after_progress.patch (b222ff87...) REPLACES original observer companion at its existing route position. Progress is already implemented, so that repeated replacement is omitted; other observer bytes are unchanged.

All original proposals remain untouched. No future patch was copied wholesale into production. Both variants are derived from exact pre/post source versions and strict full-route reconstruction, not guessed hunk line-number edits.

## Exact route proof

prepare.py first copies current production/native prerequisite files to /tmp/base. It strictly applies all original11 patches in logs/wp/core.integration-current order plus core_beforeunload_transport_chromium.patch. This original-route produces exactly the16 reviewed files from logs/wp/core.beforeunload-transport/manifest.json stage, byte-for-byte. It separately starts base+standalone progress, applies the same11 route with ONLY generic/observer replacements above, then requires exact full16-path/hash-map equality to the SAME reviewed target. Both pass. Direct standalone strict apply independently reconstructs candidate exactly. git apply --check and git apply --whitespace=error both pass at every step; apply-checks.json retains48 exact commands/results. Production inputs remain unchanged after syntax compilation. manifest.json lists every patch/base/final hash.

## Compiler proof

compile.py queries actual Siso obj/arc/arc_core/arc_web_contents.o command under chromium/src/out/Base; it substitutes ONLY candidate source and first include overlay. -c/object/dependency outputs are stripped and -fsyntax-only added. compiler.command.json and original-compiler.command.json retain exact flags; compiler-stripped.json records removed outputs. Direct clang exit0, diagnosticbytes0 in compiler.checks.json/compiler.log. No linked build/test/app invocation. Same final16 route bytes were already reviewed/compiled by parent; no redundant composed compiler run was needed.

## Limits and next step

Actual existing-golden loadProgress1/.7 differences may persist: raw evidence includes extra intermediate native.7 receipts and source coalescing is timing-dependent. This fixes unsupported sync/policy-dependent receipt loss, not native event equality. No forced terminal1 or count padding. Parent must obtain fresh code/route review and coordinate source freeze before promotion. Parent should promote narrow source and companion alternatives together, archive their hashes, then decide whether any ONE shared runtime is justified. No own waiter/job remains.

Durable evidence: logs/wp/core.progress-publication/. Parent fresh source/route review CLEAN; runner P2 .mojom seal closed and reviewed. Actual production promotion is deferred until renderer owner releases141601 freeze. No own job/waiter.
