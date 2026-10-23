Staged close accounting primitive — not an activated close fix
==============================================================

Files: arc/browser/core_close_state.h and core_close_state.mm UNDER THIS STAGE.
Reviewed design input: production arc/browser/core_close_accounting_plan.md.
No production/shared/native headers, source, GNI, lookup, source token, public
Close method, observer producer, or app is changed or activated.

Concrete API
------------
CreateCoreCloseState / CoreCloseStateIsBusy: opaque shared handle. State has three
independent delegate flag+pending pairs (UI, Navigation delegate, Media), plus
two independent observer batch counts (WC and NavigationController). It holds no
native pointer, wrapper, delegate, or task runner. The caller owns association to
one logical Arc WC; discard/rebind association is deliberately not implemented.
State construction and every mutation/query bind to one primary UI sequence via
SEQUENCE_CHECKER. The primitive cannot independently identify that sequence as
Chromium UI without a caller contract; it never creates or selects a runner.

ScopedCoreCloseInvocation: noncopyable/nonmovable LIFO scope. Enter saves the
previous channel Boolean and sets true; destruction restores previous. Thus
inner same-channel return leaves outer busy, and other channels remain independent.
It retains only the exact accounting handle, not the originating wrapper.

MakeCoreCloseAsyncInvocation: increments channel pending before returning a
OnceClosure. Caller immediately posts that closure with its original runner,
location, delay and nesting policy. The outer closure is not weak-canceled:
it invokes the supplied conditional closure if nonnull and only then decrements.
A canceled/expired weak TARGET inside that conditional closure returns without
app work, and still reaches the decrement. Null work behaves the same. Pending
remains nonzero throughout nested calls and through the conditional invocation's
normal return. It does not wait for a dialog's later app completion callback.
The exact state is captured, never looked up again by a native address.

CoreCloseObserverBatch: constructor increments ONE channel batch. Caller keeps
all existing per-registration task posts untouched. PostFinalizer then posts ONE
ordinary zero-delay task to the supplied same-sequence SingleThreadTaskRunner;
that distinct task decrements. The batch can have zero delivery tasks. Calling
PostFinalizer twice CHECKs. There is no helper that regroups/reorders delivery
closures, no inline completion, and no destructor decrement.

Illustrative adapter usage (not implemented):

  CoreCloseObserverBatch batch(state, CoreCloseObserverChannel::kWebContents);
  for (registration : existing_registrations) {
    runner->PostTask(existing_location, existing_weak_delivery);
  }
  bool accepted = batch.PostFinalizer(existing_location, runner);

The producer must not yield/reenter app code between collecting its registrations
and posting the finalizer, must post all deliveries on this same primary runner,
and must call PostFinalizer even for an empty notification. Passing a secondary
UI-capable runner would not prove source FIFO; the helper checks current sequence
but the caller is responsible for selecting the original primary runner.

Cancellation/rejection boundary
-------------------------------
Resetting/destroying an unrun async wrapper does NOT decrement. Destroying an
unfinalized batch token, rejecting its finalizer post, or canceling that posted
finalizer also does NOT decrement. State can remain busy until its own disposal.
This intentionally declines to invent a cancellation rollback which the native
sources do not prove. Async callers must observe the result of their own post;
PostFinalizer returns its post result, marked nodiscard. No failure path silently
pretends the work ran. Owner integration must choose and document a source-backed
shutdown/rejection policy BEFORE enabling synchronous idle Close.

The original native cancellation/destructor route is not fully proved. This
artifact is a primitive proposal, not evidence that permanently busy counters on
live rejected posts are production-acceptable. The helper also does not implement
arbitrary Objective-C exception recovery or asynchronous cross-thread cancellation
mutations; normal invocation return and primary-sequence use are the proven scope.

Validation
----------
Actual Chromium Arc Browser compilation flags were obtained through siso query.
Only source path and include-overlay were substituted, outputs/dependency writes
were stripped, and -fsyntax-only added. Actual compiler exit 0, no diagnostics;
checker process exit 0. See check.log and /tmp/core_check_close_accounting.py.
No build lock, build, app or native binary invocation occurred. No standalone
runtime claim: the workspace has no prebuilt standalone base/test-support archive,
and a fake task/callback implementation would not verify Chromium cancellation
behavior. Integration and runtime comparison remain parent/lead work.
