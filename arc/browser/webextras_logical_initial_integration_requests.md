# Webextras initial logical integration seams

Proposed interfaces only. No Core constructor, publisher, native ownership or
production activation is implemented here. Native construction must be settled
before these consumers activate; exact peer proposals remain held below.

## Factories and service adoption

Media candidate16776036 (review ae363ab2):
`arc/browser/webextras_media.h` exposes
`arc::WebextrasMediaPeerFactory() -> CoreMediaPeerFactory`,
`arc::AssociateWebextrasMediaFacade(ArcWebContents*, const CoreLogicalWebContentsHandle&)`,
and `arc::CreateWebextrasMediaService(ArcWebContents*)`.
The first is nonobserving and stores weak Root plus the same supplied state.
The association adopts the SAME actual Source from the completed Root and weakly
records its existing facade. It retains the peer, never Root, and creates no
Source. Service allocation is valid only after association and only at the Core
cache miss. Existing lazy public getter must never become an alternate factory.
The old InitializeWebextrasMediaSource eager hook is superseded for this route.

DevTools candidate8dccf5c9 remains HOLD for an exercised property-metadata
regression; the separate native-source-backed genuine facade-cache correction
is being prepared. Factory/adoption signatures themselves are unchanged:
`arc/browser/webextras_devtools_logical_peer.h` exposes
`arc::MakeWebextrasDevToolsPeerFactory() -> CoreDevToolsPeerFactory`, and
`WebextrasCreateDevToolsService(const arc::WebextrasDevToolsPeerHandle&)`.
Core must pass the SAME actual peer installed by that factory, obtained from its
stored logical Root on the service-cache miss, and cache the returned service.
A static downcast is valid only under that exact factory precondition. No peer
fallback allocation or native/public owner lookup is provided by this seam.

Both real peers implement the typed Attach/Retire interface from the bare Core
library. Both Attach methods require complete Media and DevTools slots, exact
peer identity and the Root's matching initial live binding. Factories do not
subscribe, call the app, post, access siblings, publish routes, or create services.
Media association may occur before or after Media Attach once Root is complete;
there is no package-imposed facade-first construction order.

## Construction and lifetime requirements

Original public construction is nonobserving DevTools then Media with managed
binding empty, then weak logical route/DevTools attach/Media attach/root observer,
then ObjC facade allocation and initial setters (Core141606). Bare libraryd4b
requires an initial live immutable binding and invokes Media factory first.
Those foundation limitations must be corrected by Core in a separate reviewed
transaction, including C156 helper recursion and the actual persistent holder.
These seams are not permission to adopt the bare library's order as native parity.
The public kinds0/6 both have peers; other internal kinds are a separate Core gate.

Core native-route proposalc5dc9cf6 supplies an existing-only weak real Root lookup
and initial-only publisher, with exact binding/state/control identity checks.
It is compatible with the weak peer routes but is not a persistent owner or a
facade association. Core must publish only after both peer factories return and
retain a real strong Root holder independently. No empty lookup may authorize
skipping mandatory receipts/accounting or creating a second logical object.

An initialized-facade-to-stored-Root accessor is still needed for service adoption
after physical native destruction. Native-only lookup cannot implement that case.
Media currently requires explicit facade association even if the native route
exists. No public owner getter, wrapperForWebContents resurrection or service
getter may substitute for that private stored identity.

No qualified discard/rebind, standalone ownership/final disposal, Root WCO order,
recursive retirement or native pending-transport cleanup is supplied. Media's
synchronous PiP still needs its facade/view backend and separate migration policy.
DevTools explicit detach, physical host-close and final destruction are distinct
native operations; current Retire is not a full cleanup implementation. Those
lifecycle and pending-error gates remain HOLD, independently of initial receipts.
PDF still needs genuine selected UI-registration dispatch and logical payload
retention; a CloseState-only queue is insufficient. Shared declarations/build
integration and all Core edits belong to the lead/Core owner.

## Validation boundaries

Media actual Source/Root/primitive tests, real-flags Media+Root syntax, reconstruction
and fresh review are clean in their initial scope. DevTools18 actual extracted
Root/peer groups and real-flags syntax pass, but8dcc alone fails the already-tested
V_toolboxWebContents property contract. Both candidates are unapplied. Neither
compile/extraction proof supplies full engine link, genuine transport, construction,
UI recursion or required-scenario diff acceptance. Current results remain in
logs/wp/webextras.acceptance-current.md and the append-only work report.
