// Copyright 2026 ArcMod. Chrome's autofill bubbles for an Arc window. Arc's app has UI for the address and
// card save offers (ArcWebContentsUIDelegate didSubmitAddressEntry:/didSubmitCreditCardInfo:); the other
// autofill bubbles have no Arc UI and stay closed.
#include "arc/api/arc_save_requests.h"

#include <memory>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/no_destructor.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/ui/autofill/autofill_bubble_base.h"
#include "chrome/browser/ui/autofill/autofill_bubble_handler.h"
#include "chrome/browser/ui/autofill/payments/save_card_bubble_controller.h"
#include "chrome/browser/ui/autofill/save_address_bubble_controller.h"
#include "chrome/browser/ui/autofill/update_address_bubble_controller.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "components/autofill/core/browser/foundations/autofill_client.h"
#include "components/autofill/core/browser/ui/payments/payments_ui_closed_reasons.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/unowned_user_data/scoped_unowned_user_data.h"

namespace arc {
namespace {

using autofill::AutofillBubbleBase;
using autofill::PaymentsUiClosedReason;
using Decision = autofill::AutofillClient::AddressPromptUserDecision;

// A bubble Arc has no UI for. Process lifetime, so Chrome's controllers may keep pointing at it.
class NoUIBubble : public AutofillBubbleBase {
 public:
  void Hide() override {}
  bool IsMouseHovered() const override { return false; }
};

AutofillBubbleBase* NoUI() {
  static base::NoDestructor<NoUIBubble> bubble;
  return bubble.get();
}

// Tells the app after Chrome has stored the returned bubble, so an immediate answer can't race it.
void PostDelivery(content::WebContents* contents, base::WeakPtr<AutofillSaveOffer> offer, bool is_card) {
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(
                     [](base::WeakPtr<content::WebContents> contents, base::WeakPtr<AutofillSaveOffer> offer,
                        bool is_card) {
                       if (contents && offer) DeliverAutofillSaveOffer(contents.get(), offer, is_card);
                     },
                     contents->GetWeakPtr(), std::move(offer), is_card));
}

const autofill::AutofillProfile& ProfileToSave(const autofill::SaveAddressBubbleController& controller) {
  return controller.GetAutofillProfile();
}
const autofill::AutofillProfile& ProfileToSave(const autofill::UpdateAddressBubbleController& controller) {
  return controller.GetProfileToSave();
}

// An address save or update offer. Like Chrome's views bubble it owns the controller and reports the
// decision, then OnBubbleClosed(); Chrome hides it on navigation, a newer offer or tab close.
template <class Controller>
class AddressOffer : public AutofillBubbleBase, public AutofillSaveOffer, public content::WebContentsObserver {
 public:
  AddressOffer(content::WebContents* contents, std::unique_ptr<Controller> controller)
      : content::WebContentsObserver(contents), controller_(std::move(controller)) {}

  base::WeakPtr<AutofillSaveOffer> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

  // AutofillSaveOffer: Arc's app answered (decision 2/3 as in Arc's engine).
  void Accept() override { Decide(Decision::kAccepted, ProfileToSave(*controller_)); }
  void Decline() override { Decide(Decision::kDeclined, std::nullopt); }

  // AutofillBubbleBase:
  void Hide() override { Close(); }
  bool IsMouseHovered() const override { return false; }

  // content::WebContentsObserver:
  void WebContentsDestroyed() override { Close(); }

 private:
  void Decide(Decision decision, base::optional_ref<const autofill::AutofillProfile> profile) {
    if (closed_) return;
    controller_->OnUserDecision(decision, profile);
    Close();
  }
  // The controller stays alive until this object is deleted: Chrome may hide the offer from inside
  // OnUserDecision().
  void Close() {
    if (closed_) return;
    closed_ = true;
    weak_factory_.InvalidateWeakPtrs();
    Observe(nullptr);
    controller_->OnBubbleClosed();
    base::SequencedTaskRunner::GetCurrentDefault()->DeleteSoon(FROM_HERE, this);
  }

  std::unique_ptr<Controller> controller_;
  bool closed_ = false;
  base::WeakPtrFactory<AutofillSaveOffer> weak_factory_{this};
};

// A local card save offer. Chrome's SaveCardBubbleControllerImpl belongs to the tab; save is OnSaveButton()
// then OnBubbleClosed(kAccepted), cancel OnBubbleClosed(kCancelled) (outcomes 1/2 in Arc's engine).
class CardOffer : public AutofillBubbleBase, public AutofillSaveOffer, public content::WebContentsObserver {
 public:
  CardOffer(content::WebContents* contents, autofill::SaveCardBubbleController* controller)
      : content::WebContentsObserver(contents), controller_(controller) {}

  base::WeakPtr<AutofillSaveOffer> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

  // AutofillSaveOffer:
  void Accept() override {
    if (closed_) return;
    controller_->OnSaveButton({});
    Close(PaymentsUiClosedReason::kAccepted);
  }
  void Decline() override { Close(PaymentsUiClosedReason::kCancelled); }

  // AutofillBubbleBase: hidden by Chrome without an answer, as a views bubble closed programmatically.
  void Hide() override { Close(PaymentsUiClosedReason::kNotInteracted); }
  bool IsMouseHovered() const override { return false; }

  // content::WebContentsObserver: the controller goes with the tab.
  void WebContentsDestroyed() override {
    controller_ = nullptr;
    Close(PaymentsUiClosedReason::kNotInteracted);
  }

 private:
  void Close(PaymentsUiClosedReason reason) {
    if (closed_) return;
    closed_ = true;
    weak_factory_.InvalidateWeakPtrs();
    Observe(nullptr);
    if (controller_) {
      std::exchange(controller_, nullptr)->OnBubbleClosed(reason);
    }
    base::SequencedTaskRunner::GetCurrentDefault()->DeleteSoon(FROM_HERE, this);
  }

  raw_ptr<autofill::SaveCardBubbleController> controller_;
  bool closed_ = false;
  base::WeakPtrFactory<AutofillSaveOffer> weak_factory_{this};
};

class ArcAutofillBubbleHandler : public autofill::AutofillBubbleHandler {
 public:
  explicit ArcAutofillBubbleHandler(BrowserWindowInterface* browser)
      : scoped_user_data_(browser->GetUnownedUserDataHost(), *this) {}
  ~ArcAutofillBubbleHandler() override = default;

  AutofillBubbleBase* ShowSaveCreditCardBubble(content::WebContents* web_contents,
                                               autofill::SaveCardBubbleController* controller,
                                               bool is_user_gesture) override {
    auto* offer = new CardOffer(web_contents, controller);
    PostDelivery(web_contents, offer->GetWeakPtr(), /*is_card=*/true);
    return offer;
  }
  AutofillBubbleBase* ShowSaveAddressProfileBubble(
      content::WebContents* web_contents,
      std::unique_ptr<autofill::SaveAddressBubbleController> controller,
      bool is_user_gesture) override {
    return ShowAddressOffer(web_contents, std::move(controller));
  }
  AutofillBubbleBase* ShowUpdateAddressProfileBubble(
      content::WebContents* web_contents,
      std::unique_ptr<autofill::UpdateAddressBubbleController> controller,
      bool is_user_gesture) override {
    return ShowAddressOffer(web_contents, std::move(controller));
  }

  AutofillBubbleBase* ShowIbanBubble(content::WebContents*, autofill::IbanBubbleController*, bool,
                                     autofill::IbanBubbleType) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowOfferNotificationBubble(content::WebContents*,
                                                  autofill::OfferNotificationBubbleController*,
                                                  bool) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowSaveAutofillAiDataBubble(content::WebContents*,
                                                   autofill::AutofillAiImportDataController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowAutofillAiLocalSaveNotification(
      content::WebContents*,
      autofill::AutofillAiImportDataController*) override {
    return NoUI();
  }
#if BUILDFLAG(ENABLE_DICE_SUPPORT)
  AutofillBubbleBase* ShowAddressSignInPromo(content::WebContents*, const autofill::AutofillProfile&) override {
    return NoUI();
  }
#endif
  AutofillBubbleBase* ShowFilledCardInformationBubble(content::WebContents*,
                                                      autofill::FilledCardInformationBubbleController*,
                                                      bool) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowVirtualCardEnrollBubble(content::WebContents*,
                                                  autofill::VirtualCardEnrollBubbleController*,
                                                  bool) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowVirtualCardEnrollConfirmationBubble(
      content::WebContents*,
      autofill::VirtualCardEnrollBubbleController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowMandatoryReauthBubble(content::WebContents*,
                                                autofill::MandatoryReauthBubbleController*,
                                                bool,
                                                autofill::MandatoryReauthBubbleType) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowSaveCardConfirmationBubble(content::WebContents*,
                                                     autofill::SaveCardBubbleController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowSaveIbanConfirmationBubble(content::WebContents*,
                                                     autofill::IbanBubbleController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowOmniboxAutofillBubble(content::WebContents*,
                                                autofill::OmniboxAutofillBubbleController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowPaymentsChurnedUsersBubble(content::WebContents*,
                                                     autofill::PaymentsChurnedUsersBubbleController*,
                                                     bool) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowPaymentsChurnedUsersConfirmationBubble(
      content::WebContents*,
      autofill::PaymentsChurnedUsersBubbleController*) override {
    return NoUI();
  }
  AutofillBubbleBase* ShowWalletReminderNoticeBubble(content::WebContents*,
                                                     autofill::WalletReminderNoticeBubbleController*,
                                                     bool) override {
    return NoUI();
  }

 private:
  template <class Controller>
  AutofillBubbleBase* ShowAddressOffer(content::WebContents* web_contents, std::unique_ptr<Controller> controller) {
    auto* offer = new AddressOffer<Controller>(web_contents, std::move(controller));
    PostDelivery(web_contents, offer->GetWeakPtr(), /*is_card=*/false);
    return offer;
  }

  ui::ScopedUnownedUserData<autofill::AutofillBubbleHandler> scoped_user_data_;
};

}  // namespace

std::unique_ptr<autofill::AutofillBubbleHandler> CreateAutofillBubbleHandler(BrowserWindowInterface* browser) {
  return std::make_unique<ArcAutofillBubbleHandler>(browser);
}

}  // namespace arc
