# HELD DevTools embedder events pipeline

These are reviewable, unapplied proposals. No actual Chromium/shared source,
production Objective-C++, probe, maintained patch, app, build graph or lock was
changed. The lead must adopt the prerequisites and review this proposal before
activation. This does not supersede the archived outgoing-protocol withdrawal.

## Exact composition

Shared order, relative to current Chromium:

1. `webextras_devtools_hooks.patch`, SHA256
   `dcbf472a06bd6a722cfaf591c3a0c4662b8759196d77babeb7fec4ad14bebd17`.
2. `webextras_devtools_host_docking.patch`, SHA256
   `d9295b5cd7faea6251b4845d196323c2c8f5dfe31bd25bf26b78b5bfa8f1eac9`.
3. `webextras_devtools_embedder_events.patch`, SHA256
   `2fae816858d06e2a78eddb7c32caa0cea0bdd27634adaa0a23dcc67e8944d24f`.

Owned order, relative to current `arc_webextras_devtools.mm`, SHA256
`cef1ff1c919943659096e7218cd4b056ad3f986bcf39f364e906d7f1c53342c0`:

1. `webextras_devtools_integration_held.patch`, SHA256
   `a2a9d232a57adc19daaa13c182c719523c4eb2a315f93002c1a08ca1d3c18cad`.
2. `webextras_devtools_embedder_companion_held.patch`, SHA256
   `ed9a828d55029381d8b899711f3459b52ed71369acdab881323173d7c69f6bfb`.

The old held companion still supplies feature44-before-owner-access and real
focus callback wiring. Its statement that the precise frontend trigger is
unproven is superseded by the original evidence below. Both companions require
the app-owned ArcRuntimeFeatureEnabled declaration from
`logs/wp/app.pending-integration.patch`; only that header hunk was temporarily
staged for compiler checks. No shared feature implementation is invented here.

## Original proof and producer policy

Read `webextras_native_notes.md` sections at256 and313. Original embedder
registration/one-string handlers route inspectModeChanged via c0ebaa0/c0ef394,
bindings4718fd0, window471a128, inspected-WC user data, producer4359a74.
requestDockPosition uses registrationc0eba34, handlerc0ed650, bindings4718fc0,
window471a0d8, producer4359388. Both handlers require exactly one string;
wrong arity/type does not invoke the callback. This is separate from the
outgoing CDP dispatch registration/virtual slot. There is no backend ack gate.

Original resources.pak SHA256
`56734b34502ff1e68d72a9d36b19715a8e55fd3b910a8ce7839304cbd0fe07bc`;
clean extraction explicitly uses Homebrew brotli. The regression pins the
following clean original resource hashes:

- 52853, elements.js:
  `2616bfe4be3a3ce9eab925f250452f2f1e21398c763cb1a0c790787092e32702`.
  The relevant UTF-8 byte offset is575760 (575747 was a character index in
  earlier hook notes, not the byte offset).
- 53015, legacy.js:
  `723be0b9811a6686918ea2889e72a69466d07197fcb3313804f86fa86884853e`.
  Dock request byte offsets30936/31298.
- 52490, compatibility.js:
  `5155e65c618a6776d555abd85bcafdabe1d4fc606aaeeaa6292cea1eb3c6217c`.
  Dock method45044, inspect method45170.

The shared frontend changes reproduce these exact producers:

- InspectElementModeController.setMode returns while targets are suspended;
  otherwise it assigns global mode, calls each scoped OverlayModel without
  awaiting, updates toggle state, then sends ONE host.inspectModeChanged(mode).
  Zero models still sends once. Same-mode/tooltip calls repeat; multiple exit
  events can each send None. modelAdded replay and direct suspension reset send
  nothing. No showDistances action is added.
- DockController.initialize with canDock=false requests undocked and returns.
  setDockSide validates/falls back to right, skips an unchanged side, then
  requests the exact side BEFORE CSS, settings, BeforeDockSideChanged and
  setIsDocked. It does not wait for the docking response callback.
- Host API and nonembedded stub expose both methods; the compatibility host
  sends inspectModeChanged with [mode], null callback and requestDockPosition
  with [side], no callback, matching original compatibility JS.

## Native and owned file chain

The new shared patch changes eleven files:

- Four frontend producer/API files: core/host/InspectorFrontendHostAPI.ts,
  InspectorFrontendHostStub.ts, panels/elements/InspectElementModeController.ts,
  ui/legacy/DockController.ts; plus devtools_compatibility.js.
- devtools_embedder_message_dispatcher.{h,cc}: typed one-const-std::string&
  virtual handlers and registrations. The existing ParseAndHandle/GetIfString
  parser supplies exact arity/type rejection. No new permissive JSON parser.
- devtools_ui_bindings.{h,cc}: copy the inspect/dock hook, synchronously pass
  inspected WC, frontend WC and exact string, then return without further
  member access. Generic message dispatch also tail-returns; no outgoing
  protocol interception or required agent attachment. The outer frontend-host
  callback tail-runs bindings dispatch, which tail-dispatches to the typed
  handler; DispatcherImpl tail-runs its stored callback. RepeatingCallback::Run
  retains bind state through invocation. ParseAndHandle's stack ParamTuple owns
  the string and only returns true after invoking the method. This bounded
  chain does not access a destroyed bindings/dispatcher after hook reentry.
- arcmod_hooks.h: creation hook and distinct inspect/dock string hook slots.
  Empty hooks preserve ordinary native behavior. Hooks share no frontend state.
- devtools_window.cc: invoke copied creation hook after completed construction,
  before returning to caller agent AttachTo and ScheduleShow. Weak window and
  inspected anchors prevent a reentrant hook from returning a dead/closing
  window. Inspected contents are weakly captured BEFORE construction and
  checked before notification and afterwards. If the target expired while a
  window remains live, the factory calls existing InspectedContentsClosing and
  immediately returns null without further member access. This marks closing
  and uses native ClosePage retirement; destruction can be asynchronous. The
  sole Create caller with nonnull inspected contents snapshots a
  weak inspected pointer for the failed-create policy-dialog branch.

The owned companion keeps focus and feature44 wiring unchanged and installs
creation/inspect/dock adapters keyed by inspected contents. Creation stores the
weak frontend, native user data and observer without calling app code, public
getters, KVO or renderer functions. The getter therefore works before the first
dock request. This association is separate from Show/readiness: currentPanel
continues to queue while opening, and openNetworkView retains its pending panel
until Show instead of dispatching script into an unready frontend.

Show completes readiness and pending panel queries only; it never queues a
display callback. requestDockPosition is the ONLY display source, maps captured
undocked/left/right/bottom to1/2/3/4 and all other strings to0, then posts through
the existing strong-registration/weak-delegate+service UI queue adapter. Inspect
uses the same model: searchForNode1, searchForUAShadowDOM2,
captureAreaScreenshot3, showDistances4, all other strings including none0.

A nonnil delegate replacement updates the existing bridge; nil drops only the
current strong bridge; captured old bridges survive nil/new and close. Receipt
promotes the native weak registration alias. Delivery reads that captured
bridge's current weak delegate and service and permits a nil service argument.
No frontend identity, Show flag, navigation or close epoch cancels accepted
notifications. There is no inferred event when native state changes.

Early association also makes a frontend peer exist before Show. Original
natural-close43586ac tests nativeImpl+68 peer presence. The held companion uses
current observer presence as that explicit association proxy, snapshots it and
resets/disarms before notification. It does not depend on WeakPtr validity
inside WebContentsDestroyed or _shown. C156 notifies WebContentsDestroyed then
ResetWebContents in its destructor; using observer presence avoids coupling to
WeakPtr teardown order. A retired observer is destroyed before native/app close
reentry, so its later destruction task cannot erase a replacement frontend.

## Checks and limits

All five patches pass strict sequential apply checks in temporary copies.
Fresh actual macOS arm64 Chromium commands, VFS staged headers/TUs and
-fsyntax-only compile the composed devtools_window.cc, devtools_ui_bindings.cc,
devtools_embedder_message_dispatcher.cc and held owned .mm: each exit0 with
zero diagnostic bytes. Proof, command arrays, overlays, source copies and logs:
`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-embedder-events-xx8j_8nc/`.
This checks actual C156 API compatibility; Android branches were not compiled.

Run the owned extraction regression with:

```
python3 arc/browser/webextras_devtools_embedder_regression.py \
  --original-resource-dir /tmp/webextras-frontend-emission-_mseb9oi/resources-correct
```

It passes11 original-versus-staged inspect/dock/compatibility producer cases,
10 existing actual close/reset cases plus a pending-natural-close/replacement
case, and5 staged actual receipt/association/queue groups, plus5 extracted factory-tail
weak/reentry/retirement cases. Extracted event SHA256
`b71355e7df070bde72fcd0a5fbe746df02ab0e2e1a0cabd27e1a79b6025cd433`;
close SHA256 `9d07de836c40f29e55e233f8f194d4c6223a0cb038bcef8afd900292e19cb714`.
It verifies no per-model/Show duplication, request ordering before CSS/events,
mode capture, quiet suspension/replay, early getter state, accepted queued work
after close/replacement, bridge replacement and stale observer cancellation.
It executes extracted frontend methods and Foundation/native mocks, not
constructors, browser IPC or Chromium's real task runner. TypeScript extraction
is transpiled with the repository compiler, not a full frontend build.
Independent recon reran the producer/close/receipt regression and found no
substantive source issue. The final additional factory-tail cases were locally
executed after adding target-detach retirement; the constructor and native
ClosePage operation remain mocks.

A no-emit semantic TypeScript probe used the actual frontend compiler config,
real global_typings/global_defs.d.ts, legacy/legacy-defs.d.ts and installed
filesystem typings, with temporary read overlays for the four changed TS files.
Standalone composite/incremental bookkeeping is disabled identically for both
runs; noEmit remains enabled. The exact probe script is retained with evidence.
Baseline and staged each produce the SAME55 diagnostics, zero additions or
removals, and zero diagnostics in the four changed files across1041 source files.
The remaining diagnostics include missing mocha/node ambient types, standalone
isolatedDeclarations/allowJs setup and inherited DOM declaration conflicts.
This is a comparative limited type probe, NOT a clean full frontend typecheck.
The comparison and complete diagnostics are in the compiler proof directory.
No suppressions or invented type declarations were added.

Remaining limits are explicit: the owner-WebContents weak anchor is a proxy for
nativeImpl lifetime, with discard/rebind unproved; canonical wrapper identity
versus original fresh getter wrappers remains broader package work; hosted
undocked native close semantics remain unvalidated. Existing constructor
creation-test/pref callbacks precede our factory hook and could theoretically
pump a nested event loop; the hook guarantees ordering on the normal production
construction path and adds no nested loop, but does not repair arbitrary
pre-existing constructor reentry. Retirement through InspectedContentsClosing
assumes the normal newly constructed close_on_detach=true state; exotic earlier
constructor callbacks changing ownership/browser state are unproved. No app code runs in the new association path.
The replacement-frontend mocks prove wrapper cancellation/reentry only. Real
C156 ToggleDevToolsWindow uses FindDevToolsWindow(agent), which can find a
closing instance until bindings detach even though the inspected-WC getter
lookup skips closing windows. Immediate real F2 creation during before-unload is
therefore unproved; no FindDevToolsWindow filter change is proposed here.
Actual paired runtime/golden comparisons, frontend bundling and native UI remain
required before acceptance. There is no runtime/parity claim from these checks.
