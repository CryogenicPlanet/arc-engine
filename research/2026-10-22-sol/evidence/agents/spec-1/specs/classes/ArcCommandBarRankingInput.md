# ArcCommandBarRankingInput

ArcCore 1.170.0 arm64. 7 declared rows; superclass `NSObject`. Class metadata `0xe37d748`, class_ro `0xe2abaf0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline optional vector of float query-embedding components.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_command_bar_ranking_input` | 0x8 | 32 | `{optional<arc::CommandBarRankingInput>=""(?="__null_state_"c"__val_"{CommandBarRankingInput="query_embedding"{vector<float, std::allocator<float>>="__layout_"{__vector_layout<float, std::allocator<float>>="__begin_"^f"__size_"Q"__capacity_"Q}}})"__engaged_"B}` | `0xe2abaa8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_input`: `T{optional<arc::CommandBarRankingInput>=(?=c{CommandBarRankingInput={vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B},R,N,V_command_bar_ranking_input` at `0xe2abad0`.
- `queryEmbedding`: `T@"NSArray",R,C,N` at `0xe2abae0`.

## Lifecycle, ownership, callbacks and threading

0x4432bfc converts an ObjC collection through 0x443181c into native float-vector storage; queryEmbedding 0x4432e20 creates NSMutableArray and boxes each float. mutableCommandBarRankingInput 0x4432be0 returns inline vector address when engaged. 0x4432f6c destroys vector allocation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The adapter preserves float order; model dimensions and input validation belong to conversion helper/model, not this getter. No stock Chromium/CEF inference API equivalence is asserted.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -mutableCommandBarRankingInput | `^v16@0:8` | 0x4432be0 | Returns address of inline native value at self+8 when engaged; aborts on empty optional. | 0x4b08a00 | [0x4432be0](../bodies/0x4432be0.c); record 0xd77b770; dyld L2623 | high / local_verified / available | Caller mutation and pointer escape lifetime must be audited. |
| (base) | -init: | `@24@0:8@16` | 0x4432bfc | Calls super init; engages empty native vector then replaces it with float-vector conversion from 0x443181c. | 0x5810,0x443181c | [0x4432bfc](../bodies/0x4432bfc.c); record 0xd77b77c; dyld L2624 | medium / reviewed_partial / available | Conversion element types, dimension limits and NaN handling remain unverified. |
| (base) | -initWithCommandBarRankingInput: | `@24@0:8r^v16` | 0x4432d2c | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x5810,0x4047380,0x8790 | [0x4432d2c](../bodies/0x4432d2c.c); record 0xd77b788; dyld L2625 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -queryEmbedding | `@16@0:8` | 0x4432e20 | Requires engaged optional; creates NSMutableArray of NSNumber floats in native vector order. |  ; ObjC selectors arrayWithCapacity:,numberWithFloat:,addObject: | [0x4432e20](../bodies/0x4432e20.c); record 0xd77b794; dyld L2626 | medium / local_verified / available | Mutation of returned array does not establish writeback; validate caller expectations. |
| (base) | -command_bar_ranking_input | `{optional<arc::CommandBarRankingInput>=(?=c{CommandBarRankingInput={vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B}16@0:8` | 0x4432ee0 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x4047380,0x8790 | [0x4432ee0](../bodies/0x4432ee0.c); record 0xd77b7a0; dyld L2627 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4432f6c | Destroys backing storage; clears object/backing fields. ARC injection may hide retain/release operations. | 0x5810 | [0x4432f6c](../bodies/0x4432f6c.c); record 0xd77b7ac; dyld L2628 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x4432fc0 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x4432fc0](../bodies/0x4432fc0.c); record 0xd77b7b8; dyld L2629 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover vector length/dimension validation and NSNumber conversion policy in 0x443181c.
