# ArcAIServiceConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37b8f8`, class_ro `0xe2a3b10`; instanceStart 8, instanceSize 160 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 152 | `_nativeAIServiceConfig` | `{optional<arc::AIServiceConfiguration>=""(?="__null_state_"c"__val_"{AIServiceConfiguration="type"i"display_name"{basic_string<cha…` | 0xe2a3a58 |

init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error: rejects type -1 with NSError domain company.thebrowser.Arc, code 2 and Invalidtype text, writing the supplied error pointer (0x43dcbdc). It uses libraryPath.path, converts five strings, narrows maxRetries to a signed byte at +0x88 and stores a double at +0x90. Optional engagement is +0x98.

## Lifecycle, callbacks and relations

nativeAIServiceConfig copies an engaged configuration and setNativeAIServiceConfig: implements engaged/empty assignment (0x43dd120, 0x43dd184); destruction calls 0x42a3344 only while engaged (0x43dd3a0). No callback registration or library loading happens in these configuration IMPs. The timeout getter has a decompiler signature defect; raw instructions load d0 from +0x90 after the engagement check (0x43dd10c).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific configuration ABI including narrowing and error behavior; strings describe external service deployment, without proving any particular library identity.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIServiceConfiguration.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error: | `@84@0:8q16@24@32@40@48@56i64d68^@76` | 0x43dcbdc | Type -1 writes NSError(domain company.thebrowser.Arc, code 2) and returns nil; otherwise converts strings/path, narrows retries to signed byte and stores timeout double. | 0x042a3344, 0x01b356b8 | IMP 0x43dcbdc; record 0xd7776c0; dyld L1224; otool L21805 | medium / available / bounded_review |
| base | - type | `q16@0:8` | 0x43dcff4 | Local field operation: return (long)*(int *)(param_1 + 8); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43dcff4; record 0xd7776cc; dyld L1225; otool L21808 | medium / available / local_verified |
| base | - displayName | `@16@0:8` | 0x43dd008 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x98, 0x27, 0x10, 0x18. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43dd008; record 0xd7776d8; dyld L1226; otool L21811 | medium / available / local_observation |
| base | - libraryPath | `@16@0:8` | 0x43dd038 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x98, 0x3f, 0x28, 0x30. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43dd038; record 0xd7776e4; dyld L1227; otool L21814 | medium / available / local_observation |
| base | - libraryVersion | `@16@0:8` | 0x43dd068 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x98, 0x57, 0x40, 0x48. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43dd068; record 0xd7776f0; dyld L1228; otool L21817 | medium / available / local_observation |
| base | - appEnvironment | `@16@0:8` | 0x43dd098 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x98, 0x6f, 0x58, 0x60. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43dd098; record 0xd7776fc; dyld L1229; otool L21820 | medium / available / local_observation |
| base | - fullAppVersion | `@16@0:8` | 0x43dd0c8 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x98, 0x87, 0x70, 0x78. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43dd0c8; record 0xd777708; dyld L1230; otool L21823 | medium / available / local_observation |
| base | - maxRetries | `i16@0:8` | 0x43dd0f8 | Local field operation: return (int)*(char *)(param_1 + 0x88); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43dd0f8; record 0xd777714; dyld L1231; otool L21826 | medium / available / local_verified |
| base | - timeout | `d16@0:8` | 0x43dd10c | After engagement check at +0x98, raw word fd404800 loads d0 from self+0x90; Ghidra incorrectly omits double result. | local state / indirect operation; see body | IMP 0x43dd10c; record 0xd777720; dyld L1232; otool L21829 | medium / available_signature_artifact / bounded_review |
| base | - nativeAIServiceConfig | `{optional<arc::AIServiceConfiguration>=(?=c{AIServiceConfiguration=i{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}cd})B}16@0:8` | 0x43dd120 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x98, 0x8. Engagement guard/trap is visible. | 0x043da310 | IMP 0x43dd120; record 0xd77772c; dyld L1233; otool L21832 | medium / available / local_observation |
| base | - setNativeAIServiceConfig: | `v168@0:8{optional<arc::AIServiceConfiguration>=(?=c{AIServiceConfiguration=i{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}cd})B}16` | 0x43dd184 | Assigns/replaces local native or ObjC state; self offsets 0x98, 0x8, 0x27, 0x10, 0x20, 0x18, 0x3f, 0x28. Calls 0x00359ad8, 0x00359998, 0x043da310, 0x042a3344. | 0x00359ad8, 0x00359998, 0x043da310, 0x042a3344 | IMP 0x43dd184; record 0xd777738; dyld L1234; otool L21835 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43dd3a0 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x98, 0x8. | 0x042a3344 | IMP 0x43dd3a0; record 0xd777744; dyld L1235; otool L21838 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43dd3b4 | Initializes native storage/engagement fields at self offsets 0x8, 0x98; returns self. | local state / indirect operation; see body | IMP 0x43dd3b4; record 0xd777750; dyld L1236; otool L21841 | medium / available / local_observation |

## Remaining consequential questions

Determine valid service type values, whether nil error pointers are ever used on the rejection path, and allowed retry/timeout ranges. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
