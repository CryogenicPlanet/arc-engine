// Copyright 2026 ArcMod. Browser-side renderer transport for ArcWebContents.
#import "arc/api/arc_internal.h"

#include <limits>
#include <map>
#include <utility>

#include "arc/common/ad_block_browser.h"
#include "arc/common/effects_browser.h"
#include "arc/common/renderer.mojom.h"
#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "base/strings/sys_string_conversions.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"

@interface ArcWebContents (RendererCountAccessors)
@property(nonatomic) int blockedResourceCount;
@end

@interface ArcWebContents (RendererFieldStateAccessors)
@property(nonatomic) BOOL undoableFieldActive;
@property(nonatomic) BOOL freeformTextFieldActive;
@property(nonatomic) BOOL passwordTextFieldActive;
@property(nonatomic) BOOL hasDirtyTextField;
@property(nonatomic) BOOL hadFormOrUserEdits;
@property(nonatomic, strong) NSString* inputEvent;
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
    InitializeRendererProcessBridge();
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

  // The message came from an active frame of this web contents.
  bool CurrentFrameBelongsHere() {
    auto* frame = content::RenderFrameHost::FromID(hosts_.current_context());
    return frame && frame->IsActive() && content::WebContents::FromRenderFrameHost(frame) == web_contents();
  }

  void ReportDisallowedLoad() override {
    if (!CurrentFrameBelongsHere()) {
      return;
    }
    if (blocked_resource_count_ < std::numeric_limits<int>::max()) {
      ++blocked_resource_count_;
      ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:web_contents()];
      // Setter dispatch preserves Arc's observable KVO property contract.
      wrapper.blockedResourceCount = blocked_resource_count_;
    }
  }

  void PrimaryPageChanged(content::Page&) override {
    ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:web_contents()];
    if (blocked_resource_count_ != 0) {
      blocked_resource_count_ = 0;
      wrapper.blockedResourceCount = 0;
    }
    // A new page starts without edits or a focused field (order as in Arc's engine).
    if (wrapper.hadFormOrUserEdits) wrapper.hadFormOrUserEdits = NO;
    if (wrapper.hasDirtyTextField) wrapper.hasDirtyTextField = NO;
    if (wrapper.undoableFieldActive) wrapper.undoableFieldActive = NO;
    if (wrapper.freeformTextFieldActive) wrapper.freeformTextFieldActive = NO;
    if (wrapper.passwordTextFieldActive) wrapper.passwordTextFieldActive = NO;
  }

  // Field state setters change only on a change, like Arc's engine (the app observes them with KVO).
  void FocusedFieldChanged(bool editable, bool password) override {
    if (!CurrentFrameBelongsHere()) {
      return;
    }
    ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:web_contents()];
    if (wrapper.undoableFieldActive != editable) wrapper.undoableFieldActive = editable;
    if (wrapper.freeformTextFieldActive != editable) wrapper.freeformTextFieldActive = editable;
    if (wrapper.passwordTextFieldActive != password) wrapper.passwordTextFieldActive = password;
  }

  void ReportUserInput(const std::string& event_json, bool text_edit) override {
    if (!CurrentFrameBelongsHere()) {
      return;
    }
    ArcWebContents* wrapper = [ArcWebContents wrapperForWebContents:web_contents()];
    if (text_edit) {
      if (!wrapper.hasDirtyTextField) wrapper.hasDirtyTextField = YES;
      if (!wrapper.hadFormOrUserEdits) wrapper.hadFormOrUserEdits = YES;
    }
    wrapper.inputEvent = base::SysUTF8ToNSString(event_json);
  }


 private:
  friend class content::WebContentsUserData<RendererBridge>;
  explicit RendererBridge(content::WebContents* contents)
      : content::WebContentsObserver(contents),
        content::WebContentsUserData<RendererBridge>(*contents) {
    effects_subscription_ = ObserveProfileEffectsConfiguration(
        Profile::FromBrowserContext(contents->GetBrowserContext()),
        base::BindRepeating(&RendererBridge::EffectsChanged, weak_ptr_factory_.GetWeakPtr()));
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
    ConfigureRendererHost(frame, connection);
    ConfigureEffectsFrame(connection);
  }

  void ConfigureRendererHost(content::RenderFrameHost* frame, FrameConnection& connection) {
    if (connection.host_receiver) {
      hosts_.Remove(connection.host_receiver);
    }
    mojo::PendingAssociatedRemote<mojom::RendererHost> host;
    connection.host_receiver = hosts_.Add(
        this, host.InitWithNewEndpointAndPassReceiver(), frame->GetGlobalId());
    connection.agent->SetRendererHost(std::move(host), base::BindOnce(
        [](content::GlobalRenderFrameHostId id) {
          auto* frame = content::RenderFrameHost::FromID(id);
          if (frame && frame->IsRenderFrameLive()) {
            RendererProcessFrameReady(frame->GetProcess());
          }
        }, frame->GetGlobalId()));
  }

  void ConfigureEffectsFrame(FrameConnection& connection) {
    const auto& configuration = GetProfileEffectsConfiguration(
        Profile::FromBrowserContext(web_contents()->GetBrowserContext()));
    std::vector<mojom::HostEffectsConfigurationPtr> serialized;
    for (const auto& [host, rule] : configuration) {
      auto entry = mojom::HostEffectsConfiguration::New();
      entry->host = host;
      for (const auto& source : rule.stylesheets) {
        entry->stylesheets.push_back(mojom::EffectSource::New(source.identifier, source.source));
      }
      for (const auto& source : rule.scripts) {
        entry->scripts.push_back(mojom::EffectSource::New(source.identifier, source.source));
      }
      serialized.push_back(std::move(entry));
    }
    connection.agent->ConfigureEffects(std::move(serialized));
  }

  void EffectsChanged() {
    web_contents()->ForEachRenderFrameHost([this](content::RenderFrameHost* frame) {
      const auto it = agents_.find(frame->GetGlobalId());
      if (it == agents_.end()) {
        FrameAgent(frame);
      } else if (frame->IsRenderFrameLive()) {
        ConfigureEffectsFrame(it->second);
      }
    });
  }

  std::map<content::GlobalRenderFrameHostId, FrameConnection> agents_;
  mojo::AssociatedReceiverSet<mojom::RendererHost, content::GlobalRenderFrameHostId> hosts_;
  base::CallbackListSubscription effects_subscription_;
  int blocked_resource_count_ = 0;
  base::WeakPtrFactory<RendererBridge> weak_ptr_factory_{this};
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};
WEB_CONTENTS_USER_DATA_KEY_IMPL(RendererBridge);

void InitializeRendererBridge(content::WebContents* contents) {
  InitializeRendererProcessBridge();
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
