# ArcCommandBarRankingResult

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37d6a8`, class_ro `0xe2ab920`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline candidate-id string and float score record.

## State and layout

Metadata instance start 8, size 48 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_command_bar_ranking_result` | 0x8 | 40 | `{optional<arc::CommandBarRankingResult>=""(?="__null_state_"c"__val_"{CommandBarRankingResult="id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"score"f})"__engaged_"B}` | `0xe2ab8c8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_result`: `T{optional<arc::CommandBarRankingResult>=(?=c{CommandBarRankingResult={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}f})B},R,N,V_command_bar_ranking_result` at `0xe2ab8f0`.
- `candidateID`: `T@"NSString",R,C,N` at `0xe2ab900`.
- `score`: `Tf,R,N` at `0xe2ab910`.

## Lifecycle, ownership, callbacks and threading

0x44326cc constructs string/score and engages optional. candidateID 0x43ed470 converts UTF-8 string; score 0x4432890 loads float +0x20. mutableCommandBarRankingResult aliases 0x442c260 used by ArcVisitCategory; identical code does not make the score an integer. Native getters/copy constructors retain their own metadata encodings.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This wrapper carries ranking output; no inference or ranking model runs in 0x44326cc. The Arc ranking producer is outside this assignment and cannot be replaced by a CEF navigation suggestion API based only on these fields.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -mutableCommandBarRankingResult | `^v16@0:8` | 0x442c260 | Returns address of inline native value at self+8 when engaged; aborts on empty optional. | 0x4b08a00 | [0x442c260](../bodies/0x442c260.c); record 0xd77b6a0; dyld L2603 | high / local_verified / available | Caller mutation and pointer escape lifetime must be audited. |
| (base) | -init:score: | `@28@0:8@16f24` | 0x44326cc | Calls super init; builds engaged native value with candidate-id string (nil becomes empty) and float score at self+0x20. | 0x5810,0x1b356b8 | [0x44326cc](../bodies/0x44326cc.c); record 0xd77b6ac; dyld L2604 | medium / local_verified / available | Ranking score range, normalization and ordering are producer contracts. |
| (base) | -initWithCommandBarRankingResult: | `@24@0:8r^v16` | 0x44327e8 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x5810,0x60a60 | [0x44327e8](../bodies/0x44327e8.c); record 0xd77b6b8; dyld L2605 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -candidateID | `@16@0:8` | 0x43ed470 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x43ed470](../bodies/0x43ed470.c); record 0xd77b6c4; dyld L2606 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -score | `f16@0:8` | 0x4432890 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4432890](../bodies/0x4432890.c); record 0xd77b6d0; dyld L2607 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -command_bar_ranking_result | `{optional<arc::CommandBarRankingResult>=(?=c{CommandBarRankingResult={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}f})B}16@0:8` | 0x44328a4 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x60a60 | [0x44328a4](../bodies/0x44328a4.c); record 0xd77b6dc; dyld L2608 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43ed510 | Destroys backing storage. ARC injection may hide retain/release operations. | 0x5810 | [0x43ed510](../bodies/0x43ed510.c); record 0xd77b6e8; dyld L2609 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43ed538 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43ed538](../bodies/0x43ed538.c); record 0xd77b6f4; dyld L2610 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
