# Stock Chromium matcher repair: bounded initial result

The replacement matcher ran successfully on the supplied ArcCore 1.170.0 and stock Chromium 156 images. **It establishes no reliable unknown-function matches and no count of Arc modifications.** The central blocker is the saved Ghidra bounds: enormous overlapping spans prevent trustworthy body attribution through most of ArcCore. A separate experiment on 1,694 named method identities demonstrates both substantial build-layout differences and false matches from short instruction signatures.

This stage concerns engine correspondence. It does not establish compatibility with the Arc 1.168.0 app interface, count app-called methods, infer callback ownership, or implement a replacement engine. No engine pseudocode was consumed; `/home/exedev/work/inputs/decompile/` was absent when inspected. Older app-layer pseudocode was not treated as engine pseudocode.

**Evidence and image identity.** All addresses below are unslid virtual addresses in the respective engine images. None are app `0x100…` addresses. Input hashes, complete segment/section mappings, command extents, UUIDs and symbol-table locations are in [images.json](images.json) and [validation.json](validation.json).

| Property | ArcCore 1.170.0 | Supplied stock Chromium 156 |
|---|---:|---:|
| File bytes | 239,297,488 | 642,894,320 |
| `__TEXT,__text` bytes | 206,182,804 | 331,458,112 |
| Architecture / subtype | thin little-endian arm64 / 0 | thin little-endian arm64 / 0 |
| Load commands | 80 | 81 |
| `LC_FUNCTION_STARTS` | absent | 887,056 starts |
| Non-STAB text `N_SECT` symbol records | 7 | 1,373,940 |
| Distinct stock text symbol addresses | — | 921,881 |

Arc SHA-256: `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd`.

Stock SHA-256: `39647a537f42460da18576c0a5856171dfec2250ed505698dbbdd99a40b75645`.

Both images contain the exact C string `156.0.8078.48`: Arc at VA `0xd008946`, stock at VA `0x13c39b60`. This supports the supplied version label; it does not verify source commit, GN configuration, compiler, or equivalence to an upstream release build.

VA-to-file translation is explicit. For example, Arc `__LINKEDIT` VA `0xe558000` maps to file offset `0xe388000`; stock `__LINKEDIT` VA `0x16ca0000` maps to file offset `0x16980000`. Text addresses happen to equal file offsets in these inputs. Tests include a synthetic image where text VA and file offset differ.

**What was repaired.** The supplied `inputs/metadata/scripts/engine_match.py` was read as a failed prototype, with no assumed prior successful output. Its mandatory `LC_FUNCTION_STARTS` lookup cannot work for ArcCore. Its Arc-only-string classifier also cannot establish proprietary or modified code. The replacement is [tools/engine_match.py](../tools/engine_match.py), using [tools/macho.py](../tools/macho.py).

The reader checks thin arm64 headers, command count/extent/alignment, segment overlap, section/file/VM consistency, symbol/string-table ranges, used indirect-symbol/stub tables, ULEB128 termination and aligned stock starts inside `__text`. All load-command extents are checked; unused command payloads are not exhaustively interpreted. Every supplied Ghidra row is checked for numeric range, ordering and region. Non-text rows are retained as evidence and excluded from matching. Input SHA-256 hashes are streamed. Baseline intervals come from `LC_FUNCTION_STARTS`, annotated and checked against executable symbols. The 34,849 symbol addresses without a corresponding LC start are recorded rather than silently treated as complete function boundaries.

The code uses positional file reads, compact integer arrays and SQLite with bounded caches. It does not load the images into Python bytes objects, build giant instruction maps, install packages, use remote machines, or run an LLM locally.

**Saved bounds are insufficient for full-body matching.** The TSV contains 607,294 rows, including 35,485 `thunk_…` names. Of these, 604,205 lie wholly inside Arc `__text`; 1,633 lie in import stubs, 149 in ObjC stubs, four in constant sections, and 1,303 are not file-backed mapped ranges, consistent with possible Ghidra external-space entries. These exclusions are explicit in `validation.json.coverage`.

Every row reports `body == end - start`. That arithmetic equality is not an independent body map. The sum is 858,548,775 bytes, far larger than the 206,182,804-byte text section, and cannot be used as executable coverage.

| Example entry | TSV end, exclusive | Reported body/span bytes |
|---|---|---:|
| `FUN_002b7940`, `0x2b7940` | `0x8b5e074` | 143,288,116 |
| `FUN_016220b0`, `0x16220b0` | `0x848e2bc` | 115,786,252 |
| `FUN_079797d4`, `0x79797d4` | `0xc20aec4` | 76,093,168 |

579,666 text entries begin inside an earlier candidate's span. Only **24,528 text candidates** satisfy the experiment's full-span isolation rule: body count equals span, no prior span covers the entry, and no later entry falls inside the candidate span. Even these are metadata-consistent byte intervals, not proven Ghidra body membership. It remains unresolved whether the exporter flattened body information, Ghidra produced unusually overlapping bodies, or both.

For example, the named method at Arc `0x1968f00`, `RenderWidgetHostViewCocoa::initWithHost:withHostHelper:`, is also inside three enormous earlier spans. The primary matcher excludes it from body matching rather than assigning all intervening bytes to any one function. This exclusion leaves only two usable named controls, both exported Chromium entry points.

**Conservative pipeline and actual results.** Both text sections were scanned for adjacent AArch64 `ADRP; ADD` pairs resolving to exact starts of C strings of at least eight bytes. References crossing candidate boundaries or inside earlier overlapping spans are rejected. This detects 5,495 Arc sites and 2,769,107 stock sites. The large disparity primarily reflects restricted Arc attribution and must not be interpreted as a code-content difference.

The pipeline finds 1,757 strings referenced by one eligible function in each image, producing 833 candidate pairs. “Unique” here means unique among detected references admitted by these rules, not unique across all real functions or all reference forms. Suffix pointers, nonadjacent instruction sequences, pointer loads and other string sections are outside scope.

Candidate checks retain ordinary integer constants, opcodes and registers while masking known AArch64 PC-relative fields: ADR/ADRP, direct branches, conditional/test branches and literal loads, plus the immediate in an adjacent ADRP-based ADD. This is relocation-sensitive instruction normalization, not comprehensive consumption of linker relocation records. Unmodeled address materialization reduces recall. Direct branch/call targets are retained; import context is resolved through validated Mach-O indirect-symbol stub tables. Internal-callee identity propagation is not implemented.

An exact classification requires substantial raw-byte equality plus a long unique-string anchor. A high-confidence hypothesis requires substantial normalized equality, a long unique anchor, and agreement of the resolved import-call sequence containing a non-`objc_msgSend` import. Ordinary constants remain significant. Both classifications refer to correspondence of candidate bytes, never ABI or semantic equivalence. Unknown classifications also require at least five accepted named controls and zero accepted negative controls; that gate did not open.

| Primary experiment | Result |
|---|---:|
| Named full-interval controls available | 2 |
| Accepted named controls | 0 |
| Explicit rotated identity negatives | 2; none accepted |
| Duplicate/signature controls | 11; none accepted |
| Unknown candidate pairs inspected | 512, covering 506 distinct Arc entries |
| Unknown exact matches | 0 |
| Unknown high-confidence hypotheses | 0 |
| Unknown ambiguous pairs | 512 |

`matches.tsv` has 514 rows: the 512 unknown pair hypotheses and the two named controls. The exported `_ChromeMain` pair is unmatched; `_ChromeAppModeStart_v8` remains ambiguous. Primary precision is **undefined**, because no positive correspondence passed the acceptance rule. Zero accepted negative controls does not establish precision.

The unknown sample is deliberately biased toward the strongest observed string anchors, then longest anchor, then address. It is not representative of the engine. The remaining 321 candidate pairs were not classified, and functions lacking admitted anchors were not searched comprehensively. Six sampled pairs have identical normalized sequences but remain ambiguous without sufficient context. For example, Arc `0xc3229e8` and stock `0x13237564` have the stock name `ReadAnythingAppController::OnDistilled(int)` and two long unique-string anchors, but no resolved import context. Their evidence remains a hypothesis.

The strongest anchored difference candidate is Arc `0xc34b324` versus stock `0x1327567c`, named `autofill::AutofillAgent::FieldTypePredictionsAvailable(...)`, with 42 unique detected string anchors and different instruction layouts. This is a useful semantic-comparison request, not evidence that Arc modified Autofill.

**Named entry-window diagnostic.** [tools/probe_entries.py](../tools/probe_entries.py) independently matches literal ObjC symbol identities from the 1.170 dyld dump to stock nlist symbols, preserving class, selector and `+`/`-` kind. No ordinal pairing, selector-only association, inferred category association, or type-encoding pairing is used. Ambiguous symbol identities are excluded. Names provide identity labels only; they are not part of the byte signatures.

Windows stop at the next Ghidra entry, next stock LC start, or 256 bytes. This examines executable entry bytes without treating a huge TSV span as a body. It still does not prove body membership. Across 1,694 independently named pairs with at least 16 available bytes, seven variable-length windows are raw-byte equal, 77 are normalized-equal, and 1,617 differ after normalization. These are entry windows, not seven exact function matches or 1,617 Arc edits.

Closed-set retrieval compares fixed-size masked prefixes against only the named control pool:

| Prefix bytes | Eligible pairs | Unique correct identity | Unique wrong identity | Ambiguous | No candidate |
|---|---:|---:|---:|---:|---:|
| 16 | 1,694 | 73 | 48 | 1,191 | 382 |
| 64 | 1,249 | 30 | 1 | 1 | 1,217 |
| 128 | 798 | 3 | 0 | 0 | 795 |

At 16 bytes, 146 rotated wrong-identity controls also have equal signatures. At 64 bytes, Arc `0x957ca0c`, `-[BrowserAccessibilityCocoa accessibilityIsAttributeSettable:]`, uniquely retrieves the wrong stock entry `0xc1d14ec`, `-[BrowserAccessibilityCocoa accessibilityActionDescription:]`, in this closed pool; its known same-identity stock entry is `0xc1d0590`. This is a concrete false-positive control against treating masked equality as sufficient. At 128 bytes only three identities are retrieved correctly; zero observed wrong identities in that tiny accepted set is not a population precision estimate. Results and checked examples are in [entry-validation.json](entry-validation.json) and [entry-pairs.tsv](entry-pairs.tsv). None promotes a primary unknown-code match.

**Artifacts and validation.**

| Artifact | Contents |
|---|---|
| [matches.tsv](matches.tsv) | Pair method, confidence, status and JSON reasons; 514 rows |
| [validation.json](validation.json) | Counts, hashes, checked controls, false-positive controls, scope and limits |
| [images.json](images.json) | Mach-O headers, mappings and image hashes |
| [evidence.sqlite](evidence.sqlite) | All supplied candidate rows, string values/locations, admitted references and anchor pairs; about 364 MiB |
| [strings.tsv](strings.tsv) | String contents and VAs used by sampled comparisons |
| [fingerprints.tsv](fingerprints.tsv) | Checked interval hashes and direct call/tail targets |
| [entry-validation.json](entry-validation.json), [entry-pairs.tsv](entry-pairs.tsv) | Broader named entry-window experiment |
| [decompile-requests.tsv](decompile-requests.tsv) | 56 prioritized exact entry/range/shard requests |
| [unit-validation.json](unit-validation.json), [unit-tests.log](unit-tests.log) | 15 passing parser, address-translation, malformed-input and instruction checks |
| [run.log](run.log), [resource.log](resource.log), [entry-resource.log](entry-resource.log) | Actual execution and resource evidence |

The primary run used 60.86 user CPU seconds, 63.39 elapsed seconds, 98% of one CPU and **256,288 KiB peak RSS**. The supplemental probe used 18.98 elapsed seconds and 171,776 KiB peak RSS. Substantial scans were serialized with the supplied heavy lock. SQLite integrity is `ok`; all 514 match rows have required fields and all 56 requested entry addresses exist in the saved TSV. One initial preflight rejected the first import-stub row; that run is explicitly preserved as failed in `preflight.log` and `preflight-resource.log`, then corrected by excluding non-text regions. It is not counted as a successful analysis.

Exact successful validation commands, executed from `/home/exedev/work/agents/matcher`:

```bash
flock /home/exedev/work/heavy.lock /usr/bin/time -v python3 -u tools/engine_match.py \
  --arc /home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64 \
  --stock /home/exedev/work/inputs/binaries/ChromiumFramework-156.arm64 \
  --ghidra-tsv /home/exedev/work/inputs/metadata/decomp/arccore-1.170.0-functions.tsv \
  --objc-dyld /home/exedev/work/inputs/context/analysis/data/dobjc170.txt \
  --out initial --controls 384 --sample 512 --requests 40
flock /home/exedev/work/heavy.lock /usr/bin/time -v python3 -u tools/probe_entries.py \
  --out initial \
  --arc /home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64 \
  --stock /home/exedev/work/inputs/binaries/ChromiumFramework-156.arm64 \
  --objc-dyld /home/exedev/work/inputs/context/analysis/data/dobjc170.txt
python3 tools/test_matcher.py --out initial
flock /home/exedev/work/heavy.lock python3 tools/finalize.py --out initial
```

For reproduction in a fresh output directory, run `bash tools/run.sh replay`. It includes those commands with logs, refuses an existing evidence database, and uses no optional Python dependencies. Increasing `--sample` can inspect more of the 833 current pairs, but cannot resolve the missing body information. `tools/export_requested_ranges.py` is a read-only Ghidra Jython helper for the parent to transfer/run if desired; it has not been executed here.

**Pinned primary-source references.** Mach-O structures and flags follow Apple XNU tag `xnu-11417.121.6`, commit `a1e26a70f38d1d7daa7b49b258e2f8538ad81650`: [EXTERNAL_HEADERS/mach-o/loader.h](https://github.com/apple-oss-distributions/xnu/blob/a1e26a70f38d1d7daa7b49b258e2f8538ad81650/EXTERNAL_HEADERS/mach-o/loader.h) (`LC_FUNCTION_STARTS` line 310; segments/sections lines 375/449; stubs line 497; indirect table line 990), and [EXTERNAL_HEADERS/mach-o/nlist.h](https://github.com/apple-oss-distributions/xnu/blob/a1e26a70f38d1d7daa7b49b258e2f8538ad81650/EXTERNAL_HEADERS/mach-o/nlist.h) (`N_STAB`, `N_TYPE`, `N_SECT` at lines 117–134). Local copies are under `sources/`.

Golden AArch64 regression bytes come from LLVM tag `llvmorg-20.1.8`, commit `87f0227cb60147a26a1eeb4fb06e3b505e9c7261`, [llvm/test/MC/AArch64/basic-a64-instructions.s](https://github.com/llvm/llvm-project/blob/87f0227cb60147a26a1eeb4fb06e3b505e9c7261/llvm/test/MC/AArch64/basic-a64-instructions.s), especially ADD immediate line 276 and signed CBZ/CBNZ vectors lines 1174–1177. Tag-resolution evidence and downloaded-file hashes are retained. No pinned Chromium source checkout or stock build configuration was supplied to this stage, so symbol hypotheses are not presented as source-level comparisons.

**Next specific questions and requests.** First, obtain actual ordered Ghidra AddressSet ranges, independent `getNumAddresses()`, thunk status and namespace for the ten overlapping megabyte candidates at priorities 1–10 in `decompile-requests.tsv`. Was the TSV's “body bytes” column exported from real body membership or calculated from min/end? The helper emits those fields without decompiling a min/end span.

Next, request engine shards for Arc `0x2df64c8` (`BrowserCrApplication::sendEvent:`), `0x31086d4` (`AppController::tryToTerminateApplication`), `0x1968f00` (`RenderWidgetHostViewCocoa::initWithHost:withHostHelper:`), `0x957ca0c` (the 64-byte false-positive control), and the two launch exports. Include actual body ranges and direct callees/callers. The remaining requests prioritize anchored upstream-function hypotheses such as Autofill. Compare semantics only after those shards arrive; different LTO, inlining, ICF, configuration and optimization can explain an unmatched function.

Finally, obtain stock source commit, compiler revision and GN/PGO/LTO configuration. With body ranges repaired, the next experiment should expand nonadjacent string/data reference recovery and independently anchored callee context, then evaluate withheld named functions and duplicate controls against the full baseline search pool before estimating correspondence coverage. This initial result cannot support a proprietary-code percentage, an Arc patch count, an ABI-equivalence claim, or a maintenance-effort estimate.
