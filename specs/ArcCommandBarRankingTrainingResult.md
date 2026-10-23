# ArcCommandBarRankingTrainingResult

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 18 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 128 bytes; class metadata `0xe37d608`, class_ro `0xe2ab730`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_command_bar_ranking_training_result` | 8 | 120 | `{optional<arc::CommandBarRankingTrainingResult>=""(?="__null_state_"c"__val_"{CommandBarRankingTrainingResult="outcome"i… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline optional result stores outcomes, failure reason, tensor delta vector, model name/version/sample count, loss vector, float norms/ratio and duration. `0x4431310` performs conversion/storage, `0x4431b28` boxes losses. Category delta `0x4402480` invokes tensor converter `0x44022d8`, but Ghidra places cleanup `0x42a43ac` before vector reads; recovered lifetime semantics are suspect and deliberately unresolved. Initializer IMPs: `0x4431310`, `0x4431a34`. Cleanup IMP: `0x4431cb0`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc data interchange, not model execution. Preserving aggregate layout and array/tensor conversion is independent of CEF navigation APIs; training producer behavior is outside this class.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableCommandBarRankingTrainingResult` | `^v16@0:8` | [0x44312f4](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44312f4.c) | Returns pointer to inline native value at self+8 when optional is engaged; disengaged path calls 0x4b08a00. | field/indirect dispatch; see body | M `0xd77b558`; D 2573 | H; available; local_field_observed |
| (base) | `-init:failureReason:delta:modelName:modelVersion:numSamples:lossesPerEpoch:gradientNormMean:gradientNormMax:deltaNorm:deltaToWeightRatio:trainingDurationMs:` | `@84@0:8q16q24@32@40i48i52@56f64f68f72f76i80` | [0x4431310](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431310.c) | Builds inline training result: converts tensor delta vector, name and loss NSNumber array; stores outcomes/version/sample count, four float metrics and duration. No training is performed in this initializer. | 0x1b356b8; 0x42a43ac; 0x42a5644; 0x42a5760; 0x42a5854; 0x443181c; 0x44323f8 | M `0xd77b564`; D 2574 | M; available; bridge_reviewed_partial |
| (base) | `-initWithCommandBarRankingTrainingResult:` | `@24@0:8r^v16` | [0x4431a34](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431a34.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | 0x42a43ac; 0x43dc67c | M `0xd77b570`; D 2575 | M; available; local_body_observed |
| (base) | `-outcome` | `q16@0:8` | [0x4431aa8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431aa8.c) | Reads native field and returns expression (long)*(int *)(param_1 + 8). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b57c`; D 2576 | H; available; local_field_observed |
| (base) | `-failureReason` | `q16@0:8` | [0x4431abc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431abc.c) | Reads native field and returns expression (long)*(int *)(param_1 + 0xc). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b588`; D 2577 | H; available; local_field_observed |
| (base) | `-modelName` | `@16@0:8` | [0x4431ad0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431ad0.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x1c391bc | M `0xd77b594`; D 2578 | M; available; local_body_observed |
| (base) | `-modelVersion` | `i16@0:8` | [0x4431b00](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431b00.c) | Reads native field and returns expression *(int *)(param_1 + 0x40). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5a0`; D 2579 | H; available; local_field_observed |
| (base) | `-numSamples` | `i16@0:8` | [0x4431b14](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431b14.c) | Reads native field and returns expression *(int *)(param_1 + 0x44). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5ac`; D 2580 | H; available; local_field_observed |
| (base) | `-lossesPerEpoch` | `@16@0:8` | [0x4431b28](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431b28.c) | Converts inline vector<float> at self+0x48 into NSMutableArray of NSNumber floats. | field/indirect dispatch; see body | M `0xd77b5b8`; D 2581 | M; available; bridge_reviewed_partial |
| (base) | `-gradientNormMean` | `f16@0:8` | [0x4431be8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431be8.c) | Reads native field and returns expression *(float *)(param_1 + 0x60). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5c4`; D 2582 | H; available; local_field_observed |
| (base) | `-gradientNormMax` | `f16@0:8` | [0x4431bfc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431bfc.c) | Reads native field and returns expression *(float *)(param_1 + 100). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5d0`; D 2583 | H; available; local_field_observed |
| (base) | `-deltaNorm` | `f16@0:8` | [0x4431c10](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431c10.c) | Reads native field and returns expression *(float *)(param_1 + 0x68). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5dc`; D 2584 | H; available; local_field_observed |
| (base) | `-deltaToWeightRatio` | `f16@0:8` | [0x4431c24](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431c24.c) | Reads native field and returns expression *(float *)(param_1 + 0x6c). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5e8`; D 2585 | H; available; local_field_observed |
| (base) | `-trainingDurationMs` | `i16@0:8` | [0x4431c38](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431c38.c) | Reads native field and returns expression *(int *)(param_1 + 0x70). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77b5f4`; D 2586 | H; available; local_field_observed |
| (base) | `-command_bar_ranking_training_result` | `{optional<arc::CommandBarRankingTrainingResult>=(?=c{CommandBarRankingTrainingResult=ii{vector<arc::CommandBarRankingTensor, std::allocator<arc::CommandBarRankingTensor>>={__vector_layout<arc::CommandBarRankingTensor, std::allocator<arc::CommandBarRankingTensor>>=^{CommandBarRankingTensor}QQ}}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}ii{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}ffffi})B}16@0:8` | [0x4431c4c](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431c4c.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x43dc67c | M `0xd77b600`; D 2587 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4431cb0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431cb0.c) | Performs C++/ObjC cleanup of native storage; invokes 0x42a43ac. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | 0x42a43ac | M `0xd77b60c`; D 2588 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4431cc4](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4431cc4.c) | Initializes native storage with explicit stores: self+8=0; self+0x78=0. | field/indirect dispatch; see body | M `0xd77b618`; D 2589 | M; available; local_body_observed |
| Extensions | `-delta` | `@16@0:8` | [0x4402480](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4402480.c) | Category iterates native delta vector and calls 0x44022d8 for tensor conversion, but recovered cleanup 0x42a43ac precedes vector reads and makes ownership interpretation suspect. | 0x42a43ac; 0x44022d8 | M `0xd778ddc`; D 4965 | U; available_semantics_suspect; unresolved |

## Remaining consequential questions

Resolve delta cleanup ordering using coherent disassembly. Validate tensor shape/data deep copy, failure enum values and producer guarantees.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcCommandBarRankingTrainingResult.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcCommandBarRankingTrainingResult.md).
