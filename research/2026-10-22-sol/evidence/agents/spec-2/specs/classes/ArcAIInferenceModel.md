# ArcAIInferenceModel

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dba8`, class_ro `0xe2acd20`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 40 | `_ai_inference_model` | `{optional<arc::AIInferenceModel>=""(?="__null_state_"c"__val_"{AIInferenceModel="type"i"directory"{basic_string<char, std::char_tr…` | 0xe2accc8 |

This is an engaged optional value, not the loaded model itself. init:directory: writes the model discriminator and converts the directory string (0x4439f8c); initWithAIInferenceModel: copies native string storage (0x443a094). modelType and directory check engagement (0x443a13c, 0x443a150); destruction frees an allocated string only while engaged (0x43ded20).

## Lifecycle, callbacks and relations

The aggregate getter copies the model descriptor (0x443a180). ArcAIService loadInferenceModel and streamInference consume it through the adapter 0x443a21c (callers 0x43d77b0, 0x43d7a5c). No model file is opened by the inspected model IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific descriptor; backend model loading lives beyond this class. CEF alone does not specify Arc model format.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIInferenceModel.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - mutableAIInferenceModel | `^v16@0:8` | 0x442c260 | Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00. | 0x04b08a00 | IMP 0x442c260; record 0xd77c020; dyld L2827; otool L29614 | medium / available / local_verified |
| base | - init:directory: | `@32@0:8q16@24` | 0x4439f8c | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x01b356b8 | IMP 0x4439f8c; record 0xd77c02c; dyld L2828; otool L33066 | medium / available / local_observation |
| base | - initWithAIInferenceModel: | `@24@0:8r^v16` | 0x443a094 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | 0x00060a60 | IMP 0x443a094; record 0xd77c038; dyld L2829; otool L33069 | medium / available / local_observation |
| base | - modelType | `q16@0:8` | 0x443a13c | Local field operation: return (long)*(int *)(param_1 + 8); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443a13c; record 0xd77c044; dyld L2830; otool L33072 | medium / available / local_verified |
| base | - directory | `@16@0:8` | 0x443a150 | Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: 0x28, 0x27, 0x10, 0x18. Engagement guard/trap is visible. | 0x01c391bc | IMP 0x443a150; record 0xd77c050; dyld L2831; otool L33075 | medium / available / local_observation |
| base | - ai_inference_model | `{optional<arc::AIInferenceModel>=(?=c{AIInferenceModel=i{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}})B}16@0:8` | 0x443a180 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x28, 0x8, 0x27, 0x10, 0x18, 0x20. Engagement guard/trap is visible. | 0x00060a60 | IMP 0x443a180; record 0xd77c05c; dyld L2832; otool L33078 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43ded20 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x28, 0x27, 0x10, 0x20. | local state / indirect operation; see body | IMP 0x43ded20; record 0xd77c068; dyld L2833; otool L22300 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43ed538 | Initializes native storage/engagement fields at self offsets 0x8, 0x28; returns self. | local state / indirect operation; see body | IMP 0x43ed538; record 0xd77c074; dyld L2834; otool L23953 | medium / available / local_observation |

## Remaining consequential questions

Recover modelType enum meanings and validation rules in the loader; distinguish descriptor lifetime from loaded resource lifetime. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
