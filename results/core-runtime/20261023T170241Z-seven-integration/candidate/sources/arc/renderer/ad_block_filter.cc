// Copyright 2026 ArcMod. Blink policy adapter backed by Chromium's rule matcher.
#include "arc/renderer/ad_block_filter.h"

#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

#include "components/subresource_filter/content/renderer/filter_utils.h"
#include "components/subresource_filter/core/common/first_party_origin.h"
#include "components/subresource_filter/core/common/indexed_ruleset.h"
#include "components/subresource_filter/core/common/load_policy.h"
#include "components/subresource_filter/core/common/memory_mapped_ruleset.h"
#include "components/subresource_filter/core/common/scoped_rule.h"
#include "third_party/blink/public/platform/web_document_subresource_filter.h"
#include "third_party/blink/public/platform/web_url.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace arc {
namespace {
bool IsInternalScheme(std::string_view scheme) {
  return scheme == "chrome-extension" || scheme == "devtools" || scheme == "chrome" ||
         scheme == "view-source";
}

url_pattern_index::proto::ElementType ArcElementType(
    network::mojom::RequestDestination destination) {
  // Keep the upstream conversion, with the recovered original table's
  // differences (0xcf27094) and its OTHER fallback for newer/unknown values.
  switch (destination) {
    case network::mojom::RequestDestination::kDocument:
      return url_pattern_index::proto::ELEMENT_TYPE_SUBDOCUMENT;
    case network::mojom::RequestDestination::kAudioWorklet:
    case network::mojom::RequestDestination::kPaintWorklet:
    case network::mojom::RequestDestination::kServiceWorker:
    case network::mojom::RequestDestination::kSharedWorker:
    case network::mojom::RequestDestination::kXslt:
      return url_pattern_index::proto::ELEMENT_TYPE_OTHER;
    default: {
      const auto type = subresource_filter::ToElementType(destination);
      return type == url_pattern_index::proto::ELEMENT_TYPE_UNSPECIFIED
                 ? url_pattern_index::proto::ELEMENT_TYPE_OTHER
                 : type;
    }
  }
}

class AdBlockFilter final : public blink::WebDocumentSubresourceFilter {
 public:
  AdBlockFilter(
      scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset,
      url::Origin document_origin,
      bool excluded,
      base::RepeatingClosure disallowed_load_callback)
      : ruleset_(std::move(ruleset)),
        matcher_(ruleset_->data()),
        first_party_(std::move(document_origin)),
        excluded_(excluded),
        disallowed_load_callback_(std::move(disallowed_load_callback)) {}
  ~AdBlockFilter() override = default;

  LoadPolicy GetLoadPolicy(
      const blink::WebURL& url,
      network::mojom::RequestDestination destination,
      subresource_filter::ScopedRule* out_rule) override {
    return Match(url, ArcElementType(destination), out_rule);
  }
  LoadPolicy GetLoadPolicyForWebSocketConnect(const blink::WebURL& url) override {
    return Match(url, url_pattern_index::proto::ELEMENT_TYPE_WEBSOCKET, nullptr);
  }
  LoadPolicy GetLoadPolicyForWebTransportConnect(const blink::WebURL& url) override {
    return Match(url, url_pattern_index::proto::ELEMENT_TYPE_WEBTRANSPORT, nullptr);
  }
  void GetDomainSelectors(std::vector<std::string_view>& selectors) override {
    if (!excluded_) {
      matcher_.GetDomainSelectors(first_party_.origin(), selectors);
    }
  }
  bool MaybeHasStyleRule(uint32_t hash) override {
    return !excluded_ && matcher_.MaybeHasStyleRule(hash);
  }
  void GetSelectorsByClass(std::string_view name,
                           uint32_t hash,
                           std::vector<std::string_view>& selectors) override {
    if (!excluded_) {
      matcher_.GetSelectorsByClass(first_party_.origin(), name, hash, selectors);
    }
  }
  void GetSelectorsById(std::string_view name,
                       uint32_t hash,
                       std::vector<std::string_view>& selectors) override {
    if (!excluded_) {
      matcher_.GetSelectorsById(first_party_.origin(), name, hash, selectors);
    }
  }
  bool IsDryRun() override { return false; }
  uint64_t GetRulesetId() const override { return matcher_.ruleset_id(); }
  void ReportDisallowedLoad() override {
    if (disallowed_load_callback_) {
      disallowed_load_callback_.Run();
    }
  }
  bool ShouldLogToConsole() override { return false; }

 private:
  LoadPolicy Match(const blink::WebURL& url,
                   url_pattern_index::proto::ElementType element_type,
                   subresource_filter::ScopedRule* out_rule) {
    if (out_rule) {
      *out_rule = subresource_filter::ScopedRule();
    }
    const GURL resource_url(url);
    if (excluded_ || IsInternalScheme(first_party_.origin().scheme()) ||
        IsInternalScheme(resource_url.scheme()) || resource_url.SchemeIs("filesystem") ||
        resource_url.SchemeIs("about") || resource_url.SchemeIs("blob") ||
        resource_url.SchemeIs("data")) {
      return kAllow;
    }
    // Native throttle 0x4733f68..0x4733fac checks activation against this
    // resource URL and its initiator origin before evaluating its load policy.
    if (matcher_.ShouldDisableFilteringForDocument(
            resource_url, first_party_.origin(),
            url_pattern_index::proto::ACTIVATION_TYPE_DOCUMENT)) {
      return kAllow;
    }
    const bool disable_generic_rules = matcher_.ShouldDisableFilteringForDocument(
        resource_url, first_party_.origin(),
        url_pattern_index::proto::ACTIVATION_TYPE_GENERICBLOCK);
    const url_pattern_index::flat::UrlRule* rule = nullptr;
    const auto policy = matcher_.GetLoadPolicyForResourceLoad(
        resource_url, first_party_, element_type, disable_generic_rules, &rule);
    if (out_rule && rule) {
      *out_rule = subresource_filter::ScopedRule(ruleset_, rule);
    }
    return policy == subresource_filter::LoadPolicy::DISALLOW ? kDisallow : kAllow;
  }

  // The owning mapping precedes its non-owning matcher and any ScopedRule
  // returned to Blink keeps its own mapping reference.
  const scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset_;
  subresource_filter::IndexedRulesetMatcher matcher_;
  subresource_filter::FirstPartyOrigin first_party_;
  const bool excluded_;
  base::RepeatingClosure disallowed_load_callback_;
};
}  // namespace

std::unique_ptr<blink::WebDocumentSubresourceFilter> CreateAdBlockFilter(
    scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset,
    url::Origin document_origin,
    bool excluded,
    base::RepeatingClosure disallowed_load_callback) {
  return std::make_unique<AdBlockFilter>(std::move(ruleset), std::move(document_origin),
                                         excluded, std::move(disallowed_load_callback));
}
}  // namespace arc
