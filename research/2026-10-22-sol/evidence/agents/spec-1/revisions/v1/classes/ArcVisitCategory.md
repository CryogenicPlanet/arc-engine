# ArcVisitCategory

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37d068`, class_ro `0xe2aa180`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline category-id string and signed integer weight, stored in an optional native value.

## State and layout

Metadata instance start 8, size 48 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_visit_category` | 0x8 | 40 | `{optional<arc::VisitCategory>=""(?="__null_state_"c"__val_"{VisitCategory="id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"weight"i})"__engaged_"B}` | `0xe2aa128` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `visit_category`: `T{optional<arc::VisitCategory>=(?=c{VisitCategory={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}i})B},R,N,V_visit_category` at `0xe2aa150`.
- `categoryId`: `T@"NSString",R,C,N` at `0xe2aa160`.
- `weight`: `Ti,R,N` at `0xe2aa170`.

## Lifecycle, ownership, callbacks and threading

0x442c27c constructs an engaged value from string/id and int weight; nil id becomes empty native string. 0x442c398 copies a native value. mutableVisitCategory 0x442c260 returns the address of inline storage when engaged; categoryId 0x43ed470 converts UTF-8 and weight 0x442c440 reads +0x20. 0x43ed510 frees long string storage.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is a value adapter for history/category producers; no ranking or persistence service is invoked by these accessors. Its mutable raw pointer and C++ aggregate return must remain private to matching native callers or be adapted explicitly.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -mutableVisitCategory | `^v16@0:8` | 0x442c260 | Returns address of inline native value at self+8 when engaged; aborts on empty optional. | 0x4b08a00 | [0x442c260](../bodies/0x442c260.c); record 0xd77ab44; dyld L2334 | high / local_verified / available | Caller mutation and pointer escape lifetime must be audited. |
| (base) | -initCategoryId:weight: | `@28@0:8@16i24` | 0x442c27c | Calls super init, engages native value with converted id string (nil becomes empty) and signed int weight at self+0x20. | 0x5810,0x1b356b8 | [0x442c27c](../bodies/0x442c27c.c); record 0xd77ab50; dyld L2335 | medium / local_verified / available | Recover category vocabulary and weight units from producer. |
| (base) | -initWithVisitCategory: | `@24@0:8r^v16` | 0x442c398 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x5810,0x60a60 | [0x442c398](../bodies/0x442c398.c); record 0xd77ab5c; dyld L2336 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -categoryId | `@16@0:8` | 0x43ed470 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x43ed470](../bodies/0x43ed470.c); record 0xd77ab68; dyld L2337 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -weight | `i16@0:8` | 0x442c440 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442c440](../bodies/0x442c440.c); record 0xd77ab74; dyld L2338 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -visit_category | `{optional<arc::VisitCategory>=(?=c{VisitCategory={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}i})B}16@0:8` | 0x442c454 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x60a60 | [0x442c454](../bodies/0x442c454.c); record 0xd77ab80; dyld L2339 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43ed510 | Destroys backing storage. ARC injection may hide retain/release operations. | 0x5810 | [0x43ed510](../bodies/0x43ed510.c); record 0xd77ab8c; dyld L2340 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43ed538 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43ed538](../bodies/0x43ed538.c); record 0xd77ab98; dyld L2341 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
