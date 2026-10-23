# ArcHSLAMatrix

ArcCore 1.170.0 arm64. 23 declared rows; superclass `NSObject`. Class metadata `0xe37bb28`, class_ro `0xe2a4388`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline 20-float matrix value; exact ivar and class_ro metadata anchors appear below.

## State and layout

Metadata instance start 8, size 88 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_hslaMatrix` | 0x8 | 80 | `{HSLAMatrix="m0"f"m1"f"m2"f"m3"f"m4"f"m5"f"m6"f"m7"f"m8"f"m9"f"m10"f"m11"f"m12"f"m13"f"m14"f"m15"f"m16"f"m17"f"m18"f"m19"f}` | `0xe2a4210` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `hslaMatrix`: `T{HSLAMatrix=ffffffffffffffffffff},N,V_hslaMatrix` at `0xe2a4238`.
- `m0`: `Tf,R,N` at `0xe2a4248`.
- `m1`: `Tf,R,N` at `0xe2a4258`.
- `m2`: `Tf,R,N` at `0xe2a4268`.
- `m3`: `Tf,R,N` at `0xe2a4278`.
- `m4`: `Tf,R,N` at `0xe2a4288`.
- `m5`: `Tf,R,N` at `0xe2a4298`.
- `m6`: `Tf,R,N` at `0xe2a42a8`.
- `m7`: `Tf,R,N` at `0xe2a42b8`.
- `m8`: `Tf,R,N` at `0xe2a42c8`.
- `m9`: `Tf,R,N` at `0xe2a42d8`.
- `m10`: `Tf,R,N` at `0xe2a42e8`.
- `m11`: `Tf,R,N` at `0xe2a42f8`.
- `m12`: `Tf,R,N` at `0xe2a4308`.
- `m13`: `Tf,R,N` at `0xe2a4318`.
- `m14`: `Tf,R,N` at `0xe2a4328`.
- `m15`: `Tf,R,N` at `0xe2a4338`.
- `m16`: `Tf,R,N` at `0xe2a4348`.
- `m17`: `Tf,R,N` at `0xe2a4358`.
- `m18`: `Tf,R,N` at `0xe2a4368`.
- `m19`: `Tf,R,N` at `0xe2a4378`.

## Lifecycle, ownership, callbacks and threading

0x43ded78 copies 80 bytes from the input matrix after a pointer/alignment check. m1..m19 load successive floats; m0 0x43dee04 has lost its return in pseudocode and is repaired using exact LDR S0,[X0,#8]; RET bytes in binary-checks.tsv. hslaMatrix 0x43dee9c copies 80 bytes unchanged into aggregate return storage X8, independently checked by SIMD load/store simulation (evidence/hsla-return-instructions.tsv); its pseudo-C numeric casts are artifacts. setHslaMatrix 0x43deec0 copies the aggregate fields.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

ArcWebContents applyColorFilter 0x44437c8 obtains the aggregate and forwards to native 0x4379218. Reproducing the object is straightforward; renderer-side matrix semantics and color space remain outside this local value review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithMatrix: | `@24@0:8[20f]16` | 0x43ded78 | Calls super init, checks input span/alignment through 0x122014, copies 80 bytes into inline matrix storage. | 0x122014 | [0x43ded78](../bodies/0x43ded78.c); record 0xd777944; dyld L1287 | medium / local_verified / available | Null/alignment contract and float matrix interpretation need caller validation. |
| (base) | -m0 | `f16@0:8` | 0x43dee04 | Returns float field _hslaMatrix at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee04](../bodies/0x43dee04.c); record 0xd777950; dyld L1288 | high / local_verified / available_incoherent_return_repaired_by_binary | No additional local question; runtime parity untested. |
| (base) | -m1 | `f16@0:8` | 0x43dee0c | Returns float field inline/native storage +0xc at self+0xc; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee0c](../bodies/0x43dee0c.c); record 0xd77795c; dyld L1289 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m2 | `f16@0:8` | 0xc7fdb4 | Returns float field inline/native storage +0x10 at self+0x10; no backend call. | No direct anonymous call recorded / indirect possible | [0xc7fdb4](../bodies/0xc7fdb4.c); record 0xd777968; dyld L1290 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m3 | `f16@0:8` | 0x43dee14 | Returns float field inline/native storage +0x14 at self+0x14; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee14](../bodies/0x43dee14.c); record 0xd777974; dyld L1291 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m4 | `f16@0:8` | 0x43dee1c | Returns float field inline/native storage +0x18 at self+0x18; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee1c](../bodies/0x43dee1c.c); record 0xd777980; dyld L1292 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m5 | `f16@0:8` | 0x43dee24 | Returns float field inline/native storage +0x1c at self+0x1c; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee24](../bodies/0x43dee24.c); record 0xd77798c; dyld L1293 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m6 | `f16@0:8` | 0x43dee2c | Returns float field inline/native storage +0x20 at self+0x20; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee2c](../bodies/0x43dee2c.c); record 0xd777998; dyld L1294 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m7 | `f16@0:8` | 0x43dee34 | Returns float field inline/native storage +0x24 at self+0x24; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee34](../bodies/0x43dee34.c); record 0xd7779a4; dyld L1295 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m8 | `f16@0:8` | 0x43dee3c | Returns float field inline/native storage +0x28 at self+0x28; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee3c](../bodies/0x43dee3c.c); record 0xd7779b0; dyld L1296 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m9 | `f16@0:8` | 0x43dee44 | Returns float field inline/native storage +0x2c at self+0x2c; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee44](../bodies/0x43dee44.c); record 0xd7779bc; dyld L1297 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m10 | `f16@0:8` | 0x43dee4c | Returns float field inline/native storage +0x30 at self+0x30; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee4c](../bodies/0x43dee4c.c); record 0xd7779c8; dyld L1298 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m11 | `f16@0:8` | 0x43dee54 | Returns float field inline/native storage +0x34 at self+0x34; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee54](../bodies/0x43dee54.c); record 0xd7779d4; dyld L1299 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m12 | `f16@0:8` | 0x43dee5c | Returns float field inline/native storage +0x38 at self+0x38; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee5c](../bodies/0x43dee5c.c); record 0xd7779e0; dyld L1300 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m13 | `f16@0:8` | 0x43dee64 | Returns float field inline/native storage +0x3c at self+0x3c; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee64](../bodies/0x43dee64.c); record 0xd7779ec; dyld L1301 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m14 | `f16@0:8` | 0x43dee6c | Returns float field inline/native storage +0x40 at self+0x40; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee6c](../bodies/0x43dee6c.c); record 0xd7779f8; dyld L1302 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m15 | `f16@0:8` | 0x43dee74 | Returns float field inline/native storage +0x44 at self+0x44; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee74](../bodies/0x43dee74.c); record 0xd777a04; dyld L1303 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m16 | `f16@0:8` | 0x43dee7c | Returns float field inline/native storage +0x48 at self+0x48; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee7c](../bodies/0x43dee7c.c); record 0xd777a10; dyld L1304 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m17 | `f16@0:8` | 0x43dee84 | Returns float field inline/native storage +0x4c at self+0x4c; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee84](../bodies/0x43dee84.c); record 0xd777a1c; dyld L1305 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m18 | `f16@0:8` | 0x43dee8c | Returns float field inline/native storage +0x50 at self+0x50; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee8c](../bodies/0x43dee8c.c); record 0xd777a28; dyld L1306 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -m19 | `f16@0:8` | 0x43dee94 | Returns float field inline/native storage +0x54 at self+0x54; no backend call. | No direct anonymous call recorded / indirect possible | [0x43dee94](../bodies/0x43dee94.c); record 0xd777a34; dyld L1307 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -hslaMatrix | `{HSLAMatrix=ffffffffffffffffffff}16@0:8` | 0x43dee9c | Copies 80 bytes unchanged from self+8 into aggregate return storage X8; bounded SIMD instruction simulation rejects Ghidra numeric casts. | No direct anonymous call recorded / indirect possible | [0x43dee9c](../bodies/0x43dee9c.c); record 0xd777a40; dyld L1308; evidence/hsla-return-instructions.tsv | high / local_verified / available_incoherent_casts_repaired_by_binary | Validate caller aggregate ABI across replacement toolchain. |
| (base) | -setHslaMatrix: | `v96@0:8{HSLAMatrix=ffffffffffffffffffff}16` | 0x43deec0 | Copies aggregate words into self+8 through self+0x57. | No direct anonymous call recorded / indirect possible | [0x43deec0](../bodies/0x43deec0.c); record 0xd777a4c; dyld L1309 | medium / local_verified / available | Validate indirect aggregate input ABI. |

## Remaining implementation questions

Validate aggregate calling convention from app/native callers and renderer matrix interpretation. The 80-byte getter is a verified bitwise copy; no numeric cast should be introduced.
