# Original native restore created-callback cancellation boundaries

Read-only original ArcCore static trace, 2026-10-23. No repository edits, builds, apps, network, or credentials. Original binary SHA-256: b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d. Addresses are unslid original arm64 VAs. No runtime cancellation experiment performed.

## Verified return/continuation boundaries

- New-browser application handler invokes created synchronously via callback at `42b9e6c`. After callback transport cleanup it rechecks only the global pending callback-active byte (`42b9eb8`), not browser/profile weak liveness; it then loads actual Browser session ID through the previously acquired raw browser pointer at `42b9ec0`. The pending-byte mismatch branches to a trap/assert path at `42ba230`, not a nil-result cancellation branch.
- Native `CreateLiveTabContext` equivalent `7d33f88` calls browser creation `3510498` at `7d341a0`; it immediately loads returned browser interface vtable at `7d341b0`, calls interface+18, obtains live context userdata at `7d341c8`, and returns that context at `7d341e4`. There is no returned-null/cancellation check between browser creation and the interface dereference. The context userdata lookup itself may return null, but the restore callers do not accept it safely.
- TAB `RestoreTab` obtains the context at `6c86fa8`; after temporary cleanup, nonzero old Tab.browser_id causes immediate context+60 read (`6c870a4`), otherwise context+28 read (`6c87010`). There is no context==null branch. The context+28-null branch traps (`6c870b8`) rather than propagating cancellation.
- WINDOW context creation at `6c858f8` stores returned context; nonempty child insertion loads its context+28 at `6c8578c` and traps for null field. No context==null cancellation branch occurs before that dereference. A later context+28 check at `6c85988` likewise checks its field after dereferencing context.

## Public profile deletion entry (bounded result)

`deleteBrowserContext:error:` at `43e8b58` translates the name and calls `42b6394` at `43e8bc0`; successful deletion scheduling reaches `7bde3bc` at `42b64f4`. For an already loaded profile, `7bde49c..4c0` creates profile keepalive state, builds continuation `7bde5a8`, then calls close-all helper `7ad55a0` at `7bde570`. Close-all enumerates browsers through `18f0070`; the final close dispatch for a matching window is native interface vtable+68 at `7ad5e54..5c`. This establishes mediated deletion, not immediate direct Profile destruction in the ObjC method. It does NOT establish that every lower-level close/destruction continuation is asynchronous: `7ad571c..24` may invoke the caller continuation before the final close enumeration. Full lower-level profile teardown scheduling is not established in this bounded trace.

## Limits

The unchecked return paths do not prove that a public close request can actually invalidate the newly created browser synchronously. A lower-level window lifetime/close deferral may guarantee validity. Conversely no nil-cancellation contract should be inferred from these native callers. Browser close implementation details are recorded below when independently verified. Arbitrary app teardown, nested message loops, and reentrant delegates are not dynamically exercised here.

## Exact bounded original disassembly

### Created callback and immediate continuation

```asm
042b9e28: ldrb     w8, [x20, #0x370]
042b9e2c: tbz      w8, #0, #0x42ba230
042b9e30: mov      x22, x0
042b9e34: mov      x19, x1
042b9e38: add      x25, x20, #0x350
042b9e3c: add      x8, sp, #0x50
042b9e40: bl       #0x42c8400
042b9e44: str      wzr, [sp, #0x60]
042b9e48: ldr      q0, [sp, #0x50]
042b9e4c: stur     q0, [sp, #0x80]
042b9e50: stp      xzr, xzr, [sp, #0x50]
042b9e54: str      wzr, [sp, #0x90]
042b9e58: mov      w8, #1
042b9e5c: str      x8, [sp, #0x78]
042b9e60: ldr      x8, [x20, #0x360]
042b9e64: add      x0, x20, #0x350
042b9e68: add      x1, sp, #0x78
042b9e6c: blr      x8
042b9e70: ldr      x0, [sp, #0x78]
042b9e74: cmp      x0, #1
042b9e78: b.ne     #0x42b9e98
042b9e7c: ldr      x23, [sp, #0x88]
042b9e80: cbz      x23, #0x42b9ea0
042b9e84: add      x8, x23, #8
042b9e88: mov      x9, #-1
042b9e8c: ldaddal  x9, x8, [x8]
042b9e90: cbnz     x8, #0x42b9ea0
042b9e94: b        #0x42baaf4
042b9e98: tbnz     w0, #0, #0x42b9ea0
042b9e9c: bl       #0x4b35898
042b9ea0: ldr      x23, [sp, #0x58]
042b9ea4: cbz      x23, #0x42b9eb8
042b9ea8: add      x8, x23, #8
042b9eac: mov      x9, #-1
042b9eb0: ldaddal  x9, x8, [x8]
042b9eb4: cbz      x8, #0x42baa70
042b9eb8: ldrb     w8, [x20, #0x370]
042b9ebc: tbz      w8, #0, #0x42ba230
042b9ec0: ldrsw    x26, [x24, #0x38]
042b9ec4: ldr      x27, [x20, #0x380]
```

### Pending state assertion destination

```asm
042ba220: blr      x8
042ba224: b        #0x42b9ba8
042ba228: brk      #0
042ba22c: hlt      #0
042ba230: brk      #1
042ba234: adrp     x9, #0xf01000
042ba238: add      x9, x9, #0xbc0
042ba23c: cmp      x8, x9
042ba240: b.eq     #0x42ba294
042ba244: adrp     x9, #0x13ef000
```

### Native factory return boundary

```asm
07d34180: adrp     x0, #0xe2ea000
07d34184: add      x0, x0, #0x5e0
07d34188: bl       #0x14cc0
07d3418c: cbnz     w0, #0x7d34258
07d34190: mov      x0, sp
07d34194: mov      x1, x20
07d34198: bl       #0x3510c88
07d3419c: mov      x0, sp
07d341a0: bl       #0x3510498
07d341a4: mov      x19, x0
07d341a8: mov      x0, sp
07d341ac: bl       #0x24708cc
07d341b0: ldr      x8, [x19]
07d341b4: ldr      x8, [x8, #0x18]
07d341b8: mov      x0, x19
07d341bc: blr      x8
07d341c0: adrp     x1, #0xdf54000
07d341c4: add      x1, x1, #0x488
07d341c8: bl       #0x4969fc
07d341cc: mov      x19, x0
07d341d0: cbz      x20, #0x7d341e4
07d341d4: mov      x0, x20
07d341d8: bl       #0x24708cc
07d341dc: mov      w1, #0xe8
07d341e0: bl       #0x5810
07d341e4: mov      x0, x19
07d341e8: add      sp, sp, #0x1d0
07d341ec: ldp      x29, x30, [sp, #0x50]
07d341f0: ldp      x20, x19, [sp, #0x40]
07d341f4: ldp      x22, x21, [sp, #0x30]
07d341f8: ldp      x24, x23, [sp, #0x20]
07d341fc: ldp      x26, x25, [sp, #0x10]
07d34200: ldp      x28, x27, [sp], #0x60
07d34204: ret      
```

### TAB factory and continuation

```asm
06c86f88: ldr      x0, [x8, #8]
06c86f8c: add      x2, sp, #0x60
06c86f90: add      x3, sp, #0x50
06c86f94: add      x5, sp, #0x38
06c86f98: add      x6, sp, #0x20
06c86f9c: add      x7, sp, #8
06c86fa0: mov      w1, #0
06c86fa4: mov      w4, #1
06c86fa8: bl       #0x7d33f88
06c86fac: mov      x24, x0
06c86fb0: ldr      x1, [sp, #0x10]
06c86fb4: stur     x26, [x29, #-0x48]
06c86fb8: sub      x0, x29, #0x48
06c86fbc: bl       #0x3f9ab0
06c86fc0: ldrsb    w8, [sp, #0x37]
06c86fc4: tbz      w8, #0x1f, #0x6c86fd8
06c86fc8: ldr      x0, [sp, #0x20]
06c86fcc: ldr      x8, [sp, #0x30]
06c86fd0: and      x1, x8, #0x7fffffffffffffff
06c86fd4: bl       #0x5810
06c86fd8: ldrsb    w8, [sp, #0x4f]
06c86fdc: tbz      w8, #0x1f, #0x6c86ff0
06c86fe0: ldr      x0, [sp, #0x38]
06c86fe4: ldr      x8, [sp, #0x48]
06c86fe8: and      x1, x8, #0x7fffffffffffffff
06c86fec: bl       #0x5810
06c86ff0: ldrsb    w8, [sp, #0x77]
06c86ff4: tbz      w8, #0x1f, #0x6c87008
06c86ff8: ldr      x0, [sp, #0x60]
06c86ffc: ldr      x8, [sp, #0x70]
06c87000: and      x1, x8, #0x7fffffffffffffff
06c87004: bl       #0x5810
06c87008: ldr      w1, [x20, #0x54]
06c8700c: cbnz     w1, #0x6c870a4
06c87010: ldr      x8, [x24, #0x28]
06c87014: cbz      x8, #0x6c870b8
06c87018: ldr      x8, [x8]
06c8701c: ldr      w2, [x8, #0x18]
06c87020: cmp      w25, #4
06c87024: cset     w3, ne
06c87028: mov      x0, x24
06c8702c: mov      x1, x20
06c87030: mov      x4, x22
06c87034: mov      x5, x23
06c87038: bl       #0x7d33900
06c8703c: mov      x22, x0
06c87040: ldrsw    x8, [x20, #0x50]
06c87044: ldr      x9, [x20, #0x40]
06c87048: cmp      x9, x8
06c8704c: b.ls     #0x6c870b4
06c87050: ldr      x9, [x21, #0x18]
06c87054: ldr      x10, [x20, #0x38]
06c87058: mov      w11, #0x298
06c8705c: smaddl   x8, w8, w11, x10
06c87060: ldr      x0, [x9, #8]
06c87064: add      x1, x8, #0x88
06c87068: bl       #0x7b60d28
06c8706c: cbz      x19, #0x6c87074
06c87070: str      x22, [x19]
06c87074: mov      x0, x24
06c87078: ldp      x29, x30, [sp, #0xc0]
06c8707c: ldp      x20, x19, [sp, #0xb0]
06c87080: ldp      x22, x21, [sp, #0xa0]
06c87084: ldp      x24, x23, [sp, #0x90]
06c87088: ldp      x26, x25, [sp, #0x80]
06c8708c: add      sp, sp, #0xd0
06c87090: ret      
06c87094: mov      x0, x24
06c87098: mov      x1, x20
06c8709c: bl       #0x7d33e00
06c870a0: b        #0x6c8703c
06c870a4: ldr      w2, [x24, #0x60]
06c870a8: mov      x0, x21
06c870ac: bl       #0x6c870d0
06c870b0: b        #0x6c87010
06c870b4: bl       #0x4ecb040
06c870b8: brk      #0
06c870bc: hlt      #0
```

### WINDOW factory and continuation

```asm
06c858bc: ldr      w8, [sp, #0x20]
06c858c0: tbnz     w8, #0, #0x6c858d4
06c858c4: ldrsb    x8, [x22, #0xa7]
06c858c8: tbz      x8, #0x3f, #0x6c858d0
06c858cc: ldr      x8, [x22, #0x98]
06c858d0: cbz      x8, #0x6c86100
06c858d4: ldr      x8, [x20, #0x18]
06c858d8: ldr      w1, [x22, #0x38]
06c858dc: ldr      w4, [x22, #0xd0]
06c858e0: ldr      x0, [x8, #8]
06c858e4: add      x2, x22, #0x90
06c858e8: add      x3, x22, #0xc0
06c858ec: add      x5, x22, #0xd8
06c858f0: add      x6, x22, #0xa8
06c858f4: add      x7, x22, #0x20
06c858f8: bl       #0x7d33f88
06c858fc: str      x0, [sp, #0xa8]
06c85900: ldr      x8, [x22, #0x48]
06c85904: cbz      x8, #0x6c861c0
06c85908: ldrsw    x9, [x22, #0x88]
06c8590c: cmp      w9, w8
06c85910: ccmn     w9, #1, #4, lt
06c85914: csel     x9, x9, xzr, gt
06c85918: cmp      x9, x8
06c8591c: b.hs     #0x6c85fd8
06c85920: mov      x25, x0
```

### WINDOW per-child insertion boundary

```asm
06c85780: add      x19, x19, #8
06c85784: subs     x23, x23, #8
06c85788: b.eq     #0x6c85974
06c8578c: ldr      x8, [x25, #0x28]
06c85790: cbz      x8, #0x6c86154
06c85794: ldr      x1, [x19]
06c85798: ldr      w9, [x1, #8]
06c8579c: cmp      w9, w21
06c857a0: cset     w3, eq
06c857a4: ldr      x8, [x8]
06c857a8: ldr      w2, [x8, #0x18]
06c857ac: ldr      w5, [x22, #0x10]
06c857b0: mov      x0, x25
06c857b4: mov      w4, #1
06c857b8: bl       #0x7d33900
06c857bc: cbnz     x0, #0x6c856e4
```

### Profile delete scheduling

```asm
042b6488: mov      x0, x20
042b648c: bl       #0x1a298f8
042b6490: add      x1, sp, #8
042b6494: bl       #0x1a299e0
042b6498: cbz      x0, #0x42b65f0
042b649c: ldr      x8, [x23, #0x750]
042b64a0: ldrb     w9, [x8, #0x24]
042b64a4: tbz      w9, #0, #0x42b66b0
042b64a8: ldr      w8, [x8, #0x20]
042b64ac: cbz      w8, #0x42b6588
042b64b0: ldr      x21, [x20, #0xa0]
042b64b4: mov      w0, #0x28
042b64b8: bl       #0x8790
042b64bc: mov      x2, x0
042b64c0: mov      w20, #1
042b64c4: adrp     x8, #0xf000
042b64c8: add      x8, x8, #0xbc0
042b64cc: adrp     x9, #0xb6d000
042b64d0: add      x9, x9, #0x3c0
042b64d4: str      w20, [x0]
042b64d8: stp      x8, x9, [x0, #8]
042b64dc: adrp     x8, #0xb77000
042b64e0: add      x8, x8, #0x80
042b64e4: str      x8, [x0, #0x18]
042b64e8: add      x1, sp, #8
042b64ec: mov      x0, x21
042b64f0: mov      w3, #1
042b64f4: bl       #0x7bde3bc
042b64f8: str      x20, [x19]
```

### Loaded profile deletion coordination

```asm
07bde498: mov      x21, x0
07bde49c: bl       #0x302d9c8
07bde4a0: mov      w25, #1
07bde4a4: mov      w1, #1
07bde4a8: bl       #0x7da3aa4
07bde4ac: mov      w0, #0x18
07bde4b0: bl       #0x8790
07bde4b4: mov      x23, x0
07bde4b8: mov      x1, x21
07bde4bc: mov      w2, #0x20
07bde4c0: bl       #0x2b994e4
07bde4c4: and      x8, x20, #0xfffffffc00000000
07bde4c8: adrp     x9, #0xe2f2000
07bde4cc: ldr      x9, [x9, #0x208]
07bde4d0: cmp      x8, x9
07bde4d4: b.ne     #0x7bde4e0
07bde4d8: mov      x0, x20
07bde4dc: bl       #0xffa8
07bde4e0: strb     w25, [sp, #0x10]
07bde4e4: add      x24, sp, #0x10
07bde4e8: str      xzr, [sp, #0x28]
07bde4ec: mov      w0, #0x68
07bde4f0: bl       #0x8790
07bde4f4: adr      x1, #0x7bdf0b0
07bde4f8: nop      
07bde4fc: adr      x2, #0x7bdf174
07bde500: nop      
07bde504: bl       #0x1064994
07bde508: adr      x8, #0x7bde5a8
07bde50c: nop      
07bde510: stp      x8, xzr, [x0, #0x20]
07bde514: str      x20, [x0, #0x30]
07bde518: strb     w25, [x0, #0x38]
07bde51c: str      x22, [x0, #0x40]
07bde520: strb     w25, [x0, #0x48]
07bde524: str      x23, [x0, #0x50]
07bde528: strb     w25, [x0, #0x58]
07bde52c: str      x19, [x0, #0x60]
07bde530: stp      xzr, x0, [sp, #0x18]
07bde534: mov      w0, #0x30
07bde538: bl       #0x8790
07bde53c: adrp     x1, #0x2076000
07bde540: add      x1, x1, #0x9c0
07bde544: adrp     x2, #0x39a3000
07bde548: add      x2, x2, #0x680
07bde54c: bl       #0x1064994
07bde550: adr      x8, #0x7be0680
07bde554: nop      
07bde558: str      x8, [x0, #0x20]
07bde55c: str      x0, [sp, #8]
07bde560: add      x2, sp, #0x20
07bde564: add      x3, sp, #8
07bde568: mov      x0, x21
07bde56c: mov      w1, #0
07bde570: bl       #0x7ad55a0
07bde574: add      x0, sp, #8
07bde578: bl       #0xd5ff9c
07bde57c: add      x0, sp, #0x20
07bde580: bl       #0xd5ff9c
07bde584: add      x0, x24, #8
07bde588: bl       #0xd5ff9c
07bde58c: b        #0x7bde3f8
```

### Close all continuation and final iteration

```asm
07ad56b4: adr      x2, #0x7ad5964
07ad56b8: nop      
07ad56bc: add      x1, sp, #8
07ad56c0: mov      w3, #0
07ad56c4: mov      w4, #0
07ad56c8: bl       #0x18f0070
07ad56cc: ldurb    w8, [x29, #-0x41]
07ad56d0: tbnz     w8, #0, #0x7ad5708
07ad56d4: ldr      x8, [x20]
07ad56d8: cbnz     x8, #0x7ad571c
07ad56dc: ldr      x8, [x21, #0x618]
07ad56e0: ldr      x8, [x8, #0x2a0]
07ad56e4: ldr      x0, [x8, #8]
07ad56e8: sub      x8, x29, #0x40
07ad56ec: str      x8, [sp, #8]
07ad56f0: adr      x2, #0x7ad5dd4
07ad56f4: nop      
07ad56f8: add      x1, sp, #8
07ad56fc: mov      w3, #0
07ad5700: mov      w4, #0
07ad5704: bl       #0x18f0070
07ad5708: ldp      x29, x30, [sp, #0x90]
07ad570c: ldp      x20, x19, [sp, #0x80]
07ad5710: ldp      x22, x21, [sp, #0x70]
07ad5714: add      sp, sp, #0xa0
07ad5718: ret      
07ad571c: mov      x0, x20
07ad5720: mov      x1, x19
07ad5724: bl       #0x40bd130
07ad5728: b        #0x7ad56dc
```

### Close-all window dispatch

```asm
07ad5dd4: stp      x22, x21, [sp, #-0x30]!
07ad5dd8: stp      x20, x19, [sp, #0x10]
07ad5ddc: stp      x29, x30, [sp, #0x20]
07ad5de0: add      x29, sp, #0x20
07ad5de4: mov      x19, x1
07ad5de8: ldr      x20, [x0]
07ad5dec: ldrb     w21, [x20, #8]
07ad5df0: ldr      x8, [x1]
07ad5df4: ldr      x8, [x8, #0x38]
07ad5df8: mov      x0, x1
07ad5dfc: blr      x8
07ad5e00: cbz      w21, #0x7ad5e10
07ad5e04: ldr      x8, [x0]
07ad5e08: ldr      x8, [x8, #0x148]
07ad5e0c: blr      x8
07ad5e10: ldr      x8, [x20]
07ad5e14: cmp      x0, x8
07ad5e18: b.eq     #0x7ad5e30
07ad5e1c: mov      w0, #1
07ad5e20: ldp      x29, x30, [sp, #0x20]
07ad5e24: ldp      x20, x19, [sp, #0x10]
07ad5e28: ldp      x22, x21, [sp], #0x30
07ad5e2c: ret      
07ad5e30: ldr      x8, [x19]
07ad5e34: ldr      x8, [x8, #0x28]
07ad5e38: mov      x0, x19
07ad5e3c: blr      x8
07ad5e40: cbz      x0, #0x7ad5e1c
07ad5e44: ldr      x8, [x19]
07ad5e48: ldr      x8, [x8, #0x28]
07ad5e4c: mov      x0, x19
07ad5e50: blr      x8
07ad5e54: ldr      x8, [x0]
07ad5e58: ldr      x8, [x8, #0x68]
07ad5e5c: blr      x8
07ad5e60: b        #0x7ad5e1c
```

## Verified normal close deferral mechanism

The public original ArcBrowser declares no `close` selector; its public close request is `tryToCloseSkipBeforeUnloadHandlers:callback:` (IMP `43e1190` -> native `42fa5c8`). That bridge checks native liveness and already-closing byte +178, but no full-restoration/creation flag. The empty-browser unload list is empty (`7d612e8..f0`), so `7d621f4` returns false and `42fa964..96c` immediately invokes `42f9f84(Impl,true)`. This delivers the close-request completion synchronously through `4307090` at `42fa134`, then dispatches Browser.GetWindow()->virtual+68 at `42fa16c..184`.

The primary window vtable starting `da86c60` has virtual+68 at `da86cc8`, whose original fixup pointer is `0x100000043cbe38`. `43cbe38` checks native browser weak liveness, calls `35ea6c0` and `35ea198`, and on empty tabstrip performs UI association cleanup. `35ea198` marks closing byte +148 at `35ea30c..310` and synchronously notifies close observers, then constructs a task closure carrying the browser weak handle (`35ea500..54c`) and posts it through the same taskrunner dispatch interface as known asynchronous insertion notifications (`35ea550..564`). Closure `3b26570` checks the weak handle and calls `33ca620`; that calls profile browser-collection removal `33ca660`, which invokes the browser destructor virtual+8 at `33ca7a4..ac`.

Therefore the normal empty-browser close path has posted actual browser destruction; it does not communicate cancellation to the suspended native restoration by returning nil. Browser logical-closing state and close observers change synchronously. This does not prove behavior if created or a close observer pumps a nested run loop, directly destroys internal C++ state, or causes unrelated shutdown. No guarantee about application finished callback under cancellation is established here. Exact factory-to-window-vtable construction mapping is independently reviewed separately; table mapping and bounded dispatch chain are verified below.

### Public close request immediate empty-browser branch

```asm
042fa934: stp      x8, x9, [x0, #0x18]
042fa938: stp      xzr, x25, [x0, #0x28]
042fa93c: str      x26, [x0, #0x38]
042fa940: str      x0, [sp, #0x18]
042fa944: add      x2, sp, #0x18
042fa948: mov      x0, x23
042fa94c: mov      x1, x22
042fa950: bl       #0x7d621f4
042fa954: mov      x22, x0
042fa958: ldr      x0, [sp, #0x18]
042fa95c: cbnz     x0, #0x42fa974
042fa960: tbnz     w22, #0, #0x42fa6f4
042fa964: mov      x0, x21
042fa968: mov      w1, #1
042fa96c: bl       #0x42f9f84
042fa970: b        #0x42fa6f4
042fa974: mov      w8, #-1
042fa978: ldaddal  w8, w8, [x0]
042fa97c: cmp      w8, #1
```

### Close completion and window virtual close dispatch

```asm
042fa128: tbz      w27, #0, #0x42fa1b0
042fa12c: cbz      x22, #0x42fa438
042fa130: mov      x0, x22
042fa134: bl       #0x4307090
042fa138: mov      w8, #-1
042fa13c: ldaddal  w8, w8, [x22]
042fa140: cmp      w8, #1
042fa144: b.ne     #0x42fa154
042fa148: ldr      x8, [x22, #0x10]
042fa14c: mov      x0, x22
042fa150: blr      x8
042fa154: cbz      x20, #0x42fa188
042fa158: ldapurb  w8, [x20, #4]
042fa15c: cbnz     w8, #0x42fa188
042fa160: cbz      x23, #0x42fa188
042fa164: ldapurb  w8, [x20, #4]
042fa168: cbnz     w8, #0x42fa444
042fa16c: ldr      x8, [x23]
042fa170: ldr      x8, [x8, #0x28]
042fa174: mov      x0, x23
042fa178: blr      x8
042fa17c: ldr      x8, [x0]
042fa180: ldr      x8, [x8, #0x68]
042fa184: blr      x8
```

### Window Close

```asm
043cbe38: stp      x22, x21, [sp, #-0x30]!
043cbe3c: stp      x20, x19, [sp, #0x10]
043cbe40: stp      x29, x30, [sp, #0x20]
043cbe44: add      x29, sp, #0x20
043cbe48: ldr      x8, [x0, #0x28]
043cbe4c: cbz      x8, #0x43cbe58
043cbe50: ldapurb  w8, [x8, #4]
043cbe54: cbz      w8, #0x43cbe68
043cbe58: ldp      x29, x30, [sp, #0x20]
043cbe5c: ldp      x20, x19, [sp, #0x10]
043cbe60: ldp      x22, x21, [sp], #0x30
043cbe64: ret      
043cbe68: mov      x19, x0
043cbe6c: ldr      x20, [x0, #0x30]
043cbe70: cbz      x20, #0x43cbe58
043cbe74: ldr      x8, [x20]
043cbe78: ldr      x8, [x8, #0x18]
043cbe7c: mov      x0, x20
043cbe80: blr      x8
043cbe84: ldr      x8, [x0, #8]!
043cbe88: adrp     x22, #0xe2f2000
043cbe8c: cbz      x8, #0x43cbebc
043cbe90: adrp     x9, #0xdf54000
043cbe94: add      x9, x9, #0x710
043cbe98: b        #0x43cbea4
043cbe9c: ldr      x8, [x8]
043cbea0: cbz      x8, #0x43cbebc
043cbea4: ldr      x10, [x8, #0x20]
043cbea8: cmp      x10, x9
043cbeac: b.hi     #0x43cbe9c
043cbeb0: b.eq     #0x43cbec4
043cbeb4: ldr      x8, [x8, #8]
043cbeb8: cbnz     x8, #0x43cbea4
043cbebc: mov      x21, #0
043cbec0: b        #0x43cbee8
043cbec4: cmp      x8, x0
043cbec8: b.eq     #0x43cbebc
043cbecc: ldr      x21, [x8, #0x28]
043cbed0: and      x8, x21, #0xfffffffc00000000
043cbed4: ldr      x9, [x22, #0x208]
043cbed8: cmp      x8, x9
043cbedc: b.ne     #0x43cbee8
043cbee0: mov      x0, x21
043cbee4: bl       #0xffa8
043cbee8: and      x8, x21, #0xfffffffc00000000
043cbeec: ldr      x9, [x22, #0x208]
043cbef0: cmp      x8, x9
043cbef4: b.ne     #0x43cbf00
043cbef8: mov      x0, x21
043cbefc: bl       #0x1fee4
043cbf00: mov      x0, x21
043cbf04: bl       #0x35ea6c0
043cbf08: cbz      w0, #0x43cbe58
043cbf0c: mov      x0, x21
043cbf10: bl       #0x35ea198
043cbf14: ldr      x8, [x20]
043cbf18: ldr      x8, [x8, #0x78]
043cbf1c: mov      x0, x20
043cbf20: blr      x8
043cbf24: ldr      x8, [x0]
043cbf28: ldr      x8, [x8, #0x18]
043cbf2c: cbnz     x8, #0x43cbe58
043cbf30: mov      x0, x19
043cbf34: bl       #0x43c9404
043cbf38: ldr      x20, [x19, #0x20]
043cbf3c: str      xzr, [x19, #0x20]
```

### Mark closing and observer entry

```asm
035ea2c0: ldr      x0, [x19, #0x28]
035ea2c4: ldr      x8, [x0]
035ea2c8: ldr      x8, [x8, #0x78]
035ea2cc: blr      x8
035ea2d0: ldr      x8, [x0]
035ea2d4: ldr      x8, [x8, #0x18]
035ea2d8: cbz      x8, #0x35ea30c
035ea2dc: ldr      x0, [x19, #0x28]
035ea2e0: ldr      x8, [x0]
035ea2e4: ldr      x8, [x8, #0x78]
035ea2e8: blr      x8
035ea2ec: ldp      x29, x30, [sp, #0x90]
035ea2f0: ldp      x20, x19, [sp, #0x80]
035ea2f4: ldp      x22, x21, [sp, #0x70]
035ea2f8: ldp      x24, x23, [sp, #0x60]
035ea2fc: ldp      x26, x25, [sp, #0x50]
035ea300: ldp      x28, x27, [sp, #0x40]
035ea304: add      sp, sp, #0xa0
035ea308: b        #0x31bb174
035ea30c: mov      w8, #1
035ea310: strb     w8, [x19, #0x148]
035ea314: add      x22, x19, #0xd8
035ea318: add      x8, x19, #0xd8
035ea31c: ldr      x8, [x8, #8]
035ea320: cmp      x8, x22
035ea324: b.eq     #0x35ea42c
035ea328: ldr      x9, [x8, #0x10]
035ea32c: cbz      x9, #0x35ea31c
```

### Posted actual browser deletion

```asm
035ea4c4: adrp     x0, #0xe379000
035ea4c8: add      x0, x0, #0x858
035ea4cc: ldr      x8, [x0]
035ea4d0: blr      x8
035ea4d4: ldr      x8, [x0]
035ea4d8: cbz      x8, #0x35ea5e4
035ea4dc: ldr      x20, [x8]
035ea4e0: cbz      x20, #0x35ea5e4
035ea4e4: adrp     x0, #0xd326000
035ea4e8: add      x0, x0, #0x7e2
035ea4ec: adrp     x1, #0xd326000
035ea4f0: add      x1, x1, #0x789
035ea4f4: add      x8, sp, #8
035ea4f8: mov      w2, #0x480
035ea4fc: bl       #0x29e40
035ea500: ldr      x0, [x19, #0x28]
035ea504: ldr      x8, [x0]
035ea508: ldr      x8, [x8, #0x68]
035ea50c: blr      x8
035ea510: mov      x21, x0
035ea514: mov      x22, x1
035ea518: mov      w0, #0x38
035ea51c: bl       #0x8790
035ea520: mov      x2, x0
035ea524: mov      w8, #1
035ea528: adrp     x9, #0x3b26000
035ea52c: add      x9, x9, #0x570
035ea530: str      w8, [x0]
035ea534: adrp     x8, #0x39e9000
035ea538: add      x8, x8, #0xcc0
035ea53c: adrp     x10, #0xb77000
035ea540: add      x10, x10, #0x80
035ea544: stp      x9, x8, [x0, #8]
035ea548: str      x10, [x0, #0x18]
035ea54c: stp      x21, x22, [x0, #0x28]
035ea550: ldr      x8, [x20]
035ea554: ldr      x8, [x8]
035ea558: add      x1, sp, #8
035ea55c: mov      x0, x20
035ea560: mov      x3, #0
035ea564: blr      x8
035ea568: ldr      x0, [x19, #0x28]
035ea56c: cbz      x0, #0x35ea590
035ea570: ldr      x8, [x0]
035ea574: ldr      x8, [x8, #0x18]
035ea578: blr      x8
035ea57c: adrp     x1, #0xdf6a000
035ea580: add      x1, x1, #0x920
035ea584: bl       #0x4969fc
035ea588: cbz      x0, #0x35ea590
035ea58c: bl       #0x35ea9f8
```

### Weak deletion task

```asm
03b26570: mov      x8, x0
03b26574: ldr      x0, [x0, #0x28]
03b26578: str      xzr, [x8, #0x28]
03b2657c: cbz      x0, #0x3b265b8
03b26580: sub      sp, sp, #0x20
03b26584: stp      x29, x30, [sp, #0x10]
03b26588: add      x29, sp, #0x10
03b2658c: ldr      x8, [x8, #0x30]
03b26590: ldapurb  w9, [x0, #4]
03b26594: cmp      w9, #0
03b26598: ccmp     x8, #0, #4, eq
03b2659c: b.ne     #0x3b265c4
03b265a0: mov      w8, #-1
03b265a4: ldaddal  w8, w8, [x0]
03b265a8: cmp      w8, #1
03b265ac: ldp      x29, x30, [sp, #0x10]
03b265b0: add      sp, sp, #0x20
03b265b4: b.eq     #0x3b265bc
03b265b8: ret      
03b265bc: mov      w1, #8
03b265c0: b        #0x5810
03b265c4: ldapurb  w9, [x0, #4]
03b265c8: cmp      w9, #0
03b265cc: csel     x8, x8, xzr, eq
03b265d0: str      x0, [sp, #8]
03b265d4: mov      x0, x8
03b265d8: bl       #0x33ca620
03b265dc: ldr      x0, [sp, #8]
03b265e0: b        #0x3b265a0
```

### Remove browser and invoke destructor

```asm
033ca620: cbz      x0, #0x33ca654
033ca624: stp      x20, x19, [sp, #-0x20]!
033ca628: stp      x29, x30, [sp, #0x10]
033ca62c: add      x29, sp, #0x10
033ca630: mov      x19, x0
033ca634: ldr      x8, [x0]
033ca638: ldr      x8, [x8, #0x38]
033ca63c: blr      x8
033ca640: bl       #0x2376d54
033ca644: mov      x1, x19
033ca648: ldp      x29, x30, [sp, #0x10]
033ca64c: ldp      x20, x19, [sp], #0x20
033ca650: b        #0x33ca660
033ca654: brk      #0
033ca658: hlt      #0
033ca65c: brk      #1
033ca660: sub      sp, sp, #0x90
033ca664: stp      x26, x25, [sp, #0x40]
033ca668: stp      x24, x23, [sp, #0x50]
033ca66c: stp      x22, x21, [sp, #0x60]
033ca670: stp      x20, x19, [sp, #0x70]
033ca674: stp      x29, x30, [sp, #0x80]
033ca678: add      x29, sp, #0x80
033ca67c: str      xzr, [sp, #0x30]
033ca680: movi     v0.2d, #0000000000000000
033ca684: stp      q0, q0, [sp, #0x10]
033ca688: ldr      x8, [x0, #0x70]
033ca68c: lsl      x8, x8, #5
033ca690: cbz      x8, #0x33ca854
033ca694: mov      x19, x0
033ca698: ldr      x20, [x0, #0x68]
033ca69c: add      x21, x20, #0x20
033ca6a0: ldr      x9, [x20, #0x18]
033ca6a4: cmp      x9, x1
033ca6a8: b.ne     #0x33cabc0
033ca6ac: mov      x0, x19
```

### Collection destructor invocation

```asm
033ca790: ldrb     w9, [sp, #0x30]
033ca794: tbz      w9, #0, #0x33caad0
033ca798: ldr      x0, [sp, #0x28]
033ca79c: str      xzr, [sp, #0x28]
033ca7a0: cbz      x0, #0x33ca7b4
033ca7a4: ldr      x8, [x0]
033ca7a8: ldr      x8, [x8, #8]
033ca7ac: blr      x8
033ca7b0: ldr      x8, [x19, #0x70]
```

### Empty unload list

```asm
07d61294: sub      sp, sp, #0x50
07d61298: stp      x24, x23, [sp, #0x10]
07d6129c: stp      x22, x21, [sp, #0x20]
07d612a0: stp      x20, x19, [sp, #0x30]
07d612a4: stp      x29, x30, [sp, #0x40]
07d612a8: add      x29, sp, #0x40
07d612ac: mov      x19, x0
07d612b0: mov      x20, x8
07d612b4: ldr      w8, [x0, #0x98]
07d612b8: cbnz     w8, #0x7d612c4
07d612bc: ldr      x8, [x19, #0x90]
07d612c0: cbnz     x8, #0x7d613f4
07d612c4: str      xzr, [x20, #0x10]
07d612c8: mov      x8, x20
07d612cc: str      xzr, [x8, #8]!
07d612d0: str      x8, [x20]
07d612d4: ldr      x0, [x19, #0x28]
07d612d8: ldr      x8, [x0]
07d612dc: ldr      x8, [x8, #0x78]
07d612e0: blr      x8
07d612e4: ldr      x8, [x0]
07d612e8: ldr      w8, [x8, #0x18]
07d612ec: cmp      w8, #1
07d612f0: b.lt     #0x7d613dc
07d612f4: mov      w21, #0
07d612f8: add      x23, x19, #0x88
07d612fc: adrp     x24, #0xe2f2000
07d61300: b        #0x7d613ac
07d61304: ldr      x8, [x8]
07d61308: cbz      x8, #0x7d61334
07d6130c: ldr      x9, [x8, #0x20]
07d61310: cmp      x22, x9
07d61314: b.lo     #0x7d61304
```

## Independent exact native window binding confirmation

The native browser constructor calls window constructor `35116fc` at `3510a14`. That constructor loads primary vtable `da86c60` at `3511724..28` and installs it at `3511748`. Its +68 entry at `da86cc8` points to `43cbe38`. It also copies weak Browser fields +28/+30 at `3511754..1780`. Thus the identified close implementation is bound to the native window constructed by the normal factory path, not merely a candidate. Independent reviewer confirmed this constructor binding and the deletion task chain.

This proves ordinary no-nested-loop public close dispatch has deferred actual browser destruction. No restore-specific native observer guard or nil-result cancellation propagation was found on that ordinary path. It remains unsafe to generalize to arbitrary profile teardown or nested run-loop pumping without further evidence.

### Factory native window construction call

```asm
03510a04: mov      w0, #0x2b8
03510a08: bl       #0x8790
03510a0c: mov      x24, x0
03510a10: mov      x1, x20
03510a14: bl       #0x35116fc
03510a18: ldr      x0, [x20, #0x48]
03510a1c: str      x24, [x20, #0x48]
03510a20: cbnz     x0, #0x3510b78
```

### Native window primary vtable and weak Browser

```asm
035116fc: sub      sp, sp, #0x70
03511700: stp      x28, x27, [sp, #0x10]
03511704: stp      x26, x25, [sp, #0x20]
03511708: stp      x24, x23, [sp, #0x30]
0351170c: stp      x22, x21, [sp, #0x40]
03511710: stp      x20, x19, [sp, #0x50]
03511714: stp      x29, x30, [sp, #0x60]
03511718: add      x29, sp, #0x60
0351171c: mov      x20, x1
03511720: mov      x19, x0
03511724: adrp     x8, #0xda86000
03511728: add      x8, x8, #0xc60
0351172c: add      x9, x8, #0x3c0
03511730: mov      x21, x0
03511734: str      x9, [x21, #0x10]!
03511738: add      x9, x8, #0x400
0351173c: mov      x27, x0
03511740: str      x9, [x27, #0x18]!
03511744: add      x9, x8, #0x378
03511748: stp      x8, x9, [x0]
0351174c: str      xzr, [x0, #0x20]
03511750: cbz      x1, #0x3511968
03511754: ldr      x8, [x20, #0x100]
03511758: cbz      x8, #0x3511770
0351175c: mov      w9, #1
03511760: ldadd    w9, w9, [x8]
03511764: mov      w10, #0x7fffffff
03511768: cmp      w9, w10
0351176c: b.eq     #0x3511ea8
03511770: ldr      x9, [x20, #0x108]
03511774: add      x10, x9, #0x28
03511778: cmp      x9, #0
0351177c: csel     x9, xzr, x10, eq
03511780: stp      x8, x9, [x19, #0x28]
```

