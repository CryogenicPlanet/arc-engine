# ArcCommandBarRankingEvaluation

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d7e8`, class_ro `0xe2abcf0`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 40 | `_command_bar_ranking_evaluation` | `{optional<arc::CommandBarRankingEvaluation>=""(?="__null_state_"c"__val_"{CommandBarRankingEvaluation="metrics"{vector<arc::Comman…` | 0xe2abc88 |

An optional evaluation holds a metrics vector, count at +0x20 and duration integer at +0x24. Initializer enumerates dictionary keys, converts keys and double values, then stores counts (0x4433ba8). metrics reconstructs an NSMutableDictionary with NSNumber double values (0x4434190).

## Lifecycle, callbacks and relations

mutableCommandBarRankingEvaluation returns interior storage only if engaged (0x442c260). Aggregate copy uses 0x4434430; .cxx_destruct frees vector string allocations (0x4434298, 0x4434380). No ranking execution, callback scheduling or persistence occurs here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc AI evaluation DTO. Independent of Chromium hosting, but dictionary conversion and numeric types are part of the interface.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcCommandBarRankingEvaluation.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - mutableCommandBarRankingEvaluation | `^v16@0:8` | 0x442c260 | Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00. | 0x04b08a00 | IMP 0x442c260; record 0xd77b828; dyld L2641; otool L29614 | medium / available / local_verified |
| base | - init:numExamplesEvaluated:evaluationDurationMs: | `@32@0:8@16i24i28` | 0x4433ba8 | Enumerates dictionary metric keys/doubleValue values into native vector and stores integer example count/duration; engages optional. | 0x042a5290, 0x01b356b8, 0x042a54c0, 0x00060a60 | IMP 0x4433ba8; record 0xd77b834; dyld L2642; otool L31785 | medium / available / bounded_review |
| base | - initWithCommandBarRankingEvaluation: | `@24@0:8r^v16` | 0x4434088 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x04434430 | IMP 0x4434088; record 0xd77b840; dyld L2643; otool L31788 | medium / available / local_observation |
| base | - metrics | `@16@0:8` | 0x4434190 | Requires engaged evaluation and constructs dictionary mapping converted metric names to NSNumber doubles. | 0x01c391bc | IMP 0x4434190; record 0xd77b84c; dyld L2644; otool L31791 | medium / available / bounded_review |
| base | - numExamplesEvaluated | `i16@0:8` | 0x442c440 | Local field operation: return *(int *)(param_1 + 0x20); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442c440; record 0xd77b858; dyld L2645; otool L29626 | medium / available / local_verified |
| base | - evaluationDurationMs | `i16@0:8` | 0x4434284 | Local field operation: return *(int *)(param_1 + 0x24); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x4434284; record 0xd77b864; dyld L2646; otool L31797 | medium / available / local_verified |
| base | - command_bar_ranking_evaluation | `{optional<arc::CommandBarRankingEvaluation>=(?=c{CommandBarRankingEvaluation={vector<arc::CommandBarRankingMetric, std::allocator<arc::CommandBarRankingMetric>>={__vector_layout<arc::CommandBarRankingMetric, std::allocator<arc::CommandBarRankingMetric>>=^{CommandBarRankingMetric}QQ}}ii})B}16@0:8` | 0x4434298 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x28, 0x8, 0x10, 0x20. Engagement guard/trap is visible. | 0x04434430 | IMP 0x4434298; record 0xd77b870; dyld L2647; otool L31800 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x4434380 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x28, 0x8, 0x10, 0x18. | local state / indirect operation; see body | IMP 0x4434380; record 0xd77b87c; dyld L2648; otool L31803 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43ed538 | Initializes native storage/engagement fields at self offsets 0x8, 0x28; returns self. | local state / indirect operation; see body | IMP 0x43ed538; record 0xd77b888; dyld L2649; otool L23953 | medium / available / local_observation |

## Remaining consequential questions

Check duplicate metric keys, metrics ordering assumptions and duration/count narrowing; identify how service callbacks populate this value. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
