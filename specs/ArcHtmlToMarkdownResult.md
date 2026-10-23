# ArcHtmlToMarkdownResult

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37ba38`, class RO `0xe2a3fd0`; raw otool `objc170.txt:22092`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Initialization copies body/title/references/selectorsApplied/metaTags and stores percentage double (0x43dd5d4). Getter IMP aliases must be interpreted with this class's layout: +8 body, +0x10 title, +0x18 references, +0x20 selectorsApplied, +0x28 percentage, +0x30 metaTags (0x43dd450, 0x420b5f8, 0x420cb48, 0x41615f8, 0x416804c, 0x420c3e0). Destructor clears five ObjC fields (0x43dd740). Copy does not prove deep copy of collection elements. No conversion engine, callback or thread guarantee is shown here. Preserve schema and nil/copy behavior; the HTML extraction/Markdown producer remains a separate route question.

The getter rows retain Ghidra’s objc_retainAutoreleaseReturnValue annotations; those object returns do not establish independent copies or ownership beyond normal return handling. See [revision notes](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/revision-evidence/notes.md) for the bounded annotation check.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _body | 8 | 8 | `@"NSString"` | 0xe2a3ea8 |
| _title | 16 | 8 | `@"NSString"` | 0xe2a3ec8 |
| _references | 24 | 8 | `@"NSArray"` | 0xe2a3ee8 |
| _selectorsApplied | 32 | 8 | `@"NSArray"` | 0xe2a3f08 |
| _selectorsContentPct | 40 | 8 | `d` | 0xe2a3f28 |
| _metaTags | 48 | 8 | `@"NSArray"` | 0xe2a3f48 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| body | `T@"NSString",R,C,N` | 0xe2a3f70 |
| title | `T@"NSString",R,C,N` | 0xe2a3f80 |
| references | `T@"NSArray",R,C,N` | 0xe2a3f90 |
| selectorsApplied | `T@"NSArray",R,C,N` | 0xe2a3fa0 |
| selectorsContentPct | `Td,R,N` | 0xe2a3fb0 |
| metaTags | `T@"NSArray",R,C,N` | 0xe2a3fc0 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBody:title:references:selectorsApplied:selectorsContentPct:metaTags:` | `@64@0:8@16@24@32@40d48@56` | [0x43dd5d4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dd5d4.txt) | Copies body, title, references, selectorsApplied and metaTags with ObjC copy; stores selectorsContentPct double at +0x28. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77780c`; dobjc170.txt:1257 | medium-local; available; local_review |
| (base) | `-body` | `@16@0:8` | [0x43dd450](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dd450.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. Runtime annotation identifies objc_retainAutoreleaseReturnValue; this is an object return with retain/autorelease handling, not a caller-owned independent copy. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777818`; dobjc170.txt:1258 | high-local; available; literal_verified |
| (base) | `-title` | `@16@0:8` | [0x420b5f8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0420b5f8.txt) | Literal return: *(ID *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. Runtime annotation identifies objc_retainAutoreleaseReturnValue; this is an object return with retain/autorelease handling, not a caller-owned independent copy. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777824`; dobjc170.txt:1259 | high-local; available; literal_verified |
| (base) | `-references` | `@16@0:8` | [0x420cb48](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0420cb48.txt) | Literal return: *(ID *)(param_1 + 0x18). Use this row metadata type; shared Ghidra name/type may differ. Runtime annotation identifies objc_retainAutoreleaseReturnValue; this is an object return with retain/autorelease handling, not a caller-owned independent copy. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777830`; dobjc170.txt:1260 | high-local; available; literal_verified |
| (base) | `-selectorsApplied` | `@16@0:8` | [0x41615f8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/041615f8.txt) | Literal return: *(ID *)(param_1 + 0x20). Use this row metadata type; shared Ghidra name/type may differ. Runtime annotation identifies objc_retainAutoreleaseReturnValue; this is an object return with retain/autorelease handling, not a caller-owned independent copy. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77783c`; dobjc170.txt:1261 | high-local; available; literal_verified |
| (base) | `-selectorsContentPct` | `d16@0:8` | [0x416804c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0416804c.txt) | Literal return: *(double *)(param_1 + 0x28). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777848`; dobjc170.txt:1262 | high-local; available; literal_verified |
| (base) | `-metaTags` | `@16@0:8` | [0x420c3e0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0420c3e0.txt) | Literal return: *(ID *)(param_1 + 0x30). Use this row metadata type; shared Ghidra name/type may differ. Runtime annotation identifies objc_retainAutoreleaseReturnValue; this is an object return with retain/autorelease handling, not a caller-owned independent copy. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777854`; dobjc170.txt:1263 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43dd740](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dd740.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777860`; dobjc170.txt:1264 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.


Original reviewed source: [ArcHtmlToMarkdownResult.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcHtmlToMarkdownResult.md).
