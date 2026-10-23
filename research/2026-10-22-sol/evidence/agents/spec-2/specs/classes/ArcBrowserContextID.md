# ArcBrowserContextID

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bf38`, class_ro `0xe2a55e0`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 40 | `_id` | `{optional<arc::BrowserContextID>=""(?="__null_state_"c"__val_"{BrowserContextID="directory_basename"{basic_string<char, std::char_…` | 0xe2a5588 |

initWithBrowserContextID: copies a basename string and native discriminator into optional storage (0x43ed360). isIncognito returns discriminator == 1 after an engagement check (0x43ed454); directoryBasename converts the string (0x43ed470). id returns an aggregate copy rather than a unique numeric identifier (0x43ed4a0).

## Lifecycle, callbacks and relations

Destruction frees heap string storage only for an engaged value (0x43ed510); construction clears engagement (0x43ed538). No profile creation, directory traversal or callback registration is present in these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent Arc profile identity wrapper; mapping incognito identities to real browser contexts requires external service policy.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcBrowserContextID.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithBrowserContextID: | `@24@0:8r^v16` | 0x43ed360 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x00060a60, 0x00359ad8, 0x00359998 | IMP 0x43ed360; record 0xd778404; dyld L1531; otool L23938 | medium / available / local_observation |
| base | - isIncognito | `B16@0:8` | 0x43ed454 | Requires engaged native ID; returns native discriminator at +0x20 == 1. | local state / indirect operation; see body | IMP 0x43ed454; record 0xd778410; dyld L1532; otool L23941 | medium / available / bounded_review |
| base | - directoryBasename | `@16@0:8` | 0x43ed470 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x28, 0x1f, 0x8, 0x10. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x43ed470; record 0xd77841c; dyld L1533; otool L23944 | medium / available / local_observation |
| base | - id | `{optional<arc::BrowserContextID>=(?=c{BrowserContextID={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}i})B}16@0:8` | 0x43ed4a0 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x28, 0x1f, 0x8, 0x10, 0x18, 0x20. Engagement guard/trap is visible. | 0x00060a60 | IMP 0x43ed4a0; record 0xd778428; dyld L1534; otool L23947 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43ed510 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x28, 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x43ed510; record 0xd778434; dyld L1535; otool L23950 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43ed538 | Initializes native storage/engagement fields at self offsets 0x8, 0x28; returns self. | local state / indirect operation; see body | IMP 0x43ed538; record 0xd778440; dyld L1536; otool L23953 | medium / available / local_observation |

## Remaining consequential questions

Determine valid discriminator values and basename canonicalization; verify whether app compares native IDs or only exposed strings/flags. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
