// WRITE-only public API adapter; route approval is mandatory in the TS caller.
// No private ABI, source discovery, delegate replacement or native input.
// Own delegate callbacks are genuine engine deliveries but SIDEcars only:
// tests/recorder/app_callbacks.inc79-85 hooks ADK.ImportDelegate specifically.
// That class's init traps (../arc/ghidra/decomp/0x1005.c:61551-61565).
// Native bridge43f3874.c113-137 requires live nodes;161-178 stores delegate weak.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

@protocol ImportFixtureProfile <NSObject>
- (instancetype)initWithSafariExportedData:(NSString*)directory profileName:(NSString*)profile name:(NSString*)name categories:(long long)categories;
- (NSString*)name; - (NSString*)profileName; - (NSString*)sourcePath; - (NSString*)appPath;
- (long long)type; - (long long)supportedCategories; - (long long)constraints;
@end
@protocol ImportFixtureNode <NSObject>
- (BOOL)isValid; - (BOOL)isFolder; - (NSString*)nodeID; - (NSString*)uuid;
- (NSString*)parentNodeID; - (NSString*)title; - (NSURL*)url;
- (NSDate*)dateAdded; - (NSArray*)liveChildren;
@end
@protocol ImportFixtureContext <NSObject>
- (id)getBookmarksTree;
- (BOOL)addBookmarkFolder:(NSString*)title withParent:(id)parent withIndexInParent:(NSInteger)index withUuid:(NSString*)uuid;
- (BOOL)import:(id)profile withCategories:(long long)categories withAllowedExtensions:(NSArray*)extensions withBookmarkImportTargets:(id)targets skipExistingCookies:(BOOL)skip withDelegate:(id)delegate error:(NSError**)error;
@end
@protocol ImportFixtureTargets <NSObject>
- (instancetype)initWithBookmarkBarBookmarksTargetNode:(id)bar withOtherBookmarksTargetNode:(id)other;
- (id)targetNodeForBookmarkBarBookmarks; - (id)targetNodeForOtherBookmarks;
@end
// Exact seven unique identities from pristine app ArcImportDelegate protocol:
// research/.../protocols/initial/protocol-specs/ArcImportDelegate.md:27-33.
@protocol ArcImportDelegate <NSObject>
- (void)didImportInitialize:(id)profile;
- (void)didImportStart:(id)profile;
- (void)didImportCategoryStart:(id)profile withCategory:(long long)category;
- (void)didImportCategoryEnd:(id)profile withCategory:(long long)category withError:(NSError*)error;
- (void)didImportEnd:(id)profile withError:(NSError*)error;
- (void)didImportLockDialogShow:(NSString*)title withDialogBody:(NSString*)body completion:(void(^)(BOOL))completion;
- (void)didImportSessionWindows:(NSArray*)windows;
@end

static NSString *gRoot,*gDirectory,*gSource,*gHTML,*gAgent,*gNonce,*gBarUUID,*gOtherUUID,*gURL,*gRequest,*gID,*gOperation;
static NSInteger gStage;
static NSHashTable *gContents;
static NSMapTable *gOwners;
static id<ImportFixtureContext> gContext;
static id gBrowser;
static id<ImportFixtureTargets> gTargets;
static id<ImportFixtureProfile> gProfile;
static id<ArcImportDelegate> gDelegate; // Strong for weak bridge.
static NSMutableDictionary *gReply;
static NSMutableArray *gEvents;
static NSObject *gEventLock;
static NSUInteger gEventSeen,gEventProcessed,gAttempts;
static BOOL gInstalled,gStarted,gImportReturned,gFinalScheduled;
static dispatch_source_t gTimer;
static struct stat gDirectoryStat,gSourceStat,gHTMLStat;
static void Require(BOOL ok,NSString *why){if(!ok)@throw [NSException exceptionWithName:@"DisposableImportFixture" reason:why userInfo:nil];}
static id Get(id object,NSString *selector){return ((id(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));}
static BOOL Flag(id object,NSString *selector){return ((BOOL(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));}
static BOOL Real(NSString *path,BOOL directory,struct stat *out){struct stat st;if(lstat(path.fileSystemRepresentation,&st)!=0||S_ISLNK(st.st_mode)||(directory?!S_ISDIR(st.st_mode):!S_ISREG(st.st_mode)))return NO;if(out)*out=st;return [[path stringByResolvingSymlinksInPath] isEqual:path];}
static BOOL Same(struct stat a,struct stat b){return a.st_dev==b.st_dev&&a.st_ino==b.st_ino;}
static void Write(id object,NSString *path){NSError *error=nil;NSData *data=[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:&error];Require(data&&[data writeToFile:path options:NSDataWritingAtomic error:&error],@"owned evidence write failed");}
static void Scope(void){
 struct stat d,s,h;Require(Real(gDirectory,YES,&d)&&Same(d,gDirectoryStat)&&(d.st_mode&0077)==0,@"driver directory changed or exposed");
 Require(Real(gSource,YES,&s)&&Same(s,gSourceStat)&&(s.st_mode&0077)==0,@"fixture directory changed or exposed");
 NSArray *names=[[NSFileManager defaultManager] contentsOfDirectoryAtPath:gSource error:NULL];Require(names.count==1&&[names[0] isEqual:@"import-bookmarks.html"],@"fixture source is not exactly one HTML file");
 NSString *path=[gSource stringByAppendingPathComponent:@"import-bookmarks.html"];Require(Real(path,NO,&h)&&Same(h,gHTMLStat)&&h.st_size>0&&h.st_size<8192,@"fixture HTML identity changed");
 NSData *bytes=[NSData dataWithContentsOfFile:path];Require([bytes isEqual:[gHTML dataUsingEncoding:NSUTF8StringEncoding]],@"fixture HTML bytes changed");
 struct stat after;Require(Real(path,NO,&after)&&Same(h,after)&&after.st_size==h.st_size,@"fixture changed during read");
}
static SEL Alias(NSString *name){return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]);}
static BOOL AliasReady(Class cls,NSString *name){Method canonical=class_getInstanceMethod(cls,NSSelectorFromString(name)),alias=class_getInstanceMethod(cls,Alias(name));return cls&&canonical&&alias&&method_getImplementation(canonical)==(IMP)_objc_msgForward&&method_getImplementation(alias)!=(IMP)_objc_msgForward;}
static BOOL AllAliasesReady(void){NSDictionary *selectors=@{
 @"ArcBrowser":@[@"context",@"isInitialized"],@"ArcWebContents":@[@"url",@"isInitialized"],@"ArcBrowserContextID":@[@"directoryBasename",@"isIncognito"],
 @"ArcBrowserContext":@[@"id",@"getBookmarksTree",@"addBookmarkFolder:withParent:withIndexInParent:withUuid:",@"import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:"],
 @"ArcImportSourceProfile":@[@"initWithSafariExportedData:profileName:name:categories:",@"name",@"profileName",@"sourcePath",@"appPath",@"type",@"supportedCategories",@"constraints"],
 @"ArcBookmarkImportTargets":@[@"initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode:",@"targetNodeForBookmarkBarBookmarks",@"targetNodeForOtherBookmarks"],
 @"ArcBookmarkTreeNode":@[@"isValid",@"isFolder",@"nodeID",@"uuid",@"parentNodeID",@"title",@"url",@"dateAdded",@"liveChildren"]};
 for(NSString *cls in selectors)for(NSString *selector in selectors[cls])if(!AliasReady(NSClassFromString(cls),selector))return NO;
 return AliasReady(NSClassFromString(@"_TtC3ADK15ChromiumBrowser"),@"browser:didCreateNewWebContents:isActive:");
}
static void ABI(NSString *cls,NSString *selector,NSString *encoding){Method m=class_getInstanceMethod(NSClassFromString(cls),NSSelectorFromString(selector));Require(m&&[encoding isEqual:@(method_getTypeEncoding(m))],[@"public ABI changed: " stringByAppendingString:selector]);}
static NSDictionary *ProfileFacts(id<ImportFixtureProfile> p){Require(p!=nil,@"nil callback profile");NSString *name=p.name,*profile=p.profileName,*source=p.sourcePath,*app=p.appPath;return @{@"name":name?:NSNull.null,@"profileName":profile?:NSNull.null,@"sourcePath":source?:NSNull.null,@"appPath":app?:NSNull.null,@"type":@(p.type),@"supportedCategories":@(p.supportedCategories),@"constraints":@(p.constraints)};}
static NSDictionary *ExpectedProfile(void){return @{@"name":@"ArcCore Disposable Safari Export",@"profileName":@"ArcCore Fixture Source",@"sourcePath":gSource,@"appPath":@"",@"type":@12,@"supportedCategories":@2,@"constraints":@0};}
static id ErrorFacts(NSError *error){return error?@{@"domain":error.domain?:@"",@"code":@(error.code),@"description":error.localizedDescription?:@""}:NSNull.null;}
static void Publish(void){gReply[@"events"]=[gEvents copy];gReply[@"recorderAliasesAfter"]=@(AllAliasesReady());gReply[@"importAttempts"]=@(gAttempts);Write(gReply,[gDirectory stringByAppendingPathComponent:@"response.json"]);}
static void Fail(NSString *why){gReply[@"complete"]=@NO;gReply[@"error"]=why;Publish();}
static id<ImportFixtureNode> Find(NSArray *nodes,NSString *uuid){id found=nil;for(id<ImportFixtureNode> n in nodes)if([n.uuid.lowercaseString isEqual:uuid]){Require(found==nil,@"duplicate fixture UUID");found=n;}return found;}
static id<ImportFixtureNode> Bar(void){
 id<ImportFixtureNode> root=[gContext getBookmarksTree];Require(root&&root.isValid&&root.isFolder,@"valid root unavailable");NSString *rootID=root.nodeID;id<ImportFixtureNode> bar=nil;
 for(id<ImportFixtureNode> n in root.liveChildren){if([n.nodeID isEqual:@"1"]){Require(!bar&&n.isValid&&n.isFolder&&[n.parentNodeID isEqual:rootID],@"invalid/duplicate public barID1");bar=n;}else Require(n.isValid&&n.isFolder&&n.liveChildren.count==0,@"unexpected bookmarks outside dedicated targets");}
 Require(bar!=nil,@"public live bookmark barID1 missing");return bar;
}
static NSDictionary *Tree(id<ImportFixtureNode> node,NSUInteger depth,NSUInteger *budget){
 Require(node&&depth<=3&&(*budget)++<16,@"import tree exceeded fixed fixture budget");BOOL folder=node.isFolder,valid=node.isValid;NSString *nodeID=node.nodeID,*uuid=node.uuid,*parent=node.parentNodeID,*title=node.title;NSURL *url=node.url;NSDate *added=node.dateAdded;
 Require(valid&&nodeID.length&&uuid.length&&parent.length&&title!=nil,@"invalid model identity/scalars");NSArray *children=node.liveChildren;Require(children!=nil&&children.count<=2,@"unexpected children/invalid live model");NSMutableArray *out=[NSMutableArray array];for(id child in children)[out addObject:Tree(child,depth+1,budget)];
 return @{@"id":nodeID,@"uuid":uuid.lowercaseString,@"parentID":parent,@"title":title,@"url":url.absoluteString?:NSNull.null,@"folder":@((BOOL)(folder)),@"valid":@((BOOL)(valid)),@"added":added?@(added.timeIntervalSince1970):NSNull.null,@"children":out};
}
static void SnapshotTargets(void){
 Scope();id<ImportFixtureNode> bar=Bar();NSArray *children=bar.liveChildren;id<ImportFixtureNode> a=Find(children,gBarUUID),b=Find(children,gOtherUUID);Require(children.count==2&&a&&b&&a!=b&&a.isValid&&b.isValid&&a.isFolder&&b.isFolder&&![a.nodeID isEqual:b.nodeID]&&[a.parentNodeID isEqual:@"1"]&&[b.parentNodeID isEqual:@"1"]&&[a.title isEqual:[@"ArcCore Import Bar " stringByAppendingString:gNonce]]&&[b.title isEqual:[@"ArcCore Import Other " stringByAppendingString:gNonce]],@"dedicated live targets missing/changed");NSUInteger budget=0;gReply[@"targets"]=@{@"bar":Tree(a,0,&budget),@"other":Tree(b,0,&budget)};
}
static void Finalize(void){
 @try{Require(gImportReturned&&gAttempts==1,@"terminal callback preceded recorded import result");NSArray *expected=@[@"initialize",@"start",@"category-start",@"category-end",@"end"];Require(gEvents.count==5&&gEventProcessed==5,@"missing/extra import callbacks");for(NSUInteger i=0;i<5;++i){NSDictionary *event=gEvents[i];Require([event[@"ordinal"] unsignedIntegerValue]==i+1&&[event[@"kind"] isEqual:expected[i]]&&[event[@"profile"] isEqual:ExpectedProfile()]&&event[@"error"]==NSNull.null&&[event[@"category"] isEqual:((i==2||i==3)?@2:NSNull.null)],@"callback lifecycle/scalar/category/error mismatch");}SnapshotTargets();gReply[@"complete"]=@YES;Publish();}
 @catch(NSException *e){Fail(e.reason?:e.name);}
}
static void Event(NSString *kind,id profile,NSNumber *category,NSError *error){
 BOOL main=NSThread.isMainThread;NSString *thread=NSThread.currentThread.name;if(!thread.length)thread=main?@"main":@"unnamed-background";
 @synchronized(gEventLock){NSUInteger ordinal=++gEventSeen;
 // Capture actual delivery thread/order now; all getter snapshots are later
 // main-queue public calls. No callback-thread contract or inline claim.
 dispatch_async(dispatch_get_main_queue(),^{@try{Require(gReply!=nil,@"callback before request");Require(ordinal==++gEventProcessed,@"concurrent callback delivery queue order changed");NSDictionary *facts=profile?ProfileFacts(profile):nil;
 [gEvents addObject:@{@"ordinal":@(ordinal),@"kind":kind,@"category":category?:NSNull.null,@"error":ErrorFacts(error),@"mainThread":@((BOOL)(main)),@"thread":thread,@"profile":facts?:NSNull.null}];Publish();Require(gEvents.count<=5&&![kind isEqual:@"lock"]&&![kind isEqual:@"sessions"],@"unexpected/extra lock/session/import callback");
 if([kind isEqual:@"end"]){Require(!gFinalScheduled,@"duplicate terminal callback");gFinalScheduled=YES;dispatch_async(dispatch_get_main_queue(),^{Finalize();});}}
 @catch(NSException *e){Fail(e.reason?:e.name);}});}
}
@interface DisposableImportDelegate : NSObject <ArcImportDelegate>
@end
@implementation DisposableImportDelegate
- (void)didImportInitialize:(id)p{Event(@"initialize",p,nil,nil);}
- (void)didImportStart:(id)p{Event(@"start",p,nil,nil);}
- (void)didImportCategoryStart:(id)p withCategory:(long long)c{Event(@"category-start",p,@(c),nil);}
- (void)didImportCategoryEnd:(id)p withCategory:(long long)c withError:(NSError*)e{Event(@"category-end",p,@(c),e);}
- (void)didImportEnd:(id)p withError:(NSError*)e{Event(@"end",p,nil,e);}
- (void)didImportLockDialogShow:(NSString*)title withDialogBody:(NSString*)body completion:(void(^)(BOOL))completion{(void)title;(void)body;(void)completion;Event(@"lock",nil,nil,nil);}
- (void)didImportSessionWindows:(NSArray*)windows{(void)windows;Event(@"sessions",nil,nil,nil);}
@end
static void Bind(void){id selected=nil;for(id contents in gContents)if(Flag(contents,@"isInitialized")&&[[Get(contents,@"url") absoluteString] isEqual:gURL]){Require(selected==nil,@"multiple live genuine wrappers at exact URL");selected=contents;}Require(selected!=nil,@"genuine exact fixture wrapper missing");gBrowser=[gOwners objectForKey:selected];Require(gBrowser&&Flag(gBrowser,@"isInitialized"),@"genuine fixture browser unavailable");id context=Get(gBrowser,@"context"),identifier=Get(context,@"id");Require(context&&[Get(identifier,@"directoryBasename") isEqual:@"Default"]&&!Flag(identifier,@"isIncognito"),@"regular Default fixture context required");gContext=context;}
static void Run(void){
 Require(!gStarted,@"request already consumed");gStarted=YES;
 gReply=[@{@"id":gID,@"operation":gOperation,@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"url":gURL,@"stage":@(gStage),@"complete":@NO,@"main":@(NSThread.isMainThread),@"recorderAliasesBefore":@(AllAliasesReady()),@"importAttempts":@0,@"accepted":@NO,@"immediateError":NSNull.null,@"callbackScope":@"own-public-protocol-sidecar-only"} mutableCopy];
 @try{Scope();Require(AllAliasesReady(),@"canonical recorder forwarding unavailable");Bind();
 if(gStage==2){Require([gOperation isEqual:@"verify"],@"restart is read-only");SnapshotTargets();gReply[@"complete"]=@YES;Publish();return;}
 Require([gOperation isEqual:@"import"],@"first stage must import exactly once");id<ImportFixtureNode> bar=Bar();NSArray *before=bar.liveChildren;Require(before.count==0,@"fresh owned bookmark bar must be empty");
 Require([gContext addBookmarkFolder:[@"ArcCore Import Bar " stringByAppendingString:gNonce] withParent:bar withIndexInParent:0 withUuid:gBarUUID],@"first target creation rejected");
 Require([gContext addBookmarkFolder:[@"ArcCore Import Other " stringByAppendingString:gNonce] withParent:bar withIndexInParent:1 withUuid:gOtherUUID],@"second target creation rejected");
 id<ImportFixtureNode> freshBar=Bar();NSArray *nodes=freshBar.liveChildren;id<ImportFixtureNode> a=Find(nodes,gBarUUID),b=Find(nodes,gOtherUUID);
 Require(nodes.count==2&&a&&b&&a!=b&&a.isValid&&b.isValid&&a.isFolder&&b.isFolder&&a.liveChildren.count==0&&b.liveChildren.count==0&&![a.nodeID isEqual:b.nodeID]&&[a.parentNodeID isEqual:@"1"]&&[b.parentNodeID isEqual:@"1"],@"two distinct empty live target folders required");
 gTargets=[(id<ImportFixtureTargets>)[NSClassFromString(@"ArcBookmarkImportTargets") alloc] initWithBookmarkBarBookmarksTargetNode:a withOtherBookmarksTargetNode:b];Require(gTargets&&gTargets.targetNodeForBookmarkBarBookmarks==a&&gTargets.targetNodeForOtherBookmarks==b,@"public target readback changed destination identity");
 gProfile=[(id<ImportFixtureProfile>)[NSClassFromString(@"ArcImportSourceProfile") alloc] initWithSafariExportedData:gSource profileName:@"ArcCore Fixture Source" name:@"ArcCore Disposable Safari Export" categories:2];NSDictionary *facts=ProfileFacts(gProfile);gReply[@"profile"]=facts;Require([facts isEqual:ExpectedProfile()],@"explicit source profile scalar readback mismatch");
 gDelegate=[DisposableImportDelegate new];Scope();gAttempts=1;Publish();NSError *error=nil;
 BOOL accepted=[gContext import:gProfile withCategories:2 withAllowedExtensions:@[] withBookmarkImportTargets:gTargets skipExistingCookies:YES withDelegate:gDelegate error:&error];gImportReturned=YES;gReply[@"accepted"]=@((BOOL)(accepted));gReply[@"immediateError"]=ErrorFacts(error);Publish();Require(accepted&&error==nil,@"public import rejected or immediate NSError; never retry");
 }@catch(NSException *e){Fail(e.reason?:e.name);}
}
static void Install(void){
 Protocol *delegateProtocol=objc_getProtocol("ArcImportDelegate");Require(delegateProtocol&&class_conformsToProtocol(DisposableImportDelegate.class,delegateProtocol),@"public import delegate protocol conformance unavailable");
 ABI(@"ArcImportSourceProfile",@"initWithSafariExportedData:profileName:name:categories:",@"@48@0:8@16@24@32q40");ABI(@"ArcBrowserContext",@"import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:",@"B68@0:8@16q24@32@40B48@52^@60");ABI(@"ArcBookmarkImportTargets",@"initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode:",@"@32@0:8@16@24");ABI(@"ArcBrowserContext",@"addBookmarkFolder:withParent:withIndexInParent:withUuid:",@"B48@0:8@16@24q32@40");
 Class cls=NSClassFromString(@"_TtC3ADK15ChromiumBrowser");NSString *name=@"browser:didCreateNewWebContents:isActive:";SEL alias=Alias(name);Require(AliasReady(cls,name),@"genuine callback recorder alias unavailable");Class owner=Nil;for(Class c=cls;c&&!owner;c=class_getSuperclass(c)){unsigned count=0;Method *methods=class_copyMethodList(c,&count);for(unsigned i=0;i<count;++i)if(method_getName(methods[i])==alias){owner=c;break;}free(methods);}Require(owner!=Nil,@"genuine callback alias owner missing");Method method=class_getInstanceMethod(owner,alias);IMP original=method_getImplementation(method);
 IMP replacement=imp_implementationWithBlock(^(id self,id browser,id contents,BOOL active){if(browser&&contents){[gContents addObject:contents];[gOwners setObject:browser forKey:contents];}((void(*)(id,SEL,id,id,BOOL))original)(self,alias,browser,contents,active);});method_setImplementation(method,replacement);gInstalled=YES;
 Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"stage":@(gStage),@"url":gURL,@"installed":@YES},[gDirectory stringByAppendingPathComponent:@"ready.json"]);
}
static void Poll(void){@try{
 if(!gInstalled){if(!AllAliasesReady())return;Install();}
 if(gStarted)return;struct stat st;if(!Real(gRequest,NO,&st))return;Require(st.st_size>0&&st.st_size<8192,@"invalid owned request size");NSData *data=[NSData dataWithContentsOfFile:gRequest];struct stat after;Require(Real(gRequest,NO,&after)&&Same(st,after)&&st.st_size==after.st_size,@"request changed during read");NSError *error=nil;id value=data?[NSJSONSerialization JSONObjectWithData:data options:0 error:&error]:nil;Require([value isKindOfClass:NSDictionary.class]&&!error,@"malformed owned request");NSDictionary *q=value;
 Require([q[@"id"] isKindOfClass:NSString.class]&&[q[@"id"] isEqual:[NSString stringWithFormat:@"%@-%ld",gNonce,(long)gStage]]&&[q[@"pid"] isKindOfClass:NSNumber.class]&&[q[@"pid"] longLongValue]==getpid()&&[q[@"nonce"] isEqual:gNonce]&&[q[@"url"] isEqual:gURL]&&[q[@"source"] isEqual:gSource]&&[q[@"barUUID"] isEqual:gBarUUID]&&[q[@"otherUUID"] isEqual:gOtherUUID]&&[q[@"stage"] integerValue]==gStage&&[q[@"operation"] isEqual:(gStage==1?@"import":@"verify")],@"owned request identity/scope mismatch");gID=q[@"id"];gOperation=q[@"operation"];Run();
 }@catch(NSException *e){if(gReply)Fail(e.reason?:e.name);else{Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"error":e.reason?:e.name},[gDirectory stringByAppendingPathComponent:@"startup-error.json"]);if(gTimer)dispatch_source_cancel(gTimer);}}}
__attribute__((constructor))static void StartDisposableImport(void){@autoreleasepool{
 NSDictionary *keys=@{@"app":@"ARC_SCENARIO_IMPORT_APP",@"library":@"ARC_SCENARIO_IMPORT_LIBRARY",@"root":@"ARC_SCENARIO_IMPORT_ROOT",@"directory":@"ARC_SCENARIO_IMPORT_DIRECTORY",@"source":@"ARC_SCENARIO_IMPORT_SOURCE",@"html":@"ARC_SCENARIO_IMPORT_HTML",@"agent":@"ARC_SCENARIO_IMPORT_AGENT",@"nonce":@"ARC_SCENARIO_IMPORT_NONCE",@"bar":@"ARC_SCENARIO_IMPORT_BAR_UUID",@"other":@"ARC_SCENARIO_IMPORT_OTHER_UUID",@"url":@"ARC_SCENARIO_IMPORT_URL",@"stage":@"ARC_SCENARIO_IMPORT_STAGE"};NSMutableDictionary *env=[NSMutableDictionary dictionary];for(NSString *key in keys){const char *v=getenv([keys[key] UTF8String]);if(!v)return;env[key]=@(v);}const char *record=getenv("ARC_RECORD_PATH");if(!record)return;
 gRoot=env[@"root"];gDirectory=env[@"directory"];gSource=env[@"source"];gHTML=env[@"html"];gAgent=env[@"agent"];gNonce=env[@"nonce"];gBarUUID=env[@"bar"];gOtherUUID=env[@"other"];gURL=env[@"url"];gStage=[env[@"stage"] integerValue];
 if(![@[@"trace156",@"ours156",@"rel156"] containsObject:gAgent]||![@[@"1",@"2"] containsObject:env[@"stage"]]||![gRoot isAbsolutePath]||!Real(gRoot,YES,NULL))return;
 NSString *base=[gRoot stringByAppendingPathComponent:@"disposable-import-driver"];NSString *library=[base stringByAppendingPathComponent:@"ScenarioDisposableImportDriver.dylib"];Dl_info ownImage;if(!dladdr((const void*)&StartDisposableImport,&ownImage)||!ownImage.dli_fname||![@(ownImage.dli_fname) isEqual:library]||![env[@"library"] isEqual:library]||!Real(library,NO,NULL))return;
 if(!Real(base,YES,NULL)||![gDirectory isEqual:[base stringByAppendingPathComponent:[NSString stringWithFormat:@"launch-%ld",(long)gStage]]]||![gSource isEqual:[base stringByAppendingPathComponent:@"fixture-source"]]||![@(record) isEqual:[gRoot stringByAppendingPathComponent:[NSString stringWithFormat:@"launch-%ld/record.jsonl",(long)gStage]]])return;
 if(!Real(gDirectory,YES,&gDirectoryStat)||!Real(gSource,YES,&gSourceStat)||!Real([gSource stringByAppendingPathComponent:@"import-bookmarks.html"],NO,&gHTMLStat))return;
 for(NSString *u in @[gNonce,gBarUUID,gOtherUUID]){NSUUID *uuid=[[NSUUID alloc] initWithUUIDString:u];if(!uuid||![uuid.UUIDString.lowercaseString isEqual:u])return;}if([gBarUUID isEqual:gOtherUUID]||[gNonce isEqual:gBarUUID]||[gNonce isEqual:gOtherUUID])return;
 NSURL *url=[NSURL URLWithString:gURL];if(![url.scheme isEqual:@"http"]||![url.host isEqual:@"127.0.0.1"]||url.port.integerValue<1024||url.port.integerValue>65535||url.user||url.password||url.query||url.fragment||![url.path isEqual:[NSString stringWithFormat:@"/arc-disposable-import-7f3a/%ld",(long)gStage]]||gHTML.length>8192)return;
 NSString *exe=NSProcessInfo.processInfo.arguments.firstObject,*app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];if(![app isEqual:env[@"app"]]||!Real(app,YES,NULL)||![exe isEqual:[app stringByAppendingPathComponent:@"Contents/MacOS/Arc"]]||![exe.lastPathComponent isEqual:@"Arc"]||![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app",gAgent]]||![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:gAgent]])return;
 unsetenv("DYLD_INSERT_LIBRARIES");gRequest=[gDirectory stringByAppendingPathComponent:@"request.json"];gContents=[NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality];gOwners=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];gEvents=[NSMutableArray array];gEventLock=[NSObject new];
 dispatch_async(dispatch_get_main_queue(),^{gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());dispatch_source_set_timer(gTimer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);dispatch_after(dispatch_time(DISPATCH_TIME_NOW,300*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(gTimer);});});
}}
