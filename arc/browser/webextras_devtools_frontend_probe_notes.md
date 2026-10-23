# Genuine DevTools frontend producer diagnostic (opt-in, unrun)

`--devtools-frontend-events` is mutually exclusive with both clock flags and with
itself. The exact11 current probe inputs after completed native9 were archived
before editing at logs/wp/webextras-native9-frontend-lane-inputs/manifest.json.
Default11 native operations/13 public fixture pairs are retained. Video/service
clock producer, validator and measurement bodies, and default Display/Closed
forwarders, are byte-compared against that archive in standalone tests.
No runtime, application launch, shared source or production edit is part of this
change or its standalone tests. Parent owns any later build/package/runtime slot.

The new lane runs between the already-authorized console-open acknowledgment and
the existing frontend getter/console-close. It does not add an open/getter/close,
execute, subscribe or other Arc public fixture call. Target census is taken before
that existing open and after. Only NEW internal devtools:/chrome-devtools: HTML
frontend targets qualify, at most8, with no page-type/first-target fallback.
Other owned-app target URLs/titles are not recorded. Genuine candidates are attached
with Target.attachToTarget(flatten:true); Runtime.evaluate uses the returned
sessionId with AgentCDP.request, because targetCommand intentionally rejects other
target types. The generic AgentCDP implementation is unchanged.

Each candidate imports its actual packaged core/sdk/sdk.js, ui/legacy/legacy.js
and core/common/common.js modules. It must expose a unique genuine SDK
TargetManager.primaryPageTarget whose inspectedURL EXACTLY matches the owned
nonce fixture URL. Its primary target ID is pinned after discovery. Endpoint PID,
original frontend target ID+URL, and refreshed SDK primary ID+URL are rechecked for
every stimulus; a read after each acknowledgment checks identity again. No fake
frontend, manually created controller, direct host method, outgoing Overlay CDP
packet, native hook call, OS input, activation or emulation substitutes for this.
Missing/unsupported/ambiguous frontend readiness is inconclusive, with raw errors.
A replacement/destruction of the frontend is inconclusive. Restore attempts use
ONLY the original target/session/SDK identity; they never select a substitute.

Inspect exercises the real global ActionRegistry action
`elements.toggle-element-search` twice, initially off then on/off. It requires an
enabled action and unsuspended targets; execute must acknowledge true and toggle
the actual action state. The expected on-mode follows the real show-ua-shadow-dom
setting (1/2), and off-mode is0. Original producer proof is native_notes.md342ff:
its global controller calls each scoped OverlayModel without awaiting, toggles the
action, then emits ONCE through inspectModeChanged. This diagnostic does not call
Overlay methods directly or treat a backend acknowledgment as the app callback.
No same-mode deduplication or suppression of genuine extra callbacks is added.

Docking is attempted ONLY when the genuine existing DockController.canDock is
true, with a valid current side. The fixed changed side is bottom if initially
right, otherwise right; setDockSide is called, followed by setDockSide(original).
Native display modes are undocked1/left2/right3/bottom4. Original producer proof is
native_notes.md383ff: requestDockPosition precedes CSS/events/setIsDocked. The
new diagnostic changes no host can_dock capability. False canDock is explicitly
unsupported/inconclusive after preserving inspect evidence. Restoring the current
side can change last-dock-state history in this disposable app profile; the
history is not silently rewritten. Docking may destroy/replace the frontend;
that is preserved as inconclusive and no identity bypass is used for restore.
The app's normal close and owned process cleanup remain the final recovery.

The M helper's opt-in callbacks-only journal is separate from gWork. Arm/read/
disarm are test-helper file requests with zero Arc public fixture calls. Immutable ownerTag, exact original URL/nonce
and spanID authorize journal-only read/disarm even when every weak owner/service/
entry and live match has disappeared. Actual Poll dispatches this narrowly before
its normal live-match gate. Wrong/missing/bool/fractional owner tags, wrong nonce
or span fail; no new arming or normal work bypasses liveness. Expired journal data
is retained until a matching disarm, which frees it without any engine call. Public
work cannot overlap an armed journal. The existing app delegate is never replaced;
only existing methods with independently checked void/object/NSUInteger ABI are
wrapped. Inspect/display/closed/focus capture selector, full encoding, canonical
service+owner association, thread, ordinal, forwarding depth, exact recorder
entry/after-forwarding offsets and native monotonic brackets. Original IMPs are
forwarded exactly once, including nested callbacks and thrown exceptions. Journal
state has weak owner/service links, retains only scalar observation data and is
bounded to128 events. Expired5s windows admit no new observations. Unknown/mismatched
services are counted, never attributed to the fixture. Disarm while a callback is
still forwarding fails rather than truncating its tail. A fresh opt-in install
requires existing inspect/focus ABI; it does not add methods when absent.

Callback polling is nominal100ms with a5s observation window and200ms quiet period. Quiet resets on ANY change
to the complete callback/forwarding/ignored/overflow journal, with stable sorted
keys; first matching callback alone does not start a fixed completion timer.
The final disarm journal must equal the last stable snapshot. An extra event
between that sample and disarm is preserved and invalidates the quiet claim.
Each transport command retains AgentCDP's existing10s timeout; synchronous PID
inspection is noninterruptible. This is a bounded observation/sample-acceptance
policy, not a universal hard wall-time cancellation promise. A lost response may
have applied a genuine mutation: restore is attempted only after a verified fresh
read of the original target's current state. Every frontend Runtime.evaluate saves
exact expression, target/session identity, raw result/exception or transport error
and host brackets BEFORE assertions. Discovery, arm/read/disarm samples, mutation
acknowledgments, after-ack reads, restoration and detach errors are durable. Detach
failure prevents success. There is no retry of a mutation to manufacture a receipt.

A separate typed raw validator handles callback-only spans. It derives expectations
from the genuine producer receipts and acknowledged UI transitions, not a supplied
expected-mode field alone. It requires nonempty actual app callbacks, matching raw
returns/null void values, engine caller labels, known selectors/classes, exact
NSUInteger/object ABI, service receiver anchored to the original console-open
recorded object, owner identity, payloads, main thread, ordinal/reentry depth and
both supported recorder-inner/outer forwarding bracket orders. Full raw nesting
must restore the existing ambient run-loop stack. Every callback span must contain
ZERO fixture public calls. Missing, no-op, wrong-mode, forged offsets/callers,
replaced owners/DTOs or incomplete forwarding cannot pass. Partial inspect spans
are validated/preserved even when docking is unsupported; the whole mode requires
all four typed spans. No callback is dropped or relabelled to obtain parity.

Callback-only spans NEVER go to the strict nonempty public fixture comparator.
The existing baseline13 fixture comparison remains unchanged. Whole-mode
acceptance additionally requires complete typed callback spans; it remains
pending fixture-comparison support while the active comparator has no verified
explicit fixture image mode. A zero-public span or default filtered zero diff
cannot turn into parity. Callback sequences and raw trace shapes are retained and
compared separately, including extra real events; no timing allowlist is added.
Typed payloads/shape are functional proof only. Nested selected public argument/
return values require the SAME frozen shared comparator, metadata and callback
inputs, with its default caller policy and explicit `--compare-boundary-arguments`
option (NO --fixture-image). The additional owned execution helper is copied once,
fingerprinted and rechecked for both launches; it verifies the existing frozen
four inputs before/after every invocation. Typed proof must first establish
nonempty actual callbacks, matched returns/identity/caller and ZERO fixture calls.
The helper independently rejects empty, incomplete, unlabeled and wrong-engine
callback inputs, then requires the comparator's callback counts to retain them.

Three independent canaries require exactly one nested public getter return
mismatch (nonnull to nil), one nested APP setter argument mismatch, and one nested
public call count mismatch, with no unrelated mismatch or callback-sequence change.
Each input has a genuine-shaped typed callback with matched null return. The
exact metadata acknowledgment must be boundary_arguments_comparison={mode:
selected-boundary-methods,caller_attribution:true}. The current legacy parser
ignores the new flag and ignores app setter arguments, so the honest canary result
is PENDING, even while its getter-return/count reports are meaningful. A future
fixture-only strict parser may reject the unsupported option; that also stays
pending. No argument normalizer, raw caller relabel or option-support guess exists.

Capability failure NEVER prevents actual frontend measurement. It records a
separate pending error and keeps complete/parity false. Actual typed spans still
run through the frozen comparator where possible, retaining exact default stdout
(first two lines unchanged) and the untouched JSON report, plus separate capability
and status metadata. A filtered zero report cannot become clean: all canaries and
explicit metadata must verify before a span comparison can be labeled compared.
Nested observable getter/setter values are never silently skipped. Baseline13
fixture capability and comparisons remain independent and unchanged.
Fatal/Check-failed scans and forced-cleanup rejection remain active in every mode.

The wrapper forwards the strict new flag under the same verified ancestor lock,
recording probeMode=devtools-frontend-events and mediaResourceUsed=false. Its
inventory is15 runtime source files: default14 plus the actual new
webextras_frontend_comparison.py execution helper. Default remains14 and both
clocks keep their16 inventory. This lane has no media asset. Tests and
notes are frozen separately in the review manifest. The exact versioned driver
extension is refreshed against the frozen adopted historical driver SHA1a31eb68;
the historical wiring patch is untouched, and partial/arbitrary drift remains
rejected before all10 wiring tests exercise the actual current default driver.

Standalone tests execute the actual producer with mocked module loading and the
actual driver orchestration with a fake protocol endpoint, including missing/
ambiguous target, absent response, no-op, wrong mode and destroyed frontend cases.
The actual callback validator tests zero-public positive spans, both recorder
bracket orders, nested focus, and receiver/ABI/caller/offset/thread/order tampering.
Foundation ObjC tests extract the actual helper and verify once-only original IMP
forwarding, nested depth, active-disarm rejection, weak owner lifetime and zero
public work. A separate acceptance test executes the actual pending comparator
and complete/parity gates. These mocks establish probe logic; they do not prove
Chromium target discovery, frontend modules, backend attachment, asynchronous
native integration, original-native callback multiplicity or real docking lifetime.


Review corrections were made only after preserving the first frozen13 inputs at
logs/wp/webextras-frontend-first-review-inputs/manifest.json. Actual orchestration
controls now inject later callback rounds at200/300ms and require quiet completion
at500ms or later, and inject another event during disarm to require inconclusive
status without dropping that event. The actual Poll-path test clears owner,
service, canonical entry and the weak map, expires the journal, rejects wrong
span/nonce/owner identities, then successfully reads/disarms with original
ownerTag and zero public calls. New arming after weak loss still fails.
