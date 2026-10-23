# Work package "core": browser, window, web contents, views, navigation

Priority: highest; other packages depend on it. Read `COMMON.md` first.

Classes (primary @implementation, all ObjC-typed methods in the metadata): ArcBrowser, ArcBrowserCreateParams,
ArcWebContents (everything except the sub-objects listed below), ArcWebContentsConfiguration,
ArcWebContentsNavigationController, ArcNavigation, ArcNavigationAction, ArcBackForwardList,
ArcBackForwardListItem, ArcSessionID, ArcRedirectEntry, ArcSecureContext, ArcKeyEventData, ArcKeyboardAccelerator,
ArcOverscrollBehavior, ArcOverscrollParams, ArcFindResult, ArcFindTextWithDirectiveResult, ArcTabGroupInfo,
ArcTabGroupProperties, ArcTabGroupResult. Also on ArcBrowserApplication (as a category in your file):
createBrowser:withDelegate:forWindow:, createPopupBrowser:withDelegate:, restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:.
On ArcBrowserContext (category): createWebContents:.

ArcWebContents sub-objects owned by later packages: devTools, mediaSession, thumbnailsGenerator, boostIDE,
pdfController, aiAgent and similar getters. Until then return the one per-WebContents instance of that class
(alloc/init of the shell class, cached) so the app gets a non-nil object; the later package replaces it.

Design (see ARCHITECTURE.md): ArcBrowser wraps a Chrome `Browser` whose BrowserWindow is
`arc::ArcBrowserWindow`; Chrome's tab strip holds the browser's WebContents. ArcWebContents wraps
`content::WebContents` with identity via WebContentsUserData, attaches Chrome's tab helpers, and translates
WebContentsDelegate/Observer events to Arc's delegates and its own observable setters (the golden shows the
engine calling setTitle:, setUrl:, setIsLoading:, setLoadProgress:, setCanGoBack:, ... on itself). nativeView,
createPaneHostView, attachToPaneHostView:, detachFromPaneHostView: per ARCHITECTURE.md. Protocol specs for the
delegates you drive: `research/2026-10-22-sol/evidence/agents/protocols/initial/protocol-specs/` (ArcBrowserDelegate,
ArcWebContentsNavigationDelegate (synchronous policy: return value range-checked 0-11), ArcWebContentsNavigationObserver,
ArcWebContentsUIDelegate (implement the parts core needs: content creation, title/favicon; dialogs and
permissions go to a later package, keep their call sites as TODO-free stubs that do nothing), ArcWebContentsTopBandColorDelegate).

Files you own: `arc/api/arc_browser.mm`, `arc/api/arc_web_contents.mm`, `arc/api/arc_navigation.mm`,
anything under `arc/browser/`, `arc/wp/core.gni`, and a section "// ---- core" you may append to
`arc/api/arc_internal.h` exposing what other packages need (e.g. `+[ArcWebContents wrapperForWebContents:]`,
`-[ArcWebContents webContents]`, `-[ArcBrowser browser]`). Add needed gn deps in core.gni.

Acceptance: `arc/tools/diff-run.sh` (launch-slice): all 9 steps pass on our engine, and no missing/return/callback
differences remain for your classes. Then the scenarios the "scenarios" package adds for tabs, navigation, windows
and pane hosting (`arc/tools/diff-run.sh <scenario>` once they exist in `results/golden/`).
