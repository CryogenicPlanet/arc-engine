# ArcRedirectEntry

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d108`, class_ro `0xe2aa3c0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 64 | `_redirect_entry` | `{optional<arc::RedirectEntry>=""(?="__null_state_"c"__val_"{RedirectEntry="url"{basic_string<char, std::char_traits<char>, std::al…` | 0xe2aa348 |

Native-copy init engages an inline record containing URL and UTF-16 title strings plus hidden and redirect-type fields (0x442cd3c). Getters require engagement; hidden is byte +0x38, redirectType int32 +0x3c (0x442ce68, 0x442ce7c).

## Lifecycle, callbacks and relations

redirect_entry copies both strings and scalar state (0x442ce90); destruction frees long strings (0x442cf28). ArcAnnotatedVisit redirectChain creates these wrappers, and setRedirectChain: converts them back (0x43ff0fc, 0x43ffb30). No redirect is executed by this record.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable history-chain payload, with producer semantics tied to history. No direct upstream RedirectEntry layout match is claimed.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcRedirectEntry.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithRedirectEntry: | `@24@0:8r^v16` | 0x442cd3c | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x00060a60, 0x00054420 | IMP 0x442cd3c; record 0xd77ac44; dyld L2358; otool L29784 | medium / available / local_observation |
| base | - url | `@16@0:8` | 0x442ce24 | Converts native URL/string storage through 0x43abd34; self offsets 0x40, 0x8. Engagement guard/trap is visible. | 0x043abd34 | IMP 0x442ce24; record 0xd77ac50; dyld L2359; otool L29787 | medium / available / local_observation |
| base | - title | `@16@0:8` | 0x442ce38 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x40, 0x37, 0x20, 0x28. Engagement guard/trap is visible. | 0x01d9fb90 | IMP 0x442ce38; record 0xd77ac5c; dyld L2360; otool L29790 | medium / available / local_observation |
| base | - hidden | `B16@0:8` | 0x442ce68 | Local field operation: return (bool)*(byte *)(param_1 + 0x38); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442ce68; record 0xd77ac68; dyld L2361; otool L29793 | medium / available / local_verified |
| base | - redirectType | `q16@0:8` | 0x442ce7c | Local field operation: return (long)*(int *)(param_1 + 0x3c); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442ce7c; record 0xd77ac74; dyld L2362; otool L29796 | medium / available / local_verified |
| base | - redirect_entry | `{optional<arc::RedirectEntry>=(?=c{RedirectEntry={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}Bi})B}16@0:8` | 0x442ce90 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x40, 0x1f, 0x8, 0x10, 0x18, 0x37, 0x20, 0x28. Engagement guard/trap is visible. | 0x00060a60, 0x00054420 | IMP 0x442ce90; record 0xd77ac80; dyld L2363; otool L29799 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x442cf28 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x40, 0x37, 0x20, 0x30, 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x442cf28; record 0xd77ac8c; dyld L2364; otool L29802 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x442cf84 | Initializes native storage/engagement fields at self offsets 0x8, 0x40; returns self. | local state / indirect operation; see body | IMP 0x442cf84; record 0xd77ac98; dyld L2365; otool L29805 | medium / available / local_observation |

## Remaining consequential questions

Recover redirect-type enum and whether URL/title correspond to pre- or post-redirect entries; validate hidden handling. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcRedirectEntry.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcRedirectEntry.md).
