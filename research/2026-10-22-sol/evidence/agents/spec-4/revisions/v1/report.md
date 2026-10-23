# spec-4 corrected research specification, revision v1

All **nine original review findings are corrected**, using original finding **data-row numbers 1–9** without sorting, merging or dropping rows. [corrections.tsv](corrections.tsv) records exactly one evidence-backed disposition for each finding. Changes affect **eight of 38 class specifications and 12 method annotations**. All **491 declared identities/types/IMPs**, their original row order, category and method kind are preserved; they resolve to **418 unique IMPs**. This is a static research specification, with remaining contract gaps explicitly retained.

The image is **ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48**, SHA-256 `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd`. Every engine address in the revised class files, correction ledger and revision evidence is an **unslid VA in that image**. The inherited old app observation belongs separately to **Arc 1.168.0 arm64** and is not new runtime evidence. Chromium source pin is `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`; CEF pin is `426920c33ef22551e0b8f48cf343d623ac25642d`.

## Corrections and evidence

| Finding data row | Class | Correction |
|---:|---|---|
| 1 | ArcBrowserContext | Removed nil stored-bridge-clear claim. Getter-returned temporary cleanup is visible; stored removal/unregister and callback suppression are unestablished. |
| 2 | ArcBrowserContextHistory | Removed nil stored-bridge-clear claim and the question premised on clearing. Nil detachment and native ownership remain unknown. |
| 3 | ArcSearchEngine | Replaced preference-key description with optional-handle access and two-native-string hash combination. Distinguished content-derived hash from native-pointer equality. |
| 4 | ArcWebContentsBoostIDE | Corrected event/success/error paths to string-valued payloads; removed native-parser claims/questions. Receiving JavaScript remains untraced. |
| 5 | ArcBrowserContextPreferences | Corrected single-handle assignment to vector assignment with self-assignment/capacity/per-element ownership paths. |
| 6 | ArcBrowserApplicationConfiguration | Flagged malformed recovered stack arguments/prototype and reconciled counts. Fresh instructions corroborate final byte/pointer stores; recovered C remains unreliable as a callable prototype. |
| 7 | ArcBrowserContext | Fixed lifecycle IMP typos to **0x43f1968 / 0x43f21a8** in this ArcCore image. Existing correct identities/matrices retained. |
| 8 | ArcMenuItemModel | Distinguished native delegate-slot clearing from wrapper shared-bridge retention. No wrapper clear or pending-callback suppression is inferred. |
| 9 | ArcSessionID | Corrected pinned `session_types.h` citation to line **185**, `SessionID window_id`; line 203 declares another field. |

[Independent analysis](revision-evidence/analysis.md) explains each correction and its limits. Original bodies were reread with `/home/exedev/work/read-function.py`: **23 unique bounded bodies**, including deeper string/hash/response helpers and nearby delegate setters. New transcripts preserve the index path, offset, length, name and failure status. All 23 transcript bodies pass exact indexed-byte comparison. These reads are separate from the preserved original 505 saved bodies.

Fresh segment-aware disassembly retained **967 instruction words in seven bounded windows**. The context nil branch **0x43ed9e8 → 0x43eda98** and history nil branch **0x4043a08 → 0x4043a94** bypass their nonnil weak stores and enter shared-handle cleanup. The menu nil path **0x444014c → 0x432bf9c** supplies zero arguments; the helper visibly clears native slots at **0x432c070**. These are local control-flow/storage observations, with indirect cleanup semantics still bounded. Three words in the native menu helper remain undecoded and are retained explicitly.

The configuration window resolves the malformed final argument expressions at the storage level: entry-SP+0/+1 byte loads feed self+9/+10, while the entry-SP+8 pointer feeds the object-store call for self+0x30. The declaration encoding remains unchanged and the prototype remains flagged. This does not establish startup consumer behavior or a callable replacement ABI.

Relevant cross-report binary evidence was independently checked against the copied image: **436 instruction-byte/file-offset comparisons** for context and downloads delegate paths. Downloads **0x4042030 → 0x40420bc** bypasses the weak store at **0x4042068**, corroborating the review's cleanup distinction. Four pinned source files were freshly read and hashed; Value string-construction anchors support the BoostIDE correction and the session structure resolves the citation. Exact source excerpts and provenance are in [revision-evidence/](revision-evidence/).

## Nearby analogous descriptions

Beyond the precise hash-matrix finding, the adjacent `ArcSearchEngine -isEqual:` matrix now explicitly states native-pointer comparison. All three outgoing BoostIDE methods and adjacent lifecycle/questions now consistently describe string payloads and receiving-JavaScript uncertainty. Context/history/menu lifecycle paragraphs were reconciled with their corrected matrices. These additional consistency edits are included among the 12 method annotations and eight class files, with before/after method records in [method-annotation-changes.json](revision-evidence/method-annotation-changes.json).

Nearby side-panel **0x44011d8** and BoostIDE **0x4414268** nil paths explicitly zero wrapper pointer/control slots in the original bodies. Their narrower wrapper-clear descriptions were retained; no cancellation guarantee was added. Vector-bearing method descriptions were scanned; the analogous context bookmark-vector setter already describes self-assignment, capacity and element cleanup accurately, so no extra edit was justified.

## Coverage and validation

| Coverage/status | Count |
|---|---:|
| Assigned/present class files | 38 / 38 |
| Declared rows / unique method identities | 491 / 491 |
| Unique IMPs | 418 |
| Exported body available / failed / missing / truncated | 491 / 0 / 0 / 0 |
| Ordinary available rows | 482 |
| Available, malformed recovered prototype | 4 |
| Available, incorrect pseudocode with raw verification | 4 |
| Available, suspect semantics | 1 |
| Local field / other local body observations | 115 / 169 |
| Partial bridge review / explicitly unresolved | 206 / 1 |
| Rows retaining unresolved questions | 491 |
| Full dynamic semantic verification | 0 |

The additional malformed configuration recovery moves one row from ordinary available to malformed prototype. Semantic-status counts are unchanged: partial observations still have unresolved contract questions. These counts do not count calls or certify implementations. [coverage.json](coverage.json), [methods.tsv](methods.tsv) and all revised matrices agree.

Validation **PASS** is recorded in [validation.json](revision-evidence/validation.json). The revision-specific validator checks original row order and all six identity/type/IMP fields, binary class/metaclass/category method lists and record anchors, 38 class/ivar layouts, exact method-matrix multiplicity/behavior/status agreement, relative class links, coverage counts, correction-row cardinality and original review association. It rechecks **505 copied bodies** against read-only indexed corpus ranges, all new transcript bodies, the new instruction bytes and four source hashes. It confirms **548 snapshot files** under `bodies/`, `evidence/`, `sources/` and the copied script sources remain byte-for-byte equal to originals, plus the archived original report. The seeded copy omitted the original two Python cache files; those caches are not input evidence or regenerated tools. All 40 original reviewed spec-file hashes still match the completed independent review snapshot.

Run from this revision directory:

```bash
PYTHONDONTWRITEBYTECODE=1 /usr/bin/time -v python revision-tools/validate_revision.py > revision-evidence/validation-run.log 2> revision-evidence/validation-resource.log
```

The successful validation took **0.33 seconds**, one CPU, **27,644 KiB peak RSS**. Binary decoding uses the preinstalled LLVM C API with bounded reads. No heavy corpus scan, build, installation, agents, network/integration access, other VM access, app/profile changes, external sends, commits or pushes were performed. All new or modified files are beneath this revision directory.

The copied `evidence/`, `sources/`, `scripts/` and [original-report.md](original-report.md) are preserved historical artifacts. Some retain superseded prose/status summaries; **this report, corrections.tsv, methods.tsv, coverage.json and the revised class files are the corrected specification**. Original reproduction/rendering scripts were not run or edited; running them would regenerate the earlier specification. New correction/evidence/validation utilities live separately under [revision-tools/](revision-tools/).

## Scope and remaining contract gaps

The three required review gates, completed independent class review and relevant original bodies/instructions/pinned source were read. Review prose was treated as a lead and checked against actual evidence. This correction does not expand selected static observations into complete callee coverage, thread delivery, stable C++ ABI or runtime compatibility. The historical handoff's 5,846-row depth-two index does not prove complete reachability or proprietary identity. Broader metadata/body-subset counts retain their distinct definitions; no Ghidra enclosing spans are summed and no unmatched code is called proprietary. The 834 figure remains declarations on imported classes, not proven calls. Missing mirrored sources do not demonstrate API absence.

Remaining gaps include context/history nil detachment and native registration lifetime; retained menu bridge reuse; queued/in-flight callbacks and profile shutdown; exact string-field/hash-symbol identities and mutation stability; BoostIDE receiving JavaScript and lifecycle gates; vector synchronization/observer registration; configuration startup consumers; existing training-result cleanup/return ambiguity; missing exact-entry context delegate adapters; and side-panel/PiP view ownership and required thread. No main-thread delivery, teardown suppression, migration safety or replacement compatibility is established. The acceptance review's narrow runtime smoke observations are not promoted into replacement-engine evidence by this static correction.
