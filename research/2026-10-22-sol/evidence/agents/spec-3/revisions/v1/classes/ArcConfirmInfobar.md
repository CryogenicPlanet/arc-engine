# ArcConfirmInfobar

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37de28`, class RO `0xe2ad5d0`; raw otool `objc170.txt:33730`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization retains optional native ConfirmInfobar with a vtable-bearing value (0x443b22c); cleanup releases its shared control block (0x443b4a8). messageText converts native UTF-16 through 0x430dbcc/0x1d9fb90 (0x443b368). accept checks engagement and calls backend vtable +0x108 if live; Ghidra incorrectly uses socket accept's C ABI (0x443b434). Callback consumption, infobar dismissal and exactly-once acceptance remain unresolved. No queue affinity was proven. Chrome infobar integration or a CEF-specific emulation needs a confirmed delegate/action contract; the message getter alone cannot specify it.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _confirm_infobar | 8 | 32 | `{optional<arc::ConfirmInfobar>=""(?="__null_state_"c"__val_"{ConfirmInfobar="_vptr$ConfirmInfobar"^^?"impl_"{shared_ptr<arc::ConfirmInfobar::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2ad598 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| confirm_infobar | `T{optional<arc::ConfirmInfobar>=(?=c{ConfirmInfobar=^^?{shared_ptr<arc::ConfirmInfobar::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_confirm_infobar` | 0xe2ad5c0 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithConfirmInfobar:` | `@24@0:8r^v16` | [0x443b22c](../bodies/0443b22c.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3fc`; dobjc170.txt:2920 | low-unresolved; available; unresolved |
| (base) | `-messageText` | `@16@0:8` | [0x443b368](../bodies/0443b368.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x430dbcc,0x1d9fb90 | metadata `0xd77c408`; dobjc170.txt:2921 | medium-local; available; conversion_partial |
| (base) | `-accept` | `v16@0:8` | [0x443b434](../bodies/0443b434.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c414`; dobjc170.txt:2922 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-confirm_infobar` | `{optional<arc::ConfirmInfobar>=(?=c{ConfirmInfobar=^^?{shared_ptr<arc::ConfirmInfobar::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x443b45c](../bodies/0443b45c.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c420`; dobjc170.txt:2923 | low-unresolved; available; unresolved |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x443b4a8](../bodies/0443b4a8.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c42c`; dobjc170.txt:2924 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4432fc0](../bodies/04432fc0.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c438`; dobjc170.txt:2925 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
