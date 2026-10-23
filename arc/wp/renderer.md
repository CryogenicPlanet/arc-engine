# Work package "renderer": Arc's renderer-side features
Read `COMMON.md` first. Arc's engine adds renderer-side agents (EffectsAgent, AdBlockerAgent, RenderFrameAgent,
RenderProcessAgent: report section "IPC examples have real plumbing") for Boosts/effects (CSS/JS injection,
HSLA color matrix), the ad blocker (block lists from the app: setBlockLists:, setExcludedDomains:, blocked
resource counts per web contents), page content extraction and top-band color sampling.
Classes: ArcBrowserContextEffects, ArcHSLAMatrix, ArcBrowserContextPageContentExtraction, ArcExtractedPageContent,
ArcContentNode, ArcMetaTagEntry, ArcMetaTagSelector, ArcHtmlToMarkdownResult; ArcWebContents categories for
insertStylesheet:, top band color (ArcWebContentsTopBandColorDelegate), blocked resource counts, page extraction.
First step: decide (and report) the mechanism: Chromium already exposes the needed renderer hooks
(ContentRendererClient/RenderFrameObserver in chrome/renderer, mojo binders) — design new mojom interfaces under
`arc/common/` and renderer code under `arc/renderer/` compiled into the same framework; the minimal Chromium patch
to register them (ChromeContentRendererClient / ChromeContentBrowserClient hooks, --arcmod-host-app gated) goes
in Requests with exact code. The ad blocker may reuse Chromium's subresource_filter or a declarativeNetRequest-like
engine; match Arc's observable behavior (blocked counts, debug info) per the golden.
Files: `arc/api/arc_renderer_*.mm`, `arc/common/**`, `arc/renderer/**`, `arc/wp/renderer.gni`.
Acceptance: diff clean in scenarios with Boosts/effects, ad blocking and page extraction (ask the scenarios package).
