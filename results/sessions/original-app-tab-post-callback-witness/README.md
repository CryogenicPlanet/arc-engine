# Saved original post-callback app facade witness

**The accepted saved L1 record shows the same app WebContentChromium receiver and ArcWebContents facade continuing after the original didCreate callback body returned. The pinned recorder's actual tagging/forwarding code does not supply a persistent object-retaining identity map. This is a concrete observed lifetime slice, not identification of the ultimate owner, universal ordinary-tab ownership, autorelease-pool drainage or an activation decision.**

## Exact saved episode

recorder-upgrade.json SHA5bbad7ff93b52b5eff66108ad868f53591ab5ba6643b68d8361a5fc416d060e7 pins recorder fee95e1339e8c1aa2ecf1fcb0e9ebf1137efe64193031b60ead9c110677bba9a and both accepted launch raws. All eight receipt artifact hashes currently match. L1 record3cdfe7c8… has7410rows; inspected seq equals line minus1. This is an existing saved receipt association, not a newly recorded run or a whole-build freeze.

- seq3034/main/depth1: engine→app `ChromiumBrowser#94 browser:didCreateNewWebContents:isActive:` receives Browser#89, ArcWebContents#118, true.
- nested3053→3054/main/depth2: app→engine `ArcWebContents#118 setUIDelegate:` installs WebContentChromium#126.
- seq3599: didCreate recordedRET. The original invoked body has already returned at this point (see recorder ordering below).
- **32 subsequent engine→app callback CALLs** have the SAME recorded receiver WebContentChromium#126, allmain/depth1/fromengine, and all first arguments ArcWebContents#118. First3612→3613, navigation3700→3701; imminent5280→5321; didClose5322→5705; PiP5706→5707.
- app→engine removals5712→5713 and5716→5717 pass the same#126 to helpers obtained from WC#118 by5710→5711 and5714→5715. These are observer-removal observations, not a dealloc/last-release assertion.

selected-rows.jsonl preserves the exact selected raw fields; episode.json records all32 callback selectors/sequences/arguments. No getter value or hidden storage is inferred without a row. L2 raw71760b4e… is receipt-hash-verified only; no second causal episode is generalized from it.

## Actual pinned recorder identity and lifetime mechanics

The inspected actual `tests/recorder/ArcEngineRecorder.dylib` is exactly fee95e13…, 92,224bytes. Current source SHA9ae39687… is separately pinned; source identity alone is NOT treated as binary equivalence. Actual Mach-O local symbols, function starts, indirect import names and arm64 instructions establish the following selected paths:

**Describe3238..4120** retains the described object only in stackslotsp+0x340 (`objc_storeStrong`3270). Generic-object tagging reads objc_getAssociatedObject at3f4c. If absent, it atomically increments numeric global gNextObjectId atc2a0 via3f8c, creates NSNumber with numberWithUnsignedLongLong: at3f9c, and calls objc_setAssociatedObject at3fd4 with **objectx0, keyx1, NUMBERx2, policy1x3** (3fc0..d4). It stores the number ON the described object, not the object in a global identity map. It formats a class/tag string; described-object stack ownership is cleared40d8..40e0 before the description-string tailreturn411c. Successive fresh associations consume new numeric IDs; same recorded#126 is supported as same tagged object, not a reused-address lookup in a retaining dictionary.

**RecForwardInvocation174c..1f2c** holds targetself and NSInvocation in stack stronglocals (1778/1794), invokes the aliased original method at1d54, restores selector/decrements recording depth and emits the RET at1e98. It then clears its strong locals, including invocation at1f08 and self at1f14, before actualreturn1f24. Its complete inspected body contains no retainArguments message call. Argument-description arrays/common values hold formatted representations rather than an observed-object identity registry in these selected paths. No persistent retention of#126 is established by this recorder code.

Concrete timing limit: recordedRET is AFTER the underlying app method body returned but BEFORE forwarding-thunk cleanup/actualreturn and the original engine bridge's eventual unwind. Accordingly “post callback” here means post original-body return/recordedRET; it is not proof that all instrumentation/engine stack frames or autorelease pools have drained. The recorder does make transient stack strong holds during each instrumented call. This does not erase the positive later same-object callback observation, but prevents turning it into an uninstrumented final-lifetime theorem. Foundation internals and a whole-recorder reproducible-build equivalence were not chased.

## What this adds to the strong-field proof

The frozen app-field scout independently proves WebContentChromium initialization strongly stores supplied ArcWebContents in webView/self+0x68 and `.cxx_destruct` releases that same field. The actual four functions and relevant data/import edges are byte-qualified against this trace App. The saved runtime episode now adds **observed survival of that same app container and facade beyond the original callback body**, through further callbacks and closing cleanup. It does not require choosing the ultimate parent container to establish that observed liveness.

Saved Swift metadata explicitly declares **var webView: ArcWebContents** (swift-types.swift7796), not let/immutable. This audit does not prove a complete writer closure or assume the field is unchanged at every later instant merely from its name. Later callbacks' WC#118 argument pairing and live#126 identity are direct observations; the ultimate receiver/storage chain, any field replacement and final release are separate source questions. The preexisting facade→logicalRoot strong edge supplies the structural ownership interpretation while the handle is unchanged; no Root pointer is tagged in these raws. This is observed App-container/facade persistence supporting that interpretation, not proof of sole or universal original Root ownership or permission for a current C156 userdata adapter.

Independent fresh raw review confirms the32-event episode and all receipt hashes. Raw recorder code and call-target evidence are retained for separate review. Only this new directory was written; no source/Git/index/compiler/query/runtime/UI/apps/queue/process/lock/Chirp/newrecording actions occurred. The rejected runtime probe was neither retried nor rephrased.
