# ArcOverscrollParams

ArcCore **1.170.0 arm64**, unslid engine VAs. 4 declared identities. Superclass `NSObject`; class metadata `0xe37c848`, class RO `0xe2a7ff8`; raw otool `objc170.txt:26970`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

init allocates ArcOverscrollBehavior and stores it strongly (0x440a7f8). Getter/setter/destructor access/replace/release the object field at +8 (0x2d0fb40, 0x43eae2c, 0x4166068). This retains the supplied behavior object, with no copy shown. No callbacks or thread policy was established. The consuming web-contents/renderer bridge, enum meanings and effect of mutating the shared behavior after application remain open route questions.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _overscrollBehavior | 8 | 8 | `@"ArcOverscrollBehavior"` | 0xe2a7fc0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| overscrollBehavior | `T@"ArcOverscrollBehavior",&,N,V_overscrollBehavior` | 0xe2a7fe8 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x440a7f8](../bodies/0440a7f8.txt) | Calls NSObject init; creates ArcOverscrollBehavior and replaces strong self+8 field. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799d4`; dobjc170.txt:1955 | medium-local; available; local_review |
| (base) | `-overscrollBehavior` | `@16@0:8` | [0x2d0fb40](../bodies/02d0fb40.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799e0`; dobjc170.txt:1956 | high-local; available; literal_verified |
| (base) | `-setOverscrollBehavior:` | `v24@0:8@16` | [0x43eae2c](../bodies/043eae2c.txt) | Literal store: *(ID *)(param_1 + 8) = param_3; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799ec`; dobjc170.txt:1957 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4166068](../bodies/04166068.txt) | Literal store: *(undefined8 *)(param_1 + 8) = 0; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799f8`; dobjc170.txt:1958 | high-local; available; literal_verified |

Unresolved engineering questions (method-specific evidence above):

- Behavior effect and mutation semantics at consumer.
