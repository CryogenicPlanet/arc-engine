# ArcHistoryQueryOptions

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 16 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c320`, class_ro `0xe2a6d30`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 16 | `_options` | `{HistoryQueryOptions="impl_"{shared_ptr<arc::HistoryQueryOptions::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}}` | 0xe2a6c98 |

Construction allocates shared native options using 0x458d908; destruction releases the shared control block (0x4403adc, 0x43de5e8). searchText converts native UTF-16; the time setters convert NSDate seconds to Chromium-style microseconds with offset 0x295e9648864000 and saturation/NaN branches (0x44036c0, 0x4403830, 0x4403978).

## Lifecycle, callbacks and relations

maxResults reads native +0x28 (0x40cbfe8). Duplicate policy maps 0..2 and traps invalid getter state (0x4403a34); matchingAlgorithm returns zero for absent or native zero, while its setter only accepts zero/one (0x4403a74, 0x4403aa4). Upstream QueryOptions and meanings are in components/history/core/browser/history_types.h:362, :384, :398; Arc wraps additional searchText, so layout equivalence is not claimed.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A query descriptor, not an executed history query. Chrome history options provide a source-backed model; CEF route needs explicit query, dedup and time-bound behavior.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcHistoryQueryOptions.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - searchText | `@16@0:8` | 0x440360c | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x8. | 0x00054420, 0x01d9fb90 | IMP 0x440360c; record 0xd7790f4; dyld L1748; otool L25487 | medium / available / local_observation |
| base | - setSearchText: | `v24@0:8@16` | 0x44036c0 | Assigns/replaces local native or ObjC state; self offsets 0x8. Calls 0x083b1228, 0x007ab764, 0x007ab624. | 0x083b1228, 0x007ab764, 0x007ab624 | IMP 0x44036c0; record 0xd779100; dyld L1749; otool L25490 | medium / available / local_observation |
| base | - beginTime | `@16@0:8` | 0x4403798 | Reads native +0x18 microseconds; subtracts epoch offset 0x295e9648864000, divides by 1e6 and constructs NSDate; handles null/infinity/saturation. | local state / indirect operation; see body | IMP 0x4403798; record 0xd77910c; dyld L1750; otool L25493 | medium / available / bounded_review |
| base | - setBeginTime: | `v24@0:8@16` | 0x4403830 | Converts NSDate Unix seconds to microseconds plus 0x295e9648864000; handles NaN/infinity/saturation and stores native +0x18. | local state / indirect operation; see body | IMP 0x4403830; record 0xd779118; dyld L1751; otool L25496 | medium / available / bounded_review |
| base | - endTime | `@16@0:8` | 0x44038e0 | Reads native +0x20 microseconds; converts same epoch/microsecond representation as beginTime to NSDate, including null/infinity branches. | local state / indirect operation; see body | IMP 0x44038e0; record 0xd779124; dyld L1752; otool L25499 | medium / available / bounded_review |
| base | - setEndTime: | `v24@0:8@16` | 0x4403978 | Converts NSDate Unix seconds with epoch/microsecond saturation logic and writes native +0x20. | local state / indirect operation; see body | IMP 0x4403978; record 0xd779130; dyld L1753; otool L25502 | medium / available / bounded_review |
| base | - maxResults | `i16@0:8` | 0x40cbfe8 | Local field operation: return *(int *)(*(long *)(param_1 + 8) + 0x28); | local state / indirect operation; see body | IMP 0x40cbfe8; record 0xd77913c; dyld L1754; otool L25505 | medium / available / local_verified |
| base | - setMaxResults: | `v20@0:8i16` | 0x4403a28 | Local field operation: *(int *)(*(long *)(param_1 + 8) + 0x28) = param_3; | local state / indirect operation; see body | IMP 0x4403a28; record 0xd779148; dyld L1755; otool L25508 | medium / available / local_verified |
| base | - duplicateHandlingPolicy | `q16@0:8` | 0x4403a34 | Reads native +0x30; maps 0/1/2 to same values, trapping outside range. | local state / indirect operation; see body | IMP 0x4403a34; record 0xd779154; dyld L1756; otool L25511 | medium / available / bounded_review |
| base | - setDuplicateHandlingPolicy: | `v24@0:8q16` | 0x4403a60 | Writes native +0x30 only under decompiled signed comparison param_3 < 3; negative edge needs instruction check. | local state / indirect operation; see body | IMP 0x4403a60; record 0xd779160; dyld L1757; otool L25514 | medium / available / bounded_review |
| base | - matchingAlgorithm | `q16@0:8` | 0x4403a74 | Absent optional returns 0; present native 0/1 return same value, other values trap. | local state / indirect operation; see body | IMP 0x4403a74; record 0xd77916c; dyld L1758; otool L25517 | medium / available / bounded_review |
| base | - setMatchingAlgorithm: | `v24@0:8q16` | 0x4403aa4 | Accepts only 0 or 1, writes native +0x34 and sets engagement at +0x38; other inputs leave state unchanged. | local state / indirect operation; see body | IMP 0x4403aa4; record 0xd779178; dyld L1759; otool L25520 | medium / available / bounded_review |
| base | - options | `{HistoryQueryOptions={shared_ptr<arc::HistoryQueryOptions::Impl>=^{Impl}^{__shared_weak_count}}}16@0:8` | 0x4401710 | Local field operation: lVar2 = *(long *)(param_1 + 0x10);; uVar3 = *(undefined8 *)(param_1 + 8);; in_x8[1] = *(undefined8 *)(param_1 + 0x10);; return (HistoryQueryOptions)HVar4.field0_0x0; | local state / indirect operation; see body | IMP 0x4401710; record 0xd779184; dyld L1760; otool L25523 | medium / available / local_verified |
| base | - setOptions: | `v32@0:8{HistoryQueryOptions={shared_ptr<arc::HistoryQueryOptions::Impl>=^{Impl}^{__shared_weak_count}}}16` | 0x4403ad0 | Assigns/replaces local native or ObjC state; self offsets 0x8. Calls 0x04331e20. | 0x04331e20 | IMP 0x4403ad0; record 0xd779190; dyld L1761; otool L25526 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43de5e8 | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x10. | local state / indirect operation; see body | IMP 0x43de5e8; record 0xd77919c; dyld L1762; otool L22233 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x4403adc | Initializes native storage/engagement fields at self offsets 0x8, 0x10; returns self. | 0x0458d908 | IMP 0x4403adc; record 0xd7791a8; dyld L1763; otool L25532 | medium / available / local_observation |

## Remaining consequential questions

Confirm all signed enum edge cases, nil/NaN dates, default begin/end bounds and actual query consumer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcHistoryQueryOptions.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcHistoryQueryOptions.md).
