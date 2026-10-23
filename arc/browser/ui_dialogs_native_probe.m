// Test-only canonical JavaScript-dialog native-control probe. No engine hooks,
// completion-block access, delegate calls, AX actions, global input or activation.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

static NSString* gRequest;
static NSString* gNonce;
static NSString* gAuthorityTitle;
static NSString* gScenario;
static NSString* gProfileIsolationNonce;
static NSString* gProfileIsolationPath;
static NSMutableSet<NSString*>* gSeenIDs;
static NSUInteger gRequestCount;
static NSUInteger gButtonAttempts;
static NSUInteger gTextEditAttempts;
static NSUInteger gPhase;
static BOOL gEnabled;
static BOOL gBusy;
static BOOL gPromptReadback;

@interface UIDialogsSheetSnapshot : NSObject
@property(nonatomic, strong) NSWindow* window;
@property(nonatomic, strong) NSMutableArray<NSButton*>* buttons;
@property(nonatomic, strong) NSMutableArray<NSTextField*>* editableFields;
@property(nonatomic, strong) NSMutableArray<NSTextField*>* secureFields;
@property(nonatomic) NSUInteger nodes;
@property(nonatomic) NSUInteger titles;
@property(nonatomic) NSUInteger messages;
@property(nonatomic) BOOL truncated;
@property(nonatomic) BOOL pruned;
@end
@implementation UIDialogsSheetSnapshot
@synthesize window = _window, buttons = _buttons, editableFields = _editableFields;
@synthesize secureFields = _secureFields;
@synthesize nodes = _nodes, titles = _titles, messages = _messages;
@synthesize truncated = _truncated, pruned = _pruned;
@end

static BOOL StageAllowed(NSString* stage) {
  if ([gScenario isEqual:@"dialogs-beforeunload"]) return [@[@"beforeunload-stay", @"beforeunload-leave"] containsObject:stage];
  if ([gScenario isEqual:@"dialogs-auth-cancel"]) return [stage isEqual:@"auth-cancel"];
  return [@[@"alert", @"confirm", @"prompt"] containsObject:stage];
}
static BOOL OperationAllowed(NSString* operation) {
  if ([operation isEqual:@"inventory"]) return YES;
  if ([gScenario isEqual:@"dialogs-beforeunload"]) return [@[@"press-beforeunload-cancel", @"press-beforeunload-ok"] containsObject:operation];
  if ([gScenario isEqual:@"dialogs-auth-cancel"]) return [operation isEqual:@"press-auth-cancel"];
  return [@[@"press-alert-ok", @"press-confirm-cancel", @"set-prompt-text", @"press-prompt-ok"] containsObject:operation];
}
static NSUInteger ButtonBudget(void) {
  if ([gScenario isEqual:@"dialogs-beforeunload"]) return 2;
  if ([gScenario isEqual:@"dialogs-auth-cancel"]) return 1;
  return 3;
}
static NSString* Phase(void) {
  if ([gScenario isEqual:@"dialogs-beforeunload"]) return gPhase == 0 ? @"beforeunload-stay" : (gPhase == 1 ? @"beforeunload-leave" : @"complete");
  if ([gScenario isEqual:@"dialogs-auth-cancel"]) return gPhase == 0 ? @"auth-cancel" : @"complete";
  switch (gPhase) {
    case 0: return @"alert";
    case 1: return @"confirm";
    case 2: return @"prompt";
    case 3: return @"prompt-edited";
    default: return @"complete";
  }
}
static NSString* StageTitle(NSString* stage) {
  if (!StageAllowed(stage)) return nil;
  if ([stage hasPrefix:@"beforeunload-"]) return @"Leave site?";
  if ([stage isEqual:@"auth-cancel"]) return @"Sign in to 127.0.0.1";
  return gAuthorityTitle;
}
static NSString* StageMessage(NSString* stage) {
  if (!StageAllowed(stage)) return nil;
  if ([stage isEqual:@"alert"]) return @"ArcCore alert 7f3a";
  if ([stage isEqual:@"confirm"]) return @"ArcCore confirm 7f3a";
  if ([stage isEqual:@"prompt"]) return @"ArcCore prompt 7f3a";
  if ([stage hasPrefix:@"beforeunload-"]) return @"Changes you made may not be saved.";
  if ([stage isEqual:@"auth-cancel"]) return @"Your connection to this site is private.";
  return nil;
}
static void WriteJSON(NSDictionary* value, NSString* path) {
  NSData* bytes = [NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted error:NULL];
  if (bytes) [bytes writeToFile:path options:NSDataWritingAtomic error:NULL];
}
static NSData* ReadBoundedFile(NSString* path) {
  if (!path) return nil;
  NSFileHandle* file = [NSFileHandle fileHandleForReadingAtPath:path];
  if (!file) return nil;
  NSData* bytes = [file readDataUpToLength:4097 error:NULL];
  [file closeAndReturnError:NULL];
  return bytes.length && bytes.length <= 4096 ? bytes : nil;
}
static BOOL GenericScenario(void) {
  return ![gScenario isEqual:@"dialogs"];
}
static BOOL CausalProfileIsolated(void) {
  if (!gProfileIsolationNonce.length || !gProfileIsolationPath) return NO;
  NSData* bytes = ReadBoundedFile(gProfileIsolationPath);
  if (!bytes) return NO;
  id marker = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  return [marker isKindOfClass:NSDictionary.class] && [marker[@"nonce"] isEqual:gProfileIsolationNonce];
}
static BOOL RequestCausallyVerified(NSDictionary* request) {
  id value = request[@"causalFixtureVerified"];
  return [value isKindOfClass:NSNumber.class] &&
      CFGetTypeID((__bridge CFTypeRef)value) == CFBooleanGetTypeID() && [value boolValue] &&
      gProfileIsolationNonce.length && [request[@"isolatedProfileNonce"] isEqual:gProfileIsolationNonce];
}
static NSString* LocalString(id object, SEL selector) {
  if (![object respondsToSelector:selector]) return nil;
  id result = [object valueForKey:NSStringFromSelector(selector)];
  return [result isKindOfClass:NSString.class] ? result : nil;
}
static BOOL Attached(NSWindow* sheet) {
  NSWindow* parent = sheet.sheetParent;
  return sheet && parent && sheet.visible && [parent.sheets indexOfObjectIdenticalTo:sheet] != NSNotFound;
}
static BOOL NativeDescendant(NSView* view, NSView* root) {
  for (NSUInteger depth = 0; view && depth <= 14; ++depth, view = view.superview) if (view == root) return YES;
  return NO;
}
static void CountMarker(NSString* text, NSString* title, NSString* message, BOOL* titleFound, BOOL* messageFound) {
  if ([text isEqual:title]) *titleFound = YES;
  if ([text isEqual:message]) *messageFound = YES;
}
static BOOL SecureTextField(NSView* view) {
  return [view isKindOfClass:NSSecureTextField.class] ||
      ([view isKindOfClass:NSTextField.class] &&
       [[(NSTextField*)view cell] isKindOfClass:NSSecureTextFieldCell.class]);
}
static void Scan(NSView* view, NSUInteger depth, NSString* title, NSString* message, UIDialogsSheetSnapshot* snapshot) {
  if (!view || snapshot.truncated) return;
  if (snapshot.nodes >= 128 || depth > 14) { snapshot.truncated = YES; return; }
  ++snapshot.nodes;
  NSString* className = NSStringFromClass(object_getClass(view));
  if ([className containsString:@"NSAccessibilityRemote"] || [className containsString:@"NSRemoteView"] ||
      [className containsString:@"WebContents"] || [className containsString:@"RenderWidgetHost"] ||
      [className containsString:@"BrowserAccessibility"]) { snapshot.pruned = YES; return; }
  // Secure fields/cells are counted by reference only, never queried for text
  // or traversed. Nonsecure editable contents are consulted solely by the one
  // canonical prompt setter after both ownership markers have been proved.
  if (SecureTextField(view)) { [snapshot.secureFields addObject:(NSTextField*)view]; return; }
  if ([view isKindOfClass:NSTextField.class] && [(NSTextField*)view isEditable]) {
    [snapshot.editableFields addObject:(NSTextField*)view];
    return;
  }
  if ([view isKindOfClass:NSTextView.class] && [(NSTextView*)view isEditable]) return;
  @try {
    if ([LocalString(view, @selector(accessibilityRole)) isEqual:NSAccessibilityWebAreaRole]) {
      snapshot.pruned = YES; return;
    }
    if ([view isKindOfClass:NSButton.class]) {
      [snapshot.buttons addObject:(NSButton*)view];
    } else {
      BOOL titleFound = NO, messageFound = NO;
      CountMarker(LocalString(view, @selector(accessibilityLabel)), title, message, &titleFound, &messageFound);
      CountMarker(LocalString(view, @selector(accessibilityTitle)), title, message, &titleFound, &messageFound);
      if ([view isKindOfClass:NSTextField.class]) CountMarker([(NSTextField*)view stringValue], title, message, &titleFound, &messageFound);
      snapshot.titles += titleFound; snapshot.messages += messageFound;
    }
    NSUInteger children = 0;
    for (NSView* child in [view.subviews copy]) {
      if (++children > 128 || snapshot.truncated) { snapshot.truncated = YES; break; }
      Scan(child, depth + 1, title, message, snapshot);
    }
  } @catch (NSException* exception) { (void)exception; snapshot.truncated = YES; }
}
static NSArray<UIDialogsSheetSnapshot*>* FindSheets(NSString* stage, BOOL* incomplete) {
  NSMutableOrderedSet<NSWindow*>* sheets = [NSMutableOrderedSet orderedSet];
  NSUInteger inspected = 0, inspectedSheets = 0;
  for (NSWindow* window in [NSApp.windows copy]) {
    if (++inspected > 64) { *incomplete = YES; break; }
    if (Attached(window)) [sheets addObject:window];
    for (NSWindow* sheet in [window.sheets copy]) {
      if (++inspectedSheets > 64) { *incomplete = YES; break; }
      if (Attached(sheet)) [sheets addObject:sheet];
      if (sheets.count > 16) { *incomplete = YES; break; }
    }
    if (*incomplete) break;
  }
  if (sheets.count > 16) { *incomplete = YES; return @[]; }
  NSMutableArray<UIDialogsSheetSnapshot*>* matching = [NSMutableArray array];
  for (NSWindow* sheet in sheets) {
    UIDialogsSheetSnapshot* snapshot = [UIDialogsSheetSnapshot new];
    snapshot.window = sheet; snapshot.buttons = [NSMutableArray array]; snapshot.editableFields = [NSMutableArray array];
    snapshot.secureFields = [NSMutableArray array];
    Scan(sheet.contentView, 0, StageTitle(stage), StageMessage(stage), snapshot);
    if (snapshot.titles && snapshot.messages) [matching addObject:snapshot];
    else if (snapshot.truncated || snapshot.pruned) *incomplete = YES;
  }
  return matching;
}
static BOOL UsableButton(NSButton* button, NSWindow* sheet, NSString* title) {
  return Attached(sheet) && button.window == sheet && button.enabled && !button.hiddenOrHasHiddenAncestor &&
      [button.title isEqual:title] && NativeDescendant(button, sheet.contentView);
}
static NSArray<NSButton*>* ButtonsWithTitle(UIDialogsSheetSnapshot* snapshot, NSString* title) {
  NSMutableArray<NSButton*>* found = [NSMutableArray array];
  // Read titles only after fixture authority and the exact stage message match.
  for (NSButton* button in snapshot.buttons) if (UsableButton(button, snapshot.window, title)) [found addObject:button];
  return found;
}
static NSArray<NSString*>* AllowedButtonTitles(NSString* operation) {
  if ([operation isEqual:@"press-beforeunload-cancel"]) return @[@"Stay", @"Stay on Page", @"Cancel"];
  if ([operation isEqual:@"press-beforeunload-ok"]) return @[@"Leave", @"Leave Page", @"OK"];
  if ([@[@"press-confirm-cancel", @"press-auth-cancel"] containsObject:operation]) return @[@"Cancel"];
  return @[@"OK"];
}
static NSArray<NSButton*>* ButtonsForOperation(UIDialogsSheetSnapshot* snapshot, NSString* operation) {
  NSMutableArray<NSButton*>* found = [NSMutableArray array];
  NSArray<NSString*>* allowed = AllowedButtonTitles(operation);
  for (NSButton* button in snapshot.buttons) {
    // Only fixed-whitelist titles within a proved matching sheet are eligible.
    NSString* title = button.title;
    if ([allowed containsObject:title] && UsableButton(button, snapshot.window, title)) [found addObject:button];
  }
  return found;
}
static NSUInteger SecureFieldCount(UIDialogsSheetSnapshot* snapshot) {
  NSUInteger count = 0;
  for (NSTextField* field in snapshot.secureFields) {
    // Presence and ownership checks only. Never consult stringValue,
    // accessibilityValue, editor, cell contents, target or delegate.
    if (SecureTextField(field) && Attached(snapshot.window) && field.window == snapshot.window &&
        !field.hiddenOrHasHiddenAncestor && NativeDescendant(field, snapshot.window.contentView)) ++count;
  }
  return count;
}
static NSDictionary* Counts(NSString* stage, NSArray<UIDialogsSheetSnapshot*>* matches, BOOL incomplete) {
  NSUInteger titles = 0, messages = 0, buttons = 0, fields = 0, secure = 0, nodes = 0, ok = 0, cancel = 0, signIn = 0;
  BOOL truncated = incomplete, pruned = NO;
  for (UIDialogsSheetSnapshot* snapshot in matches) {
    titles += snapshot.titles; messages += snapshot.messages; buttons += snapshot.buttons.count;
    fields += snapshot.editableFields.count; secure += SecureFieldCount(snapshot); nodes += snapshot.nodes;
    ok += ButtonsWithTitle(snapshot, @"OK").count; cancel += ButtonsWithTitle(snapshot, @"Cancel").count;
    if ([gScenario isEqual:@"dialogs-auth-cancel"]) signIn += ButtonsWithTitle(snapshot, @"Sign in").count;
    truncated |= snapshot.truncated; pruned |= snapshot.pruned;
  }
  NSMutableDictionary* buttonCounts = [[gScenario isEqual:@"dialogs-auth-cancel"]
      ? @{@"Cancel":@(cancel), @"Sign in":@(signIn)} : @{@"OK":@(ok), @"Cancel":@(cancel)} mutableCopy];
  if ([gScenario isEqual:@"dialogs-beforeunload"]) {
    for (NSString* title in @[@"Stay", @"Stay on Page", @"Leave", @"Leave Page"]) {
      NSUInteger count = 0;
      for (UIDialogsSheetSnapshot* snapshot in matches) count += ButtonsWithTitle(snapshot, title).count;
      buttonCounts[title] = @(count);
    }
  }
  return @{@"scenario":gScenario, @"stage":stage, @"matchingSheetCount":@(matches.count), @"markerCount":@(messages),
      @"titleCount":@(titles), @"buttonCounts":buttonCounts, @"secureFieldCount":@(secure),
      @"causalProfileIsolated":@(CausalProfileIsolated()),
      @"buttonCount":@(buttons), @"fixtureNSButtonCount":@(buttons), @"editableFieldCount":@(fields),
      @"nativeViewCount":@(nodes), @"truncated":@(truncated), @"pruned":@(pruned), @"phase":Phase(),
      @"actionAttemptCount":@(gButtonAttempts), @"textEditAttemptCount":@(gTextEditAttempts)};
}
static NSMutableDictionary* BaseResponse(NSString* identifier, NSString* operation, NSString* stage) {
  return [@{@"id":identifier, @"nonce":gNonce, @"pid":@(NSProcessInfo.processInfo.processIdentifier),
      @"mainThread":@(NSThread.isMainThread), @"scenario":gScenario, @"operation":operation, @"stage":stage,
      @"functionalAcceptance":@NO, @"phase":Phase(), @"actionAttempted":@NO, @"textEditAttempted":@NO,
      @"causalProfileIsolated":@(CausalProfileIsolated()), @"causalFixtureVerified":@NO,
      @"ownershipBasis":GenericScenario() ? @"Causal isolated-profile ownership plus driver-verified sole fixture target; no exact native-window mapping is claimed." : @"Exact fixture authority and canonical stage markers in a native sheet.",
      @"actionAttemptCount":@(gButtonAttempts), @"textEditAttemptCount":@(gTextEditAttempts),
      @"nativeActionsPerformed":@(gButtonAttempts), @"performClickReturned":@NO, @"setterReturned":@NO,
      @"textReadbackMatchesFixedReply":@(gPromptReadback),
      @"foregroundBefore":@(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier),
      @"interpretation":@"Test-only canonical native control attempts. Native setter readback and performClick return do not prove app state propagation or JavaScript completion; the driver must independently validate page results."} mutableCopy];
}
static NSString* PhaseRefusal(NSString* operation, NSString* stage) {
  if (!gEnabled) return @"explicit canonical-dialogs-v1 launch opt-in is absent";
  if (!StageAllowed(stage) || !OperationAllowed(operation)) return @"stage or operation is outside configured scenario";
  if ([gScenario isEqual:@"dialogs-beforeunload"]) {
    if ([operation isEqual:@"press-beforeunload-cancel"]) return gPhase == 0 && [stage isEqual:@"beforeunload-stay"] ? nil : @"beforeunload stay operation is out of phase";
    if ([operation isEqual:@"press-beforeunload-ok"]) return gPhase == 1 && [stage isEqual:@"beforeunload-leave"] ? nil : @"beforeunload leave operation is out of phase";
    return @"unsupported beforeunload operation";
  }
  if ([gScenario isEqual:@"dialogs-auth-cancel"]) return gPhase == 0 && [operation isEqual:@"press-auth-cancel"] && [stage isEqual:@"auth-cancel"] ? nil : @"auth cancel operation is out of phase";
  if ([operation isEqual:@"press-alert-ok"]) return gPhase == 0 && [stage isEqual:@"alert"] ? nil : @"alert operation is out of phase";
  if ([operation isEqual:@"press-confirm-cancel"]) return gPhase == 1 && [stage isEqual:@"confirm"] ? nil : @"confirm operation is out of phase";
  if ([operation isEqual:@"set-prompt-text"]) return gPhase == 2 && [stage isEqual:@"prompt"] && !gTextEditAttempts ? nil : @"prompt setter is out of phase or already attempted";
  if ([operation isEqual:@"press-prompt-ok"]) return gPhase == 3 && [stage isEqual:@"prompt"] && gTextEditAttempts == 1 && gPromptReadback ? nil : @"prompt OK requires one prior verified native setter readback";
  return @"unsupported operation";
}
static void HandleDialogRequest(NSString* identifier, NSString* operation, NSString* stage, NSDictionary* request) {
  gBusy = YES;
  BOOL incomplete = NO;
  NSArray<UIDialogsSheetSnapshot*>* matches = FindSheets(stage, &incomplete);
  NSDictionary* before = Counts(stage, matches, incomplete);
  NSMutableDictionary* response = BaseResponse(identifier, operation, stage);
  [response addEntriesFromDictionary:before]; response[@"before"] = before;
  response[@"causalFixtureVerified"] = @(RequestCausallyVerified(request));
  if ([operation isEqual:@"inventory"]) {
    response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
    WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO; return;
  }
  NSString* refusal = PhaseRefusal(operation, stage);
  if (!refusal && GenericScenario() && (!CausalProfileIsolated() || !RequestCausallyVerified(request))) refusal = @"generic dialog requires isolated profile marker and driver-verified sole fixture target request";
  if (!refusal && (incomplete || [before[@"truncated"] boolValue] || [before[@"pruned"] boolValue])) refusal = @"native discovery incomplete or pruned";
  if (!refusal && matches.count != 1) refusal = @"authority and stage message do not identify one unique sheet";
  UIDialogsSheetSnapshot* match = matches.count == 1 ? matches.firstObject : nil;
  if (!refusal && (match.titles != 1 || match.messages != 1)) refusal = @"fixture sheet has ambiguous authority or stage markers";
  if (!refusal && (match.nodes > 128 || match.buttons.count > 16 || !Attached(match.window))) refusal = @"native sheet is not small and currently attached";
  if (!refusal && [gScenario isEqual:@"dialogs-auth-cancel"] && SecureFieldCount(match) != 1) refusal = @"auth scope does not contain exactly one visible concrete secure field";
  if (!refusal && [operation isEqual:@"set-prompt-text"]) {
    if (match.editableFields.count != 1) refusal = @"prompt has no unique concrete editable nonsecure NSTextField";
    else {
      NSTextField* field = match.editableFields.firstObject;
      if (SecureTextField(field) || !field.editable || !field.enabled ||
          field.hiddenOrHasHiddenAncestor || field.window != match.window ||
          !Attached(match.window) || !NativeDescendant(field, match.window.contentView)) {
        refusal = @"prompt field ancestry or enabled state changed";
      } else if (![field.stringValue isEqual:@"initial"]) {
        refusal = @"proven fixture prompt field does not contain expected initial text";
        response[@"initialMatchesExpected"] = @NO;
      } else {
        response[@"initialMatchesExpected"] = @YES;
        ++gTextEditAttempts; gPhase = 3; response[@"textEditAttempted"] = @YES;
        @try {
          // Deliberately do not invoke target/actions, delegates, binding hooks,
          // editing notifications or completion blocks to force propagation.
          [field setStringValue:@"fixed prompt reply"];
          response[@"setterReturned"] = @YES;
          gPromptReadback = [field.stringValue isEqual:@"fixed prompt reply"];
        } @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
      }
    }
  } else if (!refusal) {
    NSArray<NSButton*>* buttons = ButtonsForOperation(match, operation);
    if (gButtonAttempts >= ButtonBudget()) refusal = @"scenario button invocation budget exhausted";
    else if (buttons.count != 1) refusal = @"fixture sheet has no unique enabled concrete button with required title";
    else {
      NSButton* button = buttons.firstObject;
      NSString* title = button.title;
      if (![AllowedButtonTitles(operation) containsObject:title] || !UsableButton(button, match.window, title) ||
          (GenericScenario() && (!CausalProfileIsolated() || !RequestCausallyVerified(request))) ||
          ([gScenario isEqual:@"dialogs-auth-cancel"] && SecureFieldCount(match) != 1)) refusal = @"button ancestry, enabled state or required secure field presence changed";
      else {
        response[@"selectedButtonTitle"] = title;
        ++gButtonAttempts;
        // Consume the phase before AppKit can reenter the run loop. A failed
        // native invocation is never replayed and cannot synthesize page state.
        if ([gScenario isEqual:@"dialogs-beforeunload"]) ++gPhase;
        else if ([gScenario isEqual:@"dialogs-auth-cancel"]) gPhase = 1;
        else gPhase = [operation isEqual:@"press-alert-ok"] ? 1 : ([operation isEqual:@"press-confirm-cancel"] ? 2 : 4);
        response[@"actionAttempted"] = @YES;
        @try { [button performClick:nil]; response[@"performClickReturned"] = @YES; }
        @catch (NSException* exception) { response[@"nativeExceptionClass"] = exception.name; }
      }
    }
  }
  if (refusal) response[@"refusalReason"] = refusal;
  response[@"phase"] = Phase(); response[@"actionAttemptCount"] = @(gButtonAttempts);
  response[@"nativeActionsPerformed"] = @(gButtonAttempts); response[@"textEditAttemptCount"] = @(gTextEditAttempts);
  response[@"textReadbackMatchesFixedReply"] = @(gPromptReadback);
  // Persist the attempted phase before a later process exit can interrupt the
  // post-action inventory. This receipt is not a successful response/completion.
  WriteJSON(response, [gRequest stringByAppendingString:@".attempt.json"]);
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 150 * NSEC_PER_MSEC), dispatch_get_main_queue(), ^{
    BOOL afterIncomplete = NO;
    NSArray<UIDialogsSheetSnapshot*>* afterMatches = FindSheets(stage, &afterIncomplete);
    response[@"after"] = Counts(stage, afterMatches, afterIncomplete);
    response[@"foregroundAfter"] = @(NSWorkspace.sharedWorkspace.frontmostApplication.processIdentifier);
    WriteJSON(response, [gRequest stringByAppendingString:@".response.json"]); gBusy = NO;
  });
}
static void Poll(void) {
  if (!NSThread.isMainThread || gBusy || gRequestCount >= 48) return;
  NSData* bytes = ReadBoundedFile(gRequest);
  if (!bytes) return;
  id request = [NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  if (![request isKindOfClass:NSDictionary.class]) return;
  NSString* identifier = request[@"id"], *operation = request[@"operation"], *stage = request[@"stage"];
  if (![identifier isKindOfClass:NSString.class] || identifier.length > 160 ||
      ![identifier hasPrefix:[gNonce stringByAppendingString:@"-"]] || [gSeenIDs containsObject:identifier] ||
      ![request[@"nonce"] isEqual:gNonce] || ![request[@"pid"] isEqual:@(NSProcessInfo.processInfo.processIdentifier)] ||
      !StageAllowed(stage) || !OperationAllowed(operation)) return;
  [gSeenIDs addObject:identifier]; ++gRequestCount;
  HandleDialogRequest(identifier, operation, stage, request);
}
__attribute__((constructor)) static void StartUIDialogsNativeProbe(void) {
  @autoreleasepool {
    NSString* executable = NSProcessInfo.processInfo.arguments.firstObject;
    NSString* app = [[[executable stringByDeletingLastPathComponent] stringByDeletingLastPathComponent] stringByDeletingLastPathComponent].lastPathComponent;
    NSString* slug = [app isEqual:@"ArcModAgent-trace156.app"] ? @"trace156" : ([app isEqual:@"ArcModAgent-ours156.app"] ? @"ours156" : nil);
    if (!slug || ![executable.lastPathComponent isEqual:@"Arc"] ||
        ![NSBundle.mainBundle.bundleIdentifier isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]]) return;
    const char* path = getenv("ARC_UI_DIALOG_REQUEST"), *nonce = getenv("ARC_UI_DIALOG_NONCE");
    const char* authority = getenv("ARC_UI_DIALOG_FIXTURE_AUTHORITY"), *enabled = getenv("ARC_UI_DIALOG_ACTION_ENABLE");
    const char* scenario = getenv("ARC_UI_DIALOG_SCENARIO");
    gScenario = scenario ? @(scenario) : @"dialogs";
    if (![@[@"dialogs", @"dialogs-beforeunload", @"dialogs-auth-cancel"] containsObject:gScenario]) return;
    if (!path || !nonce || !authority) return;
    gRequest = @(path); gNonce = @(nonce);
    NSString* hostPort = @(authority);
    NSURL* parsed = [NSURL URLWithString:[@"http://" stringByAppendingString:hostPort]];
    NSInteger port = parsed.port.integerValue;
    if (!gRequest.isAbsolutePath || ![gRequest.lastPathComponent isEqual:@"ui-dialogs-request.json"] ||
        ![gNonce hasPrefix:@"arc-ui-dialogs-"] || gNonce.length > 96 ||
        ![parsed.host isEqual:@"127.0.0.1"] || port < 1024 || port > 65535 ||
        ![hostPort isEqual:[NSString stringWithFormat:@"127.0.0.1:%ld", (long)port]]) return;
    gAuthorityTitle = [hostPort stringByAppendingString:@" says"];
    const char* isolationNonce = getenv("ARC_UI_PROFILE_ISOLATION_NONCE");
    if (isolationNonce) {
      NSString* isolated = @(isolationNonce);
      if (isolated.length && isolated.length <= 160) gProfileIsolationNonce = isolated;
    }
    gProfileIsolationPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Library/Application Support"]
        stringByAppendingPathComponent:[NSString stringWithFormat:@"ArcModAgent-%@/.arc-ui-probe-profile.json", slug]];
    gEnabled = enabled && [@(enabled) isEqual:@"canonical-dialogs-v1"];
    gSeenIDs = [NSMutableSet set];
    unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(), ^{
      WriteJSON(@{@"installed":@YES, @"pid":@(NSProcessInfo.processInfo.processIdentifier), @"nonce":gNonce,
          @"mainThread":@(NSThread.isMainThread), @"scenario":gScenario, @"actionCapabilityEnabled":@(gEnabled), @"phase":Phase(),
          @"buttonAttemptBudget":@(ButtonBudget()), @"textEditAttemptBudget":@([gScenario isEqual:@"dialogs"] ? 1 : 0),
          @"causalProfileIsolated":@(CausalProfileIsolated()),
          @"actionAttemptCount":@0, @"nativeActionsPerformed":@0, @"textEditAttemptCount":@0,
          @"functionalAcceptance":@NO}, [gRequest stringByAppendingString:@".ready.json"]);
      dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC), 250 * NSEC_PER_MSEC, 20 * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, ^{ @autoreleasepool { Poll(); } });
      dispatch_resume(timer);
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 80 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{ dispatch_source_cancel(timer); });
    });
  }
}
