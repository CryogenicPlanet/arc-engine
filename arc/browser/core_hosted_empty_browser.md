# Hosted empty browser retention — ready, unapplied

Current patch SHA95c46cc5e1118fda12c1b2aa1ff7f723201ff173086df51aa4a59fd54930e142.
This supersedes the original idle-only proposal135062; do not apply its old bytes.
Lead owns maintained Chromium patches and source integration. Native production
files remain unchanged. Actual native Browser and UnloadController compilers0,
no diagnostics; strict apply0; fresh current-patch review no P1/P2. Manifest:
logs/wp/core.hosted-empty.latest.json.

The accepted launch golden creates two native contents on the same ArcBrowser
without closing it. Current engine automatically closes the browser at last-tab
removal, so its replacement path is absent. Run6 isolates the first-WC counts
from missing replacement setup; this proposal has not been runtime exercised.

Browser::TabStripEmpty retains the browser only for host mode, idle unload state,
and force-skip false. UnloadController::TabStripEmpty likewise preserves idle only
when host and not forced. No ArcBrowserWindow::Close method is suppressed, and
no CDP constructor/factory behavior is guessed or changed.

The force exception matters: HandleBeforeClose109 returns true without changing
state when force_skip_warning_user_on_close is true. OnWindowClosing213 can then
close all tabs while still idle. The original proposal suppressed both ensuing
empty callbacks and their close completion. Browser is the first registered
observer (browser299 before features Init307 / Unload constructor225). The revised
guards let this normal callback path complete, preserving DidClose rather than
depending on the global shutdown manager's later synchronous destruction.

Normal explicit close transitions to kUnloadCompleted before closing tabs;
already-empty explicit close goes straight to completion. Both bypass retention.
Ordinary quit uses the same state path. EndSession/SilentExit set force-skip and
therefore also bypass retention. Unload/download cancellation retains its native
reset/callback route. No generic bulk-close busy gate is introduced or inferred.

After integration, validate ordinary last-tab replacement on the SAME browser,
explicit empty/populated close, exactly-once close/quit notifications, unload
cancellation and forced-close behavior where a native fixture covers it. Passing
source/compile review is not evidence these runtime checks have passed. No app,
build lock, input, profile, real Arc data, native source edit or commit was used
for this revision.
