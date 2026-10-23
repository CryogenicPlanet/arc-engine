# Extensions keyboard seam, ready proposal only

UNAPPLIED. Responds to extensions request138730 and source proof in
logs/wp/extensions-action-command-consumer.md. Lead owns the arc_internal.h
declaration; core owns the ArcBrowserWindow caller/default. Apply both together.

Proposed API: arc::HandleExtensionsKeyboardEvent(BrowserWindowInterface*,
const input::NativeWebKeyboardEvent&) ->bool. Caller copies its native browser
WeakPtr before dispatch, passes that same browser and the original event, then
returns without reading this/window/registry after a possibly destructive action.
PreHandle remains NOT_HANDLED. Default weak implementation returns false.

No dispatcher is activated by this seam. Extensions must keep its future strong
implementation inactive/false until the exact window-owned registry, suspension
propagation, registration/unload/collision and teardown contracts are integrated.
Do not use menu forwarding, global hotkeys or a partial unsuspended handler.

Validation: actual ArcBrowserWindow compiler flags with staged header overlay,
compiler0/no diagnostics; strict current-base apply-check PASS. Fresh core_review
found no P1/P2, confirmed signature/linkage/reentrant return/no duplicate API.
No build, app, keyboard event, native UI recording, production/shared edit or
runtime/parity claim. Evidence logs/wp/core.extensions-keyboard.proposal.check.log.
