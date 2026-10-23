# ArcExtensionCommand

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 120 bytes; class metadata `0xe37df68`, class_ro `0xe2ad960`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_extension_command` | 8 | 112 | `{ExtensionCommand="extension_id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{_… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline native strings, optional shortcut and Boolean global flag. `0x443baa8` and `0x443bc14` use native copy/assignment helper; `0x443bb24` preserves nil for absent shortcut; `0x41544d0` reads global flag. This class contains no command execution method. Initializer IMPs: `0x443baa8`. Cleanup IMP: `0x443bc20`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc value adapter to extension command service. Keyboard dispatch, global registration and permissions require separate service tracing; no CEF extension feature equivalence is implied.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithExtensionCommand:` | `@24@0:8r^v16` | [0x443baa8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443baa8.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x441e2e4 | M `0xd77c518`; D 2949 | M; available; local_body_observed |
| (base) | `-extensionId` | `@16@0:8` | [0x43e632c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e632c.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c524`; D 2950 | M; available; local_body_observed |
| (base) | `-commandName` | `@16@0:8` | [0x43e6350](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e6350.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c530`; D 2951 | M; available; local_body_observed |
| (base) | `-commandDescription` | `@16@0:8` | [0x443bb00](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bb00.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77c53c`; D 2952 | M; available; local_body_observed |
| (base) | `-shortcut` | `@16@0:8` | [0x443bb24](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bb24.c) | Returns nil for absent shortcut; converts optional native shortcut string otherwise. | 0x60a60; 0x1c391bc | M `0xd77c548`; D 2953 | M; available; bridge_reviewed_partial |
| (base) | `-isGlobal` | `B16@0:8` | [0x41544d0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/41544d0.c) | Loads 1 byte(s) from self+0x70 and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77c554`; D 2954 | H; available; local_field_observed |
| (base) | `-extension_command` | `{ExtensionCommand={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}B}16@0:8` | [0x443bc08](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bc08.c) | Copies inline ExtensionCommand at self+8 to native aggregate return storage via 0x441e200. | 0x441e200 | M `0xd77c560`; D 2955 | M; available; bridge_reviewed_partial |
| (base) | `-setExtension_command:` | `v128@0:8{ExtensionCommand={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}B}16` | [0x443bc14](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bc14.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | 0x441e2e4 | M `0xd77c56c`; D 2956 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x443bc20](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bc20.c) | Performs C++/ObjC cleanup of native storage; invokes 0x441e178. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | 0x441e178 | M `0xd77c578`; D 2957 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x443bc28](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443bc28.c) | Initializes native storage with explicit stores: self+0x68=0; self+0x10=0; self+8=0; self+0x20=0; self+0x18=0; self+0x30=0; self+0x28=0; self+0x40=0; self+0x38=0; self+0x49=0; self+0x41=0. | field/indirect dispatch; see body | M `0xd77c584`; D 2958 | M; available; local_body_observed |

## Remaining consequential questions

Recover shortcut syntax, command identifiers, global registration semantics and native copy-helper ownership.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcExtensionCommand.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcExtensionCommand.md).
