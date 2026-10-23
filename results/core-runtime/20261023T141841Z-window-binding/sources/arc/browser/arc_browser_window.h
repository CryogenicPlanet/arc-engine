// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ARC_BROWSER_ARC_BROWSER_WINDOW_H_
#define ARC_BROWSER_ARC_BROWSER_WINDOW_H_

#include <memory>
#include <string>
#include <vector>

#include "base/callback_list.h"
#include "base/functional/callback_helpers.h"
#include "base/scoped_observation.h"
#include "base/supports_user_data.h"
#include "base/memory/weak_ptr.h"
#include "build/build_config.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/exclusive_access/exclusive_access_context.h"
#include "chrome/browser/ui/browser_window/public/browser_collection_observer.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/dialogs/browser_dialogs.h"
#include "chrome/browser/ui/location_bar/location_bar.h"
#include "chrome/browser/ui/translate/partial_translate_bubble_model.h"
#include "chrome/browser/ui/unload_controller.h"
#include "chrome/browser/ui/webui/tab_search/tab_search.mojom.h"
#include "chrome/common/buildflags.h"
#include "components/user_education/common/new_badge/new_badge_controller.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/mojom/window_show_state.mojom-forward.h"

#if !BUILDFLAG(IS_ANDROID)
#include "components/apps/link_capturing/intent_picker_info.h"
#endif  //  !BUILDFLAG(IS_ANDROID)

class LocationBarTesting;
class GlobalBrowserCollection;
class OmniboxView;
struct BrowserWindowCreateParams;

// Chrome window adapter for Arc-owned native windows. Non-visual Chrome UI has no surface here.
namespace arc {
class ArcBrowserWindow : public BrowserWindow,
                          public BrowserCollectionObserver,
                          public base::SupportsUserData,
                          public ExclusiveAccessContext {
 public:
  ArcBrowserWindow();
  ArcBrowserWindow(const ArcBrowserWindow&) = delete;
  ArcBrowserWindow& operator=(const ArcBrowserWindow&) = delete;
  ~ArcBrowserWindow() override;

  // BrowserWindow:
  void Show() override;
  void ShowInactive() override;
  void Hide() override;
  bool IsVisible() const override;
  void SetBounds(const gfx::Rect& bounds) override;
  void Close() override;
  void Activate() override;
  void Deactivate() override;
  bool IsActive() const override;
  void FlashFrame(bool flash) override {}
  ui::ZOrderLevel GetZOrderLevel() const override;
  void SetZOrderLevel(ui::ZOrderLevel order) override {}
  gfx::NativeWindow GetNativeWindow() const override;
  bool IsOnCurrentWorkspace() const override;
  bool IsVisibleOnScreen() const override;
  void SetTopControlsShownRatio(content::WebContents* web_contents,
                                float ratio) override;
  bool DoBrowserControlsShrinkRendererSize(
      const content::WebContents* contents) const override;
  ui::NativeTheme* GetNativeTheme() override;
  const ui::ThemeProvider* GetThemeProvider() const override;
  const ui::ColorProvider* GetColorProvider() const override;
  int GetTopControlsHeight() const override;
  void SetTopControlsGestureScrollInProgress(bool in_progress) override;
  std::vector<StatusBubble*> GetStatusBubbles() override;
  void UpdateTitleBar() override {}
  void UpdateLoadingAnimations(bool is_visible) override {}
  void OnActiveTabChanged(content::WebContents* old_contents,
                          content::WebContents* new_contents,
                          int index,
                          int reason) override {}
  void OnTabDetached(content::WebContents* contents, bool was_active) override {
  }

  gfx::Rect GetRestoredBounds() const override;
  ui::mojom::WindowShowState GetRestoredState() const override;
  gfx::Rect GetBounds() const override;
  gfx::Size GetContentsSize() const override;
  void SetContentsSize(const gfx::Size& size) override;
  bool IsMaximized() const override;
  bool IsMinimized() const override;
#if BUILDFLAG(IS_ANDROID)
  bool CanResize(ui::WindowResizePrecheckResult& result) const override {
    result = ui::WindowResizePrecheckResult::kOk;
    return true;
  }
#endif
  void Maximize() override {}
  void Minimize() override {}
  void Restore() override {}
  bool GetCanResize() override;
  ui::mojom::WindowShowState GetWindowShowState() const override;
  bool IsFullscreen() const override;
  LocationBar* GetLocationBar() const override;
  ui::AcceleratorProvider* GetAcceleratorProvider() override;
  PinnedToolbarActions* GetPinnedToolbarActions() override;
  autofill::AutofillBubbleHandler* GetAutofillBubbleHandler() override;
  void SetFocusToLocationBar(bool is_user_initiated) override {}
  void UpdateReloadStopState(bool is_loading, bool force) override {}
  void UpdateToolbar(content::WebContents* contents) override {}
  bool UpdateToolbarSecurityState() override;
  void UpdateCustomTabBarVisibility(bool visible, bool animate) override {}
  void ResetToolbarTabState(content::WebContents* contents) override {}
  void FocusToolbar() override {}
  void ToolbarSizeChanged(bool is_animating) override {}
  void TabDraggingStatusChanged(bool is_dragging) override {}
  void LinkOpeningFromGesture(WindowOpenDisposition disposition) override {}
  void FocusAppMenu() override {}
  void ShowAppMenu() override {}
  void PreHandleDragUpdate(const content::DropData& drop_data,
                           const gfx::PointF& point) override {}

  void PreHandleDragExit() override {}
  void HandleDragEnded() override {}
  content::KeyboardEventProcessingResult PreHandleKeyboardEvent(
      const input::NativeWebKeyboardEvent& event) override;
  bool HandleKeyboardEvent(const input::NativeWebKeyboardEvent& event) override;
  bool IsTabStripEditable() const override;
  void DisableTabStripEditingForTesting() override;
  void SetIsTabStripEditable(bool is_editable);
  bool IsToolbarVisible() const override;
  bool IsLocationBarVisible() const override;
  bool IsToolbarShowing() const override;
  bool IsUnframedModeEnabled() const override;
  void ShowChromeLabs() override {}
  BrowserView* AsBrowserView() override;
  void ShowUpdateChromeDialog() override {}
  void ShowBookmarkBubble(const GURL& url, bool already_bookmarked) override {}
#if !BUILDFLAG(IS_ANDROID)
  void ShowIntentPickerBubble(
      std::vector<apps::IntentPickerAppInfo> app_info,
      bool show_stay_in_chrome,
      bool show_remember_selection,
      apps::IntentPickerBubbleType bubble_type,
      const std::optional<url::Origin>& initiating_origin,
      IntentPickerResponse callback) override {}
#endif  //  !define(OS_ANDROID)
#if BUILDFLAG(IS_CHROMEOS)
  void ToggleMultitaskMenu() override;
#endif  // BUILDFLAG(IS_CHROMEOS)
  ShowTranslateBubbleResult ShowTranslateBubble(
      content::WebContents* contents,
      translate::TranslateStep step,
      const std::string& source_language,
      const std::string& target_language,
      translate::TranslateErrors error_type,
      bool is_user_gesture) override;

  DownloadBubbleUIController* GetDownloadBubbleUIController() override;
  void ConfirmBrowserCloseWithPendingDownloads(
      int download_count,
      UnloadController::DownloadCloseType dialog_type,
      base::OnceCallback<void(bool)> callback) override {}

  std::unique_ptr<FindBar> CreateFindBar() override;
  web_modal::WebContentsModalDialogHost* GetWebContentsModalDialogHost()
      override;
  web_modal::WebContentsModalDialogHost* GetWebContentsModalDialogHostFor(
      content::WebContents* web_contents) override;
  void ShowAvatarBubbleFromAvatarButton(bool is_source_keyboard) override {}
  void MaybeShowProfileSwitchIPH() override {}
  void MaybeShowSupervisedUserProfileSignInIPH() override {}

#if BUILDFLAG(IS_CHROMEOS) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_WIN) || \
    BUILDFLAG(IS_LINUX)
  void ShowHatsDialog(
      const std::string& site_id,
      const std::optional<std::string>& hats_histogram_name,
      const std::optional<uint64_t> hats_survey_ukm_id,
      base::OnceClosure success_callback,
      base::OnceClosure failure_callback,
      const SurveyBitsData& product_specific_bits_data,
      const SurveyStringData& product_specific_string_data) override {}

  void ShowIncognitoClearBrowsingDataDialog() override {}
  void ShowIncognitoHistoryDisclaimerDialog() override {}
#endif

  ExclusiveAccessContext* GetExclusiveAccessContext() override;
  std::string GetWorkspace() const override;
  bool IsVisibleOnAllWorkspaces() const override;
  void ShowEmojiPanel() override {}
  void ShowCaretBrowsingDialog() override {}
  std::unique_ptr<content::EyeDropper> OpenEyeDropper(
      content::RenderFrameHost* frame,
      content::EyeDropperListener* listener) override;

  void SetNativeWindow(gfx::NativeWindow window);

  void SetCloseCallback(base::OnceClosure close_callback);

  // ExclusiveAccessContext (the app owns fullscreen UI).
  Profile* GetProfile() override;
  void EnterFullscreen(const url::Origin& origin,
                       ExclusiveAccessBubbleType bubble_type,
                       FullscreenTabParams fullscreen_tab_params) override;
  void ExitFullscreen() override;
  void UpdateExclusiveAccessBubble(const ExclusiveAccessBubbleParams& params,
                                  ExclusiveAccessBubbleHideCallback first_hide_callback) override;
  bool IsExclusiveAccessBubbleDisplayed() const override;
  void OnExclusiveAccessUserInput() override;
  content::WebContents* GetWebContentsForExclusiveAccess() override;
  bool CanUserEnterFullscreen() const override;
  bool CanUserExitFullscreen() const override;

  void CreateTabSearchBubble() override {}
  void CloseTabSearchBubble() override {}

  void set_workspace(std::string workspace) { workspace_ = workspace; }
  void set_visible_on_all_workspaces(bool visible_on_all_workspaces) {
    visible_on_all_workspaces_ = visible_on_all_workspaces;
  }

  bool IsClosed() const { return is_closed_; }

 protected:
  void DeleteBrowserWindow() final;

 private:
  class ArcLocationBar : public LocationBar {
   public:
    ArcLocationBar() : LocationBar(/*command_updater=*/nullptr) {}
    ArcLocationBar(const ArcLocationBar&) = delete;
    ArcLocationBar& operator=(const ArcLocationBar&) = delete;
    ~ArcLocationBar() override = default;

    // LocationBar:
    void FocusLocation(bool select_all, bool clear_focus_if_failed) override {}
    void FocusSearch() override {}
    void UpdateFocusBehavior(bool toolbar_visible) override {}
    void UpdateContentSettingsIcons() override {}
    void SaveStateToContents(content::WebContents* contents) override {}
    void Revert() override {}
    OmniboxView* GetOmniboxView() override;
    OmniboxPopupView* GetOmniboxPopupView() override;
    OmniboxController* GetOmniboxController() override;
    bool ShouldCloseOmniboxPopup(ui::MouseEvent* event) override;
    ChipController* GetChipController() override;
    LocationBarTesting* GetLocationBarForTesting() override;
    LocationBarModel* GetLocationBarModel() override;
    content::WebContents* GetWebContents() override;
    std::optional<bubble_anchor_util::AnchorConfiguration> GetChipAnchor()
        override;
    void OnChanged() override {}
    void UpdateWithoutTabRestore() override {}
    ui::TrackedElement* GetAnchorOrNull() override;
    BrowserWindowInterface* GetBrowser() override;
    Profile* GetProfile() override;
    bool IsInitialized() const override;
    bool IsVisible() const override;
    bool IsDrawn() const override;
    bool IsFullscreen() const override;
    bool IsEditingOrEmpty() const override;
    bool IsMouseHovered() const override;
    bool IsFocusWithin() const override;
    void InvalidateLayout() override {}
    gfx::Rect Bounds() const override;
    gfx::Rect BoundsInScreen() const override;
    gfx::Size MinimumSize() const override;
    gfx::Size PreferredSize() const override;
    void Update(content::WebContents* contents) override {}
    void ResetTabState(content::WebContents* contents) override {}
    bool HasSecurityStateChanged() override;
    void AnnounceAlert(const std::u16string& announcement) override {}
  };

  // BrowserCollectionObserver:
  void OnBrowserCreated(BrowserWindowInterface* browser) override;

  void ClearNativeWindow();
  struct NativeWindowBinding;

  ArcLocationBar location_bar_;
  std::shared_ptr<NativeWindowBinding> native_window_;

  std::string workspace_;
  bool visible_on_all_workspaces_ = false;
  bool is_closed_ = false;
  bool is_tab_strip_editable_ = true;

  base::ScopedObservation<GlobalBrowserCollection, BrowserCollectionObserver>
      browser_collection_observation_{this};
  base::WeakPtr<BrowserWindowInterface> browser_;
  base::OnceClosure close_callback_;
  base::CallbackListSubscription close_subscription_;
  base::WeakPtrFactory<ArcBrowserWindow> weak_factory_{this};
};

}  // namespace arc

#endif  // ARC_BROWSER_ARC_BROWSER_WINDOW_H_
