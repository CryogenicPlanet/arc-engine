# Bounded original DevTools lifetime audit

Read-only production/proposal audit. The only new files are this note and raw
proof under `logs/wp/webextras-devtools-lifetime-audit/`. No source/proposal,
registration, app, build, lock or board change was made. Addresses below are
unslid original arm64 VAs. Binary SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.

## Verified conclusion

The original callback cancellation lifetime is **not** the lifetime of the
currently attached Chromium WebContents. A retained ArcWebContentsDevTools
Objective-C service independently strongly owns its native DevToolsImpl. Target
cleanup and the recovered replacement path detach/rebind that same Impl without
invalidating its queued-notification guard. Therefore an accepted notification
can remain deliverable after its old target is destroyed, provided the native
DevToolsImpl still has a strong owner and the captured bridge's weak delegate
survives. Retaining only the Objective-C service is sufficient for the Impl
ownership side of that condition; the parent logical WCImpl need not survive.

This is a conditional static contract, not a new runtime observation. It proves
the current owner-WebContents WeakPtr is capable of cancelling earlier than the
original; it does not justify removing all cancellation or choosing an exact new
state-token ownership graph without integration review.

## Concrete shared allocation and independent owners

Use `owner-factory.asm`, `service-factory.asm`, `service-getter.asm`,
`wrapper-init.asm`, `wrapper-dtor.asm`, `devtools-control-vtable.json` and
`devtools-control.asm`.

1. Logical WCImpl's service setup allocates0x150 bytes at436f2c8..2d4, installs
   shared-control vtable addresspoint da824e8 at436f2dc..2e4 and passes allocation
   +0x18 to DevToolsImpl constructor435691c at436f2e8/436f304..310. This is one
   control+Impl allocation, not two guessed equal objects.
2. Factory436f330..348 sets the Impl's enable-shared self weak pair, increasing
   control+0x10 weak count, then drops the temporary extra strong reference.
   At436f388..390 the SAME Impl/control pair is stored in logical WCImpl
   +0x2b8/+0x2c0. Replacing a prior pair releases its control strong count at
  436f394..3b8. The ordinary owner therefore owns a counted shared DevToolsImpl.
3. da824e8+0x10 is43a05f0: it adds0x18 to the control address and tail-calls
   DevToolsImpl destructor4356b28 at43a05f8. This ties the allocator/control block
   to the exact destructor that poisons the notification flag.
4. Original ObjC devTools getter4411290 reads WCImpl+0x2b8 and calls42c8400 at
  44112a0. That helper reads the Impl's self weak pair at+8/+0x10, CAS-locks the
   control+8 strong count, and returns that same pointer/control aggregate.
   The getter allocates the ObjC service and calls init: with it at44112dc..1304.
5. ObjC init4414d2c..38 stores this pointer/control in service+0x20/+0x28;
  4414d40..48 increments the control's strong count at+8;4414d50 engages the
   optional at service+0x30. This is an independent strong native owner, not a
   weak WebContents observation or inferred wrapper identity.
6. ObjC .cxx_destruct4415e88..a4 tests that engagement and decrements the stored
   native control+8. Its zero/last-owner branch4415f44..54 invokes the control's
   object destructor. Releasing the service's delegate bridge is a separate
   later operation. No nativeDevTools setter appears in the exact class surface;
   the recovered target cleanup never touches the service's optional aggregate.
7. Logical WCImpl destructor43722bc..d0 independently decrements its DevTools
   control+8 and skips object destruction if the previous strong-owner count
   was nonzero. Its last-owner branch43722d4..e4 invokes the same control vslot
   +0x10. Thus parent destruction cannot force DevToolsImpl destruction while
   a retained service still owns it.

The ObjC ArcWebContents wrapper likewise retains its own logical WCImpl shared
aggregate (native optional+0x118/+0x120, engagement+0x128): getter4443ee0 copies
it with strong-count increment; .cxx_destruct4444b68..84 releases that owner.
This is a separate lifetime from physical Chromium WCImpl+0x130.

## Target lifetime is represented separately

DevToolsImpl constructor43569a0 stores the logical owner's pointer/control at
+0x38/+0x40 and43569a8..b0 increments **weak** count control+0x10. It does not
strongly keep the logical owner alive. The constructor stores a separate native
helper pointer at+0x48 and initializes current-observation state+0x50 onward.
This audit does not rely on the helper pointer's complete type.

Attachment helper4358934..4358b84 uses DevToolsImpl+0x50/+0x58 for the physical
WC's invalidation flag/pointer. Its old-target path can clear old DevTools WC
userdata peer+0x10 at4358b14..48, invokes Observe(nullptr) at43589b4..bc, releases
and clears current observation+0x50/+0x58 at43589c0..dc, then (for nonnil input)
loads the new WC's weak flag/pointer at43589e4..4358a10, installs this DevToolsImpl
in new WC user data key c9d7108, and Observe(newWC) at4358ac4..ad8. The stored WC
userdata DevTools peer is a tagged raw pointer, not another DevTools shared pair.

This helper never clears the delegate registration+0x78/+0x80 or notification
flag/anchor+0x98/+0xa0. It does not destroy/recreate DevToolsImpl. Its bool input
controls clearing the old user-data peer when an old observation remains valid;
it is not notification-generation cancellation.

The observer secondary vtable at da800c8 has its class-specific slot at
address da80320 resolving to435914c, a this-minus0x18 thunk to4358e14. This
observed-contents retirement handler clears per-target protocol state/maps and
observation+0x50/+0x58. Its complete recovered body likewise contains no write
or invalidation to notification guard+0x98/+0xa0 and no DevToolsImpl destruction.
The physical-destruction conclusion below also has the explicit logical-owner
cleanup chain, so it does not depend solely on naming this virtual slot.

## Actual target destruction and discard/replacement

Use `owner-contents-destroyed.asm`, `owner-cleanup.asm`,
`replacement-owner-detach.asm`, `owner-target-attach.asm`,
`devtools-rebind-full.asm` and `native-wc-dtor-full.asm`.

Original logical-owner WebContentsDestroyed438a18c (previously identified by its
own trace in core_close_source_notes.md) invokes its UI Closed delegate before
calling Observe(nullptr) at438a278..280 and cleanup4372b6c at438a284..288.
Cleanup4372ca8 loads the existing DevToolsImpl from owner+0x2b8;4372cb0..cbc calls
4358934 with new target nil and the previous-observation engagement bool. It
**does not release or replace the DevTools shared pair**. Only later does
cleanup zero the logical owner's physical binding+0x128/+0x130 at4373054..58.
The parent native destructor also calls that cleanup when a physical target is
still attached (4371ba4..bb4), then eventually releases its own service reference
through the counted release above. No additional guard invalidation is in this
owner teardown chain.

The recovered discard replacement branch at4398080 checks the old/native tab
relationship;43980a8..ac calls the SAME cleanup,43980b0..b8 detaches observation,
then43980bc..c4 calls43702ec with the replacement native contents. That attach
stores new target at owner+0x130 at437039c and later loads the SAME existing
DevToolsImpl from owner+0x2b8 at4370624;437062c..634 invokes4358934(newWC,true).
No DevTools allocation, strong-pair replacement or notification guard reset is
between those calls. Thus this recovered replacement preserves DevToolsImpl
identity while changing its native observation and userdata routing.

Public discard4374c00 calls the native discard manager7af7468 at4374d20. This
audit captured that entry and the concrete replacement branch above; it does
not exhaust all manager eligibility/error paths or prove which active debugger
configurations are discardable. Prior recorded ordinary discard rejected an
attached fixture debugger. Do not use this audit to force replacement with live
frontends/clients, retarget CDP hosts, or replay pending commands.

The bounded direct-store scan436d000..43a0600 found only the service-construction
nonzero writes436f388/390 to owner+0x2b8/+0x2c0. This corroborates the concrete
path, not a proof against arbitrary pointer aliases, vectorized stores or all
callers in the binary. No global alias-completeness claim is needed for the
explicit cleanup→attach chain.

## Guard lifetime and bridge delivery

DevToolsImpl allocates the notification flag at43569e8..4356a10 and stores
flag/anchor `(flag, this+0x78)` at+0x98/+0xa0 at4356a14. Destructor4356c00 clears
anchor+a0 and4356c04..10 marks flag+4 invalid. These are the only direct writes
to that guard found in the inspected4355df8..435f32c class/helper region after
excluding stack frames and unrelated loaded objects. Full cleanup/rebind bodies
were separately inspected; they neither pass the guard address to an invalidator
nor write it. This remains bounded evidence, not a whole-binary no-alias theorem.

Display/inspect helper4359648 receives this+0x78, so its+0x20/+0x28 loads at
435978c/43597a8 are exactly this+0x98/+0xa0. It captures a strong delegate-bridge
pair at4359730..48, the scalar mode, and the weak flag/anchor in an outer task;
it does NOT retain DevToolsImpl strongly. Focus uses the same wrapper in its
42d4298 async helper. Queued thunk42c1740 checks captured flag+4 at42c1750..5c,
then invokes through the anchor; it never checks current target+0x50/+0x58,
logical-owner+0x38/+0x40, a frontend epoch or current service wrapper identity.

Bridge display4415fe0 weak-loads delegate+8 and ObjC service+0x10 at4415ff8..600c
and directly sends with those locals at4416024..2c. There is no nil-service
branch. Inspect/focus bridges have the same weak-load shape. Consequently:

- Service retained, old physical target destroyed: shared DevToolsImpl remains
  alive; recovered cleanup does not poison the flag; an accepted callback can
  still deliver to a surviving weak delegate with the retained service argument.
- Service gone, logical native owner still owns DevToolsImpl: flag can stay
  live; accepted captured bridge can deliver to a surviving delegate with nil
  service. A service-only weak guard would cancel too early.
- Logical owner and service ownership both gone, with no other native strong
  handle: control reaches last owner, DevToolsImpl destructor poisons the flag,
  and accepted work skips. Strongly retaining the proposed lifetime token from
  the task or from the captured bridge would instead keep work alive too long.

These cases concern already accepted work; they do not authorize new frontend
or protocol operations against a destroyed/detached target.

## Safe implementation implication and remaining gaps

Do not turn the present physical-WC WeakPtr proxy into a claimed native match.
An eventual replacement needs a logical DevTools peer lifetime with independent
strong ownership from the service handle AND the native/logical owner graph;
queued tasks should hold only a weak cancellation reference to that lifetime
while retaining the delegate registration exactly as already recovered.
Detach/physical destruction/replacement must not invalidate that token by
itself. Final logical peer destruction must invalidate it.

A token owned only by the ObjC service is insufficient (nil-service delivery).
A token owned only by current native WC userdata is also insufficient (accepted
work can survive physical target destruction/discard). Simply adding userdata
as a second strong owner is not yet an exact mapping: original DevTools userdata
is a raw peer, and the logical WCImpl userdata installs a **weak** owner pair
(control+0x10 increment at43703f8..408), which cleanup removes at4372ed0..ee0.
A physical userdata-owned token could outlive final logical retirement, or lose
continuity during the two-phase old-target detach/new-target attachment, unless
its explicit ownership transfer/retirement is defined by core's logical peer.
Do not infer original strong ownership from the mere presence of userdata.

Remaining integration questions: where ArcCore's persistent logical owner will
retain/release this peer state; how multiple retained service wrappers/handles
would share that exact native identity; how final retirement differs from a
callback-free new-pointer cutover; what other native strong handles extend the
Impl lifetime; and whether any exceptional unexamined lifecycle aliases cancel
it. Native constructor/destructor reentry and the exact point of flag poisoning
within final teardown remain untested. The public-discard manager's full route
and live-client eligibility are not exhaustively audited. No implementation
change follows automatically from these findings. Production and both held
pipelines remain frozen for parent/lead review.
