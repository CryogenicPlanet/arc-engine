# Deferred extension lookup and command-dispatch correction

Applied to the owned context/probe sources after validation4 released its lock.
Current real/official native checks, strict TypeScript, Bun, and helper build
passed; a fresh review found no issues. Validation5 is queued to exercise the
change. Runtime parity remains unverified. The patch and temporary evidence
below retain the reviewed pre-application snapshot. This change does not add
the separate command-line positive-fixture phase.

## Proposed behavior

- `getExtensionInfo:` keeps the enabled/disabled/terminated mask (7) and wraps any found extension. Remove extra display/type/component/incognito filtering. This follows the lead-verified stock route `0x43fd5b0 -> 0x42e7df4`.
- `executeExtensionCommand:commandName:` looks up only the enabled extension. It dispatches `commands.onCommand` with exactly one native argument, the supplied command string, and user gesture enabled. No browser selection, tab object, activeTab grant, UI/incognito filter, name validation, or reserved-action special case. This follows the lead-verified stock route `0x43fda04 -> 0x42eb354`.
- Remove the now-unused visibility helper and command-only browser/tab imports/category declaration. Leave `listExtensions` and `getAllExtensionCommands` byte-for-byte unchanged.
- The generated worker records actual listener `argumentCount`, `argumentTypes`, `noTab`, command, and its real `chrome.runtime.id`. Require a string command and no tab/other value, retaining any optional undefined slots and the exact observed count for paired comparison. Do not hardcode JavaScript listener count to one.
- Keep the actual Action-B `onClicked` effect proof: installed fixture ID, a numeric tab ID, and the exact nonce fixture URL. Context command dispatch has no target-tab or independent activeTab-grant coverage.

## JavaScript binding evidence

The current commands event schema declares an optional `tabs.Tab` after `command` (`chromium/src/chrome/common/extensions/api/commands.json:37`). The current default binding path does not pad it:

- `APIEventHandler::FireEventInContext` converts only the elements supplied in the native list (`extensions/renderer/bindings/api_event_handler.cc:294-300`).
- The no-massager path validates a const argument vector then passes it unchanged to the emitter (`api_event_handler.cc:352-366`). No commands-specific handler/massager was found in the current chrome/extensions renderer sources.
- `EventEmitter::DispatchSync` calls the listener with the same vector (`event_emitter.cc:268-274`). Its asynchronous route constructs an array of exactly that size and restores exactly its length (`event_emitter.cc:345-347,411-416`).

Thus current source predicts listener count 1 for a one-element native payload. Stock Arc's live JS binding count remains unmeasured. The proposal records and compares it exactly, allows only undefined optional padding, and never adds a synthetic tab or normalizes a count difference away.

## Validation performed without applying the patch

All validation artifacts are isolated under:

`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-command-proposal-7hd8rt_i`

- Real current Chromium browser compiler flags, `-fsyntax-only` and `-ivfsoverlay` mapping both lexical and resolved context source paths to the proposed temporary copy: **exit 0** (`clang.log`, `clang-command.json`, `overlay.json`). No object/dependency output flags retained.
- Existing local `tsgo` with `--strict --noUncheckedIndexedAccess --exactOptionalPropertyTypes --noEmit`, against a temporary mirrored probe and read-only tool symlink: **exit 0** (`tsgo.log`).
- Bun build of only the temporary proposed probe into the temporary `bundled/` directory: **exit 0** (`bun.log`). Existing helper binary and bundled runtime JS were not overwritten.
- `node --check` of the generated temporary background worker: **exit 0** (`background-syntax.log`). Worker not executed and no fake callbacks emitted.
- `git apply --check arc/browser/extensions_command_dispatch.patch`: **pass**, without application.

Fresh review: **approved, no blocking findings**, by independent read-only reviewer `/root/extension_native/command_patch_review`. Reviewer confirmed both audited native routes, unchanged list/command enumeration, actual argument-count capture, and unchanged Action-B tab proof. Additional binding check: `api_signature.cc:470-480` permits omitted optional trailing arguments without inserting values. Reviewer performed no edits/build/app/lock actions.

## Snapshot and remaining gaps

Patch bases:

- `arc/api/arc_extensions_context.mm`: `ac3bc2f4048cbbc8c65621f37d7058c4255bc8b5bb4a8f6bf9250a67ff65c6fb`
- `arc/browser/extensions_service_probe.ts`: `ff838f1c0ae1c9faad66dcaa6c5f5a4df2be31afff50eb7db3d455d1b491550f`

The freeze audit found unchanged SHA256/size for both target sources, probe `.m`/`.sh`, existing probe dylib, and bundled runtime JS. During validation the context source mtime changed externally while its SHA256 and size stayed identical; `freeze-check.json` records the exact before/after values. No command issued by this worker wrote an actual source, acquired a shared lock, ran an app, or overwrote a runtime artifact.

No runtime parity established. The default public-install phase can still be blocked by native unpacked-extension policy before command execution. The separate positive command-line fixture phase remains a plan. This patch does not claim execution coverage for component/hidden/incognito lookup, reserved or unregistered command strings, disabled/unknown command targets, optional binding padding, or absence of an independent activeTab grant. Those require later real-engine observations; the proposal makes no simulated callback claim.
