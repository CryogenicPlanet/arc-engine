# ArcMod engine

Work toward an Arc-compatible Chromium engine (`ArcCore.framework`) that we build ourselves,
so ArcMod keeps getting Chromium security updates after Arc shuts down on 2026-12-22.
The ArcMod repo (`~/general/arc`) holds the app-layer mod; this folder holds the engine work.

- `NEXT.md`: the living plan and task list. Start here.
- Mac (builds): `sync-chromium.sh` checks out Chromium 156.0.8078.48 into `chromium/`.
  `build-babysit.sh` builds `out/Base` with preflight checks and recovery. `switch-xcode.sh` points
  builds at a side-by-side Xcode. `memory-log.sh` samples RAM during builds.
  Status goes to `STATUS`, logs to `logs/`.
- `patches/`: our local Chromium patches (currently one: an SDK 26.0 compatibility declaration).
- `vm/`: scripts run on the exe.dev VM `arcmod-engine` (verified Arc downloads, Chromium file list,
  Ghidra analysis and decompile of ArcCore). They live in `~/work/scripts` there.
- `analysis/`: app↔engine interface and patch-site comparison tools. Inputs go in `analysis/data/`
  (not tracked; regenerate from Arc releases).
- `results/`: small findings, such as ArcCore 1.170.0 vs Chromium 156 source paths and Arc-only strings.
