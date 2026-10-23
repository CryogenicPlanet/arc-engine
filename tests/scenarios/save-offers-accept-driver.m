// Test-only driver: answers the app's save offers with -save, as a user clicking Save in Arc's prompt.
// Wraps ADK.WebContentChromium's didSubmitCredentials/AddressEntry/CreditCardInfo (or the recorder's aliases
// of them, so recording stays intact) and calls -save on the request after the app has seen it.
#import <AppKit/AppKit.h>
#import <objc/message.h>
#import <objc/runtime.h>

static NSString* gDirectory;
static NSMutableArray* gAnswered;

static void Note(NSString* what) {
  [gAnswered addObject:what];
  NSData* data = [NSJSONSerialization dataWithJSONObject:@{@"answered" : gAnswered} options:0 error:nil];
  [data writeToFile:[gDirectory stringByAppendingPathComponent:@"answered.json"] atomically:YES];
}

static BOOL Wrap(NSString* name, NSString* label) {
  Class cls = NSClassFromString(@"_TtC3ADK18WebContentChromium");
  if (!cls) return NO;
  Method method = class_getInstanceMethod(cls, NSSelectorFromString([@"__arcrec_" stringByAppendingString:name]))
                      ?: class_getInstanceMethod(cls, NSSelectorFromString(name));
  if (!method) return NO;
  IMP original = method_getImplementation(method);
  SEL selector = method_getName(method);
  method_setImplementation(method, imp_implementationWithBlock(^(id self, id contents, id request) {
    ((void (*)(id, SEL, id, id))original)(self, selector, contents, request);
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
      ((void (*)(id, SEL))objc_msgSend)(request, NSSelectorFromString(@"save"));
      Note(label);
    });
  }));
  return YES;
}

static void Install(void) {
  if (Wrap(@"webContents:didSubmitCredentials:", @"credentials")) {
    Wrap(@"webContents:didSubmitAddressEntry:", @"address");
    Wrap(@"webContents:didSubmitCreditCardInfo:", @"card");
    return;
  }
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.05 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
    Install();
  });
}

__attribute__((constructor)) static void Start(void) {
  NSString* directory = [[NSProcessInfo processInfo] environment][@"ARC_SCENARIO_SAVE_DIRECTORY"];
  if (!directory.length) return;
  gDirectory = directory;
  gAnswered = [NSMutableArray array];
  dispatch_async(dispatch_get_main_queue(), ^{ Install(); });
}
