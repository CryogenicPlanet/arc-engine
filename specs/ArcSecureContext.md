# ArcSecureContext

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37c898`, class_ro `0xe2a8118`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared security-state handle exposing a three-way Arc enum.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_context` | 0x8 | 24 | `{optional<arc::SecureContext>=""(?="__null_state_"c"__val_"{SecureContext="impl_"{shared_ptr<arc::SecureContext::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2a80d0` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `context`: `T{optional<arc::SecureContext>=(?=c{SecureContext={shared_ptr<arc::SecureContext::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_context` at `0xe2a80f8`.
- `state`: `TQ,R,N` at `0xe2a8108`.

## Lifecycle, ownership, callbacks and threading

0x440aa28 copies shared native context. state 0x440ab4c calls 0x3c2b700 and maps native 0→0, 3→1, every other result→2; empty optional traps. This is a lossy mapping, not the Chromium enum verbatim.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: native results are consistent with Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/security_state/core/security_state.h:39-70 (NONE=0, SECURE=3, DANGEROUS=5). 0x3c2b700 contains multiple security-state branches; exact GetSecurityLevel identity is not fully verified. A CEF security-state adapter must preserve Arc numeric mapping and inputs.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithSecureContext: | `@24@0:8r^v16` | 0x440aa28 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x440aa28](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440aa28.c); record 0xd779a20; dyld L1961 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -state | `Q16@0:8` | 0x440ab4c | Requires engaged context; calls 0x3c2b700 and maps result 0 to 0, 3 to 1, all other values to 2. | 0x3c2b700 | [0x440ab4c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x440ab4c.c); record 0xd779a2c; dyld L1962; evidence/bridges-conversions.txt | medium / local_verified / available | Identify native inputs and Arc enum names; mapping is lossy. |
| (base) | -context | `{optional<arc::SecureContext>=(?=c{SecureContext={shared_ptr<arc::SecureContext::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df53c.c); record 0xd779a38; dyld L1963 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -setContext: | `v40@0:8{optional<arc::SecureContext>=(?=c{SecureContext={shared_ptr<arc::SecureContext::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | 0x43df57c | Assigns optional shared peer: copy/replace when engaged, increments new control count, releases old control block; clears engagement for empty input. | 0x4331e20,0x5810 | [0x43df57c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df57c.c); record 0xd779a44; dyld L1964 | medium / local_verified / available | Validate aggregate input ABI and detached peer handling. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df69c.c); record 0xd779a50; dyld L1965 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df760.c); record 0xd779a5c; dyld L1966 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover Arc enum names and native security input construction; do not equate all non-secure values with a single browser warning policy.


Original reviewed source: [ArcSecureContext.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcSecureContext.md).
