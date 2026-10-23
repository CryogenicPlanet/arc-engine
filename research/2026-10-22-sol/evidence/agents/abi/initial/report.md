Arc 1.168.0 arm64 can be described against ArcCore 1.170.0 with a stable **declared ObjC metadata surface**, but these artifacts do not establish behavioral compatibility with a replacement Chromium engine. The regenerated count is exactly **834 method declarations on 63 imported classes**. It is neither 834 app-called methods nor the complete reachable engine interface. Browser creation returns additional engine objects whose class symbols the app does not import.

This stage produced metadata parsers, bounded binary checks and call-evidence tables; no product implementation or Chromium build was attempted. Engine shards arrived during this initial stage, so the final artifacts include a limited inspection of the delivered engine pseudocode rather than reporting it absent.

The principal deliverables are [contract.tsv](contract.tsv), [used-calls.tsv](used-calls.tsv), [decompile-requests.tsv](decompile-requests.tsv) and [the regeneration script](../scripts/regenerate.sh). Input hashes, source locations, image versions and address bases accompany the evidence.

| Inventory measurement | Result | Interpretation |
|---|---:|---|
| App-imported engine classes | 63 | Independently checked against pristine app Mach-O undefined symbols |
| Declared methods on those classes, 1.168 / 1.170 | 834 / 834 | Includes categories, generated getters/setters, constructors/destructors and internal-looking methods |
| Base / category declarations in 1.170 | 786 / 48 | Categories remain separate records |
| Instance / class declarations | 821 / 13 | `-` and `+` are preserved |
| Distinct class/kind/selector keys | 830 | Four selectors have both base and category implementations |
| Known equal older/newer encodings | 828 | Exact raw encoding equality after an unambiguous cross-dump join |
| Older encodings unresolved | 6 | Left blank, with explicit pairing status |
| Added / removed imported-class declarations | 0 / 0 | Relative to the supplied 1.168 metadata |
| All `Arc*` engine classes / categories | 146 / 16 | Broader than the imported-class subset |
| All `Arc*` method declarations | 1,964 | Same keys in both versions; six older encodings remain unknown |
| Normalized Arc layout/property/protocol changes | 0 found | Compared actual ivar offsets/types/sizes, properties, superclass/protocols and layout maps |

All 259 supplied export names are identical in 1.168, 1.169 and 1.170. This checks names, not C prototypes, symbol values, callback behavior or 1.169 method encodings: no 1.169 ObjC dump was supplied. Detailed comparisons are in [compatibility.tsv](compatibility.tsv), [arc-method-changes.tsv](arc-method-changes.tsv), [arc-layout-semantic-changes.tsv](arc-layout-semantic-changes.tsv) and the per-version `arc-class-details-*.json` / `arc-category-details-*.json` files. The latter preserve normalized layouts without mistaking relocated metadata pointers for ABI changes.

The six unresolved older signatures are:

| Class / category | Selector | 1.168 IMP |
|---|---|---|
| ArcBrowserContext / Extensions | `getChangedContentSettingsForOrigin:` | `0x41491c0` |
| ArcBrowserContext / Extensions | `getChangedPermissionsForOrigin:` | `0x41491c0` |
| ArcBrowserContext / Extensions | `chatClosed:withReason:` | `0x4150520` |
| ArcBrowserContext / Extensions | `productConnectionClosed:withReason:` | `0x4150520` |
| ArcNotification | `browserContextQualifiedID` | `0x416315c` |
| ArcSessionID | `identifier` | `0x41650e0` |

These shared implementation addresses defeat a unique older selector/type pairing. The raw otool rows are retained in `inventory-summary.json`; they are not assigned to selectors by row position. Even when candidate rows happen to share an encoding, this report retains the missing independent selector linkage.

Pairing validation uses four independent constraints. First, the dyld parser identifies explicit class, category, method kind, selector and IMP. Second, the otool parser isolates the owning class/metaclass/category list, validates its declared count, and joins unique class/category/kind/IMP records. For 1.168, an already resolved selector-reference address may disambiguate another record only when there is exactly one matching selector; ambiguous cases stay unknown. Third, the 1.170 binary independently resolves each relative method entry, selector reference, type string and IMP. Fourth, a separate direct binary walk through the class and category lists reproduces all **4,314** 1.170 method records, including matching selectors, encodings and implementations. The whole-engine 1.168 dumps contain 4,306 methods, of which 4,250 pair unambiguously; 56 whole-engine unknowns include the six in the Arc/imported subset.

The four imported-class duplicate dispatch keys are `ArcBrowserContext` instance selectors `extensions`, `id`, `metrics`, and `ArcPermissionRequest` instance selector `additionalData`. Their category implementations differ from the base IMPs. [category-shadowing.tsv](category-shadowing.tsv) records eight such keys across all Arc classes. A replacement must preserve category attachment/override behavior; choosing the first class-list implementation is unsafe. Runtime method resolution itself was not observed.

App-side call evidence comes from all 57 supplied APP pseudocode shards and the pristine app executable, with these separate evidence levels:

| Evidence | Count | What it establishes |
|---|---:|---|
| Binary-validated ObjC stubs whose selector also exists on an Arc engine class | 682 selectors | Stub selector availability in the app |
| ObjC caller/selector rows supported by pseudocode and a binary BL or tail B | 4,336 | App dispatch to that selector stub from the saved caller range |
| ObjC caller/selector rows with pseudocode only | 11 | Source evidence; binary branch not located by this scan |
| C import caller rows with source and binary BL | 6 | Three `_CallWithEHFrame`, one `_arcCategoryEnabledFlag`, two `_emitTraceEvent` caller rows |
| Imported-class declarations sharing an app-dispatched selector | 422 | Candidate matches, **not 422 class-confirmed engine calls** |
| Imported-class declarations with no matched app caller/stub evidence | 412 | Declared only under this scan |
| Manually reviewed boundary examples | 17 | Source receiver context and limitations are recorded individually |

[used-calls.tsv](used-calls.tsv) has 4,353 caller/selector rows, including six C rows; 4,342 have binary branch evidence. These are grouped caller/selector records rather than instruction counts. The independent check validated 5,353 branch instructions. Each row records app caller VA/name, source file/line, stub VA and selector-reference VA, BL and tail-B addresses, possible engine methods and an unresolved runtime receiver status. Calls sharing generic selectors such as `cancel:`, `setDelegate:`, `name` or `close` also occur on unrelated system/app objects. They must not be counted as engine usage merely because an engine class declares that selector. The 682 selector stubs all have some source caller evidence in this snapshot; a stub alone would otherwise remain `selector_stub_only`.

[reviewed-app-boundaries.tsv](reviewed-app-boundaries.tsv) separates stronger static receiver evidence from selector overlap. Examples with exact app instructions include:

| Boundary | App caller / call instruction | Engine declaration |
|---|---|---|
| Application allocation and `init:` | `0x1004aa890` / `0x1004aa8c8` | `ArcBrowserApplication`, `0x43e6548` |
| Initialize application | `0x1004ad314` / `0x1004ad77c` | `initialize:`, `0x43e6fc8` |
| Create browser | `0x1004ad0a0` / `0x1004ad1a4`, `0x1004ad1c8` | `createBrowser:withDelegate:forWindow:`, `0x43e7048` |
| Create browser context | `0x1004af380` / `0x1004af488` | `createBrowserContext:isHidden:type:completion:error:`, `0x43e8368` |
| Create WebContents | `0x100525e6c` / `0x100525eb8` | `ArcBrowser::createWebContents:`, `0x43e154c` |
| Download pause / resume | `0x10052d0ec` / `0x10052d210`; `0x10052d2c8` / `0x10052d408` | `ArcDownloadItem`, `0x442fd80` / `0x442fe38` |
| Register navigation delegate | `0x10053b870` / `0x10053b8e0` | `ArcWebContents::setNavigationDelegate:`, `0x4440dec` |
| Open DevTools protocol connection | `0x1004f285c` / `0x1004f2914` | `openProtocolConnection:error:`, `0x4415b60`; receiver flow remains weaker |

The app's Swift metadata identifies `ChromiumBrowser.browser` as `ArcBrowser`, `WebContentChromium.webView` as `ArcWebContents`, and `ChromiumDownload.item` as `ArcDownloadItem` ([swift-types.swift](/home/exedev/work/inputs/app168/dump/swift-types.swift:7593), lines 7787 and 7696 respectively). The pseudocode loads these fields at the reviewed calls. This is corroborating static context, not a Swift type deduced from `@` encoding or a runtime receiver observation. `ArcBrowser` and `ArcWebContents` are absent from the 63 imported class symbols, illustrating why that imported surface is a lower bound. The DevTools example deliberately retains weaker receiver status.

The reverse callback contract is substantial. Direct app binary parsing found **235 method declarations across 26 `Arc*` protocols**, representing 198 unique selectors. Fifteen ADK app classes directly adopt Arc protocols and implement 250 matching class/selector records; 248 have supplied APP pseudocode. See [app-arc-protocol-methods.tsv](app-arc-protocol-methods.tsv), [app-callbacks.tsv](app-callbacks.tsv) and [app-callback-classes.tsv](app-callback-classes.tsv). Thirteen implementation/protocol pairs have different raw encodings containing richer object/block annotations on the implementation side. They are retained as raw differences, not reported as proven ABI breaks. Runtime invocation by the engine is a separate question from these app declarations.

The newly delivered engine snapshot contains 38 shards and 77,187 function headers. Of the requested function bodies, 62 are present; 818 of the 834 imported declarations have a body header at their IMP in these shards. Availability is indexed separately from semantic review. [engine-pseudocode-index.tsv](engine-pseudocode-index.tsv), [engine-request-signals.tsv](engine-request-signals.tsv), and [engine-selected.c](engine-selected.c) make the selected source reproducible. This is a partial decompile snapshot, not engine byte coverage or proof that absent headers imply absent functions.

The engine scan found **159 source dispatch sites for 153 app-protocol selectors**, all in callers containing binary branches resolving to `_objc_msgSend`. [engine-callback-selector-evidence.tsv](engine-callback-selector-evidence.tsv) retains engine function/file/line and possible app callback IMPs. The table does not assign a particular selector to a particular branch when a caller makes multiple sends, nor does it observe the dynamically selected app receiver. Concrete correspondences include:

| Engine callback source | Engine caller | App callback IMP |
|---|---|---|
| `didInitializeBrowserProcess` | `0x43eb374`, `decompile/0x43.c:157629` | `0x1004b73fc` |
| `browserContext:downloadManagerInitializedWithItems:` | `0x4042484`, `decompile/0x40.c:79780` | `0x1004e8c90` |
| `browserContext:onDownloadCreated:` | `0x404257c`, `decompile/0x40.c:79798` | `0x1004e8f38` |
| `browserContext:onDownloadUpdated:` | `0x4042610`, `decompile/0x40.c:79816` | `0x1004e91d0` |
| `browserContext:onDownloadRemoved:` | `0x404269c`, `decompile/0x40.c:79834` | `0x1004e9468` |
| `webContents:decidePolicyForNavigationAction:` | `0x4445a58`, `decompile/0x44.c:60169` | `0x100535984` |

Several contract details now have limited source and instruction evidence. [engine-observations.tsv](engine-observations.tsv) records the observations and unresolved questions explicitly:

- Application `initialize:` translates its configuration through `FUN_043ec404`, copies temporary configuration storage, calls `FUN_042b0850`, destroys the temporaries and returns an integer. The conversion helper reads named configuration selectors including `argc`, `argv`, `profilePath`, restoration policy and run-loop termination settings. This establishes an adaptation layer to investigate, not an upstream Chromium symbol mapping.
- Application construction, browser-context registration, downloads registration and WebContents navigation registration construct weak-reference bridges. For example, engine instruction `0x4440e8c` reaches `_objc_initWeak`; `0x4440e80` reaches `_objc_copyWeak`. Context registration instruction `0x43eda20` reaches `_objc_storeWeak`. The parser follows exact arm64 range-extension islands to imported symbol stubs. These operations corroborate the weak-reference construction visible in pseudocode; they do not settle native bridge ownership, teardown, nil-unregister behavior or callback execution threads.
- `tryToTerminate:` and `openProtocolConnection:error:` contain binary calls to `_objc_retainBlock` at `0x43e6f1c` and `0x4415ba4`. The APP side copies and releases completion blocks around several reviewed calls. Ghidra injections hide some retain/store operations in the displayed C, which is why the binary checks matter. Callback invoke signatures and retained lifetimes still need analysis.
- The navigation-policy bridge sends `webContents:decidePolicyForNavigationAction:`, checks the returned value against `0xc`, and either returns it or traps in the displayed code. The app callback independently declares `q32@0:8@16@24`. Policy meanings, main-thread assumptions and reentrancy remain open.
- DevTools connection creation returns nil or an `NSNumber` made by `numberWithInt:` in the displayed source. Its error conversion, connection lifetime, handle reuse and message-block contract remain open. The bare `@` method encoding by itself would not identify `NSNumber`.
- Download controls test native-wrapper state and conditionally make validity/virtual calls. Mapping those slots to stock Chromium requires separate evidence. Ghidra prints `int` for `pause`, while the independently decoded ObjC method encoding is `v16@0:8`; the pseudocode prototype is not adopted as the ABI.

APP-side threading evidence also needs care. `FUN_1004ec75c` ([0x1004.c](/home/exedev/work/inputs/app168/ghidra/decomp/0x1004.c:184992)) calls `_swift_task_getMainExecutor` / `_swift_task_isCurrentExecutor` and has an assertion path; wrappers such as `0x10053fa9c` route through it. This suggests a main-executor expectation on selected app paths. It does not prove every engine callback runs on that executor or that this helper schedules a hop. A callback behavior test must distinguish checking the current executor from dispatching to it.

[engine-runtime-calls.tsv](engine-runtime-calls.tsv) has 1,715 instruction/import-call records corroborating the selected engine functions. Each includes original branch target, any range-extension islands, imported stub and symbol. Static source correspondence remains necessary because saved function min/end bounds may contain gaps.

The first missing high-priority implementation is specific: `_CallWithEHFrame` at engine `0x441d770` is instruction `0x16fb0ab2`, a direct B to **`0x2e0238`**. Saved Ghidra metadata identifies a 24-byte helper with min/end `0x2e0238–0x2e0250`, but `0x2.c` was not delivered. No exception/unwind behavior is inferred from the export's name. The 65-row [decompile-requests.tsv](decompile-requests.tsv) records 64 exact saved function starts plus the `_ArcErrorDomain` data export, current shard availability, file offsets, questions and app evidence. [missing-engine-bodies.tsv](missing-engine-bodies.tsv) lists all 16 imported declarations without a delivered body header.

The next useful questions are concrete:

1. Obtain `0x2e0238` and inspect the actual `_CallWithEHFrame` callback invocation/unwind boundary; obtain the remaining missing method bodies listed separately.
2. Trace registration bridge callees and destructors to specify nil removal, weak target expiration, callback cancellation, reentrancy and exactly-once completion rules.
3. Resolve the filename-selection timeout/completion bridge, DevTools message block and navigation policy values against actual bridge/native implementation instructions.
4. Establish callback execution threads at the native observer/scheduler sites and compare with the app's main-executor checks.
5. Map the observed native lifecycle, BrowserContext, WebContents and download adapter calls to the pinned stock Chromium 156 baseline using independent source/symbol/disassembly evidence. This ABI stage does not establish that mapping or measure how much source must change for newer Chromium.

Reproduction was run from `/home/exedev/work/agents/abi`:

```sh
bash scripts/regenerate.sh
flock -w 60 /home/exedev/work/heavy.lock /usr/bin/time -v python3 scripts/validate.py > initial/validate-run.log 2>&1
```

The final independent validation is **PASS** in [validation.json](validation.json): 4,314 binary engine method records, all 834 contract keys, app callback metadata, 5,353 app branches, and 1,715 engine import-call paths were checked. Engine shard hashes were checked against the supplied manifest. The stages use only Python's standard library, seek/read binary access and streamed text/hashes; the recorded largest analysis RSS is approximately 126 MiB for engine indexing, and the app scan takes roughly 30 seconds on one CPU. Heavy stages use the shared lock with bounded acquisition. Logs are `initial/*-run.log`.

[input-sha256.tsv](input-sha256.tsv) pins 111 consumed input files, including both binaries, the older/newer dumps, saved function metadata, all consumed APP source shards and all 38 delivered ENGINE shards. The pristine app SHA-256 is `4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682`; ArcCore 1.170.0 is `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd`. Mach-O UUIDs are recorded in `validation.json`. The engine's Chromium version 156.0.8078.48 is the supplied artifact identity; no stock-baseline function matching was performed here.

All app addresses use unslid `0x100...` Mach-O VAs; engine addresses use the engine's low unslid VAs. Every binary read/file offset is translated through its own `LC_SEGMENT_64` mapping. For example, engine `__LINKEDIT` VA `0xe558000` has file offset `0xe388000`; there is no global VA/file-offset equality assumption. Chained pointer format 2 in the engine stores VM addresses; format 6 in the app stores image offsets. Neither `LC_FUNCTION_STARTS` nor the failed `engine_match.py` prototype is used.

Primary schema references are pinned to Apple source revisions and mirrored under `sources/`: dyld [`fd8d0c4d52320ebf64db34f3cb280310d905c5ae`, `include/mach-o/fixup-chains.h`, lines 98–102 and 161–170](https://github.com/apple-oss-distributions/dyld/blob/fd8d0c4d52320ebf64db34f3cb280310d905c5ae/include/mach-o/fixup-chains.h#L98), and objc4 [`fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.h`, `method_t` at 914, `protocol_t` at 1516, `class_ro_t` at 1598, `category_t` at 3196](https://github.com/apple-oss-distributions/objc4/blob/fb265098298302243cd7eeaa1f63f0ba7786dd9a/runtime/objc-runtime-new.h#L914). These explain on-disk structures; they are not claims about the runtime revision used to build Arc.

Static metadata stability makes the public wrapper shapes a useful preservation target. It cannot supply a maintenance effort estimate by itself: the necessary native adaptation, ownership/callback semantics, reflected/dynamic usage and actual app behavior remain to be measured. No unmatched function or absent source path is treated as evidence of proprietary code, and no pseudocode signature, parameter name or Swift-facing type is promoted into the contract without its stated provenance.
