# Independent synthesis audit

The selected revised class inventory reconciles: **146 classes, 1,964 unique declaration identities, 1,566 distinct IMPs**. The independent audit found no selected declaration/type/IMP mismatch. All **2,476 manifest-selected file hashes** and all **199 shard hashes** verify. These checks establish provenance and metadata consistency, not complete semantics or runtime compatibility.

The comprehensive deliverable is [engine-feasibility.md](engine-feasibility.md). It recommends investigating an in-tree Chromium `arc/` bridge first, based on observed Chrome service and native compositor dependencies, while retaining explicit CEF route gates. There is no demonstrated replacement-ready engine and no supported whole-rewrite effort/code-volume estimate.

## What was independently checked

The review read the ABI, CEF, Arc-layer, matcher, quality and protocol reports together with their actual TSV/JSON and validation tools; selected v1 class docs, correction ledgers and all fresh class verdicts/dispositions control the class summaries. Superseded original class-review findings are retained as historical evidence. Context reviews and the saved acceptance scripts/results were assessed separately. No other role's output-writing tool was run.

| Check | Independent result |
|---|---|
| Selected revision | `/home/exedev/work/CLASS-REVISIONS-READY`, SHA-256 `58b7e65868a774783345bc65391f16d6d983e61f648a2e604a5ec9d2d69afc8e`; 2,476 file hashes verified |
| All class/method matrices | 146 docs; 1,964 exact class/category/kind/selector keys; types/IMPs match revised TSV and independently traversed current binary method lists; historical identity/order preserved |
| Body availability | 1,565 successful indexed IMPs; sole no-exact-header method `0x4410bb4` is a binary-verified B veneer to `0x47390dc` |
| Older/current metadata | All 1,964 retained method keys/raw encodings and 146 retained instance layouts/351 ivars agree; 12 older/current raw entry checks reproduce; properties/all conformance/metaclass layouts/C++ ABI/behavior excluded |
| Corpus | 199 unique shards, 1,114,106,346 bytes, 567,959 headers, 79 failures, 567,880 successful candidates; streamed hashes and header/failure inventory |
| App call evidence | 4,353 retained rows: 4,336 binary+pseudocode selector, six binary+pseudocode C-import, 11 pseudocode-only; 5,353 distinct instruction bytes/branch targets independently checked |
| Bounded ABI/protocol evidence | 2,572 retained instruction records checked against segment-mapped bytes, EH frame data checked, 195 combined selector-send/branch checks; this is byte/target validation, not wholesale semantic certification |
| Fresh exact body samples | 19 original indexed bodies, all within 32 KiB, matched against the mandated reader; raw locations/hashes retained |
| Source provenance | 15 anchored source records; manifests rehashed for 184 CEF, 17 successful Chromium and 179 published-snapshot source files; one explicit failed fetch retained separately |
| Historical class findings | All 32 mapped to fresh dispositions: eight resolved, 24 resolved_with_limits; four fresh findings tables header-only |
| Remaining spec limitations | 1,670 rows with open questions; 97 explicit unresolved semantic labels; zero full runtime contracts certified |
| Protocol surface | 26 specs / 200 identities; five value getters distinguished from 195 candidate callback/app-service identities; 194 identities have static-send support |
| Matcher | No accepted reliable unknown-function match; false-positive named control remains rejected |
| Acceptance | Saved scripts have no expected-value assertions; both data-channel probes timeout; four screenshots inspected; no native execution in this review |

The independent parser preserves image identity, category and method kind, translates VAs with Mach-O segments and decodes chained pointer formats 2/6 where encountered. It does not import another agent's analysis parser. The source pin is Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`; CEF is `426920c33ef22551e0b8f48cf343d623ac25642d`. Five image hashes are in [image-pins.tsv](evidence/image-pins.tsv).

## Corrections the parent must preserve before publication

1. Use selected `revisions/v1/classes`, ledgers and fresh reviews together. The [spec index](spec-index.tsv) supplies all paths and hashes. Fresh review acceptance is bounded static research, not an implementable/completed contract.
2. Annotate the stale `ArcSessionID` limit in `spec-4/revision-reviews/v1/class-review.tsv`: “old-engine ambiguity” is closed by later protocol/full-metadata and parent raw-entry evidence, including signed `q` for `identifier`. The selected class doc is not being accused of an incorrect encoding. Sentinels, restore semantics and numeric correspondence to CEF remain unknown.
3. Replace historical blanket termination-retention language: the ObjC wrapper retains temporary block storage; native application storage is conditional on the first initialized request, repeats return 2 without storing the new callable, and the first path can proceed synchronously. No general beyond-return/eventual/exactly-once guarantee follows.
4. Preserve both closure metrics: **5,859 metadata candidate mappings/eight ambiguous names** and **5,847 exact exported-body-resolved/one ambiguous name**. Neither is full reachability or proprietary volume.
5. Preserve exact field limits on the 1,964 encodings/146 layouts comparison. Do not reproduce `ENGINE.md`'s ArcWebContents `_mouseInside` addition as established by these Arc-prefixed tables.
6. Keep 834 as declarations on 63 imported classes, and distinguish selector/stub calls from receiver-confirmed usage. Keep protocol purpose/adopter/invocation evidence separate; five DevTools getter identities are values.
7. Reconcile recovery status: `_CallWithEHFrame` remains an exact exported-header gap but its branch/unwind contract is recovered; pane-host creation is likewise a resolved veneer. Use the combined protocol send table for `webContents:updateDevToolsToolbox:`.
8. Do not publish successful static checks, equal errors or basic saved smoke probes as runtime replacement readiness. Helpers, launch packaging, native C++ ABI, callback lifetime/order, profiles/migrations and substantive service equivalence remain gates.

These are corrections/qualifications in this role's outputs only. Other agents' selected and historical artifacts were not edited. [findings.tsv](findings.tsv) supplies severity, evidence, correction, disposition and parent action. [correction-dispositions.tsv](evidence/correction-dispositions.tsv) preserves all original class-review finding rows.

## Reproduction and measured resources

Run from `/home/exedev/work/agents/review`. Both independent checks completed successfully; these are the exact final analysis commands:

```sh
flock -x 9 /usr/bin/time -v python3 -B synthesis/tools/audit.py > synthesis/evidence/audit-run.log 2> synthesis/evidence/audit-resource.log 9</home/exedev/work/heavy.lock
flock -x 9 /usr/bin/time -v python3 -B synthesis/tools/spot_checks.py > synthesis/evidence/spot-run.log 2> synthesis/evidence/spot-resource.log 9</home/exedev/work/heavy.lock
python3 -B synthesis/tools/build_appendices.py
python3 -B synthesis/tools/validate_outputs.py
```

The metadata/corpus audit returned **PASS**, 10.20 seconds wall time and **59,196 KiB maximum RSS**; the spot check returned **PASS**, 2.15 seconds and **31,248 KiB**. Each used approximately one CPU. Prior `*-failed*` logs record reviewer tooling assumptions corrected before these successful runs, not engine evidence failures. Final deliverable validation results and own artifact hashes are in [output-validation.json](evidence/output-validation.json) and [artifacts.sha256](artifacts.sha256).

The analysis streamed large files, opened shared evidence/index/lock read-only, used bounded body/binary reads and wrote only beneath this role directory. No agents, SSH/other machines, native app execution, installs/builds, credentials, integrations/external sends, commits/pushes or input modifications were used.

## Artifact map and limits

- [engine-feasibility.md](engine-feasibility.md): full architecture, CEF comparison, subsystem coverage, source boundaries, acceptance limits and staged next work.
- [spec-index.tsv](spec-index.tsv), [coverage-appendix.md](coverage-appendix.md), [protocol-index.tsv](protocol-index.tsv): all selected class/reviewer/revision paths and all protocol specs.
- [findings.tsv](findings.tsv): synthesis/publication corrections and open feasibility gates.
- [audit.json](evidence/audit.json), [spot-checks.json](evidence/spot-checks.json), [groups.tsv](evidence/groups.tsv): machine-readable independent counts and outcomes.
- [binary-methods.tsv](evidence/binary-methods.tsv), [metadata-keys.tsv](evidence/metadata-keys.tsv), [method-body-availability.tsv](evidence/method-body-availability.tsv): declaration/IMP/body reconciliation.
- [app-branches.tsv](evidence/app-branches.tsv), [instruction-table-checks.tsv](evidence/instruction-table-checks.tsv), [older-entry-checks.tsv](evidence/older-entry-checks.tsv): bounded binary and raw-entry corroboration.
- [corpus-checks.tsv](evidence/corpus-checks.tsv), [failure-taxonomy.tsv](evidence/failure-taxonomy.tsv), [closure-definitions.tsv](evidence/closure-definitions.tsv): corpus integrity and correctly separated closure definitions.
- [sampled-bodies.tsv](evidence/sampled-bodies.tsv), `evidence/body-*.c`, [source-pins.tsv](evidence/source-pins.tsv): exact fresh body extracts and pinned source anchors.
- [selection-hashes.tsv](evidence/selection-hashes.tsv), [correction-dispositions.tsv](evidence/correction-dispositions.tsv): selected revision provenance and all historical finding dispositions.

The review covers all declared rows mechanically, all class/review coverage records, and representative high-impact behavior. It does not independently recover or semantically prove every transitive body. Successful C exports may be malformed; sparse static sends do not prove runtime receiver identity; source correspondence does not certify native C++ prototypes or stable ABI. No comprehensive helper or older-engine-body comparison exists. No full-rewrite time/code-size estimate is supported.

The next specific questions are: recover function membership/callback ownership at profile/startup/filename/IPC/capture adapters; identify task-runner and teardown behavior; establish helper/Aperitif launch contracts; and run an explicitly asserted, image-attributed native baseline before any route prototype or feasibility commitment. Exact VAs and exit criteria are in the comprehensive report.
