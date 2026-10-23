# Real logical native route — proposed-only / HOLD

Patch core_logical_native_route.patch SHA256 c5dc9cf6f71c66c83619164c1761e81d958f4162bc0c656986712746badc1460. Stage stage/ is exact reviewed bare library18 files plus THREE owned-file changes. Base is /tmp/core-logical-library-stage, not mutable production. Bare library patchd4b42613 and reviewed full11/pre-beforeunload16 source are verified in manifest.json. No production/repo/shared/native edits, GNI change, apps, linked build, lock, board, credential, fake peer/root/token or runtime fixtures.

## Typed lookup and publication contract

FindCoreLogicalForNativeWebContents(WebContents*) requires UI and a valid nonnull UI-local borrow (nullable input supported). It calls existing ArcContentsData::FromWebContents ONLY, weak-locks the stored REAL root and checks root.GetNative()==input. No outer contents traversal or wrapper/root/state/service creation occurs. Returned handle strongly owns the actual root and therefore its actual functional peers/state for accepted caller work; it does not retain physical native ownership.

TryInstallInitialCoreLogicalForNativeWebContents(native, const realRoot&) requires caller strong ownership throughout, a completed real library root, live exact native weak binding and nonnull supplied CloseState. Existing core state is compared by BOTH pointee AND shared control identity against EVERY existing strong/weak facade, through its private direct native-match state accessor. Existing facade policies/fields are untouched; temporary precise strong loads cover the entire publication call, so facade-release teardown cannot occur before the userdata write. Direct accessors introduce no app/KVO callout.

Both actual peer getters CHECK nonnull slots, enforcing the bare library's existing no-publication-during-factories contract. This is a precondition failure, not a fake successful partial root or a fallback peer. Runtime peer behavior and package factory correctness remain separate contracts. No peers attach/subscribe or receive callbacks here.

ArcContentsData gains ONLY weak_ptr<CoreLogicalWebContents>. A default weak pointer has no owner; owner_before against empty distinguishes never-published from an expired former route. A different existing control identity is rejected EVEN if expired. Same control identity also requires same live root pointee; completed same-root retry is idempotent after the same binding/state validations. No retired resurrection, replacing, clearing, remapping, transfer or discard rebind API exists. Userdata weak ownership can keep a control block allocated, but cannot keep the root or its peers alive or make weak locks succeed after last strong release.

Only when no userdata exists does successful publication create an empty ArcContentsData; it then stores the weak root. This creates NO facade, root, state, service, subscription or global/strong owner. Existing userdata is never replaced. Caller MUST arrange actual persistent root ownership independently before releasing its supplied handle. The root's immutable native weak binding does not grant standalone native ownership or tab-side retention.

FindCoreCloseStateForNativeWebContents now tries this live matching real root FIRST and returns its EXACT existing state. Otherwise it executes the previous precise canonical strong-or-weak facade/native-match lookup verbatim. It never creates state. An expired/unpublished/stale root with no valid facade returns empty; that is NOT permission to skip mandatory work/accounting. Native-first availability MUST be established before mandatory consumers activate. Legacy fallback does not restore a retired root or authorize a later different initial route.

## Explicit boundaries

Only declarations are added to proposed private own core_logical_web_contents.h; existing proposed core_close_adapters.h contract comment describes the new root-first lookup. Lead approval/publication remains pending. No actual caller, hook installation, helper invocation, eager construction, facade-ready transaction, wrapper initializer, owner/discard/transfer/rebind/retirement/native-delete endpoint or physical ownership changes. Existing constructor still creates facade state in the old path; future construction MUST explicitly arrange same-root/state association before using this API. This proposal alone does NOT make current facades rooted or peers discoverable: nobody calls publisher yet.

Creating userdata before facade will also make existing HasCoreWebContentsData marker true. That is not activated here. Future construction/order review must account for this already-managed marker and tab insertion/app callback gates. A nullptr/empty result must never be treated as proof no callback is owed. Full logical construction, owner retention, subscriptions, final endpoints and retired/discard handoff remain HOLD. No new timing/close behavior or raw-golden parity is claimed.

## Exact verification

prepare.py verifies exact18 bare source hashes against archived bare-library manifest, copies reviewed16 stage and strictly appliesd4 to reconstruct exactly that18 tree. It separately applies the route patch with git apply --check/--whitespace=error to reconstructed18 and requires exact full18-file path/hash equality with new stage. Only WC/private logical header/close-adapter comment differ; other15 files stay byte-identical. apply-checks.json and manifest.json retain exact source/patch/final hashes and commands. Bare stage hashes remain unchanged after compiler.

compile.py queried actual Siso WC command, retained exact compiler/SDK/ARC/C++23/plugins/defines, prepended new18 stage overlay and substituted staged WC. Removed -c/object/dependency outputs, added -fsyntax-only. Actual final WC clang exit0, EMPTY diagnostics; command/query/original flags/stripping/log/checks are retained. No object, graph generation, linked build, runtime, app or test fixture. Ordinary headers outside18 tree resolve from current checkout; this is not a hermetic whole-engine build. A fresh reviewer must assess route control/state checks before lead approval; syntax success does not activate any consumer.

Fresh review CLEAN in logs/wp/core.logical-route/review.md. Independent strict
route→sync3 and sync3→route produce identical18files; combinedWC b2545aec0f1700b9c500e5aa36f793b35ff407dd052cf7d3aaf71d28f0cbaca6.
No combined compiler/runtime or actual publisher call is claimed. No activation.
A future initialized-facade→stored-logical accessor is separate and required for
lazy service adoption after physical native death; this native-only lookup does
not replace that accessor or authorize facade resurrection.
