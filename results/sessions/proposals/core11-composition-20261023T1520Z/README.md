# Private core11 / sessions composition

**All four requested routes are incomplete because the exact frozen patches conflict.** Each route stopped at its first failed strict apply check. No conflict repair, rebase, alternative patch, whole-file replacement or syntax compile was performed. This is compatibility evidence only, with no grant/adoption, native/link/runtime, public Close, functional-peer or session-acceptance claim.

## Exact inputs and route outcomes

Current production inputs were verified as sessions source **a6475f82…**, core WebContents **ff957456…**, internal header **70495f85…**, and native arcmod hooks **a6bb3a03…**. Full hashes, all affected baseline file paths/bytes, frozen patch hashes, per-step logs and stage roots are in [manifest.json](manifest.json). All core manifest production baselines and all sessions frozen patches match. The eleven core patches are the current ten in `logs/wp/core.integration-current/manifest.json`, followed by exact `core_beforeunload_transport_chromium.patch` **e57ccbd991e538d860ed62f496d4f40046afccc27fea3e54ca3fcff0dd170625** from the BeforeUnload manifest.

| Private route | Successful strict check + apply pairs | First failed check |
|---|---:|---|
|source-only core11 → sessions7|11|sessions native-source-hooks, step12|
|source-policy core11 → sessions7|11|sessions native-source-policy, step12|
|source-only sessions7 → core11|8 (sessions7 + core navigation1)|core close-transport, step9|
|source-policy sessions7 → core11|8 (sessions7 + core navigation1)|core close-transport, step9|

Total: **38 successful dry-run/apply pairs and four failed dry-runs**. Every failed command is `git apply --check --verbose`; the failed patch was not applied, including its other hunks. No complete core+sessions route exists among these four stages, so the conditional actual-Siso query and syntax compiler were **not invoked**. Older standalone syntax passes do not substitute for this missing composed check.

## Exact conflict

Every failed route reports:

```text
error: patch failed: chrome/browser/arcmod/arcmod_hooks.h:43
error: chrome/browser/arcmod/arcmod_hooks.h: patch does not apply
```

The frozen hunks require the original adjacency of `namespace arcmod {` and the navigation-throttle comment. The first chain has already inserted its own hook declarations there:

- core-first/source-only: sessions hunk `@@ -43,6 +45,15 @@` in `/tmp/arccore-sessions-native-source-hooks.patch`; full diagnostic in `source-only-core-then-sessions-12.apply.log`.
- core-first/policy: sessions hunk `@@ -43,6 +45,24 @@` in `/tmp/arccore-sessions-native-source-policy.patch`; `source-policy-core-then-sessions-12.apply.log`.
- both sessions-first routes: core hunk `@@ -43,6 +43,19 @@` in `arc/browser/core_close_transport_chromium.patch`; the respective `*-sessions-then-core-9.apply.log`.

This is a frozen patch-context conflict, not a demonstrated declaration/type conflict, runtime defect or evidence that the desired semantics cannot coexist. No overlapping-context repair was attempted.

## Header prerequisites, preservation and limits

The two core-first stages reconstruct all eleven core patches, including the independently recorded BeforeUnload header/delegate hashes. Relevant resulting headers:

| Header | SHA-256 |
|---|---|
|chrome/browser/arcmod/arcmod_hooks.h|27e5d1f5435f6f11f34f8d957dd47a4fc22851da3e6d78894de63d4f17b52b6d|
|content/public/browser/host_app_hooks.h|48f3aae4107e6880c8797cc7db11ac810cc8b7c7fba6d63a3d34db05b52a4ad3|
|arc/browser/core_close_adapters.h|65003d80ed5b36e2fbf3220ab4daa1937437ae79271346935e5c546d2c5fccd8|
|arc/browser/core_close_state.h|1f0fa0fecfdaa8bd8933ca88115e5b4fa34235c14248bfa36aba6ca1fa19ac1f|

The sessions-first stages reach the two full sessions7 owned hashes recorded in the prior1423 manifest: source-only **7a370597…**, source-policy **0aa0f890…**. Their session native hook headers are **8d528a44…** and **10d92a6d…**, respectively; core accounting headers are absent because the core chain stops before primitive/adapters creation. They are not silently supplied from an old overlay and these incomplete states are not compiled.

Every successful step and failed-check boundary preserves the current setter and QueueRestorationCallback byte for byte: strong array publication before enumeration, no incoming-count callout, `CHECK_GT(entries.size(),0u)` before commit, current-wrapper native-binding delivery check, and no old native-WebContents weak capture. Core-first stages retain core's reviewed quit guard through adapter/noncreating patches; sessions-first leaves production ff957 core WC untouched at the stopping point. These are composition preservation facts, not producer-accounting/cancellation behavior validation.

All recorded production baseline bytes, all frozen patch bytes and all three input manifests remain unchanged after this task. The script and four uniquely named stages are private `/tmp` artifacts; only this evidence directory was written in the checkout. No object/dependency outputs, native TU compilation, build/gn/autoninja/siso build, app/probe execution, runtime waiter, shared lock job, test/source/shared mutation, board or credential action occurred.

Independent fresh read-only review by `/root/sessions_recon/full_probe_review` confirms the24 baseline states,22 frozen patch hashes, three manifests, all final staged hashes,38 successful check/apply pairs, four failed checks and no compiler execution. Additional read-only reconstruction checks are in `fresh-review-checks.json`. Both full core11-only stages match every core final hash with the two exact BeforeUnload replacements.
