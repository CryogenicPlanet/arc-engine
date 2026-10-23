# ArcBrowserApplicationConfiguration

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 11 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 56 bytes; class metadata `0xe37be98`, class_ro `0xe2a53f8`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_isProduction` | 8 | 1 | `B` |
| `_shouldLoadAllProfilesAtStartup` | 9 | 1 | `B` |
| `_quitRunLoopOnTermination` | 10 | 1 | `B` |
| `_argc` | 12 | 4 | `i` |
| `_argv` | 16 | 8 | `@"NSArray"` |
| `_profilePath` | 24 | 8 | `@"NSString"` |
| `_applicationID` | 32 | 8 | `Q` |
| `_sessionRestorationPolicy` | 40 | 8 | `Q` |
| `_userDefaultsSuiteName` | 48 | 8 | `@"NSString"` |

## Lifecycle, ownership and callbacks

`0x43eca0c` calls NSObject init and stores configuration fields, but its recovered stack-argument expressions are malformed. At entry-SP+0/+1 raw byte loads feed self+9/+10; entry-SP+8 supplies the pointer passed to the object-store call for self+0x30. This corroborates the final field stores while the C prototype remains unreliable ([instruction analysis](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/revision-evidence/analysis.md)); `0x43ecb14` clears ObjC string/array slots on destruction. This is configuration storage; no profile loading or run-loop termination occurs in these methods. Shared getter `0x40b1230` is a raw ARM64 byte load despite Ghidra dropping its result; exact bytes are in accessor-instruction-checks.tsv.

## Threading and caller/callee relations

The existing APP-layer evidence has a selector-stub call from Arc 1.168.0 `0x1004ad718` in function `0x1004ad314` (initial/app168-launch-calls.tsv and app168/ghidra/decomp/0x1004.c:128994), with receiver association based on old pseudocode. This is evidence of that call site, not blanket proof of runtime receiver types or all declared calls. No scheduling appears in engine constructor `0x43eca0c`.

## Chrome / CEF route implications

Startup configuration must be consumed by the application/browser-main integration. Preserving these fields alone is insufficient to reproduce startup, helper selection, profile loading or termination. Neither CEF nor Chrome route equivalence is established by the data object.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:` | `@72@0:8i16@20@28Q36B44Q48B56B60@64` | [0x43eca0c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43eca0c.c) | Initializes NSObject and stores configuration fields. Ghidra stack-argument recovery is malformed (second byte of a bool; pointer assembled from bool). Raw instructions load entry-SP bytes +0/+1 into self+9/+10, and entry-SP pointer +8 into the object-store call for self+0x30; this corroborates the final fields without validating the recovered C prototype. | field/indirect dispatch; see body | M `0xd778314`; D 1511 | M; available_prototype_malformed; bridge_reviewed_partial |
| (base) | `-argc` | `i16@0:8` | [0x189a538](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/189a538.c) | Loads 4 byte(s) from self+0xc (_argc) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778320`; D 1512 | H; available; local_field_observed |
| (base) | `-argv` | `@16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/2cb140.c) | Loads 8 byte(s) from self+0x10 (_argv) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77832c`; D 1513 | H; available; local_field_observed |
| (base) | `-profilePath` | `@16@0:8` | [0x4084dc0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4084dc0.c) | Loads 8 byte(s) from self+0x18 (_profilePath) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778338`; D 1514 | H; available; local_field_observed |
| (base) | `-applicationID` | `Q16@0:8` | [0x4060580](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4060580.c) | Loads 8 byte(s) from self+0x20 (_applicationID) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778344`; D 1515 | H; available; local_field_observed |
| (base) | `-isProduction` | `B16@0:8` | [0x40b1230](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/40b1230.c) | Loads 1 byte(s) from self+0x8 (_isProduction) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778350`; D 1516 | H; available_pseudocode_incorrect_raw_verified; local_field_observed |
| (base) | `-sessionRestorationPolicy` | `Q16@0:8` | [0x133ae90](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/133ae90.c) | Loads 8 byte(s) from self+0x28 (_sessionRestorationPolicy) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77835c`; D 1517 | H; available; local_field_observed |
| (base) | `-shouldLoadAllProfilesAtStartup` | `B16@0:8` | [0x43ecb04](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43ecb04.c) | Loads 1 byte(s) from self+0x9 (_shouldLoadAllProfilesAtStartup) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778368`; D 1518 | H; available; local_field_observed |
| (base) | `-quitRunLoopOnTermination` | `B16@0:8` | [0x43ecb0c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43ecb0c.c) | Loads 1 byte(s) from self+0xa (_quitRunLoopOnTermination) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778374`; D 1519 | H; available; local_field_observed |
| (base) | `-userDefaultsSuiteName` | `@16@0:8` | [0x2131e0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/2131e0.c) | Loads 8 byte(s) from self+0x30 (_userDefaultsSuiteName) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778380`; D 1520 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43ecb14](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43ecb14.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77838c`; D 1521 | M; available; local_body_observed |

## Remaining consequential questions

Trace every restoration-policy enum and startup field into application initialization; validate argc/argv consistency and suite-name nil behavior. Keep the malformed recovered prototype distinct from the declared metadata encoding and raw stack stores.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcBrowserApplicationConfiguration.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcBrowserApplicationConfiguration.md).
