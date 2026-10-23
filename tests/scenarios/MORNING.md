# ArcMod morning parity queue

Written 2026-10-23 while loginwindow owns the locked screen. No new morning
scenario has been recorded or accepted. Syntax checks do not establish native UI
behavior. `ready-to-record` means a complete fixed-assertion script is present;
the first unlocked recording can still expose missing native accessibility or an
engine failure. No script synthesizes global input or touches a human profile.

| Requested coverage | Scenario | State | Fixed checks / remaining gap |
| --- | --- | --- | --- |
| Settings open/close | `arcmod-settings` | ready-to-record | One initial browser window, exactly one additional Settings window, close that window's observed AXCloseButton, browser and fixture survive. Requires Settings to expose a separate window. |
| Command bar | `arcmod-command-bar` | ready-to-record | Existing New Tab/command field/advertised AXConfirm flow, exact fixture B URL/title, unadvertised native action rejected. Uses `command-bar-navigation.ts`. |
| Close/restore tab | `arcmod-tabs-restore` | ready-to-record | Existing native archive/restore flow, selected fixture URL, old target gone, fixed restored document. Uses `tabs-restore.ts`. |
| Local mode + service filtering | `arcmod-local-services` | ready-to-record, partial | Six exact agent-domain local feature preferences false; service host navigation returns ERR_NAME_NOT_RESOLVED and a matching Document failure without an HTTP response; loopback browsing still works. Covers Chromium resolver rules, not NSURLSession interception. |
| Agent MCP tools | `arcmod-agent-mcp` | ready-to-record | Actual stdio initialize/list plus all 15 tools. Attach to exact harness PID/slug/port; separate pages, redacted fixture password, trusted text/click, stale element/native/page rejection, navigate/back/forward, PNG, PID-bound observed native New Window, exact close, detach preserves externally launched app. |
| Profiles create/switch | `arcmod-profiles` | probe-only; guaranteed inconclusive | Captures enabled state of observed New Profile, then reports blocked/unimplemented mutation. A prior golden snapshot exposes it disabled; its native validation reason is unknown. Full fixed isolation/persistence plan is exported in the scenario. |
| Spaces create/switch | `arcmod-spaces` | probe-only; guaranteed inconclusive | Captures observed New Space… gate, then reports blocked/unimplemented mutation. A prior golden snapshot exposes it disabled; its native validation reason is unknown. Full named-Space/tab/persistence plan is exported in the scenario. |
| Single Little Arc | `arcmod-little-arc` | unsupported; guaranteed inconclusive | Current agent Info.plist marker disables external Little Arc and singleton consolidation, and redirects New Little Arc to full windows. Full morning plan is exported, but the normal `windows` scenario tests the agent redirect only. |
| Native key shortcuts | `arcmod-key-shortcuts` | unsupported; guaranteed inconclusive | Exports Cmd+T/L/comma/W/Shift+T/N and agent Little Arc shortcut plan. Native bridge exposes AX actions and no PID-bound key dispatch or menu key-equivalent metadata. Menu action tests do not prove key routing. |
| Import from disposable fixture | `arcmod-disposable-import` | unsupported; guaranteed inconclusive | Emits one Netscape bookmark file under this run's outDir; never selects real CHROME/SAFARI sources. File contains exact folder `ArcCore Fixture Folder`, title `ArcCore Imported Bookmark 7f3a`, href `<fixture base>/arccore-import-fixture`, ADD_DATE 1700000000, no credentials/history/cookies. Existing wizard has no observed explicit file/source-root selector. |

## Recording after unlock

The parent scenarios worker owns recordings, profile reset, the build/run lock,
promotion, `REPORT.md` and the scoreboard. Use the normal `record-golden.ts`
workflow, one scenario at a time with a clean `ArcModAgent-trace156` data
directory. Do not record any row marked probe-only/unsupported as an accepted
golden. Candidate acceptance still requires every step to pass and nonempty valid
boundary transcripts; aliases above are separate scenarios with separate outputs.

`arcmod-agent-mcp` explicitly attaches to the harness's running agent. It never
calls `open_session` without `attach:true`, never builds another bundle, and
closes only pages it created. The subprocess holds the cooperative agent lease
until detach; while it is attached, scenario actions use MCP or PID-bound native
state rather than the separately leased CDP CLI. MCP state is not a CDP target ID.
The harness retains shutdown ownership. The fixture screenshot and tool call
names are saved under outDir for review; stdout remains JSON-RPC only.

## Requests to lead

1. Profile and Space creation: confirm whether signed-out local mode should
   enable these native commands. `../arc/mods/hello/ArcMod.m:68-69`'s
   `ArcModLocalMenuAction` only allowlists window/Settings/omnibox/close actions;
   lines 81-85 still retain original native validation, so absence from that list
   does not itself prove a command is disabled. Actual disabled evidence:
   `results/golden/arc-1.170-bookmarks-import/native-2.json:1766-1771` (New Profile)
   and `:3797-3802` (New Space…). Background/menu-validation state can affect this;
   probe after native fixture selection and the exact File/Spaces menu press.
   Do not bypass disabled menu items with private selectors. After unlocking, observe editor controls
   before implementing creation/switch mutations; fixed names, storage markers
   and expected selection/persistence are already specified in each scenario.
2. Single Little Arc: provide an explicitly isolated disposable bundle/harness
   retaining the human policy, without touching ArcMod's human app/data or
   removing the ordinary agent safety isolation. Its profile replacement and
   repeated external URL delivery need exact PID/window ownership evidence.
   Current redirect: `../arc/mods/hello/ArcMod.m:48-53`; skipped consolidation:
   `:224-228`; external-link preference override: `:295-297`.
3. Key shortcuts: add a lead-reviewed exact-process native key dispatch API with
   receipts and failure on stale PID/window, or identify an existing safe API.
   AXPress and renderer `Input.dispatchKeyEvent` are insufficient for app routing.
4. Import: expose an explicit fixture file/source-root input in the importer or
   supply a disposable browser source root that the wizard can select without
   discovering the user's profiles. Never click a discovered CHROME/SAFARI source
   merely to inspect its contents. The generated fixture is ready for this input.
5. Native services: add an exact-agent in-process probe that performs
   NSURLSession/getaddrinfo against fixed blocked hosts and a loopback control,
   asserting NSURLErrorNotConnectedToInternet / EAI_NONAME. A page fetch cannot
   prove the native Foundation service policy.

## Expected boundary surface, pending real transcripts

These are coverage intentions, not selector counts. Command/restore/Settings/MCP
should exercise ArcBrowser/ArcWebContents lifecycle, pane/view hosting and
navigation; local service rejection should exercise navigation failure delegates.
Profile/Space creation should exercise ArcBrowserContext/ArcBrowser creation and
storage isolation. Native import may exercise ArcImportSource,
ArcImportSourceProfile and ArcBookmarkImportTargets; those exact class names are
present in `arc/tools/arccore-1.170.0-objc.tsv` (there is no ArcProfile/ArcImporter).
Count actual class and
selector coverage only from accepted recorder transcripts after morning runs.
The agent Little Arc redirect is documented behavior in
`../arc/notes/agent-sessions.md`, so claiming singleton coverage from it would
contradict that source. No architecture/spec change is made here.
