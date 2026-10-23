// Test-only read-only local AppKit inventory. No hooks, actions, text editing,
// completion blocks, remote AX, activation, global input, or permission prompts.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

static NSString* gRequest;
static NSString* gToken;
static NSString* gMessage;
static NSString* gLastID;
static NSUInteger gRequests;
static const NSUInteger kNodeLimit = 1000;
static const NSUInteger kDepthLimit = 18;

@interface UIInventoryNode : NSObject
@property(nonatomic, strong) id object;
@property(nonatomic, strong) NSMutableDictionary* row;
@property(nonatomic) NSInteger parent;
@property(nonatomic) BOOL fixtureMarker;
@property(nonatomic) BOOL button;
@end
@implementation UIInventoryNode
@synthesize object = _object, row = _row, parent = _parent;
@synthesize fixtureMarker = _fixtureMarker, button = _button;
@end

static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* data = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  if (data) [data writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static NSString* LocalString(id object, SEL selector) {
  // Deliberately never inspect accessibilityValue, editable text or secure text.
  if (![object respondsToSelector:selector]) return nil;
  id value = [object valueForKey:NSStringFromSelector(selector)];
  return [value isKindOfClass:NSString.class] ? value : nil;
}
static BOOL Descendant(NSArray<UIInventoryNode*>* nodes, NSUInteger index, NSUInteger ancestor) {
  NSInteger cursor = (NSInteger)index;
  for (NSUInteger depth = 0; cursor >= 0 && depth <= kDepthLimit; ++depth) {
    if ((NSUInteger)cursor == ancestor) return YES;
    cursor = nodes[(NSUInteger)cursor].parent;
  }
  return NO;
}
static void Visit(id object, NSInteger parent, NSUInteger depth, NSString* edge,
                  NSMutableArray<UIInventoryNode*>* nodes, NSHashTable* seen,
                  BOOL* truncated) {
  if (!object || [seen containsObject:object]) return;
  if (nodes.count >= kNodeLimit || depth > kDepthLimit) { *truncated = YES; return; }
  [seen addObject:object];
  UIInventoryNode* node = [UIInventoryNode new];
  node.object = object; node.parent = parent;
  const NSUInteger index = nodes.count;
  NSString* role = nil;
  NSString* className = NSStringFromClass(object_getClass(object));
  const BOOL remote = [className containsString:@"NSAccessibilityRemote"] ||
                      [className containsString:@"NSRemoteView"];
  if (remote || [className containsString:@"WebContents"] || [className containsString:@"RenderWidgetHost"] ||
      [className containsString:@"BrowserAccessibility"]) {
    node.row = [@{@"index":@(index), @"parent":@(parent), @"depth":@(depth), @"edge":edge,
      @"class":className, @"role":NSNull.null, @"enabled":@NO, @"isNSButton":@NO,
      @"respondsToPerformClick":@NO, @"respondsToAccessibilityPerformPress":@NO,
      @"fixtureMarker":@NO, @"excludedRemoteOrRenderer":@YES} mutableCopy];
    [nodes addObject:node];
    return;
  }
  BOOL enabled = YES;
  @try {
    role = LocalString(object, @selector(accessibilityRole));
    if ([role isEqual:NSAccessibilityWebAreaRole]) {
      node.row = [@{@"index":@(index), @"parent":@(parent), @"depth":@(depth), @"edge":edge,
        @"class":className, @"role":role, @"enabled":@NO, @"isNSButton":@NO,
        @"respondsToPerformClick":@NO, @"respondsToAccessibilityPerformPress":@NO,
        @"fixtureMarker":@NO, @"excludedRemoteOrRenderer":@YES} mutableCopy];
      [nodes addObject:node];
      return;
    }
    if ([object respondsToSelector:@selector(isAccessibilityEnabled)]) enabled = [(id<NSAccessibility>)object isAccessibilityEnabled];
    // Markers must be the exact nonce alert message, not a URL/title substring.
    NSString* label = LocalString(object, @selector(accessibilityLabel));
    NSString* title = LocalString(object, @selector(accessibilityTitle));
    node.fixtureMarker = [label isEqual:gMessage] || [title isEqual:gMessage];
    if ([object isKindOfClass:NSTextField.class] && ![object isKindOfClass:NSSecureTextField.class] &&
        ![(NSTextField*)object isEditable]) {
      node.fixtureMarker |= [[(NSTextField*)object stringValue] isEqual:gMessage];
    }
  } @catch (NSException* exception) { (void)exception; }
  node.button = [object isKindOfClass:NSButton.class] || [role isEqual:NSAccessibilityButtonRole];
  node.row = [@{@"index":@(index), @"parent":@(parent), @"depth":@(depth), @"edge":edge,
                @"class":NSStringFromClass(object_getClass(object)), @"role":role ?: NSNull.null,
                @"enabled":@(enabled), @"isNSButton":@([object isKindOfClass:NSButton.class]),
                @"respondsToPerformClick":@([object respondsToSelector:@selector(performClick:)]),
                @"respondsToAccessibilityPerformPress":@([object respondsToSelector:@selector(accessibilityPerformPress)]),
                @"fixtureMarker":@(node.fixtureMarker)} mutableCopy];
  [nodes addObject:node];
  // A page body cannot establish ownership of a native alert container.
  if (remote || [role isEqual:NSAccessibilityWebAreaRole] || [className containsString:@"WebContents"] ||
      [className containsString:@"RenderWidgetHost"]) return;
  @try {
    if ([object isKindOfClass:NSWindow.class]) {
      NSWindow* window = object;
      node.row[@"visible"] = @(window.isVisible);
      node.row[@"isSheet"] = @(window.sheetParent != nil);
      Visit(window.contentView, (NSInteger)index, depth + 1, @"contentView", nodes, seen, truncated);
      for (NSWindow* sheet in window.sheets) Visit(sheet, (NSInteger)index, depth + 1, @"sheet", nodes, seen, truncated);
    } else if ([object isKindOfClass:NSView.class]) {
      for (NSView* child in [(NSView*)object subviews]) Visit(child, (NSInteger)index, depth + 1, @"subview", nodes, seen, truncated);
    }
    if ([object respondsToSelector:@selector(accessibilityChildren)]) {
      id children = [(id<NSAccessibility>)object accessibilityChildren];
      if ([children isKindOfClass:NSArray.class]) {
        NSUInteger count = 0;
        for (id child in children) {
          if (++count > kNodeLimit) { *truncated = YES; break; }
          Visit(child, (NSInteger)index, depth + 1, @"localAccessibilityChild", nodes, seen, truncated);
        }
      }
    }
  } @catch (NSException* exception) {
    node.row[@"inspectionExceptionClass"] = exception.name;
  }
}
static NSDictionary* Inventory(NSString* requestID) {
  NSMutableArray<UIInventoryNode*>* nodes = [NSMutableArray array];
  NSHashTable* seen = [NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPointerPersonality];
  BOOL truncated = NO;
  const pid_t foregroundBefore = NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier;
  NSArray<NSWindow*>* windows = [NSApp.windows copy];
  NSMutableOrderedSet<NSWindow*>* dialogs = [NSMutableOrderedSet orderedSet];
  if (NSApp.modalWindow) [dialogs addObject:NSApp.modalWindow];
  for (NSWindow* window in windows) {
    if (window.sheetParent && window.isVisible) [dialogs addObject:window];
    for (NSWindow* sheet in window.sheets) if (sheet.isVisible) [dialogs addObject:sheet];
  }
  // A real modal/sheet is the candidate cohort. Background sidebar trees and
  // remote view-service windows cannot establish ownership of our alert.
  NSArray<NSWindow*>* candidates = dialogs.count ? dialogs.array : windows;
  for (NSWindow* window in candidates) {
    if (window.isVisible || window == NSApp.modalWindow) Visit(window, -1, 0, @"window", nodes, seen, &truncated);
  }
  NSMutableIndexSet* scopes = [NSMutableIndexSet indexSet];
  NSUInteger markers = 0;
  for (NSUInteger index = 0; index < nodes.count; ++index) {
    if (!nodes[index].fixtureMarker) continue;
    ++markers;
    NSInteger cursor = nodes[index].parent;
    // Limit inferred containers. Never label an entire browser window as a
    // fixture solely because it contains a nonce; require a small subtree with
    // both the observed marker and actual buttons, or an explicit dialog role.
    for (NSUInteger ascent = 0; cursor >= 0 && ascent < 7; ++ascent) {
      UIInventoryNode* candidate = nodes[(NSUInteger)cursor];
      if ([candidate.object isKindOfClass:NSWindow.class] && ![(NSWindow*)candidate.object sheetParent]) break;
      NSUInteger descendants = 0, buttons = 0;
      for (NSUInteger child = 0; child < nodes.count; ++child) {
        if (Descendant(nodes, child, (NSUInteger)cursor)) { ++descendants; buttons += nodes[child].button; }
      }
      if (buttons && buttons <= 16 && descendants <= 128) { [scopes addIndex:(NSUInteger)cursor]; break; }
      cursor = candidate.parent;
    }
  }
  NSMutableArray* rows = [NSMutableArray arrayWithCapacity:nodes.count];
  NSUInteger fixtureButtons = 0, fixtureNSButtons = 0, fixturePressResponders = 0;
  for (NSUInteger index = 0; index < nodes.count; ++index) {
    UIInventoryNode* node = nodes[index];
    __block BOOL scoped = NO;
    [scopes enumerateIndexesUsingBlock:^(NSUInteger scope, BOOL* stop) {
      if (Descendant(nodes, index, scope)) { scoped = YES; *stop = YES; }
    }];
    node.row[@"fixtureContainer"] = @(scoped);
    if (scoped && node.button) {
      ++fixtureButtons;
      fixtureNSButtons += [node.object isKindOfClass:NSButton.class];
      fixturePressResponders += [node.object respondsToSelector:@selector(accessibilityPerformPress)];
      @try {
        NSString* title = [node.object isKindOfClass:NSButton.class] ? [(NSButton*)node.object title] :
            (LocalString(node.object, @selector(accessibilityLabel)) ?: LocalString(node.object, @selector(accessibilityTitle)));
        if (title) node.row[@"fixtureButtonTitle"] = [title substringToIndex:MIN(title.length, (NSUInteger)120)];
      } @catch (NSException* exception) { (void)exception; }
    }
    [rows addObject:node.row];
  }
  return @{@"id":requestID, @"nonce":gToken, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
           @"mainThread":@(NSThread.isMainThread), @"nodes":rows, @"truncated":@(truncated),
           @"fixtureMarkerCount":@(markers), @"fixtureContainerCount":@(scopes.count),
           @"fixtureButtonCount":@(fixtureButtons), @"fixtureNSButtonCount":@(fixtureNSButtons),
           @"fixtureAccessibilityPressResponderCount":@(fixturePressResponders),
           @"foregroundBefore":@(foregroundBefore),
           @"foregroundAfter":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
           @"applicationActive":@(NSApp.active), @"nativeActionsPerformed":@0,
           @"functionalAcceptance":@NO,
           @"interpretation":@"Read-only local capability inventory. Selector presence does not establish that a control can be pressed, that its handler works, or that a dialog completes."};
}
static void Poll(void) {
  if (!NSThread.isMainThread || gRequests >= 12) return;
  NSData* bytes = [NSData dataWithContentsOfFile:gRequest];
  if (!bytes || bytes.length > 4096) return;
  id request = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  if (![identifier isKindOfClass:NSString.class] || ![identifier hasPrefix:[gToken stringByAppendingString:@"-"]] ||
      [identifier isEqual:gLastID] || ![request[@"nonce"] isEqual:gToken] ||
      ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)] ||
      ![request[@"operation"] isEqual:@"inventory"]) return;
  gLastID = identifier; ++gRequests;
  WriteJSON(Inventory(identifier), [gRequest stringByAppendingString:@".response.json"]);
}
__attribute__((constructor)) static void StartUIControlInventory(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    NSString* slug = [app isEqual:@"ArcModAgent-trace156.app"] ? @"trace156" :
        ([app isEqual:@"ArcModAgent-ours156.app"] ? @"ours156" : nil);
    if (!slug || ![executable.lastPathComponent isEqual:@"Arc"] ||
        ![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]]) return;
    const char* path = getenv("ARC_UI_INVENTORY_REQUEST");
    const char* token = getenv("ARC_UI_INVENTORY_NONCE");
    if (!path || !token) return;
    gRequest = @(path); gToken = @(token);
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"ui-control-inventory-request.json"] ||
        ![gToken hasPrefix:@"arc-ui-control-inventory-"] || gToken.length > 96) return;
    gMessage = [@"Arc UI inventory " stringByAppendingString:gToken];
    unsetenv("DYLD_INSERT_LIBRARIES"); // Never follow this probe into renderer/helper processes.
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
                  @"nonce":gToken, @"mainThread":@(NSThread.isMainThread), @"nativeActionsPerformed":@0,
                  @"functionalAcceptance":@NO}, [gRequest stringByAppendingString:@".ready.json"]);
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), 250 * NSEC_PER_MSEC, 20 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 80 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
