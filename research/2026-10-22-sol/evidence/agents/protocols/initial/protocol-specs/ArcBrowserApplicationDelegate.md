# ArcBrowserApplicationDelegate

Application/browser process, network/GPU and browser lifecycle notifications. Classification: `lifecycle_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b08f8`; inherits `NSObject`. Pristine binary corroborates all 9 unique guide identities (11 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK14ADKApplication`: class `0x10447d428`, class_ro `0x10447d3c8`, direct protocols `ArcBrowserApplicationDelegate;ArcNotificationCenter`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Engine `0x43eb374` sends `didInitializeBrowserProcess` to a weak-loaded receiver; the actual app implementation is included in the method matrix. Native `0x42b1200` guards initialized state and calls `0x42b13c4`, whose body names `InvokeAsync` and `arc/common/utils/delegate_wrapper.h`. That supports an asynchronous native notification path, while the complete native-task/vtable-to-ObjC chain and runner identity remain unresolved.

Termination is a separate block API, not another guide protocol method. `tryToTerminate:` at `0x43e6ee0` retains block storage and reaches native `0x42b2430`; the independently read copy/destruct helpers `0x4044e80/0x4044ec0` contain retain/release instructions. Preserve immediate status separately from eventual completion. Exactly-once behavior, completion enum names and shutdown ordering remain open.

Profile creation is also a separate block API: `0x43e8368` → `0x42b3d74` → callback `0x42b3f34`, with ObjC adapter `0x43eb7a0` lacking an exact exported header. The full profile-creation result contract cannot be derived from these delegate declarations.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)didCreateNewBrowser:(ArcBrowser *)arg0;` | `v24@0:8@16` | `0x1004b6e14` (entry `0x1035fec98`) | `0x43eb0b0` |
| required | `- (void)didCloseBrowser:(NSString *)arg0 forBrowserContextID:(ArcBrowserContextID *)arg1 ofType:(int64_t)arg2 withRestorationSessionID:(ArcSessionID *)arg3;` | `v48@0:8@16@24q32@40` | `0x1004b7178` (entry `0x1035feca4`) | `0x43eb2b0`; `0x43eb300` |
| required | `- (void)didConnectToNetworkProcess;` | `v16@0:8` | `0x1004b72b4` (entry `0x1035fecb0`) | `0x43eb360` |
| required | `- (void)didInitializeBrowserProcess;` | `v16@0:8` | `0x1004b73fc` (entry `0x1035fecbc`) | `0x43eb394` |
| required | `- (void)didReceiveGPUCrashNotification:(int)arg0 withFallbackCount:(int)arg1;` | `v24@0:8i16i20` | `0x1004b7bcc` (entry `0x1035fecc8`) | `0x43eb3dc` |
| required | `- (void)didReceiveGPUInfoUpdate:(BOOL)arg0;` | `v20@0:8B16` | `0x1004b7c10` (entry `0x1035fecd4`) | `0x43eb41c` |
| required | `- (void)didQuitRunLoop;` | `v16@0:8` | `0x1004b7d50` (entry `0x1035fecec`) | `0x43eb450` |
| required | `- (void)didChangeBrowserContextAttributes:(ArcBrowserContextAttributes *)arg0;` | `v24@0:8@16` | `0x1004b7f5c` (entry `0x1035fecf8`) | `0x43eb4b0` |
| required | `- (void)didRequestDevToolsDebugging:(ArcDevToolsConnectionDialog *)arg0;` | `v24@0:8@16` | `0x1004b7c94` (entry `0x1035fece0`) | `0x43eb518` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
