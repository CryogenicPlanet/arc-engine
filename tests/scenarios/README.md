# ArcCore boundary scenarios

Run from the repository root. Default `launch-slice` retains its original two-launch steps and CLI flags.

```sh
bun tests/scenarios/check-file.ts tests/launch-slice/run.ts tests/scenarios/*.ts
bun tests/scenarios/record-golden.ts tabs-navigation
arc/tools/diff-run.sh tabs-navigation
```

The TypeScript check uses Bun transpilation. It is separate from the Chromium ObjC++ check-file tool.
A strict TypeScript check was also run using cached TypeScript 5.9.3 and Bun 1.4.3 type declarations.

`record-golden.ts` takes the common build/run lock, refuses a running trace156 app, deletes only that
agent's disposable profile, and records a fresh candidate. Only an exit-zero run with every step passing
and nonempty hooked transcripts becomes `results/golden/arc-1.170-<name>/`. Failed attempts stay in
`arc-1.170-<name>.candidate/`; they are diagnostics and must not be used as goldens. The wrapper refuses
`launch-slice`, so it cannot replace the original baseline. `launch-slice-regression` runs the same steps
into a separate directory.

Each canonical golden includes `steps.json`, `coverage.json`, and `launch-N/record.jsonl`. Coverage lists
all observed class/selector identities and counts across the whole launch, including startup and
engine-internal calls. It is a lower bound, not a statement that every method of a class was exercised.
Native snapshots exclude Apple, Services and Recent Items menu subtrees. PDF output includes a screenshot
and the pierced DOM tree (the MIME handler iframe lives in a closed shadow root).

Fixtures run in a separate process. Synchronous agent-cdp CLI calls otherwise block an in-process HTTP
server exactly when reload or redirect needs a response. The fixture process exits if its parent dies.
Native actions use ArcMod's PID-verified NativeController; page actions use agent-cdp/AgentCDP. No generic
foreground input or selector dispatch is used. With lead authorization (Chirp #133436), the sibling native adapter now supports `NativeController.action(handle, "perform", actionName)`.
It accepts only currently advertised actions, checks the current fingerprint and enabled state, and binds
handles to the snapshot PID so a restart cannot reuse them. AXWindows is inspected explicitly during menu
tracking. NativeController/global keyboard and mouse synthesis and selector fallback remain unavailable;
the unrecorded morning key driver separately supports scoped in-process app events. Observed AXCancel/AXRaise are
used only on exact agent elements when a key window is required. Native action receipts are recorded in
`native-actions.jsonl`. The stdio MCP schema remains backward compatible (press/set).

Read `REPORT.md` for the completed run, accepted coverage, actual diff headers and remaining blockers.
Never infer acceptance from diff-run.sh's exit code alone: inspect `steps.json` and `coverage.json` too.

Feature limits: `downloads` covers the default no-prompt save with exact bytes; `downloads-prompt` separately gates native save-sheet cancellation; `media` tests
playback/pause and media metadata, with PiP in `media-pip`. Tab restoration is separate from opening a
replacement (`tabs-restore`). `dialogs` tests alert/confirm/prompt; beforeunload and HTTP authentication
have their own files. Extension installation uses the explicitly enabled CDP Extensions.loadUnpacked
route, which does not establish Arc's app-level unpacked-install selector behavior.

## Overnight integration and morning queue

`bun tests/scenarios/scoreboard.ts` refreshes every accepted canonical golden's latest completed
fresh-profile replacement steps and exact first two diff lines. It labels recorder versions, literal
comparator scope and stale diffs. Latest observation means the newest valid steps artifact in the canonical
diff-run directory or explicitly reviewed fresh-profile peers listed in `scoreboard.ts`;
steps and diffs stay paired within one reviewed captured run, labeled in the table. The UI renderer
stores its harness in a subdirectory and the paired diff in its parent, pinned by report.json and raw hashes. File dates do
not prove exact build-source attribution. Preserved-profile renderer evidence is separate. `--summary`
prints one line without starting a comparison. `--run-next` runs one least-recently-attempted scenario.
The controller refreshes the current reporting source every minute, including lock waits/cooldowns.

`bun tests/scenarios/integrator.ts` owns a separate kernel lease, runs one comparison at a time and
waits at least 300 seconds after completion. Its optional `--chirp-helper <path>` invokes the enrolled
lead's helper as `python3 <path> post <summary>` hourly, including during shared-lock waits. Credentials
belong outside this repository; subagents must not refresh them. Interrupted active runners must finish
before another controller starts. Never delete the persistent lease inode to bypass exclusion.

Lead Chirp #136425 separately authorizes caller-aware rerecording, including launch-slice first:
`bun tests/scenarios/upgrade-recorder.ts <accepted-CDP-scenario> --lead-recorder-upgrade`.
`bun tests/scenarios/upgrade-recorder-queue.ts --lead-recorder-upgrade` runs one accepted-CDP pass,
skipping successful migrations and preserving a minimum300s gap after each completed helper. It has
its own kernel lease, orphan-runner checks and interrupted-promotion recovery routing.
Each helper takes the common lock and accepts only its explicit CDP whitelist. It preserves a complete
SHA256-verified v1 snapshot and exact `record.v1.jsonl` beside each new transcript; failed candidates
leave canonical untouched. `bun tests/scenarios/upgrade-recorder-check.ts` checks filesystem failure and
recovery paths using isolated dummy artifacts, without an app or real profile. The ordinary recorder's
launch-slice guard remains in force.

`renderer-public-api`, `sessions-public-api` and `sessions-tab-restart` use tests-owned, PID/nonce-bound public ObjC drivers.
Their raw required-call/value/order gates and callback sidecars are independent evidence: the default
caller-aware comparator excludes calls originating in injected fixture images. An empty default diff
cannot establish their API parity. The standard recorder remains first in DYLD_INSERT_LIBRARIES; extra
libraries must resolve inside the current output directory and extra environment keys use ARC_SCENARIO_.
`sessions-tab-restart` has an accepted39/39 original-engine golden (37engineclasses/262selectors/24callbackselectors): two individually closed tabs restore after
a genuine same-profile/new-PID restart, with a bounded opaque Tabs-file presence barrier and exact
ABC/B histories. It never decrypts payloads or flushes storage to force persistence. Its standard
Browser.close shutdown uses a different required path from the source supplemental AppKit-termination probe;
any SIGTERM fallback remains a failing step.

`bookmarks-public-api` has an accepted40/40 original-engine golden (36engineclasses/231selectors/19callbackselectors), adapted from the separately reviewed frozen profile.batch11 fixture. It retains23 snapshot/mutation facts and raw measured date windows. `bookmarks-public-api-raw.ts` independently pairs exact driver-image calls/returns and enforces74bookmark calls/16selectors, DTO receivers, Unicode arguments and membership order. The default v2 comparison excludes these direct driver calls; acceptance comes from its own standard original run, not the archived combined-fixture passes.

The morning `arcmod-command-bar` uses a tests-owned public current-editor AX setter and independently requires a NEW app-origin autocomplete query/paired true return plus actual same-window suggestions. It is written/reviewed and unrecorded, with runtime unverified. Existing navigation coverage remains separate; neither navigation nor block completion follows from the archived autocomplete proof.

See [MORNING.md](MORNING.md) for seven ready-to-record-after-unlock scripts and three incomplete plans/probes.
All `arcmod-*` scripts remain unrecorded overnight. The new in-process key driver tests seven whitelisted
app-event routes, not hardware/WindowServer delivery; Foundation/DNS service results are explicit
sidecars, not engine-selector coverage. No human profile or discovered import source may be used.

An additional app-owner morning case, `application-termination-denial`, is written and reviewed separately: native Quit with a trusted live beforeunload handler, Cancel/same-document responsiveness, retry/OK and bounded browser/helper disappearance. It is unrecorded and runtime unverified; archived single-phase/raw corruption checks do not prove the two-Quit flow. See MORNING.md and logs/wp/scenarios-termination-freeze.json.
