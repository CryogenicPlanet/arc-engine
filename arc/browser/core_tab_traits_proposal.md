# Core tab traits — staged helper only

2026-10-23. No production, native, shared-header, GNI, app, build, lock, board,
credential, or provider/caller changes. This proposal is not activated and has
no runtime parity claim. Parent remains the production writer and reviewer.

## Files and contract

- `arc/browser/core_tab_traits.h`: UI-thread-only `arc::CoreTabTraitsState`,
  constructed with a nullable native WC, vector alias `CoreTabTraitsSnapshot`,
  receipt `SnapshotCachedTraits()`, delivery `MaterializeCoreTabTraits(snapshot)`,
  and explicit `RebindForDiscard(new_contents)`.
- `arc/browser/core_tab_traits.mm`: named alert mapping, nullable cached-alert
  lookup, weak-WC source filtering, and separate per-peer/cached ICE state.

The class inherits the public `content::PeerConnectionTrackerHostObserver`.
Its existing base constructor/destructor register/unregister on the UI thread;
there is no new registration machinery, initial peer replay, seeding, delayed
work, flag invalidation, alert-change subscription, app callback, or KVO setter.
The helper retains no WC, wrapper, delegate, frame, tab, or controller.

Snapshot copies a `std::vector<uint32_t>` at receipt with NO ObjC allocation.
It preserves cached alert order and duplicate mapped indices, including zero.
Absent WC,
`tabs::TabInterface::MaybeGetFromContents`, or `tabs::TabAlertController::From`
returns an empty vector BEFORE considering custom13. Otherwise it reads
`GetAllActiveAlerts()` synchronously and maps named MediaRecording, TabCapturing,
AudioPlaying, AudioMuting, BluetoothConnected, BluetoothScanActive, UsbConnected,
HidConnected, SerialConnected, PipPlaying, DesktopCapturing, and
VrPresentingInHeadset to Arc1..12. All other current cases, kNone, and future or
unknown values map0; zero and duplicates remain in the vector. Custom13 is
added solely from the separate CACHED ICE aggregate, never derived at Snapshot.

`MaterializeCoreTabTraits` allocates a NEW NSMutableIndexSet, adds each unsigned
value<=13 (including0), and returns it. IndexSet deduplicates at this point only.
The caller must invoke it in EACH recipient's existing delivery task BEFORE
loading the weak ObjC owner, including empty snapshots and expired owners.
Native bridge4413050 constructs the set at4413074, then weak-loads owner44130c4,
then invokes setter44130d8. Do not hoist construction to receipt, share one set
across registrations, or skip construction because the weak owner has expired.

Each peer event resolves `RenderFrameHost::FromID(frame_id)` and requires its
`WebContents::FromRenderFrameHost(frame)` to equal the live weak own WC. There is
no main-frame/current-page filter. Peer key is `(GlobalRenderFrameHostId,lid)`;
Added's pid/url/configuration are irrelevant.

- Added upserts key=false, including duplicate keys. It DOES NOT recompute the
  cached aggregate, so duplicate Added can leave cached13 true while all peer
  bits are false.
- Updated requires EXACT type `oniceconnectionstatechange` and an EXISTING key.
  Values include literal double quotes: `"connected"`, `"checking"`,
  `"completed"` set true; `"failed"`, `"disconnected"`, `"closed"`, `"new"`
  set false. An unknown value leaves that peer unchanged but recomputes the
  aggregate. Unknown types, including `close`, and missing keys do nothing.
- Removed erases an existing key, if any, then recomputes the aggregate EVEN
  when the key was absent, provided the own-WC source filter passed.
- `RebindForDiscard` changes the weak WC and clears peers plus cached13. It is
  reserved for the proven native will-discard/replacement handoff, not ordinary
  navigation, primary-page changes, or tab transfers.

## Source evidence

Read `arc/browser/core_traits_publication_notes.md` fully. Input hashes are in
`inputs.json`. Exact native ICE follow-on received from core_navigation_recon
and independently available in `/tmp/core-traits-audit-final.md:104` onward
(recon-reported SHA256
`693ded547c36cce406d4a1af79d8e46f22cc9b1074ca52c146f2a329208b68ab`).

Original GetTraits4378e28 returns empty before reading alerts if native/tab/
controller is missing. Translation tablec9d724c maps original0..11 to1..12,
12..17 to0, custom18 to13, all unknowns to0. Semantic named mapping is supported
by the behavior/interface evidence recorded in the publication notes; it is
not based solely on matching current enum ordinals.

Original ICE Added4233c80/secondary4234020, Removed423408c, Updated4234298:
same-owning-WC filter, peer key, exact type/quoted values, cache rules above.
Update18 only inserts/erases active alert cache (7e21264); it does not publish a
TAB/AUDIO flag or highest-alert notification. Original peer registration
36c6750 ->266173c ->266187c ->575f20 only appends, with no replay/seed.
Evidence files: `/tmp/core-traits-methods.txt`,
`/tmp/core-traits-update-active.txt`, `/tmp/core-traits-peer-registration.txt`.

Reset4232c98 is proven `ContentsObservingTabFeature::OnDiscardContents`, bound
from tab discard callbacks by23db170 and subclass vtableda1fab8+408. It rebinds
observation, clears peer count and input18. Current counterparts:
`chrome/browser/ui/tabs/contents_observing_tab_feature.cc:13..26` and
`chrome/browser/ui/tabs/alert/tab_alert_controller.cc:310..323`.

Public API signatures/registration:
`content/public/browser/peer_connection_tracker_host_observer.{h,cc}`;
enum `components/tabs/public/tab_alert.h`;
nullable lookup `components/tabs/public/tab_interface.h` and
`chrome/browser/ui/tabs/alert/tab_alert_controller.h`.
Current quoted-value counterpart: `content/browser/webrtc/webrtc_internals.cc`
OnPeerConnectionUpdated. That broader implementation's `close` behavior is NOT
adopted; the original trait-specific observer treats it as an unknown type.

## Validation

`python3 /tmp/core-tab-traits-stage/check.py`: actual compiler exit0, no
diagnostics. The checker queries the EXISTING `arc_browser.o` siso compile
command, substitutes only the staged source, prepends the stage include root,
strips object/dependency outputs, and adds `-fsyntax-only`. Exact command is
`compiler-command.json`; output is `check.log`. No GN generation, build target,
object output, or native/AppKit execution. Patch addition apply-check is recorded
in `apply-check.log`; `core_tab_traits.patch` contains just the two new files.
No runtime test, paired recorder run, or numeric-set parity claim.

## Integration HOLD / prerequisites

The parent must review creation lifetime and exact native replacement handoff
before connecting this provider. Original peer observation begins with native
TabAlertController construction; constructing lazily at first Snapshot can lose
prior peer events. Merely registering cannot replay historical peers. Do not
claim equivalent lifetime until parent integration accounts for this.

Connect Snapshot at the existing TAB|AUDIO event RECEIPT. In EACH existing
recipient task, materialize its fresh set BEFORE loading the weak ObjC owner
and publishing. First-audio/PiP edges intentionally
can expose previous controller cache values because invalidation precedes WCO
cache updates. Do not query again at delivery, recompute from raw audio/PiP
booleans, add publications from these peer handlers, or add new scheduling.

Rebind/reset must occur at the original native will-discard handoff while
preserving logical wrapper identity, coordinated with sessions and controller
replacement timing. Do not call Rebind on ordinary navigation. The helper alone
does not resolve early controller construction, wrapper materialization, existing
replacement wiring, package/GNI dependency visibility, or runtime set coverage.

The initial receipt-time NSMutableIndexSet API was SUPERSEDED by the source-
proven receipt-vector/delivery-materializer API above. See SUPERSEDED.md.
