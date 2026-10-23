# Work package "ui": web UI requests from pages (dialogs, permissions, auth, menus, notifications)
Read `COMMON.md` first. Depends on "core" (ArcWebContents, its WebContentsDelegate): use the accessors core adds
to `arc_internal.h`; if core's delegate needs a hook to reach you, request it from core via your report.
Classes: ArcWebDialog, ArcWebLoginDialog, ArcPermissionRequest, ArcConfirmInfobar, ArcHungRendererDialog,
ArcExternalProtocolDialog, ArcDevToolsConnectionDialog, ArcContextMenuElementInfo, ArcFormControlInfo,
ArcSelectOption, ArcMenuModel, ArcMenuItemModel, ArcNotification, ArcNotificationButton, ArcNotificationItem.
Drive: ArcWebContentsUIDelegate (JavaScript alert/confirm/prompt/beforeunload with their typed completions,
permission prompts with response 0-2 (others map to 3) and quiet reasons 1-5, HTTP auth, file chooser, color/date
pickers if present, context menus, select popups, infobars, hung renderer, external protocol, downloads
filename selection only if UI-delegate driven), ArcNotificationCenter (engine asks the app to show/close web
notifications), ArcBrowserContextDelegate DevTools/extension prompts. Chrome-side hooks: JavaScriptDialogManager,
PermissionPrompt factory for the host app, LoginHandler, context menu, etc. If a Chrome hook can't be reached
without a Chromium change, describe the minimal --arcmod-host-app-gated patch in Requests.
Files: `arc/api/arc_ui_*.mm`, `arc/browser/ui_*` (C++), `arc/wp/ui.gni`.
Acceptance: diff clean for these classes and delegate callbacks in the "dialogs", "cookies-storage-site-settings"
and related scenarios recorded by the scenarios package.
