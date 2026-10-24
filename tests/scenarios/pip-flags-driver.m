// Test-only driver for Arc's picture-in-picture policies (ArcWebContents skipUserActivationForPiP: and
// ignoreDisablePiPAttribute:). It remembers the ArcWebContents the app reads URLs from (through the recorder's
// alias, so recording stays intact), then serves the scenario's numbered commands: <dir>/cmd-N.json
// {"selector", "value", "url"} calls the selector on the tab at that URL and writes <dir>/done-N.json.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSString* gDirectory;
static NSHashTable* gContents;
static NSInteger gNext = 1;

static void After(double seconds, dispatch_block_t block) {
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC)), dispatch_get_main_queue(), block);
}

static void Reply(NSInteger number, NSDictionary* reply) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:reply options:NSJSONWritingSortedKeys error:nil];
  [data writeToFile:[gDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"done-%ld.json", (long)number]]
         atomically:YES];
}

static void Serve(void) {
  NSString* path = [gDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"cmd-%ld.json", (long)gNext]];
  NSData* data = [NSData dataWithContentsOfFile:path];
  NSDictionary* command = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:nil] : nil;
  if ([command isKindOfClass:[NSDictionary class]]) {
    const NSInteger number = gNext++;
    id target = nil;
    NSInteger matches = 0;
    for (id contents in gContents) {
      NSURL* url = ((id(*)(id, SEL))objc_msgSend)(contents, NSSelectorFromString(@"url"));
      if ([url.absoluteString isEqual:command[@"url"]]) {
        target = contents;
        ++matches;
      }
    }
    SEL selector = NSSelectorFromString(command[@"selector"]);
    if (matches == 1 && [target respondsToSelector:selector]) {
      ((void (*)(id, SEL, BOOL))objc_msgSend)(target, selector, [command[@"value"] boolValue]);
      Reply(number, @{@"ok" : @YES});
    } else {
      Reply(number, @{@"ok" : @NO, @"matches" : @(matches), @"known" : @(gContents.count)});
    }
  }
  After(0.1, ^{ Serve(); });
}

// Wrap ArcWebContents' url getter (or the recorder's alias of it) to learn the live wrappers.
static BOOL CaptureContents(void) {
  Class cls = NSClassFromString(@"ArcWebContents");
  if (!cls) return NO;
  SEL alias = NSSelectorFromString(@"__arcrec_url");
  SEL plain = NSSelectorFromString(@"url");
  Method method = class_getInstanceMethod(cls, alias) ?: class_getInstanceMethod(cls, plain);
  if (!method) return NO;
  IMP original = method_getImplementation(method);
  SEL selector = method_getName(method);
  method_setImplementation(method, imp_implementationWithBlock(^id(id self) {
    [gContents addObject:self];
    return ((id(*)(id, SEL))original)(self, selector);
  }));
  return YES;
}

static void CaptureWhenLoaded(void) {
  if (!CaptureContents()) After(0.05, ^{ CaptureWhenLoaded(); });
}

__attribute__((constructor)) static void Start(void) {
  NSString* directory = [[NSProcessInfo processInfo] environment][@"ARC_SCENARIO_PIP_FLAGS_DIRECTORY"];
  if (!directory.length) return;
  gDirectory = directory;
  gContents = [NSHashTable weakObjectsHashTable];
  if (!CaptureContents()) dispatch_async(dispatch_get_main_queue(), ^{ CaptureWhenLoaded(); });
  dispatch_async(dispatch_get_main_queue(), ^{ Serve(); });
}
