# ArcWebDialog

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37ced8`, class_ro `0xe2a9c80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Value dialog descriptor containing strings, dimensions and weak native WebContents reference.

## State and layout

Metadata instance start 8, size 88 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_dialog` | 0x8 | 80 | `{WebDialog="accessible_title"{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>="__rep_"(__rep="__s"{__short="__data_"[11S]"__padding_"{__padding<1UL>="__padding_"[1c]}"__size_"b7"__is_long_"b1}"__l"{__long="__data_"^S"__size_"Q"__cap_"b63"__is_long_"b1})}"preferred_size"{Size="width_"i"height_"i}"minimum_size"{Size="width_"i"height_"i}"title"{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>="__rep_"(__rep="__s"{__short="__data_"[11S]"__padding_"{__padding<1UL>="__padding_"[1c]}"__size_"b7"__is_long_"b1}"__l"{__long="__data_"^S"__size_"Q"__cap_"b63"__is_long_"b1})}"web_contents"{weak_ptr<arc::WebContents>="__ptr_"^{WebContents}"__cntrl_"^{__shared_weak_count}}}` | `0xe2a9bf8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `dialog`: `T{WebDialog={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Size=ii}{Size=ii}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{weak_ptr<arc::WebContents>=^{WebContents}^{__shared_weak_count}}},N,V_dialog` at `0xe2a9c20`.
- `accessibleTitle`: `T@"NSString",R,N` at `0xe2a9c30`.
- `preferredSize`: `T{CGSize=dd},R,N` at `0xe2a9c40`.
- `minimumSize`: `T{CGSize=dd},R,N` at `0xe2a9c50`.
- `title`: `T@"NSString",R,N` at `0xe2a9c60`.
- `webContents`: `T@"ArcWebContents",R,N` at `0xe2a9c70`.

## Lifecycle, ownership, callbacks and threading

0x441cd64 copies strings and size fields, increments weak native control count. webContents 0x441cf64 locks weak control block and allocates a fresh ArcWebContents wrapper from live native peer; expired or null peer traps rather than returning nil in visible body. dialog 0x441d120 copies whole descriptor through 0x43466c8; 0x441d2a0 destroys it through 0x434660c.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

accessibleTitle 0x441cefc and title 0x441cf40 convert UTF-16 strings; preferred/minimum sizes 0x441cf20 / 0x441cf30 unpack integer dimensions to CGSize. The app is responsible for presenting these descriptors; window/modal lifecycle and Chrome UI delegate producer are outside this review. No CEF dialog equivalent is proved.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithWebDialog: | `@24@0:8r^v16` | 0x441cd64 | Calls super init; copies native strings/dimensions and weak WebContents pair, updating weak control count. | 0x7ab764,0x7ab624,0x5810 | [0x441cd64](../bodies/0x441cd64.c); record 0xd77a938; dyld L2283 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -accessibleTitle | `@16@0:8` | 0x441cefc | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. | 0x1d9fb90 | [0x441cefc](../bodies/0x441cefc.c); record 0xd77a944; dyld L2284 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -preferredSize | `{CGSize=dd}16@0:8` | 0x441cf20 | Reads native stored integer width/height pair and converts to CGSize doubles. | No direct anonymous call recorded / indirect possible | [0x441cf20](../bodies/0x441cf20.c); record 0xd77a950; dyld L2285 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -minimumSize | `{CGSize=dd}16@0:8` | 0x441cf30 | Reads native minimum integer width/height pair and converts to CGSize doubles. | No direct anonymous call recorded / indirect possible | [0x441cf30](../bodies/0x441cf30.c); record 0xd77a95c; dyld L2286 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -title | `@16@0:8` | 0x441cf40 | Converts stored/native UTF-16LE string data to NSString via 0x1d9fb90. | 0x1d9fb90 | [0x441cf40](../bodies/0x441cf40.c); record 0xd77a968; dyld L2287 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -webContents | `@16@0:8` | 0x441cf64 | Locks native weak WebContents reference and creates a new ArcWebContents wrapper; expired/null reference traps. | 0x5810 ; ObjC selectors initWithWebContents: | [0x441cf64](../bodies/0x441cf64.c); record 0xd77a974; dyld L2288 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -dialog | `{WebDialog={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Size=ii}{Size=ii}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{weak_ptr<arc::WebContents>=^{WebContents}^{__shared_weak_count}}}16@0:8` | 0x441d120 | Copies native dialog aggregate using 0x43466c8. | 0x43466c8 | [0x441d120](../bodies/0x441d120.c); record 0xd77a980; dyld L2289 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -setDialog: | `v96@0:8{WebDialog={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Size=ii}{Size=ii}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{weak_ptr<arc::WebContents>=^{WebContents}^{__shared_weak_count}}}16` | 0x441d12c | Assigns native dialog aggregate, including string storage, sizes and weak WebContents control reference. | 0x7ab764,0x7ab624,0x5810 | [0x441d12c](../bodies/0x441d12c.c); record 0xd77a98c; dyld L2290 | medium / reviewed_partial / available | Validate descriptor aggregate ABI and lifetime of native content. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x441d2a0 | Destroys backing storage. ARC injection may hide retain/release operations. | 0x434660c | [0x441d2a0](../bodies/0x441d2a0.c); record 0xd77a998; dyld L2291 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x441d2a8 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x441d2a8](../bodies/0x441d2a8.c); record 0xd77a9a4; dyld L2292 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Does app assume wrapper identity across webContents reads, who owns the native dialog content, and can callbacks outlive dismissal?
