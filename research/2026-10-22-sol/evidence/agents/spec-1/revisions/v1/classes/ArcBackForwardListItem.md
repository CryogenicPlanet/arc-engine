# ArcBackForwardListItem

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37bc18`, class_ro `0xe2a46e8`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Shared native history-entry wrapper exposing title, URL and validity.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_nativeBackForwardListItem` | 0x8 | 24 | `{optional<arc::BackForwardListItem>=""(?="__null_state_"c"__val_"{BackForwardListItem="impl_"{shared_ptr<arc::BackForwardListItem::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2a4680` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeBackForwardListItem`: `T{optional<arc::BackForwardListItem>=(?=c{BackForwardListItem={shared_ptr<arc::BackForwardListItem::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_nativeBackForwardListItem` at `0xe2a46a8`.
- `title`: `T@"NSString",R,N` at `0xe2a46b8`.
- `url`: `T@"NSURL",R,N` at `0xe2a46c8`.
- `isValid`: `TB,R,N` at `0xe2a46d8`.

## Lifecycle, ownership, callbacks and threading

0x43df298 copies a shared native peer into optional storage. 0x43df3bc converts UTF-16 title; 0x43df47c converts the URL through 0x43abd34. isValid 0x43df524 reads native +0x30 after checking engagement; it is not a wrapper liveness test. 0x43df57c assigns/clears the optional and 0x43df69c releases its control block.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Navigation entry data can be adapted from Chromium content APIs, but these shared Arc wrappers and native equality/invalidity semantics are an additional contract. No direct CEF equivalence has been established.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithBackForwardListItem: | `@24@0:8r^v16` | 0x43df298 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x43df298](../bodies/0x43df298.c); record 0xd777ae8; dyld L1326 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -title | `@16@0:8` | 0x43df3bc | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. Traps if outer optional is empty. | 0x54420,0x1d9fb90,0x5810 | [0x43df3bc](../bodies/0x43df3bc.c); record 0xd777af4; dyld L1327 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -url | `@16@0:8` | 0x43df47c | Copies native URL string at peer+0x18, converts through 0x43abd34 and releases temporary storage. | 0x60a60,0x43abd34,0x5810 | [0x43df47c](../bodies/0x43df47c.c); record 0xd777b00; dyld L1328; evidence/bridges-conversions.txt | medium / reviewed_partial / available | 0x43abd34 URL conversion tail and invalid URL behavior remain unverified. |
| (base) | -isValid | `B16@0:8` | 0x43df524 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x43df524](../bodies/0x43df524.c); record 0xd777b0c; dyld L1329 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -nativeBackForwardListItem | `{optional<arc::BackForwardListItem>=(?=c{BackForwardListItem={shared_ptr<arc::BackForwardListItem::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../bodies/0x43df53c.c); record 0xd777b18; dyld L1330 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -setNativeBackForwardListItem: | `v40@0:8{optional<arc::BackForwardListItem>=(?=c{BackForwardListItem={shared_ptr<arc::BackForwardListItem::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | 0x43df57c | Assigns optional shared peer: copy/replace when engaged, increments new control count, releases old control block; clears engagement for empty input. | 0x4331e20,0x5810 | [0x43df57c](../bodies/0x43df57c.c); record 0xd777b24; dyld L1331 | medium / local_verified / available | Validate aggregate input ABI and detached peer handling. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../bodies/0x43df69c.c); record 0xd777b30; dyld L1332 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../bodies/0x43df760.c); record 0xd777b3c; dyld L1333 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
