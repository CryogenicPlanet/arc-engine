# Hosted DevTools docking proposal (UNAPPLIED)

`webextras_devtools_host_docking.patch` is a lead-owned shared change proposal.
It changes no current Chromium source, maintained patch, or public hook ABI.

## Actual current-source evidence

`results/webextras/native-run4` ran both engines' eleven measured operations.
Build/package and all twelve owned source fingerprints are verified before and
through the run. Original console-open reports display mode3 after public return;
ours reports mode4 before return and a second display appears later in full raw.
Other ten action observations agree. This is not overall fixture parity: explicit
shared fixture comparison support is pending and the existing projection has
known helper/depth limitations.

Current C156 `DevToolsWindow::Create` strips `can_dock` because Arc-hosted tabs do
not have a Chrome BrowserWindowInterface/DevtoolsUIController. `ScheduleShow`
then takes its `!can_dock_` branch and calls Show immediately with undocked state;
OnLoadCompleted later shows again. This is an actual source path explaining the
observed early first callback, rather than a reason to alter the recorded mode.
`CloseWindow` also has no browser to close on its undocked branch.

## Proposed behavior

Preserve the caller's requested `can_dock` ONLY when both `arcmod-host-app` and an
active existing DevToolsShowHook identify an app-hosted frontend. Arc provides
the docking host. Other callers still require the ordinary native docking
controller. This never turns an explicitly false `can_dock` true, so worker and
explicitly undocked-only openings retain their native restriction. Android's
separate no-docking branch is unchanged.

With requested docking preserved, existing native frontend docking/load handshake
chooses the actual state and drives Show. This does not force right docking or
remap undocked to a docked mode. Current DevToolsSettings owns the real left,
right, bottom and undocked preferences. No new callback is injected here.

The owned adapter separately needs the recovered native display contract:
undocked1/left2/right3/bottom4/unknown0; its old Boolean3/4 mapping is wrong.
Original producer4359388 ->4359648 posts display via InvokeAsync unconditionally,
with a retained delegate bridge and captured mode; frontend-load scheduling alone
does not replace that queue boundary. See webextras_native_notes.md for exact
addresses, nil/nonnil delegate semantics and remaining lifetime limitations.

## Integration and verification

Apply together with the existing `webextras_devtools_hooks.patch` lifetime guards
and the owned async display/mode fix, then run a new paired native probe and the
accepted devtools scenario. The two shared patches apply in either order to the
current tree; this proposal does not rewrite the old shared hook signature.
Lead must include adopted changes in maintained patch0004.

Strict apply check passed. Both proposals were composed in temporary source
copies, mapped with `use-external-names:false` VFS overlay, then the full patched
`devtools_window.cc` passed its freshly queried real Chromium compiler command:
exit0, no diagnostics. Evidence logs/wp/webextras.devtools-host-docking.check.json
and .log. No shared source edit, build, app launch or lock was performed.
Runtime behavior, non-default docking preferences and native UI still await
integration. This proposal does not claim that mode3 may be hardcoded.
