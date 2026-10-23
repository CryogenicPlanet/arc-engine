# Work package "extensions"
Read `COMMON.md` first. Depends on "core" (ArcBrowser = Chrome Browser; tab strip holds tabs).
Classes: ArcBrowserContextExtensions, ArcExtensionInfo, ArcExtensionCommand, ArcExtensionRuntimeError,
ArcExtensionRuntimeErrorStackFrame, ArcExtensionToolbarAction. On ArcBrowser (category): executeExtensionAction:,
pinExtension:, unpinExtension:, showExtensionContextMenu:, moveExtension:targetIndex:,
triggerExtensionOmniboxInput:inputText:disposition:. Drive ArcBrowserContextExtensionsDelegate (lifecycle,
errors, pin order, readiness) and the extension parts of ArcBrowserDelegate (popup open/resize/close with the
popup's ArcWebContents, toolbar action updates, permission requests, tab group API calls forwarded to the app
and their completions, extensionGroupIdForTabWithSessionId:). Web Store installs are unattended (Arc suppresses
the install prompt; specs/ArcBrowserContextExtensions.md and the report section "Web Store install").
Toolbar pin order persists through Chrome's ToolbarActionsModel prefs.
Files: `arc/api/arc_extensions_*.mm`, `arc/browser/extensions_*`, `arc/wp/extensions.gni`.
Acceptance: diff clean in the "extensions" scenario and launch-slice (extension toolbar actions at startup).
