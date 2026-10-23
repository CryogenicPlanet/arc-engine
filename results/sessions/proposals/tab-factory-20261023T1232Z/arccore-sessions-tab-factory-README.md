# Incremental root TAB factory alternative — NOT ADOPTED

This proposal applies AFTER the frozen root-WINDOW factory proposal, never instead of its source-marker/native-factory prerequisites. No frozen root proposal, repository/shared file, build/package, app, runtime queue, Chirp or credential was modified. New writes are exclusively /tmp/arccore-sessions-tab-factory-*.

## Viable scope

An application restore request whose selected entry is a ROOT TAB with tab.group absent, and whose matched restore ID is root.id/original_id. This is a native semantic restriction, with no fixture URL/count/history gate. Existing ROOT WINDOW support remains unchanged. The internal method is renamed performNativeCreatedBrowserRestore to cover both routes.

The owned call still supplies nullptr native context, normalized root ID and NEW_FOREGROUND_TAB. Native RestoreTab respects tab.browser_id for destination selection only when disposition UNKNOWN (original native6c86ee8/current source1538). NEW_FOREGROUND_TAB skips that selection. With group absent, live-group redirection cannot occur. The native factory therefore creates A using normal defaults; the already-staged hook injects ArcBrowserWindow and invokes created before insertion, with restoring_ already true. Then native UpdateTabBrowserIDs runs at the original location, fixing the old precreated-context bypass.

The full-restoration scope is installed before created and insertion. The exact-WebContents source-marker prerequisite supplies Tab.original_id to the full/tag1 restore callback, including present -1. This is application-created full state, distinct from explicit ArcBrowser tag2 restoration using the requested historical ID. No new guessed ID queue is introduced.

Grouped TAB roots are deliberately excluded even when their group might no longer exist: a live group can redirect to B without factory creation. B's original bounded success has no created/finished callback and retains native pending manager state; neither finished(B), finished(nil), nor a guessed/leaking equivalent pending state is implemented. Existing grouped TAB/GROUP/partial WINDOW legacy precreation remains broken/unendorsed and unchanged by this incremental patch.

## Guards and ordinary semantics

RestoreTab captures tab.id and an actual ROOT TAB pointer before invoking the factory. It sets expected_root_tab only if the session type is TAB, the pre-factory lookup is valid, the iterator points to this exact tab, and the actual root type is TAB. Thus a WINDOW/GROUP/SPLIT descendant never compares its enclosing root entry to a child pointer.

Immediately after factory return, weak helper lifetime and context null are checked before accessing tab or invoking native remapping. An actual root TAB also revalidates its captured root identity before reading tab fields. The root TAB caller checks weak helper and context immediately after RestoreTab returns, before ShowBrowserWindow, and helper lifetime again afterward. RestoreTab checks helper lifetime before its OnTabRestored client call. These guard the new created-callback return boundary; they are not a blanket guarantee for arbitrary native observer destruction.

On created-close/entry-clear cancellation, root TAB follows the root-WINDOW proposal's proposed safety policy requiring lead review: a surviving selected root is consumed through the ordinary tail, restoring_ resets, observers are notified and nullopt propagates to the owned caller. A destroyed helper returns immediately. This agrees with the persistence tombstone already queued before creation, but is NOT a recovered original cancellation contract. It requires the same lead allowlist as the root-WINDOW safety deviation.

Normal existing callers retain destination/group/disposition behavior, callbacks, native remapping, OnTabRestored and final return. Additional post-destruction/null checks only affect teardown/failure cases. The guards are unconditional while accepted factory callbacks remain host gated and scoped to the owned root requests. Existing descendant callers have not all been taught to propagate null; no accepted new callback is armed for those routes. Do not expand activation there without auditing them.

The existing helper RestoreMostRecentEntry().value() limitation remains: the owned route calls RestoreEntryById directly and handles nullopt. Do not arm callbacks through MostRecent without reviewing cancellation result semantics. Existing upstream native-observer/insertion/NotifyEntriesChanged teardown is outside the bounded audit. The service/client wrapper chain remains direct-return, as verified for the root-WINDOW proposal; no post-helper service dereference was added.

## Apply order and files

First apply ONE complete frozen root-WINDOW sequence documented in /tmp/arccore-sessions-factory-README.md, including exact-source markers and the appropriate source-only or combined source-policy root patches.

Then apply these same incremental patches in either case:
1. /tmp/arccore-sessions-tab-factory-native.patch
2. /tmp/arccore-sessions-tab-factory-owned.patch

Changed native file: components/sessions/core/tab_restore_service_helper.cc only.
Changed owned file: arc/api/arc_sessions_restore.mm only.
No new header or hook declaration is introduced; the root factory hook is reused.
Applied source-only temp staging: /tmp/arccore-sessions-tab-factory-stage/.
Generator/check script: /tmp/arccore-sessions-tab-factory-{stage,check}.py.

## Validation and status

- Native incremental apply-check against frozen source-only root factory stage: EXIT0.
- Owned incremental apply-check against frozen source-only root factory stage: EXIT0.
- Native incremental apply-check against frozen combined source-policy root factory stage: EXIT0.
- Owned incremental apply-check against frozen combined source-policy root factory stage: EXIT0.
- Actual real compiler command redirected to temp helper source/header overlay with output/dependency flags removed and -fsyntax-only: EXIT0.
- Actual real compiler command redirected to temp owned ObjC++ source/header overlay similarly: EXIT0.
- Logs: /tmp/arccore-sessions-tab-factory-tab_restore_service_helper.syntax.log and /tmp/arccore-sessions-tab-factory-arc_sessions_restore.syntax.log.
- Fresh static review confirmed normal routing, actual pre-factory root identity, weak/null guard order, native remapping and full source-ID semantics; no actionable normal-route blocker found.

Normal ungrouped ROOT TAB and existing ROOT WINDOW route is ready for lead review/adoption and runtime comparison. No runtime acceptance/general session parity is claimed. Lead must explicitly allowlist cancellation deviations, retain grouped/partial route limitations, and schedule tests under the shared lock. No runtime job was launched or queued here.
