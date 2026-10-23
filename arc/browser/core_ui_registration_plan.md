# Core UI registration eligibility audit — original B70 versus full11

Read-only bounded audit, 2026-10-23. No production, staged, native, shared, app, build, lock, board or credential changes. Only /tmp evidence written. This is a correction proposal, not runtime acceptance or activation approval.

Original arm64 image: ../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore; SHA256 b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d. All hexadecimal addresses are static unslid addresses. Offline LLDB target creation/disassembly only; no process run or attach.

Compared full11: logs/wp/core.beforeunload-transport/manifest.json, stage /var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/core-beforeunload-transport-sgiwiq2d/stage. The manifest lists the two transport additions; the stage directory contains the composed core files used below. arc/api/arc_web_contents.mm SHA256 b6a66bacc567827d9507904f5058d916201cbe1a0f5a968c090559fec70032c1. See /tmp/core-ui-registration-audit-seal.json for every input/raw hash.

## Result and exact correction boundary

Five composed UI producers demonstrably acquire accounting without the original selected-registration eligibility gate: synchronous TargetURL, closing-imminent, Closed; asynchronous LOAD-status and visible-security. This is proven independently in each original route, rather than generalized from PDF141394. Registration eligibility is a live selected C++ bridge, not selector support, resolved ObjC weak delegate survival, physical WC liveness, or current navigation generation.

An eligibility-only correction is source-backed and can be reviewed separately. The complete replacement remains HOLD for the functional logical owner/self capture and dispatcher-lifetime guard. Merely making a no-op closure retain CoreCloseState does not reproduce the original logical-self lifetime. Do not activate the partial accounting replacement as equivalent.

| Composed WC location | Original evidence | Proven correction |
|---|---|---|
| NativeTargetURLChanged398 starts UI scope before bridge405 | Forward Chrome4388658..8690; format4388744; weak-lock Impl350 at438881c..884c, test raw348 at8850..54; only then save/set busy358 at8878..80; restore88c4 | Select and strongly lock active bridge before constructing UI busy scope; absent registration skips busy+inner UI delivery. Preserve pre-lock work. |
| nativeContentsClosing743 scope before direct weak delegate745 | Imminent438348c string, receiverImpl348 at3490, InvokeSync43834a0 with slot68 | Apply exact shared InvokeSync eligibility below; use selected bridge, not current ObjC delegate, as gate. |
| nativeContentsDestroyed766 scope before direct weak delegate768 | Nonquit438a1d4..dc; receiverImpl348 at438a22c; InvokeSync438a23c with slot20 | Same selected-bridge gate; preserve independent quit suppression and cleanup. This audit does not change Closed source phase or imminent synthesis. |
| LOAD-status366 weak capture,367 unconditional PostCoreUIDelegateInvocation | Status4385c30..c60 lock Impl350, c64..68 test raw348, only c6c..74 increment pending360 | Positive receipt-selected registration alone acquires pending. Missing registration still queues original-order uncounted no-op with logical-self/guard lifetime. |
| Security443 weak capture,444 unconditional PostCoreUIDelegateInvocation | Security438bd98..bdd0 lock Impl350, bdd4..d8 test raw348, only bddc..bde4 increment pending360 | Same receipt eligibility; retain independent original payload/provider gates as bounded unknowns below. |

Composed helper1012..1017 always calls MakeCoreCloseAsyncInvocation; core_close_state.mm127..133 always AddPending before posting. Consequently nil/unregistered receipt still creates pending even though the later weak-bridge body does nothing. The correction belongs at receipt/adapter selection, not as an early return inside delivery after AddPending.

## Synchronous path: what is and is not conditional

Generic InvokeSync4383ad8 receives the registration wrapper at x0. It first does dispatcher/lifetime bookkeeping at4383b4c..3be0. THEN4383be4 loads selected control[this+8]; absent or expired skips to3cb8; CAS at3c08 acquires selected STRONG owner;3c18 loads/tests raw selected[this]. Only successful selection enters busy code: save previous byte[this+10] at3c3c, set true3c44, invoke selected member3c74, restore previous3c8c. Selected strong lock releases3cb0 after return. This saves/restores nested prior state; it is not a pending increment.

Both imminent and Closed call this exact function; TargetURL contains its inlined equivalent. No app delegate resolves or respondsToSelector test appears before original busy acquisition. A selected bridge whose ObjC delegate has died is still eligible for busy scope, even if bridge inner later does nothing.

IMPORTANT: absence of selected registration does NOT prove absence of all work. Generic3b4c..3be0 and target4388784..8818 perform pre-lock dispatcher/lifetime bookkeeping. Do not implement a blanket skip of the complete producer or all queued work when registration is empty. This audit proves only conditional busy+selected inner delivery. It does not reclassify the pre-lock closure or change native-close continuation.

## Asynchronous path: receipt eligibility and two independent delivery gates

Generic InvokeAsync4378120 first weak-locks selected control at4378148..8188 and tests selected raw437818c..8190; only then increments pending[this+18] at4378194..819c. Exact status and security inline equivalents are in the table above. Status captures its C++ string before registration selection4385c24..2c. Security captures its provider result before selection438bd78..94. Preserve event receipt ordering; no policy/generation queue is implicated.

For positive selection, queued closure captures selected WEAK identity (control weakcount+10 at4378284..8c), payload and logical-self shared pair (sharedcontrol+8 increment4378208..2220). Temporary receipt strong selection is released after constructing/posting. Delivery first checks the separate invocation-wrapper lifetime WeakPtr42c1750..1778. If valid, common42c0840 weak-locks exactly the CAPTURED selected bridge at42c0890..08bc, invokes inner42c08d0 if valid, destroys payload08d4..ec, decrements pending08f4..fc; selected strong lock releases0908 afterward. Expired selected bridge still reaches pending decrement0968..70. Do not redirect to the newly active bridge or drop pending completion merely because selected bridge expired.

For absent/expired/null receipt selection, generic4378344..8414 queues an UNCOUNTED no-op. It still captures the logical-self shared pair at43783e4..ec and increments its sharedcontrol+8 at83f4..fc. Bound member15ac0 is exactly RET. Outer42db590..5c0 checks the invocation-wrapper WeakPtr; if valid5e4..ec moves logical pair to stack,5f8 invokes no-op,5fc..610 releases pair. If guard cancels the body, captures survive until callback/task destruction. This is not equivalent to no task, to a no-op retaining only accounting state, or to a task strongly retaining an ObjC facade.

No native-physical-WC-nonnull check is present in these common delivery layers. The independent invocation-wrapper guard is invalidated by final LOGICAL destructor, not ordinary physical detach/rebind. See separate fresh setter/lifetime audit /tmp/core-ui-registration-lifetime.md.

## Minimal API shape to stage after logical lifetime contract

Use an opaque private selected-registration receipt object rather than passing only CoreCloseState and a conditional block. Concrete names are proposals, not requested new public selectors.

1. A logical invocation wrapper exposes its ACTIVE weak UI registration. A receipt selection temporarily strong-locks that registration and tests its nonnull bridge. Keep facade STORED strong registration separate. Do not add selector/delegate/WC/generation predicates.
2. Synchronous helper accepts the selected receipt plus logical invocation context. Preserve producer pre-lock work; construct UI busy scope ONLY for successful selection, invoke its bridge, precise-hold selected owner through the entire inner return and scope completion. The three synchronous call sites use this helper. Do not replace dispatcher work with a global early-return.
3. Async helper accepts receipt payload plus logical invocation context/guard. Successful selection increments pending, captures selected weak identity, logical-self payload and guard; absence queues uncounted original-order no-op with the actual logical-self capture and same guard. Delivery checks outer guard then strong-locks selected bridge, precise-holds through inner callback and pending completion, never retargets.
4. Keep original enqueue order: LOAD WCO batch first, status UI task next; security remains separate. Eligibility affects UI accounting/delivery only. It must not gate unrelated LOAD isLoading/back/forward WCO notifications.
5. Preserve nonnil replacement and nil semantics: every nonnil creates a fresh registration; nil clears active weak registration while retaining facade's last stored bridge. Already selected old bridge remains eligible while any old strong owner survives, including an in-flight delivery. No current-registration/generation equality check.

Existing CoreUIDelegateBridge can represent bounded stored/active selection semantics, but it is not by itself the functional native logical owner or outer guard. LOAD status369 also needs an explicit precise-lifetime/shared helper contract: native selected strong owner lasts through inner return and pending decrement. Security446 already uses objc_precise_lifetime. This is a source lifetime invariant, not a claim about unrun compiler codegen or observed early release.

## Producers outside this proven UI correction

- Generic URL339, TITLE347, LOAD353, favicon377, traits384, progress424, visibility432, focus helper, theme458, zoom482 and history updates are WebContentsObserver-domain batches. Display helper core_render_state.mm255/306 also uses PostCoreWebContentsNotification only. PDF UI eligibility proof does not license UI registration gating on these WCO setter groups. Their logical observer accounting/owner and registration contracts remain separately reviewed.
- Initial setter group690..715 predates observer creation716 and remains direct/uncounted in current stage; this audit does not change its already separate native producer proof.
- Navigation policy256 uses Navigation-domain synchronous scope; navigation observer delivery lives in arc_navigation.mm. No UI-selected registration inference applies. Its independent registration/domain audit remains separate.
- Cross-package PDF/UI/dialog/menu/media producers require their own original route evidence. PDF141394 is independently proven and remains owned by webextras. No universal gate has been inferred for all engine-to-app callbacks.

## Other exact bounded unknowns; do not silently fold into the fix

Original LOAD has an earlier nonnull userdata/helper gate4385ae0..afc before BOTH WCO loading/status. Current CaptureState526..528 returns empty status if named CoreTabHelper is missing but still posts branches. Exact original userdata key/type has not been independently established here, so do not assert this is exactly a missing-CoreTabHelper defect or conditionally gate current behavior from the address alone. Record as a source-mapping prerequisite only.

Original visible-security438bcd4 requires managed+130 nonnull and helper1383300 result nonnull at438bcf8..bd04 before work. That returned pointer/shared payload semantics remain unclassified in this audit; current simplified GetSecurityLevel snapshot is neither proven equivalent nor proven wrong here. Receipt registration correction does not resolve that provider/guard gap.

Current close source/phase, physical reset before Closed, synthetic imminent, and owner teardown are intentionally not corrected by this report. Parent's logical/source-token plan owns them. Full11 was only read; no new runtime comparison was run, no counts claimed repaired, no acceptance claim.

## Evidence files

- /tmp/core-ui-registration-sync-raw.txt: fresh complete InvokeSync4383ad8, target4388630 and Closed438a18c bodies.
- /tmp/core-native-close-entry-guard-raw.txt: original imminent sender438348c..34a0 and complete source close entry; reused static raw.
- /tmp/core-ui-registration-async-audit.md and /tmp/core-ui-registration-raw-{load-and-invoke,async-detail,load-security,status,payload,noop-lifetime}.txt: fresh focused async producer, common dispatcher and no-op body traces.
- /tmp/core-ui-registration-lifetime.md + .raw: independent sibling fresh setter/registration/outer-guard lifetime proof; no PDF generalization.
- logs/wp/webextras.pdf-registration-review.md: independent PDF141394 original gate/bridge proof used only as the audit prompt, not a universal rule.
- /tmp/core-ui-registration-audit-seal.json: exact stage/input/evidence hashes for archiving.
