# Working on Arc Engine

## Context

- Read `README.md`, the status issue (https://github.com/CryogenicPlanet/arc-engine/issues/1), then `NEXT.md`
  (the plan and notes on open items). Before writing code under `arc/`,
  read `arc/ARCHITECTURE.md`; its rules are binding. `ENGINE.md` has facts about Arc's engine and
  procedures that worked.
- The target: Arc 1.168.0's app layer runs on our `ArcCore.framework`, built from Chromium
  156.0.8078.48, and behaves exactly like Arc 1.170.0's own ArcCore wherever the app can tell.
- ArcMod (`../arc`, github.com/CryogenicPlanet/arc) is the app side. The tests use its agent tools and
  agent builds, so keep it checked out next to this repo on `master`.
- `specs/<Class>.md` are reviewed static research on each engine class. When a spec and a recording on
  Arc's engine disagree, the recording wins.

## Workflow

- A change is done when the scenarios that exercise it pass on the release engine and
  `tests/recorder/diff.py` shows no new missing calls, extra calls or result mismatches against the
  golden. Compiling isn't acceptance. Allowed differences go in `arc/DIFF-ALLOWLIST.md` with a reason.
- Put new behavior in `arc/`. Change Chromium only through `patches/`, gated on `--arcmod-host-app` where
  behavior differs from Chrome, and regenerate them with `arc/tools/regen-patches.sh`.
- One build or app run at a time on this Mac: wrap builds and runs in `arc/tools/locked.sh`.
- Release builds: `autoninja -C chromium/src/out/Release chrome`, then
  `ARC_ENGINE_OUT=out/Release ./package-engine.sh rel156`, then `logs/run-release.sh <scenario…>`.
  Record new goldens on Arc's engine with `bun tests/scenarios/record-golden.ts <scenario>`.
- Track problems in the status issue (`gh issue edit 1 -R CryogenicPlanet/arc-engine --body-file
  <file>`, keeping its layout and the "Updated" date): add new ones; when you fix one, tick it and
  strike it through with the fixing commits so it stays as history. Comment only for big updates. Put the plan and technical notes in `NEXT.md` and durable facts in `ENGINE.md`.
  Don't add new note, report or status files; delete scratch files before you commit.
- Keep commits focused. Never commit `chromium/`, build output, `results/ours/`, `results/realcopy/`,
  browser data or files over 40 MB (GitHub rejects files over 100 MB).
- For shared coordination, fetch `https://chirp.cryo.wtf/init` at session start and follow the live
  protocol. Codex state is only in `~/.chirp/codex/`; don't use those credentials.

## Safety

- Never touch `/Applications/Arc.app`, the original Arc profile (`~/Library/Application Support/Arc`) or
  ArcMod's normal data. Agent builds (`ArcModAgent-*`) and their data are disposable.
- Real-profile checks run only on a disposable copy (`logs/run-realcopy.sh`), which is deleted afterwards.
- The exe.dev VM `arcmod-engine` is paused. Don't SSH into it, resume it or create paid capacity without
  Rahul's approval.
- ArcMod's native mod code (`../arc/mods`) belongs to the ArcMod repo; engine work doesn't change it.
