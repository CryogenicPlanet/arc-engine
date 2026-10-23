// Copyright 2026 ArcMod. ArcWebContents services: script, capture, dialogs, translate, casting, sharing,
// gestures, rendering bootstrap, accessibility override and freezing votes.

#import "arc/api/arc_internal.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "base/base64.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/task/single_thread_task_runner.h"
#include "base/values.h"
#include "chrome/browser/certificate_viewer.h"
#include "chrome/browser/media/router/media_router_feature.h"
#include "chrome/browser/translate/chrome_translate_client.h"
#include "chrome/browser/ui/tab_dialogs.h"
#include "chrome/browser/ui/tab_sharing/tab_sharing_infobar_delegate.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "components/infobars/core/infobar_delegate.h"
#include "components/media_router/browser/media_router_dialog_controller.h"
#include "components/media_router/browser/media_router_metrics.h"
#include "components/performance_manager/public/freezing/freezing.h"
#include "components/performance_manager/public/performance_manager.h"
#include "components/translate/core/browser/language_state.h"
#include "components/translate/core/browser/translate_download_manager.h"
#include "components/translate/core/browser/translate_manager.h"
#include "components/translate/core/language_detection/language_detection_util.h"
#include "content/public/browser/browser_accessibility_state.h"
#include "content/public/browser/devtools_agent_host.h"
#include "content/public/browser/devtools_agent_host_client.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/scoped_accessibility_mode.h"
#include "content/public/browser/ssl_status.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/accessibility/ax_mode.h"
#include "ui/gfx/geometry/size.h"

using FreezingVotePtr = std::unique_ptr<performance_manager::freezing::FreezingVote>;

// ---- ArcFreezingVoteToken ------------------------------------------------------------------------

// Move-owned native vote (Arc's engine keeps an optional<arc::FreezingVoteToken> holding a unique_ptr).
// The performance-manager vote is released by -clearVote or when the token dies.
ARC_EXPORT @interface ArcFreezingVoteToken : NSObject
- (const void*)freezing_vote_token;
// |token| is a FreezingVotePtr*; its vote is moved into the receiver and the source is cleared.
- (instancetype)initWithFreezingVoteToken:(void*)token;
- (void)clearVote;
@end

@implementation ArcFreezingVoteToken {
  FreezingVotePtr _vote;
}

- (const void*)freezing_vote_token {
  return &_vote;
}

- (instancetype)initWithFreezingVoteToken:(void*)token {
  if ((self = [super init]) && token) {
    _vote = std::move(*static_cast<FreezingVotePtr*>(token));
  }
  return self;
}

- (void)clearVote {
  _vote.reset();
}

@end

// ---- Per-contents service state ------------------------------------------------------------------

namespace {

// Isolated world Arc runs its page scripts in (the engine passes the literal 4).
constexpr int32_t kArcScriptWorldId = 4;
// Gesture masks are 5 bits wide in Arc's engine (mask & 0x1f).
constexpr NSInteger kGestureMask = 0x1f;
// Arc turns on the screen-reader flavor of the full accessibility mode (0x40b).
constexpr ui::AXMode kArcAccessibilityOverrideMode(ui::AXMode::kNativeAPIs | ui::AXMode::kWebContents |
                                                   ui::AXMode::kExtendedProperties | ui::AXMode::kScreenReader);

class ServicesState : public content::WebContentsUserData<ServicesState> {
 public:
  ~ServicesState() override = default;

  std::unique_ptr<content::ScopedAccessibilityMode> accessibility_override;
  base::ScopedClosureRunner rendering_bootstrap;
  __weak id gesture_handler = nil;
  NSInteger gesture_mask = 0;

 private:
  friend class content::WebContentsUserData<ServicesState>;
  explicit ServicesState(content::WebContents* contents) : content::WebContentsUserData<ServicesState>(*contents) {}
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(ServicesState);

ServicesState* StateFor(content::WebContents* contents) {
  ServicesState::CreateForWebContents(contents);
  return ServicesState::FromWebContents(contents);
}

// Runs one DevTools screenshot (Page.captureScreenshot) against a tab and hands back the decoded image on the
// main thread. Self-owned: deletes itself after the completion ran.
class ScreenshotJob : public content::DevToolsAgentHostClient {
 public:
  using Completion = void (^)(NSImage*);

  static void Start(content::WebContents* contents, bool full_page, Completion completion) {
    auto* job = new ScreenshotJob(full_page, completion);
    if (!job->Begin(contents)) {
      job->Finish(nil);
    }
  }

  void DispatchProtocolMessage(content::DevToolsAgentHost*, base::span<const uint8_t> message) override {
    std::optional<base::Value> parsed = base::JSONReader::Read(
        std::string_view(reinterpret_cast<const char*>(message.data()), message.size()), base::JSON_PARSE_RFC);
    const base::DictValue* reply = parsed ? parsed->GetIfDict() : nullptr;
    const std::optional<int> id = reply ? reply->FindInt("id") : std::nullopt;
    if (!id) {
      return;  // An event; only replies to our commands matter.
    }
    const base::DictValue* result = reply->FindDict("result");
    if (!result) {
      Finish(nil);
      return;
    }
    if (*id == kLayoutMetricsId) {
      const base::DictValue* size = result->FindDict("cssContentSize");
      const std::optional<double> width = size ? size->FindDouble("width") : std::nullopt;
      const std::optional<double> height = size ? size->FindDouble("height") : std::nullopt;
      if (!width || !height || *width < 1 || *height < 1) {
        Finish(nil);
        return;
      }
      Send(kScreenshotId, "Page.captureScreenshot",
           base::DictValue()
               .Set("format", "png")
               .Set("captureBeyondViewport", true)
               .Set("clip", base::DictValue()
                                .Set("x", 0)
                                .Set("y", 0)
                                .Set("width", *width)
                                .Set("height", *height)
                                .Set("scale", 1)));
      return;
    }
    const std::string* data = result->FindString("data");
    std::string png;
    if (!data || !base::Base64Decode(*data, &png)) {
      Finish(nil);
      return;
    }
    Finish([[NSImage alloc] initWithData:[NSData dataWithBytes:png.data() length:png.size()]]);
  }

  void AgentHostClosed(content::DevToolsAgentHost*) override {
    host_ = nullptr;
    Finish(nil);
  }

  bool MayAttachToRenderFrameHost(content::RenderFrameHost*) override { return true; }
  bool MayAttachToURL(const GURL&, bool) override { return true; }

 private:
  static constexpr int kLayoutMetricsId = 1;
  static constexpr int kScreenshotId = 2;

  ScreenshotJob(bool full_page, Completion completion) : full_page_(full_page), completion_(completion) {}

  bool Begin(content::WebContents* contents) {
    host_ = content::DevToolsAgentHost::GetOrCreateFor(contents);
    if (!host_ || !host_->AttachClient(this)) {
      host_ = nullptr;
      return false;
    }
    if (full_page_) {
      Send(kLayoutMetricsId, "Page.getLayoutMetrics", base::DictValue());
    } else {
      Send(kScreenshotId, "Page.captureScreenshot", base::DictValue().Set("format", "png"));
    }
    return true;
  }

  void Send(int id, std::string_view method, base::DictValue params) {
    const std::optional<std::string> json = base::WriteJson(
        base::DictValue().Set("id", id).Set("method", method).Set("params", std::move(params)));
    if (json && host_) {
      host_->DispatchProtocolMessage(this, base::as_byte_span(*json));
    }
  }

  // Calls the completion once and deletes the job. The detach and delete are deferred out of the protocol
  // callback so the agent host never sees its client vanish mid-dispatch.
  void Finish(NSImage* image) {
    if (finished_) {
      return;
    }
    finished_ = true;
    Completion completion = completion_;
    completion_ = nil;
    if (completion) {
      completion(image);
    }
    scoped_refptr<content::DevToolsAgentHost> host = std::move(host_);
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(
                       [](ScreenshotJob* job, scoped_refptr<content::DevToolsAgentHost> host) {
                         if (host) {
                           host->DetachClient(job);
                         }
                         delete job;
                       },
                       this, std::move(host)));
  }

  const bool full_page_;
  Completion completion_;
  scoped_refptr<content::DevToolsAgentHost> host_;
  bool finished_ = false;
};

// The tab-sharing infobar on |contents|, if a capture session put one there.
TabSharingInfoBarDelegate* TabSharingDelegateFor(content::WebContents* contents) {
  infobars::ContentInfoBarManager* manager = infobars::ContentInfoBarManager::FromWebContents(contents);
  if (!manager) {
    return nullptr;
  }
  for (infobars::InfoBar* infobar : manager->infobars()) {
    infobars::InfoBarDelegate* delegate = infobar->delegate();
    if (delegate && delegate->GetIdentifier() == infobars::InfoBarDelegate::TAB_SHARING_INFOBAR_DELEGATE) {
      return static_cast<TabSharingInfoBarDelegate*>(delegate);
    }
  }
  return nullptr;
}

translate::TranslateManager* TranslateManagerFor(content::WebContents* contents) {
  ChromeTranslateClient* client = ChromeTranslateClient::FromWebContents(contents);
  return client ? client->GetTranslateManager() : nullptr;
}

}  // namespace

// ---- ArcWebContents (Services) -------------------------------------------------------------------

@implementation ArcWebContents (Services)

// MARK: Script

- (void)executeJavascript:(NSString*)script completion:(void (^)(NSString*, BOOL))completion {
  [self executeJavascript:script reason:nil completion:completion];
}

- (void)executeJavascript:(NSString*)script
                   reason:(NSString*)reason
               completion:(void (^)(NSString*, BOOL))completion {
  content::WebContents* contents = [self webContents];
  content::RenderFrameHost* frame = contents ? contents->GetPrimaryMainFrame() : nullptr;
  if (!frame || !frame->IsRenderFrameLive()) {
    // Unavailable path: Arc calls back inline with an empty string and zero.
    if (completion) {
      completion(@"", NO);
    }
    return;
  }
  frame->ExecuteJavaScriptInIsolatedWorld(
      ArcUTF16(script),
      base::BindOnce(
          [](void (^completion)(NSString*, BOOL), base::Value value) {
            if (!completion) {
              return;
            }
            const std::optional<std::string> json = base::WriteJson(value);
            completion(ArcNSString(json.value_or(std::string())), YES);
          },
          completion),
      kArcScriptWorldId);
}

// MARK: Capture

- (void)captureViewport:(void (^)(NSImage*))completion {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    if (completion) {
      completion(nil);
    }
    return;
  }
  ScreenshotJob::Start(contents, /*full_page=*/false, completion);
}

- (void)captureFullPage:(void (^)(NSImage*))completion {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    if (completion) {
      completion(nil);
    }
    return;
  }
  ScreenshotJob::Start(contents, /*full_page=*/true, completion);
}

// MARK: Dialogs

- (void)showCertificateDialog {
  content::WebContents* contents = [self webContents];
  content::NavigationEntry* entry = contents ? contents->GetController().GetVisibleEntry() : nullptr;
  gfx::NativeWindow window = contents ? contents->GetTopLevelNativeWindow() : gfx::NativeWindow();
  if (!entry || !entry->GetSSL().certificate || !window) {
    return;
  }
  ShowCertificateViewer(contents, window, entry->GetSSL().certificate.get());
}

- (void)showCookiesDialog {
  content::WebContents* contents = [self webContents];
  if (TabDialogs* dialogs = contents ? TabDialogs::FromWebContents(contents) : nullptr) {
    dialogs->ShowCollectedCookies();
  }
}

// MARK: Translate

- (void)translateToTargetLanguage:(NSString*)language {
  content::WebContents* contents = [self webContents];
  translate::TranslateManager* manager = contents ? TranslateManagerFor(contents) : nullptr;
  if (!manager || language.length == 0) {
    return;
  }
  const std::string& source = manager->GetLanguageState()->source_language();
  manager->TranslatePage(source.empty() ? std::string_view(language_detection::kUnknownLanguageCode)
                                        : std::string_view(source),
                         ArcUTF8(language), /*triggered_from_menu=*/false);
}

- (NSArray*)supportedTranslateLanguages {
  std::vector<std::string> languages;
  translate::TranslateDownloadManager::GetSupportedLanguages(/*translate_allowed=*/true, &languages);
  NSMutableArray* result = [NSMutableArray arrayWithCapacity:languages.size()];
  for (const std::string& language : languages) {
    [result addObject:ArcNSString(language)];
  }
  return result;
}

- (void)revertTranslation {
  content::WebContents* contents = [self webContents];
  if (translate::TranslateManager* manager = contents ? TranslateManagerFor(contents) : nullptr) {
    manager->RevertTranslation();
  }
}

// MARK: Casting and tab sharing

// Toggles the Cast dialog; YES when it was opened.
- (BOOL)cast {
  content::WebContents* contents = [self webContents];
  if (!contents || !media_router::MediaRouterEnabled(contents->GetBrowserContext())) {
    return NO;
  }
  media_router::MediaRouterDialogController* controller =
      media_router::MediaRouterDialogController::GetOrCreateForWebContents(contents);
  if (!controller) {
    return NO;
  }
  if (controller->IsShowingMediaRouterDialog()) {
    controller->HideMediaRouterDialog();
    return NO;
  }
  return controller->ShowMediaRouterDialog(media_router::MediaRouterDialogActivationLocation::APP_MENU);
}

// Moves the running tab capture to this tab ("Share this tab instead").
- (void)shareThisTab {
  content::WebContents* contents = [self webContents];
  if (TabSharingInfoBarDelegate* delegate = contents ? TabSharingDelegateFor(contents) : nullptr) {
    delegate->ShareThisTabInstead();
  }
}

- (void)stopSharingThisTab {
  content::WebContents* contents = [self webContents];
  if (TabSharingInfoBarDelegate* delegate = contents ? TabSharingDelegateFor(contents) : nullptr) {
    delegate->Stop();
  }
}

// MARK: Gestures, rendering and accessibility

// The handler is held weakly; nil clears it. Arc stores the 5-bit mask next to it. The app's handler protocol
// is not recoverable from the app binary, so no events are delivered from here.
- (void)setGestureHandler:(id)handler forGestures:(NSInteger)gestures {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    return;
  }
  ServicesState* state = StateFor(contents);
  state->gesture_handler = handler;
  state->gesture_mask = gestures & kGestureMask;
}

// Keeps a hidden tab rendering (as a capturer would) until -bootstrapRenderingComplete.
- (void)bootstrapRendering {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    return;
  }
  StateFor(contents)->rendering_bootstrap = base::ScopedClosureRunner(contents->IncrementCapturerCount(
      contents->GetSize(), /*stay_hidden=*/false, /*stay_awake=*/false, /*is_activity=*/true));
}

- (void)bootstrapRenderingComplete {
  content::WebContents* contents = [self webContents];
  if (ServicesState* state = contents ? ServicesState::FromWebContents(contents) : nullptr) {
    state->rendering_bootstrap.RunAndReset();
  }
}

- (void)setAccessibilityOverrideActive:(BOOL)active {
  content::WebContents* contents = [self webContents];
  if (!contents) {
    return;
  }
  ServicesState* state = StateFor(contents);
  if (!active) {
    state->accessibility_override.reset();
  } else if (!state->accessibility_override) {
    state->accessibility_override = content::BrowserAccessibilityState::GetInstance()->CreateScopedModeForWebContents(
        contents, kArcAccessibilityOverrideMode);
  }
}

// MARK: Freezing

// Arc has one vote kind; |type| (the app passes 1) only selects it. Nil when the performance manager graph or
// this tab's page node doesn't exist (the vote would CHECK).
- (ArcFreezingVoteToken*)emitFreezingVote:(NSInteger)type {
  content::WebContents* contents = [self webContents];
  if (!contents || !performance_manager::PerformanceManager::IsAvailable() ||
      !performance_manager::PerformanceManager::GetPrimaryPageNodeForWebContents(contents)) {
    return nil;
  }
  FreezingVotePtr vote = std::make_unique<performance_manager::freezing::FreezingVote>(contents);
  return [[ArcFreezingVoteToken alloc] initWithFreezingVoteToken:&vote];
}

@end
