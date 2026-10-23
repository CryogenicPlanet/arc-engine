# ArcNetworkLoggingConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 10 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e148`, class_ro `0xe2adf70`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 32 | `_network_logging_configuration` | `{NetworkLoggingConfiguration="file_path"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__s…` | 0xe2adf18 |

A native file path string begins +8, with duration double at +0x20. Default init clears the path; no explicit duration store is visible in this initializer (0x443c3e4); filePath converts the native string (0x43e632c), setter converts input bytes (0x443c500), duration getter loads +0x20 (0xed0b1c).

## Lifecycle, callbacks and relations

Aggregate getter/copy setter duplicate string state (0x443c588, 0x443c5d8); destructor frees a heap string (0x440d470). It is consumed by startLogging at 0x44088ac; this class itself opens no output file.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent logging options; duration interpretation and file lifecycle belong to the monitor backend at 0x432c950.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcNetworkLoggingConfiguration.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init | `@16@0:8` | 0x443c3e4 | Clears native path string; no explicit duration write visible in this initializer. | local state / indirect operation; see body | IMP 0x443c3e4; record 0xd77c7c4; dyld L3014; otool L34479 | medium / available / bounded_review |
| base | - initWithNetworkLoggingConfiguration: | `@24@0:8r^v16` | 0x443c450 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x00359ad8, 0x00359998 | IMP 0x443c450; record 0xd77c7d0; dyld L3015; otool L34482 | medium / available / local_observation |
| base | - filePath | `@16@0:8` | 0x43e632c | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x1f, 0x8, 0x10. | 0x01c391bc | IMP 0x43e632c; record 0xd77c7dc; dyld L3016; otool L23413 | medium / available / local_observation |
| base | - setFilePath: | `v24@0:8@16` | 0x443c500 | Assigns/replaces local native or ObjC state; self offsets 0x1f, 0x8, 0x18, 0x10. Calls 0x01b356b8. | 0x01b356b8 | IMP 0x443c500; record 0xd77c7e8; dyld L3017; otool L34488 | medium / available / local_observation |
| base | - loggingDurationInSeconds | `d16@0:8` | 0xed0b1c | Reads 64-bit floating value from self+0x20 (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0xed0b1c; record 0xd77c7f4; dyld L3018; otool L15508 | high / available / local_verified |
| base | - setLoggingDurationInSeconds: | `v24@0:8d16` | 0x443c580 | Local field operation: *(double *)(param_1 + 0x20) = param_3; | local state / indirect operation; see body | IMP 0x443c580; record 0xd77c800; dyld L3019; otool L34494 | medium / available / local_verified |
| base | - network_logging_configuration | `{NetworkLoggingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}d}16@0:8` | 0x443c588 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x1f, 0x8, 0x10, 0x18, 0x20. | 0x00060a60 | IMP 0x443c588; record 0xd77c80c; dyld L3020; otool L34497 | medium / available / local_observation |
| base | - setNetwork_logging_configuration: | `v48@0:8{NetworkLoggingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}d}16` | 0x443c5d8 | Assigns/replaces local native or ObjC state; self offsets 0x8, 0x1f, 0x18, 0x10, 0x20. Calls 0x00359ad8, 0x00359998. | 0x00359ad8, 0x00359998 | IMP 0x443c5d8; record 0xd77c818; dyld L3021; otool L34500 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x440d470 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x440d470; record 0xd77c824; dyld L3022; otool L28035 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x416a680 | Initializes native storage/engagement fields at self offsets 0x8, 0x10, 0x18; returns self. | local state / indirect operation; see body | IMP 0x416a680; record 0xd77c830; dyld L3023; otool L8137 | medium / available / local_observation |

## Remaining consequential questions

Confirm allocation/default duration and allowed zero/negative/nonfinite values; establish path validation and overwrite behavior in writer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcNetworkLoggingConfiguration.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcNetworkLoggingConfiguration.md).
