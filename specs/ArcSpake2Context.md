# ArcSpake2Context

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 5 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 16 bytes; class metadata `0xe37ca28`, class_ro `0xe2a8600`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_ctx` | 8 | 8 | `{unique_ptr<arc::Spake2Context, std::default_delete<arc::Spake2Context>>="__ptr_"^{Spake2Context}}` |

## Lifecycle, ownership and callbacks

Unique native pointer at +8. `0x440bd1c` passes role!=0 and NSData names to `0x4341ab0` → `0x4461340`; generation `0x440be50` → `0x4341c4c` → `0x44614ac` uses 32-byte message buffer; process `0x440c0b8` → `0x4341d34` → `0x44617b4` uses 64-byte key buffer. Destructor `0x440c320` invokes `0x4341b9c` and frees native allocation.

## Threading and caller/callee relations

These wrappers return NSData or NSError on invalid context/native failure. Source-backed inference identifies BoringSSL SPAKE2: Chromium DEPS at Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `DEPS:360` pins BoringSSL `2ea3200c3a3ec176613ab7a0b9a4ffc6db846120`; fetched `specs/sources/boringssl/curve25519.h:157-215` specifies one exchange per context, one generation, then processing. Role normalization and matching function signatures/buffer constants corroborate inference; it is not a mechanical binary match. Calls have no scheduling visible; concurrency on one context is unverified.

## Chrome / CEF route implications

Cryptographic library adapter can be separate from Chromium/CEF navigation. Preserve pinned SPAKE2 protocol behavior and external key-confirmation usage. Source header :195-205 describes output as keying material for confirmation/HKDF; processing alone is not proof of authenticated peer confirmation.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithRole:myName:theirName:` | `@40@0:8q16@24@32` | [0x440bd1c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440bd1c.c) | Creates unique native SPAKE2 context using role!=0 and raw NSData names/lengths via 0x4341ab0; replaces old context with destructor cleanup. | 0x4341ab0; 0x4341b9c | M `0xd779c7c`; D 2018 | M; available; bridge_reviewed_partial |
| (base) | `-generateMessageWithPassword:error:` | `@32@0:8@16^@24` | [0x440be50](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440be50.c) | Checks native context, passes password NSData bytes to 0x4341c4c, returns NSData message or NSError; bridge uses a 32-byte output buffer. | 0x4341c4c | M `0xd779c88`; D 2019 | M; available; bridge_reviewed_partial |
| (base) | `-processMessage:error:` | `@32@0:8@16^@24` | [0x440c0b8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440c0b8.c) | Checks native context, passes peer NSData bytes to 0x4341d34, returns derived NSData or NSError; bridge has a 64-byte output buffer. | 0x4341d34 | M `0xd779c94`; D 2020 | M; available; bridge_reviewed_partial |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440c320](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440c320.c) | Clears unique context pointer, destroys native context using 0x4341b9c and frees wrapper allocation. | 0x4341b9c | M `0xd779ca0`; D 2021 | M; available; bridge_reviewed_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x407ab00](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/407ab00.c) | Initializes native storage with explicit stores: self+8=0. | field/indirect dispatch; see body | M `0xd779cac`; D 2022 | M; available; local_body_observed |

## Remaining consequential questions

Use interoperability vectors to verify role/name/password binding, wrong password/peer message, call ordering, repeated operations, NSError mapping and zeroization. Do not infer protocol security from wrapper signatures alone.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcSpake2Context.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcSpake2Context.md).
