// Copyright 2026 ArcMod. Hosts Chrome's tab-modal views dialogs (WebAuthn, payments, file access, ...)
// over Arc's windows. Chrome asks the browser window for a WebContentsModalDialogHost and CHECKs it; Arc's
// engine parents the dialog to the tab's view and tells the app about the dialog's window
// (ArcWebContentsUIDelegate webContents:showsAuxiliaryWindow:, webauthn-dialog golden).
#include "arc/browser/arc_modal_dialog_host.h"

#import <Cocoa/Cocoa.h>

#include "arc/api/arc_internal.h"
#include "base/functional/bind.h"
#include "base/observer_list.h"
#include "base/task/sequenced_task_runner.h"
#include "components/constrained_window/constrained_window_views.h"
#include "components/web_modal/modal_dialog_host.h"
#include "components/web_modal/web_contents_modal_dialog_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/views/widget/native_widget.h"
#include "ui/views/widget/widget.h"

@interface ArcWebContents (ModalDialogHost)
- (id)uiDelegate;
@end

@protocol ArcAuxiliaryWindowUI <NSObject>
@optional
- (void)webContents:(ArcWebContents*)contents showsAuxiliaryWindow:(NSWindow*)window;
@end

namespace arc {
namespace {

class TabModalDialogHost : public web_modal::WebContentsModalDialogHost,
                           public content::WebContentsUserData<TabModalDialogHost> {
 public:
  ~TabModalDialogHost() override {
    for (auto& observer : observers_) {
      observer.OnHostDestroying();
    }
  }

  // web_modal::ModalDialogHost:
  gfx::NativeView GetHostView() const override { return contents_->GetNativeView(); }
  // Top-centered over the tab, like Chrome's tab-modal sheets.
  gfx::Point GetDialogPosition(const gfx::Size& size) override {
    const gfx::Size host = GetMaximumDialogSize();
    return gfx::Point(std::max(0, (host.width() - size.width()) / 2), 0);
  }
  void AddObserver(web_modal::ModalDialogHostObserver* observer) override {
    observers_.AddObserver(observer);
    // Chrome registers a dialog's observer right after creating its widget. Without a views parent the
    // widget is a free-standing window, so find it among the app's windows now and report it once shown.
    for (NSWindow* window in [NSApp windows]) {
      if (IsConstrainedDialog(window) && ![SeenDialogs() containsObject:window]) {
        [SeenDialogs() addObject:window];
        [pending_ addObject:window];
      }
    }
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(&TabModalDialogHost::ReportNewDialogs, weak_factory_.GetWeakPtr()));
  }
  void RemoveObserver(web_modal::ModalDialogHostObserver* observer) override {
    observers_.RemoveObserver(observer);
  }

  // web_modal::WebContentsModalDialogHost:
  gfx::Size GetMaximumDialogSize() override { return contents_->GetContainerBounds().size(); }

 private:
  friend class content::WebContentsUserData<TabModalDialogHost>;
  explicit TabModalDialogHost(content::WebContents* contents)
      : content::WebContentsUserData<TabModalDialogHost>(*contents),
        contents_(contents),
        pending_([NSHashTable weakObjectsHashTable]) {}

  static NSHashTable<NSWindow*>* SeenDialogs() {
    static NSHashTable<NSWindow*>* seen = [NSHashTable weakObjectsHashTable];
    return seen;
  }

  static bool IsConstrainedDialog(NSWindow* window) {
    views::Widget* widget = views::Widget::GetWidgetForNativeWindow(gfx::NativeWindow(window));
    return widget && widget->GetNativeWindowProperty(views::kWidgetIdentifierKey) ==
                         constrained_window::kConstrainedWindowWidgetIdentifier;
  }

  void ReportNewDialogs() {
    NSArray<NSWindow*>* windows = [pending_ allObjects];
    [pending_ removeAllObjects];
    ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents_];
    id<ArcAuxiliaryWindowUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
    if (![ui respondsToSelector:@selector(webContents:showsAuxiliaryWindow:)]) {
      return;
    }
    for (NSWindow* window in windows) {
      [ui webContents:owner showsAuxiliaryWindow:window];
    }
  }

  const raw_ptr<content::WebContents> contents_;
  base::ObserverList<web_modal::ModalDialogHostObserver> observers_;
  // Dialogs found for this tab, reported after the current task.
  NSHashTable<NSWindow*>* __strong pending_;
  base::WeakPtrFactory<TabModalDialogHost> weak_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(TabModalDialogHost);

}  // namespace

web_modal::WebContentsModalDialogHost* ModalDialogHostForWebContents(content::WebContents* contents) {
  if (!contents) {
    return nullptr;
  }
  TabModalDialogHost::CreateForWebContents(contents);
  return TabModalDialogHost::FromWebContents(contents);
}

}  // namespace arc
