**Parent status: APPLIED to owned sources after validation6; do not reapply this historical proposal. Official check-file passes. Runtime correction pending validation7.**

# Deferred extension-info action availability correction

`extensions_info_action.patch` is unapplied and changes only the owned
`arc/api/arc_extensions_context.mm` producer. Actual product/helper/runtime files
remain frozen. Temporary copies, native instruction evidence, source hashes,
compiler argv/results and overlay sentinel are retained under
`logs/wp/extensions-info-action-proposal`.

## Observed state and native producer

Paired `validation6-public` observations use the same actual A/B fixture IDs.
Stock reports `enabled=false`, `hasAction=false`, `icon=null` after successful
public installation, initial snapshot and unsuccessful public enable. Our
current producer reports the same disabled state but `hasAction=true` and an
NSImage of logical size32 with representations32/64. These are actual helper
observations, not proof of canonical app-demand boundary coverage.

Stock `validation6-cli` supplies a positive/lifecycle cross-check: enabled A/B
have `hasAction=true` and icons32/64; disabling A returns false/null; reenabling
A restores true/32/64; reload converts A to disabled false/null. The exact
step/ID/state excerpts and SHA256 of each observed JSON snapshot are retained
in `observed-state-excerpts.json`. No runtime was launched for this proposal.

The correction follows the native producer, not the DTO stored getters:

| Native path/address | Proven behavior |
|---|---|
| `getInfo 0x42e7df4`, existing classreview body lines22-28 | Registry lookup mask7; found extension goes to producer `0x433513c(profile, extension)` |
| `list 0x42e65f4`, existing classreview body lines29-41 | Calls the same producer for each included extension |
| `0x43352a0–0x43352b0` | Calls action-manager factory `0x258fc1c(profile)`, then action getter `0x258fa84(manager, extension)`; result retained in x24 |
| `0x43352b4–0x4335328` | Null action creates an empty gfx image; nonnull action invokes `0x6ae40ec` before image conversion |
| `0x43353b0–0x43353dc` | Computes/stores `hasAction=(x24!=nullptr)` separately from the registry-based enabled flag |
| `0x258fa84` | Searches existing action cache first; on miss checks manifest action and enabled registry before constructing/caching an action |
| `0x6ae40ec` | Action default-image getter: existing default-icon-image branch, otherwise cached/generated placeholder branch |
| `0x4e12a78` | Native action-icon resource size is literally32 (`mov w0,#0x20; ret`) |

The pinned stock binary SHA256 is
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.
The capture script reads the arm64 FAT slice and copies only bounded native
instruction ranges into disposable minimal Mach-O containers at their original
virtual addresses. LLVM disassembles those exact bytes; no code is executed.
Per-fragment metadata retains original binary hash, native bounds, raw-byte
hash and command. The initial whole-binary range invocation ignored Mach-O
range flags; that redundant temporary output was discarded and is not evidence.

Chromium `extensions/browser/extension_action_manager.cc:88–121` implements the
same cache-first manager route; unload erases the cached action at lines81-85.
The factory redirects to the original context at lines47-51. Therefore neither
manifest presence nor `enabled && manifest` alone reproduces this producer.
The DTO getters merely expose stored snapshot fields and are unchanged.

## Minimal proposed change and limits

`MakeExtensionInfo` now obtains the real `ExtensionActionManager` action once,
sets `has_action` from its pointer, and calls the existing32px `ExtensionIcon`
helper only when that pointer exists. A missing profile/manager/action retains
false and the empty snapshot image. The enabled registry query is unchanged.
No long-lived pointer, toolbar/browser selection, synthetic image, callback,
policy override, fixture special case or DTO getter change is introduced.

The existing positive icon cache is deliberately retained for this bounded
availability correction. It still loads product icons into a profile-owned
custom cache. Native uses the action's default image, including action-specific
icons and native placeholder/cache behavior. This patch therefore fixes the
proven false/null availability mismatch without claiming full source, pixel,
placeholder, async-load or cache equivalence.

Directly switching to current Chromium `action->GetDefaultIconImage()` would
expose its16px action image/placeholder, regressing the observed native32/64
positive shape. Current Chromium `ExtensionAction::ActionIconSize()` returns
`EXTENSION_ICON_BITTY=16` on desktop; the pinned native literal is32. The action
manager may create its own internal16px image when queried, but this bounded
patch continues exposing the existing32px cache rather than that internal image.

## Request to lead for full image follow-up

Review a separate maintained Chromium compatibility patch to
`extensions/browser/extension_action.cc`: under the exact `arcmod-host-app`
switch, use native32 (`EXTENSION_ICON_SMALL`) for `ActionIconSize`, retaining
ordinary Chromium values outside hosted mode. Then switch the owned producer
to the actual manager action's `GetDefaultIconImage()` and remove its custom
snapshot icon cache as a coordinated follow-up. That would restore action-icon
precedence, manager-owned lifetime/cache and placeholder construction as well
as size. This proposal does not edit shared Chromium sources or assert the
broader shared behavior is validated. Pixel/placeholder and lifecycle evidence
must be checked on real paired fixtures, including distinct product/action
icons and missing icons.

## Compile, overlay and ownership evidence

The proposed current context TU passed `-fsyntax-only` with its exact own
`out/Base` compile command: exit0, stdout/stderr empty. All original warning,
`-Werror` and Clang plugin flags remain. Object/dependency/serialized-diagnostic
outputs are stripped. No build graph, build/package or app command ran.

VFS maps both the canonical owned source and Chromium's `arc` symlink spelling,
with `use-external-names:false`. A separate negative overlay produced the unique
`ARC_INFO_ACTION_NEGATIVE_CONTEXT` error, expected exit1, proving the compiler
read the overlaid TU. `git apply --check --whitespace=error` passed. These checks
do not verify runtime behavior or image pixels.

- Actual context SHA256: `468bc9da0bdf22561ccb96e2e9af53b6ff5d9087c8b0bb21d1b584c6d62098e5`.
- Proposed context SHA256: `fa9499ad71a54132e3f6c1c80ffb089028f89cbfe81f15e1d886c2134b7e1b38`.
- Patch SHA256: `bc8353e65e0fe03931f177104be8ba64ecdb9e3621bacba26e233c1945949402`.

Fresh read-only review approved with no blockers for this bounded correction,
independently checking native pointer/image branches, cache semantics, strict
compile/sentinel evidence and patch application. Full image source/pixel
parity remains unresolved. Actual source/helper/runtime hashes, sizes and
mtimes are recorded and verified unchanged in `freeze-check.json`.

Only this proposal patch/document and new evidence were written. No actual
product/shared/helper source, runtime artifact, active observations, profile,
app, shared lock or board state was modified. Scenario acceptance and runtime
execution of this correction remain **UNEXECUTED**.
