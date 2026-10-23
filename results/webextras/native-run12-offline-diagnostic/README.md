# Native12 independent offline diagnostic

This is conditional comparator and call-shape evidence only. Native12 remains **complete=false, parity=false**. No engine/app runtime, production/shared source edit, raw recording repair, additional normalization, filtering, or sequence/receiver renumbering occurred.

Frozen inputs: 78 files, including all Native12 bytes, immutable frontend helper `1a2c226c`, freezer `f60537a3`, sealed staged comparator `24437bed`, its dependencies, and current source provenance. Every original and frozen copy was checked before and after execution. Current source provenance is separately identified from the recorder/probe binaries frozen by Native12.

| Original span | Mode | Raw rows | Owned callbacks | Ambient callbacks | Nested app calls | Other app calls retained |
|---|---:|---:|---:|---:|---:|---:|
| inspect-on | 1 | 50 | 1 | 2 | 19 | 1 |
| inspect-off | 0 | 42 | 1 | 1 | 19 | 0 |
| dock-change | 4 | 156 | 1 | 0 | 41 | 2 |
| dock-restore | 3 | 150 | 1 | 0 | 41 | 1 |

All four saved span files exactly equal their original full-record byte slices. Modes are SDK inspect ON/OFF 1/0 and dock right→bottom→right 4/3. Ancestor-nested app calls are 19/19/41/41; all additional app and internal engine calls are retained. Ambient CrashKeys callbacks remain in the actual comparison inputs and callback sequence counts 3/2/1/1.

The actual frozen legacy four-file comparator reports zero self-differences on all four originals, but immutable helper1a2c correctly reports `verified=false`, `pending-callback-boundary-comparison-support`. Zero self-differences do not establish capability.

A private TEMP source was constructed from the exact sealed full-six-patch normalization chain →87be→f2 comparator24437 and unchanged metadata/helpers, with actual freezerf605 as its source. The freezer itself automatically selected and copied exactly seven files; no dependency was added to a frozen snapshot. Fixture capability and all four frontend canaries pass. In this TEMP branch, all four original self-comparisons report zero differences, `verified=true`, and retain every raw owned and ambient callback.

Twelve actual-data negative controls pass: four nested getter returns, four nested leaf call counts, two nested engine setter arguments in dock spans, and two ambient callback string payloads in inspect spans. Each gives exactly one intended mismatch and unchanged full callback sequence counts/order. No nested app setter exists in these recordings. Setter arguments are only diagnosed with the explicit opt-in option: the two absent-option/default setter controls remain zero-difference, as expected.

The ours inspect-on original byte slice contains six metadata-known ambient callbacks and zero owned DevTools callbacks. The typed gate rejects it exactly: `no owned DevTools callback evidence: total=6, ambient=6 (all raw callbacks retained)`. The journal is expired, quiet interval incomplete, and frontend `canDock=false`; it cannot replace the four missing ours callback spans.

Exact first two stdout lines for every original self and negative case, both opt-in and default/absent-option, are in [first-two-lines.txt](first-two-lines.txt). Full helper results, default CLI argv/stdout/stderr, JSON reports, and derivative provenance are in [diagnostic-summary.json](diagnostic-summary.json) and `comparisons/`. Original bytes are in `frozen-inputs/`; mutated controls are only in `negative-recordings/`.

The archived TEMP source/snapshot are evidence bytes. Their manifests retain the original private TEMP paths and filesystem identities; they are not relocated executable snapshots. A fresh execution must freeze a new TEMP source with the actual freezer.

No request to change shared sources arises from this diagnostic. The pending legacy capability and missing ours callback coverage remain lead-owned acceptance blockers. No new architecture/spec contradiction was found.
