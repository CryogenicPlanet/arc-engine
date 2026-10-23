# spec-2: ArcCore class specifications

Completed the assigned **37 class specs and 491 declared method rows**, preserving category and class-versus-instance identity. The assignment is the ArcCore **1.170.0 arm64** declared surface, not measured calls from the Arc **1.168.0** app. All 491 declarations are instance methods and resolve to **453 unique IMP VAs**. No product implementation, build, Ghidra job, external coordination or app execution was performed.

## Findings that affect a replacement

- **Arc services need their own contracts.** ArcAIService operations are in its Extensions category. Streaming crosses 0x43d7a5c → 0x43310bc → 0x429fa38, which installs `ai.mojom.AIStreamedInferenceListener` and reaches `ai.mojom.AIService`. Content conversion crosses 0x43de074 → 0x42a75b4 and reaches `content_conversion.mojom.ContentConversionService`. Their conversion options, result adapters, callback queues, cancellation and external library requirements remain consequential gaps. These names do not establish libaperitif ownership or CEF coverage.
- **DevTools transport overlaps CEF, but frontend embedding remains a gate.** Arc executes domain/name/params through 0x4415520 → 0x435b114 → 0x435dd94, with dictionary-param validation and saved completion state. Its frontend uses `DevToolsAPI.showPanel` or a native creation path (0x4357078); `toolboxWebContents` exposes/caches an ArcWebContents wrapper (0x4415204). Its weak delegate bridge emits display, inspect, close and focus selectors (0x4415fe0, 0x441604c, 0x44160b8, 0x441610c). CEF's pinned `include/cef_browser.h:633`, `:649`, `:660` offer transport/method/observer primitives; `:582` offers frontend creation. CEF macOS `include/internal/cef_types_mac.h:151` forces Alloy for `parent_view`. This establishes a partial source-backed adaptation route, not view/pane or callback equivalence.
- **Thumbnails and media expose Chrome-related lifecycle behavior.** Thumbnail observer registration creates shared bridges with weak observer/owner references (0x441c680 → 0x436d904), and callbacks use `webContents:thumbnailReady:` (0x441cccc). Requests reach `ThumbnailTabHelper` by its data key (0x436df2c, 0x436d69c); compare pinned Chromium `chrome/browser/ui/thumbnails/thumbnail_tab_helper.h:27`, `:52` and `.cc:242`, `:356`. Media action/PiP calls have unavailable/no-session/unsupported/already-active errors (0x4360b58, 0x436051c), plus separate app PiP callbacks (0x441b590, 0x441b5e4, 0x441b638, 0x441b68c). CEF's `SetAudioMuted` (`include/cef_browser.h:1003`) alone does not establish these contracts.
- **Value wrappers have nontrivial edge behavior.** AI streaming interval narrows to signed int16 (0x443a318); AI retries narrow to signed byte, and type -1 produces an NSError through the supplied pointer (0x43dcbdc). URL payload string/data share storage, request-method enums swap 1/2, and policy normalizes to 0/1 (0x440d76c, 0x440d834, 0x440d71c, 0x440db2c; table 0xc9d8190). Tab-group title/color/collapsed are independently optional; nil is different from explicit false/empty (0x443d9ec, 0x443dc40, 0x443dcc4). Profile attributes assignment leaves an already-cached userColor wrapper unchanged along the inspected path (0x443b028, 0x443b0f4 → 0x441e0a0).
- **Logging has a state-test difference worth preserving or testing.** Arc's predicate accepts INITIALIZING, STARTING_LOG and LOGGING (0x4408898 → 0x432c834); stock `NetExportFileWriter::IsLogging` accepts LOGGING, STARTING_LOG and STOPPING_LOG (`components/net_log/net_export_file_writer.cc:312`). The writer identification remains a source-backed inference from state strings and call shape. Arc's start rejects headless/already-active logging and schedules duration in microseconds (0x44088ac → 0x432c950).
- **Authentication is a callable-value boundary.** ArcWebLoginDialog has separate accept, cancel and close callables. Accept converts nil credentials to empty strings; each method invokes its own stored callable (0x441d470, 0x441d5b4, 0x441d464). One-shot, reentrancy and presentation-thread semantics have not been inferred from signatures.

## Evidence and reconciliation

`tools/collect.py` reparses the pristine Mach-O through segment-aware VA-to-file-offset translation and chained-pointer decoding, independently joins dyld names by **class, category, kind, selector and IMP**, and compares the initial binary inventory by identity, type encoding and method-record VA. It does not pair dyld/otool rows by list order. `tools/otool_check.py` corroborates each binary-identified encoding/IMP pair in raw otool output; repeated/shared IMPs retain separate metadata identities. Every method row carries the exact method-record VA, dyld/otool lines and immutable corpus path/offset/length.

There are 20 ArcAIService(Extensions), one ArcColor(Init), and two ArcCommandBarRankingExample(Extensions) declarations. None of these categories overrides a same-kind base selector in this assignment. Shared IMP aliases are retained in `evidence/shared-imps.json`; a Ghidra name belonging to a different class is not used to change the metadata identity or type.

Exact IMP bodies were read with the indexed corpus mechanism, with direct checks using `/home/exedev/work/read-function.py`. The reproduced collector queries the same read-only SQLite byte-range index. It stores complete bounded snapshots of all assigned IMP bodies and 50 selected bridge bodies. No repeated full-shard scan is needed. A tiny instruction recognizer verifies **59 method identities** as exact scalar-load-plus-RET leaves; it is not a full disassembler or function matcher. Segment translation is always applied even where this image's numerical file offsets happen to equal VAs.

The headerless historical handoff TSV has schema **name, call-depth, seed-reason, reported-body-bytes**, not address/size. It contains 5846 rows, including 388 named rows for these classes; 146 of those report zero bytes. Its zero depth column is not an address. Named rows can lose trailing selector colons and cannot replace metadata identity. The bounded audit is in `evidence/handoff-audit.json`. `FUN_0421f778` was not read or used. No conclusion is based on summed Ghidra bounds, unmatched functions or absent source paths.

## Coverage and confidence

| Measure | Result |
|---|---:|
| Assigned / produced class specs | 37 / 37 |
| Assigned / produced declared method identities | 491 / 491 |
| Unique IMPs | 453 |
| Body available / failed / missing / truncated, counted per method identity | 491 / 0 / 0 / 0 |
| Explicit decompiler signature/type artifact flags | 8 |
| Local operation verification only | 151 |
| Bounded static review | 140 |
| Partial local observation | 200 |
| Rows retaining explicit unresolved questions | 403 |
| Fully verified replacement behavior | 0 |
| Direct app calls analyzed in this stage | 0 |

The eight explicitly flagged cases include omitted floating returns, a hidden aggregate-return destination, truncated self types, omitted volume argument propagation, and unsettled type propagation in `toAnnotatedVisit` (0x440db0c, 0x43dd10c, 0x443b1b0, 0x40b1230, 0x441d464, 0x441abf0, 0x4400d54; one shared VA covers two class identities). Raw prefixes resolve several local operations; they do not validate the entire ABI. No merged-body corruption was established among these assigned bounded snapshots, but successful export and short pseudocode are not a clean-boundary certificate. Decompiler warning and ownership-fixup limitations remain.

`local_verified` is a narrow static field/branch observation; it is not runtime testing or a completed method contract. Likewise, `bounded_review` does not mean exhaustive callee verification. 403 rows retain concrete questions, and all methods still lack dynamic replacement equivalence testing. The preliminary 834-method imported-class declaration count is not used as app-call evidence. Zero app calls analyzed means unmeasured, not unused.

## Artifacts

- `classes/<Class>.md`: all 37 assigned class specs, including state/layout, lifecycle, callbacks, threading limits, source-backed route implications and complete method matrices.
- `methods.tsv`: 491 exact declared identities, encodings, IMPs, body/semantic status, observed behavior, backend calls, evidence and unresolved questions.
- `coverage.json`, `validation.json`: machine-readable reconciliation and validation result.
- `callbacks.tsv`: nine exact outgoing callback selector bridges with VAs and corpus anchors.
- `next-requests.tsv`: 13 prioritized next inspections with exact VA and index location; labels without an exact function entry are marked explicitly.
- `evidence/methods-bodies.json`, `layouts.json`, per-class `.txt`, `bridges.json`, `bridge-vtables.json`, `imp-prefixes.json`, `enum-tables.json`, `shared-imps.json`: reproducible bounded evidence.
- `primary-sources.tsv`: 11 pinned primary files, source URLs, local paths and SHA-256 values. Chromium revision **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**; CEF revision **426920c33ef22551e0b8f48cf343d623ac25642d**. The copied Chromium subset's omissions are not API-absence evidence.
- `tools/`: stdlib-only evidence collection/rendering/validation tools and human-reviewed annotation dictionaries. `macho_metadata.py` is a local copy of the initial inventory parser; provenance records both copies' hashes.

## Validation and limits

Exact validation run from `/home/exedev/work/agents/spec-2`:

```sh
/usr/bin/time -v bash tools/reproduce.sh > specs/validation-run.log 2> specs/validation-resource.log
```

Result: **PASS**, 37 files, 491 exact metadata identities and complete IMP snapshots, 50 bounded bridge snapshots, class-matrix reconciliation, source hashes, links and artifact flags checked. Resource measurements are in `validation-resource.log`. Tools write only under this role directory and use no local model, package install or build. Public CEF headers were fetched at the exact pin into `specs/sources/cef`.

No Arc 1.168 app callsite/receiver tracing, older engine body comparison, dynamic ownership probes, macOS view hosting tests, complete delegate-protocol audit, exhaustive C++ closure, helper executable analysis or external library implementation analysis was performed here. ObjC ownership fixups obscure some retain/copy/weak operations; explicit weak initialization/destruction and explicit copy messages are cited where observed. No queue contract is inferred from object names or atomic reference counts.

## Next specific questions

1. At DevTools native creation 0xc1057c0, what owns the toolbox WebContents/view and how are display modes/native panes embedded? Resolve this before treating CEF CDP transport as frontend parity.
2. What do streaming/conversion and DevTools completion adapters pass, and on which sequence? Follow vtables 0xdf6b378 / 0xdf6b398 / 0xdf6b528 and the label 0x44162ac; the latter has no exact indexed function entry.
3. How do subscriptions, connection IDs and callbacks drain on close or target destruction (0x435b784, 0x4356754)? What happens during reentrant delegate replacement?
4. What does the logging timer stop/finalize (0x432ccbc), and what are thumbnail no-observer/stale-image and delivery semantics (label 0x436dff4)?
5. Which caller/consumer contracts define credential helper ownership (0x6bd82ac), volume argument forwarding (0x4148644), flat history conversion (0x4336aac) and query option boundaries (0x4331e20)?

These are focused semantic/disassembly requests, not missing-decompile blockers or requests for another full matching/build pass.
