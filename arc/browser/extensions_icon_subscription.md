**Parent status: APPLIED after validation7. Official check-file passes; runtime pending validation8. Durable evidence copied unchanged to logs/wp/extensions-icon-subscription-proposal. Do not reapply this historical proposal.**

# Unapplied container icon subscription correction

This is a bounded owned proposal, **not applied to product code**. Validation7 was active/frozen while it was prepared. Only `extensions_icon_subscription.patch`, this document, and temporary evidence under `/tmp/arc-extensions-icon-subscription-v1/` were written. No app, build, shared lock, fixture, helper, recorder output, or board action was used. Temporary syntax-only compiler checks do not write object/dependency outputs.

The repository-root patch removes the container's explicit `RegisterIconUpdateObserver` subscription, its map, and its now-unused clear/erase calls. It preserves ExtensionActionViewModel construction, the model-owned ExtensionActionIconFactory/default-image observation, cached action models, ToolbarActionsModel observer updates and throttling, active-tab observation, DidFinishNavigation updates, full refreshes, and all snapshot values. No app callback, string, count, or returned image is synthesized or masked.

## Exact frozen source and patch

| Item | SHA256 |
| --- | --- |
| Current product `arc/api/arc_extensions_browser.mm` | `b1ed32a375b12d6afd346a46b5697d1237c9267147acccdf9e908ea2a771ef42` |
| Proposed browser source | `08b4caeedfed9b9659498d950f45c71853751ae47e1ea32d19a83203597cf136` |
| Repository-root proposal patch | `bdbdc77609c4aa0d8fc991d5cc452b0be9487661a9e9a449e0c0a1ef9097f395` |

`git apply --check --whitespace=error arc/browser/extensions_icon_subscription.patch` succeeds against that exact current base. Applying it only in the temporary `applied/` tree produces byte-identical proposed source. Product source hash was rechecked unchanged after the compiler checks. Parent should recheck the base/apply check after validation7 ends before applying; this document does not authorize application during a frozen run.

## Bounded native producer proof

Read-only ARM64 evidence uses the exact pinned stock engine
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`, whole-image SHA256
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.
No native code was executed. Existing `/tmp/arc-profile-disasm.py` plus `/tmp/arc-ui-capstone` decoded complete bounded byte ranges. Raw byte hashes, all decoded instruction addresses, call lists, decoded byte totals, and relevant raw vtable bytes/hashes are in `native-evidence.json` and the named `.disasm` files. Every below range decoded completely, including cold branches; ends are exclusive.

| Body | Native byte range | Bytes | SHA256 |
| --- | --- | --- | --- |
| `container_ctor` | `0x43b5410`–`0x43b599c` | 1420 | `c7e357fc0d365f80b8da4f4f33beacfb2e274c46532a9f8193f99828c238f148` |
| `action_creation` | `0x43b6e20`–`0x43b7460` | 1600 | `f790789e7a12b8ca3e9ae4749b1608f0ea1e05503ad535668ee40c92394625c6` |
| `delegate_attach` | `0x43b3840`–`0x43b3984` | 324 | `780e71db565f9c2ebb1a4273be3e3ec80bee1e38247d01d1ae03f0048fa3baf7` |
| `model_ctor` | `0x7d1c6b4`–`0x7d1c86c` | 440 | `59904d2b2470e0b29bf3a5eb893fea2a784b4815eea5475a415edfd342f70f8c` |
| `register_icon_observer` | `0x7d1ca3c`–`0x7d1caac` | 112 | `9bd205b038f60df8e2717fd23dedbb94159931883887c93b48fc00262c947806` |
| `icon_notify_thunks` | `0x7d1dcd4`–`0x7d1dd40` | 108 | `66eac2e25178572d185b83a965e924aaa967f96ca80116d386cf472ca21bf21c` |

Native action creation `43b6e20` constructs the app action delegate (`43b6f18 →43b3768`), then the ordinary ExtensionActionViewModel (`43b6f28 →7d1c600`), inserts that model in the cache, allocates ID/owner state and per-ID throttle state, then registers its command (`43b7298 →7d1da04`). Its entire 1600-byte body has no indirect call and no call to the model's RegisterIconUpdateObserver implementation. The model-create routine `7d1c600` delegates to the complete model constructor `7d1c6b4`; that constructor initializes its own callback list, icon factory and command-service observation, then calls app delegate AttachToModel at `7d1c828 →43b3840`.

AttachToModel `43b3840` stores the model, creates the native auxiliary delegate, and observes the keyed command service. Its sole indirect call at `43b390c` receives the model's browser (`model+0x40`) and uses browser vtable slot+0x38 (GetProfile); it is not a virtual call on the action model. The model constructor's sole indirect call at `7d1c75c` likewise obtains browser profile. Container construction initializes browser/tab-list/toolbar observation and posts existing initialization; its indirect calls involve browser/tab-list/task-runner objects, not a model icon registration. The full bodies and registers are retained for inspection rather than inferring absence solely from a missing direct symbol.

The native model does have an icon subscription API: primary vtable `da0afd8` slot+0x18 is `7d1ca3c`, which adds a closure to its icon callback list at model+0x60. Native icon-observer secondary vtable `da0b0c8` slot+0x10 is `7d1dd08`; this adjusts to the model, verifies current contents through `7d1d874`, and notifies that callback list (`c1e45c`). No container subscriber is installed by the complete creation/attachment path above. Ours adds such a subscriber immediately after Create in GetActionForId and feeds that callback into its per-ID toolbar update throttle. Removing that extra container producer follows the native construction path; it does not remove native model icon ownership/loading or change toolbar/native navigation producers.

Native/current output icon request size already agrees: native snapshot `43b6788` sets both dimensions to 32, then `43b67a0 →7d1cab0` gets the icon; current Snapshot uses `gfx::Size(32,32)`. Underlying action default-icon loading 32/16 differences belong to a separate proposal/audit. This patch does not address them or claim they explain the callback counts.

## Runtime observations and limits

Validation6 CLI records six startup single-action observations in each engine. Original startup accessible-name sequence is B/A plain, B/A site-access, B/A site-access. Ours is B/A plain, B/A plain, B/A site-access, followed by two additional site-access singles inside snapshot-enabled-initial. That is evidence of differing callbacks/timing, **not attribution of those two callbacks to the explicit icon subscription**. Raw records do not identify the C++ producer. Startup entries remain startup observations and unassigned entries remain diagnostics; none is reassigned to hide a discrepancy.

The finalized page-actions golden and our validation6 each have two single-action updates. Active/navigation producers are independently proven and retained, so that count equality does not prove the extra icon route matches native behavior. Removal still needs a fresh real paired rerun of page-actions and the positive CLI phase after parent integration. This proposal claims no runtime improvement, callback parity, image/pixel equivalence, or package acceptance. A future icon-content-change case may exercise additional native model/ToolbarActionsModel behavior that existing goldens do not cover.

## Temporary validation and review

The exact current `obj/arc/arc_core/arc_extensions_browser.o` command was queried read-only from the existing build graph. Output and dependency flags were removed; `-fsyntax-only` and a VFS overlay were added. Both repository and `chromium/src/arc` symlink spellings map to the proposed temporary source with `use-external-names:false`. `browser.command.json` retains the complete stripped argv and original-command hash. The browser TU check exits 0 with empty stdout/stderr (`browser.result.json`).

A separate overlay prepends only `#error ARC_EXT_ICON_SUBSCRIPTION_OVERLAY_SENTINEL` to the temporary proposed source; the same exact TU command exits 1 with that unique diagnostic. This proves the check consumes the intended overlay instead of silently compiling product bytes. `negative.command.json`, output and result retain that proof. A source sentinel is sufficient here because the patch changes no header or consumer ABI.

Fresh independent read-only review by `/root/extension_review/icon_patch_review` found no actionable defects against the verified b1ed32 base. It independently confirmed no registration in the bounded creation/attachment/constructor paths, retained model icon ownership/loading and toolbar/explicit-setIcon/active/navigation producers, and no lifetime/API regression. Its conclusion is bounded static support, not whole-program absence or attribution of validation6 extra callbacks. It performed no edits, builds or app launches.
