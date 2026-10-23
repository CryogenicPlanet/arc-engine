# ArcAutocompleteResult

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 4 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bbc8`, class_ro `0xe2a45e8`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_combinedMatches` | `@"NSArray"` | 0xe2a4580 |
| 0x10 | 8 | `_perProviderMatches` | `@"NSDictionary"` | 0xe2a45a0 |

init:perProviderMatches: stores two ObjC collections at +8 and +0x10 after NSObject init (0x43df204). Accessors return those slots (0x43dd450, 0x420b5f8); destruction clears both (0x43dd5a4). It does not compute or merge matches.

## Lifecycle, callbacks and relations

No delegate registration occurs in this class. The exact element classes, collection mutability and copy-versus-retain policy are not fully established because retain/release fixups obscure the stores (0x43df204). It is distinct from the array constructed by autocomplete completion at 0x43eceb0.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A result container can be backend independent. Matching/ranking/provider behavior must be obtained from the producer; its class name does not prove Chrome or CEF equivalence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAutocompleteResult.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init:perProviderMatches: | `@32@0:8@16@24` | 0x43df204 | After NSObject init, stores combinedMatches at +8 and perProviderMatches at +0x10; computes no matches. | local state / indirect operation; see body | IMP 0x43df204; record 0xd777ab0; dyld L1320; otool L22601 | medium / available / bounded_review |
| base | - combinedMatches | `@16@0:8` | 0x43dd450 | Local field operation: return *(ID *)(param_1 + 8); | local state / indirect operation; see body | IMP 0x43dd450; record 0xd777abc; dyld L1321; otool L21911 | medium / available / local_verified |
| base | - perProviderMatches | `@16@0:8` | 0x420b5f8 | Local field operation: return *(ID *)(param_1 + 0x10); | local state / indirect operation; see body | IMP 0x420b5f8; record 0xd777ac8; dyld L1322; otool L15625 | medium / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x43dd5a4 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x10, 0x8. | local state / indirect operation; see body | IMP 0x43dd5a4; record 0xd777ad4; dyld L1323; otool L22051 | medium / available / local_observation |

## Remaining consequential questions

Find the producer of this combined/per-provider result and actual app consumers; establish collection ownership and ordering. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
