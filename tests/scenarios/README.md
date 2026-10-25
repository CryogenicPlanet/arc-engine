# Scenarios

Each scenario drives Arc's app through one feature and records every call between the app and the engine.
It runs on Arc's own engine to make the recording we compare against (the golden), then on ours.
Run everything from the repository root.

```sh
# Record a golden on Arc's engine (ArcMod agent build trace156)
bun tests/scenarios/record-golden.ts tabs-navigation

# Build out/Base, package it, run the scenario on our engine and diff each launch
arc/tools/diff-run.sh tabs-navigation

# Or run any app directly
bun tests/launch-slice/run.ts --app ../arc/build/ArcModAgent-rel156.app \
  --out results/ours/tabs-navigation --record --scenario tabs-navigation
```

The tests expect ArcMod checked out next to this repo as `../arc` (its `master` branch). They use its
agent tools and its agent builds in `../arc/build/`.

## How a run is judged

A golden is in `results/golden/arc-1.170-<scenario>/`. It has `steps.json` (each step's outcome),
`coverage.json` (the engine classes and selectors seen) and `launch-N/record.jsonl` (the transcript).
`record-golden.ts` accepts a recording only if the run exits 0 and every step passes. Otherwise it stays
in `arc-1.170-<scenario>.candidate/`, which is diagnostics, not a golden. The original `launch-slice`
golden can't be re-recorded.

Don't trust the harness exit code alone. Check `steps.json`, then the diff from
`tests/recorder/diff.py`: missing calls, extra calls, and return or callback mismatches. Allowed
differences are listed in `arc/DIFF-ALLOWLIST.md`. Coverage is a lower bound: it lists what was called,
not every method of a class.

## How scenarios drive the app

- Page actions go through ArcMod's DevTools client (`agent-cdp`). Native actions go through its
  accessibility controller (`agent-native`), bound to the app's PID. macOS only performs menu actions in
  the active app, so the harness brings the agent app to the front and restores focus afterwards.
- Arc's sidebar tab buttons only advertise a custom accessibility action, which fails. The harness then
  activates the tab through DevTools `Target.activateTarget`.
- Fixtures (local web pages) run in a separate process, so synchronous DevTools calls can't block them.
- Some scenarios load a small driver library (`*-driver.m`, `*.m`) into the app to call Arc's public
  engine API directly. Pass extra libraries and `ARC_SCENARIO_*` environment variables through
  `c.launch({ libraries, environment })`. The recorder always loads first.
- Agent builds keep their data in `~/Library/Application Support/ArcModAgent-<slug>`, separate from your
  ArcMod and Arc profiles. `real-profile-visual` is the only scenario that uses real profile data, and
  only on a disposable copy prepared by `logs/run-realcopy.sh`.

## Coverage notes

- `downloads` covers the default save with exact bytes; `downloads-prompt` covers cancelling the save
  sheet.
- `media` covers playback, pause and metadata; `media-pip` and `pip-flags` cover picture-in-picture.
- `tabs-restore` covers Archive Tab and Restore Last Closed Tab; `sessions-tab-restart` covers restoring
  closed tabs after a restart.
- `dialogs` covers alert, confirm and prompt; beforeunload and HTTP authentication have their own files.
- `chromium-import` builds an encrypted fixture Chrome profile and imports cookies, passwords, cards,
  history and bookmarks from it.
- Extension installs use DevTools `Extensions.loadUnpacked`, which doesn't exercise Arc's own
  install-unpacked action.
