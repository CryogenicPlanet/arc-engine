# Sessions cached browser-binding audit — read-only, proposed changes HOLD

Read the complete 1,148-line current `arc/api/arc_sessions_restore.mm` (SHA **178091e65d07409a00eeeb86101a7234afd44f00a3597512c2b1be0095118978**), the complete core deassociation plan (fc1fe1b8…), related current binding getters/native factory callback, and existing original dispatch/discard evidence. `audit.json` pins exact inputs and inventories 15 paths; `source-snapshot.mm` preserves the inspected source. Independent reader `/root/sessions_factory_compat/cached_binding_restore_review` audited request/factory paths and original queued-token proof. No source, native, index, Git, regeneration, compiler, build, app, queue, Chirp or credential changes occurred.

**A physical Browser WeakPtr is not the future logical binding.** Browser's physical weak factory survives destructor entry; a forced/direct destructor need not have IsDeleteScheduled true before the fallback. Core's proposed clear of the existing facade binding therefore does not invalidate `_factoryNative`, captured `weakBrowser` copies, or QueueRestorationCallback's `nativeBrowser`. This observation alone does **not** prove every such callback must be suppressed. Cached native work, successful-result policy and logical notification delivery are different contracts.

## Required distinction before any global-close move

- **Native command continuation:** legacy `_created` callout493 returns to physical-only guards497, then BrowserLiveTabContext::From502 and RestoreEntryById514. A complete endpoint contract that makes detached facades unavailable for native commands must revalidate the same canonical facade/native association before resuming this work. Do not recreate a wrapper or invalidate the entire native WeakPtrFactory. A new facade entry guard cannot cancel an already-running stack.
- **Notification-only queue:** QueueRestorationCallback776 checks physical native presence but does not dereference Browser afterward. It selects a delegate and sends a retained current WC peer. Treating canonical deassociation as notification cancellation is **not source-proven** and can overrestrict original behavior. The separately archived equality patch is HOLD, not a correctness fix or adoption recommendation.
- **Successful finished callbacks:** full-restore completions382 and521 can select a facade whose logical binding has cleared while the physical Browser still exists. Whether original legitimately reports that facade, reports nil, or postpones completion requires its own original-lifecycle proof. A current-binding equality condition implements a proposed usable-native result policy; do not label it proven parity or immediately drain pending work in prepare.
- **Already canonical-gated consumers:** SessionsServiceObserver961 and fresh public canRestore/restore/foreign calls1096/1107/1130 reread the facade. The explicit retry1117 re-enters that method. They do not independently cache a Browser across a posted task. Existing synchronous native operations still need owner-reviewed callout continuation handling; do not claim an entry guard covers every native stack.
- **Separate WC domain:** discard and restoration-source scope retain no native Browser. Browser detach alone does not justify cancelling WC adapter callbacks, poisoning WC weak pointers, clearing source markers, or resetting services/observers.

## Complete inventory

All source line numbers below refer to the pinned 178 snapshot. The structured capture/guard/contract details are in audit.json.

| ID | Lines | Path | Current protection / disposition |
|---|---|---|---|
| Q1 | 763-788 | QueueRestorationCallback | Physical weak + ObjC receiver + current WC only; logical notification policy unproved. |
| F1 | 247-263,334-355 | factory pending state and created callback | Factory callback weak/deleteScheduled/profile; canonical/request-generation validity separate. |
| F2 | 368-387 | native-factory full restore completion | Full native-factory finished success uses physical weak/deleteScheduled only. |
| L1 | 449-514 | legacy grouped/partial created continuation | Post-created native work reacquires physical weak/Profile, lacks facade equality. |
| L2 | 518-525 | legacy full completion | Legacy finished success uses physical weak/deleteScheduled only. |
| R1 | 217-235,264-310,389-417 | load observer and posted request retries | Posted request work re-resolves current profile/service; no Browser cache. |
| S1 | 148-181,312-332,791-806 | completion/scoped association cleanup | Scopes/completion state retain ObjC values; no native Browser ownership; callback ordering separate. |
| O1 | 904-985 | SessionsServiceObserver and PopulateOriginalTabIDs | Service changes reread canonical facade and return on null; no cached native Browser. |
| C1 | 988-1039 | current inline Sessions browser-close subscriber | Close producer receives borrowed native intentionally; capture/deassociate/publish pipeline required. |
| I1 | 1041-1092 | bootstrap and insertion callback scheduling | Bootstrap/DidInsert have current-binding entry guard; delivery remains Q1. |
| P1 | 1094-1128 | public canRestore/restore + delayed retry | Fresh public commands/retry reread current facade; in-flight native continuations not covered. |
| P2 | 1129-1147 | restoreForeignTab | Foreign restore rereads current facade; in-flight AddRestoredTab is separate. |
| W1 | 81-134 | discard and finalize task | Discard captures WC/adapter state only; Browser deassociation supplies no cancellation rule. |
| W2 | 742-761 | native restoration source scope | Source scope uses WC WeakPtr for exact userdata restoration; no Browser cache. |
| V1 | 538-707 | session value classes | Navigation/tab/group/window wrappers own snapshots only. |

## Exact consumer contracts for a joint private implementation

1. **Preparation is non-callout.** Core must retain the existing facade, verify exact expected native identity, mark its own command-bearing close state unavailable and clear only the approved canonical binding. Sessions must have all affected consumer semantics reviewed before that endpoint is activated. Cached Browser WeakPtrs can remain identity/liveness witnesses; they are not command authority after the logical cutover. Do not add a generic global invalidation, observer reset, CloseState release or bulk profile-request cancellation.
2. **Command-bearing continuation validation.** For a consumer with an existing facade and captured Browser WeakPtr, a proposed authority predicate is `owner && captured && [owner browser] == captured.get()` at the point native work resumes, after any deletion-capable app callout. Add native scheduled/profile/service checks appropriate to that route as separate conditions. Legacy497 is the concrete missed point before context502/native restore514. Re-resolve service/profile, as the existing path already does; do not carry borrowed service/entry pointers across app code. All request/entry freshness checks remain independent.
3. **Factory state.** `_factoryPending`338 is consumed before app reentry; callback342 is synchronously invoked by browser_live_tab_context.cc582, whose post-callback586 likewise only tests physical WeakPtr/deleteScheduled. At347 the canonical wrapper is obtained on the legitimate creation path; before new request associations/insertion scope/created353, a joint contract must establish that this is still the supported live association. Do not use wrapperForBrowser as a destruction-time eligibility fallback. The captured raw Profile* is used as an identity comparator343, not retained ownership; `_completed`, `_serviceDestroyed` and active request generation are separate, currently unchecked at that callback boundary. This audit does not invent their cancellation behavior.
4. **Finished-result consumers.** Capture/evaluate owner identity at the actual state completion381/520, not merely at registration or creation. If the approved semantics require a live native facade, add canonical equality there; if original permits a logically detached result, preserve that instead. `finish:`312 trusts its input, sets completed/clears callback ownership before app completion324 and clears restore associations only after that call returns327–332. Do not move these callbacks or scoped resets into preparation by assumption.
5. **Queued notification consumer.** Resolve the exact original receiver/registration contract before deciding whether `[liveBrowser browser] == nativeBrowser.get()` belongs at776. The existing current-WC predicate must remain `[wrapper webContents]`, allowing a completed WC rebind; do not revert to the original inserted WC identity or add a Browser-membership condition unsupported by evidence. A pre-dispatch check is point-in-time: current `respondsToSelector:` is app-callable, so any guarantee about reentry during that selector probe needs its own review. The native bridge does not show this selector-probing call.
6. **Completion order.** Core's still-pending **close** NO batch must drain **AFTER** global publication, per the core plan. Do not reset `_closeState` or call NativeDidClose in prepare. Session restore completion/cancellation is a different state machine: equality failures in queued callbacks can trigger `full.cancelled`, pending decrement and `CompleteFullRestore`, including during a nested global callback. Neither immediate `finished(nil)` nor blanket deferral is authorized from the close-batch proof. Resolve this ordering before adoption.
7. **Guarded observers and WC state.** New SessionsServiceObserver change receipt returns on null facade at962 before tab-model use; no asynchronous raw Browser capture exists in its recursive helper/synchronous algorithm lambda935. Keep its service observation and core WillClose subscription. Bootstrap constructor950/1002 is initialization-only. Discard must retain the post-PM selected adapter and count ordering from its own evidence, not inherit a Browser-wide cancellation policy. Native restoration source-scope cleanup must restore/remove its exact WC marker if that WC survives.

## Original dispatch proof and explicit missing link

`results/sessions/native-evidence/restore-callback-liveness.md` proves original task capture of a logical Browser receiver weak manager from **Impl+298/+2a0 at43064e8/4306504**, checks that receiver token/raw receiver at **4307290–43072c0**, strongly retains the Arc WC peer, and enters dispatcher42fcf40. The dispatcher's first test42fcf60..68 is the peer's **current WC field+130**. The later restore branch weak-locks the logical delegate at42fd634..66c; the ObjC bridge43e34b4..c8 weak-loads app delegate and facade before notification. No Browser membership/current physical binding check is shown in that bounded dispatch proof.

Core cleanup42f9cf4 clears **physical association token f0 and binding f8**, plus several registrations/helpers. Neither packet maps cleanup helper108/180 or another removed registration to the **queued receiver token+298/+2a0**, nor proves a later restoration dispatcher f0/f8 gate. A logical Impl retained by the facade/producer during global notification may therefore still support queued restore receipt after physical deassociation. The missing proof is **exact token ownership/invalidation mapping or a later binding guard**, not a general claim that all weak references die together. No fresh binary disassembly/runtime was executed for this audit.

The prior evidence's phrase “current binding” describes the **retained WebContents peer**, not Browser facade binding. This audit explicitly corrects that possible reading. The queued native check itself does not dereference the cached Browser. Therefore neither native parity nor memory safety alone currently establishes “must suppress after deassociation.”

## Separate one-line equality proposal — HOLD, may overrestrict

`HOLD-queue-binding-equality.patch` is against exact current178 **only**, separate from the existing ea4ad908… physical-phase prototype. It adds:

```objc
if (liveBrowser && nativeBrowser &&
    [liveBrowser browser] == nativeBrowser.get() && [wrapper webContents]) {
```

Everything else is unchanged, including `else if (full) full.cancelled = YES`, `full.pending -= 1` and `CompleteFullRestore(full)`. Those retained operations matter: a newly failed equality can **change full restore outcome to cancellation/finished(nil)**; this is not a behavior-free safety check. Original null-WC early return precedes its tracked removal/ID handling and does not itself prove that cancellation outcome. Patch status is **HOLD adapter-policy proposal only, NOT source-correct/native parity proven**. Do not apply it alone, compose it into ea4, or infer it closes the other full restore consumers.

No compiler/test/scenario run was requested or performed. Canonical/full-window phase acceptance remains HOLD; this read-only audit supplies consumer boundaries and missing proof, not activation authority.
