# ArcCreditCardInfoSaveRequest

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37e468`, class_ro `0xe2ae848`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_credit_card_info_save_request` | 8 | 24 | `{optional<arc::CreditCardInfoSaveRequest>=""(?="__null_state_"c"__val_"{CreditCardInfoSaveRequest="impl_"{shared_ptr<arc… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Initializer `0x443ed30` stores shared native optional; generic getter/destructor/constructor at `0x43df53c`/`0x43df69c`/`0x43df760` must be interpreted with this class metadata. Save `0x443ee54` → `0x433487c` accesses native controller and reports outcome 1; cancel `0x443ee68` reports outcome 2 after liveness checks.

## Threading and caller/callee relations

Native request guards trap for disengaged state; controller liveness and shared request lifetime are distinct. No callback or main-thread assertion is declared here. Callback consumption and repeat-call safety are not proven by `0x433487c`/`0x7c7dca0`.

## Chrome / CEF route implications

Chrome autofill/save-card integration candidate based on controller-call shape, not yet symbol-identified. Requires resolving persistence, local/upload save and callback ownership; a UI stub that only returns would miss native state transitions. CEF coverage is open.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithCreditCardInfoSaveRequest:` | `@24@0:8r^v16` | [0x443ed30](../bodies/443ed30.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77cbd4`; D 3114 | M; available; local_body_observed |
| (base) | `-save` | `v16@0:8` | [0x443ee54](../bodies/443ee54.c) | Requires engaged request and calls 0x433487c; backend acts on native save-card controller and completion outcome 1. | 0x433487c | M `0xd77cbe0`; D 3115 | M; available; bridge_reviewed_partial |
| (base) | `-cancel` | `v16@0:8` | [0x443ee68](../bodies/443ee68.c) | Requires engaged request; checks native controller liveness and forwards cancellation outcome 2 through 0x7c7dca0. | 0x805e0; 0x7c72f90; 0x7c7dca0 | M `0xd77cbec`; D 3116 | M; available; bridge_reviewed_partial |
| (base) | `-credit_card_info_save_request` | `{optional<arc::CreditCardInfoSaveRequest>=(?=c{CreditCardInfoSaveRequest={shared_ptr<arc::CreditCardInfoSaveRequest::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77cbf8`; D 3117 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77cc04`; D 3118 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77cc10`; D 3119 | M; available; local_body_observed |

## Remaining consequential questions

Pin enum outcome meaning and concrete controller class, then validate one-shot behavior, closed dialog handling and profile storage selection.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
