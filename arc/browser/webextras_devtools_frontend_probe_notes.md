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
fingerprinted and rechecked for both launches; it verifies the exact frozen
static local-import closure before/after every invocation (legacy four inputs,
supported staged comparator seven inputs). Typed proof must first establish
nonempty actual callbacks, matched returns/identity/caller and ZERO fixture calls.
The helper independently rejects empty, incomplete, unlabeled and wrong-engine
callback inputs, then requires the comparator's callback counts to retain them.

Four independent canaries require exactly one nested public getter return
mismatch (nonnull to nil), one nested APP setter argument mismatch, and one nested
public call count mismatch, and one literal-filename callback argument mismatch,
with no unrelated mismatch or callback-sequence change.
Each input has a genuine-shaped typed callback with matched null return. The
fourth uses the real five-argument download filename callback, literal NSString
`<ptr>`, timeout15 versus16, and a preceding raw ArcDownloadItem.id GUID result.
It requires the callback and id ABI in the same frozen metadata. This protects
the existing callback comparison when the argument option is enabled. The
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


Final quiet controls also retain a sample whose read starts before5s but returns
after the deadline: that late sample cannot establish quiet, even if its journal
is unchanged. Native expired=true likewise prevents quiet completion. The test
places a first matching receipt at4800ms and the next at5100ms, preserving both
while requiring inconclusive status. The final-disarm journal check covers an
event arriving after the last stable read. No timing tolerance was widened.
The independent callback helper follows proven recorder ordering: unique
nonnegative IDs, per-thread nesting and callseq<returnseq, preserving original
row order; atomic IDs may print out of global order across threads. It binds the
image argument to the same verified snapshot identity. Capability subprocess or
JSON failure is durably pending and never prevents actual frontend collection.


Quiet age and completion elapsed time use the captured sample receipt timestamp
consistently, including the previous-change timestamp. Time spent saving the
durable report cannot manufacture a stable200ms sample interval. A control reads
at4800/4900ms then advances the clock200ms during save and requires inconclusive
status, rather than accepting the resulting5100ms wall time as quiet evidence.
The prior14 frozen sources were archived before this final correction at
logs/wp/webextras-frontend-second-review-inputs/manifest.json.


Native10 journal correction (helper-only, after exact26-input archive at
logs/wp/webextras-native10-journal-inputs/manifest.json): the original recorder
logged canonical inspect callbacks while its invocation passed the saved helper
`_cmd` alias. Recorder source SHA256 is
9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e.
Describe70–81 strips its KVO naming marker for object IDs; AliasFor134–136
constructs exactly `__arcrec_` plus public selector; RecForwardInvocation161–208
logs the public selector, invokes the alias, then restores the public selector.

Each installed known wrapper now supplies its fixed canonical selector separately
from raw incoming `_cmd`. The sidecar preserves rawSelector. It accepts only that
public selector, or its exact recorder alias whose resolved runtime method IMP is
that same wrapper and whose encoding equals the public method encoding. No
arbitrary stripping or caller relabeling is performed. The original app IMP still
receives the exact incoming selector once. Canonical recorder engine-origin rows,
ABI, service identity, forwarding offsets and nesting remain independently checked.
The sidecar preserves raw owner isa, actual superclass chain and reported -class.
Only exact ArcWebContents, or exact NSKVONotifying_ArcWebContents with immediate
superclass pointer ArcWebContents and reported class ArcWebContents, is accepted.
This proves identity shape, not that an object currently has active KVO observers.
No prefix-only or isKindOfClass acceptance exists. Nil reported class is retained
as null and rejected safely without preventing original forwarding.

Standalone regression executes the pinned actual AliasFor and RecForwardInvocation
bodies around the actual helper wrapper, and real Foundation addObserver/removeObserver
on an ArcWebContents fixture. It verifies canonical raw recorder selector versus
preserved alias `_cmd`, forwarding once, actual KVO runtime identity, and rejects
wrong aliases, saved IMPs, type encodings, unrelated/indirect KVO-like subclasses,
reported-class spoofing and Nil-reporting subclasses. Typed callback tests retain
canonical raw selectors/callers and cover alias/owner evidence tampering. Observation
errors now distinguish failed requests, public work, owner identity, invalid selector,
ABI, no-callback timeout, missing declared mode and missing fixed quiet interval.

The fourth canary is independently exercised against TEMP copies of the sealed
real bad87be and correctedf2b3 comparator proposals: the first three controls pass
both, while the fourth rejects bad87be and accepts correctedf2b3. These proposals
import three helper modules; the test now uses the actual owned freezer static
local-import discovery to freeze exactly seven inputs automatically, then verifies
that exact closure after the canaries. There is no manual helper copying. Current
legacy comparator imports none of these modules and retains its four-input closure.
No shared comparator/source change or corrected-helper app run is part of this work.

Frontend mode also freezes the actual recorder policy source before operations,
using the same recorderFileIdentity checks as the binary. It requires exact9ae39687
before copying an exclusive readonly frontend-recorder-policy.m and saves source,
frozen identity and policy alongside the binary/run identities. It rechecks both
source and frozen copy before EACH launch and after measurement/cleanup. Changed
source bytes or identity fail explicitly; default/video/service modes never freeze
or check this source. The source pin does not establish source-to-dylib build
provenance. Actual runtime alias IMP/encoding validation supplies the independent
behavioral check. Standalone actual-function tests cover correct freezing, changed
source, mismatched policy source before any copy, and frozen-copy tampering.


Frozen freezer code is loaded from source bytes with compile+exec into ModuleType,
not an importlib cached loader. Python -B prevents cache writes but still reads
unchecked-hash bytecode. The actual-loader regression injects such a cache: the
source-only production path never executes its marker (and the stricter freezer
may reject the cache), while the old importlib control demonstrably does. The
separate freezer implementation also rejects unmanifested files in both frozen
import directories before comparator execution, including bytecode caches,
packages, extensions, stdlib shadows and symlink manifests. Closure security is
covered by its independent23-case suite and fresh review, not by a clean parity
claim from this diagnostic.


Native11 helper correction preserves the exact27 archived input union at
logs/wp/webextras-native11-boolean-inputs/manifest.json. C logical/comparison
expressions boxed with @() produce NSNumber integers, unlike C++ bool expressions.
The frontend helper now uses explicit @YES/@NO ternaries for selector and owner
validity, alias verification, main-thread, forwarding, overflow and expiry fields.
Strict TypeScript ===true/false checks remain. Counts, modes and tags stay numeric.
A forced `clang -x objective-c -std=gnu17` regression extracts the actual complete
helper source into a .m unit and uses real Foundation JSON encoding/decoding plus
CFBoolean type checks. Its __cplusplus error and C _Generic logical-expression
assertion prevent a C++ compilation from masking this distinction. Current source
passes public/alias/real-KVO/invalid-owner/false-forward/expired/overflow cases;
exact archived0ee5 source fails the boolean type assertion. Original IMP forwarding
and zero-public-work checks remain, rather than accepting numeric truth values.

Native11 also revealed a legitimate ambient CrashKeysObserverBridge callback
beside the one owned inspect callback. Callback metadata now comes from the SAME
frozen app_callbacks.inc. Native identities are admitted exactly; Swift dotted
identities are derived only by completely consuming length-prefixed components,
including nested classes. There is no prefix stripping or unrecognized fallback.
All raw engine→app callbacks must have exact known identity, engine origin, valid
thread, matched nesting/return and selector argument count. Ambient callbacks may
be off-main or nonvoid; their arguments, returns, counts and order remain intact.
Only the four known ADK.WebContentChromium selectors count as owned journal
coverage and retain baseline service/owner, main-thread, void, ABI/mode, forwarding
and ignored-service-zero checks. Foreign-service owned callbacks cannot satisfy
coverage. Every measured span needs at least one actual owned callback.

The typed result exposes total/owned/ambient counts and ambient identities, while
the raw span and comparator still contain ALL callbacks and nested public methods.
The comparator's callback_sequence counts TOTAL callbacks. Ambient-only windows
fail owned coverage and retain their counts. Separate evidence matching supports
an explicitly labelled OFFLINE DERIVED BOOL SIDECAR DIAGNOSTIC: unchanged Native11
raw records plus only the two known numeric validity flags converted in an in-memory
sidecar establish original total2/owned1/ambient1 and ours total7/owned0/ambient7.
The actual Native11 sidecar and raw records are never rewritten. Full producer/
quiet acceptance remains false for the incomplete original window; the evidence
helper is not used as an acceptance substitute.

New synthetic tests include nested and worker-thread ambient callbacks with
nonvoid returns, malformed/unknown/wrong-origin callbacks, missing metadata and
ambient-only failure. The actual frozen DEFAULT comparator additionally proves
ambient argument/return/count changes and nested app getter return changes remain
visible, with exact raw callback sequence counts. Four capability controls remain
required; current shared support remains pending and no normalization was added.
