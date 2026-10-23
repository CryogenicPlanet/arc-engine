# ArcTabGroupResult

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37bd08`, class RO `0xe2a4bb8`; raw otool `objc170.txt:22973`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Initializer calls NSObject init and establishes scalar defaults (0x43e08e4). success and extensionGroupId are direct scalar fields; error/groupId setters use nonatomic copy (0x43e0930, 0x4168044, 0x43e0938, 0x420b594). Destructor clears two ObjC string fields (0x43e0940). It transports operation result rather than exposing a callback implementation in the reviewed methods. No threading rule was established. Error-string vocabulary and native/extension group-ID correspondence must come from the producer before implementing tab-group command parity.

The error/groupId setter rows explicitly retain objc_setProperty_nonatomic_copy annotations erased in the C assignments. See [revision notes](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/revision-evidence/notes.md) for the bounded annotation check.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _success | 8 | 1 | `B` | 0xe2a4af0 |
| _error | 16 | 8 | `@"NSString"` | 0xe2a4b10 |
| _groupId | 24 | 8 | `@"NSString"` | 0xe2a4b30 |
| _extensionGroupId | 32 | 8 | `q` | 0xe2a4b50 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| success | `TB,N,V_success` | 0xe2a4b78 |
| error | `T@"NSString",C,N,V_error` | 0xe2a4b88 |
| groupId | `T@"NSString",C,N,V_groupId` | 0xe2a4b98 |
| extensionGroupId | `Tq,N,V_extensionGroupId` | 0xe2a4ba8 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x43e08e4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e08e4.txt) | Calls NSObject init; success=false at +8 and extensionGroupId=-1 at +0x20. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777cbc`; dobjc170.txt:1369 | medium-local; available; local_review |
| (base) | `-success` | `B16@0:8` | [0x40b1230](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/040b1230.txt) | Pseudocode loses the return value under incorrect void prototype. Pristine arm64 leaf: ldrb w0, [x0, #8]; ret. Field identity/type independently checked against this class metadata. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777cc8`; dobjc170.txt:1370 | high-local; available_malformed_signature_recovered_leaf; instruction_verified |
| (base) | `-setSuccess:` | `v20@0:8B16` | [0x43e0930](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0930.txt) | Literal store: *(char *)(param_1 + 8) = (char)param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777cd4`; dobjc170.txt:1371 | high-local; available; literal_verified |
| (base) | `-error` | `@16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777ce0`; dobjc170.txt:1372 | high-local; available; literal_verified |
| (base) | `-setError:` | `v24@0:8@16` | [0x43e0938](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0938.txt) | Calls objc_setProperty_nonatomic_copy for the field shown by the decompiler assignment. This setter copies the supplied object; no deep copy of collection elements is established. Literal store: *(ID *)(param_1 + 0x10) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777cec`; dobjc170.txt:1373 | high-local; available; literal_verified |
| (base) | `-groupId` | `@16@0:8` | [0x4084dc0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04084dc0.txt) | Literal return: *(ID *)(param_1 + 0x18). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777cf8`; dobjc170.txt:1374 | high-local; available; literal_verified |
| (base) | `-setGroupId:` | `v24@0:8@16` | [0x420b594](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0420b594.txt) | Calls objc_setProperty_nonatomic_copy for the field shown by the decompiler assignment. This setter copies the supplied object; no deep copy of collection elements is established. Literal store: *(ID *)(param_1 + 0x18) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d04`; dobjc170.txt:1375 | high-local; available; literal_verified |
| (base) | `-extensionGroupId` | `q16@0:8` | [0x4060580](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04060580.txt) | Literal return: *(ID *)(param_1 + 0x20). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d10`; dobjc170.txt:1376 | high-local; available; literal_verified |
| (base) | `-setExtensionGroupId:` | `v24@0:8q16` | [0x4168044](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04168044.txt) | Literal store: *(long_long *)(param_1 + 0x20) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d1c`; dobjc170.txt:1377 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43e0940](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0940.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d28`; dobjc170.txt:1378 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Producer error/group string defaults and meaning of extension-ID sentinel.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.


Original reviewed source: [ArcTabGroupResult.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcTabGroupResult.md).
