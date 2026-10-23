# ArcCommandBarRankingExample

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: Extensions. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d798`, class_ro `0xe2abbf0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 64 | `_command_bar_ranking_example` | `{optional<arc::CommandBarRankingExample>=""(?="__null_state_"c"__val_"{CommandBarRankingExample="input"{CommandBarRankingInput="qu…` | 0xe2abb88 |

An engaged optional example owns input and candidate-vector state; selectedIndex is int32 at +0x38 (0x4433658). init:candidates:selectedIndex: converts the input, enumerates candidates into native structures, and stores the index (0x4433124); no bounds check on selectedIndex was established.

## Lifecycle, callbacks and relations

Extensions input copies native example state and constructs an ArcCommandBarRankingInput wrapper (0x4401ca0). The candidates getter shows array/candidate allocation intent, but its pseudocode has incoherent temporary cleanup/reuse and argument propagation (0x4401f34); returned contents and ownership are unresolved. Raw instructions also place cleanup-helper calls before the apparent conversion loop, so this requires coherent callee/control-flow recovery, not a guessed lifetime repair or an engine-bug diagnosis. Aggregate copies use 0x43dad44 (0x4433528, 0x443366c); destruction frees nested state (0x4433768). These are not training operations.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc ranking example DTO; both routes need conversion behavior, while model evaluation/training belongs to ArcAIService (0x43d9ab8, 0x43d9fd4).

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcCommandBarRankingExample.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - mutableCommandBarRankingExample | `^v16@0:8` | 0x4433108 | Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00. | 0x04b08a00 | IMP 0x4433108; record 0xd77b7cc; dyld L2632; otool L31708 | medium / available / local_verified |
| base | - init:candidates:selectedIndex: | `@36@0:8@16@24i32` | 0x4433124 | Converts input and candidate objects into owned native example vector; stores selectedIndex int32 and engages payload. | 0x042a3ff4, 0x04432fcc, 0x015cdd2c, 0x043d9858, 0x04434c34, 0x043da9d0, 0x042a3ea8 | IMP 0x4433124; record 0xd77b7d8; dyld L2633; otool L31711 | medium / available / bounded_review |
| base | - initWithCommandBarRankingExample: | `@24@0:8r^v16` | 0x4433528 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x042a3ff4, 0x043dad44 | IMP 0x4433528; record 0xd77b7e4; dyld L2634; otool L31714 | medium / available / local_observation |
| base | - selectedIndex | `i16@0:8` | 0x4433658 | Local field operation: return *(int *)(param_1 + 0x38); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x4433658; record 0xd77b7f0; dyld L2635; otool L31717 | medium / available / local_verified |
| base | - command_bar_ranking_example | `{optional<arc::CommandBarRankingExample>=(?=c{CommandBarRankingExample={CommandBarRankingInput={vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}}{vector<arc::CommandBarRankingCandidate, std::allocator<arc::CommandBarRankingCandidate>>={__vector_layout<arc::CommandBarRankingCandidate, std::allocator<arc::CommandBarRankingCandidate>>=^{CommandBarRankingCandidate}QQ}}i})B}16@0:8` | 0x443366c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x40, 0x8. | 0x043dad44 | IMP 0x443366c; record 0xd77b7fc; dyld L2636; otool L31720 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x4433768 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x40, 0x20, 0x28, 0x30, 0x8, 0x10, 0x18. | 0x042a3ff4 | IMP 0x4433768; record 0xd77b808; dyld L2637; otool L31723 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x442cf84 | Initializes native storage/engagement fields at self offsets 0x8, 0x40; returns self. | local state / indirect operation; see body | IMP 0x442cf84; record 0xd77b814; dyld L2638; otool L29805 | medium / available / local_observation |
| Extensions | - input | `@16@0:8` | 0x4401ca0 | Extensions category copies native example, constructs ArcCommandBarRankingInput from input subobject, then cleans temporary example. | 0x042a3ff4 | IMP 0x4401ca0; record 0xd778dbc; dyld L4961; otool L62607 | medium / available / bounded_review |
| Extensions | - candidates | `@16@0:8` | 0x4401f34 | Incoherent pseudocode: apparent temporary-vector cleanup/count reset precedes array construction/reuse; candidate initializer arguments contain object-to-bool casts and in_stack values. Array and ArcCommandBarRankingCandidate allocations show conversion intent only. Array contents, argument mapping and lifetime are unresolved; no actual engine use-after-free, empty-result or corrupt-boundary conclusion follows. | 0x042a3ff4, 0x01c391bc | IMP 0x4401f34; record 0xd778dc8; dyld L4962; otool L62610 | low / available_incoherent_pseudocode / unresolved |

## Remaining consequential questions

Recover coherent candidates control flow, argument mapping and temporary ownership across 0x4401f34, native getter 0x443366c, initializer 0x4433528 and copy helper 0x43dad44; candidate contents, ordering and lifetime remain unresolved. Resolve selectedIndex validation and ownership of returned input wrappers. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 7; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
