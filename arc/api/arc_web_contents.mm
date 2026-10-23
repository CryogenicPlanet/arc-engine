// Copyright 2026 ArcMod. Native WebContents identity, observable state and pane hosting.

#import "arc/api/arc_internal.h"
#import "arc/browser/arc_navigation.h"
#import "arc/browser/core_render_state.h"

#include "base/memory/weak_ptr.h"
#include <bit>
#include <cstdint>
#include <cmath>
#include <limits>
#include <optional>
#include <utility>
#include <vector>
#include "base/check.h"
#include "base/functional/bind.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ssl/chrome_security_state_util.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "arc/common/ad_block_browser.h"
#import <objc/message.h>
#include "content/public/browser/render_frame_host.h"
#include "ui/gfx/geometry/rect.h"
#include "components/sessions/content/session_tab_helper.h"
#include "components/zoom/zoom_controller.h"
#include "third_party/blink/public/common/page/page_zoom.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/favicon_status.h"
#include "content/public/browser/reload_type.h"
#include "content/public/browser/navigation_throttle.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/browser/visibility.h"
#include "ui/base/page_transition_types.h"

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

namespace {
struct CoreConfigurationSize {
  int width;
  int height;
};
struct CoreWebContentsConfiguration {
  bool foreground = false;
  bool untracked = false;
  bool force_display_pdf_inline = false;
  bool setup_for_dark_mode = false;
  bool transparent_background = false;
  bool disallow_downloads = false;
  bool disallow_autoplay = false;
  bool force_allow_autoplay = false;
  bool disallow_context_menus = false;
  bool initially_muted = false;
  bool initially_hidden = true;
  std::optional<CoreConfigurationSize> forced_view_size;
};
static_assert(sizeof(CoreWebContentsConfiguration) == 24);
}  // namespace

@implementation ArcWebContentsConfiguration {
  CoreWebContentsConfiguration _configuration;
}
- (BOOL)foreground { return _configuration.foreground; }
- (void)setForeground:(BOOL)value { _configuration.foreground = value; }
- (BOOL)untracked { return _configuration.untracked; }
- (void)setUntracked:(BOOL)value { _configuration.untracked = value; }
- (BOOL)forceDisplayPDFInline { return _configuration.force_display_pdf_inline; }
- (void)setForceDisplayPDFInline:(BOOL)value { _configuration.force_display_pdf_inline = value; }
- (BOOL)disallowDownloads { return _configuration.disallow_downloads; }
- (void)setDisallowDownloads:(BOOL)value { _configuration.disallow_downloads = value; }
- (BOOL)disallowAutoplay { return _configuration.disallow_autoplay; }
- (void)setDisallowAutoplay:(BOOL)value { _configuration.disallow_autoplay = value; }
- (BOOL)forceAllowAutoplay { return _configuration.force_allow_autoplay; }
- (void)setForceAllowAutoplay:(BOOL)value { _configuration.force_allow_autoplay = value; }
- (BOOL)disallowContextMenus { return _configuration.disallow_context_menus; }
- (void)setDisallowContextMenus:(BOOL)value { _configuration.disallow_context_menus = value; }
- (BOOL)transparentBackground { return _configuration.transparent_background; }
- (void)setTransparentBackground:(BOOL)value { _configuration.transparent_background = value; }
- (BOOL)initiallyHidden { return _configuration.initially_hidden; }
- (void)setInitiallyHidden:(BOOL)value { _configuration.initially_hidden = value; }
- (BOOL)initiallyMuted { return _configuration.initially_muted; }
- (void)setInitiallyMuted:(BOOL)value { _configuration.initially_muted = value; }
- (CGSize)forcedViewSize {
  const auto& size = _configuration.forced_view_size;
  return size ? CGSizeMake(size->width, size->height) : CGSizeZero;
}
- (void)setForcedViewSize:(CGSize)size {
  constexpr double minimum = static_cast<double>(0.1f);
  if (size.width <= minimum || size.height <= minimum) {
    _configuration.forced_view_size.reset();
    return;
  }
  CHECK(std::isfinite(size.width) && std::isfinite(size.height));
  CHECK_LE(size.width, std::numeric_limits<int>::max());
  CHECK_LE(size.height, std::numeric_limits<int>::max());
  _configuration.forced_view_size = CoreConfigurationSize{
      static_cast<int>(size.width), static_cast<int>(size.height)};
}
@end

@class ArcColor4f;
@class ArcWebContentsPDFController;
@protocol ArcWebContentsUserActivityDelegate;

@interface ArcWebContents ()
- (void)detachFromPaneHostView:(NSView*)host;
@end

@interface WebContentsPaneHostView : NSView
@property(nonatomic, weak) ArcWebContents* hostedContents;
@end
@implementation WebContentsPaneHostView
@synthesize hostedContents = _hostedContents;
@end

@interface ArcWebContents ()
- (instancetype)initWithWebContents:(const void*)contents;
- (void)refreshNativeState __attribute__((objc_direct));
- (void)invalidateNativeStateRefresh __attribute__((objc_direct));
- (void)completeNavigationPolicy:(int64_t)identifier allowed:(BOOL)allowed __attribute__((objc_direct));
- (void)nativeContentsDestroyed __attribute__((objc_direct));
- (void)coreDidShowJavaScriptPopup __attribute__((objc_direct));
- (id)serviceNamed:(NSString*)className __attribute__((objc_direct));
- (ArcWebContentsNavigationController*)navigationController;
- (ArcWebContentsNavigationController*)coreNavigationController __attribute__((objc_direct));
- (id)navigationDelegate __attribute__((objc_direct));
- (id)uiDelegate;
- (void)nativeContentsClosing __attribute__((objc_direct));
- (void)nativeContentsClosingWithRestorationID:(ArcSessionID*)identifier __attribute__((objc_direct));
- (NSNumber*)coreDiagnosticID __attribute__((objc_direct));
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
@property(nonatomic, strong) ArcColor4f* rootBackgroundColor;
@property(nonatomic) CGSize pendingRenderFrameViewportSize;
@property(nonatomic, strong) ArcBackForwardList* backForwardList;
@property(nonatomic, strong) ArcWebContentsPDFController* pdfController;
@property(nonatomic, weak) id<ArcWebContentsUserActivityDelegate> userActivityDelegate;
@end

ARC_EXPORT @interface ArcSecureContext : NSObject
- (instancetype)initWithSecureContext:(const void*)contents;
@property(nonatomic, readonly) NSUInteger state;
@end

@implementation ArcSecureContext {
  base::WeakPtr<content::WebContents> _contents;
}
- (instancetype)initWithSecureContext:(const void*)contents {
  if ((self = [super init]) && contents) {
    _contents = static_cast<content::WebContents*>(const_cast<void*>(contents))->GetWeakPtr();
  }
  return self;
}
- (NSUInteger)state {
  if (!_contents) { return 0; }
  auto level = chrome_security_state::GetSecurityLevel(_contents.get());
  return level == security_state::NONE ? 0 : level == security_state::SECURE ? 1 : 2;
}
@end

@protocol CoreContentsDelegate <NSObject>
- (NSInteger)webContents:(ArcWebContents*)contents decidePolicyForNavigationAction:(ArcNavigationAction*)action;
- (void)webContentsClosingImminent:(ArcWebContents*)contents withSessionID:(ArcSessionID*)identifier;
- (void)webContents:(ArcWebContents*)contents didCloseWithSessionID:(ArcSessionID*)identifier;
- (void)webContents:(ArcWebContents*)contents didChangeStatusText:(NSString*)text;
- (void)webContents:(ArcWebContents*)contents didChangeVisibleSecurityState:(ArcSecureContext*)context;
@end

namespace {
bool NeedsNavigationPolicy(content::NavigationHandle* handle) {
  return handle->IsInPrimaryMainFrame() && handle->IsRendererInitiated() &&
      !handle->IsSameDocument() && handle->GetReloadType() == content::ReloadType::NONE &&
      (handle->GetPageTransition() & ui::PAGE_TRANSITION_FORWARD_BACK) == 0;
}

class CoreNavigationPolicyThrottle final : public content::NavigationThrottle {
 public:
  explicit CoreNavigationPolicyThrottle(content::NavigationThrottleRegistry& registry)
      : content::NavigationThrottle(registry) {}
  const char* GetNameForLogging() override { return "ArcNavigationPolicy"; }
  ThrottleCheckResult WillStartRequest() override {
    auto* handle = navigation_handle();
    if (!NeedsNavigationPolicy(handle)) {
      return PROCEED;
    }
    const int64_t identifier = handle->GetNavigationId();
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:handle->GetWebContents()];
    id<CoreContentsDelegate> delegate = [owner navigationDelegate];
    if (![delegate respondsToSelector:@selector(webContents:decidePolicyForNavigationAction:)]) {
      [owner completeNavigationPolicy:identifier allowed:YES];
      return PROCEED;
    }
    const NSInteger policy = [delegate webContents:owner
        decidePolicyForNavigationAction:arc::CreateNavigationAction(handle, 1)];
    CHECK_GE(policy, 0);
    CHECK_LT(policy, 12);
    [owner completeNavigationPolicy:identifier allowed:policy == 1];
    // The delegate returns the destination disposition; current-tab navigation
    // continues here. A declined/redirected request must not also commit here.
    return policy == 1 ? PROCEED : CANCEL_AND_IGNORE;
  }
};

void InstallNavigationPolicyHook() {
  arcmod::GetNavigationThrottleHook() = base::BindRepeating(
      [](content::NavigationThrottleRegistry& registry) {
        registry.AddThrottle(std::make_unique<CoreNavigationPolicyThrottle>(registry));
      });
}

class ArcContentsObserver : public content::WebContentsObserver {
 public:
  ArcContentsObserver(content::WebContents* contents, ArcWebContents* wrapper)
      : content::WebContentsObserver(contents), wrapper_(wrapper),
        last_committed_entry_count_(contents->GetController().GetEntryCount()) {}
  void CompleteNavigationPolicy(int64_t identifier, bool allowed) {
    if (!awaiting_policy_ || identifier != pending_navigation_identifier_) { return; }
    ArcWebContents* wrapper = wrapper_;
    ArcNavigation* navigation = pending_navigation_;
    const uint64_t generation = pending_navigation_generation_;
    std::vector<PendingStateEvent> events = std::move(pending_state_events_);
    pending_state_events_.clear();
    ResetPendingNavigation();
    if (!allowed || !web_contents() || generation != navigation_generation_) {
      return;
    }
    UpdateURLForStart(wrapper);
    if (!web_contents() || generation != navigation_generation_) { return; }
    PublishPendingState(std::move(events), generation);
    if (!web_contents() || generation != navigation_generation_) { return; }
    [[wrapper coreNavigationController] didStartNavigationSnapshot:navigation inPage:NO];
  }
  void TitleWasSet(content::NavigationEntry*) override {
    ArcWebContents* wrapper = wrapper_;
    if (web_contents()) {
      wrapper.title = web_contents()->GetTitle().empty() ? nil : ArcNSString(web_contents()->GetTitle());
    }
  }
  void DidStartNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    const bool primary = handle->IsInPrimaryMainFrame();
    if (!primary) {
      if (awaiting_navigation_start_ && !awaiting_policy_) {
        awaiting_navigation_start_ = false;
        FlushPendingState(navigation_generation_);
      }
      return;
    }
    const bool in_page = handle->IsSameDocument();
    const bool needs_policy = NeedsNavigationPolicy(handle);
    const int64_t identifier = handle->GetNavigationId();
    const uint64_t generation = ++navigation_generation_;
    ArcNavigation* navigation = primary ? [ArcNavigation snapshotForNavigationHandle:handle] : nil;
    [wrapper invalidateNativeStateRefresh];
    // A superseding navigation must not inherit an older policy's queued state.
    if (awaiting_policy_) { pending_state_events_.clear(); }
    ResetPendingNavigation();
    awaiting_navigation_start_ = false;
    if (needs_policy) {
      pending_navigation_ = navigation;
      pending_navigation_identifier_ = identifier;
      pending_navigation_generation_ = generation;
      awaiting_policy_ = true;
      return;
    }
    FlushPendingState(generation);
    if (!web_contents() || generation != navigation_generation_) { return; }
    if (!in_page) { UpdateURLForStart(wrapper); }
    if (web_contents() && generation == navigation_generation_ && primary) {
      [[wrapper coreNavigationController] didStartNavigationSnapshot:navigation inPage:in_page];
    }
  }
  void DidRedirectNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    [[wrapper coreNavigationController] didRedirectNavigation:handle];
  }
  void ReadyToCommitNavigation(content::NavigationHandle* handle) override {
    if (!handle->IsInPrimaryMainFrame() || handle->IsSameDocument() ||
        saw_navigation_start_ || initial_ready_to_commit_) {
      return;
    }
    ArcWebContents* wrapper = wrapper_;
    NSURL* destination = ArcNSURL(handle->GetURL());
    const uint64_t generation = navigation_generation_;
    initial_ready_to_commit_ = true;
    wrapper.url = destination;
    if (web_contents() && generation == navigation_generation_) {
      wrapper.navigationID = visible_navigation_id_;
    }
  }
  void DidFinishNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    const bool primary = handle->IsInPrimaryMainFrame();
    if (!primary) { return; }
    const bool in_page = handle->IsSameDocument();
    const bool committed = handle->HasCommitted();
    const bool restored = handle->IsServedFromBackForwardCache();
    const int error = handle->GetNetErrorCode();
    if (awaiting_policy_ && pending_navigation_identifier_ == handle->GetNavigationId()) {
      ResetPendingNavigation();
      pending_state_events_.clear();
    }
    const uint64_t generation = ++navigation_generation_;
    ArcNavigation* navigation = primary ? [ArcNavigation snapshotForNavigationHandle:handle] : nil;
    NSURL* destination = ArcNSURL(handle->GetURL());
    if (committed) {
      // A contents first observed after its navigation started still receives
      // the start/commit URL state transitions before the app's commit callback.
      if (!saw_navigation_start_) { UpdateURLForStart(wrapper); }
      if (!web_contents() || generation != navigation_generation_) { return; }
      saw_navigation_start_ = true;
      wrapper.url = destination;
      if (!web_contents() || generation != navigation_generation_) { return; }
      wrapper.navigationID = ++visible_navigation_id_;
      if (!web_contents() || generation != navigation_generation_) { return; }
      [wrapper refreshNativeState];
    }
    if (web_contents() && generation == navigation_generation_ && primary) {
      [[wrapper coreNavigationController] didFinishNavigationSnapshot:navigation inPage:in_page
          committed:committed error:error restored:restored];
    }
  }
  void DidFinishLoad(content::RenderFrameHost* frame, const GURL&) override {
    if (web_contents() && frame == web_contents()->GetPrimaryMainFrame()) {
      [[wrapper_ coreNavigationController] didFinishDocumentLoad];
    }
  }
  void DidStartLoading() override {
    if (!web_contents()) { return; }
    if (!awaiting_policy_) {
      pending_state_events_.clear();
      awaiting_navigation_start_ = true;
      const uint64_t token = ++pending_loading_token_;
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(
          &ArcContentsObserver::FlushUnclaimedLoading, weak_factory_.GetWeakPtr(), token));
    }
    pending_state_events_.push_back(CaptureLoadingState());
  }
  void DidStopLoading() override {
    ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) = wrapper_;
    if (!wrapper) { return; }
    if (awaiting_navigation_start_ && !awaiting_policy_) {
      awaiting_navigation_start_ = false;
      FlushPendingState(navigation_generation_);
    }
    UpdateLoadingState();
  }
  void NavigationEntryCommitted(const content::LoadCommittedDetails&) override {
    if (web_contents()) {
      last_committed_entry_count_ = web_contents()->GetController().GetEntryCount();
      early_history_entry_count_ = -1;
    }
    UpdateHistory();
  }
  void NavigationListPruned(const content::PrunedDetails&) override { UpdateHistory(); }
  void NavigationEntriesDeleted() override { UpdateHistory(); }
  void LoadProgressChanged(double progress) override {
    if (awaiting_navigation_start_ || awaiting_policy_) {
      PendingStateEvent event;
      event.kind = PendingStateEvent::Kind::kProgress;
      event.progress = progress;
      pending_state_events_.push_back(event);
    } else {
      wrapper_.loadProgress = progress;
    }
  }
  void DidFirstVisuallyNonEmptyPaint() override { wrapper_.isReadyToDisplay = YES; }
  void OnVisibilityChanged(content::Visibility visibility) override {
    ArcWebContents* wrapper = wrapper_;
    wrapper.visibility = static_cast<NSInteger>(visibility);
    wrapper.state = web_contents() && web_contents()->IsCrashed() ? 1 : 0;
  }
  void DidChangeVisibleSecurityState() override {
    ArcWebContents* wrapper = wrapper_;
    if (!web_contents()) { return; }
    auto native = web_contents()->GetWeakPtr();
    const uint64_t generation = navigation_generation_;
    const int entry_count = native->GetController().GetEntryCount();
    if (entry_count > last_committed_entry_count_ && entry_count != early_history_entry_count_) {
      early_history_entry_count_ = entry_count;
      UpdateHistory();
      if (!native || generation != navigation_generation_) { return; }
    }
    id<CoreContentsDelegate> delegate = [wrapper uiDelegate];
    if (web_contents() && [delegate respondsToSelector:@selector(webContents:didChangeVisibleSecurityState:)]) {
      ArcSecureContext* state = [[ArcSecureContext alloc] initWithSecureContext:web_contents()];
      [delegate webContents:wrapper didChangeVisibleSecurityState:state];
    }
  }
  void DidChangeThemeColor() override {
    if (web_contents()) {
      auto color = web_contents()->GetThemeColor();
      wrapper_.themeColor = color ? [ArcColor colorWithSkColor:*color] : nil;
    }
  }
  void OnWebContentsFocused(content::RenderWidgetHost*) override { wrapper_.hasFocus = YES; }
  void OnWebContentsLostFocus(content::RenderWidgetHost*) override { wrapper_.hasFocus = NO; }
  void WebContentsDestroyed() override {
    ArcWebContents* wrapper = wrapper_;
    ResetPendingNavigation();
    pending_state_events_.clear();
    weak_factory_.InvalidateWeakPtrs();
    Observe(nullptr);
    [wrapper nativeContentsDestroyed];
  }
 private:
  struct PendingStateEvent {
    enum class Kind { kLoading, kProgress };
    Kind kind = Kind::kLoading;
    bool loading = false;
    bool back = false;
    bool forward = false;
    double progress = 0;
  };
  void ResetPendingNavigation() {
    pending_navigation_ = nil;
    pending_navigation_identifier_ = 0;
    pending_navigation_generation_ = 0;
    awaiting_policy_ = false;
  }
  PendingStateEvent CaptureLoadingState() {
    PendingStateEvent event;
    event.loading = web_contents()->ShouldShowLoadingUI();
    event.back = web_contents()->GetController().CanGoBack();
    event.forward = web_contents()->GetController().CanGoForward();
    return event;
  }
  void PublishLoadingState(const PendingStateEvent& state, uint64_t generation) {
    if (!web_contents() || generation != navigation_generation_) { return; }
    ArcWebContents* wrapper = wrapper_;
    auto native = web_contents()->GetWeakPtr();
    wrapper.isLoading = state.loading;
    if (!native || generation != navigation_generation_) { return; }
    wrapper.canGoBack = state.back;
    if (!native || generation != navigation_generation_) { return; }
    wrapper.canGoForward = state.forward;
    id<CoreContentsDelegate> delegate = [wrapper uiDelegate];
    if (native && generation == navigation_generation_ &&
        [delegate respondsToSelector:@selector(webContents:didChangeStatusText:)]) {
      [delegate webContents:wrapper didChangeStatusText:@""];
    }
  }
  void FlushPendingState(uint64_t generation) {
    std::vector<PendingStateEvent> events = std::move(pending_state_events_);
    pending_state_events_.clear();
    PublishPendingState(std::move(events), generation);
  }
  void PublishPendingState(std::vector<PendingStateEvent> events, uint64_t generation) {
    ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) = wrapper_;
    for (const auto& event : events) {
      if (!web_contents() || generation != navigation_generation_) { return; }
      if (event.kind == PendingStateEvent::Kind::kLoading) {
        PublishLoadingState(event, generation);
      } else {
        wrapper.loadProgress = event.progress;
      }
    }
  }
  void FlushUnclaimedLoading(uint64_t token) {
    if (token != pending_loading_token_ || !awaiting_navigation_start_ || awaiting_policy_) {
      return;
    }
    awaiting_navigation_start_ = false;
    FlushPendingState(navigation_generation_);
  }
  void UpdateURLForStart(ArcWebContents* wrapper) {
    if (!web_contents()) { return; }
    saw_navigation_start_ = true;
    const GURL& committed = web_contents()->GetLastCommittedURL();
    NSURL* url = ArcNSURL(committed.is_empty() ? web_contents()->GetVisibleURL() : committed);
    const uint64_t generation = navigation_generation_;
    wrapper.url = url;
    if (web_contents() && generation == navigation_generation_) {
      wrapper.navigationID = ++visible_navigation_id_;
    }
  }
  void UpdateHistory() {
    ArcWebContents* wrapper = wrapper_;
    if (web_contents()) {
      wrapper.backForwardList = [ArcBackForwardList snapshotForWebContents:web_contents()];
    }
  }
  void UpdateLoadingState() {
    if (!web_contents()) { return; }
    PublishLoadingState(CaptureLoadingState(), navigation_generation_);
  }
  __weak ArcWebContents* wrapper_;
  uint64_t navigation_generation_ = 0;
  int visible_navigation_id_ = 0;
  bool saw_navigation_start_ = false;
  bool initial_ready_to_commit_ = false;
  bool awaiting_navigation_start_ = false;
  bool awaiting_policy_ = false;
  ArcNavigation* __strong pending_navigation_ = nil;
  int64_t pending_navigation_identifier_ = 0;
  uint64_t pending_navigation_generation_ = 0;
  uint64_t pending_loading_token_ = 0;
  std::vector<PendingStateEvent> pending_state_events_;
  int last_committed_entry_count_ = 0;
  int early_history_entry_count_ = -1;
  base::WeakPtrFactory<ArcContentsObserver> weak_factory_{this};
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
  std::unique_ptr<arc::CoreRenderState> _renderStateObserver;
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
  uint32_t _javascriptPopupsDisplayedCount;
  BOOL _closing;
  int32_t _sessionIdentifier;
  ArcSessionID* _restorationSessionID;
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
    InstallNavigationPolicyHook();
    if (auto* helper = sessions::SessionTabHelper::FromWebContents(native)) {
      _sessionIdentifier = helper->session_id().id();
    }
    _diagnosticId = @(NextDiagnosticID());
    _services = [NSMutableDictionary dictionary];
    self.title = native->GetTitle().empty() ? nil : ArcNSString(native->GetTitle());
    self.url = ArcNSURL(native->GetVisibleURL());
    self.selectedText = nil;
    self.isLoading = native->ShouldShowLoadingUI();
    self.loadProgress = 0;
    self.favicon = nil;
    self.backForwardList = [ArcBackForwardList snapshotForWebContents:native];
    self.undoableFieldActive = NO;
    self.freeformTextFieldActive = NO;
    self.passwordTextFieldActive = NO;
    self.tabTraits = [NSMutableIndexSet indexSet];
    self.state = 0;
    self.zoomLevel = blink::ZoomLevelToZoomFactor(zoom::ZoomController::GetZoomLevelForWebContents(native));
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
    _renderStateObserver = arc::CreateCoreRenderStateObserver(self, native);
  }
  return self;
}
- (void)dealloc {
  _renderStateObserver.reset();
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
- (void)nativeContentsClosing {
  if (_closing) { return; }
  _closing = YES;
  id<CoreContentsDelegate> delegate = _uiDelegate;
  if ([delegate respondsToSelector:@selector(webContentsClosingImminent:withSessionID:)]) {
    const auto native = SessionID::FromSerializedValue(_sessionIdentifier);
    [delegate webContentsClosingImminent:self
        withSessionID:[[ArcSessionID alloc] initWithSessionId:&native]];
  }
}
- (void)nativeContentsClosingWithRestorationID:(ArcSessionID*)identifier {
  _restorationSessionID = identifier;
  [self nativeContentsClosing];
}
- (void)nativeContentsDestroyed {
  // Native removal records carry an optional historical ID. Absence (including
  // a whole-window close) is exposed as an engaged ArcSessionID(-1), not nil.
  ArcSessionID* restoration = _restorationSessionID ?: [[ArcSessionID alloc] initWithID:-1];
  [self nativeContentsClosing];
  _contents.reset();
  id<CoreContentsDelegate> delegate = _uiDelegate;
  if ([delegate respondsToSelector:@selector(webContents:didCloseWithSessionID:)]) {
    [delegate webContents:self didCloseWithSessionID:restoration];
  }
}
- (BOOL)foregroundForInsertion { return _foreground; }
- (BOOL)isInitialized { return !!_contents; }
- (void)invalidateNativeStateRefresh { ++_refreshGeneration; }
- (void)completeNavigationPolicy:(int64_t)identifier allowed:(BOOL)allowed {
  if (_observer) { _observer->CompleteNavigationPolicy(identifier, allowed); }
}
- (void)refreshNativeState {
  if (!_contents) {
    return;
  }
  // Setters run the app's KVO callbacks, which can close or navigate this contents.
  // Copy all native values before the first setter so no native dereference crosses a callback.
  const uint64_t generation = ++_refreshGeneration;
  auto native = _contents;
  NSString* title = native->GetTitle().empty() ? nil : ArcNSString(native->GetTitle());
  const BOOL loading = native->ShouldShowLoadingUI();
  const BOOL back = native->GetController().CanGoBack();
  const BOOL forward = native->GetController().CanGoForward();
  const float zoom = blink::ZoomLevelToZoomFactor(zoom::ZoomController::GetZoomLevelForWebContents(native.get()));
  content::NavigationEntry* entry = native->GetController().GetLastCommittedEntry();
  NSImage* favicon = entry && entry->GetFavicon().valid ? entry->GetFavicon().image.AsNSImage() : nil;
  self.title = title;
  if (!native || generation != _refreshGeneration) { return; }
  self.isLoading = loading;
  if (!native || generation != _refreshGeneration) { return; }
  self.canGoBack = back;
  if (!native || generation != _refreshGeneration) { return; }
  self.canGoForward = forward;
  if (!native || generation != _refreshGeneration) { return; }
  id<CoreContentsDelegate> delegate = _uiDelegate;
  if ([delegate respondsToSelector:@selector(webContents:didChangeStatusText:)]) {
    [delegate webContents:self didChangeStatusText:@""];
  }
  if (!native || generation != _refreshGeneration) { return; }
  self.favicon = favicon;
  if (!native || generation != _refreshGeneration) { return; }
  self.tabTraits = [NSMutableIndexSet indexSet];
  if (!native || generation != _refreshGeneration) { return; }
  self.zoomLevel = zoom;
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
- (ArcWebContentsNavigationController*)navigationController { return [self coreNavigationController]; }
- (ArcWebContentsNavigationController*)coreNavigationController {
  if (!_navigationController) {
    _navigationController = [[ArcWebContentsNavigationController alloc] initForWebContents:self];
  }
  return _navigationController;
}
- (NSNumber*)diagnosticId { return _diagnosticId; }
- (NSNumber*)coreDiagnosticID { return _diagnosticId; }
- (ArcSessionID*)sessionId {
  if (!_contents) {
    return nil;
  }
  auto* helper = sessions::SessionTabHelper::FromWebContents(_contents.get());
  if (!helper) { return nil; }
  const auto identifier = helper->session_id();
  return [[ArcSessionID alloc] initWithSessionId:&identifier];
}
- (void)setNavigationDelegate:(id)delegate { _navigationDelegate = delegate; }
- (id)navigationDelegate { return _navigationDelegate; }
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
- (void)stop { if (_contents) { _contents->Stop(); } }
- (void)close {
  // Native 4410bac and the accepted sessions golden forward the nonzero
  // timing value. The older spec's default-zero description is incorrect.
  [self closeWithForegroundCommitTiming:1];
}
- (void)closeWithForegroundCommitTiming:(NSInteger)timing {
  // Dispatch accounting and the timing-zero native continuation are still
  // pending; preserve the existing destruction deferral in either mode.
  if (_ownedContents) {
    // Native observers invoke app KVO callbacks. Destruction must wait until
    // Chromium has finished notifying every observer.
    content::GetUIThreadTaskRunner({})->DeleteSoon(FROM_HERE, std::move(_ownedContents));
  } else if (_contents) {
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(
        [](base::WeakPtr<content::WebContents> contents) {
          // Arc's idle native route is WebContents::Close, including the Mac
          // event-tracking check and delegate CloseContents dispatch.
          if (contents) { contents->Close(); }
        }, _contents));
  }
}
- (BOOL)muted { return _contents && _contents->IsAudioMuted(); }
- (void)setMuted:(BOOL)muted { if (_contents) { _contents->SetAudioMuted(muted); } }
- (BOOL)audible { return _contents && _contents->IsCurrentlyAudible(); }
- (BOOL)isScaleFactorOne {
  auto* controller = _contents
      ? zoom::ZoomController::FromWebContents(_contents.get()) : nullptr;
  return controller ? controller->PageScaleFactorIsOne() : YES;
}
- (BOOL)shareable { return _contents && _contents->GetLastCommittedURL().SchemeIsHTTPOrHTTPS(); }
- (BOOL)isEqual:(id)other { return self == other; }
- (BOOL)disallowDownloads { return _disallowDownloads; }
- (void)setDisallowDownloads:(BOOL)disallow { _disallowDownloads = disallow; }
- (int)javascriptPopupsDisplayedCount {
  return std::bit_cast<int32_t>(_javascriptPopupsDisplayedCount);
}
- (void)coreDidShowJavaScriptPopup {
  if (!_isBlockingJavascriptPopups) {
    // Unsigned arithmetic preserves the native counter's 32-bit wrap without
    // signed overflow. The count belongs to this wrapper across navigation.
    ++_javascriptPopupsDisplayedCount;
  }
}
- (BOOL)isBlockingJavascriptPopups { return _isBlockingJavascriptPopups; }
- (void)setIsBlockingJavascriptPopups:(BOOL)blocking { _isBlockingJavascriptPopups = blocking; }
@end

@implementation ArcBrowserContext (Core)
- (ArcWebContents*)createWebContents:(ArcWebContentsConfiguration*)configuration {
  return [ArcWebContents createWithProfile:self.profile configuration:configuration];
}
@end

// UI's dialog producer calls this before looking up the UI delegate. This
// definition alone does not wire the producer; its shared declaration is owned
// by the lead. Never create a wrapper in response to a late native event.
namespace arc {
void DidShowJavaScriptPopup(content::WebContents* contents) {
  if (!contents) {
    return;
  }
  auto* data = ArcContentsData::FromWebContents(contents);
  if (!data) {
    return;
  }
  ArcWebContents* __attribute__((objc_precise_lifetime)) wrapper =
      data->wrapper ?: data->weak_wrapper;
  if (wrapper && [wrapper webContents] == contents) {
    [wrapper coreDidShowJavaScriptPopup];
  }
}
}  // namespace arc

// Weak default so the framework links before the renderer package defines it (renderer's definition wins).
namespace arc {
__attribute__((weak)) void InitializeRendererBridge(content::WebContents* contents) {}
}  // namespace arc

// Core-only marker test used before the tab-strip observer creates wrappers.
// Existing empty/expired userdata still marks contents as already managed,
// matching the native userdata-presence gate rather than wrapper liveness.
namespace arc {
bool HasCoreWebContentsData(content::WebContents* contents) {
  return contents && ArcContentsData::FromWebContents(contents);
}
}  // namespace arc

// Existing-only lookup for app consumers. The ID belongs to the canonical
// wrapper; userdata can outlive an expired weak wrapper without retaining its ID.
bool ArcExistingWebContentsDiagnosticID(content::WebContents* contents,
                                        uint64_t* output) {
  if (!contents) { return false; }
  auto* data = ArcContentsData::FromWebContents(contents);
  if (!data) { return false; }
  ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) =
      data->wrapper ?: data->weak_wrapper;
  if (!wrapper || [wrapper webContents] != contents) { return false; }
  NSNumber* identifier = [wrapper coreDiagnosticID];
  if (!identifier) { return false; }
  if (output) { *output = identifier.unsignedLongLongValue; }
  return true;
}
