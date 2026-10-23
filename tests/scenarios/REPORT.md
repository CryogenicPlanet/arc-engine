# Scenarios work-package report

Updated 2026-10-23T09:51:45+00:00.

18 accepted named original-engine goldens, each with all steps passing and nonempty hooked recorder transcripts. This establishes scenario acceptance, not replacement-engine parity. Work is blocked on native UI coverage by the locked macOS desktop, with separate replacement-engine failures below.

## 1. Implemented files

- `tests/launch-slice/run.ts`: named scenario loading; preserved default steps and CLI flags; disposable-agent identity checks; PID-verified cleanup; native evidence capture; per-launch record validation and nonzero failure status.
- `tests/scenarios/launch-slice.ts` and `launch-slice-regression.ts`: original two-launch navigate/store/quit/relaunch/readback sequence, plus an immutable-baseline regression alias.
- `fixture-server.ts`: independent loopback fixture process so synchronous CDP commands cannot block reload/redirect responses; parent-death cleanup.
- `native.ts`: fresh observed native-control matching, scoped actions, and shared launch/quit helpers using ArcMod NativeController.
- `record-golden.ts`: serialized fresh-profile recording; candidate diagnostics; canonical publication only after all-pass + recorder validation; refuses the original launch-slice name.
- `coverage.ts`: exact class/selector/call counts, separating metadata engine methods from callbacks. Counts include startup/internal calls and do not imply entire class behavior coverage.
- `check-file.ts`: Bun syntax/transpilation check. A separate strict TypeScript 5.9.3 check with cached Bun declarations also passes.
- Scenario `.ts` files are listed below; `extension-fixture/**` provides local MV3 content/background/popup fixtures.

No engine classes were edited. Lead grant Chirp #133436 explicitly authorized sibling `../arc/tools/agent-native.ts` and `.swift`: `perform` invokes only advertised actions on a fresh fingerprint-validated enabled element; handles are bound to the originating PID; AXWindows traversal uses CFEqual identity checks. Native receipts retain foreground IDs. The stdio MCP schema remains press/set. The two-launch isolation golden proves restart rejection; unadvertised-action/navigation remains unaccepted while window controls are unavailable. No commits/pushes or VM use. The original `arc-1.170-launch-slice` golden was not modified.

## Accepted golden coverage

Engine counts use runtime TSV class identities and distinguish direction and +/- kind. Full exact selector names/call counts are linked per scenario. Counts include startup and internal calls; class presence alone does not establish complete feature coverage.

| Scenario file | Launches | Engine classes | Engine selectors | Callback selectors | Evidence |
| --- | ---: | ---: | ---: | ---: | --- |
| `bookmarks-import.ts` | 1 | 35 | 213 | 20 | [coverage](../../results/golden/arc-1.170-bookmarks-import/coverage.json) |
| `command-bar-autocomplete.ts` | 1 | 38 | 229 | 20 | [coverage](../../results/golden/arc-1.170-command-bar-autocomplete/coverage.json) |
| `context-menus.ts` | 1 | 38 | 221 | 21 | [coverage](../../results/golden/arc-1.170-context-menus/coverage.json) |
| `cookies-storage-site-settings.ts` | 1 | 38 | 225 | 25 | [coverage](../../results/golden/arc-1.170-cookies-storage-site-settings/coverage.json) |
| `devtools.ts` | 2 | 35 | 221 | 28 | [coverage](../../results/golden/arc-1.170-devtools/coverage.json) |
| `dialogs.ts` | 1 | 36 | 221 | 23 | [coverage](../../results/golden/arc-1.170-dialogs/coverage.json) |
| `dialogs-auth-cancel.ts` | 1 | 36 | 217 | 25 | [coverage](../../results/golden/arc-1.170-dialogs-auth-cancel/coverage.json) |
| `dialogs-beforeunload.ts` | 1 | 37 | 225 | 22 | [coverage](../../results/golden/arc-1.170-dialogs-beforeunload/coverage.json) |
| `downloads.ts` | 1 | 37 | 231 | 24 | [coverage](../../results/golden/arc-1.170-downloads/coverage.json) |
| `fullscreen.ts` | 1 | 36 | 220 | 23 | [coverage](../../results/golden/arc-1.170-fullscreen/coverage.json) |
| `launch-slice-regression.ts` | 2 | 37 | 224 | 27 | [coverage](../../results/golden/arc-1.170-launch-slice-regression/coverage.json) |
| `media.ts` | 1 | 35 | 212 | 20 | [coverage](../../results/golden/arc-1.170-media/coverage.json) |
| `media-pip.ts` | 1 | 38 | 228 | 32 | [coverage](../../results/golden/arc-1.170-media-pip/coverage.json) |
| `native-action-isolation.ts` | 2 | 35 | 214 | 20 | [coverage](../../results/golden/arc-1.170-native-action-isolation/coverage.json) |
| `pdf.ts` | 1 | 36 | 218 | 23 | [coverage](../../results/golden/arc-1.170-pdf/coverage.json) |
| `permissions-deny.ts` | 1 | 37 | 219 | 22 | [coverage](../../results/golden/arc-1.170-permissions-deny/coverage.json) |
| `tabs-navigation.ts` | 1 | 36 | 218 | 24 | [coverage](../../results/golden/arc-1.170-tabs-navigation/coverage.json) |
| `windows.ts` | 1 | 35 | 214 | 20 | [coverage](../../results/golden/arc-1.170-windows/coverage.json) |

### Observed engine classes per accepted scenario

**bookmarks-import:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**command-bar-autocomplete:** ArcAutocompleteMatch (8 selectors), ArcAutocompleteMatchAdditionalData (3 selectors), ArcAutocompleteResult (3 selectors), ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (7 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**context-menus:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (8 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcContextMenuElementInfo (1 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcMenuModel (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (66 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**cookies-storage-site-settings:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcDisplayNameOrOrigin (3 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcNavigationAction (5 selectors), ArcPerformanceController (1 selectors), ArcPermissionRequest (3 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (66 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**devtools:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (7 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (66 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (6 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**dialogs:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (8 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (68 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**dialogs-auth-cancel:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors), ArcWebLoginDialog (4 selectors).

**dialogs-beforeunload:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (8 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcNavigationAction (5 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (67 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**downloads:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcDownloadItem (12 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcNavigationAction (5 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (66 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**fullscreen:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (8 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (67 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**launch-slice-regression:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (10 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcNavigationAction (5 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (2 selectors), ArcWebContentsThumbnailsGenerator (2 selectors).

**media:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (20 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**media-pip:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (8 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcMediaPosition (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (68 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (5 selectors), ArcWebContentsMediaSessionPiPController (3 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**native-action-isolation:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**pdf:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcSiteSettingInfo (2 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (2 selectors), ArcWebContentsThumbnailsGenerator (2 selectors).

**permissions-deny:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcDisplayNameOrOrigin (3 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcPermissionRequest (3 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**tabs-navigation:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (20 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (8 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcNavigationAction (5 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (2 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

**windows:** ArcBackForwardList (1 selectors), ArcBackForwardListItem (1 selectors), ArcBrowser (6 selectors), ArcBrowserApplication (21 selectors), ArcBrowserApplicationConfiguration (10 selectors), ArcBrowserContext (9 selectors), ArcBrowserContextAttributes (4 selectors), ArcBrowserContextDownloads (3 selectors), ArcBrowserContextExtensions (3 selectors), ArcBrowserContextHistory (3 selectors), ArcBrowserContextID (3 selectors), ArcBrowserContextPreferences (9 selectors), ArcBrowserCreateParams (1 selectors), ArcBrowserSidePanel (2 selectors), ArcColor (4 selectors), ArcColor4f (4 selectors), ArcCpuInfo (26 selectors), ArcCrashKeysReporter (1 selectors), ArcExtensionInfo (1 selectors), ArcImageRep (4 selectors), ArcImportSourceProfile (3 selectors), ArcLocalState (1 selectors), ArcPerformanceController (1 selectors), ArcSearchEngine (8 selectors), ArcSecureContext (2 selectors), ArcSessionID (1 selectors), ArcTracingController (1 selectors), ArcURLLoadCancellationToken (1 selectors), ArcURLLoadConfiguration (8 selectors), ArcWebContents (65 selectors), ArcWebContentsBoostIDE (2 selectors), ArcWebContentsDevTools (1 selectors), ArcWebContentsMediaSession (2 selectors), ArcWebContentsNavigationController (1 selectors), ArcWebContentsThumbnailsGenerator (1 selectors).

## 2. Differential status

[DIFF-STATUS.md](DIFF-STATUS.md) preserves command/step/build evidence and timestamps. `diff-run.sh` may return zero after failed steps; inspect steps/coverage independently. Shared paths can be overwritten by other workers. Current first two lines and steps are captured below, including the default launch-slice baseline comparison.

### bookmarks-import

No completed diff file available.

### command-bar-autocomplete

Steps: launch=pass (native ready, CDP target list available); scenario actions=inconclusive (Error: command bar field not exposed); quit=fail (SIGTERM (Browser.close did not exit in 15 s); browser + 7 helpers exited).

`results/ours/command-bar-autocomplete/diff-launch-1.txt`
```
methods differing: 125 (missing 35, extra 32, returns 13, callback args 1, counts 44; internal-only 12)
callback sequence: {"golden": 47, "ours": 74, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

### context-menus

No completed diff file available.

### cookies-storage-site-settings

No completed diff file available.

### devtools

Steps: launch=pass (native ready, CDP target list available); scenario actions=inconclusive (Error: expected one native control; observed 0); quit=fail (SIGTERM (Browser.close did not exit in 15 s); browser + 7 helpers exited); launch=pass (native ready, CDP target list available); scenario actions=inconclusive (Error: expected one native control; observed 0); quit=fail (SIGTERM (Browser.close did not exit in 15 s); browser + 6 helpers exited).

`results/ours/devtools/diff-launch-1.txt`
```
methods differing: 133 (missing 30, extra 32, returns 15, callback args 1, counts 55; internal-only 11)
callback sequence: {"golden": 56, "ours": 1032, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

`results/ours/devtools/diff-launch-2.txt`
```
methods differing: 144 (missing 33, extra 31, returns 19, callback args 1, counts 60; internal-only 10)
callback sequence: {"golden": 58, "ours": 1017, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

### dialogs

No completed diff file available.

### dialogs-auth-cancel

No completed diff file available.

### dialogs-beforeunload

No completed diff file available.

### downloads

No completed diff file available.

### fullscreen

No completed diff file available.

### launch-slice

Steps: S1 launch=pass (native ready, CDP target list available); S2 open A=pass (title=launch-slice A); S3 navigate=pass (B=true (71 ms) frag=true back=true title=launch-slice B href=http://127.0.0.1:60161/b); S4 store=pass (ok); S5 close + new tab=pass; S6 quit=fail (SIGTERM (Browser.close did not exit in 15 s); browser + 8 helpers exited); S7 relaunch=pass (native ready, CDP target list available); S7 readback=pass ({"cookie":"ls_cookie=c0ffee","local":"v-41","idb":"idb-93"}); S8 quit=fail (SIGTERM (Browser.close did not exit in 15 s); browser + 6 helpers exited).

`results/ours/launch-slice/diff-launch-1.txt`
```
methods differing: 172 (missing 21, extra 36, returns 17, callback args 1, counts 97; internal-only 10)
callback sequence: {"golden": 106, "ours": 3598, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObse
```

`results/ours/launch-slice/diff-launch-2.txt`
```
methods differing: 95 (missing 22, extra 33, returns 16, callback args 1, counts 23; internal-only 10)
callback sequence: {"golden": 46, "ours": 1034, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

### launch-slice-regression

No completed diff file available.

### media

No completed diff file available.

### media-pip

No completed diff file available.

### native-action-isolation

No completed diff file available.

### pdf

No completed diff file available.

### permissions-deny

No completed diff file available.

### tabs-navigation

Steps: T1 launch=inconclusive (no readiness record for this pid within 60 s); T11 quit=fail (SIGTERM (Browser.close failed); browser + 0 helpers exited).

`results/ours/tabs-navigation/diff-launch-1.txt`
```
methods differing: 190 (missing 139, extra 21, returns 6, callback args 1, counts 23; internal-only 19)
callback sequence: {"golden": 163, "ours": 88, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserv
```

### windows

No completed diff file available.

Fresh worker-owned `arc/tools/diff-run.sh tabs-navigation` attempt completed at 09:51:47 UTC with exit1 before packaging/launch: foreign `arc/api/arc_web_contents.mm:450:21` unused variable `wrapper` (-Werror), both automatic build attempts. It produced no new navigation diff. Current files above belong to previous shared runs.

Engine method/callback differences are owned by implementation packages. Earlier worker navigation run passed T1-T10 and failed quit; current shared outputs and historical outcomes are distinguished in DIFF-STATUS.md. No replacement-engine scenario is declared diff-clean. Native control absence under foreground loginwindow must not be attributed to a missing engine feature.

## Uncovered or nonaccepted probes

The key current native blocker is independently observed: command-bar-navigation candidate `native-1.json` reports one AXWindows entry with role AXApplication and zero AXWindow controls; `native-21.json` reports two such entries after New Window. The foreground PID173 is macOS loginwindow (`ps -p 173`), consistent with locked-session AX suppression. Earlier accepted native goldens exposed proper AXWindow roles. This is not proof the engine has no windows. No unlock, activation, synthesized input, or selector fallback was attempted. Chirp #134458 corrects the earlier activation request; that request is withdrawn.

| Probe | Remaining assertion / verified limit |
| --- | --- |
| downloads-prompt | Earlier native Save sheet and Cancel worked; SaveAs omits hidden .txt extension. Fixed assertion now checks exact filename stem plus isolated directory. Rerun blocked by desktop/serialized app availability; no canonical golden. |
| command-bar-navigation | Advertised AXConfirm and unadvertised-action rejection implemented; latest run inconclusive because no native agent window exposed after New Window. |
| tabs-restore | Custom advertised sidebar press implemented, exact selected URL required before Archive/Restore; latest live validation blocked by absent native window controls. Navigation replacement is not restore. |
| history-autocomplete | Native typed visit, close visited target, exact URL suggestion and recorder HistoryURL provider/deletable provenance required. No accepted run yet; old title assertion was invalid because history title is not exposed. |
| history-search | Actual chrome://history/ DOM must contain the visited title before clear and omit it afterward; native AXRow absence is not accepted evidence. Native control coverage remains blocked. |
| find-in-page, zoom, pane-hosting | Require exact selected native fixture tab and observed actions. Retain candidates; absent window controls blocks execution. Zoom uses fixed 100%=DPR2 and 110%=2.2 expectations on this Retina fixture machine. |
| dialogs-auth | Auth cancellation accepted separately. Successful submission unaccepted: earlier two attempts never matched fixture Authorization; secure field AXConfirm now implemented but live rerun blocked. |
| extensions | CDP load/content-script/chrome.tabs.query pass. tabGroups.query never completes and observed toolbar button advertises no press action, so popup/action/group behavior remains unaccepted. CDP loading does not establish app-level install selector behavior. |

Additional limits: windows covers intentional Little Arc-to-full-window redirect, not a true NSPanel. bookmarks-import opens/cancels Chrome/Safari source wizard without importing user data. DevTools inspect asserts frontend opening, not selected DOM node. No AI class was observed in these accepted transcripts; that is only a coverage lower bound, not proof an account-free AI route is unreachable. No claim of printing/DRM/every selector coverage. Failed candidates are never goldens.

## 3. Requests to lead

- Restore an ordinary unlocked desktop session before native UI goldens; rerun downloads-prompt, command-bar-navigation, tabs-restore, history-autocomplete/search, find, zoom, pane and auth success. No credential/unlock request is made to this worker. Locked-session evidence posted #134458.
- Resolve engine startup/build/quit failures recorded in DIFF-STATUS.md, then run accepted scenarios. Earlier `tryToTerminate:` unimplemented is historical; current app worker reports a separate shutdown wait, so diagnose current logs rather than assuming the old stub persists.
- Make `arc/tools/diff-run.sh` propagate harness pipeline failure and gate acceptance on steps/coverage. Its successful diff generation currently can hide failed launches/quit.
- Authorized sibling native edits are ready for lead integration; no additional native activation grant is needed or requested. No shared engine or Chromium source was edited.

## 4. Architecture/spec findings

No architecture contract changed and no proven static-spec contradiction is asserted. Operational findings: separate-process fixtures avoid sync CDP reload/redirect deadlocks; PDF content is inside a closed-shadow MIME iframe; native DevTools frontend is not an ordinary CDP page target; Chromium suppresses notification prompts after denial until trusted navigation (`permission_request_manager.cc:725`). Golden permission expectations are fixed denied/granted values.

Verification: all owned TS/JS syntax checks, strict TypeScript 5.9.3 and sibling Swift typecheck pass. No commits/pushes, VM actions, or real Arc profile changes. Original launch-slice golden remains untouched by this worker. The queued final native batch was canceled while still waiting for a foreign-owned lock after the desktop blocker was identified, preserving the 18 accepted goldens.

Coordination used the [Chirp skill](/Users/cryogenicplanet/.agents/skills/chirp/SKILL.md).
