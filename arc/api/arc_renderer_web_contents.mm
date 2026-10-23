// Copyright 2026 ArcMod. Browser-side renderer transport for ArcWebContents.
#import "arc/api/arc_internal.h"

#include "arc/common/renderer.mojom.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"

namespace arc {
// The native WebContents owns the bridge. No wrapper or frame is kept alive.
class RendererBridge : public content::WebContentsObserver,
                       public content::WebContentsUserData<RendererBridge> {
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
    if (!agent_.is_bound() || !agent_.is_connected() || frame_id_ != frame->GetGlobalId()) {
      agent_.reset();
      frame_id_ = frame->GetGlobalId();
      frame->GetRemoteAssociatedInterfaces()->GetInterface(&agent_);
    }
    return agent_.get();
  }

  void RenderFrameDeleted(content::RenderFrameHost* frame) override {
    if (frame_id_ == frame->GetGlobalId()) {
      agent_.reset();
      frame_id_ = {};
    }
  }
  void PrimaryMainFrameRenderProcessGone(base::TerminationStatus) override {
    agent_.reset();
    frame_id_ = {};
  }

 private:
  friend class content::WebContentsUserData<RendererBridge>;
  explicit RendererBridge(content::WebContents* contents)
      : content::WebContentsObserver(contents),
        content::WebContentsUserData<RendererBridge>(*contents) {}
  content::GlobalRenderFrameHostId frame_id_;
  mojo::AssociatedRemote<mojom::RenderFrameAgent> agent_;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(RendererBridge);
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
