// Public Foundation/libc policy probe inside the exact disposable agent PID.
// No engine hooks, UI, profile access, credentials, cookies or TLS overrides.
#import <Foundation/Foundation.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

static NSString *gDirectory, *gNonce, *gAgent, *gControlURL, *gLastID;
static NSMutableDictionary *gResults, *gCompletionCounts;
static NSURLSession *gSession;
static BOOL gInstalled, gStarted, gDNSDone, gProtocolInstalled;
static NSUInteger gCompletions;
static dispatch_source_t gTimer;
static NSString *const kBlockedHost = @"firestore.googleapis.com";
static NSString *const kBlockedURL = @"https://firestore.googleapis.com/arccore-disposable-probe";
static NSString *const kPolicyMessage = @"This service is disabled in ArcMod's local mode.";

static void Require(BOOL pass, NSString *reason) {
  if (!pass) @throw [NSException exceptionWithName:@"ScenarioNativeServices" reason:reason userInfo:nil];
}
static void WriteJSON(id object, NSString *path) {
  NSError *error = nil;
  NSData *data = [NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&error];
  if (!data || ![data writeToFile:path options:NSDataWritingAtomic error:&error]) NSLog(@"ScenarioNativeServices output failed: %@", error);
}
// All fields are cached. Polling never repeats DNS, HTTP or service requests.
static NSDictionary *State(void) {
  return @{@"started":@(gStarted), @"dnsDone":@(gDNSDone), @"completions":@(gCompletions),
    @"completionCounts":[gCompletionCounts copy], @"protocolInstalled":@(gProtocolInstalled), @"results":[gResults copy]};
}
static void StartDNS(void) {
  // A missing interposer must not block the application's main thread in DNS.
  dispatch_async(dispatch_get_global_queue(QOS_CLASS_UTILITY, 0), ^{
    @autoreleasepool {
      struct addrinfo hints = {0}; hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
      struct addrinfo *blocked = NULL;
      int blockedCode = getaddrinfo(kBlockedHost.UTF8String, "443", &hints, &blocked);
      BOOL blockedNull = blocked == NULL; if (blocked) freeaddrinfo(blocked);
      struct addrinfo numeric = {0}; numeric.ai_family = AF_INET; numeric.ai_socktype = SOCK_STREAM;
      numeric.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;
      struct addrinfo *control = NULL;
      int controlCode = getaddrinfo("127.0.0.1", "80", &numeric, &control);
      BOOL loopback = control && control->ai_family == AF_INET && control->ai_addrlen >= sizeof(struct sockaddr_in) &&
        ((struct sockaddr_in *)control->ai_addr)->sin_addr.s_addr == htonl(INADDR_LOOPBACK);
      if (control) freeaddrinfo(control);
      NSDictionary *result = @{@"blockedHost":kBlockedHost, @"blockedCode":@(blockedCode),
        @"blockedIsEAINONAME":@(blockedCode == EAI_NONAME), @"blockedReturnedNull":@(blockedNull),
        @"numericControlCode":@(controlCode), @"numericControlIsLoopback":@(loopback)};
      dispatch_async(dispatch_get_main_queue(), ^{ gResults[@"dns"] = result; gDNSDone = YES; });
    }
  });
}
static void StartHTTP(NSString *name, NSString *url) {
  NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]
    cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:5];
  request.HTTPShouldHandleCookies = NO;
  gCompletionCounts[name] = @0;
  NSURLSessionDataTask *task = [gSession dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
    BOOL main = NSThread.isMainThread;
    void (^finish)(void) = ^{
      ++gCompletions; gCompletionCounts[name] = @([gCompletionCounts[name] unsignedIntegerValue] + 1);
      NSInteger status = [response isKindOfClass:NSHTTPURLResponse.class] ? [(NSHTTPURLResponse *)response statusCode] : 0;
      NSString *text = data ? [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] : nil;
      gResults[name] = @{@"mainThread":@(main), @"errorDomain":error.domain ?: NSNull.null,
        @"errorCode":@(error ? error.code : 0), @"errorDescription":error.localizedDescription ?: NSNull.null,
        @"policyMessageMatches":@(error && [error.localizedDescription isEqual:kPolicyMessage]),
        @"hasResponse":@(response != nil), @"status":@(status), @"bytes":@(data.length),
        @"fixtureControlMatches":@(text && [text containsString:@"<title>launch-slice A</title>"] && [text containsString:@"fixture A"])};
      if (gCompletions == 2) [gSession finishTasksAndInvalidate];
    };
    if (main) finish(); else dispatch_async(dispatch_get_main_queue(), finish);
  }];
  [task resume];
}
static void Start(void) {
  Require(!gStarted, @"native policy probe already started; no request replay");
  gStarted = YES;
  NSURLSessionConfiguration *configuration = NSURLSessionConfiguration.ephemeralSessionConfiguration;
  configuration.HTTPCookieStorage = nil; configuration.URLCredentialStorage = nil; configuration.URLCache = nil;
  configuration.HTTPShouldSetCookies = NO; configuration.requestCachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
  configuration.timeoutIntervalForRequest = 5; configuration.timeoutIntervalForResource = 10;
  configuration.waitsForConnectivity = NO;
  // This exact public factory is intercepted by mods/local/ServicePolicy.m.
  gSession = [NSURLSession sessionWithConfiguration:configuration delegate:nil delegateQueue:NSOperationQueue.mainQueue];
  for (Class protocol in gSession.configuration.protocolClasses)
    if ([NSStringFromClass(protocol) isEqual:@"ArcModBlockedServiceProtocol"]) gProtocolInstalled = YES;
  StartDNS(); StartHTTP(@"blockedHTTP", kBlockedURL); StartHTTP(@"controlHTTP", gControlURL);
}
static void Poll(void) {
  if (!gInstalled) {
    gInstalled = YES;
    WriteJSON(@{@"pid":@(getpid()), @"nonce":gNonce, @"agent":gAgent, @"installed":@YES,
      @"blockedHost":kBlockedHost}, [gDirectory stringByAppendingPathComponent:@"ready.json"]);
  }
  NSData *data = [NSData dataWithContentsOfFile:[gDirectory stringByAppendingPathComponent:@"request.json"]];
  id decoded = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![decoded isKindOfClass:NSDictionary.class]) return;
  NSDictionary *request = decoded;
  NSString *rid = request[@"id"], *operation = request[@"operation"];
  if (![rid isKindOfClass:NSString.class] || [rid isEqual:gLastID] || ![operation isKindOfClass:NSString.class] ||
    ![request[@"nonce"] isEqual:gNonce] || ![request[@"pid"] isKindOfClass:NSNumber.class] || [request[@"pid"] longLongValue] != getpid() ||
    ![request[@"url"] isEqual:gControlURL]) return;
  gLastID = rid;
  NSMutableDictionary *response = [@{@"id":rid, @"operation":operation, @"pid":@(getpid()), @"nonce":gNonce,
    @"main":@(NSThread.isMainThread)} mutableCopy];
  @try {
    if ([operation isEqual:@"start"]) Start(); else Require([operation isEqual:@"status"], @"unsupported operation");
    response[@"ok"] = @YES;
  } @catch (NSException *exception) { response[@"error"] = exception.reason ?: exception.name; }
  response[@"state"] = State(); WriteJSON(response, [gDirectory stringByAppendingPathComponent:@"response.json"]);
}
__attribute__((constructor)) static void StartScenarioNativeServices(void) {
  @autoreleasepool {
    const char *directory = getenv("ARC_SCENARIO_SERVICES_DIRECTORY"), *nonce = getenv("ARC_SCENARIO_SERVICES_NONCE");
    const char *agent = getenv("ARC_SCENARIO_SERVICES_AGENT"), *control = getenv("ARC_SCENARIO_SERVICES_CONTROL_URL");
    if (!directory || !nonce || !agent || !control) return;
    NSString *slug = @(agent), *exe = NSProcessInfo.processInfo.arguments.firstObject;
    NSString *app = [[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    if (![@[@"trace156", @"ours156"] containsObject:slug] || ![exe.lastPathComponent isEqual:@"Arc"] ||
      ![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app", slug]] ||
      ![NSBundle.mainBundle.bundleIdentifier isEqual:[NSString stringWithFormat:@"company.thebrowser.Browser.mod.agent.%@", slug]]) return;
    NSURL *url = [NSURL URLWithString:@(control)];
    if (![@(directory) isAbsolutePath] || ![@(nonce) hasPrefix:@"arcmod-services-"] ||
      ![url.scheme isEqual:@"http"] || ![url.host isEqual:@"127.0.0.1"] || url.port.integerValue < 1024 ||
      url.user || url.password || url.query || url.fragment || ![url.path isEqual:@"/a"]) return;
    gDirectory = @(directory); gNonce = @(nonce); gAgent = slug; gControlURL = @(control);
    gResults = [NSMutableDictionary dictionary]; gCompletionCounts = [NSMutableDictionary dictionary];
    unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      gTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(gTimer, DISPATCH_TIME_NOW, 25 * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(gTimer, ^{ @autoreleasepool { Poll(); } }); dispatch_resume(gTimer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        dispatch_source_cancel(gTimer); if (gSession) [gSession invalidateAndCancel];
      });
    });
  }
}
