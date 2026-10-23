# Original queued restore callback liveness

Read-only bounded static audit, performed 2026-10-23T13:17:35.577430+00:00. Original arm64 ArcCore SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`. No application launch, invalid-input probe, build, production edit, or callback fabrication. Current owned source reference: `arc/api/arc_sessions_restore.mm:663–688`.

## Result

Original retains the **native shared Arc WebContents peer** across the task and weak-checks its Browser receiver. At dispatch it tests whether that retained peer has **any current Chromium WebContents** in Impl+0x130. It does not retain, weak-check, compare against, or resolve the originally inserted Chromium WebContents identity at delivery. This is the same native pointer test used by public `ArcWebContents.isInitialized`.

Consequently, the current owned capture of `contents->GetWeakPtr()` at line 667 and its delivery gate at line 676 is a stronger condition than the original dispatcher. If the same Arc peer is rebound to a replacement native WC before notification, original's current+0x130 guard succeeds while a weak reference to the old native WC can be dead. This is a conditional static behavior difference, not proof that the particular close/discard-before-delivery ordering occurred in a runtime fixture. Pending core rebind integration is separate. No speculative production change is proposed here.

## Proven data flow

1. Producer calls `4346100` with inserted native WC at `43066c8`; return pair is at stack+0x30. Existing Arc UserData weak pair is strong-locked by `4346150–4346184` and written as shared pair at `43461b0`. New-peer fallback is outside the bounded existing-peer excerpt.
2. At `4306550–4306560`, the returned pair is copied into task+0x40/+0x48 and the control block strong count is incremented. Producer's local shared count is subsequently released. The task destructor also decrements control count at `43073e0–43073f4` (read during audit). There is no original Chrome WC pointer or weak handle in this task's captured argument pair.
3. Task invoker `4307290–43072c0` checks captured Browser weak flag and raw receiver; on success it moves the shared Arc peer into x1 argument storage at `43072e4–4307308` and invokes `42fcf40`. Optional historical/original session arguments are separately captured at task+0x68/+0x70 and passed in x3/x4.
4. Dispatcher's very first check at `42fcf60–42fcf68` is `peer = [x1]; current = [peer+0x130]; if (!current) return`. Neither an exact original Chrome identity comparison nor a Browser-membership check appears before restoration routing. Public `isInitialized` at `4440d80–4440d8c` tests exactly that same field.
5. Restore branch copies/retains the same peer at `42fd540–42fd558`, weak-locks the native Browser delegate control block at `42fd634–42fd66c`, and invokes delegate virtual+0x18 at `42fd6b0–42fd6c8`. The ObjC bridge `43e3458` creates an ArcWebContents wrapper from the retained peer; it loads weak app delegate and weak Browser ObjC references at `43e34b4–43e34c8`, then messages didRestore at `43e34e4`. That bridge does not reread a Chrome weak pointer or session helper. Wrapper identity is not asserted by this audit: the bridge allocates a shell for delivery.
6. Successful native discard replacement `4398080–43980d0` detaches and attaches using the same x20 Impl. Detach `4372b88` identifies pair Impl+0x128; `4373058` clears that pair including +0x130. Attach stores replacement native WC to same Impl+0x130 at `437039c`. Therefore the dispatcher reads replacement liveness after completed rebind, not the old native WC's liveness.

## Close and completion limits

- A peer whose current+0x130 is null when dispatch begins skips notification at `42fcf68 → 42fd298`. Retaining the peer alone does not guarantee delivery after permanent detach/close.
- A completed rebind leaving current+0x130 nonnull passes this initial gate. A dispatch during the transient null interval fails it. Actual event ordering and reachability require a runtime oracle; none was executed.
- Browser weak receiver and delegate/ObjC weak references are additional delivery conditions. This is not unconditional callback delivery.
- The null-current early return occurs before full tracked-WC removal and explicit-ID consumption in the later dispatcher; it does not itself synthesize `finished(nil)`. Overall cancellation/full-completion behavior after a skipped notification remains outside this bounded audit.
- No conclusion here establishes full restore scheduling, physical discard success, saved-group provenance, or pending native hooks.

## Review of parent proposed guard replacement

PASS for the bounded proposal: retain the existing strong ArcWebContents wrapper and weak Browser checks, remove the captured old Chrome WebContents WeakPtr, and gate dispatch on `[wrapper webContents]` being nonnull at delivery. This checks current native binding, **not merely ObjC/native Arc-peer lifetime**. Current core getter `arc/api/arc_web_contents.mm:682` returns `_contents.get()` (a WeakPtr-backed current binding); `isInitialized` at line 715 is the corresponding `!!_contents` predicate. The getter is declared with the exact `content::WebContents*` type in `arc/api/arc_internal.h:147`; it performs no delegate callback or native mutation.

This proposal therefore keeps the original dispatch-time nonnull-native requirement, skips a retained but permanently detached wrapper, and does not reject a canonical wrapper solely because its former native WC died. It does not retain the native WC, dereference it after an app callback, or introduce a strong Browser cycle. Strong wrapper capture preserves the wrapper during queued delivery. The check is a point-in-time predicate, as the original's +0x130 test is; no stronger guarantee that app callbacks cannot subsequently close it is inferred. Correct canonical rebinding remains a core integration dependency and is not supplied or activated by this guard change. No source edits were made by this reviewer.

## Bounded disassembly

### Resolve native Arc peer from inserted Chromium WC

```text
0434613c: adrp     x1, #0xe4a2000
04346140: add      x1, x1, #0xf38
04346144: add      x0, x8, #8
04346148: bl       #0x805e0
0434614c: cbz      x0, #0x4346230
04346150: ldr      x23, [x0, #0x10]
04346154: cbz      x23, #0x4346230
04346158: add      x8, x23, #8
0434615c: ldar     x8, [x8]
04346160: cmn      x8, #1
04346164: b.eq     #0x4346230
04346168: add      x9, x8, #1
0434616c: add      x10, x23, #8
04346170: mov      x11, x8
04346174: casal    x11, x9, [x10]
04346178: cmp      x11, x8
0434617c: mov      x8, x11
04346180: b.ne     #0x4346160
04346184: ldr      x24, [x0, #8]
04346188: cbz      x24, #0x4346228
0434618c: add      x8, x23, #8
04346190: mov      w9, #1
04346194: ldadd    x9, x8, [x8]
04346198: mov      x22, x23
0434619c: add      x8, x23, #8
043461a0: mov      x9, #-1
043461a4: ldaddal  x9, x8, [x8]
043461a8: cbz      x8, #0x4346324
043461ac: cbz      x24, #0x4346230
043461b0: stp      x24, x22, [x19]
043461b4: cbz      x22, #0x4346270
043461b8: add      x8, x22, #8
043461bc: mov      w9, #1
043461c0: ldadd    x9, x9, [x8]
```

### Insertion producer captures peer with strong control count

```text
043064e8: ldr      x21, [x20, #0x298]
043064ec: cbz      x21, #0x4306500
043064f0: ldadd    w25, w8, [x21]
043064f4: mov      w9, #0x7fffffff
043064f8: cmp      w8, w9
043064fc: b.eq     #0x43069d0
04306500: mov      w26, #1
04306504: ldr      x25, [x20, #0x2a0]
04306508: mov      w0, #0x78
0430650c: bl       #0x8790
04306510: mov      x2, x0
04306514: str      w26, [x0]
04306518: adr      x9, #0x4307290
0430651c: nop      
04306520: adr      x8, #0x43073c8
04306524: nop      
04306528: stp      x9, x8, [x0, #8]
0430652c: adrp     x8, #0x4169000
04306530: add      x9, x8, #0xbc0
04306534: adr      x8, #0x42fcf40
04306538: nop      
0430653c: stp      x9, x8, [x0, #0x18]
04306540: stp      xzr, x21, [x0, #0x28]
04306544: str      x25, [x0, #0x38]
04306548: mov      w25, #1
0430654c: ldr      x8, [sp, #0x38]
04306550: ldr      q0, [sp, #0x30]
04306554: str      q0, [x0, #0x40]
04306558: cbz      x8, #0x4306564
0430655c: add      x8, x8, #8
04306560: ldadd    x25, x8, [x8]
04306564: strb     w22, [x2, #0x50]
04306568: strb     wzr, [x2, #0x59]
0430656c: stur     xzr, [x2, #0x51]
04306570: strh     w25, [x2, #0x5a]
04306574: strb     wzr, [x2, #0x5c]
04306578: ldr      w8, [sp, #0x48]
0430657c: stur     w8, [x2, #0x5d]
04306580: ldur     w8, [sp, #0x4b]
04306584: str      w8, [x2, #0x60]
04306588: strb     wzr, [x2, #0x64]
0430658c: ldur     x8, [x27, #0x14]
04306590: str      x8, [x2, #0x68]
04306594: ldur     x8, [x27, #0x1c]
04306598: str      x8, [x2, #0x70]
0430659c: ldr      x8, [x24]
043065a0: ldr      x8, [x8]
043065a4: add      x1, sp, #0x70
043065a8: mov      x0, x24
043065ac: mov      x3, #0
043065b0: blr      x8
```

### Queued invoker checks only Browser weak state and moves peer pair

```text
04307290: ldr      x8, [x0, #0x30]
04307294: cbz      x8, #0x43072a0
04307298: ldapurb  w8, [x8, #4]
0430729c: cbz      w8, #0x43072a4
043072a0: ret      
043072a4: ldr      x8, [x0, #0x38]
043072a8: cbz      x8, #0x43072a0
043072ac: ldr      x8, [x0, #0x30]
043072b0: cbz      x8, #0x43073bc
043072b4: ldp      x9, x10, [x0, #0x20]
043072b8: ldapurb  w8, [x8, #4]
043072bc: cbnz     w8, #0x43073bc
043072c0: ldr      x8, [x0, #0x38]
043072c4: add      x8, x8, x10, asr #1
043072c8: tbz      w10, #0, #0x43072d4
043072cc: ldr      x10, [x8]
043072d0: ldr      x9, [x10, w9, uxtw]
043072d4: sub      sp, sp, #0x50
043072d8: stp      x20, x19, [sp, #0x30]
043072dc: stp      x29, x30, [sp, #0x40]
043072e0: add      x29, sp, #0x40
043072e4: ldp      q0, q1, [x0, #0x40]
043072e8: str      q0, [sp, #0x20]
043072ec: stp      xzr, xzr, [x0, #0x40]
043072f0: str      q1, [sp]
043072f4: ldur     x10, [x0, #0x5d]
043072f8: stur     x10, [sp, #0xd]
043072fc: ldp      x3, x4, [x0, #0x68]
04307300: add      x1, sp, #0x20
04307304: mov      x2, sp
04307308: mov      x0, x8
0430730c: blr      x9
04307310: ldr      x19, [sp, #0x28]
04307314: cbz      x19, #0x4307328
04307318: add      x8, x19, #8
0430731c: mov      x9, #-1
04307320: ldaddal  x9, x8, [x8]
04307324: cbz      x8, #0x430734c
04307328: ldp      x29, x30, [sp, #0x40]
0430732c: ldp      x20, x19, [sp, #0x30]
04307330: add      sp, sp, #0x50
```

### Dispatcher current peer liveness and routing entry

```text
042fcf40: sub      sp, sp, #0xe0
042fcf44: stp      x28, x27, [sp, #0x80]
042fcf48: stp      x26, x25, [sp, #0x90]
042fcf4c: stp      x24, x23, [sp, #0xa0]
042fcf50: stp      x22, x21, [sp, #0xb0]
042fcf54: stp      x20, x19, [sp, #0xc0]
042fcf58: stp      x29, x30, [sp, #0xd0]
042fcf5c: add      x29, sp, #0xd0
042fcf60: ldr      x8, [x1]
042fcf64: ldr      x8, [x8, #0x130]
042fcf68: cbz      x8, #0x42fd298
042fcf6c: mov      x20, x3
042fcf70: mov      x21, x1
042fcf74: mov      x19, x0
042fcf78: tbz      x3, #0x20, #0x42fcfdc
042fcf7c: ldrb     w8, [x19, #0x258]
042fcf80: cmp      w8, #2
042fcf84: b.ne     #0x42fcfdc
```

### Restore branch copies retained peer and invokes Browser delegate

```text
042fd4fc: ldr      x22, [x19, #0x10]
042fd500: cbz      x22, #0x42fd4f0
042fd504: ldur     q0, [x19, #8]
042fd508: add      x8, x22, #8
042fd50c: ldar     x8, [x8]
042fd510: cmn      x8, #1
042fd514: b.eq     #0x42fd4f0
042fd518: add      x10, x8, #1
042fd51c: add      x9, x22, #8
042fd520: mov      x11, x8
042fd524: casal    x11, x10, [x9]
042fd528: cmp      x11, x8
042fd52c: mov      x8, x11
042fd530: b.ne     #0x42fd510
042fd534: str      q0, [sp, #0x30]
042fd538: mov      w8, #1
042fd53c: ldadd    x8, x8, [x9]
042fd540: ldr      x8, [x21, #8]
042fd544: ldr      q0, [x21]
042fd548: str      q0, [sp, #0x20]
042fd54c: cbz      x8, #0x42fd55c
042fd550: add      x8, x8, #8
042fd554: mov      w9, #1
042fd558: ldadd    x9, x8, [x8]
```

### Restore branch delegate weak-lock and call

```text
042fd634: ldr      x23, [x19, #0xc8]
042fd638: cbz      x23, #0x42fd710
042fd63c: add      x8, x23, #8
042fd640: ldar     x8, [x8]
042fd644: cmn      x8, #1
042fd648: b.eq     #0x42fd710
042fd64c: add      x9, x8, #1
042fd650: add      x10, x23, #8
042fd654: mov      x11, x8
042fd658: casal    x11, x9, [x10]
042fd65c: cmp      x11, x8
042fd660: mov      x8, x11
042fd664: b.ne     #0x42fd644
042fd668: ldr      x24, [x19, #0xc0]
042fd66c: cbz      x24, #0x42fd700
042fd670: add      x26, x19, #0xd0
042fd674: and      x8, x26, #0xfffffffc00000000
042fd678: adrp     x9, #0xe2f2000
042fd67c: ldr      x9, [x9, #0x208]
042fd680: cmp      x8, x9
042fd684: b.ne     #0x42fd690
042fd688: add      x0, x19, #0xd0
042fd68c: bl       #0xffa8
042fd690: ldrb     w28, [x26]
042fd694: mov      w8, #1
042fd698: strb     w8, [x26]
042fd69c: mov      w8, #0xa
042fd6a0: stur     x8, [x29, #-0x58]
042fd6a4: adrp     x25, #0xe4a4000
042fd6a8: ldrb     w0, [x25, #0x45a]
042fd6ac: cbnz     w0, #0x42fdd50
042fd6b0: ldr      x8, [x24]
042fd6b4: ldr      x8, [x8, #0x18]
042fd6b8: add      x1, sp, #0x30
042fd6bc: add      x2, sp, #0x20
042fd6c0: add      x3, sp, #0x4c
042fd6c4: mov      x0, x24
042fd6c8: blr      x8
042fd6cc: mov      w8, #0xa
042fd6d0: stur     x8, [x29, #-0x58]
042fd6d4: ldrb     w0, [x25, #0x45a]
042fd6d8: cbnz     w0, #0x42fdd34
042fd6dc: cbz      x26, #0x42fd6e4
042fd6e0: strb     w28, [x26]
042fd6e4: and      x8, x26, #0xfffffffc00000000
042fd6e8: adrp     x9, #0xe2f2000
042fd6ec: ldr      x9, [x9, #0x208]
042fd6f0: cmp      x8, x9
042fd6f4: b.ne     #0x42fd700
042fd6f8: add      x0, x19, #0xd0
042fd6fc: bl       #0x1fee4
042fd700: add      x8, x23, #8
042fd704: mov      x9, #-1
042fd708: ldaddal  x9, x8, [x8]
042fd70c: cbz      x8, #0x42fd928
```

### ObjC restore delegate bridge

```text
043e3458: stp      x22, x21, [sp, #-0x30]!
043e345c: stp      x20, x19, [sp, #0x10]
043e3460: stp      x29, x30, [sp, #0x20]
043e3464: add      x29, sp, #0x20
043e3468: mov      x19, x3
043e346c: mov      x20, x2
043e3470: mov      x21, x0
043e3474: adrp     x8, #0xe28c000
043e3478: ldr      x0, [x8, #0x138]
043e347c: bl       #0x98eb2c8
043e3480: adrp     x8, #0xe28f000
043e3484: ldr      x1, [x8, #0x8e8]
043e3488: mov      x2, x20
043e348c: bl       #0x80045ec
043e3490: mov      x20, x0
043e3494: adrp     x8, #0xe28c000
043e3498: ldr      x0, [x8, #0x118]
043e349c: bl       #0x98eb2c8
043e34a0: adrp     x8, #0xe290000
043e34a4: ldr      x1, [x8, #0x698]
043e34a8: mov      x2, x19
043e34ac: bl       #0x80045ec
043e34b0: mov      x19, x0
043e34b4: add      x0, x21, #8
043e34b8: bl       #0x84edd34
043e34bc: mov      x22, x0
043e34c0: add      x0, x21, #0x10
043e34c4: bl       #0x84edd34
043e34c8: mov      x21, x0
043e34cc: adrp     x8, #0xe290000
043e34d0: ldr      x1, [x8, #0x6f8]
043e34d4: mov      x0, x22
043e34d8: mov      x2, x21
043e34dc: mov      x3, x20
043e34e0: mov      x4, x19
043e34e4: bl       #0x80045ec
043e34e8: mov      x0, x21
043e34ec: bl       #0x8003ddc
043e34f0: mov      x0, x22
043e34f4: bl       #0x8003ddc
043e34f8: mov      x0, x19
043e34fc: bl       #0x8003ddc
043e3500: mov      x0, x20
043e3504: ldp      x29, x30, [sp, #0x20]
043e3508: ldp      x20, x19, [sp, #0x10]
043e350c: ldp      x22, x21, [sp], #0x30
043e3510: b        #0x8003ddc
```

### Public isInitialized tests the same Impl field

```text
04440d78: ldrb     w8, [x0, #0x128]
04440d7c: tbz      w8, #0, #0x4440d94
04440d80: ldr      x8, [x0, #0x118]
04440d84: ldr      x8, [x8, #0x130]
04440d88: cmp      x8, #0
04440d8c: cset     w0, ne
04440d90: ret      
04440d94: brk      #1
```

### Discard replacement observer keeps the same Impl

```text
04398080: mov      x0, x19
04398084: bl       #0x7e1fae0
04398088: ldr      x8, [x0, #8]
0439808c: ldr      x9, [x20, #0x130]
04398090: cmp      x8, x9
04398094: b.ne     #0x4397e3c
04398098: mov      x19, x0
0439809c: ldr      x8, [x0, #0x10]
043980a0: ldrb     w8, [x8, #0x5b0]
043980a4: cbz      w8, #0x4397e3c
043980a8: mov      x0, x20
043980ac: bl       #0x4372b6c
043980b0: add      x0, x20, #0x28
043980b4: mov      x1, #0
043980b8: bl       #0x4353534
043980bc: ldr      x1, [x19, #0x10]
043980c0: mov      x0, x20
043980c4: bl       #0x43702ec
043980c8: mov      x0, x20
043980cc: mov      w1, #1
043980d0: bl       #0x43741e8
043980d4: b        #0x4397e3c
```

### Detach receiver identifies +128 native pair

```text
04372b6c: sub      sp, sp, #0x50
04372b70: stp      x24, x23, [sp, #0x10]
04372b74: stp      x22, x21, [sp, #0x20]
04372b78: stp      x20, x19, [sp, #0x30]
04372b7c: stp      x29, x30, [sp, #0x40]
04372b80: add      x29, sp, #0x40
04372b84: mov      x19, x0
04372b88: add      x22, x0, #0x128
```

### Detach clears native pair including +130

```text
04373054: ldr      x0, [x22]
04373058: stp      xzr, xzr, [x22]
0437305c: cbz      x0, #0x437306c
04373060: ldr      x8, [x0]
04373064: ldr      x8, [x8, #8]
04373068: blr      x8
```

### Attach replacement into same Impl+130

```text
04370348: ldr      x8, [x19, #0x130]
0437034c: cbnz     x8, #0x4370fd8
04370350: ldr      x8, [x19, #0x128]
04370354: cbz      x8, #0x4370360
04370358: cmp      x8, x20
0437035c: b.ne     #0x4370ff0
04370360: and      x8, x20, #0xfffffffc00000000
04370364: adrp     x24, #0xe2f2000
04370368: ldr      x9, [x24, #0x208]
0437036c: cmp      x8, x9
04370370: b.eq     #0x437037c
04370374: mov      x0, #0
04370378: b        #0x4370388
0437037c: mov      x0, x20
04370380: bl       #0xffa8
04370384: ldr      x0, [x19, #0x130]
04370388: and      x8, x0, #0xfffffffc00000000
0437038c: ldr      x9, [x24, #0x208]
04370390: cmp      x8, x9
04370394: b.ne     #0x437039c
04370398: bl       #0x1fee4
0437039c: str      x20, [x19, #0x130]
043703a0: mov      x0, x20
```
