// Bounded DTO-only probe. Does not initialize a browser or call AI services,
// or native-pointer APIs. Pass --observe-unresolved to separately inspect
// Example.candidates and TrainingResult.delta; ordinary runs exclude them.
// Standalone build (no ArcCore linkage):
// clang++ -fobjc-arc -framework Foundation arc/browser/ai_values_probe.mm \
//   -o /tmp/arc-ai-values-probe
// Usage: /tmp/arc-ai-values-probe /absolute/path/to/ArcCore [--observe-unresolved]
// Output is observational evidence, not scenario or execution acceptance.

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#include <dlfcn.h>

@interface ArcAIInferenceModel : NSObject
- (instancetype)init:(NSInteger)t directory:(NSString*)d;
@end
@interface ArcAIInferenceConfiguration : NSObject
- (instancetype)init:(int)m streamAfterEveryNTokens:(int)s;
@end
@interface ArcAIDistilBertClassifier : NSObject
- (instancetype)init:(NSString*)p loraConfigJSON:(NSString*)l
    configFilePath:(NSString*)c classifierHeadFilePath:(NSString*)h numClasses:(int)n;
@end
@interface ArcNomicEmbedTextModel : NSObject
- (instancetype)init:(NSString*)d version:(int)v;
@end
@interface ArcNomicEmbedTextItem : NSObject
- (instancetype)init:(NSString*)t task:(NSString*)s;
@end
@interface ArcCommandBarRankingModel : NSObject
- (instancetype)init:(NSString*)d version:(int)v;
@end
@interface ArcAIServiceConfiguration : NSObject
- (instancetype)init:(NSInteger)t displayName:(NSString*)n libraryPath:(NSURL*)p
    libraryVersion:(NSString*)v appEnvironment:(NSString*)e fullAppVersion:(NSString*)a
    maxRetries:(int)r timeout:(double)s error:(NSError**)err;
@end
@interface ArcCommandBarRankingCandidate : NSObject
- (instancetype)init:(NSString*)i archetype:(NSString*)a destination:(NSString*)d
    title:(NSString*)t url:(NSString*)u lastNavSeconds:(int)l freqEver:(int)f
    typedCount:(int)c siteEngagement:(float)s isExactURLMatch:(BOOL)e
    isBaseDomainMatch:(BOOL)b isTitlePrefix:(BOOL)p titleEmbedding:(NSArray*)v;
@end
@interface ArcCommandBarRankingInput : NSObject
- (instancetype)init:(NSArray*)q;
@end
@interface ArcCommandBarRankingResult : NSObject
- (instancetype)init:(NSString*)i score:(float)s;
@end
@interface ArcCommandBarRankingTensor : NSObject
- (instancetype)init:(NSString*)n shape:(NSArray*)s data:(NSArray*)d;
@end
@interface ArcCommandBarRankingEvaluation : NSObject
- (instancetype)init:(NSDictionary*)m numExamplesEvaluated:(int)n evaluationDurationMs:(int)d;
@end
@interface ArcCommandBarRankingExample : NSObject
- (instancetype)init:(ArcCommandBarRankingInput*)i candidates:(NSArray*)c selectedIndex:(int)s;
@end
@interface ArcCommandBarRankingTrainingResult : NSObject
- (instancetype)init:(NSInteger)o failureReason:(NSInteger)f delta:(NSArray*)d
    modelName:(NSString*)n modelVersion:(int)v numSamples:(int)s lossesPerEpoch:(NSArray*)l
    gradientNormMean:(float)m gradientNormMax:(float)x deltaNorm:(float)dn
    deltaToWeightRatio:(float)r trainingDurationMs:(int)t;
@end

static id JsonValue(id value) { return value ?: NSNull.null; }

static NSArray<NSString*>* Fields(NSString* names) {
  return [names componentsSeparatedByString:@" "];
}

static NSDictionary* Snapshot(id object, NSArray<NSString*>* fields) {
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (NSString* field in fields)
    result[field] = JsonValue([object valueForKey:field]);
  return result;
}

static NSDictionary* Boxed(NSArray<NSNumber*>* values) {
  NSMutableArray* types = [NSMutableArray array];
  for (NSNumber* value in values)
    [types addObject:[NSString stringWithUTF8String:value.objCType]];
  return @{@"values": JsonValue(values), @"objCTypes": types};
}

static NSDictionary* Surface(NSString* name, NSArray<NSString*>* fields,
                             NSString* initializer) {
  Class cls = NSClassFromString(name);
  NSMutableDictionary* methods = [NSMutableDictionary dictionary];
  for (NSString* selector in [fields arrayByAddingObject:initializer]) {
    Method method = class_getInstanceMethod(cls, NSSelectorFromString(selector));
    methods[selector] = method ? [NSString stringWithUTF8String:method_getTypeEncoding(method)]
                               : @"MISSING";
  }
  NSMutableDictionary* properties = [NSMutableDictionary dictionary];
  unsigned int count = 0;
  objc_property_t* list = class_copyPropertyList(cls, &count);
  for (unsigned int i = 0; i < count; ++i) {
    NSString* key = [NSString stringWithUTF8String:property_getName(list[i])];
    // Only the implemented, public value surface; no aggregates or unresolved getters.
    if ([fields containsObject:key])
      properties[key] = [NSString stringWithUTF8String:property_getAttributes(list[i])];
  }
  free(list);
  return @{@"methods": methods, @"properties": properties};
}

int main(int argc, char** argv) {
  @autoreleasepool {
    if (argc != 2 && argc != 3) return 2;
    BOOL observeUnresolved = argc == 3 &&
        [@(argv[2]) isEqualToString:@"--observe-unresolved"];
    if (argc == 3 && !observeUnresolved) return 2;
    if (!dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)) {
      fprintf(stderr, "%s\n", dlerror());
      return 3;
    }
    NSDictionary<NSString*, NSString*>* fieldNames = @{
      @"ArcAIInferenceModel": @"modelType directory",
      @"ArcAIInferenceConfiguration": @"maxTokens streamAfterEveryNTokens",
      @"ArcAIDistilBertClassifier": @"modelFilePath loraConfigJSON configFilePath classifierHeadFilePath numClasses",
      @"ArcNomicEmbedTextModel": @"directory version",
      @"ArcNomicEmbedTextItem": @"text task",
      @"ArcCommandBarRankingModel": @"directory version",
      @"ArcAIServiceConfiguration": @"type displayName libraryPath libraryVersion appEnvironment fullAppVersion maxRetries timeout",
      @"ArcCommandBarRankingCandidate": @"candidateID archetype destination title url lastNavSeconds freqEver typedCount siteEngagement isExactURLMatch isBaseDomainMatch isTitlePrefix titleEmbedding",
      @"ArcCommandBarRankingInput": @"queryEmbedding",
      @"ArcCommandBarRankingResult": @"candidateID score",
      @"ArcCommandBarRankingTensor": @"name shape data",
      @"ArcCommandBarRankingEvaluation": @"metrics numExamplesEvaluated evaluationDurationMs",
      @"ArcCommandBarRankingExample": @"input selectedIndex",
      @"ArcCommandBarRankingTrainingResult": @"outcome failureReason modelName modelVersion numSamples lossesPerEpoch gradientNormMean gradientNormMax deltaNorm deltaToWeightRatio trainingDurationMs"
    };
    NSDictionary<NSString*, NSString*>* initializers = @{
      @"ArcAIInferenceModel": @"init:directory:",
      @"ArcAIInferenceConfiguration": @"init:streamAfterEveryNTokens:",
      @"ArcAIDistilBertClassifier": @"init:loraConfigJSON:configFilePath:classifierHeadFilePath:numClasses:",
      @"ArcNomicEmbedTextModel": @"init:version:",
      @"ArcNomicEmbedTextItem": @"init:task:",
      @"ArcCommandBarRankingModel": @"init:version:",
      @"ArcAIServiceConfiguration": @"init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error:",
      @"ArcCommandBarRankingCandidate": @"init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding:",
      @"ArcCommandBarRankingInput": @"init:",
      @"ArcCommandBarRankingResult": @"init:score:",
      @"ArcCommandBarRankingTensor": @"init:shape:data:",
      @"ArcCommandBarRankingEvaluation": @"init:numExamplesEvaluated:evaluationDurationMs:",
      @"ArcCommandBarRankingExample": @"init:candidates:selectedIndex:",
      @"ArcCommandBarRankingTrainingResult": @"init:failureReason:delta:modelName:modelVersion:numSamples:lossesPerEpoch:gradientNormMean:gradientNormMax:deltaNorm:deltaToWeightRatio:trainingDurationMs:"
    };
    NSMutableDictionary* metadata = [NSMutableDictionary dictionary];
    for (NSString* name in fieldNames) {
      if (!NSClassFromString(name)) {
        fprintf(stderr, "Missing class: %s\n", name.UTF8String);
        return 4;
      }
      NSArray* fields = Fields(fieldNames[name]);
      if (observeUnresolved && [name isEqualToString:@"ArcCommandBarRankingExample"])
        fields = [fields arrayByAddingObject:@"candidates"];
      if (observeUnresolved && [name isEqualToString:@"ArcCommandBarRankingTrainingResult"])
        fields = [fields arrayByAddingObject:@"delta"];
      metadata[name] = Surface(name, fields, initializers[name]);
    }
    NSMutableDictionary* values = [NSMutableDictionary dictionary];
    NSArray* strings = @[NSNull.null, @"", @"π/東京/🧪", [@"long-λ" stringByPaddingToLength:4096 withString:@"long-λ" startingAtIndex:0]];
    int sample = 0;
    for (id raw in strings) {
      NSString* text = raw == NSNull.null ? nil : raw;
      NSMutableArray* floats = [@[@0.1, @(-7.25), @16777217, @0, @1.234567890123] mutableCopy];
      NSMutableArray* shape = [@[@3, @(-2), @4294967297LL, @2147483648LL] mutableCopy];
      NSMutableDictionary* metrics = [@{@"π": @1.234567890123, @"negative": @(-9.876543210987)} mutableCopy];
      ArcAIInferenceModel* inference = [(ArcAIInferenceModel*)[NSClassFromString(@"ArcAIInferenceModel") alloc] init:5 directory:text];
      ArcAIInferenceConfiguration* configuration = [(ArcAIInferenceConfiguration*)[NSClassFromString(@"ArcAIInferenceConfiguration") alloc] init:-7 streamAfterEveryNTokens:32769];
      ArcAIDistilBertClassifier* classifier = [(ArcAIDistilBertClassifier*)[NSClassFromString(@"ArcAIDistilBertClassifier") alloc] init:text loraConfigJSON:text configFilePath:text classifierHeadFilePath:text numClasses:65535];
      ArcNomicEmbedTextModel* nomic = [(ArcNomicEmbedTextModel*)[NSClassFromString(@"ArcNomicEmbedTextModel") alloc] init:text version:-3];
      ArcNomicEmbedTextItem* item = [(ArcNomicEmbedTextItem*)[NSClassFromString(@"ArcNomicEmbedTextItem") alloc] init:text task:text];
      ArcCommandBarRankingModel* model = [(ArcCommandBarRankingModel*)[NSClassFromString(@"ArcCommandBarRankingModel") alloc] init:text version:7];
      NSError* error = nil;
      ArcAIServiceConfiguration* service = [(ArcAIServiceConfiguration*)[NSClassFromString(@"ArcAIServiceConfiguration") alloc] init:4 displayName:text libraryPath:[NSURL fileURLWithPath:@"/tmp/a b/λ.dylib"] libraryVersion:text appEnvironment:text fullAppVersion:text maxRetries:128 timeout:1.234567890123 error:&error];
      ArcCommandBarRankingCandidate* candidate = [(ArcCommandBarRankingCandidate*)[NSClassFromString(@"ArcCommandBarRankingCandidate") alloc] init:text archetype:text destination:text title:text url:text lastNavSeconds:-9 freqEver:2147483647 typedCount:-2147483647 siteEngagement:0.1f isExactURLMatch:YES isBaseDomainMatch:NO isTitlePrefix:YES titleEmbedding:floats];
      ArcCommandBarRankingInput* input = [(ArcCommandBarRankingInput*)[NSClassFromString(@"ArcCommandBarRankingInput") alloc] init:floats];
      ArcCommandBarRankingResult* ranking = [(ArcCommandBarRankingResult*)[NSClassFromString(@"ArcCommandBarRankingResult") alloc] init:text score:-0.1f];
      ArcCommandBarRankingTensor* tensor = [(ArcCommandBarRankingTensor*)[NSClassFromString(@"ArcCommandBarRankingTensor") alloc] init:text shape:shape data:floats];
      ArcCommandBarRankingEvaluation* evaluation = [(ArcCommandBarRankingEvaluation*)[NSClassFromString(@"ArcCommandBarRankingEvaluation") alloc] init:metrics numExamplesEvaluated:-12 evaluationDurationMs:2147483647];
      ArcCommandBarRankingExample* example = [(ArcCommandBarRankingExample*)[NSClassFromString(@"ArcCommandBarRankingExample") alloc] init:input candidates:@[candidate] selectedIndex:-3];
      ArcCommandBarRankingTrainingResult* training = [(ArcCommandBarRankingTrainingResult*)[NSClassFromString(@"ArcCommandBarRankingTrainingResult") alloc] init:4294967297LL failureReason:-4294967297LL delta:@[tensor] modelName:text modelVersion:-11 numSamples:2147483647 lossesPerEpoch:floats gradientNormMean:0.1f gradientNormMax:12.3456789f deltaNorm:-2.5f deltaToWeightRatio:0.000123456789f trainingDurationMs:-17];
      // Mutating supplied collections distinguishes copy-from-reference behavior.
      [floats removeAllObjects];
      [shape removeAllObjects];
      [metrics removeAllObjects];
      NSMutableDictionary* sampleValues = [NSMutableDictionary dictionary];
      NSArray* objects = @[inference, configuration, classifier, nomic, item, model,
                           service, candidate, input, ranking, tensor, evaluation, training];
      for (id object in objects) {
        NSString* name = NSStringFromClass([object class]);
        sampleValues[name] = Snapshot(object, Fields(fieldNames[name]));
      }
      id copiedInput = [example valueForKey:@"input"];
      sampleValues[@"ArcCommandBarRankingExample"] = @{
        @"input": Snapshot(copiedInput, @[@"queryEmbedding"]),
        @"selectedIndex": [example valueForKey:@"selectedIndex"],
        @"inputIsSeparateObject": @(copiedInput != input),
        @"inputGetterReturnsSeparateObjects": @(copiedInput != [example valueForKey:@"input"])
      };
      if (observeUnresolved) {
        id originalCandidates = [example valueForKey:@"candidates"];
        id originalDelta = [training valueForKey:@"delta"];
        NSMutableArray* candidates = [NSMutableArray array];
        for (id value in originalCandidates)
          [candidates addObject:Snapshot(value, Fields(fieldNames[@"ArcCommandBarRankingCandidate"]))];
        NSMutableArray* tensors = [NSMutableArray array];
        for (id value in originalDelta)
          [tensors addObject:Snapshot(value, Fields(fieldNames[@"ArcCommandBarRankingTensor"]))];
        sampleValues[@"unresolvedObservations"] = @{
          @"candidates": candidates, @"delta": tensors,
          @"candidatesMutable": @([originalCandidates isKindOfClass:NSMutableArray.class]),
          @"deltaMutable": @([originalDelta isKindOfClass:NSMutableArray.class]),
          @"candidatesAreFresh": @(originalCandidates != [example valueForKey:@"candidates"]),
          @"deltaIsFresh": @(originalDelta != [training valueForKey:@"delta"])
        };
      }
      sampleValues[@"boxing"] = @{
        @"candidate": Boxed([candidate valueForKey:@"titleEmbedding"]),
        @"input": Boxed([input valueForKey:@"queryEmbedding"]),
        @"tensorData": Boxed([tensor valueForKey:@"data"]),
        @"tensorShape": Boxed([tensor valueForKey:@"shape"]),
        @"losses": Boxed([training valueForKey:@"lossesPerEpoch"])
      };
      sampleValues[@"serviceError"] = error ? @{@"domain": error.domain, @"code": @(error.code)} : NSNull.null;
      values[[NSString stringWithFormat:@"sample%d", sample++]] = sampleValues;
    }
    NSMutableArray* narrowing16 = [NSMutableArray array];
    for (NSNumber* n in @[@0, @32767, @32768, @65535, @65536, @(-1), @(-32769)]) {
      id configuration = [(ArcAIInferenceConfiguration*)[NSClassFromString(@"ArcAIInferenceConfiguration") alloc] init:-7 streamAfterEveryNTokens:n.intValue];
      id classifier = [(ArcAIDistilBertClassifier*)[NSClassFromString(@"ArcAIDistilBertClassifier") alloc] init:nil loraConfigJSON:nil configFilePath:nil classifierHeadFilePath:nil numClasses:n.intValue];
      [narrowing16 addObject:@{@"input": n, @"interval": [configuration valueForKey:@"streamAfterEveryNTokens"], @"numClasses": [classifier valueForKey:@"numClasses"]}];
    }
    NSMutableArray* narrowing8 = [NSMutableArray array];
    for (NSNumber* n in @[@127, @128, @255, @256, @(-129)]) {
      id service = [(ArcAIServiceConfiguration*)[NSClassFromString(@"ArcAIServiceConfiguration") alloc] init:4 displayName:nil libraryPath:nil libraryVersion:nil appEnvironment:nil fullAppVersion:nil maxRetries:n.intValue timeout:-1.25 error:nil];
      [narrowing8 addObject:@{@"input": n, @"configuration": Snapshot(service, Fields(fieldNames[@"ArcAIServiceConfiguration"]))}];
    }
    NSError* error = nil;
    id invalid = [(ArcAIServiceConfiguration*)[NSClassFromString(@"ArcAIServiceConfiguration") alloc] init:-1 displayName:nil libraryPath:nil libraryVersion:nil appEnvironment:nil fullAppVersion:nil maxRetries:0 timeout:0 error:&error];
    NSDictionary* result = @{@"metadata": metadata, @"values": values,
      @"narrowing16": narrowing16, @"narrowing8": narrowing8,
      @"invalid": @{@"returnedNil": @(invalid == nil), @"errorDomain": JsonValue(error.domain),
                     @"errorCode": @(error.code), @"errorDescription": JsonValue(error.localizedDescription)}};
    NSError* jsonError = nil;
    NSData* data = [NSJSONSerialization dataWithJSONObject:result
        options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&jsonError];
    if (!data) {
      fprintf(stderr, "JSON serialization: %s\n", jsonError.localizedDescription.UTF8String);
      return 5;
    }
    fwrite(data.bytes, 1, data.length, stdout);
    fputc('\n', stdout);
  }
}
