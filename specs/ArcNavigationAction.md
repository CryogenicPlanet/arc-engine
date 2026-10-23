# ArcNavigationAction

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c668`, class_ro `0xe2a7878`; instanceStart 8, instanceSize 96 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 88 | `_nativeNavigationAction` | `{NavigationAction="navigation_type"i"navigation_disposition"i"request"{URLRequest="url"{basic_string<char, std::char_traits<char>,…` | 0xe2a7800 |

Initializer copies native action state using 0x43452d8 (0x44084e4). url/referrer convert strings at +0x10/+0x28 (0x4408544, 0x440854c). navigationType and navigationDisposition adapt native scalar fields; disposition traps for unsupported values (0x4408554, 0x4408564).

## Lifecycle, callbacks and relations

Aggregate getter copies state via 0x4345224, setter assigns via 0x43452d8, destructor releases native string state (0x4408578, 0x440858c, 0x44085a0). This record does not itself decide navigation policy or launch a request.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Navigation callback payload can be adapted to either route, but enum mapping and the producer decision/callback timing must be preserved.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcNavigationAction.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithNavigationAction: | `@24@0:8r^v16` | 0x44084e4 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x043452d8 | IMP 0x44084e4; record 0xd77965c; dyld L1873; otool L26393 | medium / available / local_observation |
| base | - url | `@16@0:8` | 0x4408544 | Converts native URL/string storage through 0x43abd34; self offsets 0x10. | 0x043abd34 | IMP 0x4408544; record 0xd779668; dyld L1874; otool L26396 | medium / available / local_observation |
| base | - referrer | `@16@0:8` | 0x440854c | Converts native URL/string storage through 0x43abd34; self offsets 0x28. | 0x043abd34 | IMP 0x440854c; record 0xd779674; dyld L1875; otool L26399 | medium / available / local_observation |
| base | - navigationType | `Q16@0:8` | 0x4408554 | Local field operation: uVar1 = *(uint *)(param_1 + 8);; return (ulong)uVar1; | local state / indirect operation; see body | IMP 0x4408554; record 0xd779680; dyld L1876; otool L26402 | medium / available / local_verified |
| base | - navigationDisposition | `q16@0:8` | 0x4408564 | Local field operation: return (ulong)*(uint *)(param_1 + 0xc); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x4408564; record 0xd77968c; dyld L1877; otool L26405 | medium / available / local_verified |
| base | - nativeNavigationAction | `{NavigationAction=ii{URLRequest={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}}}16@0:8` | 0x4408578 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x8, 0x10. | 0x04345224 | IMP 0x4408578; record 0xd779698; dyld L1878; otool L26408 | medium / available / local_observation |
| base | - setNativeNavigationAction: | `v104@0:8{NavigationAction=ii{URLRequest={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}}}16` | 0x440858c | Assigns/replaces local native or ObjC state; self offsets 0x8, 0x10. Calls 0x043452d8. | 0x043452d8 | IMP 0x440858c; record 0xd7796a4; dyld L1879; otool L26411 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x44085a0 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x10. | 0x041ab210 | IMP 0x44085a0; record 0xd7796b0; dyld L1880; otool L26414 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x44085a8 | Initializes native storage/engagement fields at self offsets 0x8, 0x58, 0x18, 0x10, 0x28, 0x20, 0x38, 0x30; returns self. | local state / indirect operation; see body | IMP 0x44085a8; record 0xd7796bc; dyld L1881; otool L26417 | medium / available / local_observation |

## Remaining consequential questions

Identify the action producer and exact navigation/disposition enum names, URL/referrer normalization and any action IDs used for replies. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcNavigationAction.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcNavigationAction.md).
