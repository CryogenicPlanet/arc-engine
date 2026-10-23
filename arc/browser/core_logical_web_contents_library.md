# Bare logical WebContents library — staged implementation, HOLD

2026-10-23. Implementer scope was `/tmp` only. No actual repo/native/shared/production source edits, build graph regeneration, full build, app, attach, lock, board or credential action. Siso was queried only for existing compiler flags; the new translation unit was run with `-fsyntax-only`.

## Artifacts and base

- Stage: `/tmp/core-logical-library-stage/`.
- Patch: `/tmp/core_logical_library.patch`, SHA256 `d4b42613459f6d3f3fcaea59170bcfdb9fc9ac633a9a39b4521ef2ac727f4b58`.
- Manifest: `/tmp/core-logical-library-manifest.json`.
- Exact base is the `stage` named by `logs/wp/core.beforeunload-transport/manifest.json` (manifest SHA256 `783c6976675855a3d5abe1542ffb405d632a73a955c90cd677e488395a43deb3`). All 16 files in that composed stage were copied, including the unchanged native transport files. This was not a clone of current mutable production source. Fifteen preexisting files remain byte-identical; only staged `arc/wp/core.gni` changed, and two private files were added.

## File-by-file implementation

`arc/browser/core_logical_web_contents.h`:

- Defines `CoreLogicalWebContentsHandle` / weak handle using standard shared/weak ownership, with a private constructor/destructor and no copy operations.
- Defines typed `CoreMediaPeer` and `CoreDevToolsPeer` interfaces with pure virtual actual Attach/Retire operations and virtual out-of-line destruction. There are NO dummy peer implementations, cancellation-only objects, empty success factories, service getters, ObjC helpers or public metadata additions.
- Defines once-only factories accepting the SAME weak logical root. Their explicit construction contract forbids subscribing, app callout, posting tasks, sibling lookup and publication before construction finishes. Factories may store the weak back-reference and read the already-established initial native binding/supplied state. This is a contract for actual future package implementations, not a claim the interface can mechanically verify their internals.
- Root stores an INITIAL immutable native WeakPtr, the EXACT supplied CoreCloseState handle, and strong typed handles to BOTH real peers. Getters return native weak/local borrow, supplied state or copies of peer shared handles on UI. Those peer handles can be independently retained by service wrappers without forcing a strong peer-to-root edge.
- No native owner, ObjC facade/delegate, userdata pointer, global registry, replacement setter, synthetic phase flag, receipt cancellation or state factory exists in this library.

`arc/browser/core_logical_web_contents.mm`:

- Factory requires UI, a live initial WeakPtr, nonnull supplied state, nonempty factories and nonnull peer returns. It creates the shared root FIRST, then invokes nonobserving factories with weak references to that SAME root. Both real peer slots are assigned exactly once before return. Peer accessors CHECK their slot so construction cannot silently expose an empty sibling.
- Strong root ownership retains both peers, including the sibling peer relevant to accepted Media work. Individual services may retain the same returned peer. Actual Media cache/dispatch state and DevTools functional guard remain future package implementations; this library does not relocate or counterfeit them.
- UI/sequence checks guard construction, every getter and final destruction. Last standard shared ownership off UI invokes a stateless custom deleter which posts raw-object deletion with the Chromium UI runner's `DeleteSoon`; it captures NO shared logical owner. Consequently weak locks already fail once last strong ownership ends, even if actual UI destruction waits. UI last release deletes directly.
- Posting failure/shutdown follows the native/Chromium delete-on-UI pattern: raw object may leak rather than destroy UI-owned members on another thread. No fallback off-thread delete or lifetime padding is introduced. Accepted work must not treat a pending raw deletion as a live logical owner.
- Destructor releases members only. Member order drops root's Media handle before DevTools; independently retained service handles can keep either functional peer alive afterward. It does not invoke Attach/Retire, synthesize Closed, acquire/delete native contents or remove a borrowed tab. Those native ownership/cleanup endpoints remain deliberately unimplemented and gated.

`arc/wp/core.gni`:

- Adds the private header and translation unit to staged `wp_core_sources`. Adds NO dependencies and changes no other entry.

## Verification performed

1. Queried existing Siso command for `obj/arc/arc_core/core_render_state.o` from `chromium/src/out/Base`; selected the exact corresponding compiler line. Original selected command saved at `/tmp/core-logical-library-render-command.txt`.
2. Used its actual clang path, defines, includes, target, SDK, ARC, C++23, Chromium plugins and warnings. Removed only object/dependency output and original input/compile flags, prepended the staged include overlay and added the new input plus `-fsyntax-only`. Command saved at `/tmp/core-logical-library-compiler-command.json`, SHA256 `832f6ea44d6c4fbfb761bfcca77a2d0c4e0a92fa30e1a0743f053586dadd08ce`.
3. New TU syntax check: **PASS, exit0, no warnings/errors**. Output `/tmp/core-logical-library-compiler.log` is empty. Incomplete CoreCloseState, virtual peer and private destructor/DeleteHelper/shared-pointer lifetime syntax all compile with those real flags.
4. Patch `git apply --check`, apply to a separate copy of the exact base and full file-hash reconstruction: **PASS**. Reconstructed copy `/tmp/core-logical-library-reconstructed/` matches the full staged tree. Only the two new files and staged gni differ from base. No runtime fixtures or full build were run.

Fresh independent source review is complete: logs/wp/core.logical-library/review.md found no concrete P1/P2 within the bare-library scope. A compile pass does not validate a future package factory implementation or establish runtime parity.

## Explicit activation and architecture limits

**HOLD / unapplied / no consumer.** No wrapper uses this root. No canonical native-to-logical route, strong tab-side root holder, facade-ready transaction, eager subscribe, permission accessor, public owner semantics, history/navigation/controller rebind or new native retirement path is installed. Existing4973732 integrations are byte-unchanged because their files were not edited.

Actual Media/DevTools implementations must implement the typed peers using real functional state and store only weak logical back-references. Media must still weak-lock logical owner before cache mutation and accepted work must retain that acquired handle; DevTools must still guard its real functional peer. This stage provides ownership required for those contracts but activates neither.

The original standalone exclusive owner+128, separate ScopedProfileKeepAlive+120, UI disposal, clear-before-delete and association-checked borrowed-final-release behaviors remain source-proven design inputs, NOT implemented by this bare immutable-binding library. No standalone adoption, tab-root ownership or safe recursive discard policy is invented. Extending the root with any of those endpoints requires another reviewed patch.

Qualified discard replacement and PiP retirement order remain as in the corrected plan: DevTools detach first, then synchronous PiP with old Media userdata/receiver/weak physical binding still intact, then Media cleanup and later root WCO detach. This stage supplies no replacement API and no Retiring receipt gate. It does not claim native post-call revalidation or authorize arbitrary kReplaced handoff.

This is a compile-checked foundation available for review, not webextras activation acceptance or a completed core runtime comparison. Production/app2o freeze remains intact.

Durable evidence: logs/wp/core.logical-library/. Public proposal arc/browser/core_logical_web_contents_library.patch has the exact reviewed patch bytes; stage sources archived under evidence/stage. Parent has not applied the proposal or modified production gni.

Subsequent source qualification: logs/wp/core.logical-construction/ proves original
public create constructs nonobserving DevTools then Media while logical managed
binding is still empty; attaches existing peers before ObjC facade allocation.
This bare library instead requires an initial live weak binding and invokes Media
factory first; it activates neither. Those are explicit foundation limitations,
not an implemented native construction transaction. Other native kinds may omit
DevTools; normal public kinds0/6 have both. Current factory must not be universally
activated or described as construction parity. No source bytes were changed.
