// Test-only injection: weakly observe ArcWebContents, then invoke its public CSS API.
// Restricted to the two disposable parity apps. Compile with probe.sh build.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSHashTable* gContents;
static NSHashTable* gBrowsers;
static NSString* gRequest;
static NSString* gLastID;
static IMP gNativeView;
static IMP gInsert;
static IMP gContext;
static IMP gExcluded;
static __weak id gApplication;
static BOOL gInstalled;

static void Capture(id object) {
  if ([NSThread isMainThread]) {
    [gContents addObject:object];
  } else {
    __weak id weakObject = object;
    dispatch_async(dispatch_get_main_queue(), ^{
      id current = weakObject;
      if (current) [gContents addObject:current];
    });
  }
}

static id NativeView(id object, SEL selector) {
  Capture(object);
  return ((id (*)(id, SEL))gNativeView)(object, selector);
}

static void Insert(id object, SEL selector, id identifier, id stylesheet) {
  Capture(object);
  ((void (*)(id, SEL, id, id))gInsert)(object, selector, identifier, stylesheet);
}

static id Context(id object, SEL selector) {
  if ([NSThread isMainThread]) [gBrowsers addObject:object];
  return ((id (*)(id, SEL))gContext)(object, selector);
}

static void Excluded(id object, SEL selector, id domains) {
  gApplication = object;
  ((void (*)(id, SEL, id))gExcluded)(object, selector, domains);
}

static id ProfileContext(void) {
  id found = nil;
  for (id browser in gBrowsers) {
    id context = ((id (*)(id, SEL))objc_msgSend)(browser, NSSelectorFromString(@"context"));
    if (found && ![found isEqual:context]) return nil;
    found = context;
  }
  return found;
}

static id NodeSnapshot(id node, NSUInteger* remaining) {
  if (!node || !*remaining) return [NSNull null];
  --*remaining;
  NSMutableDictionary* value = [NSMutableDictionary dictionary];
  for (NSString* key in @[@"type", @"textContent", @"hasEmphasis", @"textSize", @"textColor",
                          @"linkURL", @"imageAlt", @"iframeOrigin", @"formName", @"formActionURL", @"tableName"])
    value[key] = [node valueForKey:key] ?: [NSNull null];
  NSMutableArray* children = [NSMutableArray array];
  for (id child in [node valueForKey:@"children"])
    [children addObject:NodeSnapshot(child, remaining)];
  value[@"children"] = children;
  id control = [node valueForKey:@"formControlInfo"];
  if (control) {
    NSMutableDictionary* fields = [NSMutableDictionary dictionary];
    for (NSString* key in @[@"controlType", @"fieldName", @"fieldValue", @"placeholder",
                            @"isChecked", @"isRequired", @"isRedacted"])
      fields[key] = [control valueForKey:key] ?: [NSNull null];
    NSMutableArray* options = [NSMutableArray array];
    for (id option in [control valueForKey:@"selectOptions"]) {
      NSMutableDictionary* item = [NSMutableDictionary dictionary];
      for (NSString* key in @[@"value", @"text", @"isSelected", @"isDisabled"])
        item[key] = [option valueForKey:key] ?: [NSNull null];
      [options addObject:item];
    }
    fields[@"selectOptions"] = options;
    value[@"formControlInfo"] = fields;
  } else value[@"formControlInfo"] = [NSNull null];
  return value;
}

static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  [data writeToFile:path options:NSDataWritingAtomic error:NULL];
}

static void Poll(void) {
  if (!gInstalled) {
    Class cls = NSClassFromString(@"ArcWebContents");
    Method native = class_getInstanceMethod(cls, @selector(nativeView));
    Method insert = class_getInstanceMethod(cls, NSSelectorFromString(@"insertStylesheet:stylesheet:"));
    Method context = class_getInstanceMethod(NSClassFromString(@"ArcBrowser"), NSSelectorFromString(@"context"));
    Method excluded = class_getInstanceMethod(NSClassFromString(@"ArcBrowserApplication"), NSSelectorFromString(@"setExcludedDomains:"));
    if (!native || !insert || !context || !excluded) return;
    gNativeView = method_setImplementation(native, (IMP)NativeView);
    gInsert = method_setImplementation(insert, (IMP)Insert);
    gContext = method_setImplementation(context, (IMP)Context);
    gExcluded = method_setImplementation(excluded, (IMP)Excluded);
    gInstalled = YES;
    WriteJSON(@{@"pid": @([NSProcessInfo processInfo].processIdentifier), @"installed": @YES},
              [gRequest stringByAppendingString:@".ready.json"]);
  }
  NSData* data = [NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![request isKindOfClass:[NSDictionary class]]) return;
  NSString* requestID = request[@"id"];
  NSString* url = request[@"url"];
  NSString* operation = request[@"operation"];
  if (![requestID isKindOfClass:[NSString class]] || [requestID isEqual:gLastID] ||
      ![url isKindOfClass:[NSString class]] || ![operation isKindOfClass:[NSString class]]) return;
  NSURL* parsed = [NSURL URLWithString:url];
  // Even an erroneous request cannot route the probe to an unrelated page.
  if (![parsed.scheme isEqual:@"http"] || ![parsed.host isEqual:@"127.0.0.1"] ||
      ![parsed.path hasPrefix:@"/arc-renderer-probe-"]) return;
  __weak id match;
  NSUInteger count = 0;
  for (id candidate in gContents) {
    id candidateURL = ((id (*)(id, SEL))objc_msgSend)(candidate, NSSelectorFromString(@"url"));
    if ([candidateURL isKindOfClass:[NSURL class]] && [[candidateURL absoluteString] isEqual:url]) {
      match = candidate;
      ++count;
    }
  }
  // No acknowledgement yet: a newly created wrapper or its URL may not be visible this tick.
  if (!count) return;
  NSMutableDictionary* response = [@{@"id": requestID, @"pid": @([NSProcessInfo processInfo].processIdentifier),
                                     @"url": url, @"matches": @(count), @"operation": operation} mutableCopy];
  @try {
    id target = match;  // Transient only; the registry and cross-tick target remain weak.
    if (count != 1 || !target) {
      response[@"error"] = @"expected exactly one live wrapper";
    } else if ([operation isEqual:@"inspect"]) {
      response[@"ok"] = @YES;
    } else if ([operation isEqual:@"insert"] && [request[@"identifier"] isKindOfClass:[NSString class]] &&
               [request[@"stylesheet"] isKindOfClass:[NSString class]]) {
      ((void (*)(id, SEL, id, id))objc_msgSend)(target, NSSelectorFromString(@"insertStylesheet:stylesheet:"),
                                               request[@"identifier"], request[@"stylesheet"]);
      response[@"ok"] = @YES;
    } else if ([operation isEqual:@"remove"] && [request[@"identifier"] isKindOfClass:[NSString class]]) {
      ((void (*)(id, SEL, id))objc_msgSend)(target, NSSelectorFromString(@"removeStylesheet:"), request[@"identifier"]);
      response[@"ok"] = @YES;
    } else if ([operation isEqual:@"extract"]) {
      id context = ProfileContext();
      if (!context) {
        response[@"error"] = @"expected exactly one browser context";
      } else {
        id service = ((id (*)(id, SEL))objc_msgSend)(context, NSSelectorFromString(@"pageContentExtraction"));
        gLastID = [requestID copy];
        __block BOOL returned = NO;
        ((void (*)(id, SEL, id, id))objc_msgSend)(service,
            NSSelectorFromString(@"getExtractedContentForWebContents:completion:"), target, ^(id content) {
          @try {
            response[@"ok"] = @YES;
            response[@"synchronous"] = @(!returned);
            response[@"mainThread"] = @([NSThread isMainThread]);
            if (content) {
              NSUInteger remaining = 256;
              response[@"result"] = @{@"url": [content valueForKey:@"url"] ?: [NSNull null],
                  @"title": [content valueForKey:@"title"] ?: [NSNull null],
                  @"extractionTime": [content valueForKey:@"extractionTime"] ?: [NSNull null],
                  @"rootNode": NodeSnapshot([content valueForKey:@"rootNode"], &remaining)};
            } else response[@"result"] = [NSNull null];
          } @catch (NSException* exception) {
            response[@"error"] = exception.reason ?: exception.name;
          }
          WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]);
        });
        returned = YES;
        return;
      }
    } else if ([operation isEqual:@"blocked-count"]) {
      int count = ((int (*)(id, SEL))objc_msgSend)(target, NSSelectorFromString(@"blockedResourceCount"));
      response[@"count"] = @(count);
      response[@"ok"] = @YES;
    } else if (([operation isEqual:@"set-block-lists"] || [operation isEqual:@"set-exclusions"]) &&
               [request[@"values"] isKindOfClass:[NSArray class]]) {
      id application = gApplication;
      if (!application) response[@"error"] = @"application was not observed";
      else {
        SEL selector = NSSelectorFromString([operation isEqual:@"set-block-lists"] ? @"setBlockLists:" : @"setExcludedDomains:");
        ((void (*)(id, SEL, id))objc_msgSend)(application, selector, request[@"values"]);
        response[@"ok"] = @YES;
      }
    } else {
      response[@"error"] = @"invalid operation or arguments";
    }
  } @catch (NSException* exception) {
    response[@"error"] = exception.reason ?: exception.name;
  }
  gLastID = [requestID copy];
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]);
}

__attribute__((constructor)) static void StartRendererProbe(void) {
  @autoreleasepool {
    NSString* executable = [NSProcessInfo processInfo].arguments.firstObject;
    NSString* appName = [[[executable stringByDeletingLastPathComponent]
                          stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    if (![executable.lastPathComponent isEqual:@"Arc"] ||
        ![@[@"ArcModAgent-trace156.app", @"ArcModAgent-ours156.app"] containsObject:appName]) return;
    const char* path = getenv("ARC_RENDERER_PROBE_REQUEST");
    if (!path) return;
    gRequest = [NSString stringWithUTF8String:path];
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"renderer-request.json"]) return;
    gContents = [NSHashTable weakObjectsHashTable];
    gBrowsers = [NSHashTable weakObjectsHashTable];
    dispatch_async(dispatch_get_main_queue(), ^{
      Poll();
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 100 * NSEC_PER_MSEC),
                                100 * NSEC_PER_MSEC, 10 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      // A failed driver cannot leave an unbounded polling job in a disposable app.
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        dispatch_source_cancel(timer);
      });
    });
  }
}
