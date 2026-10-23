// Test-only local native permission controls. No engine/delegate/block hooks,
// global input, activation, remote AX calls, credential reads or synthetic results.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

static NSString* gRequest;
static NSString* gNonce;
static NSString* gAuthority;
static NSString* gScenario;
static NSString* gIsolationNonce;
static NSString* gIsolationPath;
static NSMutableSet<NSString*>* gSeenIDs;
static NSUInteger gRequests;
static NSUInteger gAttempts;
static NSUInteger gPhase;
static BOOL gEnabled;
static BOOL gBusy;
static __weak NSWindow* gOwnerWindow;
static const NSUInteger kNodeLimit = 5000;
static const NSUInteger kDepthLimit = 48;

@interface UIPermissionNode : NSObject
@property(nonatomic, strong) id object;
@property(nonatomic, strong) NSWindow* window;
@property(nonatomic, copy) NSString* role;
@property(nonatomic) NSUInteger end;
@property(nonatomic) BOOL enabled;
@property(nonatomic) BOOL visible;
@property(nonatomic) BOOL denyMessage;
@property(nonatomic) BOOL allowMessage;
@property(nonatomic) BOOL notificationsIcon;
@property(nonatomic) BOOL notNow;
@property(nonatomic) BOOL allow;
@property(nonatomic) BOOL siteSettings;
@property(nonatomic) BOOL siteCenter;
@property(nonatomic) BOOL insecure;
@end
@implementation UIPermissionNode
@synthesize object = _object, window = _window, role = _role, end = _end;
@synthesize enabled = _enabled, visible = _visible, denyMessage = _denyMessage, allowMessage = _allowMessage;
@synthesize notificationsIcon = _notificationsIcon, notNow = _notNow, allow = _allow;
@synthesize siteSettings = _siteSettings, siteCenter = _siteCenter, insecure = _insecure;
@end
@interface UIPermissionTree : NSObject
@property(nonatomic, strong) NSMutableArray<UIPermissionNode*>* nodes;
@property(nonatomic, strong) NSHashTable* seen;
@property(nonatomic, strong) NSHashTable<NSWindow*>* windows;
@property(nonatomic) BOOL truncated;
@property(nonatomic) NSUInteger excluded;
@end
@implementation UIPermissionTree
@synthesize nodes = _nodes, seen = _seen, windows = _windows;
@synthesize truncated = _truncated, excluded = _excluded;
@end
@interface UIPermissionScope : NSObject
@property(nonatomic, strong) UIPermissionTree* tree;
@property(nonatomic, strong) UIPermissionNode* group;
@property(nonatomic, strong) NSArray<UIPermissionNode*>* denyButtons;
@property(nonatomic, strong) NSArray<UIPermissionNode*>* allowButtons;
@property(nonatomic) NSUInteger messageCount;
@property(nonatomic) NSUInteger iconCount;
@end
@implementation UIPermissionScope
@synthesize tree = _tree, group = _group, denyButtons = _denyButtons, allowButtons = _allowButtons;
@synthesize messageCount = _messageCount, iconCount = _iconCount;
@end

static BOOL FullScenario(void) { return [gScenario isEqual:@"cookies-storage-site-settings"]; }
static NSUInteger Budget(void) { return FullScenario() ? 3 : 1; }
static NSString* Phase(void) {
  if (!FullScenario()) return gPhase == 0 ? @"deny" : @"complete";
  return gPhase == 0 ? @"deny" : (gPhase == 1 ? @"allow" : (gPhase == 2 ? @"site-settings" : @"complete"));
}
static NSData* ReadBounded(NSString* path) {
  if (!path) return nil;
  NSFileHandle* file = [NSFileHandle fileHandleForReadingAtPath:path];
  if (!file) return nil;
  NSData* data = [file readDataUpToLength:4097 error:NULL];
  [file closeAndReturnError:NULL];
  return data.length && data.length <= 4096 ? data : nil;
}
static void WriteJSON(NSDictionary* response, NSString* path) {
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:response options:NSJSONWritingPrettyPrinted error:NULL];
  if (bytes) [bytes writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static BOOL ProfileIsolated(void) {
  if (!gIsolationNonce.length) return NO;
  NSData* bytes = ReadBounded(gIsolationPath);
  if (!bytes) return NO;
  id marker = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  return [marker isKindOfClass:NSDictionary.class] && [marker[@"nonce"] isEqual:gIsolationNonce];
}
static BOOL CausalRequest(NSDictionary* request) {
  id value = request[@"causalFixtureVerified"];
  return [value isKindOfClass:NSNumber.class] && CFGetTypeID((__bridge CFTypeRef)value) == CFBooleanGetTypeID() &&
      [value boolValue] && gIsolationNonce.length && [request[@"isolatedProfileNonce"] isEqual:gIsolationNonce];
}
static BOOL SecureField(id object) {
  return [object isKindOfClass:NSSecureTextField.class] ||
      ([object isKindOfClass:NSTextField.class] && [[(NSTextField*)object cell] isKindOfClass:NSSecureTextFieldCell.class]);
}
static NSString* ReadString(id object, SEL selector) {
  if (![object respondsToSelector:selector]) return nil;
  id result = [object valueForKey:NSStringFromSelector(selector)];
  return [result isKindOfClass:NSString.class] ? result : nil;
}
static NSString* PermissionMessage(BOOL allow) {
  NSString* authority = allow ? [gAuthority stringByReplacingOccurrencesOfString:@"127.0.0.1:" withString:@"localhost:"] : gAuthority;
  return [NSString stringWithFormat:@"Allow http://%@ to send you notifications?", authority];
}
static void CompareKnown(UIPermissionNode* node, NSString* value) {
  if (!value) return;
  node.denyMessage |= [value isEqual:PermissionMessage(NO)];
  node.allowMessage |= [value isEqual:PermissionMessage(YES)];
  node.notificationsIcon |= [value isEqual:@"Notifications"] && [node.role isEqual:NSAccessibilityImageRole];
  node.notNow |= [value isEqual:@"Not Now"];
  node.allow |= [value isEqual:@"Allow"];
  node.siteSettings |= [value isEqual:@"Site Settings"];
  node.siteCenter |= [value isEqual:@"Site Control Center"];
  node.insecure |= [value isEqual:@"Lock Off Insecure"];
}
static BOOL ExcludedClass(NSString* name) {
  return [name containsString:@"NSAccessibilityRemote"] || [name containsString:@"NSRemoteView"] ||
      [name containsString:@"WebContents"] || [name containsString:@"RenderWidgetHost"] ||
      [name containsString:@"BrowserAccessibility"];
}
static void Visit(id object, NSWindow* owner, NSUInteger depth, UIPermissionTree* tree) {
  if (!object || [tree.seen containsObject:object]) return;
  if (tree.seen.count >= kNodeLimit || depth > kDepthLimit) { tree.truncated = YES; return; }
  [tree.seen addObject:object];
  if (ExcludedClass(NSStringFromClass(object_getClass(object)))) { ++tree.excluded; return; }
  if ([object isKindOfClass:NSWindow.class]) {
    if (![tree.windows containsObject:object] || ![(NSWindow*)object isVisible]) { ++tree.excluded; return; }
    owner = object;
  } else if ([object isKindOfClass:NSView.class] && [(NSView*)object window]) {
    owner = [(NSView*)object window];
    if (![tree.windows containsObject:owner]) { ++tree.excluded; return; }
  }
  if (!owner || !owner.visible) return;
  if (SecureField(object) ||
      ([object isKindOfClass:NSTextField.class] && [(NSTextField*)object isEditable]) ||
      ([object isKindOfClass:NSTextView.class] && [(NSTextView*)object isEditable])) { ++tree.excluded; return; }
  UIPermissionNode* node = [UIPermissionNode new];
  node.object = object; node.window = owner; node.visible = YES;
  @try {
    node.role = ReadString(object, @selector(accessibilityRole));
    if ([node.role isEqual:NSAccessibilityWebAreaRole] || [node.role isEqual:NSAccessibilityTextFieldRole] ||
        [node.role isEqual:NSAccessibilityTextAreaRole]) { ++tree.excluded; return; }
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return;
    if ([object isKindOfClass:NSView.class] && [(NSView*)object isHiddenOrHasHiddenAncestor]) return;
    node.enabled = [object respondsToSelector:@selector(isAccessibilityEnabled)] && [(id<NSAccessibility>)object isAccessibilityEnabled];
    if ([object isKindOfClass:NSButton.class]) node.enabled &= [(NSButton*)object isEnabled];
    CompareKnown(node, ReadString(object, @selector(accessibilityLabel)));
    CompareKnown(node, ReadString(object, @selector(accessibilityTitle)));
    if ([object isKindOfClass:NSButton.class]) CompareKnown(node, [(NSButton*)object title]);
    // Only genuine noneditable local AXStaticText may expose its textual value.
    // Every value is compared against fixed fixture constants and discarded.
    if ([node.role isEqual:NSAccessibilityStaticTextRole]) CompareKnown(node, ReadString(object, @selector(accessibilityValue)));
    [tree.nodes addObject:node];
    // Prefer local accessibility hierarchy so SwiftUI AXGroup ownership is
    // preserved when the same backing object also appears in NSView.subviews.
    if ([object respondsToSelector:@selector(accessibilityChildren)]) {
      id children = [(id<NSAccessibility>)object accessibilityChildren];
      if ([children isKindOfClass:NSArray.class]) {
        NSUInteger count = 0;
        for (id child in children) {
          if (++count > kNodeLimit || tree.truncated) { tree.truncated = YES; break; }
          Visit(child, owner, depth + 1, tree);
        }
      }
    }
    if ([object isKindOfClass:NSWindow.class]) Visit([(NSWindow*)object contentView], owner, depth + 1, tree);
    else if ([object isKindOfClass:NSView.class]) {
      NSUInteger count = 0;
      for (NSView* child in [[(NSView*)object subviews] copy]) {
        if (++count > kNodeLimit || tree.truncated) { tree.truncated = YES; break; }
        Visit(child, owner, depth + 1, tree);
      }
    }
  } @catch (NSException* exception) { (void)exception; tree.truncated = YES; }
  node.end = tree.nodes.count;
}
static UIPermissionTree* Tree(BOOL ownerOnly, BOOL children) {
  UIPermissionTree* tree = [UIPermissionTree new];
  tree.nodes = [NSMutableArray array];
  tree.seen = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  tree.windows = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  NSMutableArray<NSWindow*>* roots = [NSMutableArray array];
  if (ownerOnly) {
    NSWindow* owner = gOwnerWindow;
    if (owner && owner.visible) [roots addObject:owner];
    if (children) {
      NSUInteger inspectedChildren = 0;
      for (NSUInteger index = 0; index < roots.count; ++index) {
        if (index >= 64) { tree.truncated = YES; break; }
        for (NSWindow* child in [roots[index].childWindows copy]) {
          if (++inspectedChildren > 64) { tree.truncated = YES; break; }
          if (roots.count >= 64) { tree.truncated = YES; break; }
          if (child.visible && [roots indexOfObjectIdenticalTo:child] == NSNotFound) [roots addObject:child];
        }
      }
    }
  } else {
    NSUInteger inspected = 0;
    for (NSWindow* window in [NSApp.windows copy]) {
      if (++inspected > 64) { tree.truncated = YES; break; }
      if (window.visible) [roots addObject:window];
    }
  }
  for (NSWindow* root in roots) [tree.windows addObject:root];
  for (NSWindow* root in roots) Visit(root, root, 0, tree);
  return tree;
}
static BOOL ActionButton(UIPermissionNode* node) {
  return node.visible && node.enabled && node.window.visible &&
      ([node.object isKindOfClass:NSButton.class] || [node.role isEqual:NSAccessibilityButtonRole]) &&
      ([node.object isKindOfClass:NSButton.class] || [node.object respondsToSelector:@selector(accessibilityPerformPress)]);
}
static NSArray<UIPermissionScope*>* PermissionScopes(UIPermissionTree* tree, BOOL allow) {
  NSMutableArray<UIPermissionScope*>* candidates = [NSMutableArray array];
  for (NSUInteger index = 0; index < tree.nodes.count; ++index) {
    UIPermissionNode* group = tree.nodes[index];
    if (![group.role isEqual:NSAccessibilityGroupRole] || group.end <= index || group.end - index > 128) continue;
    UIPermissionScope* scope = [UIPermissionScope new]; scope.tree = tree; scope.group = group;
    NSMutableArray* denyButtons = [NSMutableArray array], *allowButtons = [NSMutableArray array];
    for (NSUInteger child = index; child < group.end; ++child) {
      UIPermissionNode* node = tree.nodes[child];
      if (node.window != group.window) continue;
      scope.messageCount += allow ? node.allowMessage : node.denyMessage;
      scope.iconCount += node.notificationsIcon;
      if (ActionButton(node)) {
        if (node.notNow) [denyButtons addObject:node];
        if (node.allow) [allowButtons addObject:node];
      }
    }
    scope.denyButtons = denyButtons; scope.allowButtons = allowButtons;
    if (scope.messageCount == 1 && scope.iconCount == 1 && denyButtons.count == 1 && allowButtons.count == 1) [candidates addObject:scope];
  }
  // Keep only innermost qualifying groups. Two independent native groups remain
  // ambiguous, even if one group happens to have fewer nodes than the other.
  NSMutableArray* smallest = [NSMutableArray array];
  for (UIPermissionScope* candidate in candidates) {
    NSUInteger index = [tree.nodes indexOfObjectIdenticalTo:candidate.group];
    BOOL containsOther = NO;
    for (UIPermissionScope* other in candidates) {
      NSUInteger child = [tree.nodes indexOfObjectIdenticalTo:other.group];
      if (other != candidate && child > index && child < candidate.group.end) { containsOther = YES; break; }
    }
    if (!containsOther) [smallest addObject:candidate];
  }
  return smallest;
}
static NSArray<UIPermissionNode*>* SiteButtons(UIPermissionTree* tree) {
  NSMutableArray* buttons = [NSMutableArray array]; NSWindow* owner = gOwnerWindow;
  for (UIPermissionNode* node in tree.nodes) if (node.window == owner && node.siteSettings && ActionButton(node)) [buttons addObject:node];
  return buttons;
}
static NSDictionary* Summary(NSString* stage, BOOL inspect, UIPermissionTree** treeOut, NSArray<UIPermissionScope*>** scopesOut) {
  BOOL site = [stage isEqual:@"site-settings"];
  UIPermissionTree* tree = Tree(site, site && inspect);
  NSArray<UIPermissionScope*>* scopes = site ? @[] : PermissionScopes(tree, [stage isEqual:@"allow"]);
  if (treeOut) *treeOut = tree;
  if (scopesOut) *scopesOut = scopes;
  NSUInteger messages = 0, icons = 0, deny = 0, allow = 0;
  for (UIPermissionScope* scope in scopes) { messages += scope.messageCount; icons += scope.iconCount; deny += scope.denyButtons.count; allow += scope.allowButtons.count; }
  NSUInteger centers = 0, insecureSubtrees = 0, insecure = 0;
  if (site && inspect) {
    for (NSUInteger index = 0; index < tree.nodes.count; ++index) {
      UIPermissionNode* center = tree.nodes[index];
      if (!center.siteCenter || !([center.role isEqual:NSAccessibilityButtonRole] || [center.object isKindOfClass:NSButton.class])) continue;
      ++centers; NSUInteger found = 0;
      for (NSUInteger child = index + 1; child < center.end; ++child) found += tree.nodes[child].insecure;
      insecure += found; insecureSubtrees += found == 1;
    }
  }
  return @{@"stage":stage, @"matchingGroupCount":@(scopes.count), @"exactMessageCount":@(messages),
      @"notificationsIconCount":@(icons), @"expectedButtonCounts":@{@"Not Now":@(deny), @"Allow":@(allow)},
      @"siteSettingsButtonCount":@(site ? SiteButtons(tree).count : 0), @"siteControlCenterCount":@(centers),
      @"lockOffInsecureCount":@(insecure), @"siteControlCenterInsecureSubtreeCount":@(insecureSubtrees),
      @"siteSettingsObserved":@(inspect && !tree.truncated && centers == 1 && insecure == 1 && insecureSubtrees == 1),
      @"nodeCount":@(tree.nodes.count), @"truncated":@(tree.truncated), @"excludedRemoteRendererOrPrivateCount":@(tree.excluded),
      @"ownerWindowAvailable":@(gOwnerWindow != nil), @"causalProfileIsolated":@(ProfileIsolated()),
      @"phase":Phase(), @"actionAttemptCount":@(gAttempts), @"nativeActionsPerformed":@(gAttempts)};
}
static BOOL LiveButton(UIPermissionNode* node, NSString* label) {
  id object = node.object;
  if (!node.window || !node.window.visible || ExcludedClass(NSStringFromClass(object_getClass(object))) || SecureField(object)) return NO;
  @try {
    if ([object isKindOfClass:NSView.class] && ([(NSView*)object window] != node.window || [(NSView*)object isHiddenOrHasHiddenAncestor])) return NO;
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return NO;
    if (![object respondsToSelector:@selector(isAccessibilityEnabled)] || ![(id<NSAccessibility>)object isAccessibilityEnabled]) return NO;
    if ([object isKindOfClass:NSButton.class]) return [(NSButton*)object isEnabled] && [[(NSButton*)object title] isEqual:label];
    return [ReadString(object, @selector(accessibilityRole)) isEqual:NSAccessibilityButtonRole] &&
        [object respondsToSelector:@selector(accessibilityPerformPress)] &&
        ([ReadString(object, @selector(accessibilityLabel)) isEqual:label] || [ReadString(object, @selector(accessibilityTitle)) isEqual:label]);
  } @catch (NSException* exception) { (void)exception; return NO; }
}
static BOOL ValidStage(NSString* stage) { return [stage isEqual:@"deny"] || (FullScenario() && [@[@"allow", @"site-settings"] containsObject:stage]); }
static BOOL ValidOperation(NSString* operation) {
  return [@[@"inventory", @"press-deny"] containsObject:operation] ||
      (FullScenario() && [@[@"press-allow", @"press-site-settings", @"inspect-site-settings"] containsObject:operation]);
}
static void HandleRequest(NSDictionary* request) {
  gBusy = YES;
  NSString* stage = request[@"stage"], *operation = request[@"operation"], *identifier = request[@"id"];
  BOOL inspect = [operation isEqual:@"inspect-site-settings"];
  UIPermissionTree* tree = nil; NSArray<UIPermissionScope*>* scopes = nil;
  NSDictionary* before = Summary(stage, inspect, &tree, &scopes);
  NSMutableDictionary* response = [@{@"id":identifier, @"nonce":gNonce, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
      @"mainThread":@(NSThread.isMainThread), @"scenario":gScenario, @"stage":stage, @"operation":operation,
      @"functionalAcceptance":@NO, @"actionAttempted":@NO, @"nativeMethodReturned":@NO,
      @"nativeActionKind":NSNull.null, @"accessibilityPressReturn":NSNull.null,
      @"causalFixtureVerified":@(CausalRequest(request)), @"ownershipBasis":@"Fresh isolated-profile marker and driver-verified sole fixture target; native group markers prove permission scope. Exact page-to-window mapping is not claimed.",
      @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"interpretation":@"Public native control invocation is experimental. A false accessibility press or lost response still consumes the attempt. Only independent page state and boundary recording can establish functional acceptance."} mutableCopy];
  [response addEntriesFromDictionary:before]; response[@"before"] = before;
  if ([operation isEqual:@"inventory"] || inspect) {
    if (inspect && ![stage isEqual:@"site-settings"]) response[@"refusalReason"] = @"site inspection requires site-settings stage";
    response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
    WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO; return;
  }
  NSString* refusal = nil;
  if (!gEnabled) refusal = @"explicit canonical-permissions-v1 launch opt-in is absent";
  else if (!ProfileIsolated() || !CausalRequest(request)) refusal = @"fresh profile isolation and driver causal fixture verification are required";
  else if (tree.truncated) refusal = @"native tree limit or exception prevents complete scope discovery";
  else if (gAttempts >= Budget()) refusal = @"native invocation budget exhausted";
  else if (![stage isEqual:Phase()]) refusal = @"request stage is out of phase";
  else if (([operation isEqual:@"press-deny"] && ![stage isEqual:@"deny"]) ||
           ([operation isEqual:@"press-allow"] && ![stage isEqual:@"allow"]) ||
           ([operation isEqual:@"press-site-settings"] && ![stage isEqual:@"site-settings"])) refusal = @"operation and stage disagree";
  UIPermissionNode* button = nil;
  NSString* label = [operation isEqual:@"press-deny"] ? @"Not Now" : ([operation isEqual:@"press-allow"] ? @"Allow" : @"Site Settings");
  if (!refusal && [stage isEqual:@"site-settings"]) {
    NSArray* buttons = SiteButtons(tree);
    if (!gOwnerWindow || buttons.count != 1) refusal = @"stored native owner window has no unique enabled Site Settings button";
    else button = buttons.firstObject;
  } else if (!refusal) {
    if (scopes.count != 1) refusal = @"no unique smallest bounded permission AXGroup with exact message, Notifications image and both buttons";
    else button = [operation isEqual:@"press-deny"] ? scopes.firstObject.denyButtons.firstObject : scopes.firstObject.allowButtons.firstObject;
  }
  if (!refusal && (!button || !LiveButton(button, label) || !ProfileIsolated() || !CausalRequest(request))) refusal = @"native button or causal scope changed before invocation";
  if (!refusal) {
    if ([operation isEqual:@"press-allow"]) gOwnerWindow = button.window;
    ++gAttempts;
    gPhase = !FullScenario() ? 3 : gPhase + 1;
    response[@"actionAttempted"] = @YES; response[@"selectedButtonLabel"] = label;
    response[@"actionAttemptCount"] = @(gAttempts); response[@"nativeActionsPerformed"] = @(gAttempts); response[@"phase"] = Phase();
    response[@"nativeActionKind"] = [button.object isKindOfClass:NSButton.class] ? @"performClick" : @"accessibilityPerformPress";
    // Persist intent after consuming phase/budget, before a native call can
    // reenter the run loop or terminate the process. Never retry this attempt.
    WriteJSON(response, [gRequest stringByAppendingString:@".attempt.json"]);
    @try {
      if ([button.object isKindOfClass:NSButton.class]) [(NSButton*)button.object performClick:nil];
      else response[@"accessibilityPressReturn"] = @([(id<NSAccessibility>)button.object accessibilityPerformPress]);
      response[@"nativeMethodReturned"] = @YES;
    } @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
  } else response[@"refusalReason"] = refusal;
  response[@"actionAttemptCount"] = @(gAttempts); response[@"nativeActionsPerformed"] = @(gAttempts); response[@"phase"] = Phase();
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 150 * NSEC_PER_MSEC), dispatch_get_main_queue(), ^{
    response[@"after"] = Summary(stage, [stage isEqual:@"site-settings"], NULL, NULL);
    response[@"ownerWindowAvailable"] = @(gOwnerWindow != nil);
    response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
    WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
  });
}
static void Poll(void) {
  if (!NSThread.isMainThread || gBusy || gRequests >= 48) return;
  NSData* bytes = ReadBounded(gRequest); if (!bytes) return;
  id request = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  if (![identifier isKindOfClass:NSString.class] || identifier.length > 160 ||
      ![identifier hasPrefix:[gNonce stringByAppendingString:@"-"]] || [gSeenIDs containsObject:identifier] ||
      ![request[@"nonce"] isEqual:gNonce] || ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)] ||
      !ValidStage(request[@"stage"]) || !ValidOperation(request[@"operation"])) return;
  NSString* operation = request[@"operation"], *stage = request[@"stage"];
  if (([operation isEqual:@"press-deny"] && ![stage isEqual:@"deny"]) ||
      ([operation isEqual:@"press-allow"] && ![stage isEqual:@"allow"]) ||
      ([@[@"press-site-settings", @"inspect-site-settings"] containsObject:operation] && ![stage isEqual:@"site-settings"])) return;
  [gSeenIDs addObject:identifier]; ++gRequests; HandleRequest(request);
}
__attribute__((constructor)) static void StartUIPermissionControlsProbe(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    NSString* slug = [app isEqual:@"ArcModAgent-trace156.app"] ? @"trace156" : ([app isEqual:@"ArcModAgent-ours156.app"] ? @"ours156" : nil);
    if (!slug || ![executable.lastPathComponent isEqual:@"Arc"] ||
        ![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]]) return;
    const char* path = getenv("ARC_UI_PERMISSION_REQUEST"), *nonce = getenv("ARC_UI_PERMISSION_NONCE");
    const char* authority = getenv("ARC_UI_PERMISSION_FIXTURE_AUTHORITY"), *scenario = getenv("ARC_UI_PERMISSION_SCENARIO");
    const char* enabled = getenv("ARC_UI_PERMISSION_ACTION_ENABLE"), *isolation = getenv("ARC_UI_PROFILE_ISOLATION_NONCE");
    if (!path || !nonce || !authority || !scenario) return;
    gRequest = @(path); gNonce = @(nonce); gAuthority = @(authority); gScenario = @(scenario);
    NSURL* parsed = [NSURL URLWithString:[@"http://" stringByAppendingString:gAuthority]]; NSInteger port = parsed.port.integerValue;
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"ui-permissions-request.json"] ||
        ![gNonce hasPrefix:@"arc-ui-permissions-"] || gNonce.length > 96 ||
        ![@[@"permissions-deny", @"cookies-storage-site-settings"] containsObject:gScenario] ||
        ![parsed.host isEqual:@"127.0.0.1"] || port < 1024 || port > 65535 ||
        ![gAuthority isEqual:[NSString stringWithFormat:@"127.0.0.1:%ld", (long)port]]) return;
    gEnabled = enabled && [@(enabled) isEqual:@"canonical-permissions-v1"];
    if (isolation && @(isolation).length && @(isolation).length <= 160) gIsolationNonce = @(isolation);
    gIsolationPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Library/Application Support"]
        stringByAppendingPathComponent:[NSString stringWithFormat:@"ArcModAgent-%@/.arc-ui-probe-profile.json", slug]];
    gSeenIDs = [NSMutableSet set]; unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"nonce":gNonce,
          @"mainThread":@(NSThread.isMainThread), @"scenario":gScenario, @"phase":Phase(),
          @"actionCapabilityEnabled":@(gEnabled), @"actionAttemptBudget":@(Budget()), @"actionAttemptCount":@0,
          @"nativeActionsPerformed":@0, @"causalProfileIsolated":@(ProfileIsolated()), @"functionalAcceptance":@NO},
          [gRequest stringByAppendingString:@".ready.json"]);
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), 250 * NSEC_PER_MSEC, 20 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } }); dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 100 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
