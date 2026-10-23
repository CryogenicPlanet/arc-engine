# DevTools hook proposal

Proposal: `arc/browser/webextras_devtools_hooks.patch`. The lead owns applying
these changes to Chromium and incorporating them into maintained patch 0004.
No Chromium source, maintained patch, shared header, application, or existing
notes were modified to prepare or validate this proposal.

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
- This is compile evidence only. Runtime callback timing/behavior awaits lead
  application, package registration, and the owned-app comparison run.
