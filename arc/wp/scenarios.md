# Work package "scenarios": golden transcripts covering every engine feature

Read `COMMON.md` first. You don't write engine code. You make the test scenarios and record their golden
transcripts on Arc's own engine, so the other packages can prove 1-1 behavior.

Own: `tests/launch-slice/run.ts` (refactor: keep the current scenario as the default "launch-slice" and keep all
current CLI flags working; add `--scenario <name>` loading `tests/scenarios/<name>.ts`), `tests/scenarios/**`,
new `results/golden/arc-1.170-<scenario>/` directories (never modify `arc-1.170-launch-slice`).
`arc/tools/diff-run.sh` already passes `--scenario <name>` for non-default scenarios.

Golden runs: Arc's engine app is `../arc/build/ArcModAgent-trace156.app` (rebuild with
`cd ../arc && bun tools/build-mod.ts --agent trace156 --source $PWD/../arc-engine/swap/hybrid-156/Arc.app` if
missing). Always record inside the lock: `arc/tools/locked.sh bun tests/launch-slice/run.ts --app <that app> --out
results/golden/arc-1.170-<scenario> --record --scenario <scenario>` after deleting
`~/Library/Application Support/ArcModAgent-trace156`. Every step must assert fixed expected values (pass/fail/
inconclusive), like the existing steps; a golden run counts only if all its steps pass on Arc's engine.

Scenarios to cover, highest value first (each a separate file; several launches per scenario are fine):
tabs-navigation (several tabs, back/forward, reload, same-document nav, redirects, close/reopen tab),
windows (second window, little arc/popup if reachable), pane-hosting/split view, downloads (file + prompt
handling), history and search (command bar typing/autocomplete, history entries, delete history),
cookies-storage-site-settings (cookie APIs, permission prompt e.g. notifications/geolocation with deny/allow),
dialogs (alert/confirm/prompt/beforeunload, HTTP auth), media (video playback, media session, PiP),
devtools (open/close, inspect), find-in-page, pdf (open, viewer), extensions (load unpacked, toolbar action,
popup, chrome.tabs/tabGroups), bookmarks/import if reachable, context menus, fullscreen, zoom.
Page-level actions go through CDP (`../arc/tools/agent-cdp.ts`); app-level actions (command bar, new window,
split view, keyboard shortcuts) through ArcMod's native agent tools: read `../arc/notes/computer-use-tools.md`,
`../arc/notes/agent-sessions.md`, `../arc/tools/agent-browser.ts`, `../arc/tools/agent-mcp.ts`. Use background,
PID-bound input; never steal focus from the user's own apps more than needed.

Report: the scenarios you added, which engine classes/selectors each one exercises (count from its transcript),
and anything that could not be automated (with why).
