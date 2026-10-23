# ArcSiteEngagementDetails

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d298`, class_ro `0xe2aa880`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 40 | `_site_engagement_details` | `{optional<arc::SiteEngagementDetails>=""(?="__null_state_"c"__val_"{SiteEngagementDetails="url"{basic_string<char, std::char_trait…` | 0xe2aa828 |

Optional native snapshot contains a URL string at +8 and score double at +0x20; native-copy init duplicates string storage (0x442db84), URL getter converts it (0x442dc2c), score getter requires engagement (0x442dc40).

## Lifecycle, callbacks and relations

Aggregate getter copies string and score (0x442dc54); shared lifecycle IMPs clear engagement and release heap strings (0x43ed538, 0x43ed510). No engagement scoring algorithm or persistent write occurs here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Source-backed domain comparison: components/site_engagement/content/site_engagement_score.h:29 and :148 describe upstream score state; exact backend function identity is not established. Either route needs an appropriate profile-scoped producer.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcSiteEngagementDetails.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithSiteEngagementDetails: | `@24@0:8r^v16` | 0x442db84 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x00060a60 | IMP 0x442db84; record 0xd77ae28; dyld L2405; otool L30156 | medium / available / local_observation |
| base | - url | `@16@0:8` | 0x442dc2c | Converts native URL/string storage through 0x43abd34; self offsets 0x28, 0x8. Engagement guard/trap is visible. | 0x043abd34 | IMP 0x442dc2c; record 0xd77ae34; dyld L2406; otool L30159 | medium / available / local_observation |
| base | - score | `d16@0:8` | 0x442dc40 | Local field operation: return *(double *)(param_1 + 0x20); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x442dc40; record 0xd77ae40; dyld L2407; otool L30162 | medium / available / local_verified |
| base | - site_engagement_details | `{optional<arc::SiteEngagementDetails>=(?=c{SiteEngagementDetails={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}d})B}16@0:8` | 0x442dc54 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x28, 0x1f, 0x8, 0x10, 0x18, 0x20. Engagement guard/trap is visible. | 0x00060a60 | IMP 0x442dc54; record 0xd77ae4c; dyld L2408; otool L30165 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43ed510 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x28, 0x1f, 0x8, 0x18. | local state / indirect operation; see body | IMP 0x43ed510; record 0xd77ae58; dyld L2409; otool L23950 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43ed538 | Initializes native storage/engagement fields at self offsets 0x8, 0x28; returns self. | local state / indirect operation; see body | IMP 0x43ed538; record 0xd77ae64; dyld L2410; otool L23953 | medium / available / local_observation |

## Remaining consequential questions

Identify site-engagement service query, score units/range and refresh semantics. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
