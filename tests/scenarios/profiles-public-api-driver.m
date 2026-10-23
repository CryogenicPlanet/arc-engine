// Test-only driver for the profile lifecycle API (ArcBrowserApplication create/get/load/delete and
// availableBrowserContextAttributes), injected after the recorder. It captures the app's ArcBrowserApplication
// when the app first lists profiles (through the recorder's alias, so recording stays intact), waits for the
// scenario's go file, runs a fixed sequence and writes <dir>/result.json.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSString* gDirectory;
static id gApplication;
static NSMutableDictionary* gResult;
static NSString* const kName = @"Profile 7f3a";

static void Write(void) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:gResult options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:nil];
  [data writeToFile:[gDirectory stringByAppendingPathComponent:@"result.json"] atomically:YES];
}

static NSArray* Names(void) {
  NSMutableArray* names = [NSMutableArray array];
  NSArray* attributes = ((id(*)(id, SEL))objc_msgSend)(gApplication, NSSelectorFromString(@"availableBrowserContextAttributes"));
  for (id entry in attributes) {
    NSString* base = ((id(*)(id, SEL))objc_msgSend)(entry, NSSelectorFromString(@"directoryBasename"));
    NSString* name = ((id(*)(id, SEL))objc_msgSend)(entry, NSSelectorFromString(@"userVisibleName"));
    [names addObject:[NSString stringWithFormat:@"%@=%@", base, name]];
  }
  return [names sortedArrayUsingSelector:@selector(compare:)];
}

static NSString* Describe(id context) {
  if (!context) return @"nil";
  id identifier = ((id(*)(id, SEL))objc_msgSend)(context, NSSelectorFromString(@"id"));
  NSString* base = ((id(*)(id, SEL))objc_msgSend)(identifier, NSSelectorFromString(@"directoryBasename"));
  BOOL incognito = ((BOOL(*)(id, SEL))objc_msgSend)(identifier, NSSelectorFromString(@"isIncognito"));
  return [NSString stringWithFormat:@"%@%@", base, incognito ? @" (incognito)" : @""];
}

static void After(double seconds, dispatch_block_t block) {
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC)), dispatch_get_main_queue(), block);
}

static void Run(void) {
  gResult = [NSMutableDictionary dictionaryWithDictionary:@{@"started" : @YES, @"namesBefore" : Names()}];
  NSError* error = nil;
  BOOL emptyOK = ((BOOL(*)(id, SEL, id, BOOL, NSUInteger, id, NSError**))objc_msgSend)(
      gApplication, NSSelectorFromString(@"createBrowserContext:isHidden:type:completion:error:"), @"", NO, 0,
      ^(id context) { gResult[@"emptyNameCompletion"] = Describe(context); }, &error);
  gResult[@"createEmptyName"] = @{@"returned" : @(emptyOK), @"error" : error ? @(error.code) : @"none"};
  error = nil;
  BOOL ok = ((BOOL(*)(id, SEL, id, BOOL, NSUInteger, id, NSError**))objc_msgSend)(
      gApplication, NSSelectorFromString(@"createBrowserContext:isHidden:type:completion:error:"), kName, NO, 0,
      ^(id context) {
        gResult[@"created"] = Describe(context);
        gResult[@"namesAfterCreate"] = Names();
        // Arc's engine names the new directory itself; get and delete take the directory name.
        NSString* directory = Describe(context);
        gResult[@"getByName"] = Describe(((id(*)(id, SEL, id, NSUInteger))objc_msgSend)(
            gApplication, NSSelectorFromString(@"getBrowserContext:type:"), kName, 0));
        gResult[@"get"] = Describe(((id(*)(id, SEL, id, NSUInteger))objc_msgSend)(
            gApplication, NSSelectorFromString(@"getBrowserContext:type:"), directory, 0));
        ((void (*)(id, SEL, id, NSUInteger, id))objc_msgSend)(
            gApplication, NSSelectorFromString(@"loadBrowserContext:type:completion:"), @"Default", 1, ^(id loaded) {
              gResult[@"loadIncognito"] = Describe(loaded);
              ((void (*)(id, SEL, id, NSUInteger, id))objc_msgSend)(
                  gApplication, NSSelectorFromString(@"loadBrowserContext:type:completion:"), @"Missing 7f3a", 0,
                  ^(id missing) {
                    gResult[@"loadMissing"] = Describe(missing);
                    NSError* deleteError = nil;
                    BOOL deletedByName = ((BOOL(*)(id, SEL, id, NSError**))objc_msgSend)(
                        gApplication, NSSelectorFromString(@"deleteBrowserContext:error:"), kName, &deleteError);
                    gResult[@"deleteByName"] = @{@"returned" : @(deletedByName), @"error" : deleteError ? @(deleteError.code) : @"none"};
                    deleteError = nil;
                    BOOL deleted = ((BOOL(*)(id, SEL, id, NSError**))objc_msgSend)(
                        gApplication, NSSelectorFromString(@"deleteBrowserContext:error:"), directory, &deleteError);
                    gResult[@"delete"] = @{@"returned" : @(deleted), @"error" : deleteError ? @(deleteError.code) : @"none"};
                    deleteError = nil;
                    BOOL deletedMissing = ((BOOL(*)(id, SEL, id, NSError**))objc_msgSend)(
                        gApplication, NSSelectorFromString(@"deleteBrowserContext:error:"), @"Missing 7f3a", &deleteError);
                    gResult[@"deleteMissing"] = @{@"returned" : @(deletedMissing), @"error" : deleteError ? @(deleteError.code) : @"none"};
                    After(3, ^{
                      gResult[@"namesAfterDelete"] = Names();
                      gResult[@"complete"] = @YES;
                      Write();
                    });
                  });
            });
      },
      &error);
  gResult[@"create"] = @{@"returned" : @(ok), @"error" : error ? @(error.code) : @"none"};
  Write();
}

static void WaitForGo(void) {
  if (gApplication && [[NSFileManager defaultManager] fileExistsAtPath:[gDirectory stringByAppendingPathComponent:@"go"]]) {
    Run();
    return;
  }
  After(0.25, ^{ WaitForGo(); });
}

// Wrap the method (or the recorder's alias of it) the app calls to list profiles, to learn the instance.
static BOOL CaptureApplication(void) {
  Class cls = NSClassFromString(@"ArcBrowserApplication");
  if (!cls) return NO;
  SEL alias = NSSelectorFromString(@"__arcrec_availableBrowserContextAttributes");
  SEL plain = NSSelectorFromString(@"availableBrowserContextAttributes");
  Method method = class_getInstanceMethod(cls, alias) ?: class_getInstanceMethod(cls, plain);
  if (!method) return NO;
  IMP original = method_getImplementation(method);
  SEL selector = method_getName(method);
  method_setImplementation(method, imp_implementationWithBlock(^id(id self) {
    if (!gApplication) gApplication = self;
    return ((id(*)(id, SEL))original)(self, selector);
  }));
  return YES;
}

static void CaptureWhenLoaded(void) {
  if (!CaptureApplication()) After(0.05, ^{ CaptureWhenLoaded(); });
}

__attribute__((constructor)) static void Start(void) {
  NSString* directory = [[NSProcessInfo processInfo] environment][@"ARC_SCENARIO_PROFILES_DIRECTORY"];
  if (!directory.length) return;
  gDirectory = directory;
  if (!CaptureApplication()) dispatch_async(dispatch_get_main_queue(), ^{ CaptureWhenLoaded(); });
  dispatch_async(dispatch_get_main_queue(), ^{ WaitForGo(); });
}
