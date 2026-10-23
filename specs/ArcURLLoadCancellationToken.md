# ArcURLLoadCancellationToken

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37cb68`, class_ro `0xe2a8ef0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned std::string token container; it has no declared cancel method.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_token` | 0x8 | 24 | `{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}` | `0xe2a8eb8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `token`: `T{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})},N,V_token` at `0xe2a8ee0`.

## Lifecycle, ownership, callbacks and threading

0x440d37c copies input string using short/long string handling; token 0xef938c returns a value copy and setToken 0x440d41c replaces string storage. 0x440d470 frees heap storage for long strings and 0x416a680 initializes empty storage. None of these five bodies sends a cancellation operation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Cancellation routing requires the external consumer that indexes network requests by this token. CEF request cancellation is not equivalent until that identity/lifetime protocol is recovered.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithToken: | `@24@0:8r^v16` | 0x440d37c | Calls super init; copies native string into self+8 with short/long-string assignment handling. | 0x359ad8,0x359998 | [0x440d37c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440d37c.c); record 0xd779f54; dyld L2084 | medium / local_verified / available | Cancellation consumer and token identity are outside this class. |
| (base) | -token | `{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}16@0:8` | 0xef938c | Returns native string by value; heap-backed long strings are copied via 0x60a60. | 0x60a60 | [0xef938c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0xef938c.c); record 0xd779f60; dyld L2085 | medium / local_verified / available | Cancellation consumer and token identity are outside this class. |
| (base) | -setToken: | `v40@0:8{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}16` | 0x440d41c | Assigns native string at self+8 with self-assignment/short-string/long-string handling. | 0x359ad8,0x359998 | [0x440d41c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440d41c.c); record 0xd779f6c; dyld L2086 | medium / local_verified / available | Cancellation consumer and token identity are outside this class. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x440d470 | Destroys backing storage. ARC injection may hide retain/release operations. | 0x5810 | [0x440d470](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440d470.c); record 0xd779f78; dyld L2087 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x416a680 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x416a680](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x416a680.c); record 0xd779f84; dyld L2088 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Who generates the string, which service consumes it, and does destroying the token affect a request?


Original reviewed source: [ArcURLLoadCancellationToken.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcURLLoadCancellationToken.md).
