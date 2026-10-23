# Private initial frontend host gate correction and integration plan

Status: proposal only; this file authorizes no apply or runtime action. Parent owns the
integration transaction; recon owns the executor. Frozen initial5 and later logical-peer
proposals are unchanged. No production/shared/Chromium source was edited by this work.

The exact original chain must be followed by `webextras_devtools_host_gate_held.patch`:

| Prerequisite | SHA256 |
|---|---|
| `webextras_devtools_hooks.patch` | `dcbf472a06bd6a722cfaf591c3a0c4662b8759196d77babeb7fec4ad14bebd17` |
| `webextras_devtools_host_docking.patch` | `d9295b5cd7faea6251b4845d196323c2c8f5dfe31bd25bf26b78b5bfa8f1eac9` |
| `webextras_devtools_embedder_events.patch` | `2fae816858d06e2a78eddb7c32caa0cea0bdd27634adaa0a23dcc67e8944d24f` |
| `webextras_devtools_integration_held.patch` | `a2a9d232a57adc19daaa13c182c719523c4eb2a315f93002c1a08ca1d3c18cad` |
| `webextras_devtools_embedder_companion_held.patch` | `ed9a828d55029381d8b899711f3459b52ed71369acdab881323173d7c69f6bfb` |

The first three change shared Chromium files; the last two change only
`arc/api/arc_webextras_devtools.mm`. They may be reconstructed as independent shared and
owned chains, then the new delta applies to shared output. All chains were strictly
checked and applied in an isolated directory. Per-file actual base, initial5 output and
corrected output pins are in `logs/wp/webextras-devtools-host-gate/reconstruction.json`.
These are evidence of this private capture, not requirements that obsolete whole-file
hashes survive other integrations. Capture and review current bases after acquiring the
actual integration slot; strict-apply/rebase only targeted contexts while preserving
foreign content. The earlier read-only compatibility receipt is
`logs/wp/webextras-initial-frontend-current-base/receipt.json` (`b2ff399e...`).

## Behavior and ordinary Chrome scope

`GetDevToolsURL` appends `arcmodHost=true` only when the browser process has
`--arcmod-host-app`, before its existing `SanitizeFrontendURL` call. The actual query
sanitizer accepts that key only for the same flag and the exact value `true`; every
other value is omitted. Existing sanitizer iteration treats duplicate entries
independently, so malicious/duplicate values cannot advertise the capability in an
ordinary browser. Existing `IsValidFrontendURL` compares the sanitized URL to the
original, and `ReadyToCommitNavigation` rejects invalid frontends. This is a scoped
frontend capability, not a new security boundary or global feature flag.

Actual inspect and dock producers consult `Root.Runtime.Runtime.queryParam`; no new
host call is issued in ordinary Chrome. The compatibility methods independently gate
IPC on the same URL value. The native dispatcher registers the two new method names
only in host mode, and their native callbacks check host mode as well. Host input
mapping, repetition behavior and ordering remain the same as initial5: inspect after
models/toolbar toggle; dock request before CSS/settings/events/ack, same side quiet,
invalid side becomes right; undock initialization still reports once.

New weak guards in bindings LoadCompleted/FrontendLoaded, window LoadCompleted,
OpenNode and the constructor/factory tail run only in host mode. Ordinary branches
retain baseline calls and return values, including the original policy-dialog pointer
and constructor return path. New Focus/FrontendCreated hooks are host-only. Show's
copied callback and post-call weak check remain inside the pre-existing nonempty
Show-hook branch. With its empty default, ordinary Chrome executes exactly the prior
Show path; installing a nonempty hook explicitly opts into that hook branch. New hook
getters/declarations have empty defaults. No hooks are removed or renamed.

## Exact integration file ownership

Chromium paths and the one manifest owner for each:

| Owning patch | Chromium-relative path |
|---|---|
| existing `0004-arcmod-hooks.patch` | `chrome/browser/arcmod/arcmod_hooks.h` |
| existing `0004-arcmod-hooks.patch` | `chrome/browser/devtools/devtools_window.cc` |
| new `0010-webextras.patch` | `chrome/browser/devtools/devtools_ui_bindings.cc` |
| new `0010-webextras.patch` | `chrome/browser/devtools/devtools_ui_bindings.h` |
| new `0010-webextras.patch` | `chrome/browser/devtools/devtools_embedder_message_dispatcher.cc` |
| new `0010-webextras.patch` | `chrome/browser/devtools/devtools_embedder_message_dispatcher.h` |
| new `0010-webextras.patch` | `third_party/devtools-frontend/src/front_end/core/host/InspectorFrontendHostAPI.ts` |
| new `0010-webextras.patch` | `third_party/devtools-frontend/src/front_end/core/host/InspectorFrontendHostStub.ts` |
| new `0010-webextras.patch` | `third_party/devtools-frontend/src/front_end/devtools_compatibility.js` |
| new `0010-webextras.patch` | `third_party/devtools-frontend/src/front_end/panels/elements/InspectElementModeController.ts` |
| new `0010-webextras.patch` | `third_party/devtools-frontend/src/front_end/ui/legacy/DockController.ts` |

The final five paths are regular tracked leaves in a separate frontend checkout.
Chromium records `third_party/devtools-frontend/src` as mode160000 gitlink
`bf69d041b0ad1f74730204a73bff29f3ef1d4ead`, not five regular outer-repository files.
The current outer-only regen tool cannot preserve these edits. Clock audit owns the
distinct reviewed repo-aware regen/sync correction and roundtrip tests. Its proposed
manifest retains the two-column outer-relative paths above. Do not add or commit a
changed gitlink, pretend the inner leaves are tracked by outer Git, or generate an
empty patch. Parent must obtain the exact tool-owner ACK before adopting that separate
proposal; this host gate does not change those tools or MANIFEST.

Additional shared API requirement: exactly one declaration
`bool ArcRuntimeFeatureEnabled(NSUInteger identifier);` in the webextras section of
`arc/api/arc_internal.h`, if still absent at apply time. Preserve concurrent header
changes; do not copy a historical header or pending app patch. No implementation or
Feature44 default changes are proposed. Owned initializer/callers retain the existing
Feature44 decision. `arc/wp/webextras.gni` and other package files need no change.

## Bounded transaction candidate for the parent/recon executor

1. Complete fresh reviews of this delta, repo-aware tools and executor; obtain owner
   ACK for exact shared tool edits. Acquire one ancestor `arc/tools/locked.sh` slot,
   confirm disposable app/helper processes are absent. Capture actual current targeted
   source/header/MANIFEST, relevant Git HEAD/index/gitlink state, all patch destination
   bytes and required baseline/golden/harness/tool inputs. Use `GIT_OPTIONAL_LOCKS=0`
   for actual-repository reads. A lock alone does not freeze foreign writers.
2. Reconstruct the three shared + two owned proposals + this gate privately against
   those current bases, including the current one-line declaration. Review changed
   contexts if strict application fails. Verify exactly-once manifest ownership and
   nested checkout identity, rejecting unsupported index/gitlink/symlink/conflict state.
   Record expected output bytes before any actual mutation. Preserve foreign deltas.
3. Apply only the reviewed bounded outputs and manifest additions; run reviewed
   repo-aware `arc/tools/regen-patches.sh`. It may regenerate foreign owning patches:
   preserve/capture their inputs and verify expected outputs, rather than erasing them.
   Prove clean replay in private outer+nested fixtures, exact intended source bytes,
   unchanged index/gitlink, and no unintended files/foreign changes.
4. Check the real current build flags for all three native DevTools translation units
   and the owned ObjC++ unit; run the real frontend build/type checks and package build.
   Phase-freeze all eleven Chromium sources, owned source, current header, MANIFEST,
   all regenerated patch destinations, adopted tools and transitive execution inputs.
   Verify phase seals after generation, build, packaging, every runtime and comparison.
5. Reuse `webextras_scenario_current_run.py --locked` under that ancestor, selecting
   fresh positive run numbers for launch-slice, devtools, media, pdf and media-pip.
   It already does API generation/build/package/canonical harness/frozen comparator,
   exact app/helper identity checks, profile rename journaling and restoration without
   deletion or process killing. Executor must add its wider integration source seals
   and separately reviewed cache restoration; avoid duplicating this runtime machinery.
   Do not invoke current `diff-run.sh` beneath the lock: it reacquires it and deadlocks.
   The equivalent standard pipeline is explicitly reported, not mislabeled a direct
   diff-run invocation. Compare the exact frozen baseline outcome shapes: launch9/9,
   media/pdf/PiP no regression; DevTools must retain its exact existing inconclusive
   shape or fully improve under the unchanged scenario. Keep raw recordings, full diff
   arrays and callback ordering; baseline dirty boundary diffs remain parity=false.
6. On any apply/check/build/runtime/phase-seal regression, stop further runtime and
   restore only transaction-owned bytes to captured slot-entry bases, after confirming
   live bytes are still the transaction outputs. Preserve concurrent foreign edits;
   fail closed for manual reconciliation if they intersect. Regenerate patches from
   the restored combined source state and verify replay/foreign preservation. Ensure
   all owned processes have naturally exited and profile/cache recovery is verified
   BEFORE returning/releasing the slot; the parent executor owns interrupted recovery.
   On success retain the same cleanup proof, report exact scenario outcomes + SHA pins
   and post the authorized APPLIED receipt. No parity/acceptance claim from harness
   success alone.

## Private validation and remaining holds

`python3 arc/browser/webextras_devtools_host_gate_test.py` reconstructs only private
copies (`--proof-dir /tmp/<fresh-proof>` preserves frozen evidence), strictly replays the published delta, and executes actual extracted producer,
compatibility, load, OpenNode, native capability, handler and factory-tail bodies.
Twelve producer ordinary/host comparisons, five compatibility gates, native baseline
comparisons under simulated weak expiry, malformed/duplicate capability values, five
host factory reentry cases and native callback-copy/self-destruction gates passed. Three specific extracted-body
negative controls (ungated inspect, compatibility IPC and ordinary OpenNode lifetime
guard) each failed at the expected behavior assertion.
Three native DevTools units and the unchanged composed owned unit passed genuine
siso-query build flags with private VFS overlays, exits0 and empty diagnostics. The
shared header overlay was captured fresh and adds only the missing declaration.
These are extracted/mocked runtime tests and syntax checks, not a frontend bundle,
real constructor/ClosePage execution, app run or boundary acceptance.

No logical Root construction, peer8dcc/cachef702/cleanup e00a activation, rebinding,
Browser creation or global Feature44 enablement is included. The initial pipeline's
physical WebContents weak guard remains a proxy with incomplete native lifetime
coverage; the newer independently proven focus self-retention is not implemented by
this delta. Canonical frontend wrapper identity versus the native fresh getter,
Browser-null hosted undocked close, immediate closing-window F2 reuse, arbitrary
constructor reentry, complete native connection/retirement semantics and all existing
Root lifetime holds remain. No frozen proposal or native evidence is rewritten.
