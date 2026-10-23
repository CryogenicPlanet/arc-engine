# AI backend decision and bounded implementation

Verified locally on 2026-10-23. This is the `ai` package's backend decision,
not a claim that AI feature acceptance has passed.

## Backend

Preserve Arc's app-supplied external-library `AIService_*` C interface behind
its Mojo service. Do not substitute TFLite or ChromeML without evidence that
they load the supplied models and reproduce the results.

`swap/hybrid-156/` is empty in this checkout. The original engine examined is
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
Its `Resources/arc/services/ai/public/mojom/ai_service.mojom-c_api.h` is identical
to the stock 1.168 header in this checkout. That interface declares:

- Embedding modes ONNX Runtime, CoreML, MLX, and remote (lines 48-53).
- Library path/version, app environment/version, retries and timeout (101-110).
- Creation/initialization and loading/ranking/evaluation/training callbacks
  (334-345, 478-507); exported C symbol names (585-626).
- `MlxError` among ranking training failures (77).

The engine contains `AIService_Create`, `AIService_Initialize`,
`ai.mojom.AIService` and `ai.mojom.AIStreamedInferenceListener` strings. The
trace app contains `AIInfraProtocols`, `MLXActor`, `LocalDistilBertClassifier`
and `LocalNomicEmbedText` strings. These establish interface capabilities,
not the identity/version of a library that actually ran. `libaperitif` evidence
describes a launch/sandbox shim, not a recovered inference implementation.

## Chromium 156 alternatives

Local source inspection found:

- `components/optimization_guide/core/inference/bert_model_executor.cc:51`
  constructs TFLite `BertNLClassifier`. No evidence establishes compatibility
  with Arc's model/config/classifier-head/LoRA file descriptors.
- `services/passage_embeddings/BUILD.gn:7` uses TFLite and SentencePiece.
  Compatibility with Arc's Nomic directory/version contract is unestablished.
- `services/on_device_model/ml/chrome_ml_holder.cc:64` loads a separate
  `optimization_guide_internal` library exposing `GetChromeMLAPIV2`.
  The local `components/optimization_guide/internal/` directory is empty;
  `components/optimization_guide/features.gni:25` explains the branding/internal
  source gate. That service is not Arc's `AIService_*` ABI.

## Implemented boundary

`arc_ai_models.mm` implements seven descriptor/configuration classes;
`arc_ai_ranking.mm` implements seven ranking value classes. They preserve
owned snapshots, nil-string conversion, float/int/double element widths,
array order, optional engagement, and documented integer narrowing. Original
engine-internal native pointer/aggregate selectors are not reproduced, per
`ARCHITECTURE.md`; ranking values are copied directly within their owned file.

`ArcCommandBarRankingExample.candidates` and
`ArcCommandBarRankingTrainingResult.delta` return fresh empty mutable arrays:
original-engine probes observe this even for four populated fixtures. Their
reviewed specs leave conversion semantics unresolved; these implementations
preserve the observed result instead of reconstructing the apparent conversion
intent. Wider semantics remain unresolved. Property declarations use `@dynamic`.

`ai_values_probe.mm` cold-loads exactly the framework path supplied by the
caller and emits sorted JSON for the implemented public value surface. It
does not initialize a browser, load a model/library, or execute AI services.
Ordinary runs exclude the two unresolved getters; `--observe-unresolved`
separately inspects them. The probe accepts only one framework per process,
avoiding duplicate ObjC class registration.

`bash arc/browser/ai_values_probe.sh` serializes the paired probe with the
shared lock, compares raw JSON (including exact method encodings/property
attributes) and runs the existing boundary recorder/differ. Generated evidence
is under `results/ai-values/`. The 2026-10-23 09:04 UTC linked-engine raw
comparison matched exactly: 14 classes, 74 public methods, 60 properties, four
value fixtures, and signed-narrowing grids. Neither probe runs a browser or AI
service; these results establish only the exercised value-transport surface.
The final locked recorder probe captured 414 calls on each engine covering all
74 public methods and found zero differences, including counts and nesting.
Raw and recorded JSON also matched, checking recorder noninterference. The
recorder loads after the framework's ObjC classes register; missing method
coverage fails the harness before a diff is produced.

## Acceptance blockers and lead requests

At the initial census, launch-slice, tabs-navigation, and windows golden
transcripts contained no calls to these classes or `createAIService:`.
An AI/command-bar scenario or explicit original-service probes are required
to establish callbacks, errors, result shapes, cancellation and scheduling.
The scenarios worker was notified on Chirp (#132892, #132918, #132952).
The later 09:11 UTC census found zero AI calls in all 16 finalized transcripts;
candidate directories were excluded.

Execution remains unimplemented: `ArcAIService`, the application
`createAIService:` category, `ArcContentConversionService`, and
`ArcWebContentsAIAgent` retain existing stubs. There is no verified external
library identity/version, model format or observed service callback contract.
Reconstructing utility-process service registration and sandbox integration
requires lead-owned Chromium/build hooks. Direct in-process library loading
would change the original isolation and is not an approved substitute.

Invalid/nil `Example` input remains unverified and currently fails a CHECK.
The configuration invalid-type error path tolerates a null error pointer;
the inspected original pseudocode dereferences it. Neither difference is
claimed to be accepted behavior for invalid callers.

The original engine's actual invalid-type error description is `Invalid type`.
This contradicts the `Invalidtype` text in `specs/ArcAIServiceConfiguration.md`,
which reflects a decompiler constant identifier rather than the literal.

For the remaining `ArcWebContentsAIAgent`, original-engine disassembly recovers
key values 2/3/4/5 as DEL/CR/ESC/TAB with key codes 51/36/53/48, default empty
string/key code zero (`0x473e230`, table `0xe21b208`). Events use zero modifiers,
no repeat, no context, current window number and system uptime. Text input
iterates UTF-16 code units with key code zero. The original performs no focus
change. Cached agent identity is explicit at `0x4441b4c`. However, the exact
frame selected through native Impl+`0x8d8` remains unmapped; choosing main or
focused frame interchangeably would invent semantics. No event implementation
or dynamic input equivalence is claimed.
