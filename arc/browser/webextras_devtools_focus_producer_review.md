# Independent focus producer and payload review

Result: **CLEAR for the bounded static audit; no P1/P2 findings.** This review
checks `webextras_devtools_focus_producer_audit.md` at
`0885a5ae96e462031952c06d94fa29679810fefcb1a9d4fb33d3a3a4eb1c9b1a`
and its manifest at
`eeac0e7ea52b86a250a92f97e095ae82621d7a77fa16b552c27ea779efb0ab16`.
It does not approve activation of a proposal or claim runtime/golden coverage.
Only this review note and `logs/wp/webextras-devtools-focus-review/` were written.

## Independent verification

The review rehashed all 27 manifest artifacts and the note, independently
rehashed thin FE0/full B70 and checked byte equality of the sole arm64 slice
(offset262438912, size239297264), regenerated all18 bounded disassemblies and
compared their complete bytes to the frozen audit files, rehashed all nine
original decoded frontend assets, and verified every recorded source/input
snapshot. All checks passed. This did not re-extract resources.pak.

Manual dataflow review supports the critical correction. Factory436f2e8 puts
Impl at allocation+18;436f348 installs (Impl,control) at allocation+20, hence
Impl+8/+10. Focus4359dd0 takes exactly that pair, CAS-locks the control's strong
count and supplies it to42d4298. This is distinct from the logical owner's weak
pair at Impl+38/+40. As an additional independent identity check, raw control
vtable da824e8+10 resolves43a05f0; that adds18 to control and branches4356b28,
the actual DevToolsImpl disposal body.

Counted helper42d437c..4394 stores and strong-retains that self pair in inner
callback+38/+40, selects invoker4146f80 and destructor42c1618. Invoker4146fc4
passes the stored pair as the bridge argument. Destructor42c1630..44 releases
its strong count. The enclosing registration invoker42c08d0 calls the inner
callback before releasing it42c08d4..ec. Self therefore survives the app callback;
this is stronger than a weak-self-only task. The selected native bridge has a
separate capture and does not redirect to a replacement registration.

The inert branch42d4554..456c also stores and strong-retains self, at task+40/+48.
Invoker42db5e4..5fc moves the pair to its local call argument;42db60c releases it
after calling the no-op. The independently disassembled no-op15ac0 is a literal
ret. Destructor42c0c98..ca8 releases the task's pair if the task does not invoke.
There is no pending increment on this branch.

The exact endpoint matters: after returning from the app callback,42c08d4..ec
can destroy the inner self payload **before** the separate pending decrement
42c08f4..fc. This review establishes self retention through the bridge call; it
does not infer another self lease through all enclosing accounting cleanup or
provide a replacement implementation. It establishes no Root retention and no
analogous payload rule for display/inspect producers.

## Producer and diagnostic review

The genuine packaged frontend action registration53009 loads the real command
menu delegate in53010. Its handleAction requests installed host bringToFront
before opening QuickOpen and returning true. Packaged legacy53015 executes the
registered loader/delegate; host52476 uses the installed host or an inert hosted
fallback. Genuine compatibility52490 emits bringToFront with no params through
DevToolsHost. The prospective diagnostic correctly requires the existing genuine
frontend, initialized ShortcutRegistry, and existing enabled ActionRegistry entry;
it does not construct a host, register a new action, call the app delegate or
simulate OS input.

The reviewed native browser path is method dispatchc0f7f04 ->c0eede0 ->zero-arg
handlerc0eef68 ->bindingsc0f85d4 ->window471a060 ->focus4359dd0 ->helper42d4298
->bridge441610c. Encoded pointer words, dispatch offsets and the real selector
webContentsDevToolsNeedsFocus: agree. Window stage3, inspected target, userdata
and peer are real gates. Supplied43585e4 is not this producer. Current C156 maps
the wire name to ActivateWindow and PRIMARY places its hook after the loaded-stage
gate; neither native path requires an OS focus transition. Ordinary show/open and
other real activation/reveal paths cannot be assigned one callback count from
static source alone.

## Limits

No frontend evaluation, app launch, recording, build, helper change, proposal
change, coverage modification or user profile access occurred. Action execution
and native delegate delivery remain untested. Missing frontend access or readiness
must remain inconclusive. Existing uncovered NativeDevToolsNeedsFocus status is
unchanged. Any focus-only implementation correction still needs its own review
and tests with external peer owners removed before queued work drains; this
review approves the bounded evidence, not such an implementation.
