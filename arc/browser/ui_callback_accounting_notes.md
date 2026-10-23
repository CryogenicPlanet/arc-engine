UI close-accounting handoff — source audit only
==============================================

This is a UI-owned companion to core_close_accounting_plan.md, not an applied
implementation or permission to change public-close timing. Shared state/API,
native-close-source continuation, bulk-close behavior and rebind policy remain
lead decisions. Current UI production and queued acceptance inputs are frozen.

Current proven per-WC app callouts
---------------------------------
Paths below are under arc/api/. Line numbers describe the current source and may
shift. Each synchronous scope must include the app selector's RETURN, preserving
BOOL results and reentrant lifetime defenses.

| File:line | Selector | Current producer/delivery |
|---|---|---|
| arc_ui_javascript.mm:74 | webContents:willRunJavaScriptAlertPanel:withMessage:completion: | synchronous |
| :77 | webContents:willRunJavaScriptConfirmPanel:withMessage:completion: | synchronous |
| :80 | webContents:willRunJavaScriptTextInputPanel:withMessage:withDefaultText:completion: | synchronous |
| :101 | webContents:runJavaScriptApplicationModalBeforeUnloadPanel:withMessage:withPrimaryButtonLabel:completion: | synchronous |
| :167 | webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel: | synchronous |
| :168 | webContentsWillCloseJavaScriptPanel: | synchronous |
| arc_ui_permissions.mm:189 | webContents:willAskForPermission:withRequests:withQuietUIReason:completion: | Show enqueued at143 |
| :152 | webContentsClosePermission: | synchronous prompt-destructor path |
| arc_ui_auth.mm:131 | webContents:showWebLoginDialog: | Show enqueued at85 |
| :120 | webContents:didAutoFillWebLoginDialog: | autofill enqueued at107 |
| :98 | webContentsDidCloseWebLoginDialog: | synchronous, including teardown |
| arc_ui_menus.mm:477 | webContents:willShowContextMenu:withElement: | synchronous BOOL |
| arc_ui_lifecycle.mm:87 | webContents:willShowConfirmInfobar: | synchronous native add or replay |
| :98 | webContents:willCloseConfirmInfobar: | synchronous native remove |
| :172 | webContents:showHungRendererDialog: | synchronous BOOL |
| :191 | webContentsHideHungRendererDialog: | synchronous teardown |

The four lifecycle selectors extend the core plan's callsite inventory. Infobar
Initialize posts Replay at arc_ui_lifecycle.mm:60; Replay115-123 discovers zero or
more current infobars and calls OnInfoBarAdded. Whether that discovery task itself
is a pending UI invocation needs native producer proof. Do not count every posted
native-discovery task merely because it can eventually call an app selector.

Do not broaden per-WC membership without evidence
------------------------------------------------
Menu model notifications at arc_ui_menus.mm:340/349 (menuItemModelChanged: and
menuItemModelRemoved:) have shared snapshots/state but no proven logical-WC close
domain association. Notification-center display:/close: and shutdown close: at
arc_ui_notifications.mm:346/356/388 are profile/global, not arbitrary source-WC
callouts. Native action dispatch/displayed-query tasks at82/427 likewise do not
establish membership in a WC UI invocation domain.

Retained completions are not outstanding app invocations
-------------------------------------------------------
JS, permission and login completions, infobar actions, and hung wait/crash closures
must not hold the original invocation busy while waiting for the user. Their
later app-to-native work can trigger NEW app calls which need their own scopes.

- JS completion executes native work on UI, or marshals at147; Handle/Cancel close
  the app panel before invoking detached native callbacks117-128.
- Permission completion queues Resolve at191. Resolve can destroy the prompt and
  cause a new synchronous close selector; it is not the original show invocation.
- Auth responses execute on UI or marshal at55/65/75. Registration can invoke
  autofill and enqueue delivery before the Show task is enqueued83-85. Preserve
  this native order; do not buffer autofill behind Show.
- Context-menu scope ends when the app delegate returns, before fallback Cocoa
  tracking at492. The native menu lifetime is not one long delegate invocation.

Minimal lead contract required before owner instrumentation
----------------------------------------------------------
1. Obtain already-initialized per-logical-WC accounting state without creating a
   wrapper or new native identity. Permission construction initially has only a
   native contents pointer; define whether/where initialized state is available
   before its existing delivery-time wrapper lookup.
2. Nested synchronous guards preserve prior busy depth and span selector return.
   They capture state BEFORE callout, then clean up without dereferencing a native
   producer which the callback may have destroyed.
3. Queue tokens acquire pending state BEFORE enqueue and decrement AFTER invocation
   returns, including missing delegate/selector and expired weak target. The native
   invocation wrapper performs that decrement (42c0840); its task destructor
   (42c17f0) does NOT. Rejected-post/cancelled-task behavior remains unproven: do
   not promise rollback on rejection or cancellation. A weak member-task binder
   alone cannot own this accounting: its skipped expired-target branch would
   never execute the invocation wrapper's cleanup. Core's staged primitive keeps
   invocation completion separate from destruction; shared adoption is pending.
4. Every cleanup uses the exact captured state, not a later lookup by a recycled
   native pointer. Define state lifetime through discard/rebind before wiring;
   no reset/generation cancellation is justified merely by native replacement.
5. Preserve current delegate lookup/capture timing and weak/generation guards.
   Auth and permission close callbacks can originate during destruction; state
   must be captured while the relevant association is still valid.
6. Do not invoke public Close or inline a retry from a scope/token destructor on a
   native callback stack. Keep existing public close deferred until coordinated
   core/UI/webextras/native-source/bulk-close accounting is complete.

Existing defenses to preserve: JS detaches completion before reentrant close;
hung Show rechecks weak identity/generation175, Hide captures owner before clearing
state; infobar removal retains its local entry93-101. An app selector can destroy
its producer, replace its delegate, close contents, or complete synchronously.

This audit did not change delivery timing, instrument counters, run apps, build,
modify shared declarations/native patches, or claim callback-close runtime parity.
