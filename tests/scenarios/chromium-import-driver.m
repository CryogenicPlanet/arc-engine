// Test-only driver for importing from a Chromium-family profile (ArcBrowserContext
// import:withCategories:withAllowedExtensions:...). It captures the app's ArcBrowserApplication the first time
// the app lists profiles, waits for the scenario's go file, loads the Default context and imports the
// scenario's fixture Chrome profile through a stand-in source object (our engine reads only the source's
// public properties), then writes <dir>/result.json with the delegate events.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSString* gDirectory;
static id gApplication;
static id gContext;
static NSMutableDictionary* gResult;
static NSMutableArray* gEvents;

@interface ChromiumImportFixtureSource : NSObject
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) NSString* profileName;
@property(nonatomic, copy) NSString* sourcePath;
@property(nonatomic, copy) NSString* appPath;
@property(nonatomic) long long type;
@property(nonatomic) long long supportedCategories;
@end
@implementation ChromiumImportFixtureSource
@end

static void Write(void) {
  gResult[@"events"] = gEvents;
  NSData* data = [NSJSONSerialization dataWithJSONObject:gResult options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:nil];
  [data writeToFile:[gDirectory stringByAppendingPathComponent:@"result.json"] atomically:YES];
}

static void Event(NSString* kind, NSNumber* category, NSError* error) {
  NSMutableDictionary* event = [NSMutableDictionary dictionaryWithDictionary:@{@"event" : kind}];
  if (category) event[@"category"] = category;
  if (error) event[@"error"] = error.localizedDescription ?: @"error";
  [gEvents addObject:event];
  Write();
}

@interface ChromiumImportDelegate : NSObject
@end
@implementation ChromiumImportDelegate
- (void)didImportInitialize:(id)context { Event(@"initialize", nil, nil); }
- (void)didImportStart:(id)context { Event(@"start", nil, nil); }
- (void)didImportCategoryStart:(id)context withCategory:(long long)category { Event(@"category-start", @(category), nil); }
- (void)didImportCategoryEnd:(id)context withCategory:(long long)category withError:(NSError*)error {
  Event(@"category-end", @(category), error);
}
- (void)didImportEnd:(id)context withError:(NSError*)error {
  Event(@"end", nil, error);
  gResult[@"complete"] = @YES;
  Write();
}
@end
static ChromiumImportDelegate* gDelegate;

static void After(double seconds, dispatch_block_t block) {
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC)), dispatch_get_main_queue(), block);
}

static void Import(NSDictionary* request) {
  ChromiumImportFixtureSource* source = [[ChromiumImportFixtureSource alloc] init];
  source.name = @"Chrome";
  source.profileName = @"Fixture 7f3a";
  source.sourcePath = request[@"source"];
  source.appPath = @"/Applications/Google Chrome.app";
  source.type = 2;
  source.supportedCategories = [request[@"categories"] longLongValue];
  gDelegate = [[ChromiumImportDelegate alloc] init];
  NSError* error = nil;
  BOOL accepted = ((BOOL(*)(id, SEL, id, long long, id, id, BOOL, id, NSError**))objc_msgSend)(
      gContext,
      NSSelectorFromString(@"import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:"),
      source, [request[@"categories"] longLongValue], request[@"extensions"], nil, [request[@"skipExisting"] boolValue],
      gDelegate, &error);
  gResult[@"accepted"] = @(accepted);
  gResult[@"immediateError"] = error ? error.localizedDescription : @"none";
  Write();
}

static void Run(void) {
  NSData* data = [NSData dataWithContentsOfFile:[gDirectory stringByAppendingPathComponent:@"go"]];
  NSDictionary* request = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:nil] : nil;
  gResult = [NSMutableDictionary dictionaryWithDictionary:@{@"started" : @YES}];
  gEvents = [NSMutableArray array];
  if (![request isKindOfClass:[NSDictionary class]]) {
    gResult[@"failure"] = @"bad request";
    gResult[@"complete"] = @YES;
    Write();
    return;
  }
  ((void (*)(id, SEL, id, NSUInteger, id))objc_msgSend)(
      gApplication, NSSelectorFromString(@"loadBrowserContext:type:completion:"), @"Default", 0, ^(id context) {
        if (!context) {
          gResult[@"failure"] = @"Default context did not load";
          gResult[@"complete"] = @YES;
          Write();
          return;
        }
        gContext = context;
        Import(request);
      });
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
  NSString* directory = [[NSProcessInfo processInfo] environment][@"ARC_SCENARIO_CHROMIUM_IMPORT_DIRECTORY"];
  if (!directory.length) return;
  gDirectory = directory;
  if (!CaptureApplication()) dispatch_async(dispatch_get_main_queue(), ^{ CaptureWhenLoaded(); });
  dispatch_async(dispatch_get_main_queue(), ^{ WaitForGo(); });
}
