# Original destructor prefix versus unload admission

**No direct pre-publication destructor write was found that changes the exact immediate native/controller rejection gates inspected in original7d621f4. The physical flag written at36a8960 is not the type word read by admission. Forced/direct admission remains UNKNOWN; this is not a proof that fresh admission succeeds, nor evidence supporting a guessed command-ineligible skip.**

## Exact receiver and field separation

Physical destructor36a8920 receives the full native allocation in x0/x19. It reinstalls vptrs at physical+0, +0x20 and +0x28 (36a893c..58). The interface pointer is physical+0x28, assigned to x20 by36a8954..58. It writes **physical+0x91=1 at36a8960**, equivalently interface+0x69. It then obtains the logical peer from the native userdata keydf6a920 and invokes logical close producer42feb24 at36a89f4. Its bounded direct prefix contains no write to an UnloadController state field.

The fresh termination callback obtains native UnloadController from userdata keydf54710 (42c2418..2430; prior pinned key literal UnloadController::kDataKeyImpl). Admission7d621f4 receives that controller in x0/x19, **writes its own controller+0xc8=2 at7d62218..1c**, then loads the native interface from **controller+0x28 at7d62220**. Equal numeric offsets across these receivers do not identify the same field.

The newly installed interface vtable isdd0c250 (=dd0c070+0x1e0). Its virtual+0x60 slotdd0c2b0 points to2596740:

    ldr w0, [x0, #8]
    ret

Thus admission reads a type word at **interface+8 = physical+0x30**, not a constant-return thunk and not physical+0x91. The prefix vptr stores and byte+0x91 store do not overwrite that word. Slots+0x18/+0x20 both point4c9ac8, returning interface+0xa8, consistent with the before/after-publication userdata lookups; no private logical-Impl layout is inferred.

## Inspected admission gates

| Address | Exact operation |
|---|---|
| 7d62230..34 | native type==3 returns0 through7d6225c |
| 7d62238..48 | controller+0x98==4 returns0; ==5 enters extra checks |
| 7d6224c..58 | state5 requires controller+0x78/+0x90 empty; nonempty branches to diagnostics7d62330/3c, otherwise returns0 |
| 7d62278..84 | other states enter helper7d61294, which is outside this immediate-gate comparison |
| 7d622d8..dc | empty resulting controller+0x78 returns0 |
| 7d622ec..7d62324 | installs continuation, validates state/queue/continuation, writes state+0x98=1, invokes7d61400 |

The bounded admission body7d621f4..7d6232c has no direct read of interface+0x69/physical+0x91. The destructor prefix36a8920..36a89f8 has no direct write to controller+0x98, +0x78, +0x90 or +0xc8. Prior controller lifecycle state can independently select an early return or another branch, and helper/callout effects are not exhaustively resolved. No state names or public C156 enum equivalence are invented for numeric4/5.

Logical Browser closing is separate again:42feb24 writes **logical Impl+0x178 at42fec20** and performs cleanup before application global delivery. Neither the type thunk nor these immediate UnloadController gates reads that logical field. Logical cleanup helper transitive effects and other observers can reenter; this local comparison does not prove they cannot change native/controller state.

## Later fallback is after publication

After42feb24 returns, destructor36a89f8..36a8a80 looks up the UnloadController keydf54710 and reads controller+0x148. If its tested bit is clear, a later branch reaches35ea198 at36a8b94. Those operations occur AFTER the global producer call36a89f4, and cannot be described as a prefix mutation establishing rejection before that call. Existing normal-close state might already differ; forced/direct prior state is not established here.

The next design decision still requires the independent native collection-removal/admission contract identified in the termination audit. Do not transfer logical closing+0x178, physical+0x91 or a facade binding clear into a new skip condition. No successful or failed reentrant termination execution is claimed, and no new command-admission policy, activation or probe is recommended.

Manifest pins full original B70, the static extraction tool, prior evidence, instruction bytes and interface slots. Independent focused reviewer full_probe_review confirms the pointer/offset comparison and bounded unknowns. Only this new evidence directory was written. No production/Git/index/query/compiler/runtime/app/process/queue/Chirp actions occurred; prior evidence remained unchanged.
