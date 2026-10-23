// Copyright 2026 ArcMod. JavaScript dialogs are presented by Arc's app.
#import "arc/api/arc_internal.h"
#include "arc/browser/ui_hooks.h"

#include <utility>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "components/javascript_dialogs/core/dialog_util.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/javascript_dialog_manager.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/base/l10n/l10n_util.h"

@interface ArcWebContents (UIInternal)
- (id)uiDelegate;
- (BOOL)isBlockingJavascriptPopups;
@end

@protocol ArcHostJavaScriptUI <NSObject>
- (void)webContents:(ArcWebContents*)contents willRunJavaScriptAlertPanel:(NSString*)title
        withMessage:(NSString*)message completion:(void (^)(void))completion;
- (void)webContents:(ArcWebContents*)contents willRunJavaScriptConfirmPanel:(NSString*)title
        withMessage:(NSString*)message completion:(void (^)(BOOL))completion;
- (void)webContents:(ArcWebContents*)contents willRunJavaScriptTextInputPanel:(NSString*)title
        withMessage:(NSString*)message withDefaultText:(NSString*)text
         completion:(void (^)(NSString*))completion;
- (void)webContents:(ArcWebContents*)contents
    runJavaScriptApplicationModalBeforeUnloadPanel:(NSString*)title
        withMessage:(NSString*)message withPrimaryButtonLabel:(NSString*)label
         completion:(void (^)(BOOL))completion;
- (void)webContentsWillCloseJavaScriptPanel:(ArcWebContents*)contents;
- (void)webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:(ArcWebContents*)contents;
@end

namespace {
class HostJavaScriptDialogManager
    : public content::JavaScriptDialogManager,
      public content::WebContentsObserver,
      public content::WebContentsUserData<HostJavaScriptDialogManager> {
 public:
  ~HostJavaScriptDialogManager() override = default;

  void RunJavaScriptDialog(content::WebContents* contents, content::RenderFrameHost* frame,
      content::JavaScriptDialogType type, const std::u16string& message,
      const std::u16string& default_text, DialogClosedCallback callback,
      bool* suppressed) override {
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
    id<ArcHostJavaScriptUI> delegate = [owner respondsToSelector:@selector(uiDelegate)]
        ? [owner uiDelegate] : nil;
    SEL selector = type == content::JAVASCRIPT_DIALOG_TYPE_ALERT
        ? @selector(webContents:willRunJavaScriptAlertPanel:withMessage:completion:)
        : type == content::JAVASCRIPT_DIALOG_TYPE_CONFIRM
        ? @selector(webContents:willRunJavaScriptConfirmPanel:withMessage:completion:)
        : @selector(webContents:willRunJavaScriptTextInputPanel:withMessage:withDefaultText:completion:);
    *suppressed = callback_ || [owner isBlockingJavascriptPopups] ||
        ![delegate respondsToSelector:selector];
    if (*suppressed) { return; }
    callback_ = std::move(callback);
    before_unload_ = false;
    default_text_ = default_text;
    auto weak = weak_factory_.GetWeakPtr();
    const uint64_t generation = ++generation_;
    const auto* main_frame = contents->GetPrimaryMainFrame();
    NSString* title = ArcNSString(javascript_dialogs::util::DialogTitle(
        main_frame->GetLastCommittedURL(), main_frame->GetLastCommittedOrigin(),
        frame->GetLastCommittedOrigin()));
    NSString* body = ArcNSString(message);
    if (type == content::JAVASCRIPT_DIALOG_TYPE_ALERT) {
      [delegate webContents:owner willRunJavaScriptAlertPanel:title withMessage:body
          completion:^{ Complete(weak, generation, true, std::u16string()); }];
    } else if (type == content::JAVASCRIPT_DIALOG_TYPE_CONFIRM) {
      [delegate webContents:owner willRunJavaScriptConfirmPanel:title withMessage:body
          completion:^(BOOL accepted) { Complete(weak, generation, accepted, std::u16string()); }];
    } else {
      [delegate webContents:owner willRunJavaScriptTextInputPanel:title withMessage:body
          withDefaultText:ArcNSString(default_text) completion:^(NSString* text) {
            Complete(weak, generation, text != nil, ArcUTF16(text));
          }];
    }
  }

  void RunBeforeUnloadDialog(content::WebContents* contents, content::RenderFrameHost*,
      bool is_reload, DialogClosedCallback callback) override {
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
    id<ArcHostJavaScriptUI> delegate = [owner respondsToSelector:@selector(uiDelegate)]
        ? [owner uiDelegate] : nil;
    if (callback_ || ![delegate respondsToSelector:
        @selector(webContents:runJavaScriptApplicationModalBeforeUnloadPanel:withMessage:withPrimaryButtonLabel:completion:)]) {
      std::move(callback).Run(true, std::u16string());
      return;
    }
    callback_ = std::move(callback);
    before_unload_ = true;
    auto weak = weak_factory_.GetWeakPtr();
    const uint64_t generation = ++generation_;
    [delegate webContents:owner runJavaScriptApplicationModalBeforeUnloadPanel:
        ArcNSString(l10n_util::GetStringUTF16(is_reload
            ? IDS_BEFORERELOAD_MESSAGEBOX_TITLE : IDS_BEFOREUNLOAD_MESSAGEBOX_TITLE))
        withMessage:ArcNSString(l10n_util::GetStringUTF16(IDS_BEFOREUNLOAD_MESSAGEBOX_MESSAGE))
        withPrimaryButtonLabel:ArcNSString(l10n_util::GetStringUTF16(is_reload
            ? IDS_BEFORERELOAD_MESSAGEBOX_OK_BUTTON_LABEL : IDS_BEFOREUNLOAD_MESSAGEBOX_OK_BUTTON_LABEL))
        completion:^(BOOL accepted) { Complete(weak, generation, accepted, std::u16string()); }];
  }

  bool HandleJavaScriptDialog(content::WebContents*, bool accepted,
                              const std::u16string* override_text) override {
    if (!callback_) { return false; }
    const std::u16string text = override_text ? *override_text : default_text_;
    auto callback = std::move(callback_);
    ++generation_;
    default_text_.clear();
    ClosePanel();
    std::move(callback).Run(accepted, text);
    return true;
  }

  void CancelDialogs(content::WebContents*, bool) override {
    if (!callback_) { return; }
    auto callback = std::move(callback_);
    ++generation_;
    default_text_.clear();
    ClosePanel();
    std::move(callback).Run(false, std::u16string());
  }

  void WebContentsDestroyed() override {
    // The renderer-side callback is no longer valid. Late app completions are inert.
    weak_factory_.InvalidateWeakPtrs();
    callback_.Reset();
    Observe(nullptr);
  }

 private:
  explicit HostJavaScriptDialogManager(content::WebContents* contents)
      : content::WebContentsObserver(contents),
        content::WebContentsUserData<HostJavaScriptDialogManager>(*contents) {}
  friend content::WebContentsUserData<HostJavaScriptDialogManager>;

  static void Complete(base::WeakPtr<HostJavaScriptDialogManager> weak,
      uint64_t generation, bool accepted, std::u16string text) {
    if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce(&HostJavaScriptDialogManager::Complete, weak, generation, accepted, std::move(text)));
      return;
    }
    if (weak) { weak->Finish(generation, accepted, text); }
  }
  void Finish(uint64_t generation, bool accepted, const std::u16string& text) {
    if (!callback_ || generation != generation_) { return; }
    auto callback = std::move(callback_);
    default_text_.clear();
    std::move(callback).Run(accepted, text);
  }
  void ClosePanel() {
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:web_contents()];
    id<ArcHostJavaScriptUI> delegate = [owner respondsToSelector:@selector(uiDelegate)]
        ? [owner uiDelegate] : nil;
    SEL selector = before_unload_
        ? @selector(webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:)
        : @selector(webContentsWillCloseJavaScriptPanel:);
    if ([delegate respondsToSelector:selector]) {
      if (before_unload_) { [delegate webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:owner]; }
      else { [delegate webContentsWillCloseJavaScriptPanel:owner]; }
    }
  }
  DialogClosedCallback callback_;
  std::u16string default_text_;
  uint64_t generation_ = 0;
  bool before_unload_ = false;
  base::WeakPtrFactory<HostJavaScriptDialogManager> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(HostJavaScriptDialogManager);
}  // namespace

namespace arc {
content::JavaScriptDialogManager* GetJavaScriptDialogManager(content::WebContents* contents) {
  if (!contents) { return nullptr; }
  HostJavaScriptDialogManager::CreateForWebContents(contents);
  return HostJavaScriptDialogManager::FromWebContents(contents);
}
}  // namespace arc
