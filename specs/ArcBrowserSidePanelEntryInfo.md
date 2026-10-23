# ArcBrowserSidePanelEntryInfo

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c0f0`, class_ro `0xe2a5e50`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_hasActiveEntry` | 8 | 1 | `B` |
| `_extensionId` | 16 | 8 | `@"NSString"` |
| `_cachedWebContents` | 24 | 8 | `@"ArcWebContents"` |

## Lifecycle, ownership and callbacks

Simple ObjC result record: active flag, extension-ID string and cached ArcWebContents. Setters use normal field/strong-store operations (`0x43e0930`, `0x420b588`, `0x4401164`), destructor clears object slots (`0x43e0940`). Producer `ArcBrowserSidePanel` `0x4401570` fills these fields. Initializer IMPs: . Cleanup IMP: `0x43e0940`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Result adapter for native panel queries; cachedWebContents identity and actual view ownership belong to native panel/controller behavior, not this data record.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-hasActiveEntry` | `B16@0:8` | [0x40b1230](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/40b1230.c) | Loads 1 byte(s) from self+0x8 (_hasActiveEntry) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778c94`; D 1653 | H; available_pseudocode_incorrect_raw_verified; local_field_observed |
| (base) | `-setHasActiveEntry:` | `v20@0:8B16` | [0x43e0930](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e0930.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd778ca0`; D 1654 | M; available; local_body_observed |
| (base) | `-extensionId` | `@16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/2cb140.c) | Loads 8 byte(s) from self+0x10 (_extensionId) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778cac`; D 1655 | H; available; local_field_observed |
| (base) | `-setExtensionId:` | `v24@0:8@16` | [0x420b588](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/420b588.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd778cb8`; D 1656 | M; available; local_body_observed |
| (base) | `-cachedWebContents` | `@16@0:8` | [0x4084dc0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4084dc0.c) | Loads 8 byte(s) from self+0x18 (_cachedWebContents) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd778cc4`; D 1657 | H; available; local_field_observed |
| (base) | `-setCachedWebContents:` | `v24@0:8@16` | [0x4401164](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4401164.c) | Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present. | field/indirect dispatch; see body | M `0xd778cd0`; D 1658 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43e0940](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43e0940.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd778cdc`; D 1659 | M; available; local_body_observed |

## Remaining consequential questions

Determine whether retaining this result may keep a cached WebContents alive after tab/panel closes.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcBrowserSidePanelEntryInfo.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcBrowserSidePanelEntryInfo.md).
