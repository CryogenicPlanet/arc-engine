# Can CEF host Arc's app layer? First-pass mapping of ArcCore 1.170.0 onto CEF 156

Written 2026-10-22. This is a first pass made from metadata, headers and selector scans. Nothing was built or run.
Companion data: `results/cef-mapping.tsv` (one row per class).

## Answer

**No, not at parity.** A shim on prebuilt CEF could host a reduced Arc, but on macOS it can't
host the Arc we have. Recommendation: **compile Chromium with our own `arc/` layer**. Use CEF's
BSD-licensed source as reference code for the embedding plumbing, not as the runtime.

- **Mac limit.** CEF on macOS can't put a Chrome-style browser inside an NSView the app owns. Passing
  `parent_view` forces Alloy style (`internal/cef_types_mac.h:151-153`). Arc hosts every tab in its own
  AppKit views, so on CEF every tab would be Alloy style. CEF's docs list Chrome-style-only features:
  "datalist/autofill, extensions, gamepad, webhid, webmidi", plus Chrome's dialogs for device selection,
  permissions and settings.
- **Chrome services are unreachable.** libcef exports only the CEF C API, so Chrome's browser-layer
  services can't be called from our layer even though they run in-process: extensions, history, omnibox,
  bookmarks, search engines, password manager, autofill, sessions, translate, media session and PDF.
- **Coverage.** Of the 146 classes, 17 map directly to CEF, 44 partly, 54 need Chrome internals and 31 are
  Arc-only. For the 63 classes the app imports, the split is 8 / 15 / 23 / 17. Weighted by method, about
  half of the surface is reachable through CEF or a workaround in our layer.

## 0. Method, sources, conventions

**Arc side**
- Classes and selectors come from `analysis/data/dobjc170.txt` (dyld_info), including the `(Extensions)`
  categories. Those categories hold much of the real API, e.g. `ArcWebContents nativeView`,
  `ArcBrowserContextHistory searchHistory:completion:` and all of `ArcAIService`.
  - Counting every implementation gives 1,964 methods across the 146 classes and 834 for the 63 imported
    classes, which matches ENGINE.md §2.
  - This document counts **1,701 distinct selectors** (725 for imported classes), leaving out
    `.cxx_construct`, `.cxx_destruct` and `dealloc`.
- **App references** are a heuristic. For each class, I counted how many of its selectors appear in the
  1.168.0 app decompile (`~/general/arc/ghidra/decomp`) as an `objc_stub::` name or a selector string. Of
  1,701 selectors, 935 appear. Generic names (`url`, `title`, `init`) inflate the count. A 0 suggests the
  app never calls the class directly: `ArcSpake2Context`, `ArcSessionTab`, `ArcSessionWindow`,
  `ArcHungRendererDialog`, `ArcExtensionCommand`, `ArcPasteAndGoClassification`, `ArcReference`,
  `ArcBookmarkImportTargets`, `ArcFindTextWithDirectiveResult`, `ArcBrowserSidePanelEntryInfo`,
  `ArcMemoryDumpProvider`.
- **Callbacks into the app.** The app's class dump (`~/general/arc/dump/objc-classes.h`) declares
  **26 `Arc*` delegate protocols with 200 distinct callbacks**. These are the engine→app half of the
  interface, and dyld_info on ArcCore doesn't show them. They are mapped in §3.
- Purposes are inferred from selectors, `results/arc-files-1.170.0.txt` and Chromium naming. Treat each
  one-liner as a strong guess unless it is obvious.

**CEF side**
- I used the headers from `chromiumembedded/cef`, **branch 8078, commit `426920c3`** (2026-10-22, "Update
  to Chromium version 156.0.8078.48"). That is the same Chromium version as Arc 1.170.0's engine
  (`CHROMIUM_BUILD_COMPATIBILITY.txt`: `refs/tags/156.0.8078.48`).
- They are cached in `analysis/cef-headers/` (see `SOURCE.txt`).
- `include/` is byte-identical to commit `47dbc1df`. That commit built the newest prebuilt listed in
  `cef-builds.spotifycdn.com/index.json`: **`cef_binary_156.0.7+g47dbc1d+chromium-156.0.8078.17`**
  (macosarm64, stable, 2026-10-22). No prebuilt for `.48` was listed yet.
- The newest versioned API in these headers is 15600 (`CEF_API_ADDED(15600)`, e.g. `cef_preference.h:68`).
- Citations are `header:line`, relative to `analysis/cef-headers/`.
- Statements about CEF that aren't in the headers come from the CEF architecture doc
  (`chromiumembedded.github.io/cef/architecture`, fetched 2026-10-22) or are marked *(guess)*.

**Classifications**

| Label | Meaning |
|---|---|
| **CEF** (CEF public API) | CEF covers it directly. |
| Partial (CEF partial / workaround) | CEF covers part, and the rest is doable in our layer: own code, the in-process DevTools protocol (CDP) through `ExecuteDevToolsMethod` (`cef_browser.h:649`), or renderer code through `CefRenderProcessHandler`. |
| **Chrome** (Needs Chrome internals) | Needs services that CEF doesn't expose. For the delegate callbacks in §3 this also covers content-layer events CEF hides. A workaround may exist, and is noted. |
| Arc-only (Arc-only logic) | Arc's own feature. Either route has to reimplement it. |

- Engine-independent utilities, such as `ArcCpuInfo` and `ArcSpake2Context`, count as Partial, because
  CEF has nothing to offer but our own code is trivial.
- Value types take the classification of the subsystem they belong to.

## 1. The deciding constraint: runtime style on macOS

CEF has two per-browser runtime styles (`internal/cef_types_runtime.h:39-58`):

- **Chrome style.** "provides the full Chrome UI and browser functionality".
- **Alloy style.** "provides less default browser functionality but adds additional client callbacks
  and support for windowless (off-screen) rendering".
- **Mixing them.** Both run on the Chrome bootstrap. The Alloy bootstrap was removed in M128, and with
  it `include/cef_extension.h`, in CEF commit `a461a897` on 2024-07-01, "Delete Alloy bootstrap".
  A Views window can host at most one Chrome-style BrowserView (`internal/cef_types_runtime.h:54-58`).

**What macOS adds:** `CefWindowInfo.runtime_style` documents "Alloy style will always be used if
|windowless_rendering_enabled| is true or if |parent_view| is provided"
(`internal/cef_types_mac.h:151-153`). On Windows and Linux only the windowless condition applies
(`internal/cef_types_win.h:121-123`, `internal/cef_types_linux.h:147-149`).
`CefWindowInfo::SetAsChild(parent, bounds)` sets `parent_view` (`internal/cef_mac.h:90-94`).

**Why it bites Arc:**
- Arc creates its own NSWindows in Swift. It asks the engine for an NSView per tab (`ArcWebContents
  createPaneHostView`, `nativeView`, `attachToPaneHostView:`) and moves tabs between windows: Little Arc,
  split view, `ArcBrowser takeOwnershipOfWebContents:`.
- On CEF/macOS that is only possible with `parent_view`, so **every Arc tab would be Alloy style**.
- Getting Chrome style instead means a CEF Views window (`CefWindow` + `CefBrowserView`). CEF creates
  that NSWindow and allows one Chrome-style BrowserView per window. Arc's AppKit window, sidebar and
  split view don't fit.

**What Alloy style loses, from the headers:**
- Permission requests are denied or ignored by default: `cef_permission_handler.h:96-98,119-121`.
  That is fine, since we would handle them.
- Downloads are cancelled by default: `cef_client.h:104-106`, `cef_download_handler.h:111-113`.
  Also fine, since we handle them.
- Chrome commands are "Only used with Chrome style" (`cef_browser.h:1040-1051`). That rules out print
  preview, save page, task manager and similar commands through `ExecuteChromeCommand`.
- Policy management is Chrome-style only (`internal/cef_types.h:479`).
- So are the status bubble and zoom bubble (`internal/cef_types.h:731-741`).
- The DevTools popup hook is Chrome-style only (`cef_life_span_handler.h:152`).

**What Alloy style loses, from the CEF docs:** the Chrome runtime "adds implementations for many advanced
features (including datalist/autofill, extensions, gamepad, webhid, webmidi, etc.), along with standard
Chrome UI toolbars and dialogs for device selection, user permissions, settings, etc."

**The second wall:** "The libcef shared library exports a C API that isolates the user from the CEF runtime
and code base" (same doc).
- Chrome's `HistoryService`, `AutocompleteController`, `ExtensionService`, `PasswordManager` and the
  rest exist inside the CEF process.
- Our shim can't link against them, because libcef exports no Chromium symbols.
- Prebuilt CEF only helps where a `cef_*` API exists.

**Escape hatches, none of them "prebuilt CEF":**
1. Reparent a Chrome-style browser's NSView out of a hidden `CefWindow` into Arc's view hierarchy.
   This is unsupported and fragile *(guess)*.
2. Patch CEF to allow Chrome style in a native NSView parent. Windows and Linux already allow it.
   This means building CEF from source, which is a compile route.
3. Accept Alloy style and a reduced feature set.

## 2. Per-class mapping (all 146)

Columns:
- **Imp**: the 1.168.0 app imports the class by name (63 classes).
- **Sel (app)**: distinct selectors, with the number the app references by name in parentheses.
- **Class.**: classification.
- **Conf**: confidence (H, M or L).

The TSV has the same data.

### A. Application, process and diagnostics

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcBrowserApplication` | **Y** | 59 (50) | Process singleton: Chromium startup/run loop/quit, profiles, browsers, URL loader, flags, tracing, import, a11y, diagnostics | Partial | CefInitialize/CefRunMessageLoop/CefQuitMessageLoop/CefShutdown (cef_app.h:80,132,140,103); CefRequestContext::CreateContext (cef_request_context.h:121); CefURLRequest (cef_urlrequest.h:78); CefBeginTracing (cef_trace.h:83); CefTaskManager (cef_task_manager.h:60). Import, feature flags at runtime, GPU log, histograms, encryption key, enterprise ID, block lists are own code or Chrome internals; see spot check | M |
| `ArcBrowserApplicationConfiguration` | **Y** | 10 (1) | Startup config: argc/argv, profile path, app ID, production flag, session-restore policy, user-defaults suite | Partial | CefMainArgs + CefSettings.root_cache_path/cache_path (internal/cef_types.h:296,324); OnBeforeCommandLineProcessing (cef_app.h:208). sessionRestorationPolicy has no CEF equivalent (no navigation-history restore) | H |
| `ArcBrowserApplicationDebugProperty` | **Y** | 5 (2) | Name/value diagnostics row (versions, GPU, flags) shown in Arc's debug UI | Partial | cef_version_info (cef_version_info.h:60); CDP SystemInfo.getInfo via ExecuteDevToolsMethod (cef_browser.h:649). Own code; content of rows will differ | M |
| `ArcLocalState` |  | 10 (2) | Local State prefs: typed getters, change callbacks, browser policy lookup | **CEF** | CefPreferenceManager::GetGlobalPreferenceManager/GetPreference/AddPreferenceObserver (cef_preference.h:152,169,215); custom prefs via OnRegisterCustomPreferences (cef_browser_process_handler.h:78). getBrowserPolicy: no API; chrome_policy_id is Chrome-style only (internal/cef_types.h:479-487); read managed CFPreferences ourselves | H |
| `ArcTracingController` |  | 11 (9) | Start/stop/abort a Chromium trace to file, with delegate callbacks | **CEF** | CefBeginTracing/CefEndTracing + CefEndTracingCallback (cef_trace.h:83,100,52). No abort-without-writing distinct from EndTracing with empty callback | H |
| `ArcTracingConfiguration` | **Y** | 9 (4) | Trace config: categories string, file path, duration, privacy filtering | **CEF** | CefBeginTracing categories string (cef_trace.h:71-83). Privacy filtering flag has no CEF knob; duration = our timer | M |
| `ArcNetworkActivityMonitor` |  | 6 (3) | Start a NetLog capture for a duration | Partial | No runtime NetLog API; --log-net-log via OnBeforeCommandLineProcessing (cef_app.h:208) needs relaunch; CDP Network domain as substitute. Debug feature | M |
| `ArcNetworkLoggingConfiguration` |  | 8 (2) | NetLog file path + duration | Partial | As ArcNetworkActivityMonitor | M |
| `ArcCpuInfo` | **Y** | 28 (27) | base::CPU snapshot (brand, vendor, ARM part, feature bits) | Partial | None needed: sysctlbyname in our layer. Engine-independent utility | H |
| `ArcPerformanceController` | **Y** | 6 (2) | Battery Saver / Memory Saver (high-efficiency) mode control and observer | **Chrome** | None; Chrome's performance_manager user_tuning is not exposed. Alloy-style browsers are not TabStripModel tabs, so Chrome's discarding would not apply anyway (guess) | H |
| `ArcGPULogMessage` | **Y** | 6 (3) | GPU process log message (severity/header/message) | **Chrome** | None (GpuDataManager hidden); CDP SystemInfo.getInfo gives GPU info, not logs. Diagnostics; can stub | M |
| `ArcMemoryDumpMetric` |  | 4 (2) | Metric the app contributes to memory-infra dumps | **Chrome** | None (MemoryDumpManager hidden). Debug only; can stub | M |
| `ArcMemoryDumpProvider` |  | 1 (0) | App-registered memory-infra dump provider | **Chrome** | None. Debug only; can stub | M |
| `ArcCrashKeysReporter` | **Y** | 1 (1) | Static hook forwarding crash keys to Arc's Sentry observer | Arc-only | CefSetCrashKeyValue (cef_crash_util.h:143) sets keys; no observer API. Sentry integration is TBC-specific; can drop | M |
| `ArcBreadcrumb` |  | 8 (5) | Engine breadcrumb (category, message, file/line) delivered to Sentry | Arc-only | None (cef_log, internal/cef_logging_internal.h:59, is output-only). Telemetry; can drop | M |
| `ArcHistogramSample` |  | 5 (4) | One perf histogram sample with browser state flags | Arc-only | Partial data: CDP Browser.getHistograms via ExecuteDevToolsMethod (cef_browser.h:649). TBC telemetry; endpoints gone after shutdown | M |
| `ArcHistogramSamples` |  | 38 (36) | Periodic perf sample: memory footprint per process type, CPU %, wakeups, battery drain, memory-pressure windows | Arc-only | Partial: CefTaskManager::GetTaskInfo cpu/memory per process (cef_task_manager.h:89); no power/battery metrics. Arc's histograms_observer over performance_manager/power metrics | M |
| `ArcAdhocHistogramSample` | **Y** | 5 (4) | Ad-hoc histogram sample (type, value) | Arc-only | As ArcHistogramSample | M |
| `ArcAdhocHistogramSamples` |  | 2 (1) | Batch of ad-hoc samples | Arc-only | As ArcHistogramSample | M |
| `ArcURLLoadConfiguration` | **Y** | 27 (18) | App-initiated URL load (method, body/file, headers, redirects, streaming, timeout, cache, block-local) | **CEF** | CefURLRequest::Create + CefRequest (cef_urlrequest.h:78; cef_request.h:83-184). blockLocalRequest = our check | H |
| `ArcURLLoadCancellationToken` | **Y** | 3 (2) | Cancel handle for an app URL load | **CEF** | CefURLRequest::Cancel (cef_urlrequest.h:128) | H |

### B. Profile (browser context) and profile services

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcBrowserContext` | **Y** | 55 (45) | Profile: sub-object hub plus cookies, content settings, bookmarks, favicons, import, localStorage, ad-block debug, URL loads | Partial | CefRequestContext (cef_request_context.h:108): GetCookieManager 174, Get/SetContentSetting 301/321, Get/SetWebsiteSetting 267/288, RegisterSchemeHandlerFactory 191; CreateBrowser with request_context (cef_browser.h:301). Bookmarks, favicon DB, localStorage, import need own code or Chrome internals; see spot check | M |
| `ArcBrowserContextID` |  | 4 (3) | Profile identity (incognito flag, directory basename) | Partial | CefRequestContext::GetCachePath (cef_request_context.h:166); empty cache_path = incognito (internal/cef_types.h:543-552). Trivial | H |
| `ArcBrowserContextAttributes` | **Y** | 6 (3) | Profile attributes (dir, user-visible name, colour) | Partial | Own store; CefRequestContext::SetChromeColorScheme (cef_request_context.h:345) for theme only. ProfileAttributesStorage replaced by our JSON | H |
| `ArcBrowserContextPreferences` |  | 40 (26) | Profile prefs: downloads dir/prompt, search suggest, password/credit-card saving, notification requests, auto-open, search engines, paste-and-go | Partial | CefRequestContext is a CefPreferenceManager: Get/SetPreference/CanSetPreference/AddPreferenceObserver (cef_preference.h:169,201,190,215). Search engines (TemplateURLService) and classifyPasteAndGoInput need Chrome internals; password/card toggles are moot without those features | M |
| `ArcBrowserContextDownloads` |  | 11 (4) | Download manager per profile: delegate (created/updated/removed/filename), startDownload | Partial | CefDownloadHandler::CanDownload/OnBeforeDownload/OnDownloadUpdated (cef_download_handler.h:102,120,135); StartDownload (cef_browser.h:512). No persisted download history (downloadManagerInitializedWithItems) or remove; keep our own list | H |
| `ArcDownloadItem` | **Y** | 19 (15) | Download item: bytes, speed, path, URL, state, ETA; pause/resume/cancel/remove | Partial | CefDownloadItem getters (cef_download_item.h:54-171); CefDownloadItemCallback Pause/Resume/Cancel (cef_download_handler.h:71-83). Callback only valid inside OnDownloadUpdated; keep latest callback per id. remove = our list | H |
| `ArcBrowserContextDevTools` |  | 6 (4) | Profile-level DevTools window (for non-tab targets such as workers/extension pages) | Partial | CDP + DevTools frontend loaded in a CEF browser; remote_debugging_port (internal/cef_types.h:427). No CEF API to open DevTools for a non-browser target | L |
| `ArcBrowserContextPageContentExtraction` |  | 8 (3) | Extract structured page content (Chrome AIPageContent-style tree) for AI features | Partial | Renderer DOM walk: CefFrame::VisitDOM/CefDOMNode (cef_frame.h:226, cef_dom.h:166) or V8; or CDP DOMSnapshot/Accessibility via ExecuteDevToolsMethod (cef_browser.h:649). Chrome's blink AIPageContent agent is hidden; reimplementation will not match byte-for-byte | L |
| `ArcSiteSettingInfo` | **Y** | 11 (5) | Per-origin content setting / permission value | **CEF** | Get/SetContentSetting, AddSettingObserver (cef_request_context.h:301,321,333). Listing all changed settings for an origin = iterate the content-type enum | H |
| `ArcCookie` | **Y** | 24 (19) | Cookie value object | **CEF** | CefCookie (internal/cef_types.h:867); CefCookieManager Visit/Set/Delete/Flush (cef_cookie.h:75,99,116,126) | H |
| `ArcMetrics` |  | 3 (1) | Profile metrics accessor: all site-engagement scores | **Chrome** | Partial: per-URL SITE_ENGAGEMENT website setting via GetWebsiteSetting (cef_request_context.h:267, internal/cef_types_content_settings.h:77); no enumeration. Whether Alloy browsers feed engagement is unknown (guess: no) | M |
| `ArcSiteEngagementDetails` | **Y** | 4 (2) | Site engagement (url, score) | **Chrome** | As ArcMetrics | M |
| `ArcNotification` | **Y** | 32 (29) | Web notification bridged to the app's notification centre (click/close/settings/reply) | **Chrome** | None (no notification API); workaround: renderer polyfill of window.Notification via CefRenderProcessHandler + process messages. Polyfill misses service-worker/push notifications | M |
| `ArcNotificationButton` | **Y** | 8 (6) | Notification action button | **Chrome** | As ArcNotification | M |
| `ArcNotificationItem` | **Y** | 4 (4) | Notification list item | **Chrome** | As ArcNotification | M |

### C. History, omnibox, search engines, bookmarks

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcBrowserContextHistory` |  | 27 (8) | History service: search, top sites, add/delete/expire, visits (sync-style annotated visits) | **Chrome** | None; workaround = own SQLite store fed by OnLoadEnd/OnAddressChange (cef_load_handler.h:97, cef_display_handler.h:57). Own store loses Chrome-history integrations (omnibox, chrome://history, chrome.history) | H |
| `ArcHistoryItem` | **Y** | 10 (6) | History URL row (id, url, title, visit/typed counts, times) | **Chrome** | None; own store | H |
| `ArcHistoryQueryOptions` | **Y** | 14 (10) | History query options (text, time range, max, dedupe, matching) | **Chrome** | None; own store | H |
| `ArcHistoryDeletion` |  | 11 (3) | History deletion info (all/expired/time range/urls/visit ids) | **Chrome** | None; own store | H |
| `ArcAnnotatedVisit` |  | 61 (8) | History visit with annotations/clusters/sync originator fields | **Chrome** | None. App 1.168.0 does not call the visit-level history methods (selector scan) | H |
| `ArcVisitCategory` |  | 6 (1) | Visit content category (id, weight) | **Chrome** | None | H |
| `ArcRedirectEntry` |  | 6 (2) | Redirect chain entry for visits | **Chrome** | None | H |
| `ArcMostVisitedURL` | **Y** | 5 (2) | TopSites entry | **Chrome** | None; derive from own history store | H |
| `ArcBrowserContextAutocomplete` |  | 3 (1) | Omnibox AutocompleteController with selectable providers | **Chrome** | None. Reimplementation = new product (ranking, providers, search suggest) | H |
| `ArcAutocompleteMatch` | **Y** | 25 (9) | AutocompleteMatch (destination, relevance, type, provider, inline text, keyword, ...) | **Chrome** | None | H |
| `ArcAutocompleteMatchAdditionalData` | **Y** | 6 (2) | Key/value extra data on a match | **Chrome** | None | H |
| `ArcAutocompleteResult` |  | 3 (2) | Combined and per-provider matches | **Chrome** | None | H |
| `ArcPasteAndGoClassification` |  | 4 (0) | AutocompleteClassifier result for paste-and-go | **Chrome** | None; workaround = URL heuristics (CefParseURL, cef_parser.h:60) | H |
| `ArcSearchEngine` | **Y** | 12 (9) | TemplateURL (name, keyword, URL, favicon, prepopulated) | **Chrome** | None; workaround = own engine list in our layer | H |
| `ArcBookmarkTreeNode` | **Y** | 17 (12) | BookmarkModel node (folder/url, ids, uuid, dates, children) | **Chrome** | None; workaround = own bookmark store. Arc uses it for imports; extensions' chrome.bookmarks is moot without extensions | H |
| `ArcBookmarkImportTargets` |  | 5 (0) | Bookmark-bar / other-bookmarks target nodes for import | **Chrome** | None | H |

### D. Passwords and autofill

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcLoginCredentials` |  | 5 (3) | Credential (url, username, password) from import / password store | **Chrome** | None (PasswordStore hidden) | H |
| `ArcLoginCredentialsSaveRequest` | **Y** | 6 (4) | Password-manager 'save/update password?' request (save/cancel/never) | **Chrome** | None. Blocker for password manager parity | H |
| `ArcAddressEntrySaveRequest` | **Y** | 4 (2) | Autofill 'save address?' request | **Chrome** | None | H |
| `ArcCreditCardInfoSaveRequest` | **Y** | 4 (2) | Autofill 'save card?' request | **Chrome** | None | H |

### E. Extensions, tab groups, side panel

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcBrowserContextExtensions` |  | 23 (17) | Extensions: list/enable/disable/reload/uninstall/install (Web Store, unpacked), info, commands, pinned order, run JS in background page | **Chrome** | None: no extension API in CEF 156 headers (grep: only cef_browser.h:128 mentions extension tabIds). Blocker; see summary | H |
| `ArcExtensionInfo` | **Y** | 17 (11) | Extension metadata (id, name, icon, permissions, enabled, omnibox keyword) | **Chrome** | None | H |
| `ArcExtensionCommand` | **Y** | 8 (0) | Extension keyboard command | **Chrome** | None | H |
| `ArcExtensionToolbarAction` | **Y** | 9 (6) | Extension toolbar action (icon, tooltip, enabled, pinned position) | **Chrome** | None | H |
| `ArcExtensionRuntimeError` |  | 9 (4) | Extension runtime error report | **Chrome** | None | H |
| `ArcExtensionRuntimeErrorStackFrame` |  | 6 (4) | Stack frame of an extension error | **Chrome** | None | H |
| `ArcTabGroupInfo` | **Y** | 13 (10) | chrome.tabGroups bridge: group info | **Chrome** | None (needs extension system) | H |
| `ArcTabGroupProperties` |  | 10 (7) | chrome.tabGroups bridge: properties | **Chrome** | None | H |
| `ArcTabGroupResult` | **Y** | 9 (7) | chrome.tabGroups bridge: result | **Chrome** | None | H |
| `ArcBrowserSidePanel` |  | 11 (4) | Extension side panel (chrome.sidePanel): title, web contents, close | **Chrome** | None | H |
| `ArcBrowserSidePanelEntryInfo` |  | 6 (0) | Side-panel entry for a tab | **Chrome** | None | H |

### F. Browser (window) and sessions

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcBrowser` |  | 38 (32) | Chrome Browser bound to an Arc window: tab ownership, cookies, omnibox, extension actions, session restore, side panel | **Chrome** | Partly CEF: CreateBrowser (cef_browser.h:301), CefCookieManager; rest none. Arc's model (one Browser per window, many tabs) has no CEF counterpart; see spot check | H |
| `ArcBrowserCreateParams` |  | 10 (3) | Browser::CreateParams for popups/PiP windows (title, resizable, bounds, PiP size) | Partial | OnBeforePopup windowInfo/popupFeatures (cef_life_span_handler.h:100) | M |
| `ArcSessionID` | **Y** | 7 (5) | SessionID used as tab/window identity across the API | Partial | CefBrowser::GetIdentifier (cef_browser.h:131); persistence across launches is ours | M |
| `ArcSessionWindow` |  | 9 (0) | sessions::SessionWindow (import / foreign-session restore) | **Chrome** | None; no API to restore navigation entries | H |
| `ArcSessionTab` | **Y** | 9 (0) | sessions::SessionTab | **Chrome** | None | H |
| `ArcSessionTabGroup` |  | 7 (2) | sessions::SessionTabGroup | **Chrome** | None | H |
| `ArcSessionNavigation` |  | 10 (2) | Serialized navigation entry | **Chrome** | None (GetNavigationEntries is read-only, cef_browser.h:669) | H |

### G. Web contents: core, navigation, DevTools

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcWebContents` |  | 160 (110) | Tab: view hosting, state, find, zoom, capture, print, JS, a11y, translate, effects, freezing, delegates | Partial | CefBrowser/CefBrowserHost (cef_browser.h) + CefClient handlers + CDP. See spot check §4.4: of ~109 app-facing selectors, ~24 map directly, ~68 need a workaround (CDP, renderer code, own code), ~17 need Chrome internals or are Arc-only | M |
| `ArcWebContentsConfiguration` | **Y** | 24 (12) | Creation options (foreground, hidden, muted, transparent, no downloads/context menus, autoplay, forced size) | Partial | CefWindowInfo.hidden (internal/cef_types_mac.h:110), CefBrowserSettings.background_color (internal/cef_types.h:728), SetAudioMuted (cef_browser.h:1003), CanDownload (cef_download_handler.h:102), OnBeforeContextMenu (cef_context_menu_handler.h:106). Per-tab autoplay policy: no API (global --autoplay-policy switch or AUTOPLAY content setting, internal/cef_types_content_settings.h:86) | M |
| `ArcWebContentsNavigationController` |  | 17 (12) | Load (with initiator/transition/typed flags), back/forward/go-to-item, reload, observers | Partial | LoadURL/LoadRequest (cef_frame.h:152,146), GoBack/GoForward/Reload/ReloadIgnoreCache (cef_browser.h:88,100,112,118); go-to-item via CDP Page.navigateToHistoryEntry | H |
| `ArcNavigation` | **Y** | 8 (7) | Navigation handle snapshot (url, status code, headers, redirected-to-other-tab) | Partial | OnLoadStart/End/Error (cef_load_handler.h:82,97,110); OnResourceResponse (cef_resource_request_handler.h:145). No NavigationHandle identity; correlate ourselves | M |
| `ArcNavigationAction` |  | 7 (4) | Navigation policy request (url, referrer, type, disposition) | Partial | OnBeforeBrowse (cef_request_handler.h:92), OnOpenURLFromTab (117), CefRequest::GetTransitionType (cef_request.h:213) | H |
| `ArcBackForwardList` |  | 8 (5) | Back/forward list (back/current/forward items and lists) | **CEF** | GetNavigationEntries + CefNavigationEntryVisitor (cef_browser.h:669,224) | H |
| `ArcBackForwardListItem` | **Y** | 6 (3) | Back/forward item (title, url) | **CEF** | CefNavigationEntry::GetURL/GetTitle (cef_navigation_entry.h:64,83) | H |
| `ArcWebContentsDevTools` |  | 22 (18) | Per-tab DevTools: open/close/panels, inspect, in-process CDP sessions | Partial | ShowDevTools/CloseDevTools/HasDevTools (cef_browser.h:582,591,598); ExecuteDevToolsMethod/SendDevToolsMessage/AddDevToolsMessageObserver (649,633,660). Docked-in-Arc toolbox, panel switching and confirmation prompts are not in CEF | M |
| `ArcFindResult` |  | 3 (2) | Find-in-page result (matches, active ordinal) | **CEF** | Find/StopFinding (cef_browser.h:560,569); CefFindHandler::OnFindResult (cef_find_handler.h:60) | H |
| `ArcFindTextWithDirectiveResult` |  | 5 (0) | Text-fragment (#:~:text=) find result | **Chrome** | None; workaround: navigate same-document to a text-fragment URL (guess) | L |
| `ArcErrorStatus` |  | 5 (2) | Renderer termination status + error code | **CEF** | OnRenderProcessTerminated (cef_request_handler.h:270) | H |
| `ArcSecureContext` |  | 4 (3) | Visible security state of the page | Partial | CefSSLStatus via GetVisibleNavigationEntry (cef_ssl_status.h:55-80, cef_browser.h:958). No Safe Browsing/DANGEROUS level; no change event | M |
| `ArcFreezingVoteToken` |  | 3 (1) | performance_manager freezing vote (prevent/allow freezing) | Partial | CDP Page.setWebLifecycleState via ExecuteDevToolsMethod (cef_browser.h:649). Semantics differ: no Chrome freezing policy to vote on | L |
| `ArcOverscrollBehavior` |  | 5 (2) | Page overscroll-behavior x/y (for swipe navigation) | Partial | Renderer JS/computed style via CefRenderProcessHandler (cef_render_process_handler.h:98) | L |
| `ArcOverscrollParams` |  | 3 (1) | Overscroll params wrapper | Partial | As ArcOverscrollBehavior | L |

### H. Web contents: prompts, dialogs, menus

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcPermissionRequest` | **Y** | 5 (3) | Permission prompt request (type, origin, extra data) | **CEF** | OnShowPermissionPrompt / OnRequestMediaAccessPermission / OnDismissPermissionPrompt (cef_permission_handler.h:124,103,143). Device choosers (USB/HID/Bluetooth/serial) and quiet-UI reasons not exposed (guess) | M |
| `ArcDisplayNameOrOrigin` |  | 5 (2) | Prompt subject: display name or origin | Partial | CefFormatUrlForSecurityDisplay (cef_parser.h:81). Trivial | H |
| `ArcWebLoginDialog` |  | 9 (5) | HTTP/proxy auth dialog (authority, accept with credentials, cancel) | **CEF** | CefRequestHandler::GetAuthCredentials + CefAuthCallback (cef_request_handler.h:165). Password-manager autofill of the dialog absent | H |
| `ArcExternalProtocolDialog` |  | 7 (5) | External protocol launch dialog (program name, origin, accept/cancel) | Partial | OnProtocolExecution (cef_resource_request_handler.h:203); launch via NSWorkspace | H |
| `ArcHungRendererDialog` |  | 5 (0) | Hung renderer dialog (wait, kill) | **CEF** | OnRenderProcessUnresponsive/Responsive (cef_request_handler.h:247,259); CefUnresponsiveProcessCallback Wait/Terminate (cef_unresponsive_process_callback.h:53,59) | H |
| `ArcDevToolsConnectionDialog` |  | 6 (3) | Approve/deny/disable an incoming DevTools remote-debugging connection | **Chrome** | None (remote debugging is a static port, internal/cef_types.h:427). Purpose inferred from selectors only | L |
| `ArcConfirmInfobar` |  | 4 (2) | Chrome confirm infobar (message, accept) | **Chrome** | None. Low importance | H |
| `ArcWebDialog` |  | 8 (3) | WebUI constrained dialog (title, sizes, web contents) | **Chrome** | None | H |
| `ArcContextMenuElementInfo` |  | 15 (2) | Context-menu target info (link/src/selection/misspelling/suggestions/input type) | **CEF** | CefContextMenuParams (cef_context_menu_handler.h:212-344) | H |
| `ArcMenuModel` |  | 5 (1) | ui::MenuModel wrapper (context menus, extension action menus) | Partial | CefMenuModel (cef_menu_model.h:51) in RunContextMenu (cef_context_menu_handler.h:120). Extension menu items absent | M |
| `ArcMenuItemModel` |  | 19 (6) | Menu item (type, label, state, icon, accelerator, submenu, activate) | Partial | CefMenuModel GetLabelAt/IsEnabled/IsChecked/GetSubMenuAt/GetAccelerator (cef_menu_model.h:201,292,319,261,396) | M |
| `ArcKeyboardAccelerator` |  | 3 (1) | ui::Accelerator (key, modifiers) | Partial | CefMenuModel::GetAccelerator (cef_menu_model.h:396) | H |

### I. Web contents: media, PDF, thumbnails, page content, AI agent

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcWebContentsMediaSession` |  | 30 (19) | Media controls: play/pause/next/prev/seek/volume/mute, camera/mic/hang-up, PiP request/exit, auto-PiP | **Chrome** | Only SetAudioMuted (cef_browser.h:1003) and OnMediaAccessChange (cef_display_handler.h:163); rest via injected JS (lossy) | H |
| `ArcWebContentsMediaSessionPiPController` |  | 4 (2) | Arc's custom video PiP (video NSView + source bounds) | **Chrome** | None; Arc replaced Chrome's PiP window controller | H |
| `ArcMediaImage` |  | 6 (1) | Media-session artwork | **Chrome** | None | H |
| `ArcMediaPosition` | **Y** | 6 (3) | Media-session position (rate, duration, time) | **Chrome** | None | H |
| `ArcWebContentsPDFController` |  | 7 (2) | PDF viewer: bookmarks/outline, available/visible pages, page text | **Chrome** | None. Workaround via scripting the PDF viewer frame is speculative | H |
| `ArcWebContentsThumbnailsGenerator` |  | 11 (8) | Tab thumbnails with observers | Partial | CDP Page.captureScreenshot via ExecuteDevToolsMethod (cef_browser.h:649). Cost/latency higher than Chrome's ThumbnailTabHelper (guess) | M |
| `ArcExtractedPageContent` | **Y** | 6 (2) | Extracted page content (url, title, root node, time) | Partial | As ArcBrowserContextPageContentExtraction | L |
| `ArcContentNode` |  | 15 (3) | AIPageContent node (type, text, emphasis, size, colour, link, image alt, iframe, form, table) | Partial | As ArcBrowserContextPageContentExtraction | L |
| `ArcFormControlInfo` |  | 10 (1) | Form control in extracted content (type, name, value, placeholder, checked, required, redacted) | Partial | As ArcBrowserContextPageContentExtraction | L |
| `ArcSelectOption` |  | 6 (3) | <select> option in extracted content | Partial | As ArcBrowserContextPageContentExtraction | L |
| `ArcWebContentsAIAgent` |  | 4 (2) | AI agent input into page (text input, key press) | Partial | CDP Input.insertText/dispatchKeyEvent via ExecuteDevToolsMethod (cef_browser.h:649); SendKeyEvent (751) | M |
| `ArcAccessibilityTreeNode` | **Y** | 13 (5) | AX tree node snapshot for the AI agent (role, name, value, actionable flags, child tree) | Partial | CDP Accessibility.getFullAXTree via ExecuteDevToolsMethod (cef_browser.h:649); SetAccessibilityState (987) | M |
| `ArcAccessibilityActionData` |  | 6 (2) | AX action + value to perform on a node | Partial | CDP DOM.resolveNode + Runtime.callFunctionOn / Input.* (cef_browser.h:649) | M |
| `ArcKeyEventData` | **Y** | 5 (2) | Synthetic key event (type, DOM code, key, VK) | Partial | CefKeyEvent via SendKeyEvent (cef_browser.h:751) or CDP Input.dispatchKeyEvent | M |

### J. Utilities and value types

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcImage` |  | 11 (4) | gfx::Image + sandboxed image decode helpers | Partial | CefImage AddPNG/AddJPEG/GetAsPNG (cef_image.h:97,107,171); decode via ImageIO. Loses the out-of-process (sandboxed) decoder | M |
| `ArcImageRep` | **Y** | 15 (3) | Single image representation (pixels, PNG/JPEG encode, decode) | Partial | CefImage AddBitmap/GetAsBitmap/GetAsJPEG (cef_image.h:83,156,186) | M |
| `ArcColor` | **Y** | 8 (8) | SkColor value | **CEF** | cef_color_t (internal/cef_types.h:56). Value type | H |
| `ArcColor4f` |  | 8 (5) | SkColor4f value | **CEF** | cef_color_t (internal/cef_types.h:56). Value type | H |
| `ArcURLUtilities` | **Y** | 2 (2) | eTLD+1 from host; host formatting for display | Partial | CefFormatUrlForSecurityDisplay (cef_parser.h:81); eTLD+1 needs own Public Suffix List | H |
| `ArcSpake2Context` |  | 3 (0) | BoringSSL SPAKE2 password-authenticated key exchange | Partial | None in CEF; link our own BoringSSL/SPAKE2. No app call sites found by selector scan | M |

### K. Arc-only services (AI, command bar, content conversion, import, Effects, Boost IDE)

| Class | Imp | Sel (app) | Purpose | Class. | CEF mapping / notes | Conf |
|---|:-:|--:|---|---|---|:-:|
| `ArcAIService` |  | 22 (18) | AI service: embeddings, LLM inference streaming, classifiers, Nomic embeddings, command-bar ranking/training, DataDog tracing | Arc-only | None; host the native library in our own XPC/helper process | H |
| `ArcAIServiceConfiguration` | **Y** | 11 (5) | AI service config (library path/version, env, retries, timeout) | Arc-only | None | H |
| `ArcAIInferenceModel` | **Y** | 6 (1) | Inference model descriptor | Arc-only | None | H |
| `ArcAIInferenceConfiguration` | **Y** | 6 (2) | Inference config (max tokens, stream cadence) | Arc-only | None | H |
| `ArcAIDistilBertClassifier` | **Y** | 9 (1) | DistilBERT + LoRA classifier descriptor | Arc-only | None | H |
| `ArcNomicEmbedTextModel` | **Y** | 6 (2) | Nomic text-embedding model descriptor | Arc-only | None | H |
| `ArcNomicEmbedTextItem` | **Y** | 6 (2) | Nomic embedding input (text, task) | Arc-only | None | H |
| `ArcCommandBarRankingCandidate` | **Y** | 17 (6) | Command-bar ranking candidate features (recency, frequency, typed, engagement, matches, embedding) | Arc-only | None | H |
| `ArcCommandBarRankingEvaluation` |  | 7 (3) | Ranking model evaluation result | Arc-only | None | H |
| `ArcCommandBarRankingExample` | **Y** | 7 (1) | Training example (input, candidates, selected index) | Arc-only | None | H |
| `ArcCommandBarRankingInput` | **Y** | 5 (1) | Ranking query input (query embedding) | Arc-only | None | H |
| `ArcCommandBarRankingModel` | **Y** | 6 (2) | Ranking model descriptor (directory, version) | Arc-only | None | H |
| `ArcCommandBarRankingResult` | **Y** | 6 (2) | Ranking result (candidate, score) | Arc-only | None | H |
| `ArcCommandBarRankingTensor` | **Y** | 7 (3) | Tensor (name, shape, data) | Arc-only | None | H |
| `ArcCommandBarRankingTrainingResult` |  | 16 (12) | On-device training result (losses, gradient norms, delta) | Arc-only | None | H |
| `ArcContentConversionService` |  | 6 (3) | HTML-to-Markdown conversion service backed by a TBC library | Arc-only | None; run the library in our layer | H |
| `ArcHtmlToMarkdownResult` |  | 7 (1) | Conversion result (body, title, references, selectors, meta tags) | Arc-only | None | H |
| `ArcMetaTagEntry` |  | 3 (2) | Captured meta tag | Arc-only | None | H |
| `ArcMetaTagSelector` |  | 3 (1) | Meta tag selector | Arc-only | None | H |
| `ArcReference` |  | 3 (0) | Conversion reference (text, min chars) | Arc-only | None | H |
| `ArcImportSource` | **Y** | 3 (2) | Importable browser (id, type) | Arc-only | None; reimplement importers in our layer. Arc's importer reads Chrome/Safari data | H |
| `ArcImportSourceProfile` | **Y** | 12 (9) | Importable browser profile (paths, categories, Safari export) | Arc-only | None; cookies can be written via CefCookieManager::SetCookie (cef_cookie.h:99) | H |
| `ArcBrowserContextEffects` |  | 17 (12) | Per-host-pattern injected CSS/JS, colour filter, colour scheme, zoom (Arc 'Boosts') | Arc-only | Renderer: CefRenderProcessHandler::OnContextCreated + CefV8Context::Eval (cef_render_process_handler.h:98, cef_v8.h:220), SendProcessMessage (cef_frame.h:257); zoom: SetZoomLevel (cef_browser.h:482). Arc's EffectsAgent is renderer mojo; colour-filter matrix becomes CSS filter (lossy, guess) | M |
| `ArcHSLAMatrix` | **Y** | 23 (1) | 20-float colour matrix for Effects colour filter | Arc-only | None; CSS filter approximation in renderer | M |
| `ArcWebContentsBoostIDE` |  | 8 (4) | Boost IDE WebUI <-> app JSON channel (responses/events) | Arc-only | CefMessageRouterBrowserSide/RendererSide (wrapper/cef_message_router.h:258,445) + scheme handler (cef_scheme.h:65) | M |

### C exports

| Symbol | CEF mapping | Class. |
|---|---|---|
| `_ChromeMain` | Our `ChromeMain` runs `CefExecuteProcess`, `CefInitialize` and `CefRunMessageLoop` (`cef_app.h:64,80,132`). The app's NSApplication subclass must adopt `CefAppProtocol` (`cef_application_mac.h:74`). Arc's ArcCore ships Chrome's `BrowserCrApplication`, so the shim has to supply an equivalent. | Partial |
| `_ChromeAppModeStart_v8`, `_ChromeWebAppShortcutCopierMain` | PWA app shims. CEF has nothing equivalent. Arc probably doesn't need them *(guess)*. | Chrome |
| `_SetLogMessageHandler` | There is no log-callback API, only `log_file`/`log_severity` settings (`internal/cef_types.h:370`). | Partial |
| `_emitTraceEvent`, `_arcCategoryEnabledFlag` | `cef_trace_event_*` (`internal/cef_trace_event_internal.h:51-75`). No query for whether a category is enabled. | Partial |
| `_CallWithEHFrame`, `_ArcErrorDomain` | Trivial own code. | Partial |

## 3. Callbacks into the app (26 protocols, 200 callbacks)

The app implements these, and the engine must call them. Here "Chrome" also covers content-layer events
that CEF doesn't surface.

| Protocol | Callbacks | CEF / Partial / Chrome / Arc | Mapping |
|---|--:|---|---|
| `ArcWebContentsUIDelegate` | 65 | 20 / 14 / 25 / 6 | Per-callback table in §4.4 |
| `ArcWebContentsMediaSessionDelegate` | 20 | 2 / 0 / 18 / 0 | Only camera/mic state maps, via `OnMediaAccessChange` (`cef_display_handler.h:163`) |
| `ArcBrowserDelegate` | 19 | 2 / 0 / 15 / 2 | Popups → `OnBeforePopup` (`cef_life_span_handler.h:100`); fullscreen → `OnFullscreenModeChange` (`cef_display_handler.h:87`); extension popups and toolbar, tab groups, restore: none |
| `ArcBrowserContextDelegate` | 18 | 1 / 2 / 5 / 10 | Chat, product-connection, artifact, feature-flag and Dia callbacks are Arc-only; session restore and extension prompts need Chrome |
| `ArcBrowserContextExtensionsDelegate` | 10 | 0 / 0 / 10 / 0 | None |
| `ArcBrowserApplicationDelegate` | 9 | 4 / 3 / 2 / 0 | `OnContextInitialized` (`cef_browser_process_handler.h:87`), return from `CefRunMessageLoop`; no GPU-crash or GPU-info events |
| `ArcWebContentsNavigationObserver` | 8 | 3 / 5 / 0 / 0 | §4.5 |
| `ArcImportDelegate` | 7 | 0 / 0 / 0 / 7 | Our importer |
| `ArcBrowserContextDownloadsDelegate` | 5 | 3 / 2 / 0 / 0 | §4.7 |
| `ArcURLLoadDelegate` | 5 | 4 / 1 / 0 / 0 | `CefURLRequestClient` (`cef_urlrequest.h:145-190`). Headers arrive with data or completion, not as a separate event *(guess)* |
| `ArcWebContentsDevToolsDelegate` | 5 | 1 / 2 / 2 / 0 | §4.6 |
| `ArcDevToolsProtocolCommand` + `Event` | 5 | 5 / 0 / 0 / 0 | CDP JSON through `ExecuteDevToolsMethod`/`AddDevToolsMessageObserver` (`cef_browser.h:649,660`) |
| `ArcBrowserSidePanelDelegate` | 4 | 0 / 0 / 4 / 0 | None |
| `ArcBrowserContextPreferencesObserver` | 3 | 1 / 0 / 2 / 0 | `AddPreferenceObserver` (`cef_preference.h:215`); the search-engine events need Chrome |
| `ArcTracingControllerDelegate` | 3 | 3 / 0 / 0 / 0 | `CefEndTracingCallback` (`cef_trace.h:52`) |
| `ArcWebContentsBoostIDEDelegate` | 3 | 0 / 0 / 0 / 3 | `CefMessageRouter` (`wrapper/cef_message_router.h:258`) |
| `ArcNotificationCenter` | 2 | 0 / 0 / 2 / 0 | None. A JS polyfill is a partial workaround |
| `ArcHistogramSamplesDelegate` | 2 | 0 / 0 / 0 / 2 | Telemetry |
| Single-callback protocols: `ArcAccessibilityObserver`, `ArcBrowserContextHistoryDelegate`, `ArcCrashKeysObserver`, `ArcNavigationDelegate`, `ArcPerformanceObserver`, `ArcThumbnailsGeneratorObserver`, `ArcTopBandColorDelegate` | 7 | 1 / 2 / 2 / 2 | Navigation policy → `OnBeforeBrowse` (`cef_request_handler.h:92`). The top-band colour comes from Arc's own Blink code (`arc/blink/core/top_band_color_tracker.cc`), so it is Arc-only and only approximable |
| **Total** | **200** | **50 / 31 / 87 / 32** | |

## 4. Spot checks, method by method

In these tables, (-) after a selector means the 1.168.0 app doesn't reference that selector by name.
Internal plumbing (`native*`, `owner`, `*_bridge`, `init*With*:`) is left out.

### 4.1 `ArcBrowserApplication` (imported; 59 selectors, 50 referenced)

| Selector(s) | Class. | CEF mapping |
|---|---|---|
| `init`, `init:`, `initialize:`, `run` | **CEF** | `CefInitialize` + `CefRunMessageLoop` (`cef_app.h:80,132`). `didInitializeBrowserProcess` ← `OnContextInitialized` (`cef_browser_process_handler.h:87`). The app's `NSApplication` must adopt `CefAppProtocol` (`cef_application_mac.h:74`). |
| `tryToTerminate:`, `quit`(-) | **CEF** | `TryCloseBrowser` on every browser (`cef_browser.h:377`), then `CefQuitMessageLoop` and `CefShutdown` (`cef_app.h:140,103`). |
| `version` | **CEF** | `cef_version_info` (`cef_version_info.h:60`). |
| `createBrowserContext:isHidden:type:completion:error:`, `loadBrowserContext:type:completion:`, `getBrowserContext:type:` | **CEF** | `CefRequestContext::CreateContext` with `cache_path` under `root_cache_path` (`cef_request_context.h:121`; `internal/cef_types.h:543-552`). Completion ← `OnRequestContextInitialized` (`cef_request_context_handler.h:61`). |
| `availableBrowserContextAttributes`, `deleteBrowserContext:error:` | Partial | Own profile registry. Delete the directory once all browsers in the context have closed. |
| `localState` | **CEF** | Global `CefPreferenceManager` (`cef_preference.h:152`). |
| `tracingController` | **CEF** | `cef_trace.h:83,100`. |
| `loadUrl:withDelegate:`, `cancelUrlLoad:` | **CEF** | `CefURLRequest::Create` / `Cancel` (`cef_urlrequest.h:78,128`). |
| `devToolsWebSocketDebugURL` | **CEF** | `remote_debugging_port` and the `DevToolsActivePort` file (`internal/cef_types.h:418-427`). |
| `createBrowser:withDelegate:forWindow:`, `createPopupBrowser:withDelegate:` | Partial | Our own window model (§4.3). Popups ← `OnBeforePopup` (`cef_life_span_handler.h:100`). |
| `restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:` | **Chrome** | Navigation stacks can't be restored. `GetNavigationEntries` is read-only (`cef_browser.h:669`). |
| `enableAccessibilityForScreenReader`, `ensureBasicAccessibilityEnabled`, `disableAccessibility`, `setAccessibilityObserver:` | Partial | `SetAccessibilityState` per browser (`cef_browser.h:987`). Detect VoiceOver ourselves (`cef_browser.h:974-977`). |
| `showTaskManager` | Partial | `IDC_TASK_MANAGER` works only in Chrome style (`cef_browser.h:1051`). Instead, build a UI on `CefTaskManager` (`cef_task_manager.h:60-106`). |
| `isFeatureEnabled:`, `setFeatureEnabled:enabled:`, `setStringFeatureValue:value:`, `setIntFeatureValue:value:` | Partial | Feature switches can only be set at startup, through `OnBeforeCommandLineProcessing` (`cef_app.h:208`). No runtime query; `GetChromeVariationsAsSwitches` (`cef_preference.h:131`) only lists variations. Persist the values and apply them on the next launch. |
| `getNonDefaultFlags:`, `setAllFlagsToDefault:`(-) | Partial | The about:flags list lives in a Local State pref, reachable through the global preference manager. Whether CEF applies it is a *(guess)*. |
| `isEncryptionAvailable`, `getBase64EncodedEncryptionKey` | Partial | Read the Keychain item ourselves. `CefSettings.keychain_service_name` (`internal/cef_types.h:515-520`, API 15600) lets CEF's OSCrypt use Arc's existing item. |
| `getEnterpriseDeviceID` | Partial | Own stable ID. |
| `getGPULogMessages`, `setFreeMemoryTargetOnCriticalMemoryPressure:`(-) | **Chrome** | None. Stub them. |
| `getHistogramsJSON`, `debugProperties` | Partial | CDP `Browser.getHistograms` and `SystemInfo.getInfo` through `ExecuteDevToolsMethod`. Whether browser-level CDP domains work on a tab session is a *(guess)*. |
| `setNotificationCenter:` | **Chrome** | No notification bridge. A renderer polyfill can cover page notifications, but not push. |
| `availableImportSourceProfiles:`, `availableImportSources:`, `loadImportSourceProfiles:completion:`, `beginImportSequence`, `endImportSequence` | Arc-only | Rewrite the importer. Cookies go in through `CefCookieManager::SetCookie` (`cef_cookie.h:99`); passwords have no destination. |
| `setBlockLists:`, `setExcludedDomains:` | Arc-only | Ad blocker. Block requests in `OnBeforeResourceLoad` (`cef_resource_request_handler.h:91`) via the per-context `GetResourceRequestHandler` (`cef_request_context_handler.h:86`), and inject cosmetic CSS from the renderer. |
| `setArtifactAllowedScriptSources:` | Arc-only | Custom scheme handler with CSP headers (`cef_scheme.h:65`). |
| `createAIService:` | Arc-only | Own process for the AI library. |
| `setHistogramSamplesDelegate:`, `setBreadcrumbCallback:`, `setDumpWithoutCrashingCallback:`, `setOnWebContentsCloseDebugLogCallback:` | Arc-only | Telemetry and debugging. `CefDumpWithoutCrashing` exists (`base/cef_dump_without_crashing.h:68`). |
| `frameForDisplayId:error:` | Partial | NSScreen lookup. |

### 4.2 `ArcBrowserContext` (imported) and its sub-objects

**`ArcBrowserContext`** (55 selectors, 45 referenced)

| Selector(s) | Class. | CEF mapping |
|---|---|---|
| `preferences`, `history`, `downloads`, `effects`, `pageContentExtraction`, `metrics`, `extensions`, `id` | n/a | Sub-objects, covered below and in §4.7–4.10. |
| `userVisibleName`, `setUserVisibleName:`, `setDelegate:`, `isEqual:`, `hash` | Partial | Own metadata. |
| `createWebContents:` | **CEF** | `CreateBrowser`/`CreateBrowserSync` with `request_context` (`cef_browser.h:301,319`). On macOS this is Alloy style (§1). |
| `getAllCookies:`, `getAllCookies:completion:`, `deleteAllCookies:` | **CEF** | `VisitAllCookies` / `DeleteCookies` (`cef_cookie.h:75,116`). |
| `getContentSettingForOrigin:withContentSettingType:`, `setContentSettingsForOrigin:withContentSettingTypes:withSiteSetting:`, `setPermissionsForOrigin:withPermissionTypes:withSiteSetting:`, `getChangedContentSettingsForOrigin:`, `getChangedPermissionsForOrigin:`(-) | **CEF** | `Get/SetContentSetting` (`cef_request_context.h:301,321`). For "changed" settings, iterate the type enum (`internal/cef_types_content_settings.h`). |
| `loadUrl:withDelegate:`, `cancelUrlLoad:` | **CEF** | `CefURLRequest::Create` with this `request_context` (`cef_urlrequest.h:78-81`). |
| `getLocalStorage:key:completion:`, `setLocalStorage:key:value:completion:` | Partial | CDP `DOMStorage.getDOMStorageItems`/`setDOMStorageItem` keyed by storage key. Whether this works without a page open for that origin is a *(guess)*. |
| `favicon:completion:`, `favicon:iconTypes:sizeInPixels:fallbackToHost:completion:`, `faviconTempFileForPage:…`, `setFaviconData:forPage:…`, `setFavicon:forPage:…`(-) | Partial | Own favicon cache, filled from `OnFaviconURLChange` (`cef_display_handler.h:72`) and `DownloadImage` (`cef_browser.h:527`). Chrome's favicon database isn't reachable, but its SQLite file can be imported once. |
| `getBookmarksTree`, `getImportedBookmarksTree:`, `addBookmark:…`, `addBookmarkFolder:…`, `renameBookmark:withTitle:`, `editBookmark:withNewURL:`, `removeBookmark:withParent:`, `moveBookmark:newParent:atIndex:`, `isBookmarked:`, `clearBookmarks`, `add/removeBookmarkModelObserver:` | **Chrome** | `BookmarkModel` isn't exposed. An own store is feasible. |
| `createAutocompleteControllerWithProviders:` | **Chrome** | §4.9. |
| `import:withCategories:…:error:`, `importPasswords:` | Arc-only + **Chrome** | Rewrite the importer. Passwords need a password store, which isn't exposed. |
| `getAdBlockDebugInfo:completion:` | Arc-only | Ad blocker. |
| `chatClosed:withReason:`(-), `productConnectionClosed:withReason:`(-) | Arc-only | Chat and product-connection bridges through process messages (`cef_frame.h:257`). |

**`ArcBrowserContextPreferences`** (40 selectors). `CefRequestContext` is a `CefPreferenceManager`
(`cef_request_context.h:108`).

| Selector(s) | Class. | CEF mapping |
|---|---|---|
| `addObserver:`, `removeObserver:` | **CEF** | `AddPreferenceObserver` (`cef_preference.h:215`). |
| `promptForDownloadPath`, `downloadFilePath`, and their setters | **CEF** | `Get/SetPreference` (`cef_preference.h:169,201`). Our `OnBeforeDownload` applies them (`cef_download_handler.h:52-58`). |
| `notificationPermissionRequestsEnabled`, `setNotificationPermissionRequestsEnabled:` | **CEF** | Default content setting (`cef_request_context.h:307-310`). |
| `autoOpenFileExtensions`(-), `canAutoOpenFile:`(-), `setAutoOpenFile:autoOpen:error:`(-), `clearAutoOpenFiles`(-) | Partial | Prefs plus our open-on-complete in `OnDownloadUpdated`. |
| `setUserColor:` | Partial | Own attributes; `SetChromeColorScheme` (`cef_request_context.h:345`). |
| `historyDatabaseFilePath`(-) | Partial | Path of our own store. |
| `searchSuggestEnabled`, `keywordSearchSpaceTriggeringEnabled`, and their setters | **Chrome** | The prefs can be read and written, but they only mean something with Chrome's omnibox. |
| `canEnablePasswordSaving`, `passwordSavingEnabled`, `setPasswordSavingEnabled:`, `canEnableCreditCardsSaving`, `creditCardsSavingEnabled`, `setCreditCardsSavingEnabled:` | **Chrome** | Same: prefs only (`CanSetPreference` at `cef_preference.h:190`), and the features are absent. |
| `getDefaultSearchEngine`, `getSearchEngines`, `setDefaultSearchEngine:`, `addSearchEngine:error:`, `removeSearchEngine:error:`, `setIsActiveSearchEngine:isActive:error:`(-) | **Chrome** | `TemplateURLService` isn't exposed. An own list is feasible. |
| `classifyPasteAndGoInput:maxDisplayCharacters:`(-) | **Chrome** | `AutocompleteClassifier`. |

**`ArcBrowserContextEffects`** (all 10 register/unregister selectors, Arc-only)
- Inject CSS and JS per host pattern from `CefRenderProcessHandler::OnContextCreated`
  (`cef_render_process_handler.h:98`), driven by browser→renderer process messages.
- Approximate colour filter and colour scheme with CSS `filter` and `color-scheme` *(guess: lossy, and
  first paint may flash)*.
- Zoom → `SetZoomLevel` on matching navigations (`cef_browser.h:482`).

**`ArcBrowserContextPageContentExtraction getExtractedContentForWebContents:completion:`** (Partial, L)
- Chrome's Blink AIPageContent agent is hidden.
- Rebuild the node tree with a renderer DOM walk (`cef_frame.h:226`, `cef_dom.h:166`) or CDP
  `DOMSnapshot.captureSnapshot`.

**`ArcBrowserContextDevTools close`, `webContents`** (Partial)
- DevTools for non-tab targets: load the DevTools front-end in a CEF browser against the CDP `Target`
  domain.

### 4.3 `ArcBrowser` (returned object; 38 selectors, 32 referenced). Overall: **Chrome**

Arc maps one Chrome `Browser` to each Arc window and puts many tabs in it. Extensions' `chrome.windows`,
`chrome.tabs` and `chrome.tabGroups` see that model. CEF has no window-with-tabs object; each
`CefBrowser` is one page.

| Selector(s) | Class. | CEF mapping |
|---|---|---|
| `createWebContents:` | **CEF** | `CreateBrowser` (`cef_browser.h:301`). |
| `tryToCloseSkipBeforeUnloadHandlers:callback:` | **CEF** | `CloseBrowser(force)` / `TryCloseBrowser` (`cef_browser.h:362,377`). |
| `getAllCookies:`, `getAllCookies:completion:`, `setCookie:withCookie:`, `removeCookie:withName:`, `saveCookiesToDisk`(-) | **CEF** | `cef_cookie.h:75,99,116,126`. |
| `context`, `type`, `sessionId`, `windowIdentifier`, `setWindowIdentifier:`, `isInitialized`, `setDelegate:`, `browserCreateParams` | Partial | Own model object. |
| `setWindow:`, `takeOwnershipOfWebContents:` | Partial | Reparent each tab's NSView (`GetWindowHandle`, `cef_browser.h:410`) with AppKit. Moving Alloy views between windows should work *(guess)*; nothing in Chrome moves with them. |
| `isJavaScriptFromAppleEventsAllowed`, `toggleJavaScriptFromAppleEventsAllowed` | Partial | A pref plus our own AppleScript support. ArcCore currently ships Chrome's `BrowserCrApplication(AppleScriptAdditions)`. |
| `isCastEnabled` | Partial | `CefMediaRouter` (`cef_media_router.h:60`) handles presentation/Cast routes. It has no Chrome Cast dialog and no tab mirroring *(guess)*. |
| `autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:`, `deleteAutocompleteMatch:` | **Chrome** | §4.9. |
| `executeExtensionAction:`, `pinExtension:`, `unpinExtension:`, `moveExtension:targetIndex:`, `showExtensionContextMenu:`, `triggerExtensionOmniboxInput:inputText:disposition:` | **Chrome** | §4.8. |
| `canRestoreWebContentsWithSessionId:`, `restoreWebContentsWithSessionId:`, `restoreForeignTab:` | **Chrome** | `TabRestoreService`/sessions, with no way to restore navigation history. Foreign tabs depend on sync, which ends 2026-11-23. |
| `sidePanel` | **Chrome** | Extension side panel. |

### 4.4 `ArcWebContents` (returned object; 160 selectors, 110 referenced). Overall: Partial

| Area: selectors | Class. | CEF mapping |
|---|---|---|
| View hosting: `createPaneHostView`, `nativeView`, `attachToPaneHostView:`, `detachFromPaneHostView:`, `bootstrapRendering`, `bootstrapRenderingComplete`, `isInitialized` | Partial | `CefWindowInfo::SetAsChild` (`internal/cef_mac.h:90`), so Alloy style only (§1). `GetWindowHandle` (`cef_browser.h:410`), `OnAfterCreated` (`cef_life_span_handler.h:169`). We would write the pane and overlay views. |
| Closing: `close`, `closeWithForegroundCommitTiming:`, `willRunUnloadEvents`, `hasUnloadHandler:`(-) | Partial | `CloseBrowser`/`TryCloseBrowser` (`cef_browser.h:362,377`). The `TryCloseBrowser` result reveals pending unload handlers; there is no direct query. |
| Identity: `sessionId`, `diagnosticId`, `isEqual:`, `hash` | Partial | `GetIdentifier` (`cef_browser.h:131`). |
| `clone` | Partial | New browser plus a URL load. The back/forward list is lost. |
| `discard` | **Chrome** | No discard. Closing and recreating loses history and form state. |
| Delegates: `setNavigationDelegate:`, `setUIDelegate:`, `setActionsDelegate:`(-), `setTopBandColorDelegate:`, `setGestureHandler:forGestures:`, `setUserActivityDelegate:`(-) | Partial | Our `CefClient` handlers. Top band colour and gestures are approximations. |
| State: `url`, `title`, `isLoading`, `canGoBack`, `canGoForward`, `loadProgress` | **CEF** | `OnAddressChange`, `OnTitleChange`, `OnLoadingProgressChange` (`cef_display_handler.h:57,65,141`); `OnLoadingStateChange` (`cef_load_handler.h:64`). |
| `favicon` | Partial | `OnFaviconURLChange` + `DownloadImage`. |
| `themeColor`, `rootBackgroundColor` | Partial | No callback. The renderer reads `<meta name=theme-color>` and computed styles. |
| `isReadyToDisplay`, `navigationID`, `contentsMimeType`, `pendingRenderFrameViewportSize`, `tabTraits`, `inputEvent` | Partial | Proxies: `OnDocumentAvailableInMainFrame` (`cef_request_handler.h:280`), our own counter, the main-frame `OnResourceResponse` MIME type (`cef_resource_request_handler.h:145`). Some are *(guess)*. |
| `visibility` | Partial | Windowed browsers follow the NSView's hidden and occlusion state. `WasHidden` is windowless-only (`cef_browser.h:702-707`). |
| `state`, `errorStatus` | **CEF** | `OnRenderProcessTerminated` (`cef_request_handler.h:270`). |
| `backForwardList` | **CEF** | `GetNavigationEntries` (`cef_browser.h:669`). |
| `navigationController`, `devTools`, `mediaSession`, `thumbnailsGenerator`, `pdfController`, `boostIDE`, `aiAgent` | n/a | Sub-objects (§4.5, §4.6, §2.I). |
| `stop` | **CEF** | `StopLoad` (`cef_browser.h:124`). |
| `find:forward:matchCase:findMatch:completion:`, `stopFind` | **CEF** | `Find`/`StopFinding` (`cef_browser.h:560,569`); `OnFindResult` (`cef_find_handler.h:60`). |
| `findTextWithDirectives:completion:`(-) | **Chrome** | Text-fragment highlighting isn't exposed. |
| Zoom: `zoomFactor`, `zoomLevel`, `setZoomFactor:`, `resetZoomAndScaleFactors`, `isScaleFactorOne`, `deviceScaleFactor` | Partial | `Get/SetZoomLevel`, `Zoom` (`cef_browser.h:473,482,459`). Pinch scale needs CDP `Emulation.setPageScaleFactor`. |
| `setSafeAreaInsets:left:bottom:right:` | Partial | CDP emulation override *(guess)*. |
| `executeJavascript:completion:`(-), `executeJavascript:reason:completion:` | Partial | `ExecuteJavaScript` returns no result (`cef_frame.h:162`). Use CDP `Runtime.evaluate`, or renderer `CefV8Context::Eval` (`cef_v8.h:220`) plus a process message. |
| `insertStylesheet:stylesheet:`, `removeStylesheet:` | Partial | Renderer injection or CDP `CSS`. |
| `applyColorFilter:`, `removeColorFilter`, `applyColorScheme:`, `removeColorScheme` | Arc-only | Effects (§4.2). |
| `print`, `basicPrint`(-) | Partial | `Print()` (`cef_browser.h:537`). In Alloy style on macOS this is presumably the system dialog, not Chrome's print preview *(guess)*. Also `PrintToPDF` (546). |
| `save` | Partial | `IDC_SAVE_PAGE` is Chrome-style only (`cef_browser.h:1051`). Use CDP `Page.captureSnapshot` (MHTML) or `GetSource` (`cef_frame.h:129`). |
| `viewSource` | **CEF** | `CefFrame::ViewSource` (`cef_frame.h:122`). |
| `captureViewport:`, `captureFullPage:`, `captureRegion:completion:` | Partial | CDP `Page.captureScreenshot` (clip, `captureBeyondViewport`). |
| `isFullscreen`, `exitFullscreen` | **CEF** | `cef_browser.h:1020,1034`. |
| `muted`(-), `setMuted:` | **CEF** | `IsAudioMuted`/`SetAudioMuted` (`cef_browser.h:1010,1003`). |
| `audible`(-) | **Chrome** | No audible-state API or event. |
| `disallowDownloads`, `setDisallowDownloads:` | **CEF** | `CanDownload` (`cef_download_handler.h:102`). |
| `isBlockingJavascriptPopups`, `setIsBlockingJavascriptPopups:`, `javascriptPopupsDisplayedCount` | **CEF** | Our `OnJSDialog` policy (`cef_jsdialog_handler.h:86`). |
| `hasFocus`, `activateTab:` | **CEF** | `CefFocusHandler` (`cef_focus_handler.h:62-78`); `SetFocus` (`cef_browser.h:401`). |
| `selectedText`, `undoableFieldActive`, `freeformTextFieldActive`, `passwordTextFieldActive`, `hadFormOrUserEdits`, `hasDirtyTextField`, `isScrollOffsetAtTop` | Partial | Renderer `OnFocusedNodeChanged` (`cef_render_process_handler.h:132`) plus DOM listeners, sent to the browser as process messages. |
| `consumeExternalProtocolLaunchPermission` | Partial | Our state in `OnProtocolExecution` (`cef_resource_request_handler.h:203`). |
| `clearSiteDataAndReload:` | Partial | `DeleteCookies(url)`, CDP `Storage.clearDataForOrigin`, then `Reload`. |
| `showBlockedPopups` | Partial | Our own popup blocker, from `OnBeforePopup`'s `user_gesture`. |
| `showCertificateDialog` | Partial | Own viewer from `CefSSLStatus`/`CefX509Certificate` (`cef_ssl_status.h:80`). |
| `showCookiesDialog` | **Chrome** | The page-info cookies dialog isn't exposed. An own UI over `VisitUrlCookies` is possible. |
| Accessibility and AI agent: `rootFrameAccessibilityTreeId`, `accessibilityTreeForTreeId:`, `performAccessibilityAction:treeId:nodeId:`, `typeText:inFrameWIthAccessibilityTreeId:`, `sendKeyEvent:treeId:`, `setAccessibilityOverrideActive:` | Partial | CDP `Accessibility.getFullAXTree`, `DOM`, `Input`; `SendKeyEvent` (`cef_browser.h:751`). AX tree and node IDs become CDP IDs. |
| `emitFreezingVote:` | Partial | CDP `Page.setWebLifecycleState` (L). |
| `translateToTargetLanguage:`, `supportedTranslateLanguages`, `revertTranslation` | **Chrome** | Translate isn't exposed. |
| `shareThisTab`, `stopSharingThisTab` | **Chrome** | Tab-capture switching isn't exposed. |
| `skipUserActivationForPiP:`, `ignoreDisablePiPAttribute:`, `cast` | **Chrome** | Arc's auto-PiP and Cast UI. |
| `blockedResourceCount` | Arc-only | Ad-blocker counter. |
| `setTitle:`, `setUrl:`, `setState:`, and about 40 other `set*` selectors | n/a | Setters the engine uses internally to push state. The app doesn't need them. |

**`ArcWebContentsUIDelegate` (65 callbacks): 20 CEF, 14 Partial, 25 Chrome, 6 Arc-only**

| Callback(s) | Class. | CEF source |
|---|---|---|
| `didCloseWithSessionID` | **CEF** | `OnBeforeClose` (`cef_life_span_handler.h:293`) |
| `didCreateNewWebContent:targetURL:disposition:` | **CEF** | `OnBeforePopup` + `OnAfterCreated` (`cef_life_span_handler.h:100,169`) |
| `webContentsDidRequestExitFullscreen:` | **CEF** | `OnFullscreenModeChange` (`cef_display_handler.h:87`) |
| `webContentsDidRequestEnterFullscreen:withDisplayId:requestingOrigin:` | Partial | Same callback, but no display ID or origin |
| `didChangeStatusText:` | **CEF** | `OnStatusMessage` (`cef_display_handler.h:108`) |
| `willShowContextMenu:withElement:` | **CEF** | `RunContextMenu` + `CefContextMenuParams` (`cef_context_menu_handler.h:120,212-344`) |
| `willRunJavaScriptAlert/Confirm/TextInputPanel` (3), `webContentsWillCloseJavaScriptPanel:` | **CEF** | `OnJSDialog`, `OnResetDialogState`/`OnDialogClosed` (`cef_jsdialog_handler.h:86,118,124`) |
| `runJavaScriptApplicationModalBeforeUnloadPanel…`, `webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:` | **CEF** | `OnBeforeUnloadDialog` (`cef_jsdialog_handler.h:105`) |
| `webContentsClosePermission:` | **CEF** | `OnDismissPermissionPrompt` (`cef_permission_handler.h:143`) |
| `willAskForPermission:withRequests:withQuietUIReason:completion:` | Partial | `OnShowPermissionPrompt`/`OnRequestMediaAccessPermission` (`cef_permission_handler.h:124,103`). No quiet-UI reason, and no device choosers *(guess)*. |
| `didChangeMuteState:` | **CEF** | We set mute ourselves |
| `showWebLoginDialog:`, `webContentsDidCloseWebLoginDialog:` | **CEF** | `GetAuthCredentials` (`cef_request_handler.h:165`) |
| `didConsumeKey:withModifierFlags:underlyingEventType:` | **CEF** | `OnKeyEvent` (`cef_keyboard_handler.h:73`) |
| `showHungRendererDialog:`, `webContentsHideHungRendererDialog:` | **CEF** | `OnRenderProcessUnresponsive`/`Responsive` (`cef_request_handler.h:247,259`) |
| `webContentsProcessGone:`, `webContentsRenderViewReady:` | **CEF** | `OnRenderProcessTerminated`, `OnRenderViewReady` (`cef_request_handler.h:270,226`) |
| `webContentsActivate:`, `webContentsSelect:shouldActivate:` | Partial | No `ActivateContents` hook; use focus callbacks |
| `webContentsClosingImminent:withSessionID:` | Partial | `DoClose` (`cef_life_span_handler.h:277`; Alloy only) |
| `didChangeVisibleSecurityState:` | Partial | Poll `CefSSLStatus` on navigation (`cef_ssl_status.h:55-80`) |
| `canExternallyNavigate:`, `willExternallyNavigate:`, `showExternalProtocolDialog:` | Partial | `OnProtocolExecution` (`cef_resource_request_handler.h:203`) |
| `didCaptureImage:`, `showsAuxiliaryWindow:` | Partial | Purpose unclear *(guess)* |
| `hasBlockedPopupWindowsChanged:` | Partial | Our own popup blocker |
| `didShowWebUI:`, `didDestroyWebUI:newURL:` | Partial | URL-scheme check in `OnLoadStart` |
| `didChangePointerLock:requestingOrigin:`, `webContentsWantsOverscrollSwipeEvents:`, `didChangeAudibleState:` | **Chrome** | Content-layer events that CEF doesn't surface |
| `didSubmitAddressEntry:`, `didSubmitCredentials:`, `didSubmitCreditCardInfo:`, `didAutoFillWebLoginDialog:` | **Chrome** | Autofill and password manager |
| `finalizeDiscardRequest:` | **Chrome** | Discarding |
| `askForExtensionInstallationConsent:`, `askForExtensionUninstallationConsent:` | **Chrome** | Extensions |
| `showWebUIDialog:`, `didCloseWebUIDialog:`, `willShowConfirmInfobar:`, `willCloseConfirmInfobar:` | **Chrome** | Chrome UI hooks |
| Translate (7 callbacks) | **Chrome** | Translate |
| `onShareThisTabStateChanged:`, `showDevToolsToolbox:`, `updateDevToolsToolbox:`, `didFinishPDFRendering:` | **Chrome** | — |
| `onHighCPUUsageWhileInBackground:…`, `onCopyLinkToContextMenu:`, `promptForArtifactLinkPermission…`, chat frame association, disassociation and send (3) | Arc-only | `CefTaskManager`; process messages |

### 4.5 Navigation: `ArcWebContentsNavigationController`, `ArcNavigation`, `ArcNavigationAction` and the observers

| Selector / callback | Class. | CEF mapping |
|---|---|---|
| `load:initiatorOrigin:` | Partial | `LoadURL`/`LoadRequest` (`cef_frame.h:152,146`). Initiator origin can't be set; only the referrer can (`cef_request.h:104`). |
| `load:initiatorOrigin:transition:fromAddressBar:typedWithHttpScheme:`(-) | Partial | `CefRequest` exposes `GetTransitionType` only (`cef_request.h:213`), so typed or omnibox transitions can't be set. |
| `goBack`, `goForward`, `reload`, `reloadWithoutCache` | **CEF** | `cef_browser.h:88,100,112,118`. |
| `goToBackForwardListItem:` | Partial | `CefBrowser` has no go-to-index. Use CDP `Page.getNavigationHistory` + `Page.navigateToHistoryEntry`. |
| `addObserver:`, `removeObserver:`, `observers` | Partial | Our own observer list. |
| Policy: `decidePolicyForNavigationAction:` (`ArcNavigationAction`: url, referrer, type, disposition) | **CEF** | `OnBeforeBrowse` + `OnOpenURLFromTab` (`cef_request_handler.h:92,117`). |
| `didStartProvisionalNavigation:` | Partial | `OnBeforeBrowse`. CEF's `OnLoadStart` fires *after* commit (`cef_load_handler.h:70-79`). |
| `didReceiveServerRedirectForProvisionalNavigation:` | **CEF** | `OnBeforeBrowse(is_redirect)`; `OnResourceRedirect` (`cef_resource_request_handler.h:125`). |
| `didCommitNavigation:`, `didFinishNavigation:` | **CEF** | `OnLoadStart`/`OnLoadEnd` (`cef_load_handler.h:82,97`). |
| `didFailNavigation:`, `didFailProvisionalNavigation:withError:` | Partial | Both come from `OnLoadError` (`cef_load_handler.h:110`). Tell them apart by whether `OnLoadStart` already fired. |
| `didStartInPageNavigation:`, `didFinishInPageNavigation:` | Partial | No same-document callbacks (`cef_load_handler.h:76-77`). Infer from `OnAddressChange` without `OnLoadStart`. |
| `ArcNavigation responseStatusCode`, `responseHeaders` | Partial | `httpStatusCode` from `OnLoadEnd`; headers from the main-frame `OnResourceResponse` (`cef_resource_request_handler.h:145`). The headers need a resource handler on every request. |
| `ArcNavigation didRedirectToOtherTab` | Arc-only | Arc's redirect-to-existing-tab feature. |

### 4.6 DevTools: `ArcWebContentsDevTools` and `ArcBrowserContextDevTools`

| Selector / callback | Class. | CEF mapping |
|---|---|---|
| `open`, `close` | **CEF** | `ShowDevTools(windowInfo → Arc's DevTools pane)`, `CloseDevTools` (`cef_browser.h:582,591`). `windowInfo` is honoured when the browser isn't Views-hosted (`cef_browser.h:577-578`). |
| `inspectElementAt:`(-) | **CEF** | `inspect_element_at` (`cef_browser.h:576`). |
| `webContents`, `toolboxWebContents` | Partial | We get the DevTools browser through our client's `OnAfterCreated`. There is no device-mode toolbox. |
| `openInspectView`, `openConsoleView`, `openNetworkView`, `currentPanel:error:` | Partial (L) | No API. Script the DevTools front-end; its internals are unstable. |
| `execute:completion:error:` | **CEF** | `ExecuteDevToolsMethod` + `OnDevToolsMethodResult` (`cef_browser.h:649`; `cef_devtools_message_observer.h:94`). |
| `subscribe:autoEnable:notification:`, `unsubscribe:` | **CEF** | `AddDevToolsMessageObserver` + `OnDevToolsEvent` (`cef_browser.h:660`; `cef_devtools_message_observer.h:109`). |
| `openProtocolConnection:error:`, `sendProtocolMessage:connectionID:error:`, `closeProtocolConnection:` | Partial | `SendDevToolsMessage` (`cef_browser.h:633`). It appears to be one session per browser, so several app connections would need ID remapping and would share domain-enable state *(guess)*. ADK's in-process CDP client sits on this. |
| Delegate `displayAs:`, `webContentsDevToolsDidClose:`, `webContentsDevToolsNeedsFocus:` | Partial / **CEF** | Our layout; `OnBeforeClose` of the DevTools browser. |
| Delegate `needsConfirmation:completion:`, `inspectModeDidChange:` | **Chrome** | Not exposed. |
| App: `devToolsWebSocketDebugURL` | **CEF** | `remote_debugging_port` (`internal/cef_types.h:427`). |
| Context: `close`, `webContents`; delegate `showDevTools:for:`, `didCloseDevTools:` | Partial | Worker and extension targets go through CDP `Target` plus a front-end browser. `devToolsNeedsConfirmation` needs Chrome. |

### 4.7 Downloads: `ArcBrowserContextDownloads` and `ArcDownloadItem`

| Selector / callback | Class. | CEF mapping |
|---|---|---|
| `setDelegate:` | **CEF** | `CefClient::GetDownloadHandler` (`cef_client.h:109`). |
| `startDownload:error:` | Partial | `StartDownload(url)` (`cef_browser.h:512`) needs a browser; use a hidden one for profile-level downloads. |
| `onDownloadCreated:`, `onDownloadUpdated:` | **CEF** | `OnBeforeDownload` / `OnDownloadUpdated` (`cef_download_handler.h:120,135`). |
| `onDeterminingFilename:defaultFilename:completionTimeout:completion:` | **CEF** | `suggested_name` + `CefBeforeDownloadCallback::Continue(path, show_dialog)` (`cef_download_handler.h:58,120`). |
| `downloadManagerInitializedWithItems:`, `onDownloadRemoved:` | Partial | No download history in CEF. Persist our own list, and import Chrome's `History` `downloads` table once. |
| `completedBytes`, `totalBytes`, `currentSpeed`, `targetPath`, `sourceURL`, `id`, `startTime`, `isPaused` | **CEF** | `CefDownloadItem` (`cef_download_item.h:117,111,98,135,147,141,123,85`). |
| `currentState`, `isDone`(-), `canResume`(-), `remainingTimeInSeconds` | **CEF** | Derived from `IsInProgress`/`IsComplete`/`IsCanceled`/`IsInterrupted`/`GetInterruptReason` (`cef_download_item.h:60-92`). |
| `pause`, `resume`, `cancel:` | **CEF** | `CefDownloadItemCallback` (`cef_download_handler.h:71,77,83`). Keep the most recent callback for each download. |
| `remove` | Partial | Our own list. |
| `webContents` | **CEF** | The `browser` argument. |

### 4.8 Extensions: `ArcBrowserContextExtensions` and friends. Overall: **Chrome**, no workaround

| Selector / callback | Class. | Note |
|---|---|---|
| `isExtensionSystemReady`, `listExtensions`, `getExtensionInfo:` | **Chrome** | No CEF API. `grep -i extension include/` finds only V8 extensions, MIME helpers and `cef_browser.h:128` ("tabId for extension APIs"). |
| `enableExtension:`, `disableExtension:`, `reloadExtension:`, `uninstallExtension:` | **Chrome** | Same. CDP `Extensions.loadUnpacked`/`uninstall` need a browser-target session and a debugging flag, and Alloy tabs probably wouldn't run extensions anyway *(guess)*. |
| `installExtensionFromWebstore:unattended:completion:`, `installExtensionFromUnpackedPath:completion:` | **Chrome** | Same. |
| `getAllExtensionCommands`, `executeExtensionCommand:commandName:`, `getPinnedExtensionOrder`, `setPinnedExtensionOrder:`, `runJSInExtensionBackgroundPage:js_code:` | **Chrome** | Same. |
| `ArcBrowser` actions: execute, pin, unpin, move, context menu, omnibox input | **Chrome** | Same. |
| `ArcBrowserContextExtensionsDelegate` (10), `ArcBrowserDelegate` popup, toolbar, permission and tab-group callbacks (13), UI-delegate consent callbacks (2), `ArcBrowserSidePanel*` | **Chrome** | Same. |

### 4.9 Autocomplete. Overall: **Chrome**, workaround = a new omnibox

Affected selectors:
- `ArcBrowser autocomplete:…keyword:error:completion:`
- `ArcBrowser deleteAutocompleteMatch:`
- `ArcBrowserContext createAutocompleteControllerWithProviders:`
- `ArcBrowserContextAutocomplete autocomplete:cursorPosition:preventInlineAutocomplete:completion:`(-)
- `ArcBrowserContextAutocomplete stop`
- `ArcBrowserContextPreferences classifyPasteAndGoInput:…`
- the 25 fields of `ArcAutocompleteMatch`

None of this has a CEF counterpart.

**Workaround:** our own providers.
- History and bookmarks come from our own stores.
- Search suggestions come from `CefURLRequest` to the engine's suggest URL.
- Keyword engines come from our own list.
- Open tabs come from the app.

Arc's command-bar ranker (Arc-only, in `ArcAIService`) already reranks candidates. Its features are recency,
frequency, typed count and site engagement (`ArcCommandBarRankingCandidate`), which an own history store
can supply. What's lost is the quality of Chrome's providers, especially HistoryQuick, Shortcuts and
inline autocomplete.

### 4.10 History: `ArcBrowserContextHistory` and related types. Overall: **Chrome**, workaround = own store

| Selector | Class. | Note |
|---|---|---|
| `searchHistory:completion:`, `getTopSitesFromHistory:`, `addUrlToHistory:title:`, `deleteUrlsFromHistory:completion:`, `expireHistory:completion:`, `setDelegate:` | **Chrome** | `HistoryService` isn't exposed. Our store is fed by `OnLoadEnd`/`OnAddressChange`/`OnTitleChange`, and Arc's existing `History` SQLite file can be imported once. |
| `expireHistoryBetween:…`, `setVisitObserver:`, `addVisit:`, `addVisits:`, `getVisitsSince:`, `getVisitsForURL:`, `deleteByURL:endTime:`, `deleteByTimeRangeStart:end:`, `getLocalCacheGUID:`, `deleteByOriginatorVisitIDs:…` | **Chrome** | **All (-)**: the 1.168.0 app references none of these. They look like history-sync plumbing, which dies with sync on 2026-11-23. Stub them. |
| `didDeleteHistory:` (delegate) | **Chrome** | Our store. |

Chrome's own features also lose history: `chrome://history`, the omnibox, top sites and `chrome.history`.
In Alloy style these are unavailable regardless.

## 5. Summary

### Counts

| Scope | CEF public API | CEF partial / workaround | Needs Chrome internals | Arc-only logic | Total |
|---|--:|--:|--:|--:|--:|
| Classes, all | 17 | 44 | 54 | 31 | 146 |
| Classes imported by the app | 8 | 15 | 23 | 17 | 63 |
| Selectors, all classes | 167 (10%) | 681 (40%) | 572 (34%) | 281 (17%) | 1,701 |
| Selectors, imported classes | 93 (13%) | 262 (36%) | 234 (32%) | 136 (19%) | 725 |
| Selectors the app references (heuristic) | 94 | 418 | 276 | 147 | 935 |
| Callbacks into the app (§3) | 50 | 31 | 87 | 32 | 200 |

Selector counts take each class's classification. ArcWebContents (Partial, 160 selectors) contains
about 18 Chrome-internal and Arc-only selectors, and the Arc-only and Chrome counts are inflated by
value types such as the 25-field `ArcAutocompleteMatch`.

**By feature area** (classes, CEF / Partial / Chrome / Arc-only):

| Area | Split | Notes |
|---|---|---|
| Application/process | 5 / 6 / 4 / 6 | |
| Profile and services | 2 / 8 / 5 / 0 | |
| History, omnibox, search, bookmarks | 0 / 0 / 16 / 0 | Reimplementable as our own stores |
| Passwords and autofill | 0 / 0 / 4 / 0 | |
| Extensions, tab groups, side panel | 0 / 0 / 11 / 0 | |
| Browser and sessions | 0 / 2 / 5 / 0 | |
| Web contents core | 4 / 10 / 1 / 0 | |
| Prompts and dialogs | 4 / 5 / 3 / 0 | |
| Media, PDF, page content | 0 / 9 / 5 / 0 | |
| Utilities | 2 / 4 / 0 / 0 | |
| Arc-only services | 0 / 0 / 0 / 25 | |

The web-contents core, dialogs, cookies, content settings, downloads, URL loading, tracing and prefs map
well. Everything that makes Arc a Chrome-class browser doesn't.

### What CEF fundamentally can't do (prebuilt, public API, macOS)

1. **Chrome-style browsers inside Arc's own NSViews.** `parent_view` forces Alloy style on macOS
   (`internal/cef_types_mac.h:151-153`). Every tab therefore lacks Chrome-runtime features:
   - extensions;
   - datalist and autofill popups;
   - WebHID and WebMIDI;
   - Chrome's device-chooser and permission UIs;
   - Chrome WebUI pages such as `chrome://settings`, `chrome://extensions` and `chrome://history`. The app
     links to all three, and to `arc://password-manager`.
   - Chrome commands (`cef_browser.h:1040-1051`).
2. **Chrome extensions.** There is no API, and the Alloy-era `cef_extension.h` was deleted in M128. This
   removes Arc's extension toolbar, popups, pinning, commands, side panel, `chrome.tabGroups` bridge and
   Web Store installs. Password-manager extensions such as 1Password and Bitwarden go with it, and
   ArcMod's README relies on those for passkeys.
3. **Chrome profile services.** None of these are exported:
   - password manager and autofill: no workaround short of writing a password manager;
   - history, omnibox, bookmarks, search engines, top sites, site engagement, favicons: reimplementable
     as our own stores, at the cost of quality;
   - translate, notifications bridge (push included), media session and Arc's PiP, the PDF viewer API:
     no clean workaround.
4. **Tab lifecycle.** Navigation history can't be restored (`GetNavigationEntries` is read-only,
   `cef_browser.h:669`). That rules out tab discard and restore, session restore with back/forward
   history, `clone` with history, and memory saver. Arc restores every tab on every launch.
5. **Changes inside the engine.** Arc's renderer and Blink code can only be approximated from renderer JS,
   with flicker and lossiness *(guess)*:
   - `arc/blink/core/top_band_color_tracker.cc`;
   - the `arc.mojom` agents, including Effects and the ad blocker's cosmetic agent.

   Content-layer events CEF hides (pointer lock, audible state, same-document commit, visible security
   state) need polling or are missing.

### What CEF would make easier

- **Per-release updates.**
  - CEF branch 8078 moved to 156.0.8078.48 on the same day Arc's engine shipped it. Prebuilts are
    published automatically: the newest is 156.0.7 for `.17`.
  - There is no 31 GB checkout, no 80-minute build and no SDK juggling.
  - The C API is versioned and compatible across a range of releases (`cef_api_hash.h:64-68`), so the
    shim needs to be rebuilt only rarely.
- **Packaging.** Helpers, sandbox, framework layout and the message-pump integration are solved
  (`cef_app.h`, `cef_application_mac.h`).
- **Clean mappings for a large slice of the UI delegate:** JS dialogs, auth, permission prompts, popups,
  downloads, find, fullscreen, context menus, hung and crashed renderers, key events, status text, and
  title/favicon/progress.
- **In-process CDP** (`cef_browser.h:633-660`) covers screenshots, the AX tree, input injection, JS with
  results, history-entry jumps, lifecycle freezing and histograms. That is most of what Arc's AI agent and
  ADK need.
- **Prefs and content settings APIs:** profile and global prefs, custom pref registration, content and
  website settings.
- **Other APIs:** `CefTaskManager`, `CefComponentUpdater`, `CefMediaRouter`, `CefURLRequest`, custom
  schemes, and `CefMessageRouter` for Arc's page↔app bridges (chat, Boost IDE, artifacts).
- **Profile compatibility looks plausible.** `keychain_service_name` (API 15600) lets CEF decrypt
  cookies with Arc's existing Keychain key, and the Chrome bootstrap uses Chrome's profile layout at the
  same Chromium version *(guess; unverified)*.

### Recommendation: compile Chromium with our own `arc/` layer

1. **Parity is the goal, and prebuilt CEF can't reach it on macOS.** The blockers above cover the features
   Arc users notice first: extensions, passwords, omnibox quality, and tabs that restore with history.
   About 32–34% of the selectors need Chrome internals that libcef hides, and a further ~17% are Arc-only
   and need rewriting under either route.
2. **The CEF route isn't much cheaper to build.** We would still implement all 146 classes and 200
   callbacks in a shim. On top of that we would write our own history, bookmarks, search engines,
   omnibox and favicon stores, plus renderer agents. The real saving is per-release maintenance.
3. **A CEF fork doesn't fit.** Building CEF from source, patched for Chrome-style browsers in an NSView
   and with extension and history APIs added, is already a compile route. CEF's object model (one
   `CefBrowser` per page, one Chrome-style BrowserView per window) also clashes with Arc's: one Chrome
   `Browser` per Arc window, holding many tabs that move between windows, which is what extensions see.
   Our own `arc/` layer reproduces Arc's model directly, as TBC's does.
4. **Borrow from CEF anyway.** CEF is BSD-licensed. Its source shows how to host Chrome's `Browser` and
   `WebContents` outside Chrome's UI: `libcef/browser/chrome/*`, native-parent hosting on
   Windows/Linux, and permission, dialog and download delegates. That reference makes our `arc/` rewrite
   cheaper and helps when rebasing it each release.
5. **Keep prebuilt CEF only as a fallback "Arc-lite" engine,** in case the compile route stalls. ArcMod
   keeps running on ArcCore 1.170.0 (Chromium 156) in the meantime, so no interim engine is needed.

### Main uncertainties

- **Alloy tabs under the Chrome bootstrap.** Do they get any extension behaviour, such as content scripts
  or `chrome.*` APIs for pages? I assume not, based on the docs. This is the single biggest swing factor
  for "Arc-lite". A one-day spike with cefclient on macOS would settle it, along with NSView reparenting
  across windows.
- **Whether Rahul would accept a reduced feature set** (no extensions, no password manager) in exchange
  for cheap updates. That is a product decision, not a technical one.
- **Accuracy of the app-reference counts.** They come from a selector-name scan. Generic selectors
  overcount; calls through Swift protocols or KVC could undercount.
- **Purposes inferred from selectors.** A few are uncertain: `ArcDevToolsConnectionDialog`, `didCaptureImage`,
  `showsAuxiliaryWindow`, `ArcSpake2Context`, `ArcFreezingVoteToken`.
- **Size of the compile route.** The Arc layer is at most about 5,840 functions or 2.3 MB of code
  (ENGINE.md §3), and TBC's edits outside `arc/` (Blink top band, build files) aren't yet measured.
  Neither route has a firm effort estimate.
- **CEF capabilities not checked at runtime.** On macOS Alloy style: print path, PDF viewer,
  `<datalist>`, WebHID and gamepad, and multi-session CDP. These come from the docs or are *(guess)*.
