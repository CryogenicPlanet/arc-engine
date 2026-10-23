// Test-only external library. Distinctive fixture values validate transport, not AI semantics.
#import <Foundation/Foundation.h>
#include "../services/ai/public/mojom/ai_service.mojom-c_api.h"
#include <string_view>
#include <vector>
using namespace mojoc;
namespace {
struct FixtureState { AIServiceType type; };
NSMutableArray* gRecords;
String S(const char* s) { return {s, static_cast<unsigned>(std::string_view(s).size())}; }
NSString* Text(String s) { return [[NSString alloc] initWithBytes:s.string length:s.length encoding:NSUTF8StringEncoding] ?: @""; }
void Log(NSString* method, NSDictionary* args = @{}) {
  NSDictionary* row = @{@"method": method, @"mainThread": @(NSThread.isMainThread), @"args": args};
  @synchronized(NSProcessInfo.processInfo) {if(!gRecords)gRecords=[NSMutableArray array];[gRecords addObject:row];}
  const char* path = getenv("ARC_AI_FIXTURE_LOG");
  if (!path) return;
  NSData* data = [NSJSONSerialization dataWithJSONObject:row options:NSJSONWritingSortedKeys error:nullptr];
  FILE* file = fopen(path, "a");
  if (file) { fwrite(data.bytes, 1, data.length, file); fputc('\n', file); fclose(file); }
}
NSMutableArray* Floats(FloatArray values) { NSMutableArray* a=[NSMutableArray array];for(unsigned i=0;i<values.length;++i)[a addObject:@(values.begin[i])];return a; }
bool Error(String s) { return [Text(s) hasPrefix:@"error"]; }
void RankResult(RankCommandBarCallback cb, UserData u, bool error) {
  AIErrorOrCommandBarRankingResults r{};
  CommandBarRankingResult values[]={{S("fixture-B"),-7.5f},{S("fixture-A"),.125f}};
  if(error){r.type=AIErrorOrCommandBarRankingResults::AIErrorOrCommandBarRankingResultsEnumError;r.value.error=AIServiceErrorModelNotLoaded;}
  else{r.type=AIErrorOrCommandBarRankingResults::AIErrorOrCommandBarRankingResultsEnumResults;r.value.results={values,2};}
  cb(&r,u);
}
}
extern "C" AIServiceHandle AIService_Create() { Log(@"Create"); return new FixtureState{AIServiceTypeNone}; }
extern "C" void AIService_Initialize(AIServiceHandle h, AIServiceConfiguration* c, InitializeCallback cb, UserData u) {
  static_cast<FixtureState*>(h)->type = c->type;
  Log(@"Initialize", @{@"type": @(c->type), @"displayName": Text(c->displayName), @"libraryPath": Text(c->libraryPath), @"libraryVersion": Text(c->libraryVersion), @"appEnvironment": Text(c->appEnvironment), @"fullAppVersion": Text(c->fullAppVersion), @"maxRetries": @(c->maxRetries), @"timeout": @(c->timeoutSeconds)});
  if ([Text(c->appEnvironment) isEqual:@"hang"]) return;
  cb(Error(c->appEnvironment) ? AIServiceErrorInvalidParameter : AIServiceErrorNone, u);
}
extern "C" void AIService_SetUser(AIServiceHandle, String id, String email, SetUserCallback cb, UserData u) {
  Log(@"SetUser", @{@"id":Text(id), @"email":Text(email)});
  if ([Text(id) isEqual:@"hang"]) return;
  if ([Text(id) isEqual:@"async"]) dispatch_async(dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0), ^{ cb(AIServiceErrorNone, u); });
  else cb(Error(id) ? AIServiceErrorOtherError : AIServiceErrorNone, u);
}
extern "C" void AIService_EnableDataDogTracing(AIServiceHandle, int8_t rate, EnableDataDogTracingCallback cb, UserData u) { Log(@"EnableDataDogTracing", @{@"rate":@(rate)}); cb(rate < 0 ? AIServiceErrorInvalidParameter : AIServiceErrorNone,u); }
extern "C" void AIService_Type(AIServiceHandle h, TypeCallback cb, UserData u) { Log(@"Type"); AIErrorOrServiceType r{}; r.type=AIErrorOrServiceType::AIErrorOrServiceTypeEnumService_type; r.value.serviceType=static_cast<FixtureState*>(h)->type; cb(&r,u); }
extern "C" void AIService_WaitForDebuggerToAttach(AIServiceHandle, uint16_t seconds, WaitForDebuggerToAttachCallback cb, UserData u) { Log(@"WaitForDebuggerToAttach", @{@"seconds":@(seconds)}); cb(u); }
extern "C" void AIService_Crash(AIServiceHandle, CrashCallback cb, UserData u) { Log(@"Crash"); if(cb)cb(u); }
extern "C" void AIService_UpdateFeatureFlags(AIServiceHandle, String s, UpdateFeatureFlagsCallback cb, UserData u) { Log(@"UpdateFeatureFlags", @{@"json":Text(s)}); cb(!Error(s),u); }
extern "C" void AIService_GetEmbeddingsFor(AIServiceHandle, String* inputs, unsigned n, AIEmbeddingConfiguration* c, GetEmbeddingsForCallback cb, UserData u) {
  NSMutableArray* a=[NSMutableArray array]; for(unsigned i=0;i<n;++i) [a addObject:Text(inputs[i])];
  Log(@"GetEmbeddingsFor", @{@"inputs":a,@"method":@(c->method),@"model":@(c->model)});
  AIErrorOrEmbeddings r{}; float data[]={0.125f,-7.5f,31.25f}; std::vector<FloatArray> values(n);
  for(unsigned i=0;i<n;++i) values[i]={data,3};
  if(n && Error(inputs[0])) { r.type=AIErrorOrEmbeddings::AIErrorOrEmbeddingsEnumError; r.value.error=AIServiceErrorModelNotLoaded; }
  else { r.type=AIErrorOrEmbeddings::AIErrorOrEmbeddingsEnumEmbeddings; r.value.embeddings={values.data()}; }
  cb(&r,u);
}
extern "C" void AIService_LoadInferenceModel(AIServiceHandle, AIInferenceModel* m, LoadInferenceModelCallback cb, UserData u) { Log(@"LoadInferenceModel", @{@"type":@(m->type),@"directory":Text(m->directory)}); cb(Error(m->directory)?AIServiceErrorCannotDownloadModel:AIServiceErrorNone,u); }
extern "C" void AIService_GetStreamedInferenceFor(AIServiceHandle, String prompt, AIStreamedInferenceConfiguration* c, AIInferenceModel* m, AIStreamedInferenceListenerHandle l, AIStreamedInferenceListener table, GetStreamedInferenceForCallback cb, UserData u) {
  Log(@"GetStreamedInferenceFor", @{@"prompt":Text(prompt),@"maxTokens":@(c->maxTokens),@"streamAfterEveryNTokens":@(c->streamAfterEveryNTokens),@"directory":Text(m->directory),@"listener":@(reinterpret_cast<uintptr_t>(l)),@"userDataID":@(u.id)});
  if([Text(prompt) isEqual:@"spaced"]) {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,50*NSEC_PER_MSEC),dispatch_get_global_queue(QOS_CLASS_DEFAULT,0),^{table.inferenceInProgress(l,S("spaced-first-π"),u);});
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,100*NSEC_PER_MSEC),dispatch_get_global_queue(QOS_CLASS_DEFAULT,0),^{table.inferenceInProgress(l,S("spaced-second-東京"),u);});
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,150*NSEC_PER_MSEC),dispatch_get_global_queue(QOS_CLASS_DEFAULT,0),^{cb(AIServiceErrorNone,u);});
    return;
  }
  if([Text(prompt) isEqual:@"fixture-log"]) {
    NSData* data;@synchronized(NSProcessInfo.processInfo){data=[NSJSONSerialization dataWithJSONObject:gRecords?:@[] options:0 error:nullptr];}
    String result={static_cast<const char*>(data.bytes),static_cast<unsigned>(data.length)};
    table.inferenceInProgress(l,result,u);cb(AIServiceErrorNone,u);return;
  }
  if(Error(prompt)) { cb(AIServiceErrorModelNotLoaded,u); return; }
  table.inferenceInProgress(l,S("partial-π"),u); table.inferenceInProgress(l,S("final-東京"),u); cb(AIServiceErrorNone,u);
}
extern "C" void AIService_LoadClassifier(AIServiceHandle, AIDistilBertClassifier* c, LoadClassifierCallback cb, UserData u) { Log(@"LoadClassifier",@{@"modelFilePath":Text(c->modelFilePath),@"lora":Text(c->loraConfigJson),@"config":Text(c->configFilePath),@"head":Text(c->classifierHeadFilePath),@"numClasses":@(c->numClasses)}); cb(Error(c->modelFilePath)?AIServiceErrorModelNotLoaded:AIServiceErrorNone,u); }
extern "C" void AIService_IsClassifierLoaded(AIServiceHandle, AIDistilBertClassifier* c, IsClassifierLoadedCallback cb, UserData u) { Log(@"IsClassifierLoaded"); AIErrorOrLoaded r{}; if(Error(c->modelFilePath)) {r.type=AIErrorOrLoaded::AIErrorOrLoadedEnumError;r.value.error=AIServiceErrorModelNotLoaded;} else {r.type=AIErrorOrLoaded::AIErrorOrLoadedEnumLoaded;r.value.loaded={true};} cb(&r,u); }
extern "C" void AIService_GetClassificationFor(AIServiceHandle, String s, AIDistilBertClassifier*, GetClassificationForCallback cb, UserData u) { Log(@"GetClassificationFor",@{@"input":Text(s)}); AIErrorOrClassification r{}; float a[]={0.125f,-7.5f,31.25f}; if(Error(s)) {r.type=AIErrorOrClassification::AIErrorOrClassificationEnumError;r.value.error=AIServiceErrorModelNotLoaded;} else {r.type=AIErrorOrClassification::AIErrorOrClassificationEnumClassification;r.value.classification={{a,3}};} cb(&r,u); }
extern "C" void AIService_LoadNomicEmbedText(AIServiceHandle, NomicEmbedTextModel* m, LoadNomicEmbedTextCallback cb, UserData u) { Log(@"LoadNomicEmbedText",@{@"directory":Text(m->directory),@"version":@(m->version)}); cb(Error(m->directory)?AIServiceErrorModelNotLoaded:AIServiceErrorNone,u); }
extern "C" void AIService_NomicEmbed(AIServiceHandle, NomicEmbedTextItem* items, unsigned n, NomicEmbedCallback cb, UserData u) { NSMutableArray* a=[NSMutableArray array]; for(unsigned i=0;i<n;++i) [a addObject:@{@"text":Text(items[i].text),@"task":Text(items[i].task)}]; Log(@"NomicEmbed",@{@"items":a}); AIErrorOrNomicEmbeddings r{}; int32_t shape[]={2,3}; float data[]={.125f,-7.5f,31.25f,4,5,6}; if(n&&Error(items[0].text)) {r.type=AIErrorOrNomicEmbeddings::AIErrorOrNomicEmbeddingsEnumError;r.value.error=AIServiceErrorModelNotLoaded;} else {r.type=AIErrorOrNomicEmbeddings::AIErrorOrNomicEmbeddingsEnumEmbeddings;r.value.embeddings={shape,2,{data,6}};} cb(&r,u); }
extern "C" void AIService_LoadCommandBarRanking(AIServiceHandle, CommandBarRankingModel* m, LoadCommandBarRankingCallback cb, UserData u) { Log(@"LoadCommandBarRanking",@{@"directory":Text(m->directory),@"version":@(m->version)}); cb(Error(m->directory)?AIServiceErrorModelNotLoaded:AIServiceErrorNone,u); }
extern "C" void AIService_RankCommandBar(AIServiceHandle, CommandBarRankingInput* input, CommandBarRankingCandidate* c, unsigned n, RankCommandBarCallback cb, UserData u) { NSMutableArray* a=[NSMutableArray array]; for(unsigned i=0;i<n;++i) [a addObject:@{@"id":Text(c[i].id),@"title":Text(c[i].title),@"archetype":Text(c[i].archetype),@"destination":Text(c[i].destination),@"url":Text(c[i].url),@"lastNavSeconds":@(c[i].lastNavSeconds),@"freqEver":@(c[i].freqEver),@"typedCount":@(c[i].typedCount),@"siteEngagement":@(c[i].siteEngagement),@"isExactUrlMatch":@(c[i].isExactUrlMatch),@"isBaseDomainMatch":@(c[i].isBaseDomainMatch),@"isTitlePrefix":@(c[i].isTitlePrefix),@"embedding":Floats(c[i].titleEmbedding)}]; Log(@"RankCommandBar",@{@"candidates":a,@"query":Floats(input->queryEmbedding)}); if(n && [Text(c[0].id) isEqual:@"async"]) {
    NSDictionary* copy=@{@"candidates":a,@"query":Floats(input->queryEmbedding)};
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,50*NSEC_PER_MSEC),dispatch_get_global_queue(QOS_CLASS_DEFAULT,0),^{
      Log(@"RankCommandBar.asyncCopy",copy);RankResult(cb,u,false);
    });
  } else RankResult(cb,u,n&&Error(c[0].id));
}
extern "C" void AIService_EvaluateCommandBarRanking(AIServiceHandle, CommandBarRankingExample* e, unsigned n, EvaluateCommandBarRankingCallback cb, UserData u) { Log(@"EvaluateCommandBarRanking",@{@"count":@(n),@"selectedIndex":@(n?e[0].selectedIndex:0),@"candidatesLength":@(n?e[0].candidatesLength:0),@"query":n?Floats(e[0].input.queryEmbedding):@[]}); AIErrorOrCommandBarRankingEvaluation r{}; CommandBarRankingMetric metrics[]={{S("metric-π"),1.234567890123},{S("negative"),-7.5}}; if(n&&e[0].selectedIndex<0){r.type=AIErrorOrCommandBarRankingEvaluation::AIErrorOrCommandBarRankingEvaluationEnumError;r.value.error=AIServiceErrorInvalidParameter;}else{r.type=AIErrorOrCommandBarRankingEvaluation::AIErrorOrCommandBarRankingEvaluationEnumEvaluation;r.value.evaluation={metrics,2,17,29};} cb(&r,u); }
extern "C" void AIService_TrainCommandBarRanking(AIServiceHandle, CommandBarRankingExample* e, unsigned n, TrainCommandBarRankingCallback cb, UserData u) { Log(@"TrainCommandBarRanking",@{@"count":@(n),@"selectedIndex":@(n?e[0].selectedIndex:0),@"candidatesLength":@(n?e[0].candidatesLength:0),@"query":n?Floats(e[0].input.queryEmbedding):@[]}); AIErrorOrCommandBarRankingTrainingResult r{}; int32_t shape[]={2,3}; float data[]={.125f,-7.5f,31.25f}; CommandBarRankingTensor tensor[]={ {S("tensor-π"),shape,2,{data,3}} }; if(n&&e[0].selectedIndex<0){r.type=AIErrorOrCommandBarRankingTrainingResult::AIErrorOrCommandBarRankingTrainingResultEnumError;r.value.error=AIServiceErrorInvalidParameter;}else{r.type=AIErrorOrCommandBarRankingTrainingResult::AIErrorOrCommandBarRankingTrainingResultEnumTraining_result;r.value.trainingResult={CommandBarRankingTrainingOutcomeCompleted,CommandBarRankingTrainingFailureReasonNoFailure,tensor,1,S("fixture-model"),23,17,{data,3},.125f,31.25f,-7.5f,.25f,29};} cb(&r,u); }
