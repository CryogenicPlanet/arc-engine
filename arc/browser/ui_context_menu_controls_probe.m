// Test-only local context-menu controls. No engine hooks, direct target/action
// dispatch, global AX/input, activation, private controllers or fabricated results.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>
#import <stdatomic.h>

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
static NSMapTable* gTrackedMenus;
static NSArray* gTrackingObservers;
static BOOL gTrackingComplete = YES;
static atomic_bool gTrackingOffMain;
static NSUInteger gTrackingEvents;
static NSUInteger gTrackingBegins;
static NSUInteger gTrackingEnds;
static NSUInteger gTick;
static NSUInteger gBaselineTrackingSequence;
static NSUInteger gBaselineTrackingBegins;
static NSUInteger gBaselineTrackingMenuCount;
static NSUInteger gAttemptSequence;
static NSUInteger gAttemptBeginSequence;
static NSUInteger gMatchingEndSequence;
static NSMenu* gAttemptMenu;
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
// The notification object is the actual public NSMenu (also used by
// ui/base/test/menu_test_observer.mm). Notifications record identity/lifecycle
// only: the menu may not be tracking-ready inside its begin callback.
@interface UIContextTracking : NSObject
@property(nonatomic, strong) NSMenu* menu;
@property(nonatomic) BOOL active;
@property(nonatomic) NSUInteger beginSequence;
@property(nonatomic) NSUInteger beginTick;
@end
@implementation UIContextTracking
@synthesize menu = _menu, active = _active, beginSequence = _beginSequence, beginTick = _beginTick;
@end
@interface UIContextTrackedScope : NSObject
@property(nonatomic, strong) UIContextTracking* candidate;
@property(nonatomic, strong) NSMenuItem* reload;
@property(nonatomic) NSUInteger activeCount;
@property(nonatomic) NSUInteger newCount;
@property(nonatomic) NSUInteger reloadCount;
@property(nonatomic) NSUInteger mainExcluded;
@property(nonatomic) BOOL incomplete;
@property(nonatomic) BOOL ready;
@end
@implementation UIContextTrackedScope
@synthesize candidate = _candidate, reload = _reload, activeCount = _activeCount, newCount = _newCount;
@synthesize reloadCount = _reloadCount, mainExcluded = _mainExcluded, incomplete = _incomplete, ready = _ready;
@end
static BOOL TrackingComplete(void) {
  return gTrackingComplete && !atomic_load(&gTrackingOffMain) && gTrackingObservers.count == 2;
}
static void TrackingNotification(NSNotification* notification, BOOL begin) {
  if (!NSThread.isMainThread) { atomic_store(&gTrackingOffMain, true); return; }
  if (++gTrackingEvents > 256 || ![notification.object isKindOfClass:NSMenu.class]) { gTrackingComplete = NO; return; }
  NSMenu* menu = notification.object;
  UIContextTracking* entry = [gTrackedMenus objectForKey:menu];
  if (begin) {
    ++gTrackingBegins;
    if (!entry) {
      if (gTrackedMenus.count >= 64) { gTrackingComplete = NO; return; }
      entry = [UIContextTracking new]; entry.menu = menu; [gTrackedMenus setObject:entry forKey:menu];
    }
    if (entry.active) { gTrackingComplete = NO; return; }
    entry.active = YES; entry.beginSequence = gTrackingEvents; entry.beginTick = gTick;
  } else {
    ++gTrackingEnds;
    if (!entry || !entry.active) { gTrackingComplete = NO; return; }
    entry.active = NO;
    if (gAttempts == 1 && menu == gAttemptMenu && entry.beginSequence == gAttemptBeginSequence &&
        gTrackingEvents > gAttemptSequence && !gMatchingEndSequence) gMatchingEndSequence = gTrackingEvents;
  }
}
static UIContextTrackedScope* TrackedScope(void) {
  UIContextTrackedScope* scope = [UIContextTrackedScope new]; scope.incomplete = !TrackingComplete();
  @try {
    for (NSMenu* menu in gTrackedMenus) {
      UIContextTracking* entry = [gTrackedMenus objectForKey:menu];
      if (!entry.active) continue;
      ++scope.activeCount;
      BOOL incomplete = NO;
      if (MainMenuAncestor(menu, &incomplete)) { ++scope.mainExcluded; scope.incomplete |= incomplete; continue; }
      if (!gBaselineCaptured || entry.beginSequence <= gBaselineTrackingSequence) continue;
      // A menu already present in the pre-click native inventory is not a new
      // contextual container merely because a later tracking event was seen.
      if ([gBaselineNativeMenus containsObject:menu]) { scope.incomplete = YES; continue; }
      ++scope.newCount; scope.candidate = entry;
      NSArray<NSMenuItem*>* items = menu.itemArray;
      if (items.count > 128) { scope.incomplete = YES; continue; }
      for (NSMenuItem* item in items) {
        if (![item isKindOfClass:NSMenuItem.class] || item.menu != menu) { scope.incomplete = YES; continue; }
        if (!item.enabled || item.hidden || item.separatorItem || item.submenu ||
            item.isAccessibilityHidden || !item.isAccessibilityEnabled) continue;
        // No title is emitted. Compare only the canonical constant after the
        // bounded public supermenu chain proved this is outside the main menu.
        if ([item.title isEqual:@"Reload"]) { ++scope.reloadCount; scope.reload = item; }
      }
    }
  } @catch (NSException* exception) { (void)exception; scope.incomplete = YES; }
  scope.ready = !scope.incomplete && scope.newCount == 1 && scope.reloadCount == 1 &&
      scope.activeCount == 1 && gTrackingBegins - gBaselineTrackingBegins == 1 && gTick > scope.candidate.beginTick;
  return scope;
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
  UIContextTrackedScope* tracking = TrackedScope();
  BOOL postAXComplete = gBaselineComplete && gBaselineApplicationChildrenAvailable &&
      discovery.tree.applicationChildrenAvailable && !discovery.tree.truncated;
  BOOL postAXNoExtras = postAXComplete && !newWindows && !discovery.menus.count && !discovery.reloads.count;
  return @{@"baselineCaptured":@(gBaselineCaptured), @"baselineComplete":@(gBaselineComplete),
      @"baselineOutsideMenuCount":@(gBaselineOutsideMenus), @"newContextMenuCount":@(discovery.menus.count),
      @"reloadControlCount":@(discovery.reloads.count), @"mainMenuExcludedCount":@(discovery.mainExcluded),
      @"newNativeWindowCount":@(newWindows), @"nodeCount":@(discovery.tree.nodes.count),
      @"truncated":@(discovery.tree.truncated), @"applicationChildrenAvailable":@(discovery.tree.applicationChildrenAvailable),
      @"excludedRemoteRendererOrPrivateCount":@(discovery.tree.excluded),
      @"causalProfileIsolated":@(Isolated()), @"actionAttemptCount":@(gAttempts), @"nativeActionsPerformed":@(gAttempts),
      @"trackingObserverInstalled":@((BOOL)(gTrackingObservers.count == 2)), @"trackingComplete":@((BOOL)(!tracking.incomplete)),
      @"baselineTrackingMenuCount":@(gBaselineTrackingMenuCount), @"baselineTrackingSequence":@(gBaselineTrackingSequence),
      @"activeTrackingMenuCount":@(tracking.activeCount), @"newTrackingMenuCount":@(tracking.newCount),
      @"trackingReloadControlCount":@(tracking.reloadCount), @"trackingMainMenuExcludedCount":@(tracking.mainExcluded),
      @"trackingMenuReady":@(tracking.ready), @"trackingSequence":@(gTrackingEvents),
      @"trackingEventCount":@(gTrackingBegins + gTrackingEnds),
      @"trackingBeginCount":@(gTrackingBegins), @"trackingEndCount":@(gTrackingEnds),
      @"newTrackingBeginCount":@(gBaselineCaptured ? gTrackingBegins - gBaselineTrackingBegins : 0),
      @"nativeAttemptSequence":@(gAttemptSequence), @"attemptedTrackingMenuBeginSequence":@(gAttemptBeginSequence),
      @"matchingTrackingEndObserved":@((BOOL)(gMatchingEndSequence != 0)), @"matchingTrackingEndSequence":@(gMatchingEndSequence),
      @"trackingEndedAfterAttempt":@((BOOL)(gAttempts == 1 && gMatchingEndSequence > gAttemptSequence && gAttemptSequence > gAttemptBeginSequence)),
      @"postAXComplete":@(postAXComplete), @"postAXNoExtras":@(postAXNoExtras),
      @"contextMenuDismissed":@NO, @"visualAcceptance":@NO};
}
// AX discovery is an independent, read-only observation. If it exposes a new
// menu while tracking also proves a concrete menu, require exact public identity
// agreement; do not guess that distinct virtual objects alias the native item.
static BOOL AXCompatible(UIContextDiscovery* discovery, UIContextTrackedScope* tracking) {
  if (discovery.tree.truncated || !discovery.tree.applicationChildrenAvailable ||
      discovery.menus.count > 1 || discovery.reloads.count > 1) return NO;
  if (discovery.menus.count == 1) {
    UIContextNode* menu = discovery.tree.nodes[discovery.menus.firstObject.unsignedIntegerValue];
    if (menu.object != tracking.candidate.menu && menu.nativeMenu != tracking.candidate.menu) return NO;
  }
  return discovery.reloads.count == 0 || discovery.reloads.firstObject.object == tracking.reload;
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
      @"ownershipBasis":@"Fresh profile isolation and driver-exclusive fixture target, plus one new actual NSMenu tracking lifecycle outside the main menu after a complete empty pre-click baseline. Exact page-to-native-menu mapping is not claimed.",
      @"interpretation":@"The public NSMenuItem press is an attempt even when it returns false. Matching end-tracking and complete AX postchecks are lifecycle observations, not visual dismissal. The driver independently verifies page reload; functional and visual acceptance remain false."} mutableCopy];
  NSString* refusal = nil;
  if ([operation isEqual:@"snapshot-baseline"]) {
    if (gBaselineCaptured) refusal = @"baseline has already been captured; it cannot be overwritten";
    else {
      UIContextTrackedScope* tracking = TrackedScope();
      gBaselineTrackingMenuCount = tracking.activeCount; gBaselineTrackingSequence = gTrackingEvents; gBaselineTrackingBegins = gTrackingBegins;
      gBaselineCaptured = YES; gBaselineApplicationChildrenAvailable = tree.applicationChildrenAvailable;
      gBaselineComplete = !tree.truncated && gBaselineApplicationChildrenAvailable && !tracking.incomplete && !tracking.activeCount;
      gBaselineOutsideMenus = discovery.menus.count;
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
    UIContextTrackedScope* tracking = TrackedScope();
    if (!gEnabled) refusal = @"explicit canonical-context-v1 launch opt-in is absent";
    else if (!Isolated() || !Causal(request)) refusal = @"fresh isolated profile and driver fixture verification are required";
    else if (!gBaselineCaptured || !gBaselineComplete || gBaselineOutsideMenus != 0 || gBaselineTrackingMenuCount != 0) refusal = @"a complete empty pre-click AX and tracking baseline is required";
    else if (tree.truncated || tracking.incomplete) refusal = @"native or tracking inspection is incomplete";
    else if (gAttempts) refusal = @"the one native menu invocation budget is consumed";
    else if (!tracking.ready) refusal = @"no unique new non-main tracked menu and enabled visible Reload on a later timer tick";
    else if (!AXCompatible(discovery, tracking)) refusal = @"parallel AX inventory disagrees with the actual tracked menu scope";
    // Rebuild public native scope immediately before invocation. A changed menu,
    // item, begin generation or tracking state closes the gate.
    UIContextTrackedScope* live = TrackedScope();
    if (!refusal && (!live.ready || live.candidate != tracking.candidate || live.reload != tracking.reload ||
        live.candidate.beginSequence != tracking.candidate.beginSequence || !Isolated() || !Causal(request))) refusal = @"native Reload or causal scope changed before invocation";
    if (!refusal) {
      ++gAttempts; gAttemptMenu = live.candidate.menu; gAttemptBeginSequence = live.candidate.beginSequence;
      gAttemptSequence = ++gTrackingEvents;
      response[@"actionAttempted"] = @YES; response[@"actionAttemptCount"] = @(gAttempts); response[@"nativeActionsPerformed"] = @(gAttempts);
      response[@"nativeAttemptSequence"] = @(gAttemptSequence); response[@"attemptedTrackingMenuBeginSequence"] = @(gAttemptBeginSequence);
      response[@"nativeActionKind"] = @"NSMenuItem.accessibilityPerformPress";
      response[@"controlProof"] = @"public NSMenu tracking notifications and actual NSMenuItem";
      WriteJSON(response, [gRequest stringByAppendingString:@".attempt.json"]);
      @try { response[@"accessibilityPressReturn"] = @([live.reload accessibilityPerformPress]); response[@"nativeMethodReturned"] = @YES; }
      @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
    }
  }
  if (refusal) response[@"refusalReason"] = refusal;
  // Keep all polling and reply work in the common-mode NSTimer callback; native
  // menu tracking need not service the GCD main queue until the menu closes.
  UIContextDiscovery* after = Discover(Tree(), YES);
  NSDictionary* afterCounts = Counts(after); response[@"after"] = afterCounts;
  [response addEntriesFromDictionary:afterCounts];
  response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
}
static void Poll(void) {
  if (!NSThread.isMainThread || gBusy || gRequests >= 48) return;
  ++gTick;
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
      gTrackedMenus = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsStrongMemory];
      NSNotificationCenter* center = NSNotificationCenter.defaultCenter;
      id begin = [center addObserverForName:NSMenuDidBeginTrackingNotification object:nil queue:nil usingBlock:^(NSNotification* notification) { TrackingNotification(notification, YES); }];
      id end = [center addObserverForName:NSMenuDidEndTrackingNotification object:nil queue:nil usingBlock:^(NSNotification* notification) { TrackingNotification(notification, NO); }];
      gTrackingObservers = @[begin, end];
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"nonce":gNonce,
          @"mainThread":@(NSThread.isMainThread), @"actionCapabilityEnabled":@(gEnabled), @"causalProfileIsolated":@(Isolated()),
          @"baselineCaptured":@NO, @"actionAttemptBudget":@1, @"actionAttemptCount":@0, @"nativeActionsPerformed":@0,
          @"functionalAcceptance":@NO, @"trackingObserverInstalled":@((BOOL)(gTrackingObservers.count == 2)),
          @"trackingComplete":@(TrackingComplete()), @"visualAcceptance":@NO,
          @"pollingMode":@"NSTimer in common and event tracking modes; never action inside notification"}, [gRequest stringByAppendingString:@".ready.json"]);
      gDeadline = NSProcessInfo.processInfo.systemUptime + 100;
      NSTimer* timer = [NSTimer timerWithTimeInterval:0.25 repeats:YES block:^(NSTimer* firing) {
        if (NSProcessInfo.processInfo.systemUptime >= gDeadline) {
          [firing invalidate]; for (id observer in gTrackingObservers) [center removeObserver:observer];
          gTrackingObservers = nil; return;
        }
        @autoreleasepool { Poll(); }
      }];
      [NSRunLoop.mainRunLoop addTimer:timer forMode:NSRunLoopCommonModes];
      // Match the public Chromium menu test observer's explicit tracking mode.
      [NSRunLoop.mainRunLoop addTimer:timer forMode:NSEventTrackingRunLoopMode];
    });
  }
}
