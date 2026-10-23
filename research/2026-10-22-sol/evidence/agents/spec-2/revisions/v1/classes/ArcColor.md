# ArcColor

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 10 declared instance methods; no declared class methods. Categories: Init. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dd88`, class_ro `0xe2ad3f0`; instanceStart 8, instanceSize 12 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 4 | `_color` | `{Color="r"C"g"C"b"C"a"C}` | 0xe2ad378 |

The four-byte field is at +8. r/g/b/a return successive bytes +8/+9/+0xa/+0xb (0x40b1230, 0x43ecb04, 0x43ecb0c, 0x440c5a4); the r IMP is also used for a Boolean in ArcWebLoginDialog, so its Ghidra name/type is not authoritative. Init category initWithR:g:b:a: packs the bytes and invokes initWithColor: (0x4401b58).

## Lifecycle, callbacks and relations

initWithColor: copies the four bytes (0x443b164); color uses the hidden aggregate return destination, obscured by Ghidra, and setColor: copies four bytes (0x443b1b0, 0x443b1bc; raw prefix evidence). Both C++ lifecycle declarations alias the same return-only IMP 0x15ac0. No callback or persistence occurs in these bodies.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A four-byte Arc value ABI. Do not map its memory byte order to a Chromium SkColor integer without checking caller packing.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcColor.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithColor: | `@24@0:8r^{Color=CCCC}16` | 0x443b164 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x443b164; record 0xd77c314; dyld L2898; otool L33582 | medium / available / local_observation |
| base | - r | `C16@0:8` | 0x40b1230 | Raw 39402000/d65f03c0 loads one unsigned byte from self+8; metadata C return is authoritative over alias Ghidra signature. | local state / indirect operation; see body | IMP 0x40b1230; record 0xd77c320; dyld L2899; otool L27546 | medium / available_signature_artifact / bounded_review |
| base | - g | `C16@0:8` | 0x43ecb04 | Reads 8-bit register value from self+0x9 (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x43ecb04; record 0xd77c32c; dyld L2900; otool L27549 | high / available / local_verified |
| base | - b | `C16@0:8` | 0x43ecb0c | Reads 8-bit register value from self+0xa (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x43ecb0c; record 0xd77c338; dyld L2901; otool L27552 | high / available / local_verified |
| base | - a | `C16@0:8` | 0x440c5a4 | Reads 8-bit register value from self+0xb (inline/native field); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x440c5a4; record 0xd77c344; dyld L2902; otool L27555 | high / available / local_verified |
| base | - color | `{Color=CCCC}16@0:8` | 0x443b1b0 | Raw b9400809/b9000109/d65f03c0 copies uint32 at self+8 to hidden return destination x8; Ghidra pseudo-return is invalid. | local state / indirect operation; see body | IMP 0x443b1b0; record 0xd77c350; dyld L2903; otool L33597 | medium / available_signature_artifact / bounded_review |
| base | - setColor: | `v20@0:8{Color=CCCC}16` | 0x443b1bc | Local field operation: *(undefined4 *)(param_1 + 8) = *(undefined4 *)(ulong)(uint)param_3; | local state / indirect operation; see body | IMP 0x443b1bc; record 0xd77c35c; dyld L2904; otool L33600 | medium / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x15ac0 | Shared return-only IMP; no field cleanup. | raw return-only shared IMP | IMP 0x15ac0; record 0xd77c368; dyld L2905; otool L9486 | high / available / local_verified |
| base | - .cxx_construct | `@16@0:8` | 0x15ac0 | Shared return-only IMP; constructor leaves state unchanged and x0 self available. | raw return-only shared IMP | IMP 0x15ac0; record 0xd77c374; dyld L2906; otool L33606 | high / available / local_verified |
| Init | - initWithR:g:b:a: | `@32@0:8C16C20C24C28` | 0x4401b58 | Init category packs four uint8 components in r,g,b,a memory order and calls initWithColor:. | local state / indirect operation; see body | IMP 0x4401b58; record 0xd778d94; dyld L4955; otool L62429 | medium / available / bounded_review |

## Remaining consequential questions

Validate endianness and alpha/premultiplication semantics at the native conversion boundary; preserve hidden return calling convention. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
