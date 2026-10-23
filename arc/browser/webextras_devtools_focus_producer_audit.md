# Original DevTools focus producer audit

READ ONLY production/proposal audit. New files are this note and
`logs/wp/webextras-devtools-focus-producer-audit/` only. No helper, probe,
Chromium, API, shared patch, app, recording, lock, profile or coverage change.
This is static producer evidence and a proposed diagnostic, not execution or
acceptance of NativeDevToolsNeedsFocus. Parent owns the active Native12 run.

## Result

A genuine existing frontend action, **Run command**
(`quick-open.show-command-menu`), unconditionally calls its installed
InspectorFrontendHost's `bringToFront()` before opening the command menu. Both
original packaged assets and current C156 source contain this path. It provides
a bounded prospective diagnostic through the real frontend action registry;
no fabricated host, direct app callback or OS input is needed. Executability
against the owned original frontend target remains untested. A missing frontend
CDP route or incomplete frontend initialization must be reported as inconclusive,
without creating a replacement frontend/host or registering the action manually.

The native path requires the window's loaded stage, an inspected native target
and its real DevTools userdata peer. It does not require that an OS window was
inactive or actually changed focus. Opening/displaying DevTools is not proof
that this request occurred. A genuine startup panel reveal can independently
request focus through the same host callback, so source alone gives no exact
callback count or attribution during startup.

Two corrections matter: supplied address43585e4 is an instruction in a different
helper; the actual focus producer is4359dd0. More significantly, the focus
helper's nested argument payload **strongly retains the actual DevToolsImpl**,
including in its uncounted inert task. Earlier lifetime prose describing focus
as weak-Impl-only omitted that nested payload. The outer weak guard still exists;
this finding does not remove it, retain Root, or establish another producer's
payload. Frozen proposals and prior evidence remain unchanged.

## Pins and reproducibility

Original thin `/tmp/arccore-webextras.arm64` SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
Full original framework in `../arc/build/ArcModAgent-trace156.app` SHA256
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.
The script rehashes both, verifies the full binary's arm64 slice equals the thin
bytes, and disassembles only bounded address ranges offline. All addresses here
are original unslid arm64 VAs. Mach-O segment translation and raw pointer words
are preserved in native-data.json; low36 decoding is checked against instruction
and selector targets, rather than treating an encoded word as an ordinary pointer.

Original asset names come from the recovered original resource table. Prior
preservation-manifest records resources.pak SHA256
`56734b34502ff1e68d72a9d36b19715a8e55fd3b910a8ce7839304cbd0fe07bc`;
this audit rehashes the nine decoded resources, without claiming a fresh pak
extraction. `packaged-frontend-hashes.json` records their complete identities.
`audit.py` produced18 bounded native ranges and31 static instruction assertions.
These are source/disassembly checks, not runtime tests. `inputs.json` pins current
C156 full files and immutable PRIMARY patchdcbf472a. No compiler was needed or run.

## Genuine packaged frontend emitter

Original devtools_app.html resource52489 loads52492
`entrypoints/devtools_app/devtools_app.js`, which imports52508 shell.js; shell
imports53009 quick_open-meta.js. That metadata registers action
`quick-open.show-command-menu` with `loadActionDelegate()` returning the real
`CommandMenu.ShowActionDelegate` from53010 quick_open.js. Original53015 legacy.js
Action.execute awaits its registered loader then calls delegate.handleAction
with the real Context and action ID. ShowActionDelegate.handleAction has no
user-gesture or enabled-state branch: it calls
`InspectorFrontendHostInstance.bringToFront()`, then `QuickOpenImpl.show('>')`,
then returns true. A future diagnostic should still require the action's
ordinary enabled state, rather than bypassing the framework's UI eligibility.

Original52476 host.js installs the existing global InspectorFrontendHost if
present; otherwise it creates a hosted stub whose bringToFront is empty.
Original52490 compatibility.js creates the genuine InspectorFrontendHostImpl;
its bringToFront sends `DevToolsAPI.sendMessageToEmbedder('bringToFront', [],
null)`. The API increments a call ID, builds `{id, method}`, omits params for the
empty array and invokes the real `DevToolsHost.sendMessageToEmbedder` with JSON.
Thus a hosted stub or arbitrary inspected-page global is insufficient.

Other real emitters exist but are less isolated diagnostics. Original52910
sources.js showDebuggerPausedDetails calls window.focus then host.bringToFront;
getting there depends on debugger target/context/pause handling. Original53015
InspectorView passes host.bringToFront as the panel tabbed-location reveal
callback. Neither a raw Overlay command nor an inspected page's
Page.bringToFront is this frontend action. They must not be presented as proof
that this frontend emitter ran.

## Original browser routing, gates and timing

1. Browser bindings HandleMessage bodyc0f7f04 returns if frontend_host+68 is nil.
   It looks up method/params using strings cf1e6ce/cf1e6d5; a missing method or
   present non-list params reaches the invalid-message branchc0f7f58..7fd4.
   Optional id is read atc0f7fd8..7ff0, absent params become an empty list, and
   dispatch occursc0f8094 via dispatcher+198 andc0eede0. This is the embedder
   channel, separate from inspected-target CDP protocol dispatch.
2. Registrationc0eb73c..788 uses CStringd191ac5=`bringToFront`, member offset10
   and virtual tag1, bound to no-argument handlerc0eef68 byc0ed4a8.
   Dispatcherc0eede0 looks up the method then runs the stored repeating handler;
   c0eef94..9c tests list size zero, rejects nonzero arguments and only applies
   the bound callback atc0ef060..70 on success. Empty params produce no payload.
3. Member dispatch2a0f95c..96c/9cc..9d4 resolves bindings address pointdd00b80+10
   to c0f85d4. That loads delegate+58 and virtual18, resolving window address
   pointdd01bf0+18 to471a060. These exact encoded entries are saved.
4. Window471a068..70 requires life_stage+104==3. Its inspected contents+50 must
   exist, the contents userdata lookup keyedc9d7108 must succeed, and userdata
   peer+10 must be nonnull. It tailcalls4359dd0 at471a09c. C156's corresponding
   enum identifies stage3 as kLoadCompleted; original SetIsDocked/load stage
   machinery also uses the same3 transition in the surrounding recovered code.
   This body has no dock-mode, active-window, OS input or foreground-state test.
5. Producer4359dd0 obtains its real self shared handle: control+10 is loaded at
   4359de4, control strong count+8 CAS-locked4359df4..e1c, and the pair copied
   to sp4359e20. It passes delegate slot this+78, member28/tag1 and this shared
   handle to async helper42d4298 at4359e40..50. The source labeld381293 is
   `arc::WebContents::DevTools::Delegate::DevToolsNeedsFocus`.
6. Helper42d42c0..4304 locks the currently selected native bridge registration
   weak pair. Only a live nonnull registration increments pending at42d4308..10.
   It copies that selected strong bridge into the task and posts with zero delay
   at42d4478. Later replacements must not redirect this accepted task. The
   no-registration/expired branch posts an uncounted inert task at42d4584.
7. The outer invocation42c1740 tests captured flag+4 and anchor. Its anchor is
   this+78; the helper captured Impl+98/+a0, not physical target liveness.
   Registration invoke42c0840 and inner4146f80 dispatch the selected bridge's
   virtual28. Address pointda88250+28 resolves441610c.
8. Bridge4416120 weak-loads app delegate,441612c weak-loads ObjC service and
  4416144 sends selector `webContentsDevToolsNeedsFocus:` (selector reference
   e290ef8 ->d75e956). There is no nil-service or frontend/current-target test
   between these loads and the call. The callback is asynchronous relative to
   the frontend action. An action return/IPC acknowledgment alone is not
   evidence that the app delegate received it.

## Nested self ownership: superseding focus-only correction

The self handle identity is concrete. Original factory436f2c8 allocates the
control+Impl block, sets control vtableda824e8, constructs DevToolsImpl at
allocation+18 (436f2e8/310), and installs its own pointer/control at allocation+20
(436f348), which is Impl+8/+10. Focus4359dd0 uses exactly that pair, not the
logical owner's weak pair at Impl+38/+40.

After selecting the registration, helper42d437c..4394 copies the supplied self
pair into inner callback+38/+40 and increments its control+8 strong count.
The inner invocation4146fc4 passes the stored pair as the argument to the bridge.
Inner destructor42c1630..44 loads stored control+40 and decrements strong+8.
The callback object remains present through the bridge call; ordinary post-call
callback destruction releases it. The strong selected bridge is a separate
capture. This actual self payload can therefore preserve the peer and its
own functional state through delivery independently of ObjC service/Root.
It does not imply strong ownership of the logical Root or app delegate.

The no-registration branch likewise copies the self pair into inert task+40/+48
at42d4554..6c and increments strong+8. Inert invoker42db5e4..60c moves that
pair to its call-local payload and releases after its no-op call; if never
executed, task destructor42c0c98..ca8 releases the captured control. Pending is
not incremented on this branch. An empty closure that drops the peer immediately
is thus not structurally equivalent even though both notify nobody.

Earlier webextras_devtools_lifetime_audit_notes.md's focus weak-only sentence
and its blanket accepted-focus final-expiry implication are superseded by this
bounded nested-payload evidence. Its concrete outer guard/physical detach facts
still hold; display/inspect payloads need separate rereading before extending
this correction. Current initial peer's PostDevToolsFocus retains selected
bridge but captures only weak real peer; it also returns without posting when
bridge is absent. A focus-only future correction should capture the SAME actual
peer for counted and inert work, keep the selected bridge/delegate rules and
existing pending policy, and avoid acquiring Root or resolving a public wrapper.
Do not activate that delta without independent native/payload review and tests
where all external peer owners disappear before queued work drains.

## C156 correspondence and held PRIMARY placement

Current C156 dispatcher registers bringToFront to Delegate::ActivateWindow at
line347. HandleMessageFromDevToolsFrontend920 has the same frontend-host,
method and optional-list checks. DevToolsUIBindings::ActivateWindow1013 forwards
to its delegate. DevToolsWindow::ActivateWindow1718 returns before kLoadCompleted;
ordinary docked frontend Focus/undocked browser Activate follows that gate.
It has no separate BringToFront C++ method: the wire name maps to ActivateWindow.
Blink DevToolsHost JSON parsing, DevToolsFrontendImpl bound Mojo host and browser
DevToolsFrontendHostImpl callbacks supply the genuine IPC path; no OS input
requirement appears in the inspected C156 methods.

PRIMARY dcbf472a inserts its copied focus hook just after the loaded-stage gate,
before Chrome's focus operation, with window WeakPtr/stage recheck after app
reentry. That placement matches the recovered original notification window for
this explicit request; it does not equate callback delivery with OS focus.
There are other actual ActivateWindow callers, including existing Node frontend
reuse and ChromeDevToolsManagerDelegate::Activate. Show/DoAction is separate:
ordinary kShow/kToggle DoAction does nothing, while frontend panel reveal can
later invoke its own bringToFront callback. No blanket startup-zero assumption
or filtering of other genuine ActivateWindow producers follows from this audit.

## Minimal prospective diagnostic, not implemented

Use only an already-owned, positively identified genuine frontend page from the
same fresh recording/control fixture; preserve its real inspected target and
existing delegate registration. First record URL/frame/target identity, loaded
UI readiness, and the existence of genuine DevToolsHost plus non-hosted installed
InspectorFrontendHost. An arbitrary page with matching globals is not identity
proof. Do not load compatibility.js manually, add frontend flags or open a
surrogate frontend. Baseline callbacks remain in the raw transcript.

Once that real UI is ready, one bounded evaluation in its own main world may
import its already-used `./ui/legacy/legacy.js` relative to the actual frontend
base URL. Require `ShortcutRegistry.instance()` to succeed without forceNew or
constructor arguments (it throws if ordinary UI initialization has not created
it). Then require the ordinary ActionRegistry contains and enables
`quick-open.show-command-menu`, and invoke **that registered Action.execute()**
once. This invokes the genuine action loader and host request described above;
it neither calls the host directly nor sends a handcrafted embedder message.
Record evaluation result and task-clock markers separately from callback raw
records; leave missing/extra callbacks visible. A true result establishes action
completion, not native gate passage. Do not assert one callback absent paired
run evidence; startup/reveal callbacks may already be queued.

If frontend target discovery/evaluation is unsupported by the existing original
fixture, stop that lane as inconclusive and request a legitimate frontend access
route from lead. A genuine debugger pause is an alternative real producer but
has more settings/context/protocol timing gates and should be a separate scenario,
not a hidden replacement for this action. No diagnostic was run, no helper was
changed, and uncovered golden status remains unchanged.
