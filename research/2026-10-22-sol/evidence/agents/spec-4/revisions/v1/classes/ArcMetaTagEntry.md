# ArcMetaTagEntry

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 4 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 24 bytes; class metadata `0xe37b9e8`, class_ro `0xe2a3e10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_key` | 8 | 8 | `@"NSString"` |
| `_value` | 16 | 8 | `@"NSString"` |

## Lifecycle, ownership and callbacks

Simple key/value NSString holder. Initializer `0x43dd4e8` populates object fields and destructor `0x43dd5a4` clears them; getters `0x43dd450`/`0x420b5f8` are local slot access. Initializer IMPs: `0x43dd4e8`. Cleanup IMP: `0x43dd5a4`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc data adapter; DOM extraction and meta-tag matching are outside this container.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithKey:value:` | `@32@0:8@16@24` | [0x43dd4e8](../bodies/43dd4e8.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd7777d4`; D 1251 | M; available; local_body_observed |
| (base) | `-key` | `@16@0:8` | [0x43dd450](../bodies/43dd450.c) | Reads native field and returns expression *(ID *)(param_1 + 8). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd7777e0`; D 1252 | H; available; local_field_observed |
| (base) | `-value` | `@16@0:8` | [0x420b5f8](../bodies/420b5f8.c) | Reads native field and returns expression *(ID *)(param_1 + 0x10). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd7777ec`; D 1253 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43dd5a4](../bodies/43dd5a4.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd7777f8`; D 1254 | M; available; local_body_observed |

## Remaining consequential questions

Are key/value optional, copied or retained exactly, and does producer normalize key case? Follow constructor ARC helpers if mutable strings matter.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
