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
#include "chrome/browser/lifetime/browser_shutdown.h"
#include "base/functional/bind.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/tab_contents/core_tab_helper.h"
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
#include "content/public/browser/navigation_details.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/favicon_status.h"
#include "content/public/browser/host_app_hooks.h"
#include "content/public/browser/reload_type.h"
#include "content/public/browser/navigation_throttle.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/browser/visibility.h"
#include "ui/base/page_transition_types.h"
#include "components/url_formatter/elide_url.h"
#include "ui/gfx/font_list.h"

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

@class CoreUIDelegateBridge;
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
- (void)nativeNavigationStateChanged:(content::InvalidateTypes)flags fromContents:(content::WebContents*)contents __attribute__((objc_direct));
- (void)nativeTargetURLChanged:(const GURL&)url fromContents:(content::WebContents*)contents __attribute__((objc_direct));
- (CoreUIDelegateBridge*)coreUIDelegateBridge __attribute__((objc_direct));
- (id)coreUIDelegate __attribute__((objc_direct));
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

namespace {
struct CoreSecuritySnapshot {
  NSUInteger state = 0;
};
}  // namespace

@implementation ArcSecureContext {
  NSUInteger _state;
}
- (instancetype)initWithSecureContext:(const void*)snapshot {
  if ((self = [super init]) && snapshot) {
    _state = static_cast<const CoreSecuritySnapshot*>(snapshot)->state;
  }
  return self;
}
- (NSUInteger)state { return _state; }
@end

@protocol CoreContentsDelegate <NSObject>
- (NSInteger)webContents:(ArcWebContents*)contents decidePolicyForNavigationAction:(ArcNavigationAction*)action;
- (void)webContentsClosingImminent:(ArcWebContents*)contents withSessionID:(ArcSessionID*)identifier;
- (void)webContents:(ArcWebContents*)contents didCloseWithSessionID:(ArcSessionID*)identifier;
- (void)webContents:(ArcWebContents*)contents didChangeStatusText:(NSString*)text;
- (void)webContents:(ArcWebContents*)contents didChangeVisibleSecurityState:(ArcSecureContext*)context;
@end

// The stored bridge survives nil unregister. Every nonnil delegate assignment
// replaces it, so an old queued weak bridge expires even for the same delegate.
@interface CoreUIDelegateBridge : NSObject
@property(nonatomic, weak) id<CoreContentsDelegate> delegate;
@end
@implementation CoreUIDelegateBridge
@synthesize delegate = _delegate;
@end

namespace {
void PostCoreUI(void (^callback)()) {
  content::GetUIThreadTaskRunner({})->PostTask(
      FROM_HERE, base::BindOnce([](void (^work)()) { work(); }, callback));
}

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
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:handle->GetWebContents()];
    id<CoreContentsDelegate> delegate = [owner navigationDelegate];
    if (![delegate respondsToSelector:@selector(webContents:decidePolicyForNavigationAction:)]) {
      return PROCEED;
    }
    const NSInteger policy = [delegate webContents:owner
        decidePolicyForNavigationAction:arc::CreateNavigationAction(handle, 1)];
    CHECK_GE(policy, 0);
    CHECK_LT(policy, 12);
    // The delegate returns the destination disposition; current-tab navigation
    // continues here. A declined/redirected request must not also commit here.
    return policy == 1 ? PROCEED : CANCEL_AND_IGNORE;
  }
};

void ForwardNavigationState(content::WebContents*, content::InvalidateTypes);
void ForwardTargetURL(content::WebContents*, const GURL&);

void InstallNavigationPolicyHook() {
  content::GetHostNavigationStateCallback() = base::BindRepeating(&ForwardNavigationState);
  content::GetHostTargetURLCallback() = base::BindRepeating(&ForwardTargetURL);
  arcmod::GetNavigationThrottleHook() = base::BindRepeating(
      [](content::NavigationThrottleRegistry& registry) {
        registry.AddThrottle(std::make_unique<CoreNavigationPolicyThrottle>(registry));
      });
}

class ArcContentsObserver : public content::WebContentsObserver {
 public:
  ArcContentsObserver(content::WebContents* contents, ArcWebContents* wrapper)
      : content::WebContentsObserver(contents), wrapper_(wrapper),
        history_model_(arc::CreateCoreHistoryModel(contents)) {}
  void DidStartNavigation(content::NavigationHandle* handle) override {
    if (!handle->IsInPrimaryMainFrame()) { return; }
    ArcWebContents* wrapper = wrapper_;
    ++navigation_generation_;
    [wrapper invalidateNativeStateRefresh];
    // The controller posts the observer call at receipt, like native 436bbc0.
    // Synchronous throttle policy naturally precedes delivery on this UI stack;
    // no barrier is imposed across an asynchronous beforeunload or nested loop.
    [[wrapper coreNavigationController] didStartNavigation:handle];
  }
  void DidRedirectNavigation(content::NavigationHandle* handle) override {
    ArcWebContents* wrapper = wrapper_;
    [[wrapper coreNavigationController] didRedirectNavigation:handle];
  }
  void DidFinishNavigation(content::NavigationHandle* handle) override {
    if (!handle->IsInPrimaryMainFrame()) { return; }
    ArcWebContents* wrapper = wrapper_;
    const bool committed = handle->HasCommitted();
    const bool in_page = handle->IsSameDocument();
    const bool restored = handle->IsServedFromBackForwardCache();
    const int error = handle->GetNetErrorCode();
    const uint64_t generation = ++navigation_generation_;
    auto native = web_contents()->GetWeakPtr();
    ArcNavigation* navigation = [ArcNavigation snapshotForNavigationHandle:handle];
    if (committed) { [wrapper refreshNativeState]; }
    // Zoom still calls out synchronously. A close or superseding navigation in
    // that setter must not let this older finish overwrite controller state.
    // This protects event receipt; queued deliveries have no generation filter.
    if (!native || generation != navigation_generation_) { return; }
    [[wrapper coreNavigationController] didFinishNavigationSnapshot:navigation inPage:in_page
        committed:committed error:error restored:restored];
  }
  void DidFinishLoad(content::RenderFrameHost* frame, const GURL&) override {
    if (web_contents() && frame == web_contents()->GetPrimaryMainFrame()) {
      [[wrapper_ coreNavigationController] didFinishDocumentLoad];
    }
  }
  void NativeNavigationStateChanged(content::InvalidateTypes flags) {
    if (!web_contents()) { return; }
    const auto navigation_receipt =
        content::host_receipt_diagnostics::Begin(web_contents());
    content::host_receipt_diagnostics::Emit(
        navigation_receipt, "navigation-wc-receipt", next_navigation_id_,
        0, static_cast<int>(flags));
    const NativeNavigationState state = CaptureState(flags);
    content::host_receipt_diagnostics::Emit(
        navigation_receipt, "navigation-captured", next_navigation_id_,
        std::bit_cast<uint32_t>(state.identifier), static_cast<int>(flags));
    __weak ArcWebContents* weak_owner = wrapper_;
    // Native 4385538 posts each branch separately in this order. Capture and
    // counter advancement happen now; delivery is not tied to a navigation
    // generation, NavigationHandle, observer lifetime, or native WC lifetime.
    if (flags & content::INVALIDATE_TYPE_URL) {
      PostCoreUI(^{
        ArcWebContents* owner = weak_owner;
        const uintptr_t owner_identity = reinterpret_cast<uintptr_t>(
            (__bridge const void*)owner);
        content::host_receipt_diagnostics::Emit(
            navigation_receipt, "url-delivery", owner_identity,
            std::bit_cast<uint32_t>(state.identifier),
            static_cast<int>(flags), !!owner);
        if (!owner) { return; }
        owner.url = state.url;
        owner.navigationID = state.identifier;
        // Scalar-only return receipt: do not extend this non-precise owner's
        // lifetime by reading it again after its final existing setter call.
        content::host_receipt_diagnostics::Emit(
            navigation_receipt, "url-navigationid-return", owner_identity,
            std::bit_cast<uint32_t>(state.identifier),
            static_cast<int>(flags), true);
      });
    }
    if (flags & content::INVALIDATE_TYPE_TITLE) {
      PostCoreUI(^{
        ArcWebContents* owner = weak_owner;
        if (owner) { owner.title = state.title; }
      });
    }
    if (flags & content::INVALIDATE_TYPE_LOAD) {
      PostCoreUI(^{
        ArcWebContents* owner = weak_owner;
        if (!owner) { return; }
        owner.isLoading = state.loading;
        // Native 44130fc queries history after loading, then again after the
        // back setter. Reacquire the current native pointer after each callout.
        auto* native = [owner webContents];
        const bool back = native && native->GetController().CanGoBack();
        owner.canGoBack = back;
        native = [owner webContents];
        const bool forward = native && native->GetController().CanGoForward();
        owner.canGoForward = forward;
      });
      __weak CoreUIDelegateBridge* weak_bridge = [wrapper_ coreUIDelegateBridge];
      PostCoreUI(^{
        ArcWebContents* owner = weak_owner;
        CoreUIDelegateBridge* bridge = weak_bridge;
        id<CoreContentsDelegate> delegate = bridge.delegate;
        if (owner && [delegate respondsToSelector:@selector(webContents:didChangeStatusText:)]) {
          [delegate webContents:owner didChangeStatusText:state.status];
        }
      });
    }
    if (flags & content::INVALIDATE_TYPE_TAB) {
      PostCoreUI(^{
        ArcWebContents* owner = weak_owner;
        if (owner) { owner.favicon = state.favicon.IsEmpty() ? nil : state.favicon.AsNSImage(); }
      });
    }
    // Native TAB falls through to traits; TAB|AUDIO still posts once.
    if (flags & (content::INVALIDATE_TYPE_TAB | content::INVALIDATE_TYPE_AUDIO)) {
      PostCoreUI(^{
        // Native 4413050 materializes a fresh set per delivery, before loading
        // its weak owner. Keep the receipt-time payload as native ordinals.
        NSMutableIndexSet* traits = [NSMutableIndexSet indexSet];
        for (uint32_t trait : state.traits) {
          if (trait <= 13) { [traits addIndex:trait]; }
        }
        ArcWebContents* owner = weak_owner;
        if (owner) { owner.tabTraits = traits; }
      });
    }
  }
  void NativeTargetURLChanged(const GURL& url) {
    ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) = wrapper_;
    NSString* text = url.is_empty() ? @"" : ArcNSString(
        url_formatter::ElideUrl(url, gfx::FontList(), 400));
    // Native 4388630 uses InvokeSync, after forwarding to Chrome. The normal
    // renderer and ClearTargetURL sites both reach the hook. Nondefault native
    // width/font selection remains unaudited; 400/default is the proven fallback.
    CoreUIDelegateBridge* bridge = [wrapper coreUIDelegateBridge];
    id<CoreContentsDelegate> delegate = bridge.delegate;
    if ([delegate respondsToSelector:@selector(webContents:didChangeStatusText:)]) {
      [delegate webContents:wrapper didChangeStatusText:text];
    }
  }
  void NavigationEntryCommitted(const content::LoadCommittedDetails&) override {
    UpdateHistory();
  }
  void NavigationListPruned(const content::PrunedDetails&) override { UpdateHistory(); }
  void NavigationEntriesDeleted() override { UpdateHistory(); }
  void NavigationEntryChanged(const content::EntryChangedDetails& details) override {
    if (arc::UpdateCoreHistoryEntry(history_model_, details.changed_entry,
                                    details.index)) {
      PostHistory();
    }
  }
  void LoadProgressChanged(double progress) override {
    __weak ArcWebContents* weak_owner = wrapper_;
    const auto receipt = content::host_receipt_diagnostics::Begin(web_contents());
    content::host_receipt_diagnostics::Emit(
        receipt, "progress-wco-enqueue", 0, 0, progress);
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      const uintptr_t owner_identity = reinterpret_cast<uintptr_t>(
          (__bridge const void*)owner);
      content::host_receipt_diagnostics::Emit(
          receipt, "progress-delivery", owner_identity, 0, progress, !!owner);
      if (owner) {
        owner.loadProgress = progress;
      }
      content::host_receipt_diagnostics::Emit(
          receipt, "progress-return", owner_identity, 0, progress, !!owner);
    });
  }
  void OnVisibilityChanged(content::Visibility visibility) override {
    const NSInteger value = static_cast<NSInteger>(visibility);
    __weak ArcWebContents* weak_owner = wrapper_;
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      if (owner) { owner.visibility = value; }
    });
  }
  void DidChangeVisibleSecurityState() override {
    if (!web_contents()) { return; }
    const auto level = chrome_security_state::GetSecurityLevel(web_contents());
    const CoreSecuritySnapshot snapshot{
        level == security_state::NONE ? 0u : level == security_state::SECURE ? 1u : 2u};
    __weak ArcWebContents* weak_owner = wrapper_;
    __weak CoreUIDelegateBridge* weak_bridge = [wrapper_ coreUIDelegateBridge];
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      CoreUIDelegateBridge* bridge __attribute__((objc_precise_lifetime)) = weak_bridge;
      id<CoreContentsDelegate> delegate = bridge.delegate;
      if (owner && [delegate respondsToSelector:@selector(webContents:didChangeVisibleSecurityState:)]) {
        ArcSecureContext* context = [[ArcSecureContext alloc] initWithSecureContext:&snapshot];
        [delegate webContents:owner didChangeVisibleSecurityState:context];
      }
    });
  }
  void DidChangeThemeColor() override {
    if (!web_contents()) { return; }
    const auto color = web_contents()->GetThemeColor();
    __weak ArcWebContents* weak_owner = wrapper_;
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      if (owner) { owner.themeColor = color ? [ArcColor colorWithSkColor:*color] : nil; }
    });
  }
  void OnWebContentsFocused(content::RenderWidgetHost*) override {
    if (!web_contents()) { return; }
    auto native = web_contents()->GetWeakPtr();
    __weak ArcWebContents* weak_owner = wrapper_;
    // Gained focus first hops through a weak native task. Only a surviving
    // native peer produces the second, weak-owner property publication.
    PostCoreUI(^{
      if (native) { PostFocus(weak_owner, true); }
    });
  }
  void OnWebContentsLostFocus(content::RenderWidgetHost*) override {
    PostFocus(wrapper_, false);
  }
  void WebContentsDestroyed() override {
    ArcWebContents* wrapper = wrapper_;
    Observe(nullptr);
    [wrapper nativeContentsDestroyed];
  }
 private:
  struct NativeNavigationState {
    int32_t identifier = 0;
    bool loading = false;
    NSURL* __strong url = nil;
    NSString* __strong title = nil;
    NSString* __strong status = nil;
    gfx::Image favicon;
    std::vector<uint32_t> traits;
  };
  NativeNavigationState CaptureState(content::InvalidateTypes flags) {
    NativeNavigationState state;
    auto* native = web_contents();
    if (flags & content::INVALIDATE_TYPE_URL) {
      auto* committed = native->GetController().GetLastCommittedEntry();
      const GURL& url = committed ? committed->GetVirtualURL() : native->GetVisibleURL();
      state.url = ArcNSURL(url.is_empty() ? native->GetVisibleURL() : url);
      // Native 4385614 captures the old signed32 value and advances immediately,
      // even if its queued delivery is subsequently canceled by wrapper death.
      state.identifier = std::bit_cast<int32_t>(next_navigation_id_++);
    }
    if (flags & content::INVALIDATE_TYPE_TITLE) {
      state.title = native->GetTitle().empty() ? nil : ArcNSString(native->GetTitle());
    }
    if (flags & content::INVALIDATE_TYPE_LOAD) {
      state.loading = native->ShouldShowLoadingUI();
      auto* helper = CoreTabHelper::FromWebContents(native);
      // GetStatusText delegates to the exact static native status helper.
      state.status = helper ? ArcNSString(helper->GetStatusText()) : @"";
    }
    if (flags & content::INVALIDATE_TYPE_TAB) {
      auto* entry = native->GetController().GetLastCommittedEntry();
      if (entry && entry->GetFavicon().valid) { state.favicon = entry->GetFavicon().image; }
    }
    if (flags & (content::INVALIDATE_TYPE_TAB | content::INVALIDATE_TYPE_AUDIO)) {
      // The nonempty cached-alert/ICE provider is staged separately. Preserve
      // its native vector until each delivery constructs the Objective-C set.
      state.traits = {};
    }
    return state;
  }
  static void PostFocus(ArcWebContents* wrapper, bool focused) {
    __weak ArcWebContents* weak_owner = wrapper;
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      if (owner) { owner.hasFocus = focused; }
    });
  }
  void UpdateHistory() {
    if (!web_contents()) { return; }
    arc::RefreshCoreHistoryModel(history_model_, web_contents());
    PostHistory();
  }
  void PostHistory() {
    const auto model = history_model_;
    __weak ArcWebContents* weak_owner = wrapper_;
    PostCoreUI(^{
      ArcWebContents* owner __attribute__((objc_precise_lifetime)) = weak_owner;
      if (owner) { owner.backForwardList = arc::CreateCoreBackForwardList(model); }
    });
  }
  __weak ArcWebContents* wrapper_;
  uint64_t navigation_generation_ = 0;
  uint32_t next_navigation_id_ = 0;
  std::shared_ptr<arc::CoreHistoryModel> history_model_;
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

void ForwardNavigationState(content::WebContents* contents, content::InvalidateTypes flags) {
  auto* data = ArcContentsData::FromWebContents(contents);
  ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) =
      data ? (data->wrapper ?: data->weak_wrapper) : nil;
  [wrapper nativeNavigationStateChanged:flags fromContents:contents];
}
void ForwardTargetURL(content::WebContents* contents, const GURL& url) {
  auto* data = ArcContentsData::FromWebContents(contents);
  ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) =
      data ? (data->wrapper ?: data->weak_wrapper) : nil;
  [wrapper nativeTargetURLChanged:url fromContents:contents];
}


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
  CoreUIDelegateBridge* _uiDelegateBridge;
  __weak CoreUIDelegateBridge* _activeUIDelegateBridge;
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
  // Match the original Closed producer: quit suppresses its app delivery while
  // cleanup still runs. Snapshot before any app callout can change quit state.
  const bool trying_to_quit = browser_shutdown::IsTryingToQuit();
  // Native removal records carry an optional historical ID. Absence (including
  // a whole-window close) is exposed as an engaged ArcSessionID(-1), not nil.
  ArcSessionID* restoration = _restorationSessionID ?: [[ArcSessionID alloc] initWithID:-1];
  [self nativeContentsClosing];
  _contents.reset();
  if (!trying_to_quit) {
    id<CoreContentsDelegate> delegate = _uiDelegate;
    if ([delegate respondsToSelector:@selector(webContents:didCloseWithSessionID:)]) {
      [delegate webContents:self didCloseWithSessionID:restoration];
    }
  }
}
- (BOOL)foregroundForInsertion { return _foreground; }
- (BOOL)isInitialized { return !!_contents; }
- (void)invalidateNativeStateRefresh { ++_refreshGeneration; }
- (void)nativeNavigationStateChanged:(content::InvalidateTypes)flags
                       fromContents:(content::WebContents*)contents {
  if (_contents.get() == contents && _observer) { _observer->NativeNavigationStateChanged(flags); }
}
- (void)nativeTargetURLChanged:(const GURL&)url fromContents:(content::WebContents*)contents {
  if (_contents.get() == contents && _observer) { _observer->NativeTargetURLChanged(url); }
}
- (CoreUIDelegateBridge*)coreUIDelegateBridge { return _activeUIDelegateBridge; }
- (id)coreUIDelegate { return _uiDelegate; }
- (void)refreshNativeState {
  // Native invalidation hooks now own URL/title/loading/history flags/favicon.
  // Zoom's existing per-commit scheduling is outside this dispatcher audit.
  if (_contents) {
    self.zoomLevel = blink::ZoomLevelToZoomFactor(
        zoom::ZoomController::GetZoomLevelForWebContents(_contents.get()));
  }
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
- (void)setUIDelegate:(id)delegate {
  _uiDelegate = delegate;
  if (delegate) {
    CoreUIDelegateBridge* bridge = [[CoreUIDelegateBridge alloc] init];
    bridge.delegate = delegate;
    _uiDelegateBridge = bridge;
    _activeUIDelegateBridge = bridge;
  } else {
    // Native nil unregister leaves the last stored shared bridge alive. Old
    // queued weak-bridge status tasks remain eligible; future captures stop.
    _activeUIDelegateBridge = nil;
  }
}
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
