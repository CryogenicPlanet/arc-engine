// Copyright 2026 ArcMod. Hosted Chrome extension UI.
#ifndef ARC_BROWSER_EXTENSIONS_BRIDGE_H_
#define ARC_BROWSER_EXTENSIONS_BRIDGE_H_
#ifdef __OBJC__
#import "arc/api/arc_internal.h"
namespace arc {
void InstallBrowserExtensions(ArcBrowser* browser, id delegate);
id BrowserExtensionDelegate(ArcBrowser* browser);
// Queue against the current bridge target. Clearing the delegate invalidates
// queued requests; a nonnil replacement updates the same target at delivery.
// False means there is no live bridge/callback target to enqueue against.
bool DispatchBrowserExtensionRequest(ArcBrowser* browser,
                                     void (^request)(id delegate));
ArcBrowser* BrowserForHostedExtensions(BrowserWindowInterface* browser);
void RegisterHostedExtensionAPIs();
}
#endif
#endif
