# Racy controls-only correction

The shared proposal remains exactly
`22ea77b1dc0895c4872bccc1758e1a750d6d5f3bd99663942699dcab47b00a12`:
helper `d40e1002c625bc86fa5f32b92488f0e2124a9b44342290ad50615abed97a06dc`,
wrapper `a5dda4489fa02d6b6927bd0a1b1938c430fd31491ba932bfecdad2ec604b51af`.
No candidate, production or frozen predecessor bytes change in this correction.

The original local run observed full old26 private-add-N rescue and incorrectly
made that outcome mandatory. Fresh review observed new-file hunks without the
tracked edits. Its boundary control demonstrated that an index rewrite in a
later timestamp second can retain that omission. Thus the old rescue was an
observation from one run, not a guarantee. The original test/notes/freeze and
reviewer's failed-suite/boundary evidence remain preserved with exact hashes.

`webextras_nested_regeneration_racy_controls_test.py` source-loads the pinned
original test and replaces only
`test_old26_no_new_leaf_counterexample_and_add_n_behavior`. An AST comparison
requires every other node to remain identical. All corrected-candidate matrices,
exact-full-byte comparisons, first-private-read mtime checks, precision failure
checks, protected identity checks and the copied62 regression lane are unchanged.
The old26 no-new-leaf successful empty-output counterexample remains mandatory.

Only the old26 add-N branch becomes descriptive. It records the exact observed
and expected patch text, hashes, outer/nested tracked hunk presence, full/partial/
none rescue classification, captured cached/index/working timestamp seconds,
and private index mtime plus wall time before/after add-N. The instrumentation
performs no extra Git operation or delay and changes no timestamp. It neither
requires nor forces an old26 rescue/failure. This old branch is diagnostic and
does not relax acceptance of the corrected candidate.

Run `python3 -B arc/browser/webextras_nested_regeneration_racy_controls_test.py`.
It executes the complete62+7 suite using the same copied proposal/helpers and
disposable repositories. All writes are under the distinct
`logs/wp/webextras-nested-regeneration-racy-controls/` evidence prefix or private
temporary repositories. The executable corrected body and precise test-only
diff are archived there. `controls-preservation.json` proves unchanged originals
and candidate assertions; `freeze.json` pins final source/result bytes.

For a read-only replay, source-load this controls test into a ModuleType, copy
its evidence tree to a fresh temporary proof directory, rebind EVIDENCE there,
then call main(). No published evidence should be overwritten during review.

No production Git/source/tool/index/config/epoch/lock/build/runtime/board action,
helper change, owner authorization or actual adoption is part of this work.
The candidate remains subject to independent review and renewed owner grant.
