// App-event shortcut routing only. Events remain inside this disposable PID;
// no CGEvent posting, WindowServer injection, activation or private selectors.
#import <AppKit/AppKit.h>
#include <stdlib.h>
#include <unistd.h>

static NSString *gDirectory, *gNonce, *gAgent, *gFixtureURL, *gLastID;
static id gMonitor;
static NSEvent *gDown, *gUp;
static BOOL gDispatching, gDownSeen, gUpSeen, gInstalled;
static NSUInteger gDispatchCount;
static dispatch_source_t gTimer;

static void Require(BOOL pass, NSString *reason) {
  if (!pass) @throw [NSException exceptionWithName:@"ScenarioKeyEvents" reason:reason userInfo:nil];
}
static void WriteJSON(id object, NSString *path) {
  NSError *error = nil;
  NSData *data = [NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted | NSJSONWritingSortedKeys error:&error];
  if (!data || ![data writeToFile:path options:NSDataWritingAtomic error:&error]) NSLog(@"ScenarioKeyEvents output failed: %@", error);
}
static NSDictionary *Shortcuts(void) {
  NSEventModifierFlags command = NSEventModifierFlagCommand;
  // The seven cmd-* entries are default menu equivalents in shortcut-menu-inventory.json.
  // The virtual key codes describe a US ANSI layout; no hardware/layout coverage.
  return @{
    @"cmd-t":@{@"characters":@"t", @"keyCode":@17, @"flags":@(command)},
    @"cmd-l":@{@"characters":@"l", @"keyCode":@37, @"flags":@(command)},
    @"cmd-comma":@{@"characters":@",", @"keyCode":@43, @"flags":@(command)},
    @"cmd-w":@{@"characters":@"w", @"keyCode":@13, @"flags":@(command)},
    @"cmd-shift-t":@{@"characters":@"T", @"keyCode":@17, @"flags":@(command | NSEventModifierFlagShift)},
    @"cmd-n":@{@"characters":@"n", @"keyCode":@45, @"flags":@(command)},
    @"cmd-option-n":@{@"characters":@"n", @"keyCode":@45, @"flags":@(command | NSEventModifierFlagOption)},
    // Not a shortcut: Return commits the command bar. Arc's command bar field
    // advertises AXConfirm but does not act on it (no navigation, no new tab),
    // so the scenario confirms it the way a person does, through this route.
    @"return":@{@"characters":@"\r", @"keyCode":@36, @"flags":@0}
  };
}
static void Install(void) {
  // Observe only the exact synthetic objects currently sent by this driver.
  // Other local keyboard events pass through without inspection or logging.
  gMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown | NSEventMaskKeyUp handler:^NSEvent *(NSEvent *event) {
    if (gDispatching && event == gDown) gDownSeen = YES;
    if (gDispatching && event == gUp) gUpSeen = YES;
    return event;
  }];
  gInstalled = YES;
  WriteJSON(@{@"pid":@(getpid()), @"nonce":gNonce, @"agent":gAgent, @"installed":@YES,
    @"localMonitorInstalled":@((BOOL)(gMonitor != nil)), @"scope":@"in-process NSApplication.sendEvent; no OS hardware delivery"},
    [gDirectory stringByAppendingPathComponent:@"ready.json"]);
}
static NSWindow *ObservedWindow(NSString *identifier) {
  Require([identifier isKindOfClass:NSString.class] && [identifier hasPrefix:@"bigBrowserWindow-"], @"exact observed full-browser identifier required");
  NSWindow *found = nil; NSUInteger count = 0;
  for (NSWindow *window in NSApp.windows) {
    if ([window.identifier isEqual:identifier] || [window.accessibilityIdentifier isEqual:identifier]) { found = window; ++count; }
  }
  Require(count == 1 && found.windowNumber > 0 && found.contentView.window == found, @"native window is absent or ambiguous");
  Require(NSApp.keyWindow == found && found.isKeyWindow, @"observed agent window is not key; no event dispatched");
  return found;
}
static NSDictionary *Dispatch(NSString *shortcut, NSString *identifier) {
  NSDictionary *spec = Shortcuts()[shortcut]; Require(spec != nil, @"shortcut is outside fixed whitelist");
  NSWindow *window = ObservedWindow(identifier);
  NSInteger number = window.windowNumber;
  NSTimeInterval timestamp = NSProcessInfo.processInfo.systemUptime;
  NSEventModifierFlags flags = [spec[@"flags"] unsignedIntegerValue];
  NSString *characters = spec[@"characters"];
  unsigned short keyCode = [spec[@"keyCode"] unsignedShortValue];
  // charactersIgnoringModifiers retains Shift, as NSEvent's contract specifies.
  gDown = [NSEvent keyEventWithType:NSEventTypeKeyDown location:NSZeroPoint modifierFlags:flags timestamp:timestamp
    windowNumber:number context:nil characters:characters charactersIgnoringModifiers:characters isARepeat:NO keyCode:keyCode];
  gUp = [NSEvent keyEventWithType:NSEventTypeKeyUp location:NSZeroPoint modifierFlags:flags timestamp:timestamp + 0.001
    windowNumber:number context:nil characters:characters charactersIgnoringModifiers:characters isARepeat:NO keyCode:keyCode];
  Require(gDown && gUp && gDown.window == window && gUp.window == window, @"could not construct exact-window local events");
  BOOL activeBefore = NSApp.isActive;
  NSString *responder = window.firstResponder ? NSStringFromClass(window.firstResponder.class) : @"(none)";
  gDownSeen = NO; gUpSeen = NO; gDispatching = YES;
  BOOL downReturned = NO, upReturned = NO;
  @try {
    ++gDispatchCount;
    [NSApp sendEvent:gDown]; downReturned = YES;
    [NSApp sendEvent:gUp]; upReturned = YES;
  } @finally { gDispatching = NO; }
  NSDictionary *receipt = @{@"pid":@(getpid()), @"sequence":@(gDispatchCount), @"shortcut":shortcut,
    @"windowIdentifier":identifier, @"windowNumber":@(number), @"keyCode":@(keyCode), @"modifierFlags":@(flags),
    @"mainThread":@(NSThread.isMainThread), @"keyWindowMatchedBefore":@YES,
    @"downReturned":@(downReturned), @"upReturned":@(upReturned),
    @"localMonitorInstalled":@((BOOL)(gMonitor != nil)), @"localMonitorSawDown":@(gDownSeen), @"localMonitorSawUp":@(gUpSeen),
    @"appActiveBefore":@(activeBefore), @"appActiveAfter":@(NSApp.isActive), @"firstResponderBefore":responder,
    @"scope":@"app-event routing; not WindowServer/hardware delivery"};
  gDown = nil; gUp = nil;
  return receipt;
}
static void Poll(void) {
  if (!gInstalled) { if (!NSApp || !NSApp.mainMenu) return; Install(); }
  NSData *data = [NSData dataWithContentsOfFile:[gDirectory stringByAppendingPathComponent:@"request.json"]];
  id decoded = data ? [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL] : nil;
  if (![decoded isKindOfClass:NSDictionary.class]) return;
  NSDictionary *request = decoded;
  NSString *rid = request[@"id"], *shortcut = request[@"shortcut"], *window = request[@"windowIdentifier"];
  if (![rid isKindOfClass:NSString.class] || [rid isEqual:gLastID] || ![request[@"nonce"] isEqual:gNonce] ||
    ![request[@"pid"] isKindOfClass:NSNumber.class] || [request[@"pid"] longLongValue] != getpid() ||
    ![request[@"url"] isEqual:gFixtureURL] || ![shortcut isKindOfClass:NSString.class] || ![window isKindOfClass:NSString.class]) return;
  gLastID = rid; // Never replay an uncertain or failed native input.
  NSMutableDictionary *response = [@{@"id":rid, @"pid":@(getpid()), @"nonce":gNonce, @"shortcut":shortcut} mutableCopy];
  @try { response[@"receipt"] = Dispatch(shortcut, window); response[@"ok"] = @YES; }
  @catch (NSException *exception) { response[@"error"] = exception.reason ?: exception.name; }
  WriteJSON(response, [gDirectory stringByAppendingPathComponent:@"response.json"]);
}
__attribute__((constructor)) static void StartScenarioKeyEvents(void) {
  @autoreleasepool {
    const char *directory = getenv("ARC_SCENARIO_KEYS_DIRECTORY"), *nonce = getenv("ARC_SCENARIO_KEYS_NONCE");
    const char *agent = getenv("ARC_SCENARIO_KEYS_AGENT"), *fixture = getenv("ARC_SCENARIO_KEYS_URL");
    if (!directory || !nonce || !agent || !fixture) return;
    NSString *slug = @(agent), *exe = NSProcessInfo.processInfo.arguments.firstObject;
    NSString *app = [[[exe stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    if (![@[@"trace156", @"ours156"] containsObject:slug] || ![exe.lastPathComponent isEqual:@"Arc"] ||
      ![app.lastPathComponent isEqual:[NSString stringWithFormat:@"ArcModAgent-%@.app", slug]] ||
      ![NSBundle.mainBundle.bundleIdentifier isEqual:[NSString stringWithFormat:@"company.thebrowser.Browser.mod.agent.%@", slug]]) return;
    NSURL *url = [NSURL URLWithString:@(fixture)];
    if (![@(directory) isAbsolutePath] || ![@(nonce) hasPrefix:@"arcmod-keys-"] ||
      ![url.scheme isEqual:@"http"] || ![url.host isEqual:@"127.0.0.1"] || url.port.integerValue < 1024 ||
      url.user || url.password || url.query || url.fragment || ![url.path isEqual:@"/keys-target"]) return;
    gDirectory = @(directory); gNonce = @(nonce); gAgent = slug; gFixtureURL = @(fixture);
    unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      gTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(gTimer, DISPATCH_TIME_NOW, 25 * NSEC_PER_MSEC, 5 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(gTimer, ^{ @autoreleasepool { Poll(); } }); dispatch_resume(gTimer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        dispatch_source_cancel(gTimer); if (gMonitor) { [NSEvent removeMonitor:gMonitor]; gMonitor = nil; }
      });
    });
  }
}
