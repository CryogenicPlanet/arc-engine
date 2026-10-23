// Copyright 2026 ArcMod. Command bar ranking values. See specs/ArcCommandBarRanking*.md.
// These objects transport values; they do not run ranking or training.
// Original engine C++ aggregate and native-pointer selectors are intentionally
// omitted. File-local ivars below permit direct copies of our own storage
// without introducing extra ObjC calls at the recorded boundary.

#import "arc/api/arc_internal.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "base/check.h"

namespace {

struct RankingCandidate {
  std::string id;
  std::string archetype;
  std::string destination;
  std::string title;
  std::string url;
  int last_nav_seconds;
  int freq_ever;
  int typed_count;
  float site_engagement;
  bool is_exact_url_match;
  bool is_base_domain_match;
  bool is_title_prefix;
  std::vector<float> title_embedding;
};

struct RankingInput {
  std::vector<float> query_embedding;
};

struct RankingResult {
  std::string id;
  float score;
};

struct RankingTensor {
  std::string name;
  std::vector<int> shape;
  std::vector<float> data;
};

struct RankingMetric {
  std::string name;
  double value;
};

struct RankingEvaluation {
  std::vector<RankingMetric> metrics;
  int num_examples_evaluated;
  int evaluation_duration_ms;
};

struct RankingExample {
  RankingInput input;
  std::vector<RankingCandidate> candidates;
  int selected_index;
};

struct RankingTrainingResult {
  int outcome;
  int failure_reason;
  std::vector<RankingTensor> delta;
  std::string model_name;
  int model_version;
  int num_samples;
  std::vector<float> losses_per_epoch;
  float gradient_norm_mean;
  float gradient_norm_max;
  float delta_norm;
  float delta_to_weight_ratio;
  int training_duration_ms;
};

std::vector<float> UnboxFloats(NSArray<NSNumber*>* values) {
  std::vector<float> result;
  result.reserve(values.count);
  for (NSNumber* value in values) {
    result.push_back(value.floatValue);
  }
  return result;
}

NSMutableArray<NSNumber*>* BoxFloats(const std::vector<float>& values) {
  NSMutableArray<NSNumber*>* result =
      [NSMutableArray arrayWithCapacity:values.size()];
  for (float value : values) {
    [result addObject:[NSNumber numberWithFloat:value]];
  }
  return result;
}

}  // namespace

ARC_EXPORT @interface ArcCommandBarRankingCandidate : NSObject {
 @public
  std::optional<RankingCandidate> _candidate;
}
@property(nonatomic, readonly, copy) NSString* candidateID;
@property(nonatomic, readonly, copy) NSString* archetype;
@property(nonatomic, readonly, copy) NSString* destination;
@property(nonatomic, readonly, copy) NSString* title;
@property(nonatomic, readonly, copy) NSString* url;
@property(nonatomic, readonly) int lastNavSeconds;
@property(nonatomic, readonly) int freqEver;
@property(nonatomic, readonly) int typedCount;
@property(nonatomic, readonly) float siteEngagement;
@property(nonatomic, readonly) BOOL isExactURLMatch;
@property(nonatomic, readonly) BOOL isBaseDomainMatch;
@property(nonatomic, readonly) BOOL isTitlePrefix;
@property(nonatomic, readonly, copy) NSArray<NSNumber*>* titleEmbedding;
@end

@implementation ArcCommandBarRankingCandidate

- (instancetype)init:(NSString*)candidateID
             archetype:(NSString*)archetype
           destination:(NSString*)destination
                 title:(NSString*)title
                   url:(NSString*)url
        lastNavSeconds:(int)lastNavSeconds
              freqEver:(int)freqEver
            typedCount:(int)typedCount
        siteEngagement:(float)siteEngagement
       isExactURLMatch:(BOOL)isExactURLMatch
     isBaseDomainMatch:(BOOL)isBaseDomainMatch
         isTitlePrefix:(BOOL)isTitlePrefix
        titleEmbedding:(NSArray<NSNumber*>*)titleEmbedding {
  if ((self = [super init])) {
    _candidate.emplace(ArcUTF8(candidateID), ArcUTF8(archetype),
                       ArcUTF8(destination), ArcUTF8(title), ArcUTF8(url),
                       lastNavSeconds, freqEver, typedCount, siteEngagement,
                       isExactURLMatch, isBaseDomainMatch, isTitlePrefix,
                       UnboxFloats(titleEmbedding));
  }
  return self;
}

- (NSString*)candidateID {
  CHECK(_candidate);
  return ArcNSString(_candidate->id);
}

- (NSString*)archetype {
  CHECK(_candidate);
  return ArcNSString(_candidate->archetype);
}

- (NSString*)destination {
  CHECK(_candidate);
  return ArcNSString(_candidate->destination);
}

- (NSString*)title {
  CHECK(_candidate);
  return ArcNSString(_candidate->title);
}

- (NSString*)url {
  CHECK(_candidate);
  return ArcNSString(_candidate->url);
}

- (int)lastNavSeconds {
  CHECK(_candidate);
  return _candidate->last_nav_seconds;
}

- (int)freqEver {
  CHECK(_candidate);
  return _candidate->freq_ever;
}

- (int)typedCount {
  CHECK(_candidate);
  return _candidate->typed_count;
}

- (float)siteEngagement {
  CHECK(_candidate);
  return _candidate->site_engagement;
}

- (BOOL)isExactURLMatch {
  CHECK(_candidate);
  return _candidate->is_exact_url_match;
}

- (BOOL)isBaseDomainMatch {
  CHECK(_candidate);
  return _candidate->is_base_domain_match;
}

- (BOOL)isTitlePrefix {
  CHECK(_candidate);
  return _candidate->is_title_prefix;
}

- (NSArray<NSNumber*>*)titleEmbedding {
  CHECK(_candidate);
  return BoxFloats(_candidate->title_embedding);
}

@end

ARC_EXPORT @interface ArcCommandBarRankingInput : NSObject {
 @public
  std::optional<RankingInput> _input;
}
@property(nonatomic, readonly, copy) NSArray<NSNumber*>* queryEmbedding;
@end

@implementation ArcCommandBarRankingInput

- (instancetype)init:(NSArray<NSNumber*>*)queryEmbedding {
  if ((self = [super init])) {
    _input.emplace(UnboxFloats(queryEmbedding));
  }
  return self;
}

- (NSArray<NSNumber*>*)queryEmbedding {
  CHECK(_input);
  return BoxFloats(_input->query_embedding);
}

@end

ARC_EXPORT @interface ArcCommandBarRankingResult : NSObject
@property(nonatomic, readonly, copy) NSString* candidateID;
@property(nonatomic, readonly) float score;
@end

@implementation ArcCommandBarRankingResult {
  std::optional<RankingResult> _result;
}

- (instancetype)init:(NSString*)candidateID score:(float)score {
  if ((self = [super init])) {
    _result.emplace(ArcUTF8(candidateID), score);
  }
  return self;
}

- (NSString*)candidateID {
  CHECK(_result);
  return ArcNSString(_result->id);
}

- (float)score {
  CHECK(_result);
  return _result->score;
}

@end

ARC_EXPORT @interface ArcCommandBarRankingTensor : NSObject {
 @public
  std::optional<RankingTensor> _tensor;
}
@property(nonatomic, readonly, copy) NSString* name;
@property(nonatomic, readonly, copy) NSArray<NSNumber*>* shape;
@property(nonatomic, readonly, copy) NSArray<NSNumber*>* data;
@end

@implementation ArcCommandBarRankingTensor

- (instancetype)init:(NSString*)name
                  shape:(NSArray<NSNumber*>*)shape
                   data:(NSArray<NSNumber*>*)data {
  if ((self = [super init])) {
    std::vector<int> dimensions;
    dimensions.reserve(shape.count);
    for (NSNumber* dimension in shape) {
      dimensions.push_back(dimension.intValue);
    }
    _tensor.emplace(ArcUTF8(name), std::move(dimensions), UnboxFloats(data));
  }
  return self;
}

- (NSString*)name {
  CHECK(_tensor);
  return ArcNSString(_tensor->name);
}

- (NSArray<NSNumber*>*)shape {
  CHECK(_tensor);
  NSMutableArray<NSNumber*>* result =
      [NSMutableArray arrayWithCapacity:_tensor->shape.size()];
  for (int dimension : _tensor->shape) {
    [result addObject:[NSNumber numberWithInt:dimension]];
  }
  return result;
}

- (NSArray<NSNumber*>*)data {
  CHECK(_tensor);
  return BoxFloats(_tensor->data);
}

@end

ARC_EXPORT @interface ArcCommandBarRankingEvaluation : NSObject
@property(nonatomic, readonly, copy) NSDictionary<NSString*, NSNumber*>* metrics;
@property(nonatomic, readonly) int numExamplesEvaluated;
@property(nonatomic, readonly) int evaluationDurationMs;
@end

@implementation ArcCommandBarRankingEvaluation {
  std::optional<RankingEvaluation> _evaluation;
}

- (instancetype)init:(NSDictionary<NSString*, NSNumber*>*)metrics
      numExamplesEvaluated:(int)numExamplesEvaluated
      evaluationDurationMs:(int)evaluationDurationMs {
  if ((self = [super init])) {
    std::vector<RankingMetric> entries;
    entries.reserve(metrics.count);
    for (NSString* name in metrics) {
      entries.push_back({ArcUTF8(name), metrics[name].doubleValue});
    }
    _evaluation.emplace(std::move(entries), numExamplesEvaluated,
                        evaluationDurationMs);
  }
  return self;
}

- (NSDictionary<NSString*, NSNumber*>*)metrics {
  CHECK(_evaluation);
  NSMutableDictionary<NSString*, NSNumber*>* result =
      [NSMutableDictionary dictionaryWithCapacity:_evaluation->metrics.size()];
  for (const RankingMetric& metric : _evaluation->metrics) {
    result[ArcNSString(metric.name)] = [NSNumber numberWithDouble:metric.value];
  }
  return result;
}

- (int)numExamplesEvaluated {
  CHECK(_evaluation);
  return _evaluation->num_examples_evaluated;
}

- (int)evaluationDurationMs {
  CHECK(_evaluation);
  return _evaluation->evaluation_duration_ms;
}

@end

ARC_EXPORT @interface ArcCommandBarRankingExample : NSObject
@property(nonatomic, readonly) NSArray* candidates;
@property(nonatomic, readonly) ArcCommandBarRankingInput* input;
@property(nonatomic, readonly) int selectedIndex;
@end

@implementation ArcCommandBarRankingExample {
  std::optional<RankingExample> _example;
}

@dynamic candidates;

@dynamic input;

- (instancetype)init:(ArcCommandBarRankingInput*)input
             candidates:(NSArray<ArcCommandBarRankingCandidate*>*)candidates
          selectedIndex:(int)selectedIndex {
  if ((self = [super init])) {
    CHECK(input);
    CHECK(input->_input);
    std::vector<RankingCandidate> values;
    values.reserve(candidates.count);
    for (ArcCommandBarRankingCandidate* candidate in candidates) {
      CHECK(candidate);
      CHECK(candidate->_candidate);
      values.push_back(*candidate->_candidate);
    }
    _example.emplace(*input->_input, std::move(values),
                     selectedIndex);
  }
  return self;
}

- (ArcCommandBarRankingInput*)input {
  CHECK(_example);
  return [[ArcCommandBarRankingInput alloc]
      init:BoxFloats(_example->input.query_embedding)];
}

- (int)selectedIndex {
  CHECK(_example);
  return _example->selected_index;
}

- (NSArray*)candidates {
  CHECK(_example);
  // Original-engine cold-load probes return an empty array even for a populated
  // candidate vector. Preserve that observation, not the apparent conversion
  // intent in the incoherent decompilation. Wider semantics remain unresolved.
  return [NSMutableArray array];
}

@end

ARC_EXPORT @interface ArcCommandBarRankingTrainingResult : NSObject
@property(nonatomic, readonly) NSArray* delta;
@property(nonatomic, readonly) NSInteger outcome;
@property(nonatomic, readonly) NSInteger failureReason;
@property(nonatomic, readonly, copy) NSString* modelName;
@property(nonatomic, readonly) int modelVersion;
@property(nonatomic, readonly) int numSamples;
@property(nonatomic, readonly, copy) NSArray<NSNumber*>* lossesPerEpoch;
@property(nonatomic, readonly) float gradientNormMean;
@property(nonatomic, readonly) float gradientNormMax;
@property(nonatomic, readonly) float deltaNorm;
@property(nonatomic, readonly) float deltaToWeightRatio;
@property(nonatomic, readonly) int trainingDurationMs;
@end

@implementation ArcCommandBarRankingTrainingResult {
  std::optional<RankingTrainingResult> _result;
}

@dynamic delta;

- (instancetype)init:(NSInteger)outcome
              failureReason:(NSInteger)failureReason
                      delta:(NSArray<ArcCommandBarRankingTensor*>*)delta
                  modelName:(NSString*)modelName
               modelVersion:(int)modelVersion
                 numSamples:(int)numSamples
             lossesPerEpoch:(NSArray<NSNumber*>*)lossesPerEpoch
           gradientNormMean:(float)gradientNormMean
            gradientNormMax:(float)gradientNormMax
                  deltaNorm:(float)deltaNorm
         deltaToWeightRatio:(float)deltaToWeightRatio
         trainingDurationMs:(int)trainingDurationMs {
  if ((self = [super init])) {
    std::vector<RankingTensor> tensors;
    tensors.reserve(delta.count);
    for (ArcCommandBarRankingTensor* tensor in delta) {
      CHECK(tensor);
      CHECK(tensor->_tensor);
      tensors.push_back(*tensor->_tensor);
    }
    _result.emplace(static_cast<int>(outcome), static_cast<int>(failureReason),
                    std::move(tensors), ArcUTF8(modelName), modelVersion,
                    numSamples, UnboxFloats(lossesPerEpoch), gradientNormMean,
                    gradientNormMax, deltaNorm, deltaToWeightRatio,
                    trainingDurationMs);
  }
  return self;
}

- (NSInteger)outcome {
  CHECK(_result);
  return _result->outcome;
}

- (NSInteger)failureReason {
  CHECK(_result);
  return _result->failure_reason;
}

- (NSString*)modelName {
  CHECK(_result);
  return ArcNSString(_result->model_name);
}

- (int)modelVersion {
  CHECK(_result);
  return _result->model_version;
}

- (int)numSamples {
  CHECK(_result);
  return _result->num_samples;
}

- (NSArray<NSNumber*>*)lossesPerEpoch {
  CHECK(_result);
  return BoxFloats(_result->losses_per_epoch);
}

- (float)gradientNormMean {
  CHECK(_result);
  return _result->gradient_norm_mean;
}

- (float)gradientNormMax {
  CHECK(_result);
  return _result->gradient_norm_max;
}

- (float)deltaNorm {
  CHECK(_result);
  return _result->delta_norm;
}

- (float)deltaToWeightRatio {
  CHECK(_result);
  return _result->delta_to_weight_ratio;
}

- (int)trainingDurationMs {
  CHECK(_result);
  return _result->training_duration_ms;
}

- (NSArray*)delta {
  CHECK(_result);
  // Like -candidates, the original getter returns an empty array for the
  // populated tensors exercised by ai_values_probe. Do not synthesize a delta
  // from the stored tensors without contrary runtime evidence.
  return [NSMutableArray array];
}

@end
