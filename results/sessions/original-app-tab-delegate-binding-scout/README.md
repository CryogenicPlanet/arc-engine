# Original app ChromiumBrowser delegate binding scout

**A concrete conditional ADKBrowserDelegate object+witness assignment route is proved. End-to-end selection of that route for a particular ordinary callback remains unresolved at the incoming Browser existential.** This is more than conformance existence, but not a universal/runtime binding or persistent owner claim. No subject/action-storage tracing or activation recommendation.

## Initializer and exact weak assignment

ChromiumBrowser initializer100525930 initializes browserDelegate object slotself+72 to nil and witnessself+80 to0 at10052597c..990. Its supplied arguments include native ArcBrowser/context-history bridge; it does not install a concrete browserDelegate. Saved source0x1005.c34048–50 agrees. The public Swift browserDelegate setter100524c24 passes offset-cell1044cbde8 (=72) to100524dc4; the generic helper stores incoming witnessx1 atself+80 (100524dfc), weak-assigns objectx0 toself+72 throughswift_unknownObjectWeakAssign (100524e08), then releases its temporary. No strong browserDelegate ownership is inferred.

ADK.Browser's ChromiumBrowser conformance table10418c868 has setter slot+0x28 at10418c890, resolving100528c38 under verified DYLD_CHAINED_PTR_64_OFFSET format6. This thunk passes the SAME browserDelegate offset-cell viax4 to100528dd4. That helper performs the corresponding witness store100528e0c and weak-object assignment100528e18. These are actual mutation paths, not just protocol declarations.

## One positively identified delegate creator and its attach caller

Getter/creator10133e674 returns a cached delegate object when its dictionary lookup succeeds (10133e6c8..6e4). On cache miss it calls metadata accessor1004ee120 at10133e75c, allocates at10133e760 and initializes at10133e780 through1004eb7b8. Metadata accessor returns class10447dcc0; its actual classRO10447dc60 names **_TtC3ADK18ADKBrowserDelegate**. Constructor1004eb7b8 initializes that class's lazy actionSubject, extension-group lookup and restore callback, then super init. Its result is passed through dictionary-update machinery and returned; this scout does not trace the cache helper/disposal or the subject graph. The cache-hit branch does not allocate a fresh object.

One attach routine10133f508 preserves its incoming ADK.Browser objectx0 inx21 and Browser witnessx1 inx19 (10133f534/530). On the inspected branch it calls10133e674 at10133f7cc, receives the concrete delegate object inx0, then **sets x1=104187fd8** at10133f7d4/7d8 and invokes incoming Browser witness+0x28 at10133f7e8, with Swiftselfx20 set to the Browser object. Table104187fd8 is the already positively identified **ADKBrowserDelegate : ADK.BrowserDelegate** conformance, not SessionRestorationBrowserDelegate's10418f348. The intervening instructions leave the creator's resultx0 intact. Thus an actual object+concrete witness pair is passed to the Browser setter, not merely listed in a conformance inventory.

**If incoming Browser witness is10418c868**, this call reaches100528c38→100528dd4 and writes that concrete delegate object/table into ChromiumBrowser's weak browserDelegate pair. The selected callback helper10052dfe0 subsequently weak-loads that same pair and calls its witness+8, as proven in the frozen prior scout. This establishes the conditional static route.

The outer caller supplying10133f508's Browser object/witness is not identified in this bounded slice. Nor is every path through10133f508 this branch: the raw predicate10133f7a4..7b4 may branch elsewhere; its numeric conditions are not renamed to guessed restoration/incognito semantics. The separate restoration handoff reference to104187fd8 in1013564cc was only a locator and is not treated as ordinary-browser binding proof. We do not claim that conformance existence or app creator return shapes alone resolve the outer dynamic existential. This is the exact remaining targeted binding boundary.

## Image applicability and scope

Pristine original app168 `../arc/bin/Arc.arm64` SHA4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682 and actual trace App executable SHA155155b7ab879a0692f1a0dc691202d901b48ab8593afb60b1a91c519f302d5a differ as full images. All nine complete functions inspected here have identical same-VA bytes and independently decoded LC_FUNCTION_STARTS endpoints in both images; manifest.json records all spans. The relevant offset cell, Browser setter witness slot and ADKBrowserDelegate class data/name also agree. This does not seal the whole App, all callees/witnesses/mod hooks or a runtime path.

The frozen facade-owner/witness reports remain unchanged. App→WebContentChromium persistence and factory100586f60's subject/action storage are outside this task and remain owned by the separate factory worker. This report neither extends their claims nor activates a Root adapter. Independent focused review confirms the setter/creator/conditional-attach dataflow and unresolved incoming Browser-witness selection.

Only this new evidence directory was written. No production/Git/index/compiler/query/runtime/UI/apps/queue/lock/process/Chirp actions. Original binary reads and saved metadata/decomp were used; no private layout was fabricated.
