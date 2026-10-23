# ArcImportSourceProfile

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 14 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c3e8`, class_ro `0xe2a7040`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_profile` | 8 | 24 | `{optional<arc::ImportSourceProfile>=""(?="__null_state_"c"__val_"{ImportSourceProfile="impl_"{shared_ptr<arc::ImportSour… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared import profile; `0x440403c` creates Safari-export source configuration, converting path and profile/display names. Type uses lookup `0x44045a8`; categories are masked to 0xfff (`0x4404770`); constraints returns native type==3 (`0x440484c`). `0x44044b0` captures completion for available-extension enumeration through `0x433ae6c`. Initializer IMPs: `0x4403f18`, `0x440403c`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Chrome/importer-service integration is indicated by native profile and import flow in ArcBrowserContext `0x43f3874`; source profile itself is data/configuration. CEF route needs independent importer support and data migration contracts.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithImportSourceProfile:` | `@24@0:8r^v16` | [0x4403f18](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4403f18.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd779244`; D 1780 | M; available; local_body_observed |
| (base) | `-initWithSafariExportedData:profileName:name:categories:` | `@48@0:8@16@24@32q40` | [0x440403c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440403c.c) | Creates native import-source profile from Safari export path, UTF-16 profile/display names and category mask narrowed to short; wraps shared allocation. | 0x8a7fc4; 0x1b356b8; 0x7040958; 0x7040a20; 0x83b1228 | M `0xd779250`; D 1781 | M; available; bridge_reviewed_partial |
| (base) | `-availableExtensions:` | `v24@0:8@?16` | [0x44044b0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44044b0.c) | Calls 0x433ae6c with captured completion to enumerate available extensions; result adapter 0x440486c handles conversion. | 0x433ae6c | M `0xd77925c`; D 1782 | M; available; bridge_reviewed_partial |
| (base) | `-name` | `@16@0:8` | [0x43df3bc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df3bc.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x54420; 0x1d9fb90 | M `0xd779268`; D 1783 | M; available; local_body_observed |
| (base) | `-type` | `q16@0:8` | [0x44045a8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44045a8.c) | Maps native source type minus one through table 0xc9d8168 with bounds check. | field/indirect dispatch; see body | M `0xd779274`; D 1784 | M; available; bridge_reviewed_partial |
| (base) | `-sourcePath` | `@16@0:8` | [0x44045f0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44045f0.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd779280`; D 1785 | M; available; local_body_observed |
| (base) | `-appPath` | `@16@0:8` | [0x44046b0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44046b0.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77928c`; D 1786 | M; available; local_body_observed |
| (base) | `-supportedCategories` | `q16@0:8` | [0x4404770](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4404770.c) | Returns native supported-category mask AND 0xfff. | field/indirect dispatch; see body | M `0xd779298`; D 1787 | M; available; bridge_reviewed_partial |
| (base) | `-profileName` | `@16@0:8` | [0x440478c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440478c.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x54420; 0x1d9fb90 | M `0xd7792a4`; D 1788 | M; available; local_body_observed |
| (base) | `-constraints` | `q16@0:8` | [0x440484c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440484c.c) | Returns Boolean-like public constraint value based solely on native source type == 3. | field/indirect dispatch; see body | M `0xd7792b0`; D 1789 | M; available; bridge_reviewed_partial |
| (base) | `-profile` | `{optional<arc::ImportSourceProfile>=(?=c{ImportSourceProfile={shared_ptr<arc::ImportSourceProfile::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd7792bc`; D 1790 | H; available; local_field_observed |
| (base) | `-setProfile:` | `v40@0:8{optional<arc::ImportSourceProfile>=(?=c{ImportSourceProfile={shared_ptr<arc::ImportSourceProfile::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43df57c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df57c.c) | Assigns optional shared native handle at self+8, with control-count retention/release and explicit engagement transition at self+0x18. | 0x4331e20 | M `0xd7792c8`; D 1791 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd7792d4`; D 1792 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd7792e0`; D 1793 | M; available; local_body_observed |

## Remaining consequential questions

Recover source type table, category bitset/constraints, Safari archive format, extension enumeration callback queue and exported-path validation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcImportSourceProfile.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcImportSourceProfile.md).
