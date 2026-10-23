// Test-only two-tab restart driver, adapted from the SHA256-frozen sessions v6 probe.
// Public ObjC ABI only. Canonical recorder forwarding is never replaced.
// Genuine app callback aliases are wrapped; no custom delegates or fabricated callbacks.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
static NSHashTable *gContents;
static NSMapTable *gOwners;
static NSMutableSet *gHooks;
static NSMutableDictionary *gRecords;
static NSMutableArray *gEvents;
static NSString *gRequest,*gNonce,*gLastID,*gAgent,*gError,*gContextKey,*gBase;
static id gBrowser,gSentinel;
static NSUInteger gDepth;
static BOOL gInstalled;
static NSDictionary *gSnapshot;
static dispatch_source_t gTimer;
static id Get(id o,NSString *s){return ((id(*)(id,SEL))objc_msgSend)(o,NSSelectorFromString(s));}
static BOOL Flag(id o,NSString *s){return ((BOOL(*)(id,SEL))objc_msgSend)(o,NSSelectorFromString(s));}
static NSString *SID(id o){return o?[@(((NSInteger(*)(id,SEL))objc_msgSend)(o,NSSelectorFromString(@"identifier"))) stringValue]:nil;}
static void Require(BOOL b,NSString *s){if(!b)@throw [NSException exceptionWithName:@"TabRestartProbe" reason:s userInfo:nil];}
static NSString *Key(id browser){id c=Get(browser,@"context"),i=Get(c,@"id");NSString *b=Get(i,@"directoryBasename");return b?[NSString stringWithFormat:@"%@/%d",b,Flag(i,@"isIncognito")]:nil;}
static void Write(id v,NSString *p){NSData *d=[NSJSONSerialization dataWithJSONObject:v options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:NULL];[d writeToFile:p options:NSDataWritingAtomic error:NULL];}
static NSDictionary *Event(NSString *type,NSDictionary *values){NSMutableDictionary *e=[@{@"type":type,@"main":@(NSThread.isMainThread),@"insideAction":@((BOOL)(gDepth!=0)),@"order":@(gEvents.count)} mutableCopy];[e addEntriesFromDictionary:values?:@{}];[gEvents addObject:e];return e;}
static void Register(id b,id c){if(c){[gContents addObject:c];if(b)[gOwners setObject:b forKey:c];}}
static SEL Alias(NSString *name){return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]);}
static BOOL AliasReady(Class cls,NSString *name){
 if(!cls)return NO;Method canonical=class_getInstanceMethod(cls,NSSelectorFromString(name)),alias=class_getInstanceMethod(cls,Alias(name));
 return canonical&&alias&&method_getImplementation(canonical)==(IMP)_objc_msgForward&&method_getImplementation(alias)!=(IMP)_objc_msgForward;
}
static NSArray *CallbackHooks(void){return @[
 @[@"_TtC3ADK15ChromiumBrowser",@"browser:didCreateNewWebContents:isActive:"],
 @[@"_TtC3ADK15ChromiumBrowser",@"browser:didRestoreWebContents:withSessionId:"],
 @[@"_TtC3ADK18WebContentChromium",@"webContents:didCloseWithSessionID:"],
 @[@"_TtC3ADK28TablessWebContentsUIDelegate",@"webContents:didCloseWithSessionID:"]];}
static BOOL AllAliasesReady(void){
 NSDictionary *engine=@{@"ArcBrowser":@[@"context",@"isInitialized",@"canRestoreWebContentsWithSessionId:",@"restoreWebContentsWithSessionId:"],
 @"ArcWebContents":@[@"close",@"isInitialized",@"url",@"sessionId"],@"ArcSessionID":@[@"identifier",@"initWithID:"],
 @"ArcBrowserContext":@[@"id"],@"ArcBrowserContextID":@[@"directoryBasename",@"isIncognito"]};
 for(NSString *cls in engine)for(NSString *sel in engine[cls])if(!AliasReady(NSClassFromString(cls),sel))return NO;
 for(NSArray *hook in CallbackHooks())if(!AliasReady(NSClassFromString(hook[0]),hook[1]))return NO;return YES;
}
static void WrapAlias(Class cls,NSString *name,id(^make)(IMP,SEL)){
 Require(AliasReady(cls,name),@"recorder forwarding/alias disappeared");SEL alias=Alias(name);Class owner=Nil;
 for(Class c=cls;c&&!owner;c=class_getSuperclass(c)){unsigned n=0;Method *methods=class_copyMethodList(c,&n);for(unsigned i=0;i<n;++i)if(method_getName(methods[i])==alias){owner=c;break;}free(methods);}
 Require(owner!=Nil,@"alias owner missing");NSString *key=[NSString stringWithFormat:@"%@/%@",NSStringFromClass(owner),name];if([gHooks containsObject:key])return;
 Method method=class_getInstanceMethod(owner,alias);method_setImplementation(method,imp_implementationWithBlock(make(method_getImplementation(method),alias)));[gHooks addObject:key];
}
static void HookCallbacks(void){Class delegate=NSClassFromString(@"_TtC3ADK15ChromiumBrowser");
 WrapAlias(delegate,@"browser:didCreateNewWebContents:isActive:",^id(IMP original,SEL alias){return ^(id o,id b,id contents,BOOL active){Register(b,contents);((void(*)(id,SEL,id,id,BOOL))original)(o,alias,b,contents,active);};});
 WrapAlias(delegate,@"browser:didRestoreWebContents:withSessionId:",^id(IMP original,SEL alias){return ^(id o,id b,id contents,id identifier){Register(b,contents);
 @try{if(b==gBrowser){NSMutableDictionary *match=nil;for(NSString *url in gRecords){NSMutableDictionary *r=gRecords[url];if([r[@"restoreRequested"] boolValue]&&[SID(r[@"closedID"]) isEqual:SID(identifier)]){Require(!match,@"ambiguous historical callback ID");match=r;}}
 NSDictionary *e=Event(match?@"restored":@"unexpected-restored",@{@"id":SID(identifier)?:NSNull.null,@"url":match[@"url"]?:NSNull.null,@"initialized":@((BOOL)(contents&&Flag(contents,@"isInitialized")))});
 if(match){match[@"restoredContents"]=contents?:NSNull.null;match[@"callbackID"]=identifier?:NSNull.null;[match[@"restores"] addObject:e];}
 }}@catch(NSException *e){gError=e.reason?:e.name;}
 ((void(*)(id,SEL,id,id,id))original)(o,alias,b,contents,identifier);};});
 for(NSString *name in @[@"_TtC3ADK18WebContentChromium",@"_TtC3ADK28TablessWebContentsUIDelegate"]){
 WrapAlias(NSClassFromString(name),@"webContents:didCloseWithSessionID:",^id(IMP original,SEL alias){return ^(id o,id contents,id identifier){
 @try{for(NSString *url in gRecords){NSMutableDictionary *r=gRecords[url];if(r[@"sourceContents"]==contents&&[r[@"closeRequested"] boolValue]){
 if(identifier)r[@"closedID"]=identifier;NSDictionary *e=Event(@"closed",@{@"url":url,@"id":SID(identifier)?:NSNull.null,@"nilID":@((BOOL)(identifier==nil)),@"sourceID":r[@"sourceID"]?:NSNull.null});[r[@"closes"] addObject:e];
 }}}@catch(NSException *e){gError=e.reason?:e.name;}
 ((void(*)(id,SEL,id,id))original)(o,alias,contents,identifier);};});}
}
static BOOL Eligible(id identifier){return identifier&&gBrowser&&Flag(gBrowser,@"isInitialized")&&((BOOL(*)(id,SEL,id))objc_msgSend)(gBrowser,NSSelectorFromString(@"canRestoreWebContentsWithSessionId:"),identifier);}
static id Find(NSString *url){id found=nil;for(id c in gContents)if(Flag(c,@"isInitialized")&&[[Get(c,@"url") absoluteString] isEqual:url]){Require(!found,@"multiple initialized wrappers match URL");found=c;}return found;}
static void ValidURL(NSString *url){Require([url isKindOfClass:NSString.class],@"URL required");NSURL *u=[NSURL URLWithString:url];NSURL *base=[NSURL URLWithString:gBase];Require([u.scheme isEqual:@"http"]&&[u.host isEqual:@"127.0.0.1"]&&[u.port isEqual:base.port]&&[u.path hasPrefix:@"/arc-sessions-restart-7f3a/"]&&!u.user&&!u.password&&!u.query&&!u.fragment,@"exact fixture origin/path required");}
static id ParseID(NSString *s){Require([s isKindOfClass:NSString.class],@"decimal ID string required");NSScanner *scan=[NSScanner scannerWithString:s];long long n=0;Require([scan scanLongLong:&n]&&scan.isAtEnd&&n>0&&n<=INT32_MAX,@"ID outside positive signed32 range");id result=((id(*)(id,SEL,NSInteger))objc_msgSend)([NSClassFromString(@"ArcSessionID") alloc],NSSelectorFromString(@"initWithID:"),(NSInteger)n);Require([SID(result) isEqual:s],@"NSInteger ID roundtrip failed");return result;}
static NSDictionary *Snapshot(void){NSMutableArray *records=[NSMutableArray array];NSHashTable *restoredObjects=[NSHashTable hashTableWithOptions:NSPointerFunctionsObjectPointerPersonality];NSUInteger restoredCount=0;BOOL restoredNotSentinel=YES;for(NSString *url in [[gRecords allKeys] sortedArrayUsingSelector:@selector(compare:)]){NSDictionary *r=gRecords[url];id old=r[@"sourceContents"],restored=r[@"restoredContents"],callback=r[@"callbackID"];
 BOOL init=restored&&restored!=NSNull.null&&Flag(restored,@"isInitialized");
 if(init){++restoredCount;[restoredObjects addObject:restored];restoredNotSentinel&=(restored!=gSentinel);}
 [records addObject:@{@"url":url,@"sourceID":r[@"sourceID"]?:NSNull.null,@"restorationID":SID(r[@"closedID"])?:NSNull.null,@"callbackID":callback&&callback!=NSNull.null?SID(callback)?:NSNull.null:NSNull.null,
 @"closes":[r[@"closes"] copy],@"restores":[r[@"restores"] copy],@"oldInitialized":@((BOOL)(old&&Flag(old,@"isInitialized"))),@"restoredInitialized":@(init),@"restoredURL":init?[Get(restored,@"url") absoluteString]?:NSNull.null:NSNull.null,@"restoredLiveID":init?SID(Get(restored,@"sessionId"))?:NSNull.null:NSNull.null,@"canRestore":@(Eligible(r[@"closedID"])),@"closeRequested":r[@"closeRequested"]?:@NO,@"restoreRequested":r[@"restoreRequested"]?:@NO}];}
 return @{@"records":records,@"events":[gEvents copy],@"contextKey":gContextKey?:NSNull.null,@"browserInitialized":@((BOOL)(gBrowser&&Flag(gBrowser,@"isInitialized"))),@"sentinelInitialized":@((BOOL)(gSentinel&&Flag(gSentinel,@"isInitialized"))),@"sentinelURL":gSentinel&&Flag(gSentinel,@"isInitialized")?[Get(gSentinel,@"url") absoluteString]?:NSNull.null:NSNull.null,@"simultaneousRestoredDistinct":@((BOOL)(restoredCount==2&&restoredObjects.count==2&&restoredNotSentinel)),@"recorderAliases":@(AllAliasesReady()),@"error":gError?:NSNull.null};}
static NSMutableDictionary *NewRecord(NSString *url){return [@{@"url":url,@"closes":[NSMutableArray array],@"restores":[NSMutableArray array]} mutableCopy];}
static void Setup(void){if(gInstalled||!AllAliasesReady())return;HookCallbacks();gInstalled=YES;
 Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"bundle":NSBundle.mainBundle.bundleIdentifier,@"installed":@YES,@"recorderAliases":@YES},[gRequest stringByAppendingString:@".ready.json"]);}
// Poll/status are pure cached serialization. Explicit snapshot requests sample public APIs.
static NSDictionary *State(void){NSMutableDictionary *s=[gSnapshot?:@{} mutableCopy];s[@"events"]=[gEvents copy];s[@"error"]=gError?:NSNull.null;
 NSMutableArray *records=[NSMutableArray array];for(NSDictionary *old in s[@"records"]){NSMutableDictionary *r=[old mutableCopy];NSDictionary *live=gRecords[r[@"url"]];r[@"closes"]=[live[@"closes"] copy];r[@"restores"]=[live[@"restores"] copy];[records addObject:r];}s[@"records"]=records;return s;}
static void Poll(void){Setup();if(!gInstalled)return;NSData *data=[NSData dataWithContentsOfFile:gRequest];NSDictionary *q=data?[NSJSONSerialization JSONObjectWithData:data options:0 error:NULL]:nil;if(![q isKindOfClass:NSDictionary.class])return;
 NSString *rid=q[@"id"],*op=q[@"operation"];if(![rid isKindOfClass:NSString.class]||[rid isEqual:gLastID]||![op isKindOfClass:NSString.class]||![q[@"nonce"] isEqual:gNonce]||[q[@"pid"] intValue]!=getpid())return;gLastID=rid;
 NSMutableDictionary *response=[@{@"id":rid,@"pid":@(getpid()),@"nonce":gNonce,@"operation":op,@"main":@(NSThread.isMainThread)} mutableCopy];
 @try{
 if([op isEqual:@"capture"]){Require(gRecords.count==0,@"fixtures already captured");NSArray *urls=q[@"urls"];Require([urls isKindOfClass:NSArray.class]&&urls.count==2,@"exactly two URLs required");NSString *sentinel=q[@"sentinelURL"];ValidURL(sentinel);id sc=Find(sentinel);Require(sc!=nil,@"sentinel missing");id owner=[gOwners objectForKey:sc];Require(owner&&Flag(owner,@"isInitialized"),@"sentinel owner missing");
 NSMutableDictionary *pending=[NSMutableDictionary dictionary];
 for(NSString *url in urls){ValidURL(url);id c=Find(url);Require(c&&[gOwners objectForKey:c]==owner,@"fixtures and sentinel must share browser");Require(!pending[url],@"duplicate fixture URL");NSMutableDictionary *r=NewRecord(url);r[@"sourceContents"]=c;r[@"sourceID"]=SID(Get(c,@"sessionId"))?:NSNull.null;pending[url]=r;}
 NSString *contextKey=Key(owner);Require(contextKey!=nil,@"source context missing");[gRecords addEntriesFromDictionary:pending];gBrowser=owner;gSentinel=sc;gContextKey=contextKey;Event(@"captured",@{});
 }else if([op isEqual:@"close"]){NSString *url=q[@"url"];ValidURL(url);NSMutableDictionary *r=gRecords[url];Require(r&&!r[@"closeRequested"],@"close fixture missing or repeated");Require(Flag(gSentinel,@"isInitialized")&&[gOwners objectForKey:gSentinel]==gBrowser,@"live sentinel required");r[@"closeRequested"]=@YES;++gDepth;@try{((void(*)(id,SEL))objc_msgSend)(r[@"sourceContents"],NSSelectorFromString(@"close"));}@finally{--gDepth;}
 }else if([op isEqual:@"resume"]){NSString *sentinel=q[@"sentinelURL"],*url=q[@"fixtureURL"];ValidURL(sentinel);ValidURL(url);Require(!gRecords[url],@"resume repeated");id sc=Find(sentinel),owner=sc?[gOwners objectForKey:sc]:nil;Require(owner&&Flag(owner,@"isInitialized"),@"fresh sentinel owner missing");Require(!gBrowser||gBrowser==owner,@"resumed tabs must share new browser");Require([Key(owner) isEqual:q[@"contextKey"]],@"resumed public context differs");id historical=ParseID(q[@"restorationID"]),source=ParseID(q[@"sourceID"]);NSMutableDictionary *r=NewRecord(url);r[@"closedID"]=historical;r[@"sourceID"]=SID(source);gRecords[url]=r;gBrowser=owner;gSentinel=sc;gContextKey=Key(owner);Event(@"resumed",@{@"url":url,@"id":SID(historical),@"sourceID":SID(source)});
 }else if([op isEqual:@"restore"]){NSString *url=q[@"url"];ValidURL(url);NSMutableDictionary *r=gRecords[url];Require(r&&!r[@"restoreRequested"]&&Eligible(r[@"closedID"]),@"restore missing, repeated or ineligible");r[@"restoreRequested"]=@YES;++gDepth;@try{((void(*)(id,SEL,id))objc_msgSend)(gBrowser,NSSelectorFromString(@"restoreWebContentsWithSessionId:"),r[@"closedID"]);}@finally{--gDepth;}
 }else Require([op isEqual:@"status"]||[op isEqual:@"snapshot"],@"unknown operation");
 if(![op isEqual:@"status"])gSnapshot=Snapshot();response[@"ok"]=@YES;
 }@catch(NSException *e){response[@"error"]=e.reason?:e.name;}
 response[@"state"]=State();Write(response,[gRequest stringByAppendingString:@".response.json"]);
}
__attribute__((constructor))static void Start(void){@autoreleasepool{
 const char *a=getenv("ARC_SCENARIO_RESTART_AGENT"),*q=getenv("ARC_SCENARIO_RESTART_REQUEST"),*n=getenv("ARC_SCENARIO_RESTART_NONCE"),*b=getenv("ARC_SCENARIO_RESTART_BASE");
 if(!a||!q||!n||!b||!getenv("ARC_RECORD_PATH"))return;gAgent=@(a);if(![@[@"trace156",@"ours156"] containsObject:gAgent])return;
 NSString *exe=NSProcessInfo.processInfo.arguments.firstObject,*app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
 if(![exe.lastPathComponent isEqual:@"Arc"]||![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app",gAgent]]||![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:gAgent]])return;
 gRequest=@(q);gNonce=@(n);gBase=@(b);NSURL *base=[NSURL URLWithString:gBase];
 if(![gRequest isAbsolutePath]||![gNonce hasPrefix:@"sessions-tab-restart-"]||![base.scheme isEqual:@"http"]||![base.host isEqual:@"127.0.0.1"]||base.port.integerValue<1024||base.user||base.password||base.query||base.fragment||base.path.length)return;
 unsetenv("DYLD_INSERT_LIBRARIES");gContents=[NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality];
 gOwners=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];
 gHooks=[NSMutableSet set];gRecords=[NSMutableDictionary dictionary];gEvents=[NSMutableArray array];
 dispatch_async(dispatch_get_main_queue(),^{gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());dispatch_source_set_timer(gTimer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);
 dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);dispatch_after(dispatch_time(DISPATCH_TIME_NOW,300*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(gTimer);});});}}
