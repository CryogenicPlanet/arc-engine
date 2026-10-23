# ArcDisplayNameOrOrigin

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dec8`, class_ro `0xe2ad750`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 32 | `_display_name_or_origin` | `{DisplayNameOrOrigin="name_or_origin"{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>="__rep_"(__rep=…` | 0xe2ad6f8 |

The record holds a native UTF-16 string plus an isOrigin byte at +0x20. Initialization copies native string state (0x443b898), nameOrOrigin converts it through 0x1d9fb90 (0x441cefc), and isOrigin directly loads the flag (0x4081f00).

## Lifecycle, callbacks and relations

Aggregate getter copies the string and flag; setter assigns both (0x443b948, 0x443b998). Destruction frees long-string storage (0x443ba1c). There is no demonstrated origin parsing, formatting policy, delegate or persistence operation in this class.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent presentation DTO; origin/security policy must be supplied by its producer.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcDisplayNameOrOrigin.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithDisplayNameOrOrigin: | `@24@0:8r^v16` | 0x443b898 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x007ab764, 0x007ab624 | IMP 0x443b898; record 0xd77c460; dyld L2931; otool L33850 | medium / available / local_observation |
| base | - nameOrOrigin | `@16@0:8` | 0x441cefc | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x1f, 0x8, 0x10. | 0x01d9fb90 | IMP 0x441cefc; record 0xd77c46c; dyld L2932; otool L29226 | medium / available / local_observation |
| base | - isOrigin | `B16@0:8` | 0x4081f00 | Reads 8-bit register value from self+0x20 (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4081f00; record 0xd77c478; dyld L2933; otool L33856 | high / available / local_verified |
| base | - display_name_or_origin | `{DisplayNameOrOrigin={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}B}16@0:8` | 0x443b948 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x1f, 0x8, 0x10, 0x18, 0x20. | 0x00054420 | IMP 0x443b948; record 0xd77c484; dyld L2934; otool L33859 | medium / available / local_observation |
| base | - setDisplay_name_or_origin: | `v48@0:8{DisplayNameOrOrigin={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}B}16` | 0x443b998 | Assigns/replaces local native or ObjC state; self offsets 0x8, 0x1f, 0x18, 0x10, 0x20. Calls 0x007ab764, 0x007ab624. | 0x007ab764, 0x007ab624 | IMP 0x443b998; record 0xd77c490; dyld L2935; otool L33862 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x443ba1c | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x443ba1c; record 0xd77c49c; dyld L2936; otool L33865 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x416a680 | Initializes native storage/engagement fields at self offsets 0x8, 0x10, 0x18; returns self. | local state / indirect operation; see body | IMP 0x416a680; record 0xd77c4a8; dyld L2937; otool L8137 | medium / available / local_observation |

## Remaining consequential questions

Find who chooses display-name versus origin and whether strings are already escaped/localized. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
