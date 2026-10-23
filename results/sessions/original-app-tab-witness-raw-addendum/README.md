# Two original-app witness receivers — raw qualification addendum

**Both complete receiver bodies are now raw-disassembled and pristine/trace byte-qualified.** The ordinary action payload/tag/forward call and the Session incoming-pair store are locally tractable in those bodies. This upgrades those specific saved-decompilation claims; it is **not Root-owner, installed receiver, runtime branch or persistent lifetime proof**. The frozen original witness packet is unchanged. No source/tool/Git/index, compiler/query/build, app/probe/runtime/UI, signal, queue, lock or Chirp action occurred. Only this addendum directory was written.

## Complete boundaries and code equality

LC_FUNCTION_STARTS was decoded independently from both file images, using preferred text-base-relative ULEB deltas. Exact starts and next starts agree; complete body byte hashes agree:

| Receiver | Complete half-open extent | Bytes | SHA256 in both |
|---|---|---|---|
| ADKBrowserDelegate witness | 1004eba88..1004ebb38 |176|bc223a2e2e7eb17f57fa27175389ca7cf21924150fd864af519d9b1aeff67756|
| SessionRestoration implementation behind witness thunk |10055a3bc..10055ad70|2484|d392f7af18ac0950314df72eaeef3a90e78948710ab1fa9c131b4ca077dce98e|

Both complete disassemblies are saved. LC_FUNCTION_STARTS blob SHA is1b4cda91ef58b05bcafbcfe4f69e5881436dff5760f08c54f734d6993b3069c6 in both. Pristine full-file SHA4c03750e2a2c4f8843dec4b90db2e0f1c5e4ba01ef2140e3961a6194bcc9d682 and trace-app executable SHA155155b7ab879a0692f1a0dc691202d901b48ab8593afb60b1a91c519f302d5a still differ overall. Only these bodies and explicitly listed data/import entries are qualified by this addendum; prior unrelated span comparisons are not substituted.

## Ordinary receiver: raw payload, tag and forwarding

- Entry1004eba9c..aa4 saves incoming **x4→x19, x3→x21, x2→x22**. Generic action metadata/size supplies a temporary atx24; metadata accessor/getter bodies are not expanded.
- **1004ebae8** stores the incoming x2/x3 pair (savedx22/x21) into that temporary. **1004ebaec** stores the incoming flag byte attemporary+10.
- **1004ebaf0..afc** supplies `(temporary, action metadata, tag0)` to exact import `_swift_storeEnumTagMultiPayload` at1035aeac4. Its symbol identity was independently resolved from **both actual Mach-O indirect-symbol tables**, not guessed from Ghidra function names. Existing saved enum metadata names tag0 onNewWebContentCreated; this addendum's raw result is the literal tag0 call and payload stores.
- **1004ebb04** retains the incoming object through actual `_swift_unknownObjectRetain`; **1004ebb0c** directly calls100586f60 with the temporary inx0. The prior BufferingSubject packet owns that callee; it was not expanded here.
- Raw1004ebae4 stores getter returnx0 inx20; **1004ebb14** calls `_swift_release` with that samex20 after forwarding. This is the returned subject's local release, not evidence that the subject persists elsewhere. **1004ebb1c** then calls1004ed27c with the temporary; that cleanup callee is unexpanded. No Root/persistent ownership follows from these local retain/release calls.

## Session receiver: incoming pair → receiver field → value array → element store

The body has early exits/diagnostic branches; the following is a conditional supported local store path, not a universal call outcome.

1. **10055a3dc..e4** saves receiver Swift-selfx20 inx27, incoming x3 inx22 and x2 inx21. After allocating its frame, **10055a414** stores `(incoming x3, incoming x2)` atsp+30. No intermediate store overwrites those two frame words before the later load; the prologue/epilogue's similarly printed stack offsets refer to different sp values.
2. **10055aa50..78** loads offset cell **1044cc928**, computes receiver+offset, opens modify access, and obtains that receiver field into the dictionary working valuex20. The raw offset-cell value is **8**, same in both files. The saved metadata/decomp name/type for this cell is unreceivedRestoredContents: dictionary BrowserID→array WebContent; its *source name/type* remains metadata/decomp corroboration, while the machine-code receiver-field address is raw-qualified here. TrackedBrowsers offset cell1044cc930 is16 and also equal.
3. The local dictionary paths restore their working value to **[receiverx27+offsetx22]** at10055aae4/10055aaf4, or capacity/copy path10055ad50. Slot indexx23 comes from the local lookup return; those lookup/copy helper bodies are deliberately unexpanded.
4. **10055ab78** loads the dictionary's value-storage base fromworking dictionary+38 intox21; **10055ab7c** loads the chosen value array from[x21+x23*8] intox19. New-entry code10055ab44..50 initializes that same value slot from the saved empty-array reference. The relevant reference cell104167650 is pinned and byte-equal; no loaded external singleton implementation is claimed.
5. If the local uniqueness/capacity paths obtain a replacement array, **10055ad18/10055ad3c** explicitly write its returned pointer back into the same dictionary value slot. Both rejoin the local count/update path; no further callee tracing is required to identify the subsequent destination pointer.
6. **10055ab90..a4** loads old count, increments/stores it atarray+10 and computes destinationarray+oldCount*16. **10055aba8** reloads incoming x3 intox9 and incoming x2 intox19 fromsp+30. **10055abac performs `stp x19,x9,[x8,#0x20]`**: the original incoming pair is directly written into that selected dictionary-backed array element. This is a concrete pair STORE, not an inference from `_swift_unknownObjectRetain`.
7. It ends modify access10055abb4 and retains incoming object10055abbc, then calls100559bf0 with a local before epilogue. That cleanup body and transitive effects remain unexpanded. The raw store is proved; indefinite persistence, all later release behavior or arbitrary reentrant cleanup effects are not inferred.

This raw local chain upgrades the former saved-decomp-only store claim. It does not establish that this receiver was installed in the ordinary callback, which branch ran, whether the stored action survives all later operations, or that a Root owner/native browser association is valid. No constructor/binding/subject investigation was repeated.

## Relevant data/import qualification and remaining grade limits

`relevant-data-and-imports.json` pins six relevant **actual indirect-symbol identities**, each12-byte stub and its8-byte pointer cell in both binaries: storeEnumTagMultiPayload, unknownObjectRetain, release, beginAccess, endAccess and isUniquelyReferenced_nonNull_native. All compared bytes match. It separately pins the two field-offset cells and empty-array reference. These are file-level code/data bindings, **not loaded runtime resolution or standard-library implementation validation**. No external import body was disassembled.

The class/protocol/table identity and first witness-slot mappings remain in the prior frozen packet. This addendum does not promote stripped Swift method names, metadata source names, dynamic receiver registration, subject survival, cleanup-helper semantics or all-path ownership into raw proof. It upgrades only complete receiver code identity, direct local payload/tag/call instructions, and the locally traceable field/value-array/pair store. It recommends no Root activation or source change.
