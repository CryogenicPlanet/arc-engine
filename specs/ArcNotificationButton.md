# ArcNotificationButton

ArcCore **1.170.0 arm64**, unslid engine VAs. 9 declared identities. Superclass `NSObject`; class metadata `0xe37c780`, class RO `0xe2a7bb8`; raw otool `objc170.txt:26602`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Stored fields title/icon/placeholder are returned from +0x10/+0x18/+0x20 (0x2cb140, 0x4084dc0, 0x4060580). Setters use objc_storeStrong, not copy (0x420b588, 0x4401164, 0x4408ac4); isTextInput is scalar +8 (0x40b1230, 0x43e0930). Destructor clears three object fields (0x4408ad0). ArcNotification creates/fills these wrappers in buttons (0x4409688). No callback/queue contract was established. Preserve this data transport and distinguish mutable object retention from copy; platform action parsing remains in the notification class.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _isTextInput | 8 | 1 | `B` | 0xe2a7af0 |
| _title | 16 | 8 | `@"NSString"` | 0xe2a7b10 |
| _icon | 24 | 8 | `@"NSImage"` | 0xe2a7b30 |
| _placeholder | 32 | 8 | `@"NSString"` | 0xe2a7b50 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| title | `T@"NSString",&,N,V_title` | 0xe2a7b78 |
| icon | `T@"NSImage",&,N,V_icon` | 0xe2a7b88 |
| placeholder | `T@"NSString",&,N,V_placeholder` | 0xe2a7b98 |
| isTextInput | `TB,N,V_isTextInput` | 0xe2a7ba8 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-title` | `@16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77977c`; dobjc170.txt:1901 | high-local; available; literal_verified |
| (base) | `-setTitle:` | `v24@0:8@16` | [0x420b588](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0420b588.txt) | Literal store: *(ID *)(param_1 + 0x10) = param_3; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779788`; dobjc170.txt:1902 | high-local; available; literal_verified |
| (base) | `-icon` | `@16@0:8` | [0x4084dc0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04084dc0.txt) | Literal return: *(ID *)(param_1 + 0x18). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779794`; dobjc170.txt:1903 | high-local; available; literal_verified |
| (base) | `-setIcon:` | `v24@0:8@16` | [0x4401164](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04401164.txt) | Literal store: *(ID *)(param_1 + 0x18) = param_3; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797a0`; dobjc170.txt:1904 | high-local; available; literal_verified |
| (base) | `-placeholder` | `@16@0:8` | [0x4060580](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04060580.txt) | Literal return: *(ID *)(param_1 + 0x20). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797ac`; dobjc170.txt:1905 | high-local; available; literal_verified |
| (base) | `-setPlaceholder:` | `v24@0:8@16` | [0x4408ac4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04408ac4.txt) | Literal store: *(ID *)(param_1 + 0x20) = param_3; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797b8`; dobjc170.txt:1906 | high-local; available; literal_verified |
| (base) | `-isTextInput` | `B16@0:8` | [0x40b1230](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/040b1230.txt) | Pseudocode loses the return value under incorrect void prototype. Pristine arm64 leaf: ldrb w0, [x0, #8]; ret. Field identity/type independently checked against this class metadata. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797c4`; dobjc170.txt:1907 | high-local; available_malformed_signature_recovered_leaf; instruction_verified |
| (base) | `-setIsTextInput:` | `v20@0:8B16` | [0x43e0930](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e0930.txt) | Literal store: *(char *)(param_1 + 8) = (char)param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797d0`; dobjc170.txt:1908 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4408ad0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04408ad0.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797dc`; dobjc170.txt:1909 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.


Original reviewed source: [ArcNotificationButton.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcNotificationButton.md).
