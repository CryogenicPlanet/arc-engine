# Discard finalizer: nil ObjC owner policy

**The original selected bridge does not require a nonnil ArcWebContents owner before sending `webContents:finalizeDiscardRequest:`.** If that bridge is invoked while its weak app delegate survives and its weak owner has become nil, the original sends the selector to the delegate with a nil first argument. Current owned source adds an owner-presence condition. This is a proved conditional bridge-policy discrepancy; whether that combination is reachable in an intact canonical lifetime remains unproved here.

## Exact original dataflow

Original universal framework SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`, arm64 unslid VAs. The full `[4446b10,4446b74)` bridge body is recorded in `original-disassembly.txt`:

| Address | Operation |
|---|---|
| 4446b20 / 24 | Preserve actual result `x2` in `x19`, bridge receiver in `x20`. |
| 4446b28 / 2c / 30 | Weak-load-retain bridge+0x10 (ObjC delegate), save in `x21`. |
| 4446b34 / 38 / 3c | Weak-load-retain bridge+8 (ObjC owner), save in `x20`. |
| 4446b40 / 44 | Load selector from `e291310`: `webContents:finalizeDiscardRequest:`. |
| 4446b48 / 4c / 50 | Set message receiver `x0=delegate`, first explicit argument `x2=owner`, second `x3=result`. |
| 4446b54 | Call `objc_msgSend` unconditionally. |
| 4446b58 / 5c, 60 / 70 | Release selected owner and delegate after the message. |

The delegate returned by `objc_loadWeakRetained` at4446b2c is held in callee-saved `x21` from4446b30, across the second weak load at4446b38 and the send at4446b54; its first release is4446b70, after the owner release at4446b5c. The selected surviving delegate is therefore strongly pinned through owner loading and callback receipt.

There is **no CBZ/CBNZ, conditional branch, owner test, or owner-derived eligibility test anywhere in that complete bridge body**. In particular the owner is an argument, not the message receiver. A nil delegate naturally produces no effective ObjC message; a nil owner with a surviving delegate does not suppress the send. Original has no separate delegate-present branch either. Keeping the owned delegate/optional-selector checks does not require removing them as part of this narrow candidate.

The import mapping is independently resolved from this actual image's chained imports: `84edd34 → c4a5b14 → [d824118]` imports `_objc_loadWeakRetained`; `80045ec → c4a5a90 → [d824000]` imports `_objc_msgSend`; release imports `_objc_release`. `mapping.json` preserves raw slots and ordinals. The original bridge vtable `da89030` slot+0xb0 at `da890e0` targets `4446b10`. These are decoded mapped pointers, not inference from selector names alone.

The inspected setter preserves ObjC self in `x20` at4441148 and incoming delegate in `x19` at4441144. It installs bridge vptr at allocation+0x18 (44411a4..ac), transfers the weak self temporary to allocation+0x20 (4441184..8c, 4441198..a0, 44411b8..c0), and weak-installs the delegate at allocation+0x28 (44411c4..cc). Relative to the installed bridge receiver these are exactly owner+8 and delegate+0x10, corroborating the prior frozen capture audit.

## Bounded known-invoker checks

The preserved capture audit's task dispatcher checks manager lifetime (`42c1750..5c`, `42c178c..9c`) and captured native registration (`42c0890..08bc`, raw selection `42c08c0`). It then calls the inner member invoker at `42c08d0`. The member invoker `42c29d0..42c2a28` resolves the saved virtual member, loads the actual Boolean at42c2a14, passes the captured native Impl pair as `x1` at42c2a18, and tailcalls the resolved bridge at42c2a28. The bridge does not read that Impl argument. None of these inspected checks reads or tests the bridge's weak ObjC owner; its load first occurs at4446b38 immediately before the unconditional send.

The original **earlier** post-PM native binding check `4374d34..38` (frozen capture audit) is distinct. A native Impl's physical WC presence at posting is not a later weak ObjC facade load. These findings do not remove that earlier native-binding guard or prove that the facade can actually expire while a selected registration remains live.

## Current owned implication and limits

Pinned current `arc/api/arc_sessions_restore.mm` SHA256 `178091e65d07409a00eeeb86101a7234afd44f00a3597512c2b1be0095118978`:

- Lines126–128 precisely retain the selected adapter, weak-load contents, and select its delegate.
- Lines129–131 require `contents && delegate && respondsToSelector` before sending. Only the `contents &&` term is unsupported by the original bridge policy.
- Line133 decrements pending after delivery; this audit proposes no accounting/order change.

The smallest reviewable owned candidate is to remove **only** `contents &&`, retaining the weak owner load (which can yield nil), selected-adapter precise lifetime, surviving delegate/selector checks, actual Boolean, queue ordering and pending decrement. Parent alone may prepare that private hunk. No source change or activation is performed or authorized by this evidence packet. Removing this condition does not supply the missing functional logical Root ownership, actual registration wiring, absent-registration deferred retention, or other shared prerequisites.

No new runtime/nil-owner probe was prepared or executed. The simultaneous live-bridge/surviving-delegate/nil-owner state is not established reachable, nor ruled out. Strong native Impl retention does not itself prove retention of the ObjC owner. No conclusion follows here from the save helper, fabricated Root objects, ordinary app container graphs, all teardown paths or arbitrary reentry. Existing capture/accounting artifacts remain byte-identical; the manifest pins them, source, extraction tool and actual binary.
