# ArcAutocompleteMatch

ArcCore **1.170.0 arm64**, unslid engine VAs. 27 declared identities. Superclass `NSObject`; class metadata `0xe37db08`, class RO `0xe2acb60`; raw otool `objc170.txt:32803`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization retains a native shared handle in an optional (0x4438b70), with cleanup at 0x4439d80. Most fields read native state and convert URLs or UTF-16 strings (for example 0x4438c94, 0x4438f9c). additionalData constructs an ObjC array of key/value records (0x4439348); duplicates lazily constructs/caches ArcAutocompleteMatch wrappers, and associatedKeyword wraps an optional associated match (0x44398c8, 0x4439ba8). No callback thread or native-match mutation policy is established. ArcBrowser consumes the native accessor for deletion (0x43e21d8). Chromium's AutocompleteMatch data is a source-backed comparison, not a proven binary type match: components/omnibox/browser/autocomplete_match.h:867,898,980,1083,1090. CEF navigation primitives do not by themselves reproduce ranking/provider/deletion semantics; API availability was not exhaustively audited here.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `48`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _duplicates | 8 | 8 | `@"NSArray"` | 0xe2ac978 |
| _associatedKeyword | 16 | 8 | `@"ArcAutocompleteMatch"` | 0xe2ac998 |
| _autocomplete_match | 24 | 24 | `{optional<arc::AutocompleteMatch>=""(?="__null_state_"c"__val_"{AutocompleteMatch="impl_"{shared_ptr<arc::AutocompleteMatch::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2ac9b8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| autocomplete_match | `T{optional<arc::AutocompleteMatch>=(?=c{AutocompleteMatch={shared_ptr<arc::AutocompleteMatch::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_autocomplete_match` | 0xe2ac9e0 |
| destinationUrl | `T@"NSURL",R,N` | 0xe2ac9f0 |
| destinationUrlSpec | `T@"NSString",R,C,N` | 0xe2aca00 |
| imageUrl | `T@"NSURL",R,N` | 0xe2aca10 |
| imageUrlSpec | `T@"NSString",R,C,N` | 0xe2aca20 |
| relevance | `Ti,R,N` | 0xe2aca30 |
| contents | `T@"NSString",R,C,N` | 0xe2aca40 |
| matchType | `T@"NSString",R,C,N` | 0xe2aca50 |
| providerType | `T@"NSString",R,C,N` | 0xe2aca60 |
| inlineAutocompleteText | `T@"NSString",R,C,N` | 0xe2aca70 |
| description | `T@"NSString",R,C,N` | 0xe2aca80 |
| additionalData | `T@"NSArray",R,N` | 0xe2aca90 |
| hasTabMatch | `TB,R,N` | 0xe2acaa0 |
| allowedToBeDefaultMatch | `TB,R,N` | 0xe2acab0 |
| deletable | `TB,R,N` | 0xe2acac0 |
| swapContentsAndDescription | `TB,R,N` | 0xe2acad0 |
| isFromPrevious | `TB,R,N` | 0xe2acae0 |
| isFromKeyword | `TB,R,N` | 0xe2acaf0 |
| keyword | `T@"NSString",R,C,N` | 0xe2acb00 |
| fillIntoEdit | `T@"NSString",R,C,N` | 0xe2acb10 |
| fillIntoEditAdditionalText | `T@"NSString",R,C,N` | 0xe2acb20 |
| transition | `T@"NSString",R,C,N` | 0xe2acb30 |
| duplicates | `T@"NSArray",R,N,V_duplicates` | 0xe2acb40 |
| associatedKeyword | `T@"ArcAutocompleteMatch",R,N,V_associatedKeyword` | 0xe2acb50 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithAutocompleteMatch:` | `@24@0:8r^v16` | [0x4438b70](../bodies/04438b70.txt) | Calls NSObject init; retains native shared match at self+0x18/+0x20 and engages +0x28. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77be9c`; dobjc170.txt:2792 | medium-local; available; local_review |
| (base) | `-destinationUrl` | `@16@0:8` | [0x4438c94](../bodies/04438c94.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x7cf864, 0x60a60, 0x43abd34. | 0x7cf864,0x60a60,0x43abd34 | metadata `0xd77bea8`; dobjc170.txt:2793 | low-unresolved; available; unresolved |
| (base) | `-destinationUrlSpec` | `@16@0:8` | [0x4438d44](../bodies/04438d44.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x7cf864,0x60a60,0x1c391bc | metadata `0xd77beb4`; dobjc170.txt:2794 | medium-local; available; conversion_partial |
| (base) | `-imageUrl` | `@16@0:8` | [0x4438e0c](../bodies/04438e0c.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x7cf864, 0x60a60, 0x43abd34. | 0x7cf864,0x60a60,0x43abd34 | metadata `0xd77bec0`; dobjc170.txt:2795 | low-unresolved; available; unresolved |
| (base) | `-imageUrlSpec` | `@16@0:8` | [0x4438ebc](../bodies/04438ebc.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x7cf864,0x60a60,0x1c391bc | metadata `0xd77becc`; dobjc170.txt:2796 | medium-local; available; conversion_partial |
| (base) | `-relevance` | `i16@0:8` | [0x4438f84](../bodies/04438f84.txt) | Observed return expression: *(int *)(*(long *)(param_1 + 0x18) + 8). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bed8`; dobjc170.txt:2797 | medium-local; available; literal_partial |
| (base) | `-contents` | `@16@0:8` | [0x4438f9c](../bodies/04438f9c.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bee4`; dobjc170.txt:2798 | medium-local; available; conversion_partial |
| (base) | `-matchType` | `@16@0:8` | [0x4439064](../bodies/04439064.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0xb89b80,0x1c391bc | metadata `0xd77bef0`; dobjc170.txt:2799 | medium-local; available; conversion_partial |
| (base) | `-providerType` | `@16@0:8` | [0x4439120](../bodies/04439120.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x433190c,0x1c391bc | metadata `0xd77befc`; dobjc170.txt:2800 | medium-local; available; conversion_partial |
| (base) | `-inlineAutocompleteText` | `@16@0:8` | [0x44391c0](../bodies/044391c0.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bf08`; dobjc170.txt:2801 | medium-local; available; conversion_partial |
| (base) | `-description` | `@16@0:8` | [0x4439280](../bodies/04439280.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bf14`; dobjc170.txt:2802 | medium-local; available; conversion_partial |
| (base) | `-additionalData` | `@16@0:8` | [0x4439348](../bodies/04439348.txt) | Copies native additional-data collection; wraps each pair in ArcAutocompleteMatchAdditionalData using init:value: and returns array. | 0xd14e0,0x1c391bc,0x3f9ab0 | metadata `0xd77bf20`; dobjc170.txt:2803 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-hasTabMatch` | `B16@0:8` | [0x443953c](../bodies/0443953c.txt) | Requires match optional engagement; absent native tab-match optional returns false, otherwise low bit of native flag at +0x418. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf2c`; dobjc170.txt:2804 | medium-local; available; local_review |
| (base) | `-allowedToBeDefaultMatch` | `B16@0:8` | [0x4439568](../bodies/04439568.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x18) + 100). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf38`; dobjc170.txt:2805 | medium-local; available; literal_partial |
| (base) | `-deletable` | `B16@0:8` | [0x4439580](../bodies/04439580.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x14). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf44`; dobjc170.txt:2806 | medium-local; available; literal_partial |
| (base) | `-swapContentsAndDescription` | `B16@0:8` | [0x4439598](../bodies/04439598.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x330). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf50`; dobjc170.txt:2807 | medium-local; available; literal_partial |
| (base) | `-isFromPrevious` | `B16@0:8` | [0x44395b0](../bodies/044395b0.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x478). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf5c`; dobjc170.txt:2808 | medium-local; available; literal_partial |
| (base) | `-isFromKeyword` | `B16@0:8` | [0x44395c8](../bodies/044395c8.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x450). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bf68`; dobjc170.txt:2809 | medium-local; available; literal_partial |
| (base) | `-keyword` | `@16@0:8` | [0x44395e0](../bodies/044395e0.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bf74`; dobjc170.txt:2810 | medium-local; available; conversion_partial |
| (base) | `-fillIntoEdit` | `@16@0:8` | [0x44396a8](../bodies/044396a8.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bf80`; dobjc170.txt:2811 | medium-local; available; conversion_partial |
| (base) | `-fillIntoEditAdditionalText` | `@16@0:8` | [0x4439768](../bodies/04439768.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77bf8c`; dobjc170.txt:2812 | medium-local; available; conversion_partial |
| (base) | `-transition` | `@16@0:8` | [0x4439828](../bodies/04439828.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x4331c6c,0x1c391bc | metadata `0xd77bf98`; dobjc170.txt:2813 | medium-local; available; conversion_partial |
| (base) | `-duplicates` | `@16@0:8` | [0x44398c8](../bodies/044398c8.txt) | Lazily constructs/caches ObjC duplicate-match array from native handles returned by 0x4331ae4. | 0x4331ae4 | metadata `0xd77bfa4`; dobjc170.txt:2814 | medium-partial; available; partial_review |
| (base) | `-associatedKeyword` | `@16@0:8` | [0x4439ba8](../bodies/04439ba8.txt) | Lazily obtains associated match via 0x43319d8 and wraps as ArcAutocompleteMatch when present. | 0x43319d8 | metadata `0xd77bfb0`; dobjc170.txt:2815 | medium-partial; available; partial_review |
| (base) | `-autocomplete_match` | `{optional<arc::AutocompleteMatch>=(?=c{AutocompleteMatch={shared_ptr<arc::AutocompleteMatch::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x4414800](../bodies/04414800.txt) | Copies optional native shared handle from self+0x18 into aggregate return storage, increments non-null control block; source engagement is at self+0x28. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bfbc`; dobjc170.txt:2816 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4439d80](../bodies/04439d80.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bfc8`; dobjc170.txt:2817 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4439e5c](../bodies/04439e5c.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77bfd4`; dobjc170.txt:2818 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native result mutation and cache invalidation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Key vocabulary and value encoding at native producer.
- Native duplicate mutation and cache invalidation.
- Cache refresh and absence semantics.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
