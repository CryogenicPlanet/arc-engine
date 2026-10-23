// Copyright 2026 ArcMod. Hooks that ArcCore (Arc's engine interface, //arc, linked into the same framework)
// fills in when Arc's app hosts this engine (--arcmod-host-app). Chrome code calls them only when set, so
// Chrome itself does not depend on //arc. Header-only: function-local statics in inline functions are shared
// across translation units.

#ifndef CHROME_BROWSER_ARCMOD_ARCMOD_HOOKS_H_
#define CHROME_BROWSER_ARCMOD_ARCMOD_HOOKS_H_

#include <cstdint>
#include <memory>
#include <optional>

#include "base/files/file_path.h"
#include "base/functional/callback.h"
#include "base/functional/callback_helpers.h"
#include "base/no_destructor.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/tab_helpers.h"
#include "net/base/auth.h"

class LoginHandler;
class Profile;
namespace net {
class AuthChallengeInfo;
}  // namespace net
namespace permissions {
class PermissionPrompt;
}  // namespace permissions
namespace content {
class JavaScriptDialogManager;
class NavigationThrottleRegistry;
class VideoOverlayWindow;
class VideoPictureInPictureWindowController;
class WebContents;
struct ContextMenuParams;
}  // namespace content
namespace extensions {
class Extension;
class PermissionSet;
}  // namespace extensions
namespace ui {
class MenuModel;
}  // namespace ui
namespace download {
class DownloadItem;
}  // namespace download

class BrowserWindowInterface;
struct BrowserWindowCreateParams;
namespace arcmod {

// Runs after Chrome computes all native restoration params. A scoped embedder
// request may supply its BrowserWindow and return a one-shot notification that
// runs after native creation and before tab insertion. Empty means no override.
using SessionBrowserCreatedCallback =
    base::OnceCallback<void(base::WeakPtr<BrowserWindowInterface>)>;
using SessionBrowserFactoryHook = base::RepeatingCallback<
    SessionBrowserCreatedCallback(Profile*, BrowserWindowCreateParams&)>;
inline SessionBrowserFactoryHook& GetSessionBrowserFactoryHook() {
  static base::NoDestructor<SessionBrowserFactoryHook> hook;
  return *hook;
}


// Marks one actual historical WebContents insertion, preserving both source
// IDs and optional presence. The returned scope clears its exact WC marker.
using SessionTabRestoreScopeHook = base::RepeatingCallback<
    base::ScopedClosureRunner(content::WebContents*, int32_t, int32_t)>;
inline SessionTabRestoreScopeHook& GetSessionTabRestoreScopeHook() {
  static base::NoDestructor<SessionTabRestoreScopeHook> hook;
  return *hook;
}

// Adds ArcCore's navigation throttles (synchronous navigation policy from Arc's app) to every navigation.
// Called from ChromeContentBrowserClient::CreateThrottlesForNavigation after Chrome's own throttles.
using NavigationThrottleHook =
    base::RepeatingCallback<void(content::NavigationThrottleRegistry&)>;
inline NavigationThrottleHook& GetNavigationThrottleHook() {
  static base::NoDestructor<NavigationThrottleHook> hook;
  return *hook;
}

// Lets Arc's app choose a download's filename before the download starts (Arc's
// browserContext:onDeterminingFilename:... delegate call). Runs from
// ChromeDownloadManagerDelegate::NotifyExtensions, ahead of extensions. Returns
// true if it will answer: then it must run |done| exactly once with the chosen
// path, or an empty path to keep Chrome's default.
using DownloadFilenameHook = base::RepeatingCallback<bool(
    Profile* profile,
    download::DownloadItem* download,
    const base::FilePath& default_basename,
    base::OnceCallback<void(const base::FilePath&)> done)>;
inline DownloadFilenameHook& GetDownloadFilenameHook() {
  static base::NoDestructor<DownloadFilenameHook> hook;
  return *hook;
}

// Synchronous observer-attachment window: after LoadErrorReporter::Init and
// before ExtensionService construction/Init. Never initialize/reenter Chrome,
// load extensions, create a facade or call an app delegate from this hook.
using ExtensionProfileWillInitializeHook =
    base::RepeatingCallback<void(Profile*)>;
inline ExtensionProfileWillInitializeHook& GetExtensionProfileWillInitializeHook() {
  static base::NoDestructor<ExtensionProfileWillInitializeHook> hook;
  return *hook;
}

// Each hook below is empty unless ArcCore set it (host mode). Callers fall back to
// Chrome's own behavior when a hook is empty or declines.

// Tab group id that extension APIs report for a tab (Arc's app owns tab groups):
// chrome.tabs Tab.groupId and tabs.query({groupId}). nullopt: no group (-1).
using ExtensionGroupIdHook =
    base::RepeatingCallback<std::optional<int>(content::WebContents*)>;
inline ExtensionGroupIdHook& GetExtensionGroupIdHook() {
  static base::NoDestructor<ExtensionGroupIdHook> hook;
  return *hook;
}

// Extension permission confirmation shown by Arc's app instead of Chrome's
// dialog. Returns true if it will answer: then it runs |done| exactly once.
using ExtensionPermissionsHook = base::RepeatingCallback<bool(
    Profile* profile,
    const extensions::Extension* extension,
    const extensions::PermissionSet* permissions,
    base::OnceCallback<void(bool accepted)> done)>;
inline ExtensionPermissionsHook& GetExtensionPermissionsHook() {
  static base::NoDestructor<ExtensionPermissionsHook> hook;
  return *hook;
}

// JavaScript dialogs (alert/confirm/prompt/beforeunload) for a tab, shown by
// Arc's app. Returning nullptr keeps Chrome's manager.
using JavaScriptDialogManagerHook = base::RepeatingCallback<
    content::JavaScriptDialogManager*(content::WebContents*)>;
inline JavaScriptDialogManagerHook& GetJavaScriptDialogManagerHook() {
  static base::NoDestructor<JavaScriptDialogManagerHook> hook;
  return *hook;
}

// Context menus shown by Arc's app. Returns true if it presents |model| (owned
// by Chrome's RenderViewContextMenu until the next menu or its destruction;
// hold it weakly).
using ContextMenuHook = base::RepeatingCallback<bool(
    content::WebContents* web_contents,
    ui::MenuModel* model,
    const content::ContextMenuParams& params)>;
inline ContextMenuHook& GetContextMenuHook() {
  static base::NoDestructor<ContextMenuHook> hook;
  return *hook;
}

// Video picture-in-picture window hosted by Arc's app. nullptr: Chrome's.
using VideoOverlayWindowHook =
    base::RepeatingCallback<std::unique_ptr<content::VideoOverlayWindow>(
        content::VideoPictureInPictureWindowController*)>;
inline VideoOverlayWindowHook& GetVideoOverlayWindowHook() {
  static base::NoDestructor<VideoOverlayWindowHook> hook;
  return *hook;
}

// DevTools frontends shown by Arc's app instead of a Chrome window. Called by
// DevToolsWindow::Show; Chrome then runs the toggle action and returns.
using DevToolsShowHook = base::RepeatingCallback<void(
    content::WebContents* inspected,
    content::WebContents* frontend,
    bool docked)>;
inline DevToolsShowHook& GetDevToolsShowHook() {
  static base::NoDestructor<DevToolsShowHook> hook;
  return *hook;
}

// Permission prompts shown by Arc's app (same shape as
// permissions::PermissionPrompt::Factory). nullptr: Chrome's prompt.
using PermissionPromptHook = base::RepeatingCallback<
    std::unique_ptr<permissions::PermissionPrompt>(content::WebContents*,
                                                    void* /*PermissionPrompt::Delegate**/)>;
inline PermissionPromptHook& GetPermissionPromptHook() {
  static base::NoDestructor<PermissionPromptHook> hook;
  return *hook;
}

// HTTP auth prompts shown by Arc's app. When set, it handles every prompt.
// |auth_required_callback| is content::LoginDelegate::LoginAuthRequiredCallback.
using LoginHandlerHook = base::RepeatingCallback<std::unique_ptr<LoginHandler>(
    const net::AuthChallengeInfo& auth_info,
    content::WebContents* web_contents,
    base::OnceCallback<void(const std::optional<net::AuthCredentials>&)>
        auth_required_callback)>;
inline LoginHandlerHook& GetLoginHandlerHook() {
  static base::NoDestructor<LoginHandlerHook> hook;
  return *hook;
}

// Gives ArcCore's standalone web contents (created by Arc's app outside a browser tab strip) Chrome's full
// set of tab helpers, as tab strip adoption does.
class HostedTabHelpers {
 public:
  static void Attach(content::WebContents* web_contents) {
    TabHelpers::AttachTabHelpers(web_contents);
  }
};

// Host-only interception BEFORE CanCloseContents mutates unload state. The
// callback either consumes/cancels the exact source or runs |continue_close|.
// Continuation owns only WeakPtrs to THIS delegate and THIS source; it resumes
// the original operation without entering this hook again. It may run inline.
// Empty hook and nonhost Chrome preserve the original path. This hook does not
// implement public Busy policy, a source token, or any bulk-close interception.
using NativeCloseContentsHook = base::RepeatingCallback<void(
    content::WebContents* source, base::OnceClosure continue_close)>;
inline NativeCloseContentsHook& GetNativeCloseContentsHook() {
  static base::NoDestructor<NativeCloseContentsHook> hook;
  return *hook;
}

// Synchronous pre-forward notification. A consumer may invoke app code, then
// choose whether the ORIGINAL BeforeUnload forwarding operation still applies.
// Skip leaves the caller's borrowed output bool UNCHANGED. The output pointer
// is never exposed to the hook; no delayed response/continuation is supported.
// Consumers must validate logical association after their own app callouts.
enum class NativeBeforeUnloadDisposition {
  kContinueOriginal,
  kSkipOriginal,
};
using NativeBeforeUnloadFiredHook =
    base::RepeatingCallback<NativeBeforeUnloadDisposition(
        content::WebContents* source, bool proceed)>;
inline NativeBeforeUnloadFiredHook& GetNativeBeforeUnloadFiredHook() {
  static base::NoDestructor<NativeBeforeUnloadFiredHook> hook;
  return *hook;
}

}  // namespace arcmod

#endif  // CHROME_BROWSER_ARCMOD_ARCMOD_HOOKS_H_
