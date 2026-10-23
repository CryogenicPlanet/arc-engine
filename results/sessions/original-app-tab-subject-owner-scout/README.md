# Ordinary ADKBrowserDelegate → lazy subject ownership scout

**The actual getter has a concrete managed strong cache store/retain/previous-value release. Its one inspected constructor initializes the new subject's buffer flag to1.** This closes that local owner link and constructor-state boundary only. The installed callback receiver, actual callback flag, final delegate/subject lifetime and complete Root-owner graph remain **UNRESOLVED**. Renderer85455 capture/source hold is respected; no Root activation recommendation.

Only getter **1004eb4f8** and exactly one constructor **100586ecc** were inspected. No destructor, publisher/subscriber/event-consumer graph or further accessor body was opened. Only this evidence directory was written; no source/tool/Git/index/compiler/query/build/app/runtime/UI/queue/Chirp/lock/process action occurred. Prior witness, raw-addendum and buffer packets remain unchanged and sealed.

## Getter: explicit managed cache and owned return

The prior raw witness packet already establishes the ordinary receiver's direct call at1004ebae0 to **1004eb4f8**. This packet inspects that exact getter, not a guessed similarly named property. Saved metadata names ADKBrowserDelegate's lazy actionSubject as BufferingSubject<ADKBrowserAction,Never>; the exact raw owner-field offset is independently loaded from **1044c98c8 =8**, equal in pristine/trace files.

- **1004eb508..514** loads the field offset, reads `[receiver Swift-selfx20+offset]` and branches on null.
- Cache hit **1004eb518/568** retains that cached pointer and returns it at1004eb56c; it does not allocate or replace the field.
- Cache miss saves the receiver inx19, obtains generic metadata using the bounded cache/type-reference operands, allocates via actual `_swift_allocObject`1004eb540 and invokes the **single constructor100586ecc** at1004eb548. That constructor returns the same object pointer inx0; the accessor helper/type instantiation internals are unexpanded.
- **1004eb550** reloads previous owner-field value intox21. **1004eb554 stores the newly returned subject into `[receiverx19+offsetx22]`**. **1004eb558 retains the new subject**, then **1004eb55c..560 releases the previous field value**. On this path the common retain receives zero after1004eb564; returnx0 is the new subject saved inx20.

Together with the owned allocation/returned retain, this is managed strong cache assignment: the new object has field ownership and an independently owned getter result, with the prior slot value released after replacement. It is not merely a pointer retained for a single callback, and no weak-store operation appears in this getter. The miss path reloads/releases a potentially changed previous slot; no concurrency/reentrant lazy-initialization policy is inferred from that fact.

The already-qualified ordinary witness subsequently calls `_swift_release` on this getter result at1004ebb14. That balances its local subject result, **not the cache field itself**. Conditional on the ADKBrowserDelegate remaining alive and that field remaining unchanged, the cache is a separate owner across the witness's local release. This is a local owner edge, not proof that any registered application Root keeps the delegate alive after the callback.

**Release boundary:** this scout proves the getter's replacement release and uses the prior exact caller-local release. It deliberately chooses the constructor as the one extra routine. The ADKBrowserDelegate final destructor/field-release routine was **not inspected**; final object teardown/disposal and every eventual buffer release remain unproved here. No destructor claim is inferred from metadata alone.

## One constructor: initial buffer flag, not callback state

Exact constructor **100586ecc..100586f60** operates on the newly allocated subject in Swift-selfx20. Raw instructions show:

- Array.init results stored at **subject+10**100586eec and **subject+18**100586f0c, matching saved buffer/subscribers field order.
- **100586f10 loads1;100586f14 directly stores byte1 at subject+20**.
- Remaining constructor instructions initialize a metadata-offset completion value, then100586f4c returns this same subject. Its completion value-witness implementation is not expanded or otherwise assigned new semantics here.

This proves **initial flag1 for a subject produced by this constructor**, not the flag observed during an actual ordinary-tab callback. A cache hit returns an older subject, whose flag may have changed; subscriber and buffer-drain state transitions are outside scope. Even a miss branch was not dynamically observed. The earlier buffer packet separately proves send helper100586f60 tests byte+20==1 and issues a mutating buffer-field append call on that branch. Combining these static facts does not prove the actual callback selected this concrete receiver, allocated a new subject or buffered its action at runtime.

## Complete spans and relevant data/import pins

LC_FUNCTION_STARTS in **both** file images supplies these complete next-start bounds; pristine/trace code is byte-identical:

| Routine | Half-open extent | Bytes | SHA256 in both |
|---|---|---|---|
| Actual lazy getter |1004eb4f8..1004eb580|136|e4c7ee6512aada31690ef270eeee882664fe5b8047741ca8f2d0dfe2bf60aab5|
| Single subject constructor |100586ecc..100586f60|148|8b3bf8a401c00398a25877e289beb53ba261820d2aabac13f8dd661e6a523d6b|

Pristine full SHA4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682; trace executable SHA155155b7ab879a0692f1a0dc691202d901b48ab8593afb60b1a91c519f302d5a. `relevant-data-and-imports.json` separately pins field offsetcell1044c98c8, type cache1044c98b8 and the getter's16-byte generic type reference. All match in both files. Type-cache/reference contents are not promoted to a complete runtime generic-metadata proof.

Six direct import names were resolved independently from both actual Mach-O indirect-symbol tables: allocObject, retain, release, Array.init, AnySubscriber metadata and Subscribers.Completion metadata. Their exact12-byte stubs and8-byte pointer cells match. No imported implementation or runtime binding is validated. Complete raw functions, saved decompilation and metadata excerpts are retained. These are new exact span qualifications, not an assumption from previously pinned field/buffer spans or whole-binary equality.

The remaining boundary is explicit: **local strong cache owner and new-subject initial flag are established; actual receiver registration, actual flag/action buffering, final delegate cache teardown and long-term/Root ownership are not**. No further function or graph was opened to fill those gaps.
