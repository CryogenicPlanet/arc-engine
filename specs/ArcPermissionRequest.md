# ArcPermissionRequest

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37e558`, class_ro `0xe2aebe8`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Permission descriptor with native type/origin and two independently declared additionalData implementations.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_additionalData` | 0x8 | 8 | `@"NSDictionary"` | `0xe2aeb60` |
| `_permission_request` | 0x10 | 24 | `{optional<arc::PermissionRequest>=""(?="__null_state_"c"__val_"{PermissionRequest="impl_"{shared_ptr<arc::PermissionRequest::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2aeb80` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `permission_request`: `T{optional<arc::PermissionRequest>=(?=c{PermissionRequest={shared_ptr<arc::PermissionRequest::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_permission_request` at `0xe2aeba8`.
- `type`: `Tq,R,N` at `0xe2aebb8`.
- `originUrl`: `T@"NSString",R,C,N` at `0xe2aebc8`.
- `additionalData`: `T@"NSDictionary",R,N,V_additionalData` at `0xe2aebd8`.

## Lifecycle, ownership, callbacks and threading

0x443f8d4 copies shared peer into optional +0x10. type 0x443f9f8 loads native int +0x18; originUrl shares UTF-8 converter IMP 0x4416b90. Base additionalData 0x2d0fb40 returns self+8. Category Extensions additionalData 0x440a85c ignores that field, traverses native optional map at peer+0x30 when flag +0x48 is set and returns a copied dictionary with NSNumber keys and NSString values. Both identities must be retained; category runtime selection requires Objective-C load behavior validation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The permission type vocabulary and extra-data schema are Arc-specific. Generic Chromium/CEF permission callbacks cannot be assumed to supply the same dictionary keys, request identity or prompt ownership. This class declares no grant/deny method; resolution is elsewhere.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithPermissionRequest: | `@24@0:8r^v16` | 0x443f8d4 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x443f8d4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443f8d4.c); record 0xd77cd60; dyld L3151 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -type | `q16@0:8` | 0x443f9f8 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x443f9f8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x443f9f8.c); record 0xd77cd6c; dyld L3152 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -originUrl | `@16@0:8` | 0x4416b90 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x4416b90](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4416b90.c); record 0xd77cd78; dyld L3153 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -permission_request | `{optional<arc::PermissionRequest>=(?=c{PermissionRequest={shared_ptr<arc::PermissionRequest::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x440a2ac | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x440a2ac](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440a2ac.c); record 0xd77cd84; dyld L3154 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -additionalData | `@16@0:8` | 0x2d0fb40 | Base implementation returns cached NSDictionary field at self+8; no native map conversion. | No direct anonymous call recorded / indirect possible | [0x2d0fb40](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x2d0fb40.c); record 0xd77cd90; dyld L3155 | medium / local_verified / available | Runtime effective category implementation must be validated. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x440a40c | Destroys backing storage with shared-count decrement and last-owner virtual destruction; clears object/backing fields. ARC injection may hide retain/release operations. | 0x5810 | [0x440a40c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440a40c.c); record 0xd77cd9c; dyld L3156 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x440a4dc | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x440a4dc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440a4dc.c); record 0xd77cda8; dyld L3157 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |
| Extensions | -additionalData | `@16@0:8` | 0x440a85c | Creates mutable dictionary; for engaged native request converts optional map entries to NSNumber unsigned-integer keys and UTF-8 NSString values; returns dictionary copy. Empty request traps. | 0x5810,0x1c391bc ; ObjC selectors numberWithUnsignedInteger:,setObject:forKey: | [0x440a85c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440a85c.c); record 0xd779a0c; dyld L4973 | medium / local_verified / available | Recover numeric map-key schema and runtime override precedence. |

## Remaining implementation questions

Recover permission-type and map-key enums, request cancellation, prompt decision consumer and category load/override behavior.


Original reviewed source: [ArcPermissionRequest.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcPermissionRequest.md).
