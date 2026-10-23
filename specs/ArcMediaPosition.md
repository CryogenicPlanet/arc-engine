# ArcMediaPosition

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37e0f8`, class_ro `0xe2ade80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared media_session::MediaPosition object with time projections.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_media_position` | 0x8 | 24 | `{optional<arc::MediaPosition>=""(?="__null_state_"c"__val_"{MediaPosition="impl_"{shared_ptr<media_session::MediaPosition>="__ptr_"^{MediaPosition}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ade08` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `media_position`: `T{optional<arc::MediaPosition>=(?=c{MediaPosition={shared_ptr<media_session::MediaPosition>=^{MediaPosition}^{__shared_weak_count}}})B},R,N,V_media_position` at `0xe2ade30`.
- `playbackRate`: `Td,R,N` at `0xe2ade40`.
- `duration`: `Td,R,N` at `0xe2ade50`.
- `currentTime`: `Td,R,N` at `0xe2ade60`.
- `endOfMedia`: `TB,R,N` at `0xe2ade70`.

## Lifecycle, ownership, callbacks and threading

0x443c1dc copies a shared native position. playbackRate 0x443c300 loads native double; duration 0x443c318 divides native microseconds by 1e6, preserving signed infinity sentinels. currentTime 0x443c36c calls 0x32b5dd0 then applies the same conversion. That helper computes elapsed-time*rate, adds base position and clamps to [0,duration]; its pseudo-C return type is wrong, so source identity remains an inference. endOfMedia 0x443c3cc reads a separate flag, not currentTime==duration.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: 0x32b5dd0 corresponds to MediaPosition::GetPosition/GetPositionAtTime logic at Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c services/media_session/public/cpp/media_position.cc:43-77; private field order in media_position.h:98-110 agrees with the metadata type and observed offsets. The header :63-67 explicitly says end_of_media may be true before duration is reached. No CEF exact timing contract is established.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithMediaPosition: | `@24@0:8r^v16` | 0x443c1dc | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x443c1dc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443c1dc.c); record 0xd77c75c; dyld L3004 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -playbackRate | `d16@0:8` | 0x443c300 | Requires engaged optional; returns native double at peer+0. | No direct anonymous call recorded / indirect possible | [0x443c300](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443c300.c); record 0xd77c768; dyld L3005; evidence/bridges-deep.txt | medium / reviewed_partial / available | Time source/edge-case runtime behavior remains unverified. |
| (base) | -duration | `d16@0:8` | 0x443c318 | Requires engaged optional; converts signed microsecond duration to double seconds, mapping sentinel extremes to signed infinity. | No direct anonymous call recorded / indirect possible | [0x443c318](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443c318.c); record 0xd77c774; dyld L3006; evidence/bridges-deep.txt | medium / reviewed_partial / available | Time source/edge-case runtime behavior remains unverified. |
| (base) | -currentTime | `d16@0:8` | 0x443c36c | Requires engaged optional; calls 0x32b5dd0 for advancing/clamped position then converts microseconds to seconds with infinity handling. | 0x32b5dd0 | [0x443c36c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443c36c.c); record 0xd77c780; dyld L3007; evidence/bridges-deep.txt | medium / reviewed_partial / available | Time source/edge-case runtime behavior remains unverified. |
| (base) | -endOfMedia | `B16@0:8` | 0x443c3cc | Requires engaged optional; reads independent native flag at peer+0x20. | No direct anonymous call recorded / indirect possible | [0x443c3cc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443c3cc.c); record 0xd77c78c; dyld L3008; evidence/bridges-deep.txt | medium / reviewed_partial / available | Time source/edge-case runtime behavior remains unverified. |
| (base) | -media_position | `{optional<arc::MediaPosition>=(?=c{MediaPosition={shared_ptr<media_session::MediaPosition>=^{MediaPosition}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df53c.c); record 0xd77c798; dyld L3009 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df69c.c); record 0xd77c7a4; dyld L3010 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df760.c); record 0xd77c7b0; dyld L3011 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Confirm time source and saturated arithmetic against bounded ARM64; test paused, negative-rate, infinite-duration and end-of-media cases.


Original reviewed source: [ArcMediaPosition.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcMediaPosition.md).
