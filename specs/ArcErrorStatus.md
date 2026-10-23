# ArcErrorStatus

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37df18`, class RO `0xe2ad840`; raw otool `objc170.txt:33906`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

An eight-byte inline native value is copied in initialization/getter/setter (0x443ba3c, 0x443ba90, 0x443ba9c); two 32-bit fields back processTerminationStatus and crashedErrorCode (0x443ba88, 0x189a538). Lifecycle IMP 0x15ac0 is a single ret: .cxx_construct returns self with x0 unchanged and performs no stores or zeroing; .cxx_destruct has void semantics and performs no cleanup. Ghidra misrepresents aggregate return state at 0x443ba90, so metadata remains the ABI anchor. No callbacks/queue or ownership of external resources were established. Preserve value bits and independently resolve status/error enums at the producer; names alone cannot define crash handling.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _error_status | 8 | 8 | `{ErrorStatus="process_termination_status"i"crashed_error_code"i}` | 0xe2ad7e8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| error_status | `T{ErrorStatus=ii},N,V_error_status` | 0xe2ad810 |
| processTerminationStatus | `Tq,R,N` | 0xe2ad820 |
| crashedErrorCode | `Ti,R,N` | 0xe2ad830 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithErrorStatus:` | `@24@0:8r^{ErrorStatus=ii}16` | [0x443ba3c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443ba3c.txt) | Calls NSObject init then copies eight-byte native error status into self+8. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4bc`; dobjc170.txt:2940 | medium-local; available; local_review |
| (base) | `-processTerminationStatus` | `q16@0:8` | [0x443ba88](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443ba88.txt) | Literal return: (long)*(int *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4c8`; dobjc170.txt:2941 | high-local; available; literal_verified |
| (base) | `-crashedErrorCode` | `i16@0:8` | [0x189a538](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0189a538.txt) | Literal return: *(unsigned_int *)(param_1 + 0xc). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4d4`; dobjc170.txt:2942 | high-local; available; literal_verified |
| (base) | `-error_status` | `{ErrorStatus=ii}16@0:8` | [0x443ba90](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443ba90.txt) | Copies eight bytes self+8 into aggregate result storage; Ghidra nominal return type is degraded. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4e0`; dobjc170.txt:2943 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-setError_status:` | `v24@0:8{ErrorStatus=ii}16` | [0x443ba9c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443ba9c.txt) | Literal store: *(undefined8 *)(param_1 + 8) = *(undefined8 *)param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4ec`; dobjc170.txt:2944 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x15ac0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/00015ac0.txt) | No-op destructor: the exact arm64 body is a single ret, with no stores or cleanup calls. This declaration has void return semantics; unchanged x0 does not create an object result. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c4f8`; dobjc170.txt:2945 | high-local; available; instruction_verified |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x15ac0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/00015ac0.txt) | No-op initializer: the exact arm64 body is ret with x0 unchanged, returning self under this method encoding. It performs no stores, zeroing or field initialization. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c504`; dobjc170.txt:2946 | high-local; available; instruction_verified |

Unresolved engineering questions (method-specific evidence above):

- Status/error enum meanings are defined by producer, not this copy.
- Aggregate ABI confirmation.


Original reviewed source: [ArcErrorStatus.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcErrorStatus.md).
