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
foreground input or selector dispatch is used. Current native tools cannot invoke AXConfirm, keyboard
shortcuts or Arc's custom sidebar actions; related probes remain nonpassing until that capability exists.

Read `REPORT.md` for the completed run, accepted coverage, actual diff headers and remaining blockers.
Never infer acceptance from diff-run.sh's exit code alone: inspect `steps.json` and `coverage.json` too.

Feature limits: `downloads` currently covers the default no-prompt save with exact bytes; `media` tests
playback/pause and media metadata, with PiP in `media-pip`. Tab restoration is separate from opening a
replacement (`tabs-restore`). `dialogs` tests alert/confirm/prompt; beforeunload and HTTP authentication
have their own files. Extension installation uses the explicitly enabled CDP Extensions.loadUnpacked
route, which does not establish Arc's app-level unpacked-install selector behavior.
