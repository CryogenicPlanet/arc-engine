# ArcBrowserDelegate

Content creation/restoration, extension popups/toolbar, tab groups and browser UI. Classification: `browser_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b1ca0`; inherits `NSObject`. Pristine binary corroborates all 19 unique guide identities (22 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK15ChromiumBrowser`: class `0x10447de98`, class_ro `0x10447de38`, direct protocols `ArcBrowserDelegate;ArcBrowserSidePanelDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

The surface includes object notifications and completion-based tab-group/app services; `browser:extensionGroupIdForTabWithSessionId:` is a return-value query. The extended encodings preserve these distinctions. Sender and app adopter evidence are given per method; native window/content ownership is not inferred from method names.

Browser delegate creation is separately evidenced at `0x43e1284` in the class review. Actual content factories and take/restore routes have different native paths. Callback object transfer, tab-group error conventions and callback ordering remain unresolved here.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)browser:(ArcBrowser *)arg0 didOpenExtensionPopup:(NSString *)arg1 webContents:(ArcWebContents *)arg2;` | `v40@0:8@16@24@32` | `0x100529ff0` (entry `0x1035fed7c`) | `0x43e35a4` |
| required | `- (void)browser:(ArcBrowser *)arg0 didResizeExtensionPopup:(NSString *)arg1 size:(CGSize)arg2;` | `v48@0:8@16@24{CGSize=dd}32` | `0x100529cf0` (entry `0x1035fed64`) | `0x43e3650` |
| required | `- (void)browser:(ArcBrowser *)arg0 didCloseExtensionPopup:(NSString *)arg1;` | `v32@0:8@16@24` | `0x100529e78` (entry `0x1035fed70`) | `0x43e36e4` |
| required | `- (void)browser:(ArcBrowser *)arg0 didUpdateExtensionToolbarAction:(ArcExtensionToolbarAction *)arg1;` | `v32@0:8@16@24` | `0x10052a1b8` (entry `0x1035fed88`) | `0x43e3770` |
| required | `- (void)browser:(ArcBrowser *)arg0 didUpdateExtensionToolbarActions:(NSArray *)arg1;` | `v32@0:8@16@24` | `0x100529b50` (entry `0x1035fed58`) | `0x43e3860` |
| required | `- (void)browser:(ArcBrowser *)arg0 didCreateNewWebContents:(ArcWebContents *)arg1 isActive:(BOOL)arg2;` | `v36@0:8@16@24B32` | `0x100529a48` (entry `0x1035fed4c`) | `0x43e3430` |
| required | `- (void)browser:(ArcBrowser *)arg0 didRestoreWebContents:(ArcWebContents *)arg1 withSessionId:(ArcSessionID *)arg2;` | `v40@0:8@16@24@32` | `0x1005299b0` (entry `0x1035fed40`) | `0x43e34e4` |
| required | `- (void)browser:(ArcBrowser *)arg0 didChangeFullscreenState:(BOOL)arg1 initiatedBy:(int64_t)arg2;` | `v36@0:8@16B24q28` | `0x10052a2e8` (entry `0x1035fed94`) | `0x43e38e4` |
| required | `- (void)browser:(ArcBrowser *)arg0 didAskAbout:(NSString *)arg1;` | `v32@0:8@16@24` | `0x10052a3e8` (entry `0x1035feda0`) | `0x43e3964` |
| required | `- (void)browser:(ArcBrowser *)arg0 didAskAboutImage:(NSURL *)arg1;` | `v32@0:8@16@24` | `0x10052a548` (entry `0x1035fedac`) | `0x43e39e0` |
| required | `- (void)browser:(ArcBrowser *)arg0 didRequestExtensionPermissions:(ArcExtensionInfo *)arg1 completion:(void (^)(BOOL))arg2;` | `v40@0:8@16@24@?32` | `0x10052a704` (entry `0x1035fedb8`) | `0x43e3ad0` |
| required | `- (void)browser:(ArcBrowser *)arg0 createTabGroupWithSessionIds:(NSArray *)arg1 properties:(ArcTabGroupProperties *)arg2 completion:(void (^)(ArcTabGroupResult *))arg3;` | `v48@0:8@16@24@32@?40` | `0x10052aab0` (entry `0x1035fedc4`) | `0x43e3cac` |
| required | `- (void)browser:(ArcBrowser *)arg0 addTabsToGroupWithId:(NSString *)arg1 sessionIds:(NSArray *)arg2 completion:(void (^)(ArcTabGroupResult *))arg3;` | `v48@0:8@16@24@32@?40` | `0x10052ae08` (entry `0x1035fedd0`) | `0x43e3fc8` |
| required | `- (void)browser:(ArcBrowser *)arg0 removeTabsFromGroupWithSessionIds:(NSArray *)arg1 completion:(void (^)(ArcTabGroupResult *))arg2;` | `v40@0:8@16@24@?32` | `0x10052b150` (entry `0x1035feddc`) | `0x43e42b8` |
| required | `- (void)browser:(ArcBrowser *)arg0 updateTabGroupWithId:(NSString *)arg1 properties:(ArcTabGroupProperties *)arg2 completion:(void (^)(ArcTabGroupResult *))arg3;` | `v48@0:8@16@24@32@?40` | `0x10052b3a4` (entry `0x1035fede8`) | `0x43e457c` |
| required | `- (void)browser:(ArcBrowser *)arg0 moveTabGroupWithId:(NSString *)arg1 toNewIndex:(int64_t)arg2 completion:(void (^)(ArcTabGroupResult *))arg3;` | `v48@0:8@16@24q32@?40` | `0x10052b5e0` (entry `0x1035fedf4`) | `0x43e4828` |
| required | `- (void)browser:(ArcBrowser *)arg0 getTabGroupWithId:(NSString *)arg1 completion:(void (^)(ArcTabGroupInfo *))arg2;` | `v40@0:8@16@24@?32` | `0x10052b80c` (entry `0x1035fee00`) | `0x43e4ab8` |
| required | `- (void)browser:(ArcBrowser *)arg0 getAllTabGroupsWithCompletion:(void (^)(NSArray *))arg1;` | `v32@0:8@16@?24` | `0x10052bbcc` (entry `0x1035fee0c`) | `0x43e4c30` |
| required | `- (int64_t)browser:(ArcBrowser *)arg0 extensionGroupIdForTabWithSessionId:(int64_t)arg1;` | `q32@0:8@16q24` | `0x10052be84` (entry `0x1035fee18`) | `0x43e4e1c` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
