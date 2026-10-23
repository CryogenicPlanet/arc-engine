# Private discard nil-owner bridge-policy candidate review

**PASS within the one-line conditional bridge-policy scope. No P1/P2 defect found.** This candidate is private, unapplied and uncompiled; it is not an activation recommendation or full discard/Root/accounting qualification.

## Exact change and provenance

Live production base is **178091e65d07409a00eeeb86101a7234afd44f00a3597512c2b1be0095118978**. Target is **6c793d12c18ecbeb0577d21629cfc8914e8ec0bc3d6b014b87d764c1d6135d55**; patch is **a0aee98cd69696518835094f8fc94a82d102ff4b17b9e36d86ed115ceda6ec3e**. All match manifest.json. Exact in-memory hunk replay reconstructs the target at its stated line with no fuzz, offset or filesystem application; reverse replacement recovers the production base. The sole removed line is`if(contents && delegate &&` and sole added line`if(delegate &&`(with the original whitespace). No other target byte changes.

## Captures, lifetime and accounting preserved

Targetarc_sessions_restore.mm122–134 retains the same weakContents/weakAdapter captures, precise strong selectedAdapter126, weak owner load127 and selected-adapter delegate load128. selectedAdapter remains precisely alive through app callback andpending--133. The delegate is still a strong local selected from that same adapter, and the delegate/optional-selector predicates129–130 remain. The actual BOOL, selector/argument order, single existing PostTask, state capture, and pending increment124/decrement133 are byte-unchanged. There is no new registration lookup/retarget, strong owner capture, native access or second counter/task.

Only when the selected delegate survives/responds while weakContents has become nil does the local send-policy differ: candidate sends finalizeDiscardRequest withnil first argument instead of suppressing it. It still decrements pending after either send or skip. This changes a conditional branch's observable behavior; it is not described as behavior-free or as proven runtime reachability. Ordinary unchanged Objective-C call/return assumptions apply; app exceptions and unrelated lifetime architecture were not newly audited.

## Original comparison

Freshly hash-verified original B70 body **4446b10..4446b74** preserves actual result, weak-load-retains delegate+10 intox21 at4446b2c/30, then owner+8 intox20 at4446b38/3c, and **unconditionally sends at4446b54** withdelegate as message receiver andowner as first explicit argument. Its complete extracted body has no conditional branch or owner test. The selected delegate is held until its post-send release4446b70. Mapping.json's selector/import/vtable qualification and all archived range hashes match the frozen raw packet. This independently supports the local nil-owner policy: a surviving delegate is sentnil when that weak owner load is nil. Native Impl argumentx1 is not read by this bridge.

The candidate appropriately leaves the surviving-delegate and ARCH optional-selector checks in place. The original's unconditional send does not require broadening this hunk to unsupported selectors. Earlier post-PM binding guard110–114 remains untouched; physical presence at posting is distinct from a later weak ObjC owner load. No claim is made that a valid selected registration/delegate can actually coexist with an expired canonical facade in a supported runtime path.

## Limits and status

This hunk does not supply functional logical Root ownership, authoritative shared registration, absent-registration retention, outer accounting or a save-helper contract. Their semantics are not inferred. It leaves all global-close/factory/restore/discard setup behavior unchanged, and is separate from held global-close v2. Neither nil-owner delivery reachability, universal finalizer exactly-once behavior nor full discard parity is proved. The frozen raw packet's separate independent review is complete and agrees on the conditional local bridge policy; its scope explicitly excludes approval of this candidate. This candidate review independently checked the bounded bridge instructions and pins.

Standalone adoption remains **HELD pending the full Core dispatcher/Root lifetime contract**. The posted task strongly captures SessionsDiscardState, which strongly owns its adapter, while the facade owner is weak. Consequently this implementation may admit a surviving adapter/delegate with nil owner even if the original intact Root/registration lifetime excludes that state. Removing the owner predicate aligns the bridge policy only; it does not establish equivalent generated-event reachability. This review does not recommend activating the one-line patch independently.

No syntax/compiler/build/runtime/query/test/probe/app/UI, Git/index/GNI, source/shared, queue/process/lock/Chirp action occurred. Only candidate-review.md andcandidate-review.json were added to this private candidate directory. Production and original evidence were unchanged; Renderer85455 hold respected. Future adoption/checks remain parent-owned and are not performed or recommended by this PASS.
