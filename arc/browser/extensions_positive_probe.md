# Combined deferred positive extension probe

Applied by the extensions worker after validation5 completed. This single patch
supersedes `extensions_command_line_probe.patch` and
`extensions_browser_discovery.patch`; do not apply those two in addition. It
provides one concrete `.m`/`.ts` source version, with no `.sh` or product changes.
Actual helper build and strict TypeScript checks are being repeated before
runtime. The temporary pre-application evidence below remains preserved.

Only this patch and document are written to the checkout by this integration task. Temporary proposed/base sources, independent dylib/bundle outputs and logs are under `/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-combined-proposal-d2cay3wm`. Actual helper/product source files and existing runtime dylib/JS content, sizes and mtimes remained unchanged. No apps, shared builds, locks, actual fixture files or profiles were touched.

## Explicit phase and real ownership

Default `public-install` preserves its operation order, public A/B installation and enabled/reload assertions. Explicit `--fixture-mode=command-line` (or separate argument value) is a distinct environment and does not establish public-install success.

Both engines receive identical canonical absolute nonce fixture A/B paths through one `--load-extension=A,B` argument. Manifest/worker/popup/icon hashes and launch intents precede launch, and the intent is subsequently bound to the actual PID. The inherited engine/source/recorder/export/Mach-O provenance and native app-window/helper/CDP PID readiness gates remain unchanged. Comma-containing CLI paths fail before launch.

The helper owns pending startup intents separately from actual adopted IDs. `adopt-command-line` reconciles actual public list/getInfo results against exact nonce name, version `1.2.3`, returned real ID and `fromWebstore=false`; conflicting nonce IDs fail. CLI mode cannot call the public A/B installer and produces no synthetic install completion. Missing startup ownership stays unresolved even if ready or after process exit. After both actual fixtures appear, baseline pin order excludes only proven adopted IDs. Cleanup remains strict about outstanding installs, actual identity/absence and exact baseline restoration.

Every real callback entering before adoption is independently retained in the startup observation stream, including callbacks during incomplete discovery attempts. Its actual per-operation attribution remains separate. Late/unassigned/outside-context fixture callbacks remain visible failures. No callback/error result is fabricated.

## Browser selection through the native service

Actual browser `setDelegate:` captures populate one global pointer-personality weak-browser/weak-real-delegate map independent of temporary context wrappers. Original setter and callback forwarding stays intact. Request-local candidate `browser.context.extensions` objects are compared against the actual captured extension service using audited public `isEqual:` in both directions plus public hash agreement. Exactly one matching browser is selected; zero or multiple matches fail the existing registry/browser precondition. No activation, basename/pointer-context routing, fake browser or strong persistent retention is added.

Raw `browserCandidates` records actual equality/hash/ABI results and target/candidate context-ID fields. Exposed ID-field agreement is diagnostic only. Metadata `isEqual:B24@0:8@16`/`hash:Q16@0:8`, spec rows40-41 and exact native bodies `0x43fca30`/`0x43fcba0` were audited: identity is native optional/service identity, independent of wrapper identity. Hash requires an engaged native optional; detached/closed-service behavior remains an unvalidated runtime limitation of the public getter sequence. Actual Action-B tab ID and exact nonce URL assertions remain mandatory.

## CLI ordering and failure sample

CLI adoption is followed by native metadata/list/commands, explicit public enable attempts and strict enabled preconditions. Positive pin/command/action/negative-MV3-JS/disable-enable/popup observations occur before reload. CLI reload retains the actual typed `enabled` value exactly: a fresh command-line extension may reload through ordinary unpacked policy. No reinstall, policy override or repair is performed.

Only CLI mode moves malformed-manifest installation to the final diagnostic stage after the positive operations and reload, before uninstall/cleanup. Both engines use the same CLI sequence. The default public-install phase still attempts malformed installation immediately after the initial snapshot and before public enable. A stalled final diagnostic preserves earlier positive observations but still fails the run and leaves cleanup uncertified while its native completion is outstanding.

Any failed install operation triggers a passive one-second `/usr/bin/sample` attempt before cleanup. Preconditions require the exact already-launched PID to own the verified agent endpoint, its exact disposable executable command and completed native readiness. The sampler has a five-second process timeout, numeric PID only, no process-name fallback, no debugger attach, no `-wait`/`-e`, no global sampling and no input. Before/after executable/start-time and endpoint checks, source/binary freshness, exit/stdout/stderr, and exact sample-file hash are retained in per-step sidecars. Verification/sampler failure is recorded diagnostically and never substitutes a native completion or weakens ownership checks.

`--no-error-dialogs` remains a recorded fixture environment precondition. The actual timeout's cause is unresolved; a warning dialog is not established and no dialog-cause assertion is made.

## Recorder evidence

Recorder v2 excludes probe-image calls, and default `diff.py` also drops non-app probe calls under caller attribution. A clean empty standard diff cannot prove public-method parity. Helper semantic and typed required-row sidecars preserve actual native values, callbacks/completion interleaving, pins and real effects independently. Startup callbacks compare separately. Known differences remain false even with zero valid observations; zero paired observations cannot exit successfully.

No verified paired fixture-call/return inventory or caller-preserving probe-image opt-in is added. `boundaryMethodParity` remains null, and standard/overall parity is unknown unless a known mismatch makes it false. Nonempty retained app/engine rows also cannot establish fixture-method coverage; their true caller labels are preserved. Canonical extensions acceptance, positive legacy background JS and executable browser-category pin/unpin/move coverage remain separate/uncovered.

## Checks and review

Final temporary checks passed: strict `xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework AppKit`; existing strict `tsgo --ignoreConfig --noEmit` with unchecked-index/exact-optional checks; Bun bundle; generated worker `node --check`; existing runner `bash -n`; `git apply --check`; evidence-schema and mismatch/zero-count precedence checks. These execute no app callbacks, native probe requests or samples. Logs plus frozen-source/artifact audit are retained under the temporary directory.

Fresh combined read-only review approved with no concrete blockers. The reviewer independently checked combined mode/order, strict adoption/cleanup, weak service discovery, callback attribution, bounded PID sampling and unknown method parity. Existing strict late/outside-context rules may conservatively fail a delayed startup callback even though its independent startup journal is retained; they cannot turn it into false success.

Patch bases: `.m` SHA256 `c36966c10fbac3a1eae4536261ae10afc7a15ca37745a2018848accdc11f9871`; `.ts` SHA256 `3d5b2f22972d7fd0e7a828254796bb743678ff46d15dec2d7124ed2901657355`.

## Runtime follow-up

No runtime diff was run for this version. After helper rebuild, use a fresh
output directory and acquire the shared runtime lock:

```sh
arc/tools/locked.sh arc/browser/extensions_service_probe.sh run \
  --out results/extensions-service-probe/cli-UNIQUE \
  --fixture-mode=command-line --agents trace156,ours156 --port 9458
```

Stock may reject/ignore CLI loading. Popup presentation may remain unavailable on a locked desktop. Dirty disposable app/profile state can prevent native readiness; no reset is performed. A later fresh-profile proposal must verify exact disposable bundle/no live process under the shared slot, journal canonical owned paths, preserve recoverable backups, and separately verify native Arc state isolation rather than assume Chromium `--user-data-dir` suffices. Unresolved startup or install ownership requires explicit later cleanup evidence, never a clean claim based on process exit.
