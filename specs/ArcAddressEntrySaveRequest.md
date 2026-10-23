# ArcAddressEntrySaveRequest

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37e418`, class_ro `0xe2ae778`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Save/cancel handle for an address-entry offer, backed by an optional shared native request.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_address_entry_save_request` | 0x8 | 24 | `{optional<arc::AddressEntrySaveRequest>=""(?="__null_state_"c"__val_"{AddressEntrySaveRequest="impl_"{shared_ptr<arc::AddressEntrySaveRequest::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ae740` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `address_entry_save_request`: `T{optional<arc::AddressEntrySaveRequest>=(?=c{AddressEntrySaveRequest={shared_ptr<arc::AddressEntrySaveRequest::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_address_entry_save_request` at `0xe2ae768`.

## Lifecycle, ownership, callbacks and threading

Initializer 0x443ebe4 copies the shared peer; 0x43df69c decrements it on destruction. save 0x443ed08 and cancel 0x443ed1c trap on an empty wrapper, then call 0x4330cdc / 0x4330d38. Those helpers send decision integers 2 / 3 to 0x7c6a394 and then call 0x7c6a4f4; the wrapper itself does not reset its optional. Do not infer one-shot behavior from the selector.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The 0x4330cdc / 0x4330d38 call chain is an autofill-decision integration gate. Exact Chromium callback enum and prompt-dismissal ownership remain unidentified; a CEF generic form-fill API is not demonstrated to reproduce it.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithAddressEntrySaveRequest: | `@24@0:8r^v16` | 0x443ebe4 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x443ebe4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443ebe4.c); record 0xd77cb84; dyld L3106 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -save | `v16@0:8` | 0x443ed08 | Requires engaged optional; calls 0x4330cdc, which supplies decision 2 to 0x7c6a394 and then 0x7c6a4f4; wrapper does not clear its optional. | 0x4330cdc | [0x443ed08](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443ed08.c); record 0xd77cb90; dyld L3107; evidence/bridges-embedding.txt | medium / reviewed_partial / available | Resolve decision enum, repeat-call safety and offer dismissal. |
| (base) | -cancel | `v16@0:8` | 0x443ed1c | Requires engaged optional; calls 0x4330d38, which supplies decision 3 to 0x7c6a394 and then 0x7c6a4f4; wrapper does not clear its optional. | 0x4330d38 | [0x443ed1c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443ed1c.c); record 0xd77cb9c; dyld L3108; evidence/bridges-embedding.txt | medium / reviewed_partial / available | Resolve decision enum, repeat-call safety and offer dismissal. |
| (base) | -address_entry_save_request | `{optional<arc::AddressEntrySaveRequest>=(?=c{AddressEntrySaveRequest={shared_ptr<arc::AddressEntrySaveRequest::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df53c.c); record 0xd77cba8; dyld L3109 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df69c.c); record 0xd77cbb4; dyld L3110 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df760.c); record 0xd77cbc0; dyld L3111 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Does native completion consume the offer, can save/cancel repeat safely, and who dismisses/releases the app prompt?


Original reviewed source: [ArcAddressEntrySaveRequest.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcAddressEntrySaveRequest.md).
