# ArcMod morning parity queue

Written 2026-10-23 while loginwindow owns the locked screen. No new morning
scenario has been recorded or accepted. Syntax checks do not establish native UI
behavior. `ready-to-record` means a complete fixed-assertion script is present;
the first unlocked recording can still expose missing native accessibility or an
engine failure. No script synthesizes global input or touches a human profile.

| Requested coverage | Scenario | State | Fixed checks / remaining gap |
| --- | --- | --- | --- |
| Settings open/close | `arcmod-settings` | ready-to-record | One initial browser window, exactly one additional Settings window, close that window's observed AXCloseButton, browser and fixture survive. Requires Settings to expose a separate window. |
| Command bar | `arcmod-command-bar` | ready-to-record; runtime unverified | One observed New Tab setup if needed, one public current-editor AX setter, exact seeded fixture A field and actual same-window suggestions, new app-origin autocomplete with paired true return, complete main raw stack and ordinary graceful quit. No navigation/block-completion/parity claim; `command-bar-navigation.ts` remains separate coverage. |
| Close/restore tab | `arcmod-tabs-restore` | ready-to-record | Existing native archive/restore flow, selected fixture URL, old target gone, fixed restored document. Uses `tabs-restore.ts`. |
| Local mode + service filtering | `arcmod-local-services` | ready-to-record; unrecorded until morning | Six exact agent-domain preferences false; Chromium DNS rejection and loopback recovery. Tests-owned `arcmod-native-services-driver.m` also probes the exact public NSURLSession factory and getaddrinfo inside the same PID: blocked firestore.googleapis.com must return ArcMod's exact -1009 policy failure/EAI_NONAME, with loopback HTTP200 and numeric DNS controls. Native Foundation/libc proof is sidecar assertions, outside the engine boundary recorder's method scope. |
| Agent MCP tools | `arcmod-agent-mcp` | ready-to-record | Actual stdio initialize/list plus all 15 tools. Attach to exact harness PID/slug/port; separate pages, redacted fixture password, trusted text/click, stale element/native/page rejection, navigate/back/forward, PNG, PID-bound observed native New Window, exact close, detach preserves externally launched app. |
| Profiles create/switch | `arcmod-profiles` | probe-only; guaranteed inconclusive | Captures enabled state of observed New Profile, then reports blocked/unimplemented mutation. Static name-field ID importMenuFeatureNewProfileTextField, picker spacePicker and submit labels are recovered. Creation selected-Spaces/behavior and exact move/reload confirmation flow remain unresolved. Full fixed isolation/persistence plan and known controls are exported. |
| Spaces create/switch | `arcmod-spaces` | ready-to-record; runtime unverified | Checked one initial Space; one exact owned Space-name setter and Create button; require two Space entries and fixed new name. Previous/Next must restore exact A/B selection, then repeat in a new PID with the same no-symlink disposable profile. Static identifiers/bindings are recovered; live editor behavior, placement and persistence remain unverified. |
| Single Little Arc | `arcmod-little-arc` | unsupported; guaranteed inconclusive | Current agent Info.plist marker disables external Little Arc and singleton consolidation, and redirects New Little Arc to full windows. Full morning plan is exported, but the normal `windows` scenario tests the agent redirect only. |
| App-event key shortcuts | `arcmod-key-shortcuts` | ready-to-record; unrecorded until morning | Tests-owned `arcmod-key-driver.m` sends only seven whitelisted keyDown/keyUp pairs through public NSApplication.sendEvent inside the exact disposable PID and already-key observed window. Fixed native outcomes cover Cmd+T/L/comma/W/Shift+T/N/Option+N; optional local-monitor observations are retained in receipts. This tests app-event/key-equivalent routing, not OS hardware/WindowServer delivery or keyboard layouts. |
| Import from disposable fixture | `arcmod-disposable-import` | unsupported; guaranteed inconclusive | Emits one Netscape bookmark file under this run's outDir; never selects real CHROME/SAFARI sources. File contains exact folder `ArcCore Fixture Folder`, title `ArcCore Imported Bookmark 7f3a`, href `<fixture base>/arccore-import-fixture`, ADD_DATE 1700000000, no credentials/history/cookies. Existing wizard has no observed explicit file/source-root selector. |

## Additional app-owner morning case

`application-termination-denial` is **ready-to-record after unlock, runtime
unverified**, separate from the ten requested ArcMod scripts. It covers the
app owner's #139590 gap: a trusted live beforeunload fixture, native Quit Arc,
Cancel, the same responsive PID/document/handler, a second Quit, then OK and
browser/recorded-helper disappearance within15seconds. The real userQuitApp:
menu route reaches public NSApp terminate:. Actual exact-session Page dialog
events and fixture binding sidecars are distinguished from the full boundary
recorder's two app-origin ArcBrowserApplication.tryToTerminate: pairs.

The tests-owned raw module enforces complete caller attribution/hooks, paired
main-thread receiver/caller/depth, exactly one termination call per phase and no
calls outside phases, and retained complete byte prefixes. Final native action
uses existing PID/fingerprint validation plus receipt without a post-exit native
snapshot. Unexpected app quit confirmation, uncertain action, changed process
identity or missing control/event fails; no action retry, Browser.close or
scenario signal contributes to acceptance. Process disappearance is observed;
OS exit status/block completion are not. Bounded existing matching crash reports
are checked, with delayed publication explicitly unobserved.

Bun/build/strict TypeScript and fresh review pass. The archived SINGLE-phase
parser positive and real non-app child observation are offline proof only;
20 corruption/error cases reject. A final timing-only correction separately
records first observed absence and proof-writing times. Frozen inputs and exact
check receipts are `logs/wp/scenarios-termination-{freeze.json,offline-check.ts,offline-check.json}`.
The actual native two-Quit flow has never run. All eleven morning scripts remain
unrecorded; the requested ten still have seven complete scripts and three blocked
plans.

## Recording after unlock

The parent scenarios worker owns recordings, profile reset, the build/run lock,
promotion, `REPORT.md` and the scoreboard. Use the normal `record-golden.ts`
workflow, one scenario at a time with a clean `ArcModAgent-trace156` data
directory. Do not record any row marked probe-only/unsupported as an accepted
golden. Candidate acceptance still requires every step to pass and nonempty valid
boundary transcripts; aliases above are separate scenarios with separate outputs.

`arcmod-command-bar` uses tests-owned `arcmod-command-bar-driver.m` and
`arcmod-command-bar-raw.ts`. It adapts the frozen omnibox pair's current-editor
route with exact PID/bundle/nonce, private outDir requests, pinned window and a
latched single setter. Cursor device/inode and an exact complete raw prefix bind
the query to that mutation. The raw gate retains all main callbacks/getters and
permits only the same already-open app run frame. Both archived positives and
24 corrupted cases passed offline validation; strict clang/Bun/TypeScript and
fresh review passed. These checks do not execute the morning UI. Its standard
graceful quit is still a required future assertion; the archived pair used
SIGTERM cleanup. No AXConfirm navigation or block invocation is inferred.

`arcmod-spaces` now has a complete guarded script. Static app strings name
`spaceCreationSpaceNameTextField` and `spaceCreationCreateSpaceButton`; decompiled
Create Space binding and name state are cited in the source. The first runtime
recording must still observe exactly one owned enabled/settable field and one
advertised button. It requires one initial dynamic Space entry, exactly two after
creation, an exact fixed new name, Previous/Next selecting fixture A/B, then a real
new-PID restart and repeated selection/persistence checks. The original name may
be empty. It does not infer default tab counts or profile assignment. No action is
retried after uncertain dispatch. Profile/storage symlinks and non-directories
are rejected before following paths; canonical parent/device/inode are rechecked
across both launches. Bun/strict TypeScript and fresh independent review passed;
no app, native action or recording was run. Source SHA256:
`fd04d27294868cd5e564ceda2c750bd82e7cb935fa0f3c1b8e19f9a353fca9d0`.

`arcmod-agent-mcp` explicitly attaches to the harness's running agent. It never
calls `open_session` without `attach:true`, never builds another bundle, and
closes only pages it created. The subprocess holds the cooperative agent lease
until detach; while it is attached, scenario actions use MCP or PID-bound native
state rather than the separately leased CDP CLI. MCP state is not a CDP target ID.
The harness retains shutdown ownership. The fixture screenshot and tool call
names are saved under outDir for review; stdout remains JSON-RPC only.

## Requests to lead

1. Profile creation/switching: establish creation selected-Spaces/behavior and the
   exact move/reload confirmation branch/actions after unlock before mutations. The menu commands are **not
   inherently disabled**: accepted `results/golden/arc-1.170-context-menus/native-1.json`
   exposes enabled New Profile (:1769), New Space… (:3817) and Change Profile
   (:3868); older bookmarks-import snapshots expose disabled commands.
   Background/menu-validation state can affect this. `../arc/notes/shortcut-audit.md:16-17`
   independently records change-profile/naming sheets opened then cancelled,
   but gives no control labels. `../arc/mods/hello/ArcMod.m:68-69`'s local allowlist
   omits these commands while :81-85 retain original validation, which does not
   establish an account/local-mode gate. The guaranteed-inconclusive profile probe
   observes the current exact state without bypassing it. Fixed profile/Space
   names, storage markers, tab selection and persistence assertions are written;
   a targeted static scout subsequently recovered shared name-field ID
   `importMenuFeatureNewProfileTextField` (0x1015.c:55051), picker `spacePicker`
   (:39865), profile display-name rows, and Create Profile/Change Profile/Next/
   Back/Cancel labels (:31216-31336). These are exported as `morningControls`.
   Change Profile→New Profile writes an input enum (:40630-40638), rather than
   proving creation behavior or focused-Space preselection; context constructor
   :30082-30112 accepts those unresolved caller arguments (0x1029.c:81898).
   Move/reload warning branch/actions remain unproved. Do not guess or mutate
   private context fields to finish the flow.
   Spaces now has the separately reviewed write-only script described below.

2. Single Little Arc: provide an explicitly isolated disposable bundle/harness
   retaining the human policy, without touching ArcMod's human app/data or
   removing the ordinary agent safety isolation. Its profile replacement and
   repeated external URL delivery need exact PID/window ownership evidence.
   Current redirect: `../arc/mods/hello/ArcMod.m:48-53`; skipped consolidation:
   `:224-228`; external-link preference override: `:295-297`. Historical
   `../arc/notes/local-mode.md:50-59` validates a separate singleton mode, but
   the current harness identity guards (`tests/launch-slice/run.ts:33,41`)
   reject that bundle; it is not a drop-in substitute for trace156/ours156.
3. Key shortcuts now have a tests-owned in-process AppKit driver. It constructs
   public NSEvent keyDown/keyUp objects and calls NSApplication.sendEvent for an
   exact observed agent window, refusing dispatch if that window is no longer key.
   This exercises BrowserApp's real app-event path and key-equivalent routing;
   it does not test physical keyboard/WindowServer delivery or arbitrary layouts.
   Native AX actions only select fixtures, confirm command-bar setup and close
   Settings; they do not substitute for any of the seven tested shortcuts.
   The sibling inventory supplies the exact default bindings in
   `../arc/resources/shortcut-menu-inventory.json`; `../arc/notes/perf-wins.md:18`
   documents BrowserApp.sendEvent forwarding to super. No recording was attempted.
4. Import: expose an explicit fixture file/source-root input in the importer or
   supply a disposable browser source root that the wizard can select without
   discovering the user's profiles. Never click a discovered CHROME/SAFARI source
   merely to inspect its contents. Existing `bookmarks-import/native-3.json:1347-1392`
   exposes CHROME/SAFARI/Maybe Later/Next only. `../arc/tools/import-arc-data.ts:136,148`
   hardcodes human Arc data and Keychain access, so it must not be used for this
   fixture. The generated fixture is ready for an explicit safe input.
   Read-only follow-up found public
   `ArcImportSourceProfile.initWithSafariExportedData:profileName:name:categories:`
   (runtime TSV:874) and the public context import entrypoint (:2387), but the
   initializer only copies a supplied path into custom native source type13.
   The original archive/file/directory format and absence of real-profile fallback
   are unverified. Modern Chromium Safari ZIP support is not proof of this custom
   original importer. Do not pass the bare HTML or guess a ZIP layout.
   The missing already-exported original body is `FUN_083680dc` in research
   snapshot `/home/exedev/work/inputs/decompile/0x83.c`, byte2002662/5304Cbytes;
   shard SHA256 `37105cd32dd0b385f4b2ff4154e5e46eab8a33ef6e18c156830f917c91ccc2a1`.
   Evidence: research `arc-layer/decompile/handoff-audit.tsv:1886` and
   `corpus-inventory.tsv:34`. Lead request #139599 asks for that bounded body and
   format/path-resolution callees from an authorized snapshot. No VM/SSH/resume
   was attempted. Import targets require two live valid bookmark nodes (invalid
   supplied nodes can trap); the delegate is weakly held and fresh callback
   profile wrappers must be compared by scalar fields. These are additional
   preconditions, not proof that import is ready.
5. Native services now have a tests-owned own-PID Foundation/libc probe using
   NSURLSession's designated public factory and getaddrinfo. It uses ephemeral
   sessions without cookie/credential/cache storage, preserves TLS/ATS policy,
   starts each request once and polls only cached state. The blocked host is
   checked against `../arc/resources/local-service-policy.json`; expected -1009
   plus the exact local-mode error message and EAI_NONAME come from
   `../arc/mods/local/ServicePolicy.m`. No page fetch is used as proof of the native
   policy. Morning recording must verify protocol installation, error wrapping,
   loopback HTTP permission and dyld interposition in the signed disposable app.

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
