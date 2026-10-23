// Narrow CSS + annotated extraction driver. Standard recorder aliases only;
// no private C++ ABI, feature toggles, input, window actions or timestamp getter.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <stdlib.h>
#include <unistd.h>

static NSString *gDirectory, *gNonce, *gAgent, *gInitialURL, *gCurrentURL, *gLastID;
static NSPointerArray *gContents;
static NSHashTable *gSeen;
static NSMapTable *gOwners;
static id gSelected, gBrowser;
static BOOL gInstalled, gExtractPending, gNavigated, gRecorderForwarding;
static NSUInteger gExtractionCalls, gCompletionCalls, gInsertCalls, gRemoveCalls;
static NSMutableArray *gCompletionCounts;
static dispatch_source_t gTimer;

static void Require(BOOL pass, NSString *reason) {
  if (!pass) @throw [NSException exceptionWithName:@"RendererPublicAPI" reason:reason userInfo:nil];
}
static id Get(id object, NSString *name) {
  return ((id (*)(id, SEL))objc_msgSend)(object, NSSelectorFromString(name));
}
static BOOL Flag(id object, NSString *name) {
  return ((BOOL (*)(id, SEL))objc_msgSend)(object, NSSelectorFromString(name));
}
static void WriteJSON(id object, NSString *path) {
  NSError *error = nil;
  NSData *data = [NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&error];
  if (!data || ![data writeToFile:path options:NSDataWritingAtomic error:&error]) NSLog(@"RendererPublicAPI output failed: %@", error);
}
static SEL Alias(NSString *name) { return NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]); }
static BOOL AliasReady(Class cls, NSString *name) {
  Method method = class_getInstanceMethod(cls, NSSelectorFromString(name));
  Method alias = class_getInstanceMethod(cls, Alias(name));
  return method && alias && method_getImplementation(method) == (IMP)_objc_msgForward && method_getImplementation(alias) != (IMP)_objc_msgForward;
}
static BOOL AllAliasesReady(void) {
  NSDictionary *methods = @{
    @"_TtC3ADK15ChromiumBrowser":@[@"browser:didCreateNewWebContents:isActive:"],
    @"ArcWebContents":@[@"url", @"isInitialized", @"insertStylesheet:stylesheet:", @"removeStylesheet:"],
    @"ArcBrowser":@[@"context"], @"ArcBrowserContext":@[@"pageContentExtraction"],
    @"ArcBrowserContextPageContentExtraction":@[@"getExtractedContentForWebContents:completion:"]
  };
  for (NSString *name in methods) {
    Class cls = NSClassFromString(name); if (!cls) return NO;
    for (NSString *selector in methods[name]) if (!AliasReady(cls, selector)) return NO;
  }
  return YES;
}
static void Install(void) {
  Class cls = NSClassFromString(@"_TtC3ADK15ChromiumBrowser");
  SEL alias = Alias(@"browser:didCreateNewWebContents:isActive:");
  Method method = class_getInstanceMethod(cls, alias);
  IMP original = method_getImplementation(method);
  IMP replacement = imp_implementationWithBlock(^(id self, id browser, id contents, BOOL active) {
    if (browser && contents) {
      if (![gSeen containsObject:contents]) { [gSeen addObject:contents]; [gContents addPointer:(__bridge void *)contents]; }
      [gOwners setObject:browser forKey:contents];
    }
    ((void (*)(id, SEL, id, id, BOOL))original)(self, alias, browser, contents, active);
  });
  if (!class_addMethod(cls, alias, replacement, method_getTypeEncoding(method))) method_setImplementation(class_getInstanceMethod(cls, alias), replacement);
  gInstalled = YES; gRecorderForwarding = YES;
  WriteJSON(@{@"pid":@(getpid()), @"nonce":gNonce, @"agent":gAgent,
    @"installed":@YES, @"recorderAliases":@YES}, [gDirectory stringByAppendingPathComponent:@"ready.json"]);
}
// Status polling is cached only: no engine selectors, tree traversal or lookup.
static NSDictionary *State(void) {
  return @{@"selected":@((BOOL)(gSelected != nil)), @"insertCalls":@(gInsertCalls), @"removeCalls":@(gRemoveCalls),
    @"extractionCalls":@(gExtractionCalls), @"completionCalls":@(gCompletionCalls),
    @"completionCounts":[gCompletionCounts copy], @"pending":@(gExtractPending)};
}
typedef struct { NSUInteger nodes; NSUInteger controls; NSUInteger options; } Counts;
static id SnapshotNode(id node, NSUInteger depth, Counts *counts) {
  Require(node != nil && depth <= 32 && counts->nodes < 256, @"extraction exceeded bounded node/depth limit");
  ++counts->nodes;
  NSMutableDictionary *result = [NSMutableDictionary dictionary];
  for (NSString *name in @[@"type", @"textContent", @"hasEmphasis", @"textSize", @"textColor", @"linkURL", @"imageAlt", @"iframeOrigin", @"formName", @"formActionURL", @"tableName"])
    result[name] = [node valueForKey:name] ?: NSNull.null;
  id rawChildren = [node valueForKey:@"children"];
  Require([rawChildren isKindOfClass:NSArray.class], @"public node children is not an array");
  NSMutableArray *children = [NSMutableArray array];
  for (id child in rawChildren) [children addObject:SnapshotNode(child, depth + 1, counts)];
  result[@"children"] = children;
  id control = [node valueForKey:@"formControlInfo"];
  result[@"formControlInfo"] = NSNull.null;
  if (control) {
    ++counts->controls;
    NSMutableDictionary *fields = [NSMutableDictionary dictionary];
    for (NSString *name in @[@"controlType", @"fieldName", @"fieldValue", @"placeholder", @"isChecked", @"isRequired", @"isRedacted"])
      fields[name] = [control valueForKey:name] ?: NSNull.null;
    id rawOptions = [control valueForKey:@"selectOptions"];
    Require([rawOptions isKindOfClass:NSArray.class] && [rawOptions count] <= 32, @"invalid or unbounded public select options");
    NSMutableArray *options = [NSMutableArray array];
    for (id option in rawOptions) {
      ++counts->options;
      NSMutableDictionary *fields = [NSMutableDictionary dictionary];
      for (NSString *name in @[@"value", @"text", @"isSelected", @"isDisabled"]) fields[name] = [option valueForKey:name] ?: NSNull.null;
      [options addObject:fields];
    }
    fields[@"selectOptions"] = options; result[@"formControlInfo"] = fields;
  }
  return result;
}
static void Select(void) {
  Require(!gSelected, @"fixture already selected");
  id selected = nil; NSUInteger matches = 0;
  NSArray *known = gContents.allObjects;
  Require(known.count <= 16, @"unexpected number of native wrappers");
  for (id contents in known) {
    if (!Flag(contents, @"isInitialized")) continue;
    id url = Get(contents, @"url");
    if ([url isKindOfClass:NSURL.class] && [[url absoluteString] isEqual:gInitialURL]) { selected = contents; ++matches; }
  }
  Require(matches == 1, @"expected one observed exact fixture wrapper");
  id browser = [gOwners objectForKey:selected]; Require(browser != nil, @"fixture has no observed native browser owner");
  gSelected = selected; gBrowser = browser; gCurrentURL = gInitialURL;
}
static void Extract(NSMutableDictionary *response) {
  Require(!gExtractPending && gExtractionCalls < 2, @"only two sequential extraction requests supported");
  id context = Get(gBrowser, @"context"); Require(context != nil, @"fixture browser has no public context");
  id service = Get(context, @"pageContentExtraction"); Require(service != nil, @"public context has no extraction service");
  NSUInteger index = gExtractionCalls++; [gCompletionCounts addObject:@0]; gExtractPending = YES;
  __block BOOL returned = NO;
  ((void (*)(id, SEL, id, id))objc_msgSend)(service, NSSelectorFromString(@"getExtractedContentForWebContents:completion:"), gSelected, ^(id content) {
    BOOL main = NSThread.isMainThread;
    BOOL synchronous = main && !returned;
    void (^finish)(void) = ^{
      ++gCompletionCalls; gCompletionCounts[index] = @([gCompletionCounts[index] unsignedIntegerValue] + 1);
      gExtractPending = NO;
      response[@"mainThread"] = @(main); response[@"synchronous"] = @(synchronous);
      response[@"completionCount"] = gCompletionCounts[index];
      @try {
        Require(main, @"extraction callback was off main thread");
        Require(content != nil, @"public extraction returned nil (loopback support remains unverified)");
        Require([gCompletionCounts[index] unsignedIntegerValue] == 1, @"duplicate extraction completion");
        Counts counts = {0, 0, 0};
        response[@"result"] = @{@"url":[content valueForKey:@"url"] ?: NSNull.null,
          @"title":[content valueForKey:@"title"] ?: NSNull.null,
          @"rootNode":SnapshotNode([content valueForKey:@"rootNode"], 0, &counts)};
        response[@"counts"] = @{@"nodes":@(counts.nodes), @"controls":@(counts.controls), @"options":@(counts.options)};
        response[@"ok"] = @YES;
      } @catch (NSException *exception) { response[@"ok"] = @NO; response[@"error"] = exception.reason ?: exception.name; }
      gRecorderForwarding = AllAliasesReady(); response[@"recorderForwarding"] = @(gRecorderForwarding);
      response[@"state"] = State();
      WriteJSON(response, [gDirectory stringByAppendingPathComponent:@"response.json"]);
    };
    if (main) finish(); else dispatch_async(dispatch_get_main_queue(), finish);
  });
  returned = YES;
}
static void Poll(void) {
  if (!gInstalled) { if (!AllAliasesReady()) return; Install(); }
  NSData *data = [NSData dataWithContentsOfFile:[gDirectory stringByAppendingPathComponent:@"request.json"]];
  id decoded = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![decoded isKindOfClass:NSDictionary.class]) return;
  NSDictionary *request = decoded;
  NSString *rid = request[@"id"], *operation = request[@"operation"], *url = request[@"url"];
  NSString *secondURL = [[[[NSURL URLWithString:gInitialURL] URLByDeletingLastPathComponent] URLByAppendingPathComponent:@"b"] absoluteString];
  if (![rid isKindOfClass:NSString.class] || [rid isEqual:gLastID] || ![operation isKindOfClass:NSString.class] ||
    ![request[@"nonce"] isEqual:gNonce] || ![request[@"pid"] isKindOfClass:NSNumber.class] || [request[@"pid"] longLongValue] != getpid() ||
    ![url isKindOfClass:NSString.class] || (![url isEqual:gInitialURL] && ![url isEqual:secondURL])) return;
  gLastID = rid;
  NSMutableDictionary *response = [@{@"id":rid, @"operation":operation, @"pid":@(getpid()),
    @"nonce":gNonce, @"main":@(NSThread.isMainThread)} mutableCopy];
  @try {
    if (![operation isEqual:@"status"]) Require(!gExtractPending, @"cannot overlap a pending extraction");
    if ([operation isEqual:@"select"]) { Require([url isEqual:gInitialURL], @"initial selection URL mismatch"); Select(); }
    else if ([operation isEqual:@"bind-after-navigation"]) {
      Require(gSelected && !gNavigated && [url isEqual:secondURL], @"invalid document navigation stage");
      Require([[Get(gSelected, @"url") absoluteString] isEqual:secondURL], @"selected native wrapper did not navigate to exact fixture b");
      gNavigated = YES; gCurrentURL = secondURL;
    } else if ([operation isEqual:@"status"]) { /* Pure cached state only. */ }
    else {
      Require(gSelected && [url isEqual:gCurrentURL], @"operation does not own the currently bound fixture URL");
      if ([operation isEqual:@"extract"]) { Extract(response); return; }
      if ([operation isEqual:@"insert"]) {
        Require([request[@"identifier"] isKindOfClass:NSString.class] && [request[@"stylesheet"] isKindOfClass:NSString.class] && gInsertCalls < 5, @"invalid or excess stylesheet insertion");
        ++gInsertCalls;
        ((void (*)(id, SEL, id, id))objc_msgSend)(gSelected, NSSelectorFromString(@"insertStylesheet:stylesheet:"), request[@"identifier"], request[@"stylesheet"]);
      } else if ([operation isEqual:@"remove"]) {
        Require([request[@"identifier"] isKindOfClass:NSString.class] && gRemoveCalls < 5, @"invalid or excess stylesheet removal");
        ++gRemoveCalls;
        ((void (*)(id, SEL, id))objc_msgSend)(gSelected, NSSelectorFromString(@"removeStylesheet:"), request[@"identifier"]);
      } else Require(NO, @"unsupported operation");
    }
    if (![operation isEqual:@"status"]) gRecorderForwarding = AllAliasesReady();
    response[@"ok"] = @YES; response[@"recorderForwarding"] = @(gRecorderForwarding);
  } @catch (NSException *exception) { response[@"error"] = exception.reason ?: exception.name; }
  response[@"state"] = State(); WriteJSON(response, [gDirectory stringByAppendingPathComponent:@"response.json"]);
}
__attribute__((constructor)) static void StartRendererPublicAPI(void) {
  @autoreleasepool {
    const char *directory = getenv("ARC_SCENARIO_RENDERER_DIRECTORY"), *nonce = getenv("ARC_SCENARIO_RENDERER_NONCE");
    const char *agent = getenv("ARC_SCENARIO_RENDERER_AGENT"), *fixture = getenv("ARC_SCENARIO_RENDERER_URL");
    if (!directory || !nonce || !agent || !fixture || !getenv("ARC_RECORD_PATH")) return;
    NSString *slug = @(agent), *exe = NSProcessInfo.processInfo.arguments.firstObject;
    NSString *app = [[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    if (![@[@"trace156", @"ours156"] containsObject:slug] || ![exe.lastPathComponent isEqual:@"Arc"] ||
      ![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app", slug]] ||
      ![NSBundle.mainBundle.bundleIdentifier isEqual:[NSString stringWithFormat:@"company.thebrowser.Browser.mod.agent.%@", slug]]) return;
    NSURL *url = [NSURL URLWithString:@(fixture)];
    if (![@(directory) isAbsolutePath] || ![@(nonce) hasPrefix:@"renderer-public-api-"] ||
      ![url.scheme isEqual:@"http"] || ![url.host isEqual:@"127.0.0.1"] || url.port.integerValue < 1024 ||
      url.user || url.password || url.query || url.fragment || ![url.path isEqual:@"/arc-renderer-probe-7f3a/a"]) return;
    gDirectory = @(directory); gNonce = @(nonce); gAgent = slug; gInitialURL = @(fixture); gCurrentURL = gInitialURL;
    unsetenv("DYLD_INSERT_LIBRARIES");
    gContents = [NSPointerArray weakObjectsPointerArray];
    gSeen = [NSHashTable hashTableWithOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality];
    gOwners = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];
    gCompletionCounts = [NSMutableArray array];
    dispatch_async(dispatch_get_main_queue(), ^{
      gTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(gTimer, DISPATCH_TIME_NOW, 25 * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(gTimer, ^{ @autoreleasepool { Poll(); } }); dispatch_resume(gTimer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(gTimer); });
    });
  }
}
