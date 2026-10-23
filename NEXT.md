# ArcMod engine: next steps

Goal: keep ArcMod on current Chromium after Arc shuts down on 2026-12-22, by building our own
engine that matches Arc's `ArcCore` interface. Updated 2026-10-23.
Facts, locations, procedures and the step-by-step runbook are in `ENGINE.md`.
Static research is complete and reviewed: report `../arc/notes/engine-feasibility.md`, class and protocol
index `specs/README.md` (146 classes, 1,964 declared methods, 26 protocols; declared coverage, not behavior).

## Evening status (2026-10-23 ~16:00 PDT)

Release engine (`out/Release`, packaged as ArcMod agent `rel156` via `ARC_ENGINE_OUT=out/Release
./package-engine.sh rel156`; runner `logs/run-release.sh <scenarios>`): launch-slice, tabs-navigation,
profiles-public-api, webauthn-dialog, navigation-forms, media, downloads, pdf pass every step with 0 crashes
and 0-1 missing/extra calls vs Arc's engine; password-save, autofill-save, save-offers-accept fail only the
slow quit that Arc's engine also fails. Real-profile copy (APFS clone of ArcMod's data, Chromium 155 profile)
on the release engine: launched twice, quit cleanly, 0 crashes; the copy was deleted
(`logs/run-realcopy.sh`). Tab restoration wasn't observable (background app, locked screen: 0 page targets).
ArcMod READMEs (master e4d3fec, no-login 61402b0) describe the engine and the release build. Next: native-UI
checks on an unlocked screen, then the gaps listed below.

## Afternoon status (2026-10-23 ~13:30 PDT): lead implementing directly

The Sol workers are stopped: every `codex exec` hit the ChatGPT usage limit ("try again at Oct 30th, 2026
10:40 AM"). Their last changes are verified and committed (9a0d00d patches, 43e5792 evidence). Until Oct 30
the lead implements directly; prompts for relaunching are in `logs/wp/*.prompt`.

New scenarios (CDP-only, so they run with the screen locked) and what they found:
- `password-save`: Arc's engine offers the save through `webContents:didSubmitCredentials:`
  (ArcLoginCredentialsSaveRequest). Ours never did. Fixed: ManagePasswordsUIController hook (0004) + request
  answered through PasswordsModelDelegate. Transcript now matches (init, callback, app reads isUpdate).
- `autofill-save`: address/card offers (`didSubmitAddressEntry:`/`didSubmitCreditCardInfo:`). Ours CRASHED on
  address submit: `BrowserWindow::FindBrowserWindowWithWebContents` only returns BrowserViews (null for every
  Arc window). Fixed in 0003 (host mode returns the browser's own window) + `ArcBrowserWindow` now has an
  AutofillBubbleHandler that forwards address/card offers to the app.
- Same transcripts show more gaps, not yet fixed: focused-field KVO setters (`undoableFieldActive`,
  `freeformTextFieldActive`, `passwordTextFieldActive`, `hasDirtyTextField`, `hadFormOrUserEdits`) never update
  after creation; `setInputEvent:` JSON (`{"input":{"id":N,"value":…}}`, `{"click":{"id":N}}`) never sent;
  ours asks `decidePolicyForNavigationAction:` for a form POST where Arc's engine doesn't.
- Profile attributes: `userVisibleName` now reports the directory name while the profile has Chromium's
  default name ("Default", as in every golden); `userColor` still differs (ours #1C1E22, Arc #A8C7FA in dark
  mode): source being identified.

Later the same afternoon:
- Stub inventory (nm of our ArcCore vs the 1.170 metadata): 238 ObjC methods were still logging stubs, 116 of
  them with selectors Arc's app references. These included find in page, zoom, print/save, executeJavascript,
  capture, translate, accessibility tree, color effects, import, favicons, search engines and profile
  create/load/delete. Five Claude subagents (Sonnet; brief `arc/wp/CLAUDE-SUBAGENTS.md`) each write one new
  file; the lead integrates, builds and verifies. `gen_api_table.py` now counts only built sources, so an
  unfinished file can't drop stubs.
- Profiles: `profiles-public-api` (in-process driver, golden on Arc's engine) passes on ours. Arc's semantics:
  create takes the visible name and Chrome picks the directory; get/delete take the directory; error 2 for an
  invalid request, 3 for an unknown profile.
- `save-offers-accept` (driver answers Save): fixed a dev-build DCHECK in core_render_state (null compositor
  on hide); address offers now taken at ChromeAutofillClient::ConfirmSaveAddressProfile (0004) because Arc's
  engine offers them on background tabs too; iOS promo guarded (0003; it crashed on accept).
- Thumbnails: the generator held a standing ThumbnailImage subscription per tab, which makes Chrome capture
  after every load and LoadIfNecessary() restored tabs. Now it subscribes only for a request or when a tab
  is hidden, like Arc's sparse deliveries.

## Morning status (2026-10-23 ~07:00 PDT)

Our ArcCore (Chromium 156 from source + arc/ layer + patches 0001-0006) runs Arc 1.168's unmodified app.
Scoreboard (`tests/scenarios/SCOREBOARD.md`, steps passing on OUR engine vs goldens recorded on Arc's engine):
launch-slice 9/9, launch-slice-regression 9/9, tabs-navigation 11/11, media 4/4, media-pip 5/5,
fullscreen 4/4, extensions-page-actions 6/6, bookmarks-public-api 40/40, renderer-public-api 37/37,
sessions-tab-restart 13/15, sessions-public-api 9/12, pdf 4/5, downloads 2/3; dialogs, context menus,
permissions, cookies/site-settings, devtools, command-bar partial (several need native UI; the screen was
locked overnight, so native-UI scenarios couldn't run on either engine).
Boundary diffs: most remaining differences are engine-internal reads; re-record goldens with the caller-aware
recorder (e7ef669) for app-facing-only diffs.
Release build: `chromium/src/out/Release` (dcheck off), packaged with `build-mod.ts --engine`.
Morning to-do: unlock the desktop and record native-UI goldens (ArcMod parity scenarios are written: profiles,
local mode, Little Arc, command bar, shortcuts, Settings, restore, Spaces, agent tools, import), finish the
partial scenarios, real-profile copy test, then decide when ArcMod's default build switches engines.

## Building our engine (in progress, 2026-10-23)

Rahul: "just go build it" — an ArcCore built from Chromium so the engine can follow Chromium releases.
- [x] Launch-slice harness + traced baseline on Arc's 1.170 engine (`tests/launch-slice/`, `results/launch-slice/`).
- [x] ArcCore.framework from our Chromium 156 build in Arc's layout (`patches/0002`), Arc's 146 classes declared,
      unimplemented methods are logging stubs from real metadata (`arc/api/arc_api_stubs.mm`, `arc/tools/gen_api_table.py`).
- [x] Hosted bootstrap: `initialize:` starts Chrome's browser process inside Arc's NSApplication, `run` runs the loop
      (`patches/0003`: split RunContentProcess, hosted ChromeMain, skip Chrome's AppController/menu/NSApp checks).
- [x] Profiles (first pass): `availableBrowserContextAttributes`, `getBrowserContext:type:`, ArcBrowserContext with
      service wrappers (still stubs inside), ArcSearchEngine, ArcColor (`arc/api/arc_browser_context.mm`, `arc_values.mm`).
- [x] Stubs: property getters return typed placeholders (from real metadata) instead of nil; NSObject skip list
      narrowed (macOS 26 added `-[NSObject addObserver:]`, which collides with Arc's selector).
- [ ] NEXT: `createBrowser:withDelegate:forWindow:` (Chrome Browser + a BrowserWindow that reports to Arc's delegate),
      then WebContents, `nativeView`/pane host views, navigation. App currently stops at
      "ADK.swift:678: Expected a loaded profile '' to be available" right after createBrowser returns nil.
      `loadUrl:withDelegate:` (app's URL loader for remote config) also still a stub.
- Test loop: `./build-babysit.sh && ./try-engine.sh 30` (prints unimplemented calls in order + newest crash).

### Orchestration (Rahul: "feature complete", "1-1 behavior, programmatic", Sol workers grind, Claude leads)
- 1-1 check: `tests/recorder/` (boundary recorder dylib + diff.py); golden transcripts from Arc's engine in
  `results/golden/`; `arc/tools/diff-run.sh [scenario]` builds, runs ours, diffs. Allowed diffs: `arc/DIFF-ALLOWLIST.md`.
- Contract: `arc/ARCHITECTURE.md`; briefs: `arc/wp/COMMON.md` + `arc/wp/<package>.md`; per-package sources in
  `arc/wp/<package>.gni`; build/run lock `arc/tools/locked.sh`.
- Sol workers (codex exec, gpt-6.1-sol high, full access, no commits): logs `logs/wp/<pkg>.log`, report
  `logs/wp/<pkg>.report.md`. Wave 1 running: core, profile, app, scenarios; wave 2 running: ai, renderer, omnibox.
  Wave 2 waiting on core: ui, extensions, webextras, sessions. Lead reviews each report, commits, launches next.
- Board: the lead PULLS chirp projects/arc at every checkpoint (cursor in logs/lead-board-cursor); routing to
  @claude isn't automatic. Workers apply their own reviewed patches (COMMON.md "Integration authority",
  patches/MANIFEST + arc/tools/regen-patches.sh).
- Lead owns: patches/ (host mode 0003 incl. custom BrowserWindow gate), arc_internal.h (core appends its section),
  arc_browser_application.mm, arc_browser_context.mm, arc_values.mm, allowlist.
- ArcMod integration: `bun tools/build-mod.ts --engine <ArcCore.framework>` (arc ebd02f4); package-engine.sh uses it.
- Remaining lead items: release build (dcheck_always_on=false, is_official_build?), ArcMod docs/README for the
  engine, no-login branch port of --engine, real-profile test, Chromium version bump procedure.
- [ ] Browser/WebContents creation, native views (pane host), navigation, delegates — then the launch-slice harness.
- Loop: `./build-babysit.sh` → `./package-engine.sh ours156` → `../arc/tools/run-mod.sh --agent ours156 --background
  --cdp-port 9334` → read `../arc/build/ArcModAgent-ours156-run.log` ("ArcCore unimplemented:" lines show what's next).
- Dev builds have DCHECKs on (fatal); Arc shipped a release build. Release builds need `dcheck_always_on=false`.

## Proposed next milestone (superseded by the build work above)

- [ ] **M1: launch-slice runtime baseline** (`../arc/notes/engine-milestone-1.md`). Observe-only: run the
      app on ArcCore 1.170.0 through a fixed launch/navigate/store/quit/relaunch scenario with asserted
      outcomes, and trace engine entries and app callbacks in the browser process with lldb. Gates G0–G10.
      No engine code, no VMs. Debugger authorization on this Mac may need Rahul (see its prerequisites).

## Now (running)

- [x] **VM decompile** (done 2026-10-23 04:46 UTC; 567,959 functions; VM PAUSED since 05:31 UTC, no SSH or resume without Rahul's approval) (`arcmod-engine.exe.xyz`, `~/work/STATUS`): Ghidra analysis of ArcCore 1.170.0,
      then a project save, then export and a parallel decompile. Watched from the Mac. On failure: read
      `logs/ghidra-*.log` and `dmesg`. The analysis is saved, so rerun `scripts/phase-b2-ghidra.sh` to
      redo only the decompile.

## 1. Map Arc's own code (needs the decompile)

Sol (Rahul's sub-agents) owns the agent review and spec work from 2026-10-23. Done 2026-10-23: `specs/`, `research/2026-10-22-sol/`.

- [ ] Pull back only what's needed: the function list plus the Arc-layer extraction. Leave the full
      decompile on the VM (outbound traffic is metered).
- [x] (first pass, vm/arc_layer.py: ~5,840 functions / ~2.3 MB upper bound) Extract the Arc layer: methods of the 146 `Arc*` classes, functions that reference `../../arc/`
      paths, the 200 Arc-only strings, and the `arc.mojom.*` agents. Then follow calls into code that
      isn't stock Chromium.
- [~] (replaced: agents name functions by guess-and-check; see ENGINE.md §5.2) Match functions against our base build. Upload `out/Base/Chromium Framework.framework` (613 MB,
      1.37M named functions) to the VM and match by unique strings, constants and call structure.
      Unmatched code is Arc's own code, or their edits to Chromium.
- [ ] Measure the rewrite: Arc-layer function count and code size, which Chromium functions they
      edited, and the likely build-file edits (UI swaps, policy, prefs, field trials).

## 2. Choose the engine route

- [x] (done 2026-10-23: compile wins; results/cef-mapping.md) CEF mapping: sort the app's 834 engine methods (63 classes) into three groups: maps to CEF's
      public API, needs Chrome internals, or Arc-only feature.
- [x] (done by Codex/Sol, arc 23047ea) Write `notes/engine-feasibility.md` in the ArcMod repo: measured size, CEF vs compile-with-our-layer,
      effort, risks and a recommendation. Commit it, then ask before pushing.

## 3. If we compile Chromium with our own `arc/` layer

- [ ] Generate `ArcCore.h` headers from the engine's ObjC metadata: 146 classes, signatures and protocols.
- [ ] Skeleton `arc/` gn target producing `ArcCore.framework` that exports the same 259 symbols, then
      check that Arc's Swift app loads against it.
- [ ] Implement in launch-path order: `ArcBrowserApplication` → `ArcBrowserContext` → `ArcBrowser` →
      `ArcWebContents` → navigation, devtools and downloads → UI hooks (dialogs, prompts) → extras.
- [ ] Rebuild per Chromium release (incremental). Final optimized builds use Xcode 26.5 (or whatever
      SDK that Chromium targets), which also lets us drop `patches/0001-sdk-26.0-compat-*.patch`.

## 4. Keep current ArcMod alive meanwhile

- [ ] Engine updater: `tools/update-engine.ts` (reads Arc's feed, verifies the EdDSA signature, runs
      compatibility checks, bundles the engine in LFS) plus `build-mod.ts --engine <version>`, on both branches.
- [ ] Wider live test of the engine swap (1.168.0 app + Chromium 156): extensions, downloads,
      video/DRM, printing and the shortcut matrix.
- [ ] Check each remaining Arc release before Dec 22 for shutdown timers. Take only the engine, never
      the app layer.
- [ ] Back up the profile before the first launch on a new engine (Chromium profile upgrades are one-way).

## Housekeeping

- [x] `arcmod-engine` paused 2026-10-23 05:31 UTC with Rahul's conditional approval; disk retained, 0 USD/h. Deleting it still needs his approval.
- [ ] Move the engine scripts (`~/general/arc-engine/*.sh`, `patches/`, and the VM scripts kept in the
      session scratchpad) into a versioned place, e.g. `tools/engine/` in the ArcMod repo.
- [ ] Test the passkey routes (phone QR, security key, 1Password/Bitwarden extension) and update the README.
- [ ] Xcode 26.5 from developer.apple.com once Rahul's Apple Account is sorted. Not needed until final builds.

## Done

- [x] Engine swap proven: the 1.168.0 app layer runs on 1.170.0's Chromium 156 engine.
- [x] Engine composition mapped: Chrome's browser layer plus a Browser Company `arc/` tree; no added
      files inside Chromium's own directories; 200 Arc-only strings.
- [x] Base build: `chromium/src/out/Base/Chromium.app` (156.0.8078.48, unstripped, peak 35 GB RAM, no swap).
- [x] exe.dev skill (`~/.claude/skills/exe-dev/`) and the VM setup.

### Open: `webContentsRenderViewReady:` (lead, 2026-10-23)
Arc's engine sends it sparsely (8 of ~60 golden launches: relaunch with a replaced view, auth cancel,
site settings). Forwarding every Chrome RenderViewReady, or every one after the first, adds many calls
the goldens don't have, so it's not sent yet. Find the producer of bridge slot 0x4447d6c in ArcCore 1.170
(lldb backtrace on the trace156 app, as for the field-state setters) before sending it.

### Known gaps after the stub burn-down (lead, 2026-10-23 evening)
Stub inventory: 31 unimplemented ObjC methods remain (19 app-referenced, almost all `isEqual:`/`hash`
on canonical service wrappers, where NSObject identity is equivalent). Behavior gaps that remain:
- Import from other browsers: history, bookmarks and passwords use Chrome's importer; cookies,
  extensions, cards and sessions report "cannot be imported from this source" (Arc's engine had its own
  importers). ArcMod's `tools/import-arc-data.ts` covers Arc→ArcMod separately.
- Boosts color filters are recorded but not drawn (needs the renderer transport, see
  arc/common/RENDERER_VISUAL_GAPS.md); color schemes use a `color-scheme` stylesheet approximation.
- Auto-PiP flags (`ignoreDisablePiPAttribute:`/`skipUserActivationForPiP:`) and safe-area insets are
  stored only; the app sends the default (NO) in every golden.
- `webContentsRenderViewReady:` not sent (see above); Protected Audience not exposed (Arc exposes it).
- Native-UI scenarios (command bar, menus, shortcuts, Little Arc, Settings, Spaces, profiles UI, import
  wizard, autofill dropdown rendering) still need an unlocked screen.
