# ArcSessionWindow

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 11 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c988`, class_ro `0xe2a8438`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_nativeSessionWindow` | 8 | 24 | `{optional<arc::SessionWindow>=""(?="__null_state_"c"__val_"{SessionWindow="impl_"{shared_ptr<arc::SessionWindow::Impl>="… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native session-window handle; `0x440af50` retains it. `0x440b0a8` and `0x440b464` wrap native tab/group collections; `0x440b36c` reads selected index. `0x440b388` converts epoch-microsecond time to NSDate, including zero/infinity sentinels. Type/show-state use mappings (`0x440b074`, `0x440b430`). Initializer IMPs: `0x440af50`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Source-backed structure candidate Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:176`/:203/:216/:223 supports fields, not a proved full layout identity. Replacement must preserve session restoration data, group identity, timestamp conversion and selected-tab semantics in either route.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithSessionWindow:` | `@32@0:8{SessionWindow={shared_ptr<arc::SessionWindow::Impl>=^{Impl}^{__shared_weak_count}}}16` | [0x440af50](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440af50.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd779b4c`; D 1990 | M; available; local_body_observed |
| (base) | `-windowType` | `q16@0:8` | [0x440b074](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b074.c) | Reads native session-window type and translates through explicit branch mapping. | field/indirect dispatch; see body | M `0xd779b58`; D 1991 | M; available; bridge_reviewed_partial |
| (base) | `-tabs` | `@16@0:8` | [0x440b0a8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b0a8.c) | Calls 0x4341268, wraps each native session tab as ArcSessionTab and returns mutable array. | 0x4341268 | M `0xd779b64`; D 1992 | M; available; bridge_reviewed_partial |
| (base) | `-selectedTabIndex` | `i16@0:8` | [0x440b36c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b36c.c) | Reads native selected-tab index at session-window+0x34. | field/indirect dispatch; see body | M `0xd779b70`; D 1993 | M; available; bridge_reviewed_partial |
| (base) | `-lastModified` | `@16@0:8` | [0x440b388](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b388.c) | Reads native time at session-window+0x40, handles sentinel values and converts Chromium time epoch to NSDate seconds since 1970. | field/indirect dispatch; see body | M `0xd779b7c`; D 1994 | M; available; bridge_reviewed_partial |
| (base) | `-showState` | `q16@0:8` | [0x440b430](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b430.c) | Maps native show-state integer through lookup table 0xc9d8180 with bounds branch. | field/indirect dispatch; see body | M `0xd779b88`; D 1995 | M; available; bridge_reviewed_partial |
| (base) | `-tabGroups` | `@16@0:8` | [0x440b464](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440b464.c) | Calls 0x43413d0 and wraps each native session-tab-group handle in an array. | 0x43413d0 | M `0xd779b94`; D 1996 | M; available; bridge_reviewed_partial |
| (base) | `-nativeSessionWindow` | `{optional<arc::SessionWindow>=(?=c{SessionWindow={shared_ptr<arc::SessionWindow::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd779ba0`; D 1997 | H; available; local_field_observed |
| (base) | `-setNativeSessionWindow:` | `v40@0:8{optional<arc::SessionWindow>=(?=c{SessionWindow={shared_ptr<arc::SessionWindow::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43df57c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df57c.c) | Assigns optional shared native handle at self+8, with control-count retention/release and explicit engagement transition at self+0x18. | 0x4331e20 | M `0xd779bac`; D 1998 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd779bb8`; D 1999 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd779bc4`; D 2000 | M; available; local_body_observed |

## Remaining consequential questions

Recover enum conversion tables, timestamp sentinel boundaries, tab/group order and whether native record updates are visible through existing wrappers.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcSessionWindow.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcSessionWindow.md).
