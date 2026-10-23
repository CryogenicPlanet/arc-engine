// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "arc/browser/arc_browser_window.h"

#import <AppKit/AppKit.h>

#include <utility>

#include "base/feature_list.h"
#include "base/values.h"
#include "build/build_config.h"
#include "chrome/browser/ui/browser_init_state.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/create_browser_window.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/find_bar/find_bar.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/user_education/browser_user_education_interface.h"
#include "chrome/browser/ui/views/bubble_anchor_util_views.h"
#include "components/sharing_message/sharing_dialog_data.h"
#include "components/user_education/common/new_badge/new_badge_controller.h"
#include "content/public/browser/keyboard_event_processing_result.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/mojom/window_show_state.mojom.h"
#include "ui/color/color_provider_key.h"
#include "ui/color/color_provider_manager.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/mac/coordinate_conversion.h"
#include "base/functional/bind.h"

namespace arc {

// ArcBrowserWindow::ArcLocationBar -----------------------------------------

OmniboxView* ArcBrowserWindow::ArcLocationBar::GetOmniboxView() {
  return nullptr;
}

OmniboxPopupView* ArcBrowserWindow::ArcLocationBar::GetOmniboxPopupView() {
  return nullptr;
}

OmniboxController* ArcBrowserWindow::ArcLocationBar::GetOmniboxController() {
  return nullptr;
}

bool ArcBrowserWindow::ArcLocationBar::ShouldCloseOmniboxPopup(
    ui::MouseEvent* event) {
  return false;
}

ChipController* ArcBrowserWindow::ArcLocationBar::GetChipController() {
  return nullptr;
}

LocationBarTesting*
ArcBrowserWindow::ArcLocationBar::GetLocationBarForTesting() {
  return nullptr;
}

LocationBarModel* ArcBrowserWindow::ArcLocationBar::GetLocationBarModel() {
  return nullptr;
}

content::WebContents* ArcBrowserWindow::ArcLocationBar::GetWebContents() {
  return nullptr;
}

std::optional<bubble_anchor_util::AnchorConfiguration>
ArcBrowserWindow::ArcLocationBar::GetChipAnchor() {
  return {};
}

ui::TrackedElement* ArcBrowserWindow::ArcLocationBar::GetAnchorOrNull() {
  return nullptr;
}

BrowserWindowInterface* ArcBrowserWindow::ArcLocationBar::GetBrowser() {
  return nullptr;
}

Profile* ArcBrowserWindow::ArcLocationBar::GetProfile() {
  return nullptr;
}

bool ArcBrowserWindow::ArcLocationBar::IsInitialized() const {
  return true;
}

bool ArcBrowserWindow::ArcLocationBar::IsVisible() const {
  return true;
}

bool ArcBrowserWindow::ArcLocationBar::IsDrawn() const {
  return true;
}

bool ArcBrowserWindow::ArcLocationBar::IsFullscreen() const {
  return false;
}

bool ArcBrowserWindow::ArcLocationBar::IsEditingOrEmpty() const {
  return false;
}

bool ArcBrowserWindow::ArcLocationBar::IsMouseHovered() const {
  return false;
}

bool ArcBrowserWindow::ArcLocationBar::IsFocusWithin() const {
  return false;
}

gfx::Rect ArcBrowserWindow::ArcLocationBar::Bounds() const {
  return gfx::Rect();
}

gfx::Rect ArcBrowserWindow::ArcLocationBar::BoundsInScreen() const {
  return gfx::Rect();
}

gfx::Size ArcBrowserWindow::ArcLocationBar::MinimumSize() const {
  return gfx::Size();
}

gfx::Size ArcBrowserWindow::ArcLocationBar::PreferredSize() const {
  return gfx::Size();
}

bool ArcBrowserWindow::ArcLocationBar::HasSecurityStateChanged() {
  return false;
}

// ArcBrowserWindow ----------------------------------------------------------

ArcBrowserWindow::ArcBrowserWindow() {
  // The adapter is supplied before Browser construction; bind it once Chrome publishes the browser.
  browser_collection_observation_.Observe(
      GlobalBrowserCollection::GetInstance());
}

ArcBrowserWindow::~ArcBrowserWindow() {
  if (browser_) {
    // BrowserWindow implementations are expected to call
    // TearDownPreBrowserWindowDestruction() before destruction.
    browser_->GetFeatures().TearDownPreBrowserWindowDestruction();
    browser_ = nullptr;
  }
}

void ArcBrowserWindow::Close() {
  if (!browser_ || is_closed_) {
    return;
  }
  UnloadController::From(browser_.get())->OnWindowClosing();
}

bool ArcBrowserWindow::IsActive() const {
  return [native_window_.GetNativeNSWindow() isKeyWindow];
}

ui::ZOrderLevel ArcBrowserWindow::GetZOrderLevel() const {
  return ui::ZOrderLevel::kNormal;
}

gfx::NativeWindow ArcBrowserWindow::GetNativeWindow() const {
  return native_window_;
}

bool ArcBrowserWindow::IsOnCurrentWorkspace() const {
  return true;
}

bool ArcBrowserWindow::IsVisibleOnScreen() const {
  return IsVisible();
}

void ArcBrowserWindow::SetTopControlsShownRatio(
    content::WebContents* web_contents,
    float ratio) {}

bool ArcBrowserWindow::DoBrowserControlsShrinkRendererSize(
    const content::WebContents* contents) const {
  return false;
}

ui::NativeTheme* ArcBrowserWindow::GetNativeTheme() {
  return nullptr;
}

const ui::ThemeProvider* ArcBrowserWindow::GetThemeProvider() const {
  return nullptr;
}

const ui::ColorProvider* ArcBrowserWindow::GetColorProvider() const {
  return ui::ColorProviderManager::Get().GetColorProviderFor(
      ui::ColorProviderKey());
}

int ArcBrowserWindow::GetTopControlsHeight() const {
  return 0;
}

void ArcBrowserWindow::SetTopControlsGestureScrollInProgress(
    bool in_progress) {}

std::vector<StatusBubble*> ArcBrowserWindow::GetStatusBubbles() {
  return {};
}

gfx::Rect ArcBrowserWindow::GetRestoredBounds() const {
  return GetBounds();
}

ui::mojom::WindowShowState ArcBrowserWindow::GetRestoredState() const {
  return ui::mojom::WindowShowState::kDefault;
}

gfx::Rect ArcBrowserWindow::GetBounds() const {
  NSWindow* window = native_window_.GetNativeNSWindow();
  return window ? gfx::ScreenRectFromNSRect(window.frame) : gfx::Rect();
}

gfx::Size ArcBrowserWindow::GetContentsSize() const {
  NSSize size = [native_window_.GetNativeNSWindow().contentView frame].size;
  return gfx::Size(size.width, size.height);
}

void ArcBrowserWindow::SetContentsSize(const gfx::Size& size) {
  [native_window_.GetNativeNSWindow() setContentSize:NSMakeSize(size.width(), size.height())];
}

bool ArcBrowserWindow::IsMaximized() const {
  return [native_window_.GetNativeNSWindow() isZoomed];
}

bool ArcBrowserWindow::IsMinimized() const {
  return [native_window_.GetNativeNSWindow() isMiniaturized];
}

bool ArcBrowserWindow::GetCanResize() {
  return (native_window_.GetNativeNSWindow().styleMask & NSWindowStyleMaskResizable) != 0;
}

ui::mojom::WindowShowState ArcBrowserWindow::GetWindowShowState() const {
  return ui::mojom::WindowShowState::kDefault;
}

bool ArcBrowserWindow::IsFullscreen() const {
  return (native_window_.GetNativeNSWindow().styleMask & NSWindowStyleMaskFullScreen) != 0;
}

bool ArcBrowserWindow::UpdateToolbarSecurityState() {
  return false;
}

bool ArcBrowserWindow::IsVisible() const {
  return [native_window_.GetNativeNSWindow() isVisible];
}

LocationBar* ArcBrowserWindow::GetLocationBar() const {
  return const_cast<ArcLocationBar*>(&location_bar_);
}

ui::AcceleratorProvider* ArcBrowserWindow::GetAcceleratorProvider() {
  return nullptr;
}

PinnedToolbarActions* ArcBrowserWindow::GetPinnedToolbarActions() {
  return nullptr;
}

autofill::AutofillBubbleHandler* ArcBrowserWindow::GetAutofillBubbleHandler() {
  return nullptr;
}

content::KeyboardEventProcessingResult
ArcBrowserWindow::PreHandleKeyboardEvent(
    const input::NativeWebKeyboardEvent& event) {
  return content::KeyboardEventProcessingResult::NOT_HANDLED;
}

bool ArcBrowserWindow::HandleKeyboardEvent(
    const input::NativeWebKeyboardEvent& event) {
  return false;
}

bool ArcBrowserWindow::IsTabStripEditable() const {
  return is_tab_strip_editable_;
}

void ArcBrowserWindow::DisableTabStripEditingForTesting() {
  is_tab_strip_editable_ = false;
}

bool ArcBrowserWindow::IsToolbarVisible() const {
  return false;
}

bool ArcBrowserWindow::IsToolbarShowing() const {
  return false;
}

bool ArcBrowserWindow::IsLocationBarVisible() const {
  return false;
}

bool ArcBrowserWindow::IsUnframedModeEnabled() const {
  return false;
}

BrowserView* ArcBrowserWindow::AsBrowserView() {
  return nullptr;
}

void ArcBrowserWindow::DeleteBrowserWindow() {
  delete this;
}

ShowTranslateBubbleResult ArcBrowserWindow::ShowTranslateBubble(
    content::WebContents* contents,
    translate::TranslateStep step,
    const std::string& source_language,
    const std::string& target_language,
    translate::TranslateErrors error_type,
    bool is_user_gesture) {
  return ShowTranslateBubbleResult::kSuccess;
}

#if BUILDFLAG(IS_CHROMEOS)
void ArcBrowserWindow::ToggleMultitaskMenu() {
  return;
}
#endif  // BUILDFLAG(IS_CHROMEOS)

DownloadBubbleUIController* ArcBrowserWindow::GetDownloadBubbleUIController() {
  return nullptr;
}

std::unique_ptr<FindBar> ArcBrowserWindow::CreateFindBar() {
  return nullptr;
}

web_modal::WebContentsModalDialogHost*
ArcBrowserWindow::GetWebContentsModalDialogHost() {
  return nullptr;
}

web_modal::WebContentsModalDialogHost*
ArcBrowserWindow::GetWebContentsModalDialogHostFor(
    content::WebContents* web_contents) {
  return nullptr;
}

ExclusiveAccessContext* ArcBrowserWindow::GetExclusiveAccessContext() {
  return this;
}

std::string ArcBrowserWindow::GetWorkspace() const {
  return workspace_;
}

bool ArcBrowserWindow::IsVisibleOnAllWorkspaces() const {
  return visible_on_all_workspaces_;
}

std::unique_ptr<content::EyeDropper> ArcBrowserWindow::OpenEyeDropper(
    content::RenderFrameHost* frame,
    content::EyeDropperListener* listener) {
  return nullptr;
}

void ArcBrowserWindow::SetNativeWindow(gfx::NativeWindow window) {
  native_window_ = window;
}

void ArcBrowserWindow::SetCloseCallback(base::OnceClosure close_callback) {
  close_callback_ = std::move(close_callback);
}

void ArcBrowserWindow::OnBrowserCreated(BrowserWindowInterface* browser) {
  if (BrowserInitState::From(browser)->create_params().window == this) {
    browser_ = browser->GetWeakPtr();
    browser_collection_observation_.Reset();
    close_subscription_ = browser->RegisterBrowserDidClose(base::BindRepeating(
        [](ArcBrowserWindow* window, BrowserWindowInterface*) {
          window->is_closed_ = true;
          if (window->close_callback_) {
            std::move(window->close_callback_).Run();
          }
        }, base::Unretained(this)));
  }
}

void ArcBrowserWindow::Show() {
  [native_window_.GetNativeNSWindow() orderFront:nil];
}
void ArcBrowserWindow::ShowInactive() {
  [native_window_.GetNativeNSWindow() orderFront:nil];
}
void ArcBrowserWindow::Hide() {
  [native_window_.GetNativeNSWindow() orderOut:nil];
}
void ArcBrowserWindow::Activate() {
  [native_window_.GetNativeNSWindow() makeKeyWindow];
}
void ArcBrowserWindow::Deactivate() {
  [native_window_.GetNativeNSWindow() resignKeyWindow];
}
void ArcBrowserWindow::SetBounds(const gfx::Rect& bounds) {
  [native_window_.GetNativeNSWindow()
      setFrame:gfx::ScreenRectToNSRect(bounds)
       display:YES];
}

Profile* ArcBrowserWindow::GetProfile() {
  return browser_ ? browser_->GetProfile() : nullptr;
}
void ArcBrowserWindow::EnterFullscreen(const url::Origin& origin,
                                       ExclusiveAccessBubbleType bubble_type,
                                       FullscreenTabParams fullscreen_tab_params) {
  if (!IsFullscreen()) {
    [native_window_.GetNativeNSWindow() toggleFullScreen:nil];
  }
}
void ArcBrowserWindow::ExitFullscreen() {
  if (IsFullscreen()) {
    [native_window_.GetNativeNSWindow() toggleFullScreen:nil];
  }
}
void ArcBrowserWindow::UpdateExclusiveAccessBubble(
    const ExclusiveAccessBubbleParams& params,
    ExclusiveAccessBubbleHideCallback first_hide_callback) {}
bool ArcBrowserWindow::IsExclusiveAccessBubbleDisplayed() const { return false; }
void ArcBrowserWindow::OnExclusiveAccessUserInput() {}
content::WebContents* ArcBrowserWindow::GetWebContentsForExclusiveAccess() {
  return browser_ ? browser_->GetTabStripModel()->GetActiveWebContents() : nullptr;
}
bool ArcBrowserWindow::CanUserEnterFullscreen() const { return true; }
bool ArcBrowserWindow::CanUserExitFullscreen() const { return true; }

}  // namespace arc
