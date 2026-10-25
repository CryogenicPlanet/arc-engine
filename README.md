# Arc Engine

Arc Engine is the browser engine for [ArcMod](https://github.com/CryogenicPlanet/arc): Arc's
`ArcCore.framework`, rebuilt from Google's Chromium source so Arc can keep getting Chromium
security updates.

> **Experimental.** Most of Arc works on it in automated tests, but it isn't ready for your
> daily profile yet. See the [status issue](https://github.com/CryogenicPlanet/arc-engine/issues/1).

Arc for macOS is two pieces. The Swift app draws everything you see: the sidebar, spaces, the
command bar and Little Arc. Underneath it, `ArcCore.framework` is The Browser Company's build of
Chromium plus an Objective-C API that the app calls (146 classes). Arc shuts down on 2026-12-22,
and its engine stops at Chromium 156 with no security updates after that.

Arc Engine implements the same API on upstream Chromium. ArcMod runs Arc's app on it, and moving
to a new Chromium release means rebuilding the engine, not waiting for The Browser Company.

Not affiliated with The Browser Company.

## Not a Chromium fork

This repo doesn't contain Chromium. It holds what we add to it:

- `arc/`: our implementation of Arc's engine API on top of Chromium's browser layer. It's linked
  into the Chromium tree as `//arc`.
- `patches/`: the small changes to Chromium itself, such as the framework layout, a mode for
  being hosted by Arc's app, and hooks for Arc's UI. Behavior changes only apply when Arc's app
  starts the engine. `patches/MANIFEST` lists the patch that owns each changed file.

`sync-chromium.sh` checks out an unmodified Chromium 156.0.8078.48 (the version Arc 1.170.0
shipped), applies the patches and links `arc/` in.

## Build it

You need an Apple Silicon Mac with full Xcode and about 100 GB free. The first build takes hours;
later builds are incremental.

```sh
git clone https://github.com/CryogenicPlanet/arc-engine.git
cd arc-engine
./sync-chromium.sh    # depot_tools, Chromium 156, our patches and the //arc link
./build-babysit.sh    # out/Base, with preflight checks and retries for known failures

cd chromium/src
gn gen out/Release --args='is_debug=false dcheck_always_on=false symbol_level=1 target_cpu="arm64" proprietary_codecs=true ffmpeg_branding="Chrome" enable_nacl=false'
autoninja -C out/Release chrome
```

The engine you want is
`chromium/src/out/Release/Chromium.app/Contents/Frameworks/ArcCore.framework`. Use the release
build: `out/Base` keeps Chromium's internal debug checks on, and some of them stop on behavior
that Arc's shipped engine allowed.

## Use it with ArcMod

ArcMod includes this repo as its `engine/` submodule. Its builder patches a copy of Arc's app,
and `--engine` replaces Arc's engine in that copy with this one. From the ArcMod checkout:

```sh
bun tools/build-mod.ts --validation \
  --engine engine/chromium/src/out/Release/Chromium.app/Contents/Frameworks/ArcCore.framework
```

[ArcMod's README](https://github.com/CryogenicPlanet/arc#arc-engine-experimental) has the full
steps. Without `--engine`, ArcMod keeps using Arc's own engine.

## Status

Every engine method Arc's app calls is implemented, and most of Arc behaves like it does on
Arc's own engine: launching, profiles, Settings, the command bar, tabs, downloads, media and
picture-in-picture, PDFs, bookmarks, history, importing from other browsers and more. Known
problems are listed in the pinned [status issue](https://github.com/CryogenicPlanet/arc-engine/issues/1), which is kept up to date.

## How it's tested

The goal is to behave exactly like Arc's engine wherever Arc's app can tell the difference, so
the tests check the boundary between the two:

- `tests/recorder/` is loaded into Arc's app and logs every call between the app and the engine,
  with arguments and results: calls into the 146 engine classes, and the 270 callbacks the engine
  makes into the app.
- `tests/scenarios/` has 57 scenarios that drive Arc through DevTools and the macOS accessibility
  API: tabs, profiles, downloads, dialogs, imports and more.
- Each scenario is recorded on Arc's own engine first; those recordings are in `results/golden/`.
  Then it runs on ours, and `tests/recorder/diff.py` compares the two transcripts call by call.

A method counts as done when no scenario that uses it shows a missing call or a different result.

### Running the tests

The tests drive ArcMod's agent builds and use its `tools/`, so clone ArcMod's `master` branch
next to this repo as `../arc`:

```sh
git clone https://github.com/CryogenicPlanet/arc.git
git clone https://github.com/CryogenicPlanet/arc-engine.git
```

1. Unpack Arc 1.168.0 from ArcMod's `bin/Arc-1.168.0-88413.app.tar.xz` into
   `swap/stock-1.168.0/`.
2. Package an ArcMod agent build on the release engine. It lands in `../arc/build/`.

   ```sh
   ARC_ENGINE_OUT=out/Release ./package-engine.sh rel156
   ```

3. Run a scenario with the recorder, then diff it against the recording on Arc's engine:

   ```sh
   bun tests/launch-slice/run.ts --app ../arc/build/ArcModAgent-rel156.app \
     --out results/ours/media --record --scenario media
   python3 tests/recorder/diff.py results/golden/arc-1.170-media/launch-1/record.jsonl \
     results/ours/media/launch-1/record.jsonl
   ```

`arc/tools/diff-run.sh <scenario>` does all three for the `out/Base` build.
[tests/scenarios/README.md](tests/scenarios/README.md) covers recording new goldens and how
scenarios drive the app. Agent builds keep their data in their own folder, separate from your
ArcMod and Arc profiles.

## Repository layout

- `arc/`: the engine layer. [arc/ARCHITECTURE.md](arc/ARCHITECTURE.md) has the rules for code in it.
- `patches/`: our Chromium patches and their `MANIFEST`.
- `tests/`: the recorder, the harness and the scenarios.
- `results/`: recordings on Arc's engine (`golden/`), recent release runs and small findings.
- `specs/`: one file per engine class: what each method does and which Chromium code it maps to.
- `research/`, `analysis/`, `vm/`: the static research on Arc 1.170.0's engine that the specs came
  from, and the tools that produced it.
- Build scripts: `sync-chromium.sh`, `build-babysit.sh`, `switch-xcode.sh` (builds with a
  side-by-side Xcode), `package-engine.sh` and `memory-log.sh`.

[NEXT.md](NEXT.md) is the task list, and [ENGINE.md](ENGINE.md) has what we know about Arc's
engine. Coding agents working on this repo should read [AGENTS.md](AGENTS.md) first.
