# Renderer integration, 2026-10-23

## Mechanism and landed hooks

Browser-owned `RendererBridge` follows each WebContents/frame for direct CSS,
Boosts and real blocked-count endpoints. A profile-aware process registry binds
an associated `RenderProcessAgent` per renderer process; the process owns rule
mapping, exclusions and cosmetic cache shared by every frame. Debug selects the
oldest surviving process binding, independent of requested URL. Internal Mojo
is our transport, not a copy of proprietary binary messages.

Patch 0005 registers frame agents from `RenderFrameCreated`, host-gated; patch
0004 forwards the host switch. Core initializes the bridge when creating an
ArcWebContents wrapper. Those hooks are landed and rendered CSS is verified.
Owned first-frame/first-use fallbacks support the new process agent while the
exact startup/launch/profile hooks below remain pending.

## Pending shared changes requested from lead

### Native document-start hook

At the start of `ChromeContentRendererClient::RunScriptsAtDocumentStart`, before
extension dispatch (which can destroy the frame), call:

```cpp
if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app") &&
    !arc::RunEffectsAtDocumentStart(render_frame)) {
  return;
}
```

Declaration/definition exist in `arc/renderer/render_frame_agent.{h,cc}`.
The receiver caches profile configuration without applying it to the current
document. Document-start applies exact-host main-frame Boost CSS and isolated
world 6 JavaScript, then cosmetics for all surviving frames. False reports that
Boost JavaScript destroyed the frame, so Chrome must skip extension dispatch.
An observer callback after
document creation does not establish the recovered native timing contract.

### Process and profile lifecycle

The complete apply-checkable proposal is `renderer_pending_hooks.patch`.
No shared source was changed by the renderer worker.

- Include `arc/renderer/render_process_agent.h` and call
  `arc::EnsureRenderProcessAgent()` in hosted `RenderThreadStarted`.
- Include `arc/common/ad_block_browser.h` and call
  `arc::RendererProcessWillLaunch(host, true)` after existing Chrome setup in hosted
  `ChromeContentBrowserClient::RenderProcessWillLaunch`. Native registration
  sequence is reserved here, before readiness/acknowledgment. The true argument
  is valid only when the paired renderer-thread startup hook is included in
  the same build. The default false argument safely delays actual binding
  until a frame acknowledgment proves the process binder is registered.
- Declare/call `arc::InitializeRendererProfile(profile)` when Profile is created
  or adopted. Lead-owned `arc_browser_context.mm::initWithProfile:` is the
  earliest wrapper adoption point in the proposal. Actual earlier Profile
  birth is preferable if it can predate wrapper adoption and feature changes.

Global manager rules are optional and differ from retained per-profile rules.
Disable clears the global pair without sending updates; profile/process rules
remain. Enabled registration inherits profile snapshots; disabled registration
receives no initial rules/exclusions. Reenable applies an engaged global pair
(if any), then exclusions; it does not substitute profile caches into already
bound false-born processes. Explicit exclusions propagate even while disabled.

### Runtime feature lookup and refresh

App worker implemented `bool ArcRuntimeFeatureEnabled(NSUInteger)` and made
its public getter delegate to that same table/ID45 special case. Lead must
still declare it in `arc_internal.h`; renderer will then switch its temporary
public-getter fallback to avoid extra native boundary calls.

App feature11 setter must call `arc::RefreshRendererAdBlockFeature()` outside
the feature lock, with a weak no-op caller fallback if renderer may be absent.
The proposal contains this change. Refresh updates the global manager/profile
state; renderer filtering itself is not gated by feature11. Calling refresh
inside the feature lock deadlocks when the native helper reads the table.

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

### PiP process policies requested by core

`renderer_pip_blink_proposal.patch` preserves exact native process-global flags
and three consumer locations. It passes apply-check and all three modified
translation units pass actual Base flags using temporary VFS overlays; no
Chromium files were changed. See `logs/wp/renderer-pip-proposal/REVIEW.md`.
Core keeps public selectors; renderer will supply common/IPC forwarding after
the policy hooks land. Shared declarations requested:

```cpp
namespace arc {
void SetRendererSkipUserActivationForPiP(content::WebContents*, bool);
void SetRendererIgnoreDisablePiPAttribute(content::WebContents*, bool);
}
```

Native defaults are skip=false, ignore=true. Ignore affects eligibility only,
not AttributeChanged exit. Video skip bypasses activation/token consumption;
document PiP consumes/checks first and only then includes the skip flag.
No per-tab storage or navigation replay should be added.
