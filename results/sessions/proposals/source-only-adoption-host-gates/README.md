# Source-only adoption: host gates

**Recommendation: apply this additive refinement immediately after the six original source-only patches.** It confines the new helper safety behavior to `--arcmod-host-app` and preserves the original nonhost control flow. No blocking issue was found in the scoped static audit; the fresh review is recorded separately.

`host-gating.patch` SHA256: `574e225c4e37e53ff4d77c84ffbcd22a861684fa9cfe5631a28ba89c5d274e03`.

The patch applies strictly to the exact source-only six-stage pinned by `current-six-live-base-e973/manifest.json`. It changes only `components/sessions/core/tab_restore_service_helper.cc`, using generic `base/command_line.h`. Helper weak-token acquisition, new post-callback liveness/null/root checks, cancellation returns and early `live_tab` null publication are host-only. Browser factory and source marker hooks were already host+nonempty-hook gated and remain unchanged. See `semantic-diff.md`, exact `before.cc`/`after.cc` and the patch.

Strict applicability succeeded; application in a fresh separate temporary tree produced exactly `after.cc`. The historical source-only stage, original proposals and all selected production inputs remained unchanged. `manifest.json` pins the bases, selected patches, before/after, private stage and freshness checks. The owned source and all other staged files remain exact.

Actual Siso-derived syntax checks pass for all three affected native translation units (helper, BrowserLiveTabContext and browser_tabrestore) and the owned sessions translation unit: all exit 0 with empty logs. Full argv are retained; ARC/Werror remain where applicable, and object/dependency outputs are stripped. `vfs-syntax.json` additionally records the same four syntax checks using the original logical source paths and an LLVM VFS overlay to the staged files. No native object/build/link/app/lock operation occurred.

This is an additive host-gating proposal, not adoption or runtime acceptance. Parent integrates under the shared lock, handles MANIFEST/regen, and validates launch-slice plus sessions scenarios with rollback as required by COMMON. Startup policy is excluded. Original-cancellation/canonical-browser questions and functional close/accounting/root decisions are not resolved or approved by this result.
