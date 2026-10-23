# spec-1: Arc class research specifications

Completed the assigned **34 class specifications and 491 declaration rows** for ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48. Every row preserves class, category, class/instance kind, selector, encoding and IMP. This stage specifies local behavior and compatibility questions; it does not implement a replacement or choose an engine route.

## Inventory and coverage

Independent Mach-O metadata parsing reproduces the assigned inventory and initial Arc inventory exactly. Raw dyld tuples match by explicit identity/IMP. Raw otool encodings and IMPs are verified using the binary method-list address plus its entry offset, avoiding class-wide order pairing and nested metaclass/protocol confusion. Evidence: `evidence/metadata.tsv`, `otool-reconciliation.tsv`, `layout.json`, `reconciliation.json`.

| Measure | Count |
|---|---:|
| Assigned/generated classes | 34 / 34 |
| Declared/unique full method identities | 491 / 491 |
| Base/category declaration rows | 457 / 34 |
| Class (`+`)/instance (`-`) rows | 4 / 487 |
| Distinct implementation VAs | 436 |
| Rows with indexed body / unique indexed IMPs | 490 / 435 |
| Failed body rows / missing indexed body rows | 0 / 1 |
| Local-operation verified / veneer verified rows | 318 / 1 |
| Reviewed partial / semantically unresolved rows | 156 / 16 |
| Rows retaining an explicit open question | 395 |
| Fully end-to-end verified method semantics | 0 |

`local_verified` only means the stated local operation is supported by the exact body or a constrained instruction check. It does not certify all callees, callback ownership, aggregate ABI or runtime parity. `reviewed_partial` records useful behavior with consequential open questions. `unresolved` is deliberately unknown. Availability is separate from semantic verification. Three indexed bodies have explicitly identified decompiler-output repairs: two lost returns and one false aggregate conversion; these do not count as exported decompile failures.

## Consequential findings

1. **Native hosting is an app-used contract.** ArcCore category `ArcWebContents.createPaneHostView` at `0x4410bb4` is a branch veneer to `0x47390dc`, which allocates `WebContentsPaneHostView` with NSZeroRect. `nativeView` (`0x4410bb8`) ensures/returns a native host; attach/detach (`0x4410cd0`, `0x4410ec0`) reach helpers `0x4739114`/`0x47391c8`, which accept only exact host-class membership. Host attach `0x4737eb4` and sync `0x4738bb0` handle hosted subviews, constraints, screen/scale/refresh and visibility. These are specific compatibility requirements, not just a returned NSView pointer.

2. **Two base/category collisions must remain independent.** Permission `additionalData`: base `0x2d0fb40` returns self+8, whereas Extensions `0x440a85c` converts a native optional numeric-key/string-value map into a copied dictionary. WebContents `setUserActivityDelegate:`: base `0x43fe218` stores weak property state, whereas Extensions `0x441066c` manages observer/native registration. Runtime override selection has not been dynamically validated. Equal selectors are insufficient identities.

3. **Ownership and delivery are more than ObjC signatures.** WebContents delegate bridge setters (`0x4440dec`, `0x444112c`, `0x444146c`, `0x44417ac`) capture weak self/delegate behind shared native bridges. Context extension registration (`0x43fcbd8`) explicitly clears bridge storage on nil; Downloads category setter (`0x4041fec`) does not show the same unregister-on-nil behavior. Many method encodings expose C++ shared_ptr/optional/string/vector aggregates directly. Metadata declarations and refcount operations support local ownership observations, but toolchain ABI and in-flight callbacks need validation.

4. **Callbacks can fail inline.** Image decode (`0x4405930`→`0x4337e00`) invokes an empty result inline for invalid/empty input or nonpositive scale; valid bytes are copied before posting a decode task, with 128 MiB default limit when zero is supplied. WebContents JavaScript (`0x4442eb8`/`0x444302c`→`0x4377d70`) also has inline empty-result paths. Neither signature establishes a universal asynchronous or main-thread callback guarantee.

5. **PDF control depends on an internal bridge.** Query wrappers `0x443e0b4`..`0x443e614` have immediate bool/error outcomes separate from query callbacks. Native helpers `0x436c99c`, `0x436cb2c`, `0x436ccbc`, `0x436cec4`, `0x436d054` distinguish detached contents, missing glue and missing PDF controller, with Arc source-path anchors. Page/bookmark/text schemas and cancellation remain unresolved.

6. **Decompiler text cannot be implemented literally.** `ArcHistogramSample.browserIsVisible` (`0x40b1230`) and `ArcHSLAMatrix.m0` (`0x43dee04`) have empty-return pseudocode despite proven LDRB/LDR-S + RET instructions. HSLA aggregate getter `0x43dee9c` is independently verified to copy 80 bytes unchanged using SIMD loads/stores; apparent numeric casts are artifacts. Extraction bridge `0x42f0254` has suspect string-plus-object pointer expressions; its detailed layout remains unresolved.

7. **Some APIs are only data carriers.** URLLoadCancellationToken (`0x440d37c`, `0xef938c`, `0x440d41c`) owns a string and declares no cancellation operation. SiteSettingInfo setters (`0x440bb94`, `0x440bc8c`, `0x440bcf8`) mutate a descriptor; profile persistence requires an external consumer. Ranking objects carry features/vectors/results; these wrappers do not establish a ranking algorithm.

## Direct old-app evidence, separately from declarations

`evidence/app168-embedding-calls.tsv` verifies these actual branches in pristine **Arc 1.168.0 app**, using decoded selector stubs and segment-based VA/file-offset translation:

| App instruction VA | Kind | Selector stub VA | Selector |
|---|---|---|---|
| `0x1005426b4` | BL | `0x1035b6c00` | createPaneHostView |
| `0x1005426d4` | B tail | `0x1035b1f40` | attachToPaneHostView: |
| `0x1005426e8` | B tail | `0x1035b8800` | detachFromPaneHostView: |

Exact old app bodies at `0x1005426a0`, `0x1005426c4`, `0x1005426d8` are preserved in `evidence/app168-embedding-pseudocode.txt`; input `app168/ghidra/decomp/0x1005.c:53029,53041,53054` associates the receiver with `ADK.WebContentChromium.webView`. This proves selector sends plus receiver-property flow, not runtime receiver class. It is a three-selector sample, not a census of 491 app-called methods. The historical 834 figure remains a declared surface on imported classes.

## Chrome/CEF route implications and pinned sources

Chromium source pin: **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c** (156.0.8078.48). CEF source pin: **426920c33ef22551e0b8f48cf343d623ac25642d**. Copied/fetched primary files are read as evidence, not instructions. Fetched-file URL/hash records are under `sources/*manifest.json`.

| Contract | Primary-source location | Status |
|---|---|---|
| Native host | Chromium `content/public/browser/web_contents.h:1241`; `content/browser/web_contents/web_contents_view_mac.mm:138` | Source APIs expose native view; they do not establish Arc pane/compositor behavior (`0x4737eb4`, `0x4738bb0`). |
| CEF native embedding | CEF `include/internal/cef_types_mac.h:110,151` | `parent_view` forces Alloy style. This is a documented route constraint, not a successful Arc host implementation. |
| Downloads | CEF `include/cef_browser.h:512` | StartDownload is a candidate operation; does not prove profile-scoped Arc Downloads delegate/error parity (`0x40422b0`, `0x43328cc`, `0x836ba70`). |
| PDF | CEF `include/cef_browser.h:546` | PrintToPDF output does not establish recovered bookmark/text/page-query contract (`0x436c99c` etc.). |
| Media time | Chromium `services/media_session/public/cpp/media_position.cc:43`; `.h:63,98` | Source-backed inference for advancing/clamped position helper `0x32b5dd0`, consistent with native metadata and wrapper offsets; exact symbol identity still unproven. |
| Security enum | Chromium `components/security_state/core/security_state.h:39` | Source-backed inference for native enum vocabulary; Arc `0x440ab4c` maps 0→0, 3→1, others→2. No full `0x3c2b700` identity claim. |
| Engagement | Chromium `components/site_engagement/content/site_engagement_service.h:69,121,169` | Source-backed service candidate for `0x42efb64`; source UI-thread requirement is conditional on identification, not evidence Arc checks threads. |
| Extension pin order | Chromium `chrome/browser/ui/toolbar/toolbar_actions_model.h:124`; `.cc:688,764` | Candidate Chrome model behavior for `0x42eb484` / `0x42eb57c`; exact mapping/persistence remains partial. |
| Session entries | Chromium `components/sessions/core/serialized_navigation_entry.h:85,88,101` | Source-backed route analogy; does not establish native Arc Impl or persistence format compatibility (`0x443cadc`, URL getters). |

Native hosting combined with extension/profile services and PDF extraction remains an unresolved route gate. No missing file in the copied source subset is used as API-absence evidence. No unmatched function is called proprietary, and no source-path absence is taken as no-change proof.

## Method/class coverage

| Class | Declared rows | Local verified | Partial | Unresolved | Veneer |
|---|---:|---:|---:|---:|---:|
| [ArcAddressEntrySaveRequest](classes/ArcAddressEntrySaveRequest.md) | 6 | 4 | 2 | 0 | 0 |
| [ArcAdhocHistogramSample](classes/ArcAdhocHistogramSample.md) | 5 | 5 | 0 | 0 | 0 |
| [ArcAdhocHistogramSamples](classes/ArcAdhocHistogramSamples.md) | 3 | 3 | 0 | 0 | 0 |
| [ArcBackForwardListItem](classes/ArcBackForwardListItem.md) | 8 | 7 | 1 | 0 | 0 |
| [ArcBookmarkImportTargets](classes/ArcBookmarkImportTargets.md) | 6 | 6 | 0 | 0 | 0 |
| [ArcBrowserContextDownloads](classes/ArcBrowserContextDownloads.md) | 13 | 9 | 4 | 0 | 0 |
| [ArcBrowserContextExtensions](classes/ArcBrowserContextExtensions.md) | 25 | 10 | 15 | 0 | 0 |
| [ArcBrowserContextPageContentExtraction](classes/ArcBrowserContextPageContentExtraction.md) | 10 | 7 | 3 | 0 | 0 |
| [ArcCommandBarRankingCandidate](classes/ArcCommandBarRankingCandidate.md) | 19 | 16 | 1 | 2 | 0 |
| [ArcCommandBarRankingInput](classes/ArcCommandBarRankingInput.md) | 7 | 4 | 1 | 2 | 0 |
| [ArcCommandBarRankingResult](classes/ArcCommandBarRankingResult.md) | 8 | 6 | 0 | 2 | 0 |
| [ArcContentNode](classes/ArcContentNode.md) | 17 | 15 | 0 | 2 | 0 |
| [ArcExtensionRuntimeError](classes/ArcExtensionRuntimeError.md) | 11 | 8 | 1 | 2 | 0 |
| [ArcExtractedPageContent](classes/ArcExtractedPageContent.md) | 8 | 6 | 0 | 2 | 0 |
| [ArcFindResult](classes/ArcFindResult.md) | 3 | 3 | 0 | 0 | 0 |
| [ArcFormControlInfo](classes/ArcFormControlInfo.md) | 12 | 10 | 0 | 2 | 0 |
| [ArcHSLAMatrix](classes/ArcHSLAMatrix.md) | 23 | 23 | 0 | 0 | 0 |
| [ArcHistogramSample](classes/ArcHistogramSample.md) | 6 | 6 | 0 | 0 | 0 |
| [ArcHungRendererDialog](classes/ArcHungRendererDialog.md) | 7 | 5 | 2 | 0 | 0 |
| [ArcImageRep](classes/ArcImageRep.md) | 17 | 3 | 14 | 0 | 0 |
| [ArcMediaImage](classes/ArcMediaImage.md) | 9 | 8 | 1 | 0 | 0 |
| [ArcMediaPosition](classes/ArcMediaPosition.md) | 8 | 4 | 4 | 0 | 0 |
| [ArcMetrics](classes/ArcMetrics.md) | 5 | 4 | 1 | 0 | 0 |
| [ArcNavigation](classes/ArcNavigation.md) | 10 | 10 | 0 | 0 | 0 |
| [ArcPermissionRequest](classes/ArcPermissionRequest.md) | 8 | 8 | 0 | 0 | 0 |
| [ArcSecureContext](classes/ArcSecureContext.md) | 6 | 6 | 0 | 0 | 0 |
| [ArcSessionNavigation](classes/ArcSessionNavigation.md) | 12 | 7 | 5 | 0 | 0 |
| [ArcSiteSettingInfo](classes/ArcSiteSettingInfo.md) | 13 | 6 | 7 | 0 | 0 |
| [ArcTracingConfiguration](classes/ArcTracingConfiguration.md) | 11 | 11 | 0 | 0 | 0 |
| [ArcURLLoadCancellationToken](classes/ArcURLLoadCancellationToken.md) | 5 | 5 | 0 | 0 | 0 |
| [ArcVisitCategory](classes/ArcVisitCategory.md) | 8 | 6 | 0 | 2 | 0 |
| [ArcWebContents](classes/ArcWebContents.md) | 163 | 79 | 83 | 0 | 1 |
| [ArcWebContentsPDFController](classes/ArcWebContentsPDFController.md) | 9 | 4 | 5 | 0 | 0 |
| [ArcWebDialog](classes/ArcWebDialog.md) | 10 | 4 | 6 | 0 | 0 |

## Reproduction and validation

From `/home/exedev/work/agents/spec-1`:

```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash scripts/reproduce.sh > specs/validation-run.log 2> specs/validation-resource.log
```

Validation checks exact assignment/identity reconciliation, all raw otool type/IMP records, indexed bytes/checksums, three syntactic ARM64 repairs, three old-app branch/stub decodes, and specification/TSV row coverage. It does not verify behavioral equivalence. `evidence/validation.json` reports PASS. Reproduction is offline, standard-library Python plus the provided indexed reader; no build, decompiler job or product implementation. Sources are fetched only by optional `scripts/fetch_sources.py`; offline reproduction validates saved source hashes.

All 435 extracted unique method bodies are at most 6,578 pseudocode bytes; the extraction cap is 65,536. Significant bridge reads use the provided reader cap of 32,768 and no selected bridge was truncated. `evidence/body-index.tsv` records original shard/offset/length and extract hash; `call-edges.tsv` records 545 syntactic direct-call/function-pointer edges. Calls are observations, not full semantic attribution.

The historical handoff contains 5846 rows. Exact-name cross-check finds 239 of the 435 local IMP-body names in it; metadata aliases and differently scoped names are retained independently. This is a bounded starting index, not an exhaustive or proprietary-code map. No giant merged-function bounds are used or summed. Known giant `0x421f778` is not followed.

## Deliverables and limits

- `classes/*.md`: exactly the 34 assigned class specs, full layout/property anchors, lifecycle/threading/route notes and method matrices.
- `methods.tsv`: 491 rows with required fields plus semantic status, backend callees and metadata anchors.
- `coverage.json`: declaration/body/semantic counts, per-class reconciliation and base/category collisions.
- `evidence/`: exact IMP extracts/index records, raw metadata reconciliation, bounded bridge excerpts, instruction checks, app-call sample and remaining unknown rows.
- `scripts/`: copied bounded metadata parser, extraction/generation/audit tools, reviewed annotations and reproducible shell entry point.
- `sources/`: pinned fetched primary files and hashes; optional failed fetches are explicitly recorded and not used as absence evidence.

No live macOS execution, ABI conformance test, full app-call census, exhaustive callback ownership proof, renderer/Mojo schema recovery, full native class layout reconstruction, or independent second-agent review was performed. The main decompile was not contacted or altered. libaperitif/helper executables and older engine bodies were not analyzed. No code-volume or maintenance-effort estimate is derived from Ghidra spans.

## Next specific questions

1. WebContents/pane: audit `0x4737eb4`, `0x4738bb0`, `0x473c4dc` and `0x437854c` for host ownership, compositor transfer, focus and close-retry termination; validate in an app host on macOS.
2. Delegate bridges: recover vtable targets installed at `0x4440dec`, `0x444112c`, `0x444146c`, `0x44417ac`, `0x4041fec`, `0x43fcbd8`; trace each decision/completion ownership and nil-unregister rule.
3. JavaScript/extraction: resolve execution world integer 4 and result schema in `0x4377d70`→`0x66776dc`; repair suspect extraction expressions at `0x42f0254` and follow successful continuation `0x42f102c`/`0x42f1760` into renderer/schema production.
4. PDF: follow `0x41a7920` and `0x41a8a58` for page/bookmark/text callback schemas, index units, cancellation and render-process teardown.
5. Profile services/persistence: identify `0x42efb64` keyed service, `0x42eb57c` pin update consumer, SiteSettingInfo commit caller, and ArcSessionNavigation native producer/serialized format.
6. ABI/data: validate every C++ aggregate used by the preserved app against replacement compiler/runtime settings; recover media sizes count and cached-buffer mutation behavior (`0x4407384`, `0x4407618`). The 16 explicitly unknown method rows in `evidence/unresolved.json` are a bounded follow-up queue.
