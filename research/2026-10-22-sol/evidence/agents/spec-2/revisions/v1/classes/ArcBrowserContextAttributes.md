# ArcBrowserContextAttributes

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dd38`, class_ro `0xe2ad2e0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_userColor` | `@"ArcColor"` | 0xe2ad258 |
| 0x10 | 56 | `_browser_context_attributes` | `{BrowserContextAttributes="directory_basename"{basic_string<char, std::char_traits<char>, std::allocator<char>>="__rep_"(__rep="__…` | 0xe2ad278 |

The native attributes value begins at +0x10; +8 caches an ArcColor object. Initialization delegates to native assignment at 0x441e0a0 (0x443afd0). directoryBasename and userVisibleName convert native strings (0x443ab84, 0x443aba8). userColor lazily constructs and caches ArcColor from native color storage at +0x40 (0x443b028).

## Lifecycle, callbacks and relations

Aggregate getter copies strings and color (0x443b07c); aggregate setter calls 0x441e0a0 (0x443b0f4). Destruction frees native strings and clears cached color (0x443b100). The setter only passes self+0x10 to 0x441e0a0, which updates native strings/color without touching the cached ObjC slot at self+8; an already-created userColor wrapper therefore remains unchanged along this assignment path (0x443b0f4, 0x441e0a0, 0x443b028). No disk operation is shown.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc profile presentation data. Either hosting route can marshal values, but profile identity/persistence and cached-color invalidation must match the engine contract.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcBrowserContextAttributes.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithBrowserContextAttributes: | `@24@0:8r^v16` | 0x443afd0 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x0441e0a0 | IMP 0x443afd0; record 0xd77c2ac; dyld L2888; otool L33500 | medium / available / local_observation |
| base | - directoryBasename | `@16@0:8` | 0x443ab84 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x27, 0x10, 0x18. | 0x01c391bc | IMP 0x443ab84; record 0xd77c2b8; dyld L2889; otool L33306 | medium / available / local_observation |
| base | - userVisibleName | `@16@0:8` | 0x443aba8 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x3f, 0x28, 0x30. | 0x01c391bc | IMP 0x443aba8; record 0xd77c2c4; dyld L2890; otool L33309 | medium / available / local_observation |
| base | - userColor | `@16@0:8` | 0x443b028 | Lazily allocates ArcColor from native color at +0x40 and caches ObjC wrapper at +8. | local state / indirect operation; see body | IMP 0x443b028; record 0xd77c2d0; dyld L2891; otool L33509 | medium / available / bounded_review |
| base | - browser_context_attributes | `{BrowserContextAttributes={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{Color=CCCC}}16@0:8` | 0x443b07c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x27, 0x10, 0x18, 0x20, 0x3f, 0x28, 0x30, 0x38. | 0x00060a60 | IMP 0x443b07c; record 0xd77c2dc; dyld L2892; otool L33512 | medium / available / local_observation |
| base | - setBrowser_context_attributes: | `v72@0:8{BrowserContextAttributes={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{Color=CCCC}}16` | 0x443b0f4 | Assigns/replaces local native or ObjC state; self offsets 0x10. Calls 0x0441e0a0. | 0x0441e0a0 | IMP 0x443b0f4; record 0xd77c2e8; dyld L2893; otool L33515 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x443b100 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x3f, 0x28, 0x38, 0x27, 0x10, 0x20, 0x8. | local state / indirect operation; see body | IMP 0x443b100; record 0xd77c2f4; dyld L2894; otool L33518 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x443b154 | Initializes native storage/engagement fields at self offsets 0x28, 0x20, 0x38, 0x30, 0x18, 0x10; returns self. | local state / indirect operation; see body | IMP 0x443b154; record 0xd77c300; dyld L2895; otool L33521 | medium / available / local_observation |

## Remaining consequential questions

Does the app use the stale cached-color path after native reassignment, and what owns/updates the persisted profile attributes? Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
