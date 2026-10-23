// Copyright 2026 ArcMod. Keep Chrome's native HTTP auth/password-manager flow.
#import "arc/browser/ui_values.h"
#include "arc/browser/ui_hooks.h"

#include "base/functional/bind.h"
#include "base/memory/raw_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "components/password_manager/core/browser/http_auth_observer.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"

@interface ArcWebContents (UIInternal)
- (id)uiDelegate;
@end
@protocol ArcHostAuthUI <NSObject>
- (void)webContents:(ArcWebContents*)contents showWebLoginDialog:(ArcWebLoginDialog*)dialog;
- (void)webContentsDidCloseWebLoginDialog:(ArcWebContents*)contents;
@end

namespace {
class HostLoginHandler : public LoginHandler,
                         public password_manager::HttpAuthObserver {
 public:
  HostLoginHandler(const net::AuthChallengeInfo& info, content::WebContents* contents,
      content::LoginDelegate::LoginAuthRequiredCallback callback)
      : LoginHandler(info, contents, std::move(callback)), contents_(contents->GetWeakPtr()) {}
  ~HostLoginHandler() override {
    weak_factory_.InvalidateWeakPtrs();
    CloseDialog();
    if (http_auth_manager_) { http_auth_manager_->DetachObserver(this); }
  }

  bool BuildViewImpl(const std::u16string& authority, const std::u16string& explanation,
                     LoginModelData* model) override {
    if (!contents_) { return false; }
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_.get()];
    id<ArcHostAuthUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
    if (![ui respondsToSelector:@selector(webContents:showWebLoginDialog:)]) { return false; }
    auto weak = weak_factory_.GetWeakPtr();
    arc::UIWebLoginDialogData data;
    data.is_proxy_login = auth_info().is_proxy;
    // LoginHandler::GetDialogStrings derives this explanation from the
    // authority URL using network::IsUrlPotentiallyTrustworthy.
    data.is_url_potentially_trustworthy = explanation.empty();
    data.authority = authority;
    data.accepted = [weak](const std::string& username, const std::string& password) {
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce([](base::WeakPtr<HostLoginHandler> handler, std::string user, std::string pass) {
            if (handler) { handler->SetAuth(base::UTF8ToUTF16(user), base::UTF8ToUTF16(pass)); }
          }, weak, username, password));
    };
    data.cancelled = [weak]() {
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce([](base::WeakPtr<HostLoginHandler> handler) {
            if (handler) { handler->CancelAuth(true); }
          }, weak));
    };
    data.closed = [weak]() {
      content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
          base::BindOnce([](base::WeakPtr<HostLoginHandler> handler) {
            if (handler) { handler->CancelAuth(false); }
          }, weak));
    };
    dialog_ = [[ArcWebLoginDialog alloc] initWithWebLoginDialog:&data];
    if (model) {
      http_auth_manager_ = model->model;
      http_auth_manager_->SetObserverAndDeliverCredentials(this, *model->form);
    }
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE,
        base::BindOnce(&HostLoginHandler::Show, weak));
    return true;
  }

  void CloseDialog() override {
    if (!dialog_) { return; }
    dialog_ = nil;
    if (!shown_ || !contents_) { return; }
    shown_ = false;
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_.get()];
    id<ArcHostAuthUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
    if ([ui respondsToSelector:@selector(webContentsDidCloseWebLoginDialog:)]) {
      [ui webContentsDidCloseWebLoginDialog:owner];
    }
  }
  void OnAutofillDataAvailable(std::u16string_view, std::u16string_view) override {
    // The app's credential value adapter is supplied by the profile package;
    // do not auto-submit credentials or fabricate a credentials object here.
  }
  void OnLoginModelDestroying() override { http_auth_manager_ = nullptr; }

 private:
  void Show() {
    if (!contents_ || !dialog_) { return; }
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_.get()];
    id<ArcHostAuthUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
    if (![ui respondsToSelector:@selector(webContents:showWebLoginDialog:)]) {
      CancelAuth(false);
      return;
    }
    shown_ = true;
    [ui webContents:owner showWebLoginDialog:dialog_];
  }
  base::WeakPtr<content::WebContents> contents_;
  raw_ptr<password_manager::HttpAuthManager> http_auth_manager_ = nullptr;
  ArcWebLoginDialog* __strong dialog_;
  bool shown_ = false;
  base::WeakPtrFactory<HostLoginHandler> weak_factory_{this};
};
}  // namespace

namespace arc {
std::unique_ptr<LoginHandler> CreateLoginHandler(const net::AuthChallengeInfo& info,
    content::WebContents* contents, content::LoginDelegate::LoginAuthRequiredCallback callback) {
  return std::make_unique<HostLoginHandler>(info, contents, std::move(callback));
}
}  // namespace arc
