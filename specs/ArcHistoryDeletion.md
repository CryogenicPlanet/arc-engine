# ArcHistoryDeletion

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d388`, class_ro `0xe2aac20`; instanceStart 8, instanceSize 144 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_deletedItems` | `@"NSArray"` | 0xe2aab38 |
| 0x10 | 128 | `_history_deletion` | `{optional<arc::HistoryDeletion>=""(?="__null_state_"c"__val_"{HistoryDeletion="is_all_history"B"is_from_expiration"B"has_time_rang…` | 0xe2aab58 |

The engaged native deletion payload begins at +0x10 and +8 caches the ObjC deletedItems array. Initialization copies through 0x4337338 after disposing prior state (0x442e4f4). The native flags expose all-history, expiration and time-range presence; date getters convert stored seconds (0x442e568, 0x442e57c, 0x442e590, 0x442e5a4, 0x442e5c8).

## Lifecycle, callbacks and relations

deletedItems lazily wraps ArcHistoryItem values (0x442e5ec). faviconUrls/restrictUrls build sets, and deletedVisitIds boxes native IDs (0x442e6c4, 0x442e7a8, 0x442e88c). Destruction frees native state and cached array (0x442e994). This is a deletion notification payload, not evidence that reading it deletes history.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Upstream history::DeletionInfo is a domain comparison at components/history/core/browser/history_types.h:792, not a proven direct layout mapping. Chrome history observer semantics need preservation; CEF support is not established.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcHistoryDeletion.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithHistoryDeletion: | `@24@0:8r^v16` | 0x442e4f4 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x043374e4, 0x04337338 | IMP 0x442e4f4; record 0xd77af54; dyld L2434; otool L30387 | medium / available / local_observation |
| base | - isAllHistory | `B16@0:8` | 0x442e568 | Local field operation: return (bool)*(byte *)(param_1 + 0x10); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442e568; record 0xd77af60; dyld L2435; otool L30390 | medium / available / local_verified |
| base | - isFromExpiration | `B16@0:8` | 0x442e57c | Local field operation: return (bool)*(byte *)(param_1 + 0x11); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442e57c; record 0xd77af6c; dyld L2436; otool L30393 | medium / available / local_verified |
| base | - hasTimeRange | `B16@0:8` | 0x442e590 | Local field operation: return (bool)*(byte *)(param_1 + 0x12); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442e590; record 0xd77af78; dyld L2437; otool L30396 | medium / available / local_verified |
| base | - timeRangeStart | `@16@0:8` | 0x442e5a4 | Local field operation: return IVar2; Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442e5a4; record 0xd77af84; dyld L2438; otool L30399 | medium / available / local_verified |
| base | - timeRangeEnd | `@16@0:8` | 0x442e5c8 | Local field operation: return IVar2; Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442e5c8; record 0xd77af90; dyld L2439; otool L30402 | medium / available / local_verified |
| base | - deletedItems | `@16@0:8` | 0x442e5ec | Requires engaged payload; lazily constructs/caches array of ArcHistoryItem from native deleted-item vector. | local state / indirect operation; see body | IMP 0x442e5ec; record 0xd77af9c; dyld L2440; otool L30405 | medium / available / bounded_review |
| base | - faviconUrls | `@16@0:8` | 0x442e6c4 | Requires engaged payload; converts native URL collection into NSMutableSet. | 0x043abd34 | IMP 0x442e6c4; record 0xd77afa8; dyld L2441; otool L30408 | medium / available / bounded_review |
| base | - restrictUrls | `@16@0:8` | 0x442e7a8 | Requires engaged payload; converts native restricted URL collection into NSMutableSet. | 0x043abd34 | IMP 0x442e7a8; record 0xd77afb4; dyld L2442; otool L30411 | medium / available / bounded_review |
| base | - deletedVisitIds | `@16@0:8` | 0x442e88c | Requires engaged payload; builds array boxing native visit IDs as NSNumber. | local state / indirect operation; see body | IMP 0x442e88c; record 0xd77afc0; dyld L2443; otool L30414 | medium / available / bounded_review |
| base | - history_deletion | `{optional<arc::HistoryDeletion>=(?=c{HistoryDeletion=BBBdd{vector<arc::HistoryItem, std::allocator<arc::HistoryItem>>={__vector_layout<arc::HistoryItem, std::allocator<arc::HistoryItem>>=^{HistoryItem}QQ}}{set<std::string, std::less<std::string>, std::allocator<std::string>>={__tree<std::string, std::less<std::string>, std::allocator<std::string>>=^v{__tree_end_node<std::__tree_node_base<void *> *>=^v}Q}}{set<std::string, std::less<std::string>, std::allocator<std::string>>={__tree<std::string, std::less<std::string>, std::allocator<std::string>>=^v{__tree_end_node<std::__tree_node_base<void *> *>=^v}Q}}{vector<long long, std::allocator<long long>>={__vector_layout<long long, std::allocator<long long>>=^qQQ}}})B}16@0:8` | 0x442e94c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x88, 0x10. Engagement guard/trap is visible. | 0x04337338 | IMP 0x442e94c; record 0xd77afcc; dyld L2444; otool L30417 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x442e994 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x88, 0x10, 0x8. | 0x043374e4 | IMP 0x442e994; record 0xd77afd8; dyld L2445; otool L30420 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x442e9c8 | Initializes native storage/engagement fields at self offsets 0x10, 0x88; returns self. | local state / indirect operation; see body | IMP 0x442e9c8; record 0xd77afe4; dyld L2446; otool L30423 | medium / available / local_observation |

## Remaining consequential questions

Determine payload producer/notification timing, absent-range interpretation, cache validity and how restricted URLs/deleted IDs relate to actual history changes. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcHistoryDeletion.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcHistoryDeletion.md).
