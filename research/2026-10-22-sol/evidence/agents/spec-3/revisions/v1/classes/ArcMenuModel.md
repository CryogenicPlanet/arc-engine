# ArcMenuModel

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37e5f8`, class RO `0xe2aef38`; raw otool `objc170.txt:35701`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native shared menu; items lazily materializes/caches ArcMenuItemModel wrappers from 0x432c5e4 (0x4440638, 0x444075c). Shared destructor releases cached ObjC collection and native handle (0x440a40c). menuWillShow/Close indirectly dispatch slots +0x100/+0x108 (0x4440a3c, 0x4440a5c). Their identification with ui::MenuModel hooks is a source-backed inference from pinned ui/base/models/menu_model.h:222,226 and call context; the binary vtable type has not been independently resolved. No callbacks/queue contract or item-cache invalidation was established. A replacement must preserve show/close lifecycle and dynamic-item changes, not only enumerate a static menu.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _items | 8 | 8 | `@"NSArray"` | 0xe2aeed0 |
| _menu_model | 16 | 24 | `{optional<arc::MenuModel>=""(?="__null_state_"c"__val_"{MenuModel="impl_"{shared_ptr<arc::MenuModel::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2aeef0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| menu_model | `T{optional<arc::MenuModel>=(?=c{MenuModel={shared_ptr<arc::MenuModel::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_menu_model` | 0xe2aef18 |
| items | `T@"NSArray",R,N,V_items` | 0xe2aef28 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithMenuModel:` | `@24@0:8r^v16` | [0x4440638](../bodies/04440638.txt) | Calls NSObject init; retains native shared menu handle at self+0x10/+0x18 and engages +0x20. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cec0`; dobjc170.txt:3183 | medium-local; available; local_review |
| (base) | `-items` | `@16@0:8` | [0x444075c](../bodies/0444075c.txt) | Lazily builds/caches ArcMenuItemModel array from native vector returned by 0x432c5e4. | 0x432c5e4 | metadata `0xd77cecc`; dobjc170.txt:3184 | medium-partial; available; partial_review |
| (base) | `-menuWillShow` | `v16@0:8` | [0x4440a3c](../bodies/04440a3c.txt) | Requires native menu optional then indirectly calls backend menu vtable slot +0x100. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ced8`; dobjc170.txt:3185 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-menuWillClose` | `v16@0:8` | [0x4440a5c](../bodies/04440a5c.txt) | Requires native menu optional then indirectly calls backend menu vtable slot +0x108. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cee4`; dobjc170.txt:3186 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-menu_model` | `{optional<arc::MenuModel>=(?=c{MenuModel={shared_ptr<arc::MenuModel::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x440a2ac](../bodies/0440a2ac.txt) | Copies optional native shared handle from self+0x10 into aggregate return storage, increments non-null control block; source engagement is at self+0x20. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cef0`; dobjc170.txt:3187 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440a40c](../bodies/0440a40c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cefc`; dobjc170.txt:3188 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440a4dc](../bodies/0440a4dc.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cf08`; dobjc170.txt:3189 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Dynamic native menu lifetime and cached items.
- Mutation and cache-refresh contract between menu opens.
- MenuWillShow identity is source-backed inference; dynamic menu/cache refresh order.
- MenuWillClose identity is source-backed inference; destruction after close.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
