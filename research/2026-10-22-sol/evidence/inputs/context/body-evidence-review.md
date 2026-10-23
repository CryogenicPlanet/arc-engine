# Independent review of engine body findings

A fresh GPT-6.1-Sol reviewer inspected the completed ABI, CEF-route and Arc-layer body reports and sampled their retained pseudocode, bounded instruction evidence and exact upstream source. The review made no app launches, builds, network requests or runtime comparisons. It supports the sampled static findings below; it does not certify every body or convert inferred source identities into stable ABI guarantees.

## Required correction to closure counts

`collected/agents/arc-layer/decompile/report.md:17` calls 5,847 rows “possible mappings” and reports one ambiguity. Its `tools/audit_closure.py:39–48` drops metadata candidates without an exact C-body header. Its own audit summary correctly labels this count `resolved_export_rows`.

The broader metadata result is **5,859 candidate mappings and eight ambiguous normalized names**, as the ABI report at line 35 and CEF report at line 57 retain. Twelve omitted records include `ArcCpuInfo::armPartNumber @ 0xc482a80`, `hasSSE41 @ 0xc4a24e4`, six additional `ArcWebContentsConfiguration::.cxx_destruct` aliases and `_ChromeWebAppShortcutCopierMain @ 0xc47e10c`. The ABI `closure-audit.tsv` retains these records. Both counts can be reported with their definitions; the smaller exported-body subset must not replace metadata ambiguity or imply complete reachability.

## Supported sampled observations

- **Delegate removal differs.** ABI `binary-instructions.tsv:338` shows `0x4042030` branching directly to cleanup at `0x40420bc` for a nil downloads delegate, bypassing `_objc_storeWeak @ 0x4042068`. The context setter has the same cleanup-only shape. This does not prove cancellation of queued callbacks.
- **Navigation policy is synchronous at the bridge.** The send at `0x4445abc` is followed immediately by `cmp x0,#12; b.hs`. Accepted representations are 0–11; other values trap. Enum names are not established by that range.
- **DevTools's error-out argument is x3.** At `0x4415b74` the binary saves x3; it later forwards the saved value to `0x4402548`, corroborating rejection of Ghidra's apparent extra parameter. `0x435c8fc` copies callable state into a queued task and `0x435ca2c` invokes that callable. Ownership after return is supported; exact executor identity and suppression of already queued work are not.
- **Profile creation has a strong upstream identity.** `0x42b3d74` constructs callback binding and calls `0x7bdc470(name,0,hidden,binding,0)`. The unused-path loop, profile attributes, paired hidden flags and source location match `ProfileManager::CreateMultiProfileAsync` in `chrome/browser/profiles/profile_manager.cc:1037–1098` at `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`. This does not demonstrate unchanged implementation or replacement ABI compatibility.
- **Bootstrap reaches content process lifecycle.** The locale/signal, initialization/reinitialization and run sequence at `0x2d2f3cc` strongly matches `content::RunContentProcess` in the exact source. Keep this as a source-backed identity rather than a byte-equivalence claim.
- **Native hosting is a substantive feasibility gate.** `0x473cad8` checks host class, known subview and `ViewsHostable`, then conditionally calls `0x6846c88`. Its CATransaction/layer transfer/compositor behavior strongly matches `RenderWidgetHostViewMac::TakeFallbackContentFrom`. The pinned CEF macOS header forces Alloy style for `parent_view`. This supports a real native embedding constraint, not blanket CEF impossibility or demonstrated runtime failure.
- **Category and receiver distinctions matter.** Permission base `additionalData @ 0x2d0fb40` is a shared self+8 getter; its Extensions implementation at `0x440a85c` builds a dictionary. The sampled app `additionalData` send also calls autocomplete selectors and does not prove a permission-request receiver. Keep 834 as declarations.

## Synthesis gates

Recovered C++ prototypes must not be published as ABI specifications. The allocator at `0x786cf44`, for example, loses its returned value in pseudocode. Preserve instruction, metadata and source corroboration where needed.

Reconcile cross-report recovery status. Arc-layer's request for `_CallWithEHFrame` remains true as an exported-header gap, but ABI's binary/unwind investigation now explains the branch, special exception frame and landing-pad metadata. Do not call that contract wholly unresolved or replace it with a plain forwarding call.

All artifact paths above refer to the completed report snapshots under `collected/agents/` in this research directory. Detailed VA/source claims must retain the underlying engine/app image identities and static-versus-runtime boundaries in the final report.
