# ArcSessionNavigation

ArcCore 1.170.0 arm64. 12 declared rows; superclass `NSObject`. Class metadata `0xe37e1e8`, class_ro `0xe2ae150`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Read-only session-entry wrapper exposing stored navigation data.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_session_navigation` | 0x8 | 24 | `{optional<arc::SessionNavigation>=""(?="__null_state_"c"__val_"{SessionNavigation="impl_"{shared_ptr<arc::SessionNavigation::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ae098` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `session_navigation`: `T{optional<arc::SessionNavigation>=(?=c{SessionNavigation={shared_ptr<arc::SessionNavigation::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_session_navigation` at `0xe2ae0c0`.
- `faviconURL`: `T@"NSURL",R,N` at `0xe2ae0d0`.
- `virtualURL`: `T@"NSURL",R,N` at `0xe2ae0e0`.
- `originalRequestURL`: `T@"NSURL",R,N` at `0xe2ae0f0`.
- `referrerURL`: `T@"NSURL",R,N` at `0xe2ae100`.
- `title`: `T@"NSString",R,C,N` at `0xe2ae110`.
- `hasPostData`: `TB,R,N` at `0xe2ae120`.
- `timestamp`: `T@"NSDate",R,N` at `0xe2ae130`.
- `isOverridingUserAgent`: `TB,R,N` at `0xe2ae140`.

## Lifecycle, ownership, callbacks and threading

0x443c968 copies shared native peer. URL getters 0x8374c34/0x8374ce8/0x8374d9c/0x8374e50 convert optional native URL strings, returning nil when absent. title 0x443ca8c reads UTF-16 data; timestamp 0x443cadc converts stored native time to NSDate since 1970 with zero/infinity handling. No disk write occurs in these getters.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed route inference only: Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/sessions/core/serialized_navigation_entry.h exposes virtual_url, title, referrer, timestamp and has_post_data; this does not identify the exact native Arc Impl or establish serialized format compatibility. A CEF in-memory navigation list alone is not proof of full Chrome session restoration.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithSessionNavigation: | `@24@0:8r^v16` | 0x443c968 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x443c968](../bodies/0x443c968.c); record 0xd77c894; dyld L3034 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -faviconURL | `@16@0:8` | 0x8374c34 | Requires engaged wrapper; reads optional native URL string at Impl+8 using 0x5533b24 and converts through 0x43abd34; absent yields nil. | 0x5533b24,0x43abd34,0x5810 | [0x8374c34](../bodies/0x8374c34.c); record 0xd77c8a0; dyld L3035 | medium / reviewed_partial / available | URL producer/storage and invalid URL conversion remain unresolved. |
| (base) | -virtualURL | `@16@0:8` | 0x8374ce8 | Requires engaged wrapper; reads optional native URL string at Impl+0x28 using 0x5533b24 and converts through 0x43abd34; absent yields nil. | 0x5533b24,0x43abd34,0x5810 | [0x8374ce8](../bodies/0x8374ce8.c); record 0xd77c8ac; dyld L3036 | medium / reviewed_partial / available | URL producer/storage and invalid URL conversion remain unresolved. |
| (base) | -originalRequestURL | `@16@0:8` | 0x8374d9c | Requires engaged wrapper; reads optional native URL string at Impl+0x48 using 0x5533b24 and converts through 0x43abd34; absent yields nil. | 0x5533b24,0x43abd34,0x5810 | [0x8374d9c](../bodies/0x8374d9c.c); record 0xd77c8b8; dyld L3037 | medium / reviewed_partial / available | URL producer/storage and invalid URL conversion remain unresolved. |
| (base) | -referrerURL | `@16@0:8` | 0x8374e50 | Requires engaged wrapper; reads optional native URL string at Impl+0x68 using 0x5533b24 and converts through 0x43abd34; absent yields nil. | 0x5533b24,0x43abd34,0x5810 | [0x8374e50](../bodies/0x8374e50.c); record 0xd77c8c4; dyld L3038 | medium / reviewed_partial / available | URL producer/storage and invalid URL conversion remain unresolved. |
| (base) | -title | `@16@0:8` | 0x443ca8c | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. Traps if outer optional is empty. | 0x1d9fb90 | [0x443ca8c](../bodies/0x443ca8c.c); record 0xd77c8d0; dyld L3039 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -hasPostData | `B16@0:8` | 0x443cac0 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x443cac0](../bodies/0x443cac0.c); record 0xd77c8dc; dyld L3040 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -timestamp | `@16@0:8` | 0x443cadc | Requires engaged peer; converts native timestamp to NSDate dateWithTimeIntervalSince1970, with zero and infinity branches. |  ; ObjC selectors dateWithTimeIntervalSince1970: | [0x443cadc](../bodies/0x443cadc.c); record 0xd77c8e8; dyld L3041 | medium / reviewed_partial / available | Validate epoch offset and extreme-time saturation from raw instructions. |
| (base) | -isOverridingUserAgent | `B16@0:8` | 0x443cb80 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x443cb80](../bodies/0x443cb80.c); record 0xd77c8f4; dyld L3042 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -session_navigation | `{optional<arc::SessionNavigation>=(?=c{SessionNavigation={shared_ptr<arc::SessionNavigation::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../bodies/0x43df53c.c); record 0xd77c900; dyld L3043 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../bodies/0x43df69c.c); record 0xd77c90c; dyld L3044 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../bodies/0x43df760.c); record 0xd77c918; dyld L3045 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover persisted session format, entry IDs, POST-data restoration policy and nil URL sentinel meanings.
