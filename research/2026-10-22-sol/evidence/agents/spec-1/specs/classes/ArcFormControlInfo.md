# ArcFormControlInfo

ArcCore 1.170.0 arm64. 12 declared rows; superclass `NSObject`. Class metadata `0xe37cfc8`, class_ro `0xe2a9f80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned extracted form-control snapshot with lazily cached select-option objects.

## State and layout

Metadata instance start 8, size 160 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_selectOptions` | 0x8 | 8 | `@"NSArray"` | `0xe2a9ea8` |
| `_form_control_info` | 0x10 | 144 | `{optional<arc::FormControlInfo>=""(?="__null_state_"c"__val_"{FormControlInfo="control_type"i"field_name"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}"field_value"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}"placeholder"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}"select_options"{vector<arc::SelectOption, std::allocator<arc::SelectOption>>="__layout_"{__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>="__begin_"^{SelectOption}"__size_"Q"__capacity_"Q}}"is_checked"B"is_required"B"is_redacted"B})"__engaged_"B}` | `0xe2a9ec8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `form_control_info`: `T{optional<arc::FormControlInfo>=(?=c{FormControlInfo=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{vector<arc::SelectOption, std::allocator<arc::SelectOption>>={__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>=^{SelectOption}QQ}}BBB})B},R,N,V_form_control_info` at `0xe2a9ef0`.
- `controlType`: `Tq,R,N` at `0xe2a9f00`.
- `fieldName`: `T@"NSString",R,C,N` at `0xe2a9f10`.
- `fieldValue`: `T@"NSString",R,C,N` at `0xe2a9f20`.
- `placeholder`: `T@"NSString",R,C,N` at `0xe2a9f30`.
- `isChecked`: `TB,R,N` at `0xe2a9f40`.
- `isRequired`: `TB,R,N` at `0xe2a9f50`.
- `isRedacted`: `TB,R,N` at `0xe2a9f60`.
- `selectOptions`: `T@"NSArray",R,N,V_selectOptions` at `0xe2a9f70`.

## Lifecycle, ownership, callbacks and threading

0x442b9ec initializes optional native value via 0x4294be4, not an identified DOM service. fieldName/fieldValue/placeholder 0x442ba74/0x442bb64/0x442bc54 return nil for absent optional native strings. isChecked/isRequired/isRedacted 0x442bd44/0x442bd58/0x442bd6c read stored flags; redaction policy is not implemented by those loads. selectOptions 0x442bd80 lazily creates and caches ArcSelectOption objects from 0x48-byte native elements.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This extraction schema needs a renderer/IPC producer, not merely HTML form automation. No CEF exact form-content extraction schema is established. The wrapper itself exposes no callbacks or persistence writes.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithFormControlInfo: | `@24@0:8r^v16` | 0x442b9ec | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x4294a84,0x4294be4 | [0x442b9ec](../bodies/0x442b9ec.c); record 0xd77aa44; dyld L2310 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -controlType | `q16@0:8` | 0x442ba60 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442ba60](../bodies/0x442ba60.c); record 0xd77aa50; dyld L2311 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -fieldName | `@16@0:8` | 0x442ba74 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc; returns nil for absent optional string. Traps if outer optional is empty. | 0x60a60,0x1c391bc,0x5810 | [0x442ba74](../bodies/0x442ba74.c); record 0xd77aa5c; dyld L2312 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -fieldValue | `@16@0:8` | 0x442bb64 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc; returns nil for absent optional string. Traps if outer optional is empty. | 0x60a60,0x1c391bc,0x5810 | [0x442bb64](../bodies/0x442bb64.c); record 0xd77aa68; dyld L2313 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -placeholder | `@16@0:8` | 0x442bc54 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc; returns nil for absent optional string. Traps if outer optional is empty. | 0x60a60,0x1c391bc,0x5810 | [0x442bc54](../bodies/0x442bc54.c); record 0xd77aa74; dyld L2314 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -isChecked | `B16@0:8` | 0x442bd44 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442bd44](../bodies/0x442bd44.c); record 0xd77aa80; dyld L2315 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -isRequired | `B16@0:8` | 0x442bd58 | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442bd58](../bodies/0x442bd58.c); record 0xd77aa8c; dyld L2316 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -isRedacted | `B16@0:8` | 0x442bd6c | Returns native/inline field; traps when the backing optional is empty. | No direct anonymous call recorded / indirect possible | [0x442bd6c](../bodies/0x442bd6c.c); record 0xd77aa98; dyld L2317 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -selectOptions | `@16@0:8` | 0x442bd80 | Lazily creates cached NSMutableArray of ArcSelectOption wrappers from native vector elements (stride 0x48); engaged outer optional required. |  ; ObjC selectors arrayWithCapacity:,initWithSelectOption:,addObject: | [0x442bd80](../bodies/0x442bd80.c); record 0xd77aaa4; dyld L2318 | medium / local_verified / available | Snapshot mutability, redaction and native producer validation remain unknown. |
| (base) | -form_control_info | `{optional<arc::FormControlInfo>=(?=c{FormControlInfo=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{vector<arc::SelectOption, std::allocator<arc::SelectOption>>={__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>=^{SelectOption}QQ}}BBB})B}16@0:8` | 0x442be5c | Indexed body available; local calls recorded, end-to-end behavior unresolved. | 0x4294be4 | [0x442be5c](../bodies/0x442be5c.c); record 0xd77aab0; dyld L2319 | low / unresolved / available | Audit native preconditions, result/error shape, callbacks and downstream service behavior. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x442bea4 | Destroys backing storage; clears object/backing fields. ARC injection may hide retain/release operations. | 0x4294a84 | [0x442bea4](../bodies/0x442bea4.c); record 0xd77aabc; dyld L2320 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x442bed8 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x442bed8](../bodies/0x442bed8.c); record 0xd77aac8; dyld L2321 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Which fields are redacted, when is the snapshot taken, and does extraction include cross-origin subframes?
