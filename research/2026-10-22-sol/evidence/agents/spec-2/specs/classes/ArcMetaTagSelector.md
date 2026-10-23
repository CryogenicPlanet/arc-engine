# ArcMetaTagSelector

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 4 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37b998`, class_ro `0xe2a3d10`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_kind` | `q` | 0xe2a3ca8 |
| 0x10 | 8 | `_value` | `@"NSString"` | 0xe2a3cc8 |

initWithKind:value: stores a scalar kind and explicitly copies value (0x43dd458). Getters read kind at +8 and value at +0x10 (0x2d0fb40, 0x420b5f8); destructor clears the value slot (0x425b6d8).

## Lifecycle, callbacks and relations

ArcContentConversionService conversion enumerates these objects, sends kind/value and builds a native selector vector (0x43de074). This class does not query DOM metadata itself.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable selector DTO for the Arc conversion service; exact selector kinds and matching rules remain a service contract.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcMetaTagSelector.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithKind:value: | `@32@0:8q16@24` | 0x43dd458 | After NSObject init, stores kind and an explicit copy of value. | local state / indirect operation; see body | IMP 0x43dd458; record 0xd77779c; dyld L1245; otool L21975 | medium / available / bounded_review |
| base | - kind | `q16@0:8` | 0x2d0fb40 | Reads 64-bit register value from self+0x8 (_kind); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2d0fb40; record 0xd7777a8; dyld L1246; otool L21978 | high / available / local_verified |
| base | - value | `@16@0:8` | 0x420b5f8 | Local field operation: return *(ID *)(param_1 + 0x10); | local state / indirect operation; see body | IMP 0x420b5f8; record 0xd7777b4; dyld L1247; otool L15625 | medium / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x425b6d8 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x10. | local state / indirect operation; see body | IMP 0x425b6d8; record 0xd7777c0; dyld L1248; otool L19979 | medium / available / local_observation |

## Remaining consequential questions

Resolve kind enum and matching case/namespace rules in the conversion backend. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
