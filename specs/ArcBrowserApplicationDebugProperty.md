# ArcBrowserApplicationDebugProperty

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37bdf8`, class RO `0xe2a5008`; raw otool `objc170.txt:23394`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

The native DebugProperty contains two inline C++ strings according to ivar metadata. Initialization and setter copy both strings (0x43e6224, 0x43e6408); name/value convert each via 0x1c391bc (0x43e632c, 0x43e6350), and destruction frees long-string storage (0x43e64e4). The native property getter copies into return storage (0x43e6374). Producer is ArcBrowserApplication debugProperties, which constructs these wrappers (0x43e8f44). No callbacks, live binding, or sequence requirement was established. A replacement can marshal two strings but must identify the debug-property producer keys and values before claiming diagnostic parity.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _property | 8 | 48 | `{DebugProperty="name"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"value"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}}` | 0xe2a4fb0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| property | `T{DebugProperty={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}},N,V_property` | 0xe2a4fd8 |
| name | `T@"NSString",R,N` | 0xe2a4fe8 |
| value | `T@"NSString",R,N` | 0xe2a4ff8 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithDebugProperty:` | `@64@0:8{DebugProperty={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}}16` | [0x43e6224](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e6224.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x359ad8, 0x359998. | 0x359ad8,0x359998 | metadata `0xd777fd4`; dobjc170.txt:1439 | low-unresolved; available; unresolved |
| (base) | `-name` | `@16@0:8` | [0x43e632c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e632c.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd777fe0`; dobjc170.txt:1440 | medium-local; available; conversion_partial |
| (base) | `-value` | `@16@0:8` | [0x43e6350](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e6350.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd777fec`; dobjc170.txt:1441 | medium-local; available; conversion_partial |
| (base) | `-property` | `{DebugProperty={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}}16@0:8` | [0x43e6374](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e6374.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x60a60. | 0x60a60 | metadata `0xd777ff8`; dobjc170.txt:1442 | low-unresolved; available; unresolved |
| (base) | `-setProperty:` | `v64@0:8{DebugProperty={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}}16` | [0x43e6408](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e6408.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x359ad8, 0x359998. | 0x359ad8,0x359998 | metadata `0xd778004`; dobjc170.txt:1443 | low-unresolved; available; unresolved |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43e64e4](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e64e4.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778010`; dobjc170.txt:1444 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4083e80](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04083e80.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77801c`; dobjc170.txt:1445 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcBrowserApplicationDebugProperty.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcBrowserApplicationDebugProperty.md).
