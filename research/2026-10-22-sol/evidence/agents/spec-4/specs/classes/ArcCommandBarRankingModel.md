# ArcCommandBarRankingModel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 48 bytes; class metadata `0xe37d6f8`, class_ro `0xe2aba10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_command_bar_ranking_model` | 8 | 40 | `{optional<arc::CommandBarRankingModel>=""(?="__null_state_"c"__val_"{CommandBarRankingModel="directory"{basic_string<cha… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline optional directory byte string and integer version; `0x4432940` populates descriptor, `0x442c454` copies it, `0x442c260` returns borrowed mutable storage. This class does not itself load or execute a ranking model. Initializer IMPs: `0x4432940`, `0x4432a5c`. Cleanup IMP: `0x43ed510`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc model-descriptor ABI; no browser engine dependency is established by this container. Loading/training integration belongs to its consumers.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableCommandBarRankingModel` | `^v16@0:8` | [0x442c260](../bodies/442c260.c) | Returns pointer to inline native value at self+8 when optional is engaged; disengaged path calls 0x4b08a00. | field/indirect dispatch; see body | M `0xd77b708`; D 2613 | H; available; local_field_observed |
| (base) | `-init:version:` | `@28@0:8@16i24` | [0x4432940](../bodies/4432940.c) | Stores converted directory byte string and integer version in inline optional model descriptor; does not load model files. | 0x1b356b8 | M `0xd77b714`; D 2614 | M; available; bridge_reviewed_partial |
| (base) | `-initWithCommandBarRankingModel:` | `@24@0:8r^v16` | [0x4432a5c](../bodies/4432a5c.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x60a60 | M `0xd77b720`; D 2615 | M; available; local_body_observed |
| (base) | `-directory` | `@16@0:8` | [0x43ed470](../bodies/43ed470.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77b72c`; D 2616 | M; available; local_body_observed |
| (base) | `-version` | `i16@0:8` | [0x442c440](../bodies/442c440.c) | Reads native field and returns expression *(int *)(param_1 + 0x20). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b738`; D 2617 | H; available; local_field_observed |
| (base) | `-command_bar_ranking_model` | `{optional<arc::CommandBarRankingModel>=(?=c{CommandBarRankingModel={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}i})B}16@0:8` | [0x442c454](../bodies/442c454.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x60a60 | M `0xd77b744`; D 2618 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43ed510](../bodies/43ed510.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77b750`; D 2619 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43ed538](../bodies/43ed538.c) | Initializes native storage with explicit stores: self+8=0; self+0x28=0. | field/indirect dispatch; see body | M `0xd77b75c`; D 2620 | M; available; local_body_observed |

## Remaining consequential questions

Model layout/version compatibility, filesystem ownership and whether directory must be absolute remain unresolved.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
