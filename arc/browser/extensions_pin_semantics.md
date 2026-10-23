**Proposal only: native release pin semantics. Product sources remain frozen.**

The adjacent `extensions_pin_semantics.patch` contains three repository-root
hunks: the Chromium `ToolbarActionsModel` change and two owned companions in
`arc_extensions_context.mm` and `arc_extensions_browser.mm`. No hunk has been
applied to the actual source tree. The lead must first fold the native hunk
into a maintained `patches/*.patch` and land/apply it. **Do not apply either
owned companion first:** their newly reachable native calls can trigger the
current Chromium debug restrictions.

The proposal changes neither service factories nor pref storage scope. Native
OTR persistence scope is unresolved; absence of an OTR guard does not establish
where Arc's original engine stores those writes.

The narrow native change is gated on the exact `arcmod-host-app` command-line
switch. Outside that mode, all original Chromium assertions still run.

| Native method | Host-only change | Preserved behavior |
|---|---|---|
| `SetActionVisibility` | Permit the release-engine paths rejected by the force-pinned, OTR, and stored-pref-membership debug assertions | The visible-state `DCHECK_NE`, real pref append/remove and write, filtered-state consistency `DCHECK`, and actual action dispatcher notification |
| `MovePinnedAction` | Permit OTR calls past its OTR debug assertion | The actual force-pinned early-return guard, nonempty/present-action/stored-action hard CHECKs, native order calculation and rotation, pref write, and filtered-state consistency `DCHECK` |

There is no direct replacement of prefs, manually constructed pinned list,
synthetic observer event, or fake app callback. Unknown IDs go through native
`SetActionVisibility`, including repeated writes when an ID is stored but
still absent from the visible projection. The native getter continues to hide
IDs without toolbar actions. The proposal intentionally does not add a stored
pref deduplication step: it would change the original release behavior.

The context companion preserves stable first-occurrence input deduplication,
the copied previous visible order, per-request pin/readback/gather order, model
reacquisition, and existing visibility safety rechecks. It removes only the
extra OTR early return and HasAction/force-pinned filters. A visibly pinned
forced item now reaches `MovePinnedAction` and consumes its sequential index,
while the native forced-move guard remains a no-op. The existing shared
`_orderSuppression` AutoReset remains around the entire mutation sequence,
including pref notifications and moves. `OnToolbarPinnedActionsChanged`
continues returning while suppressed; no final catch-up DidChange callback
is introduced. Mutation-specific `didChangePinnedExtensionOrder:` delivery
should therefore remain zero. Runtime callback behavior is still unexecuted.

The browser companion pins iff not visibly pinned and unpins iff visibly
pinned, removing extra HasAction/force/OTR filters. Its move path keeps
`index < visible_count` and `IsActionPinned`, and adds the original cache
requirement. `GetCachedActionForId` performs only `actions_.find` and returns
an existing nonnull value; it does not create an action, register observers,
or call the autovivifying `GetActionForId`. Missing cache entries, null entries,
unpinned IDs, and invalid indices remain no-ops. Forced moves reach the native
no-op guard. Existing nullable browser/model/bridge lifetime checks remain.

Retained original evidence, refined by the extensions worker's native scout:

| Original address/path | Consequential evidence |
|---|---|
| `42eb57c`, `research/2026-10-22-sol/evidence/agents/cef/decompile/functions/42eb57c.c` | Stable dedup; omitted visible IDs call unpin; each requested ID is checked/pinned/checked and immediately gathered in the same loop; then gathered IDs call Move at incrementing indices. Lines 195-215 establish that this is not a separate post-pin visibility scan. |
| `7cd94c0`, adjacent `7cd94c0.c` | Native visibility setter reads `extensions.pinned_extensions`, erases or appends without HasAction/force/OTR guards, writes prefs, and calls the real dispatcher. Hidden duplicate-pref writes are permitted by this release path. |
| `7cd9f40`, adjacent `7cd9f40.c` | Native force-pinned guard returns before hard list/action checks; nonforced moves preserve native ordering/pref operations. |
| `35d8bf4`, extensions-scout supplied getter evidence | Unknown IDs remain in prefs but are filtered from visible pinned actions. This address was supplied by the extensions scout; its body was not independently recovered by this worker. |
| `43e2454`, `43e2540`, `spec-3/revisions/v1/bodies/043e2454.txt` and `043e2540.txt` | Browser pin/unpin forward through the actual ExtensionsContainer. |
| `43b6c18`, `spec-2/classreview/backend/043b6c18.c` | Pin only when absent from visible pinned list; unpin only when present. No additional HasAction/force/OTR guard. |
| `43e2714` and `43b606c`, `spec-3/revisions/v1/bodies/043e2714.txt` and `spec-2/classreview/backend/043b606c.c` | Browser move requires model, in-range index, existing nonnull cached action, and currently pinned ID; otherwise no-op. |

All abbreviated `spec-*` paths above are under
`research/2026-10-22-sol/evidence/agents/`. Static decompilation establishes the
bounded paths above; callback timing, observer reentrancy, and complete OTR
persistence parity require actual paired execution. Existing weak-profile,
model-reacquisition, and visibility safety checks are retained rather than
claiming native parity for arbitrary reentrant mutations.

Validation performed without building or launching:

- `git apply --check --whitespace=error arc/browser/extensions_pin_semantics.patch`
  passed against the frozen actual files.
- The same complete patch was checked/applied to temporary copied originals.
  All three resulting files matched the proposed copies byte-for-byte.
- All three proposed translation units passed Clang `-fsyntax-only` with their
  own actual `out/Base` compile command, including the real warning/error/plugin
  flags. Sources were selected through a VFS overlay with
  `use-external-names: false`, mapping both canonical owned-file paths and the
  Chromium `arc` symlink spelling. No source argument was redirected to a
  differently configured translation unit.
- Object and dependency output options were stripped before invocation:
  `-o`, `-MF`, `-MT`, `-MQ`, `-MJ`, `-MD`, `-MMD`, `-MP`, `-MG`, and serialized
  diagnostic output options when present. No build graph command was executed.
- Separate negative VFS sentinels produced the expected unique `#error` for
  each original TU path. This verifies that successful checks actually read
  the overlays, including both owned ObjC++ files, rather than the unchanged
  real sources. These expected failures test overlay routing only.
- Original source SHA256 values were checked again after validation; all
  three actual product/shared source files were unchanged.

| Proposed TU | Actual Base target | Compiler exit | Diagnostics |
|---|---|---:|---|
| `../../chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `obj/chrome/browser/ui/toolbar/impl/toolbar_actions_model.o` | 0 | None |
| `../../arc/api/arc_extensions_context.mm` | `obj/arc/arc_core/arc_extensions_context.o` | 0 | None |
| `../../arc/api/arc_extensions_browser.mm` | `obj/arc/arc_core/arc_extensions_browser.o` | 0 | None |

Temporary copies, overlay, exact stripped argv, command hashes, compiler output,
and negative-overlay results are retained for review at:

`logs/wp/extensions-pin-semantics-proposal`

The original three-TU snapshot at that path is preserved. The refreshed current
context evidence is retained separately at:

`logs/wp/extensions-pin-semantics-proposal/v2-context-refresh`

This v2 refresh incorporates the parent's current native-correct list and
command-enumeration/shortcut implementation into the context base. The actual
pin-setter body was unchanged; only the proposal context hunk offsets and base
fingerprint changed. The complete refreshed patch passed apply-check and was
applied only to copied current sources. Only the combined current-context plus
pin-companion TU was recompiled with its exact Base command and stripped output
and dependency flags: exit 0, no diagnostics. Its new VFS path was independently
verified with the expected context-only negative sentinel.

The native and browser source AND proposed-copy SHA256 values match the
original checked snapshot. Their prior successful compilation and VFS evidence
are retained; neither TU was redundantly recompiled. The table above therefore
combines v1 native/browser evidence with v2 current-context evidence. Parent
wording and prior snapshot logs remain preserved. Actual product/shared sources,
runtime helpers, artifacts, and the source-frozen batch were not modified.

| Frozen original file | SHA256 |
|---|---|
| `chromium/src/chrome/browser/ui/toolbar/toolbar_actions_model.cc` | `af43ceaf3dca0ea0639a257ee18091e4a43759f0be732326eb9b171d16f14d5a` |
| `arc/api/arc_extensions_context.mm` | `ac3bc2f4048cbbc8c65621f37d7058c4255bc8b5bb4a8f6bf9250a67ff65c6fb` |
| `arc/api/arc_extensions_browser.mm` | `b3aabc4d7fd31bc0fb92c72632752205574f1f16efbb8dd621329a3696950b58` |

No synthetic tests mirroring the setter were added. They would not establish
pref propagation or actual app callback behavior. **Meaningful runtime remains
UNEXECUTED.** Before acceptance, the lead needs the same original/current
scenario with real visible fixtures, unknown/hidden IDs, repeated hidden-ID
writes, forced pins, cache-missing browser moves, and invalid move indices.
Measure actual prefs, real dispatcher effects, visible order, and exact zero
context DidChange callbacks during the setter. OTR tests require separately
proven ownership and actual persistence readback; this proposal supplies no
assumed storage destination.

Cleanup for unknown IDs cannot be certified through visible pinned order alone:
the original getter filters them, and omitting an already-hidden ID from a later
context setter does not remove its stored entry. Use an explicitly disposable
test-owned data scope with raw pref baseline/readback or independently verified
removal of that entire test-owned scope after exit. Preserve raw evidence and
never treat filtered getter equality as proof that hidden prefs were restored.

This is a proposed compatibility correction and compile evidence only. It does
not make the package acceptance-clean or validate the source-frozen batch.

A fresh read-only reviewer checked the native bodies, assertion scope, context
algorithm, cache-only browser guard, and documentation and reported no remaining
findings. The reviewer performed no edits or runtime checks.
