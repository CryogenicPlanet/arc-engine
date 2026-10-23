# ArcExtensionInfo

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 19 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d4c8`, class_ro `0xe2ab160`; instanceStart 8, instanceSize 232 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 224 | `_extension_info` | `{optional<arc::ExtensionInfo>=""(?="__null_state_"c"__val_"{ExtensionInfo="name"{basic_string<char, std::char_traits<char>, std::a…` | 0xe2ab058 |

An optional inline extension snapshot contains name/ID/description/version/keyword strings, URL, permissions, icon and scalar flags. Default init engages a default native record (0x442f174); native-copy init uses 0x4334f1c (0x442f218); destructor uses 0x4334d2c (0x442f528). Getters generally trap if empty.

## Lifecycle, callbacks and relations

permissions constructs an array of strings (0x442f330); icon returns nil for empty image state or converts it (0x442f3f0). enabled, hasAction and fromWebstore are stored flags (0x442f444, 0x442f458, 0x442f4cc). None of these IMPs installs, enables or executes an extension.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome extension data needs an actual extension-service producer for a full replacement. The DTO alone is portable; compatibility of extension execution under a CEF route remains outside this evidence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcExtensionInfo.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - mutableExtensionInfo | `^v16@0:8` | 0x442f158 | Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00. | 0x04b08a00 | IMP 0x442f158; record 0xd77b154; dyld L2482; otool L30760 | medium / available / local_verified |
| base | - init | `@16@0:8` | 0x442f174 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x04334d2c, 0x043386a0 | IMP 0x442f174; record 0xd77b160; dyld L2483; otool L30763 | medium / available / local_observation |
| base | - initWithExtensionInfo: | `@24@0:8r^v16` | 0x442f218 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x04334d2c, 0x04334f1c | IMP 0x442f218; record 0xd77b16c; dyld L2484; otool L30766 | medium / available / local_observation |
| base | - name | `@16@0:8` | 0x442f28c | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0xe0, 0x1f, 0x8, 0x10. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x442f28c; record 0xd77b178; dyld L2485; otool L30769 | medium / available / local_observation |
| base | - id | `@16@0:8` | 0x442f2bc | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0xe0, 0x37, 0x20, 0x28. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x442f2bc; record 0xd77b184; dyld L2486; otool L30772 | medium / available / local_observation |
| base | - description | `@16@0:8` | 0x442f2ec | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0xe0, 0x67, 0x50, 0x58. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x442f2ec; record 0xd77b190; dyld L2487; otool L30775 | medium / available / local_observation |
| base | - url | `@16@0:8` | 0x442f31c | Converts native URL/string storage through 0x43abd34; self offsets 0xe0, 0x38. Engagement guard/trap is visible. | 0x043abd34 | IMP 0x442f31c; record 0xd77b19c; dyld L2488; otool L30778 | medium / available / local_observation |
| base | - permissions | `@16@0:8` | 0x442f330 | Requires engaged native extension snapshot; converts native UTF-16 permissions into NSMutableArray. | 0x01d9fb90 | IMP 0x442f330; record 0xd77b1a8; dyld L2489; otool L30781 | medium / available / bounded_review |
| base | - icon | `@16@0:8` | 0x442f3f0 | Requires engaged snapshot; absent native image returns nil, present image converts through 0x2a02ccc. | 0x02a02ccc | IMP 0x442f3f0; record 0xd77b1b4; dyld L2490; otool L30784 | medium / available / bounded_review |
| base | - rating | `d16@0:8` | 0x442f41c | Local field operation: return *(double *)(param_1 + 0x98); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442f41c; record 0xd77b1c0; dyld L2491; otool L30787 | medium / available / local_verified |
| base | - numRatings | `q16@0:8` | 0x442f430 | Local field operation: return (long)*(int *)(param_1 + 0xa0); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442f430; record 0xd77b1cc; dyld L2492; otool L30790 | medium / available / local_verified |
| base | - enabled | `B16@0:8` | 0x442f444 | Local field operation: return (bool)*(byte *)(param_1 + 0xa4); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442f444; record 0xd77b1d8; dyld L2493; otool L30793 | medium / available / local_verified |
| base | - hasAction | `B16@0:8` | 0x442f458 | Local field operation: return (bool)*(byte *)(param_1 + 0xa5); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442f458; record 0xd77b1e4; dyld L2494; otool L30796 | medium / available / local_verified |
| base | - version | `@16@0:8` | 0x442f46c | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0xe0, 0xbf, 0xa8, 0xb0. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x442f46c; record 0xd77b1f0; dyld L2495; otool L30799 | medium / available / local_observation |
| base | - omniboxKeyword | `@16@0:8` | 0x442f49c | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0xe0, 0xd7, 0xc0, 0xc8. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x442f49c; record 0xd77b1fc; dyld L2496; otool L30802 | medium / available / local_observation |
| base | - fromWebstore | `B16@0:8` | 0x442f4cc | Local field operation: return (bool)*(byte *)(param_1 + 0xd8); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442f4cc; record 0xd77b208; dyld L2497; otool L30805 | medium / available / local_verified |
| base | - extension_info | `{optional<arc::ExtensionInfo>=(?=c{ExtensionInfo={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{vector<std::u16string, std::allocator<std::u16string>>={__vector_layout<std::u16string, std::allocator<std::u16string>>=^vQQ}}{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}}diBB{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}B})B}16@0:8` | 0x442f4e0 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0xe0, 0x8. Engagement guard/trap is visible. | 0x04334f1c | IMP 0x442f4e0; record 0xd77b214; dyld L2498; otool L30808 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x442f528 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0xe0, 0x8. | 0x04334d2c | IMP 0x442f528; record 0xd77b220; dyld L2499; otool L30811 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x442f53c | Initializes native storage/engagement fields at self offsets 0x8, 0xe0; returns self. | local state / indirect operation; see body | IMP 0x442f53c; record 0xd77b22c; dyld L2500; otool L30814 | medium / available / local_observation |

## Remaining consequential questions

Find snapshot refresh points, permission text localization, icon representation and extension-service dependencies. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcExtensionInfo.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcExtensionInfo.md).
