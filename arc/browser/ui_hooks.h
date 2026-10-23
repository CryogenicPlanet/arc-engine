// Copyright 2026 ArcMod. Host UI entry points called by Chromium's gated hooks.
#ifndef ARC_BROWSER_UI_HOOKS_H_
#define ARC_BROWSER_UI_HOOKS_H_

#include <memory>
#include "components/permissions/permission_prompt.h"
#include "chrome/browser/ui/login/login_handler.h"

namespace content {
class JavaScriptDialogManager;
class RenderWidgetHost;
class WebContents;
struct ContextMenuParams;
}
namespace ui { class MenuModel; }

void ArcUIWebContentsCreated(content::WebContents* contents);

namespace arc {
bool ShowHungRendererDialog(content::WebContents* contents, content::RenderWidgetHost* widget,
                            base::RepeatingClosure restarter);
bool HideHungRendererDialog(content::WebContents* contents, content::RenderWidgetHost* widget);
content::JavaScriptDialogManager* GetJavaScriptDialogManager(content::WebContents* contents);
bool ShowContextMenu(content::WebContents* contents, ui::MenuModel* model,
                     const content::ContextMenuParams& params);
std::unique_ptr<permissions::PermissionPrompt> CreatePermissionPrompt(
    content::WebContents* contents, permissions::PermissionPrompt::Delegate* delegate);
std::unique_ptr<LoginHandler> CreateLoginHandler(const net::AuthChallengeInfo& info,
    content::WebContents* contents, content::LoginDelegate::LoginAuthRequiredCallback callback);
}
#endif
