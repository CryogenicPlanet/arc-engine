// Test-only boundary driver: cold executable or DYLD injection in disposable agent apps.
// All outputs are fixture observations, never product inference semantics.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <dlfcn.h>

@interface NSObject (AIProbe)
- (id)init:(id)delegate;
- (id)createAIService:(NSError**)error;
- (BOOL)isAvailable;
- (void)initialize:(id)config callback:(void (^)(NSInteger))cb;
- (void)type:(void (^)(NSInteger, NSInteger))cb;
- (void)setUser:(NSString*)user email:(NSString*)email callback:(void (^)(NSInteger))cb;
- (void)enableDataDogTracing:(int8_t)rate callback:(void (^)(NSInteger))cb;
- (void)updateFeatureFlags:(NSString*)flags completion:(void (^)(BOOL))cb;
- (void)embeddingsFor:(NSArray*)texts method:(NSInteger)method model:(NSInteger)model completion:(void (^)(NSArray*,NSInteger))cb;
- (void)loadInferenceModel:(id)model completion:(void (^)(NSInteger))cb;
- (unsigned short)streamInference:(NSString*)prompt configuration:(id)config model:(id)model partialResultCallback:(void (^)(NSString*))partial completion:(void (^)(NSInteger))cb;
- (void)loadClassifier:(id)model completion:(void (^)(NSInteger))cb;
- (void)isClassifierLoaded:(id)model completion:(void (^)(BOOL,NSInteger))cb;
- (void)getClassificationFor:(NSString*)text classifier:(id)model completion:(void (^)(NSArray*,NSInteger))cb;
- (void)loadNomicEmbedText:(id)model completion:(void (^)(NSInteger))cb;
- (void)nomicEmbed:(NSArray*)items completion:(void (^)(NSArray*,NSArray*,NSInteger))cb;
- (void)loadCommandBarRanking:(id)model completion:(void (^)(NSInteger))cb;
- (void)rankCommandBar:(id)input candidates:(NSArray*)candidates completion:(void (^)(NSArray*,NSInteger))cb;
- (void)evaluateCommandBarRanking:(NSArray*)examples completion:(void (^)(id,NSInteger))cb;
- (void)trainCommandBarRanking:(NSArray*)examples completion:(void (^)(id,NSInteger))cb;
- (void)waitForDebuggerToAttach:(int)seconds;
- (void)crash;
- (id)init:(NSInteger)type displayName:(NSString*)name libraryPath:(NSURL*)path libraryVersion:(NSString*)version appEnvironment:(NSString*)environment fullAppVersion:(NSString*)app maxRetries:(int)retries timeout:(double)timeout error:(NSError**)error;
- (id)init:(NSInteger)type directory:(NSString*)directory;
- (id)init:(int)tokens streamAfterEveryNTokens:(int)interval;
- (id)init:(NSString*)path loraConfigJSON:(NSString*)lora configFilePath:(NSString*)config classifierHeadFilePath:(NSString*)head numClasses:(int)classes;
- (id)init:(NSString*)directory version:(int)version;
- (id)init:(NSString*)text task:(NSString*)task;
- (id)init:(NSString*)identifier archetype:(NSString*)archetype destination:(NSString*)destination title:(NSString*)title url:(NSString*)url lastNavSeconds:(int)nav freqEver:(int)frequency typedCount:(int)typed siteEngagement:(float)engagement isExactURLMatch:(BOOL)exact isBaseDomainMatch:(BOOL)base isTitlePrefix:(BOOL)prefix titleEmbedding:(NSArray*)embedding;
- (id)init:(id)input candidates:(NSArray*)candidates selectedIndex:(int)index;
- (id)initWithKind:(NSInteger)kind value:(NSString*)value;
- (void)initializeWithLibraryPath:(NSURL*)path displayName:(NSString*)name libraryVersion:(NSString*)version appEnvironment:(NSString*)environment fullAppVersion:(NSString*)app callback:(void (^)(NSInteger))cb;
- (void)convertHtmlToMarkdown:(NSString*)html url:(NSString*)url linkStyle:(NSInteger)style strategy:(NSInteger)strategy urlTransform:(NSInteger)transform skipNoscript:(BOOL)skip passThroughInputElements:(BOOL)pass metaTagsToCapture:(NSArray*)tags deduplicateMetaTagContent:(BOOL)dedup completion:(void (^)(id,NSInteger))cb;
@end

static NSMutableArray* gEvents;
static NSMutableDictionary* gReturned;
static NSMutableSet* gCompleted;
static NSMutableArray* gFixtureRecords;
static NSString* gOutput;
static NSString* gFixture;
static __weak id gApplication;
static IMP gExcluded;
static BOOL gStarted;

static id Snapshot(id value) {
  if (!value) return NSNull.null;
  if ([value isKindOfClass:NSString.class] || [value isKindOfClass:NSNumber.class] || value==NSNull.null) return value;
  if ([value isKindOfClass:NSArray.class]) { NSMutableArray* a=[NSMutableArray array];for(id x in value)[a addObject:Snapshot(x)];return a; }
  if ([value isKindOfClass:NSDictionary.class]) { NSMutableDictionary* d=[NSMutableDictionary dictionary];for(id k in value)d[[k description]]=Snapshot(value[k]);return d; }
  NSMutableDictionary* result=[NSMutableDictionary dictionary];
  NSArray* fields=@[@"candidateID",@"score",@"metrics",@"numExamplesEvaluated",@"evaluationDurationMs",@"outcome",@"failureReason",@"delta",@"modelName",@"modelVersion",@"numSamples",@"lossesPerEpoch",@"gradientNormMean",@"gradientNormMax",@"deltaNorm",@"deltaToWeightRatio",@"trainingDurationMs",@"name",@"shape",@"data",@"body",@"title",@"references",@"selectorsApplied",@"selectorsContentPct",@"metaTags",@"referenceText",@"minimumCharacterCount",@"key",@"value"];
  for(NSString* f in fields) if([value respondsToSelector:NSSelectorFromString(f)]) result[f]=Snapshot([value valueForKey:f]);
  return result;
}
static void Event(NSString* name,NSString* phase,id value) {
  @synchronized(gEvents) {
    BOOL returned=[gReturned[name] boolValue];
    [gEvents addObject:@{@"method":name,@"phase":phase,@"mainThread":@(NSThread.isMainThread),@"synchronous":@(!returned),@"value":Snapshot(value)}];
    NSData* data=[NSJSONSerialization dataWithJSONObject:gEvents options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:nullptr];
    [data writeToFile:gOutput options:NSDataWritingAtomic error:nullptr];
  }
}
static void Done(NSString* name,id value) { Event(name,@"callback",value); @synchronized(gEvents) { [gCompleted addObject:name]; } }
static void Observe(NSString* name,void (^invoke)(void)) {
  @synchronized(gEvents) {gReturned[name]=@NO;}
  Event(name,@"call",nil);
  @try { invoke(); } @catch(NSException* e) {Done(name,@{@"exception":e.reason?:e.name});}
  @synchronized(gEvents) {gReturned[name]=@YES;}
  Event(name,@"return",nil);
  NSTimeInterval deadline=NSDate.timeIntervalSinceReferenceDate+4;
  for(;;) {
    @synchronized(gEvents) {if([gCompleted containsObject:name])return;}
    if(NSDate.timeIntervalSinceReferenceDate>=deadline){Event(name,@"timeout",nil);return;}
    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:.01]];
  }
}
static id Alloc(NSString* name) { return [NSClassFromString(name) alloc]; }
static id Config(NSString* environment) { return [Alloc(@"ArcAIServiceConfiguration") init:4 displayName:@"Fixture π" libraryPath:[NSURL fileURLWithPath:gFixture] libraryVersion:@"fixture-23" appEnvironment:environment fullAppVersion:@"1.170.fixture" maxRetries:(getenv("ARC_AI_PROBE_EMBEDDINGS_ONLY")?0:2) timeout:.3 error:nullptr]; }
static void Run(id application) {
  gEvents=[NSMutableArray array];gReturned=[NSMutableDictionary dictionary];gCompleted=[NSMutableSet set];gFixtureRecords=[NSMutableArray array];
  NSError* error=nil; id service=[application createAIService:&error];
  Event(@"createAIService",@"observation",@{@"created":@(service!=nil),@"error":error.localizedDescription?:NSNull.null});
  if(!service)return;
  Event(@"isAvailable.before",@"observation",@([service isAvailable]));
  Observe(@"type.beforeInitialize",^{[service type:^(NSInteger type,NSInteger e){Done(@"type.beforeInitialize",@[@(type),@(e)]);}];});
  Observe(@"setUser.beforeInitialize",^{[service setUser:@"fixture" email:@"" callback:^(NSInteger e){Done(@"setUser.beforeInitialize",@(e));}];});
  Observe(@"initialize",^{[service initialize:Config(@"fixture") callback:^(NSInteger e){Done(@"initialize",@(e));}];});
  Event(@"isAvailable.after",@"observation",@([service isAvailable]));
  Observe(@"initialize.again",^{[service initialize:Config(@"fixture") callback:^(NSInteger e){Done(@"initialize.again",@(e));}];});
  if(getenv("ARC_AI_PROBE_STREAM_ONLY")) {
    id model=[Alloc(@"ArcAIInferenceModel") init:5 directory:@"fixture/model-π"];
    id configuration=[Alloc(@"ArcAIInferenceConfiguration") init:37 streamAfterEveryNTokens:3];
    Observe(@"stream.spaced",^{unsigned short token=[service streamInference:@"spaced" configuration:configuration model:model partialResultCallback:^(NSString* text){Event(@"stream.spaced",@"partial",text);} completion:^(NSInteger e){Done(@"stream.spaced",@(e));}];Event(@"stream.spaced",@"token",@(token));});
    Event(@"probe",@"finished",@YES);return;
  }
  // Original utility traps on the legacy embeddings endpoint; isolate this run.
  if (getenv("ARC_AI_PROBE_EMBEDDINGS_ONLY")) {
    // Let the scoped process observer sample the initialized utility before
    // the fatal legacy call; keep main-queue work available while waiting.
    NSDate* sampleDeadline=[NSDate dateWithTimeIntervalSinceNow:.25];
    while(sampleDeadline.timeIntervalSinceNow>0)
      [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:.01]];
  Observe(@"embeddings",^{[service embeddingsFor:@[@"π",@"東京"] method:1 model:2 completion:^(NSArray* a,NSInteger e){Done(@"embeddings",@[Snapshot(a),@(e)]);}];});
  Event(@"embeddings.afterFirst.available",@"observation",@([service isAvailable]));
  Observe(@"embeddings.error",^{[service embeddingsFor:@[@"error"] method:1 model:2 completion:^(NSArray* a,NSInteger e){Done(@"embeddings.error",@[Snapshot(a),@(e)]);}];});
    Event(@"probe",@"finished",@YES);return;
  }
  Observe(@"type",^{[service type:^(NSInteger type,NSInteger e){Done(@"type",@[@(type),@(e)]);}];});
  Observe(@"setUser",^{[service setUser:@"user-π" email:@"fixture@example.invalid" callback:^(NSInteger e){Done(@"setUser",@(e));}];});
  Observe(@"setUser.error",^{[service setUser:@"error" email:@"" callback:^(NSInteger e){Done(@"setUser.error",@(e));}];});
  Observe(@"setUser.async",^{[service setUser:@"async" email:@"" callback:^(NSInteger e){Done(@"setUser.async",@(e));}];});
  Observe(@"tracing",^{[service enableDataDogTracing:17 callback:^(NSInteger e){Done(@"tracing",@(e));}];});
  Observe(@"flags",^{[service updateFeatureFlags:@"{\"fixture\":true}" completion:^(BOOL ok){Done(@"flags",@(ok));}];});
  id inference=[Alloc(@"ArcAIInferenceModel") init:5 directory:@"fixture/model-π"];
  id inferenceConfig=[Alloc(@"ArcAIInferenceConfiguration") init:37 streamAfterEveryNTokens:3];
  Observe(@"loadInference",^{[service loadInferenceModel:inference completion:^(NSInteger e){Done(@"loadInference",@(e));}];});
  Observe(@"stream",^{unsigned short token=[service streamInference:@"prompt-π" configuration:inferenceConfig model:inference partialResultCallback:^(NSString* s){Event(@"stream",@"partial",s);} completion:^(NSInteger e){Done(@"stream",@(e));}];Event(@"stream",@"token",@(token));});
  id classifier=[Alloc(@"ArcAIDistilBertClassifier") init:@"fixture/classifier-π" loraConfigJSON:@"{\"rank\":7}" configFilePath:@"fixture/config" classifierHeadFilePath:@"fixture/head" numClasses:3];
  Observe(@"loadClassifier",^{[service loadClassifier:classifier completion:^(NSInteger e){Done(@"loadClassifier",@(e));}];});
  Observe(@"isClassifierLoaded",^{[service isClassifierLoaded:classifier completion:^(BOOL loaded,NSInteger e){Done(@"isClassifierLoaded",@[@(loaded),@(e)]);}];});
  Observe(@"classification",^{[service getClassificationFor:@"text-π" classifier:classifier completion:^(NSArray* a,NSInteger e){Done(@"classification",@[Snapshot(a),@(e)]);}];});
  Observe(@"classification.error",^{[service getClassificationFor:@"error" classifier:classifier completion:^(NSArray* a,NSInteger e){Done(@"classification.error",@[Snapshot(a),@(e)]);}];});
  id nomic=[Alloc(@"ArcNomicEmbedTextModel") init:@"fixture/nomic" version:19];
  Observe(@"loadNomic",^{[service loadNomicEmbedText:nomic completion:^(NSInteger e){Done(@"loadNomic",@(e));}];});
  NSArray* items=@[[Alloc(@"ArcNomicEmbedTextItem") init:@"first-π" task:@"search_document"],[Alloc(@"ArcNomicEmbedTextItem") init:@"second-東京" task:@"search_query"]];
  Observe(@"nomic",^{[service nomicEmbed:items completion:^(NSArray* shape,NSArray* data,NSInteger e){Done(@"nomic",@[Snapshot(shape),Snapshot(data),@(e)]);}];});
  id model=[Alloc(@"ArcCommandBarRankingModel") init:@"fixture/rank" version:23];
  Observe(@"loadRanking",^{[service loadCommandBarRanking:model completion:^(NSInteger e){Done(@"loadRanking",@(e));}];});
  NSArray* floats=@[@.125,@(-7.5),@31.25];
  id input=[Alloc(@"ArcCommandBarRankingInput") init:floats];
  id candidate=[Alloc(@"ArcCommandBarRankingCandidate") init:@"fixture-A" archetype:@"tab" destination:@"current" title:@"title-π" url:@"https://example.invalid/π" lastNavSeconds:17 freqEver:29 typedCount:31 siteEngagement:.125 isExactURLMatch:YES isBaseDomainMatch:NO isTitlePrefix:YES titleEmbedding:floats];
  NSArray* candidates=@[candidate];
  Observe(@"rank",^{[service rankCommandBar:input candidates:candidates completion:^(NSArray* a,NSInteger e){Done(@"rank",@[Snapshot(a),@(e)]);}];});
  id asyncCandidate=[Alloc(@"ArcCommandBarRankingCandidate") init:@"async" archetype:@"tab" destination:@"current" title:@"title-async-π" url:@"https://example.invalid/async" lastNavSeconds:17 freqEver:29 typedCount:31 siteEngagement:.125 isExactURLMatch:YES isBaseDomainMatch:NO isTitlePrefix:YES titleEmbedding:floats];
  Observe(@"rank.async",^{[service rankCommandBar:input candidates:@[asyncCandidate] completion:^(NSArray* values,NSInteger e){Done(@"rank.async",@[Snapshot(values),@(e)]);}];});
  NSArray* examples=@[[Alloc(@"ArcCommandBarRankingExample") init:input candidates:candidates selectedIndex:0]];
  Observe(@"evaluate",^{[service evaluateCommandBarRanking:examples completion:^(id r,NSInteger e){Done(@"evaluate",@[Snapshot(r),@(e)]);}];});
  Observe(@"train",^{[service trainCommandBarRanking:examples completion:^(id r,NSInteger e){Done(@"train",@[Snapshot(r),@(e)]);}];});
  Observe(@"tracing.error",^{[service enableDataDogTracing:-7 callback:^(NSInteger e){Done(@"tracing.error",@(e));}];});
  Observe(@"stream.error",^{unsigned short token=[service streamInference:@"error" configuration:inferenceConfig model:inference partialResultCallback:^(NSString* text){Event(@"stream.error",@"partial",text);} completion:^(NSInteger e){Done(@"stream.error",@(e));}];Event(@"stream.error",@"token",@(token));});
  id badInference=[Alloc(@"ArcAIInferenceModel") init:5 directory:@"error"];
  Observe(@"loadInference.error",^{[service loadInferenceModel:badInference completion:^(NSInteger e){Done(@"loadInference.error",@(e));}];});
  id badClassifier=[Alloc(@"ArcAIDistilBertClassifier") init:@"error" loraConfigJSON:nil configFilePath:nil classifierHeadFilePath:nil numClasses:3];
  Observe(@"loadClassifier.error",^{[service loadClassifier:badClassifier completion:^(NSInteger e){Done(@"loadClassifier.error",@(e));}];});
  Observe(@"isClassifierLoaded.error",^{[service isClassifierLoaded:badClassifier completion:^(BOOL loaded,NSInteger e){Done(@"isClassifierLoaded.error",@[@(loaded),@(e)]);}];});
  id badNomic=[Alloc(@"ArcNomicEmbedTextModel") init:@"error" version:19];
  Observe(@"loadNomic.error",^{[service loadNomicEmbedText:badNomic completion:^(NSInteger e){Done(@"loadNomic.error",@(e));}];});
  NSArray* badItems=@[[Alloc(@"ArcNomicEmbedTextItem") init:@"error" task:@"search_query"]];
  Observe(@"nomic.error",^{[service nomicEmbed:badItems completion:^(NSArray* shape,NSArray* data,NSInteger e){Done(@"nomic.error",@[Snapshot(shape),Snapshot(data),@(e)]);}];});
  id badModel=[Alloc(@"ArcCommandBarRankingModel") init:@"error" version:23];
  Observe(@"loadRanking.error",^{[service loadCommandBarRanking:badModel completion:^(NSInteger e){Done(@"loadRanking.error",@(e));}];});
  id badCandidate=[Alloc(@"ArcCommandBarRankingCandidate") init:@"error" archetype:nil destination:nil title:nil url:nil lastNavSeconds:0 freqEver:0 typedCount:0 siteEngagement:0 isExactURLMatch:NO isBaseDomainMatch:NO isTitlePrefix:NO titleEmbedding:@[]];
  Observe(@"rank.error",^{[service rankCommandBar:input candidates:@[badCandidate] completion:^(NSArray* values,NSInteger e){Done(@"rank.error",@[Snapshot(values),@(e)]);}];});
  NSArray* badExamples=@[[Alloc(@"ArcCommandBarRankingExample") init:input candidates:candidates selectedIndex:-1]];
  Observe(@"evaluate.error",^{[service evaluateCommandBarRanking:badExamples completion:^(id value,NSInteger e){Done(@"evaluate.error",@[Snapshot(value),@(e)]);}];});
  Observe(@"train.error",^{[service trainCommandBarRanking:badExamples completion:^(id value,NSInteger e){Done(@"train.error",@[Snapshot(value),@(e)]);}];});
  // Fixture Crash exports only acknowledges; it cannot crash the browser.
  [service waitForDebuggerToAttach:0];[service crash];
  Observe(@"setUser.timeout",^{[service setUser:@"hang" email:@"" callback:^(NSInteger e){Done(@"setUser.timeout",@(e));}];});
    Observe(@"fixtureLog.ai",^{[service streamInference:@"fixture-log" configuration:inferenceConfig model:inference partialResultCallback:^(NSString* text){
      NSArray* rows=[NSJSONSerialization JSONObjectWithData:[text dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nullptr];
      if([rows isKindOfClass:NSArray.class])[gFixtureRecords addObjectsFromArray:rows];
    } completion:^(NSInteger e){Done(@"fixtureLog.ai",@(e));}];});
  id bad=[application createAIService:nullptr];
  Observe(@"initialize.error",^{[bad initialize:Config(@"error") callback:^(NSInteger e){Done(@"initialize.error",@(e));}];});
  Event(@"isAvailable.afterError",@"observation",@([bad isAvailable]));
  Observe(@"type.afterError",^{[bad type:^(NSInteger type,NSInteger e){Done(@"type.afterError",@[@(type),@(e)]);}];});
  Observe(@"setUser.afterError",^{[bad setUser:@"user" email:@"" callback:^(NSInteger e){Done(@"setUser.afterError",@(e));}];});
  if ([service isAvailable]) {
    __weak id weakEphemeral;
    @autoreleasepool {
      __block id ephemeral=[application createAIService:nullptr];
      Observe(@"released.initialize",^{[ephemeral initialize:Config(@"fixture") callback:^(NSInteger e){Done(@"released.initialize",@(e));}];});
      weakEphemeral=ephemeral;
      [ephemeral setUser:@"async" email:@"" callback:^(NSInteger e){Done(@"released.setUser",@(e));}];
      ephemeral=nil;
    }
    Event(@"released.owner",@"observation",@(weakEphemeral==nil));
    NSDate* deadline=[NSDate dateWithTimeIntervalSinceNow:.25];
    while(deadline.timeIntervalSinceNow>0)[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:.01]];
    @synchronized(gEvents){Event(@"released.setUser",@"callbackObserved",@([gCompleted containsObject:@"released.setUser"]));}

  }
  id conversion=[Alloc(@"ArcContentConversionService") init];
  Observe(@"conversion.initialize",^{[conversion initializeWithLibraryPath:[NSURL fileURLWithPath:gFixture] displayName:@"Conversion π" libraryVersion:@"fixture-23" appEnvironment:@"fixture" fullAppVersion:@"1.170.fixture" callback:^(NSInteger e){Done(@"conversion.initialize",@(e));}];});
  Observe(@"conversion.setUser",^{[conversion setUser:@"user-π" email:@"fixture@example.invalid" callback:^(NSInteger e){Done(@"conversion.setUser",@(e));}];});
  NSArray* tags=@[[Alloc(@"ArcMetaTagSelector") initWithKind:0 value:@"og:title"],[Alloc(@"ArcMetaTagSelector") initWithKind:1 value:@"description"]];
  Observe(@"conversion.convert",^{[conversion convertHtmlToMarkdown:@"<main>fixture-π</main>" url:@"https://example.invalid/path" linkStyle:1 strategy:2 urlTransform:1 skipNoscript:YES passThroughInputElements:NO metaTagsToCapture:tags deduplicateMetaTagContent:YES completion:^(id r,NSInteger e){Done(@"conversion.convert",@[Snapshot(r),@(e)]);}];});
  Observe(@"conversion.error",^{[conversion convertHtmlToMarkdown:@"error" url:nil linkStyle:0 strategy:0 urlTransform:0 skipNoscript:NO passThroughInputElements:YES metaTagsToCapture:@[] deduplicateMetaTagContent:NO completion:^(id r,NSInteger e){Done(@"conversion.error",@[Snapshot(r),@(e)]);}];});
  Observe(@"fixtureLog.conversion",^{[conversion convertHtmlToMarkdown:@"fixture-log" url:@"https://example.invalid/log" linkStyle:0 strategy:0 urlTransform:0 skipNoscript:NO passThroughInputElements:NO metaTagsToCapture:@[] deduplicateMetaTagContent:NO completion:^(id value,NSInteger e){
    NSString* text=[value valueForKey:@"body"];
    NSArray* rows=text?[NSJSONSerialization JSONObjectWithData:[text dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nullptr]:nil;
    if([rows isKindOfClass:NSArray.class])[gFixtureRecords addObjectsFromArray:rows];
    Done(@"fixtureLog.conversion",@(e));
  }];});
  Observe(@"conversion.async",^{[conversion convertHtmlToMarkdown:@"async" url:@"https://example.invalid/async" linkStyle:1 strategy:2 urlTransform:1 skipNoscript:YES passThroughInputElements:NO metaTagsToCapture:tags deduplicateMetaTagContent:YES completion:^(id value,NSInteger e){Done(@"conversion.async",@[Snapshot(value),@(e)]);}];});
  NSString* logPath=[[gOutput stringByDeletingLastPathComponent] stringByAppendingPathComponent:@"fixture.jsonl"];
  NSMutableData* output=[NSMutableData data];
  for(id row in gFixtureRecords){[output appendData:[NSJSONSerialization dataWithJSONObject:row options:NSJSONWritingSortedKeys error:nullptr]];[output appendData:[@"\n" dataUsingEncoding:NSUTF8StringEncoding]];}
  [output writeToFile:logPath options:NSDataWritingAtomic error:nullptr];
  Event(@"conversion.afterAsync.available",@"observation",@([conversion isAvailable]));
  Observe(@"conversion.afterAsync.setUser",^{[conversion setUser:@"user" email:@"" callback:^(NSInteger e){Done(@"conversion.afterAsync.setUser",@(e));}];});
  Event(@"probe",@"finished",@YES);
}
static void Excluded(id object,SEL selector,id domains) {gApplication=object;((void(*)(id,SEL,id))gExcluded)(object,selector,domains);}
static void Poll(void) {
  if(gStarted)return;
  if(!gExcluded) {
    Method m=class_getInstanceMethod(NSClassFromString(@"ArcBrowserApplication"),NSSelectorFromString(@"setExcludedDomains:"));
    if(m) {
      gExcluded=method_setImplementation(m,(IMP)Excluded);
      NSData* ready=[NSJSONSerialization dataWithJSONObject:@{@"pid":@(NSProcessInfo.processInfo.processIdentifier),@"installed":@YES} options:0 error:nullptr];
      [ready writeToFile:[gOutput stringByAppendingString:@".ready.json"] options:NSDataWritingAtomic error:nullptr];
    }
  }
  id application=gApplication;if(!application)return;gStarted=YES;
  // Observe pumps a nested run loop. Starting it inside a main GCD block
  // prevents queued main-thread calls/replies from entering that serial queue.
  // A run-loop timer leaves the main dispatch queue available during Observe.
  NSTimer* timer=[NSTimer timerWithTimeInterval:12 repeats:NO block:^(NSTimer* firedTimer){
    (void)firedTimer;
    Run(application);
  }];
  [NSRunLoop.mainRunLoop addTimer:timer forMode:NSRunLoopCommonModes];
}
#if defined(ARC_AI_PROBE_STANDALONE)
int main(int argc,char** argv) {
  @autoreleasepool {
    if(argc!=4)return 2;
    gFixture=@(argv[2]);gOutput=@(argv[3]);
    if(!dlopen(argv[1],RTLD_NOW|RTLD_LOCAL)){fprintf(stderr,"%s\n",dlerror());return 3;}
    id application=[Alloc(@"ArcBrowserApplication") init:nil];Run(application);
  }
}
#else
__attribute__((constructor)) static void StartAIProbe(void) {
  @autoreleasepool {
    NSString* exe=NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    if(![exe.lastPathComponent isEqual:@"Arc"]||![@[@"ArcModAgent-trace156.app",@"ArcModAgent-ours156.app"] containsObject:app])return;
    const char* out=getenv("ARC_AI_PROBE_OUTPUT");const char* fixture=getenv("ARC_AI_PROBE_FIXTURE");if(!out||!fixture)return;
    gOutput=@(out);gFixture=@(fixture);
    dispatch_async(dispatch_get_main_queue(),^{
      dispatch_source_t timer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());
      dispatch_source_set_timer(timer,dispatch_time(DISPATCH_TIME_NOW,100*NSEC_PER_MSEC),100*NSEC_PER_MSEC,10*NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer,^{@autoreleasepool{Poll();}});dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW,180*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(timer);});
    });
  }
}
#endif
