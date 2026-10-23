# AI backend decision and bounded implementation

Verified locally on 2026-10-23. This is the `ai` package's backend decision,
not a claim that AI feature acceptance has passed.

## Backend

Preserve Arc's app-supplied external-library `AIService_*` C interface behind
its Mojo service. Do not substitute TFLite or ChromeML without evidence that
they load the supplied models and reproduce the results.

`swap/hybrid-156/` is empty in this checkout. The original engine examined is
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
Its `Resources/arc/services/ai/public/mojom/ai_service.mojom-c_api.h` is identical
to the stock 1.168 header in this checkout. That interface declares:

- Embedding modes ONNX Runtime, CoreML, MLX, and remote (lines 48-53).
- Library path/version, app environment/version, retries and timeout (101-110).
- Creation/initialization and loading/ranking/evaluation/training callbacks
  (334-345, 478-507); exported C symbol names (585-626).
- `MlxError` among ranking training failures (77).

The engine contains `AIService_Create`, `AIService_Initialize`,
`ai.mojom.AIService` and `ai.mojom.AIStreamedInferenceListener` strings. The
trace app contains `AIInfraProtocols`, `MLXActor`, `LocalDistilBertClassifier`
and `LocalNomicEmbedText` strings. These establish interface capabilities,
not the identity/version of a library that actually ran. `libaperitif` evidence
describes a launch/sandbox shim, not a recovered inference implementation.

## Chromium 156 alternatives

Local source inspection found:

- `components/optimization_guide/core/inference/bert_model_executor.cc:51`
  constructs TFLite `BertNLClassifier`. No evidence establishes compatibility
  with Arc's model/config/classifier-head/LoRA file descriptors.
- `services/passage_embeddings/BUILD.gn:7` uses TFLite and SentencePiece.
  Compatibility with Arc's Nomic directory/version contract is unestablished.
- `services/on_device_model/ml/chrome_ml_holder.cc:64` loads a separate
  `optimization_guide_internal` library exposing `GetChromeMLAPIV2`.
  The local `components/optimization_guide/internal/` directory is empty;
  `components/optimization_guide/features.gni:25` explains the branding/internal
  source gate. That service is not Arc's `AIService_*` ABI.

## Implemented boundary

`arc_ai_models.mm` implements seven descriptor/configuration classes;
`arc_ai_ranking.mm` implements seven ranking value classes. They preserve
owned snapshots, nil-string conversion, float/int/double element widths,
array order, optional engagement, and documented integer narrowing. Original
engine-internal native pointer/aggregate selectors are not reproduced, per
`ARCHITECTURE.md`; ranking values are copied directly within their owned file.

`ArcCommandBarRankingExample.candidates` and
`ArcCommandBarRankingTrainingResult.delta` return fresh empty mutable arrays:
original-engine probes observe this even for four populated fixtures. Their
reviewed specs leave conversion semantics unresolved; these implementations
preserve the observed result instead of reconstructing the apparent conversion
intent. Wider semantics remain unresolved. Property declarations use `@dynamic`.

`ai_values_probe.mm` cold-loads exactly the framework path supplied by the
caller and emits sorted JSON for the implemented public value surface. It
does not initialize a browser, load a model/library, or execute AI services.
Ordinary runs exclude the two unresolved getters; `--observe-unresolved`
separately inspects them. The probe accepts only one framework per process,
avoiding duplicate ObjC class registration.

`bash arc/browser/ai_values_probe.sh` serializes the paired probe with the
shared lock, compares raw JSON (including exact method encodings/property
attributes) and runs the existing boundary recorder/differ. Generated evidence
is under `results/ai-values/`. The 2026-10-23 09:04 UTC linked-engine raw
comparison matched exactly: 14 classes, 74 public methods, 60 properties, four
value fixtures, and signed-narrowing grids. Neither probe runs a browser or AI
service; these results establish only the exercised value-transport surface.
The final locked recorder probe captured 414 calls on each engine covering all
74 public methods and found zero differences, including counts and nesting.
Raw and recorded JSON also matched, checking recorder noninterference. The
recorder loads after the framework's ObjC classes register; missing method
coverage fails the harness before a diff is produced.

## Run 1 acceptance blockers and lead requests (historical)

At the initial census, launch-slice, tabs-navigation, and windows golden
transcripts contained no calls to these classes or `createAIService:`.
An AI/command-bar scenario or explicit original-service probes are required
to establish callbacks, errors, result shapes, cancellation and scheduling.
The scenarios worker was notified on Chirp (#132892, #132918, #132952).
The later 09:11 UTC census found zero AI calls in all 16 finalized transcripts;
candidate directories were excluded.

Execution remains unimplemented: `ArcAIService`, the application
`createAIService:` category, `ArcContentConversionService`, and
`ArcWebContentsAIAgent` retain existing stubs. There is no verified external
library identity/version, model format or observed service callback contract.
Reconstructing utility-process service registration and sandbox integration
requires lead-owned Chromium/build hooks. Direct in-process library loading
would change the original isolation and is not an approved substitute.

Invalid/nil `Example` input remains unverified and currently fails a CHECK.
The configuration invalid-type error path tolerates a null error pointer;
the inspected original pseudocode dereferences it. Neither difference is
claimed to be accepted behavior for invalid callers.

The original engine's actual invalid-type error description is `Invalid type`.
This contradicts the `Invalidtype` text in `specs/ArcAIServiceConfiguration.md`,
which reflects a decompiler constant identifier rather than the literal.

For the remaining `ArcWebContentsAIAgent`, original-engine disassembly recovers
key values 2/3/4/5 as DEL/CR/ESC/TAB with key codes 51/36/53/48, default empty
string/key code zero (`0x473e230`, table `0xe21b208`). Events use zero modifiers,
no repeat, no context, current window number and system uptime. Text input
iterates UTF-16 code units with key code zero. The original performs no focus
change. Cached agent identity is explicit at `0x4441b4c`. However, the exact
frame selected through native Impl+`0x8d8` remains unmapped; choosing main or
focused frame interchangeably would invent semantics. No event implementation
or dynamic input equivalence is claimed.


## Run 2: authorized in-process external-library bridge

Lead decision on restart authorizes in-process loading. `arc_ai_service.mm`,
`arc_ai_conversion.mm` and `arc_ai_agent.mm` now implement the execution surface.
The exact trace156 generated C headers are copied byte-for-byte under
`arc/services/{ai,content_conversion}/public/mojom/`. They are included through
system-header adapters because their foreign C pointer layout cannot use
Chromium `raw_ptr`. Borrowed result buffers are immediately bounded as spans
and copied before the C callback returns. No model or ranking policy is added.

AI/control calls use the main sequence; conversion executes on workers, and
all application replies are asynchronous on main, matching recovered affinity. Before initialization,
service operations return their original synchronous NotInitialized (3) shapes;
retry-pending unavailable utility control calls may instead return -1.
availability measures transport establishment independently of library errors.
Repeated initialization returns AlreadyInitialized (4). The library loader uses
RTLD_LAZY | RTLD_LOCAL, and gates the original required symbol list. Empty path
is 5, absent path 6, load or symbol failure 7, null Create handle 0. Subsequent
operations after a failed load return 3. `createAIService:` ignores NSError**
and always creates a fresh wrapper, as the original does.

All replies use NSInteger error enums with -1 success, not NSError. Nomic
returns shape and flat float arrays separately. Rank errors return an immutable
empty array; classification/embedding/evaluation/training/conversion errors use
nil values. Streaming has independent NSString partial replies and a uint16
request token. Loaded models, candidates, training examples, embeddings and
conversion options are snapshots; result floats retain float width. Ranking
result factories write owned storage directly, avoiding extra public initializer
calls. The existing observed-empty candidates/delta getters remain unchanged.

### Original utility exceptions recovered from local disassembly

The reviewed specs do not recover these details; use the original local Mach-O
at `../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`
with bounded `lldb disassemble -s ... -e ...`. No VM or SSH was used.

- AI Initialize entry 0x4279cd4. Its C config is zeroed at 0x427a058,
  then only type, appEnvironment and fullAppVersion are populated through
  0x427a220. Display name, library path/version, retries and timeout reach
  the C library as zero/empty. The full config still determines engine loading.
  This corrects the earlier assumption that timeoutSeconds reaches the library.
- The required AI symbol vector at 0x427a228..0x427a734 contains 19 names,
  excluding GetEmbeddingsFor. Conversion requires all five header symbols.
- UpdateFeatureFlags is an engine override that always replies false
  (0x40bf000; utility vtable slot 0xda7a4e8), despite requiring its C symbol.
- Debugger waiting uses the engine helper (0x8367790), never its C symbol;
  public int seconds narrows to uint16 at 0x429a764. The helper polls at
  100ms intervals for 10*seconds attempts (0x837743c).
- Legacy GetEmbeddingsFor is a fatal utility trap (0x2076b80; slot 0xda7a4f0).
  The final in-process bridge preserves this fatal stub and never calls the
  optional C export. Its former optional-call/error2 prototype is removed.
  Fatal utility behavior now kills the browser process, the explicit isolation
  difference under the user's in-process decision; see final validation below.
- AI Crash does call its external symbol with a null C callback (0x427accc).
  Conversion utility Crash instead invokes an engine fatal helper (0x4283c70);
  no public conversion Crash selector is declared or added here.
- Conversion forwards all five configuration strings normally. Nil/empty URL
  converge to a nonnull empty String (0x4284dec..0x4284df8). A zero-length
  returned title maps to ObjC nil (0x42a8f28..0x42a8f54).
- Loader dlopen flags are 5 (0x2e8f988); native utility teardown calls dlclose
  (0x42769a8). Callback adapters post onto the utility origin sequence
  (0x427f20c/0x428549c), with Mojo returning to browser UI.
- No per-request timeout was recovered. Browser connection retries use
  1<<(retry_count+1) seconds (0x4299c58); initial connection failure sets count4,
  so first retry is32s. This IPC reconnect mechanism does not apply to a
  successfully established in-process transport and is not a model retry policy.

### Explicit isolation and lifecycle differences

Arc hosted each library service in a utility process. This bridge hosts it
in-process as authorized. A library crash therefore affects the browser, and
library global state can be shared between service instances. Modules remain
loaded for this process lifetime: the ABI exposes no Destroy/Cancel method,
so safe executable-code unloading after asynchronous work cannot be established.
Dropping a wrapper invalidates queued app replies and releases app block
captures. Input snapshots live through the C invocation, matching the utility shim;
asynchronous libraries must copy arguments they retain. Invalidation removes
request IDs, so late library callbacks are dropped without dereferencing a
retired native pointer. The ABI has no handle-destruction operation. This is separate
from feature acceptance and is not claimed to reproduce utility isolation.

### Web input target

The primary main frame mapping is now recovered: local decompile
`research/2026-10-22-sol/evidence/agents/spec-2/classreview/backend/042b66bc.c`
labels process/routing IDs read through WebContentsImpl+0x8d8 as main-frame IDs.
Agent bridges 0x4354de4/0x4354ea0 use that same field and GetView-style helper
0x10f9b60. The implementation uses GetPrimaryMainFrame()->GetView()->GetNativeView(),
not the focused frame or WebContents container view. Core's existing service
cache supplies one agent per native WebContents. UTF-16-unit NSEvent behavior
and key mappings from run1 are implemented with weak native lifetime.

### Validation state

All five production ObjC++ files pass the actual Chromium compiler flags.
A 09:35 UTC finalized-golden census found zero AI calls in21 transcripts,
including command-bar-autocomplete. The cold original-service probe verifies
fresh creation and isAvailable=false, then fails the original engine's explicit
Mojo/UI-thread checks on initialize; initialized injected probes are required.
Fixture dylib compilation succeeds with all25 declared exports. Full build,
paired service/input probes, boundary recording and scenario diffs are pending.


### Run 2 successful fixture evidence (09:50 UTC)

The original fixture needs a signed copy inside the disposable app bundle;
loading from the external build directory returned loader error7. Delaying
injection until browser readiness also avoids incomplete early startup.
The successful original run has149 events and41 external ABI rows, recovered
through fixture-only diagnostic ABI requests because utility processes strip
our logging environment variable. This proves the config fields above and
all forwarded request payloads (including candidate/example embeddings).
Initialize-again returns synchronous4. Library Initialize error1 leaves
isAvailable=true, Type(4,-1), and subsequent SetUser success. A never-initialized
SetUser returns synchronous3. After utility connection failure, retry-pending
control calls may report -1; this is distinct from a fresh service.

Library AI calls execute on utility main; conversion initialize/user also run
there, whereas external conversion runs on a worker. The in-process bridge
now uses main for AI/control calls and workers for conversion, and posts all
application completions back to main. Blocking library work can consequently
block the browser in-process, an isolation consequence.

Conversion C->Mojo result marshalling omits selectorsApplied entirely
(0x42857b8). Only the service bridge drops that field; the renderer-owned
public result constructor/getter correctly preserves explicit arrays.
Conversion requires its library callback synchronously: 0x4284d8c hands output
storage to the callback and consumes it immediately after the C call returns.
A deferred reply yields null Mojo response/disconnect, mapped to error8, and
later calls see unavailable/error3. Our token registry reproduces this failure
without allowing the original late write to retired stack storage.

The original browser AI request token is shared uint16 state (0xe3baa98),
not a streaming-only counter: SetUser, tracing, classifier load/status/inference,
Nomic load/embed, ranking load/rank/evaluate/train and streaming advance it.
Type, Initialize, debugger, Crash, flags, legacy embeddings and inference model
load do not. Unavailable browser transport skips increments; a connected
library-load failure does not. Production now uses this exact mapping.

Immediate two-partial streaming fixtures expose transport races: original
receives only the first partial, and its no-partial error9 completes as10.
Native partial forwarding is repeatable and error passthrough is literal;
there is no first-only filter or 9->10 mapping. Utility removes the listener
before the service reply, so listener-disconnect10 can win the one-shot browser
completion. The in-process bridge currently forwards both supplied partials
and the library error unchanged. This scheduling/isolation difference remains
explicitly unaccepted; a spaced-partial original probe is pending. We do not
invent a filter/remapping to fit a racing fixture.

The 14 value classes were freshly revalidated with414 calls per engine:
zero method/count/return/callback differences and exact metadata/raw values.
Run2 launch-slice built and packaged successfully; navigation/storage pass,
quit failures are external to AI. Snapshot results/ai-run2/launch-slice:
launch1 methods172, launch2 methods95, both with zero AI calls.

Cross-package blocker: ArcReference remains an app-owned stub in the packaged
ours engine, so rich conversion references are empty/zero despite correct
constructor arguments. Requests posted as Chirp134485; selectors omission
ownership corrected in134508. Main classification/Nomic/ranking/evaluation/
training result payloads otherwise match the original successful probe.
The module source disassembly is saved in ai_original_abi.disasm.txt.


### Streaming transport reconstruction (10:06 UTC; runtime pending)

The direct-delivery prototype described above is superseded by
ai_stream_transport.{h,mm}. It uses two real in-process Mojo message pipes,
as Arc does for streamed listener messages versus service completion. Each
listener notification reads one message and rearms; library completion closes
the listener sender before writing its response to the separate reply pipe.
A listener peer closure can therefore win completion10; the service reply
otherwise preserves the exact library error. The one-shot completion cancels
both receivers, naturally dropping any remaining queued listener messages.
No first-partial filter or error9->10 mapping is present.

The spaced original fixture delivered both partials, then10 despite C success;
this confirms repeatable partials and the transport race (injected-stream-only).
The helper requires an initialized browser/Mojo/UI runner, just as the original
service does; paired validation now uses injected initialized apps. Helper
syntax passes actual Chromium flags and the dependency is the public
//mojo/public/cpp/system target. A fresh build/package/injected ours run is queued.

Pending ownership now stores only callbacks, lifetime and stream context.
Input snapshots are retained by the invocation task through C return, following
the original borrowing contract. Owner invalidation erases request IDs and
releases block captures outside the registry mutex. A final helper ref released
on a library worker is deleted on UI, so receiver/watchers remain sequence-safe.

Conversion worker dispatch first passes through main, publishing earlier
initialization/handle writes before starting conversion work. After C return,
an unanswered conversion request is atomically claimed before completing8,
avoiding a check-then-remove race with a deferred foreign callback.

Latest finalized-golden census:23 transcripts, zero AI calls
(results/ai-run2/golden-coverage.json). No app-reachable AI scenario has yet
been established by the scenarios worker; direct probes are synthetic ABI
coverage, not an account-free AI feature acceptance claim.


### Native input parity and final review (10:19 UTC)

The paired initialized probe now passes26 cases/34 real native keyDown events
per engine. `results/ai-agent-probe/20261023T101521Z/comparison.json` reports
match:true and no differences. It compares actual UTF16 code units, surrogate
halves, key codes/defaults/low32 narrowing, modifiers/location/repeat/window,
renderer-view identity, cached aiAgent identity, DOM input events and values.
Both main input and focused same-origin iframe match. Nonempty text and every
key case must capture native events, preventing an empty/empty false pass.
OOPIF and IME remain uncovered. An earlier no-call startup attempt is separately
preserved; the corrected runner waits for exact app-ready PID. All owned probe
apps/watchers exited.

Final read-only review found and fixed nested-runloop reentrancy in partial
callback delivery: a local strong typed block keeps its captures alive even if
the app releases the service and owner invalidation clears stored blocks during
a nested UI loop. Completion also uses a local strong typed block. Real compiler
check passes. APP has now implemented ArcReference (#135194); fresh runtime
verification still required. Final build attempt10:18 failed foreign
arc_app_termination.mm99 protected GetServiceForBrowserContext access, reported
#135286; no own compilation error, no shared edits.


### Legacy embeddings crash/recovery clarification

Bounded static recheck of the bundled original arm64 ArcCore establishes that
legacy `embeddingsFor:method:model:completion:` does not dispatch to the external
`AIService_GetEmbeddingsFor` symbol. The utility AI vtable at `0xda7a4a8` routes
this method to `0x2076b80`, whose complete body is `brk; hlt; brk`. The required
symbol vector also omits this export. The current optional C dispatch / missing
symbol error2 fallback is therefore **unaccepted**, pending the lead decision
requested in Chirp135314; no production change was made during this recheck.

The browser request at `0x429aa84` has two distinct paths:

- Already unavailable: `0x429aad0..0x429aaf0` synchronously passes native error3,
  converted by the public embeddings adapter `0x43db45c` into `(nil, 3)`.
- Connected: `0x429ab58..0x429abac` sends a normal Mojo request through
  `0x41f6530`, holding callback state `0x42a368c`. The generated responder
  vtable `0xd9d49a8` has destructors `0x41ff8f0` / `0x41ff940` which only release
  callback state. Its state destructor `0x42a34c0` only destroys the captured
  callable; it does not invoke a default result. Legacy embeddings has no entry
  in the browser's pending callback maps. Thus the static crash path drops this
  completion, rather than returning error2 or error10. This is a static finding;
  no new runtime embeddings/crash probe was run.

Process crash and pipe disconnection are different mechanisms. The native
observer vtable `0xda7a9d8` has destructor slots followed by two no-op lifecycle
callbacks (`0x15ac0`) and crash callback `0x42a2948`. This layout corresponds to
the lifecycle observer declared in the locally copied Chromium
`content/public/browser/service_process_host.h`. The explicitly installed Mojo
disconnect callback at `0x42990ac..0x42990bc` also targets the no-op `0x15ac0`.
An ordinary pipe closure therefore does not itself establish that the following
process-crash fallback routine executes.

`0x42a2948` first compares the service interface name against
`ai.mojom.AIService`, then invokes and clears these pending maps. Offsets below
are relative to the native browser AI implementation; the table lists public
ObjC callback arguments after the existing adapters run.

| Pending operation | Map offset | Crash fallback | Handler evidence |
| --- | --- | --- | --- |
| SetUser | `+0x1d8` | `-1` | `0x42a2a50..0x42a2a70` |
| EnableDataDogTracing | `+0x200` | `-1` | `0x42a2aac..0x42a2acc` |
| LoadClassifier | `+0x228` | `-1` | `0x42a2b08..0x42a2b28` |
| IsClassifierLoaded | `+0x250` | `(NO, -1)` | `0x42a2b64..0x42a2b84` |
| Classification | `+0x278` | `(nil, 10)` | `0x42a2bc0..0x42a2c40` |
| LoadNomicEmbedText | `+0x2a0` | `-1` | `0x42a2cf0..0x42a2d10` |
| LoadCommandBarRanking | `+0x2f0` | `-1` | `0x42a2d4c..0x42a2d6c` |
| NomicEmbed | `+0x2c8` | `(nil, nil, 10)` | `0x42a2da8..0x42a2e60` |
| RankCommandBar | `+0x318` | `([], 10)` | `0x42a2f50..0x42a2fd0` |
| EvaluateCommandBarRanking | `+0x340` | `(nil, 10)` | `0x42a3040..0x42a30f8` |
| TrainCommandBarRanking | `+0x368` | `(nil, 10)` | `0x42a31a0..0x42a31e0` |

The first four maps receive encoded zero, not encoded error0: for generic error
callbacks this is native success, and for classifier status it is successful
false. Method-to-map associations are established by the native enqueue sites
(`0x4299e98`, `0x429a37c`, `0x42a0b90`, `0x42a144c`,
`0x42a1ea8`, `0x429afbc`, `0x429c408`, `0x429b848`, and the
ranking/evaluation/training map accesses after their shared-counter increments).
Streaming uses its separate listener pipes and one-shot completion; utility
death closes those peers and can supply error10. Direct Mojo callbacks for
Type, Initialize, debugger, Crash, flags, legacy embeddings and LoadInferenceModel
are outside these maps and have no fallback established by this routine.

Availability at `0x43d63a0` reads the remote disconnect bit (`+0x1a0` on the
remote's native state), so a dead utility becomes unavailable once transport
disconnect is observed. Public Initialize checks bound remote storage instead
(`0x4298894` and `0x42988fc`) and returns AlreadyInitialized4 synchronously while
that disconnected remote remains bound. The crash observer ends at
`0x42a322c -> 0x4299c58`, scheduling automatic reconnection; with a previous
successful initial connection the retry counter starts0 and the first delay is
2seconds. Reconnection `0x4298d6c` replaces the remote, sends stored configuration
Initialize at `0x4299408`, saved SetUser at `0x4299488`, saved tracing at
`0x42994d8`, and replays stored model loads beginning `0x42994dc`. It does not
permanently invalidate the ObjC owner's lifetime.

Cross-instance scope remains uncertain at the public boundary. The constructor
registers an observer through the global tracker at `0x4297f18`; the crash
handler filters the interface name but does not visibly compare a process ID.
It is therefore an inference that one AI utility crash could flush pending maps
of other registered AI implementations, even where their own remote remains
connected. This has not been verified with a multi-service runtime probe and
must not be presented as proven public behavior.

The closest in-process observable emulation would asynchronously retire the
failed connection, drop the legacy embeddings completion, apply the per-operation
crash fallbacks above, close stream peers, and retain configuration/owner state
for recovery. That is a recommendation, not an accepted implementation decision;
an intentional omission of recovery should be recorded as an isolation-related
difference rather than represented as library/model semantics.


### Final initialized test harness correction (10:25 UTC)

Fresh production build completed10:22:59 and signed packaging passed. The first
injected in-process test ran inside a main GCD block, and Observe's nested
NSRunLoop could not reenter that serial queue to execute queued library calls
or replies. Synchronous unavailable/repeated-init branches still worked, while
all queued calls timed out. This is an invalid observation host, not accepted
bridge behavior; preserved in ours156/injected-mainqueue-deadlock. The verified
own driver was interrupted with SIGINT so its finally cleaned the disposable
app; both PIDs exited.

The same probe now schedules Run from a one-shot main NSRunLoop NSTimer, outside
GCD block execution, so both engines are tested with a free main dispatch queue.
Both harness compile modes pass -Wall/-Wextra/-Werror. Probe-only paired rerun
queued, using the already-built package. Primary AI diagnostic snapshot now
precedes secondary service creation, and both runs always write captured
snapshots rather than mixing in process-global direct-file logs. Strict ordered
comparison ai_compare_service.py normalizes only opaque C transport IDs and
the per-agent conversion library path; app-visible stream tokens stay strict.


### Final parity fixes and legacy decision (10:32 UTC)

The corrected NSTimer probe produced152 boundary events per engine with strict
ordered comparison: ZERO ABI row differences and exactly one callback value
difference. Native conversion.afterAsync.setUser returned success-1
synchronously despite isAvailablefalse; the bridge returned3. The separate
atomic disconnected state now preserves this control fallback while fresh
uninitialized controls still return3. Full conversion values including the
APP-owned ArcReference match. Normal two-pipe stream matches token4, first
partial only, completion-1; error stream token17 and disconnect10 match.

The earlier optional GetEmbeddingsFor/error2 implementation is REMOVED. The
user's explicit decision requires faithful original behavior with in-process
hosting as an isolation difference. Accordingly the connected legacy endpoint
posts base::ImmediateCrash on the service queue, exactly reproducing its fatal
utility stub rather than inventing a model result or disconnect policy. It never
resolves/calls the optional C export. Unavailable still replies(nil,3). The
process boundary changes: original utility dies and browser can recover, whereas
the authorized in-process host dies; crash recovery cannot run in that process.
This supersedes the earlier pending policy request135314, posted as135475.
Strictly isolated disposable-app termination probes and a fresh final build/pair
are queued. Both production changes pass check-file.


Final10:32 recording has ZERO differences across all152 application-boundary
events. Its only ABI-log insert was Conversion.asyncCopy: the test library's
direct logger appended to fixture.jsonl after the captured snapshot was written.
The direct logger now writes fixture-direct.jsonl separately; the authoritative
captured snapshot stays immutable. No record filtering was added. The failed
comparison is preserved as results/ai-run2/service-diff-before-log-isolation.json.
A probe-only rerun is queued; no production change or extra build is needed.


### Completed validation (10:44 UTC)

Final normal synthetic pair is strict MATCH:152 boundary events and41 captured
ABI rows per engine, zero event/ABI/coverage errors. Spaced stream also passes
exact token0, two partials, completion10, main-thread/asynchronous shape.

Legacy original utility turnover is now observed, not inferred: initial69810
was alive before call, exited after it; replacement69855 appeared~2s later
while browser69325 survived. Both embeddings callbacks were dropped. Browser
availability recovered despite Config.maxRetries0; the attempted assumption
that0 disables native retries was wrong and that failed run is preserved.
The initial utility is so short-lived that the first20ms poll missed it; a250ms
pre-call run-loop window in this isolated probe captures it. Strict turnover
checks were retained. utility-processes.json records verifiedtrue with no errors.

The ours legacy probe then verified initialized call/return and owned browser
PID70225 exit with no callback. Both probes passed, confirming the intentional
in-process fatal boundary. Normal comparison, DTOs and native input remain
clean. All owned wrappers/watchers/drivers/apps exited; temporary app-bundled
fixture removed. logs/wp/ai.report.md is final, including exact scenario diff
headers, bounded reachability evidence and uncovered production-model/AI-UI
acceptance. No pending implementation or lead policy decision remains for the
authorized in-process scope.
