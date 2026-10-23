# Original DevTools target retirement and facade toolbox cache audit

Read-only native/source audit, 2026-10-23. No peer, production, shared source,
frozen proposal, build, app/profile, runtime, lock or board change. Evidence is
`logs/wp/webextras-devtools-retirement-audit/`; `audit.py` regenerates bounded
static disassembly and instruction-anchor checks. It never executes the engine.

Thin arm64 SHA256 `fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
The B70 universal original SHA256
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`
was rehashed; its arm64 slice is byte-identical to FE0. All addresses are unslid.
The inspected held peer is frozen8dcc (`webextras_devtools_logical_peer_held.patch`),
composed source `9e3ca70a0fb3ce7c22f7a7e1913252bc2378d821ea4e6934dba9292c2be42ee6`.
Later peer/cache corrections are separate writers' artifacts.

## Three distinct operations, not one generic Retire

| Native entry | Trigger and verified effect | Deliberately different effects |
|---|---|---|
| `4358934(this,newWC,clearOldUserdata)` | Explicit bind/detach. Clears raw-client map, Observe(nullptr), clears physical weak binding, optionally installs new native weak/userdata binding and Observe(newWC). | Does not directly drain outer pending-command/subscription maps or invalidate Impl notification guards. |
| `4358e14(this)` / secondary thunk `435914c(this+18)` | Physical WebContentsDestroyed observation. Clears raw clients and weak physical binding; moves BOTH outer pending maps out before callouts; directly errors pending commands then all subscriptions. | No direct Impl-guard invalidation, delegate/frontend/toolbox reset, frontend CloseWindow call, or protocol reconnect. |
| `4356b28(this)` | Final Impl destruction: Observe(nullptr), poison command-router guard, destroy raw clients and callback containers, later poison delegate-notification guard and release frontend/logical handles. | Does not run the observer's explicit pending/subscription host-closed callbacks. |

The old generic word “retirement” must not conflate those triggers. Original
logical-owner cleanup calls `4358934(nil,...)`; physical observer delivery calls
`4358e14` only if that observer is still registered. If an earlier owner cleanup
already removed it during the same destruction, do not manufacture a second
observed-target callback phase. Registration/observer ordering is a Core
integration question; source does not authorize calling both unconditionally.

### Physical observation identity is pinned, not guessed from offsets

Native physical destructor `3692120` prepares observer-list+158 at36926a4,
virtual member offset258 at36926a8 and virtual flag1 at36926ac, then invokes
observer dispatcher1dc0dd8 at36926b0. Dispatcher1dc1024..102c loads that observer's
vtable entry+258 and calls it at1dc0f40..44. DevTools secondary vtable addresspoint
da800c8 has cellda80320 (=addresspoint+258) resolving to435914c; the thunk subtracts
18 and branches4358e14. Physical destructor then dispatches its reset operation
at36926bc..cc, before clearing/invalidation of native WC weak state at36926f0..
This is the WebContentsDestroyed/ResetWebContents sequence corresponding to
C156 `WebContentsImpl::~WebContentsImpl` lines1616–1625; slot identity is tied to
the original physical destructor, not a guessed C156 vtable layout.

### Explicit bind/detach path

435895c..89b0 walks raw map+100, destroys each entry with435d09c and frees its
node, zeroes list head+110, bucket entries and size+118, retaining bucket storage.
It does not reset per-Impl connection counter+f8. The old-userdata branch
4358af4..8b4c runs before that clear when requested/valid. Observe(nullptr) is
43589b4..bc; old weak flag/pointer+50/+58 clear43589c0..dc. A nonnil incoming WC
gets its own weak state/userdata route and observation43589e4..8ad8.
No access to pending map+a8 or subscriptions+d0 occurs in the full function.
The callback/client object destructor can queue its own close notification;
that is not the outer pending-map retirement below.

### Observed target destruction: exact ordering and payload

1. 4358e30..84 clears the same raw-client map with435d09c, retaining bucket
   storage and connection counter. Client destructor435d09c calls close435c74c
   before poisoning that CLIENT's private guard+38/+40 and freeing its payload.
   Close clears host+28 before detaching. It posts a copied notification via
   435c8fc with `DevTools connection closed` (d3813a8), then clears the stored
   callback. The copied task invokes at435ca2c; it does not need the client to
   remain alive. Raw close notices are therefore accepted/queued before outer
   pending-map callbacks, but ordinary loop execution does not deliver them yet.
2. 4358e88..a4 clears Impl physical weak flag+50/pointer+58. This handler contains
   no Observe(nullptr) call; the base reset phase follows the physical observer
   notification in the original destructor. Do not silently equate that detail
   with explicit detach.
3. 4358ea8..8f08 moves pending command map+a8 (ID→callable) into stack locals,
   clearing its source buckets/head/size. 4358f0c..68 then moves subscription
   map+d0 (event key→vector of ID/callable records) likewise. BOTH live maps are
   empty before the first pending callback. Original bucket iteration order is
   not ascending request ID or sorted event key.
4. For every retired pending command, 4358fc4..fd8 creates an error with tagged
   code39, message d3811ee=`DevTools agent host closed`, source line376. It invokes
   the stored callable DIRECTLY at4358ff4. This is not a queue helper.
5. After commands, subscriptions iterate event buckets and then each event's
   insertion-ordered vector, stride40 (435903c..9054,4359028). It reads that
   registration's ID at4359054, creates the SAME error at4359058..906c (source
   line383), and DIRECTLY invokes `(ID,error)` at4359090. There is one terminal
   error for each stored subscription, including records whose event never fired.
   No `.disable` protocol command is sent by this function.
6. Retired subscription containers/callback captures are destroyed at43590e4..
   and retired command captures/nodes/bucket storage at43590ec..9108. Their
   captured resources survive the error iteration; no final clear of the live
   maps occurs after callback entry.

Error constructor4b37104 shifts tagged39 right2 into native status14 at
4b37144..4c, copies the literal message, and returns owned error state. NSError
converter44025f4 decodes native status14 and takes fallback code10 at4402690;
its NSString message comes from that owned message. The command adapter44162ac
converts the error then calls the app block DIRECTLY with `(nil,NSError)` at
4416300..10. Subscription adapter4416474 calls `(NSNumber(ID),nil,NSError)`
DIRECTLY at44164ec..6500. These error deliveries are synchronous in the original
physical observer handler even though ordinary successful subscription events
use a posted copied notification. A shared generic NotifySubscription would
change retirement timing.

Reentry consequence: commands and subscriptions removed from the live maps cannot
be redispatched/cancelled by reentrant lookup/unsubscribe. New records created
reentrantly live in the new maps and are not erased at the old-loop end. During
outer error callouts the code operates on local retired containers; after their
preparation it does not dereference Impl again, and does not acquire a strong
Impl/logical-owner keepalive. A faithful adaptation must not add a whole-peer
lease or touch `this` after a callback can release the final peer. Raw-client
payload release/native DetachClient reentry before those snapshots is a separate
harder boundary, not covered by the later callback-loop property. No generation,
new-target revalidation or universal reentrant rebind policy was recovered.
Nested app event-loop draining can deliver earlier posted raw errors during a
synchronous pending error callback; no absolute total callback order is claimed.

### Final destructor and guard distinction

4356b58..60 Observe(nullptr) happens first. Command-router weak anchor+130 is
cleared4356b64, its flag+128 is poisoned4356b74. Router closures use435cca8,
which checks that captured flag/anchor and directly enters435a838 only if valid;
it takes no strong Impl lock. Raw clients are then destroyed4356b98..bd4.
Subscriptions are destroyed through435d160 at4356bd8..dc, pending commands through
4356be0/6de0..df8: only callable DESTRUCTORS run, not error invoke functions.
Delegate-notification anchor+a0/flag+98 is invalidated LATER at4356c00..30, after
those containers, then registration+78/+80 and frontend handles+70/+68 release.
The two guards are distinct; native destruction does not poison both at entry.
The 8dcc candidate uses one peer weak factory and invalidates it before PImpl
member teardown. Exact destructor-phase reentry parity is therefore still a
bounded integration gap, even though accepted normal notifications correctly
survive mere physical detach. No runtime failure is asserted from this difference.

### Raw final cleanup is NOT universally silent

Do not read “pending containers are destroyed without terminal errors” as
“raw clients are silently detached.” All four paths reach the same raw-client
unique-owner destructor435d09c: public close4356904, explicit bind4358974,
observed destruction4358e48 and final Impl destruction4356ba8. That destructor
calls435c74c at435d0bc BEFORE poisoning its private weak flag435d0d0. If a
notification remains stored, close435c840..85c creates/queues the generic
`DevTools connection closed` error; it then empties the stored callback so an
already closed client does not emit another one.

The complete public raw chain has no hidden peer cancellation: ObjC open4415bbc
selects raw adapter4416690 and passes its retained caller block holder to
4355df8. That function copies it into raw client+8/+18/+20 at4355e4c..8c.
435c8fc copies those fields into the posted task at435c994..9b8. Task435ca2c
invokes that copied callable at435caa8 without an Impl/client/service guard.
Adapter4416690 error branch converts status and directly calls the retained
caller block `(nil,NSError)` at44166e0..66f0. Final delegate flag98 poisoning or
client flag38 poisoning cannot cancel that already copied raw notification.
Therefore a blanket **silent raw final detach is contradicted by this chain**.
The conditional case where its notification was already emptied is naturally
silent, and final outer command/subscription cleanup is separately silent.

There IS a distinct raw-client lifetime gap in the current C156 candidate:
its oversized-message task strongly retains ProtocolClient. Original435c5b8..
618 captures only that client's weak flag38/anchor40; thunk40c5ad0 tests the flag
then invokes Close on the raw anchor, with no strong client retention. Thus
original map destruction can close/free a still-live raw client immediately,
queue its copied error, and make the older oversized task inert. A C156 task
strongly retaining the client can postpone destructor-driven close until that
older task runs. Explicit public close already calls Close immediately despite
that retained task; final/map teardown currently relies on shared destruction.
This supports a bounded weak-client task/explicit immediate raw cleanup design
review, not silence, a new peer guard, callback padding, reconnect, or retarget.
The precise adaptation and reentrant backend detach safety remain unimplemented.

## C156 correspondence and concrete staged gap

C156 `DevToolsAgentHost::GetOrCreateFor` selects the primary-frame agent
(`render_frame_devtools_agent_host.cc:178–186`), rather than the distinct tab
agent API. Frame-tree teardown can call RenderFrameDevToolsAgentHost's
DestroyOnRenderFrameGone694..704 and ForceDetachAllSessionsImpl. C156
`DevToolsAgentHostImpl::ForceDetachAllSessionsImpl:481–488` calls DetachClient and
then each client's AgentHostClosed. Explicit DetachClient344..352 alone detaches
that session; it does not call client AgentHostClosed itself. Its internal owner
protects the host during detach, not Arc's outer pending maps.

The held ProtocolClient translates transport close to a posted generic close
notification; its command receiver ignores `failure`. That matches the separately
recovered native internal transport close435d880 empty-envelope route, whose
outer command router ignores missing id/method. It does NOT implement4358e14's
later explicit outer host-closed error phase.

8dcc `Impl::Retire` currently only removes native userdata, Observe(nullptr) and
clears `_contents`; both explicit peer Retire and WebContentsDestroyed call it.
It neither closes/removes raw `_connections` nor snapshots/errors `_pending` and
`_subscriptions`. Final peer destruction invalidates one shared guard then
PImpl member destructors run. Thus accepted frontend-notification lifetime was
addressed by that candidate, while these per-target cleanup contracts remain open.

Recommended next delta, subject to the lead's explicit trigger contract:

- Separate explicit bind/detach from observed-target destruction. Explicit detach
  must close/retire raw clients without draining outer pending/subscription maps.
  Preserve IDs and selected registration/guard continuity.
- Only the genuine observed-target phase should move BOTH live pending tables
  into owned locals after physical binding clear, invoke command errors first
  and subscription errors with original IDs synchronously, and then destroy the
  retired locals. Leave frontend/toolbox lifecycle and command-channel reconnect
  alone. Do not route through success-event async notification helpers.
- Final destruction releases captures without fabricating host-closed pending
  errors. Reconcile separate command-router/delegate guard poisoning order if
  exact teardown reentry is activated; a new alive token is not a replacement.
- Needed meaningful tests: populated raw/command/subscription state on each of the
  three triggers; callback IDs/error/nesting; both live maps empty at first
  callback; reentrant new registration surviving; unsubscribe against retired
  IDs; nil service and final peer release during first callback; accepted old
  queued events still independent; no fake DidClose/reconnect or synthetic
  protocol disable; raw task acceptance versus direct callback timing.

No implementation or synthetic behavior tests were added by this audit. Raw final-close delivery remains eligible as proven above; only the outer pending terminal errors are absent at final destruction.

## Toolbox facade-cache proof for the separate metadata repair

The metadata TSV declares `toolboxWebContents` with
`T@"ArcWebContents",R,N,V_toolboxWebContents`, with REAL ObjC ivar at+8.
Native service getter4415204 reads Impl+70; service.webContents44151a0 instead
reads Impl+68. These are distinct handles. A C156 in-tab frontend accessor cannot
be assumed to provide native toolbox identity.

- If Impl toolbox+70 is NIL, branch4415230→44152e0 returns nil. It does **not**
  clear, release or overwrite the retained ObjC cache at service+8.
- If nonnil, 4415234 loads actual facade cache. If present, 4415248 calls its
  nativeWebContents aggregate getter, checks engagement, reloads current Impl
  toolbox, then compares cached native pointer x22 to current pointer x21 at
  4415280..84. Equality returns the SAME retained/autoreleased cached wrapper.
- Missing/different cache allocates/initializes an ArcWebContents using the CURRENT
  toolbox native handle4415288..52ac; stores new cache at44152b4 and releases the
  previous ObjC wrapper at44152bc. This is real facade-owned cache storage.
- Service `.cxx_destruct` releases native handle4415e98..ea4, bridge4415eb0..ebc,
  THEN storeStrong(&self+8,nil)4415ec0..ed4. Old cache may survive absent toolbox
  and native-handle/bridge teardown; release timing matters beyond property text.

Native explicit frontend close4357fd0 calls CloseWindow4358008, invokes close
notification helper4358240 at4358064 (its synchronous bridge call is43583c0),
then clears Impl frontend+68 at4358080..84. Toolbox-change callback4358b84(nil)
clears Impl toolbox+70 at4358c48..4c. Neither directly mutates the ObjC service
cache. The actual DidClose ObjC bridge44160b8..6108 only weak-loads delegate/service
and sends the callback; it contains no service cache write. The app callback can
itself call the getter and populate/replace that cache; this is a separate
reentrant action, not a hidden native reset. This is bounded source proof for
these paths, not a whole-binary alias-completeness theorem.

Consequently the separate repair must restore a genuinely used facade cache and
V/offset, remove the peer-owned duplicate/eager cache reset/retirement lease, and
preserve old cache on native absence. Full cache population still needs the real
native toolbox identity/discovery seam; inventing an alias to frontend+68 or a
dummy ivar does not satisfy that gap. Parent/writer were given the exact ranges;
no cache proposal is edited here.

## Validation, acceptance and remaining limits

`audit.py` verifies pinned thin/source bytes, regenerates24 native ranges,
resolves Mach-O cstrings/vtable cells and checks critical exact instruction
anchors. C156 excerpts/hashes, original full/thin identity, reviewed peer snapshot
and all artifact hashes are recorded in the manifest. These are static evidence
checks, not an engine test/build. No scenario was rerun and no diff status changed.

Lead still owns observer/owner cleanup transaction ordering, actual binding/root
ownership, final-release reentry, distinct toolbox discovery and runtime
comparison. This audit makes no claim that observed-target cleanup is full
frontend close or that raw clients can reconnect/retarget. It refines the specs'
remaining close/error questions; it does not authorize an architecture exception
or overwrite prior frozen proof.
