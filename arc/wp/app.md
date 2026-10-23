# Work package "app": application-level services

Read `COMMON.md` first. ArcBrowserApplication's bootstrap (init:, initialize:, run, quit, version,
isEncryptionAvailable) and profile queries are the lead's (`arc/api/arc_browser_application.mm`,
`arc_browser_context.mm`); add everything else as categories in your files.

Classes: the rest of ArcBrowserApplication (notification center, histogram/breadcrumb/crash/accessibility
callbacks and observers, features/flags (isFeatureEnabled:, setFeatureEnabled:enabled:, setStringFeatureValue:,
setIntFeatureValue:, getNonDefaultFlags:, setAllFlagsToDefault:), accessibility modes, showTaskManager,
tracingController, networkActivityMonitor, debugProperties, getGPULogMessages, getHistogramsJSON,
getBase64EncodedEncryptionKey, getEnterpriseDeviceID, memory pressure target, setOnWebContentsCloseDebugLogCallback:,
devToolsWebSocketDebugURL, loadUrl:withDelegate:, cancelUrlLoad:, setBlockLists:, setExcludedDomains:,
setArtifactAllowedScriptSources:, frameForDisplayId:error:, availableImportSources/Profiles,
loadImportSourceProfiles:completion:, begin/endImportSequence, tryToTerminate:, deleteBrowserContext:error:,
loadBrowserContext:type:completion:, createBrowserContext:isHidden:type:completion:error:),
ArcLocalState, ArcCpuInfo, ArcMetrics, ArcHistogramSample, ArcHistogramSamples, ArcAdhocHistogramSample,
ArcAdhocHistogramSamples, ArcTracingConfiguration, ArcTracingController, ArcNetworkActivityMonitor,
ArcNetworkLoggingConfiguration, ArcURLLoadConfiguration, ArcURLLoadCancellationToken, ArcURLUtilities,
ArcGPULogMessage, ArcCrashKeysReporter, ArcPerformanceController, ArcBrowserApplicationDebugProperty,
ArcMemoryDumpMetric, ArcMemoryDumpProvider, ArcBreadcrumb, ArcErrorStatus, ArcImportSource,
ArcImportSourceProfile, ArcBookmarkImportTargets, ArcSpake2Context, ArcReference, ArcFreezingVoteToken,
ArcImage, ArcImageRep, ArcMediaImage (incl. +decodeFromData:... completion). Delegates: ArcBrowserApplicationDelegate
(GPU info/crash, network process, didCreateNewBrowser/didCloseBrowser come from core: coordinate),
ArcURLLoadDelegate, ArcTracingControllerDelegate, ArcHistogramSamplesDelegate, ArcCrashKeysObserver,
ArcPerformanceObserver, ArcAccessibilityObserver, ArcNotificationCenter (app service), ArcImportDelegate.

Note `-[ArcBrowserApplication init:]` already stores the app delegate (weak global in arc_browser_application.mm);
if you need it, ask the lead to expose it in arc_internal.h (Requests), don't edit that file.

Files you own: `arc/api/arc_app_*.mm` (create as needed), `arc/wp/app.gni`.

Acceptance: `arc/tools/diff-run.sh` (launch-slice): no missing/return/callback differences for your classes (the
golden has the full launch sequence including the URL loader fetching remote config, histograms, crash keys,
CPU info, import sources), then any app-level scenarios the "scenarios" package records.
