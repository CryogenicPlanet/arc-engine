# ArcBrowserContextDelegate

Profile lifecycle, DevTools/extension prompts, artifact policy and app product/chat services. Classification: `profile_service_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b0db0`; inherits `NSObject`. Pristine binary corroborates all 18 unique guide identities (20 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK28BrowserContextDelegateBridge`: class `0x10447da70`, class_ro `0x10447da10`, direct protocols `ArcBrowserContextPreferencesObserver;ArcBrowserContextDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Context delegate registration at `0x43ed9a8` has a weak-target bridge. Its nil path does not implement the same native unregister operation as WebContents navigation/UI setters; do not adopt one generic nil rule. The binary/body finding is independently rechecked in the focus evidence.

The declaration mixes notifications, synchronous BOOL/list queries, prompts with completion blocks, and app-owned product/chat streaming services. The optional `browserContext:diaResourceRequestedForType:resourceID:completion:` remains optional. The typed matrix preserves these differences; it does not infer schemas or an engine implementation of the app service.

For profile creation, the app copies completion in `0x1004af380` around its engine call. Engine `0x43e8368` reaches native `0x42b3d74/0x42b3f34`; the missing exact `0x43eb7a0` adapter blocks a complete completion/error/ownership contract.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 showDevTools:(ArcBrowserContextDevTools *)arg1 for:(NSString *)arg2;` | `v40@0:8@16@24@32` | `0x1004e104c` (entry `0x1035ff430`) | `0x43f689c` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didCloseDevTools:(NSString *)arg1;` | `v32@0:8@16@24` | `0x1004e13f4` (entry `0x1035ff43c`) | `0x43f6934` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 devToolsNeedsConfirmation:(NSString *)arg1 withMessage:(NSString *)arg2 completion:(void (^)(BOOL))arg3;` | `v48@0:8@16@24@32@?40` | `0x1004e148c` (entry `0x1035ff448`) | `0x43f6a58` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 extensionPermissionPrompt:(NSWindow *)arg1 extensionInfo:(ArcExtensionInfo *)arg2 withMessages:(NSArray *)arg3 completion:(void (^)(BOOL))arg4;` | `v56@0:8@16@24@32@40@?48` | `0x1004e1678` (entry `0x1035ff454`) | `0x43f6c64` |
| required | `- (void)didStartRestoringSession:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e0c90` (entry `0x1035ff418`) | `0x43f79f4` |
| required | `- (void)didFinishRestoringSession:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e0ce8` (entry `0x1035ff424`) | `0x43f7a48` |
| required | `- (void)didSkipRestoringSession:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e0c38` (entry `0x1035ff40c`) | `0x43f7a9c` |
| required | `- (BOOL)browserContext:(ArcBrowserContext *)arg0 shouldAllowArtifactNavigationToURL:(NSURL *)arg1 forSessionID:(NSString *)arg2 folderName:(NSString *)arg3;` | `B48@0:8@16@24@32@40` | `0x1004e1980` (entry `0x1035ff460`) | `0x43f7b68` |
| required | `- (NSArray *)browserContext:(ArcBrowserContext *)arg0 allowedArtifactImageSourcesForSessionID:(NSString *)arg1 folderName:(NSString *)arg2;` | `@40@0:8@16@24@32` | `0x1004e1cb4` (entry `0x1035ff46c`) | `0x43f7c6c` |
| required | `- (void)didDestroy:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e52b4` (entry `0x1035ff4d8`) | `0x43f7f1c` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 chatDataStreamOpenRequestedForChatId:(NSString *)arg1 update:(void (^)(NSString *))arg2 completion:(void (^)(NSError *))arg3;` | `v48@0:8@16@24@?32@?40` | `0x1004e2088` (entry `0x1035ff478`) | `0x43f6e20` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 chatDataStreamCloseRequestedForChatId:(NSString *)arg1 completion:(void (^)(NSError *))arg2;` | `v40@0:8@16@24@?32` | `0x1004e26f4` (entry `0x1035ff484`) | `0x43f6fa0` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 sendChatDataToChatId:(NSString *)arg1 data:(NSString *)arg2 completion:(void (^)(NSError *))arg3;` | `v48@0:8@16@24@32@?40` | `0x1004e2c70` (entry `0x1035ff490`) | `0x43f711c` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 productConnectionOpenRequestedForID:(NSString *)arg1 target:(NSData *)arg2 update:(void (^)(NSData *))arg3 completion:(void (^)(NSError *))arg4;` | `v56@0:8@16@24@32@?40@?48` | `0x1004e3608` (entry `0x1035ff49c`) | `0x43f72e8` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 productConnectionCloseRequestedForID:(NSString *)arg1 completion:(void (^)(NSError *))arg2;` | `v40@0:8@16@24@?32` | `0x1004e3e4c` (entry `0x1035ff4a8`) | `0x43f7474` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 productConnectionReceivedMessageForID:(NSString *)arg1 payload:(NSData *)arg2 completion:(void (^)(NSError *))arg3;` | `v48@0:8@16@24@32@?40` | `0x1004e44b0` (entry `0x1035ff4b4`) | `0x43f75e8` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 featureFlagsSubscribe:(void (^)(NSString *))arg1;` | `v32@0:8@16@?24` | `0x1004e49d8` (entry `0x1035ff4c0`) | `0x43f7944` |
| optional | `- (void)browserContext:(ArcBrowserContext *)arg0 diaResourceRequestedForType:(NSString *)arg1 resourceID:(NSString *)arg2 completion:(void (^)(NSData *))arg3;` | `v48@0:8@16@24@32@?40` | `0x1004e4dac` (entry `0x1035ff4cc`) | Uncorroborated in bounded scan |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
