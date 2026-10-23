# ArcMenuItemModel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 24 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 80 bytes; class metadata `0xe37e5a8`, class_ro `0xe2aee38`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_hasAccelerator` | 8 | 1 | `B` |
| `_accelerator` | 16 | 8 | `@"ArcKeyboardAccelerator"` |
| `_icon` | 24 | 8 | `@"NSImage"` |
| `_submenuModel` | 32 | 8 | `@"ArcMenuModel"` |
| `_menu_item_model_delegate_bridge` | 40 | 16 | `{shared_ptr<ArcMenuItemModelDelegateBridge>="__ptr_"^{ArcMenuItemModelDelegateBridge}"__cntrl_"^{__shared_weak_count}}` |
| `_menu_item_model` | 56 | 24 | `{optional<arc::MenuItemModel>=""(?="__null_state_"c"__val_"{MenuItemModel="impl_"{shared_ptr<arc::MenuItemModel::Impl>="… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native item wraps a menu-model pointer plus index. `0x443fb34`, `0x443fb68`, `0x443fc3c` through `0x443fcbc`, `0x443fcdc`, `0x443ff68` dispatch to model vtable at that index. `0x443fd88` lazily caches submenu strongly; delegate setter `0x443ff88` captures weak model/delegate and registers bridge. Initializer IMPs: `0x443fa10`. Cleanup IMP: `0x44404a4`.

## Threading and caller/callee relations

`0x443ff88` installs native bridge and nil path unregisters through `0x432bf9c`. Bridge callbacks `0x4445404` and `0x4445458` emit changed/removed notifications. Index-based virtual calls and cached submenu imply a consequential invalidation boundary; no synchronization or post-removal validity is established. Category overrides are independently retained in the matrix and category-overrides.json.

## Chrome / CEF route implications

Source-backed ui::MenuModel candidate: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `ui/base/models/menu_model.h:104`/:124/:173/:187/:190/:194/:199/:211/:214. Vtable offsets are evidence, not a complete proved class match. Extensions overrides hasAccelerator/accelerator/icon with native queries (`0x44080f8`, `0x44081f0`, `0x4408318`), whereas base IMPs read ivars. Chrome-native menu state and app callback adaptation must be preserved; CEF menu equivalence needs separate comparison.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithMenuItemModel:` | `@24@0:8r^v16` | [0x443fa10](../bodies/443fa10.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77cdbc`; D 3160 | M; available; local_body_observed |
| (base) | `-type` | `q16@0:8` | [0x443fb34](../bodies/443fb34.c) | Calls native menu-model vtable+0x20 with stored item index and widens result to public integer type. | field/indirect dispatch; see body | M `0xd77cdc8`; D 3161 | M; available; bridge_reviewed_partial |
| (base) | `-label` | `@16@0:8` | [0x443fb68](../bodies/443fb68.c) | Calls native menu-model label getter at stored index and converts UTF-16 result to NSString. | 0x1d9fb90 | M `0xd77cdd4`; D 3162 | M; available; bridge_reviewed_partial |
| (base) | `-logicalType` | `q16@0:8` | [0x443fc14](../bodies/443fc14.c) | Calls 0x432bb60 and widens native logical type. | 0x432bb60 | M `0xd77cde0`; D 3163 | M; available; bridge_reviewed_partial |
| (base) | `-isChecked` | `B16@0:8` | [0x443fc3c](../bodies/443fc3c.c) | Calls native menu-model vtable+0x98 with stored index and returns Boolean checked state. | field/indirect dispatch; see body | M `0xd77cdec`; D 3164 | M; available; bridge_reviewed_partial |
| (base) | `-isEnabled` | `B16@0:8` | [0x443fc5c](../bodies/443fc5c.c) | Calls native menu-model vtable+0xb8 with stored index for enabled state. | field/indirect dispatch; see body | M `0xd77cdf8`; D 3165 | M; available; bridge_reviewed_partial |
| (base) | `-isVisible` | `B16@0:8` | [0x443fc7c](../bodies/443fc7c.c) | Calls native menu-model vtable+0xc0 with stored index for visible state. | field/indirect dispatch; see body | M `0xd77ce04`; D 3166 | M; available; bridge_reviewed_partial |
| (base) | `-isAlerted` | `B16@0:8` | [0x443fc9c](../bodies/443fc9c.c) | Calls native menu-model vtable+0xc8 with stored index for alerted state. | field/indirect dispatch; see body | M `0xd77ce10`; D 3167 | M; available; bridge_reviewed_partial |
| (base) | `-isNewFeature` | `B16@0:8` | [0x443fcbc](../bodies/443fcbc.c) | Calls native menu-model vtable+0xd0 with stored index for new-feature state. | field/indirect dispatch; see body | M `0xd77ce1c`; D 3168 | M; available; bridge_reviewed_partial |
| (base) | `-accessibleName` | `@16@0:8` | [0x443fcdc](../bodies/443fcdc.c) | Calls native accessible-name getter with stored item index and converts UTF-16 result. | 0x1d9fb90 | M `0xd77ce28`; D 3169 | M; available; bridge_reviewed_partial |
| (base) | `-submenuModel` | `@16@0:8` | [0x443fd88](../bodies/443fd88.c) | Returns cached submenu at self+0x20 or wraps native vtable+0xe8 submenu via 0x433cf10 and caches it strongly. | 0x433cf10 | M `0xd77ce34`; D 3170 | M; available; bridge_reviewed_partial |
| (base) | `-activated` | `v16@0:8` | [0x443ff68](../bodies/443ff68.c) | Requires engaged handle; calls native menu-model vtable+0xf0 with stored item index to activate it. | field/indirect dispatch; see body | M `0xd77ce40`; D 3171 | M; available; bridge_reviewed_partial |
| (base) | `-setDelegate:` | `v24@0:8@16` | [0x443ff88](../bodies/443ff88.c) | Creates bridge with weak model/delegate, registers via 0x433cc10; nil path clears bridge and unregisters through 0x432bf9c. | 0x432bf9c; 0x433cc10 | M `0xd77ce4c`; D 3172 | M; available; bridge_reviewed_partial |
| (base) | `-menu_item_model` | `{optional<arc::MenuItemModel>=(?=c{MenuItemModel={shared_ptr<arc::MenuItemModel::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x44402c8](../bodies/44402c8.c) | Copies optional native shared handle from self+0x38 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77ce58`; D 3173 | H; available; local_field_observed |
| (base) | `-hasAccelerator` | `B16@0:8` | [0x40b1230](../bodies/40b1230.c) | Loads 1 byte(s) from self+0x8 (_hasAccelerator) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77ce64`; D 3174 | H; available_pseudocode_incorrect_raw_verified; local_field_observed |
| (base) | `-accelerator` | `@16@0:8` | [0x2cb140](../bodies/2cb140.c) | Loads 8 byte(s) from self+0x10 (_accelerator) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77ce70`; D 3175 | H; available; local_field_observed |
| (base) | `-icon` | `@16@0:8` | [0x4084dc0](../bodies/4084dc0.c) | Loads 8 byte(s) from self+0x18 (_icon) and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class. | field/indirect dispatch; see body | M `0xd77ce7c`; D 3176 | H; available; local_field_observed |
| (base) | `-menu_item_model_delegate_bridge` | `{shared_ptr<ArcMenuItemModelDelegateBridge>=^{ArcMenuItemModelDelegateBridge}^{__shared_weak_count}}16@0:8` | [0x4440308](../bodies/4440308.c) | Returns copy of shared bridge/native handle and increments its control count; exact field offset is in the body and layout. | field/indirect dispatch; see body | M `0xd77ce88`; D 3177 | H; available; local_field_observed |
| (base) | `-setMenu_item_model_delegate_bridge:` | `v32@0:8{shared_ptr<ArcMenuItemModelDelegateBridge>=^{ArcMenuItemModelDelegateBridge}^{__shared_weak_count}}16` | [0x4440328](../bodies/4440328.c) | Assigns shared native bridge handle, incrementing new control count and releasing previous shared owner; identity is defined by metadata, despite shared Ghidra name. | field/indirect dispatch; see body | M `0xd77ce94`; D 3178 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x44404a4](../bodies/44404a4.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77cea0`; D 3179 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4440628](../bodies/4440628.c) | Initializes native storage with explicit stores: self+0x48=0; self+0x28=0; self+0x30=0; self+0x38=0. | field/indirect dispatch; see body | M `0xd77ceac`; D 3180 | M; available; local_body_observed |
| Extensions | `-hasAccelerator` | `B16@0:8` | [0x44080f8](../bodies/44080f8.c) | Category queries optional native accelerator through 0x432bdc0 and returns engagement; overrides base byte getter. | 0x432bdc0 | M `0xd779630`; D 4968 | M; available; bridge_reviewed_partial |
| Extensions | `-accelerator` | `@16@0:8` | [0x44081f0](../bodies/44081f0.c) | Category queries same optional accelerator and returns nil or a newly wrapped ArcKeyboardAccelerator; overrides base ivar getter. | 0x432bdc0 | M `0xd77963c`; D 4969 | M; available; bridge_reviewed_partial |
| Extensions | `-icon` | `@16@0:8` | [0x4408318](../bodies/4408318.c) | Category obtains native image through 0x432be28, returns nil for empty image or converts through 0x2a02ccc; overrides base ivar getter. | 0x2a02ccc; 0x432be28 | M `0xd779648`; D 4970 | M; available; bridge_reviewed_partial |

## Remaining consequential questions

Callbacks `0x4445404`/`0x4445458` send menuItemModelChanged:/Removed:. Determine index invalidation after removal, callback thread/reentrancy, event flags and submenu lifetime. Category hasAccelerator uses a recovered secondary return register; confirm disassembly.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
