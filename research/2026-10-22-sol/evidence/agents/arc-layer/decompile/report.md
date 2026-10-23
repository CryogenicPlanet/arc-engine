# Arc layer: second-stage engine review

The inspected launch path requires an Arc object/delegate bridge, Chromium process and run-loop initialization, Chrome profile creation, native browser/content peers, and Arc's AppKit hosting bridge. Preserving exported ObjC names alone would leave these behaviors unspecified. This stage recovers concrete paths and several contracts, but **does not establish a complete replacement, an exclusive Arc code size, or a maintenance-effort estimate**.

Evidence is ArcCore **1.170.0 arm64 / Chromium 156.0.8078.48**, with Arc **1.168.0 arm64** app callsites. Engine addresses below are unslid low Mach-O VAs; app addresses start `0x100…`. Binary reads use segment mappings from the initial Mach-O parser. `input-pins.tsv` records hashes, including engine `69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd` and app `4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682`.

Every engine function cited below has its original **input shard path, byte offset, byte length and body hash** in `reviewed-body-index.tsv`, plus an exact local copy in `bodies/<VA>.c`. These copies preserve Ghidra output; they are not corrected C source. `claims.json` records the substantive observations, app/source references and unresolved qualifications. The validation checks provenance and literal anchors, not semantic correctness.

## Corpus and closure audit

Read `inputs/context/ENGINE.md` §§2, 5.2, 5.3 and `lead-review.md`; independently inventoried and streamed the base export and **both** completed extra batches. The base contains 38 shards, batch `60e9f287fbfb43c6985744de1f22c196` contains four, and batch `f025e06f4ec94a9fb1a918545284f407` contains 157. They are **199 distinct shards**, with no duplicate copies. Extra-batch file hashes and sizes match their immutable manifests; all file hashes are recorded in `corpus-inventory.tsv`.

The scan reproduces **567,959 headers**. The read-only index marks **79 failure placeholders**, leaving 567,880 successful body candidates. Failure presence is not automatically a timeout; exported success does not validate boundaries. All 140 initial entry requests were checked across base and extra batches: **138 have exact successful bodies**, `0x4410bb4` is the independently verified pane-creation veneer to `0x47390dc`, and `_CallWithEHFrame` at `0x441d770` has no exact exported body. Availability is recorded in `initial-request-availability.tsv`.

The handoff's `claude-handoff/scripts/arc_layer.py` has important limits:

- It keys bodies and sizes by **names**, strips trailing `:` from header names, and therefore collapses `ArcBrowserApplication::init` (`0x43e6538`) with `init:` (`0x43e6548`). Its size lookup still uses unstripped function-table names. This explains **524 zero-size rows**, rather than zero-byte implementations. The 5,846 handoff names produce **5,847 possible address mappings**, with that one ambiguity; **525 resolved mappings** have changed punctuation.
- Its ObjC seed regex misses `objc::Categories::Arc…` methods. Its call regex follows only textual `FUN_XXXXXXXX(...)` expressions for two levels. It does not follow named ObjC calls, class/category dispatch, vtables, block invocations, `LAB_…` callbacks or general function-pointer assignments. `0x42b3d74` storing `FUN_042b3f34`, and `0x43e8368` storing `LAB_043eb7a0`, are consequential examples in `0x42.c` and `0x43.c`.
- It never uses the stock baseline to stop at identified Chromium functions. Two-level closure size therefore includes upstream work and is also incomplete in other directions. It is neither a proprietary-code total nor an upper bound on all Arc implementation code.

| Reproduced measure | Result | Meaning |
|---|---:|---|
| Handoff rows | 5,846 | 1,908 seeds; 1,558 depth-one; 2,380 depth-two names |
| Handoff seed reasons | 1,378 ObjC; 524 path/IPC; 6 exports | Historical script's rules, with name loss |
| Exact exported Arc class/category bodies | 1,519 | 1,379 ordinary class bodies + 140 category bodies; discovery inventory |
| Functions containing literal `../../arc/` source paths | 510 | 105 distinct Arc paths; compiled-source references, not exclusive ownership |
| Revised VA-keyed seeds | 2,045 | Adds categories, preserves selectors, excludes `0x421f778` |
| Revised two-level direct-call syntax inventory | **5,793** | 2,045 seeds + 1,495 depth-one + 2,253 depth-two entries |
| Missing exported bodies within that revised syntax inventory | 0 | Presence only; no reachability/boundary guarantee |

The revised seed reasons before excluding the known malformed body are 1,379 class bodies, 140 category bodies, 521 additional path/IPC bodies and six exports. The 510 path-bearing functions overlap named bodies and other seed reasons; these counts must not be added as exclusive Arc code. `audited-direct-closure.tsv`, `direct-edges.tsv`, `arc-source-body-evidence.tsv` and `audit-summary.json` preserve the definitions. `noncall-function-references.tsv` contains **4,362 syntactic function-or-label references** from seeds that are not direct calls; these include ordinary local labels, not 4,362 proven callbacks.

The six binary Arc paths absent as literal source paths from exported bodies are exactly the six initial instruction gaps: feature-flags private API, histograms observer, autofill error dialog, extension install prompt, artifacts untrusted UI, and HEIF decoder. This is an export/decompilation coverage limit. It is not evidence that those paths or behavior are absent from the executable. See the instruction requests below.

No code-size sum is adopted. **1,196 resolved handoff mappings** have a different header byte count from the initial TSV extent. Some differences can reflect gaps or differing exporter metrics. The initial TSV's span sum is invalid for exclusive coverage, as already established by `initial/report.md`.

## Bootstrap and callbacks

**Application construction.** In engine `0x43.c @ 0x43e6548` (`ArcBrowserApplication::init:`), the wrapper obtains the native application through `0x42.c @ 0x42b17e8`, installs a shared/control-block-shaped peer, creates `ArcLocalState`, and builds a delegate bridge with `_objc_initWeak`. It passes the bridge through `0x43.c @ 0x4332148` to `0x42.c @ 0x42b7974`. The app's getter at `0x1004aa890`, `app168/ghidra/decomp/0x1004.c`, sends `init:` at **0x1004aa8c8**; this exact binary call was independently decoded in `initial/app168-launch-calls.tsv`. This supports a weak ObjC delegate bridge plus reference-counted native lifetime pattern. It does not establish all destruction ordering or cancellation rules.

**Configuration and initialization.** App function `0x1004ad314`, old app `0x1004.c`, constructs the configuration and sends `initialize:` at **0x1004ad77c**. Engine `0x43.c @ 0x43e6fc8` calls the configuration converter `0x43ec404` and native initializer `0x42.c @ 0x42b0850`. The converter explicitly reads `argc`, `argv`, `profilePath`, `applicationID`, `isProduction`, `sessionRestorationPolicy`, `shouldLoadAllProfilesAtStartup`, `quitRunLoopOnTermination` and `userDefaultsSuiteName`; application ID and session-restoration values other than 0/1 enter trap paths. The initializer checks native `+0x98`: an existing runner produces `0xffffffff`; otherwise it constructs configuration/runner state and enters `0x42aa7ec`.

`0x42.c @ 0x42aa7ec` builds an Arc-specific delegate object through `0x429608c`, creates content-runner state and calls `0x2d.c @ 0x2d2f3cc`. The latter has a strong source-backed identity as **`content::RunContentProcess`**, including one-time initialization, locale/signal setup, runner initialization/reinitialization and final run dispatch. Compare pinned Chromium `content/app/content_main.cc:189` at revision **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. This identifies a dependency and likely function identity; it does not prove unchanged source or byte/ABI equivalence. The exported `_ChromeMain` at `0x0.c @ 0x41c8` also reaches `0x42aa7ec`. This stage does not establish an app `main` call to that export; its directly evidenced app route is the ObjC initialization/run interface.

**Run and shutdown.** App `0x1004ae240` sends `run` at **0x1004ae254**. Engine `0x43.c @ 0x43e6ec0` → `0x42.c @ 0x42b1ba8` → callback installation `0x42aaaa0` → `0x42aac34`, whose explicit location is `RunMainLoop`, `../../arc/browser/arc_browser_runner.cc:241`. When that returns, `0x42b1ba8` calls `0x42ab360`; this logs browser-runner shutdown and calls `0x31.c @ 0x318b9bc`. Its initialization/shutdown checks, pre-shutdown/main-loop cleanup and shutdown flag strongly identify **`content::BrowserMainRunnerImpl::Shutdown`**, pinned source `content/browser/browser_main_runner_impl.cc:158`.

**Did-initialize notification.** Engine `0x42.c @ 0x42b1200` guards native `+0x149`, sets it, and invokes `0x42b13c4` with `arc::BrowserApplicationDelegate::DidInitializeBrowserProcess`. `0x42b13c4` contains `InvokeAsync` and `../../arc/common/utils/delegate_wrapper.h`, and submits a task through an indirect runner. Installing a delegate through `0x42b7974` after `+0x149` is set also schedules that notification. Separately, ObjC bridge `0x43.c @ 0x43eb374` sends `didInitializeBrowserProcess`; app callback `0x1004b73fc` forwards to `0x1004b7350` in old app `0x1004.c`. The complete vtable/task path and queue identity remain unresolved. Startup callback target **`LAB_042c18f0`**, stored in `0x42b1ba8`, has no own exported body. These bodies support asynchronous notification and late-registration handling, not an exact globally-once callback guarantee.

The app's observer-registration sends remain separately established by the initial seven-call launch sample. This stage does not infer that optional observers can be removed safely.

## Profiles, browser peers and content

**Profile creation is a Chrome service path.** App `0x1004af380`, old app `0x1004.c`, copies a block, sends `createBrowserContext:isHidden:type:completion:error:` at **0x1004af488**, releases its copy, and branches on the immediate return; its failure branch converts NSError to Swift error. Engine `0x43.c @ 0x43e8368` validates type 0/1, stores completion capture target `LAB_043eb7a0`, and calls `0x42.c @ 0x42b3d74`. That native method rejects an empty profile name and null callback with explicit error strings, then stores completion function **0x42b3f34** and calls **0x7b.c @ 0x7bdc470** in extra batch `f025…`.

`0x7bdc470` strongly identifies **`ProfileManager::CreateMultiProfileAsync`**: matching function/path location, generated unused profile path loop, profile-attribute setup, hidden-state pair and posted disk work. Pinned source `chrome/browser/profiles/profile_manager.cc:1037` explicitly requires the UI thread, sets ephemeral/omitted flags from `is_hidden`, and posts work before asynchronous profile creation. This is source evidence for the identified upstream operation; it does not independently prove the thread on which Arc's wrapper was called.

Native completion `0x42.c @ 0x42b3f34` handles null profile with `failed to create profile on disk`, otherwise gets/tracks a context through `0x42b4cc4` and chooses an ordinary or alternative context branch based on type. It invokes the stored native callback through an indirect slot. The ObjC conversion at **0x43eb7a0 is missing**, so exact completion signature, callback cardinality, error-object mapping and retention/cancellation behavior remain open. Ghidra's retain/release injections are not a sufficient ownership proof.

**Native services precede cached ObjC service wrappers.** `0x42.c @ 0x42b4cc4` calls `0x42c5400` on its context-creation path. In `0x42c5400`, the native service at `+0xe0` is constructed and later initialized via `0x42df0b8`; `+0xf0` via `0x42e4000`; `+0x100` via `0x42e8260`. Those field identities are independently anchored by ObjC getters: engine `0x44.c @ 0x44378a0` builds `ArcBrowserContextDownloads` from `+0xe0`, `0x4437a90` builds `ArcBrowserContextEffects` from `+0xf0`, and category `0x43.c @ 0x43f598c` builds `ArcBrowserContextExtensions` from `+0x100`. All three cache the ObjC result with associated objects. The getters are lazy wrappers; native construction and initialization already occurs in the traced context path. `0x42c5400` also contains a conditional migration involving `arc.dnt_disabled_migration_done` and `enable_do_not_track`. This is a maintenance surface to inspect, not proof of unchanged upstream behavior or safe service deferral. Only the context-creation app call is proved here; no specific app send to these three getters is asserted.

**Browser and content creation.** App `0x1004ad0a0`, old app `0x1004.c`, sends `createBrowser:withDelegate:forWindow:` at **0x1004ad1a4** and **0x1004ad1c8** in alternative branches. Engine `0x43.c @ 0x43e7048` extracts the native context, calls `0x42.c @ 0x42b32d0` → constructor `0x42f8300`, wraps the peer in `ArcBrowser`, and calls `setDelegate:`. Full native-window and browser ownership contracts remain outside this bounded trace.

There are two distinct content paths: `ArcBrowser::createWebContents:` at `0x43.c @ 0x43e154c` → `0x42.c @ 0x42faa84`, and context category `createWebContents:` at `0x43.c @ 0x43f35bc` → `0x42.c @ 0x42d7064`. Both convert configuration and call `0x2d.c @ 0x2dba704`, strongly identified by its unique trace and behavior as **`WebContentsImpl::CreateWithOpener`**, pinned `content/browser/web_contents/web_contents_impl.cc:1628`. The browser route additionally calls **0x3870000** before wrapping the resulting content; the context route goes through `0x43.c @ 0x4346528`. They must not be treated as interchangeable solely because they return `ArcWebContents`. This stage does not name the extra navigation operation or prove an app call to either creation selector.

The thin peer initializers `0x44.c @ 0x443739c` and `0x4440c4c` copy native peer/control-block fields and increment reference counts; content initialization additionally calls `0x440e174`. These observations support the need to recover native peer lifetime and delegate installation, not a stable C++ ABI across Chromium releases.

## Native-view contract

The old app's `WebContentChromium.webView` flow in `app168/ghidra/decomp/0x1005.c` has four verified sends:

| App function | Exact send | Selector | Engine evidence |
|---|---|---|---|
| `0x10054267c` | `0x100542690` BL | `nativeView` | `0x44.c @ 0x4410bb8` |
| `0x1005426a0` | `0x1005426b4` BL | `createPaneHostView` | initial veneer `0x4410bb4` → `0x47.c @ 0x47390dc` |
| `0x1005426c4` | `0x1005426d4` tail B | `attachToPaneHostView:` | `0x44.c @ 0x4410cd0` |
| `0x1005426d8` | `0x1005426e8` tail B | `detachFromPaneHostView:` | `0x44.c @ 0x4410ec0` |

`app168-essential-sends.tsv` includes instruction words, selector-reference VAs and mapped file offsets. Selector stubs prove sends, while the saved app receiver flow supplies the class association; runtime receiver identity is not proved.

Creation at `0x47.c @ 0x47390dc` allocates **`WebContentsPaneHostView` with NSZeroRect**. `nativeView` and attachment refresh native host state through `0x43.c @ 0x4376580` and use native peer field `+0x288`. Attachment calls `0x47.c @ 0x4739114`; detach calls `0x47391c8`. These helpers use **`isMemberOfClass:`** for `WebContentsPaneHostView`, then send `attachWebContentsHostView:` / `detachWebContentsHostView`. Thus the observed adapter expects the exact pane-host class, including its class identity behavior.

Actual pane attachment at **`0x47.c @ 0x4737eb4`** detaches/replaces prior hosted content, handles the same-view case with state synchronization, obtains screen scale/refresh/display identity, updates an indirect bridge, adds the hosted view and four edge constraints, handles first-responder state and calls `syncHostedWebContentsState`. Detach at **`0x4738a30`** tears down bridge state, removes the view when its superview is that pane, and clears the attached-view field. Null pane arguments are ignored by the outer Arc methods.

A replacement route therefore needs a compatible pane class, attach/switch/detach semantics, layout, display/focus and bridge lifetime behavior. This closes the earlier question of whether these methods were trivial generic view accessors. It **does not decide CEF versus a Chromium fork**: adapting another engine to this contract remains a separate feasibility/runtime gate. Exact compositor, coalesced-resize, GPU, focus and destruction behavior is not fully recovered here.

## IPC evidence and boundaries

Four names now have body-level plumbing evidence, beyond embedded strings:

- **`0x43.c @ 0x43755a4`** builds a `RenderProcessAgent` pipe/remote, calls `0x28.c @ 0x28bbd60` with its name/interface payload, sets up `RenderFrameAgent` state and calls generated-looking helper `0x44.c @ 0x442ad7c`. The meaning of `0x28bbd60` as a precise Chromium method is still a naming question.
- **`0x43.c @ 0x43a809c`** packages `arc.mojom.EffectsAgent` and `arc.mojom.AdBlockerAgent` names and captured callbacks through `0x18.c @ 0x18aade4`, consistent with interface registration. Its binder targets `LAB_043a8a50` and `LAB_043a482c` have no own exported bodies.
- **`0x43.c @ 0x43aa39c` / `0x43aaea8`** construct receiver plumbing for `RenderFrameAgent` / `RenderProcessAgent` through `0x9.c @ 0x92d8c0`, with dispatch/validation labels **`LAB_04429bcc` / `LAB_0442ad24`**. Those labels also lack own exported bodies.

This supports real browser/receiver and binder machinery for the named interfaces. No exact schema, method ordinal table, version negotiation, startup necessity, or renderer callback ownership is established. No direct app caller to these C++ routines is claimed. `unresolved-callees.tsv` separates present-but-unnamed routines from missing-entry recovery work.

## Malformed regions and scope limits

The known **`FUN_0421f778`** is excluded entirely from the revised closure. Initial metadata gives a **24,347,032-byte span**, while its current `0x42.c` header reports 10,600 bytes and text is 48,632 bytes. Those are incompatible size bases; the explicit handoff exclusion remains in force.

`merged-metadata-regions.tsv` independently flags **ten metadata extents above 1 MiB**, with counts of other saved entries inside each span. Examples beyond `0x421f778`: `FUN_002b7940` spans 143,288,116 bytes / 371,256 interior saved entries, but current extra-batch `0x2.c` header reports 21,900 bytes; `FUN_005f90d0` spans 73,827,996 bytes but its `0x5.c` header reports 916. These spans are not exclusive functions or code volumes. The initial whole-table overlap count reproduces **579,666 starts within a prior maximum span**; overlap alone does not prove every pseudocode body is wrong.

`malformed-body-review.tsv` separately catches **six extreme text/header expansion cases**. Four report a four-byte body: **`0x1f4191c`, `0x392d43c`, `0x483613c`, `0x49a56bc`**, all in extra batch `f025…`, shards `0x1f.c`, `0x39.c`, `0x48.c`, `0x49.c`. The engine instruction at each entry is **`0xd503201f` (NOP)**; the next saved entry is `VA+4`, while their pseudocode is 85–349 KB and contains labels beyond the four-byte extent. They cannot be read as isolated four-byte implementations. Two more, `0x3ff8420` and `0x3ffdcc0` in extra `0x3f.c`, report 48/44 bytes but have 785,012/776,750 bytes of pseudocode; they remain expansion/boundary review candidates, with no accepted semantics. None of these six lies in either the supplied handoff mappings or revised direct-call inventory. The heuristic does not certify the other bodies as valid.

This stage uses **67 bounded exact body copies**, four targeted Chromium identity comparisons and eight new app B/BL sends. Together with the initial seven launch sends that is a 15-send sample, not an app-wide call census. **834 methods on 63 imported classes remains a declaration count** from the intact initial evidence. Returned objects, categories and callbacks expand the interface. The initial 146-class / 1,964-method metadata inventory is preserved; it is a different set from exported named bodies.

No stock binary match, changed-file proof, complete callback ownership proof, native execution, macOS smoke test, CEF runtime comparison, helper executable analysis or `libaperitif.dylib` semantics is claimed. A source identity does not establish unchanged behavior; missing source paths and unmatched functions do not establish proprietary code. More disassembly and runtime contracts are needed before estimating Chromium-upgrade maintenance.

## Reproduction and next requests

Exact completed validation run, from `/home/exedev/work/agents/arc-layer`:

```sh
flock /home/exedev/work/heavy.lock bash decompile/tools/reproduce.sh
sha256sum -c initial/artifacts.sha256 > decompile/initial-integrity.log
```

**PASS**: 199 shard inventory; 567,959 headers; 79 indexed failures; all 5,846 handoff names accounted for with their ambiguity preserved; 67 exact body copies; 11 claim groups / 87 literal-anchor checks; eight app binary sends; four pinned-source identity comparisons; 138/140 exact initial entry bodies. The serial tools used one CPU; maximum measured RSS was **161,020 KiB** (diagnostics), with the corpus audit at **125,860 KiB**. Per-script resource logs preserve the measurements. Initial artifact checks all pass. Tools, caches and outputs stay under this role's directory; no input modifications or external actions were taken.

`additional-shards.tsv` is a **13-row function-boundary/disassembly request list**, not a request to transfer more complete shards:

| Target VAs | Required recovery | Dependent conclusion |
|---|---|---|
| `0x43eb7a0` | Completion adapter entry referenced by context-create wrapper | ObjC callback arguments, NSError mapping, retention and delivery |
| `0x42c18f0` | Startup callback entry referenced by native run | Initialization notification ordering and task ownership |
| `0x4429bcc`, `0x442ad24` | Generated dispatch/validation entry bodies | Frame/process IPC schemas, ordinals and compatibility |
| `0x43a8a50`, `0x43a482c` | Effects/ad-blocking binder entries | Receiver construction, target lifetime and registration behavior |
| `0x41d8c74`, `0x432a3d4`, `0x43acfa0`, `0x43b4c70`, `0x43ce074`, `0x8368628` | These are **instruction VAs**, not verified entries; recover exact containing body ranges | Remaining six binary source-path gaps; no complete subsystem coverage claim |
| `0x441d770` | Exported `CallWithEHFrame` entry, no exact exported body | Imported C exception/unwind contract; no new semantic conclusion here |

The next bounded work should recover the context completion and startup callback entries first, then source-name present callees **`0x3870000`** (browser content path), **`0x28bbd60` / `0x18aade4` / `0x92d8c0`** (IPC glue), and **`0x473cf0c` / `0x473a110`** (native-host bridge). The macOS validation gate should exercise profile success/failure, late delegate registration, attach/switch/detach across windows and focus/display transitions. These questions directly constrain interface preservation and engine-route feasibility; a larger closure size cannot answer them.
