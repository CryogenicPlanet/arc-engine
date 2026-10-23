# Original discard native Impl and blocked-count audit

Static-only original trace156 ArcCore arm64 audit, 2026-10-23. No application run, memory attach, build, production edit, board access, or credential access. All addresses are unslid image virtual addresses. Raw supporting ranges: `/tmp/arccore-sessions-discard-impl-disassembly.txt`; independent direct-edge/offset scan: `/tmp/arccore-sessions-discard-impl-review-scan.txt`.

## Result

The successful replacement branch retains the SAME `arc::WebContents::Impl` allocation/shared peer. It detaches the old Chromium `content::WebContents`, then installs the replacement Chromium WebContents into that same Impl and stores that same Impl shared/weak pair into the replacement's Arc UserData. This is native-pointer dataflow proof, not a conclusion from ObjC-shell identity or public hash equality.

The blocked-count field is decimal1484 = **hex0x5cc**, not hex0x1484. Its ordinary observer subobject offset is decimal168 = **hex0xa8**, not hex0x168. The audited handoff does not execute constructor initialization, write/reset the count, or synthesize PrimaryPageChanged during Observe(newWC). The retained native Impl therefore retains that count through the handoff itself. PrimaryPageChanged later resets via the equality-guarded setter. This statement does not predict arbitrary reentrant observer/app behavior or queued notification arrival times.

## Exact identity chain

1. ObjC discard selector `4441db4..4441dc0`: test engaged flag at ObjC self+0x128, load its native Impl from self+0x118, tailbranch4374c00. No optional/shared peer replacement.
2. Native discard `4374c8c` keeps original Impl in x21. `4374d20` calls the performance-manager discard helper7af7468; after it, `4374d34` rereads `[x21+0x130]`. The finalize task capture at4374d3c..4374d9c reads the SAME Impl shared_this pair `[x21+8]/[x21+0x10]`, then calls4378120. It does not discover/construct a fresh Impl.
3. Replacement observer `4398080..43980d4`: extract event payload7e1fae0; compare oldWC `[payload+8]` with `[x20+0x130]`; check replacement `[payload+0x10]+0x5b0` was-discarded flag; call4372b6c with x0=x20; call4353534 with x0=x20+0x28,x1=nil; call43702ec with x0=x20,x1=payload.newWC; call43741e8 with x0=x20,w1=1.
4. Detach4372b6c keeps x19=this and x22=this+0x128. Final4373054..4373058 zeros only the owned/native Chromium WC pair at Impl+0x128/+0x130. It does not replace `this` or clear shared_this+8/+0x10.
5. Intermediate4353534 receives Impl+0x28, so its stores at receiver+0x30/+0x38 clear **Impl+0x58/+0x60**, a browser weak reference. These are not the ObjC wrapper's self+0x118 peer or the Impl shared_this pair.
6. Attach43702ec keeps x19=this and x20=newWC at4370308..0c. It requires a detached Impl (`[x19+0x130]==nil` at4370348) and any ownedWC field+0x128 either nil or equal to the replacement. At437039c it stores newWC to SAME Impl+0x130.
7. New WC Arc UserData is ensured by43703a0..a4->43713e0. That helper allocates only an0x18-byte userdata shell and initializes its shared-pair fields to zero; it does NOT allocate or construct an Impl. Attach43703bc..4370408 then takes SAME Impl's shared_this pair (`ldr x20,[x19+0x10]`;`ldur q0,[x19+8]`) and stores it into newWC's Arc UserData (`stur q0,[userdata+8]`). This proves replacement canonical native peer identity.
8. Impl constructor is the separate4371518 function; its zero initialization at4371a94 is not the rebind path. Attach's allocations are WC helper objects, not a second Impl. The shared pair stored into replacement UserData is the original pair regardless.

## Counter, observer attachment, and notifications

- Constructor4371a94: `str wzr,[x19,#0x5cc]` initializes the native Impl counter once.
- Setter438eb84:438eba4 loads `[x0,#0x5cc]`;438eba8 compares w1;438ebac skips unchanged values;438ebb0 stores new w1. Observer notification is after this write.
- Public synchronization440edbc loads w2 from native Impl+0x5cc and calls the ObjC property setter. This is a mirror of native Impl storage, not proof that ObjC shell owns the native count.
- PrimaryPageChanged thunk438f2fc subtracts0xa8 from its observer-subobject receiver, sets w1=0, then tailbranches438eb84. Direct reset438f2f4 also sets w1=0 and tailbranches438eb84.
- Attach43706c4..cc passes the replacement WC to `WebContentsObserver::Observe`23db2f4 on **SAME Impl+0xa8**. Detach4372d74..7c detaches this same observer when old member+0xc0 is engaged.
- Observe23db2f4 only compares old/new observed pointers; unregisters old via151dc6c; writes observer+0x18; registers new via1c44a40. Register/unregister maintain the native WC observer list and weak references. They do not dispatch PrimaryPageChanged or invoke a virtual observer method. Thus the mere re-observation does not reset count.
- Post-rebind43741e8 writes state value1 to Impl+0x290, captures that value for observer notification. It does not write0x5cc. Its callback scheduling/virtual observer delivery must not be confused with a count reset.
- Audited full detach4372b6c..437319c and attach43702ec..4370ffc have no explicit count write or setter/reset call. Root-Impl helper4373cc4 reattaches browser/delegate observation and writes only separate+0x5c0 flag in that band. The helper atImpl+0x488,435d6d4, clears its own+0x40 field (=Impl+0x4c8), not the counter. Detach439dfa0 takes Impl+0x5a8 map; its map operations do not zero Impl+0x5cc.

## Bounded scan and limits

Independent full __text direct B/BL target scan found exactly four direct edges to counter setter438eb84, all tail B:438eb80 directincrement;438f2f0 observerincrement;438f2f8 directreset;438f304 observerreset. No direct BL caller. In Arc code range0x4200000..0x4470000, direct scalar immediate stores to displacement0x5cc are constructor4371a94 and setter438ebb0 only. This mechanical scan alone does not establish pointer types elsewhere, exclude paired/vector/bulk stores with another displacement, or resolve virtual calls; the handoff-body dataflow supplies the relevant identity and write evidence.

Fresh reviewer also found exactly three direct calls to constructor4371518:436ee0c,4371038,4371264, each immediately after a fresh0x620 allocation. Direct reset438f2f4 and observer reset438f2fc have no direct B/BL callers; their vtable references are da81238 and da81b68. Observer-relative counter displacement0x524 (=0x5cc-0xa8) has only the increment load438f2e4 and no direct Arc-region store. Evidence: `/tmp/arccore-sessions-discard-impl-review-{callers,contexts,alias524}.txt`.

The attach/detach functions invoke many native helper/userdata operations and may schedule observer work. This audit is not an exhaustive proof of all indirect callbacks under adversarial reentrancy, profile teardown, later navigation, or arbitrary app delegates. No runtime notification ordering or count snapshot was measured. The exact native detach/rebind route is nevertheless sufficient to reject an unconditional counter-zero solely because Chromium WebContents changes: original native state stays in the retained Impl and the explicit page-change reset remains separate.

## Integration implication

Keep the blocked count at retained Arc-WebContents/native-peer lifetime through successful discard rebinding; fresh Chromium-frame endpoints and native-WebContents-bound helpers still need recreation. If our bridge stores the count in old Chromium-WC userdata, preserve/seed that logical count into the replacement bridge before publishing any count, with no synthetic zero publication during the handoff. Reset on actual PrimaryPageChanged via the existing equality guard. The choice of transfer API and canonical-owner storage belongs to renderer/core workers; this is evidence, not a production patch or runtime acceptance claim.
