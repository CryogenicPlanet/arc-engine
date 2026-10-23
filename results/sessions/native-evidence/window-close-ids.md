# Original whole-window close: per-WebContents callback IDs

Read-only static audit, 2026-10-23. Original ArcCore arm64 unslid VAs, binary SHA-256 b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d. No repo edits, builds, apps, network, credentials, or encrypted session-file access.

## Bounded conclusion

Normal whole-window closure does not expose WINDOW child historical IDs through `webContents:didCloseWithSessionID:`. The per-tab historical-entry producer suppresses duplicate tab capture while its LiveTabContext is in the helper's closing-context set. That yields an absent removal-record optional. Native WebContents destruction converts absent optional to an ArcSessionID containing -1, not nil. Thus the callback is not a supported source of persisted child historical IDs for restart expectations. This finding is original static evidence, not a new runtime callback observation.

## Exact data flow

1. Whole-browser closing calls the restoration helper's BrowserClosing method at `35ea2bc` (helper starts `34f54c0`). It inserts the exact LiveTabContext pointer into the helper+80 closing-context tree at `34f5508..5568` before constructing/populating the WINDOW entry (`34f5580+`).
2. Original BrowserTabStripModelDelegate CreateHistoricalTab starts `319633c`; it obtains the same service and live tab, computes the current tab index, and calls helper `322a1fc` at `3196580`. The helper finds the LiveTabContext for the live WC, reads its live-context userdata at `322a254..26c`, checks membership in helper+80 at `322a270..290`, and returns all-zero (absent optional) at `322a294`. It performs no recursive WINDOW-child ID lookup on this branch.
3. NativeWC OnTabStripModelChanged (`4397de8`) handles removal variant2 at `4397f3c`, finds its exact live WC in removal records (stride0x30), and for removal reasons0/3 copies optional SessionID payload+24 / engagement+28 to its own +338 /+33c at `4397f94..fa0`. Reason1 instead calls destruction notification directly. This observer does not query restoration service entries or substitute WINDOW/root/live IDs.
4. Destruction notification `438a18c` reads engagement+33c; if absent, chooses scalar -1 at `438a1c4`, else stored +338 at `438a1d0`. It invokes UIDelegate virtual+20 via `4383ad8` (`438a224..23c`). Bridge vtable base da89030 has +20 slot da89050 ->4445cf0; bridge constructs ArcSessionID via `initWithSessionId:` and calls `webContents:didCloseWithSessionID:` at `4445d54`.
5. ArcSessionID initializer `440abb4` copies the 32-bit scalar and sets its optional engagement byte regardless of value (`440abf4..ac00`). There is no -1-to-nil conversion. The bridge holds weak delegate/wrapper references; no callback delivery guarantee is inferred when a delegate or wrapper has already disappeared.

## Limits

Direct historical single-tab capture has a different branch and may return its new Tab.id. These findings apply to the ordinary whole-window BrowserClosing path that inserts its context before removing tabs. They do not promise callback ordering relative to persistence, session-file flush, or encrypted file readiness. No decryption/private C++ ABI use is needed for this audit, and no opaque-file barrier is validated by it.

## Exact bounded original disassembly

### BrowserClosing invocation before tab closure

```asm
035ea260: ldr      x0, [x19, #0x28]
035ea264: ldr      x8, [x0]
035ea268: ldr      x8, [x8, #0x78]
035ea26c: blr      x8
035ea270: ldr      x8, [x0]
035ea274: ldr      w8, [x8, #0x18]
035ea278: cmp      w8, #0
035ea27c: cset     w8, ne
035ea280: cmp      w21, #2
035ea284: ccmp     w21, #4, #4, ne
035ea288: csinc    w8, w8, wzr, ne
035ea28c: cmp      x20, #0
035ea290: ccmp     w8, #0, #4, ne
035ea294: b.eq     #0x35ea2c0
035ea298: ldr      x0, [x19, #0x28]
035ea29c: ldr      x8, [x0]
035ea2a0: ldr      x8, [x8, #0x18]
035ea2a4: blr      x8
035ea2a8: adrp     x1, #0xdf54000
035ea2ac: add      x1, x1, #0x488
035ea2b0: bl       #0x4969fc
035ea2b4: mov      x1, x0
035ea2b8: add      x0, x20, #0x18
035ea2bc: bl       #0x34f54c0
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
```

### Closing context membership insertion

```asm
034f54c0: sub      sp, sp, #0x70
034f54c4: stp      x26, x25, [sp, #0x20]
034f54c8: stp      x24, x23, [sp, #0x30]
034f54cc: stp      x22, x21, [sp, #0x40]
034f54d0: stp      x20, x19, [sp, #0x50]
034f54d4: stp      x29, x30, [sp, #0x60]
034f54d8: add      x29, sp, #0x60
034f54dc: ldrb     w8, [x0, #0x38]
034f54e0: tbnz     w8, #0, #0x34f5778
034f54e4: mov      x20, x1
034f54e8: mov      x19, x0
034f54ec: and      x8, x1, #0xfffffffc00000000
034f54f0: adrp     x21, #0xe2f2000
034f54f4: ldr      x9, [x21, #0x208]
034f54f8: cmp      x8, x9
034f54fc: b.ne     #0x34f5508
034f5500: mov      x0, x20
034f5504: bl       #0xffa8
034f5508: mov      x23, x19
034f550c: ldr      x8, [x23, #0x80]!
034f5510: cbnz     x8, #0x34f5a48
034f5514: mov      x22, x23
034f5518: mov      w0, #0x28
034f551c: bl       #0x8790
034f5520: cbz      x0, #0x34f5a74
034f5524: mov      x1, x0
034f5528: cmn      x0, #0x20
034f552c: b.eq     #0x34f5a74
034f5530: str      x20, [x1, #0x20]
034f5534: stp      xzr, xzr, [x1]
034f5538: str      x22, [x1, #0x10]
034f553c: str      x1, [x23]
034f5540: ldr      x8, [x19, #0x78]
034f5544: ldr      x8, [x8]
034f5548: cbz      x8, #0x34f5554
034f554c: str      x8, [x19, #0x78]
034f5550: ldr      x1, [x23]
034f5554: ldr      x0, [x19, #0x80]
034f5558: bl       #0x67d9c
034f555c: mov      x0, #0
034f5560: ldr      x8, [x19, #0x88]
034f5564: add      x8, x8, #1
034f5568: str      x8, [x19, #0x88]
034f556c: and      x8, x0, #0xfffffffc00000000
034f5570: ldr      x9, [x21, #0x208]
034f5574: cmp      x8, x9
034f5578: b.ne     #0x34f5580
034f557c: bl       #0x270f0
034f5580: mov      w0, #0xf0
034f5584: bl       #0x8790
034f5588: mov      x21, x0
034f558c: bl       #0x3aa7c30
034f5590: ldr      w8, [x20, #0x40]
034f5594: str      w8, [x0, #0x38]
034f5598: ldr      x8, [x20, #0x28]
```

### Per-tab delegate helper call

```asm
0319653c: ldr      x0, [x20, #8]
03196540: ldr      x8, [x0]
03196544: ldr      x8, [x8, #0x78]
03196548: blr      x8
0319654c: mov      x1, x19
03196550: bl       #0x27de0
03196554: mov      x19, x0
03196558: ldr      x0, [x21, #0x108]
0319655c: cbz      x0, #0x3196574
03196560: adrp     x1, #0xccb9000
03196564: add      x1, x1, #0xd1f
03196568: mov      w2, #0x17
0319656c: bl       #0x3bc2f4
03196570: tbnz     w0, #0, #0x319659c
03196574: add      x0, x21, #0x18
03196578: mov      x1, x22
0319657c: mov      x2, x19
03196580: bl       #0x322a1fc
03196584: and      x0, x0, #0xffffffffff
03196588: ldp      x29, x30, [sp, #0x30]
0319658c: ldp      x20, x19, [sp, #0x20]
03196590: ldp      x22, x21, [sp, #0x10]
03196594: ldp      x24, x23, [sp], #0x40
03196598: ret      
0319659c: mov      x0, #0
031965a0: b        #0x3196588
```

### CreateHistoricalTab context suppression

```asm
0322a1fc: sub      sp, sp, #0x70
0322a200: stp      x22, x21, [sp, #0x40]
0322a204: stp      x20, x19, [sp, #0x50]
0322a208: stp      x29, x30, [sp, #0x60]
0322a20c: add      x29, sp, #0x60
0322a210: ldrb     w8, [x0, #0x38]
0322a214: tbnz     w8, #0, #0x322a294
0322a218: mov      x21, x2
0322a21c: mov      x20, x1
0322a220: mov      x19, x0
0322a224: ldr      x1, [x1, #0x10]
0322a228: adrp     x8, #0xe481000
0322a22c: ldr      x8, [x8, #0x618]
0322a230: ldr      x8, [x8, #0x2a0]
0322a234: ldr      x0, [x8, #8]
0322a238: bl       #0x18efdf0
0322a23c: cbz      x0, #0x322a270
0322a240: mov      x22, x0
0322a244: ldr      x8, [x0]
0322a248: ldr      x8, [x8, #0x50]
0322a24c: blr      x8
0322a250: tbnz     w0, #0, #0x322a31c
0322a254: ldr      x8, [x22]
0322a258: ldr      x8, [x8, #0x18]
0322a25c: mov      x0, x22
0322a260: blr      x8
0322a264: adrp     x1, #0xdf54000
0322a268: add      x1, x1, #0x488
0322a26c: bl       #0x4969fc
0322a270: mov      x8, x19
0322a274: ldr      x9, [x8, #0x80]!
0322a278: cbz      x9, #0x322a338
0322a27c: ldr      x10, [x9, #0x20]
0322a280: cmp      x0, x10
0322a284: b.lo     #0x322a324
0322a288: b.ne     #0x322a330
0322a28c: cmp      x9, x8
0322a290: b.eq     #0x322a338
0322a294: mov      x0, #0
0322a298: ldp      x29, x30, [sp, #0x60]
0322a29c: ldp      x20, x19, [sp, #0x50]
0322a2a0: ldp      x22, x21, [sp, #0x40]
0322a2a4: add      sp, sp, #0x70
0322a2a8: ret      
0322a2ac: ldr      x12, [x12]
0322a2b0: cbz      x12, #0x322a2d8
0322a2b4: ldr      w13, [x12, #0x1c]
0322a2b8: cmp      w8, w13
0322a2bc: b.lt     #0x322a2ac
0322a2c0: b.le     #0x322a2d0
0322a2c4: ldr      x12, [x12, #8]
0322a2c8: cbnz     x12, #0x322a2b4
0322a2cc: b        #0x322a2d8
0322a2d0: cmp      x12, x11
0322a2d4: b.ne     #0x322a294
0322a2d8: ldr      x11, [x10, #8]
0322a2dc: cbz      x11, #0x322a304
0322a2e0: mov      x10, x11
0322a2e4: ldr      x11, [x11]
0322a2e8: cbnz     x11, #0x322a2e0
0322a2ec: cmp      x10, x9
0322a2f0: b.eq     #0x322a3c0
0322a2f4: mov      x11, x10
0322a2f8: ldr      x12, [x11, #0x38]!
0322a2fc: cbz      x12, #0x322a2d8
0322a300: b        #0x322a2b4
0322a304: mov      x11, x10
0322a308: ldr      x10, [x10, #0x10]
0322a30c: ldr      x12, [x10]
0322a310: cmp      x11, x12
0322a314: b.ne     #0x322a304
0322a318: b        #0x322a2ec
0322a31c: mov      x0, #0
0322a320: b        #0x322a270
0322a324: ldr      x9, [x9]
0322a328: cbnz     x9, #0x322a27c
0322a32c: b        #0x322a338
0322a330: ldr      x9, [x9, #8]
0322a334: cbnz     x9, #0x322a27c
```

### Removed-record copy

```asm
04397f24: cmp      x8, x9
04397f28: b.ne     #0x4398104
04397f2c: mov      x0, x19
04397f30: mov      w1, #0x20
04397f34: bl       #0x5810
04397f38: b        #0x4397e3c
04397f3c: ldrb     w9, [x19, #0x28]
04397f40: cmp      w9, #1
04397f44: b.ne     #0x4398184
04397f48: ldr      x9, [x19, #0x10]
04397f4c: add      x9, x9, x9, lsl #1
04397f50: lsl      x10, x9, #4
04397f54: cbz      x10, #0x4397e3c
04397f58: ldr      x9, [x19, #8]
04397f5c: add      x9, x9, #0x24
04397f60: ldur     x11, [x9, #-0x14]
04397f64: cmp      x11, x8
04397f68: b.eq     #0x4397f7c
04397f6c: add      x9, x9, #0x30
04397f70: subs     x10, x10, #0x30
04397f74: b.ne     #0x4397f60
04397f78: b        #0x4397e3c
04397f7c: ldur     w8, [x9, #-8]
04397f80: cmp      w8, #3
04397f84: b.eq     #0x4397f94
04397f88: cmp      w8, #1
04397f8c: b.eq     #0x4397fa8
04397f90: cbnz     w8, #0x4397e3c
04397f94: ldr      w8, [x9]
04397f98: ldrb     w9, [x9, #4]
04397f9c: strb     w9, [x20, #0x33c]
04397fa0: str      w8, [x20, #0x338]
04397fa4: b        #0x4397e3c
04397fa8: mov      x0, x20
04397fac: bl       #0x438a18c
04397fb0: b        #0x4397e3c
```

### Destruction chooses optional value or sentinel

```asm
0438a18c: sub      sp, sp, #0x60
0438a190: stp      x24, x23, [sp, #0x20]
0438a194: stp      x22, x21, [sp, #0x30]
0438a198: stp      x20, x19, [sp, #0x40]
0438a19c: stp      x29, x30, [sp, #0x50]
0438a1a0: add      x29, sp, #0x50
0438a1a4: mov      x19, x0
0438a1a8: mov      w8, #0xa
0438a1ac: str      x8, [sp, #0x10]
0438a1b0: adrp     x22, #0xe4a4000
0438a1b4: ldrb     w0, [x22, #0x45a]
0438a1b8: cbnz     w0, #0x438a3fc
0438a1bc: ldrb     w8, [x19, #0x33c]
0438a1c0: cbnz     w8, #0x438a1d0
0438a1c4: mov      w5, #-1
0438a1c8: b        #0x438a1d4
0438a1cc: bl       #0x50d342c
0438a1d0: ldr      w5, [x19, #0x338]
0438a1d4: adrp     x8, #0xe49c000
0438a1d8: ldrb     w8, [x8, #0xf50]
0438a1dc: tbnz     w8, #0, #0x438a268
0438a1e0: ldr      x20, [x19, #0x10]
0438a1e4: cbz      x20, #0x438a1cc
0438a1e8: ldur     q0, [x19, #8]
0438a1ec: add      x8, x20, #8
0438a1f0: ldar     x8, [x8]
0438a1f4: cmn      x8, #1
0438a1f8: b.eq     #0x438a1cc
0438a1fc: add      x10, x8, #1
0438a200: add      x9, x20, #8
0438a204: mov      x11, x8
0438a208: casal    x11, x10, [x9]
0438a20c: cmp      x11, x8
0438a210: mov      x8, x11
0438a214: b.ne     #0x438a1f4
0438a218: str      q0, [sp]
0438a21c: mov      w8, #1
0438a220: ldadd    x8, x8, [x9]
0438a224: adrp     x1, #0xd382000
0438a228: add      x1, x1, #0x6bd
0438a22c: add      x0, x19, #0x348
0438a230: mov      x4, sp
0438a234: mov      w2, #0x20
0438a238: mov      w3, #1
0438a23c: bl       #0x4383ad8
0438a240: ldr      x21, [sp, #8]
```

### UIDelegate integer callback invocation

```asm
04383c48: mov      w8, #0xa
04383c4c: str      x8, [sp, #0x28]
04383c50: adrp     x27, #0xe4a4000
04383c54: ldrb     w0, [x27, #0x45a]
04383c58: cbnz     w0, #0x4383d5c
04383c5c: add      x0, x28, x22, asr #1
04383c60: tbz      w22, #0, #0x4383c6c
04383c64: ldr      x8, [x0]
04383c68: ldr      x21, [x8, w21, uxtw]
04383c6c: add      x2, sp, #0x24
04383c70: mov      x1, x20
04383c74: blr      x21
```

### ObjC close bridge

```asm
04445cf0: stp      x22, x21, [sp, #-0x30]!
04445cf4: stp      x20, x19, [sp, #0x10]
04445cf8: stp      x29, x30, [sp, #0x20]
04445cfc: add      x29, sp, #0x20
04445d00: mov      x19, x2
04445d04: mov      x20, x0
04445d08: add      x0, x0, #0x10
04445d0c: bl       #0x84edd34
04445d10: mov      x21, x0
04445d14: add      x0, x20, #8
04445d18: bl       #0x84edd34
04445d1c: mov      x20, x0
04445d20: adrp     x8, #0xe28c000
04445d24: ldr      x0, [x8, #0x118]
04445d28: bl       #0x98eb2c8
04445d2c: adrp     x8, #0xe290000
04445d30: ldr      x1, [x8, #0x698]
04445d34: mov      x2, x19
04445d38: bl       #0x80045ec
04445d3c: mov      x19, x0
04445d40: adrp     x8, #0xe291000
04445d44: ldr      x1, [x8, #0x280]
04445d48: mov      x0, x21
04445d4c: mov      x2, x20
04445d50: mov      x3, x19
04445d54: bl       #0x80045ec
04445d58: mov      x0, x19
04445d5c: bl       #0x8003ddc
04445d60: mov      x0, x20
04445d64: bl       #0x8003ddc
04445d68: mov      x0, x21
04445d6c: ldp      x29, x30, [sp, #0x20]
04445d70: ldp      x20, x19, [sp, #0x10]
04445d74: ldp      x22, x21, [sp], #0x30
04445d78: b        #0x8003ddc
```

### ArcSessionID scalar copy

```asm
0440abb4: sub      sp, sp, #0x30
0440abb8: stp      x20, x19, [sp, #0x10]
0440abbc: stp      x29, x30, [sp, #0x20]
0440abc0: add      x29, sp, #0x20
0440abc4: mov      x19, x2
0440abc8: adrp     x8, #0xe380000
0440abcc: ldr      x8, [x8, #0x8d0]
0440abd0: stp      x0, x8, [sp]
0440abd4: adrp     x8, #0xe28c000
0440abd8: ldr      x1, [x8, #0xb30]
0440abdc: mov      x0, sp
0440abe0: bl       #0x9968234
0440abe4: cbz      x0, #0x440ac04
0440abe8: ldrb     w8, [x0, #0xc]
0440abec: cbz      w8, #0x440abf4
0440abf0: strb     wzr, [x0, #0xc]
0440abf4: ldr      w8, [x19]
0440abf8: str      w8, [x0, #8]
0440abfc: mov      w8, #1
0440ac00: strb     w8, [x0, #0xc]
0440ac04: ldp      x29, x30, [sp, #0x20]
0440ac08: ldp      x20, x19, [sp, #0x10]
0440ac0c: add      sp, sp, #0x30
0440ac10: ret      
```


Additional delivery limit: `438a1d4..1dc` checks global byte `e49cf50` bit0 and skips UIDelegate notification if set. Therefore even sentinel delivery is conditional on that native state as well as live wrapper/delegate weak references; this audit does not promise one callback for every child during whole-app shutdown. Ordinary source NSWindow closure and whole-app shutdown must remain distinct probe phases.

## Independently verified removal record construction

`DetachTabImpl3195520` calls `319633c` at `3195694` when historical capture is requested; false gate instead uses zero/absent optional at `31958c8`. It stores that optional in DetachedTab+1c at `3195808..0c`. Notification producer `31968c0` passes &DetachedTab+1c in x5 at `3196950..54` into RemovedTab construction `33c91e0 ->33c9360`; final constructor stores that optional into RemovedTab+24 at `33c93e8`. Its fast path `3196de0 ->7e1d178` preserves the same optional argument. Thus the absence from the closing-context branch reaches exactly the optional consumed by the native WC observer, without a WINDOW-child lookup between them.

### Historical capture gate and call

```asm
03195670: add      x0, sp, #0x50
03195674: bl       #0x1683934
03195678: mov      x0, x23
0319567c: mov      x1, x21
03195680: bl       #0x3195a78
03195684: str      w20, [sp, #0x18]
03195688: tbz      w24, #0, #0x31958c8
0319568c: ldr      x0, [x23, #0x10]
03195690: ldr      x1, [x26, #0x40]
03195694: bl       #0x319633c
03195698: str      x0, [sp, #8]
0319569c: ldr      x0, [x23]
031956a0: mov      x1, x25
031956a4: bl       #0x3abe30
031956a8: mov      x26, x0
```

### DetachedTab optional store

```asm
031957e0: mov      x1, #0
031957e4: bl       #0x23db2f4
031957e8: mov      w0, #0x28
031957ec: bl       #0x8790
031957f0: str      x25, [x0]
031957f4: stp      w19, w21, [x0, #8]
031957f8: ldp      w9, w10, [sp, #0x14]
031957fc: strb     w9, [x0, #0x10]
03195800: ldr      w8, [sp, #0x1c]
03195804: stp      w10, w8, [x0, #0x14]
03195808: ldr      x8, [sp, #8]
0319580c: stur     x8, [x0, #0x1c]
03195810: ldur     x8, [x29, #-0x60]
03195814: adrp     x9, #0xd824000
03195818: ldr      x9, [x9, #0x20]
0319581c: ldr      x9, [x9]
```

### No-capture optional

```asm
031958b8: ldr      x8, [x23, #0x28]
031958bc: b        #0x31955cc
031958c0: mov      x10, #0
031958c4: b        #0x3195600
031958c8: str      xzr, [sp, #8]
031958cc: b        #0x319569c
031958d0: add      x10, x10, #1
```

### RemovedTab constructor argument

```asm
03196918: mov      w3, #1
0319691c: bl       #0x42316e0
03196920: ldr      x8, [x20, #0x10]
03196924: lsl      x21, x8, #3
03196928: stp      xzr, xzr, [x29, #-0x98]
0319692c: stur     xzr, [x29, #-0x88]
03196930: cbz      x21, #0x31969ac
03196934: mov      x8, #0
03196938: mov      x9, #0
0319693c: ldr      x22, [x20, #8]
03196940: mov      x23, #-0x5555555555555556
03196944: movk     x23, #0xaaab
03196948: mov      w24, #0x30
0319694c: ldr      x10, [x22]
03196950: mov      x5, x10
03196954: ldr      x11, [x5], #0x1c
03196958: str      x11, [sp]
0319695c: ldur     x11, [x29, #-0x88]
03196960: cmp      x8, x11
03196964: b.ne     #0x3196de0
03196968: sub      x0, x29, #0x98
0319696c: mov      x1, sp
03196970: add      x2, x10, #8
03196974: add      x3, x10, #0x14
03196978: add      x4, x10, #0x18
0319697c: bl       #0x33c91e0
```

### RemovedTab final optional store

```asm
033c93b0: mov      x0, x24
033c93b4: bl       #0xffa8
033c93b8: str      x24, [x21, #8]
033c93bc: cbz      x24, #0x33c93c4
033c93c0: ldr      x24, [x24, #0x40]
033c93c4: and      x8, x24, #0xfffffffc00000000
033c93c8: ldr      x9, [x25, #0x208]
033c93cc: cmp      x8, x9
033c93d0: b.ne     #0x33c93dc
033c93d4: mov      x0, x24
033c93d8: bl       #0xffa8
033c93dc: str      x24, [x21, #0x10]
033c93e0: stp      w23, w22, [x21, #0x18]
033c93e4: str      w20, [x21, #0x20]
033c93e8: stur     x19, [x21, #0x24]
033c93ec: mov      x0, x21
033c93f0: ldp      x29, x30, [sp, #0x40]
033c93f4: ldp      x20, x19, [sp, #0x30]
033c93f8: ldp      x22, x21, [sp, #0x20]
033c93fc: ldp      x24, x23, [sp, #0x10]
```

