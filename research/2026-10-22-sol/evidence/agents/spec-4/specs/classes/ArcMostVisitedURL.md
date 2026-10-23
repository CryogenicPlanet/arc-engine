# ArcMostVisitedURL

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 64 bytes; class metadata `0xe37d248`, class_ro `0xe2aa790`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_most_visited_site` | 8 | 56 | `{optional<arc::MostVisitedURL>=""(?="__null_state_"c"__val_"{MostVisitedURL="url"{basic_string<char, std::char_traits<ch… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline optional URL byte string plus UTF-16 title. `0x442d974` deep-copies long strings, `0x442da54` converts URL representation and `0x442d10c` returns URL spec NSString; `0x442da68` converts title. Initializer IMPs: `0x442d974`. Cleanup IMP: `0x442db28`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Value adapter for history/top-sites outputs; algorithm and caching belong to native producer, not the wrapper.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithMostVisitedSite:` | `@24@0:8r^v16` | [0x442d974](../bodies/442d974.c) | Copies inline native URL byte string and title UTF-16 string into engaged optional; long strings are deep-copied. | 0x54420; 0x60a60 | M `0xd77adcc`; D 2396 | M; available; bridge_reviewed_partial |
| (base) | `-url` | `@16@0:8` | [0x442da54](../bodies/442da54.c) | Converts native URL byte string through 0x43abd34 into ObjC URL representation. | 0x43abd34 | M `0xd77add8`; D 2397 | M; available; bridge_reviewed_partial |
| (base) | `-urlSpec` | `@16@0:8` | [0x442d10c](../bodies/442d10c.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77ade4`; D 2398 | M; available; local_body_observed |
| (base) | `-title` | `@16@0:8` | [0x442da68](../bodies/442da68.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x1d9fb90 | M `0xd77adf0`; D 2399 | M; available; local_body_observed |
| (base) | `-most_visited_site` | `{optional<arc::MostVisitedURL>=(?=c{MostVisitedURL={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}})B}16@0:8` | [0x442da98](../bodies/442da98.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x54420; 0x60a60 | M `0xd77adfc`; D 2400 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442db28](../bodies/442db28.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77ae08`; D 2401 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442d2a0](../bodies/442d2a0.c) | Initializes native storage with explicit stores: self+8=0; self+0x38=0. | field/indirect dispatch; see body | M `0xd77ae14`; D 2402 | M; available; local_body_observed |

## Remaining consequential questions

URL canonicalization and invalid URL handling, title encoding errors and ranking/caching inputs remain unresolved.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
