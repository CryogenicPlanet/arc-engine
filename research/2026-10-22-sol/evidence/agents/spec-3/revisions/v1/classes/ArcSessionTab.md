# ArcSessionTab

ArcCore **1.170.0 arm64**, unslid engine VAs. 11 declared identities. Superclass `NSObject`; class metadata `0xe37e238`, class RO `0xe2ae290`; raw otool `objc170.txt:34697`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native SessionTab shared handle (0x443cb9c); navigation array is cached ObjC state at +8 (0x443cde8). setCurrentNavigationIndex traps for negative/out-of-range index (0x443ccf8). setNavigations stores supplied array strongly, extracts native navigation handles and replaces native vector through 0x4340c10 (0x443d0c8); current-index adjustment was not established. groupId returns nil if absent or uppercase 128-bit hex string (0x443d7b4). lastActiveTime converts Windows-epoch microseconds with special values (source-backed epoch inference from base/time/time.h:537-546 and literal offset) (0x443cd44). Pinned components/sessions/core/session_types.h:44-49,77,86,100 provides a source-backed SessionTab comparison, not verified layout equivalence. Persistence is not implemented by this wrapper; foreign-tab restore consumes it in ArcBrowser 0x43e2c10. CEF history enumeration alone does not establish session metadata/restore parity. Threading and native-session cache invalidation remain open.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _navigations | 8 | 8 | `@"NSArray"` | 0xe2ae1e8 |
| _session_tab | 16 | 24 | `{optional<arc::SessionTab>=""(?="__null_state_"c"__val_"{SessionTab="impl_"{shared_ptr<arc::SessionTab::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2ae208 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| session_tab | `T{optional<arc::SessionTab>=(?=c{SessionTab={shared_ptr<arc::SessionTab::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_session_tab` | 0xe2ae230 |
| isPinned | `TB,R,N` | 0xe2ae240 |
| currentNavigationIndex | `Ti,N` | 0xe2ae250 |
| lastActiveTime | `T@"NSDate",R,N` | 0xe2ae260 |
| navigations | `T@"NSArray",&,N,V_navigations` | 0xe2ae270 |
| groupId | `T@"NSString",R,C,N` | 0xe2ae280 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithSessionTab:` | `@24@0:8r^v16` | [0x443cb9c](../bodies/0443cb9c.txt) | Calls NSObject init; retains native shared SessionTab handle at self+0x10/+0x18 and engages +0x20. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c92c`; dobjc170.txt:3048 | medium-local; available; local_review |
| (base) | `-isPinned` | `B16@0:8` | [0x443ccc0](../bodies/0443ccc0.txt) | Observed return expression: (bool)*(byte *)(**(long **)(param_1 + 0x10) + 0x40). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c938`; dobjc170.txt:3049 | medium-local; available; literal_partial |
| (base) | `-currentNavigationIndex` | `i16@0:8` | [0x443ccdc](../bodies/0443ccdc.txt) | Observed return expression: *(int *)(**(long **)(param_1 + 0x10) + 0xc). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c944`; dobjc170.txt:3050 | medium-local; available; literal_partial |
| (base) | `-setCurrentNavigationIndex:` | `v20@0:8i16` | [0x443ccf8](../bodies/0443ccf8.txt) | Checks currentNavigationIndex nonnegative and below native navigation count; invalid index traps, valid updates native field at tab+0xc. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c950`; dobjc170.txt:3051 | medium-local; available; local_review |
| (base) | `-lastActiveTime` | `@16@0:8` | [0x443cd44](../bodies/0443cd44.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: dateWithTimeIntervalSince1970:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c95c`; dobjc170.txt:3052 | low-unresolved; available; unresolved |
| (base) | `-navigations` | `@16@0:8` | [0x443cde8](../bodies/0443cde8.txt) | Lazily builds/caches ArcSessionNavigation wrappers from 0x4340aa4; subsequent reads return cached array. | 0x4340aa4 | metadata `0xd77c968`; dobjc170.txt:3053 | medium-partial; available; partial_review |
| (base) | `-setNavigations:` | `v24@0:8@16` | [0x443d0c8](../bodies/0443d0c8.txt) | Stores ObjC navigation array strongly, extracts each native session_navigation, builds vector and calls 0x4340c10. | 0x41e7c00,0x4340c10 | metadata `0xd77c974`; dobjc170.txt:3054 | medium-partial; available; partial_review |
| (base) | `-groupId` | `@16@0:8` | [0x443d7b4](../bodies/0443d7b4.txt) | Returns nil for absent native group; present 128-bit group token formats as two uppercase 16-digit hexadecimal halves. | 0x331000,0x1c391bc | metadata `0xd77c980`; dobjc170.txt:3055 | medium-local; available; local_review |
| (base) | `-session_tab` | `{optional<arc::SessionTab>=(?=c{SessionTab={shared_ptr<arc::SessionTab::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x440a2ac](../bodies/0440a2ac.txt) | Copies optional native shared handle from self+0x10 into aggregate return storage, increments non-null control block; source engagement is at self+0x20. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c98c`; dobjc170.txt:3056 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440a40c](../bodies/0440a40c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c998`; dobjc170.txt:3057 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440a4dc](../bodies/0440a4dc.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c9a4`; dobjc170.txt:3058 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Cached-navigation consistency if native session changes.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Interaction with simultaneous setNavigations:.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Cache invalidation if underlying native session changes externally.
- Whether current index is adjusted; partial mutation/error behavior for invalid members.
- Token half ordering cross-check against Chromium serialization.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
