**Parent status: APPLIED to owned sources after validation6; do not reapply this historical proposal. Official check-file passes. Runtime correction pending validation7.**

# Native popup delivery and extension-ready producer proposal

Unapplied proposal; this worker has not edited actual engine/helper/runtime
sources. The evidence and staged copies are under
`logs/wp/extensions-validation6-positive-audit/`. The context hunk is based on the
lead-applied disabled-info correction (`fa9499ad…`); it preserves that body. Runtime verification remains
unexecuted. The observed malformed-install timeout and uncertified ours cleanup
prevent any acceptance claim.

## Evidence and scope

The real validation6 CLI positives demonstrate native popup-open asynchronous
versus ours synchronous, while actual popup URL, sizes 320x160→400x220, close and
fixture effects agree. `REPORT.md` records exact operation boundaries, raw callback
sequences, pin snapshot counts/intermediate states, command argument shape,
lifecycle ordering residuals, extra initial singles, pending tooltip feature
configuration and the real inter-operation unassigned callback on both engines.

The bounded original-binary audit proves all three popup producers explicitly
use DelegateWrapper::InvokeAsync; renderer-origin callbacks alone did not supply
that proof. See `native-popup-proof/README.md`, bounded disassemblies and their
binary/slice SHA256 provenance.

| Event | Producer | Async enqueue | ObjC send |
|---|---|---|---|
| Open | `0x836c160`, renderer starts at `0x836c230` | `0x836c740` | `0x43e35a4` |
| Resize | `0x430415c` | `0x43043cc` | `0x43e3650` |
| Close | `0x4304768` | `0x4304d48` | `0x43e36e4` |

Open has no first-load readiness gate before enqueue. Native close enqueues before
view detach `0x4304b34` and host clear `0x4304b60`. No artificial delay, timer or
callback sorting is justified. The native open payload copies the extension ID
and retains browser/contents shared pairs at `0x836c610..0x836c64c`; its destructor
`0x42de9a0` releases them. The bridge weak-loads the ObjC delegate and browser at
`0x43e3554/0x43e3560`. No popup-generation/current-host check was found in the queued
open path.

## Proposed source change

`extensions_positive_delivery.patch` changes these two owned files. In
`arc/api/arc_extensions_browser.mm`:

- Snapshot the real popup ArcWebContents and extension ID after renderer start,
  then queue open through the existing `DispatchBrowserExtensionRequest` bridge.
- Snapshot ID and CGSize before queuing resize.
- Snapshot ID and enqueue close before detaching/resetting the actual host.
- A private helper retains the existing `SuppressDelegate`/destructor preflight:
  a suppressed action cannot enqueue a new notification. Already queued events
  use the browser bridge's weak lifetime/current target/callback epoch policy.
  Clearing its delegate invalidates queued delivery; replacing a nonnil delegate
  updates the existing target. No additional action-generation guard is added.
- Queued blocks contain only event-time values, a copied block, and a weak local
  ArcBrowser. No C++ `this`, raw model/host pointer, member reference or loop
  reference escapes. `OnLoaded`, real host loading, popup completion handling,
  dimensions and WebContents ownership are unchanged.

One native lifetime detail remains uncovered: native retains a shared contents
peer; our retained ArcWebContents wrapper uses the core's existing WeakPtr-backed
native contents. Keeping that ObjC wrapper alive does not keep a closed
content::WebContents alive. The probe does not cover close-before-queued-open;
this proposal neither claims identical ownership there nor adds a generation
filter that would contradict the native retained event payload. Any stronger
contents ownership adaptation requires core evidence and coordination.

In `arc/api/arc_extensions_context.mm`, the single enable-notification producer
moves from `OnExtensionLoaded` to `OnExtensionReady`; its payload, posting and
weak-lifetime logic remain unchanged. Native Impl constructor `0x42e5b60` installs
primary vtable `0xda7cb48`: Loaded slot +0x10 is no-op `0x15ac0`; Ready slot +0x18
is `0x42e9448`, which dispatches member offset +0x10 with the exact
`arc::BrowserContext::Extensions::Delegate::DidEnableExtension` label at
`0xd37ee6e`. Unloaded +0x20 is `0x42e9650`, with the exact DidDisable label at
`0xd37eead`. Current Chromium's `ExtensionRegistryObserver` ABI agrees with these
slots; Ready follows initialization of browser state supporting child-process
startup. Native bounded proof is in `native-lifecycle-proof/`.

The registration audit also finds native SetProfile `0x42e8260` registers its
registry observer at `0x42e8384`, before ToolbarActionsModel factory/Get at
`0x42e83b4..0x42e83dc`. In the current engine the toolbar model may already be
initialized before this facade's delegate observer is created. Moving a local
initializer alone would not establish insertion-order parity. **No observer
registration change or artificial callback reordering is proposed.** The
recorded disable/full/pin-order interleaving remains unresolved. The Ready
correction follows the native event producer; it does not claim to repair the
entire observed order. Likewise,
extra initial singles lack timestamp/producer evidence sufficient for a throttle
change. Tooltip text remains exact observed data pending app feature-state work.

## Apply and checks

Current source base SHA256:
`6a55ee3efe5010cf616e42a82d35aee7dfc455657fe6587eac3a88759c19b727`.
Proposed source SHA256:
`b1ed32a375b12d6afd346a46b5697d1237c9267147acccdf9e908ea2a771ef42`.
Current context base SHA256 (includes lead's bounded Info fix):
`fa9499ad71a54132e3f6c1c80ffb089028f89cbfe81f15e1d886c2134b7e1b38`.
Proposed context SHA256:
`475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d`.
Final V3 patch SHA256:
`b043521eb8239600c8b8984542197a6e1bcfe95963b61383b6a4dd186a01a740`.

Apply only final V3, not the preserved open-only v1 or popup-only v2 scratch
patches as well. Strict temporary `git apply --check --whitespace=error-all` and
actual temporary application pass, producing identical proposed bytes. The real
Base commands for browser and context TUs were queried without a build/lock;
object/dependency/diagnostic output flags were removed and VFS syntax compilation
returned 0 with empty diagnostics. Both repository and Chromium source aliases
map to the staged file. A deliberately invalid staged-source sentinel was rejected
by the same compiler, proving the overlay was read. `OnLoaded` is byte-identical.
No trivial behavior-mirroring test was added.

Evidence: `delivery-proposal-v3/{metadata.json,context.command.json,context.result.json,negative-context.result.json}`;
browser source bytes remain identical to the positively/negatively checked v2,
whose evidence remains in
`delivery-proposal-v2/{browser.command.json,browser.result.json,negative-browser.result.json}`.
Earlier v1/v2 apply/compiler evidence remains separately preserved.
Fresh code review found no defects; final review evidence is retained with the
analysis logs. No app launch or runtime test has been performed for the proposal.

## Lead follow-up

Review/apply after the frozen batch, then exercise real popup timing and an
explicit early-close case. Use the retained native registration proof to coordinate an earlier real profile
service initialization/lifetime hook with core if further ordering correction is
required: native Impl attachment is independent of the ObjC delegate bridge,
whereas the current observer is created in setDelegate and reset on nil. The
native trace proves registration-before-toolbar-lookup, not runtime observer
insertion order. Do not remove/re-add Chrome observers or manufacture callback
order. Establish the real initialization dependencies first.
No new architecture contradiction was established. Existing comments that imply
uniform native async popup delivery become accurate only after the proposal is
applied and independently validated.
