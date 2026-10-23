# APPLIED extension startup initialization scheduling change

Owned patch: `arc/browser/extensions_startup_init.patch`. Evidence: `logs/wp/extensions-startup-init-proposal/`. Prepared only after the native construction report's final independent review PASS. The parent applied this owned-file patch under the shared lock during validation14, after the historical private checks below; no shared/helper/GNI source was changed by the patch.

| Item | SHA-256 |
|---|---|
| Exact browser base | `03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e` |
| Proposed browser | `99496aeb55684aac98f6b83bed1a8505a9e89ac06dbb107bac7fc50ccc2dd728` |
| Startup patch | `421323820a53dbb167a69519d00fbdc2bda51730bcbf8d8e6c52254cbdc5d0ec` |

## One-hunk behavior change

Current BrowserExtensions construction unconditionally calls `OnToolbarModelInitialized` synchronously. The proposal changes only that constructor tail:

- A nonnull, already-initialized ToolbarActionsModel posts a weak `OnToolbarModelInitialized` member callback through the current single-thread task runner, with no timer or artificial delay.
- A null or not-ready model does not request eager initialization. A nonnull not-ready model remains observed and uses its genuine `OnToolbarModelInitialized` notification.

The existing initializer, cache lookup/creation, full-snapshot producer, delegate transport, and shutdown remain byte-identical. No extra readiness check is added inside the initializer. The callback captures only weak self: it does not capture action IDs, snapshots, active tab, or delegate from construction. It reads current IDs/cache state and invokes the existing snapshot code when the deferred callback runs. This preserves the actual snapshot point instead of merely postponing delivery of a constructor-time payload.

Shutdown already invalidates the weak factory before timer/model teardown, canceling the pending callback. No additional cancellation flag, epoch, delegate-presence gate, registration boolean, wrapper creation, observer-list manipulation, or readiness notification is introduced. Existing SetDelegate cache preservation, omnibox guard, popup/keyboard handling, icon route removal, and all pin methods are preserved by the single-hunk scope.

## Final reviewed native construction proof

`logs/wp/extensions-keyboard-construction-lifecycle.md` SHA `aec47b99dd7038f50e442d603c6abb94b1614ba5a14a3901c0d918dc9b85d99f`; evidence JSON SHA `ca5e39fb5650bcddcdab38c40a3e04e0c91eb9437042116473225dabf8ac972a`. Its durable `review-status.json` is PASS; all 33 code and 12 data windows were independently checked. Copies of the final report, evidence, disassembly, manifest and review status are pinned in `native-proof.json`/`inputs/`. Native image SHA is `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`; addresses are unslid arm64 VAs.

| Native window | Byte SHA-256 | Proven operation |
|---|---|---|
| Manager constructor `0x43b5410–0x43b599c` | `c7e357fc0d365f80b8da4f4f33beacfb2e274c46532a9f8193f99828c238f148` | AddObserver at `0x43b5744`; null model / not-ready test at `0x43b5748–0x43b5754` returns through `0x43b5810`. Ready branch posts a weak callback at `0x43b580c`, delay zero. |
| Ready callback `0x40c5ad0–0x40c5b24` | `0bc0c6a86c45f9c711f655c8c25f653a21064edc41cfab1d2dc901f0ef4a65cf` | Weak flag/pointer guard, member `{0xa8,1}` → manager vtable +0xa8 → Init `0x43b7b88`. |
| Init and observer `0x43b7b88–0x43b7c48` | `cec104414e1f93f882f995cacd30c13037bfbb830c82d85e56a7622e2a7cfc60` | Null model returns; otherwise actual model IDs are created/cached before full publisher `0x43b618c`. No readiness or public delegate recheck. Secondary initialized observer `0x43b7be8` performs the same walk synchronously. |
| Toolbar OnReady `0x35d8688–0x35d8bf4` | `8dc6519637c57f0fa6a18c6efc1158f59b76a8398af02382697a51184e4f1864` | Builds IDs, stores ready=true at `0x35d8900`, then synchronously calls registered initialized observers at `0x35d8968`. |

Native AddObserver does not invoke the observer immediately. Current Chromium source corroborates the adaptation: AddObserver only adds the observer (`toolbar_actions_model.cc:99`); OnReady initializes IDs, sets `actions_initialized_=true`, then notifies (`323–341`). The ready accessor is a direct field read (`toolbar_actions_model.h:120`). The current TaskRunner API supports a posted zero-delay callback; no claim is made that the original concrete native runner is identical or that every possible nested pump waits until the enclosing constructor returns.

Native Init also performs initial RegisterCommand during missing-cache creation. This proposal does not implement the separately documented current action-command registration gap; it changes scheduling only. Full-snapshot optional-empty behavior, transport eligibility, weak-expiry/retained payload accounting, and the public wrapper's currently lazy construction remain separate work. No early ArcBrowser wrapper or native backend is created here to approximate the original ownership contract.

## Compile, preservation, and composition

The actual browser TU is checked through a symlink-aware VFS overlay using its own current `obj/arc/arc_core/arc_extensions_browser.o` command and original source spelling. Complete original command, stripped argv, output and flags are retained. Every non-output argument stays in order, including optimization, warning/error/plugin, Objective-C ARC, SDK/module and sanitizer options. Only dependency/object/serialized-diagnostic output options are removed; overlay, syntax-only and `-Wno-unused-command-line-argument` are added.

Positive compile **exit0**, empty diagnostics. The distinct `#error ARC_EXTENSION_STARTUP_INIT_NEGATIVE_BROWSER` overlay produces **exit1** with the sentinel, confirming the exact browser-TU invocation selected the candidate overlay. No build was executed; `siso query commands` only retrieved the current TU command. No object output, engine build/package, app, service probe, API, board or lock is used. This compiler check is separate from the concurrent frozen validation13 runtime.

Actual core window header during compilation is `ab5c8ed063dd6f968669e7beb5107575fa9849f449f26f5aa3a564a0c9657fc3`. Its implementation is recorded as current `8615c32234a29498c4b73ff50bbee78cdf87cb541424308bcaa109861795889a`, rather than assuming the earlier task's `5a32…` identity. The observed implementation change predates this proposal's input snapshot and was reported to the parent; this worker did not write it. The browser TU reads the actual header; the core implementation is provenance only and was not separately compiled here. Actual source/input hashes and mtimes are checked before/after.

Pin V8 remains authoritative and unmodified at `53537bc27d6267cb35fe3927c5752af95c50b2042e4eea1a723f1dfb755206ae`. Temporary startup→pin and pin→startup applications both pass strict whitespace checks and produce identical bytes across the three affected files (`pin-composition.json`). This is private composition proof, not a Pin V9 publication. The existing pin/native prerequisites remain unapplied. If the parent later activates startup initialization on a new actual browser base, the pin browser hunk coordinates/base documentation require their own subsequent refresh.

Fresh review checks the one-hunk scope, final native proof, weak cancellation, actual deferred snapshot point, current-header compiler records, and retained Pin V8 artifacts. `review.md`, input-after and artifact manifests retain the closing evidence.

## Integration request and limits

Keep this proposal unapplied through existing source freezes. The parent may choose a later separate runtime batch after any actual activation and official real-file check. No scenario/diff was run, no prior callback-order/count residual is claimed fixed, and package acceptance is **UNEXECUTED** for this change. The reviewed native construction evidence contradicts the current unconditional synchronous initializer, while broader early backend ownership and initial command registration remain unresolved. ARCHITECTURE's native-boundary acceptance requirement remains in force.

## Parent staged application

Validation14 applied the exact421323 patch after acquiring the shared lock at2026-10-23T14:33:14.867964Z. Exact030preimage became99496aeb55684aac98f6b83bed1a8505a9e89ac06dbb107bac7fc50ccc2dd728; official check-file exit0, no rollback. Receipt: results/extensions-validation14/startup-application.json; parent and monitor independently read actual postimage hash. Full build/runtime14 remains in progress at this note. Historical patch is not additive to current source. PinV9authoritative refresh is pending publication on994.
