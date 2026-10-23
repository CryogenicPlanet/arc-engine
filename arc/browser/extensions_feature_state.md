# Extension feature-state audit: superseded by app integration

The individual-feature startup proposal is **superseded and withdrawn**. No
`extensions_feature_state.patch` remains, and no product source was changed.
Integrate the app owner's existing
`logs/wp/app.pending-integration.patch:49-60` instead. It defaults hosted Arc to
`--disable-field-trial-config` when neither explicit field-trial-config switch
is supplied. Do not add separate forced disables for these two features, and
do not change Snapshot's tooltip or pinned-index construction.

## Concrete source of the overrides

The current Base build enables field-trial testing and uses Chromium branding:

- `chromium/src/out/Base/gen/components/variations/service/buildflags.h` has
  `FIELDTRIAL_TESTING_ENABLED=1`.
- `chromium/src/out/Base/gen/build/branding_buildflags.h` has
  `GOOGLE_CHROME_BRANDING=0` and `CHROMIUM_BRANDING=1`.
- `ShouldUseFieldTrialTestingConfig` in
  `components/variations/service/variations_field_trial_creator.cc:133-155`
  applies the testing configuration by default for this build unless disabled.
  Lines 329-333 actually apply it.

Both overriding experiments include `mac`:

| Testing configuration | Mac experiment | Feature enabled |
| --- | --- | --- |
| `testing/variations/fieldtrial_testing_config.json:8102-8120` | `ExtensionsPinnedByDefault` / `Enabled_20260716` | `ExtensionsPinnedByDefault` |
| `testing/variations/fieldtrial_testing_config.json:8121-8138` | `ExtensionsToolbarAndMenuRedesign` / `Enabled` | `ExtensionsMenuAccessControl` |

These are also present in the generated, Mac-specific Base configuration,
`out/Base/gen/components/variations/field_trial_config/fieldtrial_testing_config.cc`:
the feature arrays at lines 11660-11665 and 11684-11689 contain these names,
and the adjacent platform arrays specify `Study::PLATFORM_MAC`; study entries
at lines 18831-18837 name both experiments. These are current generated build
inputs, not merely experiments
for another platform in the source JSON.

Thus the testing configuration supplies a concrete override producer for both
observed differences. Before this configuration was identified, the origin
was unresolved; it is no longer necessary to hypothesize a changed compiled
default or a model algorithm difference. Actual post-integration runtime
verification remains outstanding.

## Native and current compiled defaults

Static reads of the original arm64 ArcCore in
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/ArcCore`:

| Original feature object VA | Name pointer VA | Default-state word at +8 |
| --- | --- | --- |
| `0xe2ea3b0` | `0xd26d6a5`: `ExtensionsPinnedByDefault` | `0`, disabled |
| `0xe2e8080` | `0xd2210f8`: `ExtensionsMenuAccessControl` | `0`, disabled |

Current source likewise defaults pinning disabled in
`chrome/browser/ui/ui_features.cc:152-154` and menu access control disabled on
desktop in `extensions/common/extension_features.cc:161-167`.

Native `OnExtensionInstalled` at `0x7cd9280` checks the pin feature at
`0x7cd9370`, reads `extensions.pinned_by_default` at `0x7cd9394`, and combines
these with the existing policy, enterprise, sync, and action checks before
calling `SetActionVisibility` at `0x7cd94b4`. Its algorithm matches the current
`ToolbarActionsModel::OnExtensionInstalled`.

Native Snapshot calls `GetTooltip` at `0x43b673c -> 0x7d1d0cc`. That method
checks menu access control at `0x7d1d13c-148`; the disabled path tail-calls
`GetAccessibleName` at `0x7d1d184 -> 0x7d1cea4`. The legacy granted-access
suffix is `Has access to this site`. The enabled branch uses the newer menu
tooltip resources, including `Allowed to read & change this site`.

Native single-action publication scans the actual pinned vector at
`0x43b7dfc-0x43b7e98` and passes an engaged zero-based index only for a matching
ID. Own Snapshot does the same. Index zero is valid; it must not be normalized
to nil.

## Recorded runtime differences

The original finalized
`results/golden/arc-1.170-extensions-page-actions/launch-1/record.jsonl` records:

- Sequence 2990: `didChangePinnedExtensionOrder:` with `[]`.
- Sequences 3196 and 3958: `pinnedPosition` returns null.
- Sequence 3964: tooltip `ArcCore fixture action\nHas access to this site`.

The pre-integration
`results/extensions-validation4/extensions-page-actions/launch-1/record.jsonl`
records:

- Sequence 3116: the same empty pin order; sequence 3252 adds
  `[nokhgadimgeidcniagoklokbibfpogjb]`.
- Sequences 4042 and 4530: `pinnedPosition` returns 0.
- Sequence 4536: tooltip
  `ArcCore fixture action\nAllowed to read & change this site`.

These values agree with the two testing-config feature enables. Disabling the
implicit testing configuration restores both features' existing disabled
defaults, so the model should retain the unpinned fixture and GetTooltip
should select the legacy accessible-name path. This is a source-backed
prediction, not a claim of a completed paired run.

## Argument precedence and integration request

The app-owned policy preserves explicit `--enable-field-trial-config` and
`--disable-field-trial-config`. Chromium also treats
`--enable-benchmarking=enable-field-trial-config` as an explicit enable; that
enable wins even when the app adds its default disable. Caller
`--enable-features`/`--disable-features`, manually forced trials, and other
normal native variation mechanisms are not replaced by per-feature startup
overrides.

By contrast, the withdrawn two-feature proposal would append their names to
`--disable-features`, which wins even over explicit `--enable-features`
(`base/feature_list.cc:452-456`). That stronger policy is unsupported by the
default-state golden and unnecessary once the existing app integration lands.

Request to lead/app owner: integrate the existing app-owned startup policy,
then rerun paired extensions-page-actions and launch-slice. Check the real pin
order, both pinnedPosition reads, tooltip text after navigation, and callback
shape/order; do not normalize the differences in the recorder.

## Work performed

Only this owned note was retained. The temporary individual-feature patch
passed `git apply --check --whitespace=error` and applied byte-for-byte to a
temporary copied source before withdrawal. No real TU syntax check was run
after the stop request, because that proposal was superseded. Temporary copy
metadata remains under
`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-feature-state-hvs0yx98`.
Actual `arc/api/arc_browser_application.mm` stayed unchanged with SHA256
`085aadd7b63c192737124bee20a6e2fb09bf86de6cec1debe2acad74c0d14e19`.
No build, app, profile, runtime helper, or shared-lock action was performed.

Fresh read-only review by `/root/extension_scout/feature_note_review` found no
material error. It independently checked the named native feature objects,
generated Mac trials, current defaults, argument precedence, recorded sequence
values, and unchanged startup-source hash. No post-integration runtime claim
is made.
