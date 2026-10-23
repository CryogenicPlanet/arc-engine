# ArcExtensionToolbarAction

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 11 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dfb8`, class_ro `0xe2ada90`; instanceStart 8, instanceSize 120 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 112 | `_extension_toolbar_action` | `{ExtensionToolbarAction="id"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data…` | 0xe2ad9f8 |

Initialization copies the native toolbar-action record via 0x441e6d8 (0x443bc48). id is a converted native byte string (0x43e632c), accessibleName/tooltipText are converted UTF-16 (0x443bca0, 0x443bcc4), and icon is nil when the image is absent (0x443bce8).

## Lifecycle, callbacks and relations

pinnedPosition boxes a present optional integer and returns nil when absent (0x443bd08); enabled reads +0x68 (0x4243428). Aggregate assignment and lifecycle call native helpers (0x443bd4c, 0x443bd58, 0x443bd60). This object does not invoke a toolbar action or persist pinning.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc snapshot of Chrome-extension toolbar state; CEF feasibility depends on the producer/action API, not these getters.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcExtensionToolbarAction.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithExtensionToolbarAction: | `@24@0:8r^v16` | 0x443bc48 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x0441e6d8 | IMP 0x443bc48; record 0xd77c598; dyld L2961; otool L34081 | medium / available / local_observation |
| base | - id | `@16@0:8` | 0x43e632c | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x1f, 0x8, 0x10. | 0x01c391bc | IMP 0x43e632c; record 0xd77c5a4; dyld L2962; otool L23413 | medium / available / local_observation |
| base | - accessibleName | `@16@0:8` | 0x443bca0 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x37, 0x20, 0x28. | 0x01d9fb90 | IMP 0x443bca0; record 0xd77c5b0; dyld L2963; otool L34087 | medium / available / local_observation |
| base | - tooltipText | `@16@0:8` | 0x443bcc4 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x4f, 0x38, 0x40. | 0x01d9fb90 | IMP 0x443bcc4; record 0xd77c5bc; dyld L2964; otool L34090 | medium / available / local_observation |
| base | - icon | `@16@0:8` | 0x443bce8 | Returns nil when native image storage absent; otherwise converts through 0x2a02ccc. | 0x02a02ccc | IMP 0x443bce8; record 0xd77c5c8; dyld L2965; otool L34093 | medium / available / bounded_review |
| base | - enabled | `B16@0:8` | 0x4243428 | Reads 8-bit register value from self+0x68 (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4243428; record 0xd77c5d4; dyld L2966; otool L34096 | high / available / local_verified |
| base | - pinnedPosition | `@16@0:8` | 0x443bd08 | Returns nil when optional pin position absent; otherwise NSNumber integer. | local state / indirect operation; see body | IMP 0x443bd08; record 0xd77c5e0; dyld L2967; otool L34099 | medium / available / bounded_review |
| base | - extension_toolbar_action | `{ExtensionToolbarAction={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}}B{optional<int>=(?=ci)B}}16@0:8` | 0x443bd40 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x8. | 0x0441e600 | IMP 0x443bd40; record 0xd77c5ec; dyld L2968; otool L34102 | medium / available / local_observation |
| base | - setExtension_toolbar_action: | `v128@0:8{ExtensionToolbarAction={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}}B{optional<int>=(?=ci)B}}16` | 0x443bd4c | Assigns/replaces local native or ObjC state; self offsets 0x8. Calls 0x0441e6d8. | 0x0441e6d8 | IMP 0x443bd4c; record 0xd77c5f8; dyld L2969; otool L34105 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x443bd58 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x8. | 0x0441e4e8 | IMP 0x443bd58; record 0xd77c604; dyld L2970; otool L34108 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x443bd60 | Initializes native storage/engagement fields at self offsets 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10; returns self. | 0x043386a0 | IMP 0x443bd60; record 0xd77c610; dyld L2971; otool L34111 | medium / available / local_observation |

## Remaining consequential questions

Confirm pinning-index meaning and absence semantics, snapshot ownership, and refresh after extension action changes. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
