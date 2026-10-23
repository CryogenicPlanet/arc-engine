Reviewed all **34 assigned class specs and 491 declaration rows** in `../spec-1/specs/classes/`. Four method descriptions require correction before publication. Thirty classes remain acceptable as bounded, incomplete research specifications; this is not a behavioral compatibility certification. The writer's files were not edited.

The findings are in [findings.tsv](findings.tsv), with exact original-corpus byte offsets, body hashes and required corrections. [class-review.tsv](class-review.tsv) contains one individual verdict, evidence selection and limits for every assigned class.

| Finding | Class / selector | Required correction |
|---|---|---|
| F01, medium | `ArcWebContents.web_contents`, `0x4443ee0` | Empty optional returns empty; engaged optional copies the native peer/control pair and increments the nonnull strong count. The displayed trap checks null aggregate return storage inside the engaged branch. The current “traps when backing optional is empty” claim is false. |
| F02, medium | `ArcContentNode.textColor`, `0x4430bdc` | This produces an `NSString`, with nil for zero native color and `#%08X` formatting otherwise; it is not a direct native-field return. Empty outer optional still traps. Instructions independently recover the omitted variadic color argument. |
| F03, medium | `ArcAdhocHistogramSamples..cxx_destruct`, `0x4166068` | Clears/releases the stored samples object at self+8 through injected `objc_storeStrong`. It accepts no supplied replacement value. |
| F04, medium | `ArcHistogramSample..cxx_destruct`, `0x425b6d8` | Clears/releases the stored NSNumber at self+0x10. The generic supplied-value setter description is incorrect. |

The two destructor errors are confined to method descriptions; their class lifecycle prose already describes clearing the fields. Correct the matrix and `methods.tsv` together. All four rows are currently labeled `local_verified`, so those labels cannot validate their existing wording.

Coverage was reproduced independently from the pristine **ArcCore 1.170.0 arm64 Mach-O**, raw `dobjc170.txt`, the read-only corpus index and the saved function TSV. No writer Python modules were imported. The parser checks chained pointer format, translates VAs through segment mappings, parses complete base/metaclass/category method lists and preserves `(class, category, kind, selector)` identity. It compares each binary encoding, IMP and method-record VA with the writer row; it also checks every class's declared ivar/property records and object layout against the binary. Every Markdown method matrix has exactly the corresponding TSV identity once.

| Check | Independently reproduced |
|---|---:|
| Assigned/present classes | 34 / 34 |
| Declaration rows / unique full identities | 491 / 491 |
| Base / category rows | 457 / 34 |
| Instance / class methods | 487 / 4 |
| Distinct IMPs | 436 |
| Rows with indexed body / distinct indexed IMPs | 490 / 435 |
| Failed method bodies | 0 |
| Missing indexed method body | 1, verified B veneer |
| Writer semantic labels | 318 local, 156 partial, 16 unresolved, 1 veneer |

[method-checks.tsv](evidence/method-checks.tsv) records all declarations, original shard byte ranges, body names, hashes and warnings. All 435 distinct indexed method extracts equal the writer's extracts byte-for-byte. [citation-checks.tsv](evidence/citation-checks.tsv) resolves 634 distinct method/bridge code VAs: 633 indexed bodies and the verified veneer `0x4410bb4→0x47390dc`. No extracted body was truncated; the largest is 32,594 C-text bytes. These are text lengths, not native code coverage. Small object offsets, metadata records, the `0x8000000` decode-budget constant and ARM64 instruction words are not counted as code VAs. Class/ivar/property/method metadata VAs are checked separately; the cited setting table at `0xc9d8188` is readable binary data, with bytes `00 01 02 03 04 00` recorded in [data-checks.json](evidence/data-checks.json). No enclosing TSV spans were summed or treated as body-byte maps.

The source comparisons use Chromium **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c** (156.0.8078.48) and CEF **426920c33ef22551e0b8f48cf343d623ac25642d**. [source-checks.tsv](evidence/source-checks.tsv) records exact URLs, local paths and SHA-256 hashes for 11 primary files. All eight files separately cached by the writer match these independently read/fetched copies. The other three are present in the pinned input subset. Relevant checked locations are:

- Chromium `services/media_session/public/cpp/media_position.cc:43` and `.h:63,98`: advancing/clamped media position and independent end-of-media flag. The engine's recovered helper signature is inconsistent, so the named function correspondence remains an inference.
- Chromium `components/security_state/core/security_state.h:39`, `components/site_engagement/content/site_engagement_service.h:69,121,169`, `chrome/browser/ui/toolbar/toolbar_actions_model.h:124` and `.cc:688,764`, and `components/sessions/core/serialized_navigation_entry.h:85`: enum/service/session comparisons support the bounded hypotheses. They do not prove those unnamed engine callees' identities or imply Arc enforces Chromium's documented UI-thread requirement.
- Chromium `content/public/browser/web_contents.h:1241` and `content/browser/web_contents/web_contents_view_mac.mm:138`: native-view APIs exist; they do not establish the extra Arc pane-host contract.
- CEF `include/internal/cef_types_mac.h:151`: providing `parent_view` forces Alloy style. `include/cef_browser.h:512,546` documents StartDownload and PrintToPDF, respectively, without proving Arc profile/delegate or PDF-query equivalence.

Consequential wrapper/bridge claims were reviewed per class, including download versus extension nil-delegate behavior; extraction empty-result paths; shared/weak ownership; permission base/category dictionary implementations; native host class checks and attach/detach; close reposting; image/JavaScript inline callbacks; PDF immediate-status versus callback results; raw size buffers; and weak dialog content lookup. The preserved `ArcPermissionRequest.additionalData` and `ArcWebContents.setUserActivityDelegate:` category collisions are correctly kept separate. No selector-only app receiver identification or unqualified Chrome symbol match was accepted.

The **three Arc 1.168.0 app branch samples** were independently decoded through selector stubs/selrefs using the app's own segment mappings and chained-pointer format. They resolve create/attach/detach pane-host selector sends at `0x1005426b4`, `0x1005426d4`, `0x1005426e8`; nearby older app pseudocode in `inputs/app168/ghidra/decomp/0x1005.c:53029,53041,53054` supports the `ADK.WebContentChromium.webView` property flow. It does not prove a runtime receiver class. These samples are not a census of app-used methods; neither 491 nor the historical 834 is a direct-call count.

Review depth is explicit in [manual-review.json](evidence/manual-review.json) and each class row. Every class's prose and method claims were reviewed, and every declaration/body identity was mechanically checked. Semantic body review concentrated on consequential methods and bridge callees. Trivial accessors were sampled, not exhaustively proven: the instruction check covers all 20 HSLA float getters and the repaired histogram byte getter; shared field getters, constructors, cache getters and setters were sampled from their actual bodies. The large WebContents initializer, related delegate setters and native host routines also received targeted excerpts; this is not a complete transitive call-graph audit. The 633 available citation bodies were extracted/validated, not all semantically certified.

[decompiler-limits.tsv](evidence/decompiler-limits.tsv) explicitly flags 12 recovered-body limitations. The lost scalar returns and false HSLA aggregate conversions were independently repaired with bounded instruction checks. Candidate initializer argument shuffling, extraction's string-plus-object pointers, the media-position return type, Downloads/PDF/delegate argument recovery and indirect cleanup remain unresolved. Some limitations are already well labeled by the writer; they are not reasons to invent missing contracts. Borrowed `sizes` and inline mutable pointers have no independent ownership guarantee: lifetime, invalidation by native storage replacement and cross-toolchain aggregate ABI still require caller/producer evidence. The MediaImage buffer is explicitly freed at dealloc, and its native setter does not rebuild it in the visible wrapper. Callback captures and shared-count increments likewise do not establish delivery sequence, one-shot behavior or safety during every teardown interleaving.

The exact completed validation commands, run from `/home/exedev/work/agents/spec-4`, were:

```sh
/usr/bin/time -v python3 classreview/tools/audit.py > classreview/evidence/audit-run.log 2> classreview/evidence/audit-resource.log
python3 classreview/tools/sources.py > classreview/evidence/source-run.json
/usr/bin/time -v python3 classreview/tools/verify_binary.py > classreview/evidence/binary-run.log 2> classreview/evidence/binary-resource.log
python3 classreview/tools/limits.py
python3 classreview/tools/finish.py > classreview/evidence/finish-run.json
```

All completed successfully. The initial inventory audit was also run under `flock /home/exedev/work/heavy.lock`; these final bounded checks take less than one second each. Final inventory audit: **22,788 KiB peak RSS, 0.59 s elapsed, one CPU**. Constrained binary verification: **24 engine checks and three old-app branches passed**. Scripts use the standard library and bounded reads. This review did no builds, product implementation, remote-machine access, dynamic macOS execution, broad stock-binary matching or proprietary-code volume/maintenance-effort estimation. `quality/initial/report.md`, `lead-review.md`, `ENGINE.md` and the bounded handoff were read as context; their closure/byte-sum/history claims were not used as semantic proof.

The next specific questions are: (1) resolve F01–F04 in the writer's published artifacts; (2) obtain coherent instructions/caller ABI evidence for the flagged argument/return recovery before treating those prototypes as implementable; (3) trace callback consumers and teardown for extraction/PDF/downloads/JavaScript; and (4) test the native pane-host lifecycle plus required Chrome services before choosing an engine route. No missing engine shard blocks this review, and no claim here establishes the complete changes needed for a current-Chromium replacement.
