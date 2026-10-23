# ArcMediaImage

ArcCore 1.170.0 arm64. 9 declared rows; superclass `NSObject`. Class metadata `0xe37c578`, class_ro `0xe2a7540`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native media image wrapper plus separately allocated CGSize buffer.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_sizes` | 0x8 | 8 | `^{CGSize=dd}` | `0xe2a74b8` |
| `_nativeMediaImage` | 0x10 | 16 | `{MediaImage="impl_"{shared_ptr<media_session::MediaImage>="__ptr_"^{MediaImage}"__cntrl_"^{__shared_weak_count}}}` | `0xe2a74d8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeMediaImage`: `T{MediaImage={shared_ptr<media_session::MediaImage>=^{MediaImage}^{__shared_weak_count}}},N,V_nativeMediaImage` at `0xe2a7500`.
- `src`: `T@"NSString",R,N` at `0xe2a7510`.
- `type`: `T@"NSString",R,N` at `0xe2a7520`.
- `sizes`: `T^{CGSize=dd},R,N,V_sizes` at `0xe2a7530`.

## Lifecycle, ownership, callbacks and threading

0x4407384 copies shared media image and allocates 16 bytes per native size, converting integer width/height to double. sizes 0x2d0fb40 returns the raw buffer pointer; this class declares no size-count method. dealloc 0x44074d0 frees that allocation before super dealloc. 0x4407624 releases shared peer storage. src 0x440751c returns UTF-8 text and type 0x44075d4 converts UTF-16 text.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Raw-pointer size enumeration and its lifetime are Arc ABI requirements. Native metadata identifies media_session::MediaImage; a CEF metadata/image adapter would still need an explicit count/buffer-ownership contract.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithMediaImage: | `@32@0:8{MediaImage={shared_ptr<media_session::MediaImage>=^{MediaImage}^{__shared_weak_count}}}16` | 0x4407384 | Copies native shared MediaImage; allocates CGSize buffer and converts each native integer width/height to doubles. | 0x4331e20,0x433cab0,0x8790,0x5810 | [0x4407384](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4407384.c); record 0xd779540; dyld L1849 | medium / local_verified / available | Recover size count from caller; validate allocation overflow/empty behavior. |
| (base) | -dealloc | `v16@0:8` | 0x44074d0 | Frees nonnull sizes allocation via 0x10ba4, then calls super dealloc. | 0x10ba4 | [0x44074d0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44074d0.c); record 0xd77954c; dyld L1850 | medium / local_verified / available | Do callers retain sizes pointer after object destruction? |
| (base) | -src | `@16@0:8` | 0x440751c | Reads native MediaImage URL through 0x7cf864, copies native bytes and converts UTF-8 string via 0x1c391bc. | 0x7cf864,0x60a60,0x1c391bc,0x5810 | [0x440751c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440751c.c); record 0xd779558; dyld L1851; evidence/bridges-conversions.txt | medium / local_verified / available | Invalid URL behavior and size-vector relationship need producer validation. |
| (base) | -type | `@16@0:8` | 0x44075d4 | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. | 0x1d9fb90 | [0x44075d4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44075d4.c); record 0xd779564; dyld L1852 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -sizes | `^{CGSize=dd}16@0:8` | 0x2d0fb40 | Returns 64-bit field _sizes at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x2d0fb40](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x2d0fb40.c); record 0xd779570; dyld L1853 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -nativeMediaImage | `{MediaImage={shared_ptr<media_session::MediaImage>=^{MediaImage}^{__shared_weak_count}}}16@0:8` | 0x44075f8 | Copies shared pointer/control pair from self+0x10; increments strong count. Ghidra return-register typing is inconsistent. | No direct anonymous call recorded / indirect possible | [0x44075f8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44075f8.c); record 0xd77957c; dyld L1854 | medium / local_verified / available | Validate aggregate return ABI. |
| (base) | -setNativeMediaImage: | `v32@0:8{MediaImage={shared_ptr<media_session::MediaImage>=^{MediaImage}^{__shared_weak_count}}}16` | 0x4407618 | Assigns native shared image pair; size-buffer update is not shown in this accessor. | 0x4331e20 | [0x4407618](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4407618.c); record 0xd779588; dyld L1855 | medium / reviewed_partial / available | Can mutation leave src/type and sizes inconsistent? |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4407624 | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x4407624](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x4407624.c); record 0xd779594; dyld L1856 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x44076e0 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x44076e0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x44076e0.c); record 0xd7795a0; dyld L1857 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

How does the app determine sizes count, and can setNativeMediaImage desynchronize the cached size buffer?


Original reviewed source: [ArcMediaImage.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcMediaImage.md).
