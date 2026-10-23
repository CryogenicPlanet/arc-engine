// Copyright 2026 ArcMod.
#ifndef ARC_BROWSER_ARC_MODAL_DIALOG_HOST_H_
#define ARC_BROWSER_ARC_MODAL_DIALOG_HOST_H_

namespace content {
class WebContents;
}  // namespace content
namespace web_modal {
class WebContentsModalDialogHost;
}  // namespace web_modal

namespace arc {

// The tab's host for Chrome's tab-modal views dialogs, parented to the tab's view in Arc's window (one per
// tab, lives with it). Shown dialogs are reported to the app's UI delegate (showsAuxiliaryWindow:).
web_modal::WebContentsModalDialogHost* ModalDialogHostForWebContents(content::WebContents* contents);

}  // namespace arc

#endif  // ARC_BROWSER_ARC_MODAL_DIALOG_HOST_H_
