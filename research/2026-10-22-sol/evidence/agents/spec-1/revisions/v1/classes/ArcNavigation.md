# ArcNavigation

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37cd48`, class_ro `0xe2a96a0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native navigation-event identity wrapper and captured response-header dictionary.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_responseHeaders` | 0x8 | 8 | `@"NSDictionary"` | `0xe2a9608` |
| `_nativeNavigation` | 0x10 | 24 | `{optional<arc::WebContents::Navigation>=""(?="__null_state_"c"__val_"{Navigation="impl_"{shared_ptr<arc::WebContents::Navigation::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2a9628` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeNavigation`: `T{optional<arc::WebContents::Navigation>=(?=c{Navigation={shared_ptr<arc::WebContents::Navigation::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_nativeNavigation` at `0xe2a9650`.
- `url`: `T@"NSString",R,N` at `0xe2a9660`.
- `didRedirectToOtherTab`: `TB,R,N` at `0xe2a9670`.
- `responseStatusCode`: `Tq,R,N` at `0xe2a9680`.
- `responseHeaders`: `T@"NSDictionary",R,N` at `0xe2a9690`.

## Lifecycle, ownership, callbacks and threading

init: 0x4416808 copies native shared navigation and builds a dictionary from native string key/value pairs when nonempty. responseHeaders 0x43dd450 returns that captured field; url 0x4416b90, redirect flag 0x4416bbc and status 0x4416bd4 read live native fields. isEqual: 0x44169e8 compares peer pointers after class/optional checks; hash 0x4416b58 hashes 8 bytes of that pointer.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is event identity, not URL value equality. A replacement navigation observer needs distinct objects/lifetimes and header capture rules. The native producer and Chromium NavigationHandle relationship remain outside this wrapper review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -init: | `@32@0:8{Navigation={shared_ptr<arc::WebContents::Navigation::Impl>=^{Impl}^{__shared_weak_count}}}16` | 0x4416808 | Calls super init, copies shared native navigation into optional at self+0x10 and builds stored response-header dictionary when native headers are nonempty. | 0x5810,0x1c391bc ; ObjC selectors dictionaryWithCapacity:,setObject:forKeyedSubscript: | [0x4416808](../bodies/0x4416808.c); record 0xd77a550; dyld L2193 | medium / local_verified / available | Header duplicate/case and snapshot timing require producer audit. |
| (base) | -isEqual: | `B24@0:8@16` | 0x44169e8 | Identity fast path; same-class objects compare optional engagement and native peer pointer. | 0x5810 ; ObjC selectors isKindOfClass: | [0x44169e8](../bodies/0x44169e8.c); record 0xd77a55c; dyld L2194 | medium / local_verified / available | Validate empty-optional class edge cases. |
| (base) | -hash | `Q16@0:8` | 0x4416b58 | Requires engaged optional; hashes 8-byte native peer pointer via 0x18f3c. | 0x18f3c,0x4b08a00 | [0x4416b58](../bodies/0x4416b58.c); record 0xd77a568; dyld L2195 | medium / local_verified / available | Hash stability across native detachment remains unverified. |
| (base) | -url | `@16@0:8` | 0x4416b90 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x4416b90](../bodies/0x4416b90.c); record 0xd77a574; dyld L2196 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -didRedirectToOtherTab | `B16@0:8` | 0x4416bbc | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4416bbc](../bodies/0x4416bbc.c); record 0xd77a580; dyld L2197 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -responseStatusCode | `q16@0:8` | 0x4416bd4 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x4416bd4](../bodies/0x4416bd4.c); record 0xd77a58c; dyld L2198 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -responseHeaders | `@16@0:8` | 0x43dd450 | Returns native/inline field. | No direct anonymous call recorded / indirect possible | [0x43dd450](../bodies/0x43dd450.c); record 0xd77a598; dyld L2199 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -nativeNavigation | `{optional<arc::WebContents::Navigation>=(?=c{Navigation={shared_ptr<arc::WebContents::Navigation::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x440a2ac | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x440a2ac](../bodies/0x440a2ac.c); record 0xd77a5a4; dyld L2200 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x440a40c | Destroys backing storage with shared-count decrement and last-owner virtual destruction; clears object/backing fields. ARC injection may hide retain/release operations. | 0x5810 | [0x440a40c](../bodies/0x440a40c.c); record 0xd77a5b0; dyld L2201 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x440a4dc | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x440a4dc](../bodies/0x440a4dc.c); record 0xd77a5bc; dyld L2202 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Does header snapshot omit duplicates and preserve case, and can native event state change after callback?
