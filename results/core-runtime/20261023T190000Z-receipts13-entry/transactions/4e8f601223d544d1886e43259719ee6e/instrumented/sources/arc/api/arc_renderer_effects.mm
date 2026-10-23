// Copyright 2026 ArcMod. Profile-owned Boosts and native zoom registration.
#import "arc/api/arc_internal.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>

#include "arc/common/effects_browser.h"
#include "base/functional/bind.h"
#include "base/location.h"
#include "base/no_destructor.h"
#include "base/supports_user_data.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/host_zoom_map.h"

ARC_EXPORT @interface ArcBrowserContextEffects : NSObject
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context;
- (instancetype)initWithBrowserContextEffects:(const void*)service;
- (instancetype)initWithBrowserContextEffects:(const void*)service
                            forBrowserContext:(ArcBrowserContext*)context;
@property(nonatomic, weak) ArcBrowserContext* owner;
@end

namespace arc {
namespace {
class ProfileEffectsData final : public base::SupportsUserData::Data {
 public:
  static ProfileEffectsData* For(Profile* profile) {
    auto* data = static_cast<ProfileEffectsData*>(profile->GetUserData(&kKey));
    if (!data) {
      auto owned = std::make_unique<ProfileEffectsData>();
      data = owned.get();
      profile->SetUserData(&kKey, std::move(owned));
    }
    return data;
  }

  const scoped_refptr<EffectsServiceIdentity> identity =
      base::MakeRefCounted<EffectsServiceIdentity>(uint8_t{0});
  ProfileEffectsConfiguration configuration;
  base::RepeatingClosureList observers;

 private:
  inline static const int kKey = 0;
};

void WithEffectsProfile(base::WeakPtr<Profile> profile,
                        base::OnceCallback<void(Profile*)> action) {
  if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&WithEffectsProfile, std::move(profile), std::move(action)));
    return;
  }
  if (profile) {
    std::move(action).Run(profile.get());
  }
}

void NotifyEffectsChanged(Profile* profile) {
  // Original NotifyBoostConfigChanged posts before notifying registered agents.
  content::GetUIThreadTaskRunner({})->PostTask(
      FROM_HERE, base::BindOnce([](base::WeakPtr<Profile> weak) {
        if (weak) {
          ProfileEffectsData::For(weak.get())->observers.Notify();
        }
      }, profile->GetWeakPtr()));
}

void RegisterSource(bool script,
                    std::string source,
                    std::string identifier,
                    std::string host,
                    Profile* profile) {
  auto& rule = ProfileEffectsData::For(profile)->configuration[host];
  auto& sources = script ? rule.scripts : rule.stylesheets;
  const auto found = std::ranges::find(sources, identifier, &EffectSource::identifier);
  if (found == sources.end()) {
    sources.push_back({std::move(identifier), std::move(source)});
  } else {
    // Replacing an identifier preserves its insertion/execution position.
    found->source = std::move(source);
  }
  NotifyEffectsChanged(profile);
}

void UnregisterSource(bool script, std::string identifier, Profile* profile) {
  auto& configuration = ProfileEffectsData::For(profile)->configuration;
  for (auto host = configuration.begin(); host != configuration.end(); ++host) {
    auto& sources = script ? host->second.scripts : host->second.stylesheets;
    const auto found = std::ranges::find(sources, identifier, &EffectSource::identifier);
    if (found == sources.end()) {
      continue;
    }
    sources.erase(found);
    // Original 0x42e2384 tests only the remaining CSS size and the color-filter
    // optional. For our CSS/JS-only entries it drops a host with no CSS, even
    // if other scripts remain. It returns after the first sorted-host match.
    if (host->second.stylesheets.empty()) {
      configuration.erase(host);
    }
    NotifyEffectsChanged(profile);
    return;
  }
}
}  // namespace

const ProfileEffectsConfiguration& GetProfileEffectsConfiguration(Profile* profile) {
  if (!profile) {
    static const base::NoDestructor<ProfileEffectsConfiguration> empty;
    return *empty;
  }
  return ProfileEffectsData::For(profile)->configuration;
}

base::CallbackListSubscription ObserveProfileEffectsConfiguration(
    Profile* profile,
    base::RepeatingClosure callback) {
  return profile ? ProfileEffectsData::For(profile)->observers.Add(std::move(callback))
                 : base::CallbackListSubscription();
}
}  // namespace arc

@implementation ArcBrowserContextEffects {
  base::WeakPtr<Profile> _profile;
  scoped_refptr<arc::EffectsServiceIdentity> _serviceIdentity;
}
@synthesize owner = _owner;
- (instancetype)initWithArcBrowserContext:(ArcBrowserContext*)context {
  arc::EffectsServiceData native;
  Profile* profile = [context profile];
  if (profile) {
    native.profile = profile->GetWeakPtr();
    native.identity = arc::ProfileEffectsData::For(profile)->identity;
  }
  return [self initWithBrowserContextEffects:&native forBrowserContext:context];
}
- (instancetype)initWithBrowserContextEffects:(const void*)service {
  if ((self = [super init])) {
    if (service) {
      const auto& native = *static_cast<const arc::EffectsServiceData*>(service);
      _profile = native.profile;
      _serviceIdentity = native.identity;
    }
    if (!_serviceIdentity) {
      _serviceIdentity = base::MakeRefCounted<arc::EffectsServiceIdentity>(uint8_t{0});
    }
  }
  return self;
}
- (instancetype)initWithBrowserContextEffects:(const void*)service
                            forBrowserContext:(ArcBrowserContext*)context {
  if ((self = [self initWithBrowserContextEffects:service])) {
    _owner = context;
  }
  return self;
}
- (BOOL)isEqual:(id)object {
  if (self == object) {
    return YES;
  }
  if (![object isKindOfClass:[ArcBrowserContextEffects class]]) {
    return NO;
  }
  ArcBrowserContextEffects* other = object;
  return _serviceIdentity && _serviceIdentity.get() == other->_serviceIdentity.get();
}
- (NSUInteger)hash {
  return _serviceIdentity ? reinterpret_cast<NSUInteger>(_serviceIdentity.get()) : [super hash];
}
- (void)registerStylesheet:(NSString*)stylesheet
            withIdentifier:(NSString*)identifier
            forHostPattern:(NSString*)host {
  arc::WithEffectsProfile(_profile, base::BindOnce(&arc::RegisterSource, false,
                                                 ArcUTF8(stylesheet), ArcUTF8(identifier), ArcUTF8(host)));
}
- (void)unregisterStylesheet:(NSString*)identifier {
  arc::WithEffectsProfile(_profile, base::BindOnce(&arc::UnregisterSource, false, ArcUTF8(identifier)));
}
- (void)registerScript:(NSString*)script
        withIdentifier:(NSString*)identifier
        forHostPattern:(NSString*)host {
  arc::WithEffectsProfile(_profile, base::BindOnce(&arc::RegisterSource, true,
                                                 ArcUTF8(script), ArcUTF8(identifier), ArcUTF8(host)));
}
- (void)unregisterScript:(NSString*)identifier {
  arc::WithEffectsProfile(_profile, base::BindOnce(&arc::UnregisterSource, true, ArcUTF8(identifier)));
}
- (void)registerZoomFactor:(float)factor forHostPattern:(NSString*)hostPattern {
  arc::WithEffectsProfile(_profile, base::BindOnce([](float factor, std::string host, Profile* profile) {
    if (auto* zoom = content::HostZoomMap::GetDefaultForBrowserContext(profile)) {
      zoom->SetZoomLevelForHost(host, std::log(static_cast<double>(factor)) / 0.1823215567939546);
    }
  }, factor, ArcUTF8(hostPattern)));
}
- (void)unregisterZoomFactor:(NSString*)hostPattern {
  arc::WithEffectsProfile(_profile, base::BindOnce([](std::string host, Profile* profile) {
    if (auto* zoom = content::HostZoomMap::GetDefaultForBrowserContext(profile)) {
      zoom->SetZoomLevelForHost(host, zoom->GetDefaultZoomLevel());
    }
  }, ArcUTF8(hostPattern)));
}
@end
