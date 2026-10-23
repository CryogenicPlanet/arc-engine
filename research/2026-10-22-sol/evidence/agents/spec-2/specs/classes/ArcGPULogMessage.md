# ArcGPULogMessage

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e0a8`, class_ro `0xe2add70`; instanceStart 8, instanceSize 64 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 56 | `_gpu_log_message` | `{GPULogMessage="severity"i"header"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="…` | 0xe2add08 |

This native value stores severity int32 at +8 and two strings at +0x10/+0x28. initWithGPULogMessage: invokes assignment helper 0x441e818 (0x443c0a8); severity is a scalar read (0x443ba88); header/message convert strings (0x443ab84, 0x443aba8).

## Lifecycle, callbacks and relations

Aggregate getter copies strings and severity (0x443c100); setter reuses native assignment (0x443c17c); destructor frees strings (0x443c188). Shared IMPs with ArcBrowserContextAttributes reflect field access/layout reuse, not shared semantic classes. No log collection backend is identified here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable Arc log DTO; the GPU process log feed must be supplied separately by the host engine.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcGPULogMessage.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithGPULogMessage: | `@24@0:8r^v16` | 0x443c0a8 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x0441e818 | IMP 0x443c0a8; record 0xd77c6f4; dyld L2994; otool L34323 | medium / available / local_observation |
| base | - severity | `q16@0:8` | 0x443ba88 | Local field operation: return (long)*(int *)(param_1 + 8); | local state / indirect operation; see body | IMP 0x443ba88; record 0xd77c700; dyld L2995; otool L33925 | medium / available / local_verified |
| base | - header | `@16@0:8` | 0x443ab84 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x27, 0x10, 0x18. | 0x01c391bc | IMP 0x443ab84; record 0xd77c70c; dyld L2996; otool L33306 | medium / available / local_observation |
| base | - message | `@16@0:8` | 0x443aba8 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x3f, 0x28, 0x30. | 0x01c391bc | IMP 0x443aba8; record 0xd77c718; dyld L2997; otool L33309 | medium / available / local_observation |
| base | - gpu_log_message | `{GPULogMessage=i{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}}16@0:8` | 0x443c100 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x8, 0x27, 0x10, 0x18, 0x20, 0x3f, 0x28, 0x30. | 0x00060a60 | IMP 0x443c100; record 0xd77c724; dyld L2998; otool L34335 | medium / available / local_observation |
| base | - setGpu_log_message: | `v72@0:8{GPULogMessage=i{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}}16` | 0x443c17c | Assigns/replaces local native or ObjC state; self offsets 0x8. Calls 0x0441e818. | 0x0441e818 | IMP 0x443c17c; record 0xd77c730; dyld L2999; otool L34338 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x443c188 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x3f, 0x28, 0x38, 0x27, 0x10, 0x20. | local state / indirect operation; see body | IMP 0x443c188; record 0xd77c73c; dyld L3000; otool L34341 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x443b154 | Initializes native storage/engagement fields at self offsets 0x28, 0x20, 0x38, 0x30, 0x18, 0x10; returns self. | local state / indirect operation; see body | IMP 0x443b154; record 0xd77c748; dyld L3001; otool L33521 | medium / available / local_observation |

## Remaining consequential questions

Establish severity enum, message source/order and callback delivery policy in the producer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
