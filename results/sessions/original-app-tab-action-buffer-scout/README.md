# Original app tab action BufferingSubject — bounded static scout

**The buffering branch issues a concrete mutating append into the subject's buffer field; it is not merely a temporary retain or a name-based inference.** Whether the actual ordinary-tab callback took that branch, used this concrete receiver, or left the subject/action alive after the caller returned remains **UNKNOWN**. The imported standard-library append implementation and its lowered storage/disposal were **not validated**. No Root activation recommendation follows.

Scope: only helper **100586f60..1005870b0** (336 bytes), its **Array.append import stub1035a832c..1035a8338** (12 bytes), and saved field/import metadata. No buffer-drain/release implementation, subscriber callback, constructor or wider Swift graph was expanded. The prior witness packet remains unchanged and sealed. No source/tool/Git/index, compiler/query/build, app/probe/runtime/UI, signal, queue, lock or Chirp action. Only this evidence directory was written; current Renderer capture/source holds are respected.

## True path: a specific field append

Saved metadata identifies ARCFoundation.BufferingSubject fields `buffer: [A]`, subscribers array, isBuffering bool and completion. Raw helper instructions establish the actual operands and branch, rather than inferring persistence from BufferingSubject/Combine names:

- **100586fb0..fb8** tests subject byte **+20** for exactly **1**.
- **100586fbc..fcc** invokes the element value-witness slot+10 with destination temporaryx22, incoming action pointerx21 and element metadatax19 (the ABI initialize-with-copy operation).
- **100586fd0..fe0** begins modify access to the concrete subject **+10** field.
- **100586ff4** moves Swift selfx20 to that **field address**, and **100586ffc** calls1035a832c with the copied temporary. Saved stubs2026 names this exact import **`_$sSa6appendyyxnF` / Array.append(_:)**; the neighboring metadata accessor is separately named at1035a8368. The subject+10 field is therefore the mutating array receiver, not an unrelated local array or subscribers snapshot.
- **100587000..008** ends access and goes directly to the shared epilogue/return. There is no local buffer clear, removal or copied-action destroy after append in this helper.

Under the ordinary Array.append contract, this is a path that appends an owned copy of the incoming action into concrete subject buffer state, and does not undo that append before returning. It differs materially from passing only the caller's temporary to a synchronous subscriber. **The app's direct machine instructions stop at the imported call**: its actual array element STR/ownership balancing/disposal live in the external standard library, whose implementation was not extracted or validated here. Do not relabel this bounded field-append call proof as an independently observed post-return array snapshot or an all-path lifetime guarantee.

The prior packet proves the ADKBrowserDelegate implementation constructs an onNewWebContentCreated action carrying the incoming WebContent pair and passes it to this helper. It does not prove the concrete delegate assignment, the actual flag or the subject's lifetime in that callback. No claim that the flag starts true, that a subscriber was absent/present, or that the lazy subject field retains this exact subject after caller release is made without its constructor/getter binding proof. The old packet's temporary object retain alone remains insufficient.

## Nonbuffering path: synchronous forwarding, downstream unknown

When byte+20 is not1, **10058700c..020** reads the subject's subscribers **+18** array into a local snapshot. It retains that snapshot at10058705c, constructs the sequence/callback context, invokes Sequence.forEach at10058708c, then releases the snapshot at100587094 and returns. The action pointer is supplied through the local callback context. This branch contains no direct append/write to buffer+10 and no new posted action storage in the inspected helper.

That is synchronous forwarding by this helper, **not proof that subscribers never retain or enqueue the action**. Its callback body/subscriber implementations were deliberately not traced. The snapshot retain/release is array lifetime bookkeeping, not evidence of persistent action ownership. Subsequent buffer drain/clear, subject deinit, action disposal, and lifetime through delegate/caller release remain outside this bounded scout.

## Pristine versus trace applicability

Fresh full binary hashes and code-span comparisons are in pristine-vs-trace-spans.json. Pristine `../arc/bin/Arc.arm64` SHA **4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682** differs overall from saved ArcModAgent-trace156's executable. The **newly checked helper336-byte span** and **append-stub12-byte span** are each byte-identical across those two files. Their SHA values are ed5aee87… and534c8c5d… respectively. These checks are independent of the earlier field scout's different span comparisons.

Only these code spans transfer. This does not seal runtime flag/data, actual dynamic delegate/subject identity, generic metadata state, loaded standard-library implementation/import binding, other caller/getter/constructor code, downstream subscriber behavior or arbitrary app-version equivalence. No running app was inspected. Exact raw instructions, saved decompilation, saved import names and metadata excerpts are retained alongside input pins.

Conclusion: **yes, a concrete buffer-field append is statically present on the flag==1 branch; actual callback branch and persistent runtime lifetime are unknown.** Exact external append store/disposal and downstream action ownership remain unvalidated. This is a narrow lifetime boundary finding, not source parity, test acceptance or activation authority.
