// Copyright 2026 ArcMod. Browser registry of renderer-process adblock agents.
#import "arc/api/arc_internal.h"

#include <map>
#include <utility>

#include "arc/common/ad_block_browser.h"
#include "arc/common/renderer.mojom.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/no_destructor.h"
#include "base/supports_user_data.h"
#include "base/scoped_multi_source_observation.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_process_host_creation_observer.h"
#include "content/public/browser/render_process_host_observer.h"
#include "ipc/ipc_channel_proxy.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/callback_helpers.h"

namespace arc {
namespace {
constexpr int kProfileRulesKey = 0;
struct ProfileRules final : public base::SupportsUserData::Data {
  ~ProfileRules() override { ReleaseRendererRulesetFile(std::move(file)); }
  base::File file;
  bool has_ruleset_update = false;
  bool has_cosmetic_rules = false;
  uint64_t ruleset_id = 0;
  uint64_t exclusions_version = 0;
  std::vector<std::string> excluded_origins;
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
        Track(host);
        Bind(host);
      }
    }
  }

  void InitializeProfile(Profile* profile) {
    if (!profile) {
      return;
    }
    profiles_.insert_or_assign(reinterpret_cast<uintptr_t>(profile), profile->GetWeakPtr());
    if (!profile->GetUserData(&kProfileRulesKey)) {
      auto rules = std::make_unique<ProfileRules>();
      const auto& configuration = GetAdBlockConfiguration();
      if (configuration.enabled) {
        ApplyManagerRules(*rules);
        rules->excluded_origins = configuration.excluded_origins;
        rules->exclusions_version = configuration.config_version;
      }
      profile->SetUserData(&kProfileRulesKey, std::move(rules));
    }
  }

  void OnRenderProcessHostCreated(content::RenderProcessHost* host) override {
    ProcessWillLaunch(host);
  }

  void ProcessWillLaunch(content::RenderProcessHost* host) {
    Track(host);
    auto& entry = entries_.at(host->GetID());
    // The embedder launch hook precedes Created/Ready. Their fallback must not
    // register the same incarnation twice or change its original sequence.
    if (!entry.attempted) {
      Bind(host);
    }
  }
  void RenderProcessReady(content::RenderProcessHost* host) override {
    Track(host);
    auto& entry = entries_.at(host->GetID());
    if (!entry.attempted) {
      Bind(host);
    }
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
    Track(host);
    auto& entry = entries_.at(host->GetID());
    entry.frame_ready = true;
    if (!entry.remote.is_bound() && !entry.retried) {
      entry.retried = true;
      Bind(host);
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
    bool frame_ready = false;
    bool retried = false;
    bool initial_configuration_captured = false;
    bool has_exclusions = false;
    ProfileRules pending;
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
    if (!host->GetChannel() || !entry.profile ||
        host->GetBrowserContext() != entry.profile.get()) {
      return;
    }
    entry.remote.reset();
    entry.attempted = true;
    entry.acknowledged = false;
    // Native registers/counters synchronously at the browser bind attempt,
    // independently of renderer startup or Ping response ordering.
    entry.bound_sequence = ++bound_sequence_;
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
    entry.bound_sequence = 0;
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
    entry.frame_ready = false;
    entry.retried = false;
  }

  ProfileRules* Rules(Profile* profile) {
    return profile ? static_cast<ProfileRules*>(profile->GetUserData(&kProfileRulesKey)) : nullptr;
  }

  bool ApplyManagerRules(ProfileRules& rules) {
    const auto& configuration = GetAdBlockConfiguration();
    if (!configuration.has_ruleset_update ||
        (rules.has_ruleset_update && rules.ruleset_id == configuration.ruleset_id)) {
      return false;
    }
    ReleaseRendererRulesetFile(std::move(rules.file));
    rules.file = configuration.indexed_ruleset.IsValid()
                     ? configuration.indexed_ruleset.Duplicate() : base::File();
    rules.has_ruleset_update = true;
    rules.ruleset_id = configuration.ruleset_id;
    rules.has_cosmetic_rules = configuration.has_cosmetic_rules;
    return true;
  }

  void SendRules(Entry& entry, const ProfileRules& rules) {
    entry.remote->SetAdBlockRuleset(
        rules.file.IsValid() ? rules.file.Duplicate() : base::File(), rules.ruleset_id,
        rules.has_cosmetic_rules);
  }

  void CopyRules(ProfileRules& destination, const ProfileRules& source) {
    ReleaseRendererRulesetFile(std::move(destination.file));
    destination.file = source.file.IsValid() ? source.file.Duplicate() : base::File();
    destination.has_ruleset_update = source.has_ruleset_update;
    destination.ruleset_id = source.ruleset_id;
    destination.has_cosmetic_rules = source.has_cosmetic_rules;
  }

  void ResetPending(Entry& entry) {
    ReleaseRendererRulesetFile(std::move(entry.pending.file));
    entry.pending.has_ruleset_update = false;
    entry.pending.excluded_origins.clear();
    entry.pending.exclusions_version = 0;
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
    entry.pending.excluded_origins = rules->excluded_origins;
    entry.pending.exclusions_version = rules->exclusions_version;
    entry.has_exclusions = true;
  }

  void SendPendingConfiguration(Entry& entry) {
    if (entry.pending.has_ruleset_update) {
      SendRules(entry, entry.pending);
    }
    if (entry.has_exclusions) {
      entry.remote->SetAdBlockExcludedOrigins(entry.pending.excluded_origins,
                                              entry.pending.exclusions_version);
    }
  }

  void ConfigurationChanged() {
    const auto& configuration = GetAdBlockConfiguration();
    std::erase_if(profiles_, [](const auto& pair) { return !pair.second; });
    for (const auto& [identity, weak_profile] : profiles_) {
      auto* rules = Rules(weak_profile.get());
      // Explicit exclusions are delivered even while disabled. Compiled rules
      // remain gated at the browser service until an enabled publication.
      const bool update_rules = configuration.enabled && ApplyManagerRules(*rules);
      const bool update_exclusions = rules->exclusions_version != configuration.config_version;
      if (update_exclusions) {
        rules->excluded_origins = configuration.excluded_origins;
        rules->exclusions_version = configuration.config_version;
      }
      for (auto& [id, entry] : entries_) {
        if (entry.profile.get() != weak_profile.get() ||
            !entry.initial_configuration_captured) {
          continue;
        }
        if (update_rules) {
          CopyRules(entry.pending, *rules);
          if (entry.remote.is_bound()) {
            SendRules(entry, entry.pending);
          }
        }
        if (update_exclusions) {
          entry.pending.excluded_origins = rules->excluded_origins;
          entry.pending.exclusions_version = rules->exclusions_version;
          entry.has_exclusions = true;
          if (entry.remote.is_bound()) {
            entry.remote->SetAdBlockExcludedOrigins(entry.pending.excluded_origins,
                                                    entry.pending.exclusions_version);
          }
        }
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
void RendererProcessWillLaunch(content::RenderProcessHost* host) { Registry().ProcessWillLaunch(host); }
void RendererProcessFrameReady(content::RenderProcessHost* host) { Registry().FrameReady(host); }
void GetRendererAdBlockDebugInfo(Profile* profile, const GURL& url,
                                 base::OnceCallback<void(const std::string&)> completion) {
  Registry().RequestDebug(profile, url, std::move(completion));
}
}  // namespace arc
