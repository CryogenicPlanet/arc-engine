// Copyright 2026 ArcMod. Per-profile preferences backed by Chromium services.
#import "arc/api/arc_internal.h"

#include "base/functional/bind.h"
#include "base/scoped_observation.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/download/download_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_observer.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/template_url.h"
#include "components/search_engines/template_url_service.h"
#include "components/search_engines/template_url_service_observer.h"

// arc_values.mm owns this primary implementation.
@interface ArcSearchEngine : NSObject
- (instancetype)initWithTemplateURLData:(const TemplateURLData&)data;
- (const TemplateURLData&)templateURLData;
@end

@protocol ArcProfilePreferencesObserver <NSObject>
@optional
- (void)browserContextPreferencesDidUpdate:(ArcBrowserContext*)context;
- (void)browserContextPreferencesDidUpdateSearchEngines:(ArcBrowserContext*)context;
@end

ARC_EXPORT @interface ArcBrowserContextPreferences : NSObject
@property(nonatomic, weak) ArcBrowserContext* owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@end

namespace {
constexpr const char* kObservedPrefs[] = {
    "download.prompt_for_download", "download.default_directory",
    "download.extensions_to_open", "search.suggest_enabled",
    "credentials_enable_service", "autofill.credit_card_enabled",
    "omnibox.keyword_space_triggering_enabled"};

class PreferencesObserver final : public ProfileObserver,
                                  public TemplateURLServiceObserver {
 public:
  PreferencesObserver(Profile* profile, base::RepeatingClosure changed,
                      base::RepeatingClosure search_changed)
      : changed_(std::move(changed)), search_changed_(std::move(search_changed)) {
    profile_observation_.Observe(profile);
    registrar_.Init(profile->GetPrefs());
    for (const char* key : kObservedPrefs) {
      if (profile->GetPrefs()->FindPreference(key)) {
        registrar_.Add(key, base::BindRepeating(&PreferencesObserver::Changed,
                                               base::Unretained(this)));
      }
    }
    if (auto* service = TemplateURLServiceFactory::GetForProfile(profile)) {
      search_observation_.Observe(service);
      service->Load();
    }
  }
  void OnProfileWillBeDestroyed(Profile* profile) override {
    registrar_.Reset();
    search_observation_.Reset();
    profile_observation_.Reset();
  }
  void OnTemplateURLServiceChanged() override {
    search_changed_.Run();
  }
  void OnTemplateURLServiceShuttingDown() override {
    search_observation_.Reset();
  }
 private:
  void Changed() { changed_.Run(); }
  base::RepeatingClosure changed_;
  base::RepeatingClosure search_changed_;
  PrefChangeRegistrar registrar_;
  base::ScopedObservation<Profile, ProfileObserver> profile_observation_{this};
  base::ScopedObservation<TemplateURLService, TemplateURLServiceObserver>
      search_observation_{this};
};

BOOL ReadBoolean(ArcBrowserContext* context, const char* key) {
  Profile* profile = ArcProfileFrom(context);
  return profile && profile->GetPrefs()->FindPreference(key) && profile->GetPrefs()->GetBoolean(key);
}
void WriteBoolean(ArcBrowserContext* context, const char* key, BOOL value) {
  if (Profile* profile = ArcProfileFrom(context)) {
    if (profile->GetPrefs()->FindPreference(key)) { profile->GetPrefs()->SetBoolean(key, value); }
  }
}
BOOL CanModify(ArcBrowserContext* context, const char* key) {
  Profile* profile = ArcProfileFrom(context);
  return profile && profile->GetPrefs()->IsUserModifiablePreference(key);
}
}  // namespace

@implementation ArcBrowserContextPreferences {
  __weak ArcBrowserContext* _owner;
  NSHashTable<id<ArcProfilePreferencesObserver>>* _observers;
  std::unique_ptr<PreferencesObserver> _nativeObserver;
}
- (instancetype)init {
  if ((self = [super init])) {
    _observers = [NSHashTable weakObjectsHashTable];
  }
  return self;
}
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  if ((self = [self init])) {
    self.owner = context;
  }
  return self;
}
- (instancetype)initWithBrowserContextPreferences:(const void*)unused
                               forBrowserContext:(ArcBrowserContext*)context {
  return [self initWithArcBrowserContext:context];
}
- (ArcBrowserContext*)owner { return _owner; }
- (void)setOwner:(ArcBrowserContext*)owner {
  if (_owner == owner) {
    return;
  }
  _nativeObserver.reset();
  _owner = owner;
  if (Profile* profile = ArcProfileFrom(owner)) {
    __weak ArcBrowserContextPreferences* weakSelf = self;
    _nativeObserver = std::make_unique<PreferencesObserver>(profile,
        base::BindRepeating(^{
          ArcBrowserContextPreferences* wrapper = weakSelf;
          if (!wrapper) { return; }
          ArcBrowserContext* context = wrapper->_owner;
          for (id<ArcProfilePreferencesObserver> observer in wrapper->_observers.allObjects) {
            if ([observer respondsToSelector:@selector(browserContextPreferencesDidUpdate:)]) {
              [observer browserContextPreferencesDidUpdate:context];
            }
          }
        }), base::BindRepeating(^{
          ArcBrowserContextPreferences* wrapper = weakSelf;
          if (!wrapper) { return; }
          ArcBrowserContext* context = wrapper->_owner;
          for (id<ArcProfilePreferencesObserver> observer in wrapper->_observers.allObjects) {
            if ([observer respondsToSelector:@selector(browserContextPreferencesDidUpdateSearchEngines:)]) {
              [observer browserContextPreferencesDidUpdateSearchEngines:context];
            }
          }
        }));
  }
}
- (void)addObserver:(id<ArcProfilePreferencesObserver>)observer {
  // Arc's bridge reads the owner during observer registration.
  (void)[self owner];
  if (observer) {
    [_observers addObject:observer];
  }
}
- (void)removeObserver:(id)observer { [_observers removeObject:observer]; }
- (BOOL)searchSuggestEnabled { return ReadBoolean(_owner, "search.suggest_enabled"); }
- (void)setSearchSuggestEnabled:(BOOL)value {
  WriteBoolean(_owner, "search.suggest_enabled", value);
}
- (BOOL)promptForDownloadPath {
  Profile* profile = ArcProfileFrom(_owner);
  return profile && DownloadPrefs::FromBrowserContext(profile)->PromptForDownload();
}
- (void)setPromptForDownloadPath:(BOOL)value {
  WriteBoolean(_owner, "download.prompt_for_download", value);
}
- (BOOL)passwordSavingEnabled { return ReadBoolean(_owner, "credentials_enable_service"); }
- (void)setPasswordSavingEnabled:(BOOL)value {
  WriteBoolean(_owner, "credentials_enable_service", value);
}
- (BOOL)canEnablePasswordSaving { return CanModify(_owner, "credentials_enable_service"); }
- (BOOL)creditCardsSavingEnabled { return ReadBoolean(_owner, "autofill.credit_card_enabled"); }
- (void)setCreditCardsSavingEnabled:(BOOL)value {
  WriteBoolean(_owner, "autofill.credit_card_enabled", value);
}
- (BOOL)canEnableCreditCardsSaving { return CanModify(_owner, "autofill.credit_card_enabled"); }
- (BOOL)keywordSearchSpaceTriggeringEnabled {
  return ReadBoolean(_owner, "omnibox.keyword_space_triggering_enabled");
}
- (void)setKeywordSearchSpaceTriggeringEnabled:(BOOL)value {
  WriteBoolean(_owner, "omnibox.keyword_space_triggering_enabled", value);
}
- (BOOL)notificationPermissionRequestsEnabled {
  Profile* profile = ArcProfileFrom(_owner);
  return profile && HostContentSettingsMapFactory::GetForProfile(profile)
                            ->GetDefaultContentSetting(ContentSettingsType::NOTIFICATIONS, nullptr) !=
                        CONTENT_SETTING_BLOCK;
}
- (void)setNotificationPermissionRequestsEnabled:(BOOL)value {
  if (Profile* profile = ArcProfileFrom(_owner)) {
    HostContentSettingsMapFactory::GetForProfile(profile)->SetDefaultContentSetting(
        ContentSettingsType::NOTIFICATIONS, value ? CONTENT_SETTING_ASK : CONTENT_SETTING_BLOCK);
  }
}
- (NSURL*)downloadFilePath {
  Profile* profile = ArcProfileFrom(_owner);
  return profile ? [NSURL fileURLWithPath:ArcNSString(
      DownloadPrefs::FromBrowserContext(profile)->DownloadPath().value()) isDirectory:YES] : nil;
}
- (void)setDownloadFilePath:(NSURL*)url {
  if (Profile* profile = ArcProfileFrom(_owner); profile && url.isFileURL) {
    DownloadPrefs::FromBrowserContext(profile)->SetDownloadPath(base::FilePath(ArcUTF8(url.path)));
  }
}
- (void)clearAutoOpenFiles {
  if (Profile* profile = ArcProfileFrom(_owner)) {
    DownloadPrefs::FromBrowserContext(profile)->ResetAutoOpenByUser();
  }
}
- (NSArray<NSString*>*)autoOpenFileExtensions {
  Profile* profile = ArcProfileFrom(_owner);
  if (!profile) {
    return @[];
  }
  NSString* extensions = ArcNSString(profile->GetPrefs()->GetString("download.extensions_to_open"));
  return extensions.length ? [extensions componentsSeparatedByString:@":"] : @[];
}
- (NSURL*)historyDatabaseFilePath {
  Profile* profile = ArcProfileFrom(_owner);
  return profile ? [NSURL fileURLWithPath:ArcNSString(profile->GetPath().AppendASCII("History").value())] : nil;
}
- (ArcSearchEngine*)getDefaultSearchEngine {
  Profile* profile = ArcProfileFrom(_owner);
  TemplateURLService* service = profile ? TemplateURLServiceFactory::GetForProfile(profile) : nullptr;
  const TemplateURL* engine = service ? service->GetDefaultSearchProvider() : nullptr;
  return engine ? [[ArcSearchEngine alloc] initWithTemplateURLData:engine->data()] : nil;
}
- (NSArray<ArcSearchEngine*>*)getSearchEngines {
  Profile* profile = ArcProfileFrom(_owner);
  TemplateURLService* service = profile ? TemplateURLServiceFactory::GetForProfile(profile) : nullptr;
  NSMutableArray* engines = [NSMutableArray array];
  if (service) {
    for (const TemplateURL* engine : service->GetTemplateURLs()) {
      [engines addObject:[[ArcSearchEngine alloc] initWithTemplateURLData:engine->data()]];
    }
  }
  return [engines copy];
}
- (BOOL)setDefaultSearchEngine:(ArcSearchEngine*)engine {
  Profile* profile = ArcProfileFrom(_owner);
  if (!profile || ![engine isKindOfClass:[ArcSearchEngine class]]) {
    return NO;
  }
  auto* service = TemplateURLServiceFactory::GetForProfile(profile);
  TemplateURL* native = service ? service->GetTemplateURLForKeyword([engine templateURLData].keyword()) : nullptr;
  if (!native || !service->CanMakeDefault(native)) {
    return NO;
  }
  service->SetUserSelectedDefaultSearchProvider(native);
  return YES;
}
@end
