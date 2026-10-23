# ArcAIInferenceConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dbf8`, class_ro `0xe2ace10`; instanceStart 8, instanceSize 20 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 12 | `_ai_inference_configuration` | `{optional<arc::AIStreamedInferenceConfiguration>=""(?="__null_state_"c"__val_"{AIStreamedInferenceConfiguration="max_tokens"i"stre…` | 0xe2acdb8 |

The initializer engages an inline configuration, stores maxTokens as int32 at +8 and truncates streamAfterEveryNTokens to signed int16 at +0xc (0x443a318). Getters require the engagement byte at +0x10; an empty configuration traps rather than returning defaults (0x443a3d4, 0x443a3e8). The mutable accessor returns interior storage (0x443a2fc); the aggregate getter copies the optional (0x443a3fc).

## Lifecycle, callbacks and relations

No callback is installed here. ArcAIService consumes this configuration through its streaming adapter at 0x43d7a5c → 0x443a40c. There is no persistence operation in these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific value adapter; either backend needs the Arc ABI and integer narrowing behavior. No CEF API equivalence is established.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcAIInferenceConfiguration.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - mutableAIInferenceConfiguration | `^{AIStreamedInferenceConfiguration=is}16@0:8` | 0x443a2fc | Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00. | 0x04b08a00 | IMP 0x443a2fc; record 0xd77c088; dyld L2837; otool L33138 | medium / available / local_verified |
| base | - init:streamAfterEveryNTokens: | `@24@0:8i16i20` | 0x443a318 | Engages configuration; stores maxTokens int32 at +8 and narrows stream interval to int16 at +0xc. | local state / indirect operation; see body | IMP 0x443a318; record 0xd77c094; dyld L2838; otool L33141 | medium / available / bounded_review |
| base | - initWithAIInferenceConfiguration: | `@24@0:8r^{AIStreamedInferenceConfiguration=is}16` | 0x443a374 | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x443a374; record 0xd77c0a0; dyld L2839; otool L33144 | medium / available / local_observation |
| base | - maxTokens | `i16@0:8` | 0x443a3d4 | Local field operation: return *(int *)(param_1 + 8); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443a3d4; record 0xd77c0ac; dyld L2840; otool L33147 | medium / available / local_verified |
| base | - streamAfterEveryNTokens | `i16@0:8` | 0x443a3e8 | Local field operation: return (int)*(short *)(param_1 + 0xc); Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x443a3e8; record 0xd77c0b8; dyld L2841; otool L33150 | medium / available / local_verified |
| base | - ai_inference_configuration | `{optional<arc::AIStreamedInferenceConfiguration>=(?=c{AIStreamedInferenceConfiguration=is})B}16@0:8` | 0x443a3fc | Local field operation: return *(optional<arc::AIStreamedInferenceConfiguration> *)(param_1 + 8); | local state / indirect operation; see body | IMP 0x443a3fc; record 0xd77c0c4; dyld L2842; otool L33153 | medium / available / local_verified |
| base | - .cxx_construct | `@16@0:8` | 0x4400f4c | Initializes native storage/engagement fields at self offsets 0x8, 0x10; returns self. | local state / indirect operation; see body | IMP 0x4400f4c; record 0xd77c0d0; dyld L2843; otool L24403 | medium / available / local_observation |

## Remaining consequential questions

Confirm overflow/negative token handling at the service boundary, and whether app callers rely on the signed 16-bit truncation. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcAIInferenceConfiguration.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcAIInferenceConfiguration.md).
