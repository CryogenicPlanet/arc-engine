# HELD typed DevTools cleanup triggers

This DISTINCT unapplied incremental proposal starts AFTER the frozen initial
functional peer `8dccf5c9...` and required genuine facade-cache correction
`f702890b...`. Earlier proposals, notes, tests and freeze artifacts remain
unchanged. Only owned proposals, tests, notes and evidence are published; actual
production/shared files, Chromium, build graph, apps/profiles and board were not
changed. This is not runtime acceptance or Core lifecycle activation.

Read first the complete native retirement audit:
`arc/browser/webextras_devtools_retirement_audit.md`, SHA256
`df79de95b0651d2d85a6c0f12cf88eef641502e4dbf118b892a716150a4cef91`;
manifest `logs/wp/webextras-devtools-retirement-audit/manifest.json`, SHA256
`bc5891eb5620ae7eac4c9164b45d4e82e06dc43fb9fcc540a4bb5d5b5f702b29`.
The separate `webextras_devtools_raw_final_qualification.md` corrects the old
blanket silent-final wording. **Final raw Close notifications are eligible;
only final OUTER pending-command/subscription containers are silent.**

## Source files / actual functional state

The patch changes the held `.mm` and owned private peer header. All functionality
continues to reside in the real `WebextrasDevToolsPeer`/PImpl and actual
`ProtocolClient`; no detached alive token, fake lifetime owner, service-only
guard, dummy sibling, root construction or facade resurrection was added.

### Explicit detach: native4358934

`Retire` moves the currently owned raw connection batch into an independent
local map and explicitly calls `Close` on each before removing its own native
association, Observe(nullptr), and physical binding reset. Raw connection ID
counter stays unchanged. It does not drain outer commands/subscriptions, reset
frontend/toolbox state, emit DidClose, send disable, reconnect, retarget, or
invalidate either real peer guard. Closing raw clients explicitly prevents an
unrelated temporary shared client handle from postponing accepted Close output.

The local raw batch and post-detach weak-peer survival check are C156 safety
adaptations, not recovered native instruction order; see the explicit HOLD
boundary below. No strong peer/root lease spans backend detach or app calls.

### Genuine observed target destruction: native4358e14

`Impl::WebContentsDestroyed` is separate from explicit Retire. It first closes
the independent raw batch, then (only if the actual peer still survives that
C156 boundary) clears the physical binding. It moves BOTH live pending-command
and subscription maps into independent strong locals and leaves empty live
maps BEFORE the first terminal app callback.

The standalone `DeliverObservedHostClosed` uses only those local containers:

1. Pending command callbacks receive `(nil,NSError)` directly.
2. Each subscription receives `(originalID,nil,NSError)` directly, after all
   pending commands; records for the same event retain insertion order.
3. Each error has domain `company.thebrowser.Arc`, code10 and exact description
   `DevTools agent host closed`.

No async success-event helper is used for these terminal errors. There is no
whole-peer/root lock, no weak cancellation check after error delivery starts,
and no peer/state/this access after the first app callback can release the final
peer. All old errors continue from owned locals even after that release. Both
old maps are already absent from live lookup/unsubscribe, and reentrant fresh
records remain in the new live maps after the old loops finish. There is no
synthetic `.disable`, reconnect, frontend close or late live-map clear.

This observed phase contains no Observe(nullptr); the genuine Chromium base
reset follows observer notification. Core must not synthesize this callback
when earlier explicit owner cleanup has already detached the observer.

### Representation and order limit

Subscriptions are represented as Foundation unordered event-key buckets, each
holding an insertion-ordered array of ID/domain/callback records. There is no
extra callback-owning ID index. Subscribe appends; unsubscribe scans buckets for
the ID, removes the exact record, and retains existing last-literal-event disable
behavior. Normal successful events still queue copied callbacks in that event's
insertion order, equivalent to prior monotonically increasing IDs for a single
event. Already accepted queued successes remain independent of later terminal
errors, unsubscribe, facade release and final peer destruction.

Terminal pending commands and EVENT BUCKETS are not numerically/globally sorted.
Original unordered-map bucket hashing/order is not reproduced by Foundation
hashing; exact cross-command and cross-event-bucket terminal order remains
**HOLD / unproved**. Within a single event the vector order is source-proven and
preserved. No comparator normalization, padding or guessed total order is added.

### Final peer destruction: native4356b28

Two WeakPtrFactories guard the SAME real functional peer:

- Command-router guard is invalidated after final Observe-detach and before raw
  clients/outer callback captures are destroyed.
- Delegate-notification guard remains eligible during raw and outer capture
  release, then invalidates before remaining native/frontend state release.

The constructor supplies the command client its real command-router weak peer;
Show/dock/inspect/focus/native hooks retain their distinct real delegate weak
peer. Physical detach invalidates neither. This models the recovered separate
native flag phases without a new cancellation-only object.

Final cleanup explicitly closes raw clients, preserving accepted copied raw
Close error notifications, then releases subscription and pending-command
containers WITHOUT manufacturing their terminal host-closed error callbacks.
It does not call the observed-target error helper. Internal command transport
errors remain ignored by its router as before; no reconnect is introduced.
This does not establish all C156 internal transport destructor-side-effect
ordering or arbitrary final-destructor app reentry as native parity.

### Actual raw ProtocolClient

The oversized-message Close task now holds only that actual client's WeakPtr.
It neither strongly retains the client nor postpones destructor-driven Close.
Client destruction calls Close first, then invalidates its own weak factory,
matching the native Close-before-client-guard-poison sequence.

Raw Close moves the host and copied callable into locals before calling C156
DetachClient. It sets attached=false and clears the stored callback first.
After detach it uses ONLY locals to post exactly one generic
`DevTools connection closed` error. It never accesses this afterward, so native
observer reentry can remove the last client owner safely. AgentHostClosed also
uses local host/callable state. Internal direct command completion copies its
callable and tail-calls it without a hidden shared_from_this client lease or
later member access. Copied raw success/error tasks carry no new peer/client/
service guard and remain deliverable after raw client destruction.

## Critical C156-only safety boundary; activation remains HOLD

C156 `DevToolsAgentHostImpl::DetachClient`344–352 retains its host and calls
DetachInternal. DetachInternal disposes handlers, detaches the session, invokes
Chrome manager ClientDetached, and can call external DevToolsAgentHostDetached
observers through NotifyDetached. Explicit DetachClient itself does NOT call
client.AgentHostClosed; ForceDetachAllSessionsImpl481–488 calls that separately.
These paths can reenter native code, and that earlier boundary is distinct from
the recovered later outer-error loop which never dereferences Impl after app
callout begins.

For memory safety this proposal isolates the OLD raw batch before native calls
and weakly checks actual peer survival afterwards. This changes the observable
live raw-map phase compared with the recovered native in-place raw-map clear;
reentrant new raw records survive the isolated batch. It also copies/clears the
raw callable before detach, while native may reach its copy after backend detach.
Nested reentrant close or event-loop draining could observe that notice phase.
Those details are explicitly **HOLD**, not a claim of native reentrant raw-map,
callback timing, new-target validation or binding replacement equivalence.
The original has no recovered post-call peer guard at this point. C156
ForceDetachAllSessions' own raw-client assumptions are not repaired here.

No generic retirement generation, eager frontend close, pseudo alive gate or
strong peer/root keepalive is used to hide this boundary. Full Core registration,
owner cleanup ordering, initial construction corrections, native exclusive
ownership, replacement/rebind and lifecycle activation remain separate reviewed
work. Do not call both explicit and genuine observed triggers unconditionally.

## Verification and review

Exact private source passes the real Chromium/macOS arm64 SDK/ARC/C++23/plugins/
Werror syntax command with zero diagnostics. Strict incremental reconstruction
and both prior Core library composition orders produce exact staged bytes.
There was no GN regeneration, object build/link, app/native runtime or new
scenario/golden diff. Production DevTools remains `cef1ff1c...`.

Run the owned test driver on a PRIVATE stage copy:

```
python3 arc/browser/webextras_devtools_trigger_cleanup_test.py \
  --stage <composed-private-stage> --baseline-stage <original8dcc-private-stage> \
  --negative-controls
```

- **25 actual Root/peer groups** pass, including the previous18 and new distinct
  trigger tests: raw-only detach/counters/maps; both maps empty before direct
  errors; exact payloads/command-before-subscription/per-event order; retired
  unsubscribe/no disable; fresh reentrant records survive; first error finally
  releases the peer while old locals finish; accepted old successes survive;
  backend last-release safety; explicitly qualified reentrant raw batch safety;
  router-before/raw/subscriptions/pending/delegate guard phase pumping.
- **8 actual ProtocolClient groups** pass against a mock native host/queue:
  envelope guards, async copied raw close, weak oversized-task lifetime,
  no double close, old success plus final raw error eligibility, direct command
  completion final client release, backend detach final release, distinct host
  close, and nested oversized task during destructor Close.
- Actual extracted facade runtime metadata/cache tests still pass: full native
  property attributes, ivar type/offset8, genuine public getter, independent
  strong per-facade cache and peer→bridge→cache release. Original8dcc negative
  still exits42 for the exercised metadata P2.
- **7 compiled negative controls** fail their intended behavioral assertion:
  missing both-map snapshot, queued terminal subscription errors, late router
  invalidation, early delegate invalidation, extra strong Root during errors,
  strong oversized-client task and strong client during direct completion.
- Structural tail/guard/helper checks preserve no this-after-DetachClient, no
  peer/state access after terminal app callbacks, distinct actual-peer guards,
  direct terminal delivery and absence of invented numeric bucket sorting.

The original frozen tests remain byte-unchanged. The owned peer test makes an
explicit private adapter for changed representation/mock shapes and retains
byte invariants for unaffected error/notification/query/raw-envelope helpers.
Native host, Chromium WeakPtr/task runner, JSON and media peer dependencies are
mocks; Foundation ARC, standard shared Root ownership and actual extracted
producer/state/client bodies run. These tests do not validate engine IPC,
external observer behavior, real native iteration order or runtime acceptance.
Fresh independent review is required before this proposal is called ready, and
all activation/order gates above remain HOLD after such a bounded review.
