# HELD focus-only actual DevTools peer payload correction

This DISTINCT private incremental proposal applies AFTER initial logical peer
8dccf5c9, required facade-cache correction f702890b, and typed cleanup increment
e00a2896. It does not rewrite those proposals, tests, notes or freeze artifacts.
Actual production/shared files, Chromium, GNI/build graph, apps/profiles, locks,
board and existing recordings were not changed. COMMON's integration authority
does not activate the held logical lifecycle or this correction.

## Native evidence and scope

Read `webextras_devtools_focus_producer_audit.md` SHA
0885a5ae96e462031952c06d94fa29679810fefcb1a9d4fb33d3a3a4eb1c9b1a,
and its independent review `webextras_devtools_focus_producer_review.md` SHA
9fba4dbd8fd011fcbb743908b37fe1dcff5015ce8a7a87b50aabfc76664c534f.
Original arm64 image remains fe0f5127. Factory436f348 installs the self weak pair
of the SAME DevToolsImpl/control. Focus4359de4..e20 acquires it, and helper
42d437c..4394 copies that self pair into counted inner payload+38/+40. The selected
bridge is independently retained. Inner payload destruction42c08d4..ec follows
app return and PRECEDES pending decrement42c08f4..fc. No extra self lease through
that finalizer follows from the evidence.

The no-registration branch42d4554..456c retains the SAME peer in an uncounted
inert task; run42db5e4..60c or canceled-task destruction42c0c98..ca8 releases it.
No Root, public wrapper, service or app delegate is a strong payload. The outer
actual-peer guard remains. This focus-specific evidence supersedes older blanket
weak-only focus lifetime prose; it does not establish display/inspect payloads.

## Implementation

`webextras_devtools_focus_payload_held.patch` changes only two held files:

- The actual `WebextrasDevToolsPeer` additionally inherits
  `std::enable_shared_from_this<WebextrasDevToolsPeer>`. Its existing make_shared
  factory/control block remains the sole native identity; no duplicate peer,
  alive token, facade factory, Root holder or global map is introduced.
- `PostDevToolsFocus` acquires the same actual peer through its own
  `weak_from_this().lock()`. It never locks Root or resolves a public wrapper.
  If final strong ownership has already ended, acquisition fails without
  resurrecting the peer. Otherwise the real peer handle is bound by value as
  the native argument of the task. This happens with or without a selected
  bridge, so absence now posts the required inert peer-owning work.
- A selected strong bridge and the existing delegate WeakPtr guard retain their
  policy. Delegate/service weak loads remain at delivery; nil service is
  permitted. Nonnil replacement updates the selected bridge, while nil/re-add
  cannot redirect an older captured bridge. The by-value actual peer argument
  survives the app call and releases when that inner invocation returns.
  It is not an additional lease around a hypothetical accounting finalizer.
- Display, inspect and the entire ObjC facade/cache are byte-identical to e00.
  No producer hook, selector, encoding, property metadata, raw client, command,
  subscription or cleanup trigger is changed.

**Delegate pending accounting is absent in this held chain.** The correction
preserves that absence and adds no invented counter/state/guard. Native pending
parity, including selected-live acquisition and decrement after inner destruction,
remains HOLD for the separately reviewed typed adapter. The inert work remains
non-notifying; this proposal does not claim native counter implementation.

## Verification

The composed private source passes the exact e00 Chromium/macOS arm64 SDK/ARC/
C++23/compiler-plugin/Werror syntax command: exit0, empty diagnostics. This was
fsyntax-only, not a shared build, object/link, app launch or runtime comparison.
Both prior Core Root composition orders strictly apply the incremental patch
and reconstruct the identical held source/header bytes.

`webextras_devtools_focus_payload_test.py --stage <private-stage>
--negative-controls` runs actual extracted functional Root/peer/facade/producer
bodies with native host, WeakPtr, queue, transport and JSON mocks. It preserves
all25 prior groups, except that their final destruction-phase weak canary now
uses an unchanged display receipt instead of focus. The former focus canary's
weak-only expectation is specifically superseded by the new native proof; this
private adapter is explicit and does not edit either frozen prior test.

Eight added groups prove same actual peer/control block and functional state;
Root and facade death before drain; peer alive through app return; nil service;
same-bridge nonnil replacement; old bridge across nil/re-add; weak delegate
expiry; no-registration task execution and task cancellation; Root already dead
before a service-owned focus receipt; and display/inspect still cancel once the
last focus payload releases. All33 groups pass. The prior weak-only focus body
and a new skip-inert perturbation each compile and fail their intended ownership
assertion. No synthetic native pending counter or app callback is manufactured.

The queue mock copies bound captures until RunOne returns. Its tests measure
eligibility through callback and release by completed task destruction; they do
not independently measure C156's moved-bound-argument destruction endpoint.
Production BindOnce move semantics and the original native inner destructor
provide that narrower post-app-return ordering evidence. No exact pending
finalizer timing claim is made from the mock.

The actual facade metadata/cache fixture also passes: full
T@"ArcWebContents",R,N,V_toolboxWebContents, ivar type/offset8, genuine getter,
independent strong facade caches, and peer→bridge→cache final-release order.
The original8dcc negative still exits42. The test-only cache seed remains a
fixture mechanism, not a new production toolbox population path.

## Remaining holds

Fresh independent code/test review is bounded-CLEAR, no P1/P2 findings:
logs/wp/webextras-devtools-focus-payload/review.md SHAe186198d, with independently
repeated33 groups and both compiled negatives. Native evidence review is separate
and frozen. This is readiness for held integration review only. All prior
peer/cache/cleanup lifecycle and C156 reentry/order,
Core ownership/factory/facade activation, toolbox population/identity, typed
pending accounting, physical replacement/rebind and runtime scenario/golden
acceptance gates remain. No existing diff or coverage count changes merely
because this unapplied focus payload correction compiles and passes mocks.
