# ArcFindResult

ArcCore 1.170.0 arm64. 3 declared rows; superclass `NSObject`. Class metadata `0xe37cc08`, class_ro `0xe2a9180`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Snapshot result with signed 64-bit match count and active ordinal.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_numMatches` | 0x8 | 8 | `q` | `0xe2a9118` |
| `_activeMatchOrdinal` | 0x10 | 8 | `q` | `0xe2a9138` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `numMatches`: `Tq,R,N,V_numMatches` at `0xe2a9160`.
- `activeMatchOrdinal`: `Tq,R,N,V_activeMatchOrdinal` at `0xe2a9170`.

## Lifecycle, ownership, callbacks and threading

0x440e120 sign-extends two native int fields into self+8/+0x10; 0x2d0fb40 and 0x2cb140 return them. ArcWebContents find callback trampoline 0x444510c allocates this object and passes it to the captured block.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The class is a result adapter; find-request sequencing and cancellation belong to ArcWebContents 0x4442354 / native 0x43770c0. No independent backend lives in this record.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithFindResult: | `@24@0:8r^{FindResult=ii}16` | 0x440e120 | Calls super init; sign-extends native int numMatches/activeMatchOrdinal into two 64-bit fields. | No direct anonymous call recorded / indirect possible | [0x440e120](../bodies/0x440e120.c); record 0xd77a0fc; dyld L2122; evidence/bridges-callbacks.txt:0x444510c | medium / local_verified / available | Producer ordinal conventions remain unverified. |
| (base) | -numMatches | `q16@0:8` | 0x2d0fb40 | Returns 64-bit field _numMatches at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x2d0fb40](../bodies/0x2d0fb40.c); record 0xd77a108; dyld L2123 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -activeMatchOrdinal | `q16@0:8` | 0x2cb140 | Returns 64-bit field _activeMatchOrdinal at self+0x10; no backend call. | No direct anonymous call recorded / indirect possible | [0x2cb140](../bodies/0x2cb140.c); record 0xd77a114; dyld L2124 | high / local_verified / available | No additional local question; runtime parity untested. |

## Remaining implementation questions

Does the producer use a special ordinal for zero matches, and can multiple callbacks be delivered per request?
