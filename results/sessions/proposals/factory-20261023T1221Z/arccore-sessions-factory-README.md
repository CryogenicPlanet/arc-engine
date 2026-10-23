# Native restore factory proposal — NOT ADOPTED

Prepared read-only from repository owned source SHA256 d5fdeb5800148ac9c2db37c81c4ce20f40d6e31b2067eb6d56332e30ecc43b6a. All writes are new /tmp/arccore-sessions-factory-* files/directories. No repository/shared edits, object build, package, app run, runtime waiter, Chirp or credential access.

## Ready normal route

Application restore of a ROOT WINDOW only: selected.type WINDOW and matched ID equals the root id/original_id. FindRestoreEntry normalizes a root original-ID match to root.id. Descendant WINDOW requests promoted by nonempty app_name are deliberately excluded. No fixture URL, tab count, group emptiness, metadata emptiness or test-only gate selects this route.

The request calls native RestoreEntryById(nullptr, rootID, NEW_FOREGROUND_TAB). Native helper sets restoring_ and persistence OnRestoreEntryById before reaching context creation. BrowserLiveTabContext::Create continues to compute app/normal type, bounds, show state, workspace, title and every extra_data option. A generic optional host factory hook receives those final params, injects ArcBrowserWindow, and returns a one-shot after-create callback. Browser::Create runs normally. After native creation and before insertion, the callback binds the canonical ArcBrowser wrapper, full-restoring state and exact-source insertion scope, then invokes created.

The request token is found through the canonical exact Profile wrapper and active request, and consumed before invoking app code. Nested requests cannot consume it. The factory callback is host gated. Empty/declined factory hooks retain Chrome's existing contract unchanged. There is no reserved precreated context and no global host-mode reuse gate. Native whole-window AddRestoredTab, saved group/split reconstruction, selection and UpdateTabBrowserIDs remain intact. Explicit ArcBrowser WINDOW restoration has no token and therefore still creates its native new destination.

The owned companion covers ROOT WINDOW only. Legacy TAB/GROUP/partial-window application routes remain unchanged and are NOT endorsed by this proposal. Generic factory notification can later support native-created TAB A, but TAB/helper cancellation and original existing-group-B pending-completion semantics must be designed separately; no finished(B)/finished(nil) is synthesized here.

## Safety deviation — lead allowlist required

After the deletion-capable created callback, the factory uses BrowserWindowInterface::GetWeakPtr and IsDeleteScheduled before returning a context. A null context is handled explicitly in the full WINDOW branch. The helper has a last-member WeakPtrFactory and checks its weak self before touching entry/context after callback. It also verifies that the same root entry still exists; cleared/replaced entries cannot leave a dangling Window reference in use.

If the helper survives but factory/context creation is cancelled, root removal follows the ordinary RestoreEntryById tail, restoring_ becomes false, observers are notified, and nullopt is returned. This deliberately consumes a surviving selected root consistently with the persistence tombstone already queued by OnRestoreEntryById. It is a proposed safety policy requiring lead review, not a recovered original cancellation contract or an adopted change. If the helper/service was destroyed, the weak guard returns immediately without accessing destroyed state. Completed/failed owned requests finish asynchronously; zero-insertion/cancelled completion behavior is also not original-parity evidence.

The helper guards are unconditional, while the factory callback is host gated and the owned token root-WINDOW scoped. This scope difference requires explicit lead review/allowlisting. Ordinary Chrome paths with no accepted callback retain factory behavior, but the helper guards improve teardown handling on those paths too.

Original static evidence (/tmp/arccore-sessions-native-cancellation-evidence.md) shows created at42b9e6c, unchecked continuation/context dereferences, and ordinary physical browser deletion posted after logical close. It does not establish finished(nil), root retention/consumption under created-close, arbitrary nested-runloop teardown, or profile-deletion parity. Do not claim those edges match original.

## Destruction continuation audit

- TabRestoreServiceImpl::RestoreEntryById at components/sessions/core/tab_restore_service_impl.cc:1906–1911 directly returns helper_.RestoreEntryById(...); there is no service/client access after helper returns.
- ChromeTabRestoreServiceClient::CreateLiveTabContext at chrome/browser/sessions/chrome_tab_restore_service_client.cc:39–58 directly returns BrowserLiveTabContext::Create(profile_,...); no post-callback client access follows.
- The native factory uses only the copied weak browser after notification.
- The new owned performNativeWindowRestore uses retained request/ObjC state, the returned result and copied weak browser after native return; it does not dereference the service again.
- Existing upstream deletion-capable observer/insertion paths outside this new factory boundary, including NotifyEntriesChanged observer iteration, are not a blanket audited lifetime guarantee. The new weak checks protect the created-callback return boundary, not arbitrary restoration reentrancy.
- Additional caller limitation: helper RestoreMostRecentEntry at tab_restore_service_helper.cc:658–665 unwraps RestoreEntryById with .value(). Do not broaden hook activation to that route without reviewing its new cancellation-result handling. The owned root-WINDOW implementation calls RestoreEntryById directly and handles nullopt. This is another reason unconditional helper guards require a lead allowlist rather than claiming general cancellation safety.

## Artifacts and application order

Choose ONE prerequisite/patch pair; do not apply both variants.

1. Source-only prerequisite:
   /tmp/arccore-sessions-native-source-hooks.patch
   /tmp/arccore-sessions-source-registration.patch
   Then:
   /tmp/arccore-sessions-factory-native.patch
   /tmp/arccore-sessions-factory-owned.patch

2. Combined source+policy prerequisite:
   /tmp/arccore-sessions-native-source-policy.patch
   /tmp/arccore-sessions-source-policy-registration.patch
   Then:
   /tmp/arccore-sessions-factory-native-after-source-policy.patch
   /tmp/arccore-sessions-factory-owned-after-source-policy.patch

The primary factory header hunk DOES NOT apply unchanged after the combined source+policy prerequisite. The explicit after-source-policy variant resolves that context difference; semantic factory changes are the same.

Native file scope: chrome/browser/arcmod/arcmod_hooks.h, chrome/browser/ui/browser_live_tab_context.cc, components/sessions/core/tab_restore_service_helper.{h,cc}. Owned scope: arc/api/arc_sessions_restore.mm. No ChromeTabRestoreServiceClient reuse patch is needed. No arc_internal.h declaration is added.

Source-only applied staging: /tmp/arccore-sessions-factory-stage/
Source-only prerequisite baseline: /tmp/arccore-sessions-factory-base/
Combined applied staging: /tmp/arccore-sessions-factory-combined-stage/
Combined prerequisite baseline: /tmp/arccore-sessions-factory-combined-base/

## Validation

Both primary patches: git apply --check against source-only prerequisite baseline EXIT0.
Both after-source-policy patches: git apply --check against combined prerequisite baseline EXIT0.
Actual compiler commands obtained read-only via siso query commands, then redirected to temp sources with temp header overlay, dependency/output flags removed, and -fsyntax-only. All three translation units passed EXIT0:
- browser_live_tab_context.cc (rechecked after final declined-hook passthrough fix)
- tab_restore_service_helper.cc
- arc_sessions_restore.mm
Logs: /tmp/arccore-sessions-factory-{browser_live_tab_context,tab_restore_service_helper,arc_sessions_restore}.syntax.log
No object, shared build, package, runtime or differential comparison was performed.
Fresh read-only reviewer found no actionable normal-route ABI/lifetime/reentrancy/completion defect and separately corroborated wrapper/client post-helper continuation audit. This is normal-route implementation readiness for lead review, not general session parity or acceptance.

## Remaining acceptance work

Lead chooses/lands declarations and both exact-source and factory hooks, reviews/allowlists safety deviations, compiles/packages under the shared lock, then runs original-versus-ours full-window fixture and existing close/reopen/restart coverage. Verify callback IDs/order/history and raw identity fields. Original fresh wrapper vs architecture canonical-wrapper discrepancy remains a separate lead decision. No current normal route test result is claimed for this proposal.
