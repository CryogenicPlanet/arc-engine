// Copyright 2026 ArcMod. Exact app-library AI C ABI, hosted in-process.
// The original hosts this library in a utility process. No inference, ranking,
// retry, or timeout policy is implemented here: configuration goes to the library.
#import "arc/api/arc_internal.h"
#include "arc/browser/ai_bridge.h"
#include "arc/browser/ai_transport.h"
#include "arc/browser/ai_stream_transport.h"
#include "base/functional/bind.h"
#include "base/immediate_crash.h"
#include <unistd.h>
#include "base/debug/debugger.h"
#include "base/memory/raw_ptr_exclusion.h"

namespace {
namespace abi = mojoc;
using ErrorBlock = void (^)(NSInteger);
using ValueBlock = void (^)(id, NSInteger);

uint16_t NextRequestID() {
  static std::atomic<uint16_t> next{0};
  return next++;
}

struct AIState {
  std::shared_ptr<arc_ai::Lifetime> lifetime = std::make_shared<arc_ai::Lifetime>();
  dispatch_queue_t queue = dispatch_get_main_queue();
  std::atomic<bool> available{false};
  RAW_PTR_EXCLUSION void* module = nullptr;
  abi::AIServiceHandle handle = nullptr;
#define AI_SYMBOL(name) decltype(&abi::AIService_##name) name = nullptr;
  AI_SYMBOL(Create)
  AI_SYMBOL(Initialize)
  AI_SYMBOL(SetUser)
  AI_SYMBOL(EnableDataDogTracing)
  AI_SYMBOL(Type)
  AI_SYMBOL(WaitForDebuggerToAttach)
  AI_SYMBOL(Crash)
  AI_SYMBOL(UpdateFeatureFlags)
  AI_SYMBOL(LoadInferenceModel)
  AI_SYMBOL(GetStreamedInferenceFor)
  AI_SYMBOL(LoadClassifier)
  AI_SYMBOL(IsClassifierLoaded)
  AI_SYMBOL(GetClassificationFor)
  AI_SYMBOL(LoadNomicEmbedText)
  AI_SYMBOL(NomicEmbed)
  AI_SYMBOL(LoadCommandBarRanking)
  AI_SYMBOL(RankCommandBar)
  AI_SYMBOL(EvaluateCommandBarRanking)
  AI_SYMBOL(TrainCommandBarRanking)
#undef AI_SYMBOL

  NSInteger Load(const std::string& path) {
    if (path.empty()) return abi::AIServiceErrorInvalidLibraryPath;
    if (access(path.c_str(), F_OK)) return abi::AIServiceErrorLibraryNotFound;
    module = arc_ai::OpenLibrary(path);
    if (!module) return abi::AIServiceErrorLibraryMissingRequiredSymbols;
    for (const char* name : abi::Symbols) {
      if (std::string_view(name) == abi::kSymbolAIService_GetEmbeddingsFor) continue;
      if (!dlsym(module, name)) return abi::AIServiceErrorLibraryMissingRequiredSymbols;
    }
#define AI_SYMBOL(name) name = arc_ai::Symbol<decltype(name)>(module, abi::kSymbolAIService_##name);
    AI_SYMBOL(Create)
    AI_SYMBOL(Initialize)
    AI_SYMBOL(SetUser)
    AI_SYMBOL(EnableDataDogTracing)
    AI_SYMBOL(Type)
    AI_SYMBOL(WaitForDebuggerToAttach)
    AI_SYMBOL(Crash)
    AI_SYMBOL(UpdateFeatureFlags)
    AI_SYMBOL(LoadInferenceModel)
    AI_SYMBOL(GetStreamedInferenceFor)
    AI_SYMBOL(LoadClassifier)
    AI_SYMBOL(IsClassifierLoaded)
    AI_SYMBOL(GetClassificationFor)
    AI_SYMBOL(LoadNomicEmbedText)
    AI_SYMBOL(NomicEmbed)
    AI_SYMBOL(LoadCommandBarRanking)
    AI_SYMBOL(RankCommandBar)
    AI_SYMBOL(EvaluateCommandBarRanking)
    AI_SYMBOL(TrainCommandBarRanking)
#undef AI_SYMBOL
    handle = Create();
    return handle ? abi::AIServiceErrorNone : abi::AIServiceErrorIllegalState;
  }
};

NSMutableArray* Floats(abi::FloatArray values) {
  NSMutableArray* result = [NSMutableArray arrayWithCapacity:values.length];
  for (unsigned int i = 0; i < values.length; ++i) [result addObject:@(arc_ai::Buffer(values.begin, values.length)[i])];
  return result;
}
void ErrorReply(abi::AIServiceError error, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  arc_ai::Deliver(pending, ^{ ((ErrorBlock)pending->completion)(error); });
  }
}
void IgnoredReply(abi::UserData data) { arc_ai::Find(data); }
void TypeReply(abi::AIErrorOrServiceType* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  NSInteger type = result->type == abi::AIErrorOrServiceType::AIErrorOrServiceTypeEnumService_type
                       ? result->value.serviceType : -1;
  NSInteger error = type == -1 ? result->value.error : -1;
  // A service may legitimately report type None; use the union tag for errors.
  if (result->type == abi::AIErrorOrServiceType::AIErrorOrServiceTypeEnumService_type) error = -1;
  arc_ai::Deliver(pending, ^{ ((void (^)(NSInteger, NSInteger))pending->completion)(type, error); });
  }
}
void FlagsReply(bool value, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (pending) arc_ai::Deliver(pending, ^{ ((void (^)(BOOL))pending->completion)(value); });
  }
}
void LoadedReply(abi::AIErrorOrLoaded* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  BOOL loaded = NO;
  NSInteger error = -1;
  if (result->type == abi::AIErrorOrLoaded::AIErrorOrLoadedEnumError) error = result->value.error;
  else loaded = result->value.loaded.isLoaded;
  arc_ai::Deliver(pending, ^{ ((void (^)(BOOL, NSInteger))pending->completion)(loaded, error); });
  }
}
void ClassificationReply(abi::AIErrorOrClassification* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  const bool failed = result->type == abi::AIErrorOrClassification::AIErrorOrClassificationEnumError;
  id value = failed ? nil : Floats(result->value.classification.classification);
  NSInteger error = failed ? result->value.error : -1;
  arc_ai::Deliver(pending, ^{ ((ValueBlock)pending->completion)(value, error); });
  }
}
void NomicReply(abi::AIErrorOrNomicEmbeddings* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  NSMutableArray* shape = nil;
  id values = nil;
  NSInteger error = -1;
  if (result->type == abi::AIErrorOrNomicEmbeddings::AIErrorOrNomicEmbeddingsEnumError) error = result->value.error;
  else {
    const auto& embeddings = result->value.embeddings;
    shape = [NSMutableArray arrayWithCapacity:embeddings.shapeLength];
    for (unsigned int i = 0; i < embeddings.shapeLength; ++i) [shape addObject:@(arc_ai::Buffer(embeddings.shape, embeddings.shapeLength)[i])];
    values = Floats(embeddings.data);
  }
  arc_ai::Deliver(pending, ^{ ((void (^)(id, id, NSInteger))pending->completion)(shape, values, error); });
  }
}
void RankReply(abi::AIErrorOrCommandBarRankingResults* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  id values = @[];
  NSInteger error = -1;
  if (result->type == abi::AIErrorOrCommandBarRankingResults::AIErrorOrCommandBarRankingResultsEnumError) error = result->value.error;
  else {
    NSMutableArray* rows = [NSMutableArray arrayWithCapacity:result->value.results.resultsLength];
    for (unsigned int i = 0; i < result->value.results.resultsLength; ++i) [rows addObject:arc_ai::RankingResult(arc_ai::Buffer(result->value.results.results, result->value.results.resultsLength)[i])];
    values = rows;
  }
  arc_ai::Deliver(pending, ^{
    id output = values;
    if (error == -1) {
      NSMutableArray* rows = [NSMutableArray arrayWithCapacity:[values count]];
      for (arc_ai::Factory factory in values) [rows addObject:factory()];
      output = rows;
    }
    ((ValueBlock)pending->completion)(output, error);
  });
  }
}
void EvaluationReply(abi::AIErrorOrCommandBarRankingEvaluation* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  const bool failed = result->type == abi::AIErrorOrCommandBarRankingEvaluation::AIErrorOrCommandBarRankingEvaluationEnumError;
  arc_ai::Factory value = failed ? (arc_ai::Factory)nil : arc_ai::RankingEvaluation(result->value.evaluation);
  NSInteger error = failed ? result->value.error : -1;
  arc_ai::Deliver(pending, ^{ ((ValueBlock)pending->completion)(value ? value() : nil, error); });
  }
}
void TrainingReply(abi::AIErrorOrCommandBarRankingTrainingResult* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  const bool failed = result->type == abi::AIErrorOrCommandBarRankingTrainingResult::AIErrorOrCommandBarRankingTrainingResultEnumError;
  arc_ai::Factory value = failed ? (arc_ai::Factory)nil : arc_ai::RankingTrainingResult(result->value.trainingResult);
  NSInteger error = failed ? result->value.error : -1;
  arc_ai::Deliver(pending, ^{ ((ValueBlock)pending->completion)(value ? value() : nil, error); });
  }
}
void StreamReply(abi::AIServiceError error, abi::UserData data) {
  auto pending = arc_ai::Find(data, false);
  if (pending && pending->context) {
    std::static_pointer_cast<arc_ai::StreamTransport>(pending->context)->Finish(error);
  }
}
void PartialReply(abi::AIStreamedInferenceListenerHandle listener, abi::String text, abi::UserData ignored) {
  const uint64_t key = reinterpret_cast<uintptr_t>(listener);
  auto pending = arc_ai::Find(abi::UserData{static_cast<uint16_t>(key), key}, false);
  if (pending && pending->context) {
    std::static_pointer_cast<arc_ai::StreamTransport>(pending->context)->SendPartial(
        text.length ? std::string(text.string, text.length) : std::string());
  }
}

// All data is copied on the UI caller's stack before sending to the serial
// library sequence. Holding pending also covers a synchronous C callback.
template <typename Invoke>
void Send(std::shared_ptr<AIState> state, std::shared_ptr<arc_ai::Input> input,
          id completion, Invoke invoke, id partial = nil, bool streamed = false) {
  auto pending = arc_ai::Request(completion, partial, state->lifetime, input->texts.size());
  auto data = arc_ai::Register<abi::UserData>(pending);
  if (streamed) {
    pending->context = arc_ai::CreateStreamTransport(
        base::BindRepeating([](abi::UserData data, std::string text) {
          auto request = arc_ai::Find(data, false);
          void (^partial)(NSString*) = request ? request->partial : nil;
          if (request && request->owner->alive.load() && partial) {
            partial(ArcNSString(text));
          }
        }, data),
        base::BindOnce([](abi::UserData data, int error) {
          auto request = arc_ai::Find(data);
          if (request && request->owner->alive.load()) {
            ErrorBlock completion = request->completion;
            completion(error);
          }
        }, data));
  }
  dispatch_async(state->queue, ^{
    invoke(state, input, data);
    (void)pending;
  });
}
}  // namespace

ARC_EXPORT @interface ArcAIService : NSObject
@end
@implementation ArcAIService {
  std::shared_ptr<AIState> _state;
}
- (instancetype)initWithAIService:(const void*)unused {
  if ((self = [super init])) _state = std::make_shared<AIState>();
  return self;
}
- (void)dealloc {
  if (_state) arc_ai::Invalidate(_state->lifetime);
}
- (BOOL)isAvailable { return _state && _state->available.load(); }
- (void)initialize:(id)configuration callback:(ErrorBlock)callback {
  auto input = std::make_shared<arc_ai::Input>();
  if (!arc_ai::CopyServiceConfiguration(configuration, *input)) { callback(1); return; }
  if (!_state) _state = std::make_shared<AIState>();
  if (_state->available.exchange(true)) { callback(4); return; }
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    const auto& path = input->service.libraryPath;
    NSInteger error = state->Load(std::string(path.string, path.length));
    if (error != -1) { ErrorReply(static_cast<abi::AIServiceError>(error), data); return; }
    abi::AIServiceConfiguration config{};
    config.type = input->service.type;
    config.appEnvironment = input->service.appEnvironment;
    config.fullAppVersion = input->service.fullAppVersion;
    // Original utility C shim strips display/path/version/retries/timeout.
    input->service = config;
    state->Initialize(state->handle, &input->service, ErrorReply, data);
  });
}
- (void)type:(void (^)(NSInteger, NSInteger))callback {
  if (!_state || !_state->available.load()) { callback(-1, 3); return; }
  Send(_state, std::make_shared<arc_ai::Input>(), callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::AIErrorOrServiceType result{}; result.type = abi::AIErrorOrServiceType::AIErrorOrServiceTypeEnumError; result.value.error = abi::AIServiceErrorNotInitialized; TypeReply(&result, data); return; }
    state->Type(state->handle, TypeReply, data);
  });
}
- (void)setUser:(NSString*)user email:(NSString*)email callback:(ErrorBlock)callback {
  if (!_state || !_state->available.load()) { callback(3); return; }
  (void)NextRequestID();
  auto input = std::make_shared<arc_ai::Input>();
  input->texts = {input->String(ArcUTF8(user)), input->String(ArcUTF8(email))};
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { ErrorReply(abi::AIServiceErrorNotInitialized, data); return; }
    state->SetUser(state->handle, input->texts[0], input->texts[1], ErrorReply, data);
  });
}
- (void)enableDataDogTracing:(char)rate callback:(ErrorBlock)callback {
  if (!_state || !_state->available.load()) { callback(3); return; }
  (void)NextRequestID();
  Send(_state, std::make_shared<arc_ai::Input>(), callback, [rate](auto state, auto input, auto data) {
    if (!state->handle) { ErrorReply(abi::AIServiceErrorNotInitialized, data); return; }
    state->EnableDataDogTracing(state->handle, rate, ErrorReply, data);
  });
}
- (void)waitForDebuggerToAttach:(int)seconds {
  if (!_state || !_state->available.load()) return;
  Send(_state, std::make_shared<arc_ai::Input>(), nil, [seconds](auto state, auto input, auto data) {
    if (!state->handle) { IgnoredReply(data); return; }
    base::debug::WaitForDebugger(static_cast<uint16_t>(seconds), false);
    IgnoredReply(data);
  });
}
- (void)crash {
  if (!_state || !_state->available.load()) return;
  Send(_state, std::make_shared<arc_ai::Input>(), nil, [](auto state, auto input, auto data) {
    if (!state->handle) { IgnoredReply(data); return; }
    // Utility acknowledges its Mojo request, then passes a null C closure.
    IgnoredReply(data);
    state->Crash(state->handle, nullptr, data);
  });
}
- (void)updateFeatureFlags:(NSString*)flags completion:(void (^)(BOOL))callback {
  if (!_state || !_state->available.load()) { callback(NO); return; }
  auto input = std::make_shared<arc_ai::Input>(); input->texts.push_back(input->String(ArcUTF8(flags)));
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { FlagsReply(false, data); return; }
    // The original utility override completes false without invoking the C symbol.
    FlagsReply(false, data);
  });
}
- (void)embeddingsFor:(NSArray<NSString*>*)texts method:(NSInteger)method model:(NSInteger)model completion:(ValueBlock)callback {
  if (!_state || !_state->available.load()) { callback(nil, 3); return; }
  dispatch_async(_state->queue, ^{
    // Original utility vtable slot +0x48 is the fatal stub at 0x2076b80;
    // it never dispatches AIService_GetEmbeddingsFor or returns a result.
    // In-process hosting moves this fatal boundary into the browser process.
    base::ImmediateCrash();
  });
}
#define AI_LOAD(selector, copy, field, symbol, tracked) \
- (void)selector:(id)model completion:(ErrorBlock)callback { \
  if (!_state || !_state->available.load()) { callback(3); return; } \
  if (tracked) (void)NextRequestID(); \
  auto input = std::make_shared<arc_ai::Input>(); \
  if (!arc_ai::copy(model, *input)) { callback(1); return; } \
  Send(_state, input, callback, [](auto state, auto input, auto data) { \
    if (!state->handle) { ErrorReply(abi::AIServiceErrorNotInitialized, data); return; } \
    state->symbol(state->handle, &input->field, ErrorReply, data); \
  }); \
}
AI_LOAD(loadInferenceModel, CopyInferenceModel, model, LoadInferenceModel, false)
AI_LOAD(loadClassifier, CopyClassifier, classifier, LoadClassifier, true)
AI_LOAD(loadNomicEmbedText, CopyNomicModel, nomic, LoadNomicEmbedText, true)
AI_LOAD(loadCommandBarRanking, CopyRankingModel, ranking, LoadCommandBarRanking, true)
#undef AI_LOAD
- (unsigned short)streamInference:(NSString*)prompt configuration:(id)config model:(id)model
            partialResultCallback:(void (^)(NSString*))partial completion:(ErrorBlock)callback {
  if (!_state || !_state->available.load()) { callback(3); return 0; }
  auto input = std::make_shared<arc_ai::Input>();
  if (!arc_ai::CopyInferenceModel(model, *input) || !arc_ai::CopyInferenceConfiguration(config, *input)) { callback(1); return 0; }
  input->texts.push_back(input->String(ArcUTF8(prompt)));
  const uint16_t token = NextRequestID();
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { StreamReply(abi::AIServiceErrorNotInitialized, data); return; }
    state->GetStreamedInferenceFor(state->handle, input->texts[0], &input->inference, &input->model,
        reinterpret_cast<void*>(static_cast<uintptr_t>(data.pointer)), {PartialReply}, StreamReply, data);
  }, partial, true);
  return token;
}
- (void)isClassifierLoaded:(id)model completion:(void (^)(BOOL, NSInteger))callback {
  if (!_state || !_state->available.load()) { callback(NO, 3); return; }
  (void)NextRequestID();
  auto input = std::make_shared<arc_ai::Input>();
  if (!arc_ai::CopyClassifier(model, *input)) { callback(NO, 1); return; }
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::AIErrorOrLoaded r{}; r.type = abi::AIErrorOrLoaded::AIErrorOrLoadedEnumError; r.value.error = abi::AIServiceErrorNotInitialized; LoadedReply(&r, data); return; }
    state->IsClassifierLoaded(state->handle, &input->classifier, LoadedReply, data);
  });
}
- (void)getClassificationFor:(NSString*)text classifier:(id)model completion:(ValueBlock)callback {
  if (!_state || !_state->available.load()) { callback(nil, 3); return; }
  (void)NextRequestID();
  auto input = std::make_shared<arc_ai::Input>();
  if (!arc_ai::CopyClassifier(model, *input)) { callback(nil, 1); return; }
  input->texts.push_back(input->String(ArcUTF8(text)));
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::AIErrorOrClassification r{}; r.type = abi::AIErrorOrClassification::AIErrorOrClassificationEnumError; r.value.error = abi::AIServiceErrorNotInitialized; ClassificationReply(&r, data); return; }
    state->GetClassificationFor(state->handle, input->texts[0], &input->classifier, ClassificationReply, data);
  });
}
- (void)nomicEmbed:(NSArray*)items completion:(void (^)(id, id, NSInteger))callback {
  if (!_state || !_state->available.load()) { callback(nil, nil, 3); return; }
  (void)NextRequestID();
  auto input = std::make_shared<arc_ai::Input>(); arc_ai::CopyNomicItems(items, *input);
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::AIErrorOrNomicEmbeddings r{}; r.type = abi::AIErrorOrNomicEmbeddings::AIErrorOrNomicEmbeddingsEnumError; r.value.error = abi::AIServiceErrorNotInitialized; NomicReply(&r, data); return; }
    state->NomicEmbed(state->handle, input->items.data(), input->items.size(), NomicReply, data);
  });
}
- (void)rankCommandBar:(id)rankingInput candidates:(NSArray*)candidates completion:(ValueBlock)callback {
  if (!_state || !_state->available.load()) { callback(@[], 3); return; }
  (void)NextRequestID();
  auto input = std::make_shared<arc_ai::Input>();
  if (!arc_ai::CopyRankingInput(rankingInput, *input)) { callback(@[], 1); return; }
  arc_ai::CopyRankingCandidates(candidates, *input);
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::AIErrorOrCommandBarRankingResults r{}; r.type = abi::AIErrorOrCommandBarRankingResults::AIErrorOrCommandBarRankingResultsEnumError; r.value.error = abi::AIServiceErrorNotInitialized; RankReply(&r, data); return; }
    state->RankCommandBar(state->handle, &input->ranking_input, input->candidates.data(), input->candidates.size(), RankReply, data);
  });
}
#define AI_EXAMPLES(selector, symbol, reply, ResultType, errorTag) \
- (void)selector:(NSArray*)examples completion:(ValueBlock)callback { \
  if (!_state || !_state->available.load()) { callback(nil, 3); return; } \
  (void)NextRequestID(); \
  auto input = std::make_shared<arc_ai::Input>(); arc_ai::CopyRankingExamples(examples, *input); \
  Send(_state, input, callback, [](auto state, auto input, auto data) { \
    if (!state->handle) { abi::ResultType r{}; r.type = abi::ResultType::errorTag; r.value.error = abi::AIServiceErrorNotInitialized; reply(&r, data); return; } \
    state->symbol(state->handle, input->examples.data(), input->examples.size(), reply, data); \
  }); \
}
AI_EXAMPLES(evaluateCommandBarRanking, EvaluateCommandBarRanking, EvaluationReply, AIErrorOrCommandBarRankingEvaluation, AIErrorOrCommandBarRankingEvaluationEnumError)
AI_EXAMPLES(trainCommandBarRanking, TrainCommandBarRanking, TrainingReply, AIErrorOrCommandBarRankingTrainingResult, AIErrorOrCommandBarRankingTrainingResultEnumError)
#undef AI_EXAMPLES
@end

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AI)
- (ArcAIService*)createAIService:(NSError**)unused {
  return [[ArcAIService alloc] initWithAIService:nullptr];
}
@end
