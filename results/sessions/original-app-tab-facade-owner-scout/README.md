# Original app ordinary-tab facade owner scout

**A concrete app-owned facade field and matched destructor release are proved. Persistence of the containing app wrapper beyond the callback is still unproved.** No Root ownership adapter or activation recommendation follows.

## Exact receiver and image

Saved app callback metadata identifies ADK.ChromiumBrowser (`_TtC3ADK15ChromiumBrowser`), selector browser:didCreateNewWebContents:isActive:, IMP100529a48, encodingv36@0:8@16@24B32 (abi/initial/app-callbacks.tsv48; protocols/initial/evidence/app168-implementations.tsv65 and app-contract-joins.tsv54). Its saved body is protocols/initial/bodies/app-0x100529a48.c, indexed by app-body-index.tsv58. The metadata/body alone previously established the receiver and immediate callee10052dfe0, not storage/disposal or boundary execution.

Exact original app168 binary `../arc/bin/Arc.arm64` is79,511,616 bytes, SHA4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682, matching saved input-pins.tsv2. This is the APP binary, not B70 ArcCore. The tested trace156 executable is79,509,184 bytes, SHA155155b7ab879a0692f1a0dc691202d901b48ab8593afb60b1a91c519f302d5a. Both images' LC_FUNCTION_STARTS independently delimit all four complete relevant functions; their same-VA bytes are equal:

| Span | Bytes | SHA256 |
|---|---:|---|
| 100529a48..100529ac0 callback |120|383a7b4e231f0136543daa1b6b9e861a3414cd0af80a11a3898988dfa410ec7d|
| 10052dfe0..10052e19c helper |444|a71344b84abfbcb635717ade5d44824a30333fa4924b463a2616e8aad5aec927|
| 10054683c..100546b04 initializer |712|cd8e1252722d16a45e188254a8de7497693415246a72281158b2a0a336659503|
| 10053bbdc..10053bcd8 .cxx_destruct |252|02274389fc488f1c705ffc6a073eb3ef30e35970f62ae814a94c23c89a9ca846|

Referenced ivar-offset cell1044cc1c0 is0x68 in both images; relevant indirect imports resolve to the same objc_retain/release/allocWithZone/msgSendSuper2 names in both. This qualifies these exact code/data/import edges for the trace App despite full-image difference. It does not seal the whole App, all callee bodies/Swift witnesses/metadata, the mod's hooks or an end-to-end runtime owner graph.

## Positive storage and matched disposal

Callback100529a48 forwards incoming ArcWebContents/active to helper10052dfe0 with the Swift self register. Helper weak-loads the browserDelegate, and on the live-delegate branch allocates an app WebContentChromium wrapper (metadata accessor10053b838; allocWithZone10052e108), retains supplied contents10052e110..118, and calls initializer10054683c at10052e12c.

Initializer captures incoming contents in x19 and app wrapper self in x21 (100546854..85c). It loads ivar-offset cell1044cc1c0 at1005469dc and **stores that exact supplied facade to app self+0x68 at1005469e0**. Saved app168 class layout independently names decimal104/0x68 `WebContentChromium.webView`, size8. An explicit objc_retain at100546a04..08 supplies ownership in the initialized app object. The initializer sets actual delegates/observers through the stored facade and returns self at100546ae8..b00.

The named app `.cxx_destruct` IMP10053bbdc loads the SAME offset cell at10053bc60, loads self+0x68 at10053bc64, then **calls objc_release at10053bc68**. This closes the field ownership/release pair. It is stronger than an isolated retain around a callback. The saved decompiler excerpt notably omits this webView release because of its ARC/injection representation; the exact arm64 instructions and Mach-O import names establish it.

## Exact persistent-owner boundary left unresolved

Helper10052dfe0 contains no direct store of the new app wrapper to a persistent field/dictionary. It passes the app wrapper in x2 to the selected browserDelegate Swift witness+8 at10052e168, then releases its own local wrapper at10052e174..178. Weak browserDelegate loading/temporary retain is not a persistent wrapper owner. The downstream witness implementation and its eventual storage/disposal have not been positively identified in this bounded scout. If it keeps no owner, the wrapper and its facade field can die as locals unwind; if it retains the wrapper, this proven field provides a genuine app→facade strong edge for that owner's lifetime. Neither outcome is inferred from retain counts alone.

Thus the positive result is `WebContentChromium --strong webView--> supplied ArcWebContents`, with matched app-object destruction release. Together with Core's already pinned facade→logicalRoot shared handle this establishes a possible app-owned Root path. It does NOT establish who retains WebContentChromium beyond this callback, ordinary-tab adoption/disposal timing, sole persistent ownership, all creation/close cases or equivalence to current C156's canonical native-userdata policy. The next precise missing question is the concrete target/storage/disposal reached by witness+8 at10052e168; no arbitrary Swift graph chase was performed.

## Existing evidence and design request

Core's persistent-owner-v2 report was read fully; it explicitly leaves this app receiver/storage edge open. The saved protocol/ABI evidence already supplied the exact receiver, encoding and callback body, but did not prove the field store/matched release. A bounded search of inspected App worker reports/artifacts found no prior exact10054683c/10053bbdc storage-disposal report; that is not an exhaustive absence theorem about all App work. Saved request144628 asks permission for an explicit current-C156 native-userdata→canonicalfacade→sameRoot adapter while acknowledging original ordinary ownership remains unproved. This scout neither grants that design request nor substitutes a hidden native owner. It narrows the original evidence gap as above.

Only this new evidence directory was written. Static reads/extraction only; no production/Git/index/compiler/build/runtime/UI/attach/query/lock/queue/process/Chirp/credential actions. Independent focused reviewer full_probe_review confirms the strong field, raw matched release and unresolved downstream persistent owner.
