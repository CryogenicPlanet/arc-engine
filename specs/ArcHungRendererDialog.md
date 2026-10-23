# ArcHungRendererDialog

ArcCore 1.170.0 arm64. 7 declared rows; superclass `NSObject`. Class metadata `0xe37c348`, class_ro `0xe2a6e00`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Handle for renderer-unresponsive prompt actions.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_dialog` | 0x8 | 24 | `{optional<arc::HungRendererDialog>=""(?="__null_state_"c"__val_"{HungRendererDialog="impl_"{shared_ptr<arc::HungRendererDialog::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2a6dc8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `dialog`: `T{optional<arc::HungRendererDialog>=(?=c{HungRendererDialog={shared_ptr<arc::HungRendererDialog::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_dialog` at `0xe2a6df0`.

## Lifecycle, ownership, callbacks and threading

0x4403b80 stores an optional shared native dialog. crashProcess 0x4403ca4 calls 0x435f150, which obtains a process and forwards to 0x669e6e4 with integer 2. wait 0x4403cb8 calls 0x435f1ac, which invokes a stored refcounted callback if present. Its int-pointer self type makes +6 mean byte offset +0x18; the pseudo-C pid_t return conflicts with ObjC void metadata and is not adopted.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Chrome process termination and prompt lifecycle must be mapped explicitly; CEF unresponsive-renderer callbacks are only a possible adapter route, not demonstrated equivalence.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithHungRendererDialog: | `@24@0:8r^v16` | 0x4403b80 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x4403b80](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4403b80.c); record 0xd7791bc; dyld L1766 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -crashProcess | `v16@0:8` | 0x4403ca4 | Checks engaged dialog; 0x435f150 locates process and calls 0x669e6e4 with reason integer 2. | 0x435f150 | [0x4403ca4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4403ca4.c); record 0xd7791c8; dyld L1767; evidence/bridges-deep.txt | medium / reviewed_partial / available | Identify termination reason and prompt teardown. |
| (base) | -wait | `v16@0:8` | 0x4403cb8 | Checks optional engagement at byte +0x18 (self is mis-typed int* in pseudocode); 0x435f1ac invokes stored callback with refcount lifetime protection. | 0x435f1ac | [0x4403cb8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4403cb8.c); record 0xd7791d4; dyld L1768; evidence/bridges-deep.txt | medium / reviewed_partial / available | Callback purpose, repetition and thread remain unresolved; ObjC metadata return is void. |
| (base) | -dialog | `{optional<arc::HungRendererDialog>=(?=c{HungRendererDialog={shared_ptr<arc::HungRendererDialog::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df53c.c); record 0xd7791e0; dyld L1769 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -setDialog: | `v40@0:8{optional<arc::HungRendererDialog>=(?=c{HungRendererDialog={shared_ptr<arc::HungRendererDialog::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | 0x4403ccc | Assigns optional shared dialog; copies peer/control count or releases and clears existing peer. | 0x5810 | [0x4403ccc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4403ccc.c); record 0xd7791ec; dyld L1770 | medium / local_verified / available | Aggregate input ABI and dialog invalidation need runtime validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df69c.c); record 0xd7791f8; dyld L1771 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df760.c); record 0xd779204; dyld L1772 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

What process termination result and metric side effects occur, and does waiting dismiss or rearm the prompt?


Original reviewed source: [ArcHungRendererDialog.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcHungRendererDialog.md).
