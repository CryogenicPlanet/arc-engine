# ArcAdhocHistogramSample

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37c258`, class_ro `0xe2a6b30`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Mutable sample record carrying unsigned type and signed 32-bit value.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_value` | 0x8 | 4 | `i` | `0xe2a6ac8` |
| `_type` | 0x10 | 8 | `Q` | `0xe2a6ae8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `type`: `TQ,N,V_type` at `0xe2a6b10`.
- `value`: `Ti,N,V_value` at `0xe2a6b20`.

## Lifecycle, ownership, callbacks and threading

0x44034ac calls super init and stores type at +0x10 and value at +8. The independent accessors return/store those fields (0x2cb140, 0x416803c, 0x4081a30, 0x44034fc). No callback is introduced by these bodies.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This record can be represented by an adapter; the producer defines the histogram type vocabulary. Collection validation at ArcAdhocHistogramSamples 0x4403504 permits only 0..4.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithType:value: | `@28@0:8Q16i24` | 0x44034ac | Calls super init; stores supplied unsigned type at self+0x10 and int value at self+8. | No direct anonymous call recorded / indirect possible | [0x44034ac](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44034ac.c); record 0xd779084; dyld L1736 | medium / local_verified / available | Type vocabulary and validation belong to the producer. |
| (base) | -type | `Q16@0:8` | 0x2cb140 | Returns 64-bit field _type at self+0x10; no backend call. | No direct anonymous call recorded / indirect possible | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x2cb140.c); record 0xd779090; dyld L1737 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setType: | `v24@0:8Q16` | 0x416803c | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x416803c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x416803c.c); record 0xd77909c; dyld L1738 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -value | `i16@0:8` | 0x4081a30 | Returns 32-bit field _value at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x4081a30](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4081a30.c); record 0xd7790a8; dyld L1739 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setValue: | `v20@0:8i16` | 0x44034fc | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x44034fc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44034fc.c); record 0xd7790b4; dyld L1740 | high / local_verified / available | No additional local question; runtime parity untested. |

## Remaining implementation questions

Recover type names and metric units from the producer, without treating the local record as a metrics backend.


Original reviewed source: [ArcAdhocHistogramSample.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcAdhocHistogramSample.md).
