// Test-only scoped alert action experiment. No hooks, text editing, engine or
// completion-block access, remote AX, activation, global input, or permission prompts.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

static NSString* gRequest;
static NSString* gToken;
static NSString* gMessage;
static NSMutableSet<NSString*>* gSeenIDs;
static NSUInteger gActionAttempts;
static BOOL gActionInFlight;
static BOOL gActionEnabled;
static NSUInteger gRequests;
static const NSUInteger kNodeLimit = 1000;
static const NSUInteger kDepthLimit = 18;

@interface UIAlertInventoryNode : NSObject
@property(nonatomic, strong) id object;
@property(nonatomic, strong) NSMutableDictionary* row;
@property(nonatomic) NSInteger parent;
@property(nonatomic) BOOL fixtureMarker;
@property(nonatomic) BOOL button;
@end
@implementation UIAlertInventoryNode
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
static BOOL Descendant(NSArray<UIAlertInventoryNode*>* nodes, NSUInteger index, NSUInteger ancestor) {
  NSInteger cursor = (NSInteger)index;
  for (NSUInteger depth = 0; cursor >= 0 && depth <= kDepthLimit; ++depth) {
    if ((NSUInteger)cursor == ancestor) return YES;
    cursor = nodes[(NSUInteger)cursor].parent;
  }
  return NO;
}
static void Visit(id object, NSInteger parent, NSUInteger depth, NSString* edge,
                  NSMutableArray<UIAlertInventoryNode*>* nodes, NSHashTable* seen,
                  BOOL* truncated) {
  if (!object || [seen containsObject:object]) return;
  if (nodes.count >= kNodeLimit || depth > kDepthLimit) { *truncated = YES; return; }
  [seen addObject:object];
  UIAlertInventoryNode* node = [UIAlertInventoryNode new];
  node.object = object; node.parent = parent;
  const NSUInteger index = nodes.count;
  NSString* role = nil;
  NSString* className = NSStringFromClass(object_getClass(object));
  const BOOL protectedField = [object isKindOfClass:NSSecureTextField.class] ||
      ([object isKindOfClass:NSTextField.class] && [(NSTextField*)object isEditable]) ||
      ([object isKindOfClass:NSTextView.class] && [(NSTextView*)object isEditable]);
  const BOOL remote = [className containsString:@"NSAccessibilityRemote"] ||
                      [className containsString:@"NSRemoteView"];
  const BOOL renderer = [className containsString:@"WebContents"] ||
      [className containsString:@"RenderWidgetHost"] || [className containsString:@"BrowserAccessibility"];
  if (protectedField || remote || renderer) {
    node.row = [@{@"index":@(index), @"parent":@(parent), @"depth":@(depth), @"edge":edge,
      @"class":className, @"role":NSNull.null, @"enabled":@NO, @"isNSButton":@NO,
      @"respondsToPerformClick":@NO, @"respondsToAccessibilityPerformPress":@NO,
      @"fixtureMarker":@NO, @"excludedRemoteOrRenderer":@(remote || renderer),
      @"excludedEditableOrSecureField":@(protectedField)} mutableCopy];
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
  NSMutableArray<UIAlertInventoryNode*>* nodes = [NSMutableArray array];
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
      UIAlertInventoryNode* candidate = nodes[(NSUInteger)cursor];
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
    UIAlertInventoryNode* node = nodes[index];
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
           @"applicationActive":@(NSApp.active), @"nativeActionsPerformed":@(gActionAttempts),
           @"actionAttemptCount":@(gActionAttempts),
           @"functionalAcceptance":@NO,
           @"interpretation":@"Read-only local capability inventory. Selector presence does not establish that a control can be pressed, that its handler works, or that a dialog completes."};
}
// Action discovery is stricter than capability inventory: only actual NSViews
// in a small, currently attached real native sheet can establish an action.
@interface UIAlertNativeSheet : NSObject
@property(nonatomic, strong) NSWindow* window;
@property(nonatomic, strong) NSMutableArray<NSButton*>* buttons;
@property(nonatomic, strong) NSMutableArray<NSButton*>* enabledOKButtons;
@property(nonatomic) NSUInteger markers;
@property(nonatomic) NSUInteger nodes;
@property(nonatomic) BOOL truncated;
@property(nonatomic) BOOL pruned;
@end
@implementation UIAlertNativeSheet
@synthesize window = _window, buttons = _buttons, enabledOKButtons = _enabledOKButtons;
@synthesize markers = _markers, nodes = _nodes, truncated = _truncated, pruned = _pruned;
@end

static BOOL AttachedSheet(NSWindow* sheet) {
  NSWindow* parent = sheet.sheetParent;
  return sheet && parent && sheet.isVisible &&
      [parent.sheets indexOfObjectIdenticalTo:sheet] != NSNotFound;
}
static void ScanNativeViews(NSView* view, NSUInteger depth, UIAlertNativeSheet* snapshot) {
  if (!view || snapshot.truncated) return;
  if (snapshot.nodes >= 128 || depth > 14) { snapshot.truncated = YES; return; }
  ++snapshot.nodes;
  NSString* className = NSStringFromClass(object_getClass(view));
  if ([className containsString:@"NSAccessibilityRemote"] || [className containsString:@"NSRemoteView"] ||
      [className containsString:@"WebContents"] || [className containsString:@"RenderWidgetHost"] ||
      [className containsString:@"BrowserAccessibility"]) { snapshot.pruned = YES; return; }
  const BOOL privateField = [view isKindOfClass:NSSecureTextField.class] ||
      ([view isKindOfClass:NSTextField.class] && [(NSTextField*)view isEditable]) ||
      ([view isKindOfClass:NSTextView.class] && [(NSTextView*)view isEditable]);
  if (privateField) return;
  @try {
    if ([LocalString(view, @selector(accessibilityRole)) isEqual:NSAccessibilityWebAreaRole]) {
      snapshot.pruned = YES; return;
    }
    if ([view isKindOfClass:NSButton.class]) [snapshot.buttons addObject:(NSButton*)view];
    // Editable and secure fields are never consulted, including their values.
    const BOOL textField = [view isKindOfClass:NSTextField.class];
    const BOOL protectedField = [view isKindOfClass:NSSecureTextField.class] ||
        (textField && [(NSTextField*)view isEditable]);
    if (!protectedField && ![view isKindOfClass:NSButton.class]) {
      BOOL marker = [LocalString(view, @selector(accessibilityLabel)) isEqual:gMessage] ||
                    [LocalString(view, @selector(accessibilityTitle)) isEqual:gMessage];
      if (textField) marker |= [[(NSTextField*)view stringValue] isEqual:gMessage];
      snapshot.markers += marker;
    }
    for (NSView* child in [view.subviews copy]) ScanNativeViews(child, depth + 1, snapshot);
  } @catch (NSException* exception) { (void)exception; snapshot.truncated = YES; }
}
static NSArray<UIAlertNativeSheet*>* FindNativeSheets(BOOL* truncated) {
  NSMutableOrderedSet<NSWindow*>* sheets = [NSMutableOrderedSet orderedSet];
  NSUInteger inspected = 0;
  for (NSWindow* window in [NSApp.windows copy]) {
    if (++inspected > 64) { *truncated = YES; break; }
    if (AttachedSheet(window)) [sheets addObject:window];
    for (NSWindow* sheet in [window.sheets copy]) {
      if (AttachedSheet(sheet)) [sheets addObject:sheet];
      if (sheets.count > 16) { *truncated = YES; break; }
    }
    if (*truncated) break;
  }
  if (sheets.count > 16) { *truncated = YES; return @[]; }
  NSMutableArray<UIAlertNativeSheet*>* matches = [NSMutableArray array];
  for (NSWindow* sheet in sheets) {
    UIAlertNativeSheet* snapshot = [UIAlertNativeSheet new];
    snapshot.window = sheet;
    snapshot.buttons = [NSMutableArray array];
    snapshot.enabledOKButtons = [NSMutableArray array];
    ScanNativeViews(sheet.contentView, 0, snapshot);
    if (!snapshot.markers) {
      // An incomplete unrelated tree cannot establish that there is no second
      // matching sheet, so fail closed rather than guessing global uniqueness.
      if (snapshot.truncated || snapshot.pruned) *truncated = YES;
      continue;
    }
    // Only read button titles after exact nonce ownership is established.
    for (NSButton* button in snapshot.buttons) {
      if (button.window == sheet && button.enabled && !button.hiddenOrHasHiddenAncestor &&
          [button.title isEqual:@"OK"]) [snapshot.enabledOKButtons addObject:button];
    }
    [matches addObject:snapshot];
  }
  return matches;
}
static NSDictionary* NativeCounts(NSArray<UIAlertNativeSheet*>* matches, BOOL incomplete) {
  NSUInteger markers = 0, buttons = 0, enabledOK = 0, nodes = 0;
  BOOL truncated = incomplete, pruned = NO;
  for (UIAlertNativeSheet* match in matches) {
    markers += match.markers; buttons += match.buttons.count;
    enabledOK += match.enabledOKButtons.count; nodes += match.nodes;
    truncated |= match.truncated; pruned |= match.pruned;
  }
  return @{@"matchingSheetCount":@(matches.count), @"fixtureMarkerCount":@(markers),
           @"fixtureButtonCount":@(buttons), @"fixtureNSButtonCount":@(buttons),
           @"enabledOKButtonCount":@(enabledOK), @"nativeViewCount":@(nodes),
           @"truncated":@(truncated), @"pruned":@(pruned)};
}
static BOOL NativeDescendant(NSView* view, NSView* root) {
  for (NSUInteger depth = 0; view && depth <= 14; ++depth, view = view.superview) {
    if (view == root) return YES;
  }
  return NO;
}
static void PressAlertOK(NSString* requestID) {
  gActionInFlight = YES;
  BOOL incomplete = NO;
  NSArray<UIAlertNativeSheet*>* matches = FindNativeSheets(&incomplete);
  NSDictionary* before = NativeCounts(matches, incomplete);
  NSMutableDictionary* response = [@{@"id":requestID, @"nonce":gToken,
      @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"mainThread":@(NSThread.isMainThread),
      @"operation":@"press-alert-ok", @"before":before, @"actionAttempted":@NO,
      @"actionAttemptCount":@(gActionAttempts), @"nativeActionsPerformed":@(gActionAttempts),
      @"performClickReturned":@NO, @"functionalAcceptance":@NO,
      @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"interpretation":@"A bounded native control call is experimental. Only independent page-state validation can establish that the real app handler completed the alert."} mutableCopy];
  NSString* refusal = nil;
  if (!gActionEnabled) refusal = @"explicit launch action opt-in is absent";
  else if (gActionAttempts) refusal = @"one-process action-attempt budget already consumed";
  else if (incomplete || [before[@"truncated"] boolValue] || [before[@"pruned"] boolValue]) refusal = @"native sheet discovery incomplete or pruned";
  else if (matches.count != 1) refusal = @"exact nonce alert sheet is absent or ambiguous";
  else {
    UIAlertNativeSheet* match = matches.firstObject;
    if (match.nodes > 128 || match.buttons.count > 16 || match.enabledOKButtons.count != 1) refusal = @"small native sheet has no unique enabled concrete OK button";
    else {
      NSWindow* sheet = match.window;
      NSButton* button = match.enabledOKButtons.firstObject;
      if (!AttachedSheet(sheet) || button.window != sheet || !button.enabled || button.hiddenOrHasHiddenAncestor ||
          ![button.title isEqual:@"OK"] || !NativeDescendant(button, sheet.contentView)) {
        refusal = @"native sheet/button changed before invocation";
      } else {
        // Set the guard before calling AppKit: performClick can reenter the run
        // loop and the app's real handler can synchronously close this sheet.
        ++gActionAttempts;
        response[@"actionAttempted"] = @YES;
        response[@"actionAttemptCount"] = @(gActionAttempts);
        response[@"nativeActionsPerformed"] = @(gActionAttempts);
        @try {
          [button performClick:nil];
          response[@"performClickReturned"] = @YES;
        } @catch (NSException* exception) { response[@"performClickExceptionClass"] = exception.name; }
      }
    }
  }
  if (refusal) response[@"refusalReason"] = refusal;
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 150 * NSEC_PER_MSEC), dispatch_get_main_queue(), ^{
    BOOL afterIncomplete = NO;
    NSArray<UIAlertNativeSheet*>* afterMatches = FindNativeSheets(&afterIncomplete);
    response[@"after"] = NativeCounts(afterMatches, afterIncomplete);
    response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
    WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]);
    gActionInFlight = NO;
  });
}
static void Poll(void) {
  if (!NSThread.isMainThread || gRequests >= 12 || gActionInFlight) return;
  NSData* bytes = [NSData dataWithContentsOfFile:gRequest];
  if (!bytes || bytes.length > 4096) return;
  id request = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"];
  NSString* operation = request[@"operation"];
  if (![identifier isKindOfClass:NSString.class] || identifier.length > 160 ||
      ![identifier hasPrefix:[gToken stringByAppendingString:@"-"]] || [gSeenIDs containsObject:identifier] ||
      ![request[@"nonce"] isEqual:gToken] || ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)] ||
      ![@[@"inventory", @"press-alert-ok"] containsObject:operation]) return;
  [gSeenIDs addObject:identifier]; ++gRequests;
  if ([operation isEqual:@"press-alert-ok"]) PressAlertOK(identifier);
  else WriteJSON(Inventory(identifier), [gRequest stringByAppendingString:@".response.json"]);
}
__attribute__((constructor)) static void StartUIAlertActionProbe(void) {
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
    gSeenIDs = [NSMutableSet set];
    const char* actionEnable = getenv("ARC_UI_ALERT_ACTION_ENABLE");
    gActionEnabled = actionEnable && [@(actionEnable) isEqual:@"press-alert-ok"];
    unsetenv("DYLD_INSERT_LIBRARIES"); // Never follow this probe into renderer/helper processes.
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
                  @"nonce":gToken, @"mainThread":@(NSThread.isMainThread), @"nativeActionsPerformed":@0,
                  @"functionalAcceptance":@NO, @"actionCapabilityEnabled":@(gActionEnabled)},
                [gRequest stringByAppendingString:@".ready.json"]);
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), 250 * NSEC_PER_MSEC, 20 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 80 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
