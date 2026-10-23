# ArcPasteAndGoClassification

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37d158`, class RO `0xe2aa4b0`; raw otool `objc170.txt:29847`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Native optional value contains two C++ strings, copied in init (0x442d008) and destroyed at 0x442d244. menuTitle converts first string (0x442d10c); destinationURL passes second to URL converter 0x43abd34 (0x442d13c). Native getter shares IMP/type-recovery artifacts with NomicEmbedTextItem but metadata encodings remain separate (0x442d150). This wrapper does not classify input in the reviewed methods. No callback/thread contract was established. Locate the classification producer and URL normalization/error policy before choosing Chrome omnibox integration or CEF-side emulation.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _paste_and_go_classification | 8 | 56 | `{optional<arc::BrowserContext::Preferences::PasteAndGoClassification>=""(?="__null_state_"c"__val_"{PasteAndGoClassification="menu_title"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}"destination_url_spec"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__s"{__short="__data_"[23c]"__size_"b7"__is_long_"b1}"__l"{__long="__data_"*"__size_"Q"__cap_"b63"__is_long_"b1})}})"__engaged_"B}` | 0xe2aa458 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| paste_and_go_classification | `T{optional<arc::BrowserContext::Preferences::PasteAndGoClassification>=(?=c{PasteAndGoClassification={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B},R,N,V_paste_and_go_classification` | 0xe2aa480 |
| menuTitle | `T@"NSString",R,C,N` | 0xe2aa490 |
| destinationURL | `T@"NSURL",R,N` | 0xe2aa4a0 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithPasteAndGoClassification:` | `@24@0:8r^v16` | [0x442d008](../bodies/0442d008.txt) | Calls NSObject init; copies both native strings into inline optional and engages +0x38, releasing prior long-string storage. | 0x60a60 | metadata `0xd77acac`; dobjc170.txt:2368 | medium-local; available; local_review |
| (base) | `-menuTitle` | `@16@0:8` | [0x442d10c](../bodies/0442d10c.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x1c391bc | metadata `0xd77acb8`; dobjc170.txt:2369 | medium-local; available; conversion_partial |
| (base) | `-destinationURL` | `@16@0:8` | [0x442d13c](../bodies/0442d13c.txt) | Requires engaged optional; converts native destination string self+0x20 through 0x43abd34. | 0x43abd34 | metadata `0xd77acc4`; dobjc170.txt:2370 | medium-partial; available; partial_review |
| (base) | `-paste_and_go_classification` | `{optional<arc::BrowserContext::Preferences::PasteAndGoClassification>=(?=c{PasteAndGoClassification={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B}16@0:8` | [0x442d150](../bodies/0442d150.txt) | Copies two native C++ strings from optional self+8 to aggregate return storage, preserving engagement at self+0x38; shared IMP display type is unrelated. | 0x60a60 | metadata `0xd77acd0`; dobjc170.txt:2371 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x442d244](../bodies/0442d244.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77acdc`; dobjc170.txt:2372 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x442d2a0](../bodies/0442d2a0.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77ace8`; dobjc170.txt:2373 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- URL validity and conversion failure/nil policy in deeper converter.
- Aggregate ABI must use this row encoding.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
