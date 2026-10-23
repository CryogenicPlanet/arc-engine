// Test-only public AI agent driver and native event observer for disposable apps.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSHashTable* gContents;
static NSString* gRequest;
static NSString* gLastID;
static IMP gNativeView;
static IMP gAttach;
static BOOL gInstalled;
static BOOL gObserving;
static NSUInteger gKeyDownDepth;
static NSMutableArray* gEvents;
static __weak NSView* gFirstTarget;
static NSMutableSet* gHooked;

static NSArray* CodeUnits(NSString* string) {
  NSMutableArray* units = [NSMutableArray array];
  for (NSUInteger index = 0; index < string.length; ++index)
    [units addObject:@([string characterAtIndex:index])];
  return units;
}

static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:value
      options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:nullptr];
  [data writeToFile:path options:NSDataWritingAtomic error:nullptr];
}

static id NativeView(id object, SEL selector) {
  if (NSThread.isMainThread) [gContents addObject:object];
  return ((id (*)(id, SEL))gNativeView)(object, selector);
}

static void Attach(id object, SEL selector, id pane) {
  if (NSThread.isMainThread) [gContents addObject:object];
  ((void (*)(id, SEL, id))gAttach)(object, selector, pane);
}

static BOOL IsViewClass(Class cls) {
  for (Class current = cls; current; current = class_getSuperclass(current))
    if (current == NSView.class) return YES;
  return NO;
}

// Observe each concrete owner of keyDown:, forwarding through its original IMP.
// Recursion depth suppresses duplicate captures when a view calls its super IMP.
static void InstallKeyObservers(void) {
  unsigned int count = 0;
  Class* classes = objc_copyClassList(&count);
  for (unsigned int index = 0; index < count; ++index) {
    Class cls = classes[index];
    NSString* name = NSStringFromClass(cls);
    if (!IsViewClass(cls) || [gHooked containsObject:name]) continue;
    unsigned int method_count = 0;
    Method* methods = class_copyMethodList(cls, &method_count);
    for (unsigned int item = 0; item < method_count; ++item) {
      Method method = methods[item];
      if (method_getName(method) != @selector(keyDown:)) continue;
      IMP original = method_getImplementation(method);
      IMP replacement = imp_implementationWithBlock(^(NSView* view, NSEvent* event) {
        BOOL capture = gObserving && gKeyDownDepth == 0;
        if (capture) {
          if (!gFirstTarget) gFirstTarget = view;
          [gEvents addObject:@{
            @"characters": CodeUnits(event.characters),
            @"charactersIgnoringModifiers": CodeUnits(event.charactersIgnoringModifiers),
            @"keyCode": @(event.keyCode), @"type": @(event.type),
            @"modifiers": @(event.modifierFlags), @"repeat": @(event.ARepeat),
            @"location": @[@(event.locationInWindow.x), @(event.locationInWindow.y)],
            @"timestampIsUptime": @(event.timestamp > 0 && event.timestamp <= NSProcessInfo.processInfo.systemUptime),
            @"windowMatchesTarget": @(event.windowNumber == view.window.windowNumber),
            @"targetClass": NSStringFromClass(view.class),
            @"sameTargetAsFirst": @(view == gFirstTarget),
            @"mainThread": @(NSThread.isMainThread)
          }];
        }
        ++gKeyDownDepth;
        @try { ((void (*)(id, SEL, id))original)(view, @selector(keyDown:), event); }
        @finally { --gKeyDownDepth; }
      });
      method_setImplementation(method, replacement);
      [gHooked addObject:name];
      break;
    }
    free(methods);
  }
  free(classes);
}

static void Poll(void) {
  if (!gInstalled) {
    Method method = class_getInstanceMethod(NSClassFromString(@"ArcWebContents"), @selector(nativeView));
    Method attach = class_getInstanceMethod(NSClassFromString(@"ArcWebContents"), NSSelectorFromString(@"attachToPaneHostView:"));
    if (!method || !attach) return;
    gNativeView = method_setImplementation(method, (IMP)NativeView);
    gAttach = method_setImplementation(attach, (IMP)Attach);
    gInstalled = YES;
    WriteJSON(@{@"pid": @(NSProcessInfo.processInfo.processIdentifier), @"installed": @YES},
              [gRequest stringByAppendingString:@".ready.json"]);
  }
  NSData* bytes = [NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request = bytes ? [NSJSONSerialization JSONObjectWithData:bytes options:0 error:nullptr] : nil;
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  NSString* url = request[@"url"];
  NSString* operation = request[@"operation"];
  if (![identifier isKindOfClass:NSString.class] || [identifier isEqual:gLastID] ||
      ![url isKindOfClass:NSString.class] || ![operation isKindOfClass:NSString.class]) return;
  NSURL* parsed = [NSURL URLWithString:url];
  if (![parsed.scheme isEqual:@"http"] || ![parsed.host isEqual:@"127.0.0.1"] ||
      ![parsed.path hasPrefix:@"/arc-ai-agent-probe-"]) return;
  id target = nil;
  NSUInteger matches = 0;
  for (id candidate in gContents) {
    id candidate_url = ((id (*)(id, SEL))objc_msgSend)(candidate, NSSelectorFromString(@"url"));
    if ([candidate_url isKindOfClass:NSURL.class] && [[candidate_url absoluteString] isEqual:url]) {
      target = candidate;
      ++matches;
    }
  }
  if (!matches) return;
  gLastID = [identifier copy];
  NSMutableDictionary* response = [@{@"id": identifier, @"matches": @(matches),
      @"pid": @(NSProcessInfo.processInfo.processIdentifier), @"operation": operation} mutableCopy];
  @try {
    if (matches != 1) {
      response[@"error"] = @"expected exactly one live wrapper";
    } else {
      id agent = ((id (*)(id, SEL))objc_msgSend)(target, NSSelectorFromString(@"aiAgent"));
      id repeated = ((id (*)(id, SEL))objc_msgSend)(target, NSSelectorFromString(@"aiAgent"));
      response[@"identity"] = @(agent && agent == repeated);
      InstallKeyObservers();
      gEvents = [NSMutableArray array];
      gObserving = YES;
      @try {
        if ([operation isEqual:@"text"] && [request[@"text"] isKindOfClass:NSString.class]) {
          ((void (*)(id, SEL, id))objc_msgSend)(agent, NSSelectorFromString(@"sendTextInput:"), request[@"text"]);
          response[@"ok"] = @YES;
        } else if ([operation isEqual:@"nil-text"]) {
          ((void (*)(id, SEL, id))objc_msgSend)(agent, NSSelectorFromString(@"sendTextInput:"), nil);
          response[@"ok"] = @YES;
        } else if ([operation isEqual:@"key"] && [request[@"key"] isKindOfClass:NSNumber.class]) {
          ((void (*)(id, SEL, NSInteger))objc_msgSend)(agent, NSSelectorFromString(@"sendKeyPress:"), [request[@"key"] longLongValue]);
          response[@"ok"] = @YES;
        } else if ([operation isEqual:@"inspect"]) {
          response[@"ok"] = @YES;
        } else response[@"error"] = @"invalid operation";
      } @finally { gObserving = NO; }
      response[@"events"] = [gEvents copy];
    }
  } @catch (NSException* exception) { response[@"error"] = exception.reason ?: exception.name; }
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]);
}

__attribute__((constructor)) static void StartAIAgentProbe(void) {
  @autoreleasepool {
    NSString* exe = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent]
        stringByDeletingLastPathComponent].lastPathComponent;
    const char* request = getenv("ARC_AI_AGENT_PROBE_REQUEST");
    if (!request || ![exe.lastPathComponent isEqual:@"Arc"] ||
        ![@[@"ArcModAgent-trace156.app", @"ArcModAgent-ours156.app"] containsObject:app]) return;
    unsetenv("DYLD_INSERT_LIBRARIES");
    gRequest = @(request);
    gContents = [NSHashTable weakObjectsHashTable];
    gHooked = [NSMutableSet set];
    dispatch_async(dispatch_get_main_queue(), ^{
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, DISPATCH_TIME_NOW, 25 * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 55 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
