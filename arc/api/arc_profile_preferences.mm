// Copyright 2026 ArcMod. Per-profile preferences backed by Chromium services.
#import "arc/api/arc_internal.h"

#include <algorithm>
#include <array>
#include <vector>

#include "base/functional/bind.h"
#include "base/logging.h"
#include "base/scoped_observation.h"
#include "base/supports_user_data.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/download/download_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_observer.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/webdata_services/web_data_service_factory.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/template_url.h"
#include "components/search_engines/template_url_service.h"
#include "components/search_engines/template_url_prepopulate_data.h"
#include "components/search_engines/template_url_service_observer.h"
#include "components/search_engines/keyword_web_data_service.h"
#include "components/webdata/common/web_data_service_consumer.h"

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
@property(nonatomic) BOOL promptForDownloadPath;
@property(nonatomic, strong) NSURL* downloadFilePath;
@property(nonatomic, readonly) NSURL* historyDatabaseFilePath;
@property(nonatomic) BOOL searchSuggestEnabled;
@property(nonatomic, readonly) BOOL canEnablePasswordSaving;
@property(nonatomic) BOOL passwordSavingEnabled;
@property(nonatomic) BOOL notificationPermissionRequestsEnabled;
@property(nonatomic, readonly) BOOL canEnableCreditCardsSaving;
@property(nonatomic) BOOL creditCardsSavingEnabled;
@property(nonatomic, readonly, copy) NSArray* autoOpenFileExtensions;
@property(nonatomic) BOOL keywordSearchSpaceTriggeringEnabled;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
@end

namespace {
struct SearchStartupState final : base::SupportsUserData::Data {
  unsigned observers = 0;
  bool ready = false;
  std::vector<base::OnceClosure> waiting;
};
constexpr char kSearchStartupKey[] = "ArcProfileSearchStartup";
SearchStartupState& SearchStartup(Profile* profile) {
  auto* state = static_cast<SearchStartupState*>(profile->GetUserData(kSearchStartupKey));
  if (!state) {
    auto owned = std::make_unique<SearchStartupState>();
    state = owned.get();
    profile->SetUserData(kSearchStartupKey, std::move(owned));
  }
  return *state;
}
void ReleaseSearchStartup(Profile* profile) {
  auto& state = SearchStartup(profile);
  state.ready = true;
  auto waiting = std::move(state.waiting);
  for (auto& callback : waiting)
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, std::move(callback));
}
struct SearchProvisioningMarker final : base::SupportsUserData::Data {};
constexpr char kSearchProvisioningKey[] = "ArcProfileSearchProvisioning";
// Materialize the URL displayed by Arc (including regional parameters), while
// the native service retains its substitution-aware template.
TemplateURLData DisplaySearchData(const TemplateURL* engine, TemplateURLService* service) {
  TemplateURLData data = engine->data();
  if (engine->url_ref().SupportsReplacement(service->search_terms_data())) {
    data.SetURL(GURL(engine->url_ref().ReplaceSearchTerms(
        TemplateURLRef::SearchTermsArgs(u"{searchTerms}"), service->search_terms_data())).spec());
  }
  return data;
}
void EnsureArcSearchEngines(Profile* profile, TemplateURLService* service) {
  // Arc exposes Perplexity alongside the regional Chromium prepopulation. Use
  // Chromium's built-in definition and persisted TemplateURL rather than a
  // synthetic UI-only option; existing/user-edited definitions win.
  if (!service->loaded()) return;
  Profile* original = profile->GetOriginalProfile();
  if (original->GetUserData(kSearchProvisioningKey)) return;
  original->SetUserData(kSearchProvisioningKey, std::make_unique<SearchProvisioningMarker>());
  // OTR contexts share this native service. Provision once per original profile,
  // so a newly opened incognito context cannot restore a user-deleted engine.
  if (!original->IsNewProfile() || service->GetTemplateURLForKeyword(u"perplexity.ai")) return;
  for (const TemplateURL* engine : service->GetTemplateURLs()) {
    if (engine->prepopulate_id() == 117) return;
  }
  auto data = TemplateURLPrepopulateData::GetPrepopulatedEngineFromFullList(
      *profile->GetPrefs(), {}, {}, 117);
  if (data) service->Add(std::make_unique<TemplateURL>(*data));
}
int SearchOrder(const TemplateURL* engine) {
  constexpr std::array<int, 6> ids = {1, 92, 109, 3, 101, 117};
  for (int i = 0; i < 6; ++i) if (engine->prepopulate_id() == ids[i]) return i;
  if (engine->prepopulate_id()) return 6;
  if (engine->data().starter_pack_id) return 6 + engine->data().starter_pack_id;
  return 9;
}
constexpr const char* kObservedPrefs[] = {
    "download.prompt_for_download", "download.default_directory",
    "download.extensions_to_open", "search.suggest_enabled",
    "credentials_enable_service", "autofill.credit_card_enabled",
    "omnibox.keyword_space_triggering_enabled"};

class PreferencesObserver final : public ProfileObserver,
                                  public TemplateURLServiceObserver,
                                  public WebDataServiceConsumer,
                                  public std::enable_shared_from_this<PreferencesObserver> {
 public:
  PreferencesObserver(Profile* profile, base::RepeatingClosure changed,
                      base::RepeatingClosure search_changed)
      : profile_(profile->GetWeakPtr()), changed_(std::move(changed)), search_changed_(std::move(search_changed)) {
    ++SearchStartup(profile).observers;
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
    }
  }
  ~PreferencesObserver() override {
    if (profile_) {
      auto& startup = SearchStartup(profile_.get());
      if (--startup.observers == 0 && !startup.ready) ReleaseSearchStartup(profile_.get());
    }
  }
  void Start() {
    if (auto* service = search_observation_.GetSource()) {
      service->Load();
      if (service->loaded()) ScheduleSearchUpdate();
    } else if (profile_) {
      ReleaseSearchStartup(profile_.get());
    }
  }
  void OnProfileWillBeDestroyed(Profile* profile) override {
    profile_.reset();
    registrar_.Reset();
    search_observation_.Reset();
    profile_observation_.Reset();
  }
  void OnTemplateURLServiceChanged() override {
    if (!seeding_) ScheduleSearchUpdate();
  }
  void OnTemplateURLServiceShuttingDown() override {
    search_observation_.Reset();
    if (profile_) ReleaseSearchStartup(profile_.get());
  }
  void OnWebDataServiceRequestDone(WebDataServiceBase::Handle handle,
                                 std::unique_ptr<WDTypedResult> result) override {
    // The native read is also the startup persistence barrier. It does not
    // replace the in-memory search model or generate another app notification.
    VLOG(1) << "Arc profile startup keyword read succeeded: " << bool(result)
            << "; rows: " << (result && result->GetType() == KEYWORDS_RESULT
                    ? static_cast<WDResult<WDKeywordsResult>*>(result.get())->GetValue().keywords.size()
                    : 0);
  }
 private:
  base::WeakPtr<Profile> profile_;
  bool seeding_ = false;
  bool search_update_pending_ = false;
  bool startup_keywords_flushed_ = false;
  void ScheduleSearchUpdate() {
    if (search_update_pending_) return;
    search_update_pending_ = true;
    std::weak_ptr<PreferencesObserver> weak = weak_from_this();
    dispatch_async(dispatch_get_main_queue(), ^{
      auto alive = weak.lock();
      if (!alive || !alive->profile_ || !alive->search_observation_.GetSource()) return;
      alive->search_update_pending_ = false;
      // TemplateURLService's observer list forbids nested notifications. Seed
      // only after the originating notification has unwound, and coalesce the
      // seed's own notification into this one app update.
      const bool loaded = alive->search_observation_.GetSource()->loaded();
      if (loaded) {
        alive->seeding_ = true;
        EnsureArcSearchEngines(alive->profile_.get(), alive->search_observation_.GetSource());
        alive->seeding_ = false;
      }
      if (!alive->startup_keywords_flushed_ && loaded) {
        alive->startup_keywords_flushed_ = true;
        if (auto keywords = WebDataServiceFactory::GetKeywordWebDataForProfile(
                alive->profile_.get(), ServiceAccessType::EXPLICIT_ACCESS)) {
          // Chromium delays keyword inserts for five seconds but writes the
          // builtin-version metadata immediately. A quick host shutdown can
          // interrupt that batch. GetKeywords commits pending inserts before
          // its read; WebDataServiceConsumer cancels delivery after teardown.
          keywords->GetKeywords(alive.get());
        }
      }
      alive->search_changed_.Run();
      if (alive->profile_ && loaded) ReleaseSearchStartup(alive->profile_.get());
    });
  }
  void Changed() {
    auto keep_alive = shared_from_this();
    changed_.Run();
  }
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

// Internal to the profile work package. Keep the native initial search update
// ahead of the download snapshot, as every recorded startup does. A caller that
// never requested preferences has no preference notification to wait for.
namespace arc_profile {
void AfterInitialSearchUpdate(Profile* profile, base::OnceClosure callback) {
  auto& startup = SearchStartup(profile);
  if (startup.observers && !startup.ready) startup.waiting.push_back(std::move(callback));
  else base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, std::move(callback));
}
}  // namespace arc_profile

@implementation ArcBrowserContextPreferences {
  __weak ArcBrowserContext* _owner;
  NSHashTable<id<ArcProfilePreferencesObserver>>* _observers;
  std::shared_ptr<PreferencesObserver> _nativeObserver;
}
@synthesize owner = _owner;
@dynamic downloadFilePath, historyDatabaseFilePath;
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
    _nativeObserver = std::make_shared<PreferencesObserver>(profile,
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
    _nativeObserver->Start();
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
  return engine ? [[ArcSearchEngine alloc] initWithTemplateURLData:DisplaySearchData(engine, service)] : nil;
}
- (NSArray<ArcSearchEngine*>*)getSearchEngines {
  Profile* profile = ArcProfileFrom(_owner);
  TemplateURLService* service = profile ? TemplateURLServiceFactory::GetForProfile(profile) : nullptr;
  NSMutableArray* engines = [NSMutableArray array];
  if (service) {
    auto native = service->GetTemplateURLs();
    std::stable_sort(native.begin(), native.end(), [](const auto& a, const auto& b) {
      return SearchOrder(a) < SearchOrder(b);
    });
    for (const TemplateURL* engine : native) {
      // Arc supports the bookmarks/history starter packs at this boundary.
      if (engine->data().starter_pack_id > 2) continue;
      [engines addObject:[[ArcSearchEngine alloc] initWithTemplateURLData:DisplaySearchData(engine, service)]];
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
