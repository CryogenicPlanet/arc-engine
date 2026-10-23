# Renderer integration request to lead

Mechanism: browser-owned `RendererBridge` sends associated Mojo messages to the
primary main frame. `arc::CreateRenderFrameAgent` registers the per-frame receiver
and owns a `RenderFrameObserver` using Blink's native stylesheet API. Both sides
link into ArcCore.framework. No browser-side binder is needed for this direction
of associated IPC. `ChromeContentBrowserClient` must forward the host-mode switch
so the receiver can register in child renderer processes.

Please apply the following minimal changes as a host-mode Chromium patch (worker
did not edit patches/ or chromium/src/). Locations refer to Chromium 156.

`chrome/renderer/chrome_content_renderer_client.cc`, include block:

```cpp
#include "arc/renderer/render_frame_agent.h"
```

Start of `ChromeContentRendererClient::RenderFrameCreated` (currently line 652):

```cpp
  // CreateRenderFrameAgent also checks the switch as defense in depth.
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    arc::CreateRenderFrameAgent(render_frame);
  }
```

`chrome/renderer/BUILD.gn`, `static_library("renderer")` deps:

```gn
    "//arc/renderer",
```

`chrome/browser/chrome_content_browser_client.cc`, start of
`ChromeContentBrowserClient::AppendExtraCommandLineSwitches` (currently line 2926):

```cpp
  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app") &&
      command_line->GetSwitchValueASCII(switches::kProcessType) ==
          switches::kRendererProcess) {
    command_line->AppendSwitch("arcmod-host-app");
  }
```

The `arc/wp/renderer.gni` dependency already links `//arc/renderer` into ArcCore.
The direct renderer dependency above supplies include visibility for its hook.

## Remaining acceptance gates

- Existing golden coverage: stylesheet insertion, top-band delegate setter,
  blocked-count zero state, empty excluded domains. No sampled-color callbacks.
- Missing goldens: Boost CSS/JS register/update/remove (including host grammar,
  injection world/timing and navigation), color matrices/schemes, ad-block list
  format and a blocked resource/debug report, extraction success/failure.
- The extraction request completion trampoline `0x43f9e60` is not in the retained
  corpus. The specs do not determine block arity, native node-type values,
  redaction, cross-origin rules or extraction-time units. Do not substitute an
  arbitrary DOM/accessibility schema or invent a callback signature.
- The package brief's `insertStylesheet:` is a typo: exact metadata/golden
  selector is `insertStylesheet:stylesheet:` (identifier, CSS string).
