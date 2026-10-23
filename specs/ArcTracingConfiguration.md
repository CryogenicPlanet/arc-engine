# ArcTracingConfiguration

ArcCore 1.170.0 arm64. 11 declared rows; superclass `NSObject`. Class metadata `0xe37cac8`, class_ro `0xe2a8cf0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Value object containing file path, duration and privacy-filtering option.

## State and layout

Metadata instance start 8, size 64 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_filePath` | 0x8 | 8 | `@"NSString"` | `0xe2a8c48` |
| `_tracingDuration` | 0x10 | 8 | `d` | `0xe2a8c68` |
| `_tracingConfiguration` | 0x18 | 40 | `{TracingConfiguration="file_path"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"tracing_duration_in_seconds"d"privacy_filtering_enabled"B}` | `0xe2a8c88` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `tracingConfiguration`: `T{TracingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}dB},N,V_tracingConfiguration` at `0xe2a8cb0`.
- `filePath`: `T@"NSString",&,N,V_filePath` at `0xe2a8cc0`.
- `tracingDuration`: `Td,N,V_tracingDuration` at `0xe2a8cd0`.
- `privacyFilteringEnabled`: `TB,N` at `0xe2a8ce0`.

## Lifecycle, ownership, callbacks and threading

init 0x440c640 initializes empty native path, zero duration and privacy filtering enabled. setFilePath 0x440c764 updates both ObjC cache and native string; filePath 0x440c710 reconstructs the cache from native storage on read. duration getter/setter 0x440c800/0x440c80c similarly synchronize cache/native double; setTracingConfiguration 0x440c820 replaces native aggregate without updating those caches immediately. Destructor 0x440c8ac frees native string and clears cached path.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Configuration construction performs no tracing start/stop or file write in the examined bodies. The consumer (ArcTracingController, outside assignment) must map to Chromium/Perfetto policy and file lifecycle; no CEF tracing equivalence is implied.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -init | `@16@0:8` | 0x440c640 | Calls super init; initializes native path empty, duration 0 and privacyFilteringEnabled true. | 0x5810 | [0x440c640](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c640.c); record 0xd779e24; dyld L2056 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -tracingConfiguration | `{TracingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}dB}16@0:8` | 0x440c6b8 | Copies native path string by value plus duration/privacy flag into aggregate return storage. | 0x60a60 | [0x440c6b8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c6b8.c); record 0xd779e30; dyld L2057 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -filePath | `@16@0:8` | 0x440c710 | Converts native UTF-8 path into NSString and refreshes cached _filePath before returning. | 0x1c391bc | [0x440c710](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c710.c); record 0xd779e3c; dyld L2058 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -setFilePath: | `v24@0:8@16` | 0x440c764 | Stores ObjC path and converts it into native UTF-8 string; nil produces empty native string. | 0x1b356b8,0x5810 | [0x440c764](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c764.c); record 0xd779e48; dyld L2059 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -tracingDuration | `d16@0:8` | 0x440c800 | Copies native double into ObjC duration cache and returns it. | No direct anonymous call recorded / indirect possible | [0x440c800](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c800.c); record 0xd779e54; dyld L2060 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -setTracingDuration: | `v24@0:8d16` | 0x440c80c | Stores duration into both cached and native double fields. | No direct anonymous call recorded / indirect possible | [0x440c80c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c80c.c); record 0xd779e60; dyld L2061 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -privacyFilteringEnabled | `B16@0:8` | 0xed0b14 | Returns byte field inline/native storage +0x38 at self+0x38; no backend call. | No direct anonymous call recorded / indirect possible | [0xed0b14](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0xed0b14.c); record 0xd779e6c; dyld L2062 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setPrivacyFilteringEnabled: | `v20@0:8B16` | 0x440c818 | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x440c818](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c818.c); record 0xd779e78; dyld L2063 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setTracingConfiguration: | `v56@0:8{TracingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}dB}16` | 0x440c820 | Copies native path aggregate and duration/privacy fields; cached ObjC path/duration refresh on getters. | 0x359ad8,0x359998 | [0x440c820](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c820.c); record 0xd779e84; dyld L2064 | medium / local_verified / available | Validate aggregate ABI and consumer default semantics. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x440c8ac | Destroys backing storage; clears object/backing fields. ARC injection may hide retain/release operations. | 0x5810 | [0x440c8ac](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c8ac.c); record 0xd779e90; dyld L2065 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x440c8e8 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x440c8e8](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440c8e8.c); record 0xd779e9c; dyld L2066 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Are zero duration and empty path sentinel defaults in controller, and when are paths opened or privacy filtering enforced?


Original reviewed source: [ArcTracingConfiguration.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcTracingConfiguration.md).
