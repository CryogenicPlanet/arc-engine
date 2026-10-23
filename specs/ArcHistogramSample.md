# ArcHistogramSample

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37c1b8`, class_ro `0xe2a62a0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Snapshot of integer metric value plus three browser-state bits.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_browserIsVisible` | 0x8 | 1 | `B` | `0xe2a61d8` |
| `_browserInSession` | 0x9 | 1 | `B` | `0xe2a61f8` |
| `_browserIsPlayingAudio` | 0xa | 1 | `B` | `0xe2a6218` |
| `_value` | 0x10 | 8 | `@"NSNumber"` | `0xe2a6238` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `value`: `T@"NSNumber",R,N,V_value` at `0xe2a6260`.
- `browserIsVisible`: `TB,R,N,V_browserIsVisible` at `0xe2a6270`.
- `browserInSession`: `TB,R,N,V_browserInSession` at `0xe2a6280`.
- `browserIsPlayingAudio`: `TB,R,N,V_browserIsPlayingAudio` at `0xe2a6290`.

## Lifecycle, ownership, callbacks and threading

0x4402a3c boxes an int in NSNumber and unpacks bit 0 visible, bit 1 in-session, bit 2 playing-audio into +8/+9/+10. browserIsVisible 0x40b1230 has incoherent empty-return pseudocode; actual words 0x39402000, 0xd65f03c0 prove LDRB w0,[x0,#8]; RET. 0x425b6d8 clears/releases _value (NSNumber) at self+0x10 by passing the slot and nil to objc_storeStrong through 0x8c7df80. This destructor accepts no supplied replacement argument; its exact metadata type remains v16@0:8.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This class preserves capture-time browser-state flags; producer metric sampling and submission are not implemented by the getters. No CEF or Chromium metrics service identity is asserted.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithHistogramSample: | `@24@0:8r^{HistogramSample=i{BrowserState=b1b1b1b29}}16` | 0x4402a3c | Boxes signed sample int in NSNumber; stores browser-state bits 0/1/2 as visible/in-session/playing-audio. |  ; ObjC selectors numberWithInt: | [0x4402a3c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4402a3c.c); record 0xd778e58; dyld L1687 | medium / local_verified / available | Native sampling time and metric identity require producer context. |
| (base) | -value | `@16@0:8` | 0x2cb140 | Returns 64-bit field _value at self+0x10; no backend call. | No direct anonymous call recorded / indirect possible | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x2cb140.c); record 0xd778e64; dyld L1688 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -browserIsVisible | `B16@0:8` | 0x40b1230 | Returns byte field _browserIsVisible at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x40b1230](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x40b1230.c); record 0xd778e70; dyld L1689 | high / local_verified / available_incoherent_return_repaired_by_binary | No additional local question; runtime parity untested. |
| (base) | -browserInSession | `B16@0:8` | 0x43ecb04 | Returns byte field _browserInSession at self+0x9; no backend call. | No direct anonymous call recorded / indirect possible | [0x43ecb04](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43ecb04.c); record 0xd778e7c; dyld L1690 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -browserIsPlayingAudio | `B16@0:8` | 0x43ecb0c | Returns byte field _browserIsPlayingAudio at self+0xa; no backend call. | No direct anonymous call recorded / indirect possible | [0x43ecb0c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43ecb0c.c); record 0xd778e88; dyld L1691 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x425b6d8 | Clears/releases the stored NSNumber at self+0x10 through objc_storeStrong(slot, nil); accepts no caller-supplied replacement value. | objc_storeStrong via 0x8c7df80 -> 0xc4a6dc8 | [0x425b6d8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x425b6d8.c); record 0xd778e94; dyld L1692; [revision instruction checks](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/revision-evidence/finding-instructions.json); [body/import checks](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/revision-evidence/finding-checks.json) | high / local_verified / available | Effects of releasing the stored NSNumber and destruction-thread requirements outside this wrapper remain unverified. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.


Original reviewed source: [ArcHistogramSample.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcHistogramSample.md).
