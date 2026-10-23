// Copyright 2026 ArcMod. Browser-side renderer transport for ArcWebContents.
#import "arc/api/arc_internal.h"

#include <limits>
#include <map>
#include <utility>

#include "arc/common/ad_block_browser.h"
#include "arc/common/renderer.mojom.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"

@interface ArcWebContents (RendererCountAccessors)
@property(nonatomic) int blockedResourceCount;
@end

namespace arc {
// The native WebContents owns the bridge. No wrapper or frame is kept alive.
class RendererBridge : public content::WebContentsObserver,
                       public content::WebContentsUserData<RendererBridge>,
                       public mojom::RendererHost {
 public:
  static RendererBridge* For(content::WebContents* contents) {
    if (!contents) {
      return nullptr;
    }
    CreateForWebContents(contents);
    return FromWebContents(contents);
  }

  mojom::RenderFrameAgent* Agent() {
    auto* frame = web_contents()->GetPrimaryMainFrame();
    if (!frame || !frame->IsRenderFrameLive()) {
      return nullptr;
    }
    return FrameAgent(frame);
  }

  void RenderFrameCreated(content::RenderFrameHost* frame) override {
    FrameAgent(frame);
  }

  void RenderFrameDeleted(content::RenderFrameHost* frame) override {
    const auto it = agents_.find(frame->GetGlobalId());
    if (it != agents_.end()) {
      hosts_.Remove(it->second.host_receiver);
      agents_.erase(it);
    }
  }

  void ReportDisallowedLoad() override {
    auto* frame = content::RenderFrameHost::FromID(hosts_.current_context());
    if (!frame || !frame->IsActive() ||
        content::WebContents::FromRenderFrameHost(frame) != web_contents()) {
      return;
    }
    ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:web_contents()];
    if (wrapper.blockedResourceCount < std::numeric_limits<int>::max()) {
      // Setter dispatch preserves Arc's observable KVO property contract.
      wrapper.blockedResourceCount = wrapper.blockedResourceCount + 1;
    }
  }

 private:
  friend class content::WebContentsUserData<RendererBridge>;
  explicit RendererBridge(content::WebContents* contents)
      : content::WebContentsObserver(contents),
        content::WebContentsUserData<RendererBridge>(*contents) {
    subscription_ = ObserveAdBlockConfiguration(base::BindRepeating(
        &RendererBridge::ConfigurationChanged, weak_ptr_factory_.GetWeakPtr()));
    // Also initialize already-live frames when a caller attaches late.
    contents->ForEachRenderFrameHost([this](content::RenderFrameHost* frame) {
      FrameAgent(frame);
    });
  }

  struct FrameConnection {
    mojo::AssociatedRemote<mojom::RenderFrameAgent> agent;
    mojo::ReceiverId host_receiver = 0;
  };

  mojom::RenderFrameAgent* FrameAgent(content::RenderFrameHost* frame) {
    if (!frame || !frame->IsRenderFrameLive()) {
      return nullptr;
    }
    auto& connection = agents_[frame->GetGlobalId()];
    if (!connection.agent.is_bound() || !connection.agent.is_connected()) {
      connection.agent.reset();
      frame->GetRemoteAssociatedInterfaces()->GetInterface(&connection.agent);
      ConfigureFrame(frame, connection);
    }
    return connection.agent.get();
  }

  void ConfigureFrame(content::RenderFrameHost* frame, FrameConnection& connection) {
    if (connection.host_receiver) {
      hosts_.Remove(connection.host_receiver);
    }
    mojo::PendingAssociatedRemote<mojom::RendererHost> host;
    connection.host_receiver = hosts_.Add(
        this, host.InitWithNewEndpointAndPassReceiver(), frame->GetGlobalId());
    const auto& configuration = GetAdBlockConfiguration();
    connection.agent->ConfigureAdBlock(
        configuration.indexed_ruleset.IsValid()
            ? configuration.indexed_ruleset.Duplicate()
            : base::File(),
        configuration.excluded_origins, configuration.ruleset_id, std::move(host));
  }

  void ConfigurationChanged() {
    web_contents()->ForEachRenderFrameHost([this](content::RenderFrameHost* frame) {
      const auto it = agents_.find(frame->GetGlobalId());
      if (it == agents_.end()) {
        FrameAgent(frame);
      } else if (frame->IsRenderFrameLive()) {
        ConfigureFrame(frame, it->second);
      }
    });
  }

  std::map<content::GlobalRenderFrameHostId, FrameConnection> agents_;
  mojo::AssociatedReceiverSet<mojom::RendererHost, content::GlobalRenderFrameHostId> hosts_;
  base::CallbackListSubscription subscription_;
  base::WeakPtrFactory<RendererBridge> weak_ptr_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(RendererBridge);

void InitializeRendererBridge(content::WebContents* contents) {
  RendererBridge::For(contents);
}
}  // namespace arc

@implementation ArcWebContents (RendererStylesheets)
- (void)insertStylesheet:(NSString*)identifier stylesheet:(NSString*)stylesheet {
  auto* bridge = arc::RendererBridge::For([self webContents]);
  auto* agent = bridge ? bridge->Agent() : nullptr;
  if (agent) {
    agent->InsertStylesheet(ArcUTF8(identifier), ArcUTF8(stylesheet));
  }
}
- (void)removeStylesheet:(NSString*)identifier {
  auto* bridge = arc::RendererBridge::For([self webContents]);
  auto* agent = bridge ? bridge->Agent() : nullptr;
  if (agent) {
    agent->RemoveStylesheet(ArcUTF8(identifier));
  }
}
@end
