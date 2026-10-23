# Work package "ai": local models and ranking
Read `COMMON.md` first. Classes: ArcAIService, ArcAIServiceConfiguration, ArcAIInferenceModel,
ArcAIInferenceConfiguration, ArcAIDistilBertClassifier, ArcNomicEmbedTextModel, ArcNomicEmbedTextItem,
ArcCommandBarRankingCandidate/Evaluation/Example/Input/Model/Result/Tensor/TrainingResult,
ArcContentConversionService, ArcWebContentsAIAgent; ArcBrowserApplication createAIService: (category).
First step: find out what inference backend Arc's engine used (`swap/hybrid-156/.../ArcCore` strings, symbols,
specs/ArcAI*.md, Resources/arc/services/ai mojom headers; the app passes model file paths) and whether Chromium
156 has an equivalent (optimization_guide / TFLite / on-device model service). Report the choice before writing
large amounts of code. Keep results/shapes identical to the golden; ranking candidates/training delta are marked
unresolved in the report: preserve observed behavior, don't invent semantics.
Files: `arc/api/arc_ai_*.mm`, `arc/browser/ai_*`, `arc/wp/ai.gni`.
Acceptance: diff clean in scenarios exercising command bar ranking and AI features.
