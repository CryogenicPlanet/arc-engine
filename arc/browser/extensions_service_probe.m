// Supplemental public-boundary driver. Disposable apps only; forwards real app
// delegates unchanged. No native pointers, AX, synthesized callbacks, or input.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>
#include <sys/stat.h>

@interface ExtensionsProbeEntry : NSObject
@property(nonatomic, weak) id context;
@property(nonatomic, weak) id service;
@property(nonatomic, weak) id delegate;
@property(nonatomic, weak) id browser;
@property(nonatomic, weak) id browserDelegate;
@end
@implementation ExtensionsProbeEntry
@synthesize context = _context, service = _service, delegate = _delegate;
@synthesize browser = _browser, browserDelegate = _browserDelegate;
@end

@interface ExtensionsProbeWork : NSObject
@property(nonatomic, strong) NSMutableDictionary* response;
@property(nonatomic, weak) ExtensionsProbeEntry* entry;
@property(nonatomic) BOOL returned;
@property(nonatomic) BOOL pending;
@property(nonatomic) BOOL cleanupRequested;
@property(nonatomic, copy) NSString* operationID;
@property(nonatomic, weak) id context;
@property(nonatomic, weak) id browser;
@property(nonatomic) NSTimeInterval settleAt;
@property(nonatomic) NSTimeInterval deadline;
@end
@implementation ExtensionsProbeWork
@synthesize response = _response, entry = _entry, returned = _returned;
@synthesize pending = _pending, settleAt = _settleAt, deadline = _deadline;
@synthesize cleanupRequested = _cleanupRequested, operationID = _operationID;
@synthesize context = _context, browser = _browser;
@end

static NSMapTable* gEntries;
// Independent of transient ArcBrowserContext wrappers. Never retain app objects.
static NSMapTable* gBrowserDelegates;
static NSMutableSet* gHooks;
static NSMutableDictionary* gOwned;
static NSMutableArray* gBaseline;
static NSString* gRequest;
static NSString* gFixtures;
static NSString* gToken;
static NSString* gFixtureMode;
static NSMutableSet* gStartupPending;
static NSString* gRecord;
static NSString* gLastID;
static BOOL gInstalled;
static ExtensionsProbeWork* gWork;
static ExtensionsProbeWork* gCaptureWork;  // Protected by gCaptureLock.
static NSObject* gCaptureLock;
static BOOL gStartupObservationWindow;  // Protected by gCaptureLock.
static NSMutableArray* gDiagnostics;
static NSMutableSet* gOutstandingInstalls;
static NSMutableArray* gInstallAttempts;
static IMP gExtensions;
static IMP gDelegate;
static IMP gBrowserDelegate;

static id Get(id object, NSString* selector) {
  return ((id (*)(id, SEL))objc_msgSend)(object, NSSelectorFromString(selector));
}
static id Get1(id object, NSString* selector, id argument) {
  return ((id (*)(id, SEL, id))objc_msgSend)(object, NSSelectorFromString(selector), argument);
}
static void Put(id object, NSString* selector, id argument) {
  ((void (*)(id, SEL, id))objc_msgSend)(object, NSSelectorFromString(selector), argument);
}
// ObjC C comparison expressions have type int in this .m file; boxing them
// directly produces JSON 0/1. Always emit Foundation's Boolean singletons.
static NSNumber* JSONBool(BOOL value) { return value ? @YES : @NO; }
static id PropertyValue(id object, NSString* field) {
  NSSet* booleanFields = [NSSet setWithArray:@[@"enabled",@"hasAction",@"fromWebstore",@"isGlobal",@"fromIncognito"]];
  NSMethodSignature* signature = [object methodSignatureForSelector:NSSelectorFromString(field)];
  NSString* encoding = signature ? [NSString stringWithUTF8String:signature.methodReturnType] : @"";
  if ([booleanFields containsObject:field]) {
    if (signature.numberOfArguments != 2)
      [NSException raise:@"ExtensionsProbeABI" format:@"%@.%@ lacks a zero-argument Boolean getter",NSStringFromClass([object class]),field];
    if ([encoding isEqual:@"B"])
      return JSONBool(((BOOL (*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(field)));
    if ([encoding isEqual:@"c"]) {
      signed char value = ((signed char (*)(id,SEL))objc_msgSend)(object,NSSelectorFromString(field));
      if (value != 0 && value != 1)
        [NSException raise:@"ExtensionsProbeABI" format:@"%@.%@ returned noncanonical Boolean byte %d",NSStringFromClass([object class]),field,(int)value];
      return JSONBool(value == 1);
    }
    [NSException raise:@"ExtensionsProbeABI" format:@"%@.%@ has unexpected Boolean encoding %@",NSStringFromClass([object class]),field,encoding];
  }
  return [object valueForKey:field];
}
static id Value(id object) {
  if (!object) return NSNull.null;
  if ([object isKindOfClass:NSString.class] || [object isKindOfClass:NSNumber.class] || object == NSNull.null) return object;
  if ([object isKindOfClass:NSURL.class]) return [object absoluteString];
  if ([object isKindOfClass:NSDictionary.class]) {
    NSMutableDictionary* result = [NSMutableDictionary dictionary];
    for (NSString* key in object) result[key] = Value(object[key]);
    return result;
  }
  if ([object isKindOfClass:NSArray.class]) {
    NSMutableArray* result = [NSMutableArray array];
    for (id item in object) [result addObject:Value(item)];
    return result;
  }
  if ([object isKindOfClass:NSImage.class]) {
    NSImage* image = object;
    NSMutableArray* reps = [NSMutableArray array];
    for (NSImageRep* rep in image.representations)
      [reps addObject:@{@"width":@(rep.pixelsWide), @"height":@(rep.pixelsHigh)}];
    return @{@"present":@YES, @"size":@[@(image.size.width), @(image.size.height)], @"representations":reps};
  }
  NSString* cls = NSStringFromClass([object class]);
  NSArray* fields = nil;
  if ([cls isEqual:@"ArcExtensionInfo"])
    fields = @[@"name",@"id",@"description",@"url",@"permissions",@"icon",@"rating",@"numRatings",@"enabled",@"hasAction",@"version",@"omniboxKeyword",@"fromWebstore"];
  else if ([cls isEqual:@"ArcExtensionCommand"])
    fields = @[@"extensionId",@"commandName",@"commandDescription",@"shortcut",@"isGlobal"];
  else if ([cls isEqual:@"ArcExtensionToolbarAction"])
    fields = @[@"id",@"accessibleName",@"tooltipText",@"icon",@"enabled",@"pinnedPosition"];
  else if ([cls isEqual:@"ArcExtensionRuntimeError"])
    fields = @[@"extensionID",@"contextURL",@"message",@"source",@"fromIncognito",@"severity",@"stackTrace"];
  else if ([cls isEqual:@"ArcExtensionRuntimeErrorStackFrame"])
    fields = @[@"lineNumber",@"columnNumber",@"source",@"function"];
  if ([cls isEqual:@"ArcWebContents"]) return @{@"class":cls,@"url":Value(Get(object,@"url"))};
  if (!fields) return @{@"class":cls};
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  for (NSString* field in fields) result[field] = Value(PropertyValue(object,field));
  return result;
}
static NSNumber* Offset(void) {
  struct stat status;
  return stat(gRecord.fileSystemRepresentation, &status) == 0 ? @(status.st_size) : @(-1);
}
static void WriteJSON(id object, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:NULL];
  if (data) [data writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static ExtensionsProbeEntry* Entry(id context) {
  if (!context) return nil;
  ExtensionsProbeEntry* entry = [gEntries objectForKey:context];
  if (!entry) {
    entry = [ExtensionsProbeEntry new]; entry.context = context;
    [gEntries setObject:entry forKey:context];
  }
  return entry;
}
static void Diagnostic(NSString* kind, ExtensionsProbeWork* work, NSDictionary* event) {
  [gDiagnostics addObject:@{@"kind":kind,@"operationID":work.operationID ?: (id)NSNull.null,@"event":event}];
  WriteJSON(gDiagnostics,[gRequest stringByAppendingString:@".diagnostics.json"]);
}
static void Callback(id contextOrBrowser, NSString* selector, id payload) {
  const BOOL callbackMainThread = NSThread.isMainThread;
  // Capture the operation at callback entry, before hopping queues. Never
  // consult a future request's gWork to assign an earlier callback.
  ExtensionsProbeWork* capturedWork;
  BOOL beforeStartupAdoption;
  id synchronous;
  @synchronized(gCaptureLock) {
    capturedWork = gCaptureWork;
    beforeStartupAdoption = gStartupObservationWindow;
    synchronous = capturedWork ? JSONBool(!capturedWork.returned) : (id)NSNull.null;
  }
  NSNumber* recordOffset = Offset();
  __weak id weakOwner = contextOrBrowser;
  void (^save)(void) = ^{
    NSDictionary* event = @{@"selector":selector,@"payload":Value(payload),@"mainThread":JSONBool(callbackMainThread),@"synchronous":synchronous,@"recordOffset":recordOffset};
    // Journal every actual pre-adoption callback independently of discovery
    // requests: incomplete discoveries are not exercised-operation evidence.
    if (beforeStartupAdoption) Diagnostic(@"startup-observation",capturedWork,event);
    if (!capturedWork) {
      if (!beforeStartupAdoption) Diagnostic(@"unassigned",nil,event);
      return;
    }
    if (capturedWork.context != weakOwner && capturedWork.browser != weakOwner) {
      Diagnostic(@"outside-captured-context",capturedWork,event); return;
    }
    if (gWork != capturedWork) { Diagnostic(@"late-delivery",capturedWork,event); return; }
    [capturedWork.response[@"callbacks"] addObject:event];
    [capturedWork.response[@"events"] addObject:@{@"type":@"delegate",@"value":event}];
  };
  if (callbackMainThread) save(); else dispatch_async(dispatch_get_main_queue(),save);
}
static BOOL ABI(Method method, NSUInteger count, NSString* result) {
  if (!method) return NO;
  NSMethodSignature* signature = [NSMethodSignature signatureWithObjCTypes:method_getTypeEncoding(method)];
  return signature.numberOfArguments == count && [[NSString stringWithUTF8String:signature.methodReturnType] isEqual:result];
}
static void HookDelegate(id delegate, BOOL browser) {
  if (!delegate) return;
  Class cls = object_getClass(delegate);
  NSArray* selectors = browser ? @[@"browser:didOpenExtensionPopup:webContents:",@"browser:didResizeExtensionPopup:size:",@"browser:didCloseExtensionPopup:",@"browser:didUpdateExtensionToolbarAction:",@"browser:didUpdateExtensionToolbarActions:"] :
      @[@"browserContext:didEnableExtension:",@"browserContext:didDisableExtension:",@"browserContext:didInstallExtension:",@"browserContext:didUninstallExtension:",@"browserContext:didFailLoadUnpacked:withManifestError:",@"browserContext:didFail:withRuntimeError:",@"browserContext:didFail:withManifestError:",@"browserContext:didFail:withInternalError:",@"browserContext:didChangePinnedExtensionOrder:",@"browserContextDidExtensionSystemReady:"];
  for (NSString* name in selectors) {
    NSString* key = [NSString stringWithFormat:@"%@/%@",NSStringFromClass(cls),name];
    if ([gHooks containsObject:key]) continue;
    SEL selector = NSSelectorFromString(name);
    Method method = class_getInstanceMethod(cls, selector);
    if (!method) continue;
    IMP original = method_getImplementation(method);
    IMP replacement = NULL;
    if ([name isEqual:@"browser:didResizeExtensionPopup:size:"]) {
      if (!ABI(method, 5, @"v")) continue;
      replacement = imp_implementationWithBlock(^(id receiver, id owner, id identifier, CGSize size) {
        Callback(owner, name, @[identifier ?: NSNull.null, @[@(size.width),@(size.height)]]);
        ((void (*)(id, SEL, id, id, CGSize))original)(receiver, selector, owner, identifier, size);
      });
    } else if (ABI(method, 3, @"v")) {
      replacement = imp_implementationWithBlock(^(id receiver, id owner) {
        Callback(owner, name, @[]);
        ((void (*)(id, SEL, id))original)(receiver, selector, owner);
      });
    } else if (ABI(method, 4, @"v")) {
      replacement = imp_implementationWithBlock(^(id receiver, id owner, id payload) {
        Callback(owner, name, @[payload ?: NSNull.null]);
        ((void (*)(id, SEL, id, id))original)(receiver, selector, owner, payload);
      });
    } else if (ABI(method, 5, @"v")) {
      replacement = imp_implementationWithBlock(^(id receiver, id owner, id first, id second) {
        Callback(owner, name, @[first ?: NSNull.null, second ?: NSNull.null]);
        ((void (*)(id, SEL, id, id, id))original)(receiver, selector, owner, first, second);
      });
    }
    if (!replacement) continue;
    // Override inherited methods on the concrete delegate only.
    if (!class_addMethod(cls, selector, replacement, method_getTypeEncoding(method)))
      method_setImplementation(class_getInstanceMethod(cls, selector), replacement);
    [gHooks addObject:key];
  }
}
static id Extensions(id context, SEL selector) {
  id service = ((id (*)(id, SEL))gExtensions)(context, selector);
  if (NSThread.isMainThread) Entry(context).service = service;
  return service;
}
static void Delegate(id service, SEL selector, id delegate) {
  if (NSThread.isMainThread) {
    ExtensionsProbeEntry* entry = Entry(Get(service,@"owner"));
    entry.service = service; entry.delegate = delegate;
    HookDelegate(delegate, NO);
  }
  ((void (*)(id, SEL, id))gDelegate)(service, selector, delegate);
}
static void BrowserDelegate(id browser, SEL selector, id delegate) {
  if (NSThread.isMainThread) {
    if (delegate) [gBrowserDelegates setObject:delegate forKey:browser];
    else [gBrowserDelegates removeObjectForKey:browser];
    HookDelegate(delegate, YES);
  }
  ((void (*)(id, SEL, id))gBrowserDelegate)(browser, selector, delegate);
}
// Context-ID fields are diagnostic only: their wrappers have no value-equality
// selector. Browser routing uses native extension-service equality and hash.
static NSDictionary* ContextIDFields(id context) {
  id identifier = context ? Get(context,@"id") : nil;
  return @{@"present":JSONBool(identifier != nil),
           @"directoryBasename":Value(identifier ? Get(identifier,@"directoryBasename") : nil),
           @"isIncognito":identifier ? JSONBool(((BOOL (*)(id,SEL))objc_msgSend)(identifier,NSSelectorFromString(@"isIncognito"))) : (id)NSNull.null};
}
static NSArray* MatchingBrowsers(id service, id targetContext, NSMutableArray* inventory) {
  Class serviceClass = NSClassFromString(@"ArcBrowserContextExtensions");
  if (![service isKindOfClass:serviceClass] ||
      !ABI(class_getInstanceMethod([service class],@selector(isEqual:)),3,@"B") ||
      !ABI(class_getInstanceMethod([service class],@selector(hash)),2,@"Q"))
    [NSException raise:@"ExtensionsProbeABI" format:@"target extension service identity ABI mismatch"];
  const NSUInteger targetHash = [service hash];
  NSDictionary* targetID = ContextIDFields(targetContext);
  NSMutableArray* matches = [NSMutableArray array];
  for (id browser in gBrowserDelegates.keyEnumerator) {
    id delegate = [gBrowserDelegates objectForKey:browser];
    if (!delegate) continue;
    // These fresh wrapper objects live only in this iteration's local scope.
    id context = Get(browser,@"context");
    id candidateService = context ? Get(context,@"extensions") : nil;
    BOOL identityABI = [candidateService isKindOfClass:serviceClass] &&
        ABI(class_getInstanceMethod([candidateService class],@selector(isEqual:)),3,@"B") &&
        ABI(class_getInstanceMethod([candidateService class],@selector(hash)),2,@"Q");
    BOOL equal = identityABI && [candidateService isEqual:service];
    BOOL reverseEqual = identityABI && [service isEqual:candidateService];
    NSNumber* candidateHash = identityABI ? @([candidateService hash]) : nil;
    BOOL hashEqual = candidateHash && candidateHash.unsignedIntegerValue == targetHash;
    NSDictionary* candidateID = ContextIDFields(context);
    BOOL matchesService = equal && reverseEqual && hashEqual;
    [inventory addObject:@{@"browserClass":NSStringFromClass([browser class]),
        @"delegateClass":NSStringFromClass([delegate class]),@"contextPresent":JSONBool(context != nil),
        @"candidateServicePresent":JSONBool(candidateService != nil),@"identityABI":JSONBool(identityABI),
        @"serviceEqual":JSONBool(equal),@"reverseServiceEqual":JSONBool(reverseEqual),
        @"targetServiceHash":@(targetHash),@"candidateServiceHash":candidateHash ?: (id)NSNull.null,
        @"serviceHashEqual":JSONBool(hashEqual),@"matchesService":JSONBool(matchesService),
        @"targetContextID":targetID,@"candidateContextID":candidateID,
        @"contextIDFieldsEqual":JSONBool([targetID isEqual:candidateID])}];
    if (matchesService) [matches addObject:browser];
  }
  return matches;
}
static NSDictionary* Metadata(id service, id browser) {
  NSMutableDictionary* result = [NSMutableDictionary dictionary];
  NSDictionary* classes = @{@"ArcBrowserContextExtensions":@[@"owner",@"isEqual:",@"hash",@"setDelegate:",@"isExtensionSystemReady",@"listExtensions",@"getExtensionInfo:",@"enableExtension:",@"disableExtension:",@"reloadExtension:",@"uninstallExtension:",@"installExtensionFromUnpackedPath:completion:",@"runJSInExtensionBackgroundPage:js_code:",@"getAllExtensionCommands",@"executeExtensionCommand:commandName:",@"getPinnedExtensionOrder",@"setPinnedExtensionOrder:"],
      @"ArcBrowser":@[@"executeExtensionAction:",@"pinExtension:",@"unpinExtension:",@"moveExtension:targetIndex:"]};
  for (NSString* className in classes) {
    NSMutableDictionary* methods = [NSMutableDictionary dictionary];
    Class cls = [className isEqual:@"ArcBrowser"] ? (browser ? [browser class] : NSClassFromString(@"ArcBrowser")) : [service class];
    for (NSString* name in classes[className]) {
      Method method = class_getInstanceMethod(cls, NSSelectorFromString(name));
      methods[name] = method ? [NSString stringWithUTF8String:method_getTypeEncoding(method)] : (id)NSNull.null;
    }
    result[className] = methods;
  }
  return result;
}
static BOOL Proven(id service, NSString* label, NSString* identifier) {
  if (!identifier.length || ![@[@"A",@"B"] containsObject:label]) return NO;
  id info = Get1(service,@"getExtensionInfo:",identifier);
  NSString* expected = [NSString stringWithFormat:@"ArcCore service probe %@ %@",gToken,label];
  BOOL matches = info && [Get(info,@"id") isEqual:identifier] && [Get(info,@"name") isEqual:expected];
  if (matches && [gFixtureMode isEqual:@"command-line"])
    matches = [Get(info,@"version") isEqual:@"1.2.3"] && [PropertyValue(info,@"fromWebstore") isEqual:@NO];
  return matches;
}
static void OwnershipJournal(void) {
  WriteJSON(@{@"fixtureMode":gFixtureMode,@"startupPending":[gStartupPending.allObjects sortedArrayUsingSelector:@selector(compare:)],@"owned":[gOwned copy],@"outstandingInstalls":gOutstandingInstalls.allObjects,@"attempts":[gInstallAttempts copy]},[gRequest stringByAppendingString:@".owned.json"]);
}
static NSString* ReconcileOwned(id service) {
  // An installation may have completed after its request timed out. Discover
  // only exact nonce names from the actual public installed-extension list.
  for (id info in Get(service,@"listExtensions")) {
    for (NSString* label in @[@"A",@"B"]) {
      NSString* name = [NSString stringWithFormat:@"ArcCore service probe %@ %@",gToken,label];
      if (![Get(info,@"name") isEqual:name]) continue;
      NSString* identifier = Get(info,@"id");
      if (!Proven(service,label,identifier)) return @"nonce fixture from list failed ID/name verification";
      if (gOwned[label] && ![gOwned[label] isEqual:identifier]) return @"multiple installed IDs share nonce fixture name; refusing ambiguous cleanup";
      gOwned[label] = identifier;
      [gStartupPending removeObject:label];
    }
  }
  if ([gFixtureMode isEqual:@"command-line"] && !gStartupPending.count && gOwned.count == 2)
    @synchronized(gCaptureLock) { gStartupObservationWindow = NO; }
  OwnershipJournal();
  return nil;
}
static NSString* Cleanup(ExtensionsProbeWork* work) {
  if ([gFixtureMode isEqual:@"command-line"]) {
    if (gOutstandingInstalls.count) return @"cleanup cannot certify while installs remain outstanding";
    NSString* error = ReconcileOwned(work.entry.service);
    if (error) return error;
    if (gStartupPending.count) return @"command-line startup ownership unresolved; cleanup cannot be certified";
    if (!gBaseline && gOwned.count == 2) {
      gBaseline = [NSMutableArray array];
      for (NSString* identifier in Get(work.entry.service,@"getPinnedExtensionOrder"))
        if (![gOwned.allValues containsObject:identifier]) [gBaseline addObject:identifier];
    }
  }
  if (!gBaseline) return @"baseline was never captured; refusing pin mutation";
  if (gOutstandingInstalls.count) return @"cleanup cannot certify while installs remain outstanding";
  id service = work.entry.service;
  NSString* error = ReconcileOwned(service);
  if (error) return error;
  for (NSString* label in [gOwned.allKeys copy]) {
    NSString* identifier = gOwned[label];
    if (Proven(service,label,identifier)) Put(service,@"uninstallExtension:",identifier);
    else if (Get1(service,@"getExtensionInfo:",identifier)) return @"cleanup fixture identity changed; refusing uninstall";
  }
  Put(service,@"setPinnedExtensionOrder:",gBaseline);
  work.pending = NO;
  work.settleAt = NSDate.timeIntervalSinceReferenceDate + 0.8;
  return nil;
}
static void Finish(NSString* error) {
  ExtensionsProbeWork* work = gWork;
  if (!work) return;
  @try {
    NSMutableDictionary* infos = [NSMutableDictionary dictionary];
    for (NSString* label in gOwned) infos[label] = Value(Get1(work.entry.service,@"getExtensionInfo:",gOwned[label]));
    work.response[@"infosAfter"] = infos;
    work.response[@"pinsAfter"] = Value(Get(work.entry.service,@"getPinnedExtensionOrder"));
    work.response[@"owned"] = [gOwned copy];
    work.response[@"outstandingInstalls"] = gOutstandingInstalls.allObjects;
    work.response[@"startupPending"] = [gStartupPending.allObjects sortedArrayUsingSelector:@selector(compare:)];
    work.response[@"fixtureMode"] = gFixtureMode;
    if ([work.response[@"operation"] isEqual:@"cleanup"]) {
      BOOL absent = YES;
      for (id value in infos.allValues) if (value != NSNull.null) absent = NO;
      work.response[@"cleanupCertified"] = JSONBool(!error && absent && !gOutstandingInstalls.count && !gStartupPending.count && [work.response[@"pinsAfter"] isEqual:gBaseline]);
    }
  } @catch (NSException* exception) { error = exception.reason ?: exception.name; }
  work.response[@"ok"] = JSONBool(!error);
  if (error) work.response[@"probeError"] = error;
  work.response[@"recordEnd"] = Offset();
  WriteJSON(work.response,[gRequest stringByAppendingString:@".response.json"]);
  @synchronized(gCaptureLock) { if (gCaptureWork == work) gCaptureWork = nil; }
  gWork = nil;
}
static void Poll(void) {
  if (!gInstalled) {
    Method extensions = class_getInstanceMethod(NSClassFromString(@"ArcBrowserContext"),NSSelectorFromString(@"extensions"));
    Method delegate = class_getInstanceMethod(NSClassFromString(@"ArcBrowserContextExtensions"),NSSelectorFromString(@"setDelegate:"));
    Method browser = class_getInstanceMethod(NSClassFromString(@"ArcBrowser"),NSSelectorFromString(@"setDelegate:"));
    if (!ABI(extensions,2,@"@") || !ABI(delegate,3,@"v") || !ABI(browser,3,@"v")) return;
    gExtensions = method_setImplementation(extensions,(IMP)Extensions);
    gDelegate = method_setImplementation(delegate,(IMP)Delegate);
    gBrowserDelegate = method_setImplementation(browser,(IMP)BrowserDelegate);
    gInstalled = YES;
    WriteJSON(@{@"pid":@(NSProcessInfo.processInfo.processIdentifier),@"installed":@YES,@"mainThread":JSONBool(NSThread.isMainThread)},[gRequest stringByAppendingString:@".ready.json"]);
  }
  if (gWork) {
    NSTimeInterval now = NSDate.timeIntervalSinceReferenceDate;
    if (now > gWork.deadline) { Finish(gOutstandingInstalls.count ? @"bounded operation timeout; installs outstanding; cleanup not certified" : @"bounded operation timeout"); return; }
    if (gWork.cleanupRequested && !gOutstandingInstalls.count) {
      gWork.cleanupRequested = NO;
      NSString* error = Cleanup(gWork);
      if (error) Finish(error);
      return;
    }
    if (!gWork.pending && now >= gWork.settleAt) Finish(nil);
    return;
  }
  NSData* data = [NSData dataWithContentsOfFile:gRequest];
  NSDictionary* request = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  NSString* operation = request[@"operation"];
  if (![request[@"token"] isEqual:gToken] ||
      ![identifier isKindOfClass:NSString.class] || !identifier.length || identifier.length > 160 ||
      [identifier isEqual:gLastID] || ![operation isKindOfClass:NSString.class]) return;
  gLastID = identifier;
  NSMutableArray* matches = [NSMutableArray array];
  for (ExtensionsProbeEntry* entry in gEntries.objectEnumerator) {
    id contextID = Get(entry.context,@"id");
    BOOL incognito = contextID && ((BOOL (*)(id,SEL))objc_msgSend)(contextID,NSSelectorFromString(@"isIncognito"));
    if (entry.context && entry.service && entry.delegate && !incognito &&
        [Get(contextID,@"directoryBasename") isEqual:@"Default"]) [matches addObject:entry];
  }
  gWork = [ExtensionsProbeWork new];
  gWork.response = [@{@"id":identifier,@"operation":operation,@"pid":@(NSProcessInfo.processInfo.processIdentifier),@"mainThread":JSONBool(NSThread.isMainThread),@"matches":@(matches.count),@"recordBegin":Offset(),@"callbacks":[NSMutableArray array],@"completions":[NSMutableArray array],@"events":[NSMutableArray array]} mutableCopy];
  gWork.entry = matches.firstObject;
  gWork.operationID = identifier;
  gWork.context = gWork.entry.context;
  gWork.deadline = NSDate.timeIntervalSinceReferenceDate + 12;
  gWork.settleAt = NSDate.timeIntervalSinceReferenceDate + 0.8;
  if (matches.count != 1) { Finish(@"expected one live Default context with real app extension delegate"); return; }
  id service = gWork.entry.service;
  id browser = nil;
  @synchronized(gCaptureLock) { gCaptureWork = gWork; }
  NSString* label = request[@"label"];
  NSString* ownedID = gOwned[label ?: @""];
  @try {
    NSMutableArray* inventory = [NSMutableArray array];
    gWork.response[@"browserCandidates"] = inventory;
    NSArray* browsers = MatchingBrowsers(service,gWork.entry.context,inventory);
    browser = browsers.count == 1 ? browsers.firstObject : nil;
    gWork.entry.browser = browser;
    gWork.entry.browserDelegate = browser ? [gBrowserDelegates objectForKey:browser] : nil;
    gWork.response[@"browserMatches"] = @(browsers.count);
    gWork.browser = browser;
    if ([operation isEqual:@"registry"] || [operation isEqual:@"adopt-command-line"]) {
      if ([operation isEqual:@"adopt-command-line"]) {
        if (![gFixtureMode isEqual:@"command-line"]) { Finish(@"adoption only allowed in explicit command-line fixture mode"); return; }
        NSString* error = ReconcileOwned(service);
        if (error) { Finish(error); return; }
        NSMutableDictionary* adopted = [NSMutableDictionary dictionary];
        for (NSString* item in gOwned) adopted[item] = Value(Get1(service,@"getExtensionInfo:",gOwned[item]));
        gWork.response[@"adopted"] = adopted;
        gWork.response[@"adoptionComplete"] = JSONBool(!gStartupPending.count && gOwned.count == 2);
        if (!gStartupPending.count && gOwned.count == 2 && !gBaseline) {
          gBaseline = [NSMutableArray array];
          for (NSString* identifier in Get(service,@"getPinnedExtensionOrder"))
            if (![gOwned.allValues containsObject:identifier]) [gBaseline addObject:identifier];
        }
      }
      gWork.response[@"metadata"] = Metadata(service,browser);
      gWork.response[@"ready"] = JSONBool(((BOOL (*)(id,SEL))objc_msgSend)(service,NSSelectorFromString(@"isExtensionSystemReady")));
      id repeated = Get(gWork.entry.context,@"extensions");
      gWork.response[@"identity"] = JSONBool((service == repeated) && [service isEqual:repeated] && Get(service,@"owner") == gWork.entry.context);
      NSUInteger firstHash = [service hash];
      gWork.response[@"hashStable"] = JSONBool(firstHash == [service hash]);
      gWork.response[@"browserPresent"] = JSONBool(browser && gWork.entry.browserDelegate);
      if ([gFixtureMode isEqual:@"public-install"] && !gOwned.count) gBaseline = [Get(service,@"getPinnedExtensionOrder") mutableCopy];
      gWork.response[@"baselinePins"] = Value(gBaseline);
    } else if ([operation isEqual:@"install"] || [operation isEqual:@"install-invalid"]) {
      BOOL invalid = [operation isEqual:@"install-invalid"];
      if (!invalid && ![gFixtureMode isEqual:@"public-install"]) { Finish(@"public fixture installs forbidden in command-line mode"); return; }
      if (!invalid && (![@[@"A",@"B"] containsObject:label] || ownedID)) { Finish(@"invalid or already installed fixture label"); return; }
      NSString* path = [gFixtures stringByAppendingPathComponent:invalid ? @"invalid" : label];
      ExtensionsProbeWork* work = gWork;
      work.pending = YES;
      [gOutstandingInstalls addObject:work.operationID];
      [gInstallAttempts addObject:@{@"operationID":work.operationID,@"label":invalid ? @"invalid" : label,@"path":path}];
      OwnershipJournal();
      void (^completion)(NSString*, NSInteger) = ^(NSString* extensionID, NSInteger result) {
        const BOOL callbackMainThread = NSThread.isMainThread;
        id synchronous;
        @synchronized(gCaptureLock) { synchronous = JSONBool(!work.returned); }
        void (^save)(void) = ^{
          if (!invalid && result == 0 && Proven(service,label,extensionID)) gOwned[label] = extensionID;
          NSDictionary* completionValue = @{@"id":Value(extensionID),@"result":@(result),@"mainThread":JSONBool(callbackMainThread),@"synchronous":synchronous};
          [gOutstandingInstalls removeObject:work.operationID];
          if (!work.response[@"completion"]) work.response[@"completion"] = completionValue;
          [work.response[@"completions"] addObject:completionValue];
          [work.response[@"events"] addObject:@{@"type":@"completion",@"value":completionValue}];
          work.pending = NO; work.settleAt = NSDate.timeIntervalSinceReferenceDate + 0.8;
          OwnershipJournal();
          if (gWork != work) Diagnostic(@"late-install-completion",work,completionValue);
        };
        if (callbackMainThread) save(); else dispatch_async(dispatch_get_main_queue(),save);
      };
      ((void (*)(id,SEL,id,id))objc_msgSend)(service,NSSelectorFromString(@"installExtensionFromUnpackedPath:completion:"),path,completion);
    } else if ([operation isEqual:@"snapshot"]) {
      NSArray* all = Get(service,@"listExtensions");
      NSMutableArray* infos = [NSMutableArray array];
      for (id info in all) if ([gOwned.allValues containsObject:Get(info,@"id")]) [infos addObject:Value(info)];
      gWork.response[@"list"] = infos;
      NSMutableArray* commands = [NSMutableArray array];
      for (id command in Get(service,@"getAllExtensionCommands"))
        if ([gOwned.allValues containsObject:Get(command,@"extensionId")]) [commands addObject:Value(command)];
      gWork.response[@"commands"] = commands;
    } else if ([operation isEqual:@"pins"]) {
      NSMutableArray* order = [gBaseline mutableCopy];
      for (NSString* item in request[@"labels"]) {
        if (!gOwned[item] || !Proven(service,item,gOwned[item])) { Finish(@"pin target not a proven fixture"); return; }
        [order addObject:gOwned[item]];
      }
      Put(service,@"setPinnedExtensionOrder:",order);
    } else if ([operation isEqual:@"cleanup"]) {
      // Wait for every attempted install, including timed-out requests, before
      // reconciling actual installed nonce fixtures and mutating cleanup state.
      gWork.pending = YES;
      gWork.cleanupRequested = YES;
    } else {
      if (!ownedID || !Proven(service,label,ownedID)) { Finish(@"operation target not a proven fixture"); return; }
      if ([@[@"disable",@"enable",@"reload",@"uninstall"] containsObject:operation]) {
        NSString* selector = [@{@"disable":@"disableExtension:",@"enable":@"enableExtension:",@"reload":@"reloadExtension:",@"uninstall":@"uninstallExtension:"} objectForKey:operation];
        Put(service,selector,ownedID);
      } else if ([operation isEqual:@"command"]) {
        if (![request[@"command"] isEqual:@"probe-command"]) { Finish(@"unsupported fixture command"); return; }
        ((void (*)(id,SEL,id,id))objc_msgSend)(service,NSSelectorFromString(@"executeExtensionCommand:commandName:"),ownedID,@"probe-command");
      } else if ([operation isEqual:@"background-js"]) {
        BOOL value = ((BOOL (*)(id,SEL,id,id))objc_msgSend)(service,NSSelectorFromString(@"runJSInExtensionBackgroundPage:js_code:"),ownedID,@"globalThis.arcCoreProbe = 'must-not-run-in-MV3-worker';");
        gWork.response[@"value"] = JSONBool(value);
      } else if ([operation isEqual:@"action"]) {
        if (browsers.count != 1 || !browser || !gWork.entry.browserDelegate) { Finish(@"expected one observed real app browser delegate; ambiguous or missing browser"); return; }
        Put(browser,@"executeExtensionAction:",ownedID);
        gWork.settleAt = NSDate.timeIntervalSinceReferenceDate + 3;
      } else { Finish(@"unsupported probe operation"); return; }
    }
    @synchronized(gCaptureLock) { gWork.returned = YES; }
    gWork.response[@"recordAfterReturn"] = Offset();
  } @catch (NSException* exception) { Finish(exception.reason ?: exception.name); }
}
__attribute__((constructor)) static void StartExtensionsServiceProbe(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    if (![executable.lastPathComponent isEqual:@"Arc"] || ![@[@"ArcModAgent-trace156.app",@"ArcModAgent-ours156.app"] containsObject:app]) return;
    const char* request = getenv("ARC_EXTENSIONS_PROBE_REQUEST");
    const char* fixtures = getenv("ARC_EXTENSIONS_PROBE_FIXTURES");
    const char* token = getenv("ARC_EXTENSIONS_PROBE_TOKEN");
    const char* record = getenv("ARC_RECORD_PATH");
    const char* mode = getenv("ARC_EXTENSIONS_PROBE_FIXTURE_MODE");
    if (!request || !fixtures || !token || !record) return;
    gFixtureMode = mode ? @(mode) : @"public-install";
    if (![@[@"public-install",@"command-line"] containsObject:gFixtureMode]) return;
    gRequest = @(request); gFixtures = @(fixtures); gToken = @(token); gRecord = @(record);
    if (!gRequest.isAbsolutePath || !gFixtures.isAbsolutePath || !gRecord.isAbsolutePath ||
        ![gRequest.lastPathComponent isEqual:@"extensions-service-request.json"] ||
        ![gFixtures.lastPathComponent isEqual:@"fixtures"] || ![gToken hasPrefix:@"arc-extensions-service-probe-"]) return;
    unsetenv("DYLD_INSERT_LIBRARIES");
    gEntries = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsStrongMemory];
    gBrowserDelegates = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsWeakMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsWeakMemory];
    gHooks = [NSMutableSet set]; gOwned = [NSMutableDictionary dictionary];
    gCaptureLock = [NSObject new]; gDiagnostics = [NSMutableArray array];
    gOutstandingInstalls = [NSMutableSet set]; gInstallAttempts = [NSMutableArray array];
    gStartupPending = [NSMutableSet set];
    gStartupObservationWindow = [gFixtureMode isEqual:@"command-line"];
    if ([gFixtureMode isEqual:@"command-line"]) [gStartupPending addObjectsFromArray:@[@"A",@"B"]];
    WriteJSON(gDiagnostics,[gRequest stringByAppendingString:@".diagnostics.json"]);
    OwnershipJournal();
    if (NSThread.isMainThread) Poll();
    dispatch_async(dispatch_get_main_queue(), ^{
      Poll();
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());
      dispatch_source_set_timer(timer,dispatch_time(DISPATCH_TIME_NOW,100*NSEC_PER_MSEC),100*NSEC_PER_MSEC,10*NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer,^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW,420*NSEC_PER_SEC),dispatch_get_main_queue(),^{ dispatch_source_cancel(timer); });
    });
  }
}
