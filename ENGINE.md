# ArcMod engine: knowledge base and runbook

Everything needed to resume building an Arc-compatible engine without this conversation.
The task list is `NEXT.md`. Facts here were measured on 2026-10-22/23 unless marked *unverified*.

## 1. Goal and constraints

- Arc becomes inaccessible on **2026-12-22**, and sign-in/sync ends 2026-11-23. After that, The Browser
  Company (TBC) ships no new `ArcCore`, so Chromium security updates stop.
- Goal: our own `ArcCore.framework`, built from Google's Chromium source plus our rewrite of Arc's
  layer, rebuilt per Chromium release. The patched Arc app layer (ArcMod, `~/general/arc`) runs on it.
- A stock prebuilt Chromium can't host Arc's app. It has no Arc API, and its internals are hidden and
  have no stable ABI. CEF (prebuilt embedding framework) is the cheaper alternative: a shim on its
  public API, losing Chrome-only features. The route is decided by the CEF mapping (NEXT.md §2).
- **Route decided 2026-10-23: compile Chromium with our own `arc/` layer.** CEF was rejected (`results/cef-mapping.md`).
  On macOS, embedding in the app's own NSView forces CEF's Alloy style. That means no extensions (CEF removed the
  API in M128), no autofill, password manager, history, omnibox or bookmarks, no Chrome dialogs, and no tab
  history restore. Of 1,701 methods: 167 are covered by CEF, 681 partly or by workaround, 572 need Chrome
  internals, and 281 are Arc-only. CEF's source is still useful as reference code.
- macOS builds need a Mac (this M4 Max, 48 GB). Heavy analysis runs on the exe.dev VM `arcmod-engine`.

## 2. What Arc's engine is

| Arc | ArcCore (Chromium) |
| --- | --- |
| 1.168.0 (88413) | 155.0.8059.40 |
| 1.169.0 (88758) | 155.0.8059.80 |
| 1.170.0 (89123), latest | 156.0.8078.48 |

All releases are at `https://releases.arc.net/release/Arc-<ver>-<build>.zip`. Verify a download against the feed's
`sparkle:edSignature` with Arc's `SUPublicEDKey` `PuVxvsbwitqWEvMPriVhNzmiMiVp2AqD8psqG/qcS98=`
(`vm/verify_sparkle.py`). The feed (`https://releases.arc.net/updates.xml`) also has delta updates.

**Bundle layout** (`Arc.app/Contents/Frameworks/ArcCore.framework/Versions/A/`). Our build must reproduce it:
- `ArcCore`: universal (x86_64+arm64). The arm64 slice is 228 MB, 197 MB of it `__text`.
  Bundle id `company.thebrowser.browser.framework`, `CFBundleShortVersionString` = Chromium version.
- The app links it as `@executable_path/../Frameworks/ArcCore.framework/Versions/A/ArcCore` with
  **compatibility version 8059.0.40**, so ours must declare at least that.
- `Helpers/`: `Browser Helper.app`, `(GPU)`, `(Renderer)`, `(Alerts)`, plus a second set
  `Browser Helper (Aperitif)`, `(Aperitif GPU)`, `(Aperitif Renderer)`, `(Aperitif Alerts)`, and `app_mode_loader`.
- `Libraries/`: `libaperitif.dylib`, `libvk_swiftshader.dylib`, `libvulkan.dylib`, `vk_swiftshader_icd.json`,
  `MEIPreload`, `PrivacySandboxAttestationsPreloaded`, `IwaKeyDistribution`.
- `Resources/`: 234 entries (paks, locales `*.lproj` including gendered variants, `app_mode-Info.plist`, ...).
- **Aperitif** is an open question. ArcCore only names its helpers (`kAperitifHelpers`); the code is in
  `libaperitif.dylib`. Find out what it is before deciding whether we need it.

**App↔engine interface** (`results/`):
- 259 exported symbols, identical for 1.168.0, 1.169.0 and 1.170.0.
  - C functions: `_ChromeMain` (the app's `main` calls it, Chromium owns the run loop), `_ChromeAppModeStart_v8`,
    `_ChromeWebAppShortcutCopierMain`, `_SetLogMessageHandler`, `_CallWithEHFrame`, `_emitTraceEvent`,
    `_arcCategoryEnabledFlag`, `_ArcErrorDomain`.
  - 146 exported ObjC classes: `results/arccore-1.170.0-classes.txt`.
- The 1.168.0 app imports 67 symbols from ArcCore: 63 classes + 4 C symbols
  (`results/app-1.168.0-imports-from-arccore.txt`). Those classes have 834 methods. The app also messages
  objects of other classes it receives (e.g. `ArcWebContents`) without importing them, so the real
  surface is up to all 146 classes.
- Up to 69 ObjC protocols in the engine, some of them callbacks into the app (*not yet classified*).
- About 50 DevTools-protocol method strings appear in the app layer, used through ADK's in-process CDP
  client (*rough count*). Upstream Chromium supports CDP natively.
- **Stability:** the 63 imported classes are identical between Chromium 155 and 156 across 6,280 normalized
  metadata lines. Elsewhere the changes were only additions (e.g. `ArcWebContents` gained `_mouseInside` and
  mouse-tracking methods).
- **Engine swap verified:** the 1.168.0 app layer plus 1.170.0's ArcCore ran all master mod features and
  reported Chromium 156.0.8078.48 (§5.1).

**Composition** (`results/arccore-vs-chromium-156.md`, `results/arc-files-1.170.0.txt`):
- 7,823 source paths are compiled into the engine (only files that log or assert are visible):
  - 111 in TBC's own `arc/` tree;
  - 6,074 that exist in stock Chromium 156.0.8078.48;
  - 1,638 from DEPS repos (v8, skia, ...).
  - None are new files inside Chromium's own directories.
- It's Chrome's browser layer (1,417 `chrome/` files, including 203 `chrome/browser/ui/views`) plus `arc/`:
  - `arc/browser/*_impl.cc`: the ObjC embedding API (browser application, browser context with downloads,
    extensions, preferences and effects, browser, web_contents, navigation controller, devtools, history,
    local state, tracing, AI service, content conversion).
  - `arc/browser/ui/*`: TBC's implementations of Chrome UI hooks: dialogs, permission prompts, JavaScript
    dialogs, autofill and password popups, location bar, sad tab, `browser_commands`, `window`, side panel,
    status tray, and so on.
  - Extras: importers (Chrome, Safari), ad blocker (browser service plus renderer agent), PiP, PDF controller,
    thumbnails, HEIF decoder, ffmpeg AudioToolbox glue, Sentry crash keys.
  - Renderer side: `arc.mojom.RenderFrameAgent`, `RenderProcessAgent`, `AdBlockerAgent`, `EffectsAgent`.
    There's also a built-in extension, `arc_internal_extension/background.js`.
- 200 Arc-only strings appear in no Chromium source (`results/arc-strings-not-upstream.txt`), e.g. the
  `arcMaxDisabledByOrgPolicy` policy, `ArcFeatureOverrideStudy_`, `arc.*` prefs, `ArcWebContentsLifecycle.*`
  histograms. So TBC's edits to stock Chromium are likely small: build files (swapping UI implementations),
  policy templates, prefs, field trials. Confirm with function matching.

## 3. Where everything lives

**This repo** (github.com/CryogenicPlanet/arc-engine, checked out at `~/general/arc-engine`; heavy dirs are untracked):
- `chromium/src`: Google's Chromium 156.0.8078.48 (31 GB, no history, hooks run) with `patches/` applied.
- `chromium/src/out/Base/Chromium.app`: stock release build, unstripped. `Chromium Framework.framework` is
  613 MB with 1,373,940 named functions.
- `depot_tools/`, `logs/`, `STATUS`.

**VM** `arcmod-engine.exe.xyz` (16 vCPU / 32 GB / 250 GB, standalone, 0.84 USD/h while running), under `~/work/`:
- `arc/<ver>/`: verified Arc zips, unpacked `Arc.app`, `ArcCore-<ver>.arm64`, `Arc-<ver>.arm64`, strings.
- `chromium/full/src`: Chromium 156.0.8078.48 source with macOS deps, no hooks.
  `chromium/upstream-156.0.8078.48-files.txt` is the file list; `chromium/DEPS-156.0.8078.48`.
- `ghidra-projects/ArcCore-1.170.0`: the saved analysis (3.6 GB). Reopen with `-process ArcCore-1.170.0.arm64 -noanalysis`.
- `decomp/arccore-1.170.0-functions.tsv`: 607,294 functions (`entry  end  size  name`).
- `decomp/arccore-1.170.0/0x*.c`: pseudo-C, sharded by `addr >> 20` (same format as the app decompile). Finished 2026-10-23 04:46 UTC:
  567,959 functions decompiled (79 failed), 199 shards, 1.1 GB. 92.9% are anonymous `FUN_<addr>`.
- `results/arc-layer-functions.tsv` (copy in `results/arc-layer-functions-1.170.0.tsv`) comes from `vm/arc_layer.py`.
  Seeds are 1,378 Arc ObjC methods (0.08 MB, thin wrappers over C++), 523 functions referencing `arc/`
  paths (0.48 MB) and 6 exports. Two call levels add 3,938 anonymous callees (1.78 MB). Total ~5,840
  functions, ~2.3 MB of machine code, as an upper bound (it includes Chromium callees). `FUN_0421f778` is a
  24 MB Ghidra mis-bounded region; exclude it.
- `tools/ghidra_12.1.4_PUBLIC`, `scripts/` (copies of `vm/`), `logs/`, `STATUS`.
- Ghidra names ObjC methods `Class::selector` (e.g. `ArcWebContents::blockedResourceCount`). 142 `Arc*` classes
  have named methods. Other functions are `FUN_<addr>` unless matched.

**VM** `arcmod-base.exe.xyz`: **deleted 2026-10-23** (its Ghidra run on our own build was unnecessary; see §5.2).
It was 32 vCPU / 64 GB / 100 GB, created 04:42 UTC for matching. Job: `vm/base-analysis.sh`. It uploads our stock `Chromium Framework` to
`~/work/upload/ChromiumFramework-156.0.8078.48.arm64` (sha256 `39647a53…`), runs a lite Ghidra analysis
into `ghidra-projects/ChromiumBase-156` with a 40 GB heap and 32 GB swap, then writes
`decomp/chromium-base-156-functions.tsv`.

**ArcMod repo** (`~/general/arc`): `ghidra/decomp/` is the Arc 1.168.0 **app-layer** decompile (pseudo-C, LFS).
`dump/` has class and symbol dumps. `bin/` has the 1.168.0 executable and the full app archive. `tools/build-mod.ts`
builds ArcMod (`--validation`, `--source <abs path>`).

## 4. Proven procedures

### 4.1 Engine swap test (any ArcCore under our app layer)

1. Assemble a source app: copy a 1.168.0 Arc.app (unpack `bin/Arc-1.168.0-88413.app.tar.xz`), replace
   `Contents/Frameworks/ArcCore.framework` with the candidate engine (`cp -cR`).
2. In a throwaway worktree of the ArcMod repo: `bun install`, then
   `bun tools/build-mod.ts --validation --source <ABSOLUTE path>/Arc.app`. Build-mod reads the version with
   `defaults read`, which needs an absolute path.
3. Back up `~/Library/Application Support/ArcModValidation` (`cp -cR`) and
   `defaults export company.thebrowser.Browser.mod.validation` first. Chromium upgrades profiles one way.
4. `tools/run-mod.sh --validation`, then open a localhost probe page that posts
   `navigator.userAgentData.getHighEntropyValues(["fullVersionList"])` back to a local server, to confirm the engine version.
5. Check the log (`$TMPDIR/arcmod.log` and `build/ArcModValidation-run.log`), the window list and crash reports.
   Quit with SIGTERM, then restore the profile and prefs.

### 4.2 Comparing the app↔engine interface between engine versions (`analysis/`)

- Exports: `nm -gU -arch arm64 ArcCore`. App imports: `nm -m -u Arc.arm64 | grep "(from ArcCore)"`.
- Selector names: `dyld_info -arch arm64 -objc ArcCore`. Type encodings, ivars, sizes: `otool -arch arm64 -ov ArcCore`.
- Gotchas:
  - otool doesn't resolve relative method-list names.
  - otool prints each class again in later reference sections with a short block; keep the longest block.
  - It nests metaclass and protocol methods inside the class block, so don't pair names with types by
    index. Compare normalized blocks with addresses stripped (`block-diff.ts`).
- Positive control: run it across all classes. It must flag `ArcWebContents` (155→156 added `_mouseInside`).

### 4.3 Chromium build (Mac)

- `sync-chromium.sh`: gclient sync of `refs/tags/<ver>` with no history, hooks on, `-j16`, then reapplies
  `patches/*.patch`. Main repo is a single ~3 MB/s stream from Google; deps go in parallel. About 35 min on this Wi-Fi.
- `build-babysit.sh` builds `out/Base`. Args: release, non-component, `symbol_level=1`, arm64,
  `proprietary_codecs`, `ffmpeg_branding="Chrome"`. It checks first: Xcode, Metal toolchain (auto-installs),
  SDK floor, ≥80 GB disk, file-descriptor limit, gn gen. Then it retries known failures with incremental resume.
  - First full build: about 80 min of compiling at `-j14` with `nice 10`. Peak 35 GB RAM system-wide (about 5–10 GB
    from the build itself), no swap.
- **SDK:** Chromium 156 targets SDK 26.5 (`mac_sdk_official_version`, build 25F70). This Mac has Xcode 26.0.1
  (SDK 26.0). Only one Apple API is missing (`+[ASAuthorizationWebBrowserPublicKeyCredentialManager
  isDeviceConfiguredForPasskeys]`), which `patches/0001-*` forward-declares; `patches/sdk-compat-min` lowers the floor.
  - For the final optimized builds use Xcode 26.5 from developer.apple.com, side by side, via `switch-xcode.sh`
    (writes `xcode.env`, needs no system change). Don't use 27.x for Chromium 156.
  - Switching SDKs recompiles most of the tree.
- Xcode 26+ needs `xcodebuild -downloadComponent MetalToolchain`. ANGLE compiles Metal shaders.
- Official-style builds (ThinLTO + PGO, closer to how TBC built ArcCore) have a much heavier link. On
  48 GB, cap ThinLTO jobs.

### 4.4 VM jobs

- Pattern (also in the `exe-dev` skill): `mkdir -p ~/work/logs` first, then `(nohup script > log 2>&1 < /dev/null &)`.
  Timestamped `~/work/STATUS` lines ending in `DONE`/`FAILED`. A backgrounded until-loop on the Mac reads only
  STATUS lines added after it started.
- Ghidra on a 228 MB arm64 binary: about 4 h of analysis, mostly single-threaded (`ObjcMessageAnalyzer` is late),
  then a parallel decompile (`-max-cpu 10`, about 8.5k functions/min).
  - Heap 20 GB on the 32 GB VM plus 32 GB swap. The first run, with a 26 GB heap and no swap, was OOM-killed at 1h40m.
  - `phase-b2-ghidra.sh` saves the analysis first. Rerun it to redo only export and decompile.

## 5. How we got here

- 2026-10-22: the engine swap was proven (Arc 1.168.0's app on Arc 1.170.0's ArcCore). Static research on
  ArcCore 1.170.0 produced `specs/` (one file per class) and `research/engine-feasibility.md`; the working
  files of that research run are in `research/2026-10-22-sol/`.
- 2026-10-23: route decided: compile Chromium with our own `arc/` layer; CEF was rejected
  (`results/cef-mapping.md`). ArcCore 1.170.0 was decompiled on the VM (§3). Functions Arc's layer reaches
  were named by guess-and-check against the Chromium source, not by mechanical matching.
- 2026-10-23: the engine was built, first by parallel Codex workers (until their usage limit), then by the
  lead directly. The workers' notes, probe scripts, proposal patches and probe outputs were removed from
  the tree on 2026-10-24; git history before that commit has them.

## 6. Open questions

- What is Aperitif (`libaperitif.dylib`, the second helper set)? Our engine doesn't build it, and no
  recorded scenario has needed it so far.
- Did TBC build ArcCore as an official build (ThinLTO/PGO)? That matters for performance comparisons, not
  for behavior.
