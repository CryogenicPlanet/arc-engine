# ArcMod engine: next steps

Goal: keep ArcMod on current Chromium after Arc shuts down on 2026-12-22, by building our own
engine that matches Arc's `ArcCore` interface. Updated 2026-10-23.
Facts, locations, procedures and the step-by-step runbook are in `ENGINE.md`.

## Now (running)

- [x] **VM decompile** (done 2026-10-23 04:46 UTC; 567,959 functions) (`arcmod-engine.exe.xyz`, `~/work/STATUS`): Ghidra analysis of ArcCore 1.170.0,
      then a project save, then export and a parallel decompile. Watched from the Mac. On failure: read
      `logs/ghidra-*.log` and `dmesg`. The analysis is saved, so rerun `scripts/phase-b2-ghidra.sh` to
      redo only the decompile.

## 1. Map Arc's own code (needs the decompile)

Sol (Rahul's sub-agents) owns the agent review and spec work from 2026-10-23.

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
- [ ] Write `notes/engine-feasibility.md` in the ArcMod repo: measured size, CEF vs compile-with-our-layer,
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

- [ ] Ask Rahul before pausing or deleting the `arcmod-engine` VM once matching is done (0.84 USD/h).
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
