# ArcCpuInfo

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 29 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 112 bytes; class metadata `0xe37ca78`, class_ro `0xe2a8bb0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `armPartNumber_` | 8 | 1 | `C` |
| `isHypervisorActive_` | 9 | 1 | `C` |
| `hasBMI2_` | 10 | 1 | `C` |
| `hasMMX_` | 11 | 1 | `C` |
| `hasSSE_` | 12 | 1 | `C` |
| `hasSSE2_` | 13 | 1 | `C` |
| `hasSSE3_` | 14 | 1 | `C` |
| `hasSSSE3_` | 15 | 1 | `C` |
| `hasSSE41_` | 16 | 1 | `C` |
| `hasSSE42_` | 17 | 1 | `C` |
| `hasPOPCNT_` | 18 | 1 | `C` |
| `hasAVX_` | 19 | 1 | `C` |
| `hasAVX2_` | 20 | 1 | `C` |
| `hasFMA3_` | 21 | 1 | `C` |
| `hasAESNI_` | 22 | 1 | `C` |
| `hasMTE_` | 23 | 1 | `C` |
| `hasBTI_` | 24 | 1 | `C` |
| `brand_` | 32 | 8 | `@"NSString"` |
| `vendor_` | 40 | 8 | `@"NSString"` |
| `architecture_` | 48 | 8 | `@"NSString"` |
| `type_` | 56 | 8 | `q` |
| `family_` | 64 | 8 | `q` |
| `model_` | 72 | 8 | `q` |
| `stepping_` | 80 | 8 | `q` |
| `extModel_` | 88 | 8 | `q` |
| `extFamily_` | 96 | 8 | `q` |
| `armImplementer_` | 104 | 8 | `Q` |

## Lifecycle, ownership and callbacks

`0x440c34c` snapshots native CPU data and strings. On this arm64 slice many x86 feature flags are zeroed; armPartNumber and armImplementer are explicitly initialized to zero despite informative getter names. The shared armImplementer getter `0x40b1230` needs metadata type and raw LDRB interpretation. Initializer IMPs: `0x440c34c`. Cleanup IMP: `0x440c604`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Diagnostic snapshot can be implemented without browser services, but preserve observed arm64 representation rather than synthesizing hardware information from method names.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x440c34c](../bodies/440c34c.c) | Snapshots CPU brand/vendor via 0x4341e44/0x4341ef0, architecture through 0x4572478 and native CPU data through 0x83a7e78; many x86-feature bytes are set to zero on this arm64 image. | 0x1c391bc; 0x4341e44; 0x4341ef0; 0x4572478; 0x83a7e78 | M `0xd779cc0`; D 2025 | M; available; bridge_reviewed_partial |
| (base) | `-brand` | `@16@0:8` | [0x4060580](../bodies/4060580.c) | Loads 8 byte(s) from self+0x20 (brand_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779ccc`; D 2026 | H; available; local_field_observed |
| (base) | `-vendor` | `@16@0:8` | [0x133ae90](../bodies/133ae90.c) | Loads 8 byte(s) from self+0x28 (vendor_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779cd8`; D 2027 | H; available; local_field_observed |
| (base) | `-architecture` | `@16@0:8` | [0x2131e0](../bodies/2131e0.c) | Loads 8 byte(s) from self+0x30 (architecture_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779ce4`; D 2028 | H; available; local_field_observed |
| (base) | `-type` | `q16@0:8` | [0x327c740](../bodies/327c740.c) | Loads 8 byte(s) from self+0x38 (type_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779cf0`; D 2029 | H; available; local_field_observed |
| (base) | `-family` | `q16@0:8` | [0x3547bf0](../bodies/3547bf0.c) | Loads 8 byte(s) from self+0x40 (family_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779cfc`; D 2030 | H; available; local_field_observed |
| (base) | `-model` | `q16@0:8` | [0x4080f00](../bodies/4080f00.c) | Loads 8 byte(s) from self+0x48 (model_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d08`; D 2031 | H; available; local_field_observed |
| (base) | `-stepping` | `q16@0:8` | [0x4080f20](../bodies/4080f20.c) | Loads 8 byte(s) from self+0x50 (stepping_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d14`; D 2032 | H; available; local_field_observed |
| (base) | `-extModel` | `q16@0:8` | [0x4085370](../bodies/4085370.c) | Loads 8 byte(s) from self+0x58 (extModel_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d20`; D 2033 | H; available; local_field_observed |
| (base) | `-extFamily` | `q16@0:8` | [0x35f0300](../bodies/35f0300.c) | Loads 8 byte(s) from self+0x60 (extFamily_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d2c`; D 2034 | H; available; local_field_observed |
| (base) | `-armPartNumber` | `Q16@0:8` | [0xf90b90](../bodies/f90b90.c) | Loads 8 byte(s) from self+0x68 (armImplementer_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d38`; D 2035 | H; available; local_field_observed |
| (base) | `-armImplementer` | `C16@0:8` | [0x40b1230](../bodies/40b1230.c) | Loads 1 byte(s) from self+0x8 (armPartNumber_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d44`; D 2036 | H; available_pseudocode_incorrect_raw_verified; local_field_observed |
| (base) | `-isHypervisorActive` | `C16@0:8` | [0x43ecb04](../bodies/43ecb04.c) | Loads 1 byte(s) from self+0x9 (isHypervisorActive_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d50`; D 2037 | H; available; local_field_observed |
| (base) | `-hasBMI2` | `C16@0:8` | [0x43ecb0c](../bodies/43ecb0c.c) | Loads 1 byte(s) from self+0xa (hasBMI2_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d5c`; D 2038 | H; available; local_field_observed |
| (base) | `-hasMMX` | `C16@0:8` | [0x440c5a4](../bodies/440c5a4.c) | Loads 1 byte(s) from self+0xb (hasMMX_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d68`; D 2039 | H; available; local_field_observed |
| (base) | `-hasSSE` | `C16@0:8` | [0x440c5ac](../bodies/440c5ac.c) | Loads 1 byte(s) from self+0xc (hasSSE_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d74`; D 2040 | H; available; local_field_observed |
| (base) | `-hasSSE2` | `C16@0:8` | [0x440c5b4](../bodies/440c5b4.c) | Loads 1 byte(s) from self+0xd (hasSSE2_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d80`; D 2041 | H; available; local_field_observed |
| (base) | `-hasSSE3` | `C16@0:8` | [0x440c5bc](../bodies/440c5bc.c) | Loads 1 byte(s) from self+0xe (hasSSE3_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d8c`; D 2042 | H; available; local_field_observed |
| (base) | `-hasSSSE3` | `C16@0:8` | [0x440c5c4](../bodies/440c5c4.c) | Loads 1 byte(s) from self+0xf (hasSSSE3_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779d98`; D 2043 | H; available; local_field_observed |
| (base) | `-hasSSE41` | `C16@0:8` | [0x4073a00](../bodies/4073a00.c) | Loads 1 byte(s) from self+0x10 (hasSSE41_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779da4`; D 2044 | H; available; local_field_observed |
| (base) | `-hasSSE42` | `C16@0:8` | [0x440c5cc](../bodies/440c5cc.c) | Loads 1 byte(s) from self+0x11 (hasSSE42_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779db0`; D 2045 | H; available; local_field_observed |
| (base) | `-hasPOPCNT` | `C16@0:8` | [0x440c5d4](../bodies/440c5d4.c) | Loads 1 byte(s) from self+0x12 (hasPOPCNT_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779dbc`; D 2046 | H; available; local_field_observed |
| (base) | `-hasAVX` | `C16@0:8` | [0x440c5dc](../bodies/440c5dc.c) | Loads 1 byte(s) from self+0x13 (hasAVX_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779dc8`; D 2047 | H; available; local_field_observed |
| (base) | `-hasAVX2` | `C16@0:8` | [0x440c5e4](../bodies/440c5e4.c) | Loads 1 byte(s) from self+0x14 (hasAVX2_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779dd4`; D 2048 | H; available; local_field_observed |
| (base) | `-hasFMA3` | `C16@0:8` | [0x440c5ec](../bodies/440c5ec.c) | Loads 1 byte(s) from self+0x15 (hasFMA3_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779de0`; D 2049 | H; available; local_field_observed |
| (base) | `-hasAESNI` | `C16@0:8` | [0x440c5f4](../bodies/440c5f4.c) | Loads 1 byte(s) from self+0x16 (hasAESNI_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779dec`; D 2050 | H; available; local_field_observed |
| (base) | `-hasMTE` | `C16@0:8` | [0x440c5fc](../bodies/440c5fc.c) | Loads 1 byte(s) from self+0x17 (hasMTE_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779df8`; D 2051 | H; available; local_field_observed |
| (base) | `-hasBTI` | `C16@0:8` | [0x420c7a4](../bodies/420c7a4.c) | Loads 1 byte(s) from self+0x18 (hasBTI_) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd779e04`; D 2052 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440c604](../bodies/440c604.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd779e10`; D 2053 | M; available; local_body_observed |

## Remaining consequential questions

Verify actual hardware queries, supported ARM feature extraction, selector-to-ivar mismatch (armImplementer loads byte at +8/armPartNumber_, armPartNumber loads +0x68/armImplementer_) and stale snapshot expectations.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
