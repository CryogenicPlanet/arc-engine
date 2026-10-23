# DevTools focus and lifetime hook proposal

Proposal: `arc/browser/webextras_devtools_hooks.patch`. Unapplied; the lead owns
Chromium adoption and maintained patch0004. Production `.mm`, shared sources,
probes and active hook registration remain unchanged.

## Outgoing protocol hook WITHDRAWN

`GetDevToolsFrontendMessageHook` and its outgoing CDP dispatch call site are
withdrawn. The complete prior patch is archived unchanged at
`arc/browser/webextras_devtools_hooks_outgoing_protocol_superseded.patch`,
SHA256 `3736b32ebd15e61ee8a9197ab1175d395ee61ff0efca1ec469a3732139ff67d0`.
Do not apply that superseded patch as the native inspect-mode contract.
The primary proposal no longer adds `<string_view>`, the frontend-message
hook declaration, its arcmod include in bindings, or the protocol-dispatch hunk.

New bounded original evidence establishes a different channel:
`inspectModeChanged` is an embedder message whose params are exactly one
string. Registration0xc0ebaa0..ae0 routes to handler0xc0ef394. It rejects wrong
list arity at0xc0ef3d8..e0 and wrong type via0xc0ef54c,0xc483aa0,0x170dd80.
The message invokes bindings virtualslot0x50 (address point0xdd00b80 resolves
to0x4718fd0), then its delegate/window slot0x50 (0xdd01bf0 resolves0x471a128),
then inspected-WebContents user data key0xc9d7108 and peer+0x10, reaching
0x4359a74. `dispatchProtocolMessage` has a separate registration/virtualslot
0x160. No CDP method/session/agent-host test occurs on this local mode route.
The string-to-enum conversion and queued bridge evidence are documented in
`webextras_native_notes.md`; they do not make outgoing Overlay packets the
notification source.

## Retained focus hook

`arcmod::GetDevToolsFocusHook()` is a copied
`base::RepeatingCallback<void(content::WebContents* inspected)>`.
It runs only for an actual `DevToolsWindow::ActivateWindow` after the existing
`kLoadCompleted` check. The hook receives inspected contents so the package can
resolve its service without an extra public Objective-C getter. A window
WeakPtr and repeated stage check protect ordinary activation after reentry.
An empty hook preserves existing Chrome behavior. It does not synthesize an
inspect-mode or display-mode callback.

The held package companion `webextras_devtools_integration_held.patch` uses
only this focus hook plus the app-owned feature44 helper declaration. It does
not depend on the withdrawn protocol hook; its shared bridge/queue adapter
continues to compose with this primary proposal.

## Retained Show and caller guards

The existing Show host hook remains synchronous as a shared extension point.
Current ArcCore production queues its app display callback; consumers of the
shared hook can still reenter synchronously. Preserve the copied Show hook,
window WeakPtr and `kClosing` check before DoAction. No new load-completed
restriction is added to early undocked Show. RepeatingCallback::Run already
retains its bind state, so the copy is an explicit snapshot rather than a
separately claimed closure-UAF fix.

Window LoadCompleted checks lifetime/closing after Show. Bindings
FrontendLoaded checks its own WeakPtr after delegate OnLoadCompleted; outer
LoadCompleted checks before its Android-specific client call. Node frontend
opening returns a live WeakPtr result or nullptr after ActivateWindow or
ScheduleShow. Other direct callers tail-return; InspectElement re-finds the
window. Normal live-path actions are preserved. Android-only behavior is not
compiled or run in the macOS checks.

## Recovered genuine mode emission and read-only candidate

C156 has no `inspectModeChanged` host API, compatibility message or browser
dispatch registration. A separate proposal is required; this patch adds none.

The original packaged frontend now establishes the exact emission site.
Original resource52853, `panels/elements/elements.js`, SHA256
`2616bfe4be3a3ce9eab925f250452f2f1e21398c763cb1a0c790787092e32702`,
contains the controller emission at byte575747. Its setMode checks that targets
are not all suspended, assigns the global mode, calls setInspectMode on each
scoped OverlayModel without awaiting, updates the toggle action, then invokes
ONE `InspectorFrontendHostInstance.inspectModeChanged(mode)` host call.
Resource52490, `devtools_compatibility.js`, SHA256
`5155e65c618a6776d555abd85bcafdabe1d4fc606aaeeaa6292cea1eb3c6217c`,
sends exact embedder method `inspectModeChanged`, `[inspectMode]`, null callback
at byte45170. Clean PAK extraction with explicit Homebrew brotli reproduces
these hashes in `/tmp/webextras-frontend-emission-_mseb9oi/resources-correct/`.
The native and packaged frontend evidence are detailed in
`webextras_native_notes.md`.

The original policies are now recovered, not left to inference: same-mode
setMode calls emit again (tooltip changes call setMode(this.mode)); the
suspended early return emits nothing; suspendStateChanged resets None and the
toggle directly without a host notification; modelAdded reapplies a selected
mode to the new model without a host notification. Preserve these details
rather than adding deduplication, suspension notifications or target replay.
The host call is neither per target nor dependent on backend acknowledgement.

C156's corresponding smallest frontend site is
`panels/elements/InspectElementModeController.ts::setMode` (lines91..101), after
the existing toggle update. The global controller also covers the search
toggle, capture-area screenshot, Escape and EXITED_INSPECT_MODE reset listener.
Its modelAdded and suspendStateChanged paths match the original policies.

`core/sdk/OverlayModel.ts::setInspectMode` (line327) is an unsuitable emission
point: it runs per model, waits for requestDocument, and dispatches an event
carrying only the model/boolean enabled state. The highlighter is replaceable;
ScreencastView's highlighter (line829) changes local inspect config without
sending an Overlay CDP command. A transport observer cannot cover the recovered
frontend contract.

A future minimal restored path is:

1. Add `inspectModeChanged(mode: string)` to InspectorFrontendHostAPI, its stub
   and devtools_compatibility.js. Send the exact embedder method with `[mode]`
   and null callback through existing DevToolsAPI.sendMessageToEmbedder.
2. Call that host method once after the controller's toggle update, retaining
   the original string and the recovered repeat/suspend/modelAdded policies.
3. Register a one-`const std::string&` handler in
   DevToolsEmbedderMessageDispatcher, then forward from bindings through a
   dedicated mode hook keyed by its delegate's inspected WebContents. Existing
   ParseAndHandle enforces exact arity and GetValue(string) enforces string
   type, matching the recovered native parser. Do not intercept outgoing
   protocol messages or infer backend success.
4. The package can convert the proven strings and post the existing strong
   bridge/weak delegate+service notification using its native-lifetime proxy.

This remains a design only. The hook, frontend changes and active package
registration are absent from this primary patch. Standard C156 has no UI
showDistances request, although the native receiver accepts that string; do not
invent an action. Focused controller/OverlayModel tests should cover multiple
targets, custom highlighter, same-mode repeats, entry/exit, suspension and model
replay before a separate integration is adopted. The owner-WebContents lifetime
proxy still does not prove the original nativeImpl discard/rebind lifetime.

## Validation

The revised primary patch passes strict `git apply --check --whitespace=error`
against current Chromium. Temporary copies contain the revised header and
both patched translation units. Actual current macOS arm64 build commands are
used with VFS overlays, syntax-only and no object/dependency output. The held
focus companion is checked against the revised header plus the app-owned
feature declaration, without applying any prerequisite to the workspace.
Both patched Chromium translation units and the composed held package companion
exit0 with zero diagnostic bytes. Command arrays, overlays, staged sources,
logs and checks.json are retained under
`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-focus-only-hooks-23hnd2kq/`.
The revised primary and held companion both pass strict apply checks. Recon's
independent read-only review found the protocol withdrawal and retained guards
consistent with this scope.
No application, build graph, full build, lock or active source registration is
changed. Compiler proof is syntax-only; runtime focus, hosting and restored
inspect-mode behavior remain unvalidated.
