// Test-only local context-menu controls. No engine hooks, direct target/action
// dispatch, global AX/input, activation, private controllers or fabricated results.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

static NSString* gRequest;
static NSString* gNonce;
static NSString* gIsolationNonce;
static NSString* gIsolationPath;
static NSMutableSet<NSString*>* gSeenIDs;
static NSHashTable* gBaselineAXMenus;
static NSHashTable* gBaselineNativeMenus;
static NSHashTable* gBaselineWindows;
static BOOL gBaselineCaptured;
static BOOL gBaselineComplete;
static BOOL gBaselineApplicationChildrenAvailable;
static NSUInteger gBaselineOutsideMenus;
static NSUInteger gRequests;
static NSUInteger gAttempts;
static BOOL gEnabled;
static BOOL gBusy;
static NSTimeInterval gDeadline;
static const NSUInteger kLimit = 5000;
static const NSUInteger kDepth = 48;

@interface UIContextNode : NSObject
@property(nonatomic, strong) id object;
@property(nonatomic, copy) NSString* role;
@property(nonatomic, strong) NSMenu* nativeMenu;
@property(nonatomic, strong) NSMutableIndexSet* children;
@property(nonatomic) BOOL enabled;
@property(nonatomic) BOOL mainNative;
@property(nonatomic) BOOL underMenuBar;
@end
@implementation UIContextNode
@synthesize object = _object, role = _role, nativeMenu = _nativeMenu, children = _children;
@synthesize enabled = _enabled, mainNative = _mainNative, underMenuBar = _underMenuBar;
@end
@interface UIContextTree : NSObject
@property(nonatomic, strong) NSMutableArray<UIContextNode*>* nodes;
@property(nonatomic, strong) NSMapTable* indices;
@property(nonatomic, strong) NSHashTable* seen;
@property(nonatomic, strong) NSHashTable* nativeMenus;
@property(nonatomic, strong) NSHashTable* windows;
@property(nonatomic) BOOL truncated;
@property(nonatomic) NSUInteger excluded;
@property(nonatomic) NSUInteger edges;
@property(nonatomic) BOOL applicationChildrenAvailable;
@end
@implementation UIContextTree
@synthesize nodes = _nodes, indices = _indices, seen = _seen, nativeMenus = _nativeMenus, windows = _windows;
@synthesize truncated = _truncated, excluded = _excluded, edges = _edges;
@synthesize applicationChildrenAvailable = _applicationChildrenAvailable;
@end
@interface UIContextDiscovery : NSObject
@property(nonatomic, strong) UIContextTree* tree;
@property(nonatomic, strong) NSArray<NSNumber*>* menus;
@property(nonatomic, strong) NSArray<UIContextNode*>* reloads;
@property(nonatomic) NSUInteger mainExcluded;
@end
@implementation UIContextDiscovery
@synthesize tree = _tree, menus = _menus, reloads = _reloads, mainExcluded = _mainExcluded;
@end

static NSHashTable* Identities(void) {
  return [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
}
static NSData* ReadBounded(NSString* path) {
  if (!path) return nil;
  NSFileHandle* file = [NSFileHandle fileHandleForReadingAtPath:path]; if (!file) return nil;
  NSData* bytes = [file readDataUpToLength:4097 error:NULL]; [file closeAndReturnError:NULL];
  return bytes.length && bytes.length <= 4096 ? bytes : nil;
}
static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  if (bytes) [bytes writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static BOOL Isolated(void) {
  if (!gIsolationNonce.length) return NO;
  NSData* bytes = ReadBounded(gIsolationPath); if (!bytes) return NO;
  id marker = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  return [marker isKindOfClass:NSDictionary.class] && [marker[@"nonce"] isEqual:gIsolationNonce];
}
static BOOL Causal(NSDictionary* request) {
  id value = request[@"causalFixtureVerified"];
  return [value isKindOfClass:NSNumber.class] && CFGetTypeID((__bridge CFTypeRef)value) == CFBooleanGetTypeID() &&
      [value boolValue] && gIsolationNonce.length && [request[@"isolatedProfileNonce"] isEqual:gIsolationNonce];
}
static NSString* StringGetter(id object, SEL selector) {
  if (![object respondsToSelector:selector]) return nil;
  id value = [object valueForKey:NSStringFromSelector(selector)];
  return [value isKindOfClass:NSString.class] ? value : nil;
}
static BOOL Excluded(id object) {
  NSString* name = NSStringFromClass(object_getClass(object));
  if ([name containsString:@"NSAccessibilityRemote"] || [name containsString:@"NSRemoteView"] ||
      [name containsString:@"WebContents"] || [name containsString:@"RenderWidgetHost"] ||
      [name containsString:@"BrowserAccessibility"]) return YES;
  return [object isKindOfClass:NSSecureTextField.class] ||
      ([object isKindOfClass:NSTextField.class] && ([(NSTextField*)object isEditable] ||
        [[(NSTextField*)object cell] isKindOfClass:NSSecureTextFieldCell.class])) ||
      ([object isKindOfClass:NSTextView.class] && [(NSTextView*)object isEditable]);
}
static BOOL MainMenuAncestor(NSMenu* menu, BOOL* incomplete) {
  NSHashTable* seen = Identities();
  for (NSUInteger depth = 0; menu; ++depth, menu = menu.supermenu) {
    if (depth > kDepth || [seen containsObject:menu]) { *incomplete = YES; return YES; }
    [seen addObject:menu]; if (menu == NSApp.mainMenu) return YES;
  }
  return NO;
}
static void AddEdge(UIContextTree* tree, NSInteger parent, NSUInteger child) {
  if (parent < 0 || [tree.nodes[(NSUInteger)parent].children containsIndex:child]) return;
  if (tree.edges >= 10000) { tree.truncated = YES; return; }
  ++tree.edges; [tree.nodes[(NSUInteger)parent].children addIndex:child];
}
static void Visit(id object, NSInteger parent, NSUInteger depth, UIContextTree* tree) {
  if (!object) return;
  NSNumber* known = [tree.indices objectForKey:object];
  if (known) { AddEdge(tree, parent, known.unsignedIntegerValue); return; }
  if ([tree.seen containsObject:object]) return;
  if (tree.seen.count >= kLimit || depth > kDepth) { tree.truncated = YES; return; }
  [tree.seen addObject:object];
  if (Excluded(object)) { ++tree.excluded; return; }
  UIContextNode* node = [UIContextNode new]; node.object = object; node.children = [NSMutableIndexSet indexSet];
  @try {
    node.role = StringGetter(object, @selector(accessibilityRole));
    if ([node.role isEqual:NSAccessibilityWebAreaRole] || [node.role isEqual:NSAccessibilityTextFieldRole] ||
        [node.role isEqual:NSAccessibilityTextAreaRole]) { ++tree.excluded; return; }
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return;
    if ([object isKindOfClass:NSWindow.class]) {
      if (![(NSWindow*)object isVisible]) return; [tree.windows addObject:object];
    }
    if ([object isKindOfClass:NSView.class] && [(NSView*)object isHiddenOrHasHiddenAncestor]) return;
    if ([object isKindOfClass:NSMenuItem.class] && [(NSMenuItem*)object isHidden]) return;
    if ([object isKindOfClass:NSMenu.class]) node.nativeMenu = object;
    else if ([object isKindOfClass:NSMenuItem.class]) node.nativeMenu = [(NSMenuItem*)object menu];
    if (node.nativeMenu) {
      [tree.nativeMenus addObject:node.nativeMenu];
      BOOL incomplete = NO; node.mainNative = MainMenuAncestor(node.nativeMenu, &incomplete); tree.truncated |= incomplete;
    }
    node.enabled = [object respondsToSelector:@selector(isAccessibilityEnabled)] && [(id<NSAccessibility>)object isAccessibilityEnabled];
    if ([object isKindOfClass:NSMenuItem.class]) node.enabled &= [(NSMenuItem*)object isEnabled];
    node.underMenuBar = node.mainNative || [node.role isEqual:NSAccessibilityMenuBarRole];
    NSUInteger index = tree.nodes.count; [tree.nodes addObject:node]; [tree.indices setObject:@(index) forKey:object];
    AddEdge(tree, parent, index);
    // Role and ownership metadata only. Labels are read later, exclusively for
    // eligible menu items within proved new non-menubar AXMenu containers.
    if ([object respondsToSelector:@selector(accessibilityChildren)]) {
      id children = [(id<NSAccessibility>)object accessibilityChildren];
      if ([children isKindOfClass:NSArray.class]) {
        if (object == NSApp && [children count] > 0) tree.applicationChildrenAvailable = YES;
        NSUInteger count = 0;
        for (id child in children) {
          if (++count > kLimit || tree.truncated) { tree.truncated = YES; break; }
          Visit(child, (NSInteger)index, depth + 1, tree);
        }
      }
    }
    if ([object isKindOfClass:NSWindow.class]) Visit([(NSWindow*)object contentView], (NSInteger)index, depth + 1, tree);
    else if ([object isKindOfClass:NSView.class]) {
      NSUInteger count = 0;
      for (NSView* child in [[(NSView*)object subviews] copy]) {
        if (++count > kLimit || tree.truncated) { tree.truncated = YES; break; }
        Visit(child, (NSInteger)index, depth + 1, tree);
      }
    }
  } @catch (NSException* exception) { (void)exception; tree.truncated = YES; }
}
static UIContextTree* Tree(void) {
  UIContextTree* tree = [UIContextTree new]; tree.nodes = [NSMutableArray array]; tree.seen = Identities();
  tree.indices = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsStrongMemory];
  tree.nativeMenus = Identities(); tree.windows = Identities();
  if (NSApp.mainMenu) [tree.nativeMenus addObject:NSApp.mainMenu];
  Visit(NSApp, -1, 0, tree);
  NSUInteger count = 0;
  for (NSWindow* window in [NSApp.windows copy]) {
    if (++count > 64) { tree.truncated = YES; break; }
    if (window.visible) Visit(window, -1, 0, tree);
  }
  // A shared object can be exposed through multiple AX paths. Propagate menu
  // bar ancestry through every recorded edge, never only the first parent.
  NSMutableIndexSet* pending = [NSMutableIndexSet indexSet];
  for (NSUInteger index = 0; index < tree.nodes.count; ++index) if (tree.nodes[index].underMenuBar) [pending addIndex:index];
  NSMutableIndexSet* processed = [NSMutableIndexSet indexSet];
  while (pending.count) {
    NSUInteger index = pending.firstIndex; [pending removeIndex:index]; if ([processed containsIndex:index]) continue;
    [processed addIndex:index]; tree.nodes[index].underMenuBar = YES;
    [tree.nodes[index].children enumerateIndexesUsingBlock:^(NSUInteger child, BOOL* stop) { (void)stop; if (![processed containsIndex:child]) [pending addIndex:child]; }];
  }
  return tree;
}
static NSIndexSet* Descendants(UIContextTree* tree, NSUInteger root) {
  NSMutableIndexSet* found = [NSMutableIndexSet indexSet], *pending = [NSMutableIndexSet indexSetWithIndex:root];
  while (pending.count) {
    NSUInteger index = pending.firstIndex; [pending removeIndex:index]; if ([found containsIndex:index]) continue;
    [found addIndex:index];
    [tree.nodes[index].children enumerateIndexesUsingBlock:^(NSUInteger child, BOOL* stop) { (void)stop; if (![found containsIndex:child]) [pending addIndex:child]; }];
  }
  return found;
}
static BOOL IsReload(UIContextNode* node) {
  id object = node.object;
  if (!node.enabled || node.underMenuBar || node.mainNative ||
      !([object isKindOfClass:NSMenuItem.class] || [node.role isEqual:NSAccessibilityMenuItemRole]) ||
      ![object respondsToSelector:@selector(accessibilityPerformPress)]) return NO;
  @try {
    if ([object isKindOfClass:NSMenuItem.class]) {
      NSMenu* menu = [(NSMenuItem*)object menu]; BOOL incomplete = NO;
      return menu && !MainMenuAncestor(menu, &incomplete) && !incomplete &&
          (!gBaselineCaptured || ![gBaselineNativeMenus containsObject:menu]) && [[(NSMenuItem*)object title] isEqual:@"Reload"];
    }
    return [StringGetter(object, @selector(accessibilityLabel)) isEqual:@"Reload"] || [StringGetter(object, @selector(accessibilityTitle)) isEqual:@"Reload"];
  } @catch (NSException* exception) { (void)exception; return NO; }
}
static UIContextDiscovery* Discover(UIContextTree* tree, BOOL requireNew) {
  UIContextDiscovery* result = [UIContextDiscovery new]; result.tree = tree;
  NSMutableArray<NSNumber*>* menus = [NSMutableArray array]; NSHashTable* reloadObjects = Identities();
  NSMutableArray<UIContextNode*>* reloads = [NSMutableArray array];
  for (NSUInteger index = 0; index < tree.nodes.count; ++index) {
    UIContextNode* menu = tree.nodes[index];
    if (![menu.role isEqual:NSAccessibilityMenuRole]) continue;
    if (menu.underMenuBar || menu.mainNative) { ++result.mainExcluded; continue; }
    if (requireNew && (!gBaselineCaptured || [gBaselineAXMenus containsObject:menu.object] ||
        (menu.nativeMenu && [gBaselineNativeMenus containsObject:menu.nativeMenu]))) continue;
    NSIndexSet* children = Descendants(tree, index);
    __block BOOL containsMain = NO;
    [children enumerateIndexesUsingBlock:^(NSUInteger child, BOOL* stop) {
      if (tree.nodes[child].mainNative || tree.nodes[child].underMenuBar) { containsMain = YES; *stop = YES; }
    }];
    if (containsMain) { ++result.mainExcluded; continue; }
    [menus addObject:@(index)];
    [children enumerateIndexesUsingBlock:^(NSUInteger child, BOOL* stop) {
      (void)stop; UIContextNode* node = tree.nodes[child];
      if (requireNew && IsReload(node) && ![reloadObjects containsObject:node.object]) { [reloadObjects addObject:node.object]; [reloads addObject:node]; }
    }];
  }
  // Prefer the public concrete NSMenuItem when the unique observed control has
  // that representation. Distinct Reload objects remain ambiguous, not aliases.
  result.menus = menus; result.reloads = reloads; return result;
}
static NSDictionary* Counts(UIContextDiscovery* discovery) {
  NSUInteger newWindows = 0;
  if (gBaselineCaptured) for (NSWindow* window in discovery.tree.windows) if (![gBaselineWindows containsObject:window]) ++newWindows;
  return @{@"baselineCaptured":@(gBaselineCaptured), @"baselineComplete":@(gBaselineComplete),
      @"baselineOutsideMenuCount":@(gBaselineOutsideMenus), @"newContextMenuCount":@(discovery.menus.count),
      @"reloadControlCount":@(discovery.reloads.count), @"mainMenuExcludedCount":@(discovery.mainExcluded),
      @"newNativeWindowCount":@(newWindows), @"nodeCount":@(discovery.tree.nodes.count),
      @"truncated":@(discovery.tree.truncated), @"applicationChildrenAvailable":@(discovery.tree.applicationChildrenAvailable),
      @"excludedRemoteRendererOrPrivateCount":@(discovery.tree.excluded),
      @"causalProfileIsolated":@(Isolated()), @"actionAttemptCount":@(gAttempts), @"nativeActionsPerformed":@(gAttempts),
      @"contextMenuDismissed":@(gAttempts == 1 && gBaselineComplete && gBaselineApplicationChildrenAvailable &&
          discovery.tree.applicationChildrenAvailable && !discovery.tree.truncated && discovery.menus.count == 0 && discovery.reloads.count == 0)};
}
static BOOL LiveReload(UIContextNode* node) {
  id object = node.object;
  @try {
    if (Excluded(object) || ![object respondsToSelector:@selector(isAccessibilityEnabled)] || ![(id<NSAccessibility>)object isAccessibilityEnabled]) return NO;
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return NO;
    if ([object isKindOfClass:NSMenuItem.class]) return [(NSMenuItem*)object isEnabled] && ![(NSMenuItem*)object isHidden] && IsReload(node);
    return [StringGetter(object, @selector(accessibilityRole)) isEqual:NSAccessibilityMenuItemRole] && IsReload(node);
  } @catch (NSException* exception) { (void)exception; return NO; }
}
static void HandleRequest(NSDictionary* request) {
  gBusy = YES; NSString* identifier = request[@"id"], *operation = request[@"operation"];
  UIContextTree* tree = Tree();
  UIContextDiscovery* discovery = Discover(tree, ![operation isEqual:@"snapshot-baseline"]);
  NSMutableDictionary* response = [@{@"id":identifier, @"nonce":gNonce, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
      @"mainThread":@(NSThread.isMainThread), @"operation":operation, @"functionalAcceptance":@NO,
      @"actionAttempted":@NO, @"nativeMethodReturned":@NO, @"nativeActionKind":NSNull.null,
      @"accessibilityPressReturn":NSNull.null, @"causalFixtureVerified":@(Causal(request)),
      @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"ownershipBasis":@"Fresh profile isolation and driver-exclusive fixture target, plus new local non-menubar menu identities since a complete pre-click baseline. Exact page-to-native-menu mapping is not claimed.",
      @"interpretation":@"A public native menu accessibility press is an attempt, even when it returns false. The driver must independently verify actual page reload and menu dismissal; this helper never claims functional acceptance."} mutableCopy];
  NSString* refusal = nil;
  if ([operation isEqual:@"snapshot-baseline"]) {
    if (gBaselineCaptured) refusal = @"baseline has already been captured; it cannot be overwritten";
    else {
      gBaselineCaptured = YES; gBaselineApplicationChildrenAvailable = tree.applicationChildrenAvailable;
      gBaselineComplete = !tree.truncated && gBaselineApplicationChildrenAvailable; gBaselineOutsideMenus = discovery.menus.count;
      gBaselineAXMenus = Identities(); gBaselineNativeMenus = Identities(); gBaselineWindows = Identities();
      for (UIContextNode* node in tree.nodes) if ([node.role isEqual:NSAccessibilityMenuRole]) [gBaselineAXMenus addObject:node.object];
      for (NSMenu* menu in tree.nativeMenus) [gBaselineNativeMenus addObject:menu];
      for (NSWindow* window in tree.windows) [gBaselineWindows addObject:window];
      response[@"baselineNativeMenuCount"] = @(gBaselineNativeMenus.count);
      response[@"baselineNativeWindowCount"] = @(gBaselineWindows.count); response[@"baselineAXMenuCount"] = @(gBaselineAXMenus.count);
      discovery = Discover(tree, YES);
    }
  }
  NSDictionary* before = Counts(discovery); [response addEntriesFromDictionary:before]; response[@"before"] = before;
  if ([operation isEqual:@"press-reload"]) {
    if (!gEnabled) refusal = @"explicit canonical-context-v1 launch opt-in is absent";
    else if (!Isolated() || !Causal(request)) refusal = @"fresh isolated profile and driver fixture verification are required";
    else if (!gBaselineCaptured || !gBaselineComplete || gBaselineOutsideMenus != 0) refusal = @"a complete empty pre-click context-menu baseline is required";
    else if (tree.truncated) refusal = @"native tree limit or inspection exception makes scope incomplete";
    else if (gAttempts) refusal = @"the one native menu invocation budget is consumed";
    else if (discovery.menus.count != 1 || discovery.reloads.count != 1) refusal = @"new non-menubar context menu has no unique enabled Reload control";
    UIContextNode* reload = discovery.reloads.count == 1 ? discovery.reloads.firstObject : nil;
    if (!refusal && (!LiveReload(reload) || !Isolated() || !Causal(request))) refusal = @"native Reload or causal scope changed before invocation";
    if (!refusal) {
      ++gAttempts; response[@"actionAttempted"] = @YES; response[@"actionAttemptCount"] = @(gAttempts); response[@"nativeActionsPerformed"] = @(gAttempts);
      response[@"nativeActionKind"] = [reload.object isKindOfClass:NSMenuItem.class] ? @"NSMenuItem.accessibilityPerformPress" : @"NSAccessibilityMenuItem.accessibilityPerformPress";
      WriteJSON(response, [gRequest stringByAppendingString:@".attempt.json"]);
      @try { response[@"accessibilityPressReturn"] = @([(id<NSAccessibility>)reload.object accessibilityPerformPress]); response[@"nativeMethodReturned"] = @YES; }
      @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
    }
  }
  if (refusal) response[@"refusalReason"] = refusal;
  // Keep all polling and reply work in the common-mode NSTimer callback; native
  // menu tracking need not service the GCD main queue until the menu closes.
  UIContextDiscovery* after = Discover(Tree(), YES);
  response[@"after"] = Counts(after);
  response[@"actionAttemptCount"] = @(gAttempts); response[@"nativeActionsPerformed"] = @(gAttempts);
  response[@"contextMenuDismissed"] = Counts(after)[@"contextMenuDismissed"];
  response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
}
static void Poll(void) {
  if (!NSThread.isMainThread || gBusy || gRequests >= 48) return;
  NSData* bytes = ReadBounded(gRequest); if (!bytes) return;
  id request = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL]; if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  if (![identifier isKindOfClass:NSString.class] || identifier.length > 160 ||
      ![identifier hasPrefix:[gNonce stringByAppendingString:@"-"]] || [gSeenIDs containsObject:identifier] ||
      ![request[@"nonce"] isEqual:gNonce] || ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)] ||
      ![@[@"snapshot-baseline", @"inventory", @"press-reload", @"inspect-dismissed"] containsObject:request[@"operation"]]) return;
  [gSeenIDs addObject:identifier]; ++gRequests; HandleRequest(request);
}
__attribute__((constructor)) static void StartUIContextMenuControlsProbe(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    NSString* slug = [app isEqual:@"ArcModAgent-trace156.app"] ? @"trace156" : ([app isEqual:@"ArcModAgent-ours156.app"] ? @"ours156" : nil);
    if (!slug || ![executable.lastPathComponent isEqual:@"Arc"] ||
        ![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]]) return;
    const char* path = getenv("ARC_UI_CONTEXT_REQUEST"), *nonce = getenv("ARC_UI_CONTEXT_NONCE"), *authority = getenv("ARC_UI_CONTEXT_FIXTURE_AUTHORITY");
    const char* enabled = getenv("ARC_UI_CONTEXT_ACTION_ENABLE"), *isolation = getenv("ARC_UI_PROFILE_ISOLATION_NONCE");
    if (!path || !nonce || !authority) return;
    gRequest = @(path); gNonce = @(nonce); NSString* hostPort = @(authority);
    NSURL* parsed = [NSURL URLWithString:[@"http://" stringByAppendingString:hostPort]]; NSInteger port = parsed.port.integerValue;
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"ui-context-request.json"] ||
        ![gNonce hasPrefix:@"arc-ui-context-"] || gNonce.length > 96 || ![parsed.host isEqual:@"127.0.0.1"] ||
        port < 1024 || port > 65535 || ![hostPort isEqual:[NSString stringWithFormat:@"127.0.0.1:%ld", (long)port]]) return;
    gEnabled = enabled && [@(enabled) isEqual:@"canonical-context-v1"];
    if (isolation && @(isolation).length && @(isolation).length <= 160) gIsolationNonce = @(isolation);
    gIsolationPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Library/Application Support"] stringByAppendingPathComponent:[NSString stringWithFormat:@"ArcModAgent-%@/.arc-ui-probe-profile.json", slug]];
    gSeenIDs = [NSMutableSet set]; unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"nonce":gNonce,
          @"mainThread":@(NSThread.isMainThread), @"actionCapabilityEnabled":@(gEnabled), @"causalProfileIsolated":@(Isolated()),
          @"baselineCaptured":@NO, @"actionAttemptBudget":@1, @"actionAttemptCount":@0, @"nativeActionsPerformed":@0,
          @"functionalAcceptance":@NO, @"pollingMode":@"NSRunLoopCommonModes NSTimer"}, [gRequest stringByAppendingString:@".ready.json"]);
      gDeadline = NSProcessInfo.processInfo.systemUptime + 100;
      NSTimer* timer = [NSTimer timerWithTimeInterval:0.25 repeats:YES block:^(NSTimer* firing) {
        if (NSProcessInfo.processInfo.systemUptime >= gDeadline) { [firing invalidate]; return; }
        @autoreleasepool { Poll(); }
      }];
      [NSRunLoop.mainRunLoop addTimer:timer forMode:NSRunLoopCommonModes];
    });
  }
}
