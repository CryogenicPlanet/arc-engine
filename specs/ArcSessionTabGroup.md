# ArcSessionTabGroup

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 9 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c938`, class_ro `0xe2a8308`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_nativeSessionTabGroup` | 8 | 24 | `{optional<arc::SessionTabGroup>=""(?="__null_state_"c"__val_"{SessionTabGroup="impl_"{shared_ptr<arc::SessionTabGroup::I… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native group record; `0x440ad10` retains native handle. `0x440ae34` converts group ID string, title getter aliases cookie-value string copy `0x440ae60`, color `0x440af20` reads int +0x30 and collapsed `0x440af38` reads byte +0x34. Initializer IMPs: `0x440ad10`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Source-backed candidate structure Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:123`/:133/:135 associates ID and visual data. Preserve serialized group identity and public color enum; this wrapper does not restore tabs. CEF restoration needs separate session data reconstruction.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithSessionTabGroup:` | `@32@0:8{SessionTabGroup={shared_ptr<arc::SessionTabGroup::Impl>=^{Impl}^{__shared_weak_count}}}16` | [0x440ad10](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440ad10.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd779ad8`; D 1979 | M; available; local_body_observed |
| (base) | `-groupId` | `@16@0:8` | [0x440ae34](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440ae34.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd779ae4`; D 1980 | M; available; local_body_observed |
| (base) | `-title` | `@16@0:8` | [0x440ae60](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440ae60.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd779af0`; D 1981 | M; available; local_body_observed |
| (base) | `-color` | `q16@0:8` | [0x440af20](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440af20.c) | Reads native field and returns expression (long)*(int *)(*(long *)(param_1 + 8) + 0x30). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd779afc`; D 1982 | H; available; local_field_observed |
| (base) | `-isCollapsed` | `B16@0:8` | [0x440af38](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440af38.c) | Reads native field and returns expression (bool)*(byte *)(*(long *)(param_1 + 8) + 0x34). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd779b08`; D 1983 | H; available; local_field_observed |
| (base) | `-nativeSessionTabGroup` | `{optional<arc::SessionTabGroup>=(?=c{SessionTabGroup={shared_ptr<arc::SessionTabGroup::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd779b14`; D 1984 | H; available; local_field_observed |
| (base) | `-setNativeSessionTabGroup:` | `v40@0:8{optional<arc::SessionTabGroup>=(?=c{SessionTabGroup={shared_ptr<arc::SessionTabGroup::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43df57c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df57c.c) | Assigns optional shared native handle at self+8, with control-count retention/release and explicit engagement transition at self+0x18. | 0x4331e20 | M `0xd779b20`; D 1985 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd779b2c`; D 1986 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd779b38`; D 1987 | M; available; local_body_observed |

## Remaining consequential questions

Map color/group-ID serialization, optional ownership and group order; identify snapshot versus live mutation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcSessionTabGroup.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcSessionTabGroup.md).
