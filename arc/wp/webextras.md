# Work package "webextras": web contents sub-objects
Read `COMMON.md` first. Depends on "core": each sub-object is created once per ArcWebContents; core's getters
return cached shell instances until you take over (ask core, via Requests, to construct yours with your
initializer, or add the getter in a category if core leaves it to you).
Classes: ArcWebContentsDevTools, ArcBrowserContextDevTools (+ DevTools protocol command/event value protocols),
ArcWebContentsMediaSession, ArcWebContentsMediaSessionPiPController, ArcMediaPosition, ArcWebContentsPDFController,
ArcWebContentsThumbnailsGenerator, ArcWebContentsBoostIDE, ArcAccessibilityActionData, ArcAccessibilityTreeNode,
ArcBrowserSidePanel, ArcBrowserSidePanelEntryInfo, ArcCookie only if core/profile didn't. Drive
ArcWebContentsDevToolsDelegate, ArcWebContentsMediaSessionDelegate (incl. native PiP UI),
ArcWebContentsThumbnailsGeneratorObserver, ArcWebContentsBoostIDEDelegate, ArcBrowserSidePanelDelegate.
DevTools: error-out ABI and callback (message,nil)/(nil,NSError) per the report; 8 MiB guards.
Files: `arc/api/arc_webextras_*.mm`, `arc/browser/webextras_*`, `arc/wp/webextras.gni`.
Acceptance: diff clean in the "devtools", "media", "pdf" scenarios and launch-slice.
