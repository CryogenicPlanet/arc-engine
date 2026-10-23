# Core run8: current sessions rows

**Same known session-scope differences as run7; no new supported owned bug.** Launch-slice9/9 and tabs-navigation11/11 functional steps pass after the actual shared shutdown-source change. These scenarios do not exercise session restoration and do not establish canonical sessions acceptance.

## Seals and packaged inputs

Run `results/core-runtime/20261023T145945Z-quit-suppression/` seals120 Arc source inputs identically before/after (`arc-build-inputs.changed.json=[]`). Three core source seals and two runtime-input seals agree. Relevant before/after/current hashes match:

| File | SHA-256 |
|---|---|
|arc/api/arc_sessions_restore.mm|a6475f82e58de755fbf87578cc0dcb4144c0aeb685ce0073e707af442a7e37f0|
|arc/wp/sessions.gni|6f983408cd5c903507a5ae093cc6bee7700a0aa69e83d0c017581bcb3a8a9830|
|arc/api/arc_web_contents.mm|ff957456ece2014edd6a0dda7b1424438b0b2b903fccbf257aae5becb82ceed9|
|arc/api/arc_app_termination.mm|146348e5c7a273286c8ad237bda7b2553dfc8277fb2e0fb5fefd4f6fbed0aefb|

The first two own hashes are unchanged from run7; the two shared shutdown implementation hashes are changed. The source seal covers the enumerated Arc inputs, **not a complete native Chromium/toolchain/dependency freeze**. No causal attribution follows from a changed global diff count alone.

Archived build/package logs report success. Built framework hash is `25d9efd652ceaac6735b960273232af16f6ac07726ec06be411b38db4947740a`; signed packaged and packaged-current records both equal `f0e1836d6f4035335ac9a87b1de1bcfe1ccc0dcc38c19ad7d6c11ef9a56a1d4e`. These are distinct stages. Both harness exits are0; both coverage files are accepted with no errors. Quit receipts record Browser.close followed by browser/helper absence; fatal-signature scan files are empty. Completion explicitly says **OS normal-exit status not captured**. This is not an independent crash-free/normal-exit conclusion from PID absence.

## Relevant default diff and raw boundaries

| Launch | identifier calls golden/ours | First identifier golden/ours | Global close golden/ours |
|---|---:|---:|---:|
|launch-slice1|8/5|1064772386/2108556474|0/1|
|launch-slice2|3/3|1064772440/2108556529|0/0|
|tabs-navigation1|14/14|805422707/1361381310|0/0|

All three JSON diffs retain the numeric ID return row. Only launch1 retains identifier8/5 and `ArcWebContents sessionId`6/3 call-count rows; other sessionId counts are3/3 and12/12. Different per-run native numbers alone do not prove broken ID semantics. The extra global callback is still type0/nil-restoration-ID, now at **launch-slice/launch-1/record.jsonl:6031**, returnnil6076, **before tryToTerminate:6139**, return2 at6140. Run7 positions were6033/6078 then6135/6136. Golden has no global callback and terminates6667/6668. No quit-state root cause or universal global suppression guard follows from this ordering.

Individual imminent/didClose callbacks remain main, once each per side in launch1 and tabs (none in launch2). Ours start at5555/5597 and8683/8725; their nested ID returns show live→fresh historical IDs. These ordinary-close observations do not verify restore eligibility, synchronous close parity, restart restoration or new native binding behavior.

All six transcript row1 hook records are present (ours340 classes; golden325). Row2 is the main app configuration initializer with policy0 on both sides. Golden engine-internal policy getter25/26 returns0; ours has no such raw getter. This remains excluded by the saved default caller filter, not a newly missing boundary method. Raw streams have app/engine/Combine callers, no direct driver label. Public eligibility/restore, didRestore, all four session DTO classes, foreign restore, discard and finalize-discard calls remain **zero on both sides**. Storage relaunch readback is not closed-tab/window restoration.

## Exact three diff headers

[diff-first-two-lines.txt](diff-first-two-lines.txt) preserves both first lines verbatim from each current diff; [selected-rows.json](selected-rows.json) also preserves them beside run7's headers. Original paths, lines1–2:

- `launch-slice/diff-launch-1.txt`: methods differing105 (run7 106); callback golden106/ours128 (run7 ours127).
- `launch-slice/diff-launch-2.txt`: methods differing46 (run7 58); callback golden47/ours62 (run7 ours62).
- `tabs-navigation/diff-launch-1.txt`: methods differing66 (run7 68); callback golden162/ours186 (run7 ours186).

These whole-scenario header changes span other owners, timing and implementation calls. They are not new sessions coverage or proof that the shutdown change caused each count difference. Goldens are archived original transcripts, not fresh paired original runs in this job. No session-scope diff category was newly introduced by the reviewed rows. Independent read-only second review agrees.

This audit performed no app/probe run, build, process inspection, source/shared mutation, board or credential access. All writes are new evidence files in this directory.
