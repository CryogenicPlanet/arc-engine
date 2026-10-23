// Test-only observer of genuine public top-band callbacks in disposable apps.
#import <Foundation/Foundation.h>
#import <objc/message.h>
#import <objc/runtime.h>

@class TopBandProbeObserver;
@interface TopBandProbeState : NSObject
@property(nonatomic, weak) id contents;
@property(nonatomic, weak) id originalDelegate;
@property(nonatomic, strong) TopBandProbeObserver* observer;
@property(nonatomic, copy) NSString* stage;
@property(nonatomic) BOOL armed;
@property(nonatomic) BOOL interference;
@property(nonatomic) BOOL sawSetter;
@end
@implementation TopBandProbeState
@end

static NSLock* gLock;
static NSMapTable* gStates;
static TopBandProbeState* gActive;
static NSMutableArray* gEvents;
static NSMutableArray* gDiagnostics;
static NSString* gRequest;
static NSString* gLastID;
static IMP gNativeView;
static IMP gSetDelegate;
static IMP gGetContext;
static __weak id gApplication;
static BOOL gInstalled;
static BOOL gInjected36;
static NSUInteger gContextCalls;
static BOOL gTruncated;

static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  [data writeToFile:path options:NSDataWritingAtomic error:NULL];
}

// The caller holds gLock. Callbacks are recorded on their actual calling thread.
static void FlushLocked(void) {
  WriteJSON(@{@"pid": @([NSProcessInfo processInfo].processIdentifier), @"events": gEvents,
              @"diagnostics": gDiagnostics, @"truncated": @(gTruncated),
              @"arc36SetBeforeFirstObservedContextCall": @(gInjected36),
              @"observedContextCalls": @(gContextCalls)},
            [gRequest stringByAppendingString:@".events.json"]);
}

static TopBandProbeState* StateLocked(id contents) {
  TopBandProbeState* state = [gStates objectForKey:contents];
  if (!state) {
    state = [TopBandProbeState new];
    state.contents = contents;
    [gStates setObject:state forKey:contents];
  }
  return state;
}

@interface TopBandProbeObserver : NSObject
@property(nonatomic, weak) TopBandProbeState* state;
- (void)webContents:(id)contents didSampleTopBandColor:(id)color shouldCrossfade:(BOOL)crossfade;
@end
@implementation TopBandProbeObserver
- (void)webContents:(id)contents didSampleTopBandColor:(id)color shouldCrossfade:(BOOL)crossfade {
  const BOOL mainThread = [NSThread isMainThread];
  const NSTimeInterval wallTime = [NSDate timeIntervalSinceReferenceDate] + NSTimeIntervalSince1970;
  const NSTimeInterval uptime = [NSProcessInfo processInfo].systemUptime;
  id rgba = [NSNull null];
  if (color) {
    rgba = @[@(((unsigned char (*)(id, SEL))objc_msgSend)(color, NSSelectorFromString(@"r"))),
             @(((unsigned char (*)(id, SEL))objc_msgSend)(color, NSSelectorFromString(@"g"))),
             @(((unsigned char (*)(id, SEL))objc_msgSend)(color, NSSelectorFromString(@"b"))),
             @(((unsigned char (*)(id, SEL))objc_msgSend)(color, NSSelectorFromString(@"a")))];
  }
  [gLock lock];
  TopBandProbeState* state = self.state;
  id forward = state.originalDelegate;
  if (gEvents.count < 8192) {
    [gEvents addObject:@{@"sequence": @(gEvents.count + 1), @"wallTime": @(wallTime),
      @"uptime": @(uptime), @"stage": state.stage ?: @"unassigned", @"mainThread": @(mainThread),
      @"armed": @(state.armed), @"sourceMatches": @(state.contents == contents),
      @"rgba": rgba, @"shouldCrossfade": @(crossfade),
      @"colorClass": color ? NSStringFromClass([color class]) : (id)[NSNull null]}];
  } else gTruncated = YES;
  FlushLocked();
  [gLock unlock];
  SEL callback = NSSelectorFromString(@"webContents:didSampleTopBandColor:shouldCrossfade:");
  if (forward && forward != self && [forward respondsToSelector:callback]) {
    ((void (*)(id, SEL, id, id, BOOL))objc_msgSend)(forward, callback, contents, color, crossfade);
  }
}
@end

static id NativeView(id object, SEL selector) {
  [gLock lock];
  StateLocked(object);
  [gLock unlock];
  return ((id (*)(id, SEL))gNativeView)(object, selector);
}

static void SetDelegate(id object, SEL selector, id delegate) {
  [gLock lock];
  TopBandProbeState* state = StateLocked(object);
  // Installing our observer also traverses this public-selector hook. Keep the
  // app's latest weak delegate rather than replacing it with our own observer.
  if (!delegate || delegate != state.observer) {
    state.originalDelegate = delegate;
    state.sawSetter = YES;
  }
  if (state.armed && delegate != state.observer) {
    state.interference = YES;
    [gDiagnostics addObject:@{@"kind": @"delegate-reassigned-while-armed",
      @"stage": state.stage ?: @"unassigned", @"mainThread": @([NSThread isMainThread]),
      @"wallTime": @([NSDate timeIntervalSinceReferenceDate] + NSTimeIntervalSince1970)}];
    FlushLocked();
  }
  [gLock unlock];
  // Preserve every app setter call and its argument. Interference is reported,
  // never hidden by reinstalling the observer over a later app assignment.
  ((void (*)(id, SEL, id))gSetDelegate)(object, selector, delegate);
}

static id GetContext(id object, SEL selector, id name, NSUInteger type) {
  [gLock lock];
  gApplication = object;
  const BOOL inject = !gContextCalls && getenv("ARC_TOPBAND_PUBLIC36");
  ++gContextCalls;
  [gLock unlock];
  if (inject) {
    ((void (*)(id, SEL, NSUInteger, BOOL))objc_msgSend)(object,
        NSSelectorFromString(@"setFeatureEnabled:enabled:"), 36, YES);
    [gLock lock];
    gInjected36 = YES;
    FlushLocked();
    [gLock unlock];
  }
  return ((id (*)(id, SEL, id, NSUInteger))gGetContext)(object, selector, name, type);
}

static NSDictionary* Surface(void) {
  NSMutableDictionary* value = [NSMutableDictionary dictionary];
  for (NSString* name in @[@"setTopBandColorDelegate:", @"nativeView"]) {
    Method method = class_getInstanceMethod(NSClassFromString(@"ArcWebContents"), NSSelectorFromString(name));
    value[name] = method ? @(method_getTypeEncoding(method)) : @"MISSING";
  }
  for (NSString* name in @[@"r", @"g", @"b", @"a"]) {
    Method method = class_getInstanceMethod(NSClassFromString(@"ArcColor"), NSSelectorFromString(name));
    value[[@"ArcColor." stringByAppendingString:name]] = method ? @(method_getTypeEncoding(method)) : @"MISSING";
  }
  Method callback = class_getInstanceMethod([TopBandProbeObserver class],
      NSSelectorFromString(@"webContents:didSampleTopBandColor:shouldCrossfade:"));
  value[@"observerCallback"] = @(method_getTypeEncoding(callback));
  return value;
}

static void Disarm(void) {
  [gLock lock];
  TopBandProbeState* state = gActive;
  id contents = state.contents;
  id original = state.originalDelegate;
  state.armed = NO;
  [gLock unlock];
  if (contents) ((void (*)(id, SEL, id))objc_msgSend)(contents,
      NSSelectorFromString(@"setTopBandColorDelegate:"), original);
  [gLock lock];
  state.observer = nil;
  gActive = nil;
  FlushLocked();
  [gLock unlock];
}

static void Poll(void) {
  if (!gInstalled) {
    Method native = class_getInstanceMethod(NSClassFromString(@"ArcWebContents"), @selector(nativeView));
    Method setter = class_getInstanceMethod(NSClassFromString(@"ArcWebContents"), NSSelectorFromString(@"setTopBandColorDelegate:"));
    Method context = class_getInstanceMethod(NSClassFromString(@"ArcBrowserApplication"), NSSelectorFromString(@"getBrowserContext:type:"));
    if (!native || !setter || !context) return;
    gNativeView = method_setImplementation(native, (IMP)NativeView);
    gSetDelegate = method_setImplementation(setter, (IMP)SetDelegate);
    gGetContext = method_setImplementation(context, (IMP)GetContext);
    gInstalled = YES;
    WriteJSON(@{@"pid": @([NSProcessInfo processInfo].processIdentifier), @"installed": @YES,
                @"surface": Surface()}, [gRequest stringByAppendingString:@".ready.json"]);
  }
  NSData* data = [NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![request isKindOfClass:[NSDictionary class]]) return;
  NSString* requestID = request[@"id"];
  NSString* operation = request[@"operation"];
  NSString* url = request[@"url"];
  if (![requestID isKindOfClass:[NSString class]] || [requestID isEqual:gLastID] ||
      ![operation isKindOfClass:[NSString class]] || ![url isKindOfClass:[NSString class]]) return;
  NSURL* parsed = [NSURL URLWithString:url];
  if (![parsed.scheme isEqual:@"http"] || ![parsed.host isEqual:@"127.0.0.1"] ||
      ![parsed.path hasPrefix:@"/arc-topband-probe-"]) return;
  [gLock lock];
  NSArray* candidates = gStates.keyEnumerator.allObjects;
  [gLock unlock];
  id target = nil;
  NSUInteger matches = 0;
  for (id candidate in candidates) {
    id candidateURL = ((id (*)(id, SEL))objc_msgSend)(candidate, NSSelectorFromString(@"url"));
    if ([candidateURL isKindOfClass:[NSURL class]] && [[candidateURL absoluteString] isEqual:url]) {
      target = candidate;
      ++matches;
    }
  }
  if (!matches && ![operation isEqual:@"disarm"]) return;
  NSMutableDictionary* response = [@{@"id": requestID, @"operation": operation,
    @"pid": @([NSProcessInfo processInfo].processIdentifier), @"matches": @(matches), @"ok": @NO} mutableCopy];
  @try {
    if ([operation isEqual:@"disarm"]) {
      Disarm();
      response[@"ok"] = @YES;
    } else if (matches != 1) response[@"error"] = @"expected one exact fixture wrapper";
    else if ([operation isEqual:@"arm"]) {
      [gLock lock];
      TopBandProbeState* state = StateLocked(target);
      const BOOL busy = gActive != nil;
      if (!busy) {
        gActive = state;
        state.stage = @"armed-blank";
        state.armed = YES;
        state.observer = [TopBandProbeObserver new];
        state.observer.state = state;
      }
      id observer = state.observer;
      [gLock unlock];
      if (busy) response[@"error"] = @"observer already armed";
      else {
        ((void (*)(id, SEL, id))objc_msgSend)(target, NSSelectorFromString(@"setTopBandColorDelegate:"), observer);
        response[@"ok"] = @YES;
        response[@"originalSetterObserved"] = @(state.sawSetter);
      }
    } else if ([operation isEqual:@"stage"] && [request[@"stage"] isKindOfClass:[NSString class]]) {
      [gLock lock];
      if (gActive.contents == target) {
        gActive.stage = request[@"stage"];
        response[@"ok"] = @YES;
      }
      [gLock unlock];
    } else if ([operation isEqual:@"feature36"] && [request[@"enabled"] isKindOfClass:[NSNumber class]]) {
      id application = gApplication;
      if (application) {
        response[@"before"] = @(((BOOL (*)(id, SEL, NSUInteger))objc_msgSend)(application, NSSelectorFromString(@"isFeatureEnabled:"), 36));
        ((void (*)(id, SEL, NSUInteger, BOOL))objc_msgSend)(application, NSSelectorFromString(@"setFeatureEnabled:enabled:"), 36, [request[@"enabled"] boolValue]);
        response[@"after"] = @(((BOOL (*)(id, SEL, NSUInteger))objc_msgSend)(application, NSSelectorFromString(@"isFeatureEnabled:"), 36));
        response[@"ok"] = @YES;
      } else response[@"error"] = @"application not observed";
    } else if ([operation isEqual:@"drain"]) response[@"ok"] = @YES;
    else response[@"error"] = @"invalid operation";
    [gLock lock];
    response[@"interference"] = @(gActive.interference);
    response[@"eventCount"] = @(gEvents.count);
    response[@"arc36SetBeforeFirstObservedContextCall"] = @(gInjected36);
    id application = gApplication;
    FlushLocked();
    [gLock unlock];
    if (application) response[@"feature36"] = @(((BOOL (*)(id, SEL, NSUInteger))objc_msgSend)(application, NSSelectorFromString(@"isFeatureEnabled:"), 36));
  } @catch (NSException* exception) {
    response[@"error"] = exception.reason ?: exception.name;
  }
  gLastID = [requestID copy];
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]);
}

__attribute__((constructor)) static void StartTopBandProbe(void) {
  @autoreleasepool {
    NSString* executable = [NSProcessInfo processInfo].arguments.firstObject;
    NSString* appName = [[[executable stringByDeletingLastPathComponent]
        stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    if (![executable.lastPathComponent isEqual:@"Arc"] ||
        ![@[@"ArcModAgent-trace156.app", @"ArcModAgent-ours156.app"] containsObject:appName]) return;
    const char* path = getenv("ARC_TOPBAND_PROBE_REQUEST");
    if (!path) return;
    gRequest = [NSString stringWithUTF8String:path];
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"topband-request.json"]) return;
    gLock = [NSLock new];
    gStates = [NSMapTable weakToStrongObjectsMapTable];
    gEvents = [NSMutableArray array];
    gDiagnostics = [NSMutableArray array];
    Poll();
    dispatch_async(dispatch_get_main_queue(), ^{
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, DISPATCH_TIME_NOW, 100 * NSEC_PER_MSEC, 10 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 600 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        dispatch_source_cancel(timer);
        Disarm();
      });
    });
  }
}
