# ArcTabGroupProperties

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 12 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e288`, class_ro `0xe2ae390`; instanceStart 8, instanceSize 56 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 48 | `_tab_group_properties` | `{TabGroupProperties="title"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::al…` | 0xe2ae328 |

This is a partial-properties value: title, color and collapsed are independently optional. title returns nil when absent (0x443d9ec); color/collapsed box values when present (0x443dc04, 0x443dc88). Passing nil to setters clears presence; nonnil values are converted (0x443dad0, 0x443dc40, 0x443dcc4).

## Lifecycle, callbacks and relations

init initializes absent property values (0x443d8e0), native-copy init assigns through 0x441e8ec (0x443d994). Aggregate getter uses 0x5533b24; destructor frees an engaged title (0x8374f04, 0x443dd18). No group is mutated or persisted by these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Compare upstream TabGroupVisualData at components/tab_groups/tab_group_visual_data.h:18, :35: it has concrete title/color/collapsed values, unlike this Arc partial-update payload. CEF group support needs explicit service mapping.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcTabGroupProperties.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init | `@16@0:8` | 0x443d8e0 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x0441e9f0 | IMP 0x443d8e0; record 0xd77c9b8; dyld L3061; otool L34809 | medium / available / local_observation |
| base | - initWithTabGroupProperties: | `@24@0:8r^v16` | 0x443d994 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x0441e8ec | IMP 0x443d994; record 0xd77c9c4; dyld L3062; otool L34812 | medium / available / local_observation |
| base | - title | `@16@0:8` | 0x443d9ec | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x20, 0x1f, 0x8, 0x10, 0x18. | 0x00060a60, 0x01c391bc | IMP 0x443d9ec; record 0xd77c9d0; dyld L3063; otool L34815 | medium / available / local_observation |
| base | - setTitle: | `v24@0:8@16` | 0x443dad0 | Assigns/replaces local native or ObjC state; self offsets 0x20, 0x1f, 0x8, 0x18, 0x10. Calls 0x01b356b8. | 0x01b356b8 | IMP 0x443dad0; record 0xd77c9dc; dyld L3064; otool L34818 | medium / available / local_observation |
| base | - color | `@16@0:8` | 0x443dc04 | Local field operation: return 0;; return IVar1; | local state / indirect operation; see body | IMP 0x443dc04; record 0xd77c9e8; dyld L3065; otool L34821 | medium / available / local_verified |
| base | - setColor: | `v24@0:8@16` | 0x443dc40 | Local field operation: *(undefined4 *)(param_1 + 0x28) = uVar1;; *(bool *)(param_1 + 0x2c) = param_3 != 0; | local state / indirect operation; see body | IMP 0x443dc40; record 0xd77c9f4; dyld L3066; otool L34824 | medium / available / local_verified |
| base | - collapsed | `@16@0:8` | 0x443dc88 | Local field operation: return 0;; return IVar1; | local state / indirect operation; see body | IMP 0x443dc88; record 0xd77ca00; dyld L3067; otool L34827 | medium / available / local_verified |
| base | - setCollapsed: | `v24@0:8@16` | 0x443dcc4 | Local field operation: *(ushort *)(param_1 + 0x30) = uVar1 &#124; (ushort)(param_3 != 0) << 8; | local state / indirect operation; see body | IMP 0x443dcc4; record 0xd77ca0c; dyld L3068; otool L34830 | medium / available / local_verified |
| base | - tab_group_properties | `{TabGroupProperties={optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<arc::bindings::TabGroupColor>=(?=ci)B}{optional<bool>=(?=cB)B}}16@0:8` | 0x8374f04 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x8, 0x28, 0x30. | 0x05533b24 | IMP 0x8374f04; record 0xd77ca18; dyld L3069; otool L34833 | medium / available / local_observation |
| base | - setTab_group_properties: | `v64@0:8{TabGroupProperties={optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<arc::bindings::TabGroupColor>=(?=ci)B}{optional<bool>=(?=cB)B}}16` | 0x443dd0c | Assigns/replaces local native or ObjC state; self offsets 0x8. Calls 0x0441e8ec. | 0x0441e8ec | IMP 0x443dd0c; record 0xd77ca24; dyld L3070; otool L34836 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x443dd18 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x20, 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x443dd18; record 0xd77ca30; dyld L3071; otool L34839 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x443dd40 | Initializes native storage/engagement fields at self offsets 0x8, 0x20, 0x28, 0x2c, 0x30; returns self. | local state / indirect operation; see body | IMP 0x443dd40; record 0xd77ca3c; dyld L3072; otool L34842 | medium / available / local_observation |

## Remaining consequential questions

Confirm nil-versus-empty title and nil-versus-false collapsed behavior in the mutation consumer; map color enum. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
