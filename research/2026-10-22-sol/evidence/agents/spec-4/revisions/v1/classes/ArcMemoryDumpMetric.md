# ArcMemoryDumpMetric

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 5 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c5c8`, class_ro `0xe2a7670`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_name` | 8 | 8 | `@"NSString"` |
| `_units` | 16 | 8 | `@"NSString"` |
| `_value` | 24 | 8 | `Q` |

## Lifecycle, ownership and callbacks

Three scalar/object ivars; `0x44076e8` populates name, units and unsigned 64-bit value. Getters share IMPs with unrelated classes (`0x2d0fb40`, `0x2cb140`, `0x4084dc0`); raw load widths and this metadata preserve meaning. Destructor `0x43dd5a4` clears object fields. Initializer IMPs: `0x44076e8`. Cleanup IMP: `0x43dd5a4`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Diagnostic payload independent of Chromium scheduling; units/value interpretation belongs to dump producer.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithName:units:value:` | `@40@0:8@16@24Q32` | [0x44076e8](../bodies/44076e8.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd7795b4`; D 1860 | M; available; local_body_observed |
| (base) | `-name` | `@16@0:8` | [0x2d0fb40](../bodies/2d0fb40.c) | Loads 8 byte(s) from self+0x8 (_name) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd7795c0`; D 1861 | H; available; local_field_observed |
| (base) | `-units` | `@16@0:8` | [0x2cb140](../bodies/2cb140.c) | Loads 8 byte(s) from self+0x10 (_units) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd7795cc`; D 1862 | H; available; local_field_observed |
| (base) | `-value` | `Q16@0:8` | [0x4084dc0](../bodies/4084dc0.c) | Loads 8 byte(s) from self+0x18 (_value) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd7795d8`; D 1863 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43dd5a4](../bodies/43dd5a4.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd7795e4`; D 1864 | M; available; local_body_observed |

## Remaining consequential questions

Define accepted unit strings and whether values ever encode signed measurements or sentinel values.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
