# ArcBrowserContextDownloadsDelegate

Download initial snapshot, item deltas and app-selected filename completion. Classification: `download_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b1028`; inherits `NSObject`. Pristine binary corroborates all 5 unique guide identities (6 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK36BrowserContextDownloadDelegateBridge`: class `0x10447db48`, class_ro `0x10447dae8`, direct protocols `ArcBrowserContextDownloadsDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Engine bridge bodies `0x404257c`, `0x4042610`, `0x404269c` construct `ArcDownloadItem` wrappers and send created/updated/removed notifications after loading weak receiver/context slots. The item wrapper identity does not establish snapshot immutability, post-removal validity or ordering relative to the initial list.

Filename selection is confirmed in instructions even though `0x4042728` has no exact corpus header. Vtable slot `0xdd2bfe0` points there; selector load `0x40427fc/0x4042800` precedes send `0x404281c`. Instruction `0x40427b0` converts native uint32 to double, carried in d0. Extended app metadata declares `(ArcBrowserContext *, ArcDownloadItem *, NSString *, double, void (^)(NSString *))`. The double is a timeout by selector name; its unit/default and late-result behavior are unresolved.

App filename implementation `0x1004e9748` copies the completion before forwarding. Helper `0x1004e9ac4` invokes the default filename when its own weak delegate is absent; `0x1004e9da8` bridges a chosen string and invokes completion. This app-side fallback is distinct from expiration of the engine bridge weak target.

Setter `0x4041fec` weakly installs a nonnil target; nil takes a cleanup path without clearing that target/native registration. Exact teardown, queued update suppression and exactly-once filename completion remain unknown.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 downloadManagerInitializedWithItems:(NSArray *)arg1;` | `v32@0:8@16@24` | `0x1004e8c90` (entry `0x1035e30b0`) | `0x404254c` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 onDownloadCreated:(ArcDownloadItem *)arg1;` | `v32@0:8@16@24` | `0x1004e8f38` (entry `0x1035e30bc`) | `0x40425e0` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 onDownloadUpdated:(ArcDownloadItem *)arg1;` | `v32@0:8@16@24` | `0x1004e91d0` (entry `0x1035e30c8`) | `0x4042674` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 onDownloadRemoved:(ArcDownloadItem *)arg1;` | `v32@0:8@16@24` | `0x1004e9468` (entry `0x1035e30d4`) | `0x4042700` |
| required | `- (void)browserContext:(ArcBrowserContext *)arg0 onDeterminingFilename:(ArcDownloadItem *)arg1 defaultFilename:(NSString *)arg2 completionTimeout:(double)arg3 completion:(void (^)(NSString *))arg4;` | `v56@0:8@16@24@32d40@?48` | `0x1004e9748` (entry `0x1035e30e0`) | `0x404281c` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
