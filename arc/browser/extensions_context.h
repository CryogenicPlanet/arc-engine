// Copyright 2026 ArcMod. Native extension services behind Arc's context facade.
#ifndef ARC_BROWSER_EXTENSIONS_CONTEXT_H_
#define ARC_BROWSER_EXTENSIONS_CONTEXT_H_

#include "arc/browser/extensions_values.h"
class Profile;
namespace extensions { class Extension; }
namespace arc {
void EnsureInternalExtension(Profile* profile);
ExtensionInfo MakeExtensionInfo(Profile* profile,
                                const extensions::Extension& extension);
}  // namespace arc

#ifdef __OBJC__
ARC_EXPORT @interface ArcBrowserContextExtensions : NSObject
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (instancetype)initWithBrowserContextExtensions:(const void*)extensions
                              forBrowserContext:(ArcBrowserContext*)context;
@property(nonatomic, weak) ArcBrowserContext* owner;
- (void)setDelegate:(id)delegate;
- (BOOL)isExtensionSystemReady;
- (NSArray<ArcExtensionInfo*>*)listExtensions;
- (ArcExtensionInfo*)getExtensionInfo:(NSString*)extensionID;
- (void)enableExtension:(NSString*)extensionID;
- (void)disableExtension:(NSString*)extensionID;
- (void)reloadExtension:(NSString*)extensionID;
- (void)uninstallExtension:(NSString*)extensionID;
- (void)installExtensionFromWebstore:(NSString*)extensionID
                        unattended:(BOOL)unattended
                        completion:(void (^)(NSString*, NSInteger))completion;
- (void)installExtensionFromUnpackedPath:(NSString*)path
                            completion:(void (^)(NSString*, NSInteger))completion;
- (BOOL)runJSInExtensionBackgroundPage:(NSString*)extensionID js_code:(NSString*)code;
- (NSArray<ArcExtensionCommand*>*)getAllExtensionCommands;
- (void)executeExtensionCommand:(NSString*)extensionID commandName:(NSString*)name;
- (NSArray<NSString*>*)getPinnedExtensionOrder;
- (void)setPinnedExtensionOrder:(NSArray<NSString*>*)order;
@end
#endif
#endif  // ARC_BROWSER_EXTENSIONS_CONTEXT_H_
