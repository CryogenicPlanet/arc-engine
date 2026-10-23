# autofill-save golden (accepted by the lead, 2026-10-23)

Arc's 1.170 engine delivered `webContents:didSubmitAddressEntry:` (ArcAddressEntrySaveRequest) after the
address form and `webContents:didSubmitCreditCardInfo:` (ArcCreditCardInfoSaveRequest) after the card form,
and every page step passed. As in password-save, the final quit step fails on Arc's own engine (exit takes
longer than 15 s with save offers pending); that is reference behavior, so the transcript is accepted.
