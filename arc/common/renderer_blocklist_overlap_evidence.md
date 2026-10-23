# Native SetBlockLists overlap ordering

Read-only bounded audit of the pristine ArcCore 1.170 arm64 image on
2026-10-23. Addresses are unslid. No runtime overlap experiment, application
launch, build, or production change was performed. A subsequent parent-authorized
private candidate and syntax check are described at the end.

## Result

The recovered native public SetBlockLists path serializes compilation on one
singleton pool sequence. Its compiler task and UI completion do not carry a
request generation or WeakPtr. The manager replaces its stored compiled pair
on every completion. For requests accepted and posted in order A then B,
native therefore compiles A, posts A's UI completion, then compiles B and posts
B's UI completion. This is not a parallel compiler in which a slower A can
complete after B and overwrite it.

Later URL-ruleset notifications do use an invalidated weak flag and can suppress
an earlier queued notification. That token belongs to the profile service's
notification stage, after the singleton manager has already accepted a result;
it does not cancel an older compilation or its manager completion.

## Singleton sequence proof

The backend at `0x42bd840` checks feature 11 at `0x42bd858-0x42bd860`, dropping
disabled calls. Every enabled call obtains the same static runner pointer at
`0x42bd878`. Initialization is protected by the guard at
`0x42bd918-0x42bd92c`; it constructs a 32-byte runner through `0x107d840`
at `0x42bd958` and stores it in that same static slot at `0x42bd97c`.

The runner constructor is independently identifiable as
`base::internal::PooledSequencedTaskRunner`:

| Native evidence | Corresponding Chromium 156 implementation |
| --- | --- |
| `0x107d88c` stores its pool delegate at runner+16 | `pooled_task_runner_delegate_` |
| `0x107d8d4-0x107d8f4` allocates a Sequence and invokes its TaskSource constructor with execution mode **1** | `MakeRefCounted<Sequence>(..., TaskSourceExecutionMode::kSequenced, ...)` |
| `0x107d92c` records the runner backpointer in that Sequence | constructor's `this` argument |
| `0x107d964` stores the Sequence at runner+24 | `sequence_` |
| constructor installs vtable `0xda8b6e8`, slot 0 resolves to `0xa1c3e0` | `PostDelayedTask` |
| `0xa1c4b4` loads runner+16 and runner+24 together, then `0xa1c4d8` posts the task with them | delegate `PostTaskWithSequence(..., sequence_)` |

The comparison source is
`chromium/src/base/task/thread_pool/pooled_sequenced_task_runner.cc:13-39`.
The enum in `chromium/src/base/task/thread_pool/task_source.h:30-35` assigns
`kParallel=0`, `kSequenced=1`. The singleton is therefore a shared sequencing
object, not a fresh sequence per request. Task traits at `0xc9d66c4` are
`80 00 01 00 01 00 00 00` (BEST_EFFORT, MayBlock), as separately established
in the main renderer contract evidence.

The public backend posts zero-delay tasks at `0x42bd8ec-0x42bd900`. Native
`PostDelayedTask` at `0xa1c3e0` uses that same stored Sequence. Chromium's
sequenced-task contract documents FIFO non-nested tasks of equal delay in
`chromium/src/base/task/sequenced_task_runner.h:107-114,159-171`; thread-pool
tasks are non-nested, as documented by `PostNonNestableDelayedTask` in the
pooled runner source. This establishes serialization and posting order for
this recovered path. Shutdown can still discard tasks; this audit does not
claim shutdown delivery guarantees.

## Compiler and UI completion captures

The complete worker bind state allocated at `0x42bd898-0x42bd8e8` is 64 bytes:
the ordinary bind header/callable followed by only the moved string vector
at +40/+48/+56. Its invoker `0x42c0390` moves those three words into a local
vector at `0x42c03a4-0x42c03b8`, then directly calls the stored callable at
`0x42c03c0`. There is no weak receiver, invocation token, or generation field
in this bind state or invoker.

Worker `0x42bda30` synchronously calls compiler `0x47318dc` at `0x42bda48`.
After that call returns, it gets the UI runner from `0x49fafc`, allocates a
56-byte completion bind state, and posts it at `0x42bdad4` with delay zero.
The only payload beyond the bind header/callable is the compiled pair at
+40/+48 (`0x42bdab4-0x42bdabc`). The worker returns only after this post.

UI bind invoker `0x42c05b8` moves exactly that pair onto its stack and calls
the stored callable at `0x42c05dc`. Callable `0x42c0538` obtains singleton
manager `0x4296d5c` and passes the pair to `0x42970d0` at `0x42c0560`.
Neither stage has a cancellation predicate or request-identity payload.

## Manager storage versus later notification cancellation

Manager `0x42970d0` reads the live feature flag, then unconditionally moves the
incoming pair into manager+24/+32, setting engaged byte+40 if needed:
`0x42970f4-0x4297164`. It replaces an already engaged pair at
`0x4297100-0x4297120`; it does not compare request ages. The feature branch at
`0x4297168` controls subsequent per-profile distribution, not manager storage.
Enabled distribution invokes `0x42977cc` at `0x42971c8`.

The URL setter later calls `0x4730320`. This invalidates the previous flag
at service+176 (`0x4730338-0x4730340`), installs a fresh flag at that same
slot (`0x4730344-0x473036c`), and binds that flag together with receiver+184
at `0x47303cc-0x473042c`. Its weak invoker is `0x40774c0`: it checks the
captured flag at bind+48 and receiver at bind+56 before calling the method.
The method bound at `0x473041c` is `0x4730bb4`, the URL notification method.
When it runs, it reads the service's then-current region from service+64
at `0x4730d14-0x4730d18` before sending URL Mojo at `0x4730d58`.

Consequently successive manager applications may coalesce at this later
notification stage. Observing only renderer URL deliveries cannot establish
that earlier compilation or manager storage was cancelled.

## Current owned implementation and limits

At audit time, `arc/api/arc_renderer_ad_block.mm` uses independent
`base::ThreadPool::PostTaskAndReplyWithResult` calls in `SetRendererBlockLists`.
Its per-request sequenced runner is passed only to the deferred File deleter;
it is not the compiler runner. `PublishRules` rejects a completion whenever
`state.requested_ruleset_id != generation` (and rejects unsuccessful results).

This is a static scheduling/publication-policy difference: accepting B can
already suppress a subsequent completion of A, while native queues both and
accepts each manager completion in sequence. It is **not** evidence of a
native out-of-order overwrite bug, and no overlap test here establishes the
externally observable duration of intermediate rulesets, effects of compiler
failure, concurrent non-UI public callers, or behavior during shutdown. The
present acceptance/runtime probes did not cover this overlap branch.

Reproduction uses `arc/common/renderer_contract_evidence.py` against its
default pristine image. Bounded ranges inspected include
`0x42bd840:0x158`, `0x107d840:0x140`, `0xa1c3e0:0x1a0`,
`0x42c0390:0xd8`, `0x42bda30:0x1d8`, `0x42c05b8:0xe0`,
`0x42c0538:0x80`, `0x42970d0:0x10c`, `0x4730320:0x170`,
`0x40774c0:0x50`, and `0x4730bb4:0x250`.

## Empty results, failure boundary, and private follow-up

An input with no accepted URL rules selects a null URL object at
`0x4731c98-0x4731c9c`. The cosmetic accepted flag starts at zero at
`0x4731dc0` and selects a null cosmetic object at `0x4732694-0x473269c`
when no accepted cosmetic rule set it. The compiler writes both selected
objects to its output pair at `0x47326f0`; the UI callback still submits
that pair and the manager still sets engagement. This is an accepted empty
result, not an absent completion. It covers the existing owned implementation's
successful empty-list/no-accepted-rules cases (`success=true`, invalid File).

The native recovered compiler returns this pair, with no separate success
boolean in its worker/UI bind states. This bounded audit does not establish
all native allocation/shared-region failure outcomes or their equivalence
to our temporary-file I/O failures. The follow-up therefore retains the
existing `!result.success` rejection and all existing empty-result handling;
it changes only compilation sequencing and request-generation cancellation.

At the parent's explicit request, the private candidate is
`logs/wp/renderer-compiler-sequence-fix/new/arc/api/arc_renderer_ad_block.mm`
and its sibling `renderer_compiler_sequence_fix.patch`. It uses a singleton
pooled MayBlock/BEST_EFFORT SequencedTaskRunner and invokes that runner's
`PostTaskAndReplyWithResult`, retaining request IDs as ruleset IDs. Production
source remains unchanged. The exact Base compiler command with a VFS source
overlay passed syntax checking with exit 0 and empty diagnostics; patch
application checking passed. See that directory's `REVIEW.md`, `commands.json`,
`results.json`, and compiler logs. This is not link or runtime validation.
