# Held PDF UI placement prototype — not activation-ready

The native UI-registration eligibility gate is NOT implemented. This patch is
only a placement/finalization prototype, not full native accounting parity.

`webextras_pdf_close_accounting_held.patch` is a separate unapplied incremental
change to current owned `arc_webextras_capture.mm`, alongside the held Media
companion. It requires lead approval of current bec65249 canonical-state API and
full core close composition. It makes no actual production/shared/Chromium edit,
public Close activation, build or runtime claim.

The existing observer captures the exact `CoreCloseStateForWebContents(owner)`
handle in its constructor, before its first scheduled WatchPDF. After the
existing PDFReady receipt guards accept a notification, its existing UI post is
wrapped with `MakeCoreCloseAsyncInvocation(kUI)`. The existing weak-observer
conditional is inside the outer counted invocation. The DeliverPDF body is
byte-identical: owner/delegate local acquisition, controller allocation, property
setter/KVO and delegate callback remain one delivery. Pending ends after that
whole conditional returns, including observer expiration or missing owner.

Native producer refinement: frozen pdf-producer.asm SHA256
d61408236edba6e3e10f8e470a82eddadd8bc93078568f481d6d6583b25d63e3 shows a
logical WC weak lock at439c0dc..114, then UI-registration weak-control+350 and
pointer+348 lock/validation at439c134..16c. Nil/expired branches bypass pending;
only the live selected registration reaches439c170..178. The prototype instead
wraps every currently accepted PDFReady. It can overcount and publish a setter
with no native-equivalent UI registration. This is a known gap, not a transient
test failure or a claim that placement alone repairs native Close eligibility.

Do not substitute a live weak ObjC delegate, respondsToSelector, owner, observer,
or a fake registration token for that native C++ registration. A live native UI
bridge with an expired app delegate can still count and publish its controller
setter when its weak owner is live. Both setter and callback must be conditional
on delivery through the original selected registration, not a delivery-time
lookup of the current app delegate. Existing DeliverPDF registration identity
and setter publication remain unproved even without this accounting prototype.

Exact request to lead: expose the genuine logical-peer/UI-bridge receipt API,
which (a) uses the already-captured canonical state, (b) locks logical peer and
current real UI registration before acquisition, (c) acquires UI pending only
for that live selection, (d) queues the original selected weak registration and
copied controller payload with the actual logical-peer lifetime, and (e) runs
setter/KVO and delegate work inside that conditional invocation before completing
pending. The current unconditional PostCoreUIDelegateInvocation/MakeAsync helper
alone supplies no such eligibility or selection. A separate full integration
must use that API; this prototype does not invent ownership or a bridge.

No WCObserver batch/trailer or synchronous delivery scope is added. No new
generation/native/owner filter, registration retargeting, retry, destructor
rollback or receipt replay is introduced. An unrun/dropped task leaves its
captured state pending as the actual core primitive specifies.

Original PDF audit `webextras_pdf_close_accounting_notes.md` bounds UI pending
439c170..178 before post439c308; bridge4447ff0 performs setPdfController4448058
and didFinishPDFRendering4448070 in that same invocation. Setter4444728 is the
native strong store. This establishes UI pending, not a separate WC notification
or property trailer. Existing registration/emitter parity gaps remain unchanged.

The placement test compiles/runs the actual core delegate primitive and actual proposed
PDFReady/DeliverPDF bodies with mock Foundation owner/delegate/weak binding/task
and observer-construction scaffolding. It checks one accepted post despite a
duplicate receipt, stale receipt rejection without counting, captured state
identity, pending throughout setter and delegate return, observer destruction
during setter with already-retained delegate delivery, expired observer and nil
owner conditional paths, and no destructor decrement for an unrun task. It
probes the actual UI synchronous flag to ensure this is pending-only, with no
extra sync scope. Constructor capture and the whole source are independently
checked with actual out/Base flags. No real PDF helper emitter or Chromium task
runtime is exercised by this standalone test. Native no-registration eligibility,
selected UI-bridge replacement/nil/re-add and logical-peer lifetime are NOT
modeled by it, so its PASS is not native accounting eligibility evidence.

Hashes, source snapshots, commands, strict reconstruction and executable results
are under `logs/wp/webextras-media-accounting-held/`. Media and PDF proposals
strictly reconstruct together in either order. The existing accepted PDF golden
has zero PDF-ready callbacks; these checks do not replace a future genuine ready
runtime or establish native registration identity. Lead approval, core/lifetime
gates and paired runtime remain required before activation.
