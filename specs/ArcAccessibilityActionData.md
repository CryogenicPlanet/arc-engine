# ArcAccessibilityActionData

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37bad8`, class RO `0xe2a4178`; raw otool `objc170.txt:22266`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Two initializers store an action code after a four-value range check (0x43deae8, 0x43deb44); an optional string is engaged only for non-nil input in the value initializer. value deliberately collapses absent and empty string to nil (0x43dec58). actionData copies action plus optional string rather than returning an ObjC object (0x43decac); metadata supplies the aggregate ABI. Cleanup releases long-string storage (0x43ded20). No callback or thread rule was established. Action meanings and conversion to actual accessibility actions remain unresolved; preserving an integer signature alone would miss nil/empty behavior.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `48`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _actionData | 8 | 40 | `{AccessibilityActionData="action"i"value"{optional<std::string>=""(?="__null_state_"c"__val_"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})})"__engaged_"B}}` | 0xe2a4120 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| actionData | `T{AccessibilityActionData=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}},N,V_actionData` | 0xe2a4148 |
| action | `TQ,R,N` | 0xe2a4158 |
| value | `T@"NSString",R,N` | 0xe2a4168 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithAction:` | `@24@0:8Q16` | [0x43deae8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043deae8.txt) | Calls NSObject init; checks action <=3, trapping outside this unsigned range, and stores 32-bit action at +8. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7778dc`; dobjc170.txt:1277 | medium-local; available; local_review |
| (base) | `-initWithAction:value:` | `@32@0:8Q16@24` | [0x43deb44](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043deb44.txt) | Checks action <=3; stores action at +8; a non-nil value is converted into optional C++ string storage at +0x10, engaged flag +0x28. | 0x1b356b8 | metadata `0xd7778e8`; dobjc170.txt:1278 | medium-local; available; local_review |
| (base) | `-action` | `Q16@0:8` | [0x43dec3c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dec3c.txt) | Reads unsigned action at +8; returns it only below 4, otherwise traps. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7778f4`; dobjc170.txt:1279 | medium-local; available; local_review |
| (base) | `-value` | `@16@0:8` | [0x43dec58](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043dec58.txt) | Returns nil when optional is disengaged or its string length is zero; otherwise converts the stored string through 0x1c391bc. | 0x1c391bc | metadata `0xd777900`; dobjc170.txt:1280 | medium-local; available; local_review |
| (base) | `-actionData` | `{AccessibilityActionData=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}}16@0:8` | [0x43decac](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043decac.txt) | Copies stored action and optional string into native aggregate return storage, preserving engagement. | 0x60a60 | metadata `0xd77790c`; dobjc170.txt:1281 | medium-local; available; local_review |
| (base) | `-setActionData:` | `v56@0:8{AccessibilityActionData=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}}16` | [0x43ded14](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043ded14.txt) | Forwards native action-data assignment into self+8 through 0x4332040. | 0x4332040 | metadata `0xd777918`; dobjc170.txt:1282 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43ded20](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043ded20.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777924`; dobjc170.txt:1283 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43ded48](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043ded48.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd777930`; dobjc170.txt:1284 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- What are the four action meanings?
- Enum meanings and empty-string round-trip policy?
- Enum meanings?
- Validate aggregate return ABI; action enum meanings remain open.
- Deep assignment helper and valid-action range for this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcAccessibilityActionData.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcAccessibilityActionData.md).
