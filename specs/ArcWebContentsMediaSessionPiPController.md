# ArcWebContentsMediaSessionPiPController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37ce38`, class_ro `0xe2a9a30`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_nativePiPController` | 8 | 24 | `{optional<arc::WebContents::MediaSession::PiPController>=""(?="__null_state_"c"__val_"{PiPController="impl_"{shared_ptr<… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Initializer `0x441c1f8` copies native shared optional at +8; generic destructor `0x43df69c` decrements its control block. videoView `0x441c31c` returns nil for disengaged/absent native object, otherwise returns view through native pointer chain. Returned-view retain/reparent ownership is not inferred from the getter.

## Threading and caller/callee relations

sourceElementBounds `0x441c37c` requires engaged wrapper, reads native integer x/y/w/h, converts to CGRect doubles, and gives zeros if native object is absent. No scheduling, coordinate conversion or scale adjustment is shown. The view and geometry depend on nested native controller state; thread affinity and invalidation remain unresolved.

## Chrome / CEF route implications

A route gate for native macOS embedding: replacement must provide usable native video view plus matching geometry and lifecycle. This wrapper does not prove that CEF child-view hosting or CEF PiP offers that exact contract; identify native view implementation and original-engine reparenting behavior first.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithPiPController:` | `@24@0:8r^v16` | [0x441c1f8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/441c1f8.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77a844`; D 2260 | M; available; local_body_observed |
| (base) | `-videoView` | `@16@0:8` | [0x441c31c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/441c31c.c) | Returns nil for disengaged controller or absent native object; otherwise returns native view from pointer chain Impl+0x10 -> +0x38 -> +8 -> +0x10. | field/indirect dispatch; see body | M `0xd77a850`; D 2261 | M; available; bridge_reviewed_partial |
| (base) | `-sourceElementBounds` | `{CGRect={CGPoint=dd}{CGSize=dd}}16@0:8` | [0x441c37c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/441c37c.c) | Requires engaged controller; reads integer source rectangle at native object+0x28 -> +0x110/+0x118 and converts coordinates/dimensions to CGRect doubles; absent native object gives zeros. | field/indirect dispatch; see body | M `0xd77a85c`; D 2262 | M; available; bridge_reviewed_partial |
| (base) | `-nativePiPController` | `{optional<arc::WebContents::MediaSession::PiPController>=(?=c{PiPController={shared_ptr<arc::WebContents::MediaSession::PiPController::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77a868`; D 2263 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77a874`; D 2264 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77a880`; D 2265 | M; available; local_body_observed |

## Remaining consequential questions

Determine actual NSView subclass, layer/backing ownership, bounds coordinate space and scale, view migration during PiP, destroyed source behavior and required main-thread operations.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcWebContentsMediaSessionPiPController.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcWebContentsMediaSessionPiPController.md).
