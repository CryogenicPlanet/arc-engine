# ArcMemoryDumpProvider

ArcCore **1.170.0 arm64**, unslid engine VAs. 4 declared identities. Superclass `NSObject`; class metadata `0xe37c618`, class RO `0xe2a7728`; raw otool `objc170.txt:26322`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

initWithName:dataProvider: retains block and creates native RAII provider (0x44077b4 → 0x432b4b8). Bridge prefixes provider name with bcny- and checks runtime state; identification of TLS/task-runner-looking state as the registration task runner is a source-backed inference. Callback adapter 0x4407b5c creates a completion block around the native dump callback and invokes the ObjC provider block. dealloc clears owned pointer, unregisters/destroys provider and then calls super; .cxx_destruct has a second null-guarded cleanup path (0x4407a04, 0x4407ac4). Do not translate both into unconditional duplicate destruction.

Candidate source identification is MemoryDumpManager registration/unregistration, supported by lifecycle shape and pinned base/trace_event/memory_dump_manager.h:80-106: manager does not own provider; task runner determines callback affinity. This source comparison does not prove which registration overload/sequence Arc uses. Outstanding completion lifetime, destruction sequence and whether callback can arrive after dealloc remain unresolved. A CEF route would need separate diagnostics integration; no public API-absence claim is made.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _cppProvider | 8 | 8 | `{unique_ptr<arc::MemoryDumpProvider, std::default_delete<arc::MemoryDumpProvider>>="__ptr_"^{MemoryDumpProvider}}` | 0xe2a7708 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| None declared | — | — |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithName:dataProvider:` | `@32@0:8@16@?24` | [0x44077b4](../bodies/044077b4.txt) | Converts name, captures data-provider block and installs RAII provider created by 0x432b4b8. Bridge prefixes name with bcny-; registration/current-task-runner identification is a source-backed inference from the TLS and lifecycle call pattern. | 0x1b356b8,0x432b4b8,0xf80684,0x1e3d3f8 | metadata `0xd7795f8`; dobjc170.txt:1867 | medium-partial; available; partial_review |
| (base) | `-dealloc` | `v16@0:8` | [0x4407a04](../bodies/04407a04.txt) | Clears owned provider pointer before unregister/destruction through 0x1e3d3f8, destroys callable/name storage, then calls NSObject dealloc. | 0xf80684,0x1e3d3f8 | metadata `0xd779604`; dobjc170.txt:1868 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4407ac4](../bodies/04407ac4.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | 0xf80684,0x1e3d3f8 | metadata `0xd779610`; dobjc170.txt:1869 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x407ab00](../bodies/0407ab00.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77961c`; dobjc170.txt:1870 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Exact registration sequence/task runner and asynchronous data-provider completion lifetime.
- Outstanding dump completion races and required destruction sequence.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
