# ArcCommandBarRankingCandidate

ArcCore 1.170.0 arm64. 19 declared rows; superclass `NSObject`. Class metadata `0xe37d838`, class_ro `0xe2abe90`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline ranking-input feature record, including strings, counts, engagement, match flags and title embedding.

## State and layout

Metadata instance start 8, size 184 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_command_bar_ranking_candidate` | 0x8 | 176 | `{optional<arc::CommandBarRankingCandidate>=""(?="__null_state_"c"__val_"{CommandBarRankingCandidate="id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"archetype"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"destination"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"title"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"url"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"last_nav_seconds"i"freq_ever"i"typed_count"i"site_engagement"f"is_exact_url_match"B"is_base_domain_match"B"is_title_prefix"B"title_embedding"{vector<float, std::allocator<float>>="__layout_"{__vector_layout<float, std::allocator<float>>="__begin_"^f"__size_"Q"__capacity_"Q}}})"__engaged_"B}` | `0xe2abd88` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_candidate`: `T{optional<arc::CommandBarRankingCandidate>=(?=c{CommandBarRankingCandidate={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}iiifBBB{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B},R,N,V_command_bar_ranking_candidate` at `0xe2abdb0`.
- `candidateID`: `T@"NSString",R,C,N` at `0xe2abdc0`.
- `archetype`: `T@"NSString",R,C,N` at `0xe2abdd0`.
- `destination`: `T@"NSString",R,C,N` at `0xe2abde0`.
- `title`: `T@"NSString",R,C,N` at `0xe2abdf0`.
- `url`: `T@"NSString",R,C,N` at `0xe2abe00`.
- `lastNavSeconds`: `Ti,R,N` at `0xe2abe10`.
- `freqEver`: `Ti,R,N` at `0xe2abe20`.
- `typedCount`: `Ti,R,N` at `0xe2abe30`.
- `siteEngagement`: `Tf,R,N` at `0xe2abe40`.
- `isExactURLMatch`: `TB,R,N` at `0xe2abe50`.
- `isBaseDomainMatch`: `TB,R,N` at `0xe2abe60`.
- `isTitlePrefix`: `TB,R,N` at `0xe2abe70`.
- `titleEmbedding`: `T@"NSArray",R,C,N` at `0xe2abe80`.

## Lifecycle, ownership, callbacks and threading

0x4434540 converts five ObjC strings (nil becomes empty), stores integer/float/boolean features and converts title embedding through 0x443181c. 0x4434900 copies native candidate using 0x42a3ea8. mutableCommandBarRankingCandidate 0x4434524 returns inline address when engaged; 0x4434af0 boxes the float-vector embedding into a new array. 0x4434c14 destroys owned candidate state through 0x42a3ff4.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is feature transport, not a ranking algorithm. CandidateID/archetype/destination/title/url are distinct strings (0x4434974..0x4434a34); lastNavSeconds/freqEver/typedCount are int, siteEngagement float and three match flags bytes (0x4434a64..0x4434adc). Preserve type distinctions when replacing the model adapter.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -mutableCommandBarRankingCandidate | `^v16@0:8` | 0x4434524 | Returns address of inline native value at self+8 when engaged; aborts on empty optional. | 0x4b08a00 | [0x4434524](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434524.c); record 0xd77b89c; dyld L2652 | high / local_verified / available | Caller mutation and pointer escape lifetime must be audited. |
| (base) | -init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding: | `@92@0:8@16@24@32@40@48i56i60i64f68B72B76B80@84` | 0x4434540 | Builds engaged native candidate: five converted strings, three int features, float site engagement, three boolean flags and converted float title embedding. | 0x42a3ff4,0x1b356b8,0x5810,0x443181c | [0x4434540](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434540.c); record 0xd77b8a8; dyld L2653 | medium / reviewed_partial / available | Ghidra parameter/stack recovery is imperfect; validate mixed integer/float/stack ABI and embedding conversion. |
| (base) | -initWithCommandBarRankingCandidate: | `@24@0:8r^v16` | 0x4434900 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x42a3ff4,0x42a3ea8 | [0x4434900](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434900.c); record 0xd77b8b4; dyld L2654 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -candidateID | `@16@0:8` | 0x4434974 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x4434974](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434974.c); record 0xd77b8c0; dyld L2655 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -archetype | `@16@0:8` | 0x44349a4 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x44349a4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44349a4.c); record 0xd77b8cc; dyld L2656 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -destination | `@16@0:8` | 0x44349d4 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x44349d4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44349d4.c); record 0xd77b8d8; dyld L2657 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -title | `@16@0:8` | 0x4434a04 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x4434a04](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434a04.c); record 0xd77b8e4; dyld L2658 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -url | `@16@0:8` | 0x4434a34 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x4434a34](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434a34.c); record 0xd77b8f0; dyld L2659 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -lastNavSeconds | `i16@0:8` | 0x4434a64 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434a64](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434a64.c); record 0xd77b8fc; dyld L2660 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -freqEver | `i16@0:8` | 0x4434a78 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434a78](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434a78.c); record 0xd77b908; dyld L2661 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -typedCount | `i16@0:8` | 0x4434a8c | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434a8c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434a8c.c); record 0xd77b914; dyld L2662 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -siteEngagement | `f16@0:8` | 0x4434aa0 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434aa0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434aa0.c); record 0xd77b920; dyld L2663 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -isExactURLMatch | `B16@0:8` | 0x4434ab4 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434ab4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434ab4.c); record 0xd77b92c; dyld L2664 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -isBaseDomainMatch | `B16@0:8` | 0x4434ac8 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434ac8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434ac8.c); record 0xd77b938; dyld L2665 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -isTitlePrefix | `B16@0:8` | 0x4434adc | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4434adc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434adc.c); record 0xd77b944; dyld L2666 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -titleEmbedding | `@16@0:8` | 0x4434af0 | Requires engaged optional; returns a new NSMutableArray of boxed native float embedding elements. |  ; ObjC selectors arrayWithCapacity:,numberWithFloat:,addObject: | [0x4434af0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434af0.c); record 0xd77b950; dyld L2667 | medium / local_verified / available | Dimension and NaN contracts require model/producer review. |
| (base) | -command_bar_ranking_candidate | `{optional<arc::CommandBarRankingCandidate>=(?=c{CommandBarRankingCandidate={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}iiifBBB{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B}16@0:8` | 0x4434bb0 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x42a3ea8 | [0x4434bb0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434bb0.c); record 0xd77b95c; dyld L2668 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4434c14 | Destroys backing storage. ARC injection may hide retain/release operations. | 0x42a3ff4 | [0x4434c14](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434c14.c); record 0xd77b968; dyld L2669 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x4434c28 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x4434c28](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4434c28.c); record 0xd77b974; dyld L2670 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover archetype/destination vocabulary, count/time units, feature normalization and embedding dimension requirements.


Original reviewed source: [ArcCommandBarRankingCandidate.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcCommandBarRankingCandidate.md).
