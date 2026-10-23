# Original ArcCore application restore destination and completion evidence

Read-only static investigation, 2026-10-23. No application launched and no repository files changed.

## Artifact and method

Original ArcCore: `/Users/cryogenicplanet/general/arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`

SHA-256: `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`

Addresses below are original arm64 virtual addresses (unslid). Bounded disassembly was extracted with `/tmp/arccore-sessions-persistence.py`, Capstone, from the original executable. Conclusions describe native control flow for an otherwise idle destination browser and exclude reentrant application delegate actions and unrelated concurrent restoration. No conclusion is based on replacement Chromium behavior.

## Findings

1. Application restore always enters `RestoreEntryById` with a nil native live-tab context and disposition 3 (`NEW_FOREGROUND_TAB`): `42bf0f8`, `42bf100`, `42bf104`. For a TAB, the normal browser-id lookup runs only for disposition 0 (`6c86ee8`); therefore a still-live `Tab.browser_id` alone does not redirect this application call. The optional group lookup is independent of that disposition. At `6c86f04..20`, a found live group replaces the context with B. A nonnil B and disposition != 6 skip browser creation (`6c86f24..2c`). `7d34354` only searches live browsers within the profile; it does not create one.

2. With a TAB redirected into existing B, successful nonempty return data is discarded by `42bf108..15c`, then the application manager continues/returns. The error/nil-finished branch at `42bf160+` is not taken. The pending global application callback remains active. The new-browser path at `42b9db0..42ba1a4` constructs full-restoring BrowserImpl state via `42f89b0`, invokes created phase 0, installs the per-browser finish receiver, then clears that global pending flag. A direct-BL scan of original __text found only `42b9e24` calling `42f89b0`; the finish receiver `42beaf4` is referenced by this new-browser path at `42b9e04`. Thus the redirected TAB path itself emits neither created(B) nor finished(B), and it does not emit finished(nil). It remains pending without creating a browser. A later unrelated browser creation or cancellation/destruction may affect this global pending state; this evidence does not establish eventual behavior after such events.

3. Existing-B insertion does not install full completion later in the same native insertion path. `OnTabStripModelChanged` at `4306358` records a tracked full-restore WC only when browser-local tag is ALREADY 1 (`43066cc..d4`); otherwise it queues the ordinary dispatcher via `43065b0`. Dispatcher `42fcf40` checks the existing browser-local state; idle B takes ordinary didCreate at `42fd048`. This async insertion event does not install a manager completion record or call the full BrowserImpl constructor. The service observer `42bf340` is map/observation cleanup, with no application callback invocation. Existing unrelated full restore on B, or application actions in didCreate, are excluded from this conclusion.

4. Root GROUP through application restore is a native nil-context fault path for a valid nonempty group. `6c84fc4` saves nil context at `sp+0xa8`; root id/original-id match sets the root-match flag. Type 2 routes through `6c85214` to `6c852e4`, then root-match branch `6c853f8` goes to `6c8545c`. `6c85488` reloads nil into x23. For nonzero child count, the branch goes to `6c855cc`, which executes `ldr x8,[x23,#0x28]` before checking the result. No native context creation, live-group lookup, or write to `sp+0xa8` occurs on this root GROUP path. The helper that receives the context slot address (`6c87478`) is on an excluded descendant path. An independent second static reviewer confirmed these exact offsets. This is a static fault-path finding, not a runtime crash observation. GROUP descendant TAB handling is a different path and must not be generalized from the root GROUP fault.

## Bounded original disassembly

### Application manager supplied context and result handling

```asm
042bf0e8: movi     v0.2d, #0000000000000000
042bf0ec: stp      q0, q0, [sp, #0x50]
042bf0f0: add      x8, sp, #0x50
042bf0f4: add      x0, x19, #0x18
042bf0f8: mov      x1, #0
042bf0fc: mov      x2, x25
042bf100: mov      w3, #3
042bf104: bl       #0x6c84f80
042bf108: ldrb     w8, [sp, #0x68]
042bf10c: cbz      w8, #0x42bf160
042bf110: ldr      x8, [sp, #0x58]
042bf114: cbz      x8, #0x42bf160
042bf118: ldrb     w8, [sp, #0x68]
042bf11c: cbnz     w8, #0x42bf150
042bf120: add      x21, x21, #4
042bf124: subs     x28, x28, #4
042bf128: b.eq     #0x42befdc
042bf12c: ldr      w25, [x21]
042bf130: ldapr    x0, [x26]
042bf134: cbnz     x0, #0x42bf2a8
042bf138: ldr      w8, [x27, #0x78]
042bf13c: neg      w8, w8
042bf140: orr      w0, w8, w8, asr #31
042bf144: cmp      w0, #3
042bf148: b.lt     #0x42bf0e8
042bf14c: b        #0x42bf0b4
042bf150: stur     x24, [x29, #-0x60]
042bf154: sub      x0, x29, #0x60
042bf158: bl       #0x267ce40
042bf15c: b        #0x42bf120
042bf160: ldrb     w8, [x20, #0x28]
042bf164: tbz      w8, #0, #0x42bf298
042bf168: mov      w0, #0x39
042bf16c: adrp     x1, #0xd37e000
042bf170: add      x1, x1, #0x40d
042bf174: mov      w2, #0x29
042bf178: mov      w3, #0x7a3
042bf17c: mov      x4, x22
042bf180: bl       #0x4b37104
042bf184: str      x0, [sp, #0x30]
042bf188: cmp      x0, #1
042bf18c: b.eq     #0x42bf29c
042bf190: ldr      x8, [x20, #0x18]
042bf194: add      x0, x20, #8
042bf198: add      x1, sp, #0x30
042bf19c: blr      x8
042bf1a0: ldr      x0, [sp, #0x30]
042bf1a4: cmp      x0, #1
042bf1a8: b.eq     #0x42bf1d8
042bf1ac: tbnz     w0, #0, #0x42bf1b4
042bf1b0: bl       #0x4b35898
042bf1b4: ldrb     w8, [x20, #0x28]
042bf1b8: cbz      w8, #0x42bf118
042bf1bc: ldr      x8, [x20, #0x20]
042bf1c0: ldr      x8, [x8, #8]
042bf1c4: cbz      x8, #0x42bf1d0
042bf1c8: ldur     x0, [x20, #8]
042bf1cc: blr      x8
042bf1d0: strb     wzr, [x20, #0x28]
042bf1d4: b        #0x42bf118
042bf1d8: ldr      x25, [sp, #0x40]
042bf1dc: cbz      x25, #0x42bf1b4
```

### RestoreEntryById setup and root match

```asm
06c84f80: sub      sp, sp, #0x170
06c84f84: stp      x28, x27, [sp, #0x110]
06c84f88: stp      x26, x25, [sp, #0x120]
06c84f8c: stp      x24, x23, [sp, #0x130]
06c84f90: stp      x22, x21, [sp, #0x140]
06c84f94: stp      x20, x19, [sp, #0x150]
06c84f98: stp      x29, x30, [sp, #0x160]
06c84f9c: add      x29, sp, #0x160
06c84fa0: mov      x19, x3
06c84fa4: mov      x25, x2
06c84fa8: mov      x24, x1
06c84fac: mov      x20, x0
06c84fb0: mov      x27, x8
06c84fb4: adrp     x8, #0xd824000
06c84fb8: ldr      x8, [x8, #0x20]
06c84fbc: ldr      x8, [x8]
06c84fc0: stur     x8, [x29, #-0x60]
06c84fc4: str      x1, [sp, #0xa8]
06c84fc8: mov      w28, w25
06c84fcc: mov      w1, w25
06c84fd0: bl       #0x6c86230
06c84fd4: add      x8, x20, #0x20
06c84fd8: cmp      x8, x0
06c84fdc: b.ne     #0x6c85020
06c84fe0: strb     wzr, [x27]
06c84fe4: strb     wzr, [x27, #0x18]
06c84fe8: ldur     x8, [x29, #-0x60]
06c84fec: adrp     x9, #0xd824000
06c84ff0: ldr      x9, [x9, #0x20]
06c84ff4: ldr      x9, [x9]
06c84ff8: cmp      x9, x8
06c84ffc: b.ne     #0x6c86144
06c85000: ldp      x29, x30, [sp, #0x160]
06c85004: ldp      x20, x19, [sp, #0x150]
06c85008: ldp      x22, x21, [sp, #0x140]
06c8500c: ldp      x24, x23, [sp, #0x130]
06c85010: ldp      x26, x25, [sp, #0x120]
06c85014: ldp      x28, x27, [sp, #0x110]
06c85018: add      sp, sp, #0x170
06c8501c: ret      
06c85020: mov      x26, x0
06c85024: str      w19, [sp, #0x24]
06c85028: ldr      x8, [x20, #0x10]
06c8502c: cbz      x8, #0x6c85040
06c85030: sub      x0, x8, #8
06c85034: mov      w1, w25
06c85038: mov      x2, x26
06c8503c: bl       #0x6c88efc
06c85040: mov      w19, #1
06c85044: strb     w19, [x20, #0x38]
06c85048: ldr      x22, [x26, #0x10]
06c8504c: ldr      w8, [x22, #8]
06c85050: cmp      w8, w25
06c85054: b.eq     #0x6c85064
06c85058: ldr      w8, [x22, #0xc]
06c8505c: cmp      w8, w25
06c85060: cset     w19, eq
06c85064: stp      xzr, xzr, [sp, #0x98]
06c85068: str      xzr, [sp, #0x90]
06c8506c: ldr      w8, [x22, #0x10]
06c85070: cmp      w8, #1
06c85074: b.gt     #0x6c85214
06c85078: cbz      w8, #0x6c850dc
06c8507c: cmp      w8, #1
06c85080: b.ne     #0x6c85144
06c85084: ldr      x8, [x22, #0x68]
06c85088: cmp      x8, #2
06c8508c: b.ne     #0x6c86184
06c85090: ldr      x8, [x22, #0x60]
06c85094: ldr      x1, [x8]
```

### GROUP dispatch and root branch

```asm
06c85204: bl       #0x1460670
06c85208: cbz      w0, #0x6c851cc
06c8520c: ldr      x8, [x20, #8]
06c85210: b        #0x6c851d0
06c85214: cmp      w8, #2
06c85218: b.eq     #0x6c852e4
06c8521c: cmp      w8, #3
06c85220: b.ne     #0x6c85144
06c85224: str      w19, [sp, #0x20]
06c85228: ldr      x8, [x22, #0x18]
06c8522c: cbz      x8, #0x6c852a0
06c85230: adrp     x19, #0xe3b1000
06c85234: add      x19, x19, #0x580
06c85238: ldapr    x25, [x19]
06c8523c: cbnz     x25, #0x6c85268
06c85240: adrp     x0, #0xd232000
06c85244: add      x0, x0, #0x5df
06c85248: mov      w1, #0x3e8
06c8524c: mov      w2, #0xa400
06c85250: movk     w2, #0xd693, lsl #16
06c85254: mov      w3, #0x32
06c85258: mov      w4, #1
06c8525c: bl       #0x1799ed0
06c85260: mov      x25, x0
06c85264: stlr     x0, [x19]
06c85268: ldr      x0, [x20, #0xc0]
06c8526c: cbz      x0, #0x6c85280
06c85270: ldr      x8, [x0]
06c85274: ldr      x8, [x8, #0x10]
06c85278: blr      x8
06c8527c: b        #0x6c85284
06c85280: bl       #0x20fa230
06c85284: ldr      x8, [x22, #0x18]
06c85288: subs     x8, x0, x8
06c8528c: asr      x9, x8, #0x3f
06c85290: eor      x9, x9, #0x8000000000000000
06c85294: csel     x1, x9, x8, vs
06c85298: mov      x0, x25
06c8529c: bl       #0x8f6640
06c852a0: stp      x27, x28, [sp, #0x10]
06c852a4: sub      x8, x29, #0x90
06c852a8: add      x0, x22, #0x58
06c852ac: bl       #0x6c873d0
06c852b0: add      x0, x22, #0x40
06c852b4: sub      x1, x29, #0x90
06c852b8: bl       #0x6c87308
06c852bc: ldur     x0, [x29, #-0x88]
06c852c0: bl       #0x4baedb0
06c852c4: add      x8, sp, #0x78
06c852c8: add      x8, x8, #8
06c852cc: stp      xzr, xzr, [sp, #0x80]
06c852d0: str      x8, [sp, #0x28]
06c852d4: str      x8, [sp, #0x78]
06c852d8: ldr      x19, [x22, #0x58]
06c852dc: add      x28, x22, #0x60
06c852e0: b        #0x6c85850
06c852e4: ldr      x8, [x22, #0x18]
06c852e8: cbz      x8, #0x6c85374
06c852ec: mov      x21, x19
06c852f0: adrp     x19, #0xe3b1000
06c852f4: add      x19, x19, #0x588
06c852f8: ldapr    x24, [x19]
06c852fc: cbnz     x24, #0x6c85328
06c85300: adrp     x0, #0xd232000
06c85304: add      x0, x0, #0x60e
06c85308: mov      w1, #0x3e8
06c8530c: mov      w2, #0xa400
06c85310: movk     w2, #0xd693, lsl #16
06c85314: mov      w3, #0x32
06c85318: mov      w4, #1
06c8531c: bl       #0x1799ed0
06c85320: mov      x24, x0
06c85324: stlr     x0, [x19]
06c85328: ldr      x0, [x20, #0xc0]
06c8532c: cbz      x0, #0x6c85350
06c85330: ldr      x8, [x0]
06c85334: ldr      x8, [x8, #0x10]
06c85338: blr      x8
06c8533c: b        #0x6c85354
06c85340: ldp      x1, x2, [x22, #0x80]
06c85344: add      x0, x8, #0x10
06c85348: bl       #0x60a60
06c8534c: b        #0x6c853a8
06c85350: bl       #0x20fa230
06c85354: mov      x19, x21
06c85358: ldr      x8, [x22, #0x18]
06c8535c: subs     x8, x0, x8
06c85360: asr      x9, x8, #0x3f
06c85364: eor      x9, x9, #0x8000000000000000
06c85368: csel     x1, x9, x8, vs
06c8536c: mov      x0, x24
06c85370: bl       #0x8f6640
06c85374: ldrb     w8, [x22, #0x98]
06c85378: cbz      w8, #0x6c853f8
06c8537c: ldrb     w8, [x22, #0x98]
06c85380: tbz      w8, #0, #0x6c8613c
06c85384: ldr      q0, [x22, #0x50]
06c85388: stur     q0, [x29, #-0x90]
06c8538c: sub      x8, x29, #0x90
06c85390: ldrsb    w9, [x22, #0x97]
06c85394: tbnz     w9, #0x1f, #0x6c85340
06c85398: ldur     q0, [x22, #0x80]
06c8539c: ldur     x9, [x22, #0x90]
06c853a0: stur     x9, [x8, #0x20]
06c853a4: stur     q0, [x8, #0x10]
06c853a8: add      x8, sp, #0xb0
06c853ac: stp      xzr, xzr, [sp, #0xb8]
06c853b0: add      x8, x8, #8
06c853b4: str      x8, [sp, #0xb0]
06c853b8: sub      x8, x29, #0x90
06c853bc: add      x0, sp, #0xb0
06c853c0: sub      x1, x29, #0x90
06c853c4: add      x2, x8, #0x28
06c853c8: bl       #0x6c88870
06c853cc: add      x0, x22, #0x38
06c853d0: add      x1, sp, #0xb0
06c853d4: bl       #0x6c87308
06c853d8: ldr      x0, [sp, #0xb8]
06c853dc: bl       #0x4baedb0
06c853e0: ldursb   w8, [x29, #-0x69]
06c853e4: tbz      w8, #0x1f, #0x6c853f8
06c853e8: ldur     x0, [x29, #-0x80]
06c853ec: ldur     x8, [x29, #-0x70]
06c853f0: and      x1, x8, #0x7fffffffffffffff
06c853f4: bl       #0x5810
06c853f8: cbnz     w19, #0x6c8545c
06c853fc: ldr      x8, [x22, #0x40]
06c85400: lsl      x8, x8, #3
06c85404: cbz      x8, #0x6c85428
06c85408: ldr      x24, [x22, #0x38]
06c8540c: ldr      x1, [x24]
06c85410: ldr      w9, [x1, #8]
06c85414: cmp      w9, w25
06c85418: b.eq     #0x6c86048
06c8541c: add      x24, x24, #8
06c85420: subs     x8, x8, #8
06c85424: b.ne     #0x6c8540c
06c85428: add      x3, sp, #0xa8
06c8542c: add      x5, sp, #0x90
06c85430: mov      x0, x20
06c85434: mov      x1, x22
06c85438: mov      x2, x28
06c8543c: ldr      w4, [sp, #0x24]
06c85440: bl       #0x6c87478
06c85444: ldr      x8, [x22, #0x40]
06c85448: cbnz     x8, #0x6c85508
06c8544c: ldr      w1, [x22, #8]
06c85450: mov      x0, x20
06c85454: bl       #0x6c861e4
06c85458: b        #0x6c85508
06c8545c: str      w19, [sp, #0x20]
06c85460: sub      x8, x29, #0x90
06c85464: add      x8, x8, #8
06c85468: stp      xzr, xzr, [x29, #-0x88]
06c8546c: stur     x8, [x29, #-0x90]
06c85470: add      x8, sp, #0xb0
06c85474: add      x8, x8, #8
06c85478: stp      xzr, xzr, [sp, #0xb8]
06c8547c: str      x8, [sp, #0xb0]
06c85480: ldr      x8, [x22, #0x40]
06c85484: lsl      x19, x8, #3
06c85488: ldr      x23, [sp, #0xa8]
06c8548c: cbz      x19, #0x6c8549c
06c85490: ldr      x21, [x22, #0x38]
06c85494: adrp     x26, #0xe2f2000
06c85498: b        #0x6c855cc
06c8549c: ldrb     w8, [x22, #0x98]
```

### GROUP root nil dereference

```asm
06c855b0: str      x24, [x0, #0x28]
06c855b4: ldr      x8, [x21]
06c855b8: ldrb     w9, [x8, #0xd0]
06c855bc: cbnz     w9, #0x6c85518
06c855c0: add      x21, x21, #8
06c855c4: subs     x19, x19, #8
06c855c8: b.eq     #0x6c854dc
06c855cc: ldr      x8, [x23, #0x28]
06c855d0: cbz      x8, #0x6c86160
06c855d4: ldr      x9, [x22, #0x40]
06c855d8: cbz      x9, #0x6c85fd8
06c855dc: ldr      x1, [x21]
06c855e0: ldr      x8, [x8]
06c855e4: ldr      w2, [x8, #0x18]
06c855e8: ldr      x8, [x22, #0x38]
06c855ec: ldr      x8, [x8]
06c855f0: ldr      w8, [x8, #8]
06c855f4: ldr      w9, [x1, #8]
06c855f8: cmp      w8, w9
06c855fc: cset     w3, eq
06c85600: ldr      w5, [x22, #0x10]
06c85604: mov      x0, x23
06c85608: mov      w4, #1
06c8560c: bl       #0x7d33900
06c85610: cbnz     x0, #0x6c85578
06c85614: b        #0x6c855b4
```

### TAB destination selection and native creation

```asm
06c86ea4: sub      sp, sp, #0xd0
06c86ea8: stp      x26, x25, [sp, #0x80]
06c86eac: stp      x24, x23, [sp, #0x90]
06c86eb0: stp      x22, x21, [sp, #0xa0]
06c86eb4: stp      x20, x19, [sp, #0xb0]
06c86eb8: stp      x29, x30, [sp, #0xc0]
06c86ebc: add      x29, sp, #0xc0
06c86ec0: mov      x19, x5
06c86ec4: mov      x24, x2
06c86ec8: mov      x20, x1
06c86ecc: mov      x21, x0
06c86ed0: cmp      w3, #1
06c86ed4: ccmp     x2, #0, #4, eq
06c86ed8: b.ne     #0x6c87094
06c86edc: mov      x22, x6
06c86ee0: mov      x23, x4
06c86ee4: mov      x25, x3
06c86ee8: cbnz     w3, #0x6c86f04
06c86eec: ldr      w8, [x20, #0x54]
06c86ef0: cbz      w8, #0x6c86f04
06c86ef4: cmp      w8, #0
06c86ef8: csinv    w0, w8, wzr, gt
06c86efc: bl       #0x7d342e0
06c86f00: mov      x24, x0
06c86f04: ldrb     w8, [x20, #0x100]
06c86f08: cbz      w8, #0x6c86f24
06c86f0c: ldr      x8, [x21, #0x18]
06c86f10: ldp      x0, x1, [x20, #0xf0]
06c86f14: ldr      x2, [x8, #8]
06c86f18: bl       #0x7d34354
06c86f1c: cmp      x0, #0
06c86f20: csel     x24, x24, x0, eq
06c86f24: cmp      w25, #6
06c86f28: b.eq     #0x6c86f58
06c86f2c: cbz      x24, #0x6c86f58
06c86f30: ldr      w2, [x20, #0x58]
06c86f34: tbnz     w2, #0x1f, #0x6c87010
06c86f38: ldr      x8, [x24, #0x28]
06c86f3c: cbz      x8, #0x6c870c4
06c86f40: cbnz     w25, #0x6c87010
06c86f44: ldr      x8, [x8]
06c86f48: ldr      w8, [x8, #0x18]
06c86f4c: cmp      w2, w8
06c86f50: b.gt     #0x6c87010
06c86f54: b        #0x6c87020
06c86f58: ldr      x8, [x21, #0x18]
06c86f5c: stp      xzr, xzr, [sp, #0x60]
06c86f60: str      xzr, [sp, #0x70]
06c86f64: stp      xzr, xzr, [sp, #0x50]
06c86f68: stp      xzr, xzr, [sp, #0x38]
06c86f6c: str      xzr, [sp, #0x48]
06c86f70: stp      xzr, xzr, [sp, #0x20]
06c86f74: str      xzr, [sp, #0x30]
06c86f78: add      x26, sp, #8
06c86f7c: add      x9, x26, #8
06c86f80: stp      xzr, xzr, [sp, #0x10]
06c86f84: str      x9, [sp, #8]
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
```

### TAB insertion and browser-id remap

```asm
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
06c870c0: brk      #1
06c870c4: brk      #0
06c870c8: hlt      #0
06c870cc: brk      #1
```

### Existing group context lookup

```asm
07d34354: sub      sp, sp, #0x50
07d34358: stp      x20, x19, [sp, #0x30]
07d3435c: stp      x29, x30, [sp, #0x40]
07d34360: add      x29, sp, #0x40
07d34364: stp      x0, x1, [sp, #0x20]
07d34368: cbz      x2, #0x7d343ec
07d3436c: str      xzr, [sp, #0x18]
07d34370: mov      x0, x2
07d34374: bl       #0x2376d34
07d34378: add      x8, sp, #0x20
07d3437c: add      x9, sp, #0x18
07d34380: stp      x8, x9, [sp, #8]
07d34384: adr      x2, #0x7d34410
07d34388: nop      
07d3438c: add      x1, sp, #8
07d34390: mov      w3, #0
07d34394: mov      w4, #0
07d34398: bl       #0x18f0070
07d3439c: ldr      x19, [sp, #0x18]
07d343a0: cbz      x19, #0x7d343d8
07d343a4: ldr      x8, [x19]
07d343a8: ldr      x8, [x8, #0x50]
07d343ac: mov      x0, x19
07d343b0: blr      x8
07d343b4: tbnz     w0, #0, #0x7d343d8
07d343b8: ldr      x8, [x19]
07d343bc: ldr      x8, [x8, #0x18]
07d343c0: mov      x0, x19
07d343c4: blr      x8
07d343c8: adrp     x1, #0xdf54000
07d343cc: add      x1, x1, #0x488
07d343d0: bl       #0x4969fc
07d343d4: b        #0x7d343dc
07d343d8: mov      x0, #0
07d343dc: ldp      x29, x30, [sp, #0x40]
07d343e0: ldp      x20, x19, [sp, #0x30]
07d343e4: add      sp, sp, #0x50
07d343e8: ret      
07d343ec: brk      #0
07d343f0: hlt      #0
07d343f4: brk      #1
07d343f8: stp      x29, x30, [sp, #-0x10]!
07d343fc: mov      x29, sp
07d34400: bl       #0x421d880
07d34404: mov      w1, #0x18
07d34408: ldp      x29, x30, [sp], #0x10
07d3440c: b        #0x5810
07d34410: stp      x20, x19, [sp, #-0x20]!
07d34414: stp      x29, x30, [sp, #0x10]
07d34418: add      x29, sp, #0x10
07d3441c: mov      x20, x1
07d34420: mov      x19, x0
07d34424: ldr      x8, [x1]
07d34428: ldr      x8, [x8, #0x78]
07d3442c: mov      x0, x1
07d34430: blr      x8
07d34434: ldr      x0, [x0, #8]
07d34438: cbz      x0, #0x7d34450
07d3443c: ldr      x1, [x19]
07d34440: bl       #0x7e0f4d8
07d34444: cbz      w0, #0x7d34450
```

### New-browser full state and created phase

```asm
042b9db0: ldrb     w8, [x20, #0x370]
042b9db4: cbz      w8, #0x42b9cbc
042b9db8: ldr      x19, [x20, #0x3e0]
042b9dbc: cbz      x19, #0x42b9dd4
042b9dc0: mov      w8, #1
042b9dc4: ldadd    w8, w8, [x19]
042b9dc8: mov      w9, #0x7fffffff
042b9dcc: cmp      w8, w9
042b9dd0: b.eq     #0x42ba228
042b9dd4: ldr      x22, [x20, #0x3e8]
042b9dd8: mov      w0, #0x40
042b9ddc: bl       #0x8790
042b9de0: mov      x2, x0
042b9de4: mov      w8, #1
042b9de8: adr      x9, #0x42c06a4
042b9dec: nop      
042b9df0: adrp     x10, #0x40a6000
042b9df4: add      x10, x10, #0x300
042b9df8: str      w8, [x0]
042b9dfc: adrp     x8, #0x4169000
042b9e00: add      x8, x8, #0xbc0
042b9e04: adr      x11, #0x42beaf4
042b9e08: nop      
042b9e0c: stp      x9, x10, [x0, #8]
042b9e10: stp      x8, x11, [x0, #0x18]
042b9e14: stp      xzr, x19, [x0, #0x28]
042b9e18: str      x22, [x0, #0x38]
042b9e1c: add      x0, sp, #0x68
042b9e20: mov      x1, x21
042b9e24: bl       #0x42f89b0
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
```

### New-browser manager callback consumption

```asm
042ba164: ldr      x8, [x20, #0x378]
042ba168: ldr      x9, [x8, x28, lsl #3]
042ba16c: cbz      x9, #0x42ba1b0
042ba170: ldr      x8, [x9]
042ba174: str      x8, [x23]
042ba178: str      x23, [x9]
042ba17c: ldr      x8, [x20, #0x390]
042ba180: add      x8, x8, #1
042ba184: str      x8, [x20, #0x390]
042ba188: ldrb     w8, [x20, #0x370]
042ba18c: cbz      w8, #0x42ba1a8
042ba190: ldr      x8, [x20, #0x368]
042ba194: ldr      x8, [x8, #8]
042ba198: cbz      x8, #0x42ba1a4
042ba19c: ldr      x0, [x25]
042ba1a0: blr      x8
042ba1a4: strb     wzr, [x20, #0x370]
042ba1a8: ldr      x23, [sp, #0x70]
042ba1ac: b        #0x42b9cc4
```

### Finish receiver

```asm
042beaf4: sub      sp, sp, #0x90
042beaf8: stp      x24, x23, [sp, #0x50]
042beafc: stp      x22, x21, [sp, #0x60]
042beb00: stp      x20, x19, [sp, #0x70]
042beb04: stp      x29, x30, [sp, #0x80]
042beb08: add      x29, sp, #0x80
042beb0c: mov      x21, x1
042beb10: mov      x19, x0
042beb14: adrp     x8, #0xd824000
042beb18: ldr      x8, [x8, #0x20]
042beb1c: ldr      x8, [x8]
042beb20: stur     x8, [x29, #-0x38]
042beb24: ldr      x0, [x1]
042beb28: bl       #0x42fb310
042beb2c: cmp      w0, #0
042beb30: csinv    w8, w0, wzr, gt
042beb34: ldr      x9, [x19, #0x38]
042beb38: cbz      x9, #0x42bee64
042beb3c: ldr      x10, [x19, #0x48]
042beb40: cbz      x10, #0x42bee64
042beb44: sxtw     x10, w8
042beb48: sub      x11, x9, #1
042beb4c: tst      x9, x11
042beb50: b.eq     #0x42beb6c
042beb54: mov      x12, x10
042beb58: cmp      x9, x10
042beb5c: b.hi     #0x42beb70
042beb60: udiv     x12, x10, x9
042beb64: msub     x12, x12, x9, x10
042beb68: b        #0x42beb70
042beb6c: and      x12, x11, x10
042beb70: ldr      x13, [x19, #0x30]
042beb74: ldr      x13, [x13, x12, lsl #3]
042beb78: cbz      x13, #0x42bee64
042beb7c: ldr      x20, [x13]
042beb80: cbz      x20, #0x42bee64
042beb84: b        #0x42bebb8
042beb88: tst      x9, x11
042beb8c: b.eq     #0x42beba4
042beb90: cmp      x13, x9
042beb94: b.lo     #0x42beba8
042beb98: udiv     x14, x13, x9
042beb9c: msub     x13, x14, x9, x13
042beba0: b        #0x42beba8
042beba4: and      x13, x13, x11
042beba8: cmp      x13, x12
042bebac: b.ne     #0x42bee64
042bebb0: ldr      x20, [x20]
042bebb4: cbz      x20, #0x42bee64
042bebb8: ldr      x13, [x20, #8]
042bebbc: cmp      x13, x10
042bebc0: b.ne     #0x42beb88
042bebc4: ldr      w13, [x20, #0x10]
042bebc8: cmp      w13, w8
042bebcc: b.ne     #0x42bebb0
042bebd0: str      xzr, [sp, #0x20]
042bebd4: mov      x22, x20
042bebd8: ldr      w8, [x22, #0x10]!
042bebdc: str      w8, [sp, #0x20]
042bebe0: ldur     q0, [x22, #8]
042bebe4: stur     q0, [sp, #0x28]
042bebe8: ldr      x23, [x22, #0x18]
042bebec: str      x23, [sp, #0x38]
042bebf0: ldr      x8, [x22, #0x20]
042bebf4: str      x8, [sp, #0x40]
042bebf8: ldr      x8, [x8]
042bebfc: cbz      x8, #0x42bec0c
042bec00: ldur     x0, [x22, #8]
042bec04: blr      x8
042bec08: str      x0, [sp, #0x28]
042bec0c: add      x8, sp, #0x20
042bec10: ldr      x9, [x21, #8]
042bec14: ldr      q0, [x21]
042bec18: cbz      x9, #0x42bec28
042bec1c: add      x9, x9, #8
042bec20: mov      w10, #1
042bec24: ldadd    x10, x9, [x9]
042bec28: stur     q0, [sp, #8]
042bec2c: mov      w9, #1
042bec30: str      w9, [sp, #0x18]
042bec34: str      x9, [sp]
042bec38: add      x0, x8, #8
042bec3c: mov      x1, sp
042bec40: blr      x23
042bec44: ldr      x0, [sp]
042bec48: cmp      x0, #1
042bec4c: b.ne     #0x42bec88
042bec50: ldr      x21, [sp, #0x10]
```

### Insertion queue dispatch

```asm
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

### Insertion only reads full state

```asm
043066a0: mov      w22, #1
043066a4: stp      xzr, xzr, [sp, #0x30]
043066a8: ldr      x0, [x27, #8]
043066ac: str      xzr, [sp, #0x10]
043066b0: strh     wzr, [sp, #0x18]
043066b4: strb     w22, [sp, #0x1a]
043066b8: stur     xzr, [x28, #0x11]
043066bc: stur     xzr, [x28, #0xb]
043066c0: add      x8, sp, #0x30
043066c4: add      x1, sp, #0x10
043066c8: bl       #0x4346100
043066cc: ldrb     w8, [x20, #0x258]
043066d0: cmp      w8, #1
043066d4: b.ne     #0x43064b8
043066d8: ldr      x24, [x27, #8]
043066dc: ldr      x28, [x20, #0x240]
```

### Async insertion callback state routing

```asm
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
042fcf88: ldr      x8, [x19, #0x228]
042fcf8c: ldr      x9, [x19, #0x230]
042fcf90: lsl      x10, x9, #2
042fcf94: cbz      x10, #0x42fd2b8
042fcf98: asr      x11, x10, #2
042fcf9c: mov      x10, x8
042fcfa0: lsr      x12, x11, #1
042fcfa4: add      x13, x10, x12, lsl #2
042fcfa8: ldr      w14, [x13], #4
042fcfac: mvn      x15, x11, lsr #1
042fcfb0: add      x11, x11, x15
042fcfb4: cmp      w14, w20
042fcfb8: csel     x11, x11, x12, lt
042fcfbc: csel     x10, x13, x10, lt
042fcfc0: cbnz     x11, #0x42fcfa0
042fcfc4: add      x8, x8, x9, lsl #2
042fcfc8: cmp      x10, x8
042fcfcc: b.eq     #0x42fcfdc
042fcfd0: ldr      w8, [x10]
042fcfd4: cmp      w8, w20
042fcfd8: b.le     #0x42fd4e4
042fcfdc: tbz      x4, #0x20, #0x42fd048
042fcfe0: ldrb     w8, [x19, #0x258]
042fcfe4: cmp      w8, #1
042fcfe8: b.eq     #0x42fd4f4
042fcfec: cmp      w8, #2
042fcff0: b.ne     #0x42fd048
042fcff4: ldr      x8, [x19, #0x228]
042fcff8: ldr      x9, [x19, #0x230]
042fcffc: lsl      x10, x9, #2
042fd000: cbz      x10, #0x42fd2c0
042fd004: asr      x11, x10, #2
042fd008: mov      x10, x8
042fd00c: lsr      x12, x11, #1
042fd010: add      x13, x10, x12, lsl #2
042fd014: ldr      w14, [x13], #4
042fd018: mvn      x15, x11, lsr #1
042fd01c: add      x11, x11, x15
042fd020: cmp      w14, w4
042fd024: csel     x11, x11, x12, lt
042fd028: csel     x10, x13, x10, lt
042fd02c: cbnz     x11, #0x42fd00c
042fd030: add      x8, x8, x9, lsl #2
042fd034: cmp      x10, x8
042fd038: b.eq     #0x42fd048
042fd03c: ldr      w8, [x10]
042fd040: cmp      w8, w4
042fd044: b.le     #0x42fd4d8
042fd048: ldr      x20, [x19, #0x10]
042fd04c: cbz      x20, #0x42fd4f0
042fd050: ldur     q0, [x19, #8]
042fd054: add      x8, x20, #8
042fd058: ldar     x8, [x8]
042fd05c: cmn      x8, #1
042fd060: b.eq     #0x42fd4f0
042fd064: add      x10, x8, #1
042fd068: add      x9, x20, #8
042fd06c: mov      x11, x8
042fd070: casal    x11, x10, [x9]
042fd074: cmp      x11, x8
042fd078: mov      x8, x11
042fd07c: b.ne     #0x42fd05c
042fd080: str      q0, [sp, #0x10]
042fd084: mov      w8, #1
042fd088: ldadd    x8, x8, [x9]
042fd08c: ldr      x8, [x21, #8]
042fd090: ldr      q0, [x21]
042fd094: str      q0, [sp]
042fd098: cbz      x8, #0x42fd0a8
042fd09c: add      x8, x8, #8
```

### Manager observer cleanup

```asm
042bf340: sub      sp, sp, #0x50
042bf344: stp      x22, x21, [sp, #0x20]
042bf348: stp      x20, x19, [sp, #0x30]
042bf34c: stp      x29, x30, [sp, #0x40]
042bf350: add      x29, sp, #0x40
042bf354: mov      x19, x1
042bf358: mov      x20, x0
042bf35c: ldr      x21, [x0, #0x60]
042bf360: cbz      x21, #0x42bf3b8
042bf364: ldr      x8, [x20, #0x70]
042bf368: cbz      x8, #0x42bf3b8
042bf36c: str      x19, [sp, #8]
042bf370: add      x0, sp, #8
042bf374: mov      w1, #8
042bf378: bl       #0x18f3c
042bf37c: sub      x8, x21, #1
042bf380: tst      x21, x8
042bf384: b.eq     #0x42bf3a0
042bf388: mov      x9, x0
042bf38c: cmp      x0, x21
042bf390: b.lo     #0x42bf3a4
042bf394: udiv     x9, x0, x21
042bf398: msub     x9, x9, x21, x0
042bf39c: b        #0x42bf3a4
042bf3a0: and      x9, x0, x8
042bf3a4: ldur     x10, [x20, #0x58]
042bf3a8: ldr      x10, [x10, x9, lsl #3]
042bf3ac: cbz      x10, #0x42bf3b8
042bf3b0: ldr      x1, [x10]
042bf3b4: cbnz     x1, #0x42bf400
042bf3b8: str      x19, [sp, #8]
042bf3bc: add      x0, x20, #0x88
042bf3c0: add      x1, sp, #8
042bf3c4: bl       #0x42c2d04
042bf3c8: cbz      x0, #0x42bf478
042bf3cc: ldr      x1, [x20, #0x80]
042bf3d0: add      x0, x19, #0x58
042bf3d4: bl       #0x159697c
042bf3d8: ldp      x29, x30, [sp, #0x40]
042bf3dc: ldp      x20, x19, [sp, #0x30]
042bf3e0: ldp      x22, x21, [sp, #0x20]
042bf3e4: add      sp, sp, #0x50
042bf3e8: ret      
042bf3ec: ldr      x10, [x1, #0x10]
042bf3f0: cmp      x10, x19
042bf3f4: b.eq     #0x42bf438
042bf3f8: ldr      x1, [x1]
042bf3fc: cbz      x1, #0x42bf3b8
042bf400: ldr      x10, [x1, #8]
042bf404: cmp      x10, x0
042bf408: b.eq     #0x42bf3ec
042bf40c: tst      x21, x8
042bf410: b.eq     #0x42bf428
042bf414: cmp      x10, x21
042bf418: b.lo     #0x42bf42c
042bf41c: udiv     x11, x10, x21
042bf420: msub     x10, x11, x21, x10
042bf424: b        #0x42bf42c
042bf428: and      x10, x10, x8
042bf42c: cmp      x10, x9
042bf430: b.ne     #0x42bf3b8
042bf434: b        #0x42bf3f8
042bf438: add      x8, sp, #8
042bf43c: add      x0, x20, #0x58
042bf440: bl       #0x4080740
042bf444: ldr      x8, [sp, #8]
042bf448: cbz      x8, #0x42bf3b8
042bf44c: ldrb     w9, [sp, #0x18]
042bf450: cbz      w9, #0x42bf464
042bf454: cmn      x8, #0x10
042bf458: b.eq     #0x42bf474
042bf45c: ldr      x0, [x8, #0x18]
042bf460: cbnz     x0, #0x42bf484
042bf464: mov      x0, x8
042bf468: mov      w1, #0x30
042bf46c: bl       #0x5810
042bf470: b        #0x42bf3b8
042bf474: brk      #1
042bf478: brk      #0
042bf47c: hlt      #0
042bf480: brk      #1
042bf484: mov      x9, x8
042bf488: ldr      x8, [x8, #0x20]
042bf48c: lsl      x10, x8, #2
042bf490: cbz      x10, #0x42bf4b0
042bf494: neg      x8, x8, lsl #2
042bf498: add      x10, x10, x0
042bf49c: sub      x10, x10, #4
042bf4a0: cbz      x10, #0x42bf474
042bf4a4: sub      x10, x10, #4
042bf4a8: add      x8, x8, #4
042bf4ac: cbnz     x8, #0x42bf4a0
042bf4b0: mov      x21, x9
042bf4b4: str      xzr, [x9, #0x20]
042bf4b8: ldr      x8, [x9, #0x28]
042bf4bc: lsl      x1, x8, #2
042bf4c0: bl       #0x5810
042bf4c4: mov      x8, x21
042bf4c8: b        #0x42bf464
042bf4cc: sub      sp, sp, #0x40
042bf4d0: stp      x29, x30, [sp, #0x30]
042bf4d4: add      x29, sp, #0x30
042bf4d8: adrp     x8, #0xd824000
042bf4dc: ldr      x8, [x8, #0x20]
042bf4e0: ldr      x8, [x8]
042bf4e4: stur     x8, [x29, #-8]
042bf4e8: ldr      x8, [x0, #0x30]
042bf4ec: cbz      x8, #0x42bf4f8
```

## Direct call cross-reference audit

Only direct arm64 BL instructions in original `__text` were scanned; indirect calls are not resolved by this scan.

Sections: [('0x4000', '0xc4a1994', '__text', '__TEXT')]

Direct BL callers of `42f89b0`: 0x42b9e24
