// Copyright 2026 ArcMod. Save offers Arc's app answers instead of Chrome's bubbles: passwords
// (ArcLoginCredentialsSaveRequest), addresses (ArcAddressEntrySaveRequest) and cards
// (ArcCreditCardInfoSaveRequest), delivered through ArcWebContentsUIDelegate's didSubmit… calls.

#ifndef ARC_API_ARC_SAVE_REQUESTS_H_
#define ARC_API_ARC_SAVE_REQUESTS_H_

#include <memory>

#include "base/memory/weak_ptr.h"

class BrowserWindowInterface;
namespace autofill {
class AutofillBubbleHandler;
}  // namespace autofill
namespace content {
class WebContents;
}  // namespace content

namespace arc {

// An address or card offer on Chrome's side. Gone once Chrome closes the offer (navigation, tab close) or
// after the app answered it.
class AutofillSaveOffer {
 public:
  virtual ~AutofillSaveOffer() = default;
  virtual void Accept() = 0;
  virtual void Decline() = 0;
};

// What the engine hands the request initializers (engine-internal pointer constructors, as in Arc's engine).
struct AutofillSaveOfferRef {
  base::WeakPtr<AutofillSaveOffer> offer;
};

// Chrome's autofill bubbles for an Arc window (BrowserWindow::GetAutofillBubbleHandler and the browser's
// unowned user data): address save/update and card save go to Arc's app; the rest have no Arc UI.
std::unique_ptr<autofill::AutofillBubbleHandler> CreateAutofillBubbleHandler(BrowserWindowInterface* browser);

// Tells the tab's UI delegate about an address (is_card false) or card offer.
void DeliverAutofillSaveOffer(content::WebContents* contents, base::WeakPtr<AutofillSaveOffer> offer, bool is_card);

}  // namespace arc

#endif  // ARC_API_ARC_SAVE_REQUESTS_H_
