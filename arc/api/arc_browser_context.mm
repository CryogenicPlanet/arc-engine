// Copyright 2026 ArcMod. Profiles ("browser contexts" in Arc's interface) on Chrome's ProfileManager:
// ArcColor, ArcBrowserContextAttributes, ArcBrowserContextID, ArcBrowserContext and the profile queries
// on ArcBrowserApplication. Unimplemented methods fall back to the stubs in arc_api_stubs.mm.

#import "arc/api/arc_internal.h"
#import <objc/message.h>

#include "base/files/file_path.h"
#include "chrome/browser/profiles/profile_metrics.h"
#include "chrome/browser/profiles/delete_profile_helper.h"
#include "base/task/sequenced_task_runner.h"
#include "base/functional/callback_helpers.h"
#include "base/functional/bind.h"
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

// Arc's engine reports a profile that still has Chromium's default name ("Your Arc"/"Your Chromium") by its
// directory name ("Default" in every golden); a name set through -setUserVisibleName: is reported as set.
static NSString* ArcUserVisibleProfileName(ProfileAttributesEntry* entry) {
  return entry->IsUsingDefaultName() ? base::SysUTF8ToNSString(entry->GetPath().BaseName().value())
                                     : base::SysUTF16ToNSString(entry->GetLocalProfileName());
}

@implementation ArcBrowserContextAttributes {
  NSString* _directoryBasename;
  NSString* _userVisibleName;
  ArcColor* _userColor;
}
- (instancetype)initWithEntry:(ProfileAttributesEntry*)entry {
  if ((self = [super init])) {
    _directoryBasename = base::SysUTF8ToNSString(entry->GetPath().BaseName().value());
    _userVisibleName = ArcUserVisibleProfileName(entry);
    // Arc's engine reports the profile's color seed (#A8C7FA for a default profile in dark mode, as in the
    // goldens), not the highlight color.
    _userColor = [ArcColor colorWithSkColor:entry->GetProfileThemeColors().profile_color_seed];
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

// Shares the existing context cache without exposing another Objective-C dispatch.
id GetBrowserContextService(
    ArcBrowserContext* context,
    NSMutableDictionary<NSString*, id>* __strong& services,
    NSString* class_name) {
  if (!services) {
    services = [NSMutableDictionary dictionary];
  }
  id service = services[class_name];
  if (!service) {
    // Implemented services take their profile wrapper; shell classes (stubs) get plain init.
    Class cls = NSClassFromString(class_name);
    SEL withContext = NSSelectorFromString(@"initWithArcBrowserContext:");
    service = [cls instancesRespondToSelector:withContext]
                  ? ((id (*)(id, SEL, id))objc_msgSend)([cls alloc], withContext, context)
                  : [[cls alloc] init];
    if (service) {
      services[class_name] = service;
    }
  }
  return service;
}

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
  return GetBrowserContextService(self, _services, className);
}
- (id)preferences {
  return [self serviceNamed:@"ArcBrowserContextPreferences"];
}
- (id)history {
  return GetBrowserContextService(self, _services, @"ArcBrowserContextHistory");
}
- (id)downloads {
  return [self serviceNamed:@"ArcBrowserContextDownloads"];
}
- (id)extensions {
  return [self serviceNamed:@"ArcBrowserContextExtensions"];
}
- (id)effects {
  return GetBrowserContextService(self, _services, @"ArcBrowserContextEffects");
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
  return entry ? ArcUserVisibleProfileName(entry) : @"";
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

namespace {

// Profile directory |name| in the user data dir.
base::FilePath ArcProfilePath(NSString* name) {
  return g_browser_process->profile_manager()->user_data_dir().Append(base::SysNSStringToUTF8(name));
}

bool ArcValidProfileRequest(NSString* name, NSUInteger type, bool has_completion, NSError** error) {
  const bool valid = name.length && has_completion &&
                     (type == kBrowserContextTypeRegular || type == kBrowserContextTypeIncognito);
  if (!valid && error) {
    *error = [NSError errorWithDomain:@"company.thebrowser.Arc"
                                 code:2
                             userInfo:@{NSLocalizedDescriptionKey : @"Invalid browser context request"}];
  }
  return valid;
}

ArcBrowserContext* ArcContextForProfile(Profile* profile, NSUInteger type) {
  if (!profile) {
    return nil;
  }
  if (type == kBrowserContextTypeIncognito) {
    profile = profile->GetPrimaryOTRProfile(/*create_if_needed=*/true);
  }
  return [ArcBrowserContext wrapperForProfile:profile];
}
}  // namespace

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

// Creates a new profile whose visible name is |name|; Chrome picks its directory ("Profile N"), as on Arc's
// engine, and |completion| gets it (or nil) on the main thread. Rejects an empty name (error 2), a missing
// completion or an unknown type.
- (BOOL)createBrowserContext:(NSString*)name
                    isHidden:(BOOL)isHidden
                        type:(NSUInteger)type
                  completion:(void (^)(ArcBrowserContext*))completion
                       error:(NSError**)error {
  if (!ArcValidProfileRequest(name, type, completion != nil, error)) {
    return NO;
  }
  ProfileManager::CreateMultiProfileAsync(
      base::SysNSStringToUTF16(name),
      g_browser_process->profile_manager()->GetProfileAttributesStorage().ChooseAvatarIconIndexForNewProfile(),
      isHidden,
      base::BindOnce([](void (^done)(ArcBrowserContext*), NSUInteger profile_type,
                        Profile* profile) { done(ArcContextForProfile(profile, profile_type)); },
                     completion, type));
  return YES;
}

// Loads an existing profile directory |name| (no creation) and reports it, or nil, on the main thread.
- (void)loadBrowserContext:(NSString*)name type:(NSUInteger)type completion:(void (^)(ArcBrowserContext*))completion {
  if (!completion) {
    return;
  }
  if (!ArcValidProfileRequest(name, type, true, nullptr)) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
                                                               completion(nil);
                                                             }));
    return;
  }
  // Chrome reports a missing profile through the callback too (with null).
  g_browser_process->profile_manager()->LoadProfileByPath(
      ArcProfilePath(name), /*incognito=*/false,
      base::BindOnce([](void (^done)(ArcBrowserContext*), NSUInteger profile_type,
                        Profile* profile) { done(ArcContextForProfile(profile, profile_type)); },
                     completion, type));
}

// Schedules deletion of profile directory |name| as Chrome's profile settings do (its browsers close first).
// A name that isn't a profile directory fails with error 3, as on Arc's engine.
- (BOOL)deleteBrowserContext:(NSString*)name error:(NSError**)error {
  if (!ArcValidProfileRequest(name, kBrowserContextTypeRegular, true, error)) {
    return NO;
  }
  ProfileManager* manager = g_browser_process->profile_manager();
  const base::FilePath path = ArcProfilePath(name);
  if (!manager->GetProfileAttributesStorage().GetProfileAttributesWithPath(path)) {
    if (error) {
      *error = [NSError errorWithDomain:@"company.thebrowser.Arc"
                                   code:3
                               userInfo:@{NSLocalizedDescriptionKey : @"No browser context with that name"}];
    }
    return NO;
  }
  manager->GetDeleteProfileHelper().MaybeScheduleProfileForDeletion(
      path, base::DoNothing(), ProfileMetrics::DELETE_PROFILE_SETTINGS);
  return YES;
}

@end
