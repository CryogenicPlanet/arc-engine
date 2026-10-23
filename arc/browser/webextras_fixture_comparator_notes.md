# Proposed explicit fixture comparison

This is an **unapplied proposal** for `tests/recorder/diff.py`. Only the owned patch, this note and the standalone proposal test are changed. Production probe pending guards remain in force until the shared owner adopts and verifies support. No app run is part of this proposal.

Apply from the repository root with `git apply arc/browser/webextras_fixture_comparator.patch`. The patch was generated against the currently shared comparator; the test applies it only inside a temporary repository with copies of the real boundary metadata and callback table.

## Proposed CLI contract

```sh
python3 tests/recorder/diff.py ORIGINAL OURS --json REPORT \
  --fixture-image WebextrasNativeProbe.dylib
python3 tests/recorder/diff.py ORIGINAL OURS --json REPORT \
  --fixture-image probe
```

`--fixture-image` is repeatable and admits only exact, case-sensitive recorder image basenames. The proposed safe spelling is an ASCII letter, digit or underscore followed by letters, digits, underscores, dots, pluses or hyphens. Empty names, paths, globs, `app` and `engine` are rejected. Repeated names are deduplicated. Unknown options and abbreviated options fail with exit 2. No caller labels are relabelled, and input files remain untouched.

Default event selection and report classification remain unchanged: app calls, all engine-to-app callbacks and engine `set*` calls remain eligible when caller attribution is active. Engine getters and calls from unselected images remain excluded. Public constructor `<ptr>` classification and private helper classification use the existing metadata and rules. This proposal does not introduce a different metadata filter.

Fixture mode requires a nonempty caller string without surrounding whitespace or control characters on **every** call and return in both inputs; `hooked` rows need no caller. It rejects mixed v1/v2 inputs. For admitted `->engine` fixture events only, it requires complete properly nested call/return pairs per thread, with identical caller, thread, depth, direction, kind, class, selector and receiver. Both inputs must have at least one such complete pair. Unrelated ambient calls and shutdown tails keep their existing treatment. A misspelled image with zero admitted calls cannot yield fixture acceptance.

The existing first two human output lines remain byte-for-byte unchanged for equivalent compared events. Fixture mode adds one separate JSON field:

```json
{
  "fixture_comparison": {
    "mode": "exact-image",
    "admitted_images": ["WebextrasNativeProbe.dylib"],
    "retained_fixture_events": {
      "golden": {"WebextrasNativeProbe.dylib": {"call": 13, "ret": 13}},
      "ours": {"WebextrasNativeProbe.dylib": {"call": 13, "ret": 13}}
    }
  }
}
```

Those numbers illustrate the native probe's expected public invocation total; the comparator reports actual event counts. The standalone values driver expects 72 calls and 72 returns from `probe` per input. The parent owns the driver changes that must check exact expected coverage after adoption. A repeatable selection can report zero for one image if another admitted image supplies evidence; drivers must check their expected image counts individually.

## Limits and adoption checks

The comparator still compares input arguments only for engine-to-app callbacks. It does not establish fixture input-argument equality, receiver identity, whole-transcript nesting, cross-method order or exact callback timing by itself. Its existing object ID and volatile-value normalization and mismatch exit-code behavior are unchanged. The native driver's raw traceShape, exact public invocation counts, nesting/forwarding/order validation and full completion payload/outcome checks remain mandatory. Values JSON/metadata parity and exact 72-pair coverage validation likewise remain mandatory.

A successful CLI exit is not sufficient evidence: the old comparator silently accepted unknown options. Before clearing the pending guard, verify the adopted JSON fixture metadata, the exact expected admitted-image call/return counts, empty mismatch sections and matching callback sequence. Preserve raw caller labels and do not rewrite them to `app`.

Run the proposal tests without changing production:

```sh
python3 -B arc/browser/webextras_fixture_comparator_test.py
```

Tests cover differing fixture returns, default exclusion, literal image isolation/repetition, retained app calls/callbacks/engine setters, excluded engine getters, invalid options/images/callers, mixed callers, admitted pair mismatches/incompleteness, zero evidence, unrelated ambient tails, unchanged constructor/helper classification and unchanged first two lines. Archived values-run1 and media-pip-run2 comparisons must reproduce the original comparator's full stdout and JSON under default mode.

## Concurrent proposal compatibility

Omnibox owns the separate, unapplied `/tmp/omnibox-epoch-normalization-proposal/last-visit-epoch.diff.patch`. Its raw-key prepass is intentionally before caller filtering. This proposal changes event admission only and adds no epoch or argument normalization. The lead must rebase the overlapping `load()` hunk while keeping that raw prepass before event selection; neither proposal should silently replace the other. Omnibox offered an incremental patch after this proposal is frozen.
