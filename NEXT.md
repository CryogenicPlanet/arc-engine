# Arc Engine: plan and notes

Updated 2026-10-24. [README.md](README.md) explains the project,
[ENGINE.md](ENGINE.md) has facts about Arc's engine and procedures that worked, and
[arc/ARCHITECTURE.md](arc/ARCHITECTURE.md) has the rules for code in `arc/`.

## Status

Known problems are listed in the pinned [status issue](https://github.com/CryogenicPlanet/arc-engine/issues/1); update them there, not here.
This file keeps the plan and technical notes for the open items.

## Notes on open items

- **Background tabs** (`arcmod-tabs-restore`): Arc's engine reports YES and 1 for
  `isReadyToDisplay` and `loadProgress` on the same steps. This may affect real background tabs.
- **Quit skips Chromium's shutdown.** Arc's app exits the process once `tryToTerminate:` replies, and
  `quitRunLoopOnTermination` is stored but unused; Arc's engine quits its main loop and shuts down the
  browser runner instead. `arc_app_termination.mm` flushes what we know defers writes (cookies, storage
  partitions, prefs, local state, sessions, bookmarks, history). Anything else that commits on a timer can
  still lose its last few seconds; running Chromium's shutdown after the reply would cover all of it.
- **Importing open tabs** (category 2048) from Chromium browsers. Arc's engine copies the source
  profile's `Sessions` folder first ("Could not copy source profile's Sessions directory"). Three things block a direct port:
  Chromium 156 encrypts session files by default (`kEncryptSessionStorage`: `Sessions_Encrypted`,
  file version 5, read through an `os_crypt_async::Encryptor`), and the `Encryptor` constructor that
  takes a key is private to Chromium's own crypto classes, so another browser's Safe Storage key can't
  be plugged in. `CommandStorageBackend::InitIfNecessary` creates a `Sessions` folder and deletes old
  session files, so it may only ever run on a temporary copy. `SessionFileReader` is internal. The way
  forward is either a small reader for the session file format (cleartext version 3 files in
  `Sessions/` first, then version 5 files decrypted with the source browser's Safe Storage key) or a
  patch that exposes a keyed `Encryptor` for import.
- **Boosts' color filters** need a renderer transport. Unapplied drafts
  (`renderer_effects_*_proposal.patch`) are in git history before the 2026-10-24 cleanup.
- **`webContentsRenderViewReady:`**: Arc's engine sends it rarely (8 of about 60 recorded launches).
  Find its producer (bridge slot 0x4447d6c in ArcCore 1.170) with an lldb backtrace on the
  `trace156` app before sending it.
- **Recheck against the recordings:** the focused-field properties (`undoableFieldActive`,
  `freeformTextFieldActive`, `passwordTextFieldActive`, `hasDirtyTextField`, `hadFormOrUserEdits`)
  were last seen not updating after creation; ours asked `decidePolicyForNavigationAction:` for a
  form POST where Arc's engine doesn't; profile `userColor` differed in dark mode (#1C1E22 vs
  #A8C7FA).
- **Inferred, not recorded:** Find's completion is `void (^)(ArcFindResult*)`, called once with the
  latest request's final result. `executeJavascript:`'s completion is `void (^)(NSString*, BOOL)`
  (the BOOL may be a sequence number), with JSON-encoded results. `ArcAccessibilityActionData` codes
  0-3 are DoDefault, Focus, SetValue and ScrollToMakeVisible. `captureViewport:`/`captureFullPage:`
  complete with an NSImage.
- 31 engine methods are still stubs; 19 are referenced by the app, almost all `isEqual:`/`hash` on
  service wrappers, where NSObject identity is equivalent.

## Releases

`./package-release.sh <tag>` zips the release build (`out/Release`, local symbols stripped) with
Chromium's license and credits into `dist/`. Publish it with `gh release create <tag> --prerelease`
and pin the new tag, URL and SHA-256 in ArcMod's `resources/engine-release.json`.

## Later

- **First Chromium version bump.** Sync the new tag, rebase `patches/` (`patches/MANIFEST` says which patch
  owns each file; regenerate with `arc/tools/regen-patches.sh`), rebuild, rerun the scenarios. Write the
  procedure down in ENGINE.md as you go.
- **Make our engine ArcMod's default** once items 1-3 are fixed and a real profile has run on it for a while.
  Back up the profile before its first launch on a new engine: Chromium profile upgrades are one-way.
- **Run the tests from ArcMod's `engine/` submodule.** They currently expect ArcMod next to this repo as
  `../arc`.
- **Xcode 26.5** for final builds, which lets us drop `patches/0001-*` (needs Rahul's Apple account).

## Working here

- One build or app run at a time: wrap commands in `arc/tools/locked.sh`.
- `arc/tools/diff-run.sh <scenario>` builds `out/Base`, packages it, runs the scenario and diffs it.
  New recordings on Arc's engine: `bun tests/scenarios/record-golden.ts <scenario>`.
  [tests/scenarios/README.md](tests/scenarios/README.md) has the details.
- Never touch `/Applications/Arc.app` or the original Arc profile. Real-profile checks run only on
  disposable copies, which are deleted afterwards (`results/realcopy/` is gitignored).
- The exe.dev VM `arcmod-engine` is paused. Don't SSH into it, resume it or create new paid capacity
  without Rahul's approval.
- Keep files over 40 MB out of git (GitHub rejects files over 100 MB).
