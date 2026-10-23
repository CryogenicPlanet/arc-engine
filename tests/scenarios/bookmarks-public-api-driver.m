// Canonical test-only bookmark public API driver. No C++ ABI or native input.
// Bookmark chain adapted from logs/wp/profile.batch11/bookmark-fixture/probe.mm
// SHA256 9ba51ecf20030ab5c370e25c4050ed83feb25181eaad7105a6693839094d0505.
// Frozen inputs fully verified against that directory's 22-file manifest.json.
// Only genuine app callback aliases are wrapped; canonical recorder forwarding
// stays installed. The unrelated profile storage/cookie chain is not included.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <stdlib.h>
#include <unistd.h>

static NSString *gDirectory,*gRequest,*gNonce,*gAgent,*gFixtureURL,*gLastID;
static NSHashTable *gContents;
static NSMapTable *gOwners;
static NSMutableSet *gWrapped;
static NSMutableDictionary *gResults;
static id gContext,gBrowser;
static NSURL *gOrigin;
static BOOL gInstalled,gStarted,gDone;
static dispatch_source_t gTimer;
static id Get(id object,NSString *selector){return ((id(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));}
static BOOL Flag(id object,NSString *selector){return ((BOOL(*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));}
static void Require(BOOL value,NSString *why){if(!value)@throw [NSException exceptionWithName:@"BookmarksPublicAPI" reason:why userInfo:nil];}
static void Write(NSDictionary *object,NSString *path){NSError *error=nil;NSData *data=[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:&error];if(!data||![data writeToFile:path options:NSDataWritingAtomic error:&error])NSLog(@"BookmarksPublicAPI write failed: %@",error);}
static SEL Alias(NSString *name){return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]);}
static BOOL AliasReady(Class cls,NSString *name){if(!cls)return NO;Method canonical=class_getInstanceMethod(cls,NSSelectorFromString(name)),alias=class_getInstanceMethod(cls,Alias(name));return canonical&&alias&&method_getImplementation(canonical)==(IMP)_objc_msgForward&&method_getImplementation(alias)!=(IMP)_objc_msgForward;}
static BOOL AllAliasesReady(void){NSDictionary *selectors=@{
 @"ArcBrowser":@[@"context",@"isInitialized"],@"ArcWebContents":@[@"url",@"isInitialized"],
 @"ArcBrowserContext":@[@"id",@"getBookmarksTree",@"addBookmark:withUrl:withParent:withIndexInParent:withUuid:",@"renameBookmark:withTitle:",@"editBookmark:withNewURL:",@"removeBookmark:withParent:",@"isBookmarked:"],
 @"ArcBrowserContextID":@[@"directoryBasename",@"isIncognito"],
 @"ArcBookmarkTreeNode":@[@"isFolder",@"isValid",@"nodeID",@"uuid",@"parentNodeID",@"title",@"url",@"dateAdded",@"dateFolderModified",@"liveChildren"]};
 for(NSString *cls in selectors)for(NSString *sel in selectors[cls])if(!AliasReady(NSClassFromString(cls),sel))return NO;
 return AliasReady(NSClassFromString(@"_TtC3ADK15ChromiumBrowser"),@"browser:didCreateNewWebContents:isActive:");}
static void Install(void){Class cls=NSClassFromString(@"_TtC3ADK15ChromiumBrowser");NSString *name=@"browser:didCreateNewWebContents:isActive:";Require(AliasReady(cls,name),@"genuine create callback recorder alias missing");SEL alias=Alias(name);Class owner=Nil;
 for(Class c=cls;c&&!owner;c=class_getSuperclass(c)){unsigned n=0;Method *methods=class_copyMethodList(c,&n);for(unsigned i=0;i<n;++i)if(method_getName(methods[i])==alias){owner=c;break;}free(methods);}
 Require(owner!=Nil,@"genuine callback alias owner missing");NSString *key=[NSString stringWithFormat:@"%@/%@",NSStringFromClass(owner),name];Require(![gWrapped containsObject:key],@"create alias already wrapped");Method method=class_getInstanceMethod(owner,alias);IMP original=method_getImplementation(method);
 IMP replacement=imp_implementationWithBlock(^(id self,id browser,id contents,BOOL active){if(browser&&contents){[gContents addObject:contents];[gOwners setObject:browser forKey:contents];}((void(*)(id,SEL,id,id,BOOL))original)(self,alias,browser,contents,active);});method_setImplementation(method,replacement);[gWrapped addObject:key];gInstalled=YES;
 Write(@{@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"installed":@YES,@"recorderAliases":@YES},[gDirectory stringByAppendingPathComponent:@"ready.json"]);}
static void Finish(NSString *error){if(gDone)return;gDone=YES;gResults[@"complete"]=@((BOOL)(error==nil));gResults[@"recorderAliasesAfter"]=@(AllAliasesReady());if(error)gResults[@"error"]=error;Write(gResults,[gRequest stringByAppendingString:@".response.json"]);}
@protocol ProfileProbeBookmarkNode <NSObject>
- (BOOL)isFolder;
- (BOOL)isValid;
- (NSString*)nodeID;
- (NSString*)uuid;
- (NSString*)parentNodeID;
- (NSString*)title;
- (NSURL*)url;
- (NSDate*)dateAdded;
- (NSDate*)dateFolderModified;
- (NSArray*)liveChildren;
@end
@protocol ProfileProbeBookmarkContext <NSObject>
- (id)getBookmarksTree;
- (BOOL)addBookmark:(NSString*)title withUrl:(NSURL*)url withParent:(id)parent withIndexInParent:(NSInteger)index withUuid:(NSString*)uuid;
- (BOOL)renameBookmark:(id)node withTitle:(NSString*)title;
- (BOOL)editBookmark:(id)node withNewURL:(NSURL*)url;
- (BOOL)removeBookmark:(id)node withParent:(id)parent;
- (BOOL)isBookmarked:(NSURL*)url;
@end
static id<ProfileProbeBookmarkNode> FindUUID(NSArray* nodes, NSString* uuid) {
  for (id<ProfileProbeBookmarkNode> node in nodes) if ([[node.uuid lowercaseString] isEqual:uuid]) return node;
  return nil;
}
static void BeginBookmarks(void) {
  if (gDone) return;
  @try {
    id<ProfileProbeBookmarkContext> context=gContext;
    NSArray* selectors=@[@"getBookmarksTree",@"addBookmark:withUrl:withParent:withIndexInParent:withUuid:",
      @"renameBookmark:withTitle:",@"editBookmark:withNewURL:",@"removeBookmark:withParent:",@"isBookmarked:"];
    for (NSString* selector in selectors) if (![context respondsToSelector:NSSelectorFromString(selector)]) {
      Finish([@"missing bookmark selector: " stringByAppendingString:selector]);return;
    }
    id<ProfileProbeBookmarkNode> root=[context getBookmarksTree], parent=nil;
    for (id<ProfileProbeBookmarkNode> node in root.liveChildren) {
      if ([node.nodeID isEqual:@"1"] && node.isFolder && node.isValid && [node.parentNodeID isEqual:root.nodeID]) parent=node;
    }
    if (!root.isValid || !parent) { Finish(@"valid public bookmark bar ID1 unavailable");return; }
    NSString* parentID=[parent.nodeID copy], *parentUUID=[parent.uuid copy];
    NSDate* before=parent.dateFolderModified;
    if (!before || !parentUUID.length || FindUUID(parent.liveChildren,gNonce)) {
      Finish(@"bookmark date/identity preflight failed or nonce collision");return;
    }
    NSURL* oldURL=[NSURL URLWithString:[gOrigin.absoluteString stringByAppendingFormat:@"/bookmarks/%@/ε🪐",gNonce]];
    NSURL* newURL=[NSURL URLWithString:[gOrigin.absoluteString stringByAppendingFormat:@"/bookmarks/%@/改ε🪐",gNonce]];
    NSString* oldTitle=[@"Bookmark ε🪐 " stringByAppendingString:gNonce];
    NSString* newTitle=[@"Renamed 改ε🪐 " stringByAppendingString:gNonce];
    NSTimeInterval windowStart=NSDate.date.timeIntervalSince1970;
    BOOL added=[context addBookmark:oldTitle withUrl:oldURL withParent:parent withIndexInParent:0 withUuid:gNonce];
    NSTimeInterval windowEnd=NSDate.date.timeIntervalSince1970;
    NSDate* retainedAfter=parent.dateFolderModified;
    id<ProfileProbeBookmarkNode> freshRoot=[context getBookmarksTree], freshParent=nil;
    for (id<ProfileProbeBookmarkNode> node in freshRoot.liveChildren) if ([node.nodeID isEqual:parentID] && [node.uuid isEqual:parentUUID]) freshParent=node;
    id<ProfileProbeBookmarkNode> leaf=FindUUID(freshParent.liveChildren,gNonce);
    NSDate* freshDate=freshParent.dateFolderModified;
    // JSON stores numeric raw dates only in this sidecar; normalized facts below
    // contain booleans/strings and never substitute a fixed wall-clock value.
    Write(@{@"nonce":gNonce,@"pid":@(NSProcessInfo.processInfo.processIdentifier),
      @"leafUUID":leaf.uuid?:NSNull.null,@"parentUUID":parentUUID,@"parentID":parentID,
      @"windowStart":@(windowStart),@"windowEnd":@(windowEnd),
      @"retainedParentBefore":before ? @(before.timeIntervalSince1970) : NSNull.null,
      @"retainedParentAfter":retainedAfter ? @(retainedAfter.timeIntervalSince1970) : NSNull.null,
      @"freshParentModified":freshDate ? @(freshDate.timeIntervalSince1970) : NSNull.null,
      @"leafAdded":leaf.dateAdded ? @(leaf.dateAdded.timeIntervalSince1970) : NSNull.null,
      @"leafFolderModified":leaf.dateFolderModified ? @(leaf.dateFolderModified.timeIntervalSince1970) : NSNull.null},
      [gRequest stringByAppendingString:@".bookmark-dates.json"]);
    if (!added || !freshParent || !leaf || ![leaf.uuid isEqual:gNonce] || ![leaf.parentNodeID isEqual:parentID] ||
        ![leaf.title isEqual:oldTitle] || ![leaf.url isEqual:oldURL] || !leaf.isValid) {
      Finish(@"bookmark insertion/identity reconciliation failed; discard disposable profile");return;
    }
    NSMutableDictionary* facts=[NSMutableDictionary dictionary];
    facts[@"added"]=@((BOOL)(added));
    facts[@"retainedParentDateUnchanged"]=@((BOOL)([before isEqualToDate:retainedAfter]));
    facts[@"freshRootDistinctSameIdentity"]=@((BOOL)(root!=freshRoot && [root.nodeID isEqual:freshRoot.nodeID] && [root.uuid isEqual:freshRoot.uuid]));
    facts[@"freshParentDistinctSameIdentity"]=@((BOOL)(parent!=freshParent && [parent.nodeID isEqual:freshParent.nodeID] && [parent.uuid isEqual:freshParent.uuid]));
    facts[@"freshParentDateWithinAddWindow"]=@((BOOL)(freshDate && freshDate.timeIntervalSince1970>=windowStart && freshDate.timeIntervalSince1970<=windowEnd));
    facts[@"freshParentDateAdvanced"]=@((BOOL)(freshDate && freshDate.timeIntervalSince1970>before.timeIntervalSince1970));
    facts[@"leafFolderModifiedNil"]=@((BOOL)(leaf.dateFolderModified==nil));
    facts[@"leafAddedWithinAddWindow"]=@((BOOL)(leaf.dateAdded && leaf.dateAdded.timeIntervalSince1970>=windowStart && leaf.dateAdded.timeIntervalSince1970<=windowEnd));
    facts[@"oldURLInitiallyBookmarked"]=@((BOOL)([context isBookmarked:oldURL]));
    facts[@"renamed"]=@((BOOL)([context renameBookmark:leaf withTitle:newTitle]));
    facts[@"edited"]=@((BOOL)([context editBookmark:leaf withNewURL:newURL]));
    facts[@"retainedLeafOriginalValues"]=@((BOOL)([leaf.title isEqual:oldTitle] && [leaf.url isEqual:oldURL]));
    id<ProfileProbeBookmarkNode> edited=FindUUID(freshParent.liveChildren,gNonce);
    facts[@"freshLeafDistinctSameIdentity"]=@((BOOL)(leaf!=edited && [leaf.nodeID isEqual:edited.nodeID] && [leaf.uuid isEqual:edited.uuid]));
    facts[@"freshLeafChangedValues"]=@((BOOL)([edited.title isEqual:newTitle] && [edited.url isEqual:newURL]));
    facts[@"oldURLNotBookmarked"]=@((BOOL)(![context isBookmarked:oldURL]));
    facts[@"newURLBookmarked"]=@((BOOL)([context isBookmarked:newURL]));
    facts[@"removedUsingOldDTO"]=@((BOOL)([context removeBookmark:leaf withParent:parent]));
    facts[@"bothLeafDTOsInvalid"]=@((BOOL)(!leaf.isValid && !edited.isValid));
    facts[@"removedOldDTOOriginalValues"]=@((BOOL)([leaf.title isEqual:oldTitle] && [leaf.url isEqual:oldURL]));
    facts[@"removedFreshDTOChangedValues"]=@((BOOL)([edited.title isEqual:newTitle] && [edited.url isEqual:newURL]));
    facts[@"parentStillValid"]=@((BOOL)(parent.isValid && freshParent.isValid));
    facts[@"fixtureAbsentFromRetainedParent"]=@((BOOL)(FindUUID(parent.liveChildren,gNonce)==nil));
    facts[@"newURLNoLongerBookmarked"]=@((BOOL)(![context isBookmarked:newURL]));
    gResults[@"bookmarks"]=facts;
    gResults[@"bookmarkValues"]=@{@"uuid":gNonce,@"oldTitle":leaf.title?:NSNull.null,@"oldURL":leaf.url.absoluteString?:NSNull.null,
      @"freshTitle":edited.title?:NSNull.null,@"freshURL":edited.url.absoluteString?:NSNull.null};
    Finish(nil);
  } @catch (NSException* error) { Finish([@"bookmark exception: " stringByAppendingString:error.name]); }
}

static void RunFixture(void){
 Require(!gStarted,@"bookmark mutation chain already requested");gStarted=YES;
 gResults=[@{@"id":gLastID,@"pid":@(getpid()),@"nonce":gNonce,@"agent":gAgent,@"url":gFixtureURL,@"main":@(NSThread.isMainThread),@"recorderAliasesBefore":@(AllAliasesReady())} mutableCopy];
 @try{Require(AllAliasesReady(),@"recorder forwarding changed before fixture");id selected=nil;
 for(id contents in gContents){if(Flag(contents,@"isInitialized")&&[[Get(contents,@"url") absoluteString] isEqual:gFixtureURL]){Require(!selected,@"multiple genuine wrappers match exact fixture");selected=contents;}}
 Require(selected!=nil,@"genuine fixture create callback not observed");id browser=[gOwners objectForKey:selected];Require(browser&&Flag(browser,@"isInitialized"),@"fixture owner is not live");
 id context=Get(browser,@"context"),identifier=Get(context,@"id");Require([Get(identifier,@"directoryBasename") isEqual:@"Default"]&&!Flag(identifier,@"isIncognito"),@"initialized regular Default context required");
 gBrowser=browser;gContext=context;gOrigin=[NSURL URLWithString:gFixtureURL];BeginBookmarks();
 }@catch(NSException *error){Finish(error.reason?:error.name);}
}
static void Poll(void){
 if(!gInstalled){if(!AllAliasesReady())return;Install();}
 NSData *data=[NSData dataWithContentsOfFile:gRequest];id decoded=data?[NSJSONSerialization JSONObjectWithData:data options:0 error:NULL]:nil;if(![decoded isKindOfClass:NSDictionary.class])return;NSDictionary *q=decoded;NSString *rid=q[@"id"];
 if(![rid isKindOfClass:NSString.class]||[rid isEqual:gLastID]||![q[@"pid"] isKindOfClass:NSNumber.class]||[q[@"pid"] longLongValue]!=getpid()||![q[@"nonce"] isEqual:gNonce]||![q[@"url"] isEqual:gFixtureURL]||![q[@"operation"] isEqual:@"run"])return;
 gLastID=rid;if(gStarted)return;RunFixture();
}
__attribute__((constructor))static void StartBookmarksPublicAPI(void){@autoreleasepool{
 const char *d=getenv("ARC_SCENARIO_BOOKMARKS_DIRECTORY"),*n=getenv("ARC_SCENARIO_BOOKMARKS_NONCE"),*a=getenv("ARC_SCENARIO_BOOKMARKS_AGENT"),*u=getenv("ARC_SCENARIO_BOOKMARKS_URL");
 if(!d||!n||!a||!u||!getenv("ARC_RECORD_PATH"))return;NSString *slug=@(a);if(![@[@"trace156",@"ours156"] containsObject:slug])return;
 NSString *exe=NSProcessInfo.processInfo.arguments.firstObject,*app=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
 if(![exe.lastPathComponent isEqual:@"Arc"]||![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app",slug]]||![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]])return;
 NSString *nonce=@(n);NSUUID *uuid=[[NSUUID alloc] initWithUUIDString:nonce];NSURL *url=[NSURL URLWithString:@(u)];
 if(![@(d) isAbsolutePath]||!uuid||![uuid.UUIDString.lowercaseString isEqual:nonce]||![url.scheme isEqual:@"http"]||![url.host isEqual:@"127.0.0.1"]||url.port.integerValue<1024||url.port.integerValue>65535||url.user||url.password||url.query||url.fragment||![url.path isEqual:@"/arc-bookmarks-public-7f3a"])return;
 gDirectory=@(d);gRequest=[gDirectory stringByAppendingPathComponent:@"request.json"];gNonce=nonce;gAgent=slug;gFixtureURL=@(u);unsetenv("DYLD_INSERT_LIBRARIES");
 gContents=[NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality];gOwners=[NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory|NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];gWrapped=[NSMutableSet set];
 dispatch_async(dispatch_get_main_queue(),^{gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());dispatch_source_set_timer(gTimer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);dispatch_after(dispatch_time(DISPATCH_TIME_NOW,300*NSEC_PER_SEC),dispatch_get_main_queue(),^{dispatch_source_cancel(gTimer);});});
}}
