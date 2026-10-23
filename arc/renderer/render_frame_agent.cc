// Copyright 2026 ArcMod. Frame-local stylesheet injection through Blink.
#include "arc/renderer/render_frame_agent.h"

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "arc/common/renderer.mojom.h"
#include "arc/renderer/ad_block_filter.h"
#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "components/subresource_filter/core/common/indexed_ruleset.h"
#include "components/subresource_filter/core/common/memory_mapped_ruleset.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/platform/web_document_subresource_filter.h"
#include "third_party/blink/public/platform/web_security_origin.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/public/web/web_document_loader.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace arc {
namespace {
class RenderFrameAgent final : public content::RenderFrameObserver,
                               public mojom::RenderFrameAgent {
 public:
  explicit RenderFrameAgent(content::RenderFrame* frame)
      : content::RenderFrameObserver(frame) {
    frame->GetAssociatedInterfaceRegistry()->AddInterface<mojom::RenderFrameAgent>(
        base::BindRepeating(&RenderFrameAgent::Bind, base::Unretained(this)));
  }

  void InsertStylesheet(const std::string& identifier,
                        const std::string& stylesheet) override {
    auto document = render_frame()->GetWebFrame()->GetDocument();
    if (document.IsNull()) {
      return;
    }
    auto it = stylesheets_.find(identifier);
    // Blink permits several sheets with the same key: reusing the key alone
    // would accumulate rules, and a later removal would leave old rules active.
    if (it != stylesheets_.end()) {
      document.RemoveInsertedStyleSheet(it->second);
    }
    auto key = document.InsertStyleSheet(blink::WebString::FromUtf8(stylesheet));
    stylesheets_.insert_or_assign(identifier, std::move(key));
  }

  void RemoveStylesheet(const std::string& identifier) override {
    auto it = stylesheets_.find(identifier);
    if (it == stylesheets_.end()) {
      return;
    }
    auto document = render_frame()->GetWebFrame()->GetDocument();
    if (!document.IsNull()) {
      document.RemoveInsertedStyleSheet(it->second);
    }
    stylesheets_.erase(it);
  }

  void ConfigureAdBlock(
      base::File indexed_ruleset,
      const std::vector<std::string>& excluded_origins,
      uint64_t ruleset_id,
      mojo::PendingAssociatedRemote<mojom::RendererHost> host) override {
    host_.reset();
    host_.Bind(std::move(host));
    if (indexed_ruleset.IsValid() && (!ruleset_ || ruleset_id_ != ruleset_id)) {
      auto mapping = subresource_filter::MemoryMappedRuleset::CreateAndInitialize(
          std::move(indexed_ruleset));
      if (mapping && subresource_filter::IndexedRulesetMatcher::Verify(
                         mapping->data(), 0, "Arc.AdBlock")) {
        ruleset_ = std::move(mapping);
        ruleset_id_ = ruleset_id;
      }
    }
    excluded_origins_.clear();
    for (const auto& serialized : excluded_origins) {
      excluded_origins_.push_back(url::Origin::Create(GURL(serialized)));
    }
    InstallAdBlockFilter();
  }

  void DidCreateNewDocument() override {
    stylesheets_.clear();
    InstallAdBlockFilter();
  }
  void OnDestruct() override { delete this; }

 private:
  void InstallAdBlockFilter() {
    auto* frame = render_frame()->GetWebFrame();
    auto* loader = frame->GetDocumentLoader();
    if (!ruleset_ || !loader || frame->GetDocument().IsNull()) {
      return;
    }
    const url::Origin origin = frame->GetSecurityOrigin();
    // Original 0x47336f0 also exempts extension documents before list matching.
    bool excluded = origin.scheme() == "chrome-extension";
    for (const auto& excluded_origin : excluded_origins_) {
      // Original renderer 0x47336f0 -> 0x16bce58 -> 0x14f6940.
      if (net::registry_controlled_domains::SameDomainOrHost(
              origin, excluded_origin,
              net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES)) {
        excluded = true;
        break;
      }
    }
    loader->SetSubresourceFilter(
        CreateAdBlockFilter(ruleset_, origin, excluded,
                           base::BindRepeating(&RenderFrameAgent::ReportDisallowedLoad,
                                               weak_ptr_factory_.GetWeakPtr()))
            .release());
  }

  void ReportDisallowedLoad() {
    if (host_.is_bound()) {
      host_->ReportDisallowedLoad();
    }
  }

  void Bind(mojo::PendingAssociatedReceiver<mojom::RenderFrameAgent> receiver) {
    receivers_.Add(this, std::move(receiver));
  }
  std::map<std::string, blink::WebStyleSheetKey> stylesheets_;
  scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset_;
  uint64_t ruleset_id_ = 0;
  std::vector<url::Origin> excluded_origins_;
  mojo::AssociatedRemote<mojom::RendererHost> host_;
  mojo::AssociatedReceiverSet<mojom::RenderFrameAgent> receivers_;
  base::WeakPtrFactory<RenderFrameAgent> weak_ptr_factory_{this};
};
}  // namespace

void CreateRenderFrameAgent(content::RenderFrame* frame) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    new RenderFrameAgent(frame);
  }
}
}  // namespace arc
