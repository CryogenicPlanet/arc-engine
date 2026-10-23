# Media delivery candidate — unapplied, revision 2

Revision 2 supersedes the first freeze after fresh read-only review. Exact v1
owned patch/notes/test and manifests remain in
`logs/wp/webextras-media-delivery-candidate-v2/archive-v1/`; original v1 evidence
under `logs/wp/webextras-media-delivery-candidate/` is unchanged.

This is staged implementation evidence, not runtime or acceptance evidence. Only
owned proposals, this note, the standalone test, and candidate logs were written.
No actual API/header/Chromium source, maintained patch, shared test, engine build,
runtime, app/profile, lock, or public Close path was changed by this work.

The two proposals must be reviewed together:

- `webextras_media_delivery.patch`: replacement of owned media implementation.
- `webextras_media_eagercorehookproposal.patch`: minimal shared header declaration
  and core initialization call, after services/canonical state and before initial
  observable setters. This shared change requires lead integration.

Current staged media SHA256 is
`cf829322d53d3a316e8916b838ced02cec10f5fd41bda5139feb7858e619c283`.
Delivery patch SHA256 is
`d6ba6caf28a403a87fb8d9e8417c0f164c09bcaed9b3f158e9454f37d1c3acda`;
eager hook patch SHA256 is
`86b4490c9da7294336cc726ccdc9a0a8c9e81247d861fb7629844bb3be0d7f73`.
Exact inputs, staged/current/c63b compositions, commands, hashes and checks are
recorded in `logs/wp/webextras-media-delivery-candidate-v2/validation.json` and
`freeze.json`. No original source claim relies on test callback counts.

## Native evidence and resulting implementation

The frozen delivery audit is `webextras_media_delivery_audit_notes.md`, SHA256
`ad88d9a22031f421bb655979adf94a1a13655b97962111f6db1dbae53bd02ba9`,
with manifest SHA256
`de019558a5b9bfbc5ac69124d88b57bbe763ff1990dfcc214e335871bb2c0f07`.
It independently bounds constructor/attachment, all five existing regular async
producers, eleven PiP synchronous producers, registration, bridge delivery and
logical native lifetime. The original image hash is
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.

Native 435f32c initializes info/position cache engagement as absent;
435f9c8 subscribes at physical attachment independently of delegate registration;
43614d8, 4362460 and 4362c0c update/copy native receipt values and enqueue regular
invocations. Native service setDelegate 441a4bc changes a weak app registration
without replacing observation. The implementation now follows that structure:

- A shared functional `WebextrasMediaSource` owns the observer, unset comparison
  caches, latest native position snapshot, weak registration, dispatch and PiP
  destruction latch. It holds only weak ObjC owner/service references and a
  Chromium WeakPtr for physical contents. The observer refers weakly back to it.
- A private owner-associated `WebextrasMediaSourceBox` retains that source before
  the public service exists. `InitializeWebextrasMediaSource` creates/publishes
  the source and subscribes without calling the public mediaSession getter or
  constructing the ObjC media service. Lazy service construction adopts it.
  Observer initial messages remain genuine asynchronous Mojo receipts. There is
  no cache preseed, setter-derived event, initial flush, or synthesized callback.
- The service alone owns its current shared C++ bridge. The source's last
  registration and queued selected registration are weak. The bridge contains
  weak delegate and owner. Nonnil replacement updates the same bridge; nil only
  drops the current strong bridge. Nil/re-add creates a different bridge without
  redirecting old accepted work. A source retained by work cannot indirectly
  retain the service, current bridge, app delegate, owner or PiP ObjC cache.
- Each accepted regular invocation retains actual functional source state and a
  copied native payload. Delivery locks its original selected bridge, constructs
  DTOs at delivery, then weak-loads the current delegate and owner. Nil owner is
  permitted. No service/current-registration/native/generation filter is added.
  Actions copy the filtered native vector; position copies its complete native
  value including timing base. Absent position clears the receipt cache and
  sends no position callback. Scalars compare at receipt, separately in state,
  camera, microphone order. A no-registration receipt still advances caches.
  Its uncounted inert task retains the functional Source: original state
  4361858..1870 and actions4362608..2620 retain a logical WC payload even on
  this path. It does not acquire pending or retain the bridge/app objects.
- All eleven existing PiP selectors use the common synchronous bridge lease.
  The selected `std::shared_ptr` has C++ scope lifetime through app return;
  weak-loaded delegate/owner locals have `objc_precise_lifetime`. Nested nil,
  replacement and run-loop draining may still call the old registration while
  an outer invocation leases it. Creation and playback remain separate calls,
  with the existing overlay weak check between them. Creation uses SyncPrepared:
  select/lease the bridge before DTO creation, then weak-load ObjC targets, as
  441b6a4..d8 proves. Favicon conversion occurs inside Sync after those weak
  loads, as 441bb00..38 proves. Position/title/favicon native inputs are copied
  before Sync (position4364e20..38; title43653c0..d4 and43655dc; favicon
  4365838..5854). Position441b9d4..ba04 and title441ba60..98 weak-load targets
  before unconditionally converting the DTO/string, even with an expired app
  delegate. Conversion remains inside the selected bridge lease, before the
  respondsToSelector check. No sync callback is posted.
- The canonical PiP ObjC controller remains service-owned. Source references the
  service weakly solely to maintain that cache, without gating notification on
  service liveness. Physical retirement detaches the observer/Mojo receiver
  before synchronous PiP destruction. Accepted regular work remains eligible.

The Source is not an alive token: retained tasks retain its real observer, cache,
registration and PiP state. It does not retain Chromium WebContents or an ObjC
wrapper to emulate native logical ownership. Native logical WC payload retention
also retains sibling peers, including DevTools. Source-only retention proves
retention of Media functional state; whole logical-peer lifetime equivalence is
NOT established and remains an activation hold.

Original getter proof was supplied and read during this stage. At 441b098 the
native position getter loads held MediaImpl; 441b09c tests its cached engagement
at +c8; 441b0a4..bc copies cached +b8/+c0; 441b0c8..e0 allocates a fresh DTO.
It does not read live session/owner/physical WC or store the DTO in service
`_position`; 441b108..11c returns it directly. Receipt 4362d20..40 clears absent
position; attachment does not clear this cache. The candidate getter therefore
returns a fresh DTO from Source's latest receipt snapshot, including after
physical retirement. Synthesized property metadata/setter storage is preserved.
A narrow `-Wunused-property-ivar` diagnostic scope on this getter is necessary
because native's metadata-backed setter ivar is deliberately separate from this
getter's cache; no dummy read or extra retained DTO is introduced. Raw getter
instructions are retained in `native-position-getter.asm` in the candidate logs.

`getMediaContentTypes`, canonical PiP getter, action/error methods and the volume
unavailable path remain byte-identical. The live player enumeration used by
getMediaContentTypes is not replaced by this position cache.

The separately frozen volume audit, SHA256
`263b9fc4` (full hash in the freeze manifest), proves a sixth async producer at
435fd88/435ff54 and bridge441bef8. The candidate includes private
`Source::VolumeChanged(double)` and protocol selector `webContents:didChangeVolume:`.
It has no caller until the separate renderer-double/Mojo/WCO producer proposal
is composed. No hardcoded zero, mute-to-volume conversion, clamp, browser dedup,
or setter-based notification is added. The observed golden missing callback4108
is not repaired by an unconnected entry point.

## Checks and their limits

`python3 arc/browser/webextras_media_delivery_test.py` reconstructs the delivery
proposal in a private directory, extracts actual proposed delivery/cache/Source,
actual service setDelegate and position getter, PiP create/destroy, and the nine
remaining actual overlay callback bodies. It compiles with Foundation/ARC at
`-O2 -Wall -Wextra -Werror`, and executes mock tasks/native values/observer types.
The test result and generated translation unit are retained under candidate
logs. This checks production logic rather than a copied delivery model.

Executed checks cover first unset3/0/0 versus later equality; receipt order and
async delivery; independent info changes; filtered copied actions; all five
40-byte-shaped position payload words; DTO allocation after delivery bridge
lock; no DTO for expired registration; position absent clear; fresh getter DTO
identity and stable held snapshots; newer receipt while an older callback waits;
cached getter after physical retirement; same-bridge nonnil replacement; nil;
nil/re-add; expired delegate; nil owner; nested old-bridge delivery through sync
nil and nil/re-add; strong weak-load delegate lease during nested drain; source
outliving service/physical retirement with an independently exposed bridge;
observer/cache persistence through nil; no initial replay; weak service/PiP cache
release; reentrant nil/re-add during PiP controller construction and same-bridge
replacement during favicon conversion; mutation/free of caller-owned PiP
position/title/favicon inputs during conversion; allocation with an expired app
delegate versus no registration; no-registration inert-task Source lifetime; all
eleven sync PiP selectors; and the existing destruction latch.

Whole staged media and core syntax checks use the actual out/Base compile flags,
warnings and compiler plugins through private VFS overlays. Strict git-apply
checks and byte reconstruction pass in both proposal orders. The eager hook also
composes with c63b adapter stage media/core: both syntax checks pass with the exact
existing native host_app_hooks dependency recorded in its original manifest.
Its insertion is after `_coreCloseState` creation and `_services`, before setters.

A CORE worker independently advanced actual shared core source from frozen
bc62386a to ff957456 (quit suppression) during this work. The small hook also
strictly applies to that current source, and its separate current composition
passes real-flags syntax. The same checks also pass over current bec65249 adapter
composition, whose quit guard supersedes c63b; seven source syntax checks total
(base media/core, c63b media/core, bec652 media/core, latest current core) and 25
recorded strict application/composition checks pass. This proposal does not revert or edit that change.

These are Foundation task tests and source/type checks. They do not exercise
Chromium Mojo delivery, native physical discard/rebind, link/export integration,
real app delegate lifetimes or engine parity. No diff scenario was run, and no
new acceptance status follows from these checks.

## Activation holds and requests to lead

1. Approve/integrate the eager shared initialization hook together with delivery.
   Applying only the owned source leaves fallback initialization late, losing
   the proven initial attachment/cache timing. Actual current core lacks the
   unapplied canonical close state (current adapter bec65249); no fake close state is created here.
2. Add a separately audited early discard/detach/rebind hook. Current candidate
   provides initial attachment and a WebContentsDestroyed fallback. That fallback
   detaches observation before the app PiP callback but does not prove native's
   pre-discard timing where the old logical binding remains app-queryable.
   Source.contents_ is not public owner.webContents; the CORE observer can clear
   that binding independently, so the Source WeakPtr alone proves no public old
   binding guarantee. Whole native logical WC/sibling-peer payload retention
   must also be reconciled without retaining an ObjC wrapper/physical WC. The
   early initializer deliberately does not invent a rebind policy for an
   existing logical source. Reentrant destruction/reattachment and stale overlay
   mutation remain HOLD, not presumed equivalent from the mock test. Arbitrary
   public owner reassignment and native unattached ownership remain uncovered.
3. Add the typed close-accounting API companion only after this source candidate
   is reviewed: capture the exact canonical state before subscription, use Media
   synchronous scope for each PiP invocation and pending at each live selected
   regular enqueue, and finalize only after conditional delivery returns.
   Preserve no-registration inert work and native rejected/unrun-task policy.
   No public Close activation or accounting API was included here.
4. Compose the separately reviewed volume producer/consumer dependency and then
   run fresh paired scenarios, including actual replacement/nil/deallocation,
   attach/detach, initial observer timing, and reentrant callback lifetime cases.
   No count padding, callback suppression by scenario, or golden-derived values
   are part of this proposal.

The reviewed class spec's old statement that callback scheduling was unverified
is superseded only for these bounded native producers by the delivery audit.
The receipt-cache getter is source-proven, rather than a speculative replacement
of other live session getters. No contradiction to ARCHITECTURE.md is asserted.
