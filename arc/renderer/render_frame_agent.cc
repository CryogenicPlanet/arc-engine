// Copyright 2026 ArcMod. Frame-local stylesheet injection through Blink.
#include "arc/renderer/render_frame_agent.h"

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "arc/common/renderer.mojom.h"
#include "arc/renderer/ad_block_filter.h"
#include "arc/renderer/render_process_agent.h"
#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "components/subresource_filter/core/common/memory_mapped_ruleset.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/platform/web_document_subresource_filter.h"
#include "third_party/blink/public/platform/web_security_origin.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/public/web/web_dom_event.h"
#include "third_party/blink/public/web/web_element.h"
#include "third_party/blink/public/web/web_form_control_element.h"
#include "third_party/blink/public/web/web_document_loader.h"
#include "third_party/blink/public/web/web_css_origin.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace arc {
namespace {
constexpr int kFrameAgentKey = 0;
class RenderFrameAgent;
struct FrameAgentReference final : public base::SupportsUserData::Data {
  explicit FrameAgentReference(base::WeakPtr<RenderFrameAgent> reference)
      : agent(std::move(reference)) {}
  base::WeakPtr<RenderFrameAgent> agent;
};

class RenderFrameAgent final : public content::RenderFrameObserver,
                               public mojom::RenderFrameAgent {
 public:
  explicit RenderFrameAgent(content::RenderFrame* frame)
      : content::RenderFrameObserver(frame) {
    frame->SetUserData(&kFrameAgentKey, std::make_unique<FrameAgentReference>(
                                           weak_ptr_factory_.GetWeakPtr()));
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
      document.RemoveInsertedStyleSheet(it->second, blink::WebCssOrigin::kUser);
    }
    auto key = document.InsertStyleSheet(blink::WebString::FromUtf8(stylesheet), nullptr,
                                        blink::WebCssOrigin::kUser,
                                        blink::BackForwardCacheAware::kAllow);
    stylesheets_.insert_or_assign(identifier, std::move(key));
  }

  void RemoveStylesheet(const std::string& identifier) override {
    auto it = stylesheets_.find(identifier);
    if (it == stylesheets_.end()) {
      return;
    }
    auto document = render_frame()->GetWebFrame()->GetDocument();
    if (!document.IsNull()) {
      document.RemoveInsertedStyleSheet(it->second, blink::WebCssOrigin::kUser);
    }
    stylesheets_.erase(it);
  }

  void ConfigureAdBlock(base::File, bool, const std::vector<std::string>&,
                        uint64_t, uint64_t,
                        mojo::PendingAssociatedRemote<mojom::RendererHost> host) override {
    host_.reset();
    host_.Bind(std::move(host));
  }

  void GetAdBlockDebugInfo(const std::string& url,
                           GetAdBlockDebugInfoCallback callback) override {
    const GURL parsed(url);
    auto result = EvaluateProcessAdBlockCosmetics(parsed, url::Origin::Create(parsed));
    std::move(callback).Run("/* Result: " + result.state + " */\n/ * CSS: */\n" + result.css);
  }

  void SetRendererHost(mojo::PendingAssociatedRemote<mojom::RendererHost> host,
                       SetRendererHostCallback callback) override {
    host_.reset();
    host_.Bind(std::move(host));
    std::move(callback).Run();
  }

  void AdBlockConfigurationChanged() {
    InstallAdBlockFilter();
    ApplyAdBlockCosmetics();
  }

  void ConfigureEffects(std::vector<mojom::HostEffectsConfigurationPtr> configuration) override {
    // Original EffectsAgent receiver only swaps its config pointer.
    effects_ = std::move(configuration);
  }

  void ApplyAdBlockCosmetics() {
    auto* frame = render_frame()->GetWebFrame();
    auto document = frame->GetDocument();
    if (document.IsNull()) {
      return;
    }
    auto result = EvaluateProcessAdBlockCosmetics(GURL(document.Url()), frame->GetSecurityOrigin());
    // The original cosmetic helper uses literal identifier "blocklist" in the
    // same frame-local user sheet map used by direct CSS and profile Boosts.
    if (!result.css.empty()) {
      InsertStylesheet("blocklist", result.css);
    } else {
      RemoveStylesheet("blocklist");
    }
  }

  void ApplyEffectsAtDocumentStart() {
    if (!render_frame()->IsMainFrame()) {
      return;
    }
    auto* frame = render_frame()->GetWebFrame();
    auto document = frame->GetDocument();
    if (document.IsNull()) {
      return;
    }
    const std::string host(GURL(document.Url()).host());
    mojom::HostEffectsConfigurationPtr selected;
    for (const auto& configuration : effects_) {
      if (configuration->host == host) {
        // Scripts can reenter the embedder; keep this invocation's config alive.
        selected = configuration.Clone();
        break;
      }
    }
    if (!selected) {
      return;
    }
    for (const auto& source : selected->stylesheets) {
      // Direct CSS and profile Boosts use the same original frame-local helper
      // and identifier map (0x43a9608), including replacement/removal collisions.
      InsertStylesheet(source->identifier, source->source);
    }
    auto weak = weak_ptr_factory_.GetWeakPtr();
    for (const auto& source : selected->scripts) {
      frame->ExecuteScriptInIsolatedWorld(
          6, blink::WebScriptSource(blink::WebString::FromUtf8(source->source),
                                   blink::WebURL(GURL("arc://effect/" + source->identifier))),
          blink::BackForwardCacheAware::kAllow);
      if (!weak) {
        return;
      }
    }
  }

  void DidCreateNewDocument() override {
    stylesheets_.clear();
    installed_ad_block_filter_ = false;
    installed_filter_ruleset_.reset();
    InstallAdBlockFilter();
  }
  void OnDestruct() override { delete this; }

  // Arc's engine reports the focused field and each input/click on the page (ArcWebContents field state and
  // inputEvent). Listeners live on the current document and go with it.
  void DidCreateDocumentElement() override {
    input_listeners_.clear();
    last_focus_ = {false, false};
    blink::WebDocument document = render_frame()->GetWebFrame()->GetDocument();
    if (document.IsNull()) {
      return;
    }
    input_listeners_.push_back(document.AddEventListener(
        blink::WebNode::EventType::kInput,
        base::BindRepeating(&RenderFrameAgent::OnDOMEvent, weak_ptr_factory_.GetWeakPtr(), true),
        /*use_capture=*/true));
    input_listeners_.push_back(document.AddEventListener(
        blink::WebNode::EventType::kClick,
        base::BindRepeating(&RenderFrameAgent::OnDOMEvent, weak_ptr_factory_.GetWeakPtr(), false),
        /*use_capture=*/true));
  }

  void FocusedElementChanged(const blink::WebElement& element) override {
    const bool editable = !element.IsNull() && element.IsEditable();
    bool password = false;
    if (editable && element.IsFormControlElement()) {
      password = element.To<blink::WebFormControlElement>().FormControlType() ==
                 blink::mojom::FormControlType::kInputPassword;
    }
    if (last_focus_ == std::make_pair(editable, password) || !host_.is_bound()) {
      return;
    }
    last_focus_ = {editable, password};
    host_->FocusedFieldChanged(editable, password);
  }

 private:
  void OnDOMEvent(bool is_input, blink::WebDOMEvent event) {
    const blink::WebNode target = event.Target();
    if (target.IsNull() || !host_.is_bound()) {
      return;
    }
    base::DictValue details;
    details.Set("id", target.GetDomNodeId());
    if (is_input && target.IsElementNode() && target.To<blink::WebElement>().IsFormControlElement()) {
      details.Set("value", target.To<blink::WebFormControlElement>().Value().Utf8());
    }
    base::DictValue payload;
    payload.Set(is_input ? "input" : "click", std::move(details));
    host_->ReportUserInput(base::WriteJson(payload).value_or(std::string()), is_input);
  }

  void InstallAdBlockFilter() {
    auto* frame = render_frame()->GetWebFrame();
    auto* loader = frame->GetDocumentLoader();
    if (!loader || frame->GetDocument().IsNull()) {
      return;
    }
    auto ruleset = GetProcessAdBlockRuleset();
    if (!ruleset) {
      if (installed_ad_block_filter_ && installed_filter_ruleset_) {
        // Blink requires a non-null filter. Replace the former blocking filter
        // with an allow-all adapter; process rules stay null for NoBlockList.
        loader->SetSubresourceFilter(
            CreateAdBlockFilter(installed_filter_ruleset_, frame->GetSecurityOrigin(),
                               true, base::RepeatingClosure())
                .release());
        installed_ad_block_filter_ = false;
        installed_filter_ruleset_.reset();
      }
      return;
    }
    const url::Origin origin = frame->GetSecurityOrigin();
    const url::Origin top_origin = frame->Top()->GetSecurityOrigin();
    const bool excluded = IsProcessAdBlockOriginExcluded(origin) ||
                          IsProcessAdBlockOriginExcluded(top_origin);
    loader->SetSubresourceFilter(
        CreateAdBlockFilter(ruleset, origin, excluded,
                           base::BindRepeating(&RenderFrameAgent::ReportDisallowedLoad,
                                               weak_ptr_factory_.GetWeakPtr()))
            .release());
    installed_ad_block_filter_ = true;
    installed_filter_ruleset_ = std::move(ruleset);
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
  std::vector<mojom::HostEffectsConfigurationPtr> effects_;
  scoped_refptr<const subresource_filter::MemoryMappedRuleset> installed_filter_ruleset_;
  bool installed_ad_block_filter_ = false;
  mojo::AssociatedRemote<mojom::RendererHost> host_;
  std::vector<base::ScopedClosureRunner> input_listeners_;
  std::pair<bool, bool> last_focus_{false, false};
  mojo::AssociatedReceiverSet<mojom::RenderFrameAgent> receivers_;
  base::WeakPtrFactory<RenderFrameAgent> weak_ptr_factory_{this};
};
}  // namespace

void CreateRenderFrameAgent(content::RenderFrame* frame) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    EnsureRenderProcessAgent();
    new RenderFrameAgent(frame);
  }
}

void UpdateFrameAdBlockCosmetics(content::RenderFrame* frame) {
  auto* reference = static_cast<FrameAgentReference*>(frame->GetUserData(&kFrameAgentKey));
  if (reference && reference->agent) {
    reference->agent->ApplyAdBlockCosmetics();
  }
}

void UpdateFrameAdBlockConfiguration(content::RenderFrame* frame) {
  auto* reference = static_cast<FrameAgentReference*>(frame->GetUserData(&kFrameAgentKey));
  if (reference && reference->agent) {
    reference->agent->AdBlockConfigurationChanged();
  }
}

bool RunEffectsAtDocumentStart(content::RenderFrame* frame) {
  auto* reference = static_cast<FrameAgentReference*>(frame->GetUserData(&kFrameAgentKey));
  if (!reference || !reference->agent) {
    return true;
  }
  auto agent = reference->agent;
  agent->ApplyEffectsAtDocumentStart();
  if (!agent) {
    return false;
  }
  agent->ApplyAdBlockCosmetics();
  return static_cast<bool>(agent);
}
}  // namespace arc
