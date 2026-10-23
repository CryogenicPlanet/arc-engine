# ArcNomicEmbedTextItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37d1f8`, class RO `0xe2aa690`; raw otool `objc170.txt:29991`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Two native C++ strings represent text and task; init converts nil inputs to empty strings (0x442d568). Native-copy init copies both strings (0x442d6f8); getters convert them through 0x1c391bc (0x442d10c, 0x442d7fc), and native getter/destructor share IMPs with PasteAndGoClassification because layouts match (0x442d150, 0x442d244). Metadata preserves distinct aggregate identities. mutableNomicEmbedTextItem returns engaged storage address (0x442d54c). No model execution, callbacks or threading policy was established. The engineering task here is value marshalling and lifetime; embedding task vocabulary and inference service are separate questions.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _nomic_embed_text_item | 8 | 56 | `{optional<arc::NomicEmbedTextItem>=""(?="__null_state_"c"__val_"{NomicEmbedTextItem="text"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"task"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}})"__engaged_"B}` | 0xe2aa638 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| nomic_embed_text_item | `T{optional<arc::NomicEmbedTextItem>=(?=c{NomicEmbedTextItem={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B},R,N,V_nomic_embed_text_item` | 0xe2aa660 |
| text | `T@"NSString",R,C,N` | 0xe2aa670 |
| task | `T@"NSString",R,C,N` | 0xe2aa680 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableNomicEmbedTextItem` | `^v16@0:8` | [0x442d54c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d54c.txt) | Observed return expression: (void *)(param_1 + 8). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ad64`; dobjc170.txt:2386 | medium-local; available; literal_partial |
| (base) | `-init:task:` | `@32@0:8@16@24` | [0x442d568](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d568.txt) | Creates engaged two-string native value; nil text/task become empty strings; non-nil values converted through 0x1b356b8. | 0x1b356b8 | metadata `0xd77ad70`; dobjc170.txt:2387 | medium-local; available; local_review |
| (base) | `-initWithNomicEmbedTextItem:` | `@24@0:8r^v16` | [0x442d6f8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d6f8.txt) | Calls NSObject init; copies both native strings into inline optional and engages +0x38, releasing prior long-string storage. | 0x60a60 | metadata `0xd77ad7c`; dobjc170.txt:2388 | medium-local; available; local_review |
| (base) | `-text` | `@16@0:8` | [0x442d10c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d10c.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77ad88`; dobjc170.txt:2389 | medium-local; available; conversion_partial |
| (base) | `-task` | `@16@0:8` | [0x442d7fc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d7fc.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77ad94`; dobjc170.txt:2390 | medium-local; available; conversion_partial |
| (base) | `-nomic_embed_text_item` | `{optional<arc::NomicEmbedTextItem>=(?=c{NomicEmbedTextItem={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B}16@0:8` | [0x442d150](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d150.txt) | Copies two native C++ strings from optional self+8 to aggregate return storage, preserving engagement at self+0x38; shared IMP display type is unrelated. | 0x60a60 | metadata `0xd77ada0`; dobjc170.txt:2391 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442d244](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d244.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77adac`; dobjc170.txt:2392 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442d2a0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442d2a0.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77adb8`; dobjc170.txt:2393 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- No embedding inference is run by this value constructor.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Aggregate ABI must use this row encoding.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcNomicEmbedTextItem.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcNomicEmbedTextItem.md).
