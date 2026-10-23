# APPLIED browser delegate cache-preservation change

Patch: `arc/browser/extensions_delegate_cache.patch`. Evidence: `logs/wp/extensions-delegate-cache-proposal/`. The parent extensions worker applied this owned-file change after fresh review; no shared source or maintained native patch was changed.

Exact browser base SHA-256: `ebc84f16a82b191e33f83da7b188cee175fe11ab73b71aedd595a96e31f6b35a`.
Proposed browser SHA-256: `03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e`.
Patch SHA-256: `4a136a266065a5cd7b905a89a9f61cca893ffdfb4c6492dd52226142ed01aa44`.

## Concrete discrepancy and change

Current `BrowserExtensions::SetDelegate` destroys/rebuilds cached action models on every assignment, including the same nonnil delegate. It clears action throttles, hides live popups, increments popup request serial, and schedules another full toolbar snapshot. The native setter/bridge paths below perform delegate transport changes without those operations.

This proposal changes one owned TU, `arc/api/arc_extensions_browser.mm`:

- Add private `ActionDelegate::SetDelegate(id)` to update its existing weak delegate field.
- Update BrowserExtensions' weak delegate, explicit callback-target presence, and nil epoch exactly as before; update cached ActionDelegates in place.
- Preserve action identities, active popup/pending completion, timer identities/deadlines, and popup serial across same-delegate, nonnil replacement, nil, and reinstall assignments. Do not schedule an assignment-triggered full snapshot.
- Remove `delegate_generation_`, its shutdown increment, and its former setter guard. Its only purpose was guarding the now-removed reentrant destruction/rebuild operation; no users remain. Shutdown still invalidates weak pointers before clearing timers/models, suppresses delegates and hides popups, with its destruction order otherwise byte-identical.

First construction still obtains the model, observes it, creates cached actions, and calls `OnToolbarModelInitialized`; this proposal changes no initial snapshot behavior. Actual toolbar removal and shutdown remain responsible for model teardown. No public Objective-C selector, encoding, helper, Chromium/shared file, or GNI change is included.

## Reviewed native proof

Final upstream report `logs/wp/extensions-action-command-lifecycle.md` SHA `41e25a1bdb8d3de00503274587221652ad41b4d453749cf5dc9b1afca5eca66c`; native evidence JSON SHA `7cca45c364e1e7092f9b7d7d3a7247d791b56cadead45305cd14d2e5f4f8c448`; upstream manifest SHA `576b39e1384783262f1864629ec47d73d6e01d6daa932e1b97e789fbdc9ef91d`. Durable `review-status.json` is PASS. Exact copies and five byte-window checks are retained in `native-proof.json` and `native-inputs/`.

Native image SHA `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`; addresses are unslid arm64 VAs. The reviewed class/method metadata resolves ArcBrowser `setDelegate:`, `v24@0:8@16`, IMP `0x43e1284`.

| Audited window | Byte SHA-256 | Relevant operation |
|---|---|---|
| `0x43e1284–0x43e149c` | `43c339b8d590a64f106d3cb2a196046dcfd33c137a82add08768c33ed5d86416` | Existing nonnil bridge: `0x43e12b4` addresses weak slot +8; `0x43e12bc` calls resolved `_objc_storeWeak`; then return. Nil clears public bridge/controlblock pair at `0x43e12dc`. |
| `0x4332a34–0x4332aec` | `2d03f9b510c56a00960ea33cd239d34d40bbfbe61dfd2fff96149eae18f80a1d` | First/reinstallation forwarding to native weak-pair storage. |
| `0x42e63a0–0x42e6534` | `a433b109761f297567d8dcefa71732e095ceb64832327237f5c9956d60339302` | Stores weak transport pair at native +0xc0/+0xc8, performs reference cleanup; no manager/cache calls. |
| `0x4042a20–0x4042a54` | `75d8639cc08f0ece97d32d0a2b247ed7807be0f2c80681da2127025215c0c0e3` | Known bridge disposal destroys its two Objective-C weak fields. |
| `0x4042420–0x4042484` | `6068eaf6e6ed189b11d95094e0444f9d528c6728ee71c3ea8ab245bdca6a5e1e` | Bridge destructor variants destroy two weak fields; deleting variant frees storage. |

These bounded setter/install/dispose bodies contain no action-cache clear/rebuild, popup close, action-throttle reset, registration reset, or full-toolbar snapshot. They do not prove absence of unrelated cache changes elsewhere.

## Delivery policy preserved, with explicit limits

Queued payloads still snapshot event-time values. BrowserExtensions still resolves its current weak delegate at delivery. A nonnil-to-nonnil assignment leaves epoch unchanged, so already queued work can reach the new delegate with its original payload. Explicit nil still increments the existing epoch and suppresses fresh dispatch; reinstall does not revive old-epoch tasks. Shutdown weak-pointer invalidation still revokes queued work.

This is preservation of the **existing ebc delivery policy**, not a newly proved native cancellation contract. Native nil releases its public bridge; queued strong captures may keep that native bridge alive. Exact pending-capture eligibility and cancellation remain separate work.

ActionDelegate's existing weak-field producer guard remains: weak target expiry can suppress fresh popup events even though BrowserExtensions' explicit transport-presence flag remains true until nil. The proposal only updates that field in place. It does not substitute an explicit registration boolean or claim weak-expiry equivalence. It does not solve the separate native shared-popup-contents ownership gap, OnLoaded/completion readiness mismatch, or possible reentrant host lifetime hazards documented in `logs/wp/extensions-popup-native-order-audit.md`.

## Verification

- Real current `obj/arc/arc_core/arc_extensions_browser.o` command queried read-only from siso. The complete original command is retained in `browser.original-command.txt`; its hash matches the compile capture. `flag-preservation.json` proves every non-output argument stays in order, including optimization, SDK/module, Objective-C ARC, warnings/errors, plugins, defines, and sanitizers. Only dependency/object output arguments were removed. Added flags are the VFS overlay, `-fsyntax-only`, and `-Wno-unused-command-line-argument`.
- Copied exact TU compiles with **exit 0**, empty diagnostics. Both real and symlink paths map to the same copied source. An independent `#error ARC_EXTENSION_DELEGATE_CACHE_NEGATIVE_BROWSER` overlay gives **exit 1** and the exact sentinel, proving source selection. No object, dependency file, build graph, or shared runtime artifact was written.
- A small Foundation/C++ policy harness executes the actual extracted two SetDelegate bodies against fake cached actions/timers. It passes action/timer identity, popup/pending-completion identity, serial preservation, same/replacement/nil/reinstall, event-payload retargeting, old-epoch revocation, retained weak-expiry limitation, and shutdown setter guard checks. Its queue is an explicit model of the existing policy, not Chromium's task runner, observer reentrancy, or an Arc application test. Commands/results are retained in `policy-result.json`.
- Temporary application against exact base is checked before publication. The existing pin V7 patch `76961224c918c46c82c7e435f5586b739fb94caf1d8769c5d052bc6434100d44` composes with this proposal in both orders; all three resulting files are byte-identical (`composition.json`). No existing proposal was refreshed or modified.
- Fresh independent review and final input preservation are recorded in `review.md` and `source-after.json`. All actual source/proposal inputs must remain unchanged.

## Integration request and scenario status

If the lead activates this proposal, apply only to exact browser base ebc, run the official real-file check, then refresh pin V7's browser base/hash/context documentation and revalidate its combined current-source VFS compile. Temporary composition proves no textual conflict; it is not a published pin refresh or a combined runtime result. The native-info-icon proposal's Context/native files and existing profile proposals are untouched. The already-applied icon-subscription removal is preserved; its historical patch is not an additive current-base input.

No app, API, service probe, standard scenario, or golden diff was run for this unapplied change. Historical validation11 popup ordering differs; validation12 popup is equal under different preceding activity. Neither result validates this proposal. A genuine same-browser, same-delegate repeat probe plus subsequent fresh paired validation is the requested runtime acceptance step, owned by the lead/helper worker. No acceptance criterion or full parity is claimed from compile/policy checks.

The native setter proof contradicts the current cache-rebuild-on-delegate-assignment behavior. It is consistent with ARCHITECTURE's weak-delegate/native-boundary rules; queued retention, action shortcut lifecycle, and readiness issues remain explicitly bounded.

## Parent application and current verification

Parent applied exact patch4a136a266065a5cd7b905a89a9f61cca893ffdfb4c6492dd52226142ed01aa44 to ebc base. Actual browser now03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e. Official arc/tools/check-file.sh completed exit0 (logs/wp/extensions.browser.delegate-cache.check-file.log). Historical proposal proof above remains bounded; full product build/runtime verification is pending validation13. Pin V7 refresh is being prepared; do not treat old base documentation as current. The patch is the exact historical applied delta, not additive to the new source.
