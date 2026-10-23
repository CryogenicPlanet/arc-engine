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
| Profiles create/switch | `arcmod-profiles` | ready-to-record; runtime unverified | Exact owned name field; guarded Create-only or Next/sole-Space selection; exact destination warning and Switch it up. Observe assignment, select exact profile names, prove independent A markers, return old marker, then new-PID/same-profile persistence for both. No private state mutation or action retry. |
| Spaces create/switch | `arcmod-spaces` | ready-to-record; runtime unverified | Checked one initial Space; one exact owned Space-name setter and Create button; require two Space entries and fixed new name. Previous/Next must restore exact A/B selection, then repeat in a new PID with the same no-symlink disposable profile. Static identifiers/bindings are recovered; live editor behavior, placement and persistence remain unverified. |
| Single Little Arc | `arcmod-little-arc` | unsupported; guaranteed inconclusive | Current agent Info.plist marker disables external Little Arc and singleton consolidation, and redirects New Little Arc to full windows. Full morning plan is exported, but the normal `windows` scenario tests the agent redirect only. |
| App-event key shortcuts | `arcmod-key-shortcuts` | ready-to-record; unrecorded until morning | Tests-owned `arcmod-key-driver.m` sends only seven whitelisted keyDown/keyUp pairs through public NSApplication.sendEvent inside the exact disposable PID and already-key observed window. Fixed native outcomes cover Cmd+T/L/comma/W/Shift+T/N/Option+N; optional local-monitor observations are retained in receipts. This tests app-event/key-equivalent routing, not OS hardware/WindowServer delivery or keyboard layouts. |
| Import from disposable fixture | `arcmod-disposable-import` | ready-to-record; runtime unverified | One explicit directory/HTML, native13/public12 descriptor and bookmark mask2, two live dedicated targets, one import, five own-delegate sidecar events, exact folder/leaf result and new-PID/same-profile persistence. Static route and code reviewed; no ADK callback/date/parity claim. |

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
unrecorded; the requested ten now have nine complete scripts and one blocked
plan (true Little Arc).

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

1. Profile creation/switching now has a complete write-only script. Static bindings
   prove name field `importMenuFeatureNewProfileTextField` (0x1015.c:55051),
   picker `spacePicker` (:39865-39867), valid-name/nonempty-Space validity
   (:30175-30233), bound sole-Space selection (:57263-57309) and the exact
   destination warning/`Switch it up` (:31800-32020). The first recording must
   still observe those owned controls, exact selected-name readback and document
   replacement. The scenario fails on unknown sheets, ambiguous controls or
   uncertain dispatch. Bounded read-only readiness polls precede one submit
   action. Independent review is CLEAN; frozen SHA and validation receipt are
   `logs/wp/scenarios-profiles-freeze.json`. No native run has occurred.

2. Single Little Arc: provide an explicitly isolated disposable bundle/harness
   retaining the human policy, without touching ArcMod's human app/data or
   removing the ordinary agent safety isolation. Its profile replacement and
   repeated external URL delivery need exact PID/window ownership evidence.
   Current redirect: `../arc/mods/hello/ArcMod.m:48-53`; skipped consolidation:
   `:224-228`; external-link preference override: `:295-297`. Historical
   `../arc/notes/local-mode.md:50-59` validates a separate singleton mode, but
   the current harness identity guards (`tests/launch-slice/run.ts:33,41`)
   reject that bundle; it is not a drop-in substitute for trace156/ours156.
   Read-only follow-up found no existing safe opt-in. A precise UNAPPLIED
   sibling-owner proposal is `logs/wp/scenarios-little-arc-policy-proposal.md`
   (Chirp #140942): explicit `--single-little-arc-parity` accepted only for
   trace156/ours156, typed plist marker with exact session/bundle/name/app checks,
   and exemption only from the three Little Arc exclusions. Agent identity,
   profile isolation, local service policy and ordinary defaults remain. Actual
   external URL delivery, native panel/profile controls and replacement ordering
   still need observation; this proposal does not make the case ready.
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
4. Disposable import now has the complete reviewed three-file script. The parent
   static receipt `logs/wp/scenarios-import-route-approved.json` pins original
   type13/public12/bookmark-mask2 transport, public metadata, exact directory
   parser and nine evidence artifacts. It closes a static preflight only and
   explicitly does not authorize native recording. The selected valid route
   preserves the supplied directory; it does not select a discovered human
   Chrome/Safari profile. A fresh no-follow directory contains exactly one HTML
   file with separate structural lines. No ZIP/bare-file path is inferred.
   The script creates two dedicated live bookmark target folders and makes one
   typed public import call with empty allowed extensions and skip-existing-
   cookies true. A retained own public-protocol delegate requires exactly five
   callback sidecars, nil errors and matching profile scalars. Those callbacks
   are not canonical ADK ImportDelegate recorder rows. Final fixed folder/title/
   URL structure and new-PID/same-profile persistence are asserted; date parity,
   callback threading and whole-import parity are unclaimed. Genuine create
   forwarding and exact raw public-call/return gates remain required.
   Fresh review fixed actual storage identity and restored-target reuse. Harness
   typed plist guards now run before setup and every launch; each import stage
   creates a new stage-specific fixture after driver readiness. Strict clang,
   Bun/strict TypeScript and six actual malformed-bundle CLI rejections pass;
   no apps/fixtures were launched by those negative checks. Proof is frozen in
   `logs/wp/scenarios-import-code-freeze.json` and related import receipts.
   Native success, callback order and persistence await first unlocked capture.
   The replacement context import is still a real stub. App/profile owners need
   descriptor/target/delegate seams preserving this explicit export-directory
   route; generic C156 Library/Safari discovery is not equivalent. Requests
   #140811/#141000 and profile #140912 record that remaining implementation gap.

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
