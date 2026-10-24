# Arc Engine

The browser engine for [ArcMod](https://github.com/CryogenicPlanet/arc): Arc's
`ArcCore.framework`, rebuilt from Google's Chromium source.

> **Experimental.** Most of Arc works on it in automated tests, but it isn't ready
> for your daily profile yet. See [Status](#status).

Arc for macOS is two pieces. The Swift app draws everything you see: the sidebar,
spaces, the command bar, Little Arc. Underneath it, `ArcCore.framework` is The Browser
Company's build of Chromium plus an Objective-C API the app calls (146 classes).
Arc shuts down on 2026-12-22, and ArcCore stops at Chromium 156. Its last release
gets no security updates after that.

Arc Engine is a replacement ArcCore with the same API, built from upstream Chromium.
ArcMod runs Arc's app on it, so it can keep moving to new Chromium releases.

Not affiliated with The Browser Company.

## Not a Chromium fork

This repo doesn't contain Chromium. It holds what we add to it:

- `arc/`: our implementation of Arc's engine API on top of Chromium's browser layer.
  It's linked into the Chromium tree as `//arc`.
- `patches/`: the small changes to Chromium itself, such as the framework layout,
  a mode for being hosted by Arc's app, and hooks for Arc's UI. Behavior changes
  only apply when Arc's app starts the engine. `patches/MANIFEST` lists the patch
  that owns each changed file.

`sync-chromium.sh` checks out Chromium 156.0.8078.48 (the version Arc 1.170.0
shipped), applies the patches and links `arc/` in. Moving to a new Chromium release
means re-syncing and rebasing the patches.

## How it's used

ArcMod includes this repo as its `engine/` submodule. ArcMod's builder patches a
copy of Arc's app, and with `--engine` it replaces Arc's engine in that copy with
ours. [ArcMod's README](https://github.com/CryogenicPlanet/arc#arc-engine-experimental)
has the steps.

## Status

Every engine method Arc's app calls is implemented. These behave like Arc's engine
in recorded scenarios:

- launching, quitting (with the quit warning) and relaunching with cookies and storage intact;
- profiles: creating and switching them in Arc's UI, with separate data that survives a restart;
- Settings, the command bar and ArcMod's agent tools;
- tabs and navigation;
- downloads;
- media and picture-in-picture, including the auto-PiP policies;
- PDFs;
- bookmarks and history;
- importing from Chrome, Edge, Brave, Vivaldi, Opera and Arc: history, bookmarks,
  cookies, passwords, cards and extensions;
- extensions' page actions;
- password, address and card save offers;
- passkey dialogs.

Still open: archiving a tab that was opened in the background, importing a folder
that Safari exported, importing open tabs from Chromium browsers, and Boosts' color
filters. [NEXT.md](NEXT.md) is the living plan and has the full list.

## How it's tested

The goal is to behave exactly like Arc's engine wherever Arc's app can tell the
difference, and that's checked at the boundary between the two:

- `tests/recorder/` is injected into Arc's app and logs every call between the app
  and the engine, with arguments and results: calls into the 146 engine classes, and
  the 270 callbacks the engine makes into the app.
- `tests/scenarios/` has 57 scenarios that drive Arc through DevTools and the
  macOS accessibility API: tabs, profiles, downloads, dialogs, imports and so on.
  `tests/launch-slice/run.ts` launches the app and runs them.
- Each scenario is recorded on Arc's own engine first. Those recordings are in
  `results/golden/`. Then it runs on ours, and `tests/recorder/diff.py` compares
  the two transcripts call by call.

A method counts as done when no scenario that uses it shows a missing call or a
different result. [arc/ARCHITECTURE.md](arc/ARCHITECTURE.md) has the rules.

## Build

You need an Apple Silicon Mac with full Xcode and about 100 GB free. The first build
takes hours; later ones are incremental.

```sh
git clone https://github.com/CryogenicPlanet/arc-engine.git
cd arc-engine
./sync-chromium.sh    # depot_tools, Chromium 156, our patches and the //arc link
./build-babysit.sh    # out/Base, with preflight checks and retries for known failures
```

Use a release build with ArcMod. `out/Base` keeps Chromium's internal debug checks
on, and some of them stop on behavior that Arc's shipped engine allowed.

```sh
cd chromium/src
gn gen out/Release --args='is_debug=false dcheck_always_on=false symbol_level=1 target_cpu="arm64" proprietary_codecs=true ffmpeg_branding="Chrome" enable_nacl=false'
autoninja -C out/Release chrome
```

The engine is `chromium/src/out/Release/Chromium.app/Contents/Frameworks/ArcCore.framework`.

## Running the tests

The tests drive ArcMod's agent builds (from ArcMod's `master` branch) and use its
`tools/`. They expect ArcMod checked out next to this repo as `../arc`:

```sh
git clone https://github.com/CryogenicPlanet/arc.git
git clone https://github.com/CryogenicPlanet/arc-engine.git
```

1. Unpack Arc 1.168.0 from ArcMod's `bin/Arc-1.168.0-88413.app.tar.xz` into
   `swap/stock-1.168.0/`.
2. Package an ArcMod agent build on our engine. It lands in `../arc/build/`.

   ```sh
   ARC_ENGINE_OUT=out/Release ./package-engine.sh rel156
   ```

3. Run a scenario with the recorder, then diff it against its recording on Arc's
   engine:

   ```sh
   bun tests/launch-slice/run.ts --app ../arc/build/ArcModAgent-rel156.app \
     --out results/ours/media --record --scenario media
   python3 tests/recorder/diff.py results/golden/arc-1.170-media/launch-1/record.jsonl \
     results/ours/media/launch-1/record.jsonl
   ```

`arc/tools/diff-run.sh <scenario>` does all of that for the `out/Base` build. New
recordings on Arc's engine come from `bun tests/scenarios/record-golden.ts <scenario>`,
which uses an ArcMod agent build on Arc 1.170.0's own engine
(`../arc/build/ArcModAgent-trace156.app`). Agent builds keep their data in their
own folder, separate from your ArcMod and Arc profiles.

## Layout

- `arc/`: the engine layer. [ARCHITECTURE.md](arc/ARCHITECTURE.md) is the contract
  for code in it.
- `patches/`: our Chromium patches and their `MANIFEST`.
- `tests/`: the recorder, the harness and the scenarios.
- `results/`: recordings on Arc's engine (`golden/`) and smaller findings.
- `specs/`: one file per engine class, saying what each method does and which
  Chromium code it maps to.
- `research/`, `analysis/`, `vm/`: static research on Arc 1.170.0's engine that the
  specs came from, and the tools that produced it.
- Build scripts: `sync-chromium.sh`, `build-babysit.sh`, `switch-xcode.sh` (build
  with a side-by-side Xcode), `package-engine.sh`, `memory-log.sh`.
- [NEXT.md](NEXT.md): the plan and task list. [ENGINE.md](ENGINE.md): what we know
  about Arc's engine, and procedures that worked.
