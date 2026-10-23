# ArcFindTextWithDirectiveResult

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37e058`, class_ro `0xe2adc70`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_find_text_with_directive_result` | 8 | 32 | `{FindTextWithDirectiveResult="directive"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline native record with byte-string directive and Boolean result; `0x443bf24` copies native input, `0x443bfd4` returns native record and `0x443c024` assigns it. `0x43e632c` converts directive; shared scalar getter `0x4081f00` reads result using metadata identity. Initializer IMPs: `0x443bf24`. Cleanup IMP: `0x440d470`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Result adapter. Text-fragment search algorithm, renderer execution and directive escaping occur in producer methods outside this assignment.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithFindTextWithDirectiveResult:` | `@24@0:8r^v16` | [0x443bf24](../bodies/443bf24.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x359998; 0x359ad8 | M `0xd77c698`; D 2985 | M; available; local_body_observed |
| (base) | `-directive` | `@16@0:8` | [0x43e632c](../bodies/43e632c.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c6a4`; D 2986 | M; available; local_body_observed |
| (base) | `-result` | `B16@0:8` | [0x4081f00](../bodies/4081f00.c) | Loads 1 byte(s) from self+0x20 and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77c6b0`; D 2987 | H; available; local_field_observed |
| (base) | `-find_text_with_directive_result` | `{FindTextWithDirectiveResult={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}B}16@0:8` | [0x443bfd4](../bodies/443bfd4.c) | Copies inline byte-string directive and Boolean result into native aggregate return storage, deep-copying long string storage. | 0x60a60 | M `0xd77c6bc`; D 2988 | M; available; bridge_reviewed_partial |
| (base) | `-setFind_text_with_directive_result:` | `v48@0:8{FindTextWithDirectiveResult={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}B}16` | [0x443c024](../bodies/443c024.c) | Assigns inline FindTextWithDirectiveResult string (handling self-assignment and small/long string state), then copies Boolean result at self+0x20. | 0x359998; 0x359ad8 | M `0xd77c6c8`; D 2989 | M; available; bridge_reviewed_partial |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440d470](../bodies/440d470.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77c6d4`; D 2990 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x416a680](../bodies/416a680.c) | Initializes native storage with explicit stores: self+8=0; self+0x10=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77c6e0`; D 2991 | M; available; local_body_observed |

## Remaining consequential questions

What does false mean (no match, invalid directive or execution failure), and what encoding/escaping is used for directive?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
