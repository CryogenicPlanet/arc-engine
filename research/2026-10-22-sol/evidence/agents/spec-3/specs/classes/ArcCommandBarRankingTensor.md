# ArcCommandBarRankingTensor

ArcCore **1.170.0 arm64**, unslid engine VAs. 9 declared identities. Superclass `NSObject`; class metadata `0xe37d658`, class RO `0xe2ab830`; raw otool `objc170.txt:31392`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline optional native tensor stores name, integer dimensions and float payload. Init converts shape NSNumber values with intValue and data via 0x443181c (0x4431cec); shape/data allocate boxed numeric arrays (0x4432208, 0x44322c8). Native copy and cleanup reach 0x42a5760/0x42a44a8 (0x4432164, 0x44323e4); mutableCommandBarRankingTensor exposes engaged storage (0x4431cd0). No inference execution, callback or thread contract was established in these methods. Replacement must preserve element widths and array ordering; consistency of shape product versus data length, narrowing and consumers of tensor memory remain open.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `88`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _command_bar_ranking_tensor | 8 | 80 | `{optional<arc::CommandBarRankingTensor>=""(?="__null_state_"c"__val_"{CommandBarRankingTensor="name"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"shape"{vector<int, std::allocator<int>>="__layout_"{__vector_layout<int, std::allocator<int>>="__begin_"^i"__size_"Q"__capacity_"Q}}"data"{vector<float, std::allocator<float>>="__layout_"{__vector_layout<float, std::allocator<float>>="__begin_"^f"__size_"Q"__capacity_"Q}}})"__engaged_"B}` | 0xe2ab7c8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| command_bar_ranking_tensor | `T{optional<arc::CommandBarRankingTensor>=(?=c{CommandBarRankingTensor={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{vector<int, std::allocator<int>>={__vector_layout<int, std::allocator<int>>=^iQQ}}{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B},R,N,V_command_bar_ranking_tensor` | 0xe2ab7f0 |
| name | `T@"NSString",R,C,N` | 0xe2ab800 |
| shape | `T@"NSArray",R,C,N` | 0xe2ab810 |
| data | `T@"NSArray",R,C,N` | 0xe2ab820 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableCommandBarRankingTensor` | `^v16@0:8` | [0x4431cd0](../bodies/04431cd0.txt) | Observed return expression: (void *)(param_1 + 8). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b62c`; dobjc170.txt:2592 | medium-local; available; literal_partial |
| (base) | `-init:shape:data:` | `@40@0:8@16@24@32` | [0x4431cec](../bodies/04431cec.txt) | Creates native name, int32 shape vector and float data vector; NSNumber shape values use intValue and data adapter 0x443181c. | 0x42a44a8,0x1b356b8,0x443181c | metadata `0xd77b638`; dobjc170.txt:2593 | medium-partial; available; partial_review |
| (base) | `-initWithCommandBarRankingTensor:` | `@24@0:8r^v16` | [0x4432164](../bodies/04432164.txt) | Calls NSObject init, destroys prior engaged tensor through 0x42a44a8, copies input via 0x42a5760 and engages self+0x50. | 0x42a44a8,0x42a5760 | metadata `0xd77b644`; dobjc170.txt:2594 | medium-partial; available; partial_review |
| (base) | `-name` | `@16@0:8` | [0x44321d8](../bodies/044321d8.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77b650`; dobjc170.txt:2595 | medium-local; available; conversion_partial |
| (base) | `-shape` | `@16@0:8` | [0x4432208](../bodies/04432208.txt) | Requires engaged tensor, wraps each int32 native dimension as NSNumber in new NSMutableArray. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b65c`; dobjc170.txt:2596 | medium-local; available; local_review |
| (base) | `-data` | `@16@0:8` | [0x44322c8](../bodies/044322c8.txt) | Requires engaged tensor, wraps each native float as NSNumber in new NSMutableArray. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b668`; dobjc170.txt:2597 | medium-local; available; local_review |
| (base) | `-command_bar_ranking_tensor` | `{optional<arc::CommandBarRankingTensor>=(?=c{CommandBarRankingTensor={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{vector<int, std::allocator<int>>={__vector_layout<int, std::allocator<int>>=^iQQ}}{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B}16@0:8` | [0x4432388](../bodies/04432388.txt) | Copies optional native tensor via 0x42a5760, preserving engagement in aggregate return storage. | 0x42a5760 | metadata `0xd77b674`; dobjc170.txt:2598 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x44323e4](../bodies/044323e4.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | 0x42a44a8 | metadata `0xd77b680`; dobjc170.txt:2599 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442c254](../bodies/0442c254.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b68c`; dobjc170.txt:2600 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- No shape/product validation established; integer narrowing and tensor data ownership.
- Native vector-copy invariants and validation.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- By-value aggregate ABI and native vector-copy helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
