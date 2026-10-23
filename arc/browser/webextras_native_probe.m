// Test-only boundary probe. No AX, global input, delegate replacement, or native pointer access.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <sys/stat.h>
#include <mach/mach_time.h>
#include <math.h>

@interface WebextrasProbeEntry : NSObject
@property(nonatomic, weak) id owner;
@property(nonatomic, weak) id tools;
@property(nonatomic, weak) id media;
@property(nonatomic, copy) NSString* url;
@property(nonatomic) NSUInteger identifier;
@property(nonatomic) NSUInteger shown;
@property(nonatomic) NSUInteger closed;
@end
@implementation WebextrasProbeEntry
@synthesize owner = _owner, tools = _tools, media = _media, url = _url;
@synthesize identifier = _identifier, shown = _shown, closed = _closed;
@end

// Value object implements only the verified public command/event value protocols.
@interface WebextrasProbeCommand : NSObject
@property(nonatomic, copy) NSString* domain;
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) NSString* params;
@end
@implementation WebextrasProbeCommand
@synthesize domain = _domain, name = _name, params = _params;
@end

@interface WebextrasProbeWork : NSObject
@property(nonatomic, weak) WebextrasProbeEntry* entry;
@property(nonatomic, strong) NSMutableDictionary* response;
@property(nonatomic, copy) NSString* waiting;
@property(nonatomic) NSUInteger previousCount;
@property(nonatomic) NSTimeInterval deadline;
@property(nonatomic) BOOL inInvocation;
@property(nonatomic) NSUInteger forwardingCallbacks;
@property(nonatomic) BOOL commandCompleted;
@property(nonatomic) BOOL subscriptionSeen;
@property(nonatomic, strong) NSNumber* subscriptionToken;
@property(nonatomic, copy) NSString* nonce;
@property(nonatomic, copy) NSString* currentSelector;
@property(nonatomic, strong) NSMutableArray* publicCalls;
@property(nonatomic, strong) NSMutableArray* timeline;
@property(nonatomic) NSTimeInterval started;
@end
@implementation WebextrasProbeWork
@synthesize entry = _entry, response = _response, waiting = _waiting;
@synthesize previousCount = _previousCount, deadline = _deadline;
@synthesize inInvocation = _inInvocation, forwardingCallbacks = _forwardingCallbacks;
@synthesize commandCompleted = _commandCompleted, subscriptionSeen = _subscriptionSeen;
@synthesize subscriptionToken = _subscriptionToken, nonce = _nonce, currentSelector = _currentSelector;
@synthesize publicCalls = _publicCalls, timeline = _timeline, started = _started;
@end

static NSMapTable* gEntries;
static NSString* gRequest;
static NSString* gExpectedURL;
static NSString* gRecord;
static NSString* gLastID;
static WebextrasProbeWork* gWork;
static BOOL gInstalled;
static NSUInteger gEntryID;
static IMP gSetURL;
static IMP gTools;
static IMP gMedia;
static IMP gDisplay;
static IMP gClose;
// Opt-in observation is separate from public-call work and never retains a delegate/service.
static BOOL gFrontendEvents;
static IMP gInspect;
static IMP gFocus;
@interface WebextrasFrontendObservation : NSObject
@property(nonatomic, weak) WebextrasProbeEntry* entry;
@property(nonatomic, copy) NSString* spanID;
@property(nonatomic, copy) NSString* nonce;
@property(nonatomic, copy) NSString* expectedURL;
@property(nonatomic) NSUInteger ownerTag;
@property(nonatomic, strong) NSNumber* recordBegin;
@property(nonatomic, strong) NSMutableArray* callbacks;
@property(nonatomic) NSTimeInterval deadline;
@property(nonatomic) NSUInteger forwarding;
@property(nonatomic) NSUInteger ignored;
@property(nonatomic) BOOL overflow;
@end
@implementation WebextrasFrontendObservation
@synthesize nonce=_nonce,expectedURL=_expectedURL,ownerTag=_ownerTag;
@synthesize entry=_entry,spanID=_spanID,recordBegin=_recordBegin,callbacks=_callbacks,deadline=_deadline;
@synthesize forwarding=_forwarding,ignored=_ignored,overflow=_overflow;
@end
static NSObject* gFrontendLock;
static WebextrasFrontendObservation* gFrontendObservation;

// Opt-in only. The DTO is a real public position result, retained across requests.
static BOOL gMediaClock;
static id gHeldPosition;
static NSString* gHeldToken;
static __weak WebextrasProbeEntry* gHeldEntry;
static NSUInteger gPositionTag;
static NSUInteger gHeldPositionTag;

static NSNumber* Offset(void) {
  struct stat status;
  return stat(gRecord.fileSystemRepresentation, &status) == 0 ? @(status.st_size) : @(-1);
}
static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  if (data) [data writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static id SafeJSON(id value, NSUInteger depth) {
  if (!value) return NSNull.null;
  if ([value isKindOfClass:NSString.class] || [value isKindOfClass:NSNumber.class] || value == NSNull.null) return value;
  if (depth > 6) return @"<depth-limit>";
  if ([value isKindOfClass:NSDictionary.class]) {
    NSMutableDictionary* result = [NSMutableDictionary dictionary];
    for (id key in value) result[[key description]] = SafeJSON(value[key], depth + 1);
    return result;
  }
  if ([value isKindOfClass:NSArray.class]) {
    NSMutableArray* result = [NSMutableArray array];
    for (id item in value) [result addObject:SafeJSON(item, depth + 1)];
    return result;
  }
  return @{@"class":NSStringFromClass(object_getClass(value)), @"description":[value description] ?: @""};
}
static id ErrorJSON(NSError* error) {
  if (!error) return NSNull.null;
  return @{@"domain":error.domain ?: @"", @"code":@(error.code),
           @"description":error.localizedDescription ?: @"", @"userInfo":SafeJSON(error.userInfo, 0)};
}
static void BeginPublic(WebextrasProbeWork* work, NSString* selector) {
  work.inInvocation = YES;
  work.currentSelector = selector;
  [work.timeline addObject:@{@"event":@"publicCall", @"selector":selector}];
  [work.publicCalls addObject:[@{@"selector":selector, @"recordBegin":Offset()} mutableCopy]];
}
static void EndPublic(WebextrasProbeWork* work) {
  work.inInvocation = NO;
  [work.timeline addObject:@{@"event":@"publicReturn", @"selector":work.currentSelector}];
  work.publicCalls.lastObject[@"recordEnd"] = Offset();
  work.response[@"recordAfterReturn"] = Offset();
}
static id Parsed(NSString* text) {
  NSData* data = [text dataUsingEncoding:NSUTF8StringEncoding];
  return data ? [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingFragmentsAllowed error:NULL] : nil;
}
static void BlockResult(WebextrasProbeWork* work, NSString* channel, NSNumber* token, NSString* message, NSError* error) {
  if (!work) return;
  const BOOL mainThread = NSThread.isMainThread;
  NSNumber* offset = Offset();
  id beforeReturn = mainThread ? (work.inInvocation ? @YES : @NO) : (id)NSNull.null;
  NSString* selector = mainThread ? work.currentSelector : nil;
  __weak WebextrasProbeWork* weakWork = work;
  void (^save)(void) = ^{
    WebextrasProbeWork* current = weakWork;
    if (!current || current != gWork) return;
    NSMutableDictionary* event = [@{@"event":channel, @"mainThread":(mainThread ? @YES : @NO),
        @"beforePublicReturn":beforeReturn, @"publicSelector":selector ?: (id)NSNull.null,
        @"recordOffset":offset, @"message":message ?: (id)NSNull.null,
        @"error":ErrorJSON(error), @"subscriptionID":token ?: (id)NSNull.null} mutableCopy];
    id parsed = Parsed(message);
    event[@"parsed"] = SafeJSON(parsed, 0);
    [current.response[@"callbacks"] addObject:event];
    [current.timeline addObject:@{@"event":channel, @"selector":selector ?: (id)NSNull.null,
        @"beforePublicReturn":beforeReturn, @"mainThread":(mainThread ? @YES : @NO)}];
    if ([channel isEqual:@"subscription"]) {
      if ([parsed isKindOfClass:NSDictionary.class]) {
        NSArray* arguments = parsed[@"args"];
        id first = [arguments isKindOfClass:NSArray.class] ? arguments.firstObject : nil;
        if ([first isKindOfClass:NSDictionary.class] && [first[@"value"] isEqual:current.nonce]) current.subscriptionSeen = YES;
      }
    } else current.commandCompleted = YES;
  };
  if (mainThread) save(); else dispatch_async(dispatch_get_main_queue(), save);
}
static WebextrasProbeEntry* Entry(id owner) {
  WebextrasProbeEntry* entry = [gEntries objectForKey:owner];
  if (!entry) {
    entry = [WebextrasProbeEntry new];
    entry.owner = owner;
    entry.identifier = ++gEntryID;
    [gEntries setObject:entry forKey:owner];
  }
  return entry;
}
static void Capture(id owner, id service, NSString* url, BOOL media) {
  __weak id weakOwner = owner;
  __weak id weakService = service;
  void (^save)(void) = ^{
    id current = weakOwner;
    if (!current) return;
    WebextrasProbeEntry* entry = Entry(current);
    if (url) entry.url = url;
    if (weakService) {
      if (media) entry.media = weakService;
      else entry.tools = weakService;
    }
  };
  if ([NSThread isMainThread]) save();
  else dispatch_async(dispatch_get_main_queue(), save);
}
static void SetURL(id owner, SEL selector, id url) {
  NSString* text = [url isKindOfClass:NSURL.class] ? [url absoluteString] :
      ([url isKindOfClass:NSString.class] ? [url copy] : @"");
  Capture(owner, nil, text, NO);
  ((void (*)(id, SEL, id))gSetURL)(owner, selector, url);
}
static id Tools(id owner, SEL selector) {
  id tools = ((id (*)(id, SEL))gTools)(owner, selector);
  Capture(owner, tools, nil, NO);
  return tools;
}
static id Media(id owner, SEL selector) {
  id media = ((id (*)(id, SEL))gMedia)(owner, selector);
  Capture(owner, media, nil, YES);
  return media;
}
static WebextrasProbeWork* CallbackEntry(id tools, BOOL shown, NSUInteger mode, NSMutableDictionary** callback) {
  if (!NSThread.isMainThread) return nil;
  for (WebextrasProbeEntry* entry in gEntries.objectEnumerator) {
    if (entry.tools != tools) continue;
    if (shown) ++entry.shown; else ++entry.closed;
    if (gWork && gWork.entry == entry) {
      WebextrasProbeWork* work = gWork;
      ++work.forwardingCallbacks;
      NSMutableDictionary* event = [@{@"event":shown ? @"display" : @"closed", @"mode":@(mode),
          @"mainThread":@YES, @"beforePublicReturn":(work.inInvocation ? @YES : @NO),
          @"publicSelector":work.currentSelector ?: (id)NSNull.null, @"recordOffset":Offset(),
          @"forwarded":@NO} mutableCopy];
      [work.response[@"callbacks"] addObject:event];
      [work.timeline addObject:@{@"event":shown ? @"display" : @"closed", @"selector":work.currentSelector ?: (id)NSNull.null,
          @"beforePublicReturn":(work.inInvocation ? @YES : @NO), @"mainThread":@YES}];
      *callback = event;
      return work;
    }
  }
  return nil;
}
// Off-main delivery is a failed observation, reported after forwarding the existing app IMP.
// Its timestamp prevents a delayed dispatch from being attributed to a later work item.
static void BackgroundCallback(id tools, BOOL shown, NSUInteger mode, NSTimeInterval entered, NSNumber* offset) {
  __weak id weakTools = tools;
  dispatch_async(dispatch_get_main_queue(), ^{
    WebextrasProbeWork* work = gWork;
    if (!work || work.started > entered || work.entry.tools != weakTools) return;
    if (shown) ++work.entry.shown; else ++work.entry.closed;
    [work.response[@"callbacks"] addObject:@{@"event":shown ? @"display" : @"closed", @"mode":@(mode),
        @"mainThread":@NO, @"beforePublicReturn":NSNull.null, @"recordOffset":offset, @"forwarded":@YES}];
  });
}
static void Display(id receiver, SEL selector, id tools, NSUInteger mode) {
  NSTimeInterval entered = NSDate.timeIntervalSinceReferenceDate;
  NSNumber* offset = Offset();
  NSMutableDictionary* event = nil;
  WebextrasProbeWork* work = CallbackEntry(tools, YES, mode, &event);
  ((void (*)(id, SEL, id, NSUInteger))gDisplay)(receiver, selector, tools, mode);
  if (work) { event[@"forwarded"] = @YES; event[@"recordAfterForwarding"] = Offset(); --work.forwardingCallbacks; }
  else if (!NSThread.isMainThread) BackgroundCallback(tools, YES, mode, entered, offset);
}
static void Closed(id receiver, SEL selector, id tools) {
  NSTimeInterval entered = NSDate.timeIntervalSinceReferenceDate;
  NSNumber* offset = Offset();
  NSMutableDictionary* event = nil;
  WebextrasProbeWork* work = CallbackEntry(tools, NO, 0, &event);
  ((void (*)(id, SEL, id))gClose)(receiver, selector, tools);
  if (work) { event[@"forwarded"] = @YES; event[@"recordAfterForwarding"] = Offset(); --work.forwardingCallbacks; }
  else if (!NSThread.isMainThread) BackgroundCallback(tools, NO, 0, entered, offset);
}
// This wrapper observes the existing app IMP, including nested callbacks, exactly once.
static NSMutableDictionary* FrontendCallbackBegin(id receiver, SEL selector, id tools, NSUInteger mode,
                                                 WebextrasFrontendObservation** captured) {
  @synchronized(gFrontendLock) {
    WebextrasFrontendObservation* observation=gFrontendObservation;
    if (!observation || NSDate.timeIntervalSinceReferenceDate > observation.deadline) return nil;
    WebextrasProbeEntry* entry=observation.entry;id owner=entry.owner;id registeredTools=entry.tools;
    if (!owner || !tools || registeredTools != tools) { ++observation.ignored; return nil; }
    if (observation.callbacks.count >= 128) { observation.overflow=YES; return nil; }
    NSMutableDictionary* event=[@{ @"selector":NSStringFromSelector(selector), @"mode":@(mode),
      @"ownerTag":@(entry.identifier), @"ownerClass":NSStringFromClass(object_getClass(owner)),
      @"serviceClass":NSStringFromClass(object_getClass(tools)), @"delegateClass":NSStringFromClass(object_getClass(receiver)),
      @"abi":@(method_getTypeEncoding(class_getInstanceMethod(object_getClass(receiver),selector))),
      @"mainThread":@(NSThread.isMainThread), @"ordinal":@(observation.callbacks.count+1),
      @"forwardDepth":@(++observation.forwarding), @"recordOffset":Offset(), @"forwarded":@NO,
      @"nativeBeforeTicks":[NSString stringWithFormat:@"%llu",(unsigned long long)mach_absolute_time()] } mutableCopy];
    [observation.callbacks addObject:event]; *captured=observation; return event;
  }
}
static void FrontendCallbackEnd(WebextrasFrontendObservation* observation, NSMutableDictionary* event, BOOL returned) {
  if (!event) return;
  @synchronized(gFrontendLock) {
    event[@"recordAfterForwarding"]=Offset();event[@"forwarded"]=@(returned);
    event[@"nativeAfterTicks"]=[NSString stringWithFormat:@"%llu",(unsigned long long)mach_absolute_time()];
    --observation.forwarding;
  }
}
static void FrontendDisplay(id receiver, SEL selector, id tools, NSUInteger mode) {
  WebextrasFrontendObservation* observation=nil;NSMutableDictionary* event=FrontendCallbackBegin(receiver,selector,tools,mode,&observation);BOOL returned=NO;
  @try { Display(receiver,selector,tools,mode);returned=YES; }
  @finally { FrontendCallbackEnd(observation,event,returned); }
}
static void FrontendClosed(id receiver, SEL selector, id tools) {
  WebextrasFrontendObservation* observation=nil;NSMutableDictionary* event=FrontendCallbackBegin(receiver,selector,tools,0,&observation);BOOL returned=NO;
  @try { Closed(receiver,selector,tools);returned=YES; }
  @finally { FrontendCallbackEnd(observation,event,returned); }
}
static void FrontendInspect(id receiver, SEL selector, id tools, NSUInteger mode) {
  WebextrasFrontendObservation* observation=nil;NSMutableDictionary* event=FrontendCallbackBegin(receiver,selector,tools,mode,&observation);BOOL returned=NO;
  @try { ((void (*)(id,SEL,id,NSUInteger))gInspect)(receiver,selector,tools,mode);returned=YES; }
  @finally { FrontendCallbackEnd(observation,event,returned); }
}
static void FrontendFocus(id receiver, SEL selector, id tools) {
  WebextrasFrontendObservation* observation=nil;NSMutableDictionary* event=FrontendCallbackBegin(receiver,selector,tools,0,&observation);BOOL returned=NO;
  @try { ((void (*)(id,SEL,id))gFocus)(receiver,selector,tools);returned=YES; }
  @finally { FrontendCallbackEnd(observation,event,returned); }
}
static const char* Unqualified(const char* type) {
  for (;;) {
    switch (*type) {
      case 'r': case 'n': case 'N': case 'o': case 'O': case 'R': case 'V': ++type; break;
      default: return type;
    }
  }
}
static BOOL Argument(NSMethodSignature* signature, NSUInteger index, const char* expected) {
  if (index >= signature.numberOfArguments) return NO;
  return [[NSString stringWithUTF8String:Unqualified([signature getArgumentTypeAtIndex:index])] isEqualToString:@(expected)];
}
static BOOL Signature(Method method, NSUInteger count, const char* result) {
  if (!method) return NO;
  NSMethodSignature* signature = [NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(method)];
  return signature.numberOfArguments == count && signature.methodReturnType[0] == result[0];
}
static BOOL Install(void) {
  Class contents = NSClassFromString(@"ArcWebContents");
  Class delegate = NSClassFromString(@"_TtC3ADK18WebContentChromium");
  Method url = class_getInstanceMethod(contents, NSSelectorFromString(@"setUrl:"));
  Method tools = class_getInstanceMethod(contents, NSSelectorFromString(@"devTools"));
  Method media = class_getInstanceMethod(contents, NSSelectorFromString(@"mediaSession"));
  Method display = class_getInstanceMethod(delegate, NSSelectorFromString(@"webContentsDevTools:displayAs:"));
  Method close = class_getInstanceMethod(delegate, NSSelectorFromString(@"webContentsDevToolsDidClose:"));
  if (!Signature(url, 3, "v") || !Signature(tools, 2, "@") || !Signature(media, 2, "@") ||
      !Signature(display, 4, "v") || !Signature(close, 3, "v")) return NO;
  NSMethodSignature* displayABI = [NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(display)];
  if (!Argument(displayABI, 2, "@") || !Argument(displayABI, 3, "Q")) return NO;
  Method inspect=NULL,focus=NULL;
  if(gFrontendEvents){
    inspect=class_getInstanceMethod(delegate,NSSelectorFromString(@"webContentsDevTools:inspectModeDidChange:"));
    focus=class_getInstanceMethod(delegate,NSSelectorFromString(@"webContentsDevToolsNeedsFocus:"));
    NSMethodSignature* inspectABI=inspect?[NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(inspect)]:nil;
    NSMethodSignature* focusABI=focus?[NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(focus)]:nil;
    NSMethodSignature* closeABI=[NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(close)];
    if(!Signature(inspect,4,"v")||!Signature(focus,3,"v")||!Argument(inspectABI,2,"@")||!Argument(inspectABI,3,"Q")||!Argument(focusABI,2,"@")||!Argument(closeABI,2,"@"))return NO;
    gInspect=method_setImplementation(inspect,(IMP)FrontendInspect);
    gFocus=method_setImplementation(focus,(IMP)FrontendFocus);
  }
  gSetURL = method_setImplementation(url, (IMP)SetURL);
  gTools = method_setImplementation(tools, (IMP)Tools);
  gMedia = method_setImplementation(media, (IMP)Media);
  gDisplay = method_setImplementation(display, gFrontendEvents ? (IMP)FrontendDisplay : (IMP)Display);
  gClose = method_setImplementation(close, gFrontendEvents ? (IMP)FrontendClosed : (IMP)Closed);
  return YES;
}
static BOOL CleanupSubscription(WebextrasProbeWork* work);
static void Finish(NSString* error) {
  if (!gWork) return;
  if (!CleanupSubscription(gWork)) error = error ?: @"subscription cleanup failed";
  gWork.response[@"publicCalls"] = gWork.publicCalls ?: @[];
  gWork.response[@"timeline"] = gWork.timeline ?: @[];
  gWork.response[@"ok"] = error ? @NO : @YES;
  if (error) gWork.response[@"probeError"] = error;
  gWork.response[@"recordEnd"] = Offset();
  WriteJSON(gWork.response, [gRequest stringByAppendingString:@".response.json"]);
  gWork = nil;
}
static NSString* FrontendObservationOperation(WebextrasProbeWork* work, WebextrasProbeEntry* entry, NSDictionary* request, NSString* operation) {
  if(!gFrontendEvents)return @"frontend observation was not explicitly enabled";
  NSString* spanID=request[@"spanID"];NSNumber* ownerTag=request[@"ownerTag"];NSString* nonce=request[@"observationNonce"];
  NSArray* components=[NSURL URLWithString:gExpectedURL].pathComponents;
  NSString* expectedNonce=components.count==3?components[1]:nil;
  if(![spanID isKindOfClass:NSString.class]||!spanID.length||spanID.length>128||![ownerTag isKindOfClass:NSNumber.class]||!ownerTag.unsignedIntegerValue||CFGetTypeID((__bridge CFTypeRef)ownerTag)==CFBooleanGetTypeID()||![ownerTag isEqualToNumber:@(ownerTag.unsignedIntegerValue)]||
     ![nonce isKindOfClass:NSString.class]||![nonce isEqual:expectedNonce]||![request[@"url"] isEqual:gExpectedURL])return @"invalid frontend span/nonce/owner identity";
  @synchronized(gFrontendLock){
    if([operation isEqual:@"frontend-observe-arm"]){
      if(gFrontendObservation)return @"frontend observation already armed";
      if(!entry.owner||!entry.tools||entry.identifier!=ownerTag.unsignedIntegerValue)return @"frontend arm owner/service unavailable";
      WebextrasFrontendObservation* observation=[WebextrasFrontendObservation new];
      observation.entry=entry;observation.spanID=spanID;observation.nonce=nonce;observation.expectedURL=gExpectedURL;observation.ownerTag=entry.identifier;observation.recordBegin=Offset();observation.callbacks=[NSMutableArray array];
      observation.deadline=NSDate.timeIntervalSinceReferenceDate+5;gFrontendObservation=observation;
    }
    WebextrasFrontendObservation* observation=gFrontendObservation;
    if(!observation||![observation.spanID isEqual:spanID]||observation.ownerTag!=ownerTag.unsignedIntegerValue||![observation.nonce isEqual:nonce]||![observation.expectedURL isEqual:request[@"url"]])return @"frontend span/owner changed";
    if(![operation isEqual:@"frontend-observe-arm"]&&![operation isEqual:@"frontend-observe-read"]&&![operation isEqual:@"frontend-observe-disarm"])return @"unsupported frontend observation operation";
    work.response[@"observation"]=@{@"spanID":observation.spanID,@"ownerTag":@(observation.ownerTag),@"observationNonce":observation.nonce,@"recordBegin":observation.recordBegin,
      @"recordEnd":Offset(),@"callbacks":SafeJSON(observation.callbacks,0),@"forwarding":@(observation.forwarding),
      @"ignoredServiceCallbacks":@(observation.ignored),@"overflow":@(observation.overflow),@"expired":@(NSDate.timeIntervalSinceReferenceDate>observation.deadline)};
    if([operation isEqual:@"frontend-observe-disarm"]){
      if(observation.forwarding)return @"frontend callbacks still forwarding";
      gFrontendObservation=nil;
    }
  }
  return nil;
}
static BOOL HasABI(id target, NSString* selector, NSUInteger count, char result) {
  NSMethodSignature* signature = [target methodSignatureForSelector:NSSelectorFromString(selector)];
  return signature && signature.numberOfArguments == count && signature.methodReturnType[0] == result;
}
static NSString* ClockTicks(void) {
  return [NSString stringWithFormat:@"%llu", (unsigned long long)mach_absolute_time()];
}
static NSMutableDictionary* ClockBegin(WebextrasProbeWork* work, id receiver, NSString* selector) {
  BeginPublic(work, selector);
  NSMutableDictionary* call = work.publicCalls.lastObject;
  call[@"receiverClass"] = NSStringFromClass(object_getClass(receiver));
  call[@"abi"] = [NSString stringWithUTF8String:method_getTypeEncoding(class_getInstanceMethod(object_getClass(receiver), NSSelectorFromString(selector)))];
  call[@"clockBeforeTicks"] = ClockTicks();
  return call;
}
static void ClockEnd(WebextrasProbeWork* work, NSMutableDictionary* call) {
  call[@"clockAfterTicks"] = ClockTicks();
  EndPublic(work);
}
static BOOL ClockDouble(WebextrasProbeWork* work, id receiver, NSString* selector) {
  if (!HasABI(receiver, selector, 2, 'd')) return NO;
  NSMutableDictionary* call = ClockBegin(work, receiver, selector);
  double value = ((double (*)(id, SEL))objc_msgSend)(receiver, NSSelectorFromString(selector));
  ClockEnd(work, call);
  if (!isfinite(value)) { call[@"nonfinite"] = @YES; return NO; }
  call[@"value"] = @(value);
  // The recorder formats doubles with %%g; preserve its source-derived spelling too.
  call[@"recordValue"] = [NSString stringWithFormat:@"%g", value];
  return YES;
}
static NSString* ClockOperation(WebextrasProbeWork* work, WebextrasProbeEntry* entry, NSDictionary* request, NSString* operation) {
  if (!gMediaClock) return @"media clock was not explicitly enabled";
  if ([operation isEqual:@"clock-release"]) {
    if (![request[@"heldToken"] isEqual:gHeldToken] || gHeldEntry != entry) return @"stale held position token";
    gHeldPosition = nil; gHeldToken = nil; gHeldEntry = nil;
    return nil;
  }
  if ([operation isEqual:@"clock-snapshot"]) {
    id media = entry.media;
    if (!HasABI(media, @"position", 2, '@')) return @"unexpected position getter ABI";
    NSMutableDictionary* call = ClockBegin(work, media, @"position");
    id position = ((id (*)(id, SEL))objc_msgSend)(media, NSSelectorFromString(@"position"));
    ClockEnd(work, call);
    call[@"resultClass"] = position ? NSStringFromClass(object_getClass(position)) : (id)NSNull.null;
    // A failed receipt may not leave an earlier phase's DTO available as a fallback.
    gHeldPosition = nil; gHeldToken = nil; gHeldEntry = nil;
    if (!position || object_getClass(position) != NSClassFromString(@"ArcMediaPosition")) return @"position did not return an actual ArcMediaPosition";
    gHeldPosition = position; gHeldToken = request[@"id"]; gHeldEntry = entry;
    gHeldPositionTag = ++gPositionTag;
  } else if (![operation isEqual:@"clock-held-time"]) return @"unsupported media clock operation";
  else if (!gHeldPosition || gHeldEntry != entry || ![request[@"heldToken"] isEqual:gHeldToken]) return @"stale held position token";
  mach_timebase_info_data_t timebase;
  if (mach_timebase_info(&timebase) != KERN_SUCCESS || !timebase.denom) return @"mach timebase unavailable";
  work.response[@"clock"] = @{@"heldToken":gHeldToken, @"dtoTag":@(gHeldPositionTag),
      @"timebaseNumer":@(timebase.numer), @"timebaseDenom":@(timebase.denom)};
  if ([operation isEqual:@"clock-snapshot"]) {
    if (!ClockDouble(work, gHeldPosition, @"playbackRate") || !ClockDouble(work, gHeldPosition, @"duration") ||
        !ClockDouble(work, gHeldPosition, @"currentTime")) return @"unexpected/nonfinite position double getter";
    if (!HasABI(gHeldPosition, @"endOfMedia", 2, 'B')) return @"unexpected endOfMedia ABI";
    NSMutableDictionary* call = ClockBegin(work, gHeldPosition, @"endOfMedia");
    BOOL value = ((BOOL (*)(id, SEL))objc_msgSend)(gHeldPosition, NSSelectorFromString(@"endOfMedia"));
    ClockEnd(work, call);
    call[@"value"] = value ? @YES : @NO;
  } else if (!ClockDouble(work, gHeldPosition, @"currentTime")) return @"unexpected/nonfinite held currentTime getter";
  return nil;
}
static BOOL CleanupSubscription(WebextrasProbeWork* work) {
  if (!work.subscriptionToken) return YES;
  id tools = work.entry.tools;
  NSMethodSignature* signature = [tools methodSignatureForSelector:NSSelectorFromString(@"unsubscribe:")];
  if (!signature || signature.numberOfArguments != 3 ||
      (signature.methodReturnType[0] != 'B' && signature.methodReturnType[0] != 'c') || !Argument(signature, 2, "@")) return NO;
  BeginPublic(work, @"unsubscribe:");
  BOOL value = ((BOOL (*)(id, SEL, id))objc_msgSend)(tools, NSSelectorFromString(@"unsubscribe:"), work.subscriptionToken);
  EndPublic(work);
  work.response[@"unsubscribeReturn"] = value ? @YES : @NO;
  work.subscriptionToken = nil;
  return value;
}
static WebextrasProbeCommand* Command(NSString* domain, NSString* name, NSString* params) {
  WebextrasProbeCommand* command = [WebextrasProbeCommand new];
  command.domain = domain; command.name = name; command.params = params;
  return command;
}
static BOOL ExecuteABI(id tools) {
  NSMethodSignature* signature = [tools methodSignatureForSelector:NSSelectorFromString(@"execute:completion:error:")];
  return signature && signature.numberOfArguments == 5 &&
      (signature.methodReturnType[0] == 'B' || signature.methodReturnType[0] == 'c') &&
      Argument(signature, 2, "@") && Argument(signature, 3, "@?") && Argument(signature, 4, "^@");
}
static void Poll(void) {
  if (!gInstalled) {
    if (!Install()) return;
    gInstalled = YES;
    WriteJSON(@{@"pid":@([NSProcessInfo processInfo].processIdentifier), @"installed":@YES,
                @"mainThread":([NSThread isMainThread] ? @YES : @NO)}, [gRequest stringByAppendingString:@".ready.json"]);
  }
  if (gWork) {
    WebextrasProbeEntry* entry = gWork.entry;
    if (!entry.owner || !entry.tools) { Finish(@"owner or service disappeared"); return; }
    BOOL done = NO;
    if ([gWork.waiting isEqual:@"completion"]) done = gWork.commandCompleted;
    else if ([gWork.waiting isEqual:@"subscription"]) done = gWork.commandCompleted && gWork.subscriptionSeen;
    else {
      NSUInteger count = [gWork.waiting isEqual:@"shown"] ? entry.shown : entry.closed;
      done = count > gWork.previousCount;
    }
    if (done && !gWork.forwardingCallbacks) { Finish(nil); return; }
    if (NSDate.timeIntervalSinceReferenceDate >= gWork.deadline) Finish(@"native callback timed out; no AX fallback");
    return;
  }
  NSData* data = [NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  NSString* operation = request[@"operation"];
  if (![identifier isKindOfClass:NSString.class] || !identifier.length || identifier.length > 128 ||
      [identifier isEqual:gLastID] || ![operation isKindOfClass:NSString.class] ||
      ![request[@"url"] isEqual:gExpectedURL]) return;
  gLastID = identifier;
  // An immutable armed identity permits journal-only cleanup after weak owner/service loss.
  // This dispatch does not admit new arming or normal public work on another entry.
  if(gFrontendEvents&&([operation isEqual:@"frontend-observe-read"]||[operation isEqual:@"frontend-observe-disarm"])){
    @synchronized(gFrontendLock){
      WebextrasFrontendObservation* observation=gFrontendObservation;
      if(observation){
        gWork=[WebextrasProbeWork new];gWork.publicCalls=[NSMutableArray array];gWork.timeline=[NSMutableArray array];
        gWork.started=NSDate.timeIntervalSinceReferenceDate;gWork.entry=observation.entry;
        gWork.response=[@{@"id":identifier,@"operation":operation,@"url":gExpectedURL,@"pid":@(NSProcessInfo.processInfo.processIdentifier),
          @"mainThread":@(NSThread.isMainThread),@"matches":@(observation.entry.owner&&observation.entry.tools?1:0),
          @"ownerTag":@(observation.ownerTag),@"recordBegin":Offset(),@"callbacks":[NSMutableArray array]} mutableCopy];
        Finish(FrontendObservationOperation(gWork,observation.entry,request,operation));return;
      }
    }
  }
  NSMutableArray* matches = [NSMutableArray array];
  for (WebextrasProbeEntry* entry in gEntries.objectEnumerator) {
    if (entry.owner && entry.tools && entry.media && [entry.url isEqual:gExpectedURL]) [matches addObject:entry];
  }
  gWork = [WebextrasProbeWork new];
  gWork.publicCalls = [NSMutableArray array];
  gWork.timeline = [NSMutableArray array];
  gWork.started = NSDate.timeIntervalSinceReferenceDate;
  gWork.response = [@{@"id":identifier, @"operation":operation, @"url":gExpectedURL,
      @"pid":@([NSProcessInfo processInfo].processIdentifier), @"mainThread":([NSThread isMainThread] ? @YES : @NO),
      @"matches":@(matches.count), @"recordBegin":Offset(), @"callbacks":[NSMutableArray array]} mutableCopy];
  if (matches.count != 1) { Finish(@"expected exactly one live service-bearing app wrapper"); return; }
  WebextrasProbeEntry* entry = matches.firstObject;
  gWork.entry = entry;
  gWork.response[@"ownerTag"] = @(entry.identifier);
  id tools = entry.tools;
  id media = entry.media;
  @try {
    if ([operation hasPrefix:@"frontend-observe-"]) {
      Finish(FrontendObservationOperation(gWork,entry,request,operation));return;
    }
    if(gFrontendObservation){Finish(@"public probe work cannot overlap frontend observation");return;}
    if ([operation isEqual:@"registry"]) {
      gWork.response[@"toolsClass"] = NSStringFromClass(object_getClass(tools));
      gWork.response[@"mediaClass"] = NSStringFromClass(object_getClass(media));
    } else if ([operation hasPrefix:@"clock-"]) {
      NSString* failure = ClockOperation(gWork, entry, request, operation);
      Finish(failure);
      return;
    } else if ([operation isEqual:@"console-open"] || [operation isEqual:@"console-close"]) {
      BOOL opening = [operation isEqual:@"console-open"];
      NSString* selector = opening ? @"openConsoleView" : @"close";
      if (!HasABI(tools, selector, 2, 'v')) { Finish(@"unexpected DevTools method ABI"); return; }
      gWork.waiting = opening ? @"shown" : @"closed";
      gWork.previousCount = opening ? entry.shown : entry.closed;
      gWork.deadline = NSDate.timeIntervalSinceReferenceDate + 15;
      BeginPublic(gWork, selector);
      ((void (*)(id, SEL))objc_msgSend)(tools, NSSelectorFromString(selector));
      EndPublic(gWork);
      return;
    } else if ([operation isEqual:@"frontend"]) {
      if (!HasABI(tools, @"webContents", 2, '@')) { Finish(@"unexpected frontend getter ABI"); return; }
      BeginPublic(gWork, @"webContents");
      id frontend = ((id (*)(id, SEL))objc_msgSend)(tools, NSSelectorFromString(@"webContents"));
      EndPublic(gWork);
      gWork.response[@"frontendPresent"] = frontend ? @YES : @NO;
      gWork.response[@"frontendClass"] = frontend ? NSStringFromClass(object_getClass(frontend)) : (id)NSNull.null;
    } else if ([operation isEqual:@"panel-closed"]) {
      NSMethodSignature* signature = [tools methodSignatureForSelector:NSSelectorFromString(@"currentPanel:error:")];
      if (!signature || signature.numberOfArguments != 4 ||
          (signature.methodReturnType[0] != 'B' && signature.methodReturnType[0] != 'c') ||
          !Argument(signature, 2, "@?") || !Argument(signature, 3, "^@")) { Finish(@"unexpected currentPanel ABI"); return; }
      NSError* error = nil;
      BeginPublic(gWork, @"currentPanel:error:");
      BOOL value = ((BOOL (*)(id, SEL, id, NSError* __autoreleasing*))objc_msgSend)(tools, NSSelectorFromString(@"currentPanel:error:"), nil, &error);
      EndPublic(gWork);
      gWork.response[@"value"] = value ? @YES : @NO; gWork.response[@"error"] = ErrorJSON(error);
    } else if ([operation hasPrefix:@"execute-"] || [operation isEqual:@"subscription"]) {
      if (!ExecuteABI(tools)) { Finish(@"unexpected execute ABI"); return; }
      BOOL malformed = [operation isEqual:@"execute-malformed"];
      BOOL unknown = [operation isEqual:@"execute-unknown"];
      BOOL subscription = [operation isEqual:@"subscription"];
      WebextrasProbeWork* work = gWork;
      __weak WebextrasProbeWork* weakWork = work;
      work.waiting = subscription ? @"subscription" : @"completion";
      work.deadline = NSDate.timeIntervalSinceReferenceDate + 15;
      work.nonce = [request[@"nonce"] isKindOfClass:NSString.class] ? request[@"nonce"] : identifier;
      work.response[@"nonce"] = work.nonce;
      if (subscription) {
        NSMethodSignature* signature = [tools methodSignatureForSelector:NSSelectorFromString(@"subscribe:autoEnable:notification:")];
        if (!signature || signature.numberOfArguments != 5 || signature.methodReturnType[0] != '@' ||
            !Argument(signature, 2, "@") || (!Argument(signature, 3, "B") && !Argument(signature, 3, "c")) || !Argument(signature, 4, "@?")) {
          Finish(@"unexpected subscription ABI"); return;
        }
        void (^notification)(NSNumber*, NSString*, NSError*) = ^(NSNumber* token, NSString* params, NSError* error) {
          BlockResult(weakWork, @"subscription", token, params, error);
        };
        BeginPublic(work, @"subscribe:autoEnable:notification:");
        id token = ((id (*)(id, SEL, id, BOOL, id))objc_msgSend)(tools, NSSelectorFromString(@"subscribe:autoEnable:notification:"),
            Command(@"Runtime", @"consoleAPICalled", nil), YES, notification);
        EndPublic(work);
        if (![token isKindOfClass:NSNumber.class]) { Finish(@"subscription did not return a numeric token"); return; }
        work.subscriptionToken = token;
        work.response[@"subscriptionToken"] = token;
      }
      NSString* expression = @"6*7";
      if (subscription) {
        NSData* quoted = [NSJSONSerialization dataWithJSONObject:work.nonce options:NSJSONWritingFragmentsAllowed error:NULL];
        expression = [NSString stringWithFormat:@"console.log(%@);42", [[NSString alloc] initWithData:quoted encoding:NSUTF8StringEncoding]];
      }
      NSData* encoded = [NSJSONSerialization dataWithJSONObject:@{@"expression":expression, @"returnByValue":@YES} options:0 error:NULL];
      NSString* params = malformed ? @"[]" : (unknown ? @"{}" : [[NSString alloc] initWithData:encoded encoding:NSUTF8StringEncoding]);
      WebextrasProbeCommand* command = Command(unknown ? @"ArcWebextrasMissing" : @"Runtime", unknown ? @"noSuchMethod" : @"evaluate", params);
      void (^completion)(NSString*, NSError*) = malformed ? nil : ^(NSString* message, NSError* error) {
        BlockResult(weakWork, @"completion", nil, message, error);
      };
      NSError* error = nil;
      BeginPublic(work, @"execute:completion:error:");
      BOOL value = ((BOOL (*)(id, SEL, id, id, NSError* __autoreleasing*))objc_msgSend)(tools, NSSelectorFromString(@"execute:completion:error:"), command, completion, &error);
      EndPublic(work);
      work.response[@"value"] = value ? @YES : @NO; work.response[@"error"] = ErrorJSON(error);
      if (!malformed && value) return;
      if (subscription) { Finish(@"subscription command was rejected"); return; }
    } else if ([operation isEqual:@"media-types"]) {
      NSMethodSignature* typesABI = [media methodSignatureForSelector:NSSelectorFromString(@"getMediaContentTypes:")];
      if (!HasABI(media, @"getMediaContentTypes:", 3, '@') || !Argument(typesABI, 2, "^@")) { Finish(@"unexpected media types ABI"); return; }
      NSError* error = nil;
      BeginPublic(gWork, @"getMediaContentTypes:");
      id types = ((id (*)(id, SEL, NSError* __autoreleasing*))objc_msgSend)(media, NSSelectorFromString(@"getMediaContentTypes:"), &error);
      EndPublic(gWork);
      NSMutableArray* values = [NSMutableArray array];
      if (types && ![types isKindOfClass:NSIndexSet.class]) { Finish(@"unexpected media types result class"); return; }
      for (NSUInteger index = [types firstIndex]; types && index != NSNotFound; index = [types indexGreaterThanIndex:index]) [values addObject:@(index)];
      gWork.response[@"types"] = types ? values : (id)NSNull.null;
      gWork.response[@"error"] = ErrorJSON(error);
    } else if ([operation isEqual:@"pip-request-error"] || [operation isEqual:@"pip-exit-error"]) {
      BOOL requestPiP = [operation isEqual:@"pip-request-error"];
      NSString* selector = requestPiP ? @"requestPictureInPicture:" : @"exitPictureInPicture:error:";
      Method method = class_getInstanceMethod(object_getClass(media), NSSelectorFromString(selector));
      NSMethodSignature* signature = method ? [NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(method)] : nil;
      if (!signature || signature.numberOfArguments != (requestPiP ? 3u : 4u) ||
          (signature.methodReturnType[0] != 'B' && signature.methodReturnType[0] != 'c') ||
          !Argument(signature, requestPiP ? 2 : 3, "^@") ||
          (!requestPiP && !Argument(signature, 2, "B") && !Argument(signature, 2, "c"))) { Finish(@"unexpected media BOOL ABI"); return; }
      NSError* error = nil;
      BeginPublic(gWork, selector);
      BOOL value = requestPiP
          ? ((BOOL (*)(id, SEL, NSError* __autoreleasing*))objc_msgSend)(media, NSSelectorFromString(selector), &error)
          : ((BOOL (*)(id, SEL, BOOL, NSError* __autoreleasing*))objc_msgSend)(media, NSSelectorFromString(selector), NO, &error);
      EndPublic(gWork);
      gWork.response[@"value"] = value ? @YES : @NO;
      gWork.response[@"error"] = ErrorJSON(error);
    } else { Finish(@"unsupported probe operation"); return; }
  } @catch (NSException* exception) { Finish(exception.reason ?: exception.name); return; }
  Finish(nil);
}

__attribute__((constructor)) static void StartWebextrasNativeProbe(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    if (![executable.lastPathComponent isEqual:@"Arc"] ||
        ![@[@"ArcModAgent-trace156.app", @"ArcModAgent-ours156.app"] containsObject:app]) return;
    const char* path = getenv("ARC_WEBEXTRAS_PROBE_REQUEST");
    const char* url = getenv("ARC_WEBEXTRAS_PROBE_URL");
    const char* record = getenv("ARC_RECORD_PATH");
    if (!path || !url || !record) return;
    gRequest = @(path); gExpectedURL = @(url); gRecord = @(record);
    gMediaClock = [NSProcessInfo.processInfo.environment[@"ARC_WEBEXTRAS_MEDIA_CLOCK"] isEqual:@"1"];
    gFrontendEvents=[NSProcessInfo.processInfo.environment[@"ARC_WEBEXTRAS_FRONTEND_EVENTS"] isEqual:@"1"];
    if(gFrontendEvents){gFrontendLock=[NSObject new];if(gMediaClock)return;}
    NSURL* parsed = [NSURL URLWithString:gExpectedURL];
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"webextras-request.json"] ||
        ![parsed.scheme isEqual:@"http"] || ![parsed.host isEqual:@"127.0.0.1"] ||
        ![parsed.path hasPrefix:@"/arc-webextras-probe-"] || ![parsed.path hasSuffix:@"/fixture"] ||
        !parsed.port || !gRecord.isAbsolutePath) return;
    gEntries = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality
                                    valueOptions:NSPointerFunctionsStrongMemory];
    if (NSThread.isMainThread) Poll();
    dispatch_async(dispatch_get_main_queue(), ^{
      Poll();
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 100 * NSEC_PER_MSEC), 100 * NSEC_PER_MSEC, 10 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 180 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
