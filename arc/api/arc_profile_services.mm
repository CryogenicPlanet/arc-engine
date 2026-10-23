// Copyright 2026 ArcMod. Profile content settings, favicons and DOM storage.
#import "arc/api/arc_internal.h"

#include <array>
#include <optional>
#include <vector>

#include "base/functional/bind.h"
#include "base/containers/span.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "base/task/cancelable_task_tracker.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/favicon/favicon_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/browser/permission_settings_registry.h"
#include "components/content_settings/core/browser/permission_settings_info.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/favicon/core/favicon_service.h"
#include "components/favicon_base/favicon_types.h"
#include "components/prefs/pref_service.h"
#include "components/services/storage/public/mojom/local_storage_control.mojom.h"
#include "content/public/browser/storage_partition.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/blink/public/common/storage_key/storage_key.h"
#include "third_party/blink/public/mojom/dom_storage/storage_area.mojom.h"
#include "ui/gfx/image/image.h"
#include "url/origin.h"

namespace {
// ArcCore 1.170: conversion table at 0xc9d7034. Arc's public enum is stable,
// Chromium's ContentSettingsType can be reordered between releases.
constexpr std::array<int, 37> kNativeContentTypes = {
    4, 5, 10, 59, 9, 33, 6, 2, 1, 3, 60, 26, 22, 31, 13, 14, 52, 36, 44,
    40, 49, 61, 21, 43, 102, 55, 56, 39, 73, 86, 89, 93, 97, 98, 103, 110, 111};
// Verified by all 37 calls to the original ArcSiteSettingInfo.permissionType.
constexpr std::array<NSInteger, 37> kPermissionTypes = {
    9, 9, 2, 1, 13, 20, 16, 28, 28, 28, 24, 28, 28, 28, 28, 14, 4, 28,
    28, 28, 7, 8, 28, 28, 10, 22, 0, 12, 11, 28, 3, 28, 17, 18, 23, 25, 26};
struct SiteSettingRecord {
  NSString* origin;
  NSUInteger type;
  NSUInteger setting;
};
std::optional<ContentSettingsType> NativeType(NSUInteger type) {
  if (type >= kNativeContentTypes.size()) return std::nullopt;
  return static_cast<ContentSettingsType>(kNativeContentTypes[type]);
}
HostContentSettingsMap* Settings(ArcBrowserContext* context) {
  Profile* profile = ArcProfileFrom(context);
  return profile ? HostContentSettingsMapFactory::GetForProfile(profile) : nullptr;
}
NSString* OriginString(id value) {
  return ArcNSString(url::Origin::Create(ArcGURL(value)).Serialize());
}
ContentSetting ExplicitSetting(HostContentSettingsMap* map, const GURL& url,
                               ContentSettingsType type) {
  content_settings::SettingInfo info;
  if (!content_settings::PermissionSettingsRegistry::GetInstance()->Get(type))
    return CONTENT_SETTING_DEFAULT;
  PermissionSetting value = map->GetPermissionSetting(url, url, type, &info);
  // Arc's scalar getter returns DEFAULT for an options-valued permission.
  const auto* content = std::get_if<ContentSetting>(&value);
  ContentSetting setting = content ? *content : CONTENT_SETTING_DEFAULT;
  // Original 0x4348de4 returns DEFAULT when both matched patterns are '*'.
  if (info.primary_pattern == ContentSettingsPattern::Wildcard() &&
      info.secondary_pattern == ContentSettingsPattern::Wildcard())
    return CONTENT_SETTING_DEFAULT;
  return setting;
}
void WriteSetting(HostContentSettingsMap* map, const GURL& url,
                  ContentSettingsType type, NSUInteger setting) {
  const auto* registration = content_settings::PermissionSettingsRegistry::GetInstance()->Get(type);
  if (!registration) return;
  std::optional<PermissionSetting> native;
  if (setting) {
    if (type == ContentSettingsType::GEOLOCATION_WITH_OPTIONS) {
      if (setting > 3) return;
      const auto option = static_cast<PermissionOption>(setting);
      native = GeolocationSetting{option, option};
    } else {
      native = static_cast<ContentSetting>(setting);
    }
    if (!registration->delegate().IsValid(*native)) return;
  }
  map->SetPermissionSettingDefaultScope(url, url, type, native);
}
struct FaviconRequests final : base::SupportsUserData::Data {
  base::CancelableTaskTracker tracker;
};
constexpr char kFaviconRequestsKey[] = "ArcProfileFaviconRequests";
base::CancelableTaskTracker* FaviconTracker(Profile* profile) {
  auto* data = static_cast<FaviconRequests*>(profile->GetUserData(kFaviconRequestsKey));
  if (!data) {
    auto owned = std::make_unique<FaviconRequests>();
    data = owned.get();
    profile->SetUserData(kFaviconRequestsKey, std::move(owned));
  }
  return &data->tracker;
}
std::vector<uint8_t> StorageBytes(NSString* value) {
  const auto text = ArcUTF16(value);
  bool latin1 = true;
  for (char16_t c : text) if (c > 255) { latin1 = false; break; }
  std::vector<uint8_t> bytes;
  bytes.reserve(1 + text.size() * (latin1 ? 1 : 2));
  bytes.push_back(latin1 ? 1 : 0);
  for (char16_t c : text) {
    bytes.push_back(c & 0xff);
    if (!latin1) bytes.push_back(c >> 8);
  }
  return bytes;
}
NSString* StorageString(const std::vector<uint8_t>& bytes) {
  if (bytes.empty()) return nil;
  if (bytes.front() == 1)
    return [[NSString alloc] initWithBytes:base::span(bytes).subspan(1u).data() length:bytes.size() - 1
                                encoding:NSISOLatin1StringEncoding];
  if (bytes.front() == 0 && (bytes.size() - 1) % 2 == 0)
    return [[NSString alloc] initWithBytes:base::span(bytes).subspan(1u).data() length:bytes.size() - 1
                                encoding:NSUTF16LittleEndianStringEncoding];
  return nil;
}
}  // namespace

ARC_EXPORT @interface ArcSiteSettingInfo : NSObject
@property(nonatomic, strong) NSString* originUrl;
@property(nonatomic) NSUInteger contentSettingsType;
@property(nonatomic) NSUInteger siteSetting;
@property(nonatomic, readonly) NSInteger permissionType;
- (instancetype)initWithSiteSettingInfo:(const void*)record;
@end

@implementation ArcSiteSettingInfo {
  NSString* _originUrl;
  NSUInteger _contentSettingsType;
  NSUInteger _siteSetting;
}
- (instancetype)init {
  if ((self = [super init])) _originUrl = @"null";
  return self;
}
- (instancetype)initWithSiteSettingInfo:(const void*)record {
  if ((self = [super init]) && record) {
    const auto& value = *static_cast<const SiteSettingRecord*>(record);
    _originUrl = value.origin;
    _contentSettingsType = value.type;
    _siteSetting = value.setting;
  }
  return self;
}
- (NSString*)originUrl { return _originUrl; }
- (void)setOriginUrl:(NSString*)value { _originUrl = OriginString(value); }
- (NSUInteger)contentSettingsType { return _contentSettingsType; }
- (void)setContentSettingsType:(NSUInteger)value {
  CHECK_LT(value, kNativeContentTypes.size());
  _contentSettingsType = value;
}
- (NSUInteger)siteSetting { return _siteSetting; }
- (void)setSiteSetting:(NSUInteger)value { CHECK_LE(value, 4u); _siteSetting = value; }
- (NSInteger)permissionType { return kPermissionTypes[_contentSettingsType]; }
@end

@implementation ArcBrowserContext (ProfileServices)
- (ArcSiteSettingInfo*)getContentSettingForOrigin:(NSURL*)origin
                         withContentSettingType:(NSUInteger)type {
  auto native = NativeType(type);
  auto* map = Settings(self);
  GURL url = ArcGURL(origin);
  if (!native || !map || !url.is_valid()) return nil;
  ContentSetting setting = ExplicitSetting(map, url, *native);
  SiteSettingRecord record{OriginString(origin), type,
                          setting >= 0 && setting <= 4 ? static_cast<NSUInteger>(setting) : 0};
  return [[ArcSiteSettingInfo alloc] initWithSiteSettingInfo:&record];
}
- (NSArray<ArcSiteSettingInfo*>*)getChangedContentSettingsForOrigin:(NSURL*)origin {
  auto* map = Settings(self);
  GURL url = ArcGURL(origin);
  NSMutableArray* values = [NSMutableArray array];
  if (!map || !url.is_valid()) return [values copy];
  for (NSUInteger i = 0; i < kNativeContentTypes.size(); ++i) {
    ContentSetting setting = ExplicitSetting(map, url, *NativeType(i));
    if (setting == CONTENT_SETTING_DEFAULT) continue;
    SiteSettingRecord record{OriginString(origin), i,
        setting <= 4 ? static_cast<NSUInteger>(setting) : 0};
    [values addObject:[[ArcSiteSettingInfo alloc] initWithSiteSettingInfo:&record]];
  }
  return [values copy];
}
- (NSArray<ArcSiteSettingInfo*>*)getChangedPermissionsForOrigin:(NSURL*)origin {
  return [self getChangedContentSettingsForOrigin:origin];
}
- (void)setContentSettingsForOrigin:(NSURL*)origin withContentSettingTypes:(NSIndexSet*)types
                   withSiteSetting:(NSUInteger)setting {
  CHECK_LE(setting, 4u);
  auto* map = Settings(self);
  const GURL url = ArcGURL(origin);
  if (!map || !url.is_valid()) return;
  for (NSUInteger type = types.firstIndex; type != NSNotFound; type = [types indexGreaterThanIndex:type]) {
    if (auto native = NativeType(type))
      WriteSetting(map, url, *native, setting);
  }
}
- (void)setPermissionsForOrigin:(NSURL*)origin withPermissionTypes:(NSIndexSet*)types
               withSiteSetting:(NSUInteger)setting {
  CHECK_LE(setting, 4u);
  auto* map = Settings(self);
  const GURL url = ArcGURL(origin);
  if (!map || !url.is_valid()) return;
  for (NSUInteger permission = types.firstIndex; permission != NSNotFound;
       permission = [types indexGreaterThanIndex:permission]) {
    if (permission >= 28) continue;
    for (NSUInteger type = 0; type < kPermissionTypes.size(); ++type) {
      if (kPermissionTypes[type] != static_cast<NSInteger>(permission)) continue;
      WriteSetting(map, url, *NativeType(type), setting);
      break;
    }
  }
}
- (void)setUserVisibleName:(NSString*)name {
  Profile* profile = ArcProfileFrom(self);
  if (!profile) return;
  Profile* original = profile->GetOriginalProfile();
  original->GetPrefs()->SetString(prefs::kProfileName, ArcUTF8(name));
  if (auto* entry = g_browser_process->profile_manager()->GetProfileAttributesStorage()
                        .GetProfileAttributesWithPath(original->GetPath()))
    entry->SetLocalProfileName(ArcUTF16(name), false);
}
- (void)favicon:(NSURL*)url completion:(void (^)(NSImage*))completion {
  Profile* profile = ArcProfileFrom(self);
  auto* service = profile ? FaviconServiceFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS) : nullptr;
  if (!completion) return;
  if (!service) { dispatch_async(dispatch_get_main_queue(), ^{ completion(nil); }); return; }
  service->GetFaviconImageForPageURL(ArcGURL(url), base::BindOnce(^(const favicon_base::FaviconImageResult& result) {
    completion(result.image.IsEmpty() ? nil : result.image.AsNSImage());
  }), FaviconTracker(profile));
}
- (void)favicon:(NSURL*)url iconTypes:(NSInteger)types sizeInPixels:(NSInteger)size
 fallbackToHost:(BOOL)fallback completion:(void (^)(NSImage*))completion {
  Profile* profile = ArcProfileFrom(self);
  auto* service = profile ? FaviconServiceFactory::GetForProfile(profile, ServiceAccessType::EXPLICIT_ACCESS) : nullptr;
  if (!completion) return;
  if (!service) { dispatch_async(dispatch_get_main_queue(), ^{ completion(nil); }); return; }
  favicon_base::IconTypeSet nativeTypes;
  for (int i = 1; i <= 4; ++i) if ((types & (1 << (i - 1))) != 0)
    nativeTypes.insert(static_cast<favicon_base::IconType>(i));
  service->GetRawFaviconForPageURL(ArcGURL(url), nativeTypes, static_cast<int>(size), fallback,
      base::BindOnce(^(const favicon_base::FaviconRawBitmapResult& result) {
        if (!result.is_valid()) { completion(nil); return; }
        completion([[NSImage alloc] initWithData:[NSData dataWithBytes:result.bitmap_data->front()
            length:result.bitmap_data->size()]]);
      }), FaviconTracker(profile));
}
- (void)getLocalStorage:(NSURL*)origin key:(NSString*)key completion:(void (^)(NSString*))completion {
  Profile* profile = ArcProfileFrom(self);
  if (!completion) return;
  const url::Origin nativeOrigin = url::Origin::Create(ArcGURL(origin));
  if (!profile || nativeOrigin.opaque()) { dispatch_async(dispatch_get_main_queue(), ^{ completion(nil); }); return; }
  auto area = std::make_shared<mojo::Remote<blink::mojom::StorageArea>>();
  profile->GetDefaultStoragePartition()->GetLocalStorageControl()->BindStorageArea(
      blink::StorageKey::CreateFirstParty(nativeOrigin), area->BindNewPipeAndPassReceiver());
  const auto keyText = ArcUTF16(key);
  (*area)->GetAll(mojo::NullRemote(), base::BindOnce(
      ^(std::vector<blink::mojom::KeyValuePtr> values) {
        (void)area;
        for (const auto& value : values) {
          if (ArcUTF16(StorageString(value->key)) == keyText) { completion(StorageString(value->value)); return; }
        }
        completion(nil);
      }));
}
- (void)setLocalStorage:(NSURL*)origin key:(NSString*)key value:(NSString*)value completion:(void (^)(void))completion {
  Profile* profile = ArcProfileFrom(self);
  const url::Origin nativeOrigin = url::Origin::Create(ArcGURL(origin));
  if (!profile || nativeOrigin.opaque()) { if (completion) dispatch_async(dispatch_get_main_queue(), completion); return; }
  auto area = std::make_shared<mojo::Remote<blink::mojom::StorageArea>>();
  profile->GetDefaultStoragePartition()->GetLocalStorageControl()->BindStorageArea(
      blink::StorageKey::CreateFirstParty(nativeOrigin), area->BindNewPipeAndPassReceiver());
  const auto nativeKey = StorageBytes(key);
  const auto nativeValue = StorageBytes(value);
  // Include the old value so renderer StorageArea observers receive correct deltas.
  const auto keyText = ArcUTF16(key);
  (*area)->GetAll(mojo::NullRemote(), base::BindOnce(
      ^(std::vector<blink::mojom::KeyValuePtr> values) {
        std::optional<std::vector<uint8_t>> old;
        for (const auto& entry : values) if (ArcUTF16(StorageString(entry->key)) == keyText) { old = entry->value; break; }
        (*area)->Put(nativeKey, nativeValue, old, nullptr, base::BindOnce(^(bool success) {
          (void)area;
          if (completion) completion();
        }));
      }));
}
@end
