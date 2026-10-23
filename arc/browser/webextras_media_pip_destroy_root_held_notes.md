# Specific PiPDestroyed logical-owner qualification — held increment

UNAPPLIED / HOLD. This separate proposal changes only the already-staged
`WebextrasMediaSource::PiPDestroyed` producer after frozen initial Media proposal
`16776036f3321d7e5f5dbdb629561fb2fe0c8b7f0c1bc1791d096f5b2a2733c9`.
No actual production/shared source, prior frozen proposal/test/evidence,
Chromium, maintained patch, app/profile, runtime, build graph or lock changed.

## Exact source prerequisite

Read the Core final-owner qualification and sessions addendum
`results/sessions/native-evidence/discard-impl/final-owner-qualification-20261023.md`
FULL, SHA256
`dba669824f03e5705f5535cfecb07b842f4e2f3a6d5e97361d905cdbb38ceb89`,
plus its raw disassembly/manifest and the updated Core plan's final-owner
qualification. Fresh bounded llvm-objdump extracts on FE0 arm64 reproduce:

- 43639fc loads the Media peer's actual logical weak control+28. Absent control
 returns4363ab0 at4363a00. Control+8's expired -1 sentinel takes the SAME return
 at4363a10..14. CAS4363a18..30 acquires real strong logical ownership while live.
- 4363a34..38 checks that acquired logical pointer. Only then does4363a5c clear
 cached PiP peer and4363a84 synchronously dispatch PiPDestroyed.
- That actual owner lease is released at4363aa0..aa8 AFTER dispatcher return;
 the temporary same-root dispatch copy releases at4363a88..9c. The proposed
 lease matches required lifetime, not a claim of exact native reference counts.
- Final logical destructor4371b84 may still call common DeInitialize4372b6c at
 4371bb4 while physical binding exists. UI dispatcher anchor/guard clear later
 at4372070/2080 does not revive expired strong logical ownership. UI raw disposal
 43a04dc→43a05ac is not another strong shared owner.

Thin SHA256 `fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`
is checked before extraction. B70/FE0 architecture equality is already pinned
in the separate retirement audit; sessions provides independently reviewed B70
ranges. No target was launched.

## Minimal proposed change

`webextras_media_pip_destroy_root_held.patch` adds one acquisition/return at the
TOP of this producer, before `pip_destroyed_`, existing cache maintenance or
synchronous delegate dispatch:

```
auto logical = logical_.lock();
if (!logical) { return; }
```

The C++ local shared handle retains the SAME acquired functional Root, thereby
both real package peers, through function/callback return. No factory, native
lookup, facade/service keepalive, root resurrection, new state, artificial
lifetime token or dispatcher poisoning is added. The existing helper captures
SAME canonical accounting state as before; this producer simply never enters
it when Root is expired.

`PiPCreated`, the other PiP producers, regular receipt guards/queues, selected
bridge semantics, nil ObjC owner/delegate behavior, native weak routing, live
controls, cached-position getter and physical cleanup order are byte-unchanged.
No generic gate is applied to all synchronous Media callbacks.

Existing `pip_destroyed_` dedup/latch behavior is deliberately preserved as a
SEPARATE **unproven repeat-PiP HOLD**. The native bounded producer has no newly
recovered equivalent latch proof here; this task does not remove or bless it.
The test checks that an expired Root does not mutate the existing latch; it does
not claim repeated PiP events now have source-complete parity.

## Meaningful executable controls

`webextras_media_pip_destroy_root_held_test.py` reuses the immutable initial
candidate's extraction/scaffolding, compiling actual Source, actual Root library,
actual close-state primitive, real producer/RetirePhysical/dispatch/cache/adoption
bodies with Foundation ARC. Source access control alone is exposed in the
extracted class to observe existing cache/latch/native fields. A counter is added
immediately before actual EnterSync's std::exchange to observe scope acquisition;
its accounting logic is not replaced. Native WeakPtr/userdata/transport/task
runner and stateful DevTools sibling fixture remain mocks. This verifies Root's
actual strong sibling slot, not production DevTools behavior or a Core disposal
transaction.

PASS, exit0:

1. Retained service/peer with dead actual Root leaves existing PiP cache identity,
   latch, zero new callbacks/tasks and actual accounting entry count unchanged.
2. Live Root clears cache before callback, while old Source observation/native
   binding remains intact; physical detach happens only after producer returns.
3. The first callback drops ALL other Root shared refs. Acquired Root and sibling
   remain alive through callback return, including nested actual Info receipt and
   drained accepted tasks. Root/sibling expire after producer releases its lease.
4. Nil ObjC owner with live Root still delivers. An expired app delegate with a
   live selected native bridge still allows cache/latch and one sync accounting
   scope. No bridge enters no sync scope. These are separate eligibility axes.

Unchanged old16776036 staged source fails the dead-root mutation assertion with
exit73. A distinct extraction-only negative control retaining only a liveness
check, with no strong Root lease, passes dead-root eligibility but fails when
the callback drops other Root refs (exit73). Neither negative modifies staged
production bytes. No test fabricates an app notification or reference-count pad.

## Validation and freeze

Base composed Media SHA256
`bfb9a6dba3cb2cfe77eeb1c149f852523e65b3c6fa0968a2c0a4ee09c295b718`.
Corrected Media SHA256
`c4b78d579a433596c4586b10399056d50b1a5eeb1b8999fcbfd8a9f4902aec37`.
All source/header bytes other than those four added producer lines/comments
reconstruct exactly to frozen initial base. Strict whitespace-error check/apply
and full private reconstruction pass. Complete Media TU passes real Chromium
flags/plugins/SDK/ARC/C++23 via the frozen dependency VFS with only source
substitution: exit0, zero diagnostics. Four bounded native ranges/eight exact
instruction anchors corroborate the prerequisite.

Private stage `/tmp/webextras-media-pip-destroy-root-held/{base,staged,reconstructed}`;
durable snapshots, source commands, VFS, raw evidence, validation, positive and
negative result JSON are in `logs/wp/webextras-media-pip-destroy-root-held/`.
Previous delivery/accounting/retirement/initial frozen owned artifact hashes are
checked unchanged. A separate freeze records final proposal/test/notes/evidence.
Fresh independent review is required before treating even this scoped stage as
ready for lead integration.

## Scope and requests to lead

This is initial immutable-binding peer scope plus the ONE proven final weak-lock
prerequisite. Bare Root destructor still only drops members; full Core final
DeInitialize/ownership/association/dispatcher teardown, qualified discard/rebind,
recursive attachment, actual facade construction, PiP backend/controller/cache
migration and runtime acceptance remain HOLD. Do not add a temporary root owner
around genuine final disposal to force this PiP callout, or interpret this gate
as universal teardown silence. Separate DevTools raw close callbacks can survive
peer disposal under their copied-payload contract.

No scenario rerun or diff status change. No architecture exception is proposed;
this qualifies earlier live-root retirement prose instead of rewriting frozen
proof. Parent/lead must compose the reviewed producer increment with the real
Core lifecycle and perform full link/paired runtime acceptance.
