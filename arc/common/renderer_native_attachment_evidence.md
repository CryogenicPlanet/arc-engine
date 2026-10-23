# Native WebContents attachment and observer ordering

Read-only static audit, 2026-10-23. Production and all published proposals remain
unchanged. Addresses are unslid arm64 image virtual addresses. The pristine
trace156 ArcCore image was reread and matches SHA256
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.
New bounded raw ranges, mechanical scans and hashes are under
`logs/wp/renderer-native-attachment/`; the manifest records exact inputs.
No application, process attachment, build, lock or Chirp operation was performed.

## Actual non-null attachment, beyond constructor order

The requested unsigned-offset STR64 scan of `0x4370000..0x43a0000` for
immediate displacement304 finds exactly two instructions:

```text
0x437039c str x20,[x19,#304]  // attachment, replacement Chromium WebContents
0x437175c str xzr,[x19,#304]  // constructor initialization only
```

This is a bounded instruction-pattern scan, not a claim that no other addressing
mode or aliased write exists anywhere. The full attachment body identifies its
receivers: `0x4370308/0x437030c` save input x1 as new WebContents x20 and input
x0 as native Impl x19. `0x4370348..0x437035c` require the Impl detached and any
owned WebContents compatible; `0x437039c` then installs the non-null input in
that same Impl's `+0x130` field.

The ordinary WebContentsObserver subobject is `Impl+0xa8` (decimal168).
Its constructor target was null; the actual non-null registration is:

```text
0x43706c4 ldr x1,[x19,#0x130]
0x43706c8 add x0,x19,#0xa8
0x43706cc bl 0x23db2f4       // WebContentsObserver::Observe(new_contents)
```

`0x23db2f4` compares old/new targets, unregisters a prior target, stores the
new target at observer+24 and tail-calls `0x1c44a40` to register. The registration
routine checks for a duplicate observer, advances the list version, and appends
its weak observer pair through `0x575f20`. It does not call an observer virtual
method or manufacture PrimaryPageChanged. This is actual attachment evidence;
constructor member order alone supplies none of it.

## Same Impl and blocked count across discard handoff

The sessions worker already independently established the full successful
replacement chain in
`results/sessions/native-evidence/discard-impl/arccore-sessions-discard-impl-evidence.md`.
This audit read that evidence and the full named attach/detach disassembly,
then independently checked the relevant dataflow and direct setter edges.

- Replacement branch `0x43980a8..0x43980c4` calls detach `0x4372b6c` and attach
  `0x43702ec` with the **same x20 native Impl**. It passes the new Chromium
  WebContents from the event payload to attach. It does not call the Impl
  constructor or replace the native shared peer.
- Attach `0x43703bc..0x4370408` takes that original Impl's shared-this pair at
  `+8/+0x10` and installs it into the replacement WebContents' Arc UserData.
  The UserData allocation is a small shell, not a fresh Impl.
- The blocked count is **decimal1484 = hex0x5cc**. Constructor `0x4371a94`
  initializes it; setter `0x438eb84` guards equality at `0x438eba4..0x438ebac`
  before writing at `0x438ebb0` and notifying. The audited full detach
  `0x4372b6c..0x437319c` and attach `0x43702ec..0x4370ffc` contain no explicit
  count write or reset/setter call.
- Native PrimaryPageChanged remains a separate reset thunk
  `0x438f2fc..0x438f304`: subtract observer offset0xa8, pass zero to
  `0x438eb84`. Merely attaching the observer does not invoke it.

Thus the successful native handoff itself retains the Impl and its count.
Replacing Chromium WebContents alone does not justify an unconditional zero.
This does not exclude later real page changes, unrelated indirect/reentrant
callbacks, or establish when queued app notifications arrive. No runtime count
snapshot was measured here. See `counter-rebind-20261023.raw.log` for the fresh
fixed-target edge scan and supporting contexts; the sessions artifact remains
the fuller primary handoff audit.

## Navigation observer registration is earlier than top-band observer

Public `navigationController` getter `0x4411964` loads the native service at
Impl+728 (`+0x2d8`) at `0x44119cc`, then calls `0x42aec00`. That helper copies
the service's own shared-this pair at service+32/+40. It does not discover a
second WebContentsObserver. The service constructor is `0x4366200`, called
by `0x436f658`; shared-this is stored by `0x436f690` and the service alias
is stored at Impl+728 by `0x436f6d0`.

The actual attach order is grounded in one straight-line body:

```text
0x4370648 ldr x20,[x19,#0x2d8]  // native navigation controller service
0x437064c ldr x1,[x19,#0x130]
0x4370650 mov x0,x20
0x4370654 bl 0x23db2f4          // its observer attaches first
...
0x43706c4 ldr x1,[x19,#0x130]
0x43706c8 add x0,x19,#0xa8
0x43706cc bl 0x23db2f4          // native Impl/top-band observer attaches later
```

Both expose the same WebContentsObserver virtual slot at byte offset0xa8:

```text
navigation service vtable0xda80780 +0xa8 ->0x43695b0
Impl observer    vtable0xda81b40 +0xa8 ->0x438b950
```

The latter thunk subtracts0xa8 and calls the previously audited committed-primary
handler `0x438b6ec`, which refreshes the top-band sampler at `0x438b824`.
The former `0x43695b0` checks committed states8/11 and primary-frame identity,
then its commit branch invokes `0x436bbc0` at `0x4369c88` or `0x4369cf0` with
observer list service+136, method offset24 and virtual-method flag1.
Bridge vtable `0xda882d0+24` is `0x44195b4`, the known ObjC commit adapter.
This connects the service to the actual app notification instead of relying on
names or an unrelated constructor with a similar observer-container layout.

## Notification dispatch boundary: registration order is not delivery order

The helper at `0x436bbc0` identifies itself through native SourceLocation strings:

```text
0xd37eb3b "NotifyObserversImpl"
0xd37eb4f "../../arc/common/utils/observer_list.h"
```

At `0x436be30..0x436bfdc` it locks each shared observer and constructs an88-byte
callback containing method offset/virtual flag, observer and retained argument
pairs. The closure invoke pointer is `0x42c2700` (ADR at `0x436beb8`). Its
invoke thunk loads the method at closure+32, the encoded adjustment/virtual flag
at+40, receiver at+48, arguments at+56/+72, resolves the virtual method and
branches to it at `0x42c2758`.

The callback is handed to a runner obtained through the thread-local descriptor
`0xe379858`: `0x436bc94..0x436bca4` resolves TLS; `0x436bf18..0x436bf24` loads
its current handle and runner. `0x436bf28..0x436bfdc` constructs SourceLocation
line268 and invokes runner virtual slot0 with the callback and x3=0.
There is no direct bridge invocation in this helper's delivery loop.
An independent native handle-writer audit strengthens the library identity:
`0x4451058` restores TLS858 from handle+8, restores adjacent TLS840 from
handle+24, and releases runner members at handle+0/+16. Separate native
`0x4450f64/0x4450fb4` install/restore TLS840 through a16-byte sequenced handle.
This matches `base::SingleThreadTaskRunner::CurrentDefaultHandle` with its
embedded `SequencedTaskRunner::CurrentDefaultHandle` (current C156 headers and
implementations). The descriptor is in native `__thread_vars`; its stripped
symbol name is not available. Raw supporting instructions are in
`tls-native-handle.raw.log`, `tls-handle-writers.raw.log`, and
`notify-wrapper-and-return.raw.log`.

Together the matching handle layout, TLS access, callback ownership, SourceLocation
and virtual-call argument shape support the source-equivalent operation
`SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(..., TimeDelta())`.
The C156 `base/task/task_runner.h:31` contract explicitly forbids synchronous
execution inside Post*Task. The current-default single-thread runner targets
the caller thread. This identification is a native/source ABI match; the concrete
runtime runner subclass/vtable/queue was not observed.

**Ordering implication:** during ordinary non-nested UI observer dispatch, the
earlier navigation observer enqueues commit delivery, and the later Impl observer
runs its synchronous sampler refresh before that queued ObjC commit delivery.
The earlier registration is not evidence of earlier app delegate delivery.
An unconditional ordering claim across nested RunLoops, unrelated observers or
reentrant native/app behavior remains unproved. This audit changes no core hook
or proposal position. Any integration must state the non-nested scope rather
than treating a nearby method name as universal ordering proof.

## Published TopBand proposal: separate attachment gap

This section is a read-only audit of the published, unapplied proposal88, not a
new native discard behavior claim. Its staged source is pinned by
`logs/wp/renderer-top-band-proposal/published-proposal.json`.

`arc_renderer_top_band.mm:53..75` introduces a separate
`WebContentsUserData<TopBandDelegate>` whose bridge pointer is weak. The wrapper
retains its private delivery payload through an ObjC association. The sampler's
`Deliver` at lines152..157 resolves its document RFH's current Chromium
WebContents and reads that WebContents' separate delegate holder. Public setter
lines274..290 creates/updates the holder; it does not implement a rebind hook.

Consequently, destroying only `RendererBridge` during a future canonical
WebContents replacement cannot clear the old TopBandDelegate holder or bind the
existing wrapper-retained payload into the new holder. While that payload lives,
the old holder can still resolve it and a new holder can remain absent. The
native bridge slot is on the retained Impl (+632/+640), so this proposal's
per-Chromium-WebContents placement requires an explicit attachment policy.
Clearing/reattaching delivery metadata must not imply copying document sampler
state, cache, tokens, or in-flight RFH captures into a new document. Exact
retirement ordering and old-RFH callback eligibility remain separate contracts.
No implementation, no unconditional callback replay and no guessed lifecycle
reset is added here.

## Reproduction and limits

From the repository root:

```sh
python3 logs/wp/renderer-native-attachment/scan.py
python3 logs/wp/renderer-native-attachment/pointers.py
python3 arc/common/renderer_contract_evidence.py 0x43702ec:0xd10
python3 arc/common/renderer_contract_evidence.py 0x43695b0:0x1198
python3 arc/common/renderer_contract_evidence.py 0x436bbc0:0x52c
python3 arc/common/renderer_contract_evidence.py 0x42c2700:0x5c
```

The raw files also preserve exact smaller constructor, helper and callback
ranges. The new scans do not attempt exhaustive alias analysis or arbitrary
virtual/reentrant paths. The image symbols are stripped, so source-equivalent
library identities are distinguished from raw instruction facts. Committed
navigation delivery ordering is bounded to ordinary non-nested UI dispatch; no
unconditional nested/reentrant ordering or runtime measurement is claimed. Existing ALL, gradient and TopBand
artifacts remain byte-for-byte unchanged.
