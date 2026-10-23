# Media delegate delivery and close-accounting source audit

Static audit only, 2026-10-23 UTC. No implementation, shared API, probe, app,
build, lock, or accounting activation was changed. The typed adapter revision
`c63b7f4ad49cbe6e19078d3042e75e618348172ba28b3c56a795315d534a4a50`
remains staged and requires lead approval. This note follows the full adapter,
primitive, and accounting-plan documents; it does not approve Close activation.

Original thin arm64 image: `/tmp/arccore-webextras.arm64`, SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
All addresses below are unslid native VAs. Durable bounded disassembly, decoded
selector/vtable references, ranges, original hash, and selected source hashes are
in `logs/wp/webextras-media-delivery-audit/manifest.json`. Raw logs were generated
directly from this verified image, rather than copied from older commentary.
Current owned source line numbers refer to the manifest's frozen media.mm hash.
Two CORE documentation inputs changed concurrently during this audit; the manifest
records initial and freeze-time hashes, and snapshots freeze-time inputs. Owned
media source and the original image remained unchanged. The current documentation
still labels c63b as unapplied and does not authorize a new native/bulk Busy gate.

## All implemented owned media delegate callsites

The native Media delegate wrapper starts at MediaImpl+40: synchronous flag +50,
pending count +58, wrapper weak guard +60/+68. It is the fifth Close domain.

| Delivery | media.mm line | Native producer -> ObjC bridge | Acquisition / restoration or post |
|---|---:|---|---|
| Session state | 261 | 43614d8 -> 441bf5c | Async pending++ 43615b4..15bc; post 4361750 |
| Camera | 266 | 43614d8 -> 441c118 | Async pending++ 4361970..1978; post 4361b0c |
| Microphone | 271 | 43614d8 -> 441c188 | Async pending++ 4361d2c..1d34; post 4361ebc |
| Actions | 281 | 4362460 -> 441bfcc | Async pending++ 43626c4..26cc; post 4362890 |
| Regular position | 286 | 4362c0c -> 441c08c | Async pending++ 4362f30..2f38; post 43630d8 |
| PiP created | 246,380 | 43634d0 -> 441b68c | Sync set 43636b8, restore 43636fc |
| PiP destroyed | 253,296,350 | 43639e8 -> 441b718 | Sync helper4355b30, call4363a84 |
| Resolution | 376 | 4363bcc -> 441b76c | Sync set4363d74, restore4363db8 |
| Show | 362 | 4364008 -> 441b7d8 | Sync helper4355b30 |
| Hide | 367 | 43641c4 -> 441b82c | Sync helper4355b30, call4364238 |
| Close | 356 | 4364380 -> 441b880 | Sync helper4355b30, call43643f4 |
| Playback | 383 | 436453c -> 441b8d4 | Sync set43646dc, restore4364720 |
| Feature | 428 | 4364970 -> 441b944 | Sync set4364b18, restore4364b60 |
| PiP position | 401 | 4364db4 -> 441b9bc | Sync set4364f74, restore4364fe8 |
| Source title | 406 | 4365358 -> 441ba48 | Sync set4365510, restore4365554 |
| Favicons | 412 | 43657d0 -> 441bad8 | Sync set4365b44, restore4365bf4 |

The decoded native bridge vtable at da88368 maps these selectors and slots in
`selector-and-vtable-map.json`. The common synchronous helper saves the prior
flag4355c90, sets4355c98, calls4355cc4, restores4355cdc: nested scopes must restore
the previous Boolean. Creation and playback remain separate invocations; preserve
the owned native weak check between them. Feature indices cover all eleven owned
Feature callers; the new hide-PiP/media-muted no-ops have no delegate invocation.
Native volume and PiP-click callbacks exist in the vtable but have no implemented
owned delegate callsites in this source. Other callback categories are not proven
members of this Media domain by this audit.

## Constructor, attachment, subscription, and caches

Native MediaImpl constructor435f32c stores a weak logical WC peer at +20/+28,
creates its delegate-wrapper guard at435f390..3c0, initializes receiver state
at+70, and clears cache engagement flags: state+b4 at435f4bc, position+c8 at4c4,
camera+d4 at4cc, microphone+dc at4d4. Cached values are not initially engaged.
State processing436154c..1568, camera4361908..1924, and microphone4361cc4..1ce0
compare only if their engagement flag is set, then update value+flag BEFORE
testing the selected bridge. Therefore the first valid info includes all three
notifications even when its values equal state3/camera0/microphone0, provided a
registration is live when that receipt is processed. With no registration the
caches still advance; re-adding a delegate does not synthesize those events.
Current owned init sets state3 and zero-initialized camera/microphone (148..152),
so it suppresses those first equal-valued observations. This is a source gap.

Physical attachment helper435f9c8 resets old receiver/native binding before
installing incoming physical-WC weak identity. If receiver+70 is absent, it obtains
the content media session435fbb4..bb8, binds receiver435fbc0..bcc, and calls its
AddObserver route435fbd0..bd8 ->6552de0. This happens on physical attach/rebind,
not on each delegate assignment. The bounded6552de0 body sends initial observer
payloads before adding its remote to the collection at6552f2c..f34. Current C156
`MediaSessionImpl::AddObserver` at1222..1235 corroborates the initial order: info,
metadata, images, position, actions, then registration. Exact native proxy names
are not inferred solely from stripped addresses; the observed native call chain
and current source corroboration are distinguished here.

Neither435f9c8 nor4360388 resets the info engagement flags. They belong to the
logical MediaImpl, across physical attachment changes. Current owned setDelegate
167..173 instead creates the observer lazily and destroys it on nil. Source-faithful
regular delivery needs observation/cache lifetime independent of registration;
nil delegate must not itself reset the native observation or its caches.

## Receipt snapshots versus delivery allocations

The current Mojo callbacks297..299 directly invoke ObjC methods255..288. Original
native receipt introduces an additional ordinary zero-delay task per invocation.
Info receipt updates and snapshots each changed scalar separately, in state,
camera, microphone order. It does not post one whole info object for later cache
comparison. Native unset flags distinguish first receipt from later equality.

Actions receipt43624c0..2538 filters/validates native enums: indices below21 are
kept,21 is skipped, values above21 trap on this native path. Its vector storage
is constructed at receipt;436264c..267c allocates/copies the queued payload before
posting. The selected invocation captures the copied vector, not the Mojo
argument reference. At actual bridge delivery441bfcc..c02c, an index-set object
is allocated and populated with addIndex:, then the delegate and owner are
weak-loaded441c030..044 and the callback sent441c05c. Do not prebuild the ObjC
index set when receiving Mojo. Current owned filtering tolerates every >=21;
the broader invalid-enum policy is a separate bounded difference, not permission
to add traps during this scheduling change.

Position receipt checks optional engagement4362c70..74; when present, it allocates
a native shared snapshot4362c78..ca0 and copies ALL40 bytes at4362ca4..cb0,
including the MediaPosition timing base. It updates its optional cache and
captures the native shared snapshot for the queued invocation. Absence clears
the cache4362d20..d40 and does not send a position delegate callback. At delivery,
bridge441c08c allocates ArcMediaPosition at441c0a4..bc using the selected native
snapshot, then weak-loads delegate/owner and sends441c0f0. Do not capture a borrowed
optional/reference, allocate ArcMediaPosition at receipt, or resample/fix dynamic
position/currentTime in the queue. This audit does not change DTO clock semantics.

## Selected bridge, cancellation, and logical lifetime

ObjC init441a238..25c copies and strongly retains the native MediaImpl handle;
owner storage is weak441a260..268. First nonnil setDelegate441a584..5d8 creates
a shared C++ bridge holding weak delegate+owner. Nonnil replacement441a4e4..4f4
updates the SAME bridge's weak delegate. Nil441a510..524 drops the service's
strong bridge without replacing the native observer. A later nonnil assignment
creates a new bridge and installs its weak registration through43458dc and
4360388. Native register storage+40/+48 is weak, not owning.

At producer receipt, the selected bridge weak pointer is locked before pending
acquisition. Its weak control block is then captured in the posted invocation
(state43616bc..1708); the temporary strong lock is released before returning.
Queued work does not retain the app delegate, owner, ObjC media service, or the
bridge strongly. At execution42c0890..08c0, that SAME registration is locked.
If alive, its bridge weak-loads the CURRENT delegate+owner at441bf7c..90 and
invokes. No nonnil-owner prerequisite is present. Nonnil replacement affects
already queued tasks. Nil/re-add cannot redirect them to the new bridge. If a
callback nils/re-adds reentrantly, its temporary delivery lock permits that
already-running old invocation to return safely; later old queued invocations
must lock their old registration independently. Do not cancel merely because
the current service points to another bridge, or retarget by owner lookup.
In particular, a nested run loop can execute another old queued invocation while
the first delivery still holds the old bridge alive. Nil/re-add alone does not
force expiration in that interval, nor while an independently exposed shared
bridge handle remains alive. The weak lock, not a generation/current-pointer
comparison, decides eligibility.

Pending++ is contingent on a LIVE C++ bridge, not a live app weak delegate or
respondsToSelector result. A live bridge with an expired delegate still acquires
pending. Null/expired bridge branches post separate uncounted no-op work (state
43617b8..1888, actions436256c..2638); their bound target15ac0 is ret. This work
does not justify an accounting increment. Counted invocation decrements after
conditional delivery returns42c08f4..08fc, including expired bridge42c0968..0970.
Unrun/rejected-task destruction does not decrement, per the primitive's proven
policy. Do not use one cancellation mechanism for both counted and uncounted work.

The OUTER queued thunk42c1740 also carries delegate-wrapper weak guard+60/+68.
Native constructor435f3c0 establishes it; within the recovered MediaImpl method
range the destructor435f700..710 clears/invalidates it. Physical helper435f9c8
does not change it: physical-WC absence/discard is not this guard's lifetime.
The logical WC constructor allocates shared MediaImpl436f470..4b4 and installs
its shared pointer/control block at WCImpl+2c8/+2d0,436f52c..534. Each regular
invocation copies a strong LOGICAL WC payload via42c8400 and retains its control
block (state4361644..1654). Consequently its payload keeps that logical MediaImpl
alive while queued, independently of ObjC owner/service and physical contents.
Service destruction441b400 releases its native handle, bridge and weak owner;
it does not authorize extra weak-service cancellation when the selected bridge
and native logical payload remain independently alive. Public native bridge
getters can expose shared handles, so service liveness alone is not equivalence.

The detach caller4372cc8..cd0 invokes435f9c8. The helper retires old physical
observation and emits synchronous PiP destruction through43639e8->4355b30->
441b718 before its old binding is cleared. The existing discard adjunct proves
logical WC native pointer+130 still refers to oldWC at this callback entry;
later removal/clearing and new attachment are distinct phases. Detach does not
invalidate selected registration, logical info caches, or wrapper guard. Do not
cancel accepted regular work by a physical-WC generation/WeakPtr check.

## Minimal next work and limits

Restore regular async fidelity separately before typed accounting: one shared
logical media source/cache state independent of ObjC service lifetime, persistent
observation until physical detach/rebind, service-owned shared bridge with weak
delegate/owner, and per-task selected weak bridge plus copied native payload.
No queued strong app references or new service/owner/physical-WC filters. Preserve
live-bridge acquisition, current-delegate replacement, old-registration expiration,
receipt-time cache decisions, and delivery-time ObjC allocation. A weak bridge
alone is equivalent to the proven regular native outer guard only when the
implementation also preserves its logical source/payload lifetime; this note
does not bless dropping that lifetime from the replacement architecture.

After lead approval, capture the exact canonical close state during logical
service/source/overlay construction before callbacks or teardown. PiP invocation
scopes retain only that handle. Regular pending is acquired at each selected
producer enqueue, and finalized after conditional delivery returns. Do not put
accounting around the later selector alone or retain an ObjC wrapper/delegate.

Still separate/gated: implementation of old/new physical attachment hooks,
repeated PiP destruction latch behavior and reentrant attachment mutation, native
unattached ownership, arbitrary public owner reassignment, complete original
proxy symbol naming, and runtime replacement/nil/deallocation comparisons.
Nothing here proves PDF/UI enqueue grouping, thumbnail, DevTools, BoostIDE, or
side-panel membership. No full-repository provenance, build/link, runtime, or
acceptance claim is made.
