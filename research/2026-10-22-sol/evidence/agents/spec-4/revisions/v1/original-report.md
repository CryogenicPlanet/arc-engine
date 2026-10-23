# spec-4: assigned ArcCore class specifications

The assignment is complete as a research specification: **38 class files and 491 declared-method rows**, preserving `(class, category, kind, selector)` identity. The 491 rows resolve to **418 unique IMPs**. All have exported bodies. This is declaration and evidence coverage, **not 491 proven app calls or verified implementations**.

The image is **ArcCore 1.170.0 arm64 / Chromium 156.0.8078.48**. Chromium source is pinned to `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`; the SPAKE2 source hypothesis uses its DEPS-pinned BoringSSL revision `2ea3200c3a3ec176613ab7a0b9a4ffc6db846120`. No product implementation, builds, agents, machine access, integrations or external sends were used.

## Deliverables

- [classes/](classes/): exactly the 38 assigned classes, each with state/layout, ownership/lifecycle, callbacks/threading, relations, route implications and a complete category-aware method matrix.
- [methods.tsv](methods.tsv): all 491 methods, exact encodings/IMPs, body status, bounded behavior, callees, metadata/body anchors, confidence and remaining questions.
- [coverage.json](coverage.json): exact assignment reconciliation, body and semantic-status counts, per-class counts and category overrides.
- [evidence/validation.json](evidence/validation.json), [inventory-validation.json](evidence/inventory-validation.json), [otool-validation.json](evidence/otool-validation.json): independent inventory/provenance checks.
- [evidence/layout.json](evidence/layout.json), [declared-methods.tsv](evidence/declared-methods.tsv), [otool-anchors.tsv](evidence/otool-anchors.tsv): raw metadata identities, ivar records, mapped file offsets and dump lines.
- [bodies/](bodies/), [body-index.json](evidence/body-index.json), [bridges.json](evidence/bridges.json), [body-range-validation.tsv](evidence/body-range-validation.tsv): 418 IMP bodies plus 87 explicitly selected bridge/callback bodies, byte-checked against the immutable index. None is failed or truncated.
- [evidence/category-overrides.json](evidence/category-overrides.json), [shared-imps.json](evidence/shared-imps.json), [accessor-instruction-checks.tsv](evidence/accessor-instruction-checks.tsv): override/alias and narrow ARM64 load/return evidence.
- [evidence/primary-sources.tsv](evidence/primary-sources.tsv): 20 primary source files with revision, location and SHA-256. Two missing mirrored headers were fetched at exact revisions into [sources/](sources/).
- [scripts/](scripts/): stdlib-only inventory extraction, specification rendering, targeted body reads, closure crosscheck and validation. Curated analysis lives in [manual-method-review.tsv](evidence/manual-method-review.tsv) and reproducible [class_notes.py](scripts/class_notes.py).

## Consequential findings

1. **Category implementations change the contract.** `ArcBrowserContext` base `id`, `metrics`, `extensions` read ivars (`0x4084dc0`, `0x2d0fb40`, `0x2cb140`); Extensions `0x43f541c`, `0x43f5648`, `0x43f598c` construct/cache native-backed results. `ArcMenuItemModel` also has three category overrides: accelerator presence, accelerator and icon (`0x44080f8`, `0x44081f0`, `0x4408318`). Six distinct override identities remain separate in every output. Category loading precedence is not dynamically tested.

2. **Profile service identity and shutdown are central.** `ArcBrowserContext` lazily caches preferences/history/downloads/effects/extraction (`0x44374c0`, `0x44376b0`, `0x44378a0`, `0x4437a90`, `0x4437c80`) and exposes native profile-backed bookmarks, content settings, cookies, storage, import and WebContents construction. Owner accessors/destructors of preferences/history/effects use explicit weak operations (`0x43fe200`, `0x43fe218`, `0x44191d8`, `0x443616c`, `0x4435e2c`). These are ownership facts, not inferred from signatures. Native service detachment and pending callback cancellation remain open. Chrome Profile is a source-backed analogue (`chrome/browser/profiles/profile.h:68`, pinned Chromium revision above), not a proved global function match.

3. **Native hosting cannot be reduced to obtaining a view pointer.** Side-panel close is split between request and app completion (`0x4401170`, `0x44011a4`); callbacks include show/title/animated close/unhide (`0x4401984`, `0x4401a10`, `0x4401aa0`, `0x4401b04`). PiP returns a native held view (`0x441c31c`) and integer-derived CGRect (`0x441c37c`), but reparenting ownership, coordinate space, scale and required thread remain unknown. These are explicit route gates for CEF or Chrome embedding.

4. **BoostIDE is a WebUI bridge.** Success/error responses pass through `0x4414488` → `0x43d35c8` → `0x68122a8` and `0x44145b0` → `0x43d38cc` → `0x6812404`, producing `cr.webUIResponse` with true/false. Events `0x44146d8` → `0x8371aa8` use `cr.webUIListenerCallback`. Chromium `content/public/browser/web_ui_message_handler.h:76`, `:85`, `:91` corroborates resolve/reject/listener meaning. Callback targets into app are `0x4413fd8`, `0x441402c`, `0x4414080`. JS-allowed state, JSON failure handling and delegate slot ownership are unresolved.

5. **Persistence and policy are real service questions.** Preferences contain exact upstream keys; modifiability differs from enabled state (`0x4435694` versus `0x44356d8`, `0x443581c` versus `0x4435860`). Notification enable getter tests native setting **not equal to 2** (`0x443578c`), while setter writes 3/2 (`0x44357cc`). Local-state backend reports detached, missing and wrong-type errors (`0x4325ac8`, `0x4327790`); numeric browser-policy access maps to preference lookup (`0x4327f3c`). Relevant pinned source anchors are in the preferences/local-state class specs and primary-source manifest. Generic request-context preference APIs have not been shown equivalent.

6. **Tracing contains Arc collection policy.** `0x440cda8` mutex-protects an active-session read. Start bridge `0x434a700` configures systrace/categories/histograms and custom collection tasks identified by Arc tracing_controller_impl.cc markers. Stop/abort differ (`0x434b768`, `0x434b818`) and callbacks have separate started/completed/aborted adapters (`0x440d248`, `0x440d29c`, `0x440d328`). A getter lock establishes neither general thread safety nor a callback queue.

7. **Cryptographic adapter can be specified separately.** `ArcSpake2Context` uses 32-byte message and 64-byte key buffers through `0x4341c4c`/`0x4341d34`, with deeper calls `0x44614ac`/`0x44617b4`. Matching signatures, role normalization, BoringSSL path evidence and DEPS pin support SPAKE2 identification. Pinned `sources/boringssl/curve25519.h:157`, `:183`, `:195`, `:210` specifies one exchange/generation and processing order. This is corroborated source inference, not cryptographic or interoperability validation.

## Coverage and correctness limits

| Evidence level | Method rows |
|---|---:|
| Declared identities reconciled | 491 |
| Exported body available | 491 |
| Export failed / missing / truncated | 0 / 0 / 0 |
| Local field observations | 115 |
| Other local body observations | 169 |
| Curated partial bridge/behavior review | 206 |
| Specifically unresolved lifetime recovery | 1 |
| Full runtime semantic equivalence established | 0 |

All 491 rows still have an explicit remaining validation question. The 206 bridge reviews verify a bounded local account, not the whole native service. Availability and a successful export never certify Ghidra boundaries or semantics.

Eight rows have specific correctness status beyond ordinary availability:

- Four identities share `0x40b1230`: Ghidra emits an empty void body, but raw bytes `00 20 40 39 c0 03 5f d6` are `ldrb w0,[x0,#8]; ret`. Binary metadata and narrow instruction decoding recover the field load. For `ArcCpuInfo`, the `armImplementer` selector has unsigned-byte encoding and reads `armPartNumber_` at +8; `armPartNumber` has a different native field route. Do not infer fields from selector names.
- `0x44376b0` has a libedit History/int prototype for an ObjC history object getter; `0x443be7c` truncates its ObjC receiver to int in pseudocode; `0x4406140` misrecovers the NSError pointer. Metadata encodings are retained and these recovered prototypes are explicitly malformed.
- `ArcCommandBarRankingTrainingResult(Extensions) -delta`, `0x4402480`, places cleanup `0x42a43ac` before vector reads in pseudocode. The cleanup body visibly frees the vector. Tensor conversion via `0x44022d8` is visible, but lifetime/order is deliberately unresolved; no use-after-free claim is made about the binary.

Saved function-table sizes and export-header reported sizes disagree for **16 assigned unique IMPs**; see [imp-saved-function-metadata.tsv](evidence/imp-saved-function-metadata.tsv). That is a diagnostic disagreement, not proof of 16 corrupt functions. Neither end bounds nor body-byte counts are used as contiguous coverage maps, proprietary volume or effort estimates. Indirect-tail dispatch and removed-block warnings are preserved in methods.tsv.

The handoff closure has 5,846 rows; its second column is depth, not an address. Of 418 indexed IMP names, 196 occur there; of 87 selected bridge names, 52 occur there. [closure-crosscheck.tsv](evidence/closure-crosscheck.tsv) records each comparison. Aliased Ghidra names and bounded closure explain why presence/absence cannot establish ownership, completeness or proprietary code. Exact method identity comes from binary metadata. This stage does not claim a full closure extraction audit or analysis of the entire engine corpus.

Declared methods are not app-called counts. The existing Arc 1.168 app configuration initializer call at `0x1004ad718` was independently checked as a BL to selector stub `0x1035beda0`, using app segment mapping ([direct-app-call.json](evidence/direct-app-call.json)). Its receiver association remains old app-pseudocode inference. No count such as “834 called methods” is used. Full 1.168 engine semantics, helper binaries, libaperitif behavior, live macOS behavior and CEF API coverage are outside this assignment.

## Validation and reproduction

Ran successfully from `/home/exedev/work/agents/spec-4`:

```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash specs/scripts/reproduce.sh > specs/evidence/validation-run.log 2> specs/evidence/validation-resource.log
```

This checks direct Mach-O metadata against the initial inventory and raw dyld identity set, validates all raw otool IMP/type pairs without positional pairing, validates all saved body bytes against indexed ranges, checks exact assigned class/method/matrix membership, hashes primary sources and checks source citation locations, and rechecks the one app BL. The successful run used about 26 MiB maximum RSS and one CPU, with no builds or package installs. Logs: [validation-run.log](evidence/validation-run.log), [validation-resource.log](evidence/validation-resource.log).

`collect.py` and `bridges.py` use the same bounded SQLite byte-range reads as `/home/exedev/work/read-function.py`; the reproduction also invokes that helper explicitly for the uncertain delta and cleanup bodies. Re-fetching the two pinned headers, if required, is separate: `bash specs/scripts/fetch_sources.sh`. Normal reproduction uses the saved copies.

## Next specific questions

1. Resolve `0x4402480` cleanup/vector ordering with coherent ARM64 disassembly, and recover actual prototypes/return registers for `0x44376b0`, `0x443be7c`, `0x4406140`, plus native trace start `0x434a700`.
2. Resolve exact native panel/PiP view ownership, coordinate space and source destruction; exercise close animation and cached contents reattachment before choosing embedding route.
3. Trace profile/local-state teardown and callback registrar queues; verify callback cancellation, weak delegate clearing, load-state transitions, errors and reentrancy.
4. Recover the remaining browser-context delegate targets `0x43f695c` and `0x43f6ae4`, absent from the exact body index when inspecting the bridge vtable. Request coherent disassembly/entry recovery around those addresses rather than a larger decompile shard.
5. Map actual app receivers/selectors and callback protocols to these declared methods, then compare pinned CEF capabilities service by service. Present route decisions only after this native hosting and persistence evidence is resolved.
