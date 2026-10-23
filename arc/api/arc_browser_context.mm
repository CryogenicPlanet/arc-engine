// Copyright 2026 ArcMod. Profiles ("browser contexts" in Arc's interface) on Chrome's ProfileManager:
// ArcColor, ArcBrowserContextAttributes, ArcBrowserContextID, ArcBrowserContext and the profile queries
// on ArcBrowserApplication. Unimplemented methods fall back to the stubs in arc_api_stubs.mm.

#import "arc/api/arc_internal.h"

#include "base/files/file_path.h"
#include "base/supports_user_data.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/sys_string_conversions.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/ui/profiles/profile_colors_util.h"
#include "third_party/skia/include/core/SkColor.h"

namespace {
// Arc's context types: 0 is the regular profile, 1 its off-the-record profile.
constexpr NSUInteger kBrowserContextTypeRegular = 0;
constexpr NSUInteger kBrowserContextTypeIncognito = 1;
}  // namespace

@implementation ArcColor {
  uint8_t _r, _g, _b, _a;
}
- (instancetype)initWithR:(uint8_t)r g:(uint8_t)g b:(uint8_t)b a:(uint8_t)a {
  if ((self = [super init])) {
    _r = r;
    _g = g;
    _b = b;
    _a = a;
  }
  return self;
}
- (uint8_t)r {
  return _r;
}
- (uint8_t)g {
  return _g;
}
- (uint8_t)b {
  return _b;
}
- (uint8_t)a {
  return _a;
}
+ (instancetype)colorWithSkColor:(SkColor)color {
  return [[self alloc] initWithR:SkColorGetR(color) g:SkColorGetG(color) b:SkColorGetB(color) a:SkColorGetA(color)];
}
- (SkColor)skColor {
  return SkColorSetARGB(_a, _r, _g, _b);
}
@end

ARC_EXPORT @interface ArcBrowserContextAttributes : NSObject
- (instancetype)initWithEntry:(ProfileAttributesEntry*)entry;
@end

@implementation ArcBrowserContextAttributes {
  NSString* _directoryBasename;
  NSString* _userVisibleName;
  ArcColor* _userColor;
}
- (instancetype)initWithEntry:(ProfileAttributesEntry*)entry {
  if ((self = [super init])) {
    _directoryBasename = base::SysUTF8ToNSString(entry->GetPath().BaseName().value());
    _userVisibleName = base::SysUTF16ToNSString(entry->GetLocalProfileName());
    _userColor = [ArcColor colorWithSkColor:entry->GetProfileThemeColors().profile_highlight_color];
  }
  return self;
}
- (NSString*)directoryBasename {
  return _directoryBasename;
}
- (NSString*)userVisibleName {
  return _userVisibleName;
}
- (ArcColor*)userColor {
  return _userColor;
}
@end

@implementation ArcBrowserContextID {
  NSString* _directoryBasename;
  BOOL _isIncognito;
}
- (instancetype)initWithDirectoryBasename:(NSString*)basename isIncognito:(BOOL)isIncognito {
  if ((self = [super init])) {
    _directoryBasename = [basename copy];
    _isIncognito = isIncognito;
  }
  return self;
}
- (NSString*)directoryBasename {
  return _directoryBasename;
}
- (BOOL)isIncognito {
  return _isIncognito;
}
- (BOOL)isEqual:(id)other {
  if (![other isKindOfClass:[ArcBrowserContextID class]]) {
    return NO;
  }
  ArcBrowserContextID* o = other;
  return _isIncognito == o->_isIncognito && [_directoryBasename isEqualToString:o->_directoryBasename];
}
- (NSUInteger)hash {
  return _directoryBasename.hash ^ (_isIncognito ? 1 : 0);
}
@end

namespace {

// Owns the profile's one ArcBrowserContext wrapper.
struct ArcBrowserContextHolder : public base::SupportsUserData::Data {
  ArcBrowserContext* __strong wrapper;
};
const char kArcBrowserContextKey[] = "ArcBrowserContext";

}  // namespace

@interface ArcBrowserContext ()
- (instancetype)initWithProfile:(Profile*)profile;
@end

@implementation ArcBrowserContext {
  base::WeakPtr<Profile> _profile;
  ArcBrowserContextID* _id;
  __weak id _delegate;
  NSMutableDictionary<NSString*, id>* _services;
}

// Per-profile service wrappers (preferences, history, ...). Until a wrapper is implemented, this is an
// instance of its shell class, whose methods are logging stubs; the app needs a non-nil object.
- (id)serviceNamed:(NSString*)className {
  if (!_services) {
    _services = [NSMutableDictionary dictionary];
  }
  id service = _services[className];
  if (!service) {
    service = [[NSClassFromString(className) alloc] init];
    if (service) {
      _services[className] = service;
    }
  }
  return service;
}
- (id)preferences {
  return [self serviceNamed:@"ArcBrowserContextPreferences"];
}
- (id)history {
  return [self serviceNamed:@"ArcBrowserContextHistory"];
}
- (id)downloads {
  return [self serviceNamed:@"ArcBrowserContextDownloads"];
}
- (id)extensions {
  return [self serviceNamed:@"ArcBrowserContextExtensions"];
}
- (id)effects {
  return [self serviceNamed:@"ArcBrowserContextEffects"];
}
- (id)pageContentExtraction {
  return [self serviceNamed:@"ArcBrowserContextPageContentExtraction"];
}
+ (instancetype)wrapperForProfile:(Profile*)profile {
  if (!profile) {
    return nil;
  }
  auto* holder = static_cast<ArcBrowserContextHolder*>(profile->GetUserData(kArcBrowserContextKey));
  if (!holder) {
    auto owned = std::make_unique<ArcBrowserContextHolder>();
    owned->wrapper = [[ArcBrowserContext alloc] initWithProfile:profile];
    holder = owned.get();
    profile->SetUserData(kArcBrowserContextKey, std::move(owned));
  }
  return holder->wrapper;
}

- (Profile*)profile {
  return _profile.get();
}

- (instancetype)initWithProfile:(Profile*)profile {
  if ((self = [super init])) {
    _profile = profile->GetWeakPtr();
    _id = [[ArcBrowserContextID alloc]
        initWithDirectoryBasename:base::SysUTF8ToNSString(profile->GetPath().BaseName().value())
                      isIncognito:profile->IsOffTheRecord()];
  }
  return self;
}
- (ArcBrowserContextID*)id {
  return _id;
}
- (void)setDelegate:(id)delegate {
  _delegate = delegate;
}
- (NSString*)userVisibleName {
  ProfileAttributesEntry* entry =
      _profile ? g_browser_process->profile_manager()->GetProfileAttributesStorage().GetProfileAttributesWithPath(
                     _profile->GetOriginalProfile()->GetPath())
               : nullptr;
  return entry ? base::SysUTF16ToNSString(entry->GetLocalProfileName()) : @"";
}
- (BOOL)isEqual:(id)other {
  return [other isKindOfClass:[ArcBrowserContext class]] && [_id isEqual:((ArcBrowserContext*)other)->_id];
}
- (NSUInteger)hash {
  return _id.hash;
}
@end

@interface ArcBrowserApplication : NSObject
@end

@implementation ArcBrowserApplication (Profiles)

- (NSArray<ArcBrowserContextAttributes*>*)availableBrowserContextAttributes {
  NSMutableArray* result = [NSMutableArray array];
  for (ProfileAttributesEntry* entry :
       g_browser_process->profile_manager()->GetProfileAttributesStorage().GetAllProfilesAttributes()) {
    [result addObject:[[ArcBrowserContextAttributes alloc] initWithEntry:entry]];
  }
  return result;
}

// Returns the loaded profile in the user data dir named |name| ("Default", "Profile 1", ...), or its
// off-the-record profile for the incognito type. nil if it isn't loaded.
- (ArcBrowserContext*)getBrowserContext:(NSString*)name type:(NSUInteger)type {
  if (type != kBrowserContextTypeRegular && type != kBrowserContextTypeIncognito) {
    return nil;
  }
  ProfileManager* manager = g_browser_process->profile_manager();
  Profile* profile =
      manager->GetProfileByPath(manager->user_data_dir().Append(base::SysNSStringToUTF8(name)));
  if (!profile) {
    return nil;
  }
  if (type == kBrowserContextTypeIncognito) {
    profile = profile->GetPrimaryOTRProfile(/*create_if_needed=*/true);
  }
  return [ArcBrowserContext wrapperForProfile:profile];
}

@end
