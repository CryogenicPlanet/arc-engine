# DevTools hook proposal

Proposal: `arc/browser/webextras_devtools_hooks.patch`. The lead owns applying
these changes to Chromium and incorporating them into maintained patch 0004.
No Chromium source, maintained patch, shared header, application, or other
existing notes were modified to prepare or validate this proposal.

The current hooks implementation is header-only. There is no
`chrome/browser/arcmod/arcmod_hooks.cc`, so this proposal changes the existing
header and the two real call sites only.

## Hook semantics

`arcmod::GetDevToolsFrontendMessageHook()` has callback type
`void(content::WebContents* frontend, std::string_view message)`.

- Called synchronously by
  `DevToolsUIBindings::DispatchProtocolMessageFromDevToolsFrontend` after its
  existing attached-agent check, immediately before forwarding that frontend
  command to the agent.
- Receives the frontend WebContents and the complete outgoing command JSON.
  It does not receive backend replies, external client commands, or inferred
  inspect-mode notifications.
- The string view and WebContents pointer are borrowed for the callback only.
  Consumers must copy text they retain and use weak ownership for retained
  WebContents.
- The hook callback itself is copied before invocation, allowing callback
  replacement/reset during reentry. The command string is copied before the
  hook, so destroying its original owner during reentry cannot invalidate it.
  A bindings WeakPtr and original-agent snapshot prevent dispatch through a
  destroyed binding, detached agent, or replacement target after the hook
  returns. The original agent is retained across reentry, and dispatch proceeds
  only if the bindings still refer to that exact agent.

`arcmod::GetDevToolsFocusHook()` has callback type
`void(content::WebContents* inspected)`.

- Called synchronously only by an actual `DevToolsWindow::ActivateWindow`
  request after its existing `kLoadCompleted` check.
- Receives the inspected WebContents, allowing ArcCore to identify the owning
  DevTools service and send the existing `webContentsDevToolsNeedsFocus:`
  delegate callback.
- A copied hook, window WeakPtr, and repeated lifecycle check protect native
  activation after hook reentry. If the window remains alive and loaded,
  ordinary Chromium activation continues. An empty hook preserves existing
  behavior. This hook does not synthesize a display/inspect-mode callback.

## Existing Show hook and its callers

The existing `DevToolsWindow::Show` host hook also runs synchronously. The
package's `didShowFrontend:docked:` calls the actual app
`webContentsDevTools:displayAs:` delegate before returning. That delegate can
call the public `close`, which calls the frontend's `ClosePage`. Native
`DevToolsWindow::CloseContents` marks the window closing and resets its owned
frontend; destruction of `DevToolsUIBindings` destroys its owning window.
The old Show path called `DoAction(action)` unconditionally after this reentry;
`DoAction` accesses `bindings_` for inspect/reveal actions.

- Show now snapshots the callback, captures the window WeakPtr before invoking
  it, and returns if the window died or entered `kClosing`. Otherwise it runs
  the same action. No new `kLoadCompleted` requirement is added: the existing
  early undocked Show path remains valid.
- Copying the callback gives an explicit invocation snapshot. Chromium's
  current `RepeatingCallback::Run` already retains its bind state across the
  invocation (`base/functional/callback.h`); callback replacement by itself is
  not claimed as a separate closure lifetime defect.
- `DevToolsWindow::LoadCompleted` applies the same lifetime/closing check after
  Show, before resetting its pending action or accessing its completion
  callback.
- `DevToolsUIBindings::FrontendLoaded` checks its own WeakPtr after invoking
  `delegate_->OnLoadCompleted`, which reaches window LoadCompleted and Show.
  Its outer `LoadCompleted` checks again before the Android-specific client
  call. Thus unwinding this same close-from-display chain does not access a
  destroyed bindings object.
- `OpenNodeFrontendWindow` checks a saved window WeakPtr after either
  `ActivateWindow` or `ScheduleShow`, returning the live window or nullptr if
  it died/entered closing. It cannot return the previous stale raw pointer.

All other direct Show/ScheduleShow callers in `devtools_window.cc` perform no
post-call member/raw-window use: ScheduleShow, SetIsDocked, OnLoadCompleted,
worker/frame/external opening and toggle paths return directly. InspectElement
re-finds the window after opening before accessing it. These paths are left
unchanged. The guards preserve the existing behavior when the objects remain
alive and the window is not closing; they do not queue or synthesize callbacks.

## Package wiring

The package's existing `ToolsForContents` user-data mapping already associates
both the inspected contents and the frontend with the same service. These hook
signatures therefore support the requested package callback mapping unchanged:
look up the service by the frontend for outgoing-command interpretation, or by
the inspected contents for the focus delegate callback. Registration and native
command-to-mode interpretation remain package work; the Chromium hook itself
does not invent that interpretation.

## Validation

- `git -C chromium/src apply --check ../../arc/browser/webextras_devtools_hooks.patch`
  passed against the current source tree.
- Both patched `devtools_ui_bindings.cc` and `devtools_window.cc` passed
  `-fsyntax-only` with the real `out/Base` compiler commands and Chromium's
  warning/error/plugin settings: exit 0, no diagnostics.
- Compilation used temporary copies mapped over the three original paths with
  `-ivfsoverlay`, `use-external-names: false`; object/dependency output flags
  were removed. No build graph changes, builds, application launches, or lock
  acquisition were performed.
- The updated Show/caller proposal was rechecked using freshly queried current
  macOS arm64 `out/Base` compiler commands. The Android-only client-call branch
  was not compiled or run; its surrounding WeakPtr guard was compiled on macOS.
- This is compile evidence only. Runtime callback timing/behavior awaits lead
  application, package registration, and the owned-app comparison run.
