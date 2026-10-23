# Media/PDF accounting companion review — held scope only

Fresh read-only review, 2026-10-23 UTC. Scope is ONLY the accounting deltas over
delivery revision2 and core adapter bec65249, not a repeated base implementation
review. No production/shared source, frozen evidence, test, app, build, runtime,
or lock was changed or executed. Only this distinct review artifact was written.

Reviewed freeze:
`logs/wp/webextras-media-accounting-held/freeze.json`, SHA256
`1b211fcb44e44919d6f56bb61d80802d461c1edc5cb968ccccece1b8fe07128a`.

| Input | SHA256 |
|---|---|
| Media accounting patch | `11898e3d7e7ac0bdbf7faa0bd8b347c16d866cdd428fa10d57c96de4f0cdc537` |
| PDF placement prototype | `b9ffcd626460a9aa5f0c23a3e5e2ecff0ae7d2e6c92335e052daed7596bf8863` |
| Media staged source | `cfff1ed0cffb6910f6e56bc1db8ba3db2aff423f89ab43a8610d21d0ded03006` |
| PDF staged source | `87c53d7c01d4c778d504669197202adfa3dfff6507c0f0a3ddf78e584407ed6a` |
| Frozen delivery base | `d6ba6caf28a403a87fb8d9e8417c0f164c09bcaed9b3f158e9454f37d1c3acda` |
| Core adapter base | `bec65249a39939a247b3fe48a46d1b58fce040eab632fba4f49f507d0e2729b5` |

## Result

The Media accounting delta is clear for its explicitly held composition scope;
no new accounting implementation blocker was found. This is not activation or
functional ownership parity approval. PDF is ONLY a placement prototype and
remains blocked for native accounting eligibility/parity. Its freeze, notes and
test result correctly make that limitation explicit.

Media checks against the exact frozen delta:

- Source construction captures CoreCloseStateForWebContents(owner) once into
  Delivery before subscription. Composed core creates canonical state at657,
  services665, then initializes the media source666. No replacement state,
  receipt/delivery-time lookup, public getter, or owner-retaining state is added.
- AsyncPrepared locks the actual selected C++ bridge at receipt. A missing
  registration posts the existing uncounted Source-retaining inert task. A live
  registration supplies only a weak bridge to conditional delivery while
  MakeCoreCloseAsyncInvocation acquires Media pending before the unchanged post.
  The temporary strong receipt lock ends at receipt return.
- A live bridge counts even with an expired app delegate or missing optional
  selector. Later expired registration, nil owner and other conditional returns
  remain INSIDE the outer primitive; its decrement runs after conditional return.
  Accepted payload still retains functional Source and its copied native value.
- Sync and SyncPrepared alone pass a synchronous state. After selected bridge
  lock, an optional ScopedCoreCloseInvocation starts before preparation/weak
  target loads and spans app return. Nested scopes restore the previous flag.
  Async passes no synchronous state, avoiding an extra synchronous scope.
- No extra queue, observer batch/trailer, close/retry call, rejected-post
  rollback, or unrun-task destructor decrement is introduced. Destroying an
  unrun counted closure intentionally leaves its exact state pending.

## PDF blocker and valid placement boundary

The prototype captures the exact canonical state in its observer constructor
before WatchPDF is scheduled. Its single existing PDFReady post is wrapped in
UI pending; the unchanged DeliverPDF body performs controller construction,
property setter/KVO and delegate callback inside that same outer interval.
Expired observer/nil owner paths return inside the primitive and finalize.
There is no WCObserver batch/trailer, second post, or added synchronous scope.

That placement is insufficient for native eligibility. Frozen producer ASM
`d61408236edba6e3e10f8e470a82eddadd8bc93078568f481d6d6583b25d63e3`
shows logical-WC lock439c0dc..114 and selected UI-registration lock439c134..16c
BEFORE pending increment439c170..178. The prototype instead counts every locally
accepted PDFReady and later uses the existing weak observer/current delegate.
It can count/publish without the original selected registration. Full parity
requires a genuine logical-peer/selected-C++-UI-bridge receipt API before count,
and conditional setter+callback delivery through that selected registration.
An ObjC weak delegate/respondsToSelector/owner test cannot substitute for it.
Do not activate this prototype or count its passing test as eligibility proof.

## Native retirement correction supersedes frozen base prose

Parent CORE correction141280/141340, corroborated by the existing bounded raw
media helper, supersedes the old detach-before-PiP interpretation. When the old
physical binding is valid and the retirement flag permits it,435f9f0 branches
to435fb30;435fb78 invokes PiP destruction BEFORE435fb9c branches back to435f9f4,
where435f9f8 resets the receiver and435fa00 clears physical weak storage.

My earlier delivery-audit prose interpreting that order was wrong. The frozen
ad88d9a2 delivery note and revision2 artifacts remain untouched; they are not
current proof of native early-detach ordering. Revision2 RetirePhysical and its
detach-first test expectation remain superseded/HOLD pending the writer's
separate correction. No expected-old postcall check is inferred from native;
reentrant retirement/attachment remains a separate contract gate. This review's
Media accounting result does not clear that base lifecycle issue.

## Evidence checked, without rerunning

Verified the freeze hash, all six owned artifact hashes, and all 62 frozen
evidence/source snapshot hashes. Read the actual delta, notes and extracted-test
construction. The tests use the actual primitive class/functions and proposed
delivery bodies with mock bind/task/check/sequence environments, rather than a
replacement counter implementation. Media checks cover canonical capture,
receipt acquisition, pending-through-return, nested sync/async, old registration,
expired/missing app targets, nil owner, inert lifetime and unrun-task policy.
PDF checks explicitly label native UI-registration eligibility unmodeled and
test only the outer placement/finalization. Both probe actual primitive sync
flags so async pending is not mistaken for a synchronous scope.

Recorded evidence: Media test exit 0, PDF placement test exit 0; two actual
out/Base TU syntax checks exit 0 without diagnostics; 10 strict validation checks
exit 0; 12 consumer-composition checks exit 0 and combined Media compile exit 0
without diagnostics. The consumer composition commutes
in both orders and forwards into the same async entry without a second count.
No test/compiler command was executed during this review. These results do not
prove Chromium task/Mojo behavior, link/runtime parity, or shared API approval.

Activation remains HOLD for genuine logical-WC/sibling-peer ownership, corrected
retirement/rebind/public-old-binding behavior, lead-approved canonical API and
full close composition, PDF selected-registration integration, volume producer
integration, and authorized link/runtime validation. No public Close or bulk
Busy behavior is approved here.
