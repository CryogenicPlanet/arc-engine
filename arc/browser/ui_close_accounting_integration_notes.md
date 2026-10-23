# UI close-accounting companion — UNAPPLIED and deliberately partial

Private stage `/tmp/arc-ui-close-accounting/`; no live UI/core/shared/native source, helper, build/package, app, lock, board, public Close timing or activation changed. Parent alone may copy this proposal patch and notes to owned `arc/browser/` paths after fresh review. Applying the patch remains a separate lead decision. Standalone syntax checking is not a link/runtime/functional claim.

## Composition and ownership

Apply after all seven core proposals, in exact order captured by `checks.json`: native navigation hook, navigation registration, display registration, observer registration, close-state primitive, typed adapters, and zoom observer. Shared close headers/primitive are copied from the exact core seven-chain stage solely into this private composition; no cross-package live header edit is made. UI uses the existing `CoreCloseStateForWebContents` and `MakeCoreCloseAsyncInvocation` API; it creates no state, wrapper or native identity for accounting.

The patch modifies five UI sources: javascript, permissions, auth, menus and lifecycle. Sixth source notifications is an exact unchanged private copy because profile/global notification callbacks have no proven logical-WC membership. Menu item-model changed/removed selectors likewise remain excluded. Both declarations and public ABI remain core-owned; this patch contains only UI-owned source hunks.

## Concrete accounting

Thirteen direct app selectors receive kUI synchronous scopes (ten source scope sites because JS branches share scopes): alert/confirm/prompt, beforeunload show and both close selectors; permission close; auth close; context menu BOOL; infobar show/close and hung renderer show/hide. Guard cleanup retains only the exact state and executes after selector return even if the callback destroys its native producer. Existing callback detachment, weak/generation checks, return values and local infobar entry lifetime are unchanged.

JS captures the canonical handle once at its existing dialog owner lookup and reuses it for later ClosePanel. Permission captures once at its existing Show-time owner lookup solely for its later synchronous close. Auth captures once at its existing BuildView owner lookup **before** SetObserverAndDeliverCredentials can enqueue autofill, and preserves that autofill-before-show possibility. Infobar captures once at existing Initialize owner lookup before Replay; its added short local owner is only used to safely read state from the existing weak owner, never retained in accounting/task captures. Hung captures once at its existing Show lookup; Hide copies state before Clear. No state migrates or resets merely on weak owner expiry, rebind or another dialog cycle.

Auth's existing Show and DeliverAutofill posts wrap their unchanged weak-bound work with MakeCoreCloseAsyncInvocation. Pending acquires immediately at each existing enqueue. The outer bookkeeping task is not weak-cancelled: its inner weak work can expire, return for missing contents/dialog/delegate/credentials, or finish an app call, then the exact captured state's pending count decrements after return. Existing UI runner/FROM_HERE, post count, ordering and copied arguments remain. There is no rejected-post rollback, task-destructor decrement, retry, continuation or extra task.

Context scope ends before Cocoa fallback tracking; hung Show scope ends before its existing weak/generation/fallback checks. A retained JS/permission/auth completion, infobar action or hung wait/crash closure does not retain busy state. App-to-native completion marshal/Resolve posts remain unchanged. Infobar Replay is still an uncounted native discovery task pending producer proof; its actual app selectors are synchronously scoped. No new automatic Close or inline retry is introduced.

## Explicit permission Show enqueue blocker

**Producer coverage is incomplete.** Original arc/api/arc_ui_permissions.mm:140–144 constructs with only native contents/delegate and immediately posts weak Show. The first legitimate existing wrapper lookup is Show at171. `wrapperForWebContents:` creates a wrapper when absent, so moving/adding that lookup at construction would change timing/identity. Current staged CoreCloseStateForWebContents requires an existing nonnil canonical wrapper and has no native-pointer accessor. A delivery-time lookup is too late to acquire pending at construction.

Consequently this companion leaves that constructor/Show enqueue byte-identical and does NOT claim pending Show accounting, even though the later close selector has a same-state scope. It adds no redundant synchronous scope to the queued Show to disguise the gap. Permission's app response Resolve post is unchanged and is not an original host invocation.

Proposed lead/core-owned signature, documentation only (not declared/called by this UI patch):

```cpp
CoreCloseStateHandle FindCoreCloseStateForNativeWebContents(
    content::WebContents* contents);
```

Contract needed: primary-sequence, noncreating lookup of an already-associated initialized logical state; null explicitly means unavailable. It must not construct a wrapper, allocate a substitute state, duplicate ArcContentsData or silently transfer state by recycled native address. Core ACK140587 confirms availability is NOT universal: ArcContentsData exists before wrapper initialization; strong/weak wrapper publication follows initialization, and HostedTabHelpers/PermissionManager attach before wrapper creation. The global permission hook is not core-host gated. The planned accessor must require existing userdata, a precise live existing wrapper and direct native identity equality, returning that same handle or empty without invoking public getters. Lead must also prove state is available at this enqueue, or provide an approved already-canonical-state factory handoff/missing-association policy before full integration. A nullable lookup alone does not justify silently uncounted fallback. Permission pending remains an activation blocker until that association contract exists.

## Verification and limits

`check.py` obtains actual per-file UI compiler flags using siso query, strips object/dependency writes, supplies only staged source/includes/VFS overlay, sets private crash-output path, and invokes Chromium clang with fsyntax-only. All six UI sources plus the four exact seven-chain core WC/navigation/render/primitive sources exit0 with zero diagnostic bytes. Core commands are the recorded real-flag full-seven-chain commands with only private paths/overlay/output substitutions. Complete command JSON/logs and the exact VFS mapping are private alongside the proposal.

`finalize.py` verifies all six production UI base hashes unchanged; removes only the explicit accounting additions and reconstructs every source byte-for-byte; verifies unchanged existing PostTask counts and global notifications; initially checked seven patch hashes/core base hashes and strictly check-applied/applied all seven then this UI patch before parallel core drift. That initial evidence remains in initial-checks-before-core-drift.json. During finalization, core production arc_web_contents.mm and core_close_accounting_adapters.patch changed in parallel; current hashes and approved prior hashes are explicit in checks.json. There is NO current-production seven-chain apply claim and no core rebase here. Final replay copies the exact previously compiled cached seven-chain source snapshot, strictly check-applies/applies this UI-only patch against unchanged current UI bases, and compares every resulting UI file byte-for-byte. No public Close path is enabled by any of these checks.

Runtime weak-expiry/dequeue, nested/reentrant callback close, same-state lifetime/discard/rebind, synchronous callback responses and cancellation/rejection still require coordinated integration validation. ObjC exception unwinding is not promised by the primitive. Other packages' callouts/native close forwarding and Closed phase remain lead gates; accepted current UI boundary results do not establish this unapplied companion's behavior.
