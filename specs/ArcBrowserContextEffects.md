# ArcBrowserContextEffects

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 19 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37d978`, class_ro `0xe2ac3d0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_owner` | 8 | 8 | `@"ArcBrowserContext"` |
| `_browser_context_effects` | 16 | 24 | `{optional<arc::BrowserContext::Effects>=""(?="__null_state_"c"__val_"{Effects="impl_"{shared_ptr<arc::BrowserContext::Ef… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Service optional lives at +0x10; wrapper initialization is `0x4436384`/`0x44364a8`. Weak owner accessors `0x43fe200`/`0x43fe218` and destructor `_objc_destroyWeak` at `0x4435e2c` establish weak owner, independently of metadata object type. `0x44365f8` and `0x443684c` share register backend `0x42e1d80` with mode 0/1 for stylesheet/script. Filter registration copies 80 bytes (`0x4436aa0`).

## Threading and caller/callee relations

Backend `0x42e1d80` and scheme setter `0x42e3140` show `../../arc/browser/browser_context/effects_impl.cc` and NotifyBoostConfigChanged task setup. Zoom registration `0x4436dec` uses log(factor)/ln(1.2); removal `0x4436ec0` restores service default. This agrees with zoom conversion API Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `third_party/blink/public/common/page/page_zoom.h:27` as a source-backed inference. No whole-service thread contract or renderer acknowledgement was recovered.

## Chrome / CEF route implications

Arc maintains per-host configuration and notifies renderer-side effects. A CEF shim would need matching frame/world/timing, persistent host rules, matrix filtering, color-scheme and zoom behavior; public API coverage has not been verified here. Chrome route still needs the Arc effects IPC/configuration contract.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBrowserContextEffects:` | `@24@0:8r^v16` | [0x4436384](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436384.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77bbbc`; D 2724 | M; available; local_body_observed |
| (base) | `-initWithBrowserContextEffects:forBrowserContext:` | `@32@0:8r^v16@24` | [0x44364a8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44364a8.c) | Stores ObjC argument into weak slot shown in body; Ghidra injection renders it as a plain assignment. | field/indirect dispatch; see body | M `0xd77bbc8`; D 2725 | H; available; local_field_observed |
| (base) | `-registerStylesheet:withIdentifier:forHostPattern:` | `v40@0:8@16@24@32` | [0x44365f8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44365f8.c) | Converts identifier, host pattern and stylesheet text, then calls 0x42e1d80 with mode 0. Backend stores configuration and emits NotifyBoostConfigChanged work. | 0x1b356b8; 0x42e1d80 | M `0xd77bbd4`; D 2726 | M; available; bridge_reviewed_partial |
| (base) | `-unregisterStylesheet:` | `v24@0:8@16` | [0x4436794](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436794.c) | Converts identifier and calls 0x42e2240 with stylesheet mode 0. | 0x1b356b8; 0x42e2240 | M `0xd77bbe0`; D 2727 | M; available; bridge_reviewed_partial |
| (base) | `-registerScript:withIdentifier:forHostPattern:` | `v40@0:8@16@24@32` | [0x443684c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443684c.c) | Converts identifier, host pattern and script text, then calls same 0x42e1d80 with mode 1. | 0x1b356b8; 0x42e1d80 | M `0xd77bbec`; D 2728 | M; available; bridge_reviewed_partial |
| (base) | `-unregisterScript:` | `v24@0:8@16` | [0x44369e8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44369e8.c) | Converts identifier and calls 0x42e2240 with script mode 1. | 0x1b356b8; 0x42e2240 | M `0xd77bbf8`; D 2729 | M; available; bridge_reviewed_partial |
| (base) | `-registerColorFilter:forHostPattern:` | `v32@0:8[20f]16@24` | [0x4436aa0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436aa0.c) | Copies 20 floats (80 bytes) into native color-filter storage, converts host pattern and calls 0x43329b4. | 0x1b356b8; 0x43329b4 | M `0xd77bc04`; D 2730 | M; available; bridge_reviewed_partial |
| (base) | `-unregisterColorFilter:` | `v24@0:8@16` | [0x4436c20](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436c20.c) | Converts host pattern and calls 0x42e2da0 to unregister native color-filter entry. | 0x1b356b8; 0x42e2da0 | M `0xd77bc10`; D 2731 | M; available; bridge_reviewed_partial |
| (base) | `-registerColorScheme:forHostPattern:` | `v32@0:8q16@24` | [0x4436cb4](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436cb4.c) | Converts host pattern, forwards integer scheme to 0x42e3140; native config change notification is visible. | 0x1b356b8; 0x42e3140 | M `0xd77bc1c`; D 2732 | M; available; bridge_reviewed_partial |
| (base) | `-unregisterColorScheme:` | `v24@0:8@16` | [0x4436d58](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436d58.c) | Converts host pattern and forwards removal to 0x42e3468. | 0x1b356b8; 0x42e3468 | M `0xd77bc28`; D 2733 | M; available; bridge_reviewed_partial |
| (base) | `-registerZoomFactor:forHostPattern:` | `v28@0:8f16@20` | [0x4436dec](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436dec.c) | Obtains host zoom service and passes log(factor)/0.1823215567939546 to 0x64fee44 for the host pattern. | 0x119426c; 0x1b356b8; 0x4146794; 0x64fee44 | M `0xd77bc34`; D 2734 | M; available; bridge_reviewed_partial |
| (base) | `-unregisterZoomFactor:` | `v24@0:8@16` | [0x4436ec0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4436ec0.c) | Obtains host zoom service and sets host entry to native default level at service+0x70 via 0x64fee44. | 0xf71cc; 0x119426c; 0x1b356b8; 0x4146794; 0x64fee44 | M `0xd77bc40`; D 2735 | M; available; bridge_reviewed_partial |
| (base) | `-browser_context_effects` | `{optional<arc::BrowserContext::Effects>=(?=c{Effects={shared_ptr<arc::BrowserContext::Effects::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x440a2ac](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440a2ac.c) | Copies optional native shared handle from self+0x10 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77bc4c`; D 2736 | H; available; local_field_observed |
| (base) | `-owner` | `@16@0:8` | [0x43fe200](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43fe200.c) | Loads weak ObjC slot with loadWeakRetained/autorelease operations documented by Ghidra injection warnings. | field/indirect dispatch; see body | M `0xd77bc58`; D 2737 | H; available; local_field_observed |
| (base) | `-setOwner:` | `v24@0:8@16` | [0x43fe218](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43fe218.c) | Stores ObjC argument into weak slot shown in body; Ghidra injection renders it as a plain assignment. | field/indirect dispatch; see body | M `0xd77bc64`; D 2738 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4435e2c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4435e2c.c) | Performs C++/ObjC cleanup of native storage; destroys weak ivars at self+8. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77bc70`; D 2739 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440a4dc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/440a4dc.c) | Initializes native storage with explicit stores: self+0x10=0; self+0x20=0. | field/indirect dispatch; see body | M `0xd77bc7c`; D 2740 | M; available; local_body_observed |
| Extensions | `-isEqual:` | `B24@0:8@16` | [0x43f92f8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43f92f8.c) | Checks receiver identity, nil and class compatibility; compares native optional engagement and underlying identity/value after extracting both handles. | field/indirect dispatch; see body | M `0xd7786c4`; D 4929 | M; available; local_body_observed |
| Extensions | `-hash` | `Q16@0:8` | [0x43f95cc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/43f95cc.c) | Extracts native identity and hashes eight bytes using 0x18f3c; absent optional takes contract-failure path. | 0x18f3c | M `0xd7786d0`; D 4930 | M; available; local_body_observed |

## Remaining consequential questions

Recover host-pattern grammar and native map semantics, renderer injection timing/world, invalid zoom inputs, persistence and update acknowledgement. Do configuration updates affect existing documents immediately?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcBrowserContextEffects.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcBrowserContextEffects.md).
