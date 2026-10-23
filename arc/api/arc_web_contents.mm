// Copyright 2026 ArcMod. Native WebContents identity, observable state and pane hosting.

#import "arc/api/arc_internal.h"
#import "arc/browser/arc_navigation.h"

#include "base/memory/weak_ptr.h"
#include <cmath>
#include <limits>
#include "base/check.h"
#include "base/functional/bind.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "arc/common/ad_block_browser.h"
#import <objc/message.h>
#include "content/public/browser/render_frame_host.h"
#include "ui/gfx/geometry/rect.h"
#include "components/sessions/content/session_tab_helper.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/browser/visibility.h"

ARC_EXPORT @interface ArcWebContentsConfiguration : NSObject
@property(nonatomic) BOOL foreground;
@property(nonatomic) BOOL untracked;
@property(nonatomic) BOOL forceDisplayPDFInline;
@property(nonatomic) BOOL disallowDownloads;
@property(nonatomic) BOOL disallowAutoplay;
@property(nonatomic) BOOL forceAllowAutoplay;
@property(nonatomic) BOOL disallowContextMenus;
@property(nonatomic) BOOL transparentBackground;
@property(nonatomic) BOOL initiallyHidden;
@property(nonatomic) BOOL initiallyMuted;
@property(nonatomic) CGSize forcedViewSize;
@end

@implementation ArcWebContentsConfiguration
@synthesize foreground = _foreground, untracked = _untracked;
@synthesize forceDisplayPDFInline = _forceDisplayPDFInline, disallowDownloads = _disallowDownloads;
@synthesize disallowAutoplay = _disallowAutoplay, forceAllowAutoplay = _forceAllowAutoplay;
@synthesize disallowContextMenus = _disallowContextMenus, transparentBackground = _transparentBackground;
@synthesize initiallyHidden = _initiallyHidden, initiallyMuted = _initiallyMuted;
@synthesize forcedViewSize = _forcedViewSize;
- (void)setForcedViewSize:(CGSize)size {
  if (size.width <= 0.1 || size.height <= 0.1) {
    _forcedViewSize = CGSizeZero;
    return;
  }
  CHECK(std::isfinite(size.width) && std::isfinite(size.height));
  CHECK_LE(size.width, std::numeric_limits<int>::max());
  CHECK_LE(size.height, std::numeric_limits<int>::max());
  _forcedViewSize = CGSizeMake(static_cast<int>(size.width), static_cast<int>(size.height));
}
- (instancetype)init {
  if ((self = [super init])) {
    _initiallyHidden = YES;
  }
  return self;
}
@end

@interface ArcWebContents (PaneHosting)
- (void)detachFromPaneHostView:(NSView*)host;
- (void)paneHostSizeChanged:(CGSize)size;
@end

@interface WebContentsPaneHostView : NSView
@property(nonatomic, weak) ArcWebContents* hostedContents;
@end
@implementation WebContentsPaneHostView
@synthesize hostedContents = _hostedContents;
- (void)setFrameSize:(NSSize)size {
  [super setFrameSize:size];
  [_hostedContents paneHostSizeChanged:self.bounds.size];
}
@end

@interface ArcWebContents ()
- (instancetype)initWithWebContents:(const void*)contents;
- (void)refreshNativeState;
- (void)nativeContentsDestroyed;
- (id)serviceNamed:(NSString*)className;
- (ArcWebContentsNavigationController*)navigationController;
@property(nonatomic, copy) NSString* title;
@property(nonatomic, strong) NSURL* url;
@property(nonatomic, copy) NSString* selectedText;
@property(nonatomic, strong) NSImage* favicon;
@property(nonatomic) BOOL isLoading;
@property(nonatomic) double loadProgress;
@property(nonatomic) BOOL isReadyToDisplay;
@property(nonatomic) int navigationID;
@property(nonatomic) int blockedResourceCount;
@property(nonatomic) BOOL canGoBack;
@property(nonatomic) BOOL canGoForward;
@property(nonatomic) BOOL undoableFieldActive;
@property(nonatomic) BOOL freeformTextFieldActive;
@property(nonatomic) BOOL passwordTextFieldActive;
@property(nonatomic) BOOL hadFormOrUserEdits;
@property(nonatomic) BOOL hasDirtyTextField;
@property(nonatomic) BOOL hasFocus;
@property(nonatomic, strong) NSString* inputEvent;
@property(nonatomic, strong) NSMutableIndexSet* tabTraits;
@property(nonatomic) NSInteger state;
@property(nonatomic) NSInteger visibility;
@property(nonatomic) float zoomLevel;
@property(nonatomic, strong) ArcColor* themeColor;
@property(nonatomic, strong) id rootBackgroundColor;
@property(nonatomic) CGSize pendingRenderFrameViewportSize;
@property(nonatomic, strong) id backForwardList;
@property(nonatomic, strong) id pdfController;
@property(nonatomic, weak) id userActivityDelegate;
@end

namespace {
class ArcContentsObserver : public content::WebContentsObserver {
 public:
  ArcContentsObserver(content::WebContents* contents, ArcWebContents* wrapper)
      : content::WebContentsObserver(contents), wrapper_(wrapper) {}
  void TitleWasSet(content::NavigationEntry*) override { [wrapper_ refreshNativeState]; }
  void DidStartNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    const bool primary = handle->IsInPrimaryMainFrame();
    if (!primary) { return; }
    const bool in_page = handle->IsSameDocument();
    const uint64_t generation = ++navigation_generation_;
    ArcNavigation* navigation = primary ? [ArcNavigation snapshotForNavigationHandle:handle] : nil;
    [wrapper refreshNativeState];
    if (web_contents() && generation == navigation_generation_ && primary) {
      [[wrapper navigationController] didStartNavigationSnapshot:navigation inPage:in_page];
    }
  }
  void DidRedirectNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    [[wrapper navigationController] didRedirectNavigation:handle];
  }
  void DidFinishNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    const bool primary = handle->IsInPrimaryMainFrame();
    if (!primary) { return; }
    const bool in_page = handle->IsSameDocument();
    const bool committed = handle->HasCommitted();
    const bool restored = handle->IsServedFromBackForwardCache();
    const int error = handle->GetNetErrorCode();
    const uint64_t generation = ++navigation_generation_;
    ArcNavigation* navigation = primary ? [ArcNavigation snapshotForNavigationHandle:handle] : nil;
    [wrapper refreshNativeState];
    if (web_contents() && generation == navigation_generation_ && primary) {
      [[wrapper navigationController] didFinishNavigationSnapshot:navigation inPage:in_page
          committed:committed error:error restored:restored];
    }
  }
  void DidFinishLoad(content::RenderFrameHost* frame, const GURL&) override {
    if (web_contents() && frame == web_contents()->GetPrimaryMainFrame()) {
      [[wrapper_ navigationController] didFinishDocumentLoad];
    }
  }
  void DidStartLoading() override { [wrapper_ refreshNativeState]; }
  void DidStopLoading() override { [wrapper_ refreshNativeState]; }
  void LoadProgressChanged(double progress) override { wrapper_.loadProgress = progress; }
  void DidFirstVisuallyNonEmptyPaint() override { wrapper_.isReadyToDisplay = YES; }
  void OnVisibilityChanged(content::Visibility) override { [wrapper_ refreshNativeState]; }
  void OnWebContentsFocused(content::RenderWidgetHost*) override { wrapper_.hasFocus = YES; }
  void OnWebContentsLostFocus(content::RenderWidgetHost*) override { wrapper_.hasFocus = NO; }
  void WebContentsDestroyed() override {
    [wrapper_ nativeContentsDestroyed];
    Observe(nullptr);
  }
 private:
  __weak ArcWebContents* wrapper_;
  uint64_t navigation_generation_ = 0;
};

class ArcContentsData : public content::WebContentsUserData<ArcContentsData> {
 public:
  // The native tab retains its wrapper; an unattached, wrapper-owned contents instead uses weak_wrapper.
  ArcWebContents* __strong wrapper;
  ArcWebContents* __weak weak_wrapper;
 private:
  explicit ArcContentsData(content::WebContents* contents) : content::WebContentsUserData<ArcContentsData>(*contents) {}
  friend content::WebContentsUserData<ArcContentsData>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(ArcContentsData);

uint64_t NextDiagnosticID() {
  static constinit uint64_t next = 0;
  return ++next;
}
}  // namespace

@implementation ArcWebContents {
  base::WeakPtr<content::WebContents> _contents;
  std::unique_ptr<content::WebContents> _ownedContents;
  std::unique_ptr<ArcContentsObserver> _observer;
  NSMutableDictionary<NSString*, id>* _services;
  NSNumber* _diagnosticId;
  ArcWebContentsNavigationController* _navigationController;
  __weak id _navigationDelegate;
  __weak id _uiDelegate;
  __weak id _actionsDelegate;
  __weak id _topBandColorDelegate;
  uint64_t _refreshGeneration;
  BOOL _foreground;
  BOOL _disallowDownloads;
  BOOL _isBlockingJavascriptPopups;
}
@synthesize title = _title, url = _url, selectedText = _selectedText, favicon = _favicon;
@synthesize isLoading = _isLoading, loadProgress = _loadProgress, isReadyToDisplay = _isReadyToDisplay;
@synthesize navigationID = _navigationID, blockedResourceCount = _blockedResourceCount;
@synthesize canGoBack = _canGoBack, canGoForward = _canGoForward;
@synthesize undoableFieldActive = _undoableFieldActive, freeformTextFieldActive = _freeformTextFieldActive;
@synthesize passwordTextFieldActive = _passwordTextFieldActive, hadFormOrUserEdits = _hadFormOrUserEdits;
@synthesize hasDirtyTextField = _hasDirtyTextField, hasFocus = _hasFocus, inputEvent = _inputEvent;
@synthesize tabTraits = _tabTraits, state = _state, visibility = _visibility, zoomLevel = _zoomLevel;
@synthesize themeColor = _themeColor, rootBackgroundColor = _rootBackgroundColor;
@synthesize pendingRenderFrameViewportSize = _pendingRenderFrameViewportSize;
@synthesize backForwardList = _backForwardList, pdfController = _pdfController;
@synthesize userActivityDelegate = _userActivityDelegate;

+ (instancetype)wrapperForWebContents:(content::WebContents*)contents {
  if (!contents) {
    return nil;
  }
  auto* data = ArcContentsData::GetOrCreateForWebContents(contents);
  ArcWebContents* wrapper = data->wrapper ?: data->weak_wrapper;
  if (!wrapper) {
    wrapper = [[self alloc] initWithWebContents:contents];
    data->wrapper = wrapper;
    data->weak_wrapper = wrapper;
    // Renderer package: per-frame config (ad blocking, effects) before the first navigation.
    arc::InitializeRendererBridge(contents);
  }
  return wrapper;
}
+ (instancetype)createWithProfile:(Profile*)profile configuration:(ArcWebContentsConfiguration*)configuration {
  if (!profile) {
    return nil;
  }
  ArcWebContentsConfiguration* config = configuration ?: [[ArcWebContentsConfiguration alloc] init];
  content::WebContents::CreateParams params(profile);
  params.initially_hidden = config.initiallyHidden;
  auto contents = content::WebContents::Create(params);
  arcmod::HostedTabHelpers::Attach(contents.get());
  if (config.transparentBackground) {
    contents->SetPageBaseBackgroundColor(SK_ColorTRANSPARENT);
  }
  const CGSize size = config.forcedViewSize;
  if (size.width > 0 && size.height > 0) {
    contents->Resize(gfx::Rect(static_cast<int>(size.width), static_cast<int>(size.height)));
  }
  ArcWebContents* wrapper = [self wrapperForWebContents:contents.get()];
  wrapper->_ownedContents = std::move(contents);
  wrapper->_foreground = config.foreground;
  wrapper->_disallowDownloads = config.disallowDownloads;
  auto* data = ArcContentsData::FromWebContents(wrapper->_ownedContents.get());
  data->wrapper = nil;
  wrapper->_ownedContents->SetAudioMuted(config.initiallyMuted);
  return wrapper;
}
- (instancetype)initWithWebContents:(const void*)contents {
  if ((self = [super init])) {
    auto* native = static_cast<content::WebContents*>(const_cast<void*>(contents));
    _contents = native->GetWeakPtr();
    _diagnosticId = @(NextDiagnosticID());
    _services = [NSMutableDictionary dictionary];
    self.title = native->GetTitle().empty() ? nil : ArcNSString(native->GetTitle());
    self.url = ArcNSURL(native->GetVisibleURL());
    self.selectedText = nil;
    self.isLoading = native->IsLoading();
    self.loadProgress = native->GetLoadProgress();
    self.favicon = nil;
    self.backForwardList = [ArcBackForwardList snapshotForWebContents:native];
    self.undoableFieldActive = NO;
    self.freeformTextFieldActive = NO;
    self.passwordTextFieldActive = NO;
    self.tabTraits = [NSMutableIndexSet indexSet];
    self.state = 0;
    self.zoomLevel = 1;
    self.themeColor = nil;
    self.isReadyToDisplay = NO;
    self.pendingRenderFrameViewportSize = CGSizeZero;
    self.navigationID = 0;
    self.canGoBack = native->GetController().CanGoBack();
    self.canGoForward = native->GetController().CanGoForward();
    self.hadFormOrUserEdits = NO;
    self.hasDirtyTextField = NO;
    self.rootBackgroundColor = nil;
    self.blockedResourceCount = 0;
    self.visibility = static_cast<NSInteger>(native->GetVisibility());
    self.hasFocus = NO;
    _observer = std::make_unique<ArcContentsObserver>(native, self);
  }
  return self;
}
- (void)dealloc {
  _observer.reset();
  if (_ownedContents) {
    content::GetUIThreadTaskRunner({})->DeleteSoon(FROM_HERE, std::move(_ownedContents));
  }
}
- (content::WebContents*)webContents { return _contents.get(); }
- (std::unique_ptr<content::WebContents>)takeOwnedWebContents {
  if (_ownedContents) {
    ArcContentsData::FromWebContents(_ownedContents.get())->wrapper = self;
  }
  return std::move(_ownedContents);
}
- (void)nativeContentsDestroyed { _contents.reset(); }
- (BOOL)foregroundForInsertion { return _foreground; }
- (BOOL)isInitialized { return !!_contents; }
- (void)refreshNativeState {
  if (!_contents) {
    return;
  }
  // Setters run the app's KVO callbacks, which can close or navigate this contents.
  // Copy all native values before the first setter so no native dereference crosses a callback.
  const uint64_t generation = ++_refreshGeneration;
  auto native = _contents;
  NSString* title = native->GetTitle().empty() ? nil : ArcNSString(native->GetTitle());
  NSURL* url = ArcNSURL(native->GetVisibleURL());
  const BOOL loading = native->IsLoading();
  const double progress = native->GetLoadProgress();
  const BOOL back = native->GetController().CanGoBack();
  const BOOL forward = native->GetController().CanGoForward();
  ArcBackForwardList* history = [ArcBackForwardList snapshotForWebContents:native.get()];
  const NSInteger visibility = static_cast<NSInteger>(native->GetVisibility());
  self.title = title;
  if (!native || generation != _refreshGeneration) { return; }
  self.url = url;
  if (!native || generation != _refreshGeneration) { return; }
  self.isLoading = loading;
  if (!native || generation != _refreshGeneration) { return; }
  self.loadProgress = progress;
  if (!native || generation != _refreshGeneration) { return; }
  self.canGoBack = back;
  if (!native || generation != _refreshGeneration) { return; }
  self.canGoForward = forward;
  if (!native || generation != _refreshGeneration) { return; }
  self.backForwardList = history;
  if (!native || generation != _refreshGeneration) { return; }
  self.visibility = visibility;
}
// Sub-objects (DevTools, media session, ...) that implement -initWithArcWebContents: get this wrapper;
// shell classes get plain init.
- (id)serviceNamed:(NSString*)className {
  id service = _services[className];
  if (!service) {
    Class cls = NSClassFromString(className);
    SEL withContents = NSSelectorFromString(@"initWithArcWebContents:");
    service = [cls instancesRespondToSelector:withContents]
                  ? ((id (*)(id, SEL, id))objc_msgSend)([cls alloc], withContents, self)
                  : [[cls alloc] init];
    if (service) {
      _services[className] = service;
    }
  }
  return service;
}
- (id)devTools { return [self serviceNamed:@"ArcWebContentsDevTools"]; }
- (id)mediaSession { return [self serviceNamed:@"ArcWebContentsMediaSession"]; }
- (id)boostIDE { return [self serviceNamed:@"ArcWebContentsBoostIDE"]; }
- (id)aiAgent { return [self serviceNamed:@"ArcWebContentsAIAgent"]; }
- (id)thumbnailsGenerator { return [self serviceNamed:@"ArcWebContentsThumbnailsGenerator"]; }
- (ArcWebContentsNavigationController*)navigationController {
  if (!_navigationController) {
    _navigationController = [[ArcWebContentsNavigationController alloc] initForWebContents:self];
  }
  return _navigationController;
}
- (NSNumber*)diagnosticId { return _diagnosticId; }
- (ArcSessionID*)sessionId {
  if (!_contents) {
    return nil;
  }
  auto* helper = sessions::SessionTabHelper::FromWebContents(_contents.get());
  return helper ? [[ArcSessionID alloc] initWithID:helper->session_id().id()] : nil;
}
- (void)setNavigationDelegate:(id)delegate { _navigationDelegate = delegate; }
- (void)setUIDelegate:(id)delegate { _uiDelegate = delegate; }
- (id)uiDelegate { return _uiDelegate; }
- (void)setActionsDelegate:(id)delegate { _actionsDelegate = delegate; }
- (void)setTopBandColorDelegate:(id)delegate { _topBandColorDelegate = delegate; }
- (NSView*)nativeView { return _contents ? _contents->GetNativeView().GetNativeNSView() : nil; }
- (NSView*)createPaneHostView { return [[WebContentsPaneHostView alloc] initWithFrame:NSZeroRect]; }
- (void)attachToPaneHostView:(NSView*)host {
  if (![host isMemberOfClass:[WebContentsPaneHostView class]]) {
    return;
  }
  NSView* view = [self nativeView];
  if (!view || view.superview == host) {
    return;
  }
  WebContentsPaneHostView* pane = (WebContentsPaneHostView*)host;
  [pane.hostedContents detachFromPaneHostView:host];
  NSView* previousHost = view.superview;
  if ([previousHost isMemberOfClass:[WebContentsPaneHostView class]]) {
    [self detachFromPaneHostView:previousHost];
  } else {
    [view removeFromSuperview];
  }
  pane.hostedContents = self;
  view.frame = host.bounds;
  view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
  [host addSubview:view];
  if (_contents) {
    _contents->WasShown();
  }
  self.pendingRenderFrameViewportSize = host.bounds.size;
}
- (void)detachFromPaneHostView:(NSView*)host {
  if ([host isMemberOfClass:[WebContentsPaneHostView class]] && [self nativeView].superview == host) {
    ((WebContentsPaneHostView*)host).hostedContents = nil;
    [[self nativeView] removeFromSuperview];
    if (_contents) {
      _contents->WasHidden();
    }
  }
}
- (void)paneHostSizeChanged:(CGSize)size { self.pendingRenderFrameViewportSize = size; }
- (void)stop { if (_contents) { _contents->Stop(); } }
- (void)close {
  if (_ownedContents) {
    // Native observers invoke app KVO callbacks. Destruction must wait until
    // Chromium has finished notifying every observer.
    content::GetUIThreadTaskRunner({})->DeleteSoon(FROM_HERE, std::move(_ownedContents));
  } else if (_contents) {
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(
        [](base::WeakPtr<content::WebContents> contents) {
          if (contents) { contents->ClosePage(); }
        }, _contents));
  }
}
- (void)closeWithForegroundCommitTiming:(NSInteger)timing { [self close]; }
- (BOOL)muted { return _contents && _contents->IsAudioMuted(); }
- (void)setMuted:(BOOL)muted { if (_contents) { _contents->SetAudioMuted(muted); } }
- (BOOL)audible { return _contents && _contents->IsCurrentlyAudible(); }
- (BOOL)shareable { return _contents && _contents->GetLastCommittedURL().SchemeIsHTTPOrHTTPS(); }
- (BOOL)isEqual:(id)other { return self == other; }
- (BOOL)disallowDownloads { return _disallowDownloads; }
- (void)setDisallowDownloads:(BOOL)disallow { _disallowDownloads = disallow; }
- (BOOL)isBlockingJavascriptPopups { return _isBlockingJavascriptPopups; }
- (void)setIsBlockingJavascriptPopups:(BOOL)blocking { _isBlockingJavascriptPopups = blocking; }
@end

@implementation ArcBrowserContext (Core)
- (ArcWebContents*)createWebContents:(ArcWebContentsConfiguration*)configuration {
  return [ArcWebContents createWithProfile:self.profile configuration:configuration];
}
@end

// Weak default so the framework links before the renderer package defines it (renderer's definition wins).
namespace arc {
__attribute__((weak)) void InitializeRendererBridge(content::WebContents* contents) {}
}  // namespace arc
