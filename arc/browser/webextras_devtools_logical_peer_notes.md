# HELD initial functional DevTools logical peer

2026-10-23. This proposal is **unapplied, initial-only, and not runtime acceptance**.
Only this owned proposal, its owned extraction test/notes, and owned evidence
were published. Actual production/shared files, Chromium, maintained patches,
build graph, apps/profiles, credentials, and board state were not changed.

## Exact prerequisites and delta

The immutable bare Core logical library is `d4b42613459f6d3f3fcaea59170bcfdb9fc9ac633a9a39b4521ef2ac727f4b58`.
It installs neither a facade nor a tab/native root holder. This consumer does
not add either one. It does not turn that library into a native construction,
retirement, standalone ownership, discard, or replacement transaction.

Privately compose the existing shared prerequisites, in order:

1. `webextras_devtools_hooks.patch` (`dcbf472a...`),
2. `webextras_devtools_host_docking.patch` (`d9295b5c...`),
3. `webextras_devtools_embedder_events.patch` (`2fae8168...`).

Owned source starts at production `arc/api/arc_webextras_devtools.mm` SHA256
`cef1ff1c919943659096e7218cd4b056ad3f986bcf39f364e906d7f1c53342c0`, then:

1. `webextras_devtools_integration_held.patch` (`a2a9d232...`),
2. `webextras_devtools_embedder_companion_held.patch` (`ed9a828d...`),
3. **this** `webextras_devtools_logical_peer_held.patch`.

The final complete hashes are in `logs/wp/webextras-devtools-logical-peer/freeze.json`.
The old outgoing-protocol hook stays superseded. No frozen prior proposal or
audit artifact was edited. The existing app-owned feature helper declaration
was staged privately for compilation; no replacement helper implementation or
shared declaration was invented.

## Implementation

The delta modifies the held version of `arc_webextras_devtools.mm` and adds an
owned private header `arc/browser/webextras_devtools_logical_peer.h`.

`arc::WebextrasDevToolsPeer final : CoreDevToolsPeer` owns the actual functional
PImpl: raw connection map/IDs, command transport, pending command completions,
subscription dictionaries, weak delegate-registration alias, frontend weak
binding, frontend observer, opening/shown/panel state, pending panel queries and
existing toolbox cache. The PImpl contains those operations and data; it is not
a separate cancellation token with work left in an ObjC service. Its only root
back-reference is `CoreLogicalWebContentsWeakHandle`.

The typed peer's nonobserving constructor allocates local empty containers and
weak references only. `MakeWebextrasDevToolsPeerFactory()` supplies the bare
Root's typed factory. It neither observes native contents, posts tasks, looks
up a sibling, publishes userdata, creates a service, calls the app, nor installs
hooks during construction.

Separate `AttachToWebContents` requires a live initial physical weak binding,
the SAME completed Root's DevTools slot, and a nonnull installed Media slot.
Only then does it install observation, weak actual-peer native userdata and the
existing typed Show/creation/dock/inspect/focus hooks. No native userdata owns a
peer or service. Hook and frontend-observer dispatch routes to the actual peer
even before an ObjC service exists. Initial immutable Root binding is checked;
a different native replacement cannot be silently attached through this API.

`ArcWebContentsDevTools` becomes a facade retaining the SAME typed peer and its
current strong delegate bridge. Public methods forward to actual peer state;
public selectors/block signatures and the readonly `toolboxWebContents`
property declaration are retained. `WebextrasCreateDevToolsService(peer)` is an
explicit owned entry point. It creates no duplicate peer, uses no owner getter,
performs no native/root search, and never calls `wrapperForWebContents` to
resurrect an inspected facade. The existing public frontend getter still uses
canonical `ArcWebContents` wrapping as before; the known native-fresh identity
conflict is not silently resolved by this extraction.

Keeping the strong current bridge in the facade is intentional: native service
owns that bridge while its native Impl owns only a weak registration alias.
Moving the strong bridge to the root-owned peer would let registration outlive
service teardown without a captured accepted task. Non-nil replacement updates
the same current bridge; nil drops only that strong bridge, while a captured
old bridge can remain eligible through the weak alias. The bridge continues to
hold weak delegate and weak ObjC service references.

Accepted display/inspect/focus tasks capture the selected **strong bridge** and
a **weak real functional peer**. They acquire no strong peer/root lock, do not
check current service, physical contents, frontend, navigation or an epoch, and
never access the peer after app callback entry. They load the native-faithful
strong local delegate/service arguments from the bridge. Consequently an
existing service argument can indirectly keep its peer alive through that
callback, exactly because the native bridge strongly loads that weak argument;
this is separate from, and does not justify, an extra task-held strong peer.
With nil service the task still calls a surviving delegate. Final peer release
invalidates the real guard before functional transport/payload destruction.

The command transport callback also routes weakly to the actual functional
peer rather than to an ObjC service. This preserves the native Impl's ability
to process results with its logical owner alive after a facade is released.
The callback never takes a shared peer/root lock and performs no work after
its direct command completion. Actual ProtocolClient and notification/parser
helpers are unchanged; raw limits, error strings, IDs, parser depths, command
result/error dictionaries, direct completion versus queued events, literal
subscription matching and autoEnable/last-event-disable behavior are retained.

Physical `RetireFromWebContents` removes only the peer's own weak inspected
association, detaches observation and resets its weak physical binding. It does
not invalidate the real peer guard or clear delegate registration. Accepted
notifications thus remain deliverable with a service or Root owner after
physical detach/destruction. No protocol reconnection, CDP host retarget,
frontend replay or synthetic close is invented. This is the bounded initial
consumer's detach operation, **not a claim of complete native destruction
cleanup**: original4358e14 has per-target connection/pending-container retirement
and error behavior which still needs independent full semantic integration.
That gate prevents lifecycle activation of this held proposal.

## Validation

The exact composed `.mm` passes the existing Chromium macOS arm64 compiler
command with the real defines/includes/SDK/ARC/C++23/Chromium plugins/warnings,
using private staged headers and `-fsyntax-only`: exit0, zero diagnostics. No
GN regeneration, object build, link, app or runtime scenario was run. The new
private header is included by that checked translation unit. Shared prerequisite
native code was not recompiled in this extraction; its earlier independent
compile proof remains in its own frozen evidence.

Strict `git apply --check --whitespace=error`, apply and exact byte reconstruction
pass on the precomposed owned base. The bare Root source/header and supplied
close-state header are snapshotted. A structural equivalence check pins the
unchanged actual ProtocolClient, notification helpers, JSON router and public
selector/property surface in the freeze evidence.

Run the bounded functional regression on a private copy of the archived stage:

```
python3 arc/browser/webextras_devtools_logical_peer_test.py --stage <private-stage>
```

It executes actual bare Root source and actual extracted peer constructor,
Attach/Retire, native routing/hooks, weak guard queue helpers, delegate facade,
frontend receipt/reset/close and command/subscription method bodies. Eighteen
groups pass: nonobserving factory + complete slots; route before service;
service-only peer ownership after Root release; nil-service delivery; final
peer expiry; physical destruction survival; mutable/nil-new bridge selection;
nil-period weak registration; queued functional Media receipt retaining the
actual Root and DevTools sibling; reentrant final release without direct strong
peer lock; frontend replacement; exact mode maps and no Show duplication; raw
feature/ID/errors; dictionary execute/error/direct completion; subscription
queue/ID/order/disable; command routing after facade release and final expiry; actual close reset/native
fallback/reentry; natural frontend destruction before Show and deduplication; command completion
reentrant final peer release without a direct Root/peer lock.
The combined test groups merge some of these checks.

The Media peer in this unit fixture is an explicitly **mock functional** peer
whose actual receipt copies/retains the actual Root. It is not a dummy supplied
to production and proves only the ownership graph. Native WebContents/UI,
Chromium WeakPtr/task runner, JSON implementation and CDP transport are bounded
mocks. Foundation ARC and the actual Root/peer standard shared ownership run
normally. This does not validate IPC, native browser cleanup, package linking,
frontend resources, permissions or runtime/golden parity.

## Exact Core integration requests; all remain HOLD

1. After an approved real root/native ownership transaction exists, install
   `MakeWebextrasDevToolsPeerFactory()` and the actual Media factory in that
   SAME root. Preserve construction limitations documented by the bare library:
   it currently requires an initial live native binding and calls Media factory
   first; original normal public construction is nonobserving DevTools then
   Media before managed binding. This extraction does not correct or activate
   that mismatch.
2. After BOTH slots exist, call the actual peer Attach and Media's explicit
   attach/facade association in a reviewed core transaction before app callbacks.
   No synthetic root holder, fake sibling or lazy duplicate is acceptable.
3. At `arc/api/arc_web_contents.mm` current `serviceNamed:`/`devTools` around
   lines759–775, replace the DevTools dynamic `initWithArcWebContents:` route
   ONLY when that same approved root is available. On a cache miss pass
   `std::static_pointer_cast<arc::WebextrasDevToolsPeer>(root->GetDevToolsPeer())`
   to `WebextrasCreateDevToolsService`; this cast is valid only because that
   reviewed construction installed THIS exact factory in that slot. Retain the
   resulting facade in the existing canonical service cache. No fallback peer
   allocation or public owner lookup is supplied. Add the owned header through
   the lead's shared declaration/build integration if needed.
4. Root facade/native ownership, typed close accounting, native destruction
   pending/error cleanup and safe final root release still need reviewed Core
   integration. This initial candidate supplies none of them. DevTools peer
   independently retained by a service must not retain Root or be retired merely
   because the ObjC inspected facade disappears.
5. Do not invoke an arbitrary replacement attach or treat Retire as full native
   protocol/frontend retirement. The original recovered replacement keeps peer
   identity/guard, but full native eligibility, pending transports, frontend
   ownership and error ordering are not proven by this candidate.
6. Obtain a fresh independent review before adoption, then full build/link and
   paired native/runtime comparisons. Existing canonical wrapper identity,
   hosted undocked CloseWindow, closing-window F2 reuse, unbundled frontend
   changes and original constructor-reentry limits remain unchanged/open.

## Acceptance and contradictions

No new scenario diff exists for this held delta because no runtime activation
occurred. Parent's existing devtools/media/pdf/launch-slice results remain the
current canonical evidence; no zero-diff or functional-acceptance claim is made.
The lifetime audit proves the previous physical-WebContents queued guard is too
early. This proposal replaces that proxy only in a reviewable held consumer.
Architecture's canonical wrapper rule still conflicts with original DevTools
fresh wrapper getter observations; no exception was invented here. Specs'
`webContents wraps native target` wording remains inconsistent with native
frontend observations already documented by the parent. No additional contract
exception or source normalization is proposed.
