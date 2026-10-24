// Copyright 2026 ArcMod. Chrome invokes these only for --arcmod-host-app.
#include "arc/browser/ui_hooks.h"

#import "arc/api/arc_internal.h"
#include "base/functional/bind.h"
#include "net/base/auth.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"

@interface ArcWebContents (ActivateContentsHook)
- (id)uiDelegate;
@end

@protocol ArcActivateContentsUI <NSObject>
@optional
- (void)webContentsActivate:(ArcWebContents*)contents;
@end

namespace {

// Arc's engine tells the tab's UI delegate (webContentsActivate:, arcmod-tabs-restore golden), which selects it.
bool ActivateContents(content::WebContents* contents) {
  ArcWebContents* owner = [ArcWebContents wrapperForWebContents:contents];
  id<ArcActivateContentsUI> ui = [owner respondsToSelector:@selector(uiDelegate)] ? [owner uiDelegate] : nil;
  if (![ui respondsToSelector:@selector(webContentsActivate:)]) {
    return false;
  }
  [ui webContentsActivate:owner];
  return true;
}

}  // namespace

__attribute__((constructor)) static void InstallArcHostUIHooks() {
  arcmod::GetActivateContentsHook() = base::BindRepeating(&ActivateContents);
  arcmod::GetJavaScriptDialogManagerHook() = base::BindRepeating(&arc::GetJavaScriptDialogManager);
  arcmod::GetContextMenuHook() = base::BindRepeating(&arc::ShowContextMenu);
  arcmod::GetPermissionPromptHook() = base::BindRepeating(
      [](content::WebContents* contents, void* delegate) {
        return arc::CreatePermissionPrompt(contents,
            static_cast<permissions::PermissionPrompt::Delegate*>(delegate));
      });
  arcmod::GetLoginHandlerHook() = base::BindRepeating(&arc::CreateLoginHandler);
}
