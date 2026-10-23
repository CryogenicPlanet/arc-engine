// Test-owned public API adapter. Recorder canonical forwarders remain installed.
// Callback receipts below are sidecars, not canonical instrumented block calls.
// ABI: arc/tools/arccore-1.170.0-objc.tsv372,382-383,387,409-410,2394,3282-3283.
// Semantics: arc/common/renderer_exclusion_feature_gate_evidence.md and
// renderer_debug_cache_evidence.md; debug may route to a different live agent.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
@protocol AdblockApplicationPublic <NSObject>
- (void)setBlockLists:(NSArray<NSString*>*)lists;
- (void)setExcludedDomains:(NSArray<NSString*>*)domains;
- (void)setFeatureEnabled:(NSUInteger)feature enabled:(BOOL)enabled;
- (BOOL)isFeatureEnabled:(NSUInteger)feature;
@end
@protocol AdblockContextPublic <NSObject>
- (void)getAdBlockDebugInfo:(NSURL*)url completion:(void(^)(NSString*))completion;
@end
@protocol AdblockContentsPublic <NSObject>
- (int)blockedResourceCount;
@end
static NSString *gRoot,*gDirectory,*gApp,*gLibrary,*gAgent,*gNonce,*gURL,*gHost,*gLastID;
static NSHashTable *gContents;
static NSMapTable *gOwners,*gApplications;
static __weak id gApplication;
static id gSelected,gBrowser,gContext;
static BOOL gInstalled,gFailed,gAwaitingSnapshot,gPending,gDebugReturned;
static NSInteger gStage=-1;
static NSUInteger gSequence,gCalls;
static int gCachedCount;
static NSMutableArray *gDebugEvents;
static NSMutableDictionary *gReply;
static id gCompletion;
static struct stat gDirectoryIdentity;
static dispatch_source_t gTimer;
static void Require(BOOL ok,NSString *why){if(!ok)@throw [NSException exceptionWithName:@"AdblockFunctionalFixture" reason:why userInfo:nil];}
static BOOL Real(NSString *path,BOOL directory,struct stat *out){struct stat st;if(lstat(path.fileSystemRepresentation,&st)||S_ISLNK(st.st_mode)||(directory?!S_ISDIR(st.st_mode):!S_ISREG(st.st_mode)))return NO;if(out)*out=st;return [[path stringByResolvingSymlinksInPath] isEqual:path];}
static void Scope(void){struct stat st;Require(Real(gDirectory,YES,&st)&&st.st_dev==gDirectoryIdentity.st_dev&&st.st_ino==gDirectoryIdentity.st_ino&&(st.st_mode&0077)==0,@"private evidence directory changed");}
static void Write(id value,NSString *name){Scope();NSError *error=nil;NSData *data=[NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:&error];Require(data&&[data writeToFile:[gDirectory stringByAppendingPathComponent:name] options:NSDataWritingAtomic error:&error],@"owned evidence write failed");}
static id Get(id object,NSString *name){return ((id(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(name));}
static BOOL Flag(id object,NSString *name){return ((BOOL(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(name));}
static SEL Alias(NSString *name){return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]);}
static BOOL AliasReady(Class cls,NSString *name){Method m=class_getInstanceMethod(cls,NSSelectorFromString(name)),a=class_getInstanceMethod(cls,Alias(name));return m&&a&&method_getImplementation(m)==(IMP)_objc_msgForward&&method_getImplementation(a)!=(IMP)_objc_msgForward;}
static BOOL AllAliasesReady(void){NSDictionary *selectors=@{
 @"_TtC3ADK15ChromiumBrowser":@[@"browser:didCreateNewWebContents:isActive:"],@"ArcWebContents":@[@"url",@"isInitialized",@"blockedResourceCount",@"setBlockedResourceCount:"],@"ArcBrowser":@[@"context",@"isInitialized"],@"ArcBrowserContext":@[@"id",@"getAdBlockDebugInfo:completion:"],@"ArcBrowserContextID":@[@"directoryBasename",@"isIncognito"],@"ArcBrowserApplication":@[@"createBrowser:withDelegate:forWindow:",@"getBrowserContext:type:",@"setExcludedDomains:",@"setBlockLists:",@"setFeatureEnabled:enabled:",@"isFeatureEnabled:"]};
 for(NSString *cls in selectors)for(NSString *selector in selectors[cls])if(!AliasReady(NSClassFromString(cls),selector))return NO;return YES;}
static void ABI(NSString *cls,NSString *name,NSString *encoding){Method m=class_getInstanceMethod(NSClassFromString(cls),NSSelectorFromString(name));Require(m&&[encoding isEqual:@(method_getTypeEncoding(m))],[@"public ABI changed: " stringByAppendingString:name]);}
static Method OwnedAlias(NSString *cls,NSString *name){Class c=NSClassFromString(cls);Require(AliasReady(c,name),@"canonical recorder alias unavailable");SEL alias=Alias(name);for(;c;c=class_getSuperclass(c)){unsigned count=0;Method *methods=class_copyMethodList(c,&count);Method found=NULL;for(unsigned i=0;i<count;i++)if(method_getName(methods[i])==alias){found=methods[i];break;}free(methods);if(found)return found;}Require(NO,@"alias owner missing");return NULL;}
static NSDictionary *Identity(void){return @{@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"url":gURL,@"host":gHost};}
static void State(void){NSMutableDictionary *s=[Identity() mutableCopy];[s addEntriesFromDictionary:@{@"stage":@(gStage),@"calls":@(gCalls),@"selected":@((BOOL)(gSelected!=nil)),@"pending":@(gPending),@"cachedCount":@(gCachedCount),@"debugEvents":gDebugEvents,@"error":gFailed?@"driver failed":NSNull.null}];Write(s,@"state.json");}
static void Failure(NSException *e){gFailed=YES;NSMutableDictionary *s=[Identity() mutableCopy];s[@"error"]=e.reason?:e.name;Write(s,@"startup-error.json");State();}
static void CaptureApplication(id application){Require(NSThread.isMainThread,@"application capture must run on main");Require(!gApplication||gApplication==application,@"multiple genuine applications");gApplication=application;}
static void Capture(id browser,id contents){if(!browser||!contents)return;if(NSThread.isMainThread){[gContents addObject:contents];[gOwners setObject:browser forKey:contents];}else{__weak id weakBrowser=browser,weakContents=contents;dispatch_async(dispatch_get_main_queue(),^{id b=weakBrowser,c=weakContents;if(b&&c){[gContents addObject:c];[gOwners setObject:b forKey:c];}});}}
static void Select(void){Require(!gSelected&&gSequence==1&&gCalls==0,@"selection consumed");Require(gContents.count<=32,@"unexpected wrapper count");id selected=nil;for(id contents in gContents)if(Flag(contents,@"isInitialized")&&[[Get(contents,@"url") absoluteString] isEqual:gURL]){Require(!selected,@"multiple exact fixture wrappers");selected=contents;}Require(selected,@"genuine exact fixture wrapper not captured");id browser=[gOwners objectForKey:selected];Require(browser&&Flag(browser,@"isInitialized")&&gApplication,@"genuine browser/application not captured");id app=[gApplications objectForKey:browser];Require(!app||app==gApplication,@"browser/application mismatch");id context=Get(browser,@"context"),identifier=Get(context,@"id");Require(context&&identifier&&[Get(identifier,@"directoryBasename") isEqual:@"Default"]&&!Flag(identifier,@"isIncognito"),@"regular Default context required");gSelected=selected;gBrowser=browser;gContext=context;gCachedCount=[(id<AdblockContentsPublic>)selected blockedResourceCount];Require(gCachedCount==0,@"fresh fixture blocked count must start at zero");}
static void Complete(void){if(!gReply||!gDebugReturned||gPending)return;gReply[@"calls"]=@(gCalls);gReply[@"complete"]=@YES;gReply[@"ok"]=@((BOOL)(!gFailed));gReply[@"forwardingAfter"]=@(AllAliasesReady());Write(gReply,@"response.json");gReply=nil;gCompletion=nil;gAwaitingSnapshot=NO;State();}
static void Install(void){
 ABI(@"ArcBrowserApplication",@"createBrowser:withDelegate:forWindow:",@"@40@0:8@16@24@32");ABI(@"ArcBrowserApplication",@"getBrowserContext:type:",@"@32@0:8@16Q24");ABI(@"ArcBrowserApplication",@"setExcludedDomains:",@"v24@0:8@16");ABI(@"ArcBrowserApplication",@"setBlockLists:",@"v24@0:8@16");ABI(@"ArcBrowserApplication",@"setFeatureEnabled:enabled:",@"v28@0:8Q16B24");ABI(@"ArcBrowserApplication",@"isFeatureEnabled:",@"B24@0:8Q16");ABI(@"ArcBrowserContext",@"getAdBlockDebugInfo:completion:",@"v32@0:8@16@?24");ABI(@"ArcWebContents",@"blockedResourceCount",@"i16@0:8");ABI(@"ArcWebContents",@"setBlockedResourceCount:",@"v20@0:8i16");ABI(@"_TtC3ADK15ChromiumBrowser",@"browser:didCreateNewWebContents:isActive:",@"v36@0:8@16@24B32");
 {Method m=OwnedAlias(@"_TtC3ADK15ChromiumBrowser",@"browser:didCreateNewWebContents:isActive:");IMP original=method_getImplementation(m);SEL alias=method_getName(m);method_setImplementation(m,imp_implementationWithBlock(^(id self,id browser,id contents,BOOL active){Capture(browser,contents);((void(*)(id,SEL,id,id,BOOL))original)(self,alias,browser,contents,active);}));}
 {Method m=OwnedAlias(@"ArcBrowserApplication",@"createBrowser:withDelegate:forWindow:");IMP original=method_getImplementation(m);SEL alias=method_getName(m);method_setImplementation(m,imp_implementationWithBlock(^id(id self,id context,id delegate,id window){id browser=((id(*)(id,SEL,id,id,id))original)(self,alias,context,delegate,window);if(NSThread.isMainThread){@try{CaptureApplication(self);if(browser)[gApplications setObject:self forKey:browser];}@catch(NSException *e){Failure(e);}}else{__weak id a=self,b=browser;dispatch_async(dispatch_get_main_queue(),^{if(a){@try{CaptureApplication(a);if(b)[gApplications setObject:a forKey:b];}@catch(NSException *e){Failure(e);}}});}return browser;}));}
 {Method m=OwnedAlias(@"ArcBrowserApplication",@"getBrowserContext:type:");IMP original=method_getImplementation(m);SEL alias=method_getName(m);method_setImplementation(m,imp_implementationWithBlock(^id(id self,id name,NSUInteger type){id value=((id(*)(id,SEL,id,NSUInteger))original)(self,alias,name,type);if(NSThread.isMainThread){@try{CaptureApplication(self);}@catch(NSException *e){Failure(e);}}else{__weak id a=self;dispatch_async(dispatch_get_main_queue(),^{if(a){@try{CaptureApplication(a);}@catch(NSException *e){Failure(e);}}});}return value;}));}
 {Method m=OwnedAlias(@"ArcBrowserApplication",@"setExcludedDomains:");IMP original=method_getImplementation(m);SEL alias=method_getName(m);method_setImplementation(m,imp_implementationWithBlock(^(id self,id domains){((void(*)(id,SEL,id))original)(self,alias,domains);if(NSThread.isMainThread){@try{CaptureApplication(self);}@catch(NSException *e){Failure(e);}}else{__weak id a=self;dispatch_async(dispatch_get_main_queue(),^{if(a){@try{CaptureApplication(a);}@catch(NSException *e){Failure(e);}}});}}));}
 {Method m=OwnedAlias(@"ArcWebContents",@"setBlockedResourceCount:");IMP original=method_getImplementation(m);SEL alias=method_getName(m);method_setImplementation(m,imp_implementationWithBlock(^(id self,int count){((void(*)(id,SEL,int))original)(self,alias,count);if(NSThread.isMainThread){if(self==gSelected){@try{gCachedCount=count;State();}@catch(NSException *e){Failure(e);}}}else{__weak id contents=self;dispatch_async(dispatch_get_main_queue(),^{if(contents&&contents==gSelected){@try{gCachedCount=count;State();}@catch(NSException *e){Failure(e);}}});}}));}
 gInstalled=YES;NSMutableDictionary *ready=[Identity() mutableCopy];ready[@"installed"]=@YES;Write(ready,@"ready.json");State();
}
static void Apply(void){id<AdblockApplicationPublic> app=gApplication;Require(app&&gSelected&&gBrowser&&gContext,@"selected owners missing");NSString *rules=[NSString stringWithFormat:@"||%@^$script\n%@##.arc-probe-ad",gHost,gHost];
 switch(gStage){case 0:++gCalls;[app setFeatureEnabled:11 enabled:YES];++gCalls;[app setBlockLists:@[]];++gCalls;[app setExcludedDomains:@[]];break;
 case 1:++gCalls;[app setBlockLists:@[rules]];break;
 case 2:++gCalls;[app setFeatureEnabled:11 enabled:NO];++gCalls;[app setExcludedDomains:@[gHost]];break;
 case 3:++gCalls;[app setExcludedDomains:@[]];break;
 case 4:++gCalls;[app setFeatureEnabled:11 enabled:YES];break;
 case 5:++gCalls;[app setExcludedDomains:@[gHost]];break;
 case 6:++gCalls;[app setFeatureEnabled:11 enabled:NO];++gCalls;[app setExcludedDomains:@[]];break;
 case 7:++gCalls;[app setFeatureEnabled:11 enabled:YES];break;
 case 8:++gCalls;[app setExcludedDomains:@[[NSString stringWithFormat:@"%@:%@",gHost,[NSURL URLWithString:gURL].port]]];break;
 case 9:++gCalls;[app setExcludedDomains:@[]];break;
 case 10:++gCalls;[app setBlockLists:@[]];break;
 default:Require(NO,@"unknown fixed stage");}
}
static void Snapshot(void){Require(!gPending&&gDebugEvents.count==(NSUInteger)gStage,@"debug request already attempted or previous callback missing");gPending=YES;gDebugReturned=NO;
 int count=[(id<AdblockContentsPublic>)gSelected blockedResourceCount];BOOL feature=[(id<AdblockApplicationPublic>)gApplication isFeatureEnabled:11];gReply[@"count"]=@(count);gReply[@"feature"]=@(feature);
 NSInteger stage=gStage;NSString *rid=gLastID;__block NSUInteger deliveries=0;__block BOOL returned=NO;NSLock *deliveryLock=[NSLock new];
 void(^completion)(NSString*)=^(NSString *info){[deliveryLock lock];BOOL main=NSThread.isMainThread,inside=!returned;NSUInteger count=++deliveries;[deliveryLock unlock];NSString *thread=[NSString stringWithFormat:@"%llu",(unsigned long long)pthread_mach_thread_np(pthread_self())];id text=[info isKindOfClass:NSString.class]?info:NSNull.null;
 void(^observe)(void)=^{@try{Require(count==1&&gPending&&stage==gStage&&[rid isEqual:gLastID]&&text!=NSNull.null,@"duplicate/wrong-stage/nonstring debug callback");NSDictionary *event=@{@"stage":@(stage),@"id":rid,@"info":text,@"main":@(main),@"thread":thread,@"insideCall":@(inside),@"count":@(count)};[gDebugEvents addObject:event];gReply[@"debug"]=event;gPending=NO;State();Complete();}@catch(NSException *e){Failure(e);}};
 if(main)observe();else dispatch_async(dispatch_get_main_queue(),observe);
 };gCompletion=[completion copy];[(id<AdblockContextPublic>)gContext getAdBlockDebugInfo:[NSURL URLWithString:gURL] completion:gCompletion];[deliveryLock lock];returned=YES;[deliveryLock unlock];gDebugReturned=YES;Complete();
}
static void Poll(void){@try{Scope();if(!gInstalled){if(!AllAliasesReady())return;Install();}if(gFailed)return;
 NSString *path=[gDirectory stringByAppendingPathComponent:@"request.json"];struct stat st;if(!Real(path,NO,&st))return;Require(st.st_size>0&&st.st_size<4096,@"invalid request size");NSData *data=[NSData dataWithContentsOfFile:path];struct stat after;Require(Real(path,NO,&after)&&st.st_dev==after.st_dev&&st.st_ino==after.st_ino&&st.st_size==after.st_size,@"request changed during read");NSError *error=nil;id decoded=data?[NSJSONSerialization JSONObjectWithData:data options:0 error:&error]:nil;Require([decoded isKindOfClass:NSDictionary.class]&&!error,@"malformed request");NSDictionary *q=decoded;NSString *rid=q[@"id"],*operation=q[@"operation"];if([rid isEqual:gLastID])return;
 Require(!gPending&&!gReply&&gSequence<23&&[rid isKindOfClass:NSString.class]&&[q[@"pid"] isKindOfClass:NSNumber.class]&&[q[@"pid"] longLongValue]==getpid()&&[q[@"nonce"] isEqual:gNonce]&&[q[@"url"] isEqual:gURL]&&[q[@"host"] isEqual:gHost]&&[q[@"sequence"] isKindOfClass:NSNumber.class]&&[q[@"sequence"] unsignedIntegerValue]==gSequence+1&&[rid isEqual:[NSString stringWithFormat:@"%@-%lu",gNonce,(unsigned long)(gSequence+1)]],@"request sequence/PID/nonce/URL mismatch");
 NSInteger stage=[q[@"stage"] integerValue];Require([q[@"stage"] isKindOfClass:NSNumber.class],@"stage missing");if(gSequence==0)Require([operation isEqual:@"select"]&&stage==-1,@"first request must select");else if(gAwaitingSnapshot)Require([operation isEqual:@"snapshot"]&&stage==gStage,@"snapshot exact stage required");else Require([operation isEqual:@"apply"]&&stage==gStage+1&&stage>=0&&stage<=10,@"fixed once-only apply stage required");
 gLastID=rid;++gSequence;BOOL before=AllAliasesReady();gReply=[Identity() mutableCopy];[gReply addEntriesFromDictionary:@{@"id":rid,@"operation":operation,@"main":@(NSThread.isMainThread),@"ok":@NO,@"complete":@NO,@"stage":@(stage),@"sequence":@(gSequence),@"calls":@(gCalls),@"forwardingBefore":@(before)}];Write(gReply,[NSString stringWithFormat:@"intent-%lu.json",(unsigned long)gSequence]);Require(before,@"canonical forwarding missing");
 if(gSequence==1){Select();gDebugReturned=YES;Complete();}else if([operation isEqual:@"apply"]){gStage=stage;gAwaitingSnapshot=YES;Apply();gDebugReturned=YES;gReply[@"calls"]=@(gCalls);gReply[@"complete"]=@YES;gReply[@"ok"]=@YES;gReply[@"forwardingAfter"]=@(AllAliasesReady());Write(gReply,@"response.json");gReply=nil;State();}else Snapshot();
 }@catch(NSException *e){Failure(e);}}
__attribute__((constructor))static void StartAdblockFunctional(void){@autoreleasepool{
 NSDictionary *keys=@{@"root":@"ARC_SCENARIO_ADBLOCK_ROOT",@"directory":@"ARC_SCENARIO_ADBLOCK_DIRECTORY",@"app":@"ARC_SCENARIO_ADBLOCK_APP",@"library":@"ARC_SCENARIO_ADBLOCK_LIBRARY",@"agent":@"ARC_SCENARIO_ADBLOCK_AGENT",@"nonce":@"ARC_SCENARIO_ADBLOCK_NONCE",@"url":@"ARC_SCENARIO_ADBLOCK_URL",@"host":@"ARC_SCENARIO_ADBLOCK_HOST"};NSMutableDictionary *env=[NSMutableDictionary dictionary];for(NSString *key in keys){const char *value=getenv([keys[key] UTF8String]);if(!value)return;env[key]=@(value);}const char *record=getenv("ARC_RECORD_PATH");if(!record)return;
 gRoot=env[@"root"];gDirectory=env[@"directory"];gApp=env[@"app"];gLibrary=env[@"library"];gAgent=env[@"agent"];gNonce=env[@"nonce"];gURL=env[@"url"];gHost=env[@"host"];
 if(![@[@"trace156",@"ours156"] containsObject:gAgent]||![gRoot isAbsolutePath]||!Real(gRoot,YES,NULL)||![gDirectory isEqual:[gRoot stringByAppendingPathComponent:@"renderer-adblock-functional-driver"]]||!Real(gDirectory,YES,&gDirectoryIdentity)||(gDirectoryIdentity.st_mode&0077)!=0||![gLibrary isEqual:[gDirectory stringByAppendingPathComponent:@"RendererAdblockFunctionalDriver.dylib"]]||!Real(gLibrary,NO,NULL)||![@(record) isEqual:[gRoot stringByAppendingPathComponent:@"launch-1/record.jsonl"]])return;
 Dl_info image;if(!dladdr((const void*)&StartAdblockFunctional,&image)||!image.dli_fname||![@(image.dli_fname) isEqual:gLibrary])return;NSUUID *uuid=[[NSUUID alloc] initWithUUIDString:gNonce];if(!uuid||![uuid.UUIDString.lowercaseString isEqual:gNonce])return;
 NSURL *url=[NSURL URLWithString:gURL];if(![url.scheme isEqual:@"http"]||![url.host isEqual:@"127.0.0.1"]||![gHost isEqual:url.host]||url.port.integerValue<1024||url.port.integerValue>65535||url.user||url.password||url.query||url.fragment||![url.path isEqual:@"/renderer-adblock-functional-7f3a/a"])return;
 NSString *exe=NSProcessInfo.processInfo.arguments.firstObject,*app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];if(![app isEqual:gApp]||!Real(gApp,YES,NULL)||![exe isEqual:[gApp stringByAppendingPathComponent:@"Contents/MacOS/Arc"]]||![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app",gAgent]]||![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:gAgent]])return;
 unsetenv("DYLD_INSERT_LIBRARIES");gContents=[NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality];gOwners=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];gApplications=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];gDebugEvents=[NSMutableArray array];
 dispatch_async(dispatch_get_main_queue(),^{gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());dispatch_source_set_timer(gTimer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);dispatch_after(dispatch_time(DISPATCH_TIME_NOW,900*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(gTimer);});});
}}
