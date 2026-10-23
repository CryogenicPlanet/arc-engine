# Core tabTraits audit — 2026-10-23

Scope: read-only original binary / current Chromium source / production and draft source review. Only this /tmp note was written. No apps, builds, production/native/shared changes, or board activity. Parent reports narrow TAB|AUDIO predicate changes in proposals; production unchanged. No runtime parity claim.

## Independently actionable predicate defect

Original NativeNavigationStateChanged 0x4385538 publishes traits iff TAB (bit1 / 0x2) OR AUDIO (bit4 / 0x10), exactly once per native event and per registered recipient. URL/TITLE/LOAD do not independently trigger traits.

| Relevant flags | Favicon | Traits |
|---|---|---|
| NONE, URL, TITLE, LOAD, or any combination of only those | No | No |
| TAB with any other flags except AUDIO | One | One, after favicon |
| AUDIO without TAB, with any other flags | No | One |
| TAB + AUDIO, with any other flags | One | One, after favicon |

Proof: 43859e4 tests TAB ->4386004; otherwise 43859e8 tests AUDIO ->4385a00 / GetTraits4385a08. LOAD's alternate lifetime path explicitly rechecks TAB at4385fa0 ->4385ff4; then AUDIO at4385fa4; neither goes43863fc (exit). Thus LOAD is not a wider gate. Favicon Get4376700 at4385ffc or4386020 then all normal cleanup paths converge438635c ->438637c ->GetTraits4386384 ->notify4386398 ->43863fc. There is no AUDIO recheck after TAB and no duplicate when both flags are present. Empty favicon does not skip traits. Refcount cleanup4386568..65a8/65f..6600 rejoins4386370; lifetimeempty4386470 rejoins438637c. General owner validity guard4385560/43855ec can skip all property dispatch, not a TAB-specific crash condition.

Before parent's narrow update, exact draft locations were `/tmp/core-navigation-stage/arc_web_contents.mm:366` (post setter after favicon) and `:468` (capture traits), both AUDIO-only. Both must use flags&(INVALIDATE_TYPE_TAB|INVALIDATE_TYPE_AUDIO). Preserve one post, after favicon, with event-receipt snapshot. This fix does not require solving nonempty mapping. Do not predict fresh counts from source alone.

Evidence: `/tmp/core-nav-dispatch-audit.txt:5019` onward; especially5320..5340,5687..5716,5926..5950,5995 and6050..6100. Current bit constants: `chromium/src/content/public/browser/invalidate_type.h:18`..28.

## Exact original representation and translation

GetTraits4378e28 obtains native WC at Impl+130; absent native ->empty. Lookup18efea8 returns corresponding tab object; absent ->empty. Tab unowned-userdata host is +310; keydf55990 points at stringd32d76a `TabAlertController::kDataKeyImpl`. Constructor36c66xx initializes active alert vector+b0/b8 at36c67ec/f0 and registers key36c6890..68a0. GetTraits copies cached vector+b0/b8, rather than recomputing audio/media state itself.

Original input0..18 -> Arc numeric trait table at c9d724c is:

`[1,2,3,4,5,6,7,8,9,10,11,12,0,0,0,0,0,0,13]`

Input unsigned>18 (including negative bitpatterns) maps0. Zero is retained, not filtered. Bridge4413050 creates NSMutableIndexSet, reads each uint32 at44130b0, accepts unsigned<=13 at44130b4..bc, invokes addIndex at4413098..30a0, then setTabTraits at44130d8. IndexSet naturally deduplicates. No alert priority filtering or only-highest selection occurs here.

Evidence: `/tmp/core-traits-original-producer.txt`, `/tmp/core-traits-original-table-identity.txt`, `/tmp/core-traits-helper-identity.txt`, `/tmp/core-traits-controller.txt`, `/tmp/core-traits-methods.txt`, `/tmp/core-traits-update-active.txt`; ObjC conversion `/tmp/core-wco-scheduling-audit.txt:106`..148.

## Semantic mapping supported by original producer behavior

Use named current enum cases, not numeric casts. Current enum definitions: `chromium/src/components/tabs/public/tab_alert.h:12`..31. The following correspondences have source/control-flow support:

| Original input | Current named source | Arc index | Original evidence / confidence bound |
|---|---|---:|---|
|0|kMediaRecording|1|7e21310 queries capture audio/video helpers3ace2a8/26c, erases0/12/13, inserts0 iff both; capture observer slots invoke same helper.|
|1|kTabCapturing|2|Capture indicator observer423353c ->UpdateAlertState1 at42335cc..35d8; slot follows video/audio observation slots. Strong behavior+interface match; not an original symbol name.|
|2|kAudioPlaying|3|Bound recent-audibility callback37b52f0 ->UpdateAlertState2(bool) at37b5360..536c. This is recently audible, not raw IsCurrentlyAudible.|
|3|kAudioMuting|4|Same callback37b5374..53b4 uses recent bool && native muted; separate muting observer4233248..3408 computes recent timestamp and two-second timeout before UpdateAlertState3.|
|4|kBluetoothConnected|5|Original capability fixedmap c9c4c54 maps capability1->4.|
|5|kBluetoothScanActive|6|Same fixedmap maps capability2->5.|
|6|kUsbConnected|7|Same fixedmap maps capability0->6.|
|7|kHidConnected|8|Same fixedmap maps capability3->7.|
|8|kSerialConnected|9|Same fixedmap maps capability4->8.|
|9|kPipPlaying|10|Original WCO slot338 ->4233118 ->UpdateAlertState9 at4233188..3194.|
|10|kDesktopCapturing|11|Window/display capture handlers4233678/4233844 OR incoming bool with other capture-source query ->UpdateAlertState10 at423379c..37a8 /4233968..3974. Strong behavior+interface match.|
|11|kVrPresentingInHeadset|12|VR secondary observation constructor36c6934..6984, interface+60/vtable da1fff8 thunk4233b40, handler4233a10 ->UpdateAlertState11 at4233a80..3a8c. Strong behavior+interface match.|
|12,13|kAudioRecording, kVideoRecording|0|Original shared capture helper distinguishes single-type capture and translates either to0.|
|14..17|kGlicAccessing,kGlicSharing,kActorAccessing,kActorWaitingOnUser|0|Exact translation table, although individual original14..17 semantic producers not separately re-audited. Do not map to13 or omit0.|

Original capability method4232f40..3114 uses exact fixedmap {(0,6),(1,4),(2,5),(3,7),(4,8)} before UpdateAlertState(mapped,used). Current public capability enum `content/public/browser/web_contents_capability_type.h:13`..20 is USB,BTconnected,BTscan,HID,Serial and current controller325..349 has same table. Hardware-subsystem-to-original-capability producer identities were not each independently traced; named mapping is supported by matching public interface/table, not equal enum ordinals alone. Current capture/PiP/VR behavior counterpart is `tab_alert_controller.cc:352`..425 and480..502.

## Original extra input18 is active WebRTC ICE, not present in current TabAlert

Original peer-tracker secondary interface atthis+48 (vtable da1ff70) routes Added4234020 / Removed423408c / Updated4234298. It filters `RenderFrameHost::FromID` -> owning WC against this controller's WC at42340b4..c8 /42342cc..e0. Tracks peers by GlobalRFHId + local peer id (lid) in vector128/130. Updated checks exact type `oniceconnectionstatechange` at4234310..4364. Quoted values connected/completed/checking mark peer active; failed/closed/disconnected/new clear active. Any active peer ->UpdateAlertState18 via4234448..4608, which Arc converts to index13. Removal recomputes anyactive4234174..4184; discard/replacement callback4232c98 clears peer vector and alert18 at4232e04..e14 (exact event identity proven in follow-on below; it is NOT a navigation event).

Current `content/browser/webrtc/webrtc_internals.cc:258`..264 independently uses identical type/value groups. Public UI-thread `content::PeerConnectionTrackerHostObserver` exposes Added/Removed/Updated (`content/public/browser/peer_connection_tracker_host_observer.h:21`,34,44,61); constructor199 registers observer (destructor unregisters). Therefore reproducing custom13 can be owned code using this existing public interface, with original per-frame-owning-WC filter and peer-key cache. It is not available from TabAlertController::GetAllActiveAlerts or global connected-peer count. Reset is the tab will-discard callback, not primary navigation (see follow-on). Preserve its exact rebind/reset behavior; do not clear ICE peers on ordinary navigation, invent initial peer state, or mirror global counts.

## Cache timing and safe implementation requirements

Current public APIs: `tabs::TabInterface::MaybeGetFromContents` (tab_interface.h103) is nullable, matching original missingtab->empty; `TabAlertController::From` (header52) is nullable; `GetAllActiveAlerts` (header76, cc302) returns all active cached alerts. Do not use GetFromContents which crashes on standalone WC or GetAlertToShow which drops simultaneous alerts.

Current first-audible edge ordering is concretely: `WebContentsImpl::OnAudioStateChanged` sends AUDIO atweb_contents_impl.cc3159, then WCO OnAudioStateChanged3167. RecentlyAudibleHelper WCO59..78 updates recent state and notifies subscribers; TabAlertController457..466 then updates audio2/3. Thus a nativeflag receipt snapshot can see previous cached audio alerts on this first edge. Chrome NavigationStateChanged623..660 does not synchronously refresh that cache. Muting WCO precedes AUDIO (WC2931..2934); timer expiry callbackfalse precedes AUDIO (recently_audible_helper.cc81..93). PiP likewise TAB2973 precedes PiP WCO2974 and controller cache update352..355. Follow-on original sender audit below now proves the same delegate-before-WCO order for audio and PiP; the previous sender-order uncertainty is resolved. This permits preserving receipt-time cached values, including prior values on those edges. Do not replace cache with raw audibility or move snapshot delivery later to hide this without paired receipt evidence.

Minimal safe staged mapping plan after review: receipt snapshot original-shaped all-alert cache; explicit named switch above including0 fallback, fresh NSMutableIndexSet; add own source-proven ICE cache for13; preserve TAB|AUDIO publication once, normal UI post, weakowner delivery. Rebind must reattach peer/native observations while preserving wrapper identity and clear only on proven reset events. Preserve the now-proven sender/cache ordering below; do not add observer-triggered publications beyond the native flags merely to expose the newly updated cache.

Current production `arc/api/arc_web_contents.mm:750` and initial construction still use empty set; pre-update generic draft capture468..472 also empty. Nonempty states are a real implementation gap. Existing run6 recorder sees opaque NSMutableIndexSet object identities, not their contents (`logs/wp/core.run6.triage.md:67`..74); count/ordinal differences cannot establish numeric set equality. A future bounded native/ours receipt probe must record flags, cached native alert names/indices, custom ICE state, and resulting set contents at receipt plus posted delivery, under authorized app lock. No values should be synthesized or suppressed to match counts.


## Follow-on: original sender ordering and exact ICE reset (2026-10-23)

### Audio first edge: original matches current

Original `WebContentsImpl::OnAudioStateChanged` begins678c9f0, identified by TRACE string at678cb44 (`WebContentsImpl::OnAudioStateChanged`). It calculates new audibility, skips unchanged value at678ca70..74, writes current/ever-audible WC bytes+102a/+102b at678ca78..84, then broadcasts renderer audio state via259bfd4 at678ca9c. It next calls `NotifyNavigationStateChanged(AUDIO=0x10)`678caa0..aa8 ->1836d6c. Only afterward does it dispatch native WCO OnAudioStateChanged slot270:678cab4..cac4 ->24a717c. Outer-WC recursion678caec occurs after own flag and before own WCO dispatch; it returns to678cab4. Current source counterpart WC3159 before3167 has the same order.

The common NotifyNavigationStateChanged1836d6c performs audio-media bookkeeping at1836de8..df0 then invokes actual WC delegate from WC+e0, vtable+10 at1836da0..db8. The original ArcImpl handler is4385538, which captures cached controller vector4378e28 before forwarding flags at4386438. This is synchronous native delegate invocation; later ObjC setter is posted with captured values. The cache's recently-audible callback37b52f0 updates inputs2/3 when the later helper observation reaches it, not when the queued ObjC setter runs. Thus receipt can intentionally contain previous audio traits on an edge. A late `GetAllActiveAlerts` query at posted delivery would change original behavior.

For muting, the original sender also matches current: native SetAudioMuted calls WCO slot2a0 at678c63c..64c, then AUDIO invalidation678c650..658. Therefore muting cache may already be updated at capture. No uniform rule to query raw WC audio booleans reproduces both cases.

Evidence: `/tmp/core-traits-audio-sender.txt` (678c9f0..cb98 and1836d6c); `/tmp/core-traits-pip-sender.txt` also contains SetAudioMuted678c63c..658. Current sources WC2931..2934 and3159..3168; helper59..78; controller457..466.

### PiP first edge: original matches current

Original video setter678c780 skips unchanged678c7b0..7dc, writes WC+f90 at678c7e0 and calls common678c70c at678c7ec. Document setter678c840 writes WC+f91 at678c8a0 and calls same helper678c8ac. Common saves bool678c720, calls native delegate `NavigationStateChanged(TAB=2)`678c724..728 ->1836d6c FIRST. It dispatches WCO MediaPictureInPictureChanged slot338 only afterward678c72c..73c ->24a717c. Controller4233118 then updates cached input9. Visibility adjustment follows678c740..754. Current source WC2973 then2974 is the same. Direct snapshot of controller cache at TAB receipt is source-matched even though raw WC PiP bool already changed. Do not recompute input9 from raw WC PiP flags or add a PiP-observer trait setter.

Evidence: `/tmp/core-traits-pip-sender.txt`, `/tmp/core-traits-pip-delegate.txt`.

### ICE reset exact identity: tab will-discard / replacement

Correction to the earlier audit wording:4232c98 is `ContentsObservingTabFeature::OnDiscardContents(TabInterface*,oldWC,newWC)`, NOT PrimaryPageChanged or DidFinishNavigation and has NO mainframe/samedoc navigation test.

Direct binding proof: original base constructor23db170 stores tab atthis+20 (23db1ec), reads memberfunction pair(0x408,1) fromd733150 at23db244..250, binds it tothis, registers on tab+a0 discard callback list23db254..25c, then observes tab+40 WC23db274..27c. Subclass constructor36c66c0 calls that base and replaces vtable withda1fab8, whose appended+408 method is4232c98; this lies after ordinary WCO virtual surface and is a tab feature callback. Handler checks tabargumentx1==this+20 at4232cb4..b8, saves newWCargumentx3 at4232cbc, calls Observe(newWC)23db2f4 at4232cc8, resets/rebinds VR and recently-audible helpers through4232dbc, clears peers vectorcount+130, and removesinput18 at4232e04..e14.

Current exact counterpart `chrome/browser/ui/tabs/contents_observing_tab_feature.cc:13` registers `RegisterWillDiscardContents`; its OnDiscardContents21..26 CHECKs tab and Observe(new_contents). Current TabAlertController.cc310..323 rebinds the same VR and recent-audible subscriptions. Custom ICE peer cache should clear on this native replacement notification and rebind to new WC, not clear on ordinary navigation or same-document changes. Parent's wrapper migration design must respect that this native tab-feature event can occur before controller CopyStateFrom completes; this traits cache reset does not justify snapshotting new history at that time.

Evidence: `/tmp/core-traits-discard-base.txt`, `/tmp/core-traits-methods.txt:43` onward. No new native hook is needed solely for observing peer events; UI-only public PeerConnectionTrackerHostObserver constructor/destructor register/unregister in `content/public/browser/peer_connection_tracker_host_observer.cc:13`..24. A wrapper-owned observer/cache still needs explicit existing native replacement/rebind handoff so it clears at the original event.

Remaining implementation boundary: enum mapping/gating and original first-edge timing now have source support. Nonempty current alert retrieval and custom ICE cache remain unimplemented/unprobed. No runtime claim that all numeric sets/counts match. Preserve capture at native event receipt and exact native flags; no extra KVO publications from alert subscriptions.

## Final ICE edge semantics and registration audit

This section completes the provider's cache contract. Original methods and pure helpers are in `/tmp/core-traits-methods.txt`, `/tmp/core-traits-update-active.txt`; constructor/list-registration chain is in `/tmp/core-traits-peer-registration.txt`.

All Added/Removed/Updated events first resolve `RenderFrameHost::FromID(frame_id)` and compare `WebContents::FromRenderFrameHost(frame)` with this controller's observed WC. Null RFH or different WC means complete no-op. There is no primary-frame, current-page, same-document, or active-navigation condition. The peer key is the complete `(GlobalRenderFrameHostId, lid)`; pid, URL and RTC configuration do not affect it. Added primary implementation4233c80..3cd8 and its secondary-interface equivalent4234020..4078 show the same filter. Removed filter42340b4..40c8; Updated filter42342cc..42e0.

- **Added:** upsert key, then unconditionally set its active bit false. Duplicate Added resets an existing true bit. Helper4233cec performs lower_bound/exact-key matching and returns existing value address on the duplicate path4233d94..3dfc ->4233efc; absent keys allocate/insert, with initial false4233e90/4233f2c/4233fd8. Caller stores false again4233cd4/4234074. Added does NOT recompute alert18 or notify anything.
- **Updated, unknown type:** complete no-op, including the type `close`. Exact type must be `oniceconnectionstatechange` (26 characters, no quotation marks) at42342fc..4364.
- **Updated, missing key:** complete no-op; it never creates a peer from an update. Lower-bound/exact-key miss exits4234388..43dc ->42342e4 before value parsing or aggregate recomputation.
- **Updated, existing key and exact type:** parse exact strings INCLUDING literal double-quote characters. `"connected"`, `"checking"`, `"completed"` set true; `"failed"`, `"disconnected"`, `"closed"`, `"new"` set false. Unquoted values are unknown. Known true branches4234494..4504/4234564..459c; known false branches423451c..455c/42345a0..45e0/4234614..463c. An unknown value leaves the stored bit unchanged, but still recomputes `any(peer.active)` and updates cached alert18. This is not an early return: all unknown-value paths go42345f0 then scan4234448..445c then UpdateAlertState18 at4234604..460c.
- **Removed:** after the same own-WC filter, erase exact key if present. An absent key still recomputes aggregate alert18 from the unchanged map; empty map recomputes false. Removal exact-key/erase42341ac..4288 converges4234174; scan423418c..41a8 and UpdateAlertState18 at423417c..4184.
- **Discard/rebind:** clear peer map and set cached alert18 false, as already proven. Ordinary navigation does not reset it.

The provider needs a **separate cached ICE alert boolean**, not merely a map queried at snapshot. Exact illustrative sequence: Add(A), Update(A,connected), Add(A) again. The map's A bit is now false while cached alert18 remains true because Added does not recompute. A native TAB/AUDIO snapshot still contains Arc index13. A subsequent valid-type/known-peer update with unknown value recomputes false and removes it. A derived-at-snapshot implementation would prematurely remove index13.

ICE handlers are CACHE-ONLY, not even highest-alert-change notifications. Their only non-filter mutation callees are peer insertion/erase and `UpdateAlertState(18,bool)`7e21264; for18 its branches7e21288..212d8 insert/erase the active-vector value without callback. They never invoke alert notifier7e21480 (called explicitly around ordinary audio/PiP/VR updates), native `NotifyNavigationStateChanged`1836d6c, delegate virtual calls, AUDIO/TAB flags, ObjC setters, or UI tasks. Adding any such notification is unsupported.

Registration timing: controller constructor36c669c first calls tab-feature base23db170 at36c66c0, establishing initial WC observation and discard subscription. It initializes peer observer secondary base atthis+48 (36c6700..6748) and registers it at36c674c..6750 ->266173c before final controller vtables36c6784..67a4, empty active-vector36c67ec/f0 and empty peer-map36c68a4/a8. Registration266173c performs UI checks/lazy global list initialization;266187c checks duplicate observer then appends weak observer pair through575f20 at2661918 and returns. Full575f20..5760c8 is vector growth/ref release, with NO peer enumeration, virtual event call, or replay. No initial ICE seed/query is justified. Current public `PeerConnectionTrackerHostObserver` constructor/destructor (`content/public/browser/peer_connection_tracker_host_observer.cc:13`..24) similarly adds/removes the observer, and tracker-host.cc55..67 only adds/removes from its list. Instantiate the state before the peer events it must observe; creating it lazily at the first trait snapshot could miss events and has not been proven equivalent to the original native tab-controller lifetime.

Current public signatures (header34/44/61): Added(GlobalRenderFrameHostId,int,base::ProcessId,const std::string& url,const std::string& rtc_configuration); Removed(GlobalRenderFrameHostId,int); Updated(GlobalRenderFrameHostId,int,const std::string& type,const std::string& value). All callbacks and registration are UI-thread-only. Do not add an unrelated global connection-count observer or import WebRTCInternals' broader `close` handling; its quoted ICE value groups corroborate strings only.

Parent corrections now prepared and reviewed: TAB|AUDIO capture/publication once after favicon, plus receipt-native-vector/per-recipient-delivery ObjC materialization before weak owner. Current core_navigation_registration.patch SHAbb79407accb0f13e99b222cfe329ee3e77556f8a7d9984b5af313c01a00d62a1; WC-only core_observer_registration.patch SHA7e1cca4e5a5407a98f341eb93d8b6101e09310c6deac81d81fa89d08ce07f300. Strict sequential four-part apply, actual generic/observer/combinedWC/render compilers0, fresh review clean. Evidence logs/wp/core.traits-materialization.latest.json and logs/wp/core.bundle-current.20261023T133616Z/manifest.json. Still UNAPPLIED; no runtime parity claim.

Isolated complete alert/ICE helper proposal core_tab_traits_proposal.patch SHA991106bc016a4cb678f4b00b40e46c1180d608f721597cac08af612c9da30173 has actual compiler0/apply0/fresh CODE review clean; logs/wp/core.tab-traits-proposal/. It remains HOLD for native tab-controller creation lifetime, exact will-discard rebind, caller/GNI/dependency/link/runtime integration. No production provider installed.
