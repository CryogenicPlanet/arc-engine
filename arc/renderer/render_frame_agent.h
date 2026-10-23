// Copyright 2026 ArcMod.
#ifndef ARC_RENDERER_RENDER_FRAME_AGENT_H_
#define ARC_RENDERER_RENDER_FRAME_AGENT_H_

namespace content {
class RenderFrame;
}

namespace arc {
// Called only in hosted Arc mode by ChromeContentRendererClient.
void CreateRenderFrameAgent(content::RenderFrame* frame);
// ChromeContentRendererClient calls this before extension document-start code.
// False means a script destroyed the frame; the caller must return immediately.
bool RunEffectsAtDocumentStart(content::RenderFrame* frame);
// Process configuration publication visits every live renderer frame.
void UpdateFrameAdBlockConfiguration(content::RenderFrame* frame);
}  // namespace arc

#endif  // ARC_RENDERER_RENDER_FRAME_AGENT_H_
