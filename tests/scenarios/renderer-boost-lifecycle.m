// Public Effects adapter only. Canonical recorder forwarding remains installed;
// only the genuine app create-callback alias is wrapped and forwarded once.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
@protocol BoostEffectsPublic <NSObject>
- (void)registerStylesheet:(NSString*)source withIdentifier:(NSString*)identifier forHostPattern:(NSString*)host;
- (void)registerScript:(NSString*)source withIdentifier:(NSString*)identifier forHostPattern:(NSString*)host;
- (void)unregisterStylesheet:(NSString*)identifier;
- (void)unregisterScript:(NSString*)identifier;
@end
static NSString *gRoot,*gDirectory,*gApp,*gLibrary,*gAgent,*gNonce,*gURL,*gHost,*gLastID;
static NSHashTable *gContents;
static NSMapTable *gOwners;
static id gSelected,gBrowser,gContext;
static id<BoostEffectsPublic> gEffects;
static BOOL gInstalled,gFailed;
static NSUInteger gPhase,gCalls;
static struct stat gDirectoryIdentity;
static dispatch_source_t gTimer;
static NSString *const CSS=@"#probe{color:rgb(81,17,141)!important}";
static NSString *const JS=@"window.__arcBoostProbe=(window.__arcBoostProbe||0)+1;document.documentElement.dataset.arcBoostProbe=String(window.__arcBoostProbe)";
static NSString *const CSS_ID=@"arc-renderer-probe-boost",*const JS_ID=@"arc-renderer-probe-script";
static void Require(BOOL ok,NSString *why){if(!ok)@throw [NSException exceptionWithName:@"BoostLifecycleFixture" reason:why userInfo:nil];}
static BOOL Real(NSString *path,BOOL directory,struct stat *out){struct stat st;if(lstat(path.fileSystemRepresentation,&st)||S_ISLNK(st.st_mode)||(directory?!S_ISDIR(st.st_mode):!S_ISREG(st.st_mode)))return NO;if(out)*out=st;return [[path stringByResolvingSymlinksInPath] isEqual:path];}
static void Scope(void){struct stat st;Require(Real(gDirectory,YES,&st)&&st.st_dev==gDirectoryIdentity.st_dev&&st.st_ino==gDirectoryIdentity.st_ino&&(st.st_mode&0077)==0,@"owned private evidence directory changed");}
static void Write(id value,NSString *name){Scope();NSError *error=nil;NSData *data=[NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:&error];Require(data&&[data writeToFile:[gDirectory stringByAppendingPathComponent:name] options:NSDataWritingAtomic error:&error],@"owned evidence write failed");}
static id Get(id object,NSString *name){return ((id(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(name));}
static BOOL Flag(id object,NSString *name){return ((BOOL(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(name));}
static SEL Alias(NSString *name){return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]);}
static BOOL AliasReady(Class cls,NSString *name){Method m=class_getInstanceMethod(cls,NSSelectorFromString(name)),a=class_getInstanceMethod(cls,Alias(name));return m&&a&&method_getImplementation(m)==(IMP)_objc_msgForward&&method_getImplementation(a)!=(IMP)_objc_msgForward;}
static BOOL AllAliasesReady(void){NSDictionary *selectors=@{
 @"_TtC3ADK15ChromiumBrowser":@[@"browser:didCreateNewWebContents:isActive:"],@"ArcWebContents":@[@"url",@"isInitialized"],@"ArcBrowser":@[@"context",@"isInitialized"],@"ArcBrowserContext":@[@"id",@"effects"],@"ArcBrowserContextID":@[@"directoryBasename",@"isIncognito"],@"ArcBrowserContextEffects":@[@"registerStylesheet:withIdentifier:forHostPattern:",@"registerScript:withIdentifier:forHostPattern:",@"unregisterStylesheet:",@"unregisterScript:"]};
 for(NSString *cls in selectors)for(NSString *selector in selectors[cls])if(!AliasReady(NSClassFromString(cls),selector))return NO;return YES;}
static void ABI(NSString *cls,NSString *name,NSString *encoding){Method m=class_getInstanceMethod(NSClassFromString(cls),NSSelectorFromString(name));Require(m&&[encoding isEqual:@(method_getTypeEncoding(m))],[@"public ABI changed: " stringByAppendingString:name]);}
static void Select(void){Require(!gSelected&&gCalls==0,@"selection already consumed");Require(gContents.count<=32,@"unexpected number of wrappers");id selected=nil;for(id contents in gContents)if(Flag(contents,@"isInitialized")&&[[Get(contents,@"url") absoluteString] isEqual:gURL]){Require(selected==nil,@"multiple exact fixture wrappers");selected=contents;}Require(selected!=nil,@"genuine exact fixture wrapper not captured");id browser=[gOwners objectForKey:selected];Require(browser&&Flag(browser,@"isInitialized"),@"fixture browser not live");id context=Get(browser,@"context"),identifier=Get(context,@"id");Require(context&&identifier&&[Get(identifier,@"directoryBasename") isEqual:@"Default"]&&!Flag(identifier,@"isIncognito"),@"exact regular Default context required");id effects=Get(context,@"effects");Require(effects!=nil,@"public context effects missing");gSelected=selected;gBrowser=browser;gContext=context;gEffects=effects;}
static void Capture(id browser,id contents){
 if(!browser||!contents)return;
 if(NSThread.isMainThread){[gContents addObject:contents];[gOwners setObject:browser forKey:contents];}
 else{__weak id weakBrowser=browser,weakContents=contents;dispatch_async(dispatch_get_main_queue(),^{id b=weakBrowser,c=weakContents;if(b&&c){[gContents addObject:c];[gOwners setObject:b forKey:c];}});}
}
static void Install(void){
 ABI(@"ArcBrowserContextEffects",@"registerStylesheet:withIdentifier:forHostPattern:",@"v40@0:8@16@24@32");ABI(@"ArcBrowserContextEffects",@"registerScript:withIdentifier:forHostPattern:",@"v40@0:8@16@24@32");ABI(@"ArcBrowserContextEffects",@"unregisterStylesheet:",@"v24@0:8@16");ABI(@"ArcBrowserContextEffects",@"unregisterScript:",@"v24@0:8@16");ABI(@"ArcBrowserContext",@"effects",@"@16@0:8");
 Class cls=NSClassFromString(@"_TtC3ADK15ChromiumBrowser");NSString *name=@"browser:didCreateNewWebContents:isActive:";Require(AliasReady(cls,name),@"genuine recorder callback alias missing");SEL alias=Alias(name);Class owner=Nil;
 for(Class c=cls;c&&!owner;c=class_getSuperclass(c)){unsigned count=0;Method *methods=class_copyMethodList(c,&count);for(unsigned i=0;i<count;++i)if(method_getName(methods[i])==alias){owner=c;break;}free(methods);}Require(owner!=Nil,@"callback alias owner unavailable");Method method=class_getInstanceMethod(owner,alias);IMP original=method_getImplementation(method);
 IMP replacement=imp_implementationWithBlock(^(id self,id browser,id contents,BOOL active){Capture(browser,contents);((void(*)(id,SEL,id,id,BOOL))original)(self,alias,browser,contents,active);});method_setImplementation(method,replacement);gInstalled=YES;
 Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"initialURL":gURL,@"host":gHost,@"installed":@YES},@"ready.json");
}
static void Poll(void){@try{
 Scope();if(!gInstalled){if(!AllAliasesReady())return;Install();}if(gFailed)return;
 NSString *request=[gDirectory stringByAppendingPathComponent:@"request.json"];struct stat st;if(!Real(request,NO,&st))return;Require(st.st_size>0&&st.st_size<4096,@"invalid owned request size");NSData *data=[NSData dataWithContentsOfFile:request];struct stat after;Require(Real(request,NO,&after)&&st.st_dev==after.st_dev&&st.st_ino==after.st_ino&&st.st_size==after.st_size,@"owned request changed while read");NSError *error=nil;id decoded=data?[NSJSONSerialization JSONObjectWithData:data options:0 error:&error]:nil;Require([decoded isKindOfClass:NSDictionary.class]&&!error,@"malformed owned request");NSDictionary *q=decoded;NSString *rid=q[@"id"],*operation=q[@"operation"];if([rid isEqual:gLastID])return;
 Require([rid isKindOfClass:NSString.class]&&[q[@"pid"] isKindOfClass:NSNumber.class]&&[q[@"pid"] longLongValue]==getpid()&&[q[@"nonce"] isEqual:gNonce]&&[q[@"initialURL"] isEqual:gURL]&&[q[@"host"] isEqual:gHost]&&[q[@"sequence"] isKindOfClass:NSNumber.class]&&[q[@"sequence"] unsignedIntegerValue]==gPhase+1&&[rid isEqual:[NSString stringWithFormat:@"%@-%lu",gNonce,(unsigned long)(gPhase+1)]],@"owned request PID/nonce/phase/URL/host mismatch");
 NSArray *operations=@[@"select",@"register",@"unregister"];Require(gPhase<3&&[operation isEqual:operations[gPhase]],@"only fixed select/register/unregister sequence allowed");gLastID=rid;BOOL before=AllAliasesReady();
 NSMutableDictionary *reply=[@{@"id":rid,@"operation":operation,@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"initialURL":gURL,@"host":gHost,@"main":@(NSThread.isMainThread),@"ok":@NO,@"phase":@(++gPhase),@"calls":@(gCalls),@"forwardingBefore":@((BOOL)(before))} mutableCopy];
 @try{Require(before,@"canonical recorder forwarding missing");Write(reply,[NSString stringWithFormat:@"intent-%lu.json",(unsigned long)gPhase]);
 if(gPhase==1)Select();else{Require(gSelected&&gBrowser&&gContext&&gEffects&&Flag(gSelected,@"isInitialized")&&Flag(gBrowser,@"isInitialized"),@"selected context/browser no longer live");
 if(gPhase==2){Require(gCalls==0,@"register already attempted");++gCalls;[gEffects registerStylesheet:CSS withIdentifier:CSS_ID forHostPattern:gHost];++gCalls;[gEffects registerScript:JS withIdentifier:JS_ID forHostPattern:gHost];}
 else{Require(gCalls==2,@"unregister sequence differs");++gCalls;[gEffects unregisterStylesheet:CSS_ID];++gCalls;[gEffects unregisterScript:JS_ID];}}
 reply[@"ok"]=@YES;
 }@catch(NSException *e){gFailed=YES;reply[@"error"]=e.reason?:e.name;}
 reply[@"calls"]=@(gCalls);reply[@"forwardingAfter"]=@(AllAliasesReady());Write(reply,@"response.json");
 }@catch(NSException *e){gFailed=YES;Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"error":e.reason?:e.name},@"startup-error.json");}}
__attribute__((constructor))static void StartBoostLifecycle(void){@autoreleasepool{
 NSDictionary *keys=@{@"root":@"ARC_SCENARIO_BOOST_ROOT",@"directory":@"ARC_SCENARIO_BOOST_DIRECTORY",@"app":@"ARC_SCENARIO_BOOST_APP",@"library":@"ARC_SCENARIO_BOOST_LIBRARY",@"agent":@"ARC_SCENARIO_BOOST_AGENT",@"nonce":@"ARC_SCENARIO_BOOST_NONCE",@"url":@"ARC_SCENARIO_BOOST_URL",@"host":@"ARC_SCENARIO_BOOST_HOST"};NSMutableDictionary *env=[NSMutableDictionary dictionary];for(NSString *key in keys){const char *value=getenv([keys[key] UTF8String]);if(!value)return;env[key]=@(value);}const char *record=getenv("ARC_RECORD_PATH");if(!record)return;
 gRoot=env[@"root"];gDirectory=env[@"directory"];gApp=env[@"app"];gLibrary=env[@"library"];gAgent=env[@"agent"];gNonce=env[@"nonce"];gURL=env[@"url"];gHost=env[@"host"];
 if(![@[@"trace156",@"ours156"] containsObject:gAgent]||![gRoot isAbsolutePath]||!Real(gRoot,YES,NULL)||![gDirectory isEqual:[gRoot stringByAppendingPathComponent:@"renderer-boost-lifecycle-driver"]]||!Real(gDirectory,YES,&gDirectoryIdentity)||(gDirectoryIdentity.st_mode&0077)!=0||![gLibrary isEqual:[gDirectory stringByAppendingPathComponent:@"RendererBoostLifecycleDriver.dylib"]]||!Real(gLibrary,NO,NULL)||![@(record) isEqual:[gRoot stringByAppendingPathComponent:@"launch-1/record.jsonl"]])return;
 Dl_info image;if(!dladdr((const void*)&StartBoostLifecycle,&image)||!image.dli_fname||![@(image.dli_fname) isEqual:gLibrary])return;
 NSUUID *uuid=[[NSUUID alloc] initWithUUIDString:gNonce];if(!uuid||![uuid.UUIDString.lowercaseString isEqual:gNonce])return;NSURL *url=[NSURL URLWithString:gURL];if(![url.scheme isEqual:@"http"]||![url.host isEqual:@"127.0.0.1"]||![gHost isEqual:url.host]||url.port.integerValue<1024||url.port.integerValue>65535||url.user||url.password||url.query||url.fragment||![url.path isEqual:@"/renderer-boost-lifecycle-7f3a/a"])return;
 NSString *exe=NSProcessInfo.processInfo.arguments.firstObject,*app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];if(![app isEqual:gApp]||!Real(gApp,YES,NULL)||![exe isEqual:[gApp stringByAppendingPathComponent:@"Contents/MacOS/Arc"]]||![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app",gAgent]]||![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:gAgent]])return;
 unsetenv("DYLD_INSERT_LIBRARIES");gContents=[NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality];gOwners=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];
 dispatch_async(dispatch_get_main_queue(),^{gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());dispatch_source_set_timer(gTimer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);dispatch_after(dispatch_time(DISPATCH_TIME_NOW,300*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(gTimer);});});
}}
