# ArcTabGroupInfo

ArcCore **1.170.0 arm64**, unslid engine VAs. 14 declared identities. Superclass `NSObject`; class metadata `0xe37bd58`, class RO `0xe2a4d78`; raw otool `objc170.txt:23072`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initializer establishes defaults and an ObjC tab-session-ID collection (0x43e0970); groupId/title/tabSessionIds setters use nonatomic copy (0x43e0938, 0x43e09fc, 0x43e0a0c), while extensionGroupId/color/collapsed are scalar fields (0x43e09f4, 0x43e0a04, 0x43e0930). Destructor releases stored collections/strings (0x43e0a14). Shared field-load IMPs have different scalar/object types on other classes; this metadata is authoritative. No callback/queue or persistence was established. Replacement needs producer-defined defaults, color enum and session-ID ordering; live Chrome tab-group service integration remains outside this value class.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _collapsed | 8 | 1 | `B` | 0xe2a4c50 |
| _groupId | 16 | 8 | `@"NSString"` | 0xe2a4c70 |
| _extensionGroupId | 24 | 8 | `q` | 0xe2a4c90 |
| _title | 32 | 8 | `@"NSString"` | 0xe2a4cb0 |
| _color | 40 | 8 | `q` | 0xe2a4cd0 |
| _tabSessionIds | 48 | 8 | `@"NSArray"` | 0xe2a4cf0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| groupId | `T@"NSString",C,N,V_groupId` | 0xe2a4d18 |
| extensionGroupId | `Tq,N,V_extensionGroupId` | 0xe2a4d28 |
| title | `T@"NSString",C,N,V_title` | 0xe2a4d38 |
| color | `Tq,N,V_color` | 0xe2a4d48 |
| collapsed | `TB,N,V_collapsed` | 0xe2a4d58 |
| tabSessionIds | `T@"NSArray",C,N,V_tabSessionIds` | 0xe2a4d68 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x43e0970](../bodies/043e0970.txt) | Calls NSObject init; sets empty group/title strings, extensionGroupId=-1, collapsed=false, color=0 and empty tabSessionIds array. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d3c`; dobjc170.txt:1381 | medium-local; available; local_review |
| (base) | `-groupId` | `@16@0:8` | [0x2cb140](../bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d48`; dobjc170.txt:1382 | high-local; available; literal_verified |
| (base) | `-setGroupId:` | `v24@0:8@16` | [0x43e0938](../bodies/043e0938.txt) | Literal store: *(ID *)(param_1 + 0x10) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d54`; dobjc170.txt:1383 | high-local; available; literal_verified |
| (base) | `-extensionGroupId` | `q16@0:8` | [0x4084dc0](../bodies/04084dc0.txt) | Literal return: *(ID *)(param_1 + 0x18). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d60`; dobjc170.txt:1384 | high-local; available; literal_verified |
| (base) | `-setExtensionGroupId:` | `v24@0:8q16` | [0x43e09f4](../bodies/043e09f4.txt) | Literal store: *(long_long *)(param_1 + 0x18) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d6c`; dobjc170.txt:1385 | high-local; available; literal_verified |
| (base) | `-title` | `@16@0:8` | [0x4060580](../bodies/04060580.txt) | Literal return: *(ID *)(param_1 + 0x20). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d78`; dobjc170.txt:1386 | high-local; available; literal_verified |
| (base) | `-setTitle:` | `v24@0:8@16` | [0x43e09fc](../bodies/043e09fc.txt) | Literal store: *(ID *)(param_1 + 0x20) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d84`; dobjc170.txt:1387 | high-local; available; literal_verified |
| (base) | `-color` | `q16@0:8` | [0x133ae90](../bodies/0133ae90.txt) | Literal return: *(unsigned_long_long *)(param_1 + 0x28). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d90`; dobjc170.txt:1388 | high-local; available; literal_verified |
| (base) | `-setColor:` | `v24@0:8q16` | [0x43e0a04](../bodies/043e0a04.txt) | Literal store: *(unsigned_long_long *)(param_1 + 0x28) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777d9c`; dobjc170.txt:1389 | high-local; available; literal_verified |
| (base) | `-collapsed` | `B16@0:8` | [0x40b1230](../bodies/040b1230.txt) | Pseudocode loses the return value under incorrect void prototype. Pristine arm64 leaf: ldrb w0, [x0, #8]; ret. Field identity/type independently checked against this class metadata. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777da8`; dobjc170.txt:1390 | high-local; available_malformed_signature_recovered_leaf; instruction_verified |
| (base) | `-setCollapsed:` | `v20@0:8B16` | [0x43e0930](../bodies/043e0930.txt) | Literal store: *(char *)(param_1 + 8) = (char)param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777db4`; dobjc170.txt:1391 | high-local; available; literal_verified |
| (base) | `-tabSessionIds` | `@16@0:8` | [0x2131e0](../bodies/002131e0.txt) | Literal return: *(long_long *)(param_1 + 0x30). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777dc0`; dobjc170.txt:1392 | high-local; available; literal_verified |
| (base) | `-setTabSessionIds:` | `v24@0:8@16` | [0x43e0a0c](../bodies/043e0a0c.txt) | Literal store: *(ID *)(param_1 + 0x30) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777dcc`; dobjc170.txt:1393 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43e0a14](../bodies/043e0a14.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777dd8`; dobjc170.txt:1394 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Color enum meanings and extension-ID sentinel.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
