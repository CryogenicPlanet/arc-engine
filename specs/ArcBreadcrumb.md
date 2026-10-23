# ArcBreadcrumb

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 136 bytes; class metadata `0xe37dce8`, class_ro `0xe2ad1c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_breadcrumb` | 8 | 128 | `{Breadcrumb="category"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Stores inline category/message strings, unordered string map and optional file ID/line. `0x443ad84` materializes NSDictionary; `0x443ae68` and `0x443af4c` distinguish absent optional values from present strings/numbers. `0x443af90` forwards native record assignment. Initializer IMPs: `0x443ad2c`. Cleanup IMP: `0x8374b44`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc telemetry container. Conversion can be specified independently of browser embedding; its producer/crash reporter semantics need separate tracing.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBreadcrumb:` | `@24@0:8r^v16` | [0x443ad2c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443ad2c.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x441dedc | M `0xd77c22c`; D 2876 | M; available; local_body_observed |
| (base) | `-category` | `@16@0:8` | [0x43e632c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e632c.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c238`; D 2877 | M; available; local_body_observed |
| (base) | `-message` | `@16@0:8` | [0x43e6350](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e6350.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c244`; D 2878 | M; available; local_body_observed |
| (base) | `-data` | `@16@0:8` | [0x443ad84](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443ad84.c) | Converts native unordered string map to NSMutableDictionary with NSString keys/values. | 0x1c391bc | M `0xd77c250`; D 2879 | M; available; bridge_reviewed_partial |
| (base) | `-fileId` | `@16@0:8` | [0x443ae68](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443ae68.c) | Returns nil for absent optional file ID, otherwise converts copied native byte string to NSString. | 0x60a60; 0x1c391bc | M `0xd77c25c`; D 2880 | M; available; bridge_reviewed_partial |
| (base) | `-line` | `@16@0:8` | [0x443af4c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443af4c.c) | Returns nil for absent optional line, otherwise NSNumber from native signed int. | field/indirect dispatch; see body | M `0xd77c268`; D 2881 | M; available; bridge_reviewed_partial |
| (base) | `-breadcrumb` | `{Breadcrumb={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{unordered_map<std::string, std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, std::string>>>={__hash_table<std::__hash_value_type<std::string, std::string>, std::__unordered_map_hasher<std::string, std::pair<const std::string, std::string>, std::hash<std::string>, std::equal_to<std::string>>, std::__unordered_map_equal<std::string, std::pair<const std::string, std::string>, std::equal_to<std::string>, std::hash<std::string>>, std::allocator<std::pair<const std::string, std::string>>>={unique_ptr<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *[], std::__bucket_list_deallocator<std::allocator<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *>>>=^^v{__bucket_list_deallocator<std::allocator<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *>>=Q}}{__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *>=^v}Qf}}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<int>=(?=ci)B}}16@0:8` | [0x443af84](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443af84.c) | Copies inline Breadcrumb at self+8 to native aggregate return storage via 0x441de14. | 0x441de14 | M `0xd77c274`; D 2882 | M; available; bridge_reviewed_partial |
| (base) | `-setBreadcrumb:` | `v144@0:8{Breadcrumb={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{unordered_map<std::string, std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::pair<const std::string, std::string>>>={__hash_table<std::__hash_value_type<std::string, std::string>, std::__unordered_map_hasher<std::string, std::pair<const std::string, std::string>, std::hash<std::string>, std::equal_to<std::string>>, std::__unordered_map_equal<std::string, std::pair<const std::string, std::string>, std::equal_to<std::string>, std::hash<std::string>>, std::allocator<std::pair<const std::string, std::string>>>={unique_ptr<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *[], std::__bucket_list_deallocator<std::allocator<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *>>>=^^v{__bucket_list_deallocator<std::allocator<std::__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *> *>>=Q}}{__hash_node_base<std::__hash_node<std::__hash_value_type<std::string, std::string>, void *> *>=^v}Qf}}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<int>=(?=ci)B}}16` | [0x443af90](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443af90.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | 0x441dedc | M `0xd77c280`; D 2883 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x8374b44](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/8374b44.c) | Performs C++/ObjC cleanup of native storage; invokes 0x837274c. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | 0x837274c | M `0xd77c28c`; D 2884 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x443af9c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443af9c.c) | Initializes native storage with explicit stores: self+0x50=0; self+0x48=0; self+0x40=0; self+0x38=0; self+0x30=0; self+0x28=0; self+0x20=0; self+0x18=0; self+0x10=0; self+8=0; self+0x58=0x3f800000; self+0x60=0; self+0x78=0; self+0x80=0; self+0x84=0. | field/indirect dispatch; see body | M `0xd77c298`; D 2885 | M; available; local_body_observed |

## Remaining consequential questions

Are file IDs stable source identifiers, and do callbacks use zero- or one-based lines? No such meaning is inferred from field names.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcBreadcrumb.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcBreadcrumb.md).
