# ArcKeyEventData

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bb78`, class_ro `0xe2a44e8`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 4 | `_windowsVirtualKeyCode` | `i` | 0xe2a4420 |
| 0x10 | 8 | `_eventType` | `Q` | 0xe2a4440 |
| 0x18 | 8 | `_domCode` | `@"NSString"` | 0xe2a4460 |
| 0x20 | 8 | `_key` | `@"NSString"` | 0xe2a4480 |

Initializer copies domCode and key strings, stores eventType at +0x10 and Windows virtual-key int32 at +8 (0x43def08). Accessors read +8/+0x10/+0x18/+0x20 (0x4081a30, 0x2cb140, 0x4084dc0, 0x4060580); destructor clears both strings (0x43defdc).

## Lifecycle, callbacks and relations

Explicit copy messages establish copy behavior for key/code in this initializer (0x43def08). No event injection, native key conversion, callback or scheduling occurs in this class. The eventType slot is scalar despite the shared Ghidra ID-return name.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable input-event DTO. The real keyboard dispatch path must translate it to the chosen backend and preserve physical code versus logical key.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcKeyEventData.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithEventType:domCode:key:windowsVirtualKeyCode: | `@44@0:8Q16@24@32i40` | 0x43def08 | Copies domCode/key with explicit copy messages, stores eventType and virtual-key int32 in local fields. | local state / indirect operation; see body | IMP 0x43def08; record 0xd777a60; dyld L1312; otool L22514 | medium / available / bounded_review |
| base | - eventType | `Q16@0:8` | 0x2cb140 | Reads 64-bit register value from self+0x10 (_eventType); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2cb140; record 0xd777a6c; dyld L1313; otool L7249 | high / available / local_verified |
| base | - domCode | `@16@0:8` | 0x4084dc0 | Reads 64-bit register value from self+0x18 (_domCode); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4084dc0; record 0xd777a78; dyld L1314; otool L13108 | high / available / local_verified |
| base | - key | `@16@0:8` | 0x4060580 | Reads 64-bit register value from self+0x20 (_key); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4060580; record 0xd777a84; dyld L1315; otool L12042 | high / available / local_verified |
| base | - windowsVirtualKeyCode | `i16@0:8` | 0x4081a30 | Reads 32-bit register value from self+0x8 (_windowsVirtualKeyCode); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4081a30; record 0xd777a90; dyld L1316; otool L22526 | high / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x43defdc | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x20, 0x18. | local state / indirect operation; see body | IMP 0x43defdc; record 0xd777a9c; dyld L1317; otool L22529 | medium / available / local_observation |

## Remaining consequential questions

Determine eventType enum and keyboard-layout/modifier semantics from actual consumer; do not infer from Windows key naming. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcKeyEventData.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcKeyEventData.md).
