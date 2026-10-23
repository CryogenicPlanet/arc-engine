# Held explicit boundary argument comparison

UNAPPLIED proposal for `tests/recorder/diff.py`, generated **after** the frozen fixture proposal `97189c005c918fa91a7473922b04af31aff30d317cca575cf5479e6962d87060`. Production/shared files, the fixture proposal and all existing normalization proposals remain unchanged. Fresh review and lead integration are required before adoption; no runtime acceptance is claimed.

```
python3 tests/recorder/diff.py GOLDEN OURS --compare-boundary-arguments --json REPORT
python3 tests/recorder/diff.py GOLDEN OURS --compare-boundary-arguments \
  --fixture-image WebextrasNativeProbe.dylib --json REPORT
```

The opt-in flag independently reports the first normalized argument mismatch and first return mismatch per compared method. This catches selected app calls nested inside callbacks and observable engine setters. It uses the **same** existing `load()` admission, normalization, object numbering and metadata boundary set. A new argument check is eligible only for metadata-known interface methods or existing engine-to-app callbacks. Engine internal getters and calls from unselected images retain their existing exclusion. No new normalization, caller-image allowlist or pointer exception is introduced.

The existing whole-method native-constructor classification is reused for argument skipping: if **every present call on both sides** has normalized arguments containing the literal `"<ptr>"`, that method's argument comparison is skipped. This is the existing broad textual rule, not a new semantic constructor detector; it can suppress arguments of any method satisfying that exact rule. Other pointer spellings are not exempt. Return comparison and missing/extra/internal classification retain their existing behavior.

When enabled, each transcript must have at least one call, and **every call and return** must have a valid nonempty caller string under the existing fixture validator (no surrounding whitespace or control characters). Malformed JSON/non-object rows fail as in fixture validation. Caller-aware selection is mandatory even without a fixture image. Callback-only spans are supported. A valid unselected image name still passes field validation and is excluded by the old event filter. Metadata `caller_attribution:true` therefore validates fields/selection, not the semantic truth of every call origin. Whole-call nesting, nonempty engine-callback evidence, actual forwarding, receiver identity and public-work exclusion remain the driver's separate typed/ancestry gates.

Opt-in capability metadata is exactly:

```json
{"boundary_arguments_comparison":{"mode":"selected-boundary-methods","caller_attribution":true}}
```

The existing `arg_mismatch` key is reused. The legacy first-line label `callback args` is retained, but with this flag its count includes the selected boundary arguments described above. Without this flag there is no new report key, no new caller validation, and existing callback-only argument/return precedence and output remain unchanged. Both archived regressions reproduce full default stdout and JSON bytes. Strict argparse rejects abbreviated, unknown and value-bearing variants of the new boolean option.

A driver must run a deliberately differing setter-argument canary and require exactly one `arg_mismatch`, the exact capability metadata and any fixture admission counts. The actual shared comparator silently ignores the unknown flag and misses the setter argument; the fixture-only proposed CLI rejects it. Neither is supported. Do not clear a pending guard on exit zero or shape-only validation.

Validation: `python3 -B arc/browser/webextras_boundary_arguments_test.py` passes22 actual TEMP module/CLI tests. Coverage includes nested callback/app getter return, app and engine setter arguments, callback arguments, independent first return/argument evidence, counts, missing/mixed caller rejection, callback-only spans, helpers/constructors, legacy normalization, unsupported canaries and archived default byte equivalence. Shared input hashes are checked unchanged at suite exit. Evidence is under `logs/wp/webextras-boundary-arguments/`.

Strict composition: fixture→this proposal applies cleanly. The full existing fixture→epoch→SessionID→receivers→Google-query→profile-download chain reconstructs cleanly on its own, but this proposal then rejects overlapping comparator hunks. Inserting this proposal immediately after fixture instead causes epoch's overlapping `load()` hunk to reject. This is a **rebase requirement**, not successful full-chain composition; no prerequisite patch was modified, dropped or duplicated. Preserve the raw normalization prepasses before caller filtering during any lead-owned integration. Exact chain hashes/rejections are in `composition.json`.
