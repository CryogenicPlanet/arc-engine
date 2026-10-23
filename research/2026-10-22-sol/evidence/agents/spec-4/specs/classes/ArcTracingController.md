# ArcTracingController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 13 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 56 bytes; class metadata `0xe37cb18`, class_ro `0xe2a8e20`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_isTracing` | 8 | 1 | `B` |
| `_bridge` | 16 | 16 | `{shared_ptr<(anonymous namespace)::TracingControllerDelegateBridge>="__ptr_"^{TracingControllerDelegateBridge}"__cntrl_"^{__shared_weak_count}}` |
| `_tracingController` | 32 | 24 | `{optional<arc::TracingController>=""(?="__null_state_"c"__val_"{TracingController="impl_"{shared_ptr<arc::TracingControl… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared controller at +0x20 and shared delegate bridge +0x10; `0x440c8fc` copies native handle, `0x440cb90` creates weak delegate/self references. Destructor `0x440ced8` releases both. isTracing `0x440cda8` locks Impl mutex +0xc0 and reads active-session +0xb8 before caching flag at self+8.

## Threading and caller/callee relations

Start `0x440ce18` → `0x434a700` configures systrace/categories/histogram list and schedules custom memory-pressure/system/swap/browser CPU tasks at nominal native delays 500000/1000000/5000000/500000. The Arc tracing_controller_impl.cc source markers identify those collection tasks. No time unit is inferred solely from constants. Stop `0x440cec4` → `0x434b768` differs from abort `0x440ceb0` → `0x434b818`; abort explicitly posts DidAbortTracing. Delegate adapters `0x440d248`, `0x440d29c`, `0x440d328` report start, completed text and abort. Getter mutex does not establish the callback queue or whole-object thread safety.

## Chrome / CEF route implications

Chrome tracing plus Arc metric collection/output contract must be reproduced. Native config names/histograms at `0x434a700` show more than a simple start/stop toggle. CEF tracing API equivalence needs checks for categories, data destination, custom metrics, errors and delegate event order; no final route decision is claimed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-init:` | `@32@0:8{TracingController={shared_ptr<arc::TracingController::Impl>=^{Impl}^{__shared_weak_count}}}16` | [0x440c8fc](../bodies/440c8fc.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd779eb0`; D 2069 | M; available; local_body_observed |
| (base) | `-isEqual:` | `B24@0:8@16` | [0x440ca20](../bodies/440ca20.c) | Checks receiver identity, nil and class compatibility; compares native optional engagement and underlying identity/value after extracting both handles. | field/indirect dispatch; see body | M `0xd779ebc`; D 2070 | M; available; local_body_observed |
| (base) | `-hash` | `Q16@0:8` | [0x43fcba0](../bodies/43fcba0.c) | Extracts native identity and hashes eight bytes using 0x18f3c; absent optional takes contract-failure path. | 0x18f3c | M `0xd779ec8`; D 2071 | M; available; local_body_observed |
| (base) | `-setDelegate:` | `v24@0:8@16` | [0x440cb90](../bodies/440cb90.c) | Drops shared bridge for nil; otherwise creates bridge with weak delegate/self or updates it and installs through 0x434221c. | 0x434221c | M `0xd779ed4`; D 2072 | M; available; bridge_reviewed_partial |
| (base) | `-isTracing` | `B16@0:8` | [0x440cda8](../bodies/440cda8.c) | Locks native mutex at Impl+0xc0, tests active session at Impl+0xb8, unlocks and caches Boolean at self+8. | 0x2b39c8 | M `0xd779ee0`; D 2073 | M; available; bridge_reviewed_partial |
| (base) | `-start:` | `B24@0:8@16` | [0x440ce18](../bodies/440ce18.c) | Requires engaged controller, obtains tracing configuration (nil gives zero/default temporary), calls 0x434a700 and returns its success register per ObjC metadata. | 0x434a700 | M `0xd779eec`; D 2074 | M; available; bridge_reviewed_partial |
| (base) | `-abort` | `B16@0:8` | [0x440ceb0](../bodies/440ceb0.c) | Requires engaged controller and calls abort bridge 0x434b818; native abort stops timers and posts DidAbortTracing delegate notification. | 0x434b818 | M `0xd779ef8`; D 2075 | M; available; bridge_reviewed_partial |
| (base) | `-stop` | `B16@0:8` | [0x440cec4](../bodies/440cec4.c) | Requires engaged controller and calls 0x434b768; returns false when no active native session, otherwise stops collection and begins finalization. | 0x434b768 | M `0xd779f04`; D 2076 | M; available; bridge_reviewed_partial |
| (base) | `-tracingController` | `{optional<arc::TracingController>=(?=c{TracingController={shared_ptr<arc::TracingController::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43fe184](../bodies/43fe184.c) | Copies optional native shared handle from self+0x20 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd779f10`; D 2077 | H; available; local_field_observed |
| (base) | `-bridge` | `{shared_ptr<(anonymous namespace)::TracingControllerDelegateBridge>=^{TracingControllerDelegateBridge}^{__shared_weak_count}}16@0:8` | [0x43fe224](../bodies/43fe224.c) | Returns copy of shared bridge/native handle and increments its control count; exact field offset is in the body and layout. | field/indirect dispatch; see body | M `0xd779f1c`; D 2078 | H; available; local_field_observed |
| (base) | `-setBridge:` | `v32@0:8{shared_ptr<(anonymous namespace)::TracingControllerDelegateBridge>=^{TracingControllerDelegateBridge}^{__shared_weak_count}}16` | [0x43fe244](../bodies/43fe244.c) | Assigns shared native bridge handle, incrementing new control count and releasing previous shared owner; identity is defined by metadata, despite shared Ghidra name. | field/indirect dispatch; see body | M `0xd779f28`; D 2079 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440ced8](../bodies/440ced8.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd779f34`; D 2080 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43fe528](../bodies/43fe528.c) | Initializes native storage with explicit stores: self+0x30=0; self+0x10=0; self+0x18=0; self+0x20=0. | field/indirect dispatch; see body | M `0xd779f40`; D 2081 | M; available; local_body_observed |

## Remaining consequential questions

Verify start Boolean return register (native Ghidra prototype is void), output artifact semantics, delegate callback queue, simultaneous start/stop/abort, timer units and cleanup after consumer disappearance.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
