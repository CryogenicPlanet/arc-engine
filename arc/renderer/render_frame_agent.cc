// Copyright 2026 ArcMod. Frame-local stylesheet injection through Blink.
#include "arc/renderer/render_frame_agent.h"

#include <map>
#include <string>
#include <utility>

#include "arc/common/renderer.mojom.h"
#include "base/command_line.h"
#include "base/functional/bind.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/public/web/web_local_frame.h"

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

  void DidCreateNewDocument() override { stylesheets_.clear(); }
  void OnDestruct() override { delete this; }

 private:
  void Bind(mojo::PendingAssociatedReceiver<mojom::RenderFrameAgent> receiver) {
    receivers_.Add(this, std::move(receiver));
  }
  std::map<std::string, blink::WebStyleSheetKey> stylesheets_;
  mojo::AssociatedReceiverSet<mojom::RenderFrameAgent> receivers_;
};
}  // namespace

void CreateRenderFrameAgent(content::RenderFrame* frame) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    new RenderFrameAgent(frame);
  }
}
}  // namespace arc
