# Held Media close-accounting companion

Unapplied incremental proposal only. This does not activate public Close, alter
actual API/Chromium/shared source, or supply native logical-WC ownership. Apply
only after reviewed delivery revision2 and the lead-approved shared eager hook,
with current core adapter bec65249 and its canonical-state/primitive dependencies.

Inputs are frozen delivery patch
`d6ba6caf28a403a87fb8d9e8417c0f164c09bcaed9b3f158e9454f37d1c3acda`,
staged media
`cf829322d53d3a316e8916b838ced02cec10f5fd41bda5139feb7858e619c283`,
delivery freeze
`06d240456e619f5be2f55c7e5ca7c5f9f0542188f9d5b0bfea2f33549212d5dd`,
and core adapter
`bec65249a39939a247b3fe48a46d1b58fce040eab632fba4f49f507d0e2729b5`.
No frozen delivery artifact was modified. Exact companion/source/test hashes,
strict reconstruction, compiler commands and results are in
`logs/wp/webextras-media-accounting-held/`.

`webextras_media_close_accounting_held.patch` changes only the proposed owned
media implementation. It includes the already-staged core private adapter API;
cross-package use remains a lead approval dependency. No new shared declaration,
replacement state, native-pointer lookup, public service getter or accounting
state creation is introduced.

`WebextrasMediaSource` captures `CoreCloseStateForWebContents(owner)` once in its
constructor, before attachment/subscription. Its Delivery member owns that exact
handle. The eager core call is after canonical state creation and services,
before initial setters; the companion verifies that order in the bec652 context.
Owner reassignment/expiration, delegate assignment, receipt and teardown do not
look up another state. This handle accounts for the existing logical WC; it does
not substitute for that logical object's native lifetime or sibling ownership.

At every regular enqueue, the selected C++ registration is temporarily locked.
If absent, the existing uncounted inert Source-retaining task is posted. If live,
`MakeCoreCloseAsyncInvocation(..., kMedia, ...)` acquires pending at receipt and
wraps the existing conditional delivery. Copied payload, selected weak bridge
and strong functional Source remain inside that conditional invocation. The
temporary strong bridge is released on receipt return. A nil app weak delegate,
missing optional selector or later expired registration does not skip pending
acquisition/finalization. Nonnil replacement and nil/re-add keep revision2's
selected-registration behavior. The sixth volume entry naturally uses the same
helper when its independent producer is eventually composed.

Only Sync/SyncPrepared pass a synchronous state to the delivery helper. After
locking the selected bridge, before preparation or weak ObjC loads, it constructs
`ScopedCoreCloseInvocation(kMedia)`. The scope ends after conditional app delivery
returns; nested scopes restore the previous Boolean. Async uses the same target
delivery helper with no synchronous state, so there is no second sync scope
around an async selector. Existing runner, ordinary zero-delay post and callback
order are preserved. There is no observer batch/trailer and no destructor or
rejected-post rollback. The native no-run policy is inherited directly from the
existing core primitive.

Evidence is the frozen Media audit ad88d9a2/manifest de019558: state43615b4..15bc,
camera4361970..1978, microphone4361d2c..1d34, actions43626c4..26cc,
position4362f30..2f38; selected bridge and post-return decrement42c0840;
PiP4355b30 and all eleven producer scopes. Volume435ffcc..ffd4 is independently
bounded in the volume audit. Uncounted Source retention follows native
4361858..1870/4362608..2620, without acquiring pending.

The owned executable test extracts the actual proposed helper/cache/Source,
service registration/getter and PiP creation/destruction, plus the actual core
primitive's delegate state and functions. Foundation mock tasks/binding/checks
and sequence scaffolding supply the environment. Tests run with ARC, -O2,
-fno-exceptions and -Werror. They verify receipt-time counts, post-return
completion including expired/absent targets, same-bridge replacement, nil/re-add,
expired app delegate and missing selector, nil owner, nested async/sync drains,
sync-before-controller preparation, previous-flag restoration, exact canonical
capture once, inert Source lifetime without counting, and intentionally unreleased
pending after an unrun task is destroyed. A probe of the actual primitive's
EnterSync/LeaveSync verifies ordinary async callbacks have no synchronous flag;
async delivered inside a genuine outer sync retains that prior flag. C++
exception behavior is neither implemented nor inferred: Chromium uses no
exceptions and the test is compiled accordingly.

The Media companion and separate PDF placement prototype compile with actual out/Base
flags/plugins through a private VFS composition. The consumer8d8574a6 strictly
commutes with these deltas in both orders; the combined Media TU also compiles
against notification399b42ef plus outer routing9161ec2b headers/generated Mojo.
The consumer forwards into the same sixth async entry without another queue or
count. See volume-composition.json for hashes and exact checks. Foundation tests and strict
reconstruction pass. This is source/type/primitive evidence, not link or engine
runtime parity. No task here acquired a build/run lock or changed an app/profile.

The PDF artifact is explicitly only a placement prototype: its native selected
UI-registration eligibility is missing and requires a genuine shared UI bridge
API. Its passing placement test must not be read as full native accounting parity.

Activation remains HOLD for lead approval of the same-state API/core full close
composition, the early physical detach/rebind/public-old-binding contract, whole
native logical-WC/sibling-peer retention, volume producer wiring, actual link and
paired runtime cases. The retained Media Source does not prove full logical-WC
ownership equivalence. The shared state handle does not repair that gap. No
public Close query, synchronous fast path, retry or bulk Busy behavior is added.
