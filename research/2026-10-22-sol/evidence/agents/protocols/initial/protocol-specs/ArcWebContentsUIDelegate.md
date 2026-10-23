# ArcWebContentsUIDelegate

Content UI, permission/JavaScript dialogs, translation, authentication, artifacts and app service requests. Classification: `ui_policy_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b23f8`; inherits `NSObject`. Pristine binary corroborates all 65 unique guide identities (77 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK18WebContentChromium`: class `0x10447e518`, class_ro `0x10447e4b0`, direct protocols `ArcWebContentsTopBandColorDelegate;ArcWebContentsThumbnailsGeneratorObserver;ArcWebContentsUIDelegate;ArcWebContentsNavigationObserver;ArcWebContentsNavigationDelegate;ArcWebContentsMediaSessionDelegate;ArcWebContentsDevToolsDelegate;ArcWebContentsBoostIDEDelegate`.
- `_TtC3ADK28TablessWebContentsUIDelegate`: class `0x10447eec8`, class_ro `0x10447ee68`, direct protocols `ArcWebContentsUIDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Actual bridge registration `0x444112c` constructs a shared native bridge, stores weak owner/delegate, and stores vtable `0xda89030`. Its nil path clears native registration through `0x4375cdc`. This does not establish app dialog lifetime, in-flight suppression or callback thread identity.

The vtable exposes missing exact exported bodies for JavaScript alert `0x44462cc`, confirm `0x4446470`, text input `0x4446608`, before-unload `0x4446944`, and permission prompt `0x4446bc8`. Independent instructions and selector references recover the sends. These are function-boundary/export gaps, not absent shards.

Extended app metadata declares alert completion `void (^)(void)`, confirm/before-unload `void (^)(BOOL)`, text input `void (^)(NSString *)`, and permission completion `void (^)(uint64_t)`. At `0x4446dcc`, permission response values 0–2 pass through and all larger unsigned values become 3 (`cmp x1,#3`, `csel ... lo`, then native indirect invocation). Prompt quiet-UI reason input is normalized at `0x4446cc0–0x4446ccc`: 1–5 remain, others become 0. Enum names remain unestablished.

Alert and confirm paths explicitly retain a constructed block before the send, then release the local owned reference. Permission passes a stack block containing copied native callable state. The app must preserve a completion used later; neither these instructions nor type annotations establish exactly-once invocation, cancellation, timeout or main-thread delivery.

This protocol has 65 unique identities. Notifications, synchronous BOOL decisions, object/dialog commands, completion requests and chat stream callbacks are separate shapes. The table gives typed provenance for the remaining surface; behavior beyond cited bridges is intentionally unresolved.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)webContentsActivate:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100538604` (entry `0x1035ff01c`); `0x10055e3bc` (entry `0x1035ff6b8`) | `0x4445be4` |
| required | `- (void)webContentsSelect:(ArcWebContents *)arg0 shouldActivate:(BOOL)arg1;` | `v28@0:8@16B24` | `0x1005386ac` (entry `0x1035ff028`); `0x10055e3c0` (entry `0x1035ff6c4`) | `0x4445c44` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didCloseWithSessionID:(ArcSessionID *)arg1;` | `v32@0:8@16@24` | `0x100538914` (entry `0x1035ff04c`); `0x10055e3c4` (entry `0x1035ff6d0`) | `0x4445d54` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didCreateNewWebContent:(ArcWebContents *)arg1 targetURL:(NSURL *)arg2 disposition:(int64_t)arg3;` | `v48@0:8@16@24@32q40` | `0x100538768` (entry `0x1035ff034`); `0x10055e3c8` (entry `0x1035ff6dc`) | `0x444601c` |
| required | `- (void)webContentsDidRequestEnterFullscreen:(ArcWebContents *)arg0 withDisplayId:(int64_t)arg1 requestingOrigin:(NSString *)arg2;` | `v40@0:8@16q24@32` | `0x1005389e8` (entry `0x1035ff058`); `0x10055e498` (entry `0x1035ff718`) | `0x4445e78` |
| required | `- (void)webContentsDidRequestExitFullscreen:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100538b00` (entry `0x1035ff064`); `0x10055e49c` (entry `0x1035ff724`) | `0x4445ed8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangePointerLock:(BOOL)arg1 requestingOrigin:(NSString *)arg2;` | `v36@0:8@16B24@28` | `0x100538ba8` (entry `0x1035ff070`); `0x10055e324` (entry `0x1035ff514`) | `0x4445f64` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeStatusText:(NSString *)arg1;` | `v32@0:8@16@24` | `0x100538880` (entry `0x1035ff040`); `0x10055e4ac` (entry `0x1035ff754`) | `0x44462a4` |
| required | `- (BOOL)webContents:(ArcWebContents *)arg0 willShowContextMenu:(ArcMenuModel *)arg1 withElement:(ArcContextMenuElementInfo *)arg2;` | `B40@0:8@16@24@32` | `0x10053850c` (entry `0x1035ff010`); `0x10055e350` (entry `0x1035ff598`) | `0x44461fc` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didSubmitAddressEntry:(ArcAddressEntrySaveRequest *)arg1;` | `v32@0:8@16@24` | `0x100537fe4` (entry `0x1035fefc8`); `0x10055e378` (entry `0x1035ff604`) | `0x4445cc8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didSubmitCredentials:(ArcLoginCredentialsSaveRequest *)arg1;` | `v32@0:8@16@24` | `0x1005383cc` (entry `0x1035feff8`); `0x10055e358` (entry `0x1035ff5a4`) | `0x4446144` |
| required | `- (void)webContentsClosingImminent:(ArcWebContents *)arg0 withSessionID:(ArcSessionID *)arg1;` | `v32@0:8@16@24` | `0x100538438` (entry `0x1035ff004`); `0x10055e35c` (entry `0x1035ff5b0`) | `0x44460b8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didSubmitCreditCardInfo:(ArcCreditCardInfoSaveRequest *)arg1;` | `v32@0:8@16@24` | `0x100537e34` (entry `0x1035fefbc`); `0x10055e37c` (entry `0x1035ff610`) | `0x4445de0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willRunJavaScriptAlertPanel:(NSString *)arg1 withMessage:(NSString *)arg2 completion:(void (^)())arg3;` | `v48@0:8@16@24@32@?40` | `0x100538c84` (entry `0x1035ff07c`); `0x10055e328` (entry `0x1035ff520`) | `0x44463d0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willRunJavaScriptConfirmPanel:(NSString *)arg1 withMessage:(NSString *)arg2 completion:(void (^)(BOOL))arg3;` | `v48@0:8@16@24@32@?40` | `0x100538dcc` (entry `0x1035ff088`); `0x10055e32c` (entry `0x1035ff52c`) | `0x4446574` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willRunJavaScriptTextInputPanel:(NSString *)arg1 withMessage:(NSString *)arg2 withDefaultText:(NSString *)arg3 completion:(void (^)(NSString *))arg4;` | `v56@0:8@16@24@32@40@?48` | `0x100538f14` (entry `0x1035ff094`); `0x10055e330` (entry `0x1035ff538`) | `0x4446738` |
| required | `- (void)webContentsWillCloseJavaScriptPanel:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x1005390e4` (entry `0x1035ff0a0`); `0x10055e33c` (entry `0x1035ff55c`) | `0x4446928` |
| required | `- (void)webContents:(ArcWebContents *)arg0 finalizeDiscardRequest:(BOOL)arg1;` | `v28@0:8@16B24` | `0x1005393c0` (entry `0x1035ff0c4`); `0x10055e338` (entry `0x1035ff550`) | `0x4446b54` |
| required | `- (void)webContents:(ArcWebContents *)arg0 runJavaScriptApplicationModalBeforeUnloadPanel:(NSString *)arg1 withMessage:(NSString *)arg2 withPrimaryButtonLabel:(NSString *)arg3 completion:(void (^)(BOOL))arg4;` | `v56@0:8@16@24@32@40@?48` | `0x10053918c` (entry `0x1035ff0ac`); `0x10055e334` (entry `0x1035ff544`) | `0x4446a74` |
| required | `- (void)webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100539318` (entry `0x1035ff0b8`); `0x10055e38c` (entry `0x1035ff640`) | `0x4446bac` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeVisibleSecurityState:(ArcSecureContext *)arg1;` | `v32@0:8@16@24` | `0x1005381c8` (entry `0x1035fefec`); `0x10055e4a8` (entry `0x1035ff748`) | `0x4446e9c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willAskForPermission:(ArcDisplayNameOrOrigin *)arg1 withRequests:(NSArray *)arg2 withQuietUIReason:(uint64_t)arg3 completion:(void (^)(uint64_t))arg4;` | `v56@0:8@16@24@32Q40@?48` | `0x100538050` (entry `0x1035fefd4`); `0x10055e484` (entry `0x1035ff6e8`) | `0x4446d38` |
| required | `- (void)webContentsClosePermission:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100538120` (entry `0x1035fefe0`); `0x10055e488` (entry `0x1035ff6f4`) | `0x4446e1c` |
| required | `- (BOOL)webContents:(ArcWebContents *)arg0 canExternallyNavigate:(uint64_t)arg1;` | `B32@0:8@16Q24` | `0x10053947c` (entry `0x1035ff0d0`); `0x10055e48c` (entry `0x1035ff700`) | `0x4446f1c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willExternallyNavigate:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x100539548` (entry `0x1035ff0dc`); `0x10055e494` (entry `0x1035ff70c`) | `0x4446fa4` |
| required | `- (BOOL)webContentsWantsOverscrollSwipeEvents:(ArcWebContents *)arg0;` | `B24@0:8@16` | `0x100536c38` (entry `0x1035fee90`); `0x10055e390` (entry `0x1035ff64c`) | `0x4447004` |
| required | `- (void)webContents:(ArcWebContents *)arg0 askForExtensionInstallationConsent:(ArcExtensionInfo *)arg1 completion:(void (^)(BOOL))arg2;` | `v40@0:8@16@24@?32` | `0x100537ac0` (entry `0x1035fefa4`); `0x10055e4a0` (entry `0x1035ff730`) | `0x4447100` |
| required | `- (void)webContents:(ArcWebContents *)arg0 askForExtensionUninstallationConsent:(ArcExtensionInfo *)arg1 completion:(void (^)(BOOL))arg2;` | `v40@0:8@16@24@?32` | `0x100537bd8` (entry `0x1035fefb0`); `0x10055e4a4` (entry `0x1035ff73c`) | `0x4447258` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeAudibleState:(BOOL)arg1;` | `v28@0:8@16B24` | `0x100537a98` (entry `0x1035fef8c`); `0x10055e348` (entry `0x1035ff580`) | `0x4447324` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeMuteState:(BOOL)arg1;` | `v28@0:8@16B24` | `0x100537aac` (entry `0x1035fef98`); `0x10055e34c` (entry `0x1035ff58c`) | `0x4447388` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showWebUIDialog:(ArcWebDialog *)arg1;` | `v32@0:8@16@24` | `0x100537854` (entry `0x1035fef68`); `0x10055e360` (entry `0x1035ff5bc`) | `0x444740c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didCloseWebUIDialog:(ArcWebDialog *)arg1;` | `v32@0:8@16@24` | `0x100537918` (entry `0x1035fef74`); `0x10055e364` (entry `0x1035ff5c8`) | `0x4447498` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didCaptureImage:(NSImage *)arg1;` | `v32@0:8@16@24` | `0x100539604` (entry `0x1035ff0e8`); `0x10055e4bc` (entry `0x1035ff784`) | `0x4447528` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showsAuxiliaryWindow:(NSWindow *)arg1;` | `v32@0:8@16@24` | `0x10053749c` (entry `0x1035fef2c`); `0x10055e4c0` (entry `0x1035ff790`) | `0x444759c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showExternalProtocolDialog:(ArcExternalProtocolDialog *)arg1;` | `v32@0:8@16@24` | `0x100537790` (entry `0x1035fef5c`); `0x10055e370` (entry `0x1035ff5ec`) | `0x44479fc` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showWebLoginDialog:(ArcWebLoginDialog *)arg1;` | `v32@0:8@16@24` | `0x1005376cc` (entry `0x1035fef50`); `0x10055e374` (entry `0x1035ff5f8`) | `0x4447a88` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didAutoFillWebLoginDialog:(ArcLoginCredentials *)arg1;` | `v32@0:8@16@24` | `0x100537560` (entry `0x1035fef38`); `0x10055e368` (entry `0x1035ff5d4`) | `0x4447b14` |
| required | `- (void)webContentsDidCloseWebLoginDialog:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100537624` (entry `0x1035fef44`); `0x10055e36c` (entry `0x1035ff5e0`) | `0x4447b74` |
| required | `- (void)webContents:(ArcWebContents *)arg0 hasBlockedPopupWindowsChanged:(BOOL)arg1;` | `v28@0:8@16B24` | `0x100539794` (entry `0x1035ff100`); `0x10055e4c8` (entry `0x1035ff7a8`) | `0x4447bd4` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didConsumeKey:(int)arg1 withModifierFlags:(int)arg2 underlyingEventType:(uint64_t)arg3;` | `v40@0:8@16i24i28Q32` | `0x100536e50` (entry `0x1035feecc`); `0x10055e344` (entry `0x1035ff574`) | `0x4413c34` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showTranslateBubble:(NSString *)arg1 withTargetLanguage:(NSString *)arg2;` | `v40@0:8@16@24@32` | `0x100536f1c` (entry `0x1035feed8`); `0x10055e398` (entry `0x1035ff658`) | `0x4447650` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showTranslateError:(int)arg1;` | `v28@0:8@16i24` | `0x10053701c` (entry `0x1035feee4`); `0x10055e39c` (entry `0x1035ff664`) | `0x44476c4` |
| required | `- (void)webContentsShowTranslateInProgress:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x1005370d8` (entry `0x1035feef0`); `0x10055e3a0` (entry `0x1035ff670`) | `0x444771c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showTranslateComplete:(NSString *)arg1;` | `v32@0:8@16@24` | `0x100537180` (entry `0x1035feefc`); `0x10055e4b4` (entry `0x1035ff76c`) | `0x44477a0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didDetermineSourceLanguage:(NSString *)arg1 isTranslateEnabled:(BOOL)arg2 targetLanguage:(NSString *)arg3 isTranslationPending:(BOOL)arg4;` | `v48@0:8@16@24B32@36B44` | `0x100537254` (entry `0x1035fef08`); `0x10055e3a4` (entry `0x1035ff67c`) | `0x4447870` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onTranslateCompleteToLanguage:(NSString *)arg1;` | `v32@0:8@16@24` | `0x1005373c8` (entry `0x1035fef20`); `0x10055e4b8` (entry `0x1035ff778`) | `0x444790c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onTranslateError:(int)arg1;` | `v28@0:8@16i24` | `0x10053730c` (entry `0x1035fef14`); `0x10055e3a8` (entry `0x1035ff688`) | `0x4447978` |
| required | `- (BOOL)webContents:(ArcWebContents *)arg0 showHungRendererDialog:(ArcHungRendererDialog *)arg1;` | `B32@0:8@16@24` | `0x100536e48` (entry `0x1035feec0`); `0x10055e3b4` (entry `0x1035ff6ac`) | `0x4447c58` |
| required | `- (void)webContentsHideHungRendererDialog:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100536cf4` (entry `0x1035fee9c`); `0x10055e3ac` (entry `0x1035ff694`) | `0x4447cc4` |
| required | `- (void)webContentsProcessGone:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100536cf8` (entry `0x1035feea8`); `0x10055e3b0` (entry `0x1035ff6a0`) | `0x4447d18` |
| required | `- (void)webContentsRenderViewReady:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100536da0` (entry `0x1035feeb4`); `0x10055e320` (entry `0x1035ff508`) | `0x4447d6c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onHighCPUUsageWhileInBackground:(double)arg1 intervalDurationSeconds:(unsigned int)arg2;` | `v36@0:8@16d24I32` | `0x1005396c8` (entry `0x1035ff0f4`); `0x10055e380` (entry `0x1035ff61c`) | `0x4447dd8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onShareThisTabStateChanged:(BOOL)arg1;` | `v28@0:8@16B24` | `0x1005379dc` (entry `0x1035fef80`); `0x10055e340` (entry `0x1035ff568`) | `0x4447fd0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onCopyLinkToContextMenu:(NSString *)arg1;` | `v32@0:8@16@24` | `0x100539820` (entry `0x1035ff10c`); `0x10055e4b0` (entry `0x1035ff760`) | `0x4447f64` |
| required | `- (void)webContents:(ArcWebContents *)arg0 showDevToolsToolbox:(ArcWebContents *)arg1;` | `v32@0:8@16@24` | `0x100539890` (entry `0x1035ff118`); `0x10055e384` (entry `0x1035ff628`) | `0x4447e60` |
| required | `- (void)webContents:(ArcWebContents *)arg0 updateDevToolsToolbox:(CGRect)arg1;` | `v56@0:8@16{CGRect={CGPoint=dd}{CGSize=dd}}24` | `0x100539954` (entry `0x1035ff124`); `0x10055e388` (entry `0x1035ff634`) | `0x4447edc` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didFinishPDFRendering:(ArcWebContentsPDFController *)arg1;` | `v32@0:8@16@24` | `0x100539a2c` (entry `0x1035ff130`); `0x10055e4c4` (entry `0x1035ff79c`) | `0x4448070` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didShowWebUI:(NSURL *)arg1;` | `v32@0:8@16@24` | `0x100539ad4` (entry `0x1035ff13c`); `0x10055e4cc` (entry `0x1035ff7b4`) | `0x44480f4` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didDestroyWebUI:(NSURL *)arg1 newURL:(NSURL *)arg2;` | `v40@0:8@16@24@32` | `0x100539c1c` (entry `0x1035ff148`); `0x10055e588` (entry `0x1035ff7c0`) | `0x4448188` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willShowConfirmInfobar:(ArcConfirmInfobar *)arg1;` | `v32@0:8@16@24` | `0x100539ff0` (entry `0x1035ff160`); `0x10055e788` (entry `0x1035ff7d8`) | `0x444821c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 willCloseConfirmInfobar:(ArcConfirmInfobar *)arg1;` | `v32@0:8@16@24` | `0x10053a058` (entry `0x1035ff16c`); `0x10055e78c` (entry `0x1035ff7e4`) | `0x44482a8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 promptForArtifactLinkPermissionToURL:(NSURL *)arg1 willNavigate:(BOOL)arg2 forSessionID:(NSString *)arg3 folderName:(NSString *)arg4 completion:(void (^)(BOOL))arg5;` | `v60@0:8@16@24B32@36@44@?52` | `0x100539dd0` (entry `0x1035ff154`); `0x10055e6f0` (entry `0x1035ff7cc`) | `0x4413d9c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onFrameToChatAssociationRequestForChatId:(NSString *)arg1 frameId:(int)arg2 updateCallback:(void (^)(NSString *))arg3 completion:(void (^)(NSError *))arg4;` | `v52@0:8@16@24i32@?36@?44` | `0x10053a100` (entry `0x1035ff178`); `0x10055e7f8` (entry `0x1035ff7f0`) | `0x4448404` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onFrameToChatDisassociationRequestForChatId:(NSString *)arg1 frameId:(int)arg2 completion:(void (^)(NSError *))arg3;` | `v44@0:8@16@24i32@?36` | `0x10053a354` (entry `0x1035ff184`); `0x10055e9e0` (entry `0x1035ff7fc`) | `0x44485a8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 onSendChatDataToFrameForChatId:(NSString *)arg1 frameId:(int)arg2 data:(NSString *)arg3 completion:(void (^)(NSError *))arg4;` | `v52@0:8@16@24i32@36@?44` | `0x10053a528` (entry `0x1035ff190`); `0x10055ec1c` (entry `0x1035ff808`) | `0x4448740` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
