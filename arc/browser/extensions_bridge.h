// Copyright 2026 ArcMod. Hosted Chrome extension UI.
#ifndef ARC_BROWSER_EXTENSIONS_BRIDGE_H_
#define ARC_BROWSER_EXTENSIONS_BRIDGE_H_
#ifdef __OBJC__
#import "arc/api/arc_internal.h"
namespace arc {
void InstallBrowserExtensions(ArcBrowser* browser, id delegate);
id BrowserExtensionDelegate(ArcBrowser* browser);
ArcBrowser* BrowserForHostedExtensions(BrowserWindowInterface* browser);
void RegisterHostedExtensionAPIs();
}
#endif
#endif
