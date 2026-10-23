# Core9 audit: runtime is prior a647 sessions source

**The run records compiled sessions source a647, not current e973.** Known session-related differences persist; no new supported sessions-owned bug or canonical session acceptance is established. This run has zero discard calls and supplies **no runtime proof of the selected-adapter lifetime fix**.

Run: `results/core-runtime/20261023T155832Z-progress-publication/`.

## Source, build/package and cleanup pins

Both all-Arc seals contain122 entries and agree; changed list is[]. The three12-file core seals and two25-file runtime-input seals agree. Sessions source is `a6475f82e58de755fbf87578cc0dcb4144c0aeb685ce0073e707af442a7e37f0`, GNI `6f983408cd5c903507a5ae093cc6bee7700a0aa69e83d0c017581bcb3a8a9830`. Current source, read separately after collection, is **e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d**. Do not relabel this older compiled run as e973 evidence.

Compared with run8, the seal includes changed core progress source `arc_web_contents.mm`47534304…, app crash/reporter source1eccbadc…, multiple renderer/publication sources, and changed harness `tests/launch-slice/run.ts`023dbb69…. Full per-file cochange hashes are in this audit manifest. Hence whole-scenario callback/count changes cannot be attributed solely to progress publication. The all-Arc seal is not a complete native Chromium/toolchain/dependency freeze.

`build.log:3` records BUILD DONE at15:59:20; package log records successful app packaging. Built framework hash: **00d39043e093306321cb328b1b8ad33245dbe8f464cadc44c89c98879e8b510f**. Signed packaged and packaged-current hashes both: **3167c5c55956ba049e71b45880599e46b90acfb3c052bdf14f78b56f3324fb1d**. These are distinct stages.

Launch9/9 and tabs11/11 functional steps pass, both harness exits0, both coverage summaries accepted/no errors, and both fatal-signature scan lists empty. Quit receipts show Browser.close and subsequent parent/helper absence. Completion explicitly records **OS normal-exit status not captured**. These limited receipts are not proof of crash-free/normal OS termination, and this audit performs no independent process inspection.

## Exact relevant rows

| Launch | ArcSessionID identifier calls golden/ours | First value golden/ours | All callbacks golden/ours |
|---|---:|---:|---:|
|launch-slice1|8/5|1064772386/1451973155|106/128|
|launch-slice2|3/3|1064772440/1451973210|47/61|
|tabs-navigation1|14/14|805422707/567529736|162/182|

All engine→app callback totals are315/371 combined. ArcSessionID is core-owned but relevant to sessions; per-run numeric differences alone do not prove broken IDs. Only launch1 retains the identifier8/5 count mismatch and `ArcWebContents sessionId`6/3 mismatch; the other sessionId getter counts are3/3 and12/12.

Launch1's ours-only global callback is main-thread at **record.jsonl:6041**, returningnil6086, with UUIDAAD639A5-724A-4207-929C-08FFBC706CFD, context#33, **type0/restoration IDnil**. It precedes recorded tryToTerminate6149/return2 at6150. Golden has no global callback; neither side has one in launch2 or tabs. This is the known extra close row, not evidence of a missing global quit guard. Imminent/didClose remain once each per side in launch1 and tabs (none in launch2), main/nil-return; ours starts5565/5607 and8851/8893.

All six transcripts have hook row1 (golden325 classes/ours340) and app-origin configuration row2 with policy0. Golden engine-internal getter25/26 returns0; ours has no raw policy getter. The saved default caller gate excludes that internal getter, so it is not a newly missing boundary method. Raw caller labels are app/engine/Combine, without a direct driver label. The raw census has **zero** eligibility, public restore, restored callback, foreign-tab restore, session DTO, discard or finalize-discard calls on both sides. Cookie/localStorage/IndexedDB relaunch readback is not closed-tab/window restore coverage.

## Exact first two diff lines

[diff-first-two-lines.txt](diff-first-two-lines.txt) copies lines1–2 verbatim from all three raw diff text files; [selected-rows.json](selected-rows.json) retains the same headers, raw line anchors and matching JSON rows.

- launch1: methods differing106; missing5, extra3, returns13, callback args4, counts81. Callback106/128, first divergence1.
- launch2: methods differing57; missing6, extra0, returns12, callback args4, counts35. Callback47/61, first divergence1.
- tabs: methods differing56; missing5, extra1, returns15, callback args5, counts30. Callback162/182, first divergence1.

The archived original goldens are not new paired original executions. Functional coverage acceptance and these global diff totals do not resolve factory/source hooks, full restore/restart, logical-root retention, absent-registration lifetime, setter wiring, discard/selected-adapter behavior, cancellation or general callback parity. Fresh independent read-only review agrees on the meaningful statistics and limits.

All writes are new audit artifacts here. No production/shared/test edit, app/probe/build/lock/board action or credential access was performed.
