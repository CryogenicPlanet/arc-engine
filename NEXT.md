# Arc Engine: status and next steps

Updated 2026-10-24. This is the living task list. [README.md](README.md) explains the project,
[ENGINE.md](ENGINE.md) has facts about Arc's engine and procedures that worked, and
[arc/ARCHITECTURE.md](arc/ARCHITECTURE.md) has the rules for code in `arc/`.

## Where things stand

Our ArcCore (Chromium 156.0.8078.48, `arc/`, and `patches/`) runs Arc 1.168.0's unmodified app layer.
Use the release build (`chromium/src/out/Release`). It's packaged as the ArcMod agent build `rel156` with
`ARC_ENGINE_OUT=out/Release ./package-engine.sh rel156`.

On the release engine, every step passes in these scenarios, compared with recordings on Arc's engine:
launch-slice, tabs-navigation, media, media-pip, pip-flags, downloads, pdf, navigation-forms,
webauthn-dialog, profiles-public-api, chromium-import, arcmod-settings, arcmod-command-bar,
arcmod-profiles, arcmod-agent-mcp and application-termination-denial. password-save, autofill-save and
save-offers-accept fail only a slow quit that Arc's engine also fails.

A disposable copy of a real ArcMod profile launched twice on it, rendered the sidebar, the command bar and
Settings, and quit cleanly, with no crashes.

31 engine methods are still stubs. 19 of them are referenced by the app, and almost all of those are
`isEqual:`/`hash` on service wrappers, where NSObject identity is equivalent.

## Open, in priority order

1. **Background tabs:** a tab opened in the background (DevTools `background:true`) and then selected keeps
   `isReadyToDisplay` NO and `loadProgress` 0.7 on ours (YES and 1 on Arc's), and the app's Archive Tab
   then does nothing. This may affect real background tabs. Scenario: `arcmod-tabs-restore`.
2. **Safari import:** importing a folder that Safari exported (public import type 12) is rejected by ours.
   Scenario: `arcmod-disposable-import`.
3. **Open tabs import:** importing open tabs from Chromium browsers (category 2048, delivered through
   `didImportSessionWindows:`) isn't implemented.
4. **Boosts' color filters** are recorded but not drawn; color schemes use a `color-scheme` stylesheet
   approximation. Both need a renderer transport. Unapplied drafts (`renderer_effects_*_proposal.patch`)
   are in git history.
5. **Callback differences** from Arc's engine:
   - extra: `webContentsActivate:` when the tab is already selected, and
     `webContentsClosingImminent:withSessionID:`;
   - missing: `setInputEvent:` for keys and `didConsumeKey…`, camera and microphone state, `thumbnailReady`,
     and `webContentsRenderViewReady:`. Arc's engine sends the last one rarely (8 of about 60 recorded
     launches). Find its producer (bridge slot 0x4447d6c in ArcCore 1.170) with an lldb backtrace on the
     `trace156` app before sending it.
   - last seen open, recheck against the recordings: the focused-field properties (`undoableFieldActive`,
     `freeformTextFieldActive`, `passwordTextFieldActive`, `hasDirtyTextField`, `hadFormOrUserEdits`)
     don't update after creation, and ours asks `decidePolicyForNavigationAction:` for a form POST where
     Arc's engine doesn't. Ours reports a different profile `userColor` in dark mode (#1C1E22 vs #A8C7FA).
   - Protected Audience isn't exposed (Arc's engine exposes it).
6. **Scenarios that fail on Arc's engine too:** `arcmod-spaces` (after Previous Space no tab shows as
   selected) and `arcmod-key-shortcuts` (a synthesized Cmd+W runs `closeWindow:`). Fix the scenarios first.
7. **Unverified:** extension downloads from the Web Store (the tests run offline), and these behaviors that
   were inferred, not recorded:
   - Find's completion is `void (^)(ArcFindResult*)`, called once with the latest request's final result.
   - `executeJavascript:`'s completion is `void (^)(NSString*, BOOL)`; the BOOL may instead be a sequence
     number. Results come back JSON-encoded.
   - `ArcAccessibilityActionData` codes 0-3 are DoDefault, Focus, SetValue and ScrollToMakeVisible.
   - `captureViewport:`/`captureFullPage:` complete with an NSImage.

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
