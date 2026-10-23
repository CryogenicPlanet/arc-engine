# ArcContentConversionService

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ba88`, class_ro `0xe2a4088`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 16 | `_impl` | `{shared_ptr<arc::ContentConversionServiceImpl>="__ptr_"^{ContentConversionServiceImpl}"__cntrl_"^{__shared_weak_count}}` | 0xe2a4068 |

init allocates a shared ContentConversionServiceImpl and constructs it at 0x42a6200 (0x43dd9d4). initWithImpl: stores a shared native implementation (0x43dd794); destruction releases its control block (0x43de5e8). initialize converts libraryPath.path plus identity/version/environment strings and installs a callback adapter (0x43ddb74).

## Lifecycle, callbacks and relations

HTML conversion marshals text, optional URL, enum/options, Boolean flags and a vector built by sending kind/value to untyped enumerated input elements before calling 0x42a75b4 (0x43de074). ArcMetaTagSelector is a compatible candidate shape; this path does not prove the concrete receiver class. That backend retains completion by request ID, has an unavailable early completion branch, and binds content_conversion.mojom.ContentConversionService (0x42a75b4). Conversion algorithm/library contents and completion queue are not recovered.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc service protocol, not proof of stock Chromium HTML-to-Markdown behavior. CEF can host a separate service, but matching conversion and metadata policies remains required.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcContentConversionService.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithImpl: | `@32@0:8{shared_ptr<arc::ContentConversionServiceImpl>=^{ContentConversionServiceImpl}^{__shared_weak_count}}16` | 0x43dd794 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x43dd794; record 0xd777874; dyld L1267; otool L22215 | medium / available / local_observation |
| base | - init | `@16@0:8` | 0x43dd9d4 | Allocates shared implementation, constructs with 0x42a6200, delegates to initWithImpl: and releases temporary control reference. | 0x042a6200 | IMP 0x43dd9d4; record 0xd777880; dyld L1268; otool L22218 | medium / available / bounded_review |
| base | - isAvailable | `B16@0:8` | 0x43ddb0c | Consults implementation availability path with 0x1c5f40 and returns its low bit; does not itself initialize library. | 0x001c5f40 | IMP 0x43ddb0c; record 0xd77788c; dyld L1269; otool L22221 | medium / available / bounded_review |
| base | - initializeWithLibraryPath:displayName:libraryVersion:appEnvironment:fullAppVersion:callback: | `v64@0:8@16@24@32@40@48@?56` | 0x43ddb74 | Converts library URL path and identity/version strings; constructs callback adapter and forwards to 0x42a64c0. | 0x01b356b8, 0x042a59c0, 0x042a64c0, 0x041bffc8 | IMP 0x43ddb74; record 0xd777898; dyld L1270; otool L22224 | medium / available / bounded_review |
| base | - setUser:email:callback: | `v40@0:8@16@24@?32` | 0x43dde8c | Converts user/email byte strings, captures callback and forwards to 0x42a72a4. | 0x01b356b8, 0x042a72a4 | IMP 0x43dde8c; record 0xd7778a4; dyld L1271; otool L22227 | medium / available / bounded_review |
| base | - convertHtmlToMarkdown:url:linkStyle:strategy:urlTransform:skipNoscript:passThroughInputElements:metaTagsToCapture:deduplicateMetaTagContent:completion: | `v84@0:8@16@24q32q40q48B56B60@64B72@?76` | 0x43de074 | Marshals HTML, optional URL, options/flags and a native vector from kind/value messages to untyped enumerated input elements; captures completion and calls 0x42a75b4, reaching content_conversion.mojom.ContentConversionService. ArcMetaTagSelector has a compatible candidate shape; this path allocates/checks no such class and does not establish concrete receiver identity. | 0x01b356b8, 0x04047380, 0x042a5b90, 0x042a75b4, 0x042a5aac, 0x04b087f0 | IMP 0x43de074; record 0xd7778b0; dyld L1272; otool L22230 | medium / available / bounded_review |
| base | - .cxx_destruct | `v16@0:8` | 0x43de5e8 | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x10. | local state / indirect operation; see body | IMP 0x43de5e8; record 0xd7778bc; dyld L1273; otool L22233 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x416a6f0 | Initializes native storage/engagement fields at self offsets 0x8, 0x10; returns self. | local state / indirect operation; see body | IMP 0x416a6f0; record 0xd7778c8; dyld L1274; otool L8938 | medium / available / local_observation |

## Remaining consequential questions

Recover callback result/error shape, URL transform and enum validation; determine library initialization failure behavior and helper/service ownership. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 8; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
