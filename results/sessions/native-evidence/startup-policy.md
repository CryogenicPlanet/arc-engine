# Original sessionRestorationPolicy / startup preference evidence

Read-only static audit, 2026-10-23. No repository edits, builds, apps, network, or credentials. Original ArcCore binary SHA-256 b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d. Addresses are original arm64 unslid VAs. Current source comparison: `/Users/cryogenicplanet/general/arc-engine/chromium/src/chrome/browser/prefs/session_startup_pref.cc` and `.h`.

## Verified policy and receiver

`273d694` receives the PrefService in x0 and result storage in x8. It reads global configuration pointer at e4ac750, requires engagement byte +1c bit0, then selects output type from value +18: policy0 -> Type2/LAST (`273d6dc`), policy1 -> Type0/DEFAULT (`273d6cc..d4`). Invalid enum branches to trap `273d84c`. Disengaged configuration branches to trap `273d840`, NOT persisted-pref fallback.

The incoming x0 PrefService is preserved through the type override. At `273d6f4..700`, original reads `session.startup_urls` (cstring c7de74a, length20) from that same receiver through `a8100`. It iterates values, skips non-string entries (`273d770..78`), fixes URLs (`273d79c..7ac ->2371a80`), and appends each to returned vector. Therefore neither policy yields an intentionally empty URL list; retained stored URLs remain visible even with type DEFAULT or LAST.

For the proposed integration, an optional bool hook in the PrefService overload selecting LAST/DEFAULT only, followed by unchanged existing GetList/URLListToPref tail, matches this observed native behavior. Nonhost/no-hook persisted type fallback is a replacement integration boundary, not an original disengaged-config behavior. Engaged false must be distinguished from no hook/optional absence. No pref writes or disabling TabRestoreService are supported by this observed function. No actual patch generated/compiled/reviewed here.

## Validation timing and getter snapshot

ObjC `ArcBrowserApplicationConfiguration` initializer `43eca0c` stores policy directly via `43ecab0` without validating it. Native configuration conversion `43ec404` invokes `sessionRestorationPolicy` once at `43ec71c`, accepts only0/1 at `43ec720..28`, traps at `43ec9e4` for invalid input, and stores native enum at config+5c at `43ec738`. It invokes isProduction before policy (`43ec708`) and shouldLoadAllProfilesAtStartup after policy (`43ec748`). Thus do not move enum validation into ObjC configuration-object construction, or reread getter from the startup hook. Lead must retain the established complete configuration getter order; this file proves the bounded production/policy/loadProfiles portion only.

Current local Chromium Profile overload bypasses the PrefService path for guest sessions, returning DEFAULT with empty URLs. Independent original guest overload confirmation is appended below.

## Exact bounded original disassembly

### Startup type override and URL list tail

```asm
0273d694: sub      sp, sp, #0xf0
0273d698: stp      x26, x25, [sp, #0xa0]
0273d69c: stp      x24, x23, [sp, #0xb0]
0273d6a0: stp      x22, x21, [sp, #0xc0]
0273d6a4: stp      x20, x19, [sp, #0xd0]
0273d6a8: stp      x29, x30, [sp, #0xe0]
0273d6ac: add      x29, sp, #0xe0
0273d6b0: mov      x19, x8
0273d6b4: adrp     x8, #0xe4ac000
0273d6b8: ldr      x8, [x8, #0x750]
0273d6bc: ldrb     w9, [x8, #0x1c]
0273d6c0: tbz      w9, #0, #0x273d840
0273d6c4: ldr      w8, [x8, #0x18]
0273d6c8: cbz      w8, #0x273d6dc
0273d6cc: cmp      w8, #1
0273d6d0: b.ne     #0x273d84c
0273d6d4: mov      w8, #0
0273d6d8: b        #0x273d6e0
0273d6dc: mov      w8, #2
0273d6e0: str      xzr, [x19]
0273d6e4: mov      x20, x19
0273d6e8: str      xzr, [x20, #8]!
0273d6ec: str      w8, [x19]
0273d6f0: stp      xzr, xzr, [x19, #0x10]
0273d6f4: adrp     x1, #0xc7de000
0273d6f8: add      x1, x1, #0x74a
0273d6fc: mov      w2, #0x14
0273d700: bl       #0xa8100
0273d704: str      xzr, [x19, #0x10]
0273d708: ldp      x21, x8, [x0]
0273d70c: add      x9, x21, x8, lsl #5
0273d710: cmp      x21, x9
0273d714: b.hi     #0x273d834
0273d718: lsl      x22, x8, #5
0273d71c: cbz      x22, #0x273d734
0273d720: add      x23, sp, #0x20
0273d724: mov      w24, #0x78
0273d728: mov      x25, #-0x1111111111111112
0273d72c: movk     x25, #0xeeef
0273d730: b        #0x273d76c
0273d734: ldp      x29, x30, [sp, #0xe0]
0273d738: ldp      x20, x19, [sp, #0xd0]
0273d73c: ldp      x22, x21, [sp, #0xc0]
0273d740: ldp      x24, x23, [sp, #0xb0]
0273d744: ldp      x26, x25, [sp, #0xa0]
0273d748: add      sp, sp, #0xf0
0273d74c: ret      
0273d750: ldr      x0, [sp, #0x20]
0273d754: ldr      x8, [sp, #0x30]
0273d758: and      x1, x8, #0x7fffffffffffffff
0273d75c: bl       #0x5810
0273d760: add      x21, x21, #0x20
0273d764: subs     x22, x22, #0x20
0273d768: b.eq     #0x273d734
0273d76c: cbz      x21, #0x273d760
0273d770: ldrb     w8, [x21, #0x18]
0273d774: cmp      w8, #4
0273d778: b.ne     #0x273d760
0273d77c: str      xzr, [sp, #0x90]
0273d780: movi     v0.2d, #0000000000000000
0273d784: stp      q0, q0, [sp, #0x70]
0273d788: stp      q0, q0, [sp, #0x50]
0273d78c: stp      q0, q0, [sp, #0x30]
0273d790: str      q0, [sp, #0x20]
0273d794: strb     wzr, [sp, #0x1f]
0273d798: strb     wzr, [sp, #8]
0273d79c: add      x8, sp, #0x20
0273d7a0: add      x1, sp, #8
0273d7a4: mov      x0, x21
0273d7a8: mov      x2, #0
0273d7ac: bl       #0x2371a80
0273d7b0: ldrsb    w8, [sp, #0x1f]
0273d7b4: tbz      w8, #0x1f, #0x273d7c8
0273d7b8: ldr      x0, [sp, #8]
0273d7bc: ldr      x8, [sp, #0x18]
0273d7c0: and      x1, x8, #0x7fffffffffffffff
0273d7c4: bl       #0x5810
0273d7c8: ldp      x8, x9, [x19, #0x10]
0273d7cc: cmp      x8, x9
0273d7d0: b.ne     #0x273d818
0273d7d4: add      x1, sp, #0x20
0273d7d8: mov      x0, x20
0273d7dc: bl       #0x24ab6e8
0273d7e0: ldr      x8, [x19, #8]
0273d7e4: subs     x8, x0, x8
0273d7e8: asr      x8, x8, #3
0273d7ec: mul      x8, x8, x25
0273d7f0: str      x8, [x19, #0x10]
0273d7f4: b.eq     #0x273d83c
0273d7f8: add      x0, x23, #0x70
0273d7fc: mov      x1, #0
0273d800: bl       #0x282bc
0273d804: add      x0, x23, #0x20
0273d808: bl       #0x2827c
0273d80c: ldrsb    w8, [sp, #0x37]
0273d810: tbz      w8, #0x1f, #0x273d760
0273d814: b        #0x273d750
0273d818: ldr      x9, [x19, #8]
0273d81c: madd     x0, x8, x24, x9
0273d820: cbz      x0, #0x273d83c
0273d824: add      x1, sp, #0x20
0273d828: bl       #0x609a0
0273d82c: add      x0, x0, #0x78
0273d830: b        #0x273d7e0
0273d834: brk      #0
0273d838: hlt      #0
0273d83c: brk      #1
0273d840: brk      #0
0273d844: hlt      #0
0273d848: brk      #1
0273d84c: brk      #0
0273d850: hlt      #0
0273d854: brk      #1
```

### PrefService list receiver

```asm
000a8100: sub      sp, sp, #0x50
000a8104: stp      x24, x23, [sp, #0x10]
000a8108: stp      x22, x21, [sp, #0x20]
000a810c: stp      x20, x19, [sp, #0x30]
000a8110: stp      x29, x30, [sp, #0x40]
000a8114: add      x29, sp, #0x40
000a8118: mov      x19, x2
000a811c: mov      x20, x1
000a8120: mov      x21, x0
000a8124: str      xzr, [sp, #8]
000a8128: ldr      x8, [x0, #0x28]
000a812c: ldr      x22, [x8, #0x10]
000a8130: cbz      x22, #0xa8268
000a8134: ldr      w8, [x22, #8]
000a8138: adds     w8, w8, #1
000a813c: str      w8, [x22, #8]
000a8140: b.hs     #0xa81dc
000a8144: ldr      x8, [x22]
000a8148: ldr      x8, [x8, #0x20]
000a814c: add      x3, sp, #8
000a8150: mov      x0, x22
000a8154: mov      x1, x20
000a8158: mov      x2, x19
000a815c: blr      x8
000a8160: ldr      w9, [x22, #8]
000a8164: sub      w8, w9, #1
000a8168: str      w8, [x22, #8]
000a816c: cbz      w9, #0xa81e8
000a8170: cbz      w8, #0xa8244
000a8174: tbz      w0, #0, #0xa825c
000a8178: ldr      x8, [sp, #8]
000a817c: cbz      x8, #0xa81f4
000a8180: ldrb     w22, [x8, #0x18]
000a8184: str      xzr, [sp]
000a8188: ldr      x0, [x21, #0x10]
000a818c: mov      x4, sp
000a8190: mov      x1, x20
000a8194: mov      x2, x19
000a8198: mov      x3, x22
000a819c: bl       #0xa8288
000a81a0: tbz      w0, #0, #0xa8200
000a81a4: ldr      x0, [sp]
000a81a8: cbz      x0, #0xa820c
000a81ac: ldrb     w8, [x0, #0x18]
000a81b0: cmp      w8, w22
000a81b4: b.ne     #0xa8218
000a81b8: ldrb     w8, [x0, #0x18]
000a81bc: cmp      w8, #7
000a81c0: b.ne     #0xa8224
000a81c4: ldp      x29, x30, [sp, #0x40]
000a81c8: ldp      x20, x19, [sp, #0x30]
000a81cc: ldp      x22, x21, [sp, #0x20]
000a81d0: ldp      x24, x23, [sp, #0x10]
000a81d4: add      sp, sp, #0x50
000a81d8: ret      
```

### Configuration conversion getter order and enum checks

```asm
043ec6d0: adrp     x8, #0xe290000
043ec6d4: ldr      x1, [x8, #0x8f0]
043ec6d8: mov      x0, x20
043ec6dc: bl       #0x80045ec
043ec6e0: cbz      x0, #0x43ec6f4
043ec6e4: cmp      x0, #1
043ec6e8: b.ne     #0x43ec9e4
043ec6ec: mov      w8, #1
043ec6f0: b        #0x43ec6f8
043ec6f4: mov      w8, #0
043ec6f8: str      w8, [x19, #0x38]
043ec6fc: adrp     x8, #0xe290000
043ec700: ldr      x1, [x8, #0x8f8]
043ec704: mov      x0, x20
043ec708: bl       #0x80045ec
043ec70c: strb     w0, [x19, #0x58]
043ec710: adrp     x8, #0xe290000
043ec714: ldr      x1, [x8, #0x900]
043ec718: mov      x0, x20
043ec71c: bl       #0x80045ec
043ec720: cbz      x0, #0x43ec734
043ec724: cmp      x0, #1
043ec728: b.ne     #0x43ec9e4
043ec72c: mov      w8, #1
043ec730: b        #0x43ec738
043ec734: mov      w8, #0
043ec738: str      w8, [x19, #0x5c]
043ec73c: adrp     x8, #0xe290000
043ec740: ldr      x1, [x8, #0x908]
043ec744: mov      x0, x20
043ec748: bl       #0x80045ec
043ec74c: strb     w0, [x19, #0x60]
```

### Invalid enum trap

```asm
043ec9dc: bl       #0x5810
043ec9e0: b        #0x43ec994
043ec9e4: brk      #0x550b
043ec9e8: mov      x0, x19
043ec9ec: b        #0x43ec84c
```

### ObjC configuration initializer plain store

```asm
043eca80: mov      x25, x0
043eca84: cbz      x0, #0x43ecac8
043eca88: ldrb     w27, [x29, #0x11]
043eca8c: ldrb     w28, [x29, #0x10]
043eca90: str      w26, [x25, #0xc]
043eca94: add      x0, x25, #0x10
043eca98: mov      x1, x19
043eca9c: bl       #0x8c7df80
043ecaa0: add      x0, x25, #0x18
043ecaa4: mov      x1, x20
043ecaa8: bl       #0x8c7df80
043ecaac: strb     w24, [x25, #8]
043ecab0: stp      x23, x22, [x25, #0x20]
043ecab4: strb     w28, [x25, #9]
043ecab8: strb     w27, [x25, #0xa]
043ecabc: add      x0, x25, #0x30
043ecac0: mov      x1, x21
043ecac4: bl       #0x8c7df80
```

## Original Profile overload independently confirmed

Original `GetStartupPref(Profile*)` equivalent is `81ad7bc`. It classifies the incoming Profile via `5c44e8`, compares result against2 at `81ad7d4`, and guest path `81ad7dc..7f0` returns Type0/DEFAULT plus an empty URL vector without consulting global configuration. Nonguest path `81ad7f4..810` invokes incoming Profile virtual+170 (GetPrefs) and tail-delegates to `273d694` preserving result storage x8. Thus the proposed override placement only in PrefService overload preserves original guest handling. A direct call to PrefService overload has no Profile/guest classification, matching original scope. Independent reviewer verified this bounded path.

```asm
081ad7bc: stp      x20, x19, [sp, #-0x20]!
081ad7c0: stp      x29, x30, [sp, #0x10]
081ad7c4: add      x29, sp, #0x10
081ad7c8: mov      x20, x0
081ad7cc: mov      x19, x8
081ad7d0: bl       #0x5c44e8
081ad7d4: cmp      w0, #2
081ad7d8: b.ne     #0x81ad7f4
081ad7dc: str      wzr, [x19]
081ad7e0: stp      xzr, xzr, [x19, #0x10]
081ad7e4: str      xzr, [x19, #8]
081ad7e8: ldp      x29, x30, [sp, #0x10]
081ad7ec: ldp      x20, x19, [sp], #0x20
081ad7f0: ret      
081ad7f4: ldr      x8, [x20]
081ad7f8: ldr      x8, [x8, #0x170]
081ad7fc: mov      x0, x20
081ad800: blr      x8
081ad804: mov      x8, x19
081ad808: ldp      x29, x30, [sp, #0x10]
081ad80c: ldp      x20, x19, [sp], #0x20
081ad810: b        #0x273d694
081ad814: stp      x29, x30, [sp, #-0x10]!
```
