# ArcAIDistilBertClassifier

ArcCore **1.170.0 arm64**, unslid engine VAs. 11 declared identities. Superclass `NSObject`; class metadata `0xe37dc48`, class RO `0xe2acf30`; raw otool `objc170.txt:33194`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

This is configuration/value transport, not a classifier execution entry point in the reviewed methods. The five-argument initializer converts four strings, substitutes empty strings for nil, stores numClasses narrowed to short, and engages optional storage (0x443a46c). Getters expose string fields and signed 16-bit class count (0x443a764, 0x443a794, 0x443a7c4, 0x443a7f4, 0x443a824). mutableAIDistilBertClassifier exposes self+8 only when engaged (0x443a450). Native copy and cleanup helpers are 0x443a9fc and 0x42a4a04 as reached by 0x443a6f0/0x443a89c; their full semantics were not traced. No callback/queue contract is established. Replacement work is preserving value conversion, optional presence and narrowing; actual model-loading service behavior remains outside this class review.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `120`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _ai_distil_bert_classifier | 8 | 112 | `{optional<arc::AIDistilBertClassifier>=""(?="__null_state_"c"__val_"{AIDistilBertClassifier="model_file_path"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"lora_config_json"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"config_file_path"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"classifier_head_file_path"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"num_classes"s})"__engaged_"B}` | 0xe2acea8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| ai_distil_bert_classifier | `T{optional<arc::AIDistilBertClassifier>=(?=c{AIDistilBertClassifier={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}s})B},R,N,V_ai_distil_bert_classifier` | 0xe2aced0 |
| modelFilePath | `T@"NSString",R,C,N` | 0xe2acee0 |
| loraConfigJSON | `T@"NSString",R,C,N` | 0xe2acef0 |
| configFilePath | `T@"NSString",R,C,N` | 0xe2acf00 |
| classifierHeadFilePath | `T@"NSString",R,C,N` | 0xe2acf10 |
| numClasses | `Ti,R,N` | 0xe2acf20 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableAIDistilBertClassifier` | `^v16@0:8` | [0x443a450](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a450.txt) | Observed return expression: (void *)(param_1 + 8). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c0e4`; dobjc170.txt:2846 | medium-local; available; literal_partial |
| (base) | `-init:loraConfigJSON:configFilePath:classifierHeadFilePath:numClasses:` | `@52@0:8@16@24@32@40i48` | [0x443a46c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a46c.txt) | Creates engaged native value holding four converted strings and 16-bit numClasses at +0x68; nil strings become empty. No model file is loaded here. | 0x42a4a04,0x1b356b8 | metadata `0xd77c0f0`; dobjc170.txt:2847 | medium-local; available; local_review |
| (base) | `-initWithAIDistilBertClassifier:` | `@24@0:8r^v16` | [0x443a6f0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a6f0.txt) | Calls NSObject init, destroys prior engaged classifier through 0x42a4a04, copies input via 0x443a9fc and engages +0x70. | 0x42a4a04,0x443a9fc | metadata `0xd77c0fc`; dobjc170.txt:2848 | medium-partial; available; partial_review |
| (base) | `-modelFilePath` | `@16@0:8` | [0x443a764](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a764.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77c108`; dobjc170.txt:2849 | medium-local; available; conversion_partial |
| (base) | `-loraConfigJSON` | `@16@0:8` | [0x443a794](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a794.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77c114`; dobjc170.txt:2850 | medium-local; available; conversion_partial |
| (base) | `-configFilePath` | `@16@0:8` | [0x443a7c4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a7c4.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77c120`; dobjc170.txt:2851 | medium-local; available; conversion_partial |
| (base) | `-classifierHeadFilePath` | `@16@0:8` | [0x443a7f4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a7f4.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77c12c`; dobjc170.txt:2852 | medium-local; available; conversion_partial |
| (base) | `-numClasses` | `i16@0:8` | [0x443a824](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a824.txt) | Requires engaged classifier then reads signed short numClasses at +0x68, returning int. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c138`; dobjc170.txt:2853 | medium-local; available; local_review |
| (base) | `-ai_distil_bert_classifier` | `{optional<arc::AIDistilBertClassifier>=(?=c{AIDistilBertClassifier={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}s})B}16@0:8` | [0x443a838](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a838.txt) | Copies optional native classifier via 0x443a9fc, preserving engagement in aggregate return storage. | 0x443a9fc | metadata `0xd77c144`; dobjc170.txt:2854 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x443a89c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a89c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | 0x42a4a04 | metadata `0xd77c150`; dobjc170.txt:2855 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x443a8b0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443a8b0.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c15c`; dobjc170.txt:2856 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Validate numClasses narrowing/validation and later AI service loading behavior.
- Native string-copy ABI and class-count invariants.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Constructor narrowing for negative or >32767 input.
- By-value aggregate ABI and native copy helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcAIDistilBertClassifier.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcAIDistilBertClassifier.md).
