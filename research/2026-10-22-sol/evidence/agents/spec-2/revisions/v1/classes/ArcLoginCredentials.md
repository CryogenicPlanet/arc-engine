# ArcLoginCredentials

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e378`, class_ro `0xe2ae5f0`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 24 | `_login_credentials` | `{optional<arc::LoginCredentials>=""(?="__null_state_"c"__val_"{LoginCredentials="impl_"{shared_ptr<arc::LoginCredentials::Impl>="_…` | 0xe2ae588 |

Initialization stores an optional shared native credentials handle (0x443e874); aggregate getter copies it and destructor releases it (0x43df53c, 0x43df69c). url converts a native URL representation (0x443e998), username converts UTF-16 (0x443ea48).

## Lifecycle, callbacks and relations

password calls helper 0x6bd82ac and converts its returned data through the UTF-16 string helper (0x8374f3c). The helper was not followed; this does not establish secure memory, encryption or erase-on-release behavior. No keychain lookup, save or callback is demonstrated by these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A credentials-access DTO. Chrome password-manager/keychain and CEF authentication workflows require separate investigation; the password getter does not identify the storage backend.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcLoginCredentials.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithLoginCredentials: | `@24@0:8r^v16` | 0x443e874 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x443e874; record 0xd77cae4; dyld L3090; otool L35014 | medium / available / local_observation |
| base | - url | `@16@0:8` | 0x443e998 | Converts native URL/string storage through 0x43abd34; self offsets 0x18, 0x8. Engagement guard/trap is visible. | 0x007cf864, 0x00060a60, 0x043abd34 | IMP 0x443e998; record 0xd77caf0; dyld L3091; otool L35017 | medium / available / local_observation |
| base | - username | `@16@0:8` | 0x443ea48 | Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: 0x18, 0x8. Engagement guard/trap is visible. | 0x00054420, 0x01d9fb90 | IMP 0x443ea48; record 0xd77cafc; dyld L3092; otool L35020 | medium / available / local_observation |
| base | - password | `@16@0:8` | 0x8374f3c | Requires engaged native credentials; calls 0x6bd82ac and converts returned string through 0x1d9fb90. | 0x06bd82ac, 0x01d9fb90 | IMP 0x8374f3c; record 0xd77cb08; dyld L3093; otool L35023 | medium / available / bounded_review |
| base | - login_credentials | `{optional<arc::LoginCredentials>=(?=c{LoginCredentials={shared_ptr<arc::LoginCredentials::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x18, 0x10, 0x8. Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43df53c; record 0xd77cb14; dyld L3094; otool L35026 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43df69c | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x18, 0x10. | local state / indirect operation; see body | IMP 0x43df69c; record 0xd77cb20; dyld L3095; otool L22685 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43df760 | Initializes native storage/engagement fields at self offsets 0x8, 0x18; returns self. | local state / indirect operation; see body | IMP 0x43df760; record 0xd77cb2c; dyld L3096; otool L22688 | medium / available / local_observation |

## Remaining consequential questions

Recover 0x6bd82ac and credentials producer; confirm privacy/lifetime, origin matching and whether empty versus absent credentials are distinguishable. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
