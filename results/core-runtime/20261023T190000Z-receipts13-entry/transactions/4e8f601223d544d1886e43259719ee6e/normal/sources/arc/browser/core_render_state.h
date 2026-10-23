// Copyright 2026 ArcMod. Display state from submitted compositor metadata.
#ifndef ARC_BROWSER_CORE_RENDER_STATE_H_
#define ARC_BROWSER_CORE_RENDER_STATE_H_

#import "arc/api/arc_internal.h"

#include <memory>

namespace arc {
class CoreRenderState {
 public:
  virtual ~CoreRenderState() = default;
};
std::unique_ptr<CoreRenderState> CreateCoreRenderStateObserver(
    ArcWebContents* wrapper, content::WebContents* contents);
}  // namespace arc

#endif  // ARC_BROWSER_CORE_RENDER_STATE_H_
