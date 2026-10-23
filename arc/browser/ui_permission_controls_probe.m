// Test-only local native permission controls. No engine/delegate/block hooks.
// Default modes use no activation API; explicit opt-in permits one public self
// NSApp.activate call. No global input, remote AX calls, credential reads or synthetic results.
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
      @"siteSettingsObserved":@((BOOL)(inspect && !tree.truncated && centers == 1 && insecure == 1 && insecureSubtrees == 1)),
      @"nodeCount":@(tree.nodes.count), @"truncated":@(tree.truncated), @"excludedRemoteRendererOrPrivateCount":@(tree.excluded),
      @"ownerWindowAvailable":@((BOOL)(gOwnerWindow != nil)), @"causalProfileIsolated":@(ProfileIsolated()),
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
  return [@[@"inventory", @"press-deny", @"press-prompt-site-center", @"inventory-native-window", @"activate-native-app", @"prepare-native-window", @"inspect-native-window"] containsObject:operation] ||
      (FullScenario() && [@[@"press-allow", @"press-site-settings", @"inspect-site-settings"] containsObject:operation]);
}
// Diagnostic-only public local AX graph. Never used by action qualification.
static BOOL gDiagnosticOnly;
static const NSUInteger kDiagnosticEdgeLimit = 10000;
enum { DMessage = 1, DAllowMessage = 2, DIcon = 4, DNotNow = 8, DAllow = 16,
       DSettings = 32, DCenter = 64, DInsecure = 128 };
static NSArray<NSString*>* DMarkerNames(void) {
  return @[@"denyMessage", @"allowMessage", @"notificationsIcon", @"notNow", @"allow", @"siteSettings", @"siteCenter", @"insecure"];
}
static void DIncrement(NSMutableDictionary* counts, NSString* key) {
  counts[key] = @([counts[key] unsignedIntegerValue] + 1);
}
static NSMutableDictionary* DZeroMarkers(void) {
  NSMutableDictionary* counts = [NSMutableDictionary dictionary];
  for (NSString* key in DMarkerNames()) counts[key] = @0;
  return counts;
}
static void DCountMarkers(NSMutableDictionary* counts, NSUInteger markers) {
  NSArray* names = DMarkerNames();
  for (NSUInteger i = 0; i < names.count; ++i) if (markers & (1u << i)) DIncrement(counts, names[i]);
}
static NSUInteger DMarkers(id value) {
  if (![value isKindOfClass:NSString.class]) return 0;
  NSString* string = value; NSUInteger result = 0;
  if ([string isEqual:PermissionMessage(NO)]) result |= DMessage;
  if ([string isEqual:PermissionMessage(YES)]) result |= DAllowMessage;
  NSArray* fixed = @[@"Notifications", @"Not Now", @"Allow", @"Site Settings", @"Site Control Center", @"Lock Off Insecure"];
  for (NSUInteger i = 0; i < fixed.count; ++i) if ([string isEqual:fixed[i]]) result |= 1u << (i + 2);
  return result;
}
static NSString* DRole(id value) {
  NSArray* roles = @[NSAccessibilityApplicationRole, NSAccessibilityWindowRole, NSAccessibilityGroupRole,
      NSAccessibilityPopoverRole, NSAccessibilityButtonRole, NSAccessibilityImageRole, NSAccessibilityStaticTextRole,
      NSAccessibilityWebAreaRole, NSAccessibilityTextFieldRole, NSAccessibilityTextAreaRole];
  if ([value isKindOfClass:NSString.class]) for (NSString* role in roles) if ([value isEqual:role]) return role;
  return @"otherOrMissing";
}
static NSString* DType(id value) {
  if (!value) return @"absent";
  if ([value isKindOfClass:NSWindow.class]) return @"localNSWindow";
  if ([value isKindOfClass:NSString.class]) return @"string";
  if ([value isKindOfClass:NSNumber.class]) return @"number";
  if ([value isKindOfClass:NSArray.class]) return @"array";
  return @"otherObject";
}
@interface UIPermissionDiagnosticNode : NSObject
@property(nonatomic, strong) id object;
@property(nonatomic, strong) NSWindow* owner;
@property(nonatomic, copy) NSString* role;
@property(nonatomic, strong) NSMutableIndexSet* children;
@property(nonatomic) NSUInteger markers;
@property(nonatomic) BOOL enabled;
@property(nonatomic) BOOL press;
@property(nonatomic) BOOL nativeButton;
@end
@implementation UIPermissionDiagnosticNode
@synthesize object = _object, owner = _owner, role = _role, children = _children;
@synthesize markers = _markers, enabled = _enabled, press = _press, nativeButton = _nativeButton;
@end
@interface UIPermissionDiagnosticGraph : NSObject
@property(nonatomic, strong) NSMutableArray<UIPermissionDiagnosticNode*>* nodes;
@property(nonatomic, strong) NSMapTable* indices;
@property(nonatomic, strong) NSHashTable* seen;
@property(nonatomic, strong) NSHashTable* active;
@property(nonatomic, strong) NSHashTable* windows;
@property(nonatomic, strong) NSMutableDictionary* exclusions;
@property(nonatomic, strong) NSMutableDictionary* getters;
@property(nonatomic, strong) NSMutableDictionary* capabilities;
@property(nonatomic, strong) NSMutableDictionary* edgesBySource;
@property(nonatomic) NSUInteger edges;
@property(nonatomic) NSUInteger visits;
@property(nonatomic) NSUInteger aliases;
@property(nonatomic) NSUInteger cycles;
@property(nonatomic) NSUInteger appChildCount;
@property(nonatomic) BOOL truncated;
@end
@implementation UIPermissionDiagnosticGraph
@synthesize nodes = _nodes, indices = _indices, seen = _seen, active = _active, windows = _windows;
@synthesize exclusions = _exclusions, getters = _getters, capabilities = _capabilities, edgesBySource = _edgesBySource;
@synthesize edges = _edges, visits = _visits, aliases = _aliases, cycles = _cycles, appChildCount = _appChildCount, truncated = _truncated;
@end
static id DTyped(id object, NSString* key, BOOL* available) {
  if ([key isEqual:@"role"]) { *available = [object respondsToSelector:@selector(accessibilityRole)]; return *available ? [(id<NSAccessibility>)object accessibilityRole] : nil; }
  if ([key isEqual:@"children"]) { *available = [object respondsToSelector:@selector(accessibilityChildren)]; return *available ? [(id<NSAccessibility>)object accessibilityChildren] : nil; }
  if ([key isEqual:@"enabled"]) { *available = [object respondsToSelector:@selector(isAccessibilityEnabled)]; return *available ? @([(id<NSAccessibility>)object isAccessibilityEnabled]) : nil; }
  if ([key isEqual:@"description"]) { *available = [object respondsToSelector:@selector(accessibilityLabel)]; return *available ? [(id<NSAccessibility>)object accessibilityLabel] : nil; }
  if ([key isEqual:@"title"]) { *available = [object respondsToSelector:@selector(accessibilityTitle)]; return *available ? [(id<NSAccessibility>)object accessibilityTitle] : nil; }
  if ([key isEqual:@"window"]) { *available = [object respondsToSelector:@selector(accessibilityWindow)]; return *available ? [(id<NSAccessibility>)object accessibilityWindow] : nil; }
  if ([key isEqual:@"staticValue"]) { *available = [object respondsToSelector:@selector(accessibilityValue)]; return *available ? [(id<NSAccessibility>)object accessibilityValue] : nil; }
  if ([key isEqual:@"protectedContent"]) { *available = [object respondsToSelector:@selector(isAccessibilityProtectedContent)]; return *available ? @([(id<NSAccessibility>)object isAccessibilityProtectedContent]) : nil; }
  *available = NO; return nil;
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static NSArray* DAdvertisedAttributes(id object) {
  if (![object respondsToSelector:@selector(accessibilityAttributeNames)]) return nil;
  id names = [object accessibilityAttributeNames];
  return [names isKindOfClass:NSArray.class] ? names : nil;
}
static id DLegacy(id object, NSString* key, NSArray* names, BOOL* available) {
  // This fixed dictionary is the entire legacy read allowlist. No enumeration
  // of unknown attribute values, parameterized reads or legacy actions occurs.
  NSDictionary* attributes = @{@"role":NSAccessibilityRoleAttribute, @"children":NSAccessibilityChildrenAttribute,
      @"enabled":NSAccessibilityEnabledAttribute, @"description":NSAccessibilityDescriptionAttribute,
      @"title":NSAccessibilityTitleAttribute, @"window":NSAccessibilityWindowAttribute,
      @"staticValue":NSAccessibilityValueAttribute, @"protectedContent":NSAccessibilityContainsProtectedContentAttribute};
  NSString* attribute = attributes[key];
  *available = attribute && [names containsObject:attribute] && [object respondsToSelector:@selector(accessibilityAttributeValue:)];
  return *available ? [object accessibilityAttributeValue:attribute] : nil;
}
static BOOL DLegacyPressAdvertised(id object) {
  if (![object respondsToSelector:@selector(accessibilityActionNames)]) return NO;
  id actions = [object accessibilityActionNames];
  return [actions isKindOfClass:NSArray.class] && [actions count] <= 64 && [actions containsObject:NSAccessibilityPressAction];
}
#pragma clang diagnostic pop
static BOOL DKnownChildrenEqual(id left, id right, UIPermissionDiagnosticGraph* graph) {
  if (![left isKindOfClass:NSArray.class] || ![right isKindOfClass:NSArray.class]) return left == right;
  if ([left count] > kNodeLimit || [right count] > kNodeLimit) { graph.truncated = YES; return NO; }
  NSHashTable* identities = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  for (id child in left) [identities addObject:child];
  NSHashTable* other = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  for (id child in right) [other addObject:child];
  if (identities.count != other.count) return NO;
  for (id child in identities) if (![other containsObject:child]) return NO;
  return YES;
}
static NSArray* DReadPair(id object, NSString* key, NSArray* advertised, UIPermissionDiagnosticGraph* graph) {
  NSMutableDictionary* counts = graph.getters[key];
  if (!counts) { counts = [NSMutableDictionary dictionary]; graph.getters[key] = counts; }
  BOOL typedAvailable = NO, legacyAvailable = NO; id typed = nil, legacy = nil;
  @try { typed = DTyped(object, key, &typedAvailable); }
  @catch (NSException* exception) {
    (void)exception; DIncrement(counts, @"typedException"); graph.truncated = YES;
    if ([key isEqual:@"role"] || [key isEqual:@"protectedContent"]) typed = @[];
  }
  @try { legacy = DLegacy(object, key, advertised, &legacyAvailable); }
  @catch (NSException* exception) {
    (void)exception; DIncrement(counts, @"legacyException"); graph.truncated = YES;
    if ([key isEqual:@"role"] || [key isEqual:@"protectedContent"]) legacy = @[];
  }
  DIncrement(counts, typedAvailable ? @"typedAvailable" : @"typedUnavailable");
  DIncrement(counts, legacyAvailable ? @"legacyAdvertised" : @"legacyNotAdvertised");
  DIncrement(counts, [@"typedType_" stringByAppendingString:DType(typed)]);
  DIncrement(counts, [@"legacyType_" stringByAppendingString:DType(legacy)]);
  if ([key isEqual:@"role"]) {
    DIncrement(counts, [@"typedRole_" stringByAppendingString:DRole(typed)]);
    DIncrement(counts, [@"legacyRole_" stringByAppendingString:DRole(legacy)]);
  } else if ([@[@"description", @"title", @"staticValue"] containsObject:key]) {
    NSMutableDictionary* typedMatches = counts[@"typedFixedMatches"], *legacyMatches = counts[@"legacyFixedMatches"];
    if (!typedMatches) { typedMatches = DZeroMarkers(); counts[@"typedFixedMatches"] = typedMatches; }
    if (!legacyMatches) { legacyMatches = DZeroMarkers(); counts[@"legacyFixedMatches"] = legacyMatches; }
    DCountMarkers(typedMatches, DMarkers(typed)); DCountMarkers(legacyMatches, DMarkers(legacy));
  }
  BOOL disagree = ![DType(typed) isEqual:DType(legacy)];
  if ([key isEqual:@"role"]) disagree |= ![DRole(typed) isEqual:DRole(legacy)];
  else if ([key isEqual:@"children"]) disagree |= !DKnownChildrenEqual(typed, legacy, graph);
  else if ([key isEqual:@"window"]) disagree |= typed != legacy;
  else if ([key isEqual:@"enabled"] || [key isEqual:@"protectedContent"]) {
    if ([typed isKindOfClass:NSNumber.class] && [legacy isKindOfClass:NSNumber.class]) disagree |= [typed boolValue] != [legacy boolValue];
  } else disagree |= DMarkers(typed) != DMarkers(legacy);
  if (disagree) DIncrement(counts, @"resultOrFixedMatchDisagreement");
  // Raw labels/values live only until this node has compared fixed constants.
  return @[typed ?: NSNull.null, legacy ?: NSNull.null];
}
static id DPreferred(NSArray* pair, Class kind) {
  for (id value in pair) if ([value isKindOfClass:kind]) return value;
  return nil;
}
static void DEdge(UIPermissionDiagnosticGraph* graph, NSInteger parent, NSUInteger child, NSString* source) {
  if (parent < 0) return;
  DIncrement(graph.edgesBySource, source);
  if ([graph.nodes[(NSUInteger)parent].children containsIndex:child]) return;
  if (graph.edges >= kDiagnosticEdgeLimit) { graph.truncated = YES; return; }
  [graph.nodes[(NSUInteger)parent].children addIndex:child]; ++graph.edges;
}
static BOOL DVisitLimitReached(UIPermissionDiagnosticGraph* graph) {
  if (graph.visits < 20000) return NO;
  graph.exclusions[@"visitAttemptLimit"] = @1; graph.truncated = YES; return YES;
}
static void DVisit(id object, NSWindow* inheritedOwner, NSInteger parent, NSString* source,
                   NSUInteger depth, UIPermissionDiagnosticGraph* graph) {
  if (!object || DVisitLimitReached(graph)) return;
  ++graph.visits;
  NSNumber* known = [graph.indices objectForKey:object];
  if (known) {
    ++graph.aliases; DEdge(graph, parent, known.unsignedIntegerValue, source);
    if ([graph.active containsObject:object]) { ++graph.cycles; graph.truncated = YES; }
    return;
  }
  if ([graph.seen containsObject:object]) return;
  if (graph.seen.count >= kNodeLimit || depth > kDepthLimit) { DIncrement(graph.exclusions, @"nodeOrDepthLimit"); graph.truncated = YES; return; }
  [graph.seen addObject:object];
  NSString* className = NSStringFromClass(object_getClass(object));
  if (ExcludedClass(className)) { DIncrement(graph.exclusions, @"remoteOrRenderer"); return; }
  if (SecureField(object)) { DIncrement(graph.exclusions, @"secureField"); return; }
  if (([object isKindOfClass:NSTextField.class] && [(NSTextField*)object isEditable]) ||
      ([object isKindOfClass:NSTextView.class] && [(NSTextView*)object isEditable])) { DIncrement(graph.exclusions, @"editableField"); return; }
  @try {
    NSArray* advertised = DAdvertisedAttributes(object);
    if (advertised.count > 128) { DIncrement(graph.exclusions, @"legacyAttributeNameLimit"); graph.truncated = YES; return; }
    DIncrement(graph.capabilities, advertised ? @"legacyAttributeNamesAvailable" : @"legacyAttributeNamesUnavailable");
    // The only additional legacy attribute is this fixed public protection flag.
    // Read both representations before any description, title or static value.
    // Genuine absence is neutral; true, malformed, or conflicting flags prune.
    NSArray* protection = DReadPair(object, @"protectedContent", advertised, graph);
    BOOL malformedProtection = NO, protectedContent = NO;
    for (id flag in protection) {
      if (flag == NSNull.null) continue;
      if (![flag isKindOfClass:NSNumber.class] || !([flag isEqual:@NO] || [flag isEqual:@YES])) { malformedProtection = YES; continue; }
      protectedContent |= [flag boolValue];
    }
    BOOL protectionConflict = [protection[0] isKindOfClass:NSNumber.class] && [protection[1] isKindOfClass:NSNumber.class] &&
        [protection[0] boolValue] != [protection[1] boolValue];
    if (malformedProtection) DIncrement(graph.exclusions, @"malformedProtectedContentFlag");
    if (protectionConflict) DIncrement(graph.exclusions, @"typedLegacyProtectedContentConflict");
    if (protectedContent) DIncrement(graph.exclusions, @"protectedContent");
    if (malformedProtection || protectionConflict || protectedContent) return;
    NSArray* windows = DReadPair(object, @"window", advertised, graph);
    NSWindow* owner = inheritedOwner;
    NSWindow* reported = DPreferred(windows, NSWindow.class);
    if ([windows[0] isKindOfClass:NSWindow.class] && [windows[1] isKindOfClass:NSWindow.class] && windows[0] != windows[1]) {
      DIncrement(graph.exclusions, @"typedLegacyWindowConflict"); [graph.seen removeObject:object]; return;
    }
    if ([object isKindOfClass:NSWindow.class]) owner = object;
    else if ([object isKindOfClass:NSView.class] && [(NSView*)object window]) owner = [(NSView*)object window];
    else if (reported) owner = reported;
    if (reported && owner && reported != owner) { DIncrement(graph.exclusions, @"conflictingNativeWindow"); [graph.seen removeObject:object]; return; }
    BOOL application = object == NSApp;
    if (!application && !owner) { DIncrement(graph.exclusions, @"missingLocalWindow"); [graph.seen removeObject:object]; return; }
    if (!application && ![graph.windows containsObject:owner]) { DIncrement(graph.exclusions, @"unknownLocalWindow"); [graph.seen removeObject:object]; return; }
    if (!application && !owner.visible) { DIncrement(graph.exclusions, @"invisibleWindow"); return; }
    NSArray* roles = DReadPair(object, @"role", advertised, graph);
    NSString* typedRole = DRole(roles[0]), *legacyRole = DRole(roles[1]);
    NSArray* forbidden = @[NSAccessibilityWebAreaRole, NSAccessibilityTextFieldRole, NSAccessibilityTextAreaRole];
    if ([forbidden containsObject:typedRole] || [forbidden containsObject:legacyRole]) { DIncrement(graph.exclusions, @"webOrEditableRole"); return; }
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) { DIncrement(graph.exclusions, @"accessibilityHidden"); return; }
    if ([object isKindOfClass:NSView.class] && [(NSView*)object isHiddenOrHasHiddenAncestor]) { DIncrement(graph.exclusions, @"nativeViewHidden"); return; }
    UIPermissionDiagnosticNode* node = [UIPermissionDiagnosticNode new];
    node.object = object; node.owner = owner; node.children = [NSMutableIndexSet indexSet];
    node.role = ![typedRole isEqual:@"otherOrMissing"] ? typedRole : legacyRole;
    if (!application) {
      NSArray* enabled = DReadPair(object, @"enabled", advertised, graph);
      NSNumber* enabledValue = DPreferred(enabled, NSNumber.class); node.enabled = enabledValue.boolValue;
      node.nativeButton = [object isKindOfClass:NSButton.class];
      if (node.nativeButton) node.enabled &= [(NSButton*)object isEnabled];
      node.press = [object respondsToSelector:@selector(accessibilityPerformPress)];
      if ([node.role isEqual:NSAccessibilityButtonRole] || node.nativeButton) {
        DIncrement(graph.capabilities, @"buttonCount");
        if (node.nativeButton) DIncrement(graph.capabilities, @"nativeNSButtonCount");
        if (node.enabled) DIncrement(graph.capabilities, @"enabledButtonCount");
        if (node.press) DIncrement(graph.capabilities, @"publicPressCapableButtonCount");
        DIncrement(graph.capabilities, DLegacyPressAdvertised(object) ? @"buttonLegacyPressAdvertised" : @"buttonLegacyPressNotAdvertised");
      }
      for (NSString* key in @[@"description", @"title"]) {
        NSArray* values = DReadPair(object, key, advertised, graph);
        for (id value in values) node.markers |= DMarkers(value);
      }
      if (node.nativeButton) node.markers |= DMarkers([(NSButton*)object title]);
      // Inspect raw role results, never the lossy reporting buckets: an
      // unknown present role or malformed value is not evidence of absence.
      // DReadPair normalizes a genuinely absent result to NSNull. Both public
      // representations must be absent or exactly AXStaticText, with at least
      // one exact match. Native editable/protected controls were pruned above.
      id rawTypedRole = roles[0], rawLegacyRole = roles[1];
      BOOL typedStatic = [rawTypedRole isKindOfClass:NSString.class] && [rawTypedRole isEqual:NSAccessibilityStaticTextRole];
      BOOL legacyStatic = [rawLegacyRole isKindOfClass:NSString.class] && [rawLegacyRole isEqual:NSAccessibilityStaticTextRole];
      BOOL staticText = (typedStatic || legacyStatic) &&
          (rawTypedRole == NSNull.null || typedStatic) && (rawLegacyRole == NSNull.null || legacyStatic);
      if ((typedStatic || legacyStatic) && !staticText) DIncrement(graph.exclusions, @"staticTextRawRoleConflictOrMalformed");
      if (staticText) for (id value in DReadPair(object, @"staticValue", advertised, graph)) node.markers |= DMarkers(value);
      if (![node.role isEqual:NSAccessibilityImageRole]) node.markers &= ~DIcon;
    }
    NSUInteger index = graph.nodes.count; [graph.nodes addObject:node]; [graph.indices setObject:@(index) forKey:object];
    DEdge(graph, parent, index, source); [graph.active addObject:object];
    if (DVisitLimitReached(graph)) { [graph.active removeObject:object]; return; }
    NSArray* childPairs = DReadPair(object, @"children", advertised, graph);
    for (NSUInteger representation = 0; representation < childPairs.count; ++representation) {
      if (DVisitLimitReached(graph)) break;
      id children = childPairs[representation]; if (![children isKindOfClass:NSArray.class]) continue;
      if ([children count] > kNodeLimit) { graph.truncated = YES; DIncrement(graph.exclusions, @"childArrayLimit"); continue; }
      if (application) graph.appChildCount += [children count];
      for (id child in children) {
        if (DVisitLimitReached(graph)) break;
        DVisit(child, owner, (NSInteger)index,
            representation == 0 ? @"typedAXChildren" : @"legacyAXChildren", depth + 1, graph);
      }
    }
    if (!DVisitLimitReached(graph) && [object isKindOfClass:NSWindow.class]) DVisit([(NSWindow*)object contentView], owner, (NSInteger)index, @"nativeContentView", depth + 1, graph);
    else if (!DVisitLimitReached(graph) && [object isKindOfClass:NSView.class]) {
      NSArray* views = [[(NSView*)object subviews] copy];
      if (views.count > kNodeLimit) { graph.truncated = YES; DIncrement(graph.exclusions, @"subviewArrayLimit"); }
      else for (NSView* child in views) {
        if (DVisitLimitReached(graph)) break;
        DVisit(child, owner, (NSInteger)index, @"nativeSubviews", depth + 1, graph);
      }
    }
    [graph.active removeObject:object];
  } @catch (NSException* exception) { (void)exception; DIncrement(graph.exclusions, @"inspectionException"); graph.truncated = YES; }
}
static NSIndexSet* DDescendants(UIPermissionDiagnosticGraph* graph, NSUInteger index) {
  NSMutableIndexSet* found = [NSMutableIndexSet indexSet], *pending = [NSMutableIndexSet indexSetWithIndex:index];
  while (pending.count) {
    NSUInteger current = pending.firstIndex; [pending removeIndex:current];
    if ([found containsIndex:current]) continue;
    [found addIndex:current];
    // Group qualification is diagnostic only and bounded to 128 descendants.
    // Stop at the first excess node instead of rescanning a large graph for
    // every ancestor; whole-graph counters still include every admitted node.
    if (found.count > 128) break;
    [pending addIndexes:graph.nodes[current].children];
    [pending removeIndexes:found];
  }
  return found;
}
static NSDictionary* DiagnosticSummary(void) {
  UIPermissionDiagnosticGraph* graph = [UIPermissionDiagnosticGraph new];
  graph.nodes = [NSMutableArray array]; graph.exclusions = [NSMutableDictionary dictionary];
  graph.getters = [NSMutableDictionary dictionary]; graph.capabilities = [NSMutableDictionary dictionary]; graph.edgesBySource = [NSMutableDictionary dictionary];
  graph.indices = [NSMapTable mapTableWithKeyOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality valueOptions:NSPointerFunctionsStrongMemory];
  graph.seen = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  graph.active = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  graph.windows = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  NSArray* windows = [NSApp.windows copy];
  if (windows.count > 64) { graph.truncated = YES; DIncrement(graph.exclusions, @"windowRootLimit"); }
  else for (NSWindow* window in windows) if (window.visible) [graph.windows addObject:window];
  DVisit(NSApp, nil, -1, @"applicationRoot", 0, graph);
  for (NSWindow* window in graph.windows) {
    if (DVisitLimitReached(graph)) break;
    DVisit(window, window, -1, @"windowRoot", 0, graph);
  }
  NSMutableDictionary* roles = [NSMutableDictionary dictionary], *markers = DZeroMarkers();
  NSMutableDictionary* buttons = DZeroMarkers(), *enabledButtons = DZeroMarkers(), *pressButtons = DZeroMarkers();
  NSMutableDictionary* groupReasons = [NSMutableDictionary dictionary];
  __block NSUInteger groups = 0, bounded = 0, qualified = 0;
  for (NSUInteger index = 0; index < graph.nodes.count; ++index) {
    UIPermissionDiagnosticNode* node = graph.nodes[index]; DIncrement(roles, node.role); DCountMarkers(markers, node.markers);
    BOOL button = node.nativeButton || [node.role isEqual:NSAccessibilityButtonRole];
    if (button) { DCountMarkers(buttons, node.markers); if (node.enabled) DCountMarkers(enabledButtons, node.markers); if (node.press) DCountMarkers(pressButtons, node.markers); }
    if (![node.role isEqual:NSAccessibilityGroupRole]) continue;
    ++groups; NSIndexSet* descendants = DDescendants(graph, index);
    if (descendants.count > 128) { DIncrement(groupReasons, @"over128Descendants"); continue; }
    ++bounded; __block NSUInteger message = 0, icon = 0, deny = 0, allow = 0;
    [descendants enumerateIndexesUsingBlock:^(NSUInteger child, BOOL* stop) {
      (void)stop; UIPermissionDiagnosticNode* member = graph.nodes[child]; if (member.owner != node.owner) return;
      message += !!(member.markers & DMessage); icon += !!(member.markers & DIcon);
      BOOL actionable = member.enabled && (member.nativeButton || ([member.role isEqual:NSAccessibilityButtonRole] && member.press));
      deny += actionable && (member.markers & DNotNow); allow += actionable && (member.markers & DAllow);
    }];
    if (message != 1) DIncrement(groupReasons, message ? @"multipleMessages" : @"missingMessage");
    if (icon != 1) DIncrement(groupReasons, icon ? @"multipleIcons" : @"missingIcon");
    if (deny != 1) DIncrement(groupReasons, deny ? @"multipleNotNowButtons" : @"missingNotNowButton");
    if (allow != 1) DIncrement(groupReasons, allow ? @"multipleAllowButtons" : @"missingAllowButton");
    if (message == 1 && icon == 1 && deny == 1 && allow == 1) ++qualified;
  }
  return @{@"diagnosticOnly":@YES, @"functionalAcceptance":@NO, @"actionQualificationUsed":@NO,
      @"nodeCount":@(graph.nodes.count), @"edgeCount":@(graph.edges), @"visitAttemptCount":@(graph.visits), @"aliasEdgeVisitCount":@(graph.aliases),
      @"cycleCount":@(graph.cycles), @"truncated":@(graph.truncated), @"knownVisibleLocalWindowCount":@(graph.windows.count),
      @"applicationAXChildReferences":@(graph.appChildCount), @"roles":roles, @"wholeGraphFixedMarkers":markers,
      @"wholeGraphButtonFixedMarkers":buttons, @"wholeGraphEnabledButtonFixedMarkers":enabledButtons,
      @"wholeGraphPressCapableButtonFixedMarkers":pressButtons, @"edgeSources":graph.edgesBySource,
      @"exclusionReasons":graph.exclusions, @"capabilityCounts":graph.capabilities, @"typedLegacyComparisons":graph.getters,
      @"diagnosticGroupCount":@(groups), @"diagnosticBoundedGroupCount":@(bounded),
      @"diagnosticQualifiedGroupCountBeforeInnermostFilter":@(qualified), @"diagnosticGroupRejectionReasons":groupReasons,
      @"privacy":@"Only fixed fixture matches, fixed role/type buckets and counts; raw text, object descriptions and pointers are never emitted."};
}
static void HandleDiagnosticRequest(NSDictionary* request) {
  gBusy = YES;
  NSMutableDictionary* response = [@{@"id":request[@"id"], @"nonce":gNonce,
      @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"mainThread":@(NSThread.isMainThread),
      @"scenario":gScenario, @"stage":request[@"stage"], @"operation":request[@"operation"],
      @"diagnosticOnly":@YES, @"functionalAcceptance":@NO, @"actionAttempted":@NO,
      @"actionAttemptCount":@0, @"nativeActionsPerformed":@0, @"nativeMethodReturned":@NO,
      @"causalProfileIsolated":@(ProfileIsolated()), @"causalFixtureVerified":@(CausalRequest(request))} mutableCopy];
  if (![request[@"operation"] isEqual:@"inventory"]) response[@"refusalReason"] = @"diagnostic-only helper refuses every non-inventory operation";
  else if (!ProfileIsolated() || !CausalRequest(request)) response[@"refusalReason"] = @"diagnostics require fresh profile isolation and driver-verified sole fixture target";
  else [response addEntriesFromDictionary:DiagnosticSummary()];
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
}

// Separately opted-in public UI reveal attempt. This does not participate in
// permission AXGroup qualification and never uses the diagnostic graph.
static BOOL gKeyWindowBudgetConsumed;
static BOOL gNativeAppActivationEnabled;
static NSUInteger gNativeAppActivationInvocations;
static BOOL gNativeAppActivationBudgetConsumed;
static BOOL gNativeAppActivationIntentPersisted;
static BOOL gNativeAppActivationReturned;
static BOOL gKeyWindowPreparationEnabled;
static NSUInteger gKeyWindowPreparationInvocations;
static BOOL gKeyWindowIntentPersisted;
static BOOL gSiteCenterAXPressEnabled;
static BOOL gRevealEnabled;
static NSUInteger gRevealAttempts;
static BOOL gRevealBudgetConsumed;
static BOOL gRevealFirstInventoryCaptured;
static BOOL gRevealFirstInventoryEligible;
static NSString* gRevealFirstInventoryID;
static __weak id gRevealSiteCenter;
static __weak NSWindow* gRevealWindow;
static BOOL RevealTrue(id value) {
  return [value isKindOfClass:NSNumber.class] && CFGetTypeID((__bridge CFTypeRef)value) == CFBooleanGetTypeID() && [value boolValue];
}
static NSArray<UIPermissionNode*>* RevealSiteCenters(UIPermissionTree* tree) {
  NSMutableArray* buttons = [NSMutableArray array];
  for (UIPermissionNode* node in tree.nodes) if (node.siteCenter && ActionButton(node) &&
      [tree.windows containsObject:node.window]) [buttons addObject:node];
  return buttons;
}
static NSDictionary* RevealAccounting(NSDictionary* source) {
  NSMutableDictionary* result = [source mutableCopy];
  result[@"actionAttemptCount"] = @(gAttempts + gRevealAttempts);
  result[@"nativeActionsPerformed"] = @(gAttempts + gRevealAttempts);
  result[@"normalActionAttemptCount"] = @(gAttempts);
  result[@"revealAttemptCount"] = @(gRevealAttempts);
  result[@"revealAttemptBudgetConsumed"] = @((BOOL)gRevealBudgetConsumed);
  if (gKeyWindowPreparationEnabled) {
    result[@"preparationInvocationCount"] = @(gKeyWindowPreparationInvocations);
    result[@"totalPhysicalActions"] = @(gAttempts + gRevealAttempts + gKeyWindowPreparationInvocations + gNativeAppActivationInvocations);
    result[@"preparationBudgetConsumed"] = @((BOOL)gKeyWindowBudgetConsumed);
    result[@"preparationIntentPersisted"] = @((BOOL)gKeyWindowIntentPersisted);
    if (gNativeAppActivationEnabled) {
      result[@"activationInvocationCount"] = @(gNativeAppActivationInvocations);
      result[@"activationBudgetConsumed"] = @((BOOL)gNativeAppActivationBudgetConsumed);
      result[@"activationIntentPersisted"] = @((BOOL)gNativeAppActivationIntentPersisted);
      result[@"activationMethodReturned"] = @((BOOL)gNativeAppActivationReturned);
    }
  }
  return result;
}
static NSDictionary* RevealInventory(UIPermissionTree* tree, NSArray<UIPermissionScope*>* scopes, NSDictionary* request) {
  NSArray<UIPermissionNode*>* centers = RevealSiteCenters(tree);
  if (!gRevealFirstInventoryCaptured && [request[@"stage"] isEqual:@"deny"]) {
    gRevealFirstInventoryCaptured = YES;
    gRevealFirstInventoryID = [request[@"id"] copy];
    gRevealFirstInventoryEligible = !tree.truncated && scopes.count == 0 && centers.count == 1 &&
        gPhase == 0 && gAttempts == 0 && ProfileIsolated() && CausalRequest(request);
    if (gRevealFirstInventoryEligible) {
      gRevealSiteCenter = centers.firstObject.object; gRevealWindow = centers.firstObject.window;
    }
  }
  return @{@"promptSiteCenterButtonCount":@(centers.count),
      @"revealFirstInventoryCaptured":@((BOOL)gRevealFirstInventoryCaptured),
      @"revealFirstInventoryEligible":@((BOOL)gRevealFirstInventoryEligible),
      @"revealFirstInventoryID":gRevealFirstInventoryID ?: NSNull.null,
      @"revealAttemptCount":@(gRevealAttempts), @"normalActionAttemptCount":@(gAttempts)};
}
static BOOL RevealLiveSiteCenter(UIPermissionNode* node) {
  id object = node.object;
  if (!node.window || !node.window.visible || object != gRevealSiteCenter || node.window != gRevealWindow ||
      SecureField(object) || ExcludedClass(NSStringFromClass(object_getClass(object)))) return NO;
  @try {
    if ([object isKindOfClass:NSView.class] && ([(NSView*)object window] != node.window || [(NSView*)object isHiddenOrHasHiddenAncestor])) return NO;
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return NO;
    if (![object respondsToSelector:@selector(isAccessibilityEnabled)] || ![(id<NSAccessibility>)object isAccessibilityEnabled]) return NO;
    if ([object isKindOfClass:NSButton.class] && ![(NSButton*)object isEnabled]) return NO;
    BOOL label = [ReadString(object, @selector(accessibilityLabel)) isEqual:@"Site Control Center"] ||
        [ReadString(object, @selector(accessibilityTitle)) isEqual:@"Site Control Center"];
    if ([object isKindOfClass:NSButton.class]) return label || [[(NSButton*)object title] isEqual:@"Site Control Center"];
    return label && [ReadString(object, @selector(accessibilityRole)) isEqual:NSAccessibilityButtonRole] &&
        [object respondsToSelector:@selector(accessibilityPerformPress)];
  } @catch (NSException* exception) { (void)exception; return NO; }
}
static BOOL PersistRevealIntent(NSDictionary* response) {
  NSString* path = [gRequest stringByAppendingString:@".attempt.json"];
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:response options:NSJSONWritingPrettyPrinted error:NULL];
  if (!bytes || ![bytes writeToFile:path options:NSDataWritingAtomic error:NULL]) return NO;
  if (![NSFileManager.defaultManager setAttributes:@{NSFilePosixPermissions:@0600} ofItemAtPath:path error:NULL]) return NO;
  NSData* persisted = [NSData dataWithContentsOfFile:path];
  return [persisted isEqualToData:bytes];
}
static void HandleRevealRequest(NSDictionary* request) {
  gBusy = YES;
  UIPermissionTree* tree = Tree(NO, NO);
  NSArray<UIPermissionScope*>* scopes = PermissionScopes(tree, NO);
  NSArray<UIPermissionNode*>* centers = RevealSiteCenters(tree);
  NSMutableDictionary* response = [@{@"id":request[@"id"], @"nonce":gNonce,
      @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"mainThread":@((BOOL)NSThread.isMainThread),
      @"scenario":gScenario, @"stage":request[@"stage"], @"operation":request[@"operation"],
      @"diagnosticOnly":@NO, @"functionalAcceptance":@NO, @"actionAttempted":@NO,
      @"nativeMethodReturned":@NO, @"nativeActionKind":NSNull.null, @"accessibilityPressReturn":NSNull.null,
      @"causalProfileIsolated":@((BOOL)ProfileIsolated()), @"causalFixtureVerified":@((BOOL)CausalRequest(request)),
      @"notificationPendingVerified":@((BOOL)RevealTrue(request[@"notificationPendingVerified"])),
      @"promptSiteCenterButtonCount":@(centers.count), @"matchingGroupCount":@(scopes.count),
      @"truncated":@((BOOL)tree.truncated), @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"ownershipBasis":@"Unique fixed-label local native button in a fresh isolated process with driver-verified sole fixture and pending Notification request. No exact page-to-window mapping is claimed.",
      @"interpretation":@"One public Site Control Center press is an experimental reveal attempt, not permission resolution. A false press consumes the attempt without fallback; a fixed-origin prompt and canonical denial outcome remain required."} mutableCopy];
  response[@"phase"] = Phase();
  response[@"revealFirstInventoryCaptured"] = @((BOOL)gRevealFirstInventoryCaptured);
  response[@"revealFirstInventoryEligible"] = @((BOOL)gRevealFirstInventoryEligible);
  response[@"revealFirstInventoryID"] = gRevealFirstInventoryID ?: NSNull.null;
  response[@"before"] = RevealAccounting(@{@"matchingGroupCount":@(scopes.count),
      @"promptSiteCenterButtonCount":@(centers.count), @"truncated":@((BOOL)tree.truncated)});
  NSString* refusal = nil;
  if (gDiagnosticOnly || !gEnabled || !gRevealEnabled) refusal = @"canonical permission and canonical-site-center-v1 opt-ins are required; diagnostic mode forbids mutations";
  else if (!ProfileIsolated() || !CausalRequest(request) || !RevealTrue(request[@"notificationPendingVerified"])) refusal = @"fresh isolation, sole-fixture proof and pending Notification proof are required";
  else if (![request[@"stage"] isEqual:@"deny"] || gPhase != 0 || gAttempts != 0) refusal = @"reveal is limited to the initial undecided deny stage";
  else if (!gRevealFirstInventoryCaptured || !gRevealFirstInventoryEligible ||
      ![request[@"revealBaselineInventoryID"] isEqual:gRevealFirstInventoryID]) refusal = @"the first complete inventory did not establish a unique reveal candidate without a permission group";
  else if (gRevealBudgetConsumed || gRevealAttempts != 0) refusal = @"the one reveal attempt is already consumed; replay is forbidden";
  else if (tree.truncated || scopes.count != 0 || centers.count != 1) refusal = @"live tree must remain complete with zero qualified permission groups and one Site Control Center button";
  UIPermissionNode* button = centers.count == 1 ? centers.firstObject : nil;
  const BOOL nativeButton = !gSiteCenterAXPressEnabled && button && [button.object isKindOfClass:NSButton.class];
  NSString* method = nativeButton ? @"performClick" : @"accessibilityPerformPress";
  if (!refusal && (!button || !RevealLiveSiteCenter(button) || !ProfileIsolated() || !CausalRequest(request))) refusal = @"the same live Site Control Center control or causal scope changed";
  if (!refusal && gSiteCenterAXPressEnabled && ![button.object respondsToSelector:@selector(accessibilityPerformPress)]) refusal = @"explicit Site Control Center AX press is unavailable; no method fallback";
  if (!refusal) {
    // Consume the separate budget even if intent persistence fails. The method
    // was chosen before this point; neither false return nor exception retries.
    gRevealBudgetConsumed = YES;
    NSMutableDictionary* intent = [response mutableCopy];
    intent[@"actionAttempted"] = @YES; intent[@"nativeActionKind"] = method;
    intent[@"revealAttemptBudgetConsumed"] = @YES; intent[@"revealAttemptCount"] = @(gRevealAttempts + 1);
    intent[@"normalActionAttemptCount"] = @(gAttempts);
    intent[@"actionAttemptCount"] = @(gAttempts + gRevealAttempts + 1);
    intent[@"nativeActionsPerformed"] = @(gAttempts + gRevealAttempts + 1);
    if (gKeyWindowPreparationEnabled) intent[@"totalPhysicalActions"] = @(gAttempts + gRevealAttempts + gKeyWindowPreparationInvocations + gNativeAppActivationInvocations + 1);
    intent[@"intentBeforeInvocation"] = @YES;
    if (!PersistRevealIntent(intent)) {
      response[@"refusalReason"] = @"intent could not be persisted privately; budget consumed without native invocation";
      response[@"intentPersisted"] = @NO;
    } else {
      ++gRevealAttempts;
      response[@"intentPersisted"] = @YES; response[@"actionAttempted"] = @YES; response[@"nativeActionKind"] = method;
      @try {
        if (nativeButton) [(NSButton*)button.object performClick:nil];
        else response[@"accessibilityPressReturn"] = @((BOOL)[(id<NSAccessibility>)button.object accessibilityPerformPress]);
        response[@"nativeMethodReturned"] = @YES;
      } @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
    }
  } else response[@"refusalReason"] = refusal;
  [response addEntriesFromDictionary:RevealAccounting(@{})];
  response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
}

// One bounded, read-only scan after the actual reveal invocation. Its graph is
// evidence only; no normal action qualification or diagnostic-mode code changes.
static BOOL gPostRevealDiagnosticConsumed;
static void AddPostRevealDiagnostics(NSDictionary* request, NSMutableDictionary* response, UIPermissionTree* tree) {
  if (!request[@"includeRevealDiagnostics"]) return;
  if (!RevealTrue(request[@"includeRevealDiagnostics"]) || gDiagnosticOnly || !gRevealEnabled ||
      gRevealAttempts != 1 || gAttempts != 0 || gPhase != 0 ||
      ![request[@"operation"] isEqual:@"inventory"] || ![request[@"stage"] isEqual:@"deny"] ||
      !ProfileIsolated() || !CausalRequest(request) || gPostRevealDiagnosticConsumed) {
    response[@"refusalReason"] = @"post-reveal diagnostics require one actual reveal, no normal actions, initial deny scope, causal isolation and an unused read-only scan budget";
    return;
  }
  // Consume before reading, including any inspection exception; never rescan.
  gPostRevealDiagnosticConsumed = YES;
  response[@"revealDiagnosticScanCount"] = @1;
  @try {
    NSMutableDictionary* diagnostic = [DiagnosticSummary() mutableCopy];
    NSUInteger key = 0, main = 0;
    for (NSWindow* window in tree.windows) {
      key += window.isKeyWindow ? 1 : 0; main += window.isMainWindow ? 1 : 0;
    }
    NSWindow* siteWindow = gRevealWindow;
    BOOL knownSiteWindow = siteWindow && [tree.windows containsObject:siteWindow];
    diagnostic[@"nativeWindowContext"] = @{
        @"applicationActive":@((BOOL)NSApp.isActive), @"applicationHidden":@((BOOL)NSApp.isHidden),
        @"scopedKnownWindowCount":@(tree.windows.count), @"scopedKeyWindowCount":@(key), @"scopedMainWindowCount":@(main),
        @"scopedNormalTreeTruncated":@((BOOL)tree.truncated), @"siteCenterKnownWindowAvailable":@((BOOL)knownSiteWindow),
        @"siteCenterKnownWindowKey":@((BOOL)(knownSiteWindow && siteWindow.isKeyWindow)),
        @"siteCenterKnownWindowMain":@((BOOL)(knownSiteWindow && siteWindow.isMainWindow))};
    response[@"revealDiagnostic"] = diagnostic;
    response[@"readOnlyRevealDiagnostics"] = @YES;
    response[@"diagnosticOnly"] = @NO;
  } @catch (NSException* exception) {
    (void)exception;
    response[@"refusalReason"] = @"the single read-only post-reveal diagnostic scan raised an inspection exception; no retry is permitted";
  }
}

// Explicit test setup: one public makeKeyWindow on the frozen local candidate.
// Read-only setup inventories bypass RevealInventory and normal qualification.
static BOOL gKeyWindowInventoryCaptured;
static NSString* gKeyWindowInventoryID;
static __weak id gKeyWindowSiteCenter;
static __weak NSWindow* gKeyWindowCandidate;
static NSDictionary* KeyWindowState(void) {
  UIPermissionTree* tree = Tree(NO, NO);
  NSArray<UIPermissionNode*>* centers = RevealSiteCenters(tree);
  UIPermissionNode* center = centers.count == 1 ? centers.firstObject : nil;
  NSWindow* window = gKeyWindowCandidate;
  BOOL known = window && [tree.windows containsObject:window];
  BOOL sameControl = center && center.object == gKeyWindowSiteCenter;
  BOOL sameWindow = center && window && center.window == window;
  BOOL visible = known && window.visible;
  BOOL key = known && window.isKeyWindow, main = known && window.isMainWindow;
  BOOL keyIdentity = window && NSApp.keyWindow == window, mainIdentity = window && NSApp.mainWindow == window;
  BOOL scope = !tree.truncated && centers.count == 1 && known && sameControl && sameWindow && visible;
  return @{@"candidateWindowKnown":@((BOOL)known), @"sameCandidateControl":@((BOOL)sameControl),
      @"sameCandidateWindow":@((BOOL)sameWindow), @"windowVisible":@((BOOL)visible),
      @"canBecomeKeyWindow":@((BOOL)(known && window.canBecomeKeyWindow)),
      @"canBecomeMainWindow":@((BOOL)(known && window.canBecomeMainWindow)),
      @"windowIsKey":@((BOOL)key), @"windowIsMain":@((BOOL)main),
      @"appKeyWindowMatches":@((BOOL)keyIdentity), @"appMainWindowMatches":@((BOOL)mainIdentity),
      @"keyOrMainReady":@((BOOL)(scope && ((key && keyIdentity) || (main && mainIdentity)))),
      @"candidateScopeValid":@((BOOL)scope), @"applicationActive":@((BOOL)NSApp.isActive),
      @"applicationHidden":@((BOOL)NSApp.isHidden), @"truncated":@((BOOL)tree.truncated),
      @"uniqueSiteCenterCount":@(centers.count), @"foregroundPID":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier)};
}
static BOOL PersistKeyWindowIntent(NSDictionary* response) {
  NSString* path = [gRequest stringByAppendingString:@".key-window.attempt.json"];
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:response options:NSJSONWritingPrettyPrinted error:NULL];
  if (!bytes || ![bytes writeToFile:path options:NSDataWritingAtomic error:NULL]) return NO;
  if (![NSFileManager.defaultManager setAttributes:@{NSFilePosixPermissions:@0600} ofItemAtPath:path error:NULL]) return NO;
  return [[NSData dataWithContentsOfFile:path] isEqualToData:bytes];
}
static BOOL PersistNativeAppActivationIntent(NSDictionary* response) {
  NSString* path = [gRequest stringByAppendingString:@".self-activation.attempt.json"];
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:response options:NSJSONWritingPrettyPrinted error:NULL];
  if (!bytes || ![bytes writeToFile:path options:NSDataWritingAtomic error:NULL]) return NO;
  if (![NSFileManager.defaultManager setAttributes:@{NSFilePosixPermissions:@0600} ofItemAtPath:path error:NULL]) return NO;
  return [[NSData dataWithContentsOfFile:path] isEqualToData:bytes];
}
static void HandleKeyWindowRequest(NSDictionary* request) {
  gBusy = YES;
  NSString* operation = request[@"operation"];
  BOOL inventory = [operation isEqual:@"inventory-native-window"];
  BOOL prepare = [operation isEqual:@"prepare-native-window"];
  BOOL activate = [operation isEqual:@"activate-native-app"];
  NSMutableDictionary* response = [@{@"id":request[@"id"], @"nonce":gNonce,
      @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"mainThread":@((BOOL)NSThread.isMainThread),
      @"scenario":gScenario, @"stage":request[@"stage"], @"operation":operation, @"phase":Phase(),
      @"diagnosticOnly":@NO, @"functionalAcceptance":@NO, @"actionAttempted":@NO,
      @"nativeMethodReturned":@NO, @"nativeActionKind":NSNull.null,
      @"preparationIntentPersisted":@NO, @"causalProfileIsolated":@((BOOL)ProfileIsolated()),
      @"causalFixtureVerified":@((BOOL)CausalRequest(request)),
      @"preparationNotificationPendingVerified":@((BOOL)RevealTrue(request[@"preparationNotificationPendingVerified"])),
      @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"ownershipBasis":@"Unique fixed-label Site Control Center and the same weak visible local NSWindow in a fresh isolated process with exact driver fixture proof; no page-to-window mapping is claimed.",
      @"interpretation":@"makeKeyWindow is one experimental public invocation; a void return proves no window state or permission outcome. Same-window key/main observations and canonical prompt/denial/page proof remain required."} mutableCopy];
  NSString* refusal = nil;
  if (!gKeyWindowPreparationEnabled || gDiagnosticOnly || !gEnabled || !gRevealEnabled) refusal = @"explicit normal key-window preparation and permission/reveal opt-ins are required";
  else if (!ProfileIsolated() || !CausalRequest(request) || !RevealTrue(request[@"preparationNotificationPendingVerified"])) refusal = @"fresh causal isolation and exact pending Notification fixture proof are required";
  else if (![request[@"stage"] isEqual:@"deny"] || gPhase != 0 || gAttempts != 0 || gRevealAttempts != 0) refusal = @"window preparation is restricted to initial deny setup before normal/reveal actions";
  if (!refusal && inventory) {
    if (gKeyWindowInventoryCaptured) refusal = @"the first preparation inventory cannot be replaced";
    else {
      UIPermissionTree* tree = Tree(NO, NO);
      NSArray<UIPermissionNode*>* centers = RevealSiteCenters(tree);
      UIPermissionNode* center = centers.count == 1 ? centers.firstObject : nil;
      if (tree.truncated || !center || ![tree.windows containsObject:center.window] || !center.window.visible) refusal = @"one complete unique visible enabled local Site Control Center/window is required";
      else {
        gKeyWindowInventoryCaptured = YES; gKeyWindowInventoryID = [request[@"id"] copy];
        gKeyWindowSiteCenter = center.object; gKeyWindowCandidate = center.window;
      }
    }
  } else if (!refusal && (!gKeyWindowInventoryCaptured || ![request[@"preparationInventoryID"] isEqual:gKeyWindowInventoryID])) refusal = @"the frozen first preparation inventory identity is required";
  NSDictionary* before = KeyWindowState();
  response[@"before"] = before;
  if (!refusal && !RevealTrue(before[@"candidateScopeValid"])) refusal = @"the frozen candidate control/window is no longer uniquely visible and local";
  if (!refusal && activate) {
    response[@"activationAlreadyActiveBefore"] = @((BOOL)RevealTrue(before[@"applicationActive"]));
    response[@"interpretation"] = @"One public self NSApplication.activate request; void return proves no effective activation, key/main state or permission outcome. No other app is targeted, and canonical prompt/denial/page proof remains required.";
    if (!gNativeAppActivationEnabled) refusal = @"explicit canonical-self-activate-v1 opt-in is required";
    else if (gKeyWindowPreparationInvocations != 0) refusal = @"self activation is restricted to before the key-window invocation";
    else if (gNativeAppActivationBudgetConsumed) refusal = @"the single public self-activation budget is consumed; no replay";
    else {
      gNativeAppActivationBudgetConsumed = YES;
      if (@available(macOS 14.0, *)) {
        NSMutableDictionary* intent = [RevealAccounting(response) mutableCopy];
        intent[@"activationBudgetConsumed"] = @YES;
        intent[@"activationInvocationCount"] = @(gNativeAppActivationInvocations + 1);
        intent[@"totalPhysicalActions"] = @(gAttempts + gRevealAttempts + gKeyWindowPreparationInvocations + gNativeAppActivationInvocations + 1);
        intent[@"nativeActionKind"] = @"NSApplication.activate"; intent[@"actionAttempted"] = @YES;
        intent[@"preparationInventoryID"] = gKeyWindowInventoryID; intent[@"intentBeforeInvocation"] = @YES;
        BOOL persisted = PersistNativeAppActivationIntent(intent);
        gNativeAppActivationIntentPersisted = persisted;
        NSDictionary* fresh = KeyWindowState();
        if (!persisted) refusal = @"private self-activation intent persistence failed; budget consumed without invocation";
        else if (!RevealTrue(fresh[@"candidateScopeValid"]) || !ProfileIsolated() || !CausalRequest(request)) refusal = @"frozen window/profile changed after activation intent; budget consumed without invocation";
        else {
          ++gNativeAppActivationInvocations;
          response[@"actionAttempted"] = @YES; response[@"nativeActionKind"] = @"NSApplication.activate";
          @try {
            [NSApp activate];
            gNativeAppActivationReturned = YES; response[@"nativeMethodReturned"] = @YES;
          } @catch (NSException* exception) {
            (void)exception; refusal = @"the one public self-activation invocation raised an exception; no fallback";
          }
        }
      } else refusal = @"public NSApplication.activate requires macOS14 or later; no fallback";
    }
  }
  if (!refusal && prepare && gNativeAppActivationEnabled &&
      (gNativeAppActivationInvocations != 1 || !gNativeAppActivationReturned || !gNativeAppActivationIntentPersisted)) {
    refusal = @"key preparation requires the one persisted and returned self-activation invocation in this opt-in mode";
  }
  if (!refusal && prepare) {
    if (gKeyWindowBudgetConsumed) refusal = @"the single key-window preparation budget is consumed; replay is forbidden";
    else {
      gKeyWindowBudgetConsumed = YES;
      NSMutableDictionary* intent = [RevealAccounting(response) mutableCopy];
      intent[@"preparationBudgetConsumed"] = @YES; intent[@"preparationInvocationCount"] = @(gKeyWindowPreparationInvocations + 1);
      intent[@"totalPhysicalActions"] = @(gAttempts + gRevealAttempts + gKeyWindowPreparationInvocations + gNativeAppActivationInvocations + 1);
      intent[@"nativeActionKind"] = @"NSWindow.makeKeyWindow"; intent[@"actionAttempted"] = @YES;
      intent[@"preparationInventoryID"] = gKeyWindowInventoryID;
      intent[@"intentBeforeInvocation"] = @YES;
      BOOL persisted = PersistKeyWindowIntent(intent);
      gKeyWindowIntentPersisted = persisted;
      response[@"preparationIntentPersisted"] = @((BOOL)persisted);
      NSDictionary* fresh = KeyWindowState();
      if (!persisted) refusal = @"private intent persistence failed; budget consumed without invocation";
      else if (!RevealTrue(fresh[@"candidateScopeValid"]) || !ProfileIsolated() || !CausalRequest(request)) refusal = @"candidate/profile changed after intent persistence; budget consumed without invocation";
      else {
        NSWindow* window = gKeyWindowCandidate;
        ++gKeyWindowPreparationInvocations;
        response[@"actionAttempted"] = @YES; response[@"nativeActionKind"] = @"NSWindow.makeKeyWindow";
        @try { [window makeKeyWindow]; response[@"nativeMethodReturned"] = @YES; }
        @catch (NSException* exception) { (void)exception; refusal = @"the one public makeKeyWindow invocation raised an exception; no fallback"; }
      }
    }
  }
  response[@"preparationCandidateCaptured"] = @((BOOL)gKeyWindowInventoryCaptured);
  response[@"preparationInventoryID"] = gKeyWindowInventoryID ?: NSNull.null;
  response[@"preparationBudgetConsumed"] = @((BOOL)gKeyWindowBudgetConsumed);
  response[@"preparationAlreadyReadyBefore"] = @((BOOL)RevealTrue(before[@"keyOrMainReady"]));
  NSDictionary* after = KeyWindowState(); response[@"after"] = after; response[@"windowState"] = after;
  response[@"preparationReady"] = @((BOOL)RevealTrue(after[@"keyOrMainReady"]));
  if (refusal) response[@"refusalReason"] = refusal;
  [response addEntriesFromDictionary:RevealAccounting(@{})];
  response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
  WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
}

// Supplemental read-only fixture observations never participate in qualification.
static BOOL gFixtureSelectionScanConsumed;
static NSString* gFixtureSelectionFirstID;
@interface UIPermissionSelectionScan : NSObject
@property(nonatomic,strong) NSHashTable* seen;
@property(nonatomic,strong) NSHashTable<NSWindow*>* windows;
@property(nonatomic,strong) NSHashTable<NSWindow*>* fieldWindows;
@property(nonatomic,strong) NSMutableDictionary<NSString*,NSNumber*>* counts;
@property(nonatomic) NSUInteger visits;
@property(nonatomic) BOOL truncated;
@property(nonatomic) NSTimeInterval deadline;
@end
@implementation UIPermissionSelectionScan
@synthesize seen=_seen, windows=_windows, fieldWindows=_fieldWindows, counts=_counts, visits=_visits, truncated=_truncated, deadline=_deadline;
@end
static void SCount(UIPermissionSelectionScan* scan, NSString* key) { scan.counts[key]=@([scan.counts[key] unsignedIntegerValue]+1); }
static BOOL STime(UIPermissionSelectionScan* scan) {
  if (NSProcessInfo.processInfo.systemUptime<=scan.deadline) return YES;
  scan.truncated=YES; return NO;
}
static NSString* SString(id object, SEL selector, UIPermissionSelectionScan* scan) {
  if (!STime(scan)) return nil;
  NSString* result=ReadString(object,selector);
  return STime(scan) ? result : nil;
}
static BOOL SProtectionClear(id object, UIPermissionSelectionScan* scan) {
  if (!STime(scan)) return NO;
  if (ExcludedClass(NSStringFromClass(object_getClass(object))) || SecureField(object)) { SCount(scan,@"secureOrRemoteOrRendererExcluded"); return NO; }
  NSArray* names=DAdvertisedAttributes(object);
  if (!STime(scan)) return NO;
  if (names.count>128) { scan.truncated=YES; SCount(scan,@"attributeLimit"); return NO; }
  BOOL ta=NO,la=NO; id typed=DTyped(object,@"protectedContent",&ta);
  if (!STime(scan)) return NO;
  id legacy=DLegacy(object,@"protectedContent",names,&la);
  if (!STime(scan)) return NO;
  BOOL malformed=(ta && ![typed isKindOfClass:NSNumber.class]) || (la && ![legacy isKindOfClass:NSNumber.class]);
  if ((ta && [typed isKindOfClass:NSNumber.class] && !([typed isEqual:@YES] || [typed isEqual:@NO])) ||
      (la && [legacy isKindOfClass:NSNumber.class] && !([legacy isEqual:@YES] || [legacy isEqual:@NO]))) malformed=YES;
  BOOL conflict=!malformed && ta && la && [typed boolValue]!=[legacy boolValue];
  if (malformed || conflict) { scan.truncated=YES; SCount(scan,@"protectionMalformedOrConflict"); return NO; }
  if ((ta && [typed boolValue]) || (la && [legacy boolValue])) { SCount(scan,@"protectedExcluded"); return NO; }
  return YES;
}
static BOOL SVisibleOwnedView(NSView* view, NSWindow* owner, UIPermissionSelectionScan* scan) {
  if (!STime(scan)) return NO;
  if (!view || view.window!=owner || !owner.visible || ![scan.windows containsObject:owner] || view.isHiddenOrHasHiddenAncestor) return NO;
  NSUInteger depth=0;
  for (NSView* current=view;current;current=current.superview) {
    if (!STime(scan)) return NO;
    if (++depth>kDepthLimit) { scan.truncated=YES; return NO; }
    if (!SProtectionClear(current,scan) || [SString(current,@selector(accessibilityRole),scan) isEqual:NSAccessibilityWebAreaRole]) return NO;
  }
  return YES;
}
static BOOL SLimit(UIPermissionSelectionScan* scan) {
  if (!STime(scan)) return YES;
  if (scan.visits<20000 && scan.seen.count<kNodeLimit) return NO;
  scan.truncated=YES; return YES;
}
static void SVisit(id object, NSWindow* owner, NSUInteger depth, UIPermissionSelectionScan* scan) {
  if (!object || SLimit(scan)) return;
  ++scan.visits;
  if ([scan.seen containsObject:object]) return;
  if (depth>kDepthLimit) { scan.truncated=YES; return; }
  [scan.seen addObject:object];
  @try {
    if (!SProtectionClear(object,scan)) return;
    if ([object isKindOfClass:NSWindow.class]) {
      if (![scan.windows containsObject:object] || ![(NSWindow*)object isVisible]) return;
      owner=object;
    } else if ([object isKindOfClass:NSView.class]) {
      if (!SVisibleOwnedView(object,owner,scan)) return;
    } else if ([object respondsToSelector:@selector(accessibilityWindow)]) {
      id reported=[(id<NSAccessibility>)object accessibilityWindow];
      if (reported && reported!=owner) { SCount(scan,@"foreignWindowExcluded"); return; }
    }
    if (!owner || !owner.visible || ![scan.windows containsObject:owner]) return;
    if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) return;
    NSString* role=SString(object,@selector(accessibilityRole),scan);
    if ([role isEqual:NSAccessibilityWebAreaRole]) { SCount(scan,@"webAreaExcluded"); return; }
    NSString* identifier=SString(object,@selector(accessibilityIdentifier),scan);
    BOOL address=[identifier isEqual:@"contentPanelTextField"];
    if (address) {
      SCount(scan,@"addressIdentifierCount");
      [scan.fieldWindows addObject:owner];
      if ([object isKindOfClass:NSTextField.class] && [role isEqual:NSAccessibilityTextFieldRole] &&
          !SecureField(object) && SVisibleOwnedView(object,owner,scan)) {
        // This is the ONLY editable-value exception: real native, fixed ID/role,
        // verified own visible window and nonsecure/protection-clear ancestry.
        if (!STime(scan)) return;
        NSArray* liveWindows=[NSApp.windows copy];
        if (liveWindows.count>64) { scan.truncated=YES; return; }
        if ([(NSTextField*)object window]!=owner || !owner.visible || [liveWindows indexOfObjectIdenticalTo:owner]==NSNotFound || !STime(scan)) return;
        NSString* value=[(NSTextField*)object stringValue];
        if (!STime(scan)) return;
        if ([value isKindOfClass:NSString.class]) {
          SCount(scan,@"supportedNativeAddressCount");
          if ([value isEqual:[NSString stringWithFormat:@"http://%@/a",gAuthority]]) SCount(scan,@"fixedFixtureURLMatchCount");
        } else { scan.truncated=YES; SCount(scan,@"unsupportedAddressCount"); }
      } else SCount(scan,@"unsupportedAddressCount");
    }
    // No other editable/virtual text value or descendants are inspected.
    if ([object isKindOfClass:NSTextField.class] || [object isKindOfClass:NSTextView.class] ||
        [role isEqual:NSAccessibilityTextFieldRole] || [role isEqual:NSAccessibilityTextAreaRole]) return;
    if ([object respondsToSelector:@selector(accessibilityChildren)]) {
      id children=[(id<NSAccessibility>)object accessibilityChildren];
      if (!STime(scan)) return;
      if (children && ![children isKindOfClass:NSArray.class]) { scan.truncated=YES; return; }
      if ([children count]>kNodeLimit) { scan.truncated=YES; return; }
      for (id child in children) { if (SLimit(scan)) break; SVisit(child,owner,depth+1,scan); }
    }
    if ([object isKindOfClass:NSWindow.class]) SVisit([(NSWindow*)object contentView],owner,depth+1,scan);
    else if ([object isKindOfClass:NSView.class]) {
      NSArray* children=[[(NSView*)object subviews] copy];
      if (!STime(scan)) return;
      if (children.count>kNodeLimit) { scan.truncated=YES; return; }
      for (id child in children) { if (SLimit(scan)) break; SVisit(child,owner,depth+1,scan); }
    }
  } @catch (NSException* exception) { (void)exception; scan.truncated=YES; SCount(scan,@"exceptionCount"); }
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static void SActionFamilies(id object, UIPermissionSelectionScan* scan) {
  if (![object respondsToSelector:@selector(accessibilityActionNames)]) { SCount(scan,@"actionNamesUnavailableCount"); return; }
  if (!STime(scan)) return;
  id names=[object accessibilityActionNames];
  if (!STime(scan)) return;
  if (![names isKindOfClass:NSArray.class] || [names count]>64) { scan.truncated=YES; SCount(scan,@"actionNamesMalformedOrLimit"); return; }
  for (id name in names) {
    if (!STime(scan)) break;
    if (![name isKindOfClass:NSString.class] || [name length]>256) { scan.truncated=YES; SCount(scan,@"actionNameMalformedOrLengthLimit"); continue; }
    if ([name isEqual:NSAccessibilityPressAction]) SCount(scan,@"fixtureButtonAXPressAdvertisementCount");
    if ([name hasPrefix:@"Name:press"]) SCount(scan,@"fixtureButtonNamedPressAdvertisementCount");
  }
}
#pragma clang diagnostic pop
static NSDictionary* SWindowCounts(NSHashTable<NSWindow*>* windows, UIPermissionSelectionScan* scan) {
  NSUInteger key=0,main=0,keyIdentity=0,mainIdentity=0;
  for (NSWindow* window in windows) {
    if (!STime(scan)) break;
    @try {
      key+=window.isKeyWindow; main+=window.isMainWindow;
      keyIdentity+=(window.isKeyWindow && NSApp.keyWindow==window);
      mainIdentity+=(window.isMainWindow && NSApp.mainWindow==window);
    } @catch (NSException* exception) { (void)exception; scan.truncated=YES; SCount(scan,@"exceptionCount"); break; }
  }
  return @{@"windowCount":@(windows.count),@"keyCount":@(key),@"mainCount":@(main),@"sameAppKeyIdentityCount":@(keyIdentity),@"sameAppMainIdentityCount":@(mainIdentity)};
}
static NSDictionary* FixtureSelectionSummary(UIPermissionTree* normalTree) {
  UIPermissionSelectionScan* scan=[UIPermissionSelectionScan new];
  scan.seen=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  scan.windows=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  scan.fieldWindows=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  scan.counts=[NSMutableDictionary dictionary]; scan.deadline=NSProcessInfo.processInfo.systemUptime+0.750;
  NSHashTable<NSWindow*>* buttonWindows=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  NSUInteger buttonCount=0,enabledButtonCount=0;
  @try {
    NSArray<NSWindow*>* roots=[NSApp.windows copy];
    if (roots.count>64) scan.truncated=YES;
    else for (NSWindow* window in roots) if (window.visible) [scan.windows addObject:window];
    for (UIPermissionNode* node in normalTree.nodes) {
      if (!STime(scan)) break;
      id object=node.object;
      if (![node.role isEqual:NSAccessibilityButtonRole] || !node.visible || !node.window.visible || ![scan.windows containsObject:node.window]) continue;
      if (!SProtectionClear(object,scan)) continue;
      if ([object isKindOfClass:NSView.class] && !SVisibleOwnedView(object,node.window,scan)) continue;
      if ([object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden]) continue;
      BOOL fixture=[SString(object,@selector(accessibilityLabel),scan) isEqual:@"launch-slice A"] || [SString(object,@selector(accessibilityTitle),scan) isEqual:@"launch-slice A"];
      if (STime(scan) && [object isKindOfClass:NSButton.class]) fixture|=[[(NSButton*)object title] isEqual:@"launch-slice A"];
      if (!fixture) continue;
      ++buttonCount; enabledButtonCount+=node.enabled; [buttonWindows addObject:node.window];
      SActionFamilies(object,scan);
    }
    for (NSWindow* window in scan.windows) { if (SLimit(scan)) break; SVisit(window,window,0,scan); }
  } @catch (NSException* exception) { (void)exception; scan.truncated=YES; SCount(scan,@"exceptionCount"); }
  NSDictionary* allWindows=SWindowCounts(scan.windows,scan);
  NSDictionary* fixtureWindows=SWindowCounts(buttonWindows,scan);
  NSDictionary* fieldWindows=SWindowCounts(scan.fieldWindows,scan);
  BOOL active=NSApp.isActive,hidden=NSApp.isHidden;
  BOOL complete=!normalTree.truncated && !scan.truncated && STime(scan);
  BOOL uniqueSupported=complete && [scan.counts[@"addressIdentifierCount"] unsignedIntegerValue]==1 &&
      [scan.counts[@"supportedNativeAddressCount"] unsignedIntegerValue]==1 && [scan.counts[@"unsupportedAddressCount"] unsignedIntegerValue]==0;
  id fixedMatch=uniqueSupported ? @((BOOL)([scan.counts[@"fixedFixtureURLMatchCount"] unsignedIntegerValue]==1)) : NSNull.null;
  return @{@"readOnly":@YES,@"complete":@((BOOL)complete),@"truncated":@((BOOL)!complete),
      @"nodeCount":@(scan.seen.count),@"visitCount":@(scan.visits),@"counts":scan.counts,
      @"fixtureTitleButtonCount":@(buttonCount),@"enabledFixtureTitleButtonCount":@(enabledButtonCount),
      @"nativeAddressExactStringMatch":fixedMatch,@"addressObservation":uniqueSupported ? ([fixedMatch boolValue] ? @"fixed-match" : @"fixed-mismatch") : @"unknown",
      @"allKnownVisibleWindows":allWindows,@"fixtureTitleButtonWindows":fixtureWindows,@"addressIdentifierWindows":fieldWindows,
      @"applicationActive":@((BOOL)active),@"applicationHidden":@((BOOL)hidden),
      @"interpretation":@"Fixed native DISPLAY string observations only; address absence, unsupported representation or incomplete scope is unknown. No selected-page mapping or permission prerequisite is claimed."};
}
static void HandleFixtureSelectionInventory(NSDictionary* request) {
  gBusy=YES;
  NSMutableDictionary* response=[@{@"id":request[@"id"],@"nonce":gNonce,@"pid":@(NSProcessInfo.processInfo.processIdentifier),
      @"mainThread":@((BOOL)NSThread.isMainThread),@"scenario":gScenario,@"stage":request[@"stage"],@"operation":request[@"operation"],
      @"functionalAcceptance":@NO,@"actionAttempted":@NO,@"nativeMethodReturned":@NO,@"nativeActionKind":NSNull.null,
      @"actionAttemptCount":@(gAttempts),@"nativeActionsPerformed":@(gAttempts),@"phase":Phase(),
      @"causalProfileIsolated":@((BOOL)ProfileIsolated()),@"causalFixtureVerified":@((BOOL)CausalRequest(request)),
      @"notificationNotStartedVerified":@((BOOL)RevealTrue(request[@"notificationNotStartedVerified"])),
      @"readOnlyFixtureSelection":@NO,@"fixtureSelectionScanCount":@(gFixtureSelectionScanConsumed ? 1 : 0)} mutableCopy];
  NSString* refusal=nil;
  if (!RevealTrue(request[@"includeFixtureSelection"]) || !RevealTrue(request[@"notificationNotStartedVerified"])) refusal=@"strict fixture selection opt-in and pre-Notification proof required";
  else if (gDiagnosticOnly || !gEnabled || gRevealEnabled || gKeyWindowPreparationEnabled || gNativeAppActivationEnabled || gSiteCenterAXPressEnabled) refusal=@"fixture selection observation requires normal canonical actions without reveal/window/activation strategies";
  else if (![request[@"operation"] isEqual:@"inventory"] || ![request[@"stage"] isEqual:@"deny"] || gPhase!=0 || gAttempts!=0 || gRevealAttempts!=0) refusal=@"fixture selection observation is inventory-only before the first permission action";
  else if (!ProfileIsolated() || !CausalRequest(request)) refusal=@"fresh causal isolated fixture proof required";
  else if (gFixtureSelectionScanConsumed) refusal=@"fixture selection observation is one-shot; no rescan";
  if (!refusal) {
    gFixtureSelectionScanConsumed=YES; gFixtureSelectionFirstID=[request[@"id"] copy];
    UIPermissionTree* tree=nil; NSArray<UIPermissionScope*>* scopes=nil;
    NSDictionary* summary=Summary(@"deny",NO,&tree,&scopes); (void)scopes;
    [response addEntriesFromDictionary:summary]; response[@"before"]=summary;
    response[@"fixtureSelectionDiagnostic"]=FixtureSelectionSummary(tree);
    response[@"readOnlyFixtureSelection"]=@YES;
  } else response[@"refusalReason"]=refusal;
  response[@"fixtureSelectionScanCount"]=@(gFixtureSelectionScanConsumed ? 1 : 0);
  response[@"fixtureSelectionFirstID"]=gFixtureSelectionFirstID ?: NSNull.null;
  WriteJSON(response,[gRequest stringByAppendingString:@".response.json"]); gBusy=NO;
}

static void HandleRequest(NSDictionary* request) {
  if (request[@"includeFixtureSelection"]) { HandleFixtureSelectionInventory(request); return; }
  if (gDiagnosticOnly) { HandleDiagnosticRequest(request); return; }
  if ([@[@"inventory-native-window", @"activate-native-app", @"prepare-native-window", @"inspect-native-window"] containsObject:request[@"operation"]]) { HandleKeyWindowRequest(request); return; }
  if ([request[@"operation"] isEqual:@"press-prompt-site-center"]) { HandleRevealRequest(request); return; }
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
  if (gRevealEnabled) {
    [response addEntriesFromDictionary:RevealAccounting(@{})];
    response[@"before"] = RevealAccounting(before);
    if ([operation isEqual:@"inventory"]) [response addEntriesFromDictionary:RevealInventory(tree, scopes, request)];
  }
  if ([operation isEqual:@"inventory"] || inspect) {
    if (inspect && ![stage isEqual:@"site-settings"]) response[@"refusalReason"] = @"site inspection requires site-settings stage";
    if ([operation isEqual:@"inventory"]) AddPostRevealDiagnostics(request, response, tree);
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
    if (gRevealEnabled) [response addEntriesFromDictionary:RevealAccounting(@{})];
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
    response[@"ownerWindowAvailable"] = @((BOOL)(gOwnerWindow != nil));
    if (gRevealEnabled) {
      [response addEntriesFromDictionary:RevealAccounting(@{})];
      response[@"after"] = RevealAccounting(response[@"after"]);
    }
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
  if (([operation isEqual:@"press-prompt-site-center"] && ![stage isEqual:@"deny"]) ||
      ([operation isEqual:@"press-deny"] && ![stage isEqual:@"deny"]) ||
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
    const char* diagnostic = getenv("ARC_UI_PERMISSION_DIAGNOSTIC_ONLY");
    if (diagnostic && ![@(diagnostic) isEqual:@"1"]) return;
    gDiagnosticOnly = diagnostic && [@(diagnostic) isEqual:@"1"];
    gEnabled = !gDiagnosticOnly && enabled && [@(enabled) isEqual:@"canonical-permissions-v1"];
    const char* reveal = getenv("ARC_UI_PERMISSION_REVEAL_ENABLE");
    if (reveal && ![@(reveal) isEqual:@"canonical-site-center-v1"]) return;
    gRevealEnabled = !gDiagnosticOnly && reveal && [@(reveal) isEqual:@"canonical-site-center-v1"];
    const char* keyWindow = getenv("ARC_UI_PERMISSION_KEY_WINDOW_ENABLE");
    if (keyWindow && ![@(keyWindow) isEqual:@"canonical-key-window-v1"]) return;
    gKeyWindowPreparationEnabled = !gDiagnosticOnly && gEnabled && gRevealEnabled && keyWindow && [@(keyWindow) isEqual:@"canonical-key-window-v1"];
    const char* activation = getenv("ARC_UI_PERMISSION_ACTIVATE_ENABLE");
    if (activation && ![@(activation) isEqual:@"canonical-self-activate-v1"]) return;
    gNativeAppActivationEnabled = !gDiagnosticOnly && gKeyWindowPreparationEnabled && activation && [@(activation) isEqual:@"canonical-self-activate-v1"];
    const char* siteCenterAXPress = getenv("ARC_UI_PERMISSION_SITE_CENTER_AX_PRESS");
    if (siteCenterAXPress && ![@(siteCenterAXPress) isEqual:@"canonical-site-center-ax-press-v1"]) return;
    if (siteCenterAXPress && (!gEnabled || !gRevealEnabled || gDiagnosticOnly || gKeyWindowPreparationEnabled || gNativeAppActivationEnabled)) return;
    gSiteCenterAXPressEnabled = siteCenterAXPress && [@(siteCenterAXPress) isEqual:@"canonical-site-center-ax-press-v1"];
    if (isolation && @(isolation).length && @(isolation).length <= 160) gIsolationNonce = @(isolation);
    gIsolationPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Library/Application Support"]
        stringByAppendingPathComponent:[NSString stringWithFormat:@"ArcModAgent-%@/.arc-ui-probe-profile.json", slug]];
    gSeenIDs = [NSMutableSet set]; unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"nonce":gNonce,
          @"mainThread":@(NSThread.isMainThread), @"scenario":gScenario, @"phase":Phase(),
          @"diagnosticOnly":@(gDiagnosticOnly), @"actionCapabilityEnabled":@(gEnabled),
          @"siteCenterAXPressEnabled":@((BOOL)gSiteCenterAXPressEnabled), @"nativeAppActivationEnabled":@((BOOL)gNativeAppActivationEnabled), @"keyWindowPreparationEnabled":@((BOOL)gKeyWindowPreparationEnabled), @"revealCapabilityEnabled":@((BOOL)gRevealEnabled), @"revealAttemptBudget":@(gRevealEnabled ? 1 : 0), @"actionAttemptBudget":@(gDiagnosticOnly ? 0 : Budget()), @"actionAttemptCount":@0,
          @"nativeActionsPerformed":@0, @"causalProfileIsolated":@(ProfileIsolated()), @"functionalAcceptance":@NO},
          [gRequest stringByAppendingString:@".ready.json"]);
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), 250 * NSEC_PER_MSEC, 20 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } }); dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 100 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
