// Copyright 2026 ArcMod. Process-wide adblock rules and cosmetic cache.
#include "arc/renderer/render_process_agent.h"

#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "arc/common/renderer.mojom.h"
#include "arc/renderer/render_frame_agent.h"
#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/no_destructor.h"
#include "base/unguessable_token.h"
#include "components/subresource_filter/core/common/indexed_ruleset.h"
#include "components/subresource_filter/core/common/memory_mapped_ruleset.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_visitor.h"
#include "content/public/renderer/render_thread.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace arc {
namespace {
class UpdateFrames final : public content::RenderFrameVisitor {
 public:
  explicit UpdateFrames(bool cosmetics_only) : cosmetics_only_(cosmetics_only) {}
  bool Visit(content::RenderFrame* frame) override {
    if (cosmetics_only_) {
      UpdateFrameAdBlockCosmetics(frame);
    } else {
      UpdateFrameAdBlockConfiguration(frame);
    }
    return true;
  }
 private:
  const bool cosmetics_only_;
};

class RenderProcessAgent final : public content::RenderThreadObserver,
                                 public mojom::RenderProcessAgent {
 public:
  void EnsureInstalled() {
    if (!installed_ && content::RenderThread::Get()) {
      content::RenderThread::Get()->AddObserver(this);
      installed_ = true;
    }
  }
  void RegisterMojoInterfaces(blink::AssociatedInterfaceRegistry* registry) override {
    registry->AddInterface<mojom::RenderProcessAgent>(base::BindRepeating(
        &RenderProcessAgent::Bind, base::Unretained(this)));
  }
  void UnregisterMojoInterfaces(blink::AssociatedInterfaceRegistry* registry) override {
    registry->RemoveInterface(mojom::RenderProcessAgent::Name_);
  }
  void UpdateAdBlockCosmeticRules(base::File indexed_ruleset) override {
    auto mapping = MapRules(std::move(indexed_ruleset));
    if (!mapping) {
      return;
    }
    cosmetic_ruleset_ = std::move(mapping);
    Invalidate(true);
  }
  void ResetAdBlockCosmeticRules() override {
    cosmetic_ruleset_.reset();
    Invalidate(true);
  }
  void UpdateAdBlockURLRules(base::File indexed_ruleset,
                             const base::UnguessableToken& region_identity) override {
    if (url_ruleset_ && url_region_identity_ == region_identity) {
      return;
    }
    auto mapping = MapRules(std::move(indexed_ruleset));
    if (!mapping) {
      return;
    }
    url_ruleset_ = std::move(mapping);
    url_region_identity_ = region_identity;
    Invalidate();
  }
  void ResetAdBlockURLRules() override {
    url_ruleset_.reset();
    url_region_identity_ = base::UnguessableToken();
    Invalidate();
  }
  void SetAdBlockExcludedOrigins(const std::vector<std::string>& excluded_origins,
                                 uint64_t config_version) override {
    if (configured_ && config_version < config_version_) {
      return;
    }
    configured_ = true;
    config_version_ = config_version;
    excluded_origins_.clear();
    for (const auto& serialized : excluded_origins) {
      excluded_origins_.push_back(url::Origin::Create(GURL(serialized)));
    }
    Invalidate();
  }
  void Ping(PingCallback callback) override { std::move(callback).Run(); }
  void GetAdBlockDebugInfo(const std::string& url,
                           GetAdBlockDebugInfoCallback callback) override {
    const GURL parsed(url);
    auto result = EvaluateCosmetics(parsed, url::Origin::Create(parsed));
    std::move(callback).Run("/* Result: " + result.state + " */\n/ * CSS: */\n" + result.css);
  }
  scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset() const {
    return url_ruleset_;
  }
  bool IsExcluded(const url::Origin& origin) const {
    if (origin.scheme() == "chrome-extension") {
      return true;
    }
    return std::ranges::any_of(excluded_origins_, [&origin](const auto& excluded) {
      return net::registry_controlled_domains::SameDomainOrHost(
          origin, excluded, net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
    });
  }

  CosmeticResult EvaluateCosmetics(const GURL& url, const url::Origin& origin) {
    // This order is recovered from 0x43a51f4, including cache lookup before
    // NoScheme/InternalScheme. Only successful nonempty CSS is cached.
    if (!cosmetic_ruleset_) {
      return {"NoBlockList", {}};
    }
    if (IsExcluded(origin)) {
      return {"Excluded", {}};
    }
    const std::string host(url.host());
    auto& cache = css_by_host_;
    const auto cached = cache.find(host);
    if (cached != cache.end()) {
      return {"Cached: " + host, cached->second};
    }
    subresource_filter::IndexedRulesetMatcher matcher(cosmetic_ruleset_->data());
    const auto evaluated_origin = url::Origin::Create(url);
    bool generic_disabled = false;
    if (url_ruleset_) {
      // Selector indices belong to C; activation exceptions belong to CURRENT U.
      subresource_filter::IndexedRulesetMatcher url_matcher(url_ruleset_->data());
      if (url_matcher.ShouldDisableFilteringForDocument(
              url, evaluated_origin, url_pattern_index::proto::ACTIVATION_TYPE_ELEMHIDE)) {
        return {"Excluded", {}};
      }
      generic_disabled = url_matcher.ShouldDisableFilteringForDocument(
          url, evaluated_origin, url_pattern_index::proto::ACTIVATION_TYPE_GENERICHIDE);
    }
    if (!url.has_scheme()) {
      return {"NoScheme", {}};
    }
    if (url.SchemeIs("filesystem") || url.SchemeIs("about") || url.SchemeIs("blob") ||
        url.SchemeIs("data") || url.SchemeIs("chrome-extension") || url.SchemeIs("devtools") ||
        url.SchemeIs("chrome") || url.SchemeIs("view-source")) {
      return {"InternalScheme", {}};
    }
    std::vector<std::string_view> matched;
    matcher.GetDomainSelectors(origin, matched);
    const auto* index = subresource_filter::flat::GetIndexedRuleset(cosmetic_ruleset_->data().data())
                            ->style_rule_index();
    if (index && !generic_disabled) {
      // Blink currently has no consumers for its class/ID matcher methods.
      // Enumerate indexed anchors and delegate exclusions/matching to Chromium.
      // Emitting valid generic CSS also handles elements added after extraction.
      if (index->class_map()) {
        for (const auto* entry : *index->class_map()) {
          const auto name = entry->name()->string_view().substr(1);
          matcher.GetSelectorsByClass(origin, name, subresource_filter::GetStyleRuleHash(name), matched);
        }
      }
      if (index->id_map()) {
        for (const auto* entry : *index->id_map()) {
          const auto name = entry->name()->string_view().substr(1);
          matcher.GetSelectorsById(origin, name, subresource_filter::GetStyleRuleHash(name), matched);
        }
      }
    }
    std::unordered_set<std::string> selectors;
    for (const auto selector : matched) {
      selectors.emplace(selector);
    }
    if (selectors.empty()) {
      return {"Empty", {}};
    }
    std::string css;
    size_t group_count = 0;
    for (const auto& selector : selectors) {
      const size_t count = 1 + static_cast<size_t>(std::ranges::count(selector, ','));
      if (group_count && group_count + count > 4096) {
        css += "{display:none!important;}\n";
        group_count = 0;
      }
      if (group_count) {
        css += ',';
      }
      css += selector;
      group_count += count;
    }
    css += "{display:none!important;}\n";
    cache.insert_or_assign(host, css);
    return {"Success", std::move(css)};
  }


 private:
  static scoped_refptr<const subresource_filter::MemoryMappedRuleset> MapRules(
      base::File indexed_ruleset) {
    auto mapping = subresource_filter::MemoryMappedRuleset::CreateAndInitialize(
        std::move(indexed_ruleset));
    if (!mapping || !subresource_filter::IndexedRulesetMatcher::Verify(
                        mapping->data(), 0, "Arc.AdBlock")) {
      return nullptr;
    }
    return mapping;
  }
  void Invalidate(bool cosmetics_only = false) {
    css_by_host_.clear();
    UpdateFrames visitor(cosmetics_only);
    content::RenderFrame::ForEach(&visitor);
  }
  void Bind(mojo::PendingAssociatedReceiver<mojom::RenderProcessAgent> receiver) {
    receivers_.Add(this, std::move(receiver));
  }
  bool installed_ = false;
  bool configured_ = false;
  base::UnguessableToken url_region_identity_;
  uint64_t config_version_ = 0;
  scoped_refptr<const subresource_filter::MemoryMappedRuleset> cosmetic_ruleset_;
  scoped_refptr<const subresource_filter::MemoryMappedRuleset> url_ruleset_;
  std::vector<url::Origin> excluded_origins_;
  std::map<std::string, std::string> css_by_host_;
  mojo::AssociatedReceiverSet<mojom::RenderProcessAgent> receivers_;
};

RenderProcessAgent& Agent() {
  static base::NoDestructor<RenderProcessAgent> agent;
  return *agent;
}
}  // namespace

void EnsureRenderProcessAgent() {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    Agent().EnsureInstalled();
  }
}

scoped_refptr<const subresource_filter::MemoryMappedRuleset> GetProcessAdBlockRuleset() {
  return Agent().ruleset();
}
bool IsProcessAdBlockOriginExcluded(const url::Origin& origin) {
  return Agent().IsExcluded(origin);
}
CosmeticResult EvaluateProcessAdBlockCosmetics(const GURL& url, const url::Origin& origin) {
  return Agent().EvaluateCosmetics(url, origin);
}
}  // namespace arc
