// Copyright 2026 ArcMod. Display state from submitted compositor metadata.
#import "arc/browser/core_render_state.h"

#include <cmath>

#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "cc/trees/render_frame_metadata.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_frame_metadata_provider.h"
#include "content/public/browser/render_widget_host_observer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"

@interface ArcWebContents (CoreRenderState)
- (void)setPendingRenderFrameViewportSize:(CGSize)size;
- (void)setRootBackgroundColor:(id)color;
@end

ARC_EXPORT @interface ArcColor4f : NSObject
- (instancetype)initWithR:(float)r g:(float)g b:(float)b a:(float)a;
@end

namespace {
class CoreRenderStateObserver final
    : public arc::CoreRenderState,
      public content::WebContentsObserver,
      public content::RenderWidgetHostObserver,
      public content::RenderFrameMetadataProvider::Observer {
 public:
  CoreRenderStateObserver(ArcWebContents* wrapper, content::WebContents* contents)
      : content::WebContentsObserver(contents), wrapper_(wrapper) {
    ObservePrimaryWidget();
  }

  ~CoreRenderStateObserver() override { ResetWidget(); }

  void RenderFrameHostChanged(content::RenderFrameHost*,
                              content::RenderFrameHost* new_host) override {
    if (new_host && new_host->IsInPrimaryMainFrame()) {
      SetWidget(new_host->GetRenderWidgetHost());
    }
  }

  void RenderViewHostChanged(content::RenderViewHost*,
                             content::RenderViewHost*) override {
    ObservePrimaryWidget();
  }

  void PrimaryPageChanged(content::Page&) override {
    ++generation_;
    ObservePrimaryWidget();
  }

  void DidStartNavigation(content::NavigationHandle* navigation) override {
    if (navigation->IsInPrimaryMainFrame()) {
      ++generation_;
    }
  }

  void RenderWidgetHostDestroyed(content::RenderWidgetHost* widget) override {
    if (widget_observation_.IsObservingSource(widget)) {
      ResetWidget();
    }
  }

  void WebContentsDestroyed() override {
    ResetWidget();
    Observe(nullptr);
  }

  void OnRenderFrameMetadataChangedBeforeActivation(
      const cc::RenderFrameMetadata& metadata) override {
    // These are pending frame properties: publish receipt of real metadata,
    // before Viz activation. Never turn LastRenderFrameMetadata's default
    // values into an event when attaching to a widget.
    ArcWebContents* wrapper __attribute__((objc_precise_lifetime)) = wrapper_;
    if (!wrapper || !web_contents() || !widget_ || !provider_) {
      return;
    }
    const float scale = metadata.device_scale_factor;
    if (!std::isfinite(scale) || scale <= 0) {
      return;
    }
    const CGSize viewport = CGSizeMake(
        static_cast<float>(metadata.viewport_size_in_pixels.width()) / scale,
        static_cast<float>(metadata.viewport_size_in_pixels.height()) / scale);
    const SkColor4f background = metadata.root_background_color;
    const uint64_t generation = generation_;
    auto native = web_contents()->GetWeakPtr();
    auto widget = widget_;
    auto provider = provider_;
    auto self = weak_factory_.GetWeakPtr();

    [wrapper setPendingRenderFrameViewportSize:viewport];
    // A KVO callback can navigate, replace the widget, close native contents,
    // or release the owning wrapper. Do not continue an obsolete frame event.
    if (!self || !native || native->IsBeingDestroyed() || !widget || !provider ||
        generation != self->generation_ || widget.get() != self->widget_.get() ||
        provider.get() != self->provider_.get()) {
      return;
    }
    ArcColor4f* color = [[ArcColor4f alloc] initWithR:background.fR
        g:background.fG b:background.fB a:background.fA];
    [wrapper setRootBackgroundColor:color];
  }

  void OnRenderFrameMetadataChangedAfterActivation(base::TimeTicks) override {}
  void OnRenderFrameSubmission() override {}
  void OnLocalSurfaceIdChanged(const cc::RenderFrameMetadata&) override {}

 private:
  void ObservePrimaryWidget() {
    content::RenderFrameHost* frame = web_contents()
        ? web_contents()->GetPrimaryMainFrame() : nullptr;
    SetWidget(frame ? frame->GetRenderWidgetHost() : nullptr);
  }

  void SetWidget(content::RenderWidgetHost* widget) {
    if (widget == widget_.get()) {
      return;
    }
    ResetWidget();
    if (!widget) {
      return;
    }
    auto* native = content::RenderWidgetHostImpl::From(widget);
    widget_ = native->GetWeakPtr();
    provider_ = native->render_frame_metadata_provider()->GetWeakPtr();
    widget_observation_.Observe(widget);
    provider_observation_.Observe(provider_.get());
  }

  void ResetWidget() {
    ++generation_;
    // The provider belongs to the widget. Detach before widget destruction or
    // replacement can invalidate it, and before releasing widget observation.
    provider_observation_.Reset();
    widget_observation_.Reset();
    provider_.reset();
    widget_.reset();
  }

  __weak ArcWebContents* wrapper_;
  uint64_t generation_ = 0;
  base::WeakPtr<content::RenderWidgetHostImpl> widget_;
  base::WeakPtr<content::RenderFrameMetadataProviderImpl> provider_;
  base::ScopedObservation<content::RenderWidgetHost,
                          content::RenderWidgetHostObserver>
      widget_observation_{this};
  base::ScopedObservation<content::RenderFrameMetadataProvider,
                          content::RenderFrameMetadataProvider::Observer>
      provider_observation_{this};
  base::WeakPtrFactory<CoreRenderStateObserver> weak_factory_{this};
};
}  // namespace

namespace arc {
std::unique_ptr<CoreRenderState> CreateCoreRenderStateObserver(
    ArcWebContents* wrapper, content::WebContents* contents) {
  return std::make_unique<CoreRenderStateObserver>(wrapper, contents);
}
}  // namespace arc
