// Test-only public ObjC API driver. No AX, window actions, C++ ABI or global input.
// The standard boundary recorder must already own every canonical method below.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <stdlib.h>
#include <unistd.h>

static NSString *gDirectory, *gNonce, *gAgent, *gFixtureURL, *gLastID;
static NSHashTable *gSeen;
static NSPointerArray *gContents;
static NSMapTable *gOwners;
static NSMutableArray *gEvents;
static NSMutableDictionary *gSnapshots;
static NSMutableSet *gWrappedAliases;
static id gSelected, gSentinel, gBrowser, gSourceID, gClosedID, gRestored, gCallbackID;
static NSUInteger gClosedCount, gRestoreCount, gActionDepth;
static BOOL gInstalled, gCloseRequested, gRestoreRequested, gClosedSnapshot, gRestoredSnapshot;
static dispatch_source_t gTimer;

static id Get(id object, NSString *name) {
  return ((id (*)(id, SEL))objc_msgSend)(object, NSSelectorFromString(name));
}
static BOOL Flag(id object, NSString *name) {
  return ((BOOL (*)(id, SEL))objc_msgSend)(object, NSSelectorFromString(name));
}
static BOOL CanRestore(void) {
  return ((BOOL (*)(id, SEL, id))objc_msgSend)(gBrowser,
    NSSelectorFromString(@"canRestoreWebContentsWithSessionId:"), gClosedID);
}
static void Require(BOOL condition, NSString *reason) {
  if (!condition) @throw [NSException exceptionWithName:@"SessionsPublicAPI" reason:reason userInfo:nil];
}
static void WriteJSON(id object, NSString *path) {
  NSError *error = nil;
  NSData *data = [NSJSONSerialization dataWithJSONObject:object
    options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&error];
  if (!data || ![data writeToFile:path options:NSDataWritingAtomic error:&error])
    NSLog(@"SessionsPublicAPI output failed: %@", error);
}
static SEL Alias(NSString *name) { return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]); }
static BOOL AliasReady(Class cls, NSString *name) {
  if (!cls) return NO;
  Method canonical = class_getInstanceMethod(cls, NSSelectorFromString(name));
  Method alias = class_getInstanceMethod(cls, Alias(name));
  return canonical && alias && method_getImplementation(canonical) == (IMP)_objc_msgForward &&
    method_getImplementation(alias) != (IMP)_objc_msgForward;
}
static void WrapAlias(Class cls, NSString *name, id (^make)(IMP, SEL)) {
  Require(AliasReady(cls, name), @"recorder forwarding/alias disappeared");
  SEL alias = Alias(name);
  Class owner = Nil;
  // Several genuine delegate types may inherit the same recorder alias. Wrap
  // that implementation once; stacking inherited wrappers would double callbacks.
  for (Class current = cls; current && !owner; current = class_getSuperclass(current)) {
    unsigned count = 0;
    Method *methods = class_copyMethodList(current, &count);
    for (unsigned i = 0; i < count; ++i) if (method_getName(methods[i]) == alias) { owner = current; break; }
    free(methods);
  }
  Require(owner != Nil, @"recorder alias has no owning class");
  NSString *key = [NSString stringWithFormat:@"%@/%@", NSStringFromClass(owner), name];
  if ([gWrappedAliases containsObject:key]) return;
  Method method = class_getInstanceMethod(owner, alias);
  IMP replacement = imp_implementationWithBlock(make(method_getImplementation(method), alias));
  method_setImplementation(method, replacement);
  [gWrappedAliases addObject:key];
}
static void Register(id browser, id contents) {
  if (!browser || !contents) return;
  if (![gSeen containsObject:contents]) {
    [gSeen addObject:contents]; [gContents addPointer:(__bridge void *)contents];
  }
  [gOwners setObject:browser forKey:contents];
}
static void Event(NSString *type) {
  [gEvents addObject:@{@"type":type, @"main":@(NSThread.isMainThread),
    @"insideAction":@((BOOL)(gActionDepth != 0))}];
}
static NSArray *CallbackHooks(void) {
  return @[
    @[@"_TtC3ADK15ChromiumBrowser", @"browser:didCreateNewWebContents:isActive:"],
    @[@"_TtC3ADK15ChromiumBrowser", @"browser:didRestoreWebContents:withSessionId:"],
    @[@"_TtC3ADK18WebContentChromium", @"webContents:didCloseWithSessionID:"],
    @[@"_TtC3ADK28TablessWebContentsUIDelegate", @"webContents:didCloseWithSessionID:"]
  ];
}
static BOOL AllAliasesReady(void) {
  NSDictionary *engine = @{
    @"ArcWebContents":@[@"close", @"isInitialized", @"url", @"sessionId"],
    @"ArcBrowser":@[@"isInitialized", @"canRestoreWebContentsWithSessionId:", @"restoreWebContentsWithSessionId:"]
  };
  for (NSString *name in engine) for (NSString *selector in engine[name])
    if (!AliasReady(NSClassFromString(name), selector)) return NO;
  for (NSArray *hook in CallbackHooks()) if (!AliasReady(NSClassFromString(hook[0]), hook[1])) return NO;
  return YES;
}
static void Install(void) {
  Class browserDelegate = NSClassFromString(@"_TtC3ADK15ChromiumBrowser");
  WrapAlias(browserDelegate, @"browser:didCreateNewWebContents:isActive:", ^id(IMP original, SEL alias) {
    return ^(id self, id browser, id contents, BOOL active) {
      Register(browser, contents);
      ((void (*)(id, SEL, id, id, BOOL))original)(self, alias, browser, contents, active);
    };
  });
  WrapAlias(browserDelegate, @"browser:didRestoreWebContents:withSessionId:", ^id(IMP original, SEL alias) {
    return ^(id self, id browser, id contents, id identifier) {
      Register(browser, contents);
      if (gRestoreRequested && browser == gBrowser) {
        gRestored = contents; gCallbackID = identifier; ++gRestoreCount; Event(@"restored");
      }
      ((void (*)(id, SEL, id, id, id))original)(self, alias, browser, contents, identifier);
    };
  });
  for (NSString *name in @[@"_TtC3ADK18WebContentChromium", @"_TtC3ADK28TablessWebContentsUIDelegate"]) {
    WrapAlias(NSClassFromString(name), @"webContents:didCloseWithSessionID:", ^id(IMP original, SEL alias) {
      return ^(id self, id contents, id identifier) {
        if (gCloseRequested && contents == gSelected) {
          gClosedID = identifier; ++gClosedCount; Event(@"closed");
        }
        ((void (*)(id, SEL, id, id))original)(self, alias, contents, identifier);
      };
    });
  }
  gInstalled = YES;
  WriteJSON(@{@"pid":@(getpid()), @"nonce":gNonce, @"agent":gAgent,
    @"installed":@YES, @"recorderAliases":@YES, @"callbackHooks":CallbackHooks()},
    [gDirectory stringByAppendingPathComponent:@"ready.json"]);
}
// Pure cached serialization. No engine calls here or in status polling.
static NSDictionary *State(void) {
  NSMutableDictionary *state = [gSnapshots mutableCopy];
  [state addEntriesFromDictionary:@{@"closedCount":@(gClosedCount), @"restoreCount":@(gRestoreCount),
    @"events":[gEvents copy], @"closeRequested":@(gCloseRequested), @"restoreRequested":@(gRestoreRequested)}];
  return state;
}
static void SelectFixture(void) {
  Require(!gSelected, @"fixture was already selected");
  NSArray *known = gContents.allObjects;
  Require(known.count == 2, @"expected exactly two observed fixture wrappers");
  NSString *sentinelURL = [[[[NSURL URLWithString:gFixtureURL] URLByDeletingLastPathComponent]
    URLByAppendingPathComponent:@"sentinel"] absoluteString];
  id selected = nil, sentinel = nil;
  // Registration order is stable: the TS driver waits for sentinel before fixture.
  for (id contents in known) {
    Require(Flag(contents, @"isInitialized"), @"observed fixture is uninitialized");
    id url = Get(contents, @"url");
    Require([url isKindOfClass:NSURL.class], @"fixture has no public URL");
    if ([[url absoluteString] isEqual:gFixtureURL]) { Require(!selected, @"ambiguous fixture URL"); selected = contents; }
    else if ([[url absoluteString] isEqual:sentinelURL]) { Require(!sentinel, @"ambiguous sentinel URL"); sentinel = contents; }
    else Require(NO, @"unexpected observed tab; refusing close");
  }
  Require(selected && sentinel, @"exact fixture and sentinel URLs not observed");
  id browser = [gOwners objectForKey:selected];
  Require(browser && browser == [gOwners objectForKey:sentinel] && Flag(browser, @"isInitialized"), @"fixture/sentinel browser ownership mismatch");
  gSelected = selected; gSentinel = sentinel; gBrowser = browser; gSourceID = Get(selected, @"sessionId");
  Require(gSourceID && [gSourceID isKindOfClass:NSClassFromString(@"ArcSessionID")], @"missing opaque source session ID");
  [gSnapshots addEntriesFromDictionary:@{@"selected":@YES, @"knownFixtures":@2,
    @"sourceIDPresent":@YES, @"sentinelSharesBrowser":@YES,
    @"recorderForwardingBefore":@(AllAliasesReady())}];
}
static void ClosedSnapshot(void) {
  Require(gCloseRequested && gClosedCount == 1 && gClosedID && !gClosedSnapshot, @"closed snapshot requires exactly one callback and runs once");
  BOOL oldInitialized = Flag(gSelected, @"isInitialized");
  BOOL browserInitialized = Flag(gBrowser, @"isInitialized");
  BOOL sentinelInitialized = Flag(gSentinel, @"isInitialized");
  Require(browserInitialized, @"fixture browser disappeared");
  [gSnapshots addEntriesFromDictionary:@{@"oldInitializedAfterClose":@(oldInitialized),
    @"browserInitializedAfterClose":@(browserInitialized), @"sentinelInitializedAfterClose":@(sentinelInitialized),
    @"restorationIDPresent":@YES, @"restorationMatchesSourceID":@([gClosedID isEqual:gSourceID]),
    @"canRestoreAfterClose":@(CanRestore())}];
  gClosedSnapshot = YES;
}
static void RestoredSnapshot(void) {
  Require(gRestoreRequested && gRestoreCount == 1 && gRestored && !gRestoredSnapshot, @"restored snapshot requires exactly one callback and runs once");
  BOOL oldInitialized = Flag(gSelected, @"isInitialized");
  BOOL browserInitialized = Flag(gBrowser, @"isInitialized");
  BOOL sentinelInitialized = Flag(gSentinel, @"isInitialized");
  BOOL initialized = Flag(gRestored, @"isInitialized");
  Require(browserInitialized && initialized, @"restored wrapper/browser is uninitialized");
  id url = Get(gRestored, @"url");
  [gSnapshots addEntriesFromDictionary:@{@"oldInitializedAfterRestore":@(oldInitialized),
    @"browserInitializedAfterRestore":@(browserInitialized), @"sentinelInitializedAfterRestore":@(sentinelInitialized),
    @"restoredInitialized":@(initialized), @"restoredIsNewWrapper":@((BOOL)(gRestored != gSelected)),
    @"restoredURL":[url isKindOfClass:NSURL.class] ? [url absoluteString] : NSNull.null,
    @"callbackIDPresent":@((BOOL)(gCallbackID != nil)), @"callbackUsesSourceID":@((BOOL)(gCallbackID && [gCallbackID isEqual:gSourceID])),
    @"callbackUsesRestorationID":@((BOOL)(gCallbackID && [gCallbackID isEqual:gClosedID])),
    @"canRestoreAfterRestore":@(CanRestore()), @"recorderForwardingAfter":@(AllAliasesReady())}];
  gRestoredSnapshot = YES;
}
static void Poll(void) {
  if (!gInstalled) { if (!AllAliasesReady()) return; Install(); }
  NSData *data = [NSData dataWithContentsOfFile:[gDirectory stringByAppendingPathComponent:@"request.json"]];
  id decoded = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![decoded isKindOfClass:NSDictionary.class]) return;
  NSDictionary *request = decoded;
  NSString *rid = request[@"id"], *operation = request[@"operation"];
  if (![rid isKindOfClass:NSString.class] || [rid isEqual:gLastID] ||
    ![operation isKindOfClass:NSString.class] || ![request[@"nonce"] isEqual:gNonce] ||
    ![request[@"pid"] isKindOfClass:NSNumber.class] || [request[@"pid"] longLongValue] != getpid() ||
    ![request[@"url"] isEqual:gFixtureURL]) return;
  gLastID = rid;
  NSMutableDictionary *response = [@{@"id":rid, @"operation":operation, @"pid":@(getpid()),
    @"nonce":gNonce, @"main":@(NSThread.isMainThread)} mutableCopy];
  @try {
    if ([operation isEqual:@"select"]) SelectFixture();
    else if ([operation isEqual:@"close"]) {
      Require(gSelected && !gCloseRequested && !gRestoreRequested, @"close requires a selected untouched fixture");
      gCloseRequested = YES; ++gActionDepth;
      @try { ((void (*)(id, SEL))objc_msgSend)(gSelected, NSSelectorFromString(@"close")); }
      @finally { --gActionDepth; }
    } else if ([operation isEqual:@"closed-snapshot"]) ClosedSnapshot();
    else if ([operation isEqual:@"restore"]) {
      Require(gClosedSnapshot && !gRestoreRequested && gClosedID && [gSnapshots[@"canRestoreAfterClose"] boolValue], @"restore requires captured restorable fixture");
      gSnapshots[@"canRestoreBeforeRestore"] = @(CanRestore());
      gRestoreRequested = YES; ++gActionDepth;
      @try { ((void (*)(id, SEL, id))objc_msgSend)(gBrowser, NSSelectorFromString(@"restoreWebContentsWithSessionId:"), gClosedID); }
      @finally { --gActionDepth; }
      gSnapshots[@"restoreCallbackInline"] = @((BOOL)(gRestoreCount != 0));
    } else if ([operation isEqual:@"restored-snapshot"]) RestoredSnapshot();
    else Require([operation isEqual:@"status"], @"unsupported operation");
    response[@"ok"] = @YES;
  } @catch (NSException *exception) { response[@"error"] = exception.reason ?: exception.name; }
  response[@"state"] = State();
  WriteJSON(response, [gDirectory stringByAppendingPathComponent:@"response.json"]);
}
__attribute__((constructor)) static void StartSessionsPublicAPI(void) {
  @autoreleasepool {
    const char *directory = getenv("ARC_SCENARIO_SESSIONS_DIRECTORY");
    const char *nonce = getenv("ARC_SCENARIO_SESSIONS_NONCE");
    const char *agent = getenv("ARC_SCENARIO_SESSIONS_AGENT");
    const char *fixture = getenv("ARC_SCENARIO_SESSIONS_URL");
    if (!directory || !nonce || !agent || !fixture || !getenv("ARC_RECORD_PATH")) return;
    NSString *slug = @(agent), *exe = NSProcessInfo.processInfo.arguments.firstObject;
    if (![@[@"trace156", @"ours156"] containsObject:slug]) return;
    NSString *app = [[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    if (![exe.lastPathComponent isEqual:@"Arc"] ||
      ![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app", slug]] ||
      ![NSBundle.mainBundle.bundleIdentifier isEqual:[NSString stringWithFormat:@"company.thebrowser.Browser.mod.agent.%@", slug]]) return;
    NSURL *url = [NSURL URLWithString:@(fixture)];
    if (![@(directory) isAbsolutePath] || ![@(nonce) hasPrefix:@"sessions-public-api-"] ||
      ![url.scheme isEqual:@"http"] || ![url.host isEqual:@"127.0.0.1"] || url.port.integerValue < 1024 ||
      url.user || url.password || url.query || url.fragment || ![url.path isEqual:@"/arc-sessions-probe-7f3a/B"]) return;
    gDirectory = @(directory); gNonce = @(nonce); gAgent = slug; gFixtureURL = @(fixture);
    unsetenv("DYLD_INSERT_LIBRARIES");
    gSeen = [NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality];
    gContents = [NSPointerArray weakObjectsPointerArray];
    gOwners = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality
      valueOptions:NSPointerFunctionsWeakMemory];
    gEvents = [NSMutableArray array]; gSnapshots = [NSMutableDictionary dictionary];
    gWrappedAliases = [NSMutableSet set];
    dispatch_async(dispatch_get_main_queue(), ^{
      gTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(gTimer, DISPATCH_TIME_NOW, 25 * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(gTimer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(gTimer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(gTimer); });
    });
  }
}
