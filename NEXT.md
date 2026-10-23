# ArcMod engine: next steps

Goal: keep ArcMod on current Chromium after Arc shuts down on 2026-12-22, by building our own
engine that matches Arc's `ArcCore` interface. Updated 2026-10-23.
Facts, locations, procedures and the step-by-step runbook are in `ENGINE.md`.
Static research is complete and reviewed: report `../arc/notes/engine-feasibility.md`, class and protocol
index `specs/README.md` (146 classes, 1,964 declared methods, 26 protocols; declared coverage, not behavior).

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
