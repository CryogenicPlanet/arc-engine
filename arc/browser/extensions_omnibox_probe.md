# Optional browser omnibox probe

**APPLIED after validation10 completed with changed-paths=[].** The parent applied the reviewed helper-only patch and passed strict actual ObjC syntax/TypeScript checks, retained in `logs/wp/extensions.service.omnibox.{clang,tsgo}.log`. Paired runtime is pending. The historical proposal preparation below changed only temporary helper copies and owned patch/document/evidence artifacts; actual product/runtime/app state was untouched during that preparation.

The optional `--browser-omnibox` phase requires explicit command-line fixture mode plus an independent native-helper environment gate. It invokes the real `triggerExtensionOmniboxInput:inputText:disposition:` selector on enabled, nonce-owned A for each proven Arc ordinal 0 through 11. An actual MV3 `chrome.omnibox.onInputEntered` listener reports the received input/disposition/ID/nonce and real tab/window inventories to the nonce local endpoint. Neither added listener creates, navigates, activates or removes tabs/windows.

After rebuilding the helper, the parent-locked paired run can use:

```text
arc/browser/extensions_service_probe.sh run --out results/extensions-service-probe/UNIQUE --fixture-mode command-line --browser-omnibox
```

The existing `--browser-pins` flag can also be selected; its sequence remains before the omnibox phase. Runtime is **not yet executed**. Public-install mode rejects the omnibox flag; no public-enable policy bypass or default startup change is included.

## Exact bases and proposal

| File | Actual source base SHA256 | Proposed SHA256 |
| --- | --- | --- |
| `arc/browser/extensions_service_probe.m` | `810558c095dafc67ff25399524199a11b2940e9a8e0ebeae86f171daa9012b25` | `5bede75ea3575be1d089227b78d858ae6ad6df2689af49b902808354436e5d41` |
| `arc/browser/extensions_service_probe.ts` | `e366b5a7b79dece65c008e19fddcb6fdc41f4b335dae626dabe8a39f3ad676e9` | `3200708cf4024f0d71b120e891b8de3487519b5156ef3fd543d21ba6e522dd02` |

Patch SHA256: `5faac79a9ad8c6167ad12fc37b0cf7480560f81258819aa762c27212ccec14e6`.

The TS base incorporates the parent's post-validation10 recorder wording correction. Raw v2 recordings **can retain direct probe-image public calls and paired returns**, identified by the genuine `ExtensionsServiceProbe.dylib` caller. Current helper scoped projection and the default diff comparator do not establish a verified complete fixture call/return inventory; this proposal leaves `boundaryMethodParity` null. It neither removes caller attribution nor manufactures recorder rows. Retained raw recordings and helper observations remain separate evidence streams.

## Native ABI, valid range and bounded interpretation

- Runtime metadata `arc/tools/arccore-1.170.0-objc.tsv:321` gives exactly `v40@0:8@16@24q32`; original wrapper `research/2026-10-22-sol/evidence/agents/spec-2/classreview/bodies/043e280c.c:42` forwards only below 12 and otherwise traps. The helper rejects negatives, fractions, Boolean/non-number substitutes and values above 11 before any message.
- Original backend `research/2026-10-22-sol/evidence/agents/spec-2/classreview/backend/042fc76c.c:20` maps 0–10 through table VA `0xc9d6830`, defaulting valid ordinal 11 to Chromium value 1. Independent static scout `/root/extension_review/omnibox_semantics` read `[9,1,3,4,1,6,5,1,8,3,3]`; the table bytes hash is `508ed213dd41ad855139d40951719da496a7c3a2282efb7b13726873d4ea55f0`, stock arm64 slice hash `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd`. No app was launched for this read.
- Current `arc/api/arc_extensions_browser.mm:446` reproduces that Arc-to-Chromium map. `chromium/src/chrome/browser/extensions/api/omnibox/omnibox_api.cc:151` looks up the enabled extension and CHECKs it, grants activeTab if requested, dispatches the event with user gesture and notifies the omnibox watcher. This router body does not create or navigate a tab.
- The event contains only three disposition strings: **`newForegroundTab` for 2/9/10**, **`newBackgroundTab` for 3**, **`currentTab` for 0/1/4/5/6/7/8/11**. The expected string is derived only from this bounded native map and router branch. Inputs and returned strings remain actual observations.

All twelve public ordinal arguments are exercised if the phase completes. Observing the three event strings cannot distinguish internal window/popup/OTR/ignore/current casts collapsed by the router. No independent permission-grant result, window creation behavior, invalid enum path, disabled-extension path, incognito context or missing-active-tab path is claimed. Both fixtures already request tabs/activeTab and loopback host access; the phase does not measure fresh permission elevation. The router is not entirely side-effect-free: permission state and listeners/watchers can run.

## Ownership and active-tab proof before every call

ArcBrowser exposes no public active-WebContents getter. Because the router grants permission to its active tab, the parent approved a bounded genuine B-action preflight for every omnibox call. The optional fixture suffix adds a second real action listener that queries `chrome.tabs.query({})` and `chrome.windows.getAll()`, then reports the actual clicked tab, complete inventories, fixture nonce and a monotonically observed action sequence. This is an observation of an actual extension action, not a synthetic query event or substituted callback.

The phase begins immediately after existing `action-B`. Its genuine inventory is retained on that operation. Each of 12 iterations then performs:

1. Real `executeExtensionAction:` for owned B with its own `omnibox-preflight-N` helper operation and recorder span. Require one ordinary click report and one inventory report, expected consecutive sequence, B's real ID and nonce. Require exactly one actual tab and one normal non-incognito window, linked nonnegative integer IDs, and an active tab with the exact nonce fixture URL. Extra tabs/windows fail closed; no activation/removal is attempted to force the precondition.
2. PID-bound CDP check of the existing owned target's actual `location.href` and document nonce. `Browser.getWindowForTarget` must agree with the actual extension-reported window ID. No CDP navigation, activation or worker evaluation is added.
3. Native helper independent CLI/environment gate and exact service/browser revalidation: observed real browser delegate, native extension-service equality both directions plus matching hash, exactly one matching browser and equality with the operation's selected browser/service. The existing outer Default/non-incognito guard remains. Both A/B must pass nonce/name/ID/version/fromWebstore proof and actual enabled/action-bearing Boolean getters; no installs or startup ownership may be outstanding, and baseline capture must exist.
4. Only A label is accepted; a supplied raw extensionID is rejected. The exact input is `${token}/omnibox/${ordinal}/π 東京 🧭`, unique per ordinal and shared between paired engines. The helper checks that input and the phase's exact bound loopback URL, checks runtime object/object/signed-q argument types and void/count ABI, records real ID/input/ordinal/encoding/selector, then invokes `void (*)(id,SEL,id,id,long long)` objc_msgSend.
5. Require exactly one actual A listener effect with exact input, ID and nonce, actual argument count/types, and the native three-string disposition. Check its real tab/window inventory against the preflight IDs, then re-read the CDP nonce target/window and require identical evidence. Full pin order must remain unchanged.

The helper's URL binding is a nonce input guard; it is not presented as a native active-tab getter. The real B listener/CDP observations establish that precondition before dispatch, and A's real listener checks it afterward. The disposable process/lock model remains the existing runner's model; no claim of atomic protection against an independent actor changing the browser is added.

Final audits include all initial-plus-per-ordinal B preflight reports for exact expected count, ordered sequence, ownership, label and nonce, plus all A input reports for exact attempted-input count, unique inputs, ownership and nonce. This catches delayed duplicates after individual waits. Raw late/missing/foreign observations fail completion and remain in effects/observations. Partial phase wording stays incomplete until all twelve iterations pass; no expected listener output is inserted when an event is absent.

## Evidence, parity limits and preservation

Every added public invocation has its own actual helper operation/record span. Before/after listener inventories, original B action effects, exact omnibox arguments/ABI, raw listener effects and CDP identities remain in sidecars and semantic observations. Raw tab/window/target IDs are **not stripped or remapped** across engines, so existing SessionID differences can remain semantic mismatches. Within-run identity assertions and input/disposition facts can be inspected independently; they are not a promotion to total paired method parity. Known false parity remains false and the method-inventory boundary remains unknown/null.

The phase adds 12 B preflight calls and 12 A omnibox calls after the existing action-B, before existing MV3/background, lifecycle, popup, invalid-install and cleanup operations. All 22 existing static `await action(...)` expressions (including the existing optional browser-pins phase function) remain byte-identical after removing only the new block. This preserves the full default CLI 19-operation path, default public path and optional browser-pins ordering/cleanup. The existing extension body is byte-identical when the option is off; optional listeners are appended conditionally. Metadata inventory additionally includes the real omnibox selector. Chromium launch switches are unchanged; only the explicit helper opt-in environment/nonce URL and provenance flag are added.

Original outstanding-install reconciliation, nonce-only fixture uninstall, baseline restoration, PID readiness/provenance requirements and cleanup certification are unchanged. A failure retains actual partial evidence and uses the existing error cleanup. No cleanup result or runtime acceptance is claimed here.

## Temporary verification and review

- Proposed `.m` passes strict ARC syntax and dynamic-library compilation with `-Wall -Wextra -Werror`, no final diagnostics. The temporary dylib was never injected, signed into an app, or launched.
- Proposed `.ts` passes strict tsgo (`noUncheckedIndexedAccess`, `exactOptionalPropertyTypes`) and Bun bundling. Imports resolve to the real sibling agent-cdp source through a temporary symlink, without importing/running the runner in the tests.
- **39 mock-only Objective-C assertions** prove rejection of opt-out/public mode, missing/mismatched/ambiguous browser, unresolved ownership, foreign/disabled/actionless fixtures, wrong label/raw ID, invalid ordinal/types, wrong nonce input/URL, unsigned-Q method ABI, and exact signed-q Unicode dispatch arguments for all twelve valid ordinals. The standalone executable's helper constructor installs no hooks (`gInstalled` remains false); local mocks are not native app evidence.
- **55 isolated TS assertions** exercise the extracted actual required-row/semantic functions, nonce/tab/window inventory guard, default fixture byte identity, optional listener syntax/no-create behavior, raw ID retention, and final effect ledger including delayed last-preflight duplicates. Mock payloads exist only in these isolated guard tests; no mock rows enter a recorder/run artifact.
- Fresh independent reviewer `/root/extension_review/omnibox_review` initially found that delayed B preflight duplicates could escape the first successful wait. The final all-preflight ledger fixes this; focused re-review found no remaining actionable defects. A final CDP window-type narrowing satisfies strict TypeScript without changing the guard's intended condition.
- `git apply --check --whitespace=error` passes. Applying only in a fresh temporary tree yields byte-identical proposed copies. Both actual helper hashes were rechecked unchanged. Source and test logs, before/after copies, exact commands/hashes and bounded native evidence are in `logs/wp/extensions-omnibox-probe-proposal/validation.json` and adjacent files.

An unused function in the first isolated mock harness and a missing TypeScript narrowing after tightening a numeric guard were corrected before final checks. These were temporary proposal/test issues, not product/runtime results. Parent integration, rebuilt-helper verification and real paired runtime remain pending. No scenario diff or fresh parity result is asserted; no architecture/spec contradiction was found.
