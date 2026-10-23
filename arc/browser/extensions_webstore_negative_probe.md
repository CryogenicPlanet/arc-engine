# Unapplied optional Web Store malformed-ID probe

**UNAPPLIED. No real Web Store API call, app, store request, network tool, runtime fixture or shared lock was used.** This proposal changes only temporary helper copies. Actual product/helpers, GNI/build graph, packaged binaries and board were untouched. Parent application and a later locked paired run remain separate steps.

**The bounded guarantee is no Web Store fetch, download or install prompt for two fixed malformed IDs. It is not global networklessness.** Both original and current installer constructors create a dummy WebContents and observe the profile before ID validation. The parent explicitly clarified that unrelated process/profile/Chromium activity and authorized local fixture traffic are outside the intended guarantee. No recursive claim about all WebContents observers is made.

After integration/rebuilding, the optional phase could be selected with:

```text
arc/browser/extensions_service_probe.sh run --out results/extensions-service-probe/UNIQUE --fixture-mode command-line --webstore-negative
```

This command was **not run**. The independent native-helper environment gate is `ARC_EXTENSIONS_PROBE_WEBSTORE_NEGATIVE=1`; both layers reject public-install mode for this new phase. Existing browser-pins/omnibox options remain independent. Default Chromium launch switches and fixture sources are unchanged.

## Exact bases and patch

| Helper | Actual base SHA256 | Proposed SHA256 |
| --- | --- | --- |
| `extensions_service_probe.m` | `5bede75ea3575be1d089227b78d858ae6ad6df2689af49b902808354436e5d41` | `c715415d56337dad54b0ecebfb34809de2a7d4610855afb5a22fdd59b47eea9b` |
| `extensions_service_probe.ts` | `3200708cf4024f0d71b120e891b8de3487519b5156ef3fd543d21ba6e522dd02` | `a506bd8516a7b6df15d5a5faa6f330747ab149db4e0cf00eef305433c3ad774c` |

`extensions_webstore_negative_probe.patch` SHA256: `8a4c38daa4c6795398f786a24e2ea68309ddbd6927a1cdeec544c1c1502b9897`. Actual source hashes were rechecked unchanged after validation. Native metadata is additionally inventoried for the real Web Store selector.

## Original binary and current-source proof

Pinned original image: `../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`, full SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`. `logs/wp/extensions-webstore-negative-native/` retains full bounded bytes/disassembly, offsets, complete decode lengths, range hashes and a reproducible static readback script. Independent scout `/root/extension_review/webstore_native_validation` agreed with those branches and result mappings.

- Metadata at `arc/tools/arccore-1.170.0-objc.tsv:499`: `installExtensionFromWebstore:unattended:completion:`, **`v36@0:8@16B24@?28`**. Original wrapper `[0x43fd304,0x43fd46c)` captures the original NSString/completion and forwards the unattended Boolean to backend `[0x42e6790,0x42e712c)`.
- Backend tests unattended at `0x42e6894`. Both normal and unattended constructor branches converge on `BeginInstall` (`0x784a050`) at `0x42e7000`. The unattended branch replaces the prompt virtual slot with a null-return function and clears post-install UI, but this negative phase does **not** exercise prompt suppression on a valid install.
- At `0x784a08c`, BeginInstall calls ID validation `0x3367708`; `0x784a090` is `TBZ` false to `0x784a168`. The validator's first instruction compares length with **32**; `0x336770c B.NE` goes straight to false. Thus the hardcoded nonnil `@""` (0 bytes) and `@"arc-invalid-webstore-id"` (23 ASCII bytes) fail before any alphabet loop or valid-ID workflow. No valid 32-character store ID or remote URL is an actual call argument.
- Invalid branch constructs the private text `Invalid Chrome Web Store item ID`, sets native result **5** at `0x784a184`, and calls `CompleteInstall` at `0x784a188`. It returns without uniqueness/active-install setup at `0x784a1fc`, fetcher construction at `0x6f2f568`, or fetcher start at `0x6f2f608`. Those calls are exclusively on the ID-valid branch. Prompt/download work follows successful fetch processing, so the invalid branch does not reach it for either unattended value.
- Constructors `[0x786cd50,0x786cea0)` and `[0x7849f00,0x784a050)` precede this validation. The dummy WebContents/profile observation caveat above remains explicit; no claim that the entire process performs no network activity is inferred from BeginInstall's branch.
- Current `arc/api/arc_extensions_context.mm:449` uses `ArcWebstoreInstaller`, which derives from `WebstoreInstallWithPrompt`; source constructor behavior is in `chromium/src/chrome/browser/extensions/webstore_install_with_prompt.cc:17`. `webstore_standalone_installer.cc:54` validates before uniqueness checking and fetcher construction/start; `components/crx_file/id_util.cc:96` rejects any length other than 32. Both helper inputs therefore follow the current invalid branch too.

The expected completion is grounded in native code, not supplied by the probe: `CompleteInstall` calls synchronously at `0x784a594`; callback trampoline `0x42ec4c8` reaches `0x42e712c`. Table `0xc9d67cc` maps native **5→4**; ObjC adapter `0x43fec84` maps **4→public 3** using table `0xc9d8140`, loads the **captured original NSString**, and tail-calls the block. The exposed callback is `(NSString*, NSInteger)` only. The private native error string is not a public NSError/third callback argument and is not fabricated into observations. Current `ArcInstallResult` composes the same public result 3.

The backend removes its install state after callback fanout. The runner sends requests sequentially after the normal settle window, avoiding reentrant same-ID installation requests from inside completion.

## Four real optional operations and guards

After CLI startup adoption proves both nonce A/B fixtures and captures baseline pins, before existing loaded/enable/command/pin/omnibox/lifecycle/error exercise, both engines receive the same sequence:

| Step | Actual ID argument | Actual unattended BOOL |
| --- | --- | --- |
| webstore-invalid-empty-attended | empty NSString | NO |
| webstore-invalid-empty-unattended | empty NSString | YES |
| webstore-invalid-malformed-attended | `arc-invalid-webstore-id` | NO |
| webstore-invalid-malformed-unattended | `arc-invalid-webstore-id` | YES |

Each helper request must pass the independent CLI/flag gate, the selected live service/context/delegate entry, canonical `context.extensions` object identity and service owner identity, actual Default/non-incognito fields, both nonce/name/ID/version/fromWebstore fixture proofs, resolved startup ownership, baseline capture and no outstanding installs or prior uncertain result. The new API has no browser-selection or active-tab requirement. Existing process/readiness/provenance and observed browser checks remain unchanged.

Only named cases `empty`/`malformed` are accepted. Inputs are constructed inside the helper; raw `extensionID`, `storeID` and `url` fields are rejected. Unattended must be an actual JSON Boolean, never numeric 0/1. Runtime method validation requires void/count plus object/**B**/**@?** arguments and one-byte BOOL; the actual call uses `void (*)(id,SEL,id,BOOL,id)` with a nonnil typed `void (^)(NSString*,NSInteger)` block. Selector metadata does not describe the block's internal signature; bounded native callback code/current source establish those two arguments.

Real method encoding, case, actual ID/Boolean arguments, identity proof, attempt flag, complete ordered ID lists returned by public `listExtensions` and full pin order before/after are retained. Native `listExtensions` filters UI-hidden extensions, so this is the complete public-method inventory, **not a raw or complete extension-registry inventory**. Hidden registry state is not dynamically asserted by these snapshots. No service result, delegate callback or recorder row is synthesized. The runner checks actual requested-ID/result3/main-thread/synchronous completion against the native contract and preserves any differing real observation as a failure.

## Synchronous callback and cleanup safety

Before objc_msgSend, the helper sets the work pending, adds the operation ID to the existing outstanding-install set, appends the exact request to the attempt journal and writes ownership evidence. This ordering handles the native synchronous callback without losing the attempt.

At actual callback entry, main-thread and `!work.returned` synchrony are captured under the existing lock before any dispatch to main. The callback records its actual ID/result into the first-completion field, full completions array and event order; only then does normal processing continue. All callbacks, including duplicates, remain recorded. A timeout leaves the attempt outstanding and creates no completion. A genuine late completion resolves its attempt and produces the existing diagnostic failure rather than being reassigned to another operation.

Unexpected result (including success), unexpected returned ID, duplicate completion, or changed public `listExtensions` IDs/full pin order sets persistent `webstoreNegativeUncertainState`. Cleanup refuses mutation/certification in that state. Outstanding callbacks independently prevent certification. The final runner checks ownership uncertainty, and existing fixture-related late diagnostics still fail the run. Guard rejection before any attempt does not falsely mark state uncertain. Normal cleanup continues to uninstall only proven nonce A/B and restore baseline; malformed store IDs never enter owned fixtures or an uninstall target.

The completed-coverage wording is added only after all four observations pass. Failure retains the selected-but-incomplete wording, actual partial observations and strict existing error cleanup. Valid store installs, downloads, account/server behavior, attended UI, and unattended prompt suppression on a real install remain **uncovered**. These negative cases do not satisfy or replace full Web Store installation acceptance by themselves.

## Preservation, validation and review

All **24 existing static `await action(...)` call expressions** are byte-identical after removing only the new loop, preserving the designed CLI 19-operation path, public path and optional browser-pins/omnibox sequence. The entire conditional background.js generation line is unchanged, as are existing fixture files, Chromium startup arguments, operation bodies and cleanup rules except the new fail-closed uncertainty condition. Provenance records the explicit option and child environment gate.

Raw v2 recordings may contain genuine probe-image public calls/returns. Existing helper scoped projection/default comparator limitations and `boundaryMethodParity:null` remain unchanged. Actual helper observations and exact inputs/timing/Boolean values are compared separately; no masks or remapping force parity.

- Temporary `.m`: strict ARC syntax and dynamic-library compile, `-Wall -Wextra -Werror`, exit0/empty diagnostics. The temporary dylib was never injected or launched.
- Temporary `.ts`: strict tsgo and Bun bundling, exit0. Real sibling agent-cdp imports resolve through a temporary symlink; isolated tests do not import or run the runner.
- **53 mock-only Objective-C assertions**: identity/profile/nonce/gate/ABI rejection, fixed typed arguments for all four combinations, journal present before synchronous callback, real callback capture, timeout without fake completion, late completion/diagnostic, duplicate/success/wrong-ID/state-mutation uncertainty, dispatch exception, cleanup refusal and harmless pre-attempt rejection. Mocks are local standalone classes; the constructor installs no real hooks (`gInstalled` remains false).
- **56 isolated TS assertions**: extracted required-row/types/semantic and actual phase predicates reject missing fields, numeric Booleans, arbitrary IDs/URLs, incorrect callback values/timing/thread, duplicates, outstanding work and state changes while preserving actual empty-string arguments, false timing and complete public-list ID evidence.
- `git apply --check --whitespace=error`: exit0; fresh temporary application matches both proposed sources byte-for-byte. Actual source hashes remain the stated bases.
- Fresh independent read-only reviewer `/root/extension_review/webstore_negative_review` found no actionable defects in ABI, ownership, ledger/cleanup, before/after data, existing sequences or parity limits.

Exact commands/results, tests, before/after copies, hashes and metadata are in `logs/wp/extensions-webstore-negative-probe-proposal/validation.json` and adjacent files; native proof is in `logs/wp/extensions-webstore-negative-native/`. No scenario was run, no new diff-launch output exists, and no fresh runtime/API parity or cleanup success is claimed. No architecture/spec contradiction was found; the private error/public callback distinction is clarified by native code.

## Parent integration status

Applied by parent after validation11 completed. Actual helpers match proposed M `c715415d56337dad54b0ecebfb34809de2a7d4610855afb5a22fdd59b47eea9b` and TS `a506bd8516a7b6df15d5a5faa6f330747ab149db4e0cf00eef305433c3ad774c`. Actual strict syntax and TypeScript checks passed in logs/wp/extensions.service.webstore-negative.{clang,tsgo}.log. Runtime verification not yet queued at this note; no valid-store call was added. The patch remains the historical exact applied delta, not an unapplied instruction.
