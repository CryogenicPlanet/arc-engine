# Renderer integration, 2026-10-23

## Mechanism and landed hooks

Browser-owned `RendererBridge` follows every live frame and sends associated
Mojo configuration/requests. `arc::CreateRenderFrameAgent` installs a frame
observer and receiver in Chromium's hosted renderer. Both sides link into
ArcCore.framework. Patch 0005 registers the agent from
`ChromeContentRendererClient::RenderFrameCreated`, gated by `arcmod-host-app`;
patch 0004 forwards that switch to renderer processes. Core initializes the
bridge when it creates an ArcWebContents wrapper. These hooks are landed and
actual original/ours stylesheet rendering has been verified.

## Pending shared changes requested from lead

### Native document-start hook

At the start of `ChromeContentRendererClient::RunScriptsAtDocumentStart`, before
extension dispatch (which can destroy the frame), call:

```cpp
if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app"))
  arc::RunEffectsAtDocumentStart(render_frame);
```

Declaration/definition exist in `arc/renderer/render_frame_agent.{h,cc}`.
The receiver caches profile configuration without applying it to the current
document. Document-start applies exact-host main-frame Boost CSS and isolated
world 6 JavaScript, then cosmetics for all frames. An observer callback after
document creation does not establish the recovered native timing contract.

### Runtime feature lookup and refresh

Declare `bool ArcRuntimeFeatureEnabled(NSUInteger identifier)` in lead-owned
`arc_internal.h`. Implement in app-owned `arc_app_flags.mm` using the same
locked runtime table and ID45 special case as the public getter, then make
the public getter call it. Renderer reads ID11; webextras also needs ID44.
This avoids extra app-to-engine getter events from native renderer plumbing.

After changing runtime feature 11, call `arc::RefreshRendererAdBlockFeature()`
outside the feature table lock. Declare it through the shared header and keep
a weak no-op fallback in the caller while the package may be absent. Renderer
provides the strong implementation. Calling inside the feature lock deadlocks
when refresh reads the table. Existing renderer fallback uses the observed
application's public getter until the native entry point is provided.

### Blink visual hooks

See `RENDERER_VISUAL_GAPS.md` for the recovered Page-scoped optional color scheme
(higher precedence than media/preference overrides), Page/paint HSLA filter,
and native top-band tracker. These require lead-owned Blink/cc patches. No
WebSettings, CSS/SVG, or root-background approximation was added.

## Golden coverage

Final accepted scenarios cover direct stylesheet insertion/removal, top-band
delegate registration, zero blocked counts and empty excluded domains. They
do not exercise Boost registration, active ad blocking/debug, extraction, color
filters/schemes, or actual top-band callbacks. Renderer-owned bounded probes
now invoke genuine original APIs for value DTOs, stylesheet rendering, Boost
timing, enabled ad blocking and annotated page extraction. Their raw outputs
are retained separately from accepted scenario goldens. Scenarios worker has
been asked to add coverage; native UI coverage currently also depends on the
desktop's accessibility state.

The brief's `insertStylesheet:` is a typo: metadata and original calls require
`insertStylesheet:stylesheet:` (identifier, CSS string).
