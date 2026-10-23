# Original app ordinary-tab delegate witness scout

**Protocol and two static witness targets identified; actual dynamic receiver remains UNRESOLVED.** No saved constructor/assignment binding was established for the specific ChromiumBrowser callback. Exact source-level Swift requirement spelling is stripped. This is a bounded static identification, not proof that ordinary callbacks necessarily reach one class, persistent tab storage, or a restored/closed runtime state.

Original app168 arm64: `../arc/bin/Arc.arm64`, freshly hashed **4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682**. Scope is saved decompilation/metadata/symbol maps plus read-only Mach-O protocol/conformance/pointer parsing. Only this evidence directory was written. No Arc source/tool/index/Git, compiler/query/build, app/runtime/probe/UI, signal, queue, lock or Chirp action. Existing E17/Arc122 source hold is respected. Parent owns the separate WebContentChromium field/initializer/disposal proof; it was not repeated here.

## Callsite protocol and static targets

Saved swift-types.swift7593..7602 names ChromiumBrowser.browserDelegate as **ADK.BrowserDelegate?**. Saved helper10052dfe0 weak-loads that stored receiver and obtains its associated witness pointer, then invokes witness+8 at the supplied callsite10052e168. A weak-load and witness call identify a protocol existential, not its installed dynamic concrete class.

The original binary independently names protocol descriptor **103c73d5c** as moduleADK/nameBrowserDelegate, with1 class-layout signature requirement and15 protocol requirements. Saved protocol88400..21 has stripped method names. Witness+8 is its first callable requirement (table+0 is the conformance descriptor). Two positively identified same-image conformances are:

| Concrete conformance | Descriptor | Witness table | Witness+8 |
|---|---|---|---|
| ADK.ADKBrowserDelegate : ADK.BrowserDelegate | 10363b388 | 104187fd8 | **1004eba88** |
| ADK.SessionRestorationBrowserDelegate : ADK.BrowserDelegate | 103642ac8 | 10418f348 | **100555d48 → 10055a3bc** |

`witness-pointer-proof.json` validates actual **DYLD_CHAINED_PTR_64_OFFSET format6** from the containing segment's fixup metadata. Both entries are nonbind same-image target36 offsets; add preferred base100000000. Raw words are retained. The Session target's exact instruction word1400119d is an unconditional direct branch to10055a3bc, corroborating functions.tsv's saved thunk name. This avoids treating high chained-pointer bits as a literal function address or relying on a low-bit masking guess.

These are positive concrete tables; they are **not an exhaustive dynamic conformance census**. The metadata index scan skipped unrelated/imported/unsupported descriptor forms. No inference that no third conformance exists is made. Saved symbols/maps do not supply an unstripped Swift requirement identifier; address+slot identity is exact, source-level method spelling remains unknown. Semantic role is the ordinary new-web-content notification from the supplied ObjC callback, corroborated by the concrete action below; it is not renamed to a guessed Swift selector.

## Only two receiver bodies inspected

**ADKBrowserDelegate witness1004eba88**, saved0x1004.c184429, constructs an enum payload on its stack, writes incoming param3/param4 and the flagparam5, sets multi-payload tag0, retains the incoming object and calls100586f60 with that payload. Saved metadata6934ff identifies ADKBrowserAction's tag0 case as **onNewWebContentCreated(WebContent, shouldSelectTab)**, and ADKBrowserDelegate's saved field is a lazy BufferingSubject<ADKBrowserAction,Never>. This proves action forwarding by this implementation. It does **not** prove persistence in a concrete receiver field or subscriber: the subject/helper/subscriber graph was deliberately not expanded, and the retain alone is not treated as storage proof.

**SessionRestorationBrowserDelegate witness100555d48→10055a3bc**, saved0x1005.c74420, contains a conditional receiver-storage path. After browser-ID extraction/tracked-browser checks and diagnostics, it opens modify access to its named **unreceivedRestoredContents** field. The saved field type is dictionary BrowserID → array WebContent. It finds/creates that dictionary entry, ensures array capacity, increments array count, then explicitly stores **param3 at element+20 and param4 at element+28** into the resulting array storage, ends access, and retains param3. This is concrete storage, not retain-only inference: the incoming existential pair is written to dictionary-backed array memory reachable from the receiver field. Under the supplied caller's WebContent argument mapping, it stores the delivered WebContent pair on that branch. Other early-return/diagnostic paths exist; this is not unconditional storage or evidence this receiver was selected in the ordinary-tab callback.

The two complete saved bodies are copied without edits in saved-receiver-functions.c.txt, with metadata excerpts separately. No constructor/disposal routines or additional downstream receiver functions were traced. In particular, no constructor/assignment connects either concrete witness table/instance to this ChromiumBrowser's weak browserDelegate at the relevant epoch. Neither a conformance declaration, class field declaration, buffer name, payload retain nor the Session conditional store fills that binding gap.

## Handoff boundary

Actual dynamic receiver/class at10052e168: **UNRESOLVED**. Exact Swift method source spelling: **UNRESOLVED/STRIPPED**. Static protocol/table/entry addresses above: positively identified. Conditional Session receiver storage: positively identified, not attributed to the running ordinary callback. Ordinary ADK delegate persistence downstream of its action send: **UNRESOLVED** in this scout.

The next binding question belongs to the already-asked app/source owner: a saved constructor or exact browserDelegate assignment with the relevant concrete object+witness table. This report does not repeat the parent's ArcWC/webView strong field, release/destructor or extraction proof, extend the Swift graph, or infer persistent runtime ownership from retain/release counts. No runtime/activation/source change is requested.
