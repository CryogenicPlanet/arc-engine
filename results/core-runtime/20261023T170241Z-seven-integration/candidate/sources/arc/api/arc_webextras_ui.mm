// Copyright 2026 ArcMod. WebUI and side-panel boundary adapters.
#import "arc/api/arc_internal.h"

#include "base/values.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/side_panel/side_panel_registry.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "components/sessions/content/session_tab_helper.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"

@protocol WebExtrasSidePanelDelegate <NSObject>
- (void)sidePanel:(id)panel didShow:(ArcWebContents*)contents;
- (void)sidePanel:(id)panel didUpdateTitle:(NSString*)title;
- (void)sidePanelWillClose:(id)panel withAnimation:(BOOL)animate;
- (void)sidePanelWillUnhide:(id)panel;
@end
@interface NSObject (WebExtrasEntryInfo)
- (void)setHasActiveEntry:(BOOL)active;
- (void)setExtensionId:(NSString*)extension;
- (void)setCachedWebContents:(ArcWebContents*)contents;
@end

namespace {
struct SidePanelHandle { __weak ArcBrowser* browser; };
struct BoostHandle { __weak ArcWebContents* contents; };
}  // namespace

ARC_EXPORT @interface ArcBrowserSidePanel : NSObject {
  __weak ArcBrowser* _browser;
  __weak id<WebExtrasSidePanelDelegate> _delegate;
  ArcWebContents* _contents;
  NSString* _title;
  BOOL _closing;
  NSInteger _sourceSessionId;
}
- (instancetype)initWithArcBrowser:(ArcBrowser*)browser;
- (instancetype)initWithSidePanel:(const void*)handle;
- (void)setDelegate:(id<WebExtrasSidePanelDelegate>)delegate;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) ArcWebContents* webContents;
@end
@implementation ArcBrowserSidePanel
- (instancetype)initWithArcBrowser:(ArcBrowser*)browser {
  SidePanelHandle handle{browser};
  return [self initWithSidePanel:&handle];
}
- (instancetype)initWithSidePanel:(const void*)handle {
  if ((self = [super init])) {
    if (handle) { _browser = static_cast<const SidePanelHandle*>(handle)->browser; }
    _title = @"";
  }
  return self;
}
- (void)setDelegate:(id<WebExtrasSidePanelDelegate>)delegate { _delegate = delegate; }
- (NSString*)title { return _title; }
- (ArcWebContents*)webContents { return _contents; }
- (void)requestClose {
  if (!_contents || _closing) { return; }
  _closing = YES;
  id<WebExtrasSidePanelDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(sidePanelWillClose:withAnimation:)]) {
    [delegate sidePanelWillClose:self withAnimation:YES];
  }
}
- (void)didFinishClosing {
  if (!_closing) { return; }
  _closing = NO;
  _contents = nil;
  _title = @"";
}
- (id)getEntryInfoForTabWithSessionId:(NSInteger)sessionId {
  id result = [[NSClassFromString(@"ArcBrowserSidePanelEntryInfo") alloc] init];
  auto* browser = [_browser browser];
  auto* tabs = browser ? browser->GetTabStripModel() : nullptr;
  if (!tabs) { return result; }
  for (int index = 0; index < tabs->count(); ++index) {
    auto* contents = tabs->GetWebContentsAt(index);
    if (sessions::SessionTabHelper::IdForTab(contents).id() != sessionId) { continue; }
    auto* registry = SidePanelRegistry::GetDeprecated(contents);
    auto entry = registry ? registry->GetActiveEntry() : std::nullopt;
    [result setHasActiveEntry:entry.has_value()];
    if (entry) {
      auto extension = (*entry)->key().extension_id();
      [result setExtensionId:extension ? ArcNSString(*extension) : nil];
    }
    // Only return a cached WebContents when the native entry has actually been
    // shown through this adapter. A registry entry alone is not a live panel.
    if (entry && sessionId == _sourceSessionId) {
      [result setCachedWebContents:_contents];
    }
    break;
  }
  return result;
}
// Internal entry point for native panel providers. Kept separate from close
// completion because Arc owns the closing animation and native view reparenting.
- (void)showContents:(ArcWebContents*)contents title:(NSString*)title
    forTabWithSessionId:(NSInteger)sessionId {
  BOOL unhide = _closing && _contents == contents;
  _contents = contents;
  _sourceSessionId = sessionId;
  _title = [title copy] ?: @"";
  _closing = NO;
  id<WebExtrasSidePanelDelegate> delegate = _delegate;
  if (unhide && [delegate respondsToSelector:@selector(sidePanelWillUnhide:)]) {
    [delegate sidePanelWillUnhide:self];
  } else if ([delegate respondsToSelector:@selector(sidePanel:didShow:)]) {
    [delegate sidePanel:self didShow:contents];
  }
  if ([delegate respondsToSelector:@selector(sidePanel:didUpdateTitle:)]) {
    [delegate sidePanel:self didUpdateTitle:_title];
  }
}
@end

@protocol WebExtrasBoostDelegate <NSObject>
- (void)boostIDEDidOpen:(id)ide;
- (void)boostIDEWillClose:(id)ide;
- (void)boostIDE:(id)ide didRequestExecuteCommand:(NSString*)command jsonString:(NSString*)json;
@end
ARC_EXPORT @interface ArcWebContentsBoostIDE : NSObject {
  __weak ArcWebContents* _owner;
  __weak id<WebExtrasBoostDelegate> _delegate;
  NSMutableSet<NSString*>* _pending;
}
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
- (instancetype)initWithBoostIDE:(const void*)handle;
- (void)setDelegate:(id<WebExtrasBoostDelegate>)delegate;
- (void)respond:(NSString*)identifier success:(BOOL)success jsonString:(NSString*)json __attribute__((objc_direct));
- (void)didRequestCommand:(NSString*)identifier jsonString:(NSString*)json __attribute__((objc_direct));
@end
@implementation ArcWebContentsBoostIDE
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  BoostHandle handle{owner};
  return [self initWithBoostIDE:&handle];
}
- (instancetype)initWithBoostIDE:(const void*)handle {
  if ((self = [super init])) {
    if (handle) { _owner = static_cast<const BoostHandle*>(handle)->contents; }
    _pending = [NSMutableSet set];
  }
  return self;
}
- (void)setDelegate:(id<WebExtrasBoostDelegate>)delegate { _delegate = delegate; }
- (void)respond:(NSString*)identifier success:(BOOL)success jsonString:(NSString*)json {
  auto* contents = [_owner webContents];
  auto* ui = contents ? contents->GetWebUI() : nullptr;
  if (!ui || !ui->CanCallJavascript() || ![_pending containsObject:identifier]) { return; }
  [_pending removeObject:identifier];
  ui->CallJavascriptFunctionUnsafe("cr.webUIResponse", base::Value(ArcUTF8(identifier)),
                                 base::Value(static_cast<bool>(success)), base::Value(ArcUTF8(json)));
}
- (void)sendResponse:(NSString*)identifier jsonString:(NSString*)json {
  [self respond:identifier success:YES jsonString:json];
}
- (void)sendErrorResponse:(NSString*)identifier jsonString:(NSString*)json {
  [self respond:identifier success:NO jsonString:json];
}
- (void)sendEvent:(NSString*)name jsonString:(NSString*)json {
  auto* contents = [_owner webContents];
  auto* ui = contents ? contents->GetWebUI() : nullptr;
  if (ui && ui->CanCallJavascript()) {
    ui->CallJavascriptFunctionUnsafe("cr.webUIListenerCallback", base::Value(ArcUTF8(name)), base::Value(ArcUTF8(json)));
  }
}
// Called by the trusted Boost WebUI handler, once its request ID is registered.
- (void)didRequestCommand:(NSString*)identifier jsonString:(NSString*)json {
  if (!identifier) { return; }
  [_pending addObject:identifier];
  id<WebExtrasBoostDelegate> delegate = _delegate;
  if ([delegate respondsToSelector:@selector(boostIDE:didRequestExecuteCommand:jsonString:)]) {
    [delegate boostIDE:self didRequestExecuteCommand:identifier jsonString:json];
  }
}
@end
