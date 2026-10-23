// Copyright 2026 ArcMod. Everyday page features on ArcWebContents: find, zoom, print, save, fullscreen, unload.
#import "arc/api/arc_internal.h"
#include "arc/browser/core_values.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "base/time/time.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/printing/print_view_manager_common.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "components/blocked_content/popup_blocker_tab_helper.h"
#include "components/find_in_page/find_notification_details.h"
#include "components/find_in_page/find_result_observer.h"
#include "components/find_in_page/find_tab_helper.h"
#include "components/find_in_page/find_types.h"
#include "components/tabs/public/tab_interface.h"
#include "components/zoom/zoom_controller.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/browsing_data_filter_builder.h"
#include "content/public/browser/browsing_data_remover.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/reload_type.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/browser/renderer_host/render_frame_metadata_provider_impl.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "third_party/blink/public/common/page/page_zoom.h"
#include "ui/base/window_open_disposition.h"
#include "url/origin.h"

// ---- Value classes -----------------------------------------------------------------------------

namespace {
// Same layout as app_values::ErrorStatus (arc_app_values.mm), which owns ArcErrorStatus.
struct PageErrorStatus {
  int32_t process_termination_status;
  int32_t crashed_error_code;
};
}  // namespace

// Primary implementation: arc_app_values.mm. Declared here only to construct it.
@interface ArcErrorStatus : NSObject
- (instancetype)initWithErrorStatus:(const PageErrorStatus*)status;
@end

// ---- Helpers -----------------------------------------------------------------------------------

namespace {

using FindCompletion = void (^)(ArcFindResult*);

ArcFindResult* MakeFindResult(int matches, int ordinal) {
  const FindResult result{matches, ordinal};
  return [[ArcFindResult alloc] initWithFindResult:&result];
}

// Delivers at most one pending find completion per tab: on the final result of the latest request.
class FindCompletionState : public content::WebContentsUserData<FindCompletionState>,
                            public find_in_page::FindResultObserver {
 public:
  ~FindCompletionState() override = default;

  void Begin(find_in_page::FindTabHelper* helper, FindCompletion completion) {
    if (!observation_.IsObservingSource(helper)) {
      observation_.Reset();
      observation_.Observe(helper);
    }
    completion_ = [completion copy];
  }
  void Cancel() { completion_ = nil; }

  void OnFindResultAvailable(content::WebContents* web_contents) override {
    auto* helper = find_in_page::FindTabHelper::FromWebContents(web_contents);
    if (!helper || !helper->find_result().final_update()) {
      return;
    }
    const auto& details = helper->find_result();
    Deliver(details.number_of_matches(), details.active_match_ordinal());
  }
  void OnFindEmptyText(content::WebContents*) override { Deliver(0, 0); }
  void OnFindTabHelperDestroyed(find_in_page::FindTabHelper*) override {
    observation_.Reset();
    Deliver(0, 0);
  }

 private:
  friend class content::WebContentsUserData<FindCompletionState>;
  explicit FindCompletionState(content::WebContents* contents)
      : content::WebContentsUserData<FindCompletionState>(*contents) {}

  void Deliver(int matches, int ordinal) {
    FindCompletion completion = completion_;
    completion_ = nil;
    if (completion) {
      completion(MakeFindResult(matches, ordinal));
    }
  }

  FindCompletion completion_;
  base::ScopedObservation<find_in_page::FindTabHelper, find_in_page::FindResultObserver> observation_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(FindCompletionState);

// Per-tab flags set by the app.
class PageFlags : public content::WebContentsUserData<PageFlags> {
 public:
  bool ignore_disable_pip_attribute = false;
  bool skip_user_activation_for_pip = false;
  std::array<float, 4> safe_area = {0, 0, 0, 0};

 private:
  friend class content::WebContentsUserData<PageFlags>;
  explicit PageFlags(content::WebContents* contents) : content::WebContentsUserData<PageFlags>(*contents) {}
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(PageFlags);

// Self-deleting observer that reloads |contents| once its site data is cleared.
class ClearSiteDataReloader : public content::BrowsingDataRemover::Observer {
 public:
  explicit ClearSiteDataReloader(base::WeakPtr<content::WebContents> contents) : contents_(std::move(contents)) {}
  void OnBrowsingDataRemoverDone(uint64_t) override {
    if (contents_) {
      contents_->GetController().Reload(content::ReloadType::BYPASSING_CACHE, true);
    }
    delete this;
  }

 private:
  base::WeakPtr<content::WebContents> contents_;
};

// The native global latch read by -consumeExternalProtocolLaunchPermission.
constinit bool g_external_protocol_launch_latch = false;

NSError* PageError(NSInteger code, NSString* reason) {
  return [NSError errorWithDomain:@"company.thebrowser.Arc"
                             code:code
                         userInfo:@{NSLocalizedDescriptionKey : reason}];
}

BrowserWindowInterface* BrowserFor(content::WebContents* contents) {
  auto* tab = tabs::TabInterface::MaybeGetFromContents(contents);
  return tab ? tab->GetBrowserWindowInterface() : nullptr;
}

}  // namespace

// ---- ArcWebContents (Page) -------------------------------------------------------------------------

@implementation ArcWebContents (Page)

- (void)find:(NSString*)text
       forward:(BOOL)forward
     matchCase:(BOOL)matchCase
     findMatch:(BOOL)findMatch
    completion:(void (^)(ArcFindResult*))completion {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    if (completion) { completion(MakeFindResult(0, 0)); }
    return;
  }
  auto* helper = find_in_page::FindTabHelper::FromWebContents(contents);
  if (!helper) {
    find_in_page::FindTabHelper::CreateForWebContents(contents);
    helper = find_in_page::FindTabHelper::FromWebContents(contents);
  }
  FindCompletionState::CreateForWebContents(contents);
  auto* state = FindCompletionState::FromWebContents(contents);
  if (completion) {
    state->Begin(helper, completion);
  } else {
    state->Cancel();
  }
  helper->StartFinding(ArcUTF16(text), forward, matchCase, findMatch);
}

- (void)stopFind {
  content::WebContents* contents = [self webContents];
  if (!contents) { return; }
  if (auto* state = FindCompletionState::FromWebContents(contents)) {
    state->Cancel();
  }
  if (auto* helper = find_in_page::FindTabHelper::FromWebContents(contents)) {
    helper->StopFinding(find_in_page::SelectionAction::kClear);
  }
}

- (void)setZoomFactor:(float)factor {
  content::WebContents* contents = [self webContents];
  if (!contents) { return; }
  if (auto* controller = zoom::ZoomController::FromWebContents(contents)) {
    controller->SetZoomLevel(blink::ZoomFactorToZoomLevel(factor));
  }
}

- (float)zoomFactor {
  content::WebContents* contents = [self webContents];
  if (!contents) { return 1.0f; }
  return blink::ZoomLevelToZoomFactor(zoom::ZoomController::GetZoomLevelForWebContents(contents));
}

- (void)resetZoomAndScaleFactors {
  content::WebContents* contents = [self webContents];
  if (!contents) { return; }
  if (auto* controller = zoom::ZoomController::FromWebContents(contents)) {
    controller->SetZoomLevel(controller->GetDefaultZoomLevel());
  }
}

- (void)print {
  if (content::WebContents* contents = [self webContents]) {
    printing::StartPrint(contents, /*print_preview_disabled=*/false, /*has_selection=*/false);
  }
}

- (void)basicPrint {
  if (content::WebContents* contents = [self webContents]) {
    printing::StartBasicPrint(contents);
  }
}

- (void)save {
  if (content::WebContents* contents = [self webContents]) {
    contents->OnSavePage();
  }
}

- (void)viewSource {
  if (content::WebContents* contents = [self webContents]) {
    contents->GetPrimaryMainFrame()->ViewSource();
  }
}

- (BOOL)isFullscreen {
  content::WebContents* contents = [self webContents];
  return contents && contents->IsFullscreen();
}

- (void)exitFullscreen {
  content::WebContents* contents = [self webContents];
  if (contents && contents->IsFullscreen()) {
    contents->ExitFullscreen(/*will_cause_resize=*/false);
  }
}

- (float)deviceScaleFactor {
  content::WebContents* contents = [self webContents];
  auto* view = contents ? contents->GetRenderWidgetHostView() : nullptr;
  return view ? view->GetDeviceScaleFactor() : 1.0f;
}

// From the compositor's frame metadata, which the renderer sends whenever it changes.
- (BOOL)isScrollOffsetAtTop {
  content::WebContents* contents = [self webContents];
  content::RenderWidgetHostView* view = contents ? contents->GetRenderWidgetHostView() : nullptr;
  auto* host = view ? content::RenderWidgetHostImpl::From(view->GetRenderWidgetHost()) : nullptr;
  return host ? host->render_frame_metadata_provider()->LastRenderFrameMetadata().is_scroll_offset_at_top : YES;
}

- (NSString*)contentsMimeType {
  content::WebContents* contents = [self webContents];
  return contents ? ArcNSString(contents->GetContentsMimeType()) : @"";
}

- (ArcErrorStatus*)errorStatus {
  content::WebContents* contents = [self webContents];
  const PageErrorStatus status{
      contents ? static_cast<int32_t>(contents->GetCrashedStatus()) : 0,
      contents ? contents->GetCrashedErrorCode() : 0};
  return [[ArcErrorStatus alloc] initWithErrorStatus:&status];
}

- (BOOL)willRunUnloadEvents {
  content::WebContents* contents = [self webContents];
  return contents && contents->NeedToFireBeforeUnloadOrUnloadEvents();
}

- (void)hasUnloadHandler:(void (^)(BOOL))completion {
  if (!completion) { return; }
  content::WebContents* contents = [self webContents];
  const BOOL has = contents && contents->NeedToFireBeforeUnloadOrUnloadEvents();
  void (^block)(BOOL) = [completion copy];
  content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(^{ block(has); }));
}

- (BOOL)consumeExternalProtocolLaunchPermission {
  if (![self webContents]) { return NO; }
  const bool previous = g_external_protocol_launch_latch;
  if (!previous) {
    g_external_protocol_launch_latch = true;
  }
  return !previous;
}

- (void)showBlockedPopups {
  content::WebContents* contents = [self webContents];
  auto* helper = contents ? blocked_content::PopupBlockerTabHelper::FromWebContents(contents) : nullptr;
  if (!helper) { return; }
  for (const auto& [popup_id, url] : helper->GetBlockedPopupRequests()) {
    helper->ShowBlockedPopup(popup_id, WindowOpenDisposition::NEW_FOREGROUND_TAB);
  }
}

- (BOOL)clearSiteDataAndReload:(NSError**)error {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    if (error) { *error = PageError(1, @"The web contents is gone."); }
    return NO;
  }
  const GURL url = contents->GetLastCommittedURL();
  if (!url.SchemeIsHTTPOrHTTPS()) {
    if (error) { *error = PageError(2, @"The page has no site data to clear."); }
    return NO;
  }
  auto filter = content::BrowsingDataFilterBuilder::Create(content::BrowsingDataFilterBuilder::Mode::kDelete);
  filter->AddOrigin(url::Origin::Create(url));
  contents->GetBrowserContext()->GetBrowsingDataRemover()->RemoveWithFilterAndReply(
      base::Time(), base::Time::Max(),
      content::BrowsingDataRemover::DATA_TYPE_COOKIES | content::BrowsingDataRemover::DATA_TYPE_CACHE |
          content::BrowsingDataRemover::DATA_TYPE_DOM_STORAGE,
      content::BrowsingDataRemover::ORIGIN_TYPE_UNPROTECTED_WEB | content::BrowsingDataRemover::ORIGIN_TYPE_PROTECTED_WEB,
      std::move(filter), new ClearSiteDataReloader(contents->GetWeakPtr()));
  return YES;
}

- (BOOL)activateTab:(NSError**)error {
  content::WebContents* contents = [self webContents];
  BrowserWindowInterface* browser = contents ? BrowserFor(contents) : nullptr;
  const int index = browser ? browser->GetTabStripModel()->GetIndexOfWebContents(contents) : TabStripModel::kNoTab;
  if (index == TabStripModel::kNoTab) {
    if (error) { *error = PageError(3, @"The tab is not in a browser."); }
    return NO;
  }
  browser->GetTabStripModel()->ActivateTabAt(index);
  return YES;
}

// Arc's auto picture-in-picture flags. The app sends NO (Chrome's default behavior) in every recorded scenario;
// honoring YES needs the renderer to ignore disablePictureInPicture / the activation requirement (not yet).
- (void)ignoreDisablePiPAttribute:(BOOL)ignore {
  if (content::WebContents* contents = [self webContents]) {
    PageFlags::CreateForWebContents(contents);
    PageFlags::FromWebContents(contents)->ignore_disable_pip_attribute = ignore;
  }
}

- (void)skipUserActivationForPiP:(BOOL)skip {
  if (content::WebContents* contents = [self webContents]) {
    PageFlags::CreateForWebContents(contents);
    PageFlags::FromWebContents(contents)->skip_user_activation_for_pip = skip;
  }
}

// Kept for the page's safe-area insets (CSS env(safe-area-inset-*)); not forwarded to Blink yet.
- (void)setSafeAreaInsets:(float)top left:(float)left bottom:(float)bottom right:(float)right {
  if (content::WebContents* contents = [self webContents]) {
    PageFlags::CreateForWebContents(contents);
    PageFlags::FromWebContents(contents)->safe_area = {top, left, bottom, right};
  }
}

- (NSUInteger)hash {
  return reinterpret_cast<NSUInteger>((__bridge void*)self);
}

@end
