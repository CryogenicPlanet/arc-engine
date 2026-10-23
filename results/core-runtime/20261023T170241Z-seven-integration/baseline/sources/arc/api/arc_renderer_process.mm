// Copyright 2026 ArcMod. Browser registry of renderer-process adblock agents.
#import "arc/api/arc_internal.h"

#include <map>
#include <utility>

#include "arc/common/ad_block_browser.h"
#include "arc/common/ad_block_publication.h"
#include "arc/common/renderer.mojom.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/no_destructor.h"
#include "base/supports_user_data.h"
#include "base/unguessable_token.h"
#include "base/scoped_multi_source_observation.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_process_host_creation_observer.h"
#include "content/public/browser/render_process_host_observer.h"
#include "ipc/ipc_channel_proxy.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/callback_helpers.h"

namespace arc {
namespace {
constexpr int kProfileRulesKey = 0;
// Retained source state, independent of receiver-side delivery/reset state.
struct RulesSnapshot {
  ~RulesSnapshot() {
    ReleaseRendererRulesetFile(std::move(cosmetic_file));
    ReleaseRendererRulesetFile(std::move(url_file));
  }
  base::File cosmetic_file;
  base::File url_file;
  base::UnguessableToken url_region_identity;
};
struct ProfileRules final : public base::SupportsUserData::Data, RulesSnapshot {
  uint64_t exclusions_version = 0;
  std::vector<std::string> excluded_origins;
  // Native C/E tasks share a stable service-lifetime weak flag; only U cancels U.
  base::WeakPtrFactory<ProfileRules> service_weak_factory{this};
  base::WeakPtrFactory<ProfileRules> url_weak_factory{this};
};

class RendererProcessRegistry final
    : public content::RenderProcessHostCreationObserver,
      public content::RenderProcessHostObserver {
 public:
  RendererProcessRegistry() {
    subscription_ = ObserveAdBlockConfiguration(base::BindRepeating(
        &RendererProcessRegistry::ConfigurationChanged, weak_factory_.GetWeakPtr()));
    for (auto it = content::RenderProcessHost::AllHostsIterator(); !it.IsAtEnd(); it.Advance()) {
      auto* host = it.GetCurrentValue();
      if (host->IsInitializedAndNotDead()) {
        ProcessWillLaunch(host, false);
      }
    }
  }

  void InitializeProfile(Profile* profile) {
    if (!profile) {
      return;
    }
    profiles_.insert_or_assign(reinterpret_cast<uintptr_t>(profile), profile->GetWeakPtr());
    if (!profile->GetUserData(&kProfileRulesKey)) {
      profile->SetUserData(&kProfileRulesKey, std::make_unique<ProfileRules>());
      auto* rules = Rules(profile);
      const auto& configuration = GetAdBlockConfiguration();
      if (configuration.enabled) {
        // The UserData owner exists before either publication task is posted.
        if (configuration.has_ruleset_update) {
          SetProfilePair(profile, *rules);
        }
        rules->excluded_origins = configuration.excluded_origins;
        rules->exclusions_version = configuration.config_version;
      }
    }
  }

  void OnRenderProcessHostCreated(content::RenderProcessHost* host) override {
    ProcessWillLaunch(host, false);
  }

  void ProcessWillLaunch(content::RenderProcessHost* host,
                          bool early_process_agent_installed) {
    Track(host);
    auto& entry = entries_.at(host->GetID());
    if (!entry.bound_sequence) {
      // Reserve native launch order even when the process binder cannot safely
      // be requested until a frame acknowledges its installation.
      entry.bound_sequence = ++bound_sequence_;
    }
    CaptureInitialConfiguration(entry);
    entry.early_agent_known |= early_process_agent_installed;
    // The embedder launch hook precedes Created/Ready. Their fallback must not
    // register the same incarnation twice or change its original sequence.
    if (!entry.attempted && (entry.early_agent_known || entry.frame_ready)) {
      Bind(host);
    }
  }
  void RenderProcessReady(content::RenderProcessHost* host) override {
    ProcessWillLaunch(host, false);
  }
  void OnRenderProcessHostCreationFailed(
      content::RenderProcessHost* host,
      const content::ChildProcessTerminationInfo&) override {
    ForgetIncarnation(host);
  }
  void RenderProcessExited(content::RenderProcessHost* host,
                            const content::ChildProcessTerminationInfo&) override {
    ForgetIncarnation(host);
  }
  void InProcessRendererExiting(content::RenderProcessHost* host) override {
    ForgetIncarnation(host);
  }
  void RenderProcessHostDestroyed(content::RenderProcessHost* host) override {
    observations_.RemoveObservation(host);
    entries_.erase(host->GetID());
  }

  void FrameReady(content::RenderProcessHost* host) {
    if (!host || !host->IsInitializedAndNotDead()) {
      return;
    }
    ProcessWillLaunch(host, false);
    auto& entry = entries_.at(host->GetID());
    entry.frame_ready = true;
    if (!entry.remote.is_bound()) {
      if (!entry.attempted) {
        Bind(host);
      } else if (!entry.retried) {
        entry.retried = true;
        Bind(host);
      }
    }
  }

  void RequestDebug(Profile* profile, const GURL& url,
                    base::OnceCallback<void(const std::string&)> completion) {
    Entry* first = nullptr;
    for (auto& [id, entry] : entries_) {
      if (entry.profile.get() == profile && entry.bound_sequence &&
          entry.remote.is_bound() && entry.remote.is_connected() &&
          (!first || entry.bound_sequence < first->bound_sequence)) {
        first = &entry;
      }
    }
    if (!first) {
      std::move(completion).Run("No agent is bound to profile");
      return;
    }
    first->remote->GetAdBlockDebugInfo(
        url.spec(), mojo::WrapCallbackWithDefaultInvokeIfNotRun(
                        std::move(completion), std::string("No agent is bound to profile")));
  }

 private:
  struct Entry {
    base::WeakPtr<Profile> profile;
    mojo::AssociatedRemote<mojom::RenderProcessAgent> remote;
    uint64_t incarnation = 0;
    uint64_t attempt = 0;
    uint64_t bound_sequence = 0;
    bool attempted = false;
    bool acknowledged = false;
    bool early_agent_known = false;
    bool frame_ready = false;
    bool retried = false;
    bool initial_configuration_captured = false;
    bool has_exclusions = false;
    RulesSnapshot pending;
    uint64_t exclusions_version = 0;
    std::vector<std::string> excluded_origins;
  };

  void Track(content::RenderProcessHost* host) {
    if (!observations_.IsObservingSource(host)) {
      observations_.AddObservation(host);
    }
    auto* profile = Profile::FromBrowserContext(host->GetBrowserContext());
    InitializeProfile(profile);
    auto [it, inserted] = entries_.try_emplace(host->GetID());
    if (inserted) {
      it->second.profile = profile->GetWeakPtr();
      it->second.incarnation = ++incarnation_;
    }
  }

  void Bind(content::RenderProcessHost* host) {
    auto& entry = entries_.at(host->GetID());
    if ((!entry.early_agent_known && !entry.frame_ready) ||
        !host->GetChannel() || !entry.profile ||
        host->GetBrowserContext() != entry.profile.get()) {
      return;
    }
    entry.remote.reset();
    entry.attempted = true;
    entry.acknowledged = false;
    const auto id = host->GetID();
    const auto incarnation = entry.incarnation;
    const auto attempt = ++entry.attempt;
    host->GetChannel()->GetRemoteAssociatedInterface(&entry.remote);
    entry.remote.set_disconnect_handler(base::BindOnce(
        &RendererProcessRegistry::Disconnected, weak_factory_.GetWeakPtr(),
        id, incarnation, attempt));
    CaptureInitialConfiguration(entry);
    SendPendingConfiguration(entry);
    entry.remote->Ping(base::BindOnce(&RendererProcessRegistry::Bound,
                                    weak_factory_.GetWeakPtr(), id, incarnation, attempt));
  }

  void Bound(content::ChildProcessId id, uint64_t incarnation, uint64_t attempt) {
    auto it = entries_.find(id);
    if (it == entries_.end() || it->second.incarnation != incarnation ||
        it->second.attempt != attempt) {
      return;
    }
    it->second.acknowledged = true;
  }

  void Disconnected(content::ChildProcessId id, uint64_t incarnation, uint64_t attempt) {
    auto it = entries_.find(id);
    if (it == entries_.end() || it->second.incarnation != incarnation ||
        it->second.attempt != attempt) {
      return;
    }
    auto& entry = it->second;
    entry.remote.reset();
    if (entry.frame_ready && !entry.retried) {
      entry.retried = true;
      auto* host = content::RenderProcessHost::FromID(id);
      if (host && host->IsInitializedAndNotDead() &&
          host->GetBrowserContext() == entry.profile.get()) {
        Bind(host);
      }
    }
  }

  void ForgetIncarnation(content::RenderProcessHost* host) {
    const auto it = entries_.find(host->GetID());
    if (it == entries_.end()) {
      return;
    }
    auto& entry = it->second;
    entry.remote.reset();
    ResetPending(entry);
    entry.incarnation = ++incarnation_;
    entry.bound_sequence = 0;
    entry.attempted = false;
    entry.acknowledged = false;
    entry.early_agent_known = false;
    entry.frame_ready = false;
    entry.retried = false;
  }

  ProfileRules* Rules(Profile* profile) {
    return profile ? static_cast<ProfileRules*>(profile->GetUserData(&kProfileRulesKey)) : nullptr;
  }

  void SetProfilePair(Profile* profile, ProfileRules& rules) {
    const auto& configuration = GetAdBlockConfiguration();
    ReleaseRendererRulesetFile(std::move(rules.cosmetic_file));
    rules.cosmetic_file = configuration.has_cosmetic_rules && configuration.indexed_ruleset.IsValid()
                              ? configuration.indexed_ruleset.Duplicate() : base::File();
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&RendererProcessRegistry::BroadcastCosmetics,
                                  weak_factory_.GetWeakPtr(), profile->GetWeakPtr(),
                                  rules.service_weak_factory.GetWeakPtr()));

    ReleaseRendererRulesetFile(std::move(rules.url_file));
    rules.url_file = configuration.has_url_rules && configuration.indexed_ruleset.IsValid()
                        ? configuration.indexed_ruleset.Duplicate() : base::File();
    // Region identity belongs to this service replacement, not compiler content.
    rules.url_region_identity = rules.url_file.IsValid()
                                    ? base::UnguessableToken::Create() : base::UnguessableToken();
    rules.url_weak_factory.InvalidateWeakPtrs();
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&RendererProcessRegistry::BroadcastURL,
                                  weak_factory_.GetWeakPtr(), profile->GetWeakPtr(),
                                  rules.url_weak_factory.GetWeakPtr()));
  }

  void CopyRules(RulesSnapshot& destination, const RulesSnapshot& source) {
    ReleaseRendererRulesetFile(std::move(destination.cosmetic_file));
    destination.cosmetic_file = source.cosmetic_file.IsValid()
                                    ? source.cosmetic_file.Duplicate() : base::File();
    ReleaseRendererRulesetFile(std::move(destination.url_file));
    destination.url_file = source.url_file.IsValid() ? source.url_file.Duplicate() : base::File();
    destination.url_region_identity = source.url_region_identity;
  }

  std::vector<Entry*> BoundAgents(Profile* profile) {
    std::vector<Entry*> agents;
    for (auto& [id, entry] : entries_) {
      if (entry.profile.get() == profile && entry.remote.is_bound()) {
        // Awaiting Ping is still an ordered associated message destination.
        agents.push_back(&entry);
      }
    }
    return agents;
  }

  void BroadcastCosmetics(base::WeakPtr<Profile> profile,
                           base::WeakPtr<ProfileRules> rules) {
    if (!profile || !rules || Rules(profile.get()) != rules.get()) {
      return;
    }
    PublishAdBlockCosmetics(
        BoundAgents(profile.get()), [&] { return rules->cosmetic_file.IsValid(); },
        &RendererAdBlockFeatureEnabled,
        [&](Entry& entry) {
          entry.remote->UpdateAdBlockCosmeticRules(rules->cosmetic_file.Duplicate());
        },
        [](Entry& entry) { entry.remote->ResetAdBlockCosmeticRules(); });
  }

  void BroadcastURL(base::WeakPtr<Profile> profile,
                     base::WeakPtr<ProfileRules> rules) {
    if (!profile || !rules || Rules(profile.get()) != rules.get()) {
      return;
    }
    PublishAdBlockURL(
        BoundAgents(profile.get()), [&] { return rules->url_file.IsValid(); },
        &RendererAdBlockFeatureEnabled,
        [&](Entry& entry) {
          entry.remote->UpdateAdBlockURLRules(rules->url_file.Duplicate(),
                                              rules->url_region_identity);
        },
        [](Entry& entry) { entry.remote->ResetAdBlockURLRules(); },
        [](bool) {});
  }

  void ResetPending(Entry& entry) {
    ReleaseRendererRulesetFile(std::move(entry.pending.cosmetic_file));
    ReleaseRendererRulesetFile(std::move(entry.pending.url_file));
    entry.pending.url_region_identity = base::UnguessableToken();
    entry.excluded_origins.clear();
    entry.exclusions_version = 0;
    entry.has_exclusions = false;
    entry.initial_configuration_captured = false;
  }

  void CaptureInitialConfiguration(Entry& entry) {
    // Initialization eligibility is fixed per process incarnation. A fallback
    // bind retry must not turn a disabled-born process into an enabled-born one.
    if (entry.initial_configuration_captured) {
      return;
    }
    entry.initial_configuration_captured = true;
    if (!GetAdBlockConfiguration().enabled || !entry.profile) {
      return;
    }
    auto* rules = Rules(entry.profile.get());
    CopyRules(entry.pending, *rules);
    entry.excluded_origins = rules->excluded_origins;
    entry.exclusions_version = rules->exclusions_version;
    entry.has_exclusions = true;
  }

  void SendPendingConfiguration(Entry& entry) {
    // Initial registration skips invalid channels; live publication resets them.
    if (entry.pending.cosmetic_file.IsValid()) {
      entry.remote->UpdateAdBlockCosmeticRules(entry.pending.cosmetic_file.Duplicate());
    }
    if (entry.pending.url_file.IsValid()) {
      entry.remote->UpdateAdBlockURLRules(entry.pending.url_file.Duplicate(),
                                          entry.pending.url_region_identity);
    }
    if (entry.has_exclusions) {
      entry.remote->SetAdBlockExcludedOrigins(entry.excluded_origins,
                                              entry.exclusions_version);
    }
  }

  void BroadcastExclusions(base::WeakPtr<Profile> profile,
                            base::WeakPtr<ProfileRules> rules) {
    if (!profile || !rules || Rules(profile.get()) != rules.get()) {
      return;
    }
    for (auto& [id, entry] : entries_) {
      if (entry.profile.get() != profile.get() ||
          !entry.initial_configuration_captured ||
          !RendererAdBlockFeatureEnabled()) {
        continue;
      }
      // Native queued callback 0x4730fa0 captures only its weak service. It
      // reads the live feature for each agent and the latest profile origins.
      entry.excluded_origins = rules->excluded_origins;
      entry.exclusions_version = rules->exclusions_version;
      entry.has_exclusions = true;
      if (entry.remote.is_bound()) {
        entry.remote->SetAdBlockExcludedOrigins(entry.excluded_origins,
                                                entry.exclusions_version);
      }
    }
  }

  void ConfigurationChanged(AdBlockChange change) {
    const auto& configuration = GetAdBlockConfiguration();
    // Notification eligibility belongs to the live manager operation. Rules and
    // exclusions have distinct causes; compile identity is not a publication gate.
    std::erase_if(profiles_, [](const auto& pair) { return !pair.second; });
    for (const auto& [identity, weak_profile] : profiles_) {
      auto* rules = Rules(weak_profile.get());
      if (change.publish_rules && configuration.has_ruleset_update) {
        SetProfilePair(weak_profile.get(), *rules);
        for (auto& [id, entry] : entries_) {
          if (entry.profile.get() == weak_profile.get() &&
              entry.initial_configuration_captured) {
            // Preserve the existing pre-bind/retry snapshot policy, not a FIFO.
            CopyRules(entry.pending, *rules);
          }
        }
      }
      if (change.publish_exclusions) {
        rules->excluded_origins = configuration.excluded_origins;
        rules->exclusions_version = configuration.config_version;
        content::GetUIThreadTaskRunner({})->PostTask(
            FROM_HERE, base::BindOnce(&RendererProcessRegistry::BroadcastExclusions,
                                      weak_factory_.GetWeakPtr(), weak_profile,
                                      rules->service_weak_factory.GetWeakPtr()));
      }
    }
  }

  std::map<content::ChildProcessId, Entry> entries_;
  std::map<uintptr_t, base::WeakPtr<Profile>> profiles_;
  base::ScopedMultiSourceObservation<content::RenderProcessHost,
                                     content::RenderProcessHostObserver> observations_{this};
  base::CallbackListSubscription subscription_;
  uint64_t incarnation_ = 0;
  uint64_t bound_sequence_ = 0;
  base::WeakPtrFactory<RendererProcessRegistry> weak_factory_{this};
};

RendererProcessRegistry& Registry() {
  static base::NoDestructor<RendererProcessRegistry> registry;
  return *registry;
}
}  // namespace

void InitializeRendererProcessBridge() { Registry(); }
void InitializeRendererProfile(Profile* profile) { Registry().InitializeProfile(profile); }
void RendererProcessWillLaunch(content::RenderProcessHost* host,
                               bool early_process_agent_installed) {
  Registry().ProcessWillLaunch(host, early_process_agent_installed);
}
void RendererProcessFrameReady(content::RenderProcessHost* host) { Registry().FrameReady(host); }
void GetRendererAdBlockDebugInfo(Profile* profile, const GURL& url,
                                 base::OnceCallback<void(const std::string&)> completion) {
  Registry().RequestDebug(profile, url, std::move(completion));
}
}  // namespace arc
