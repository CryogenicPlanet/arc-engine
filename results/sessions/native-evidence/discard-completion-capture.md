# Original discard completion registration capture

2026-10-23. Read-only original arm64 static audit; this evidence document is the only new repository write. No application, memory attach, build, runtime queue, production/test/proposal change, board access, or credential access. VAs below are unslid original-engine addresses. Fresh independent reviewer corroborated ordering and the existing shared-hook prerequisite.

Source binary: `/Users/cryogenicplanet/general/arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`  
SHA256: `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`

## Result

Original selects its **currently registered native UI-delegate adapter AFTER the native PM discard call returns**, including after synchronous old-retirement PiP app callbacks. Its delegate-dispatch pending count increments only after that selection, before posting; it decrements after the selected callback returns (or after discovering the captured registration expired). The current sessions-owned operation order matches this. No new sessions-owned capture-order bug is established.

Do not move registration capture or this pending-count increment before the native discard call to compensate for the separately pending core old-to-new handoff. Nothing here repairs or excuses the recorded batch8 missing-finalize outcome when the old wrapper closes; that remains the shared core integration gap.

## Operation, capture and count order

The original public selector4441db4 loads its native peer from ObjC self+0x118 and tailbranches4374c00. The following native steps occur in order:

```text
4374d20  bl 7af7468                  real PM discard helper; result ->x22
4374d34  ldr x8,[x21,#0x130]        original retained Impl's current native WC
4374d38  cbz x8,4374db8             detached after operation => no finalize dispatch
4374d3c..74                        capture same Impl shared_this pair into sp
4374d88  add x0,x21,#0x348          UI-delegate registration/dispatch state
4374d90  mov w2,#0xb0               native FinalizeDiscardRequest virtual offset
4374d94  mov w3,#1                  virtual-member flag
4374d98  mov x5,x22                 actual PM Boolean result
4374d9c  bl 4378120                 finalize dispatcher

4378148  ldr x19,[x0,#8]            FIRST registration weak control read in dispatcher
4378150..88                        weak-lock CURRENT registration while preparing post
437818c  ldr x23,[x20]              CURRENT adapter raw pointer
4378194  ldr x8,[x20,#0x18]         pending native delegate-dispatch count
4378198  add x8,x8,#1
437819c  str x8,[x20,#0x18]         increment AFTER PM, BEFORE posting
4378208..20                        inner closure strongly retains captured Impl shared pair
4378224  strb w24,[x27,#0x48]       capture actual PM Boolean
4378284..8c                        retain weak ref to SELECTED registration control
43782cc  stp x25,x23,[closure,#0x38]
43782d0  str x19,[closure,#0x48]    save SELECTED registration weak control
43782ec  stp x22,x27,[closure,#0x50]
4378308  blr x8                    submit outer task
437830c..18                        release temporary strong registration lock
```

The exact diagnostic literal at4374d80 is `arc::WebContentsUIDelegate::FinalizeDiscardRequest`. The registration/count manager base is Impl+0x348, so this count storage is Impl+0x360. It is a delegate-dispatch count; this audit does not claim a public original `HasPendingDiscard` method or prove it counts only discard operations. There is no pre-PM access to that UI-registration manager/count in4374c00..4374d20.

The synchronous PiP path is separately established in `discard-impl/arccore-sessions-discard-impl-pip-evidence.md`: old retirement4372cd0 -> media helper435f9c8 ->43639e8 -> synchronous4355cb0..cc4 -> bridge441b718. It happens inside the PM operation, before the capture above. That supports the stated reentry ordering, without claiming all services/getters remain live during that callback.

## Delivery uses captured registration, then decrements

Outer closure invocation42c1740 first validates its captured manager-lifetime weak flag. It passes the captured raw registration pointer and weak control to generic delivery42c0840. That helper weak-locks the **captured** registration at42c0890..08bc; it does not reload the current manager registration. When live:

```text
42c08c8  ldr x8,[inner,#8]
42c08cc  mov x0,inner
42c08d0  blr x8                    invoke inner Finalize callback
42c08d4..08f0                      release inner closure
42c08f4  ldr x8,[manager,#0x18]
42c08f8  sub x8,x8,#1
42c08fc  str x8,[manager,#0x18]     decrement AFTER callback return
```

If captured registration locking fails,42c0968..0970 still decrements. Consequently the native count includes this request while its app callback executes; it is not decremented before invoking app code.

Inner42c29d0 resolves the saved virtual member offset0xb0 and passes the captured Impl pair and Boolean. Original UI bridge vtableda89030 slot+0xb0 =da890e0 points4446b10. That bridge weak-loads the ObjC delegate at4446b28..30 and owner at4446b34..3c, directly sends the selector at4446b54, then releases them. Selectore291310 resolves `webContents:finalizeDiscardRequest:`. There is no additional task hop inside this bridge and no reload of the current public UI delegate.

## Registration changes: only supported implications

Public setUIDelegate444112c does the following:

- **Nonnil:** allocate a new native ObjC bridge; install weak owner/delegate references (4441184..44411cc), install shared bridge in ObjC storage using setWeb_contents_ui_delegate_bridge (44411f4..4441208), then register it through4345ba0 ->4375cdc. Native4375d04..0c writes Impl+0x348/+0x350 to the new raw adapter/weak-control pair.
- **Nil:**44412b4..44412dc only reads/releases a temporary copy of the existing bridge; it does NOT call the bridge setter to clear stored ownership.44412e4..44412f0 calls4375cdc with x1=x2=0, clearing the native active registration. The ObjC bridge getter44440b8 copies storage+0xa8/+0xb0; its setter44440d8 stores/replaces that pair at44444100 and releases the old strong owner.

Supported cases (assuming the shown operations complete normally):

1. Nil registration **inside synchronous PiP before PM returns** leaves native registration absent when4378120 reads it. There is no callable newly selected finalize registration. Capturing the old adapter before PM would contradict this.
2. Nonnil replacement **inside that callback before PM returns** is the registration selected afterward. The new request does not retain the pre-PM adapter by assumption.
3. Nil **after a completion was already posted** unregisters future dispatch but preserves ObjC ownership of the bridge. The already captured weak registration can therefore still lock and deliver to its bridge's original weak app delegate/owner, if those remain alive. Delivery does not retarget to a new current delegate.
4. Nonnil replacement **after posting** replaces/releases the old stored bridge. The old queued weak capture can expire when that was its final strong owner; it does not automatically retarget to the replacement. This is conditional on old-bridge ownership, not a claim all other native holders have been exhaustively excluded.

## Current owned implementation and existing prerequisite

`arc/api/arc_sessions_restore.mm:108` performs the PM operation;112 checks surviving native binding;115..123 selects the adapter;124 increments count;125 posts;130 calls the captured adapter's weak delegate;132 decrements afterward. `ArcSessionsUIDelegateChanged` at81..91 intentionally preserves the old adapter on nil while marking registration inactive, and allocates a fresh adapter on every nonnil setter. With correct setter wiring, this matches the recovered nil/replacement/capture model. No standalone owned order fix follows from this audit.

Current checkout **still needs the already documented shared wiring**: `arc/api/arc_web_contents.mm:795` only assigns `_uiDelegate` and does not call `ArcSessionsUIDelegateChanged`; repo search finds only its definition and lazy first-use call at sessions:117. Once a first discard cached an adapter, later nil/replacement setters can therefore leave the cached adapter/active flag stale. `ArcSessionsHasPendingDiscard` likewise currently has no shared caller. These are the pre-existing lead/core prerequisites in `logs/wp/sessions.report.md:83`, not newly implemented/activated behavior and not a reason to patch around the old-wrapper-close failure in sessions-owned code.

## Limits

No runtime nil/replacement reentry fixture was run. Arbitrary exception unwinding, nested run loops, profile teardown, unobserved additional bridge owners, and task cancellation/destruction counts are not exhaustively established. This audit proves the shown PM/capture/count/callback order and bounded registration ownership; it makes no feature-backend, successful rebind, batch8 acceptance, or cross-service parity claim.
