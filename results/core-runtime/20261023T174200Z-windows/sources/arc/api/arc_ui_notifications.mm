// Copyright 2026 ArcMod. Native notifications presented by Arc's host app.
#import "arc/browser/ui_notifications.h"
#import "arc/browser/ui_values.h"
#import <UserNotifications/UserNotifications.h>

#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/memory/weak_ptr.h"
#include "base/no_destructor.h"
#include "base/notreached.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/notifications/notification_display_service_impl.h"
#include "chrome/browser/notifications/notification_platform_bridge.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/services/mac_notifications/notification_category_manager.h"
#include "chrome/common/notifications/notification_operation.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "ui/color/color_provider_manager.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/message_center/public/cpp/notification.h"

__attribute__((weak)) id ArcGetNotificationCenter() { return nil; }

@protocol UIHostNotificationCenter <NSObject>
- (void)display:(ArcNotification*)notification;
- (void)close:(NSString*)identifier;
@end

namespace {

std::string QualifiedID(Profile* profile, const std::string& local_id) {
  // Original 0x433e240: regular/incognito discriminator, path basename, local id.
  return std::string(profile && profile->IsOffTheRecord() ? "i|" : "r|") +
         (profile ? profile->GetBaseName().AsUTF8Unsafe() : std::string()) +
         "|" + local_id;
}

mac_notifications::NotificationCategoryManager& Categories() {
  // Original 0x473ed28 uses the system center for category registration; the
  // separate weak app service below owns notification presentation.
  static base::NoDestructor<mac_notifications::NotificationCategoryManager> manager(
      [UNUserNotificationCenter currentNotificationCenter]);
  return *manager;
}

}  // namespace

namespace arc {

// Our native record owns the snapshot, including its refcounted Chromium
// delegate. The weak Profile and active flag protect app-retained wrappers after
// replacement, shutdown, or close. No proprietary native aggregate is exposed.
struct UINotificationData : public std::enable_shared_from_this<UINotificationData> {
  UINotificationData(NotificationHandler::Type handler_type,
                     Profile* source_profile,
                     const message_center::Notification& source,
                     std::unique_ptr<NotificationCommon::Metadata> source_metadata)
      : notification(source),
        type(handler_type),
        profile(source_profile ? source_profile->GetWeakPtr() : base::WeakPtr<Profile>()),
        requires_profile(source_profile != nullptr),
        qualified_id(QualifiedID(source_profile, source.id())),
        metadata(std::move(source_metadata)) {}

  void Dispatch(NotificationOperation operation,
                std::optional<int> action_index = std::nullopt,
                std::optional<std::u16string> reply = std::nullopt,
                std::optional<bool> by_user = std::nullopt) {
    if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
      content::GetUIThreadTaskRunner({})->PostTask(
          FROM_HERE, base::BindOnce(&UINotificationData::Dispatch,
                                   shared_from_this(), operation, action_index,
                                   std::move(reply), by_user));
      return;
    }
    if (!active || (requires_profile && !profile)) {
      return;
    }
    // Reentrant native callbacks may replace this record or destroy the bridge.
    auto keep_alive = shared_from_this();
    auto on_closed = closed;
    scoped_refptr<message_center::NotificationDelegate> delegate(notification.delegate());
    if (operation == NotificationOperation::kClose) {
      active = false;
    }
    if (delegate) {
      switch (operation) {
        case NotificationOperation::kClick:
          delegate->Click(action_index, reply);
          break;
        case NotificationOperation::kClose:
          delegate->Close(by_user.value_or(false));
          break;
        case NotificationOperation::kSettings:
          delegate->SettingsClick();
          break;
        default:
          NOTREACHED();
      }
    } else if (profile) {
      // Web notifications normally have no NotificationDelegate. The display
      // service owns their page/service-worker/extension dispatch handlers.
      auto* service = NotificationDisplayServiceImpl::GetForProfile(profile.get());
      if (service && service->GetNotificationHandler(type)) {
        service->ProcessNotificationOperation(operation, type,
            notification.origin_url(), notification.id(), action_index, reply,
            by_user, std::nullopt, base::DoNothing());
      }
    }
    if (operation == NotificationOperation::kClose && on_closed) {
      on_closed.Run();
    }
  }

  message_center::Notification notification;
  NotificationHandler::Type type;
  base::WeakPtr<Profile> profile;
  bool requires_profile;
  bool active = true;
  std::string qualified_id;
  std::unique_ptr<NotificationCommon::Metadata> metadata;
  base::RepeatingClosure closed;
};

}  // namespace arc

@implementation ArcNotification {
  std::shared_ptr<arc::UINotificationData> _notification;
}
@synthesize iconURL = _iconURL;

- (instancetype)initWithNotification:(const void*)notification {
  CHECK(notification);
  if ((self = [super init])) {
    _notification = *static_cast<const std::shared_ptr<arc::UINotificationData>*>(notification);
    CHECK(_notification);
  }
  return self;
}
- (NSInteger)type {
  CHECK(_notification);
  // Original 0x4408c30/table 0xc9d8178 folds conversation into custom.
  const int type = _notification->notification.type();
  CHECK_GE(type, 0);
  CHECK_LE(type, 6);
  return std::min(type, 5);
}
- (NSInteger)priority {
  CHECK(_notification);
  const int priority = _notification->notification.priority();
  CHECK_GE(priority, -2);
  CHECK_LE(priority, 3);
  return priority + 2;
}
- (NSString*)title { CHECK(_notification); return ArcNSString(_notification->notification.title()); }
- (NSString*)id { CHECK(_notification); return ArcNSString(_notification->qualified_id); }
- (NSString*)browserContextQualifiedID {
  CHECK(_notification);
  return ArcNSString(_notification->qualified_id);
}
- (NSString*)browserContextLocalID {
  CHECK(_notification);
  return ArcNSString(_notification->notification.id());
}
- (NSString*)message { CHECK(_notification); return ArcNSString(_notification->notification.message()); }
- (NSString*)contextMessage {
  CHECK(_notification);
  const auto& native = _notification->notification;
  return native.UseOriginAsContextMessage()
      ? ArcNSString(base::UTF8ToUTF16(native.origin_url().spec()))
      : ArcNSString(native.context_message());
}
- (NSString*)progressStatus {
  CHECK(_notification);
  return ArcNSString(_notification->notification.progress_status());
}
- (NSURL*)origin { CHECK(_notification); return ArcNSURL(_notification->notification.origin_url()); }
- (NSURL*)documentURL {
  CHECK(_notification);
  const auto* metadata = NonPersistentNotificationMetadata::From(_notification->metadata.get());
  return metadata ? ArcNSURL(metadata->document_url) : nil;
}
- (int)progress { CHECK(_notification); return _notification->notification.progress(); }
- (BOOL)renotify { CHECK(_notification); return _notification->notification.renotify(); }
- (BOOL)permanent {
  CHECK(_notification);
  return _notification->notification.never_timeout() ||
         _notification->type == NotificationHandler::Type::WEB_PERSISTENT;
}
- (ArcBrowserContext*)browserContext {
  CHECK(_notification);
  Profile* profile = _notification->profile.get();
  return profile ? [ArcBrowserContext wrapperForProfile:profile] : nil;
}
- (BOOL)showSnoozeButton {
  CHECK(_notification);
  return _notification->notification.should_show_snooze_button();
}
- (BOOL)showSettingsButton {
  CHECK(_notification);
  return _notification->notification.should_show_settings_button();
}
- (NSArray*)items {
  CHECK(_notification);
  const auto& native_items = _notification->notification.items();
  if (native_items.empty()) {
    return nil;
  }
  NSMutableArray* items = [NSMutableArray arrayWithCapacity:native_items.size()];
  for (const auto& native : native_items) {
    ArcNotificationItem* item = [[ArcNotificationItem alloc] init];
    item.message = ArcNSString(native.message());
    item.title = ArcNSString(native.title());
    [items addObject:item];
  }
  return [items copy];
}
- (NSArray*)buttons {
  CHECK(_notification);
  const auto& native_buttons = _notification->notification.buttons();
  if (native_buttons.empty()) {
    return nil;
  }
  NSMutableArray* buttons = [NSMutableArray arrayWithCapacity:native_buttons.size()];
  for (const auto& native : native_buttons) {
    ArcNotificationButton* button = [[ArcNotificationButton alloc] init];
    button.title = ArcNSString(native.title);
    button.icon = native.icon.AsNSImage();
    button.placeholder = native.placeholder ? ArcNSString(*native.placeholder) : nil;
    button.isTextInput = native.placeholder.has_value();
    [buttons addObject:button];
  }
  return [buttons copy];
}
- (NSImage*)icon {
  CHECK(_notification);
  const auto icon = _notification->notification.icon();
  if (icon.IsEmpty()) {
    return nil;
  }
  if (icon.IsImage()) {
    return icon.GetImage().AsNSImage();
  }
  Profile* profile = _notification->profile.get();
  const ui::ColorProvider* colors = profile
      ? ThemeServiceFactory::GetForProfile(profile)->GetColorProvider()
      : ui::ColorProviderManager::Get().GetColorProviderFor(ui::ColorProviderKey());
  return gfx::Image(icon.Rasterize(colors)).AsNSImage();
}
- (NSImage*)image { CHECK(_notification); return _notification->notification.image().AsNSImage(); }
- (NSImage*)smallImage {
  CHECK(_notification);
  return _notification->notification.small_image().AsNSImage();
}
- (NSString*)category {
  CHECK(_notification);
  CHECK([NSThread isMainThread]);
  mac_notifications::NotificationCategoryManager::Buttons buttons;
  for (const auto& button : _notification->notification.buttons()) {
    // Original 0x473ed28 explicitly engages every placeholder before creating
    // native text-input actions, even when the placeholder string is empty.
    buttons.emplace_back(button.title, button.placeholder.value_or(std::u16string()));
  }
  return Categories().GetOrCreateCategory(_notification->qualified_id, buttons,
      _notification->notification.should_show_settings_button());
}
- (void)didClose:(BOOL)byUser {
  CHECK(_notification);
  _notification->Dispatch(NotificationOperation::kClose, std::nullopt, std::nullopt, byUser);
}
- (void)didClick:(int)buttonIndex input:(NSString*)input {
  CHECK(_notification);
  _notification->Dispatch(NotificationOperation::kClick,
      buttonIndex < 0 ? std::nullopt : std::optional<int>(buttonIndex),
      input ? std::optional<std::u16string>(ArcUTF16(input)) : std::nullopt);
}
- (void)didClickSettings {
  CHECK(_notification);
  _notification->Dispatch(NotificationOperation::kSettings);
}
- (void)didReceiveResponse:(id)response {
  // Original parser 0x473f138 and wrapper 0x440a1f8 discard text-input content.
  NSString* action = [response actionIdentifier];
  if ([action isEqual:UNNotificationDefaultActionIdentifier]) {
    [self didClick:-1 input:nil];
  } else if ([action isEqualToString:@"settingsButton"]) {
    [self didClickSettings];
  } else if ([action isEqualToString:@"buttonOne"]) {
    [self didClick:0 input:nil];
  } else if ([action isEqualToString:@"buttonTwo"]) {
    [self didClick:1 input:nil];
  } else {
    // Includes dismiss, closeButton, and unknown identifiers.
    [self didClose:YES];
  }
}
@end

namespace arc {
namespace {

struct NotificationRecord {
  std::shared_ptr<UINotificationData> data;
  ArcNotification* wrapper;
};

class UINotificationPlatformBridge : public NotificationPlatformBridge {
 public:
  UINotificationPlatformBridge() = default;
  ~UINotificationPlatformBridge() override {
    // Invalidate before releasing wrappers retained by the app.
    for (const auto& [identifier, record] : records_) {
      record.data->active = false;
      record.data->closed.Reset();
      Categories().ReleaseCategories({identifier});
    }
  }

  void Display(NotificationHandler::Type type,
               Profile* profile,
               const message_center::Notification& notification,
               std::unique_ptr<NotificationCommon::Metadata> metadata) override {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    auto data = std::make_shared<UINotificationData>(type, profile, notification,
                                                   std::move(metadata));
    const std::string identifier = data->qualified_id;
    Remove(identifier);
    data->closed = base::BindRepeating(&UINotificationPlatformBridge::RemoveIfCurrent,
        weak_factory_.GetWeakPtr(), identifier, std::weak_ptr<UINotificationData>(data));
    ArcNotification* wrapper = [[ArcNotification alloc] initWithNotification:&data];
    records_.emplace(identifier, NotificationRecord{data, wrapper});
    id<UIHostNotificationCenter> center = ArcGetNotificationCenter();
    if ([center respondsToSelector:@selector(display:)]) {
      [center display:wrapper];
    }
  }

  void Close(Profile* profile, const std::string& notification_id) override {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    std::string identifier = QualifiedID(profile, notification_id);
    Remove(identifier);
    id<UIHostNotificationCenter> center = ArcGetNotificationCenter();
    if ([center respondsToSelector:@selector(close:)]) {
      [center close:ArcNSString(identifier)];
    }
  }

  void GetDisplayed(Profile* profile, GetDisplayedNotificationsCallback callback) const override {
    GetDisplayedImpl(profile, std::nullopt, std::move(callback));
  }

  void GetDisplayedForOrigin(Profile* profile, const GURL& origin,
                            GetDisplayedNotificationsCallback callback) const override {
    GetDisplayedImpl(profile, origin, std::move(callback));
  }

  void SetReadyCallback(NotificationBridgeReadyCallback callback) override {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    std::move(callback).Run(true);
  }

  void DisplayServiceShutDown(Profile* profile) override {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    std::vector<std::string> identifiers;
    for (const auto& [identifier, record] : records_) {
      if (profile ? record.data->requires_profile && record.data->profile.get() == profile
                  : !record.data->requires_profile) {
        identifiers.push_back(identifier);
      }
    }
    auto weak = weak_factory_.GetWeakPtr();
    for (const auto& identifier : identifiers) {
      Remove(identifier);
      id<UIHostNotificationCenter> center = ArcGetNotificationCenter();
      if ([center respondsToSelector:@selector(close:)]) {
        [center close:ArcNSString(identifier)];
      }
      if (!weak) { return; }
    }
  }

 private:
  void Remove(const std::string& identifier) {
    auto record = records_.find(identifier);
    if (record != records_.end()) {
      record->second.data->active = false;
      record->second.data->closed.Reset();
      records_.erase(record);
    }
    Categories().ReleaseCategories({identifier});
  }

  void RemoveIfCurrent(const std::string& identifier,
                       std::weak_ptr<UINotificationData> data) {
    auto record = records_.find(identifier);
    if (record != records_.end() && record->second.data == data.lock()) {
      Remove(identifier);
    }
  }

  void GetDisplayedImpl(Profile* profile, const std::optional<GURL>& origin,
                        GetDisplayedNotificationsCallback callback) const {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    std::set<std::string> identifiers;
    for (const auto& [identifier, record] : records_) {
      const auto& data = record.data;
      if (data->active && data->profile.get() == profile &&
          (!data->requires_profile || data->profile) &&
          (!origin || data->notification.origin_url() == *origin)) {
        identifiers.insert(data->notification.id());
      }
    }
    // The protocol has no app displayed-query method, so this is an in-process
    // inventory only, not synchronization with Notification Center after restart.
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), std::move(identifiers), false));
  }

  std::map<std::string, NotificationRecord> records_;
  base::WeakPtrFactory<UINotificationPlatformBridge> weak_factory_{this};
};

}  // namespace

std::unique_ptr<NotificationPlatformBridge> CreateNotificationPlatformBridge() {
  return std::make_unique<UINotificationPlatformBridge>();
}

}  // namespace arc
