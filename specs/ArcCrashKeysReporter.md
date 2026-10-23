# ArcCrashKeysReporter

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 1 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37dea0`, class_ro `0xe2ad660`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| No declared ivars | — | — | — |

## Lifecycle, ownership and callbacks

No instance ivars. Class method `0x443b578` lazily creates process-global reporter storage and installs an observer callback with explicit weak capture; nil installs an empty callback. Adapter `0x443b78c` converts native name/value and invokes onCrashKeyEmitted:value:.

## Threading and caller/callee relations

Global reporter setter is the only declared method. Weak capture construction is explicit; reporter producer sources, cross-thread synchronization and whether initial keys replay are unresolved (`0x443b578`, `0x443b78c`). No all-thread safety claim follows from the singleton guard.

## Chrome / CEF route implications

Arc crash-key observation bridge. It must remain compatible with emitted key/value data independently of backend crash reporter choice; this review does not establish Sentry semantics or CEF coverage.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `+setArcCrashKeysObserver:` | `v24@0:8@16` | [0x443b578](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443b578.c) | Creates global crash-key reporter lazily and installs weak observer callback; nil installs empty callback. Adapter 0x443b78c calls onCrashKeyEmitted:value: with two converted strings. | 0x45559f4 | M `0xd77c44c`; D 2928 | M; available; bridge_reviewed_partial |

## Remaining consequential questions

Find crash-key emission sources and observer replacement ordering; establish callback queue, replay and process lifetime.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcCrashKeysReporter.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcCrashKeysReporter.md).
