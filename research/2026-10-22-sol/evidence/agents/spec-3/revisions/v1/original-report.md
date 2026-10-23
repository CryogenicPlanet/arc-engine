# spec-3 research specification

The assigned declared inventory reconciles exactly: **37 classes, 491 method identities, 433 distinct IMPs** in ArcCore 1.170.0 arm64. All 491 rows have exact indexed body candidates; none is an export failure or missing declared IMP. This is declared-interface coverage, not 491 app calls or full behavioral compatibility.

Deliverables: [37 class specifications](classes/), [methods.tsv](methods.tsv), [coverage.json](coverage.json), and [machine-readable evidence](evidence/). Each class has state/layout, ownership/lifecycle, callback/threading limits, caller/callee and Chrome/CEF route notes, plus a method matrix retaining category, +/- identity, full encoding, IMP, behavior, backend, evidence and status. No class outside spec-3's assignment was specified. This stage contains research specifications and small analysis tools, with no product implementation or runtime replacement validation.

## Inventory and evidence

| Check | Result |
|---|---:|
| Assigned classes / generated specs | 37 / 37 |
| Declared identities / method rows | 491 / 491 |
| Instance / class methods | 488 / 3 |
| Base / category methods | 488 / 3 |
| Distinct method IMPs | 433 |
| Shared-IMP groups within assignment | 24 |
| Base/category same-selector collisions | 0 |
| Declared IMP body available / failed / missing | 491 / 0 / 0 |
| Additional backend snapshot candidates / missing exact entries | 70 / 4 |
| Erased-return rows recovered from instruction leaves | 4 |
| Independently checked app 1.168 BL instructions | 4 |

[prepare.py](../tools/prepare.py) re-decodes class/metaclass/category method records, relative selectors, type strings and IMPs directly from the pristine engine Mach-O using the read-only parser in ../arc-layer/scripts/inventory.py. Segment mappings translate file offsets; equality of some offsets/VAs is incidental. It compares identity and IMP with raw dyld and compares types with the initial binary inventory. [check_raw_dump.py](../tools/check_raw_dump.py) independently compares class/category/kind/IMP/type multisets with raw otool, retaining 37 class-layout anchors and all 491 method-row anchors. It never pairs names/types by row order. Exact metadata record VAs are in [metadata.json](evidence/metadata.json).

Categories are ArcBrowserContextDevTools(Extensions), adding -isEqual: at 0x43f8f28 and -hash at 0x43f91fc, and ArcColor4f(Init), adding -initWithR:g:b:a: at 0x4401b90. No same-selector base override occurs in this assignment. ArcImage owns all three class methods. Shared IMPs retain separate receiver identities and independently decoded layouts; Ghidra's surviving display names are not receiver identities.

[body-index.json](evidence/body-index.json) and body snapshots retain exact corpus file/byte offset/length and failure status. [ghidra-imp-rows.tsv](evidence/ghidra-imp-rows.tsv) checks all 433 starts against saved metadata, preserving reported bounds without treating them as contiguous coverage. No body sizes or min/end ranges were summed into code volume, effort or proprietary-code claims. No selected snapshot is truncated. Export presence does not establish correct semantics.

## Consequential findings

- **Application lifecycle/profile services:** ArcBrowserApplication obtains native shared application/local state and weak delegate bridge (0x43e6548). initialize: converts config and calls native 0x42b0850 (0x43e6fc8). run reaches main-loop/shutdown orchestration (0x43e6ec0 → 0x42b1ba8); termination retains completion and handles repeated requests (0x43e6ee0 → 0x42b2430). Profile creation has immediate status/error plus retained completion, with empty-name/null-callback rejection (0x43e8368 → 0x42b3d74). Profile deletion durability and restore callback ordering remain open (0x43e8b58, 0x43ea810). Full native startup/config closure was not traced.
- **Browser and embedding:** ArcBrowser creates content through configuration and browser-layer bridge (0x43e154c → 0x42faa84), associates platform-window state (0x43e14d4 → 0x42fa9fc → 0x43c8b18), and separately takes incoming content (0x43e1734 → 0x437b014). setDelegate explicitly initializes weak self/delegate references (0x43e1284). A decoded vtable prefix links created/restored-content and extension-popup/action callbacks (0x43e33c4 through 0x43e370c); this is not the full delegate inventory.
- **Navigation object/observer contract:** short load supplies concrete defaults (0x4417484); full load validates URL/origin in the bridge and returns ArcNavigation or nil (0x441749c → 0x4366ac0). The bridge has distinct Chrome browser-tab and standalone-content paths. Observer addition deduplicates and creates weak captures; removal unregisters/erases (0x4416eac, 0x4417210). Provisional callback wraps ArcNavigation and sends webContents:didStartProvisionalNavigation: (0x44193c0). Four later vtable targets lack exact indexed bodies: these are separate backend gaps, not missing declared IMPs.
- **Native input:** ArcWebContentsAIAgent reaches per-NSString-code-unit macOS NSEvent synthesis and keyDown: (0x4440ba0 → 0x4354de4 → 0x473dff4); key enum similarly synthesizes an event (0x4440c34 → 0x4354ea0 → 0x473e20c). IME, surrogate pairs, focus, modifiers/keyCode and invocation sequence remain unverified. These bodies do not establish a CDP or renderer-Mojo contract.
- **Persistence/ownership:** SessionTab checks index bounds and caches/mutates navigation transport (0x443ccf8, 0x443cde8, 0x443d0c8); wrapper setters do not prove disk durability. Browser cookies/restore use distinct native services (0x43e18cc, 0x43e1f6c, 0x43e2b9c). Download state shifts native 0..3 to exposed 1..4, with 0 for no live item (0x442fc60); remaining-time failure returns -1 (0x442fcd0). Freezing token init moves ownership and clears source (0x443eb10); clearing/destruction differ (0x443eb94, 0x443ebb0).
- **Callbacks needing deeper lifecycle work:** dump-provider registration/cleanup and completion adaptation (0x44077b4 → 0x432b4b8, 0x4407a04, 0x4407b5c); image-decode callback/limit transport (0x4404e6c → 0x4338920 → 0x4337e00); notification response and native callbacks (0x440a1f8, 0x440a0fc, 0x440a114, 0x440a1e4); cancellation-checked DevTools dialog actions (0x4402950, 0x440296c, 0x4402988). No exactly-once, callback queue or post-destruction safety guarantee follows from the signatures.

Primary comparisons are pinned to Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c (156.0.8078.48) and CEF 426920c33ef22551e0b8f48cf343d623ac25642d. Hashes, locations and line windows are in [sources.json](evidence/sources.json) and [source excerpts](evidence/pinned-source-excerpts.txt). Plausible navigation interfaces appear in content/public/browser/navigation_controller.h:565-592,647-692 and reload/transition enum headers. Similar source-backed comparisons cover download controls, SessionTab fields, MenuModel hooks and memory-provider registration. Epoch conversion identification uses base/time/time.h:537-546 plus literal offsets. Resemblance does not name an anonymous binary function conclusively; the specs label source-backed inferences.

CEF supplies navigation and browser creation (include/cef_browser.h:82-118,301), but its macOS parent_view is an NSView and forces Alloy (include/internal/cef_types_mac.h:110-153). This is a source-backed compatibility constraint. Matching Arc native embedding, navigation identity/callback ordering, profile services, extensions/omnibox and session persistence remains a route gate. No absent-file or unmatched-function argument proves an API is unavailable or code is proprietary. This stage has no matching arm64 CEF binary or successful replacement validation.

## Decompiler and semantic limits

Ghidra prints a void return for 0x40b1230, used by three BOOL getters, and for ArcColor4f.r at 0x43dee04. [inspect_leaves.py](../tools/inspect_leaves.py) reads pristine instruction words at mapped file offsets and accepts only whitelisted load-plus-RET or RET forms. It recovers ldrb w0,[x0,#8]; ret and ldr s0,[x0,#8]; ret. Nine declared lifecycle/getter rows received this bounded instruction check. [arm64-leaves.json](evidence/arm64-leaves.json) retains words, decoded form, receiver identity and encoding.

Ghidra also mis-types accept/close/pause and several aggregate returns, including 0x4402950, 0x443b434, 0x4437324, 0x443b214 and 0x443ba90. Explicit decompiler-limit flags preserve these issues. Selected backend tracing stops at bounded anonymous callees or indirect dispatch; no giant merged function substitutes for an exact IMP. Failed/missing/malformed limits remain separate from available-body counts.

The handoff TSV has 5,846 rows with columns name, call depth, seed reason and reported size, confirmed from inputs/claude-handoff/scripts/arc_layer.py. The second column is depth 0/1/2, not an address. The producer strips trailing selector colons, keys bodies/sizes by name, and follows only direct FUN_ calls through two levels. It can lose aliases/indirect-callable targets and misassociate sizes. [handoff-audit.json](evidence/handoff-audit.json) records producer/input hashes and depth counts. This stage obtains addresses from Mach-O and the exact corpus index. The known 0x421f778 merged region is not selected or used.

There are **79 behavior-unresolved rows** and **383 rows with open questions**, including partially understood transport/lifecycle methods. This local count of 79 is unrelated to the complete corpus's 79 export-failure placeholders. Full backend semantic verification and runtime verification are both zero. Detailed evidence-level counts are in coverage.json. Literal/instruction verification means local field effects only; conversion_partial means an observed conversion, not a complete getter contract. No callback queue or global thread-safety guarantee was established. Metadata ownership markers remain declarations; observed weak initialization, block capture and retain/release operations have separate function anchors.

App 1.168 evidence is four independently checked BL instructions; receiver association remains older-app pseudocode inference. This does not establish that all assigned declarations or the preliminary 834-method imported-class surface are called. Stock Chromium was not mechanically matched, older engine bodies were not analyzed and libaperitif was outside scope. No machines, jobs, credentials, integrations or shared evidence were changed.

## Reproduction and validation

From /home/exedev/work/agents/spec-3 run `flock /home/exedev/work/heavy.lock bash tools/reproduce.sh`.

The reproducer serializes local extraction, reads immutable inputs, reuses cached public CEF files and invokes the exact indexed read-function.py helper. It writes only beneath this role directory. Primary CEF URLs/commit are in the source manifest; cached hashes are checked locally. The exact final validator is `python3 tools/validate.py`, within the same locked reproducer. [validation.json](evidence/validation.json) and [validation-run.log](evidence/validation-run.log) record checks and resource bounds. The locked reproducer passed on 2026-10-23 at 05:33:18 UTC; the validation record supplies the precise timestamp. It checked 503 byte-exact snapshots (433 IMPs plus 70 additional backend candidates), 11 pinned source hashes and four missing backend targets. The cached run used about 29 MiB maximum RSS and 99% of one CPU; initial snapshots are read through bounded indexed accesses. Snapshot availability does not count as semantic review. Checks cover inventory, raw dumps, byte-exact corpus snapshots, declared identities/types/IMPs, 37 class files/matrix counts, metadata bounds, source hashes and four observer gaps. They are artifact checks, not behavior tests of a replacement.

## Next specific questions

1. Recover callback selectors/payloads at **0x44195b4, 0x44197a8, 0x441999c, 0x4419b90**, reached from observer vtable 0xda882d0. [address-requests.json](evidence/address-requests.json) requests bounded arm64/branch-target resolution in expected shard 0x44.c. Do these include completion/failure events, and can they arrive after removeObserver?
2. Resolve 0x43c8b18/0x47359ec platform association and 0x42faa84 embedding/ownership against real app view contracts. Which NSView/NSWindow/pane survives tab transfer, and can Alloy hosting preserve it with required Chrome services?
3. Check deeper native initialization/deletion/restoration helpers reached from 0x43e6fc8, 0x43e8b58 and 0x43ea810: status enums, disk durability and callback ordering remain open.
4. Trace cancellation/completion lifetime and invocation sequences for image decoder 0x4337e00, dump callback 0x4407b5c, notification parser 0x473f138 and browser-close/autocomplete callbacks 0x42fa5c8/0x42fbd80.
5. Validate arm64 aggregate argument/return conventions and enum meanings for Color4f, ErrorStatus, dialog/infobar acceptance and PiP size. Preserve metadata receiver/category/kind identity when resolving aliases.
