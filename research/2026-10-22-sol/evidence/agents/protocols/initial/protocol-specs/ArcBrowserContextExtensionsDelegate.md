# ArcBrowserContextExtensionsDelegate

Extension lifecycle, errors, pin ordering and extension-system readiness. Classification: `extension_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x104403fd0`; inherits `NSObject`. Pristine binary corroborates all 10 unique guide identities (11 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC13ADKClientLive33BrowserExtensionsLifeCycleManager`: class `0x104491f78`, class_ro `0x104491f18`, direct protocols `ArcBrowserContextExtensionsDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

The app adopter is ADKClientLive.BrowserExtensionsLifeCycleManager, outside the prior ADK-only prefix filter. Raw metadata identifies required lifecycle/error methods and optional pinned-order/readiness methods. Preserve the exact readiness selector `browserContextDidExtensionSystemReady:`; English descriptions are not selector identities.

Required metadata does not prove all callbacks are invoked. Dynamic receiver availability checks, category bridge teardown, errors during install/uninstall and callback ordering remain unverified.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didEnableExtension:(NSString *)arg1;` | `v32@0:8@16@24` | `0x101360da8` (entry `0x1035e1698`) | `0x43fe5a0` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didDisableExtension:(NSString *)arg1;` | `v32@0:8@16@24` | `0x101360db0` (entry `0x1035e16a4`) | `0x43fe630` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didInstallExtension:(NSString *)arg1;` | `v32@0:8@16@24` | `0x101360db8` (entry `0x1035e16b0`) | `0x43fe6c0` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didUninstallExtension:(NSString *)arg1;` | `v32@0:8@16@24` | `0x101360e44` (entry `0x1035e16bc`) | `0x43fe750` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didFailLoadUnpacked:(NSString *)arg1 withManifestError:(NSString *)arg2;` | `v40@0:8@16@24@32` | `0x101360ee4` (entry `0x1035e16c8`) | `0x43fe80c` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didFail:(NSString *)arg1 withRuntimeError:(ArcExtensionRuntimeError *)arg2;` | `v40@0:8@16@24@32` | `0x101360f8c` (entry `0x1035e16d4`) | `0x43fe8cc` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didFail:(NSString *)arg1 withManifestError:(NSString *)arg2;` | `v40@0:8@16@24@32` | `0x101361058` (entry `0x1035e16e0`) | `0x43fe990` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 didFail:(NSString *)arg1 withInternalError:(NSString *)arg2;` | `v40@0:8@16@24@32` | `0x101361060` (entry `0x1035e16ec`) | `0x43fea54` |
| optional | `- (void)browserContext:(ArcBrowserContext *)arg0 didChangePinnedExtensionOrder:(NSArray *)arg1;` | `v32@0:8@16@24` | `0x101361124` (entry `0x1035e16f8`) | Uncorroborated; source candidates only |
| optional | `- (void)browserContextDidExtensionSystemReady:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x101361194` (entry `0x1035e1704`) | Uncorroborated; source candidates only |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
