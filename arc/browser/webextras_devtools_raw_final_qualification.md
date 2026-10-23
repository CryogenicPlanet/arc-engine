# Qualification of final DevTools cleanup claims

2026-10-23. This is a separate addendum; the initial peer and facade-cache
proposals, notes, tests and freeze manifests remain unchanged.

The earlier blanket phrase "silent final transport/ProtocolClient destruction"
in the held proposal notes and worker handoff was too broad. It is NOT a proven
native requirement and must not guide a client fix. The full retirement audit
`webextras_devtools_retirement_audit.md` SHA256
`df79de95b0651d2d85a6c0f12cf88eef641502e4dbf118b892a716150a4cef91`
shows two different behaviors:

- Final Impl destruction destroys OUTER pending-command and subscription
  containers without explicitly invoking their terminal host-closed callbacks.
- Final raw-client destruction reaches the SAME close path used by public
  close, explicit detach and observed-target destruction. Native4356ba8 calls
  destructor435d09c, which calls raw Close435c74c BEFORE client guard poisoning.
  Close435c830..85c copies/posts the stored close error through435c8fc. Queued
  thunk435ca2c invokes the copied raw callback; its public adapter4416690 calls
  `(nil,NSError)`. That accepted raw notification has no client/peer/service
  guard and may remain deliverable after final destruction. It naturally stays
  silent only if its stored callback was already cleared by an earlier close.

Therefore the existing `ProtocolClient::~ProtocolClient()->Close()->Notify`
behavior is not itself demonstrated wrong. No silent raw detach should be
introduced from the earlier wording. A separate supported gap is the candidate's
strong client retention by the oversized-message Close task: native captures a
weak real client guard/anchor, allowing earlier map/destructor close to proceed
and invalidate that older task. This requires a separate reentry-safe client
adaptation and tests, preserving accepted copied raw close notifications.

The audit also separates explicit detach (raw cleanup only), genuine observed
WebContents destruction (raw cleanup, physical binding clear, then synchronous
errors from BOTH snapshotted outer maps), and final destruction (distinct router
and delegate guard phases, raw Close preserved, outer maps silently destroyed).
No single generic Retire or unconditional sequence of both observer/owner
triggers is justified. Core lifecycle activation/order remains HOLD.

This addendum changes no source, runtime results or acceptance status.
