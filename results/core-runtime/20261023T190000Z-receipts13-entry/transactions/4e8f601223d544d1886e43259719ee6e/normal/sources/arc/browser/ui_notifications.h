// Copyright 2026 ArcMod. Host notification presentation and response adapter.
#ifndef ARC_BROWSER_UI_NOTIFICATIONS_H_
#define ARC_BROWSER_UI_NOTIFICATIONS_H_

#include <memory>

class NotificationPlatformBridge;

namespace arc {
// Install through Chromium's host-gated NotificationPlatformBridge factory.
std::unique_ptr<NotificationPlatformBridge> CreateNotificationPlatformBridge();
}  // namespace arc

#ifdef __OBJC__
#import "arc/api/arc_internal.h"

// The app-services package supplies a strong implementation. The notification
// package has a weak nil default until that package is linked.
id ArcGetNotificationCenter();

ARC_EXPORT @interface ArcNotification : NSObject
// Internal producer contract: pointer to shared_ptr<arc::UINotificationData>.
// This is not the proprietary arc::Notification ABI.
- (instancetype)initWithNotification:(const void*)notification;
@property(nonatomic, strong) NSURL* iconURL;
@property(nonatomic, readonly) NSInteger type;
@property(nonatomic, readonly) NSInteger priority;
@property(nonatomic, readonly) NSString* title;
@property(nonatomic, readonly) NSString* id;
@property(nonatomic, readonly) NSString* browserContextLocalID;
@property(nonatomic, readonly) NSString* browserContextQualifiedID;
@property(nonatomic, readonly) NSString* message;
@property(nonatomic, readonly) NSString* contextMessage;
@property(nonatomic, readonly) NSString* progressStatus;
@property(nonatomic, readonly) NSURL* origin;
@property(nonatomic, readonly) NSURL* documentURL;
@property(nonatomic, readonly) int progress;
@property(nonatomic, readonly) BOOL renotify;
@property(nonatomic, readonly) BOOL permanent;
@property(nonatomic, readonly) ArcBrowserContext* browserContext;
@property(nonatomic, readonly) BOOL showSnoozeButton;
@property(nonatomic, readonly) BOOL showSettingsButton;
@property(nonatomic, readonly) NSArray* items;
@property(nonatomic, readonly) NSArray* buttons;
@property(nonatomic, readonly) NSImage* icon;
@property(nonatomic, readonly) NSImage* image;
@property(nonatomic, readonly) NSImage* smallImage;
@property(nonatomic, readonly) NSString* category;
- (void)didClose:(BOOL)byUser;
- (void)didClick:(int)buttonIndex input:(NSString*)input;
- (void)didClickSettings;
- (void)didReceiveResponse:(id)response;
@end
#endif  // __OBJC__

#endif  // ARC_BROWSER_UI_NOTIFICATIONS_H_
