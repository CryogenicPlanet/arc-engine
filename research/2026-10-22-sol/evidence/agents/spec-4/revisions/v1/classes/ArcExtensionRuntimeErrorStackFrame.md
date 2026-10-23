# ArcExtensionRuntimeErrorStackFrame

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 72 bytes; class metadata `0xe37d428`, class_ro `0xe2aae60`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_extension_runtime_error_stack_frame` | 8 | 64 | `{optional<arc::StackFrame>=""(?="__null_state_"c"__val_"{StackFrame="line_number"I"column_number"I"source"{basic_string<… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline optional native StackFrame with two unsigned 32-bit numbers and two UTF-16 strings. `0x442ec50` copies data/strings, `0x442ed38`/`0x442ed4c` read numeric fields, `0x442ed60`/`0x442ed90` convert strings and `0x442edc0` returns a native copy. Initializer IMPs: `0x442ec50`. Cleanup IMP: `0x442ee58`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Data adapter for extension runtime error reporting; browser/extension execution is not performed by this class.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithExtensionRuntimeErrorStackFrame:` | `@24@0:8r^v16` | [0x442ec50](../bodies/442ec50.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x54420 | M `0xd77b060`; D 2459 | M; available; local_body_observed |
| (base) | `-lineNumber` | `I16@0:8` | [0x442ed38](../bodies/442ed38.c) | Reads native field and returns expression *(unsigned_int *)(param_1 + 8). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b06c`; D 2460 | H; available; local_field_observed |
| (base) | `-columnNumber` | `I16@0:8` | [0x442ed4c](../bodies/442ed4c.c) | Reads native field and returns expression *(unsigned_int *)(param_1 + 0xc). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b078`; D 2461 | H; available; local_field_observed |
| (base) | `-source` | `@16@0:8` | [0x442ed60](../bodies/442ed60.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x1d9fb90 | M `0xd77b084`; D 2462 | M; available; local_body_observed |
| (base) | `-function` | `@16@0:8` | [0x442ed90](../bodies/442ed90.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x1d9fb90 | M `0xd77b090`; D 2463 | M; available; local_body_observed |
| (base) | `-extension_runtime_error_stack_frame` | `{optional<arc::StackFrame>=(?=c{StackFrame=II{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}})B}16@0:8` | [0x442edc0](../bodies/442edc0.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x54420 | M `0xd77b09c`; D 2464 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442ee58](../bodies/442ee58.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77b0a8`; D 2465 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442cf84](../bodies/442cf84.c) | Initializes native storage with explicit stores: self+8=0; self+0x40=0. | field/indirect dispatch; see body | M `0xd77b0b4`; D 2466 | M; available; local_body_observed |

## Remaining consequential questions

Preserve unsigned widths and optional ABI; line/column origin and source URL normalization are not established.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
