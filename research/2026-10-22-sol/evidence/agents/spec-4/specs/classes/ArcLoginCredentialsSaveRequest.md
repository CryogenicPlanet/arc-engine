# ArcLoginCredentialsSaveRequest

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37e4b8`, class_ro `0xe2ae928`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_login_credentials_save_request` | 8 | 24 | `{optional<arc::LoginCredentialsSaveRequest>=""(?="__null_state_"c"__val_"{LoginCredentialsSaveRequest="impl_"{shared_ptr… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Initializer `0x443eefc` copies optional shared request handle. `0x443f020` reads native update flag +0x2c. save `0x443f038` calls `0x7d64988` only if manager exists, cancel `0x443f058` calls `0x7d641d0` only for updates, and neverAskAgain `0x443f080` calls `0x7d64b14` if manager exists. Shared request destructor is `0x43df69c`.

## Threading and caller/callee relations

The observable asymmetry is consequential: cancel for a new-save request has no native manager call in this IMP. Native manager methods update state and virtual callbacks (`0x7d64988`, `0x7d64b14`), but their precise identity and full persistence behavior are not established. No callback queue or general concurrency guarantee is recovered.

## Chrome / CEF route implications

Password-manager UI request bridge; storage selection, blocked-origin persistence and update behavior must integrate with chosen backend. Concrete manager naming remains a source-backed hypothesis until independently anchored; CEF public API equivalence has not been demonstrated.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithLoginCredentialsSaveRequest:` | `@24@0:8r^v16` | [0x443eefc](../bodies/443eefc.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77cc24`; D 3122 | M; available; local_body_observed |
| (base) | `-isUpdate` | `B16@0:8` | [0x443f020](../bodies/443f020.c) | Reads is-update byte from native request Impl+0x2c after optional check. | field/indirect dispatch; see body | M `0xd77cc30`; D 3123 | M; available; bridge_reviewed_partial |
| (base) | `-save` | `v16@0:8` | [0x443f038](../bodies/443f038.c) | Requires engaged request; if native manager at Impl+0x30 exists calls 0x7d64988. | 0x7d64988 | M `0xd77cc3c`; D 3124 | M; available; bridge_reviewed_partial |
| (base) | `-cancel` | `v16@0:8` | [0x443f058](../bodies/443f058.c) | Requires engaged request; calls 0x7d641d0 only when manager exists AND request is an update. | 0x7d641d0 | M `0xd77cc48`; D 3125 | M; available; bridge_reviewed_partial |
| (base) | `-neverAskAgain` | `v16@0:8` | [0x443f080](../bodies/443f080.c) | Requires engaged request; if manager exists calls 0x7d64b14. | 0x7d64b14 | M `0xd77cc54`; D 3126 | M; available; bridge_reviewed_partial |
| (base) | `-login_credentials_save_request` | `{optional<arc::LoginCredentialsSaveRequest>=(?=c{LoginCredentialsSaveRequest={shared_ptr<arc::LoginCredentialsSaveRequest::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77cc60`; D 3127 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77cc6c`; D 3128 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77cc78`; D 3129 | M; available; local_body_observed |

## Remaining consequential questions

Validate one-shot operations, manager disappearance, account versus profile storage, never-ask scope and new-save cancellation lifecycle.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
