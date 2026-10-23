# ArcOverscrollBehavior

ArcCore **1.170.0 arm64**, unslid engine VAs. 5 declared identities. Superclass `NSObject`; class metadata `0xe37c7f8`, class RO `0xe2a7f28`; raw otool `objc170.txt:26901`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

init calls NSObject init then stores both directional values as 0 (0x440a7b4). x/y are scalar loads at +8/+0x10 and setters directly replace them (0x2d0fb40, 0x2cb140, 0x440a7f0, 0x416803c). Shared IMP Ghidra names/types are not this class's API types. No callbacks, persistence or thread policy was established. Meaning of enum 0 and conversion to renderer/native overscroll policy must be recovered at the consuming method; no CEF mapping is asserted from these scalar fields.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _x | 8 | 8 | `q` | 0xe2a7ec0 |
| _y | 16 | 8 | `q` | 0xe2a7ee0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| x | `Tq,N,V_x` | 0xe2a7f08 |
| y | `Tq,N,V_y` | 0xe2a7f18 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x440a7b4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0440a7b4.txt) | Calls NSObject init; initializes x and y scalar fields to zero. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779990`; dobjc170.txt:1948 | medium-local; available; local_review |
| (base) | `-x` | `q16@0:8` | [0x2d0fb40](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/02d0fb40.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77999c`; dobjc170.txt:1949 | high-local; available; literal_verified |
| (base) | `-setX:` | `v24@0:8q16` | [0x440a7f0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0440a7f0.txt) | Literal store: *(long_long *)(param_1 + 8) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799a8`; dobjc170.txt:1950 | high-local; available; literal_verified |
| (base) | `-y` | `q16@0:8` | [0x2cb140](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/002cb140.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799b4`; dobjc170.txt:1951 | high-local; available; literal_verified |
| (base) | `-setY:` | `v24@0:8q16` | [0x416803c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0416803c.txt) | Literal store: *(void **)(param_1 + 0x10) = param_3; | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7799c0`; dobjc170.txt:1952 | high-local; available; literal_verified |

Unresolved engineering questions (method-specific evidence above):

- Overscroll enum meanings.


Original reviewed source: [ArcOverscrollBehavior.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcOverscrollBehavior.md).
