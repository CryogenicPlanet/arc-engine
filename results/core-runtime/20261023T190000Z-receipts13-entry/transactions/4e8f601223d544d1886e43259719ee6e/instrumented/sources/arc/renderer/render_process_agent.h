// Copyright 2026 ArcMod.
#ifndef ARC_RENDERER_RENDER_PROCESS_AGENT_H_
#define ARC_RENDERER_RENDER_PROCESS_AGENT_H_

#include <string>
#include "base/memory/scoped_refptr.h"

class GURL;
namespace url { class Origin; }
namespace subresource_filter { class MemoryMappedRuleset; }
namespace arc {
// Call from RenderThreadStarted; CreateRenderFrameAgent also installs a fallback.
void EnsureRenderProcessAgent();
struct CosmeticResult {
  std::string state;
  std::string css;
};
scoped_refptr<const subresource_filter::MemoryMappedRuleset> GetProcessAdBlockRuleset();
bool IsProcessAdBlockOriginExcluded(const url::Origin& origin);
CosmeticResult EvaluateProcessAdBlockCosmetics(const GURL&, const url::Origin&);
}  // namespace arc
#endif  // ARC_RENDERER_RENDER_PROCESS_AGENT_H_
