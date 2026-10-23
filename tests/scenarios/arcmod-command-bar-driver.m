// WRITE-only morning adaptation of frozen omnibox current-editor public AX proof.
// Source SHA256 fa990a89dab483d8453e0a54aa0e580bcce442e6cbb9ff5bf8ac22b7561fd4ff.
// No native runtime validation has been performed for this adaptation.
// No engine APIs, delegate/notification injection, input, activation or swizzles.
#import <AppKit/AppKit.h>
#import <objc/runtime.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <libproc.h>
#include <string.h>

#define JSONBool(value) @((BOOL)(value))

static NSString *gRequest, *gNonce, *gFixture, *gBundle;
static NSMutableSet *gSeen;
static dispatch_source_t gTimer;
static NSUInteger gRequests, gAttempts;
static BOOL gConsumed, gHandling;
static const NSUInteger kRequests=8, kNodes=2048, kDepth=32, kChildren=128, kWindows=32;
static const NSTimeInterval kScanSeconds=0.150;

// BEGIN OFFLINE GUARDS: pure value/path checks, no UI access.
static NSString* CanonicalPath(NSString* path) {
  if (![path isKindOfClass:NSString.class] || !path.isAbsolutePath) return nil;
  char* resolved=realpath(path.fileSystemRepresentation,NULL);
  if (!resolved) return nil;
  NSString* result=[NSString stringWithUTF8String:resolved]; free(resolved); return result;
}
static BOOL TrueBoolean(id value) {
  return [value isKindOfClass:NSNumber.class] && CFGetTypeID((__bridge CFTypeRef)value)==CFBooleanGetTypeID() && [value boolValue];
}
static BOOL PositiveInteger(id value) {
  if (![value isKindOfClass:NSNumber.class] || CFGetTypeID((__bridge CFTypeRef)value)==CFBooleanGetTypeID()) return NO;
  const char* type=[value objCType];
  return type && type[0] && !type[1] && strchr("sSiIlLqQ",type[0]) && [value longLongValue]>0 && [value unsignedLongLongValue]<=(unsigned long long)NSIntegerMax;
}
static BOOL ValidNonce(NSString* nonce) {
  return [nonce isKindOfClass:NSString.class] && [nonce hasPrefix:@"omnibox-action-"] && nonce.length>=20 && nonce.length<=96 &&
    [nonce rangeOfCharacterFromSet:[[NSCharacterSet characterSetWithCharactersInString:@"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-"] invertedSet]].location==NSNotFound;
}
static BOOL BundleIdentity(NSString* expected, NSString* actual, NSString* executable, NSString* processExecutable, NSDictionary* info) {
  if (!expected.length || ![expected isEqual:actual]) return NO;
  NSString* slug=[actual.lastPathComponent isEqual:@"ArcModAgent-trace156.app"] ? @"trace156" :
    [actual.lastPathComponent isEqual:@"ArcModAgent-ours156.app"] ? @"ours156" : nil;
  NSString* required=[actual stringByAppendingPathComponent:@"Contents/MacOS/Arc"];
  return slug && [executable isEqual:required] && [processExecutable isEqual:required] &&
    [info[@"ArcModAgentSession"] isEqual:slug] &&
    [info[@"CFBundleName"] isEqual:[@"ArcModAgent-" stringByAppendingString:slug]] &&
    [info[@"CFBundleIdentifier"] isEqual:[@"company.thebrowser.Browser.mod.agent." stringByAppendingString:slug]];
}
static NSString* ValidRequestDirectory(NSString* request) {
  if (![request isKindOfClass:NSString.class] || !request.isAbsolutePath || ![request.lastPathComponent isEqual:@"action-request.json"]) return nil;
  NSString* directory=CanonicalPath(request.stringByDeletingLastPathComponent); struct stat st={0};
  NSString* root=CanonicalPath(NSProcessInfo.processInfo.environment[@"ARC_SCENARIO_COMMAND_BAR_ROOT"]);
  if (!root || [root isEqual:@"/"] || ![directory isEqual:[root stringByAppendingPathComponent:@"arcmod-command-bar-driver"]] ||
      stat(directory.fileSystemRepresentation,&st) ||
      !S_ISDIR(st.st_mode) || st.st_uid!=geteuid() || (st.st_mode&0077)) return nil;
  return directory;
}
static BOOL RequestIdentity(id request) {
  if (![request isKindOfClass:NSDictionary.class]) return NO;
  id identifier=request[@"id"], operation=request[@"operation"];
  return [identifier isKindOfClass:NSString.class] && [identifier length]<=160 &&
    [identifier hasPrefix:[gNonce stringByAppendingString:@"-"]] &&
    [request[@"nonce"] isEqual:gNonce] && PositiveInteger(request[@"pid"]) && [request[@"pid"] longLongValue]==getpid() &&
    [request[@"fixtureURL"] isEqual:gFixture] &&
    TrueBoolean(request[@"isolatedProfileVerified"]) && TrueBoolean(request[@"freshProfileVerified"]) && TrueBoolean(request[@"causalFixtureVerified"]) &&
    ([operation isEqual:@"inventory"] || [operation isEqual:@"setFixtureURL"] || [operation isEqual:@"observeFixtureSuggestions"]);
}
static BOOL ReadbackAllowed(BOOL owned, BOOL secure, BOOL hidden, BOOL accessibilityHidden) {
  return owned && !secure && !hidden && !accessibilityHidden;
}
// END OFFLINE GUARDS

static void Diagnostic(const char* code) { fprintf(stderr,"[omnibox-action] gate=%s pid=%d\n",code,getpid()); fflush(stderr); }
static NSDictionary* RecorderCursor(void) {
  const char* path=getenv("ARC_RECORD_PATH");
  if(!path || path[0]!='/') return @{@"available":@NO};
  int fd=open(path,O_RDONLY|O_NOFOLLOW|O_NONBLOCK);if(fd<0) return @{@"available":@NO};
  struct stat st={0};BOOL valid=fstat(fd,&st)==0 && S_ISREG(st.st_mode) && st.st_uid==geteuid() && st.st_size>=0;
  close(fd);
  return valid ? @{@"available":@YES,@"byteOffset":@(st.st_size),@"device":@(st.st_dev),@"inode":@(st.st_ino)} : @{@"available":@NO};
}
static BOOL Write(NSDictionary* value, NSString* suffix) {
  NSData* data=[NSJSONSerialization dataWithJSONObject:value options:NSJSONWritingPrettyPrinted|NSJSONWritingSortedKeys error:NULL];
  NSString* path=[gRequest stringByAppendingString:suffix];
  if (!data || ![data writeToFile:path options:NSDataWritingAtomic error:NULL] ||
      ![NSFileManager.defaultManager setAttributes:@{NSFilePosixPermissions:@0600} ofItemAtPath:path error:NULL]) {
    Diagnostic("receipt_write_failed"); return NO;
  }
  return YES;
}
static NSData* ReadRequest(void) {
  int fd=open(gRequest.fileSystemRepresentation,O_RDONLY|O_NOFOLLOW|O_NONBLOCK); if(fd<0) return nil;
  struct stat st={0};
  if(fstat(fd,&st) || !S_ISREG(st.st_mode) || st.st_uid!=geteuid() || (st.st_mode&0077) || st.st_size<=0 || st.st_size>4096) { close(fd); return nil; }
  char bytes[4097]; ssize_t length=read(fd,bytes,sizeof(bytes)); close(fd);
  return length==st.st_size ? [NSData dataWithBytes:bytes length:(NSUInteger)length] : nil;
}
static BOOL Excluded(id object) {
  if ([object isProxy]) return YES;
  NSString* name=NSStringFromClass(object_getClass(object));
  for(NSString* marker in @[@"NSAccessibilityRemote",@"NSRemoteView",@"AXUIElement",@"WebContents",@"RenderWidgetHost",@"BrowserAccessibility",@"WKWebView",@"WKContentView",@"WebHTMLView"])
    if([name containsString:marker]) return YES;
  if([object respondsToSelector:@selector(accessibilityRole)] && [[(id<NSAccessibility>)object accessibilityRole] isEqual:NSAccessibilityWebAreaRole]) return YES;
  return NO;
}
static NSString* Role(id object) {
  id value=[object respondsToSelector:@selector(accessibilityRole)] ? [(id<NSAccessibility>)object accessibilityRole] : nil;
  return [value isKindOfClass:NSString.class] ? value : @"";
}
static BOOL Secure(id object) {
  if([object isKindOfClass:NSSecureTextField.class] || [object isKindOfClass:NSSecureTextFieldCell.class]) return YES;
  NSString* name=NSStringFromClass(object_getClass(object));
  id subrole=[object respondsToSelector:@selector(accessibilitySubrole)] ? [(id<NSAccessibility>)object accessibilitySubrole] : nil;
  return [name localizedCaseInsensitiveContainsString:@"secure"] || [name localizedCaseInsensitiveContainsString:@"password"] ||
    ([subrole isKindOfClass:NSString.class] && [subrole localizedCaseInsensitiveContainsString:@"secure"]);
}
static BOOL AXAllowed(id object) {
  return [object respondsToSelector:@selector(setAccessibilityValue:)] &&
    [object respondsToSelector:@selector(isAccessibilitySelectorAllowed:)] &&
    [(id<NSAccessibility>)object isAccessibilitySelectorAllowed:@selector(setAccessibilityValue:)];
}
static BOOL AXHidden(id object) {
  return [object respondsToSelector:@selector(isAccessibilityHidden)] && [(id<NSAccessibility>)object isAccessibilityHidden];
}
static BOOL Descendant(NSView* view, NSWindow* window) {
  if(view.window!=window) return NO;
  NSView* root=window.contentView;
  for(NSUInteger n=0;view && n<=64;++n,view=view.superview) if(view==root) return YES;
  return NO;
}
static BOOL Eligible(NSWindow* window) {
  return window && !Excluded(window) && window.visible && !window.sheetParent && ![window isKindOfClass:NSPanel.class] &&
    window.contentView && (window.styleMask&NSWindowStyleMaskTitled);
}
@interface ScenarioCommandBarActionScan : NSObject
@property(strong) NSMutableArray<NSTextField*>* fields;
@property(strong) NSHashTable* seen;
@property(strong) NSWindow* window;
@property NSTimeInterval deadline;
@property BOOL truncated;
@property BOOL budgetExceeded;
@end
@implementation ScenarioCommandBarActionScan
@end
static BOOL Budget(ScenarioCommandBarActionScan* scan) {
  if(NSProcessInfo.processInfo.systemUptime>=scan.deadline) { scan.truncated=YES;scan.budgetExceeded=YES; }
  return !scan.truncated;
}
static void Visit(NSView* view, NSUInteger depth, ScenarioCommandBarActionScan* scan) {
  if(!view || !Budget(scan) || [scan.seen containsObject:view]) return;
  if(scan.seen.count>=kNodes || depth>kDepth) { scan.truncated=YES;return; }
  [scan.seen addObject:view];
  if(Excluded(view) || !Descendant(view,scan.window)) return;
  if([view isKindOfClass:NSTextField.class] && [[view accessibilityIdentifier] isEqual:@"commandBarTextField"])
    [scan.fields addObject:(NSTextField*)view];
  NSUInteger children=0;
  for(NSView* child in view.subviews) {
    if(++children>kChildren) { scan.truncated=YES;break; }
    Visit(child,depth+1,scan);if(!Budget(scan)) break;
  }
}
static NSDictionary* SafeValue(NSTextField* field) {
  NSString* value=field.stringValue;
  if([value isEqual:@""] || [value isEqual:gFixture]) return @{@"value":value,@"valueAllowed":@YES,@"equalsFixture":JSONBool([value isEqual:gFixture])};
  return @{@"value":NSNull.null,@"valueAllowed":@NO,@"equalsFixture":@NO,@"valuePolicy":@"unexpected value redacted"};
}
static NSDictionary* FieldMetadata(NSTextField* field, NSWindow* window) {
  id cell=field.cell; id editor=[field currentEditor];
  BOOL secure=Secure(field)||Secure(cell);
  BOOL hidden=field.isHiddenOrHasHiddenAncestor||AXHidden(field);
  NSMutableDictionary* value=[@{@"identifier":@"commandBarTextField",@"concreteNSTextField":@YES,
    @"windowNumber":@(window.windowNumber),@"editable":JSONBool(field.editable),@"enabled":JSONBool(field.enabled),
    @"hidden":JSONBool(hidden),@"secure":JSONBool(secure),@"descendant":JSONBool(Descendant(field,window)),
    @"cellIsNSTextFieldCell":JSONBool([cell isKindOfClass:NSTextFieldCell.class]),
    @"cellOwned":JSONBool([cell isKindOfClass:NSTextFieldCell.class] && [(NSTextFieldCell*)cell controlView]==field),
    @"cellRole":Role(cell),@"cellSetterAllowed":JSONBool(AXAllowed(cell)),
    @"currentEditorIsNSTextView":JSONBool([editor isKindOfClass:NSTextView.class]),
    @"editorRole":Role(editor),@"editorSetterAllowed":JSONBool(AXAllowed(editor)),
    @"editorSameWindow":JSONBool([editor isKindOfClass:NSTextView.class] && [(NSTextView*)editor window]==window)} mutableCopy];
  if(!secure && !hidden) value[@"valueState"]=SafeValue(field);
  return value;
}
@interface ScenarioCommandBarSuggestionScan : NSObject
@property(strong) NSHashTable* seen;
@property(strong) NSWindow* window;
@property NSTimeInterval deadline;
@property NSUInteger roots;
@property NSUInteger exact;
@property NSUInteger reads;
@property NSUInteger redacted;
@property BOOL truncated;
@property BOOL budgetExceeded;
@end
@implementation ScenarioCommandBarSuggestionScan
@end
static BOOL ProofBudget(ScenarioCommandBarSuggestionScan* scan) {
  if(NSProcessInfo.processInfo.systemUptime>=scan.deadline) { scan.truncated=YES;scan.budgetExceeded=YES; }
  return !scan.truncated;
}
static void ProofVisit(id object, NSUInteger depth, BOOL inside, ScenarioCommandBarSuggestionScan* scan) {
  if(!object || !ProofBudget(scan) || [scan.seen containsObject:object]) return;
  if(scan.seen.count>=kNodes || depth>kDepth) { scan.truncated=YES;return; }
  [scan.seen addObject:object];
  if(Excluded(object) || Secure(object)) return;
  if([object isKindOfClass:NSTextField.class] && Secure([(NSTextField*)object cell])) return;
  if([object isKindOfClass:NSWindow.class] && object!=scan.window) return;
  if([object isKindOfClass:NSView.class] && (!Descendant(object,scan.window) || [(NSView*)object isHiddenOrHasHiddenAncestor])) return;
  if(AXHidden(object)) return;
  if([object respondsToSelector:@selector(accessibilityWindow)]) {
    id owner=[(id<NSAccessibility>)object accessibilityWindow];if(owner && owner!=scan.window) return;
  }
  id identifier=[object respondsToSelector:@selector(accessibilityIdentifier)] ? [(id<NSAccessibility>)object accessibilityIdentifier] : nil;
  BOOL root=[identifier isEqual:@"commandBarSuggestions"];
  if(root) ++scan.roots;
  // Match only a genuine descendant of the suggestions identifier, never the
  // identifier object itself. All nonmatching values are discarded/redacted.
  if(inside && [Role(object) isEqual:NSAccessibilityStaticTextRole] && [object respondsToSelector:@selector(accessibilityValue)]) {
    id value=[(id<NSAccessibility>)object accessibilityValue];++scan.reads;
    if([value isKindOfClass:NSString.class] && [value isEqual:gFixture]) ++scan.exact;
    else ++scan.redacted;
  }
  if(!ProofBudget(scan)) return;
  if([object respondsToSelector:@selector(accessibilityChildren)]) {
    id children=[(id<NSAccessibility>)object accessibilityChildren];
    if([children isKindOfClass:NSArray.class]) {
      NSUInteger count=0;
      for(id child in children) {
        if(++count>kChildren) { scan.truncated=YES;break; }
        ProofVisit(child,depth+1,inside||root,scan);if(!ProofBudget(scan)) break;
      }
    }
  }
  if(!ProofBudget(scan)) return;
  if([object isKindOfClass:NSWindow.class]) ProofVisit([(NSWindow*)object contentView],depth+1,inside||root,scan);
  else if([object isKindOfClass:NSView.class]) {
    NSUInteger count=0;
    for(NSView* child in [(NSView*)object subviews]) {
      if(++count>kChildren) { scan.truncated=YES;break; }
      ProofVisit(child,depth+1,inside||root,scan);if(!ProofBudget(scan)) break;
    }
  }
}
static NSDictionary* ObserveSuggestions(NSWindow* window) {
  ScenarioCommandBarSuggestionScan* scan=[ScenarioCommandBarSuggestionScan new];scan.window=window;
  scan.seen=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  NSTimeInterval started=NSProcessInfo.processInfo.systemUptime;scan.deadline=started+kScanSeconds;
  ProofVisit(window,0,NO,scan);
  ProofBudget(scan);
  return @{@"suggestionsRootCount":@(scan.roots),@"exactFixtureStaticTextCount":@(scan.exact),
    @"exactFixtureStaticTextObserved":JSONBool(scan.exact>0),@"staticTextValuesRead":@(scan.reads),@"unexpectedValuesRedacted":@(scan.redacted),
    @"truncated":JSONBool(scan.truncated),@"budgetExceeded":JSONBool(scan.budgetExceeded),@"nodesSeen":@(scan.seen.count),
    @"elapsedMs":@((NSProcessInfo.processInfo.systemUptime-started)*1000.0),@"functionalAcceptance":@NO};
}
static NSMutableDictionary* Base(NSDictionary* request) {
  return [@{@"id":request[@"id"],@"nonce":gNonce,@"pid":@(getpid()),@"bundle":gBundle,@"fixtureURL":gFixture,
    @"operation":request[@"operation"],@"mainThread":JSONBool(NSThread.isMainThread),@"functionalAcceptance":@NO,
    @"engineCallsPerformed":@0,@"nativeActionsPerformed":@(gAttempts),@"textSetterAttempts":@(gAttempts),@"actualSetterCalls":@(gAttempts),
    @"actionConsumed":JSONBool(gConsumed),@"noRetry":@YES} mutableCopy];
}
static NSDictionary* Process(NSDictionary* request) {
  NSMutableDictionary* receipt=Base(request);
  if([request[@"operation"] isEqual:@"observeFixtureSuggestions"] && gAttempts!=1) {
    receipt[@"status"]=@"gap-no-setter-attempt-for-postaction-observation";return receipt;
  }
  NSTimeInterval deadline=NSProcessInfo.processInfo.systemUptime+kScanSeconds;
  NSMutableArray* windows=[NSMutableArray array]; NSMutableArray* summaries=[NSMutableArray array];
  BOOL truncated=NO; NSUInteger count=0;
  for(NSWindow* window in NSApp.windows) {
    if(++count>kWindows || NSProcessInfo.processInfo.systemUptime>=deadline) { truncated=YES;break; }
    if(Eligible(window)) { [windows addObject:window];[summaries addObject:@{@"windowNumber":@(window.windowNumber),@"main":JSONBool(window==NSApp.mainWindow),@"key":JSONBool(window==NSApp.keyWindow)}]; }
  }
  receipt[@"eligibleWindows"]=summaries;
  id wanted=request[@"windowNumber"]; NSWindow* window=nil; NSUInteger matches=0;
  if(PositiveInteger(wanted)) {
    for(NSWindow* candidate in windows) {
      if(candidate.windowNumber==[wanted integerValue]) { window=candidate;++matches; }
    }
  }
  BOOL action=[request[@"operation"] isEqual:@"setFixtureURL"];
  BOOL observe=[request[@"operation"] isEqual:@"observeFixtureSuggestions"];
  if(!wanted && !action && !observe) {
    if([windows containsObject:NSApp.mainWindow]) window=NSApp.mainWindow;
    else if([windows containsObject:NSApp.keyWindow]) window=NSApp.keyWindow;
    else if(windows.count==1) window=windows.firstObject;
    matches=window?1:0;
  }
  receipt[@"selectedWindowNumber"]=window?@(window.windowNumber):(id)NSNull.null;
  if(truncated || !window || matches!=1) { receipt[@"status"]=@"gap-window-not-unique-or-window-budget";receipt[@"truncated"]=JSONBool(truncated);return receipt; }
  receipt[@"windowUnique"]=@YES;
  ScenarioCommandBarActionScan* scan=[ScenarioCommandBarActionScan new];scan.fields=[NSMutableArray array];scan.window=window;scan.deadline=deadline;
  scan.seen=[NSHashTable hashTableWithOptions:NSPointerFunctionsStrongMemory|NSPointerFunctionsObjectPointerPersonality];
  Visit(window.contentView,0,scan);
  receipt[@"candidateCount"]=@(scan.fields.count);
  NSMutableArray* metadata=[NSMutableArray array];
  for(NSTextField* field in scan.fields) {
    if(!Budget(scan)) break;
    [metadata addObject:FieldMetadata(field,window)];
  }
  Budget(scan);receipt[@"fields"]=metadata;
  receipt[@"truncated"]=JSONBool(scan.truncated);receipt[@"budgetExceeded"]=JSONBool(scan.budgetExceeded);
  if(observe) {
    receipt[@"suggestions"]=ObserveSuggestions(window);
    if(scan.fields.count==1 && !Secure(scan.fields.firstObject) && !Secure(scan.fields.firstObject.cell) &&
       !scan.fields.firstObject.isHiddenOrHasHiddenAncestor && !AXHidden(scan.fields.firstObject))
      receipt[@"fieldReadback"]=SafeValue(scan.fields.firstObject);
    receipt[@"status"]=@"observed-only-no-acceptance-assertion";return receipt;
  }
  if(!action) { receipt[@"status"]=@"inventory-unexercised";return receipt; }
  if(gConsumed) { receipt[@"status"]=@"gap-action-already-consumed";return receipt; }
  if(!Budget(scan) || scan.fields.count!=1 || !PositiveInteger(wanted)) { receipt[@"status"]=@"gap-field-not-unique-or-scan-incomplete";return receipt; }
  NSTextField* field=scan.fields.firstObject;
  if(![NSApp.windows containsObject:window] || !Eligible(window) || window.windowNumber!=[wanted integerValue] ||
     !Descendant(field,window) || ![[field accessibilityIdentifier] isEqual:@"commandBarTextField"] ||
     !field.editable || !field.enabled || Secure(field) || Secure(field.cell) || field.isHiddenOrHasHiddenAncestor || AXHidden(field)) {
    receipt[@"status"]=@"gap-field-unavailable-hidden-secure-disabled-or-not-owned";return receipt;
  }
  NSDictionary* before=SafeValue(field);receipt[@"before"]=before;
  if(!TrueBoolean(before[@"valueAllowed"])) { receipt[@"status"]=@"gap-unexpected-current-value";return receipt; }
  id target=nil; NSString* route=nil; id editor=[field currentEditor];
  BOOL editorEligible=[editor isKindOfClass:NSTextView.class] && [field currentEditor]==editor && [(NSTextView*)editor window]==window &&
    [(NSTextView*)editor isEditable] && ![(NSTextView*)editor isHiddenOrHasHiddenAncestor] && !AXHidden(editor) && !Secure(editor) && !Excluded(editor) &&
    [Role(editor) isEqual:NSAccessibilityTextAreaRole] && AXAllowed(editor);
  // Only the exact current-editor route was independently proved in the frozen
  // native pair. No cell fallback, repair, delegate or synthetic event is used.
  if(!editorEligible) { receipt[@"status"]=@"gap-no-owned-current-editor-public-AX-setter";return receipt; }
  target=editor;route=@"current-editor";
  if(!Budget(scan)) { receipt[@"status"]=@"gap-budget-before-setter";receipt[@"truncated"]=@YES;receipt[@"budgetExceeded"]=@YES;return receipt; }
  // Latch before any receipt I/O or framework setter; exceptions and uncertain
  // outcomes consume the sole attempt. No subsequent request can retry it.
  gConsumed=YES;receipt[@"route"]=route;receipt[@"actionConsumed"]=@YES;receipt[@"routeOwnershipVerified"]=@YES;
  receipt[@"status"]=@"intent-before-public-setter";receipt[@"setterReturned"]=@NO;
  receipt[@"intentUptime"]=@(NSProcessInfo.processInfo.systemUptime);
  receipt[@"recorderCursorBeforeIntent"]=RecorderCursor();
  if(!Write(receipt,@".intent.json")) { receipt[@"status"]=@"gap-intent-not-persisted-action-consumed";return receipt; }
  receipt[@"recorderCursorBeforeSetter"]=RecorderCursor();
  ++gAttempts;receipt[@"nativeActionsPerformed"]=@(gAttempts);receipt[@"textSetterAttempts"]=@(gAttempts);receipt[@"actualSetterCalls"]=@(gAttempts);
  receipt[@"setterCallBeginUptime"]=@(NSProcessInfo.processInfo.systemUptime);
  @try {
    [(id<NSAccessibility>)target setAccessibilityValue:gFixture];
    receipt[@"setterReturned"]=@YES;receipt[@"status"]=@"public-setter-returned-unverified";
  } @catch(NSException* exception) {
    (void)exception;receipt[@"status"]=@"public-setter-exception-outcome-uncertain-no-retry";
  }
  receipt[@"setterCallEndUptime"]=@(NSProcessInfo.processInfo.systemUptime);
  receipt[@"recorderCursorAfterSetter"]=RecorderCursor();
  // Readback is observation only; never repair a failed setter or invoke any
  // delegate, notification, action, engine API, or alternative setter.
  if(ReadbackAllowed(Descendant(field,window),Secure(field)||Secure(field.cell),field.isHiddenOrHasHiddenAncestor,AXHidden(field)))
    receipt[@"after"]=SafeValue(field);
  else receipt[@"after"]=@{@"value":NSNull.null,@"valueAllowed":@NO,@"valuePolicy":@"field no longer safely readable"};
  return receipt;
}
static void Poll(void) {
  if(!NSThread.isMainThread || gHandling || gRequests>=kRequests) return;
  NSData* bytes=ReadRequest();if(!bytes) return;
  id request=[NSJSONSerialization JSONObjectWithData:bytes options:0 error:NULL];
  if(!RequestIdentity(request) || [gSeen containsObject:request[@"id"]]) return;
  [gSeen addObject:request[@"id"]];++gRequests;gHandling=YES;
  @try { @autoreleasepool { Write(Process(request),@".response.json"); } }
  @catch(NSException* exception) { (void)exception;NSMutableDictionary* receipt=Base(request);receipt[@"status"]=@"helper-exception-no-retry";Write(receipt,@".response.json"); }
  @finally { gHandling=NO;if(gRequests>=kRequests) dispatch_source_cancel(gTimer); }
}
__attribute__((constructor)) static void Start(void) {
  Diagnostic("constructor_enter");
  @autoreleasepool {
    NSDictionary* env=NSProcessInfo.processInfo.environment;
    NSString* expected=CanonicalPath(env[@"ARC_SCENARIO_COMMAND_BAR_BUNDLE"]);
    NSString* fixture=env[@"ARC_SCENARIO_COMMAND_BAR_URL"],*nonce=env[@"ARC_SCENARIO_COMMAND_BAR_NONCE"];
    NSString* request=env[@"ARC_SCENARIO_COMMAND_BAR_REQUEST"];
    NSBundle* bundle=NSBundle.mainBundle;
    char executablePath[PROC_PIDPATHINFO_MAXSIZE]={0};int length=proc_pidpath(getpid(),executablePath,sizeof(executablePath));
    NSString* processExecutable=length>0?CanonicalPath([NSString stringWithUTF8String:executablePath]):nil;
    if(!BundleIdentity(expected,CanonicalPath(bundle.bundlePath),CanonicalPath(bundle.executablePath),processExecutable,bundle.infoDictionary)) { Diagnostic("bundle_identity");return; }
    NSURLComponents* url=[fixture isKindOfClass:NSString.class]?[NSURLComponents componentsWithString:fixture]:nil;NSInteger port=url.port.integerValue;
    if(![url.scheme isEqual:@"http"] || ![url.host isEqual:@"127.0.0.1"] || port<1024 || port>65535 || url.user || url.password || url.fragment || fixture.length>512 ||
       ![url.path hasPrefix:@"/"] || ![url.URL.absoluteString isEqual:fixture]) { Diagnostic("fixture_url");return; }
    if(!ValidNonce(nonce)) { Diagnostic("nonce");return; }
    NSString* directory=ValidRequestDirectory(request);if(!directory) { Diagnostic("request_directory");return; }
    gRequest=[directory stringByAppendingPathComponent:@"action-request.json"];gNonce=[nonce copy];gFixture=[fixture copy];gBundle=[expected copy];gSeen=[NSMutableSet set];
    unsetenv("DYLD_INSERT_LIBRARIES");
    dispatch_async(dispatch_get_main_queue(),^{
      Write(@{@"installed":@YES,@"readOnly":@NO,@"pid":@(getpid()),@"nonce":gNonce,@"bundle":gBundle,@"fixtureURL":gFixture,@"mainThread":JSONBool(NSThread.isMainThread),
        @"requestBudget":@(kRequests),@"lifetimeSeconds":@80,@"maximumSetterAttempts":@1,@"textSetterAttempts":@0,@"actualSetterCalls":@0,@"engineCallsPerformed":@0,@"functionalAcceptance":@NO,
        @"routePolicy":@"owned-actual-current-editor-only-no-fallback"},@".ready.json");
      gTimer=dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,0,0,dispatch_get_main_queue());
      dispatch_source_set_timer(gTimer,dispatch_time(DISPATCH_TIME_NOW,250*NSEC_PER_MSEC),250*NSEC_PER_MSEC,25*NSEC_PER_MSEC);
      dispatch_source_set_event_handler(gTimer,^{@autoreleasepool{Poll();}});dispatch_resume(gTimer);Diagnostic("polling_installed");
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW,80*NSEC_PER_SEC),dispatch_get_main_queue(),^{
        dispatch_source_cancel(gTimer);Write(@{@"expired":@YES,@"pid":@(getpid()),@"nonce":gNonce,@"requests":@(gRequests),@"textSetterAttempts":@(gAttempts),@"actualSetterCalls":@(gAttempts),@"actionConsumed":JSONBool(gConsumed)},@".expired.json");
      });
    });
  }
}
