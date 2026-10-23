// Disposable-agent public profile API probe; no native C++ ABI or real profile access.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSHashTable* gBrowsers;
static NSString* gRequest;
static NSMutableDictionary* gResults;
static NSMutableArray* gThreads;
static id gContext;
static id gBrowser;
static NSURL* gOrigin;
static NSString* gCookieName;
static NSString* gCookieValue;
static NSString* gStorageKey;
static NSString* gNonce;
static NSUInteger gCookieQueries;
static void BeginBookmarks(void);
static BOOL gInstalled, gStarted, gDone;
static id Get(id object, NSString* selector) {
  return ((id (*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));
}
static NSUInteger Integer(id object, NSString* selector) {
  return ((NSUInteger (*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));
}
static BOOL Flag(id object, NSString* selector) {
  return ((BOOL (*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(selector));
}
static void SetObject(id object, NSString* selector, id value) {
  ((void (*)(id,SEL,id))objc_msgSend)(object,NSSelectorFromString(selector),value);
}
static void SetInteger(id object, NSString* selector, NSInteger value) {
  ((void (*)(id,SEL,NSInteger))objc_msgSend)(object,NSSelectorFromString(selector),value);
}
static void Write(NSDictionary* object, NSString* path) {
  NSData* data=[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:nullptr];
  [data writeToFile:path options:NSDataWritingAtomic error:nullptr];
}
static void OnCallback(dispatch_block_t callback) {
  BOOL main=NSThread.isMainThread;
  dispatch_block_t deliver=^{ if (!gDone) { [gThreads addObject:@(main)]; callback(); } };
  if (main) deliver(); else dispatch_async(dispatch_get_main_queue(),deliver);
}
static void Finish(NSString* error) {
  if (gDone) return;
  gDone=YES;
  gResults[@"callbacksOnMain"]=[gThreads copy];
  gResults[@"cookieQueryCount"]=@(gCookieQueries);
  gResults[@"complete"]=@(error==nil);
  if (error) gResults[@"error"]=error;
  Write(gResults,[gRequest stringByAppendingString:@".response.json"]);
  dispatch_async(dispatch_get_main_queue(),^{ gContext=nil;gBrowser=nil; });
}
static void ReadCookies(BOOL removing, NSUInteger attempt) {
  if (gDone) return;
  ++gCookieQueries;
  ((void (*)(id,SEL,id,id))objc_msgSend)(gBrowser,NSSelectorFromString(@"getAllCookies:completion:"),gOrigin,^(NSArray* cookies) {
    OnCallback(^{
      id found=nil;
      for (id cookie in cookies) if ([Get(cookie,@"name") isEqual:gCookieName]) found=cookie;
      if ((found==nil)!=removing && attempt<80) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW,50*NSEC_PER_MSEC),dispatch_get_main_queue(),^{ ReadCookies(removing,attempt+1); });
        return;
      }
      if (removing) { gResults[@"cookieRemoved"]=@(found==nil); if (found) Finish(@"cookie removal not observed"); else BeginBookmarks(); return; }
      if (!found) { Finish(@"cookie write not observed"); return; }
      gResults[@"cookie"]=@{@"name":Get(found,@"name")?:NSNull.null,@"value":Get(found,@"value")?:NSNull.null,
        @"domain":Get(found,@"domain")?:NSNull.null,@"path":Get(found,@"path")?:NSNull.null,
        @"hostOnly":@(Flag(found,@"hostOnly")),@"session":@(Flag(found,@"session")),
        @"httpOnly":@(Flag(found,@"httpOnly")),@"secure":@(Flag(found,@"secure")),@"sameSite":@(Integer(found,@"sameSite"))};
      ((void (*)(id,SEL,id,id))objc_msgSend)(gBrowser,NSSelectorFromString(@"removeCookie:withName:"),gOrigin,gCookieName);
      ((void (*)(id,SEL))objc_msgSend)(gBrowser,NSSelectorFromString(@"saveCookiesToDisk"));
      ReadCookies(YES,0);
    });
  });
}
static void WriteCookie(void) {
  id cookie=[NSClassFromString(@"ArcCookie") new];
  SetObject(cookie,@"setName:",gCookieName);SetObject(cookie,@"setValue:",gCookieValue);
  SetObject(cookie,@"setDomain:",@"127.0.0.1");SetObject(cookie,@"setPath:",@"/");SetInteger(cookie,@"setSameSite:",2);
  ((void (*)(id,SEL,id,id))objc_msgSend)(gBrowser,NSSelectorFromString(@"setCookie:withCookie:"),gOrigin,cookie);
  ReadCookies(NO,0);
}
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
    facts[@"added"]=@(added);
    facts[@"retainedParentDateUnchanged"]=@([before isEqualToDate:retainedAfter]);
    facts[@"freshRootDistinctSameIdentity"]=@(root!=freshRoot && [root.nodeID isEqual:freshRoot.nodeID] && [root.uuid isEqual:freshRoot.uuid]);
    facts[@"freshParentDistinctSameIdentity"]=@(parent!=freshParent && [parent.nodeID isEqual:freshParent.nodeID] && [parent.uuid isEqual:freshParent.uuid]);
    facts[@"freshParentDateWithinAddWindow"]=@(freshDate && freshDate.timeIntervalSince1970>=windowStart && freshDate.timeIntervalSince1970<=windowEnd);
    facts[@"freshParentDateAdvanced"]=@(freshDate && freshDate.timeIntervalSince1970>before.timeIntervalSince1970);
    facts[@"leafFolderModifiedNil"]=@(leaf.dateFolderModified==nil);
    facts[@"leafAddedWithinAddWindow"]=@(leaf.dateAdded && leaf.dateAdded.timeIntervalSince1970>=windowStart && leaf.dateAdded.timeIntervalSince1970<=windowEnd);
    facts[@"oldURLInitiallyBookmarked"]=@([context isBookmarked:oldURL]);
    facts[@"renamed"]=@([context renameBookmark:leaf withTitle:newTitle]);
    facts[@"edited"]=@([context editBookmark:leaf withNewURL:newURL]);
    facts[@"retainedLeafOriginalValues"]=@([leaf.title isEqual:oldTitle] && [leaf.url isEqual:oldURL]);
    id<ProfileProbeBookmarkNode> edited=FindUUID(freshParent.liveChildren,gNonce);
    facts[@"freshLeafDistinctSameIdentity"]=@(leaf!=edited && [leaf.nodeID isEqual:edited.nodeID] && [leaf.uuid isEqual:edited.uuid]);
    facts[@"freshLeafChangedValues"]=@([edited.title isEqual:newTitle] && [edited.url isEqual:newURL]);
    facts[@"oldURLNotBookmarked"]=@(![context isBookmarked:oldURL]);
    facts[@"newURLBookmarked"]=@([context isBookmarked:newURL]);
    facts[@"removedUsingOldDTO"]=@([context removeBookmark:leaf withParent:parent]);
    facts[@"bothLeafDTOsInvalid"]=@(!leaf.isValid && !edited.isValid);
    facts[@"removedOldDTOOriginalValues"]=@([leaf.title isEqual:oldTitle] && [leaf.url isEqual:oldURL]);
    facts[@"removedFreshDTOChangedValues"]=@([edited.title isEqual:newTitle] && [edited.url isEqual:newURL]);
    facts[@"parentStillValid"]=@(parent.isValid && freshParent.isValid);
    facts[@"fixtureAbsentFromRetainedParent"]=@(FindUUID(parent.liveChildren,gNonce)==nil);
    facts[@"newURLNoLongerBookmarked"]=@(![context isBookmarked:newURL]);
    gResults[@"bookmarks"]=facts;
    gResults[@"bookmarkValues"]=@{@"uuid":gNonce,@"oldTitle":leaf.title?:NSNull.null,@"oldURL":leaf.url.absoluteString?:NSNull.null,
      @"freshTitle":edited.title?:NSNull.null,@"freshURL":edited.url.absoluteString?:NSNull.null};
    Finish(nil);
  } @catch (NSException* error) { Finish([@"bookmark exception: " stringByAppendingString:error.name]); }
}
static void Begin(id browser,id context,NSURL* origin,NSString* nonce) {
  gStarted=YES;gBrowser=browser;gContext=context;gOrigin=origin;gNonce=[nonce copy];
  gCookieName=[@"profile_probe_" stringByAppendingString:nonce];
  gCookieValue=[@"v_" stringByAppendingString:nonce];gStorageKey=gCookieName;
  gResults=[@{@"pid":@(NSProcessInfo.processInfo.processIdentifier),@"main":@(NSThread.isMainThread),@"nonce":nonce} mutableCopy];
  gThreads=[NSMutableArray array];
  gResults[@"autoPipGloballyDisabled"]=@([[NSUserDefaults standardUserDefaults] boolForKey:@"autoPipGloballyDisabled"]);
  id info=((id (*)(id,SEL,id,NSUInteger))objc_msgSend)(context,
    NSSelectorFromString(@"getContentSettingForOrigin:withContentSettingType:"),origin,29);
  gResults[@"type29"]=info ? @(Integer(info,@"siteSetting")) : NSNull.null;
  gResults[@"settingOrigin"]=info ? Get(info,@"originUrl")?:NSNull.null : NSNull.null;
  ((void (*)(id,SEL,id,id,id))objc_msgSend)(context,NSSelectorFromString(@"getLocalStorage:key:completion:"),origin,gStorageKey,^(NSString* initial) {
    OnCallback(^{
      gResults[@"storageInitial"]=initial?:NSNull.null;
      ((void (*)(id,SEL,id,id,id,id))objc_msgSend)(gContext,NSSelectorFromString(@"setLocalStorage:key:value:completion:"),gOrigin,gStorageKey,@"Latin＋ε🪐",^{
        OnCallback(^{
          ((void (*)(id,SEL,id,id,id))objc_msgSend)(gContext,NSSelectorFromString(@"getLocalStorage:key:completion:"),gOrigin,gStorageKey,^(NSString* value) {
            OnCallback(^{ gResults[@"storageReadback"]=value?:NSNull.null; WriteCookie(); });
          });
        });
      });
    });
  });
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW,25*NSEC_PER_SEC),dispatch_get_main_queue(),^{ Finish(@"profile API completion timeout"); });
}
static void Poll(void) {
  if (!gInstalled) {
    Class cls=NSClassFromString(@"ArcBrowser"); SEL sel=NSSelectorFromString(@"setDelegate:");
    Method method=class_getInstanceMethod(cls,sel);if (!method) return;
    IMP original=method_getImplementation(method);
    IMP hook=imp_implementationWithBlock(^(id browser,id delegate) {
      [gBrowsers addObject:browser];((void (*)(id,SEL,id))original)(browser,sel,delegate);
    });
    if (!class_addMethod(cls,sel,hook,method_getTypeEncoding(method))) method_setImplementation(class_getInstanceMethod(cls,sel),hook);
    gInstalled=YES;Write(@{@"pid":@(NSProcessInfo.processInfo.processIdentifier),@"installed":@YES},[gRequest stringByAppendingString:@".ready.json"]);
  }
  if (gStarted) return;
  NSData* data=[NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request=data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:nullptr] : nil;
  if (![request isKindOfClass:NSDictionary.class] || ![request[@"url"] isKindOfClass:NSString.class]) return;
  NSURL* origin=[NSURL URLWithString:request[@"url"]];
  if (![origin.scheme isEqual:@"http"] || ![origin.host isEqual:@"127.0.0.1"] || ![origin.path isEqual:@"/arc-profile-public-7f3a"] ||
      origin.user.length || origin.password.length || origin.fragment.length || origin.query.length || !origin.port ||
      ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)]) return;
  id browser=nil,context=nil;
  for (id candidate in gBrowsers) {
    if (!Flag(candidate,@"isInitialized")) continue;
    id current=Get(candidate,@"context"),identifier=Get(current,@"id");
    if (![Get(identifier,@"directoryBasename") isEqual:@"Default"] || Flag(identifier,@"isIncognito")) continue;
    browser=candidate;context=current;
  }
  NSString* nonce=request[@"nonce"];
  NSUUID* parsed=[nonce isKindOfClass:NSString.class] ? [[NSUUID alloc] initWithUUIDString:nonce] : nil;
  if (!parsed || ![nonce isEqual:parsed.UUIDString.lowercaseString]) return;
  if (browser && context) Begin(browser,context,origin,nonce);
}
__attribute__((constructor)) static void StartProfileProbe(void) {
  @autoreleasepool {
    NSString* exe=NSProcessInfo.processInfo.arguments.firstObject;
    NSString* bundle=[[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    const char* request=getenv("ARC_SCENARIO_PROFILE_PROBE_REQUEST");
    if (!request || ![exe.lastPathComponent isEqual:@"Arc"] ||
        ![@[@"ArcModAgent-trace156.app",@"ArcModAgent-ours156.app"] containsObject:bundle.lastPathComponent]) return;
    unsetenv("DYLD_INSERT_LIBRARIES");gRequest=@(request);gBrowsers=[NSHashTable weakObjectsHashTable];
    dispatch_async(dispatch_get_main_queue(),^{
      dispatch_source_t timer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());
      dispatch_source_set_timer(timer,DISPATCH_TIME_NOW,25*NSEC_PER_MSEC,5*NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer,^{ @autoreleasepool { Poll(); } });dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW,120*NSEC_PER_SEC),dispatch_get_main_queue(),^{ dispatch_source_cancel(timer); });
    });
  }
}
