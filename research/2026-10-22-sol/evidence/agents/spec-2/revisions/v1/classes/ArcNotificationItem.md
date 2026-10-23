# ArcNotificationItem

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 5 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c730`, class_ro `0xe2a7a58`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_title` | `@"NSString"` | 0xe2a79f0 |
| 0x10 | 8 | `_message` | `@"NSString"` | 0xe2a7a10 |

title and message are object slots at +8/+0x10 (0x2d0fb40, 0x2cb140). Setters replace those slots through objc store/release fixups (0x43eae2c, 0x420b588); destructor clears both (0x43dd5a4). No initializer is declared in this class.

## Lifecycle, callbacks and relations

No notification delivery, delegate, permission check or operating-system service call is present in these methods. Copy-versus-retain setter semantics require raw callsite verification because Ghidra substitutes ownership fixups.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable content DTO; notification platform integration resides elsewhere.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNotificationItem.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - title | `@16@0:8` | 0x2d0fb40 | Reads 64-bit register value from self+0x8 (_title); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2d0fb40; record 0xd779738; dyld L1894; otool L12036 | high / available / local_verified |
| base | - setTitle: | `v24@0:8@16` | 0x43eae2c | Local field operation: *(ID *)(param_1 + 8) = param_3; | local state / indirect operation; see body | IMP 0x43eae2c; record 0xd779744; dyld L1895; otool L23657 | medium / available / local_verified |
| base | - message | `@16@0:8` | 0x2cb140 | Reads 64-bit register value from self+0x10 (_message); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2cb140; record 0xd779750; dyld L1896; otool L12039 | high / available / local_verified |
| base | - setMessage: | `v24@0:8@16` | 0x420b588 | Local field operation: *(ID *)(param_1 + 0x10) = param_3; | local state / indirect operation; see body | IMP 0x420b588; record 0xd77975c; dyld L1897; otool L13105 | medium / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x43dd5a4 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x10, 0x8. | local state / indirect operation; see body | IMP 0x43dd5a4; record 0xd779768; dyld L1898; otool L22051 | medium / available / local_observation |

## Remaining consequential questions

Recover setter ownership from machine code and identify producer/localization expectations. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
