# ArcSessionID

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 16 bytes; class metadata `0xe37c8e8`, class_ro `0xe2a81f8`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_sessionId` | 8 | 8 | `{optional<arc::SessionID>=""(?="__null_state_"c"__val_"{SessionID="id_"i})"__engaged_"B}` |

## Lifecycle, ownership and callbacks

Inline optional native signed 32-bit ID. Constructor `0x440ab88` narrows long-long to int; `0x440abb4` marks optional engaged. Native optional getter `0x2d0fb40` is an eight-byte raw load; this is aggregate bits, despite Ghidra naming an unrelated ObjC object getter. No destructor is declared.

## Threading and caller/callee relations

isEqual: `0x440ac14` compares type, engagement and native integer. identifier and hash share `0x440acd8`, sign-extending ID; `0x440acf4` assigns native optional by value. No scheduling is visible and no concurrent setter/getter guarantee is inferred.

## Chrome / CEF route implications

Native session ID adapter. Preserve 32-bit narrowing, optional ABI and hash/equality consistency. Source family Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:203` references SessionIDs in session data; no numeric correspondence with CEF browser IDs is assumed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithID:` | `@24@0:8q16` | [0x440ab88](../bodies/440ab88.c) | Narrows signed long-long identifier to native signed int and calls initWithSessionId:. | field/indirect dispatch; see body | M `0xd779a70`; D 1969 | M; available; bridge_reviewed_partial |
| (base) | `-initWithSessionId:` | `@24@0:8r^{SessionID=i}16` | [0x440abb4](../bodies/440abb4.c) | Initializes NSObject and copies native 32-bit ID into optional at self+8, marks engaged at self+0xc. | field/indirect dispatch; see body | M `0xd779a7c`; D 1970 | M; available; bridge_reviewed_partial |
| (base) | `-isEqual:` | `B24@0:8@16` | [0x440ac14](../bodies/440ac14.c) | Checks receiver identity, nil and class compatibility; compares native optional engagement and underlying identity/value after extracting both handles. | field/indirect dispatch; see body | M `0xd779a88`; D 1971 | M; available; local_body_observed |
| (base) | `-hash` | `Q16@0:8` | [0x440acd8](../bodies/440acd8.c) | For engaged ID, sign-extends native int; shared by identifier and hash. Disengaged state calls bad-optional helper 0x4b08a00. | field/indirect dispatch; see body | M `0xd779a94`; D 1972 | M; available; bridge_reviewed_partial |
| (base) | `-identifier` | `q16@0:8` | [0x440acd8](../bodies/440acd8.c) | For engaged ID, sign-extends native int; shared by identifier and hash. Disengaged state calls bad-optional helper 0x4b08a00. | field/indirect dispatch; see body | M `0xd779aa0`; D 1973 | M; available; bridge_reviewed_partial |
| (base) | `-sessionId` | `{optional<arc::SessionID>=(?=c{SessionID=i})B}16@0:8` | [0x2d0fb40](../bodies/2d0fb40.c) | Loads 8 byte(s) from self+0x8 (_sessionId) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779aac`; D 1974 | H; available; local_field_observed |
| (base) | `-setSessionId:` | `v24@0:8{optional<arc::SessionID>=(?=c{SessionID=i})B}16` | [0x440acf4](../bodies/440acf4.c) | Stores optional native 32-bit identifier and engagement flag from by-value optional argument. | field/indirect dispatch; see body | M `0xd779ab8`; D 1975 | M; available; bridge_reviewed_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440ad04](../bodies/440ad04.c) | Initializes native storage with explicit stores: self+8=0; self+0xc=0. | field/indirect dispatch; see body | M `0xd779ac4`; D 1976 | M; available; local_body_observed |

## Remaining consequential questions

Which ID values are sentinels, can ID be disengaged in app paths, and how are IDs mapped across restoration? Test negative/out-of-range values explicitly.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
