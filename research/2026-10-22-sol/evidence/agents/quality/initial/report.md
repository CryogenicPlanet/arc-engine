The copied ArcCore 1.170.0 arm64 export is complete against the supplied manifests, but it does **not** establish correct function boundaries, complete Arc behavior, proprietary-code volume, or replacement effort. The independent scan verifies **199 unique shards, 567,959 C headers, 79 failure placeholders, 567,880 successful body candidates, and 607,294 TSV entries**. There are three manifest files containing 199 shard records; “199 manifests” would conflate records with files. Every listed shard matches its byte size and SHA-256, and the scan accounts for every copied C shard.

This audit read `context/lead-review.md`, `context/ENGINE.md`, the handoff script/table, the entire copied engine C export, metadata dumps, the pristine app’s ArcCore imports, the engine export trie, and the small Aperitif binary. It performed research and static analysis only. The report’s machine-readable results are [coverage.json](/home/exedev/work/agents/quality/initial/coverage.json), [quality-gates.tsv](/home/exedev/work/agents/quality/initial/quality-gates.tsv), and [validation.json](/home/exedev/work/agents/quality/initial/validation.json). Engine addresses below are unslid ArcCore **1.170.0** VAs. App addresses, where discussed through the ABI audit, belong to **1.168.0** and start at `0x100…`.

The failure taxonomy comes from the actual placeholder text, with exactly one evidence row per failed header in [failures.tsv](/home/exedev/work/agents/quality/initial/failures.tsv):

| Actual error text or terminal error | Entries | Category |
|---|---:|---|
| `Exception while decompiling …: process: timeout` | 48 | Timeout explicitly reported |
| `Recoverable Error: Unable to find unique hash for varnode` | 24 | Nonunique varnode hash |
| `Low-level Error: Trying to construct memory range beyond end of address space: ram` | 3 | Address-space range construction |
| `Low-level Error: Cannot properly adjust input varnodes` | 3 | Input-varnode adjustment |
| `Low-level Error: Forced merge caused intersection` | 1 | Forced-merge intersection |

Thus 31 failures are not reported timeouts. Each error was re-read independently through the read-only corpus index; classification does not infer root causes beyond those messages.

The copied exporter source resolves a critical size ambiguity. [ExportFunctions.java](/home/exedev/work/inputs/context/vm/ghidra/ExportFunctions.java:25) writes `end = getMaxAddress()+1` and `size = end-entry`; [DecompileAll.java](/home/exedev/work/inputs/context/vm/ghidra/DecompileAll.java:58) writes C-header sizes using `getNumAddresses()`. All 607,294 supplied TSV size fields equal their enclosing `end-entry` spans. Consequently the TSV is **not a contiguous body-byte map**, despite historical descriptions of its third column as body bytes. Nor does a C-header address count reconstruct body ranges or validate their membership.

| Structural measurement | Result | Limit |
|---|---:|---|
| Sum of TSV enclosing spans | 858,548,775 | Repeated spans/gaps; not code coverage |
| Entries starting inside an earlier enclosing span | 579,666 | Span overlap, not proven actual body overlap |
| Isolated positive enclosing spans, across all entries | 27,617 | Definition: no prior span covers entry, no later start lies before end |
| Isolated spans wholly inside Mach-O `__TEXT,__text` | 24,528 | Reproduces the lead/matcher gate after excluding other regions |
| C-header/TSV size disagreements at the same VA | 59,181 | Different export measures cannot be interchanged |
| C-header counts larger than `end-entry` | 872 | Body membership can extend before entry; min/body ranges are unavailable |
| TSV entries without a C header | 39,335 | 35,485 have `thunk_` names; 3,850 have other names |
| Spans at least 64 KiB / 1 MiB | 41 / 10 | Size thresholds are triage signals |

`DecompileAll.java` intentionally excludes Ghidra thunks, whereas the TSV includes them. The missing-header count is consistent with that design, but the TSV has no `isThunk` field, so a name alone cannot classify each omission. All exported C-header VAs join to a TSV entry. The 24,528 gate is metadata consistency, not assurance of valid executable boundaries.

The ten largest problematic enclosing spans are listed below and in [huge-bounds.tsv](/home/exedev/work/agents/quality/initial/huge-bounds.tsv). Their much smaller C-header counts demonstrate why “a 24 MB decompiled body” is inaccurate for `FUN_0421f778`.

| Engine VA | TSV enclosing span | Header body-address count |
|---|---:|---:|
| `0x2b7940` | 143,288,116 | 21,900 |
| `0x5f90d0` | 73,827,996 | 916 |
| `0x16220b0` | 115,786,252 | 9,416 |
| `0x1b88530` | 49,686,532 | 352 |
| `0x421f778` | 24,347,032 | 10,600 |
| `0x58ad1c0` | 62,365,088 | 316 |
| `0x59f8a80` | 3,612,624 | 628 |
| `0x6a8fb88` | 57,160,576 | 188 |
| `0x79797d4` | 76,093,168 | 344 |
| `0x7ad92f8` | 70,869,960 | 316 |

A huge enclosing span need not mean every displayed statement is wrong: the short bodies at `0x58ad1c0` and `0x6a8fb88`, for example, call distant targets. Ghidra may have merged or assigned distant chunks. Without saved address ranges and coherent disassembly, neither the span nor apparent C simplicity settles the boundary. Other export degradation exists independently of these ten spans: **39 bodies** report bad-instruction control flow, **five** report unimplemented instructions, **13,500** contain unrecovered-jumptable warnings, and **3,806** report unsettled type propagation. At `0x531b400` the C explicitly reaches `halt_baddata()` after a truncation warning. These are successful export candidates with unresolved recovery, not ordinary failure placeholders. Unsupported SVE decoding, indirect tail dispatch, compiler optimization and other causes must be investigated rather than declaring every warning a corrupt implementation. [warning-bodies.tsv](/home/exedev/work/agents/quality/initial/warning-bodies.tsv) preserves addresses, exact warning text and original header lines across the corpus.

The handoff table reproduces **exactly** from the complete export using the supplied algorithm: 1,378 `arc-objc` seeds, **524** `arc-path` seeds, six export seeds, 1,558 depth-one and 2,380 depth-two anonymous callees: **5,846 rows**. The historical “523” path-seed count is not the current result. Reproduction validates the algorithm’s output, not its interpretation. [closure-audit.tsv](/home/exedev/work/agents/quality/initial/closure-audit.tsv) adds exact body VAs, raw names, header counts and warnings.

The independent script review identifies these consequential limitations:

- All 5,846 supplied rows lack an explicit address field. Name-keyed joins discard identity information needed for categories, shared implementations and duplicate symbols.
- `rstrip(":")` removes actual trailing selector colons. This affects 1,825 corpus headers and causes **524 Arc-method seed rows** to receive zero sizes even though their original names exist in the TSV. Three normalized-name collisions overwrite bodies. In particular, `ArcBrowserApplication::init` and `ArcBrowserApplication::init:` collapse to one key; the retained body and the name-based size lookup refer to different method identities.
- Seeding only `^Arc…::` omits names under `objc::Categories::…` and methods whose shared IMP is named for another class. Exact IMP joins find only **1,633 of 1,964 declarations**, or **1,381 of 1,566 distinct IMPs**, in the full name-based closure. The 331 omitted declarations are enumerated in [methods-outside-closure.tsv](/home/exedev/work/agents/quality/initial/methods-outside-closure.tsv).
- The call regex recognizes only lowercase, eight-digit `FUN_…(` text. It misses `thunk_FUN_…`, named callees, virtual calls, ObjC dispatch, function-pointer/block callbacks, address-taking and deeper chains. It is not a C lexer and can match declarations, comments or strings. The anonymous function’s own declaration produces a self-token, although the visited set prevents that token from expanding the closure.
- Exactly two call rounds are followed. No stock-Chromium identity or semantic stopping rule is implemented. “Named outside Arc” functions are excluded regardless of their actual role. Source-path and `arc.mojom` substring tests establish textual references, not exclusive code ownership.

Within this closure, 1,196 header counts disagree with the TSV span at the same VA; 225 bodies have unrecovered-jumptable warnings and 87 have unsettled type propagation. Only `0x421f778` crosses a later function start in its own enclosing span. Another 5,703 entries sit inside *earlier* enclosing spans, largely because those earlier spans are huge; this does not establish that all 5,703 actual implementations overlap or are corrupt. No closure body reports the bad-instruction warning found elsewhere in the corpus. Removing the known giant leaves a name-joined TSV sum of 2,454,560, but that number still contains failed name joins, gapped/aliased ranges and upstream callees. Neither that sum nor header-count sums are a rewrite-size estimate or a bound on all proprietary code.

The class counts reconcile as follows. [inventory.json](/home/exedev/work/agents/quality/initial/inventory.json) independently parses explicit dyld class/category/method-kind/selector/IMP identities, checks the pristine app’s undefined nlist symbols with the ArcCore dylib ordinal, and compares the current engine’s export trie to the supplied exports.

| Set | Count |
|---|---:|
| Arc-prefixed metadata classes, 1.168 and 1.170 | 146 |
| Arc metadata method declarations, both versions | 1,964 = 1,821 base + 143 category |
| Arc categories | 16 |
| Current Ghidra Arc-named method/seed namespaces | 144 |
| Export names, identical supplied sets for 1.168/169/170 | 259 = 146 class + 105 metaclass + eight other symbols |
| App 1.168 ArcCore imports, binary verified | 67 = 63 class + four other symbols |
| Declarations on imported classes | 834 = 786 base + 48 category |
| Imported class/kind/selector dispatch keys | 830 |

The 146 exported class names equal the 146 Arc metadata class names; a metaclass symbol is not exported for every class. The historical “142 classes with named methods” is not reproduced: current evidence gives 144. `ArcBrowserSidePanelEntryInfo` and `ArcNotificationItem` have no Arc-named namespace, yet all seven and five respective method declarations have C headers at their IMPs under shared names. Lack of a class-named body is not missing implementation evidence. [shared-IMP-aliases.tsv](/home/exedev/work/agents/quality/initial/shared-IMP-aliases.tsv) preserves all shared declaration identities.

The 834 declarations are **not 834 proven app calls**. The ABI agent’s separate [used-calls.tsv](/home/exedev/work/agents/abi/initial/used-calls.tsv) has branch-supported caller/selector evidence and separately reviewed receiver contexts; this quality stage did not repeat that full app disassembly scan. Selector overlap alone does not identify an engine receiver, and returned objects/delegates expand the surface beyond imported classes. Stable export names and metadata counts do not establish C prototypes, block signatures, ownership, threading or behavioral compatibility.

Six older Arc encodings remain unknown in the ABI inventory, and this audit preserves them in [old-encoding-unknowns.tsv](/home/exedev/work/agents/quality/initial/old-encoding-unknowns.tsv): `ArcBrowserContext(Extensions)` selectors `getChangedContentSettingsForOrigin:`, `getChangedPermissionsForOrigin:`, `chatClosed:withReason:`, `productConnectionClosed:withReason:`; `ArcNotification::browserContextQualifiedID`; and `ArcSessionID::identifier`. The 1.168 otool dump independently shows repeated IMPs at `0x41491c0`, `0x4150520`, `0x416315c`, and `0x41650e0` (lines 61652/61655, 61754/61757, 26674/26680, 27069/27072). In the last case the candidate encodings even differ in signedness. No older selector/type pairing is inferred by row order. There is no supplied 1.169 ObjC dump.

Method identity also requires preserving base/category distinctions. `ArcPermissionRequest::additionalData` at `0x2d0fb40` joins to the shared Ghidra name `WebNNMLFeatureProvider::featureNames`; its Extensions implementation at `0x440a85c` has its own category-qualified name. Both are absent from this bounded closure. A CEF mapping keyed only by class/selector must remain provisional across that override until each implementation and runtime resolution are assessed.

The per-class availability audit covers **all 146 assigned classes and all 1,964 declarations** in [class-coverage.tsv](/home/exedev/work/agents/quality/initial/class-coverage.tsv), [method-coverage.tsv](/home/exedev/work/agents/quality/initial/method-coverage.tsv), and [spec-expectations.tsv](/home/exedev/work/agents/quality/initial/spec-expectations.tsv). Assignments are disjoint and complete: spec-1 has 34 classes, spec-2 37, spec-3 37 and spec-4 38; every group has 491 declarations. The four available spec inventories are compared by exact identity in [spec-inventory-checks.tsv](/home/exedev/work/agents/quality/initial/spec-inventory-checks.tsv). This initial snapshot reviews inventory expectations and evidence availability, not completed prose specs or their behavioral claims.

Headers exist at 1,963 of 1,964 declaration IMPs, representing shared implementations among 1,566 distinct IMPs; none of these headers is a failure placeholder. The exception, `ArcWebContents(Extensions)::createPaneHostView` at `0x4410bb4`, is a four-byte B thunk to **`0x47390dc`**. That target has exported C which allocates `WebContentsPaneHostView` and calls `initWithFrame:`. Likewise `_CallWithEHFrame` at `0x441d770` branches to **`0x2e0238`**, whose exported C invokes a pointer at argument offset `0x10`. [branch-probes.tsv](/home/exedev/work/agents/quality/initial/branch-probes.tsv) records instruction words and file offsets translated through Mach-O segments. These particular offsets happen to equal VAs; the tool does not assume that. Neither target appears in the supplied closure. These observations resolve missing-body availability questions while leaving native hosting and exception/unwind contracts unresolved. Older ABI initial-report statements about only 38 shards or these targets being absent are stale against the complete export.

The small **libaperitif.dylib was not in the Ghidra export**. Static parsing of its 169,584-byte universal copy yields:

| Measurement | x86_64 | arm64 |
|---|---|---|
| Fat-file slice offset / bytes | 16,384 / 59,152 | 81,920 / 87,664 |
| Slice SHA-256 | `b1fdaf05df7fbdb05875e45e97ca6baa39364ab1fd953fdb6a92b30c7250a409` | `d57c004cf03b49def8bf00443d278ad060fdbf18afecc7fdab37d2dff8bef832` |
| Exported symbol and VA | `_AperitifCheckInitialized`, `0x7b0` | `_AperitifCheckInitialized`, `0x810` |
| Undefined nlist symbols | 59 | 59 |
| Minimum macOS / SDK recorded | 13.0 / 26.5 | 13.0 / 26.5 |

The complete file SHA-256 is `780aa93afe922c9756df21c32bbf98b844126a088d1953680b06f19d430dc955`. Both slices are `MH_DYLIB`, load `/usr/lib/libsandbox.1.dylib` and `/usr/lib/libSystem.B.dylib`, and declare the ArcCore Libraries install path. Symtab and export-trie export sets agree. Imports include `_sandbox_init_with_parameters`, `_sandbox_apply`, `_sandbox_free_error`, executable-path/argc/argv access, allocator-zone operations, `dlopen`/`dlsym` and ordinary C/C++ runtime support. A four-byte `__init_offsets` section exists in each slice. Exact metadata and strings are in [aperitif.json](/home/exedev/work/agents/quality/initial/aperitif.json), [aperitif-symbols.tsv](/home/exedev/work/agents/quality/initial/aperitif-symbols.tsv), and [aperitif-cstrings.tsv](/home/exedev/work/agents/quality/initial/aperitif-cstrings.tsv).

Aperitif is present in **upstream Chromium**, contrary to treating its name as proof of an Arc-only subsystem. At pinned Chromium revision **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c** (156.0.8078.48), [content/public/app/aperitif_mac.h](/home/exedev/work/inputs/chromium-156/content/public/app/aperitif_mac.h:7) describes C++ initialization before system-library static initializers, primarily to engage helper sandboxing. [content/app/aperitif_mac.cc](/home/exedev/work/inputs/chromium-156/content/app/aperitif_mac.cc:56) implements early allocator-zone registration, Seatbelt argument handling and an initialized-state check. [chrome/app/chrome_exe_main_mac.c](/home/exedev/work/inputs/chromium-156/chrome/app/chrome_exe_main_mac.c:16) calls the check in a constructor; [aperitif_unittest.mm](/home/exedev/work/inputs/chromium-156/chrome/browser/mac/aperitif_unittest.mm:41) tests helper initializer ordering. The dylib’s exported check and strings `Failed to create seatbelt sandbox server`, `Failed to initialize sandbox`, `SandboxSerializer: Failed…` and `--seatbelt-client=` are consistent with that upstream role. This is a supported structural/source correspondence, not binary identity or proof of exact Arc modifications. Exact implementation semantics require additional bounded dylib decompilation/disassembly; none was performed. Helper executables, their entitlements, their load order and their actual launch paths were not copied/analyzed.

[Source provenance](/home/exedev/work/agents/quality/initial/source-provenance.tsv) records hashes, local paths and pinned upstream links. Mach-O layout constants were checked against Apple XNU tag `xnu-11417.121.6`, `EXTERNAL_HEADERS/mach-o/loader.h` and `nlist.h`, copied under this role’s `sources/`. No fetched source instructions were executed.

The exact reproducible run is:

```bash
cd /home/exedev/work/agents/quality
bash tools/run.sh
```

The small tools are [audit_export.py](/home/exedev/work/agents/quality/tools/audit_export.py), [audit_inventory.py](/home/exedev/work/agents/quality/tools/audit_inventory.py), [triage_aperitif.py](/home/exedev/work/agents/quality/tools/triage_aperitif.py), [finish_audit.py](/home/exedev/work/agents/quality/tools/finish_audit.py) and [validate.py](/home/exedev/work/agents/quality/tools/validate.py). Each analysis stage is serialized with `flock /home/exedev/work/heavy.lock`; Python uses one CPU and streams large evidence. The complete export scan’s recorded peak RSS is **153,424 KiB**, with 48.99 seconds user CPU, 4.80 seconds system CPU and 55.35 seconds elapsed. Validation rechecks every header identity/size against the independent read-only index and re-reads every failed body. These checks validate provenance and internal consistency only.

For synthesis, the following claims must remain provisional or be removed:

- “All 79 timed out”; “142 named classes”; “523 path seeds”; “24 MB function body”; and any claim that byte sums measure engine coverage, Arc implementation size or rewrite effort are contradicted or mis-scoped by current evidence.
- A bounded call closure is all proprietary code, captures every class method, establishes unchanged stock Chromium, or bounds the changes needed per Chromium release. Logging/source-path absence and failed baseline matching do not establish those conclusions.
- Stable export names, equal metadata counts, available C text or a historical engine-swap observation establish a replacement ABI or behavioral contract. Full class specs must separately review state, enums/errors, ownership, nil/teardown behavior, block/callback signatures, sequence/thread/reentrancy requirements, categories, native views and required Chrome services.
- CEF classifications shared across base/category implementations, compatibility with native pane/view hosting, and Chrome-service substitutions are settled. The pinned CEF `parent_view`/Alloy restriction remains a route gate identified by the lead review.
- Aperitif is proprietary Arc functionality or can be omitted, or helper executables have been verified. The upstream source establishes its intended sandbox role; exact Arc binary behavior and helper integration are still open.

The next specific evidence questions are: (1) export actual Ghidra address ranges and inspect coherent disassembly for the ten huge spans, the 872 count-before-entry cases and consequential warning-bearing callees; (2) resolve `0x47390dc` and the `WebContentsPaneHostView` contract through native host creation, ownership and teardown; (3) establish unwind/callback ABI at `0x2e0238` from instructions and unwind metadata; (4) independently resolve the six older selector/type associations without positional pairing; and (5) inspect copied helper executables and a bounded Aperitif decompile before making launch or sandbox-equivalence claims. No older 1.168 engine body comparison or complete semantic baseline match was done here.
