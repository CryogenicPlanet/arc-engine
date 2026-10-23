# Work package "sessions": session restore
Read `COMMON.md` first. Depends on "core". Classes: ArcSessionTab, ArcSessionWindow, ArcSessionNavigation,
ArcSessionTabGroup, ArcSessionID is core's. Covers ArcBrowser canRestoreWebContentsWithSessionId:,
restoreWebContentsWithSessionId:, restoreForeignTab:, ArcBrowserApplication restoreBrowser:... (coordinate with
core), browser:didRestoreWebContents:withSessionId:, didCloseBrowser:...withRestorationSessionID:, the
configuration's sessionRestorationPolicy, tab discarding/restoring with back/forward history (Chrome
SessionService/TabRestoreService). Files: `arc/api/arc_sessions_*.mm`, `arc/wp/sessions.gni`.
Acceptance: diff clean in a restart-with-tabs scenario and close/reopen-tab scenario.
