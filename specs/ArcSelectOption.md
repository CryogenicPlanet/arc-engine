# ArcSelectOption

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37d018`, class RO `0xe2aa090`; raw otool `objc170.txt:29519`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Inline optional native SelectOption stores two optional strings and two booleans. Init uses native copy helper 0x83694e8 and destructor frees long-string storage (0x442bee4, 0x442c1e8). value/text return nil for absent native string, while present empty strings pass through string conversion (0x442bf94, 0x442c084); optional wrapper itself must be engaged. isSelected/isDisabled read +0x48/+0x49 with engagement guard (0x442c174, 0x442c188). No callbacks or thread guarantee was established. Select-control identity, action routing and popup ownership must be found at producer/consumer; this class specifies only option transport.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `88`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _select_option | 8 | 80 | `{optional<arc::SelectOption>=""(?="__null_state_"c"__val_"{SelectOption="value"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}"text"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}"is_selected"B"is_disabled"B})"__engaged_"B}` | 0xe2aa018 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| select_option | `T{optional<arc::SelectOption>=(?=c{SelectOption={optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}BB})B},R,N,V_select_option` | 0xe2aa040 |
| value | `T@"NSString",R,C,N` | 0xe2aa050 |
| text | `T@"NSString",R,C,N` | 0xe2aa060 |
| isSelected | `TB,R,N` | 0xe2aa070 |
| isDisabled | `TB,R,N` | 0xe2aa080 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithSelectOption:` | `@24@0:8r^v16` | [0x442bee4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442bee4.txt) | Calls NSObject init, destroys prior optional string state then copies native option via 0x83694e8 and engages +0x50. | 0x83694e8 | metadata `0xd77aadc`; dobjc170.txt:2324 | medium-partial; available; partial_review |
| (base) | `-value` | `@16@0:8` | [0x442bf94](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442bf94.txt) | Requires wrapper engagement; returns nil when native value string is absent, otherwise converts present string through 0x1c391bc, including empty string. | 0x60a60,0x1c391bc | metadata `0xd77aae8`; dobjc170.txt:2325 | medium-local; available; local_review |
| (base) | `-text` | `@16@0:8` | [0x442c084](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c084.txt) | Requires wrapper engagement; returns nil when native text string is absent, otherwise converts present string through 0x1c391bc, including empty string. | 0x60a60,0x1c391bc | metadata `0xd77aaf4`; dobjc170.txt:2326 | medium-local; available; local_review |
| (base) | `-isSelected` | `B16@0:8` | [0x442c174](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c174.txt) | Observed return expression: (bool)*(byte *)(param_1 + 0x48). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ab00`; dobjc170.txt:2327 | medium-local; available; literal_partial |
| (base) | `-isDisabled` | `B16@0:8` | [0x442c188](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c188.txt) | Observed return expression: (bool)*(byte *)(param_1 + 0x49). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ab0c`; dobjc170.txt:2328 | medium-local; available; literal_partial |
| (base) | `-select_option` | `{optional<arc::SelectOption>=(?=c{SelectOption={optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}BB})B}16@0:8` | [0x442c19c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c19c.txt) | Copies optional native SelectOption to aggregate return storage via 0x83694e8, preserving wrapper engagement. | 0x83694e8 | metadata `0xd77ab18`; dobjc170.txt:2329 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442c1e8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c1e8.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ab24`; dobjc170.txt:2330 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442c254](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442c254.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ab30`; dobjc170.txt:2331 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native helper copy invariants.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Copy helper and aggregate ABI.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcSelectOption.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcSelectOption.md).
