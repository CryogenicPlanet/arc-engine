// Copyright 2026 ArcMod.
#ifndef ARC_RENDERER_RENDER_FRAME_AGENT_H_
#define ARC_RENDERER_RENDER_FRAME_AGENT_H_

namespace content {
class RenderFrame;
}

namespace arc {
// Called only in hosted Arc mode by ChromeContentRendererClient.
void CreateRenderFrameAgent(content::RenderFrame* frame);
}  // namespace arc

#endif  // ARC_RENDERER_RENDER_FRAME_AGENT_H_
