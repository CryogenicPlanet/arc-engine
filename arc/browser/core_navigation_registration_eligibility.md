# Navigation policy registration eligibility — independent original audit

Result: full11's unconditional Navigation synchronous accounting scope has its OWN proven missing-registration eligibility gap. This conclusion comes from original navigation setter, callback bridge, actual policy InvokeSync, both native producer call sites, and original-delegate forwarding; it is not inferred from UI/PDF.

Read-only static audit 2026-10-23. No source/stage/shared/native edit, compile/build/app/run/attach, lock, board or credential action. Only /tmp evidence. No public Close activation or complete policy-disposition implementation is proposed.

## Inputs

Original arm64: ../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore, imageSHA256 b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d. Static unslid addresses below.

Compared composed full11: logs/wp/core.beforeunload-transport/manifest.json -> /var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/core-beforeunload-transport-sgiwiq2d/stage/arc/api/arc_web_contents.mm, SHA256 b6a66bacc567827d9507904f5058d916201cbe1a0f5a968c090559fec70032c1.

Public metadata arc/tools/arccore-1.170.0-objc.tsv3169: -setNavigationDelegate: v24@0:8@16. Stored ivar3130 is shared_ptr<ArcWebContentsNavigationDelegateBridge>, offset152=0x98. Specs/ArcWebContents.md133 identifies original IMP4440dec; private shared aggregate getter/store at194..195. Protocol research/.../protocol-specs/ArcWebContentsNavigationDelegate.md17/31 independently records the REQUIRED synchronous q32@0:8@16@24 callback and unsigned0..11 range check.

## Registration setter and bridge identity: independently recovered

Fresh child evidence /tmp/core-navigation-registration-setter.md + .raw decodes actual metadata/classrefs/selectors, not an analogous UI layout:

- Every nonnil public assignment4440dec allocates a NEW48-byte shared allocation at4440e18..1c, even same app delegate. Bridge object vtable da88fe0 atallocation+18; weak facade atbridge+8, weak ObjC delegate atbridge+10. Facade STORED strong pointer/control98/a0 via4443f3c. Old stored strong owner is released; in-flight selected strong owners can keep old bridge alive.
- Publicsetter forwards4345ad8 ->4375b40. Native ACTIVE selected pointerImpl+3a8 stored4375b68, ACTIVE weakcontrol+3b0 stored4375b70. These are weak-control+10 operations, not a native strong registration owner.
- Nil public4440f74..f98 only gets/releases a temporary stored bridge copy; it does NOT clear98/a0 or edit stored bridge's weak delegate. It then clears native ACTIVE pair3a8/3b0 via4375b40 at4440fb0. Later nonnil creates a new bridge. Thus a cleared weak ObjC delegate and an explicitly unregistered native bridge are distinct cases.
- Registration setter does not change neighboring syncbyte3b8 or pending3c0; it does not reset/drain counts.
- Exact bridge virtual slotda88fe0+10 =4445a58. It weak-loads/retains ObjC delegate4445a78, then facade4445a84; constructs ArcNavigationAction through initWithNavigationAction:4445aa0; DIRECT objc_msgSend webContents:decidePolicyForNavigationAction:4445abc. No respondsToSelector/nilself/nildelegate conditional branch precedes action allocation/message. Unsigned return must be<12 at4445ac0/c4 or trap4445af8. Releases action, facade, delegate only after callback result.

A native registration can therefore remain live while its weak ObjC delegate has expired: producer still enters native busy scope, constructs ArcNavigationAction, nil ObjC message yields0, range check accepts0. A live delegate can receive nil facade if its weak facade has expired but the registration survives independently. Unsupported selector is not filtered by native bridge; protocol declares this method required. No recommendation to crash or alter invalid-client tolerance is inferred here.

## Actual navigation policy InvokeSync, not UI InvokeSync

Fresh /tmp/core-navigation-registration-producer.raw contains4382d08..2fdc.

Receiverx0 is the Navigation registration invocation wrapper. Key control flow:
1. Dispatcher/lifetime prefix4382d6c..2e00 runs before selected bridge acquisition: captures wrapper guard and logical shared pair into bookkeeping/no-op closure and dispatches it. Do not conflate this with native busy scope or delete it based on an empty selected bridge.
2.4382e04 loads ACTIVE selected control[this+8]. Null ->4382f24. Expired strongcount=-1 at2e14..18 ->2f24. CAS2e28 acquires selected STRONG owner.
3.4382e38 loads/tests ACTIVE raw selected[this]. Null ->2ecc. Only successful control lock plus nonnull selected raw proceeds.
4.4382e5c saves previous byte[this+10],2e64 sets it true.2e80 fetches selected bridge vtable+10 and2e90 synchronously calls that exact bridge.
5.2eac restores prior byte after callback; selected control strong lock releases2ed8 afterward. Null/expired cases return integer0 at2f24 or2ecc WITHOUT touching busy byte.
6. This sync producer does not increment pending counter[this+18]. Native policy busy is conditional save/set/restore, not an unconditional counter or an async pending acquisition.

This is a separately instantiated navigation InvokeSync with the actual DecidePolicyForNavigationAction diagnostic string4382fa4..fac. It is not UI InvokeSync4383ad8.

## Both actual native producers and original forwarding

A full original __text direct B/BL scan found exactly two calls to4382d08: BL438266c andBL4387d34. Indirect entrants are not exhaustively excluded by this direct-branch scan. Both callers explicitly pass Impl+3a8:
-4382660(receiver3a8) ->438266c.
-4387d28(receiver3a8) ->4387d34.

Their earlier native routing has additional source-specific prerequisites; these must not be silently generalized to every navigation observer or new throttle event:

ProducerA starts4382370 and has an eight-argument native operation shape. At43823b8..23c8 it checks ACTIVE NAV control3b0 exists/not-expired;43823cc..23dc separately checks ACTIVE UI control350 exists/not-expired. Missing/expired either takes4382450, which forwards original native operation through proxyImpl+28 ->4350de4 at4382470 and returns without reaching navigation InvokeSync. Additional URL/disposition route filters are present and not fully remapped by this audit.

Forwarding helper4350de4 checks proxy WeakPtr+30 and original delegate pointer+38, calls original delegate virtualslot20 at4350e34..e40 if valid, otherwise cleans its incoming native-owned parameter as shown4350dfc..e0c. This route is not an empty Arc navigation policy bridge. It does not mark Navigation busy.4353534 stores this proxy original-delegate WeakPtr/pointer atproxy+30/+38, corresponding toImpl58/60 because proxybase=Impl+28.

ProducerB starts4387780 and has a three-parameter native operation shape afterthis. ACTIVE NAV/UI checks are4387934..7960, with additional nonnull source/helper conditionsx24/x25. Failure branches4387a6c and checks Impl58/60 original-delegate WeakPtr/pointer;4387ac0..ad8 invokes original native delegate virtualslot0 with original parameters and returns its result. It does not call4382d08 in that fallback. Other special URL/routing gates and successful nonzero policy disposition adaptations remain outside this bounded eligibility audit.

Thus original absent native registration commonly bypasses policy invocation entirely and forwards native behavior. If generic InvokeSync is entered but selection later fails, its earlier bookkeeping still exists before returning0. Those two stages must not be collapsed into a universal “nil means no task at all” rule.

## Relationship to public Close's Navigation domain

PublicClose43785f0..85fc checks Impl+3b8 syncbyte OR+3c0 pending. Actual callerreceiverImpl3a8 plus InvokeSync[this+10] proves byte3b8 is exactly the sync Navigation domain checked by publicClose, rather than a similarly shaped UI field. Existing diagnostic literal navigation_delegate_wrapper_ at4378638 is additional independent evidence.

No other source-domain gate, browser bulk-close guard, policy disposition default, or Close source continuation is derived from this result.

## Full11 gap and minimally correct correction strategy

Current full11 WC256 constructs ScopedCoreCloseInvocation(...kNavigation) unconditionally BEFORE current weak navigationDelegate lookup258 and respondsToSelector branch259..261. Setter825 merely assigns _navigationDelegate weak; there is no policy bridge STORED owner / ACTIVE weak identity model. Consequently:
- Explicitly unregistered/nil receipt creates a Navigation busy scope in full11 although original native producers bypass policy or generic skips busy.
- A live selected bridge with expired ObjC delegate is collapsed into the same current weak-nil case; original still counts busy and constructs action, while full11 skips action and returnsPROCEED. Raw native integer0 is proven, but this audit does NOT assign a universal C156 throttle result to it.
- Moving the scope after respondsToSelector would fix neither identity nor expired-target eligibility; it would incorrectly suppress native busy for a live selected bridge with expired ObjC target.

Use the real policy registration selection in the functional logical invocation wrapper: preserve public fresh/non-nil, retainedstored-on-nil and ACTIVE weak unregister semantics; acquire selected strong bridge at policy receipt, validate nonnull raw; only accepted selection enters Navigation busy scope. Hold selected bridge throughout synchronous bridge call, previous-byte restoration and return. Resolve weak ObjC targets inside that selected bridge using the original retained target lifetime; do not retarget to a newly registered/current delegate. Distinguish this POLICY registration from CoreNavigationRegistration in arc_navigation.mm364+, which represents navigation OBSERVERS and a separate accounting channel.

Preserve source-specific original-delegate forwarding and any original pre-lock logical-self/guard bookkeeping. Correct current original/C156 producer eligibility/filter/disposition translation only after their independent source contract is mapped; do not add a blanket ACTIVE UI gate to all navigation callbacks simply because these two original native operations check UI registration.

Eligibility-only correction is source-backed, but complete replacement remains HOLD for functional logical owner, policy registration bridge lifetime, source routing and dispatcher guard integration. No isolated implementation or activation is supplied. No golden/runtime count repair is claimed.

## Evidence and limits

Fresh producer raw /tmp/core-navigation-registration-producer.raw: exact native navigation InvokeSync, callerA gate/receiver, callerB receiver, original proxy forwarding.
Fresh forwarding raw /tmp/core-navigation-registration-forwarding.raw: callerB gate/fallback, exact proxy registration stores4353534, publicClose Navigationbusy fields.
Fresh setter raw /tmp/core-navigation-registration-setter.raw and report.md: metadata, setters, bridgevtable, weak target/directselector and default0 source.
Existing /tmp/core-wco-full-native.txt was used to locate complete surrounding native operations; essential eligibility branches were freshly disassembled above.
Direct B/BL scanner read only original __text via /tmp/action-native-scan.py and found438266c/4387d34; no runtime values or zero-fill data read.

Input/evidence SHA256:

- /var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/core-beforeunload-transport-sgiwiq2d/stage/arc/api/arc_web_contents.mm: b6a66bacc567827d9507904f5058d916201cbe1a0f5a968c090559fec70032c1
- /tmp/core-navigation-registration-producer.raw: ef6b2087a8fd7a247e234c18240c31b8aff119d2cda3d8b50a0c85fe5442ccd5
- /tmp/core-navigation-registration-forwarding.raw: 76fdcbb57521f9266ee0f40f03587cae27b2a73afba3e00d04ad7a03e2343d3d
- /tmp/core-navigation-registration-setter.raw: 2c385e39fc80e2fd59ca5b72bd7cf633c380564c148166b83e7bf4d4c8a6f037
- /tmp/core-navigation-registration-setter.md: 8e2985ca7fccce49c9272fd9329fa9557f1292360f5761993d7d623c5fc1a9c7
