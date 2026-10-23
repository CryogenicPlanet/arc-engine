# ArcKeyboardAccelerator

ArcCore **1.170.0 arm64**, unslid engine VAs. 3 declared identities. Superclass `NSObject`; class metadata `0xe37c4d8`, class RO `0xe2a7350`; raw otool `objc170.txt:26014`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init calls NSObject init and stores native accelerator fields (0x4405fc8). key and modifiers share raw field-load IMPs with unrelated classes: self+8 and self+0x10 respectively (0x2d0fb40, 0x2cb140). This class metadata determines their actual scalar encodings, not Ghidra's ID return type. No owned ObjC field cleanup, callbacks or thread semantics were established. Key enum and modifier-mask conversion at menu/keyboard consumers remain the route questions; these methods alone do not define native event synthesis.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _key | 8 | 8 | `Q` | 0xe2a72e8 |
| _modifiers | 16 | 8 | `Q` | 0xe2a7308 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| key | `TQ,R,N,V_key` | 0xe2a7330 |
| modifiers | `TQ,R,N,V_modifiers` | 0xe2a7340 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithAccelerator:` | `@24@0:8r^{KeyboardAccelerator=Ii}16` | [0x4405fc8](../bodies/04405fc8.txt) | Calls NSObject init; widens native key to stored 64-bit scalar at +8, sign-extends native modifier int to +0x10. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77947c`; dobjc170.txt:1830 | medium-local; available; local_review |
| (base) | `-key` | `Q16@0:8` | [0x2d0fb40](../bodies/02d0fb40.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779488`; dobjc170.txt:1831 | high-local; available; literal_verified |
| (base) | `-modifiers` | `Q16@0:8` | [0x2cb140](../bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779494`; dobjc170.txt:1832 | high-local; available; literal_verified |

Unresolved engineering questions (method-specific evidence above):

- Key enum and modifier-mask mapping at consumers.
