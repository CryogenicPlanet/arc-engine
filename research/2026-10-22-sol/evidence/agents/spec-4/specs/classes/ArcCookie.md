# ArcCookie

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 26 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37d568`, class_ro `0xe2ab3c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_cookie` | 8 | 24 | `{optional<arc::Cookie>=""(?="__null_state_"c"__val_"{Cookie="impl_"{shared_ptr<arc::Cookie::Impl>="__ptr_"^{Impl}"__cntr… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native Impl; `0x442ffa0` creates native cookie and `0x4430144` shares a supplied handle. String getters/setters operate on Impl fields; flags at +0x60..+0x63, expiration double +0x68 and SameSite int +0x70 (`0x4430870`..`0x4430948`). Mutations are direct record changes, not demonstrated cookie-store writes. Initializer IMPs: `0x442ffa0`, `0x4430144`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Must map Arc cookie fields to the chosen backend and preserve expiry units/SameSite/hostOnly/session distinctions. Store operations occur on ArcBrowserContext (`0x43f2954`, `0x43f2aec`, `0x43f2ca8`); CEF cookie-manager semantic coverage is unverified here.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableCookie` | `^v16@0:8` | [0x442c4f0](../bodies/442c4f0.c) | Returns pointer to inline native value at self+8 when optional is engaged; disengaged path calls 0x4b08a00. | field/indirect dispatch; see body | M `0xd77b344`; D 2526 | H; available; local_field_observed |
| (base) | `-init` | `@16@0:8` | [0x442ffa0](../bodies/442ffa0.c) | Initializes native cookie shared Impl with a newly created native cookie value; stores engaged optional handle. | 0x16a2f44; 0x43345f8 | M `0xd77b350`; D 2527 | M; available; bridge_reviewed_partial |
| (base) | `-initWithCookie:` | `@24@0:8r^v16` | [0x4430144](../bodies/4430144.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77b35c`; D 2528 | M; available; local_body_observed |
| (base) | `-name` | `@16@0:8` | [0x4430268](../bodies/4430268.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77b368`; D 2529 | M; available; local_body_observed |
| (base) | `-setName:` | `v24@0:8@16` | [0x4430328](../bodies/4430328.c) | Requires engaged cookie; converts name NSString to native byte string (nil becomes empty) and assigns native Impl string at offset 0. | 0x359998; 0x359ad8; 0x1b356b8 | M `0xd77b374`; D 2530 | M; available; bridge_reviewed_partial |
| (base) | `-value` | `@16@0:8` | [0x440ae60](../bodies/440ae60.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77b380`; D 2531 | M; available; local_body_observed |
| (base) | `-setValue:` | `v24@0:8@16` | [0x443040c](../bodies/443040c.c) | Requires engaged cookie; converts value NSString to native byte string (nil becomes empty) and assigns Impl string at +0x18. | 0x359998; 0x359ad8; 0x1b356b8 | M `0xd77b38c`; D 2532 | M; available; bridge_reviewed_partial |
| (base) | `-domain` | `@16@0:8` | [0x44304f4](../bodies/44304f4.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77b398`; D 2533 | M; available; local_body_observed |
| (base) | `-setDomain:` | `v24@0:8@16` | [0x44305b4](../bodies/44305b4.c) | Requires engaged cookie; converts domain NSString to native byte string (nil becomes empty) and assigns Impl string at +0x48. | 0x359998; 0x359ad8; 0x1b356b8 | M `0xd77b3a4`; D 2534 | M; available; bridge_reviewed_partial |
| (base) | `-path` | `@16@0:8` | [0x4430698](../bodies/4430698.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77b3b0`; D 2535 | M; available; local_body_observed |
| (base) | `-setPath:` | `v24@0:8@16` | [0x4430758](../bodies/4430758.c) | Requires engaged cookie; converts path NSString to native byte string (nil becomes empty) and assigns Impl string at +0x30. | 0x359998; 0x359ad8; 0x1b356b8 | M `0xd77b3bc`; D 2536 | M; available; bridge_reviewed_partial |
| (base) | `-hostOnly` | `B16@0:8` | [0x4430840](../bodies/4430840.c) | Reads native field and returns expression (bool)*(byte *)(*(long *)(param_1 + 8) + 0x61). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b3c8`; D 2537 | H; available; local_field_observed |
| (base) | `-setHostOnly:` | `v20@0:8B16` | [0x4430858](../bodies/4430858.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b3d4`; D 2538 | M; available; local_body_observed |
| (base) | `-httpOnly` | `B16@0:8` | [0x4430870](../bodies/4430870.c) | Reads native field and returns expression (bool)*(byte *)(*(long *)(param_1 + 8) + 0x60). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b3e0`; D 2539 | H; available; local_field_observed |
| (base) | `-setHttpOnly:` | `v20@0:8B16` | [0x4430888](../bodies/4430888.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b3ec`; D 2540 | M; available; local_body_observed |
| (base) | `-secure` | `B16@0:8` | [0x44308a0](../bodies/44308a0.c) | Reads native field and returns expression (bool)*(byte *)(*(long *)(param_1 + 8) + 0x62). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b3f8`; D 2541 | H; available; local_field_observed |
| (base) | `-setSecure:` | `v20@0:8B16` | [0x44308b8](../bodies/44308b8.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b404`; D 2542 | M; available; local_body_observed |
| (base) | `-session` | `B16@0:8` | [0x44308d0](../bodies/44308d0.c) | Reads native field and returns expression (bool)*(byte *)(*(long *)(param_1 + 8) + 99). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b410`; D 2543 | H; available; local_field_observed |
| (base) | `-setSession:` | `v20@0:8B16` | [0x44308e8](../bodies/44308e8.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b41c`; D 2544 | M; available; local_body_observed |
| (base) | `-expirationDate` | `d16@0:8` | [0x4430900](../bodies/4430900.c) | Reads native field and returns expression *(double *)(*(long *)(param_1 + 8) + 0x68). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b428`; D 2545 | H; available; local_field_observed |
| (base) | `-setExpirationDate:` | `v24@0:8d16` | [0x4430918](../bodies/4430918.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b434`; D 2546 | M; available; local_body_observed |
| (base) | `-sameSite` | `q16@0:8` | [0x4430930](../bodies/4430930.c) | Reads native field and returns expression (long)*(int *)(*(long *)(param_1 + 8) + 0x70). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b440`; D 2547 | H; available; local_field_observed |
| (base) | `-setSameSite:` | `v24@0:8q16` | [0x4430948](../bodies/4430948.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd77b44c`; D 2548 | M; available; local_body_observed |
| (base) | `-cookie` | `{optional<arc::Cookie>=(?=c{Cookie={shared_ptr<arc::Cookie::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77b458`; D 2549 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77b464`; D 2550 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77b470`; D 2551 | M; available; local_body_observed |

## Remaining consequential questions

Establish expiration unit and epoch, cookie flag defaults, SameSite enum, partition keys and whether wrapper mutations ever persist automatically.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
