// Copyright 2026 ArcMod. Password, address and card save offers answered by Arc's app.
#import "arc/api/arc_internal.h"
#include "arc/api/arc_save_requests.h"

#include "base/functional/bind.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"
#include "chrome/browser/ui/passwords/passwords_model_delegate.h"
#include "components/password_manager/core/browser/password_form.h"
#include "components/password_manager/core/common/password_manager_ui.h"
#include "base/task/sequenced_task_runner.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"

@interface ArcWebContents (SaveRequestsInternal)
- (id)uiDelegate;
@end

@class ArcAddressEntrySaveRequest, ArcCreditCardInfoSaveRequest, ArcLoginCredentialsSaveRequest;

// The save-offer part of the app's ArcWebContentsUIDelegate.
@protocol ArcSaveOfferUI <NSObject>
@optional
- (void)webContents:(ArcWebContents*)contents didSubmitCredentials:(ArcLoginCredentialsSaveRequest*)request;
- (void)webContents:(ArcWebContents*)contents didSubmitAddressEntry:(ArcAddressEntrySaveRequest*)request;
- (void)webContents:(ArcWebContents*)contents didSubmitCreditCardInfo:(ArcCreditCardInfoSaveRequest*)request;
@end

namespace {

struct LoginCredentialsSaveOffer {
  base::WeakPtr<PasswordsModelDelegate> delegate;
  bool is_update = false;
};

// The tab's password controller, while its save or update offer is still pending.
PasswordsModelDelegate* PendingPasswordDelegate(const base::WeakPtr<PasswordsModelDelegate>& delegate) {
  if (!delegate) return nullptr;
  const password_manager::ui::State state = delegate->GetState();
  return state == password_manager::ui::PENDING_PASSWORD_STATE ||
                 state == password_manager::ui::PENDING_PASSWORD_UPDATE_STATE
             ? delegate.get()
             : nullptr;
}

}  // namespace

ARC_EXPORT @interface ArcLoginCredentialsSaveRequest : NSObject
- (instancetype)initWithLoginCredentialsSaveRequest:(const void*)request;
- (BOOL)isUpdate;
- (void)save;
- (void)cancel;
- (void)neverAskAgain;
@end

@implementation ArcLoginCredentialsSaveRequest {
  base::WeakPtr<PasswordsModelDelegate> _delegate;
  BOOL _isUpdate;
}
- (instancetype)initWithLoginCredentialsSaveRequest:(const void*)request {
  if ((self = [super init])) {
    const auto* offer = static_cast<const LoginCredentialsSaveOffer*>(request);
    _delegate = offer->delegate;
    _isUpdate = offer->is_update;
  }
  return self;
}
- (BOOL)isUpdate {
  return _isUpdate;
}
// As Chrome's save bubble: the credentials Chrome proposed, unedited.
- (void)save {
  if (PasswordsModelDelegate* delegate = PendingPasswordDelegate(_delegate)) {
    const password_manager::PasswordForm& form = delegate->GetPendingPassword();
    delegate->SavePassword(form.username_value, form.password_value);
  }
}
// Arc's engine only reports a declined update to Chrome; a declined save just leaves the offer.
- (void)cancel {
  if (!_isUpdate) return;
  if (PasswordsModelDelegate* delegate = PendingPasswordDelegate(_delegate)) {
    delegate->OnNopeUpdateClicked();
  }
}
- (void)neverAskAgain {
  if (PasswordsModelDelegate* delegate = PendingPasswordDelegate(_delegate)) {
    delegate->NeverSavePassword();
  }
}
@end

ARC_EXPORT @interface ArcAddressEntrySaveRequest : NSObject
- (instancetype)initWithAddressEntrySaveRequest:(const void*)request;
- (void)save;
- (void)cancel;
@end

@implementation ArcAddressEntrySaveRequest {
  base::WeakPtr<arc::AutofillSaveOffer> _offer;
}
- (instancetype)initWithAddressEntrySaveRequest:(const void*)request {
  if ((self = [super init])) {
    _offer = static_cast<const arc::AutofillSaveOfferRef*>(request)->offer;
  }
  return self;
}
- (void)save {
  if (_offer) _offer->Accept();
}
- (void)cancel {
  if (_offer) _offer->Decline();
}
@end

ARC_EXPORT @interface ArcCreditCardInfoSaveRequest : NSObject
- (instancetype)initWithCreditCardInfoSaveRequest:(const void*)request;
- (void)save;
- (void)cancel;
@end

@implementation ArcCreditCardInfoSaveRequest {
  base::WeakPtr<arc::AutofillSaveOffer> _offer;
}
- (instancetype)initWithCreditCardInfoSaveRequest:(const void*)request {
  if ((self = [super init])) {
    _offer = static_cast<const arc::AutofillSaveOfferRef*>(request)->offer;
  }
  return self;
}
- (void)save {
  if (_offer) _offer->Accept();
}
- (void)cancel {
  if (_offer) _offer->Decline();
}
@end

namespace arc {

void DeliverAutofillSaveOffer(content::WebContents* contents, base::WeakPtr<AutofillSaveOffer> offer, bool is_card) {
  ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
  id<ArcSaveOfferUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
  const AutofillSaveOfferRef ref{offer};
  if (is_card) {
    if (![ui respondsToSelector:@selector(webContents:didSubmitCreditCardInfo:)]) return;
    [ui webContents:owner didSubmitCreditCardInfo:[[ArcCreditCardInfoSaveRequest alloc] initWithCreditCardInfoSaveRequest:&ref]];
  } else {
    if (![ui respondsToSelector:@selector(webContents:didSubmitAddressEntry:)]) return;
    [ui webContents:owner didSubmitAddressEntry:[[ArcAddressEntrySaveRequest alloc] initWithAddressEntrySaveRequest:&ref]];
  }
}

}  // namespace arc

namespace {

// The tab's pending address offer: Chrome's answer callback, until the app answers or a newer offer or the
// tab's end replaces it.
class PendingAddressOffer : public arc::AutofillSaveOffer,
                            public content::WebContentsUserData<PendingAddressOffer> {
 public:
  ~PendingAddressOffer() override = default;

  static base::WeakPtr<arc::AutofillSaveOffer> Replace(content::WebContents* contents,
                                                       base::OnceCallback<void(bool)> answer) {
    contents->RemoveUserData(UserDataKey());
    CreateForWebContents(contents, std::move(answer));
    return FromWebContents(contents)->weak_factory_.GetWeakPtr();
  }

  void Accept() override { Answer(true); }
  void Decline() override { Answer(false); }

 private:
  friend class content::WebContentsUserData<PendingAddressOffer>;
  PendingAddressOffer(content::WebContents* contents, base::OnceCallback<void(bool)> answer)
      : content::WebContentsUserData<PendingAddressOffer>(*contents), answer_(std::move(answer)) {}

  void Answer(bool accepted) {
    weak_factory_.InvalidateWeakPtrs();
    if (answer_) std::move(answer_).Run(accepted);
  }

  base::OnceCallback<void(bool)> answer_;
  base::WeakPtrFactory<arc::AutofillSaveOffer> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(PendingAddressOffer);

void OfferAddressSave(content::WebContents* contents, base::OnceCallback<void(bool)> answer) {
  // Delivered from a task, like the bubble path, so an immediate answer can't reenter Chrome's caller.
  base::WeakPtr<arc::AutofillSaveOffer> offer = PendingAddressOffer::Replace(contents, std::move(answer));
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(
                     [](base::WeakPtr<content::WebContents> contents, base::WeakPtr<arc::AutofillSaveOffer> offer) {
                       if (contents && offer) arc::DeliverAutofillSaveOffer(contents.get(), offer, /*is_card=*/false);
                     },
                     contents->GetWeakPtr(), offer));
}

void OfferPasswordSave(content::WebContents* contents, bool is_update) {
  ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
  id<ArcSaveOfferUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
  if (![ui respondsToSelector:@selector(webContents:didSubmitCredentials:)]) return;
  const LoginCredentialsSaveOffer offer{PasswordsModelDelegateFromWebContents(contents), is_update};
  [ui webContents:owner
      didSubmitCredentials:[[ArcLoginCredentialsSaveRequest alloc] initWithLoginCredentialsSaveRequest:&offer]];
}

}  // namespace

__attribute__((constructor)) static void InstallArcSaveOfferHooks() {
  arcmod::GetPasswordSaveOfferHook() = base::BindRepeating(&OfferPasswordSave);
  arcmod::GetAddressSaveOfferHook() = base::BindRepeating(&OfferAddressSave);
}
