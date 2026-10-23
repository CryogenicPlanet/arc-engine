# ArcContextMenuElementInfo

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 17 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e508`, class_ro `0xe2aeac8`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 24 | `_context_menu_element_info` | `{optional<arc::ContextMenuElementInfo>=""(?="__null_state_"c"__val_"{ContextMenuElementInfo="impl_"{shared_ptr<arc::ContextMenuEle…` | 0xe2ae9c0 |

Initialization stores an optional shared native context record (0x443f0a0); destruction releases it (0x43df69c). x/y read native int32 at +4/+8 through the handle (0x443f1c4, 0x443f1dc); URL and text accessors convert native values, including selection text at +0x1c0 (0x443f404). Most access requires engagement.

## Lifecycle, callbacks and relations

dictionarySuggestions requires engagement and constructs/returns NSMutableArray of ObjC strings converted from native UTF-16 elements, including an empty array; it sends no copy (0x443f654). inputType maps native enum 15..23 through table 0xc9d8198, returns zero when optional input type is absent and five outside that range (0x443f844). nodeType maps native 0..6 using 0xc9d81a8, trapping outside it (0x443f88c). These are metadata adapters; no menu is displayed here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Context-menu data can be adapted by either route, but coordinate space, enum translation and top/subframe distinctions require explicit mapping. Stock Blink fields are domain context only, not proven struct identity.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcContextMenuElementInfo.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithContextMenuElementInfo: | `@24@0:8r^v16` | 0x443f0a0 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x443f0a0; record 0xd77cc8c; dyld L3132; otool L35349 | medium / available / local_observation |
| base | - x | `i16@0:8` | 0x443f1c4 | Local field operation: return *(int *)(*(long *)(param_1 + 8) + 4); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443f1c4; record 0xd77cc98; dyld L3133; otool L35352 | medium / available / local_verified |
| base | - y | `i16@0:8` | 0x443f1dc | Local field operation: return *(int *)(*(long *)(param_1 + 8) + 8); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443f1dc; record 0xd77cca4; dyld L3134; otool L35355 | medium / available / local_verified |
| base | - topLevelURL | `@16@0:8` | 0x443f1f4 | Converts native URL/string storage through 0x43abd34; self offsets 0x18, 0x8. Engagement guard/trap is visible. | 0x007cf864, 0x00060a60, 0x043abd34 | IMP 0x443f1f4; record 0xd77ccb0; dyld L3135; otool L35358 | medium / available / local_observation |
| base | - subFrameURL | `@16@0:8` | 0x443f2a4 | Converts native URL/string storage through 0x43abd34; self offsets 0x18, 0x8. Engagement guard/trap is visible. | 0x007cf864, 0x00060a60, 0x043abd34 | IMP 0x443f2a4; record 0xd77ccbc; dyld L3136; otool L35361 | medium / available / local_observation |
| base | - linkURL | `@16@0:8` | 0x442e1a0 | Converts native URL/string storage through 0x43abd34; self offsets 0x18, 0x8. Engagement guard/trap is visible. | 0x007cf864, 0x00060a60, 0x043abd34 | IMP 0x442e1a0; record 0xd77ccc8; dyld L3137; otool L30293 | medium / available / local_observation |
| base | - sourceAttributeURL | `@16@0:8` | 0x443f354 | Converts native URL/string storage through 0x43abd34; self offsets 0x18, 0x8. Engagement guard/trap is visible. | 0x007cf864, 0x00060a60, 0x043abd34 | IMP 0x443f354; record 0xd77ccd4; dyld L3138; otool L35367 | medium / available / local_observation |
| base | - selectionText | `@16@0:8` | 0x443f404 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x18, 0x8. Engagement guard/trap is visible. | 0x00054420, 0x01d9fb90 | IMP 0x443f404; record 0xd77cce0; dyld L3139; otool L35370 | medium / available / local_observation |
| base | - suggestedFilename | `@16@0:8` | 0x443f4c4 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x18, 0x8. Engagement guard/trap is visible. | 0x00054420, 0x01d9fb90 | IMP 0x443f4c4; record 0xd77ccec; dyld L3140; otool L35373 | medium / available / local_observation |
| base | - misspelledWord | `@16@0:8` | 0x443f58c | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x18, 0x8. Engagement guard/trap is visible. | 0x00054420, 0x01d9fb90 | IMP 0x443f58c; record 0xd77ccf8; dyld L3141; otool L35376 | medium / available / local_observation |
| base | - dictionarySuggestions | `@16@0:8` | 0x443f654 | Requires engaged native state or traps. Gets a temporary suggestion vector via 0x43342a4, converts each UTF-16 element with 0x1d9fb90 and returns the constructed NSMutableArray, including an empty array; no copy is sent. | 0x043342a4, 0x01d9fb90 | IMP 0x443f654; record 0xd77cd04; dyld L3142; otool L35379 | medium / available / local_observation |
| base | - inputType | `q16@0:8` | 0x443f844 | Requires engaged handle; absent native optional → 0; native values 15..23 map [3,2,5,5,5,5,5,4,1], all other present values → 5. | local state / indirect operation; see body | IMP 0x443f844; record 0xd77cd10; dyld L3143; otool L35382 | medium / available / bounded_review |
| base | - nodeType | `q16@0:8` | 0x443f88c | Requires engaged handle; native 0..6 map [0,1,3,2,4,5,6]; out-of-range traps. | local state / indirect operation; see body | IMP 0x443f88c; record 0xd77cd1c; dyld L3144; otool L35385 | medium / available / bounded_review |
| base | - isEditable | `B16@0:8` | 0x443f8bc | Local field operation: return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x251); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443f8bc; record 0xd77cd28; dyld L3145; otool L35388 | medium / available / local_verified |
| base | - context_menu_element_info | `{optional<arc::ContextMenuElementInfo>=(?=c{ContextMenuElementInfo={shared_ptr<arc::ContextMenuElementInfo::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x18, 0x10, 0x8. Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43df53c; record 0xd77cd34; dyld L3146; otool L35391 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43df69c | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x18, 0x10. | local state / indirect operation; see body | IMP 0x43df69c; record 0xd77cd40; dyld L3147; otool L22685 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43df760 | Initializes native storage/engagement fields at self offsets 0x8, 0x18; returns self. | local state / indirect operation; see body | IMP 0x43df760; record 0xd77cd4c; dyld L3148; otool L22688 | medium / available / local_observation |

## Remaining consequential questions

Establish coordinate units and enum names; verify suggestion lifetime and all supported native node/input kinds. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 4; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
