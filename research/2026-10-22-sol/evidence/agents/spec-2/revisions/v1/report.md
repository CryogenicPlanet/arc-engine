# spec-2 corrected research specification, revision v1

All **13 original finding data rows** are corrected in [corrections.tsv](corrections.tsv), with exactly one evidence-backed disposition per row. The revision retains **all 37 assigned class files and all 491 declared method identities**, including category, instance/class kind, type encoding, IMP, metadata record and original body location. There are **453 unique IMPs**, and all 491 declarations are instance methods. This is a corrected static research specification; it does not establish a working replacement engine.

The frozen specification is `/home/exedev/work/agents/spec-2/specs`. Its report is preserved byte-for-byte here as [original-report.md](original-report.md). The independent review remains at `/home/exedev/work/agents/spec-1/classreview`; its report, findings.tsv and all 37 class-review records were read. Original specifications, review artifacts, shared inputs and harnesses were not modified or executed to regenerate outputs.

## Corrections and evidence

| Original finding data rows | Result |
|---|---|
| 1 | ArcWebContentsMediaSession `getMediaContentTypes:` now describes a copied index-set object on success and nil after NSError conversion on failure. The engagement trap is retained, and content-type enum names remain unknown. Its object-return encoding was independently decoded from the binary. |
| 2–4 | ArcURLLoadConfiguration `headers`, ArcAnnotatedVisit `relatedSearches` and ArcContextMenuElementInfo `dictionarySuggestions` now describe their dictionary/array containers and empty cases. Element conversions are described separately. Suggestions returns the constructed mutable array without a copy. |
| 5–6 | Autocomplete initializer no longer promises source-zeroing on success. Stop describes the direct helper call separately from indirect saved-callable cleanup; upstream Stop identity and cancellation remain provisional. |
| 7 | ArcCommandBarRankingExample `candidates` is downgraded to **unresolved**. Array/candidate allocation shows conversion intent, while temporary cleanup/reuse and constructor argument mapping remain incoherent. No engine use-after-free, always-empty result or corrupt function boundary is asserted. |
| 8–9 | Content conversion proves kind/value dispatch on untyped input elements. ArcMetaTagSelector is a compatible candidate shape; concrete receiver identity requires producer/caller dataflow. Both class descriptions and method annotations now say so. |
| 10–11 | Media/thumbnail constructors now carry explicit signature/argument-propagation artifact labels. New instruction evidence narrows the local incoming-register mapping, while caller-side C++ ABI and ownership remain unverified. |
| 12–13 | AI ranking evaluation and training each record the explicit unsettled-type-propagation warning and qualify displayed arguments/aggregates. Available export status does not establish semantic correctness or a sound ABI. |

Every affected class's prose, matrix and methods.tsv annotation was updated together. [Independent recheck notes](revision-evidence/notes.md) give the observations and remaining limits for each finding. All 13 problematic original bodies were reread through `/home/exedev/work/read-function.py`; related getter/copy, stop, DevTools, logging and thumbnail bodies were also reread. In total the new bounded outputs retain **24 exact exported-body reads** and one explicitly identified negative query for an interior instruction address. No new body was truncated by the selected read limit.

New [instruction evidence](revision-evidence/instructions.txt) comes from bounded, segment-aware reads of the copied **ArcCore-1.170.0.arm64** binary, LC_UUID **4c4c44a0-5555-3144-a1b3-01c201c55814**, Chromium **156.0.8078.48**. All engine VAs in this revision are **unslid Mach-O virtual addresses in that image**, unless explicitly identified as file offsets. The installed LLVM library supplied disassembly; undecoded atomic words remain marked unrecognized. Bounded ranges are not assertions of recovered function boundaries, and no enclosing spans were summed.

## Nearby analogous descriptions and extra changes

A bounded consistency scan of all 491 copied method annotations/bodies checked for string-result descriptions hiding collection construction, BOOL wording on object-return methods, source-zeroing claims, unsettled-type warnings and weak-owner constructor argument artifacts. It reproduced the reviewed cases; toAnnotatedVisit's existing type warning was already flagged. The nearby ranking `input` getter, native aggregate getter/initializer and copy helper were inspected without extending a safe-array/lifetime claim to `candidates`. This was not an exhaustive semantic review of all methods or callees.

One additional class was changed: **ArcWebContentsDevTools**. `openProtocolConnection:error:` has a Ghidra extra-parameter artifact. Instructions inside its actual IMP save the error-out argument from x3 and forward that saved value to the NSError helper; the method annotation and matrix now record this. Rechecked notification helpers copy callable state into a queued task and later invoke it. The threading paragraph now records ownership beyond return while leaving executor identity, main-thread delivery and teardown suppression/drain unresolved. These are the extra changes beyond the 13 original findings; 14 method annotation rows across 11 classes changed overall.

Other relevant cross-report statements were independently checked and retained with their limits: the thumbnail user-data key resolves to `ThumbnailTabHelper::kDataKeyImpl`; the native logger's INITIALIZING/STARTING_LOG/LOGGING predicate differs from the pinned upstream LOGGING/STARTING_LOG/STOPPING_LOG predicate. The CEF DevTools primitives and macOS parent_view/Alloy requirement were read directly in the copied pinned headers. They support source comparisons and an embedding constraint, not native-hosting or replacement parity.

## Revised coverage

| Measure | Revised result |
|---|---:|
| Assigned/preserved classes | 37/37 |
| Declared identities/types/IMPs preserved | 491/491 |
| Unique IMPs | 453 |
| Base/category selector collisions | 0 |
| Category declarations: AI Extensions / Color Init / ranking-example Extensions | 20 / 1 / 2 |
| Successful body exports / failed / missing / truncated, per declaration | 491 / 0 / 0 / 0 |
| Available, without an explicit quality flag | 477 |
| Available with signature/type/argument artifact | 13 |
| Available with incoherent semantic pseudocode | 1 |
| Established corrupt function boundaries | 0 |
| Local verified / bounded review / partial local observation / unresolved | 151 / 139 / 200 / 1 |
| Rows retaining explicit unresolved questions | 403 |
| Fully verified replacement methods / direct app calls analyzed | 0 / 0 |

The original eight explicit signature/type flags remain, joined by the two weak-owner constructors, two AI warnings and extra DevTools argument artifact. The ranking getter's incoherent semantics is counted separately. A successful exported body remains available even when its semantics are unresolved. `body_malformed_established = 0` concerns established corrupt boundaries; it does not certify clean semantics. Per-class and aggregate counts in [coverage.json](coverage.json) are computed from the revised annotations. Static labels do not measure ABI compatibility or implementation coverage.

## Validation and preservation

[validation.json](validation.json) reports **PASS** for structural/provenance validation. A new revision-only validator independently decoded all **491 binary method records**, checked selectors/types/IMPs against the retained rows, verified all 491 class-matrix entries, preserved row order and non-annotation fields, checked the one-to-one finding ledger, recomputed coverage, reproduced the bounded read-helper/disassembly outputs and checked relative class links.

All **52 copied raw evidence/source files** match the frozen originals byte-for-byte, including the complete body snapshots, bridge evidence and cached CEF headers. All 11 primary-source hashes still agree. The original report, callbacks, next requests, source manifest and historical validation logs are unchanged. New evidence/provenance is separate under `revision-evidence/`; [provenance.json](revision-evidence/provenance.json) records hashes, paths, image identity, selected read bounds and changed annotation identities. The historical raw rendering records and `original-report.md` retain original counts and are not revised semantic conclusions.

Validation command, run from this revision directory:

```sh
/usr/bin/time -v python3 revision-evidence/validate.py > revision-evidence/validation-run.log 2> revision-evidence/validation-resource.log
```

The successful run used about **61 MiB peak RSS**, **0.22 seconds elapsed** and less than one CPU. No whole-corpus or function-TSV scan was performed; bounded random reads and the small copied evidence tree sufficed, so no heavy scan needed the shared heavy.lock. [SHA256SUMS](SHA256SUMS) is the current revision manifest, using paths relative to this directory. Original output-writing reproduction scripts were not run, rebuilt or changed.

## Scope and remaining contract gaps

Chromium comparisons retain pin **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**; CEF comparisons retain **426920c33ef22551e0b8f48cf343d623ac25642d**. Their source identity does not prove unchanged engine implementation, a matching replacement binary or a stable recovered C++ ABI. The preliminary 834 imported-class count measures declarations, not proven calls; this revision adds no app receiver/call analysis and does not infer proprietary code from unmatched functions.

Consequential gaps remain: ranking candidate contents/temporary ownership, conversion element-class producers, media content-type names, caller-side aggregate/owner passing, native DevTools view/pane ownership, service callback outputs and delivery queues, cancellation/draining, thumbnail image lifetime, native logger finalization, and complete reachable callee/delegate closure. Unknowns were recorded rather than filled with guessed behavior.

The lead, body-evidence and acceptance-evidence reviews were read and their static/runtime boundaries retained. This work performed no runtime comparison, profile migration, protected-media/extension/WebRTC acceptance test, macOS thread/teardown test, build, package installation, native app/profile change, external-machine access, credential/integration use, messaging, agent spawning, push or commit. The main engine VM remained untouched. No runtime compatibility, complete callees, main-thread delivery or teardown suppression is claimed.
