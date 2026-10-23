"""Human-reviewed scope notes, deliberately narrower than implementation specs."""
PURPOSE={
'ArcAccessibilityObserver':('observer_callback','Accessibility-mode change delivery'),
'ArcBrowserApplicationDelegate':('lifecycle_callback','Application/browser process, network/GPU and browser lifecycle notifications'),
'ArcBrowserContextDelegate':('profile_service_callback','Profile lifecycle, DevTools/extension prompts, artifact policy and app product/chat services'),
'ArcBrowserContextDownloadsDelegate':('download_callback','Download initial snapshot, item deltas and app-selected filename completion'),
'ArcBrowserContextExtensionsDelegate':('extension_callback','Extension lifecycle, errors, pin ordering and extension-system readiness'),
'ArcBrowserContextHistoryDelegate':('history_callback','History deletion notification'),
'ArcBrowserContextPreferencesObserver':('preferences_observer','Preference/search-engine updates and search-engine addition origin'),
'ArcBrowserDelegate':('browser_callback','Content creation/restoration, extension popups/toolbar, tab groups and browser UI'),
'ArcBrowserSidePanelDelegate':('native_ui_callback','Side-panel visibility, title, close animation and unhide requests'),
'ArcCrashKeysObserver':('diagnostic_observer','Crash-key emission to app observer'),
'ArcDevToolsProtocolCommand':('non_callback_value_protocol','Readonly domain/name/params command value consumed by DevTools APIs'),
'ArcDevToolsProtocolEvent':('non_callback_value_protocol','Readonly domain/name event subscription descriptor; notification block is separate'),
'ArcHistogramSamplesDelegate':('diagnostic_callback','Periodic and ad hoc histogram sample deliveries'),
'ArcImportDelegate':('import_callback','Import lifecycle/category/session results and lock-dialog completion'),
'ArcNotificationCenter':('app_service_protocol','Engine requests app notification display and close; not an observer subscription'),
'ArcPerformanceObserver':('performance_observer','Battery saver mode notification'),
'ArcTracingControllerDelegate':('tracing_callback','Trace start, completed result and abort lifecycle'),
'ArcURLLoadDelegate':('streaming_callback','URL loading progress, headers, data, finish and failure'),
'ArcWebContentsBoostIDEDelegate':('webui_callback','Boost IDE lifecycle and command/JSON requests to app'),
'ArcWebContentsDevToolsDelegate':('devtools_ui_callback','DevTools frontend display, close, focus, confirmation and inspect mode'),
'ArcWebContentsMediaSessionDelegate':('media_callback','Media session state/actions/position, device state and native PiP UI'),
'ArcWebContentsNavigationDelegate':('synchronous_policy_callback','App returns navigation disposition immediately'),
'ArcWebContentsNavigationObserver':('navigation_observer','Provisional/committed/in-page navigation and failure notifications'),
'ArcWebContentsThumbnailsGeneratorObserver':('image_callback','Generated thumbnail result delivery'),
'ArcWebContentsTopBandColorDelegate':('color_callback','Top-band sampled color and crossfade hint'),
'ArcWebContentsUIDelegate':('ui_policy_callback','Content UI, permission/JavaScript dialogs, translation, authentication, artifacts and app service requests'),
}
NOTES={
'ArcBrowserApplicationDelegate':[
'Engine `0x43eb374` sends `didInitializeBrowserProcess` to a weak-loaded receiver; the actual app implementation is included in the method matrix. Native `0x42b1200` guards initialized state and calls `0x42b13c4`, whose body names `InvokeAsync` and `arc/common/utils/delegate_wrapper.h`. That supports an asynchronous native notification path, while the complete native-task/vtable-to-ObjC chain and runner identity remain unresolved.',
'Termination is a separate block API, not another guide protocol method. `tryToTerminate:` at `0x43e6ee0` retains block storage and reaches native `0x42b2430`; the independently read copy/destruct helpers `0x4044e80/0x4044ec0` contain retain/release instructions. Preserve immediate status separately from eventual completion. Exactly-once behavior, completion enum names and shutdown ordering remain open.',
'Profile creation is also a separate block API: `0x43e8368` → `0x42b3d74` → callback `0x42b3f34`, with ObjC adapter `0x43eb7a0` lacking an exact exported header. The full profile-creation result contract cannot be derived from these delegate declarations.'],
'ArcBrowserContextDelegate':[
'Context delegate registration at `0x43ed9a8` has a weak-target bridge. Its nil path does not implement the same native unregister operation as WebContents navigation/UI setters; do not adopt one generic nil rule. The binary/body finding is independently rechecked in the focus evidence.',
'The declaration mixes notifications, synchronous BOOL/list queries, prompts with completion blocks, and app-owned product/chat streaming services. The optional `browserContext:diaResourceRequestedForType:resourceID:completion:` remains optional. The typed matrix preserves these differences; it does not infer schemas or an engine implementation of the app service.',
'For profile creation, the app copies completion in `0x1004af380` around its engine call. Engine `0x43e8368` reaches native `0x42b3d74/0x42b3f34`; the missing exact `0x43eb7a0` adapter blocks a complete completion/error/ownership contract.'],
'ArcBrowserContextDownloadsDelegate':[
'Engine bridge bodies `0x404257c`, `0x4042610`, `0x404269c` construct `ArcDownloadItem` wrappers and send created/updated/removed notifications after loading weak receiver/context slots. The item wrapper identity does not establish snapshot immutability, post-removal validity or ordering relative to the initial list.',
'Filename selection is confirmed in instructions even though `0x4042728` has no exact corpus header. Vtable slot `0xdd2bfe0` points there; selector load `0x40427fc/0x4042800` precedes send `0x404281c`. Instruction `0x40427b0` converts native uint32 to double, carried in d0. Extended app metadata declares `(ArcBrowserContext *, ArcDownloadItem *, NSString *, double, void (^)(NSString *))`. The double is a timeout by selector name; its unit/default and late-result behavior are unresolved.',
'App filename implementation `0x1004e9748` copies the completion before forwarding. Helper `0x1004e9ac4` invokes the default filename when its own weak delegate is absent; `0x1004e9da8` bridges a chosen string and invokes completion. This app-side fallback is distinct from expiration of the engine bridge weak target.',
'Setter `0x4041fec` weakly installs a nonnil target; nil takes a cleanup path without clearing that target/native registration. Exact teardown, queued update suppression and exactly-once filename completion remain unknown.'],
'ArcWebContentsNavigationDelegate':[
'Bridge `0x4445a58` wraps `ArcNavigationAction`, loads weak receiver/owner and sends `webContents:decidePolicyForNavigationAction:` at `0x4445abc`. `cmp x0,#12` at `0x4445ac0` followed by unsigned `b.hs` rejects values outside 0–11. The return is consumed immediately, so this boundary is synchronous; the metadata signed int64 return is not an asynchronous completion.',
'App implementation `0x100535984` forwards to `0x10053605c`; `0x100536028` supplies a mapping table on some paths. Policy ordinal names are unresolved. Chromium enum resemblance is not sufficient to name them.',
'Setter `0x4440dec` holds weak self/delegate in a shared bridge. Nil calls native `0x4375b40` with zero registration arguments. Clearing native registration does not prove destruction of the wrapper-held bridge or cancellation of tasks already carrying it.'],
'ArcWebContentsUIDelegate':[
'Actual bridge registration `0x444112c` constructs a shared native bridge, stores weak owner/delegate, and stores vtable `0xda89030`. Its nil path clears native registration through `0x4375cdc`. This does not establish app dialog lifetime, in-flight suppression or callback thread identity.',
'The vtable exposes missing exact exported bodies for JavaScript alert `0x44462cc`, confirm `0x4446470`, text input `0x4446608`, before-unload `0x4446944`, and permission prompt `0x4446bc8`. Independent instructions and selector references recover the sends. These are function-boundary/export gaps, not absent shards.',
'Extended app metadata declares alert completion `void (^)(void)`, confirm/before-unload `void (^)(BOOL)`, text input `void (^)(NSString *)`, and permission completion `void (^)(uint64_t)`. At `0x4446dcc`, permission response values 0–2 pass through and all larger unsigned values become 3 (`cmp x1,#3`, `csel ... lo`, then native indirect invocation). Prompt quiet-UI reason input is normalized at `0x4446cc0–0x4446ccc`: 1–5 remain, others become 0. Enum names remain unestablished.',
'Alert and confirm paths explicitly retain a constructed block before the send, then release the local owned reference. Permission passes a stack block containing copied native callable state. The app must preserve a completion used later; neither these instructions nor type annotations establish exactly-once invocation, cancellation, timeout or main-thread delivery.',
'This protocol has 65 unique identities. Notifications, synchronous BOOL decisions, object/dialog commands, completion requests and chat stream callbacks are separate shapes. The table gives typed provenance for the remaining surface; behavior beyond cited bridges is intentionally unresolved.'],
'ArcDevToolsProtocolCommand':[
'This protocol is a value contract, not three engine-to-app event callbacks. The app CommandShim adopts it. Engine `ArcWebContentsDevTools::execute:completion:error:` (`0x4415520`) reads command domain/name/params and feeds the native command path `0x435b114` (bounded bodies are retained). The actual command result completion is a different block interface.',
'All three getters return NSString according to the extended type array. Params being represented as a string does not establish accepted JSON grammar, error catalog, command coverage or serialization ownership.'],
'ArcDevToolsProtocolEvent':[
'This protocol is a subscription descriptor, not two notifications. App EventShim supplies readonly NSString domain/name. Engine `subscribe:autoEnable:notification:` at `0x4415838` consumes the descriptor; actual event delivery uses its separate notification block.',
'Do not count descriptor getter sends as CDP events. Lifetime of subscription IDs, unsubscription races, notification errors/queues and event JSON schema remain outside this declaration audit.'],
'ArcWebContentsDevToolsDelegate':[
'This is the DevTools frontend/UI callback interface, distinct from the command/event values and raw protocol connection blocks. Short bridges `0x4415fe0`, `0x441604c`, `0x44160b8`, `0x441610c` cover display, inspect change, close and focus. Weak loads are visible in the actual bridge instructions/source candidates.',
'The separate raw transport API `0x4415b60` has a retained callback adapter at headerless `0x4416690`: the completed ABI report corroborates message-or-error delivery. That block signature, 8 MiB transport limit and queued close notifications must not be attributed to these five protocol methods. Scheduling identity and frontend view ownership remain unresolved.'],
'ArcWebContentsMediaSessionDelegate':[
'Bridge `0x441c08c` allocates `ArcMediaPosition`, initializes it from native position state, loads weak target/owner and sends `webContents:didChangeMediaPosition:`. The bounded instruction/body evidence corroborates this local conversion, not position time units or update ordering.',
'The matrix preserves integer session/camera/microphone states, BOOL feature flags, media actions, native PiP sizes, native controller objects and title/icon values as distinct types. Native PiP view parenting, unavailable-session behavior and callback ordering need the media class findings plus runtime validation; no universal main-thread rule follows.'],
'ArcWebContentsThumbnailsGeneratorObserver':[
'Bridge `0x441cccc` weak-loads receiver/owner and sends `webContents:thumbnailReady:`. The source explicitly selects nil when native image storage is empty, otherwise reaches conversion helper `0x2a02ccc`; instructions preserve that branch. Nil is therefore a possible local bridge result, despite absent nullability in the encoding.',
'Registration `0x441c680` creates weak observer/owner captures. Thumbnail scheduling, request-to-result identity, coalescing, image ownership after removal and callback sequence remain open. The class report ties requests to a ThumbnailTabHelper data key; this audit does not treat that as proof of unchanged upstream behavior.'],
'ArcWebContentsNavigationObserver':[
'The navigation observer has eight unique methods, separate from the synchronous navigation-policy delegate. The binary/source send evidence identifies distinct provisional/commit/finish/failure and in-page selectors; it does not establish their full ordering or exactly-once behavior.',
'The completed class review shows add/remove at `0x4416eac/0x4417210` deduplicate and unregister weak bridges. Weak loads and local sends cannot establish suppression of already queued work after removal or observer destruction.'],
'ArcBrowserDelegate':[
'The surface includes object notifications and completion-based tab-group/app services; `browser:extensionGroupIdForTabWithSessionId:` is a return-value query. The extended encodings preserve these distinctions. Sender and app adopter evidence are given per method; native window/content ownership is not inferred from method names.',
'Browser delegate creation is separately evidenced at `0x43e1284` in the class review. Actual content factories and take/restore routes have different native paths. Callback object transfer, tab-group error conventions and callback ordering remain unresolved here.'],
'ArcBrowserSidePanelDelegate':[
'The four methods are app UI requests/notifications, including BOOL animation control. The completed side-panel class review distinguishes close request `0x4401170` from app completion `0x44011a4`; these methods cannot be reduced to a single synchronous close operation.',
'Native panel view ownership, which completion ends the close transition, focus transfer and AppKit thread/reentrancy constraints remain unverified.'],
'ArcURLLoadDelegate':[
'Two independently discovered app adopters implement this protocol: URLLoaderClientImpl.DataDelegate and StreamDelegate. The ADK-only historical callback inventory missed these classes. The implementation table retains each class/method entry/IMP independently.',
'The declaration distinguishes double progress, final URL/HTTP code/headers, data chunks, finish and NSError failure. It does not specify data buffering, backpressure, cancellation ordering, terminal-event cardinality or delivery queue. NSData ownership after return is not derived from its type.'],
'ArcBrowserContextExtensionsDelegate':[
'The app adopter is ADKClientLive.BrowserExtensionsLifeCycleManager, outside the prior ADK-only prefix filter. Raw metadata identifies required lifecycle/error methods and optional pinned-order/readiness methods. Preserve the exact readiness selector `browserContextDidExtensionSystemReady:`; English descriptions are not selector identities.',
'Required metadata does not prove all callbacks are invoked. Dynamic receiver availability checks, category bridge teardown, errors during install/uninstall and callback ordering remain unverified.'],
'ArcBrowserContextPreferencesObserver':[
'Two app IMPs have no exact exported app body: `browserContextPreferencesDidUpdate:` at `0x1004e82ec` and `browserContextPreferencesDidUpdateSearchEngines:` at `0x1004e82e8`. Independent branch decoding shows both are veneers to shared app implementation `0x1004e530c`, whose exact body is retained. Their entries are branches, not RET-only leaves; substantive handling by the shared routine is not established here. Metadata identities remain separate despite the shared target.',
'The third unique method carries an added-search-engine object and a signed integer origin. Duplicate protocol rows have identical raw/extended encodings. Search-engine identity, origin enum names, update ordering and observer teardown are unresolved.'],
'ArcNotificationCenter':[
'ADKApplication supplies display: and close:. This is an app service invoked by engine notification plumbing, rather than an engine notification object/value protocol. Exact ArcNotification object types come from the extended encodings.',
'The older encoding audit independently resolves ArcNotification.browserContextQualifiedID; that equality says nothing about notification presentation, user responses, callback identity or cancellation behavior.'],
}
DEFAULT_LIMIT='Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.'
NEXT={
'ArcAccessibilityObserver':'Which accessibility modes and observer removal semantics must be preserved?',
'ArcBrowserContextHistoryDelegate':'What deletion payload schema and ordering relative to persistence are required?',
'ArcBrowserContextPreferencesObserver':'What distinguishes update/search-engine callbacks, and which origin enum values are valid?',
'ArcCrashKeysObserver':'Are key/value callbacks synchronous, copied, bounded or filtered during shutdown?',
'ArcHistogramSamplesDelegate':'Which snapshot schema, sampling interval, ownership and sequence are required?',
'ArcImportDelegate':'How do category failures, importer cancellation and lock-dialog completion interact?',
'ArcPerformanceObserver':'Which battery-saver state enum values, initial-delivery and removal rules apply?',
'ArcTracingControllerDelegate':'Can start/complete/abort race with stop or destruction, and who owns the result?',
'ArcWebContentsBoostIDEDelegate':'What command/JSON schema, WebUI readiness and app completion route are required?',
'ArcWebContentsTopBandColorDelegate':'Which color space, nil policy, sample timing and crossfade semantics apply?',
}
