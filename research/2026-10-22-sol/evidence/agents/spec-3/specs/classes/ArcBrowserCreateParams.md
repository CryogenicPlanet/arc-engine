# ArcBrowserCreateParams

ArcCore **1.170.0 arm64**, unslid engine VAs. 12 declared identities. Superclass `NSObject`; class metadata `0xe37c078`, class RO `0xe2a5d20`; raw otool `objc170.txt:24493`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

The ObjC wrapper retains optional native shared creation parameters (0x4400f58, 0x43df69c). Native capability bytes back canResize/canMaximize/canFullscreen at native +0xa8/+0xa9/+0xaa (0x44010a4, 0x44010bc, 0x44010d4); initial bounds convert four integers to CGRect doubles (0x44010ec). initialOriginSpecified is native enum ==1 (0x4401118); pipSize traps when nested optional is absent (0x4401138), with an unusual second offset requiring ABI/disassembly confirmation. ArcBrowser constructs this wrapper (0x43e0c34). No callbacks or thread guarantee was established. CEF bounds/parent-view fields are comparison points (include/internal/cef_types_mac.h:100,110), but no mapping of resizing/fullscreen/PiP ownership is established.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _nativeCreateParams | 8 | 24 | `{optional<arc::BrowserCreateParams>=""(?="__null_state_"c"__val_"{BrowserCreateParams="impl_"{shared_ptr<arc::BrowserCreateParams::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2a5c78 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| nativeCreateParams | `T{optional<arc::BrowserCreateParams>=(?=c{BrowserCreateParams={shared_ptr<arc::BrowserCreateParams::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_nativeCreateParams` | 0xe2a5ca0 |
| userTitle | `T@"NSString",R,N` | 0xe2a5cb0 |
| canResize | `TB,R,N` | 0xe2a5cc0 |
| canMaximize | `TB,R,N` | 0xe2a5cd0 |
| canFullscreen | `TB,R,N` | 0xe2a5ce0 |
| initialBounds | `T{CGRect={CGPoint=dd}{CGSize=dd}},R,N` | 0xe2a5cf0 |
| initialOriginSpecified | `TB,R,N` | 0xe2a5d00 |
| pipSize | `T{CGSize=dd},R,N` | 0xe2a5d10 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBrowserCreateParams:` | `@24@0:8r^v16` | [0x4400f58](../bodies/04400f58.txt) | Calls NSObject init; copies native pointer/control block into optional, incrementing non-null shared control count, engages self+0x18. | 0x4331e20 | metadata `0xd778bfc`; dobjc170.txt:1639 | medium-local; available; local_review |
| (base) | `-userTitle` | `@16@0:8` | [0x4400fe4](../bodies/04400fe4.txt) | Converts native user-title string at native params+0x90; requires engaged native params. | 0x60a60,0x1c391bc | metadata `0xd778c08`; dobjc170.txt:1640 | medium-local; available; local_review |
| (base) | `-canResize` | `B16@0:8` | [0x44010a4](../bodies/044010a4.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 8) + 0xa8). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c14`; dobjc170.txt:1641 | medium-local; available; literal_partial |
| (base) | `-canMaximize` | `B16@0:8` | [0x44010bc](../bodies/044010bc.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 8) + 0xa9). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c20`; dobjc170.txt:1642 | medium-local; available; literal_partial |
| (base) | `-canFullscreen` | `B16@0:8` | [0x44010d4](../bodies/044010d4.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 8) + 0xaa). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c2c`; dobjc170.txt:1643 | medium-local; available; literal_partial |
| (base) | `-initialBounds` | `{CGRect={CGPoint=dd}{CGSize=dd}}16@0:8` | [0x44010ec](../bodies/044010ec.txt) | Converts four 32-bit native bounds fields at +0x20/+0x24/+0x28/+0x2c to CGRect doubles. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c38`; dobjc170.txt:1644 | medium-local; available; local_review |
| (base) | `-initialOriginSpecified` | `B16@0:8` | [0x4401118](../bodies/04401118.txt) | Returns true only when native origin field +0x58 equals 1; requires native params. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c44`; dobjc170.txt:1645 | medium-local; available; local_review |
| (base) | `-pipSize` | `{CGSize=dd}16@0:8` | [0x4401138](../bodies/04401138.txt) | Requires native params plus nested optional flag at +0xc8; reads pipSize integers at +0xb0/+0xb8 into CGSize; missing optional traps. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c50`; dobjc170.txt:1646 | medium-partial; available; partial_review |
| (base) | `-nativeCreateParams` | `{optional<arc::BrowserCreateParams>=(?=c{BrowserCreateParams={shared_ptr<arc::BrowserCreateParams::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/043df53c.txt) | Copies optional native shared handle from self+8 into aggregate return storage, increments non-null control block; source engagement is at self+0x18. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c5c`; dobjc170.txt:1647 | medium-local; available; local_review |
| (base) | `-setNativeCreateParams:` | `v40@0:8{optional<arc::BrowserCreateParams>=(?=c{BrowserCreateParams={shared_ptr<arc::BrowserCreateParams::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43df57c](../bodies/043df57c.txt) | Replaces optional native shared handle at self+8, retaining/releasing control blocks as presence changes; engagement byte self+0x18. | 0x4331e20 | metadata `0xd778c68`; dobjc170.txt:1648 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/043df69c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c74`; dobjc170.txt:1649 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/043df760.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778c80`; dobjc170.txt:1650 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Underlying creation-parameter object lifetime vs parent browser.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Coordinate space and display scaling?
- Meaning of other origin enum states.
- Validate unusual second integer offset and ABI against disassembly before implementing PiP size.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
