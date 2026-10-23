# ArcBackForwardList

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37bc68`, class RO `0xe2a48a8`; raw otool `objc170.txt:22728`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Initialization retains native optional shared state and materializes backList/forwardList plus adjacent/current ArcBackForwardListItem wrappers (0x43df76c). The five ObjC getters are field reads (0x2d0fb40, 0x2cb140, 0x4084dc0, 0x4060580, 0x133ae90); infer their types from this class's metadata, not shared Ghidra names. Destruction releases native handle and ObjC fields (0x43dfc10). The native setter changes native optional state (0x43dfaf0); refreshing the materialized ObjC arrays is not shown there. No callbacks/queue were established. History navigation consumes a native item through ArcWebContentsNavigationController at 0x44180d4. CEF GetNavigationEntries (include/cef_browser.h:669) is a candidate for history enumeration, but snapshot identity/order and go-to-item parity need testing.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `72`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _backItem | 8 | 8 | `@"ArcBackForwardListItem"` | 0xe2a4780 |
| _currentItem | 16 | 8 | `@"ArcBackForwardListItem"` | 0xe2a47a0 |
| _forwardItem | 24 | 8 | `@"ArcBackForwardListItem"` | 0xe2a47c0 |
| _backList | 32 | 8 | `@"NSArray"` | 0xe2a47e0 |
| _forwardList | 40 | 8 | `@"NSArray"` | 0xe2a4800 |
| _nativeBackForwardList | 48 | 24 | `{optional<arc::BackForwardList>=""(?="__null_state_"c"__val_"{BackForwardList="impl_"{shared_ptr<arc::BackForwardList::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2a4820 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| nativeBackForwardList | `T{optional<arc::BackForwardList>=(?=c{BackForwardList={shared_ptr<arc::BackForwardList::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_nativeBackForwardList` | 0xe2a4848 |
| backList | `T@"NSArray",R,N,V_backList` | 0xe2a4858 |
| forwardList | `T@"NSArray",R,N,V_forwardList` | 0xe2a4868 |
| backItem | `T@"ArcBackForwardListItem",R,N,V_backItem` | 0xe2a4878 |
| currentItem | `T@"ArcBackForwardListItem",R,N,V_currentItem` | 0xe2a4888 |
| forwardItem | `T@"ArcBackForwardListItem",R,N,V_forwardItem` | 0xe2a4898 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBackForwardList:` | `@24@0:8r^v16` | [0x43df76c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df76c.txt) | Retains native shared handle at +0x30/+0x38, engages +0x40; materializes back/forward arrays and adjacent/current item wrappers during init. | 0x42ae870,0x42ae8ac,0x42ae8e0 | metadata `0xd777b50`; dobjc170.txt:1336 | medium-partial; available; partial_review |
| (base) | `-nativeBackForwardList` | `{optional<arc::BackForwardList>=(?=c{BackForwardList={shared_ptr<arc::BackForwardList::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43dfab0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfab0.txt) | Copies optional native shared handle from self+0x30 into aggregate return storage, increments non-null control block; source engagement is at self+0x40. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777b5c`; dobjc170.txt:1337 | medium-local; available; local_review |
| (base) | `-setNativeBackForwardList:` | `v40@0:8{optional<arc::BackForwardList>=(?=c{BackForwardList={shared_ptr<arc::BackForwardList::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43dfaf0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfaf0.txt) | Replaces optional native shared handle at self+0x30, retaining/releasing control blocks as presence changes; engagement byte self+0x40. | 0x4331e20 | metadata `0xd777b68`; dobjc170.txt:1338 | medium-local; available; local_review |
| (base) | `-backItem` | `@16@0:8` | [0x2d0fb40](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/02d0fb40.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777b74`; dobjc170.txt:1339 | high-local; available; literal_verified |
| (base) | `-currentItem` | `@16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777b80`; dobjc170.txt:1340 | high-local; available; literal_verified |
| (base) | `-forwardItem` | `@16@0:8` | [0x4084dc0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04084dc0.txt) | Literal return: *(ID *)(param_1 + 0x18). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777b8c`; dobjc170.txt:1341 | high-local; available; literal_verified |
| (base) | `-backList` | `@16@0:8` | [0x4060580](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04060580.txt) | Literal return: *(ID *)(param_1 + 0x20). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777b98`; dobjc170.txt:1342 | high-local; available; literal_verified |
| (base) | `-forwardList` | `@16@0:8` | [0x133ae90](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0133ae90.txt) | Literal return: *(unsigned_long_long *)(param_1 + 0x28). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777ba4`; dobjc170.txt:1343 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43dfc10](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfc10.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777bb0`; dobjc170.txt:1344 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43dfd10](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfd10.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777bbc`; dobjc170.txt:1345 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Ordering and invalidation of snapshot arrays; nil initialization behavior.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcBackForwardList.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcBackForwardList.md).
