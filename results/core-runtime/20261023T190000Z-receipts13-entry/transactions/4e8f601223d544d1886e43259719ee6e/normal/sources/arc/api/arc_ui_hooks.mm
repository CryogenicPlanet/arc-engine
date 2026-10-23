// Copyright 2026 ArcMod. Chrome invokes these only for --arcmod-host-app.
#include "arc/browser/ui_hooks.h"

#include "base/functional/bind.h"
#include "net/base/auth.h"
#include "chrome/browser/arcmod/arcmod_hooks.h"

__attribute__((constructor)) static void InstallArcHostUIHooks() {
  arcmod::GetJavaScriptDialogManagerHook() = base::BindRepeating(&arc::GetJavaScriptDialogManager);
  arcmod::GetContextMenuHook() = base::BindRepeating(&arc::ShowContextMenu);
  arcmod::GetPermissionPromptHook() = base::BindRepeating(
      [](content::WebContents* contents, void* delegate) {
        return arc::CreatePermissionPrompt(contents,
            static_cast<permissions::PermissionPrompt::Delegate*>(delegate));
      });
  arcmod::GetLoginHandlerHook() = base::BindRepeating(&arc::CreateLoginHandler);
}
