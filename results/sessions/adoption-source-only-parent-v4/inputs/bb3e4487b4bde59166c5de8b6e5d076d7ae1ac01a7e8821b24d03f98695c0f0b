// Copyright 2026 ArcMod. AI descriptors, not loaded models. See specs/ArcAI*.md.
// Native C++ aggregate selectors are intentionally private to the original engine;
// this layer implements the ObjC boundary and keeps its own value storage.

#import "arc/api/arc_internal.h"
#include "arc/browser/ai_bridge.h"

#include <cstdint>
#include <optional>
#include <string>

#include "base/check.h"

namespace {

struct InferenceModel {
  int type;
  std::string directory;
};

struct InferenceConfiguration {
  int max_tokens;
  int16_t stream_interval;
};

struct Classifier {
  std::string model_path;
  std::string lora_json;
  std::string config_path;
  std::string head_path;
  int16_t num_classes;
};

struct VersionedModel {
  std::string directory;
  int version;
};

struct EmbedItem {
  std::string text;
  std::string task;
};

struct ServiceConfiguration {
  int type;
  std::string display_name;
  std::string library_path;
  std::string library_version;
  std::string environment;
  std::string app_version;
  int8_t retries;
  double timeout;
};

}  // namespace

ARC_EXPORT @interface ArcAIInferenceModel : NSObject
@property(nonatomic, readonly) NSInteger modelType;
@property(nonatomic, readonly, copy) NSString* directory;
@end

// C++ accessors below copy owned storage without adding app/engine getter calls
// during marshalling. The original uses its internal native-aggregate getters.

@implementation ArcAIInferenceModel {
 @public
  std::optional<InferenceModel> _model;
}

- (instancetype)init:(NSInteger)type directory:(NSString*)directory {
  if ((self = [super init])) {
    _model.emplace(static_cast<int>(type), ArcUTF8(directory));
  }
  return self;
}

- (NSInteger)modelType {
  CHECK(_model);
  return _model->type;
}

- (NSString*)directory {
  CHECK(_model);
  return ArcNSString(_model->directory);
}

@end

ARC_EXPORT @interface ArcAIInferenceConfiguration : NSObject
@property(nonatomic, readonly) int maxTokens;
@property(nonatomic, readonly) int streamAfterEveryNTokens;
@end

@implementation ArcAIInferenceConfiguration {
 @public
  std::optional<InferenceConfiguration> _configuration;
}

- (instancetype)init:(int)maxTokens streamAfterEveryNTokens:(int)interval {
  if ((self = [super init])) {
    _configuration.emplace(maxTokens, static_cast<int16_t>(interval));
  }
  return self;
}

- (int)maxTokens {
  CHECK(_configuration);
  return _configuration->max_tokens;
}

- (int)streamAfterEveryNTokens {
  CHECK(_configuration);
  return _configuration->stream_interval;
}

@end

ARC_EXPORT @interface ArcAIDistilBertClassifier : NSObject
@property(nonatomic, readonly, copy) NSString* modelFilePath;
@property(nonatomic, readonly, copy) NSString* loraConfigJSON;
@property(nonatomic, readonly, copy) NSString* configFilePath;
@property(nonatomic, readonly, copy) NSString* classifierHeadFilePath;
@property(nonatomic, readonly) int numClasses;
@end

@implementation ArcAIDistilBertClassifier {
 @public
  std::optional<Classifier> _classifier;
}

- (instancetype)init:(NSString*)modelPath
          loraConfigJSON:(NSString*)loraJSON
          configFilePath:(NSString*)configPath
  classifierHeadFilePath:(NSString*)headPath
              numClasses:(int)numClasses {
  if ((self = [super init])) {
    _classifier.emplace(ArcUTF8(modelPath), ArcUTF8(loraJSON),
                        ArcUTF8(configPath), ArcUTF8(headPath),
                        static_cast<int16_t>(numClasses));
  }
  return self;
}

- (NSString*)modelFilePath {
  CHECK(_classifier);
  return ArcNSString(_classifier->model_path);
}

- (NSString*)loraConfigJSON {
  CHECK(_classifier);
  return ArcNSString(_classifier->lora_json);
}

- (NSString*)configFilePath {
  CHECK(_classifier);
  return ArcNSString(_classifier->config_path);
}

- (NSString*)classifierHeadFilePath {
  CHECK(_classifier);
  return ArcNSString(_classifier->head_path);
}

- (int)numClasses {
  CHECK(_classifier);
  return _classifier->num_classes;
}

@end

ARC_EXPORT @interface ArcNomicEmbedTextModel : NSObject
@property(nonatomic, readonly, copy) NSString* directory;
@property(nonatomic, readonly) int version;
@end

@implementation ArcNomicEmbedTextModel {
 @public
  std::optional<VersionedModel> _model;
}

- (instancetype)init:(NSString*)directory version:(int)version {
  if ((self = [super init])) {
    _model.emplace(ArcUTF8(directory), version);
  }
  return self;
}

- (NSString*)directory {
  CHECK(_model);
  return ArcNSString(_model->directory);
}

- (int)version {
  CHECK(_model);
  return _model->version;
}

@end

ARC_EXPORT @interface ArcNomicEmbedTextItem : NSObject
@property(nonatomic, readonly, copy) NSString* text;
@property(nonatomic, readonly, copy) NSString* task;
@end

@implementation ArcNomicEmbedTextItem {
 @public
  std::optional<EmbedItem> _item;
}

- (instancetype)init:(NSString*)text task:(NSString*)task {
  if ((self = [super init])) {
    _item.emplace(ArcUTF8(text), ArcUTF8(task));
  }
  return self;
}

- (NSString*)text {
  CHECK(_item);
  return ArcNSString(_item->text);
}

- (NSString*)task {
  CHECK(_item);
  return ArcNSString(_item->task);
}

@end

ARC_EXPORT @interface ArcCommandBarRankingModel : NSObject
@property(nonatomic, readonly, copy) NSString* directory;
@property(nonatomic, readonly) int version;
@end

@implementation ArcCommandBarRankingModel {
 @public
  std::optional<VersionedModel> _model;
}

- (instancetype)init:(NSString*)directory version:(int)version {
  if ((self = [super init])) {
    _model.emplace(ArcUTF8(directory), version);
  }
  return self;
}

- (NSString*)directory {
  CHECK(_model);
  return ArcNSString(_model->directory);
}

- (int)version {
  CHECK(_model);
  return _model->version;
}

@end

ARC_EXPORT @interface ArcAIServiceConfiguration : NSObject
@property(nonatomic, readonly) NSInteger type;
@property(nonatomic, readonly) NSString* displayName;
@property(nonatomic, readonly) NSString* libraryPath;
@property(nonatomic, readonly) NSString* libraryVersion;
@property(nonatomic, readonly) NSString* appEnvironment;
@property(nonatomic, readonly) NSString* fullAppVersion;
@property(nonatomic, readonly) int maxRetries;
@property(nonatomic, readonly) double timeout;
@end

@implementation ArcAIServiceConfiguration {
 @public
  std::optional<ServiceConfiguration> _configuration;
}

- (instancetype)init:(NSInteger)type
           displayName:(NSString*)name
           libraryPath:(NSURL*)path
        libraryVersion:(NSString*)libraryVersion
        appEnvironment:(NSString*)environment
        fullAppVersion:(NSString*)appVersion
            maxRetries:(int)retries
               timeout:(double)timeout
                 error:(NSError**)error {
  if (type == -1) {
    if (error) {
      *error = [NSError errorWithDomain:@"company.thebrowser.Arc"
                                  code:2
                              userInfo:@{NSLocalizedDescriptionKey : @"Invalid type"}];
    }
    return nil;
  }
  if ((self = [super init])) {
    _configuration.emplace(static_cast<int>(type), ArcUTF8(name),
                           ArcUTF8(path.path), ArcUTF8(libraryVersion),
                           ArcUTF8(environment), ArcUTF8(appVersion),
                           static_cast<int8_t>(retries), timeout);
  }
  return self;
}

- (NSInteger)type {
  CHECK(_configuration);
  return _configuration->type;
}

- (NSString*)displayName {
  CHECK(_configuration);
  return ArcNSString(_configuration->display_name);
}

- (NSString*)libraryPath {
  CHECK(_configuration);
  return ArcNSString(_configuration->library_path);
}

- (NSString*)libraryVersion {
  CHECK(_configuration);
  return ArcNSString(_configuration->library_version);
}

- (NSString*)appEnvironment {
  CHECK(_configuration);
  return ArcNSString(_configuration->environment);
}

- (NSString*)fullAppVersion {
  CHECK(_configuration);
  return ArcNSString(_configuration->app_version);
}

- (int)maxRetries {
  CHECK(_configuration);
  return _configuration->retries;
}

- (double)timeout {
  CHECK(_configuration);
  return _configuration->timeout;
}

@end

namespace arc_ai {
bool CopyServiceConfiguration(id object, Input& input) {
  if (![object isKindOfClass:[ArcAIServiceConfiguration class]]) return false;
  const auto& config = ((ArcAIServiceConfiguration*)object)->_configuration;
  if (!config) return false;
  input.service = {static_cast<abi::AIServiceType>(config->type),
                   input.String(config->display_name), input.String(config->library_path),
                   input.String(config->library_version), input.String(config->environment),
                   input.String(config->app_version), config->retries, config->timeout};
  return true;
}
bool CopyInferenceModel(id object, Input& input) {
  if (![object isKindOfClass:[ArcAIInferenceModel class]]) return false;
  const auto& model = ((ArcAIInferenceModel*)object)->_model;
  if (!model) return false;
  input.model = {static_cast<abi::AIInferenceModelType>(model->type), input.String(model->directory)};
  return true;
}
bool CopyInferenceConfiguration(id object, Input& input) {
  if (![object isKindOfClass:[ArcAIInferenceConfiguration class]]) return false;
  const auto& config = ((ArcAIInferenceConfiguration*)object)->_configuration;
  if (!config) return false;
  input.inference = {config->max_tokens, config->stream_interval};
  return true;
}
bool CopyClassifier(id object, Input& input) {
  if (![object isKindOfClass:[ArcAIDistilBertClassifier class]]) return false;
  const auto& model = ((ArcAIDistilBertClassifier*)object)->_classifier;
  if (!model) return false;
  input.classifier = {input.String(model->model_path), input.String(model->lora_json),
                      input.String(model->config_path), input.String(model->head_path), model->num_classes};
  return true;
}
bool CopyNomicModel(id object, Input& input) {
  if (![object isKindOfClass:[ArcNomicEmbedTextModel class]]) return false;
  const auto& model = ((ArcNomicEmbedTextModel*)object)->_model;
  if (!model) return false;
  input.nomic = {input.String(model->directory), model->version};
  return true;
}
bool CopyRankingModel(id object, Input& input) {
  if (![object isKindOfClass:[ArcCommandBarRankingModel class]]) return false;
  const auto& model = ((ArcCommandBarRankingModel*)object)->_model;
  if (!model) return false;
  input.ranking = {input.String(model->directory), model->version};
  return true;
}
void CopyNomicItems(NSArray* objects, Input& input) {
  for (ArcNomicEmbedTextItem* object in objects) {
    CHECK([object isKindOfClass:[ArcNomicEmbedTextItem class]]);
    CHECK(object->_item);
    input.items.push_back({input.String(object->_item->text), input.String(object->_item->task)});
  }
}
}  // namespace arc_ai
