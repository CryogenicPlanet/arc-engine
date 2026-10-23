# ArcBrowserContextDevTools

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37da18`, class RO `0xe2ac5d0`; raw otool `objc170.txt:32514`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Base metadata declares six methods; Extensions adds isEqual: and hash with distinct method identities and no same-selector base override. Initialization retains optional native DevTools shared state (0x4437200); webContents reads native +0x98 and returns nil or an ArcWebContents wrapper (0x4437348). close conditionally calls 0xc107cd4 with mode 3, but Ghidra mistakes its ABI for C close(int) (0x4437324); recheck arm64 arguments before implementing. Extensions equality/hash use extracted native identity rather than ObjC wrapper address (0x43f8f28, 0x43f91fc). Cleanup shares 0x43df69c. No thread/close-completion contract is established. DevTools frontend hosting and context identity are the route questions; no CEF frontend/Chrome DevTools parity is inferred from method names.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _browser_context_dev_tools | 8 | 24 | `{optional<arc::BrowserContext::DevTools>=""(?="__null_state_"c"__val_"{DevTools="impl_"{shared_ptr<arc::BrowserContext::DevTools::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2ac598 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| browser_context_dev_tools | `T{optional<arc::BrowserContext::DevTools>=(?=c{DevTools={shared_ptr<arc::BrowserContext::DevTools::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browser_context_dev_tools` | 0xe2ac5c0 |

Categories in assignment: `Extensions` at `0xe2a5768`. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBrowserContextDevTools:` | `@24@0:8r^v16` | [0x4437200](../bodies/04437200.txt) | Calls NSObject init; retains native shared DevTools handle at self+8/+0x10 and engages +0x18. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bd04`; dobjc170.txt:2754 | medium-local; available; local_review |
| (base) | `-close` | `v16@0:8` | [0x4437324](../bodies/04437324.txt) | Requires engaged wrapper; if native frontend pointer at impl+0x90 exists, invokes 0xc107cd4 with mode 3. Ghidra uses invalid close(int) prototype. | 0xc107cd4 | metadata `0xd77bd10`; dobjc170.txt:2755 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-webContents` | `@16@0:8` | [0x4437348](../bodies/04437348.txt) | Requires engaged native DevTools; reads native webcontents pointer at impl+0x98; nil returns nil, otherwise allocates ArcWebContents wrapper. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bd1c`; dobjc170.txt:2756 | medium-local; available; local_review |
| (base) | `-browser_context_dev_tools` | `{optional<arc::BrowserContext::DevTools>=(?=c{DevTools={shared_ptr<arc::BrowserContext::DevTools::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/043df53c.txt) | Copies optional native shared handle from self+8 into aggregate return storage, increments non-null control block; source engagement is at self+0x18. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bd28`; dobjc170.txt:2757 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/043df69c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bd34`; dobjc170.txt:2758 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/043df760.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bd40`; dobjc170.txt:2759 | medium-local; available; literal_partial |
| Extensions | `-isEqual:` | `B24@0:8@16` | [0x43f8f28](../bodies/043f8f28.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: isKindOfClass:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7786a4`; dobjc170.txt:4925 | low-unresolved; available; unresolved |
| Extensions | `-hash` | `Q16@0:8` | [0x43f91fc](../bodies/043f91fc.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x18f3c. | 0x18f3c | metadata `0xd7786b0`; dobjc170.txt:4926 | low-unresolved; available; unresolved |

Unresolved engineering questions (method-specific evidence above):

- Frontend/context lifetime after close.
- Actual C++ close mode, frontend lifetime and queue.
- Ownership/liveness of native webcontents pointer after closing DevTools.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
