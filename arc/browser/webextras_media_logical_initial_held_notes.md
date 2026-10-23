# Initial functional Media peer — held proposal

This distinct incremental proposal is **UNAPPLIED / HOLD**. Production, prior
frozen proposals/evidence, Chromium, maintained patches and shared files are
unchanged. No engine build/run, app/profile action or shared lock was used.

## Exact inputs and scope

Base Media is the previously reviewed V2 + retirement-order correction + Media
accounting + volume consumer composition, SHA256
`e38db1cddc3a2ec948d052218f733ac67d081aa78e2907ad91da20caa9dc4b82`.
The root dependency is the reviewed bare library proposal
`core_logical_web_contents_library.patch`, SHA256
`d4b42613459f6d3f3fcaea59170bcfdb9fc9ac633a9a39b4521ef2ac727f4b58`.
Its exact header/implementation are copied unchanged into private staging and
executable-test extraction. Both actual peer slots must exist; no production
DevTools substitute, fake state/token, global registry, root factory fallback or
wrapper resurrection is introduced here.

Only initial ownership, receipt and accepted-work lifetime are implemented.
Bare Root has an immutable initial native weak binding. This proposal supplies
no tab-side root holder, canonical native-to-root route, ownership endpoint,
rebind, facade migration, recursive retirement policy or public Close activation.
It does not claim the bare library's live-binding/Media-first factory order is
the original engine's construction transaction (the original public create
constructs nonobserving DevTools before Media and attaches before facade).

## Proposed implementation, file by file

`arc/api/arc_webextras_media.mm`:

- The real `WebextrasMediaSource` now implements `arc::CoreMediaPeer`. Its
  once-only factory receives the real weak Root and constructs only functional
  caches/dispatch/registration state. Constructor reads the SAME Root's supplied
  close state once; it never subscribes, posts, calls the app, accesses a sibling
  or publishes the constructing Root. Source retains no strong Root.
- Explicit `AttachToWebContents` checks completed Root Media and DevTools slots,
  the same Media peer, the same live binding, and a first observer attachment.
  It installs a physical-native userdata **weak Source** route before subscribing
  the actual observer. It installs the existing overlay hook at this phase.
  Initial Mojo receipts remain asynchronous; caches are unset, with no preseed
  or flush. Root factory itself performs none of these actions.
- Info/Actions/Position/Volume producers lock the weak Root before cache mutation
  or copied payload preparation. Accepted delivery passes that acquired **Root**
  to the existing common queue helper. Counted work and uncounted no-registration
  inert work both retain it; Root owns the actual Media and sibling DevTools
  peers. Source/service-only retention cannot authorize a new receipt. No new
  owner/physical/current-registration cancellation or additional task is added.
  Selected-weak-bridge delivery, payload copies, late DTO allocation, same-state
  close accounting, synchronous PiP leases and no-registration count policy are
  inherited unchanged.
- Explicit `AssociateWebextrasMediaFacade(owner, root)` associates the SAME
  factory peer and a weak facade, without constructing another Source or
  retaining Root. Its static downcast has the documented integration precondition
  that this Root's Media slot came from `WebextrasMediaPeerFactory`; it is not a
  general foreign-peer conversion API. Both peer slots must be complete.
- `CreateWebextrasMediaService(owner)` / the existing private initializer adopt
  only that associated peer and CHECK its presence; lazy Source creation is
  removed. CORE must call the allocator only on its service-cache miss, retaining
  its existing single public service identity. The source-box association and
  service each retain only the peer, with no ObjC-facade-to-Root ownership added.
- Regular service live controls/getMediaContentTypes resolve Source's actual
  weak Root binding instead of a weak ObjC facade. Cached position stays on the
  independently retained peer and yields fresh DTOs after Root death. Existing
  error strings, live content-type backend, volume-unavailable behavior, position
  timing base, delegate semantics and public metadata remain unchanged.
- Overlay construction selects the actual peer using native weak userdata, then
  reads its weak existing facade. It never calls wrapperForWebContents or a public
  media getter to recover/resurrect one. Source retirement preserves reviewed
  PiP-before-cleanup normal-path ordering and removes this private weak route
  after the synchronous callback, before observer cleanup.

`arc/browser/webextras_media.h` declares the nonobserving factory, completed-Root
facade association and service allocator. This is a proposed private cross-package
API; the lead must approve/integrate it. It supersedes the old
`InitializeWebextrasMediaSource(owner,native)` eager hook. No replacement fake
implementation of that hook exists.

## Exact CORE integration request (not staged as a pretend constructor)

The lead must implement the reviewed initial transaction with the actual
DevTools peer, canonical close state and a real strong Root holder:

1. Establish the real canonical close-state handle and native weak binding.
2. Call `CreateCoreLogicalWebContents(binding, same_state,
   WebextrasMediaPeerFactory(), MakeWebextrasDevToolsPeerFactory())`, using the
   real DevTools package factory. Root completion installs both actual peers.
3. Explicitly attach each peer and associate the actual facade with this same
   Root. This Media API imposes **no arbitrary Attach-before-association order**:
   both require Root completion; Core chooses the source-proven transaction.
   Native weak routing and facade association must be ready before any
   app-observable setter/callout that needs them. Neither is lazy getter work.
4. On media-service cache miss only, adopt the associated peer with
   `CreateWebextrasMediaService(self)` and cache that exact ObjC service.

No actual Core callsite patch is supplied because the immutable bare library
provides neither the authoritative Root holder/facade transaction nor the real
DevTools implementation. The old eager-hook callsite cannot be composed unchanged
with the new header; this candidate is **not whole-Core/link ready**. The old
frozen eager-hook proposal remains untouched as historical evidence.

## Explicit PiP/lifecycle limits

The PiP public wrapper still resolves its source-element bounds/video-view
backend through the existing weak facade and associated view. Canonical ObjC PiP
cache identity remains service-owned. An overlay born before facade association
has no new migration/retarget policy; Core must establish the required routing
before callouts. No undocumented native identity or controller ownership is
invented. Those limits remain HOLD.

Root's immutable binding expires with the physical WebContents; retiring Source
observation alone does not clear Root's binding. This proposal therefore does
not establish post-retirement live-control behavior, old/new binding cutover,
qualified discard handling, recursive/reentrant attach, ownership endpoint or
Root WCO ordering. Prior normal-path retirement evidence is preserved; no
expected-old/generation check is claimed source-native. Root destruction only
releases its peers and does not synthesize retirement. New Info/Actions/Position
receipts explicitly reject expired Root even when Source/observer/service lives.

PDF placement prototype and its missing selected genuine UI-registration API
are unchanged and remain blocked. Existing thumbnail callback discrepancy and
all scenario acceptance results are unchanged; no boundary transcript was run.

## Validation and evidence

`webextras_media_logical_initial_held_test.py` compiles and executes actual
extracted proposed Source, delivery helper, association, service adoption,
nativeSession/cached-position getters, actual reviewed Root header/implementation,
and actual close-state primitive. Foundation task/bind, native WeakPtr/userdata
and observer transport are mock scaffolding. A **stateful test sibling** verifies
the real Root's strong DevTools slot; it is not production DevTools or proof of
its functional command behavior. No real constructor transaction is modeled.

The executable verifies nonobserving factory construction, complete-slot attach,
both association/attachment orders, exact peer adoption, SAME state accounting,
real native binding with nil facade, root-before-cache rejection after Root death,
accepted and uncounted inert Root+sibling lifetime, retained service/peer without
Root, and cached-position DTO availability after Root death. Two extraction-only
negative controls are meaningful: replacing accepted Root ownership with
Source-only ownership fails the Root+sibling test (exit73); removing receipt
Root guards fails the no-new-cache/task test (exit73). Normal source exits0.

Strict git-apply check/apply reconstructs all staged bytes; applying the bare
Root proposal and this Media increment in either order yields identical trees.
Both the complete
Media TU and unchanged Root TU compile with actual Chromium flags/plugins,
current close-state dependency and reviewed notification/outer-route generated
Mojo headers through private VFS; both exit0. No full link, Chromium runtime,
real DevTools integration or scenario acceptance is inferred.

Private stage `/tmp/webextras-media-logical-initial/{base,staged,reconstructed}`.
Durable freeze, source snapshots, commands, VFS, validation and test outputs are
under `logs/wp/webextras-media-logical-initial/`. The freeze records final hashes
and preserves all previous frozen artifact hashes. No contradiction to the
binding architecture is claimed; private shared APIs still require lead adoption.
