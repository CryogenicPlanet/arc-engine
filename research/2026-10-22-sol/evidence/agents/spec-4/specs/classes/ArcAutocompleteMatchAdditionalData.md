# ArcAutocompleteMatchAdditionalData

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 64 bytes; class metadata `0xe37dab8`, class_ro `0xe2ac8e0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_autocomplete_match_additional_data` | 8 | 56 | `{optional<arc::AutocompleteMatchAdditionalData>=""(?="__null_state_"c"__val_"{AutocompleteMatchAdditionalData="key_"{bas… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Owns an inline optional pair of native byte strings. `0x4438824` converts nil key/value to empty strings; `0x442d150` copies both strings to native return storage; `0x442d54c` returns receiver-backed mutable native storage only when engaged. Initializer IMPs: `0x4438824`, `0x4438a6c`. Cleanup IMP: `0x442d244`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc value adapter: preserve string conversion, nil/empty distinctions and C++ optional ABI. Backend autocomplete key meanings are separate; no CEF equivalence is established.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableAutocompleteMatchAdditionalData` | `^v16@0:8` | [0x442d54c](../bodies/442d54c.c) | Returns pointer to inline native value at self+8 when optional is engaged; disengaged path calls 0x4b08a00. | field/indirect dispatch; see body | M `0xd77be34`; D 2782 | H; available; local_field_observed |
| (base) | `-init:value:` | `@32@0:8@16@24` | [0x4438824](../bodies/4438824.c) | Converts key/value NSStrings to byte strings, with nil converted to empty, and copies them into inline optional. | 0x60a60; 0x1b356b8 | M `0xd77be40`; D 2783 | M; available; bridge_reviewed_partial |
| (base) | `-initWithAutocompleteMatchAdditionalData:` | `@24@0:8r^v16` | [0x4438a6c](../bodies/4438a6c.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x60a60 | M `0xd77be4c`; D 2784 | M; available; local_body_observed |
| (base) | `-key` | `@16@0:8` | [0x442d10c](../bodies/442d10c.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77be58`; D 2785 | M; available; local_body_observed |
| (base) | `-value` | `@16@0:8` | [0x442d7fc](../bodies/442d7fc.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77be64`; D 2786 | M; available; local_body_observed |
| (base) | `-autocomplete_match_additional_data` | `{optional<arc::AutocompleteMatchAdditionalData>=(?=c{AutocompleteMatchAdditionalData={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B}16@0:8` | [0x442d150](../bodies/442d150.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x60a60 | M `0xd77be70`; D 2787 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442d244](../bodies/442d244.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77be7c`; D 2788 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442d2a0](../bodies/442d2a0.c) | Initializes native storage with explicit stores: self+8=0; self+0x38=0. | field/indirect dispatch; see body | M `0xd77be88`; D 2789 | M; available; local_body_observed |

## Remaining consequential questions

Which additional-data keys are read by the app or ranking code, and may native mutable pointers outlive their wrapper?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
