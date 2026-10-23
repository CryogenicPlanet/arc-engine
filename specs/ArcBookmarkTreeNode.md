# ArcBookmarkTreeNode

ArcCore **1.170.0 arm64**, unslid engine VAs. 19 declared identities. Superclass `NSObject`; class metadata `0xe37bcb8`, class RO `0xe2a4a58`; raw otool `objc170.txt:22841`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Initialization and destruction retain/release an optional shared native handle (0x43dfd40, 0x43df69c); node accessors share an IMP with unrelated wrappers but retain their own metadata types. isValid dispatches to 0x42affa0 (0x43e05ac). liveChildren obtains current children through 0x42b00f4, creates wrappers and returns a copied array (0x43e05dc). IDs, UUID, parent ID and URL/title fields convert native values (0x43dfed8, 0x43dffa0, 0x43e0068, 0x43e01bc, 0x43e02d0). Ownership of the underlying bookmark model and validity after deletion remain open. No thread guarantee was established. Chrome bookmark-service integration is an implementation candidate, but these wrappers alone do not identify the precise service or establish a CEF route.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _node | 8 | 24 | `{optional<arc::BookmarkTreeNode>=""(?="__null_state_"c"__val_"{BookmarkTreeNode="impl_"{shared_ptr<arc::BookmarkTreeNode::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2a4940 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| node | `T{optional<arc::BookmarkTreeNode>=(?=c{BookmarkTreeNode={shared_ptr<arc::BookmarkTreeNode::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_node` | 0xe2a4968 |
| nodeType | `Tq,R,N` | 0xe2a4978 |
| isFolder | `TB,R,N` | 0xe2a4988 |
| isUrl | `TB,R,N` | 0xe2a4998 |
| nodeID | `T@"NSString",R,N` | 0xe2a49a8 |
| uuid | `T@"NSString",R,N` | 0xe2a49b8 |
| parentNodeID | `T@"NSString",R,N` | 0xe2a49c8 |
| index | `Ti,R,N` | 0xe2a49d8 |
| url | `T@"NSURL",R,N` | 0xe2a49e8 |
| title | `T@"NSString",R,N` | 0xe2a49f8 |
| faviconUrl | `T@"NSURL",R,N` | 0xe2a4a08 |
| dateAdded | `T@"NSDate",R,N` | 0xe2a4a18 |
| dateFolderModified | `T@"NSDate",R,N` | 0xe2a4a28 |
| isValid | `TB,R,N` | 0xe2a4a38 |
| liveChildren | `T@"NSArray",R,N` | 0xe2a4a48 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBookmarkTreeNode:` | `@24@0:8r^v16` | [0x43dfd40](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfd40.txt) | Calls NSObject init; replaces prior native optional, copies native pointer/control pair, increments non-null control count and engages self+0x18. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777bd0`; dobjc170.txt:1348 | medium-local; available; local_review |
| (base) | `-nodeType` | `q16@0:8` | [0x43dfe80](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfe80.txt) | Disengaged node returns 0; native type 1..5 is preserved, other values map to 0. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777bdc`; dobjc170.txt:1349 | medium-local; available; local_review |
| (base) | `-isFolder` | `B16@0:8` | [0x43dfea8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfea8.txt) | Returns zero for disengaged native node; otherwise low bit of native byte +0x2c. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777be8`; dobjc170.txt:1350 | medium-local; available; local_review |
| (base) | `-isUrl` | `B16@0:8` | [0x43dfec0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfec0.txt) | Returns zero for disengaged native node; otherwise low bit of native byte +0x2d. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777bf4`; dobjc170.txt:1351 | medium-local; available; local_review |
| (base) | `-nodeID` | `@16@0:8` | [0x43dfed8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dfed8.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x60a60,0x1c391bc | metadata `0xd777c00`; dobjc170.txt:1352 | medium-local; available; conversion_partial |
| (base) | `-uuid` | `@16@0:8` | [0x43dffa0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dffa0.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x60a60,0x1c391bc | metadata `0xd777c0c`; dobjc170.txt:1353 | medium-local; available; conversion_partial |
| (base) | `-parentNodeID` | `@16@0:8` | [0x43e0068](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0068.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x60a60,0x1c391bc | metadata `0xd777c18`; dobjc170.txt:1354 | medium-local; available; conversion_partial |
| (base) | `-index` | `i16@0:8` | [0x43e0194](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0194.txt) | Disengaged/missing-index state returns 32-bit all-ones; present index reads native +0x78 when native optional +0x80 engaged. Ghidra incorrectly renders pointer return. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777c24`; dobjc170.txt:1355 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-url` | `@16@0:8` | [0x43e01bc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e01bc.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x60a60, 0x43abd34. | 0x60a60,0x43abd34 | metadata `0xd777c30`; dobjc170.txt:1356 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-title` | `@16@0:8` | [0x43e02d0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e02d0.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd777c3c`; dobjc170.txt:1357 | medium-local; available; conversion_partial |
| (base) | `-faviconUrl` | `@16@0:8` | [0x43e03fc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e03fc.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x60a60, 0x43abd34. | 0x60a60,0x43abd34 | metadata `0xd777c48`; dobjc170.txt:1358 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-dateAdded` | `@16@0:8` | [0x43e0510](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0510.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: dateWithTimeIntervalSince1970:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777c54`; dobjc170.txt:1359 | low-unresolved; available; unresolved |
| (base) | `-dateFolderModified` | `@16@0:8` | [0x43e0564](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0564.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: dateWithTimeIntervalSince1970:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777c60`; dobjc170.txt:1360 | low-unresolved; available; unresolved |
| (base) | `-isValid` | `B16@0:8` | [0x43e05ac](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e05ac.txt) | Requires engaged native node then asks 0x42affa0 for validity. | 0x42affa0 | metadata `0xd777c6c`; dobjc170.txt:1361 | medium-partial; available; partial_review |
| (base) | `-liveChildren` | `@16@0:8` | [0x43e05dc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e05dc.txt) | Calls 0x42b00f4 for live children, wraps each as ArcBookmarkTreeNode, then copies the ObjC array. | 0x42b00f4 | metadata `0xd777c78`; dobjc170.txt:1362 | medium-partial; available; partial_review |
| (base) | `-node` | `{optional<arc::BookmarkTreeNode>=(?=c{BookmarkTreeNode={shared_ptr<arc::BookmarkTreeNode::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df53c.txt) | Copies optional native shared handle from self+8 into aggregate return storage, increments non-null control block; source engagement is at self+0x18. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777c84`; dobjc170.txt:1363 | medium-local; available; local_review |
| (base) | `-setNode:` | `v40@0:8{optional<arc::BookmarkTreeNode>=(?=c{BookmarkTreeNode={shared_ptr<arc::BookmarkTreeNode::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x43df57c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df57c.txt) | Replaces optional native shared handle at self+8, retaining/releasing control blocks as presence changes; engagement byte self+0x18. | 0x4331e20 | metadata `0xd777c90`; dobjc170.txt:1364 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df69c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777c9c`; dobjc170.txt:1365 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df760.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777ca8`; dobjc170.txt:1366 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Bookmark invalidation after parent model mutation/shutdown.
- Native node-type enum meanings.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Confirm signed exposed -1 sentinel in arm64 ABI.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Exact invalidation events and caller behavior after deletion.
- Ordering and mutation/invalid-node behavior in deeper bookmark service.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcBookmarkTreeNode.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcBookmarkTreeNode.md).
