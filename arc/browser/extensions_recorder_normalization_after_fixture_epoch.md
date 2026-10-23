# SessionID normalization after fixture admission and epoch normalization

Unapplied proposal. This is an **alternative application route** to
`extensions_recorder_normalization.patch` (the preserved original e7ef669-base
proposal), not an additional layer to apply on top of it. No actual shared
comparator, allowlist, product source, runtime helper, app, build, profile, lock,
or raw recording was changed. Acceptance is **not diff-clean** merely because
this proposal's tests pass; the lead must review/apply it and assess real diffs.

## Exact prerequisite order

Use this route if the fixture and epoch proposals are selected:

1. Begin with actual `tests/recorder/diff.py` SHA256
   `f8df579e6ea23b4eb531f357fdfe0460fa57b563daf44f9d6858c73abf84b184`.
2. Apply `arc/browser/webextras_fixture_comparator.patch`, SHA256
   `97189c005c918fa91a7473922b04af31aff30d317cca575cf5479e6962d87060`.
   Comparator becomes
   `d565702f72ba29626ce0f2d1fbc76387be071e01992aef4941c9eeb17c119fda`.
3. Apply `/tmp/omnibox-epoch-after-fixture-proposal/last-visit-epoch.after-fixture.patch`,
   SHA256 `4e99c4b5889969533683c535108cc0b544b2da3cde82d8e89f20749f9d214c26`.
   Comparator becomes
   `ff2de37382ced64753cae12ee9e20c58d68d275b434cefb549747b338905def5`.
4. Apply **only** this incremental proposal,
   `arc/browser/extensions_recorder_normalization_after_fixture_epoch.patch`,
   SHA256 `5f3f373c8d4fcb805278f7f015f242749bdb065d0062503cc2bb4161fd6e993e`.
   Final comparator SHA256:
   `63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08`.

The actual allowlist base SHA256 is
`17f7846129013795c3eefdbf2a7b51b47f4857a3046d756c5d0abfbbf6716ecd`;
steps 2 and 3 do not change it. This proposal appends one SessionID row.
Never apply the original SessionID proposal as well as step 4. Never apply the
older original-base epoch companion on top of the epoch-after-fixture patch.
The exact frozen inputs and privately applied stages are preserved under
`logs/wp/extensions-normalization-after-fixture-epoch/v1/`.

## Policy and preserved behavior

The only numeric normalization is the original proposal's per-transcript,
first-seen bijection shared by these two exact fields:

- Retained return of `->engine -[ArcSessionID identifier]`.
- Argument index 1 of retained
  `engine->app -[ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:]`
  calls.

Aliases accept only exact JSON integers 1 through 2147483647. Repeated and
distinct identities remain observable across both fields. Zero, minus one,
other invalid values, booleans, floats, strings, arrays and dictionaries remain
tagged literals, bypassing existing object/string masks so malformed values
cannot impersonate aliases. Callback returns, other argument slots, other
selectors/classes/directions and unrelated numbers retain existing comparison
behavior. This does not normalize extension group IDs or general tab/window IDs.

Alias allocation occurs only in the final selected-event normalization loop,
after the complete caller/fixture admission and validation pass. Engine getters,
unselected fixture images and unknown-image getters cannot allocate aliases.
The epoch raw-evidence prepass remains earlier than admission, exactly as in the
prerequisite epoch patch. Admission, fixture validation/count guards, epoch
helpers, generic object/string normalization, boundary classification and the
entire report/CLI tail are byte-identical to the fixture+epoch base. Generic
factory classification remains metadata-driven; this proposal adds no factory
exception.

## Validation and evidence

All checks used private copies. Final logs in
`logs/wp/extensions-normalization-after-fixture-epoch/v1/` show:

- **37 existing tests pass:** 15 epoch regressions (including the immutable
  canonical golden, eight changed epoch fields), 16 fixture regressions, and
  six epoch/fixture integration regressions.
- **65 original SessionID cases pass:** every resulting JSON report is identical
  to the corresponding preserved e7ef669 proposal report; expected equality,
  negative cases, caller attribution, classification, counts and callback
  sequence are unchanged. Thirteen cases compare equal and 52 retain differences.
- **12 new integration tests pass**, including multiple adversarial subcases:
  excluded getter allocation asymmetry; exact selected images and retained
  counts; callback-first shared mapping; alias collapse/split; integer boundary,
  sentinels and malformed literals; unrelated numeric fields/callback returns;
  excluded constructor epoch evidence; conflicting/missing epoch evidence;
  default caller policy and one-side missing-attribution fallback; fixture CLI
  rejection guards; unchanged callback ordering and fixture metadata.
- Fresh read-only review found no defects, independently checked all 65 CLI
  stdout results and the preserved prior evidence, and verified the integration
  boundaries and constant-policy harnesses (`fresh-review.md`).
- Strict `git apply --check --whitespace=error-all` on the exact combined base
  passes; actual private application matches the final SHA256 above. Both
  applied copies compile as Python. Source preservation assertions pass.
- All 12 frozen repository inputs and all 470 files in the previous proposal
  evidence directory hash-check unchanged. Raw synthetic inputs are checked
  unchanged after each comparison; archived recordings are never rewritten.

The old fixture suite asserts archived default stdout/JSON parity, including
recordings with both SessionID and epoch differences. Its copied test source and
assertions are unchanged. Its **private harness baseline** therefore has both
normalizations already present, and its private harness patch adds fixture
admission to produce the exact final comparator. This holds normalization policy
constant while testing fixture behavior. Likewise, the copied epoch suite's
private baseline has SessionID normalization already present, so its canonical
assertion still isolates the eight epoch fields. Only module roots change in the
epoch tests; their test methods/assertions are unchanged. These harness baselines
are validation controls, not alternative product patches. Public patch
application is verified separately in the exact four-step order above.

The initial fixture run held SessionID policy constant but omitted epoch policy
from its baseline, so the archived parity assertion failed. That output remains
in `fixture-regression.log`; the corrected control passes in
`fixture-regression-final.log`. The initial epoch run without a private canonical
copy skipped that one test; `epoch-regression-final.log` includes all 15 tests
without skips. Prior evidence is preserved, not overwritten.

Reproduce the final suites without writing shared files:

```sh
python3 -B -W ignore::ResourceWarning logs/wp/extensions-normalization-after-fixture-epoch/v1/epoch_test.py
TMPDIR=/tmp python3 -B -W ignore::ResourceWarning logs/wp/extensions-normalization-after-fixture-epoch/v1/fixture-regression-root/arc/browser/webextras_fixture_comparator_test.py
python3 -B -W ignore::ResourceWarning logs/wp/extensions-normalization-after-fixture-epoch/v1/epoch_fixture_integration_test.py
python3 -B logs/wp/extensions-normalization-after-fixture-epoch/v1/session_regression65.py
python3 -B -W ignore::ResourceWarning logs/wp/extensions-normalization-after-fixture-epoch/v1/session_fixture_epoch_test.py
python3 -B logs/wp/extensions-normalization-after-fixture-epoch/v1/verify.py
```

`prepare.py` reconstructs prerequisite stages and the incremental patch;
`complete_harness.py` constructs the final constant-policy test controls.
`provenance.json`, `harness-provenance.json`, `regression65.json` and
`verification.json` record exact results. The original proposal and its evidence
remain untouched.

## Remaining scope and lead request

Review and apply only the selected route, then compare the relevant real
recordings. No new app run or scenario diff was performed for this proposal.
Existing full callback-sequence differences and foreign-package/core differences
remain visible. Numeric aliases do not establish correct native routing or
callback timing. There are no newly established architecture/spec contradictions.
