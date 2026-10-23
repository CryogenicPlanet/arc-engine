# ArcExtensionRuntimeError

ArcCore 1.170.0 arm64. 11 declared rows; superclass `NSObject`. Class metadata `0xe37d478`, class_ro `0xe2aafc0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned extension runtime-error snapshot with lazily cached stack-frame wrappers.

## State and layout

Metadata instance start 8, size 152 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_stackTrace` | 0x8 | 8 | `@"NSArray"` | `0xe2aaef8` |
| `_extension_runtime_error` | 0x10 | 136 | `{optional<arc::ExtensionRuntimeError>=""(?="__null_state_"c"__val_"{ExtensionRuntimeError="extension_id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"context_url_spec"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"message"{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>="__rep_"(__rep="__s"{__short="__data_"[11S]"__padding_"{__padding<1UL>="__padding_"[1c]}"__size_"b7"__is_long_"b1}"__l"{__long="__data_"^S"__size_"Q"__cap_"b63"__is_long_"b1})}"source"{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>="__rep_"(__rep="__s"{__short="__data_"[11S]"__padding_"{__padding<1UL>="__padding_"[1c]}"__size_"b7"__is_long_"b1}"__l"{__long="__data_"^S"__size_"Q"__cap_"b63"__is_long_"b1})}"from_incognito"B"severity"i"stack_trace"{vector<arc::StackFrame, std::allocator<arc::StackFrame>>="__layout_"{__vector_layout<arc::StackFrame, std::allocator<arc::StackFrame>>="__begin_"^{StackFrame}"__size_"Q"__capacity_"Q}}})"__engaged_"B}` | `0xe2aaf18` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `extension_runtime_error`: `T{optional<arc::ExtensionRuntimeError>=(?=c{ExtensionRuntimeError={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}Bi{vector<arc::StackFrame, std::allocator<arc::StackFrame>>={__vector_layout<arc::StackFrame, std::allocator<arc::StackFrame>>=^{StackFrame}QQ}}})B},R,N,V_extension_runtime_error` at `0xe2aaf40`.
- `extensionID`: `T@"NSString",R,C,N` at `0xe2aaf50`.
- `contextURL`: `T@"NSURL",R,N` at `0xe2aaf60`.
- `message`: `T@"NSString",R,C,N` at `0xe2aaf70`.
- `source`: `T@"NSString",R,C,N` at `0xe2aaf80`.
- `fromIncognito`: `TB,R,N` at `0xe2aaf90`.
- `severity`: `Tq,R,N` at `0xe2aafa0`.
- `stackTrace`: `T@"NSArray",R,N,V_stackTrace` at `0xe2aafb0`.

## Lifecycle, ownership, callbacks and threading

0x442eeb4 copies native error via 0x4335698 and marks engagement. extensionID is UTF-8 (0x442ef28), message/source UTF-16 (0x442ef6c/0x442ef9c); contextURL uses 0x43abd34 (0x442ef58). stackTrace 0x442eff4 wraps native 0x38-byte stack entries in ArcExtensionRuntimeErrorStackFrame and caches array. severity 0x442efe0 is a numeric field; enum names are unresolved.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This record depends on extension error production and observer delivery outside the class. No native exception is raised by reading its message/stack; Chrome extension-service integration is the route gate.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithExtensionRuntimeError: | `@24@0:8r^v16` | 0x442eeb4 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x433587c,0x4335698 | [0x442eeb4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442eeb4.c); record 0xd77b0c8; dyld L2469 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -extensionID | `@16@0:8` | 0x442ef28 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x1c391bc | [0x442ef28](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442ef28.c); record 0xd77b0d4; dyld L2470 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -contextURL | `@16@0:8` | 0x442ef58 | Requires engaged error; converts native context URL string at self+0x28 through 0x43abd34. | 0x43abd34 | [0x442ef58](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442ef58.c); record 0xd77b0e0; dyld L2471 | medium / reviewed_partial / available | Invalid URL and native error producer semantics remain unresolved. |
| (base) | -message | `@16@0:8` | 0x442ef6c | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. Traps if outer optional is empty. | 0x1d9fb90 | [0x442ef6c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442ef6c.c); record 0xd77b0ec; dyld L2472 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -source | `@16@0:8` | 0x442ef9c | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. Traps if outer optional is empty. | 0x1d9fb90 | [0x442ef9c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442ef9c.c); record 0xd77b0f8; dyld L2473 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -fromIncognito | `B16@0:8` | 0x442efcc | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442efcc](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442efcc.c); record 0xd77b104; dyld L2474 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -severity | `q16@0:8` | 0x442efe0 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442efe0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442efe0.c); record 0xd77b110; dyld L2475 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -stackTrace | `@16@0:8` | 0x442eff4 | Requires engaged error; lazily creates/caches NSMutableArray of ArcExtensionRuntimeErrorStackFrame wrappers (native stride 0x38). |  ; ObjC selectors arrayWithCapacity:,initWithExtensionRuntimeErrorStackFrame:,addObject: | [0x442eff4](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442eff4.c); record 0xd77b11c; dyld L2476 | medium / local_verified / available | Frame ordering and producer lifetime remain unresolved. |
| (base) | -extension_runtime_error | `{optional<arc::ExtensionRuntimeError>=(?=c{ExtensionRuntimeError={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}Bi{vector<arc::StackFrame, std::allocator<arc::StackFrame>>={__vector_layout<arc::StackFrame, std::allocator<arc::StackFrame>>=^{StackFrame}QQ}}})B}16@0:8` | 0x442f0d0 | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x4335698 | [0x442f0d0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442f0d0.c); record 0xd77b128; dyld L2477 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x442f118 | Destroys backing storage; clears object/backing fields. ARC injection may hide retain/release operations. | 0x433587c | [0x442f118](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442f118.c); record 0xd77b134; dyld L2478 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x442f14c | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x442f14c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442f14c.c); record 0xd77b140; dyld L2479 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover severity enum, source/context URL semantics and delivery/retention of error notifications.


Original reviewed source: [ArcExtensionRuntimeError.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcExtensionRuntimeError.md).
