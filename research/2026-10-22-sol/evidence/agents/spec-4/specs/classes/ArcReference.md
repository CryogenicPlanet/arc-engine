# ArcReference

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 4 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 24 bytes; class metadata `0xe37b948`, class_ro `0xe2a3c10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_referenceText` | 8 | 8 | `@"NSString"` |
| `_minimumCharacterCount` | 16 | 8 | `q` |

## Lifecycle, ownership and callbacks

Stores referenceText NSString and signed 64-bit minimum-character count. `0x43dd3c0` initializes fields, `0x43dd450` returns text and shared raw getter `0x2cb140` returns count bits using this metadata ABI. Initializer IMPs: `0x43dd3c0`. Cleanup IMP: `0x4166068`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Small Arc data record independent of browser services. Matching/extraction algorithm using this reference is not implemented by the container.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithReferenceText:minimumCharacterCount:` | `@32@0:8@16q24` | [0x43dd3c0](../bodies/43dd3c0.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd777764`; D 1239 | M; available; local_body_observed |
| (base) | `-referenceText` | `@16@0:8` | [0x43dd450](../bodies/43dd450.c) | Reads native field and returns expression *(ID *)(param_1 + 8). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd777770`; D 1240 | H; available; local_field_observed |
| (base) | `-minimumCharacterCount` | `q16@0:8` | [0x2cb140](../bodies/2cb140.c) | Loads 8 byte(s) from self+0x10 (_minimumCharacterCount) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77777c`; D 1241 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4166068](../bodies/4166068.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd777788`; D 1242 | M; available; local_body_observed |

## Remaining consequential questions

Meaning of negative/zero count, Unicode character counting and text normalization must come from consumers.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
