# ArcHistoryItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 12 declared identities. Superclass `NSObject`; class metadata `0xe37d338`, class RO `0xe2aaaa0`; raw otool `objc170.txt:30271`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Init retains optional native HistoryItem shared handle (0x442dfd8); cleanup/accessor share 0x43df69c/0x43df53c. historyID is formatted via 0xc248b4; URLs/title convert native strings (0x442e0fc, 0x442e1a0, 0x442e250, 0x442e318). Counts read native fields (0x442e3d8, 0x442e3f0). lastVisitTime converts native microsecond Windows epoch with saturation/special handling (epoch identification is source-backed inference from base/time/time.h:537-546 and the literal offset); visitTime can be absent (0x442e408, 0x442e4a8). No callback or thread policy was established. A history query producer and persistence semantics are outside this value wrapper, so neither Chrome-history identity nor CEF service parity is claimed.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _history_item | 8 | 24 | `{optional<arc::HistoryItem>=""(?="__null_state_"c"__val_"{HistoryItem="impl_"{shared_ptr<arc::HistoryItem::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2aa9e8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| history_item | `T{optional<arc::HistoryItem>=(?=c{HistoryItem={shared_ptr<arc::HistoryItem::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_history_item` | 0xe2aaa10 |
| historyID | `T@"NSString",R,C,N` | 0xe2aaa20 |
| url | `T@"NSURL",R,N` | 0xe2aaa30 |
| urlSpec | `T@"NSString",R,C,N` | 0xe2aaa40 |
| title | `T@"NSString",R,C,N` | 0xe2aaa50 |
| visitCount | `Ti,R,N` | 0xe2aaa60 |
| typedCount | `Ti,R,N` | 0xe2aaa70 |
| lastVisitTime | `T@"NSDate",R,N` | 0xe2aaa80 |
| visitTime | `T@"NSDate",R,N` | 0xe2aaa90 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithHistoryItem:` | `@24@0:8r^v16` | [0x442dfd8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442dfd8.txt) | Calls NSObject init; copies native shared handle, increments control count and engages self+0x18 after releasing prior optional. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77aebc`; dobjc170.txt:2420 | medium-local; available; local_review |
| (base) | `-historyID` | `@16@0:8` | [0x442e0fc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e0fc.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0xc248b4,0x1c391bc | metadata `0xd77aec8`; dobjc170.txt:2421 | medium-local; available; conversion_partial |
| (base) | `-url` | `@16@0:8` | [0x442e1a0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e1a0.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x7cf864, 0x60a60, 0x43abd34. | 0x7cf864,0x60a60,0x43abd34 | metadata `0xd77aed4`; dobjc170.txt:2422 | low-unresolved; available; unresolved |
| (base) | `-urlSpec` | `@16@0:8` | [0x442e250](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e250.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x7cf864,0x60a60,0x1c391bc | metadata `0xd77aee0`; dobjc170.txt:2423 | medium-local; available; conversion_partial |
| (base) | `-title` | `@16@0:8` | [0x442e318](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e318.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77aeec`; dobjc170.txt:2424 | medium-local; available; conversion_partial |
| (base) | `-visitCount` | `i16@0:8` | [0x442e3d8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e3d8.txt) | Observed return expression: *(int *)(*(long *)(param_1 + 8) + 0xa0). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77aef8`; dobjc170.txt:2425 | medium-local; available; literal_partial |
| (base) | `-typedCount` | `i16@0:8` | [0x442e3f0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e3f0.txt) | Observed return expression: *(int *)(*(long *)(param_1 + 8) + 0xa4). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af04`; dobjc170.txt:2426 | medium-local; available; literal_partial |
| (base) | `-lastVisitTime` | `@16@0:8` | [0x442e408](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e408.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: dateWithTimeIntervalSince1970:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af10`; dobjc170.txt:2427 | low-unresolved; available; unresolved |
| (base) | `-visitTime` | `@16@0:8` | [0x442e4a8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442e4a8.txt) | Behavior unresolved beyond local wrapper evidence. Observed selector operands: dateWithTimeIntervalSince1970:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af1c`; dobjc170.txt:2428 | low-unresolved; available; unresolved |
| (base) | `-history_item` | `{optional<arc::HistoryItem>=(?=c{HistoryItem={shared_ptr<arc::HistoryItem::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df53c.txt) | Copies optional native shared handle from self+8 into aggregate return storage, increments non-null control block; source engagement is at self+0x18. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af28`; dobjc170.txt:2429 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df69c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af34`; dobjc170.txt:2430 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043df760.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77af40`; dobjc170.txt:2431 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Whether query result is stable snapshot or mutable shared backend.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcHistoryItem.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcHistoryItem.md).
