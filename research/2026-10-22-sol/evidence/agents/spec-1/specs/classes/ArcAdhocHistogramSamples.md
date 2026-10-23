# ArcAdhocHistogramSamples

ArcCore 1.170.0 arm64. 3 declared rows; superclass `NSObject`. Class metadata `0xe37c2a8`, class_ro `0xe2a6c00`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Collection adapter from native adhoc histogram samples into ObjC records.

## State and layout

Metadata instance start 8, size 16 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_samples` | 0x8 | 8 | `@"NSMutableArray"` | `0xe2a6bc8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `samples`: `T@"NSMutableArray",R,N,V_samples` at `0xe2a6bf0`.

## Lifecycle, ownership, callbacks and threading

0x4403504 allocates NSMutableArray and wraps each native 8-byte type/value pair in ArcAdhocHistogramSample; type >4 traps. samples 0x2d0fb40 returns the stored array, not a defensive copy; destructor 0x4166068 clears its object field (ARC operations are injected).

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

No Chrome/CEF service call is present in 0x4403504; compatibility concerns here are sample ordering, numeric widths and returned collection mutability. The metrics producer remains outside this class.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithAdhocHistogramSample: | `@24@0:8r^v16` | 0x4403504 | Copies native type/value pairs into a stored NSMutableArray of ArcAdhocHistogramSample; rejects type >4. |  ; ObjC selectors setType:,setValue:,addObject: | [0x4403504](../bodies/0x4403504.c); record 0xd7790c8; dyld L1743 | medium / local_verified / available | Native container length/layout and type names require producer audit. |
| (base) | -samples | `@16@0:8` | 0x2d0fb40 | Returns 64-bit field _samples at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x2d0fb40](../bodies/0x2d0fb40.c); record 0xd7790d4; dyld L1744 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4166068 | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x4166068](../bodies/0x4166068.c); record 0xd7790e0; dyld L1745 | high / local_verified / available | No additional local question; runtime parity untested. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
