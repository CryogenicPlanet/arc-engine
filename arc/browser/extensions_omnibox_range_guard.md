# Native omnibox disposition range guard

**APPLIED after validation10 completed.** Parent application matches proposed source SHA256 `ebc84f16a82b191e33f83da7b188cee175fe11ab73b71aedd595a96e31f6b35a`; official check-file passed, with output in `logs/wp/extensions.browser.omnibox-range.check-file.log`. Fresh build/runtime verification is pending. The historical patch was prepared against `84d532be49b5123068ed33ff340d741ef346a02234f3f58c43205b930f992b1e`. It adds `base/check_op.h` after `base/auto_reset.h` and an unsigned-64 CHECK before asking for the live browser:

```cpp
CHECK_LT(static_cast<unsigned long long>(disposition), 12ULL);
```

The optional omnibox helper patch artifact remains byte-identical. During the historical proposal checks, actual product/helper sources and runtime/app state were untouched. Parent application is recorded above. No invalid disposition was executed. The evidence below is a static binary read plus temporary VFS syntax checks, not a runtime crash/parity result.

## Direct pinned native proof

Source binary: `../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`, full SHA256 **`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`**, checked before extraction. Its arm64 slice starts at full-image offset `0xfa48000`, size `0xe4362f0`. Executable bytes in these __TEXT ranges have unslid VA equal to slice-relative file offset.

The requested `[0x43e280c,0x43e291c)` range is 272 bytes, fully decoded, SHA256 `49990d02d6341613948c7b692bcb38fcf7ca27089153e2d58264dc58d3716ef2`. The metadata-sized method ends at `0x43e2918` (268 bytes), SHA256 `4aa5a54abf0a7643cd7c529a093f1f9a5f926b36ca62118c91e34f833b1445e1`; the extra requested word at `0x43e2918` is `mov x19,x0` outside that method. Raw bytes, complete address/byte/instruction listings, file offsets and hashes are retained under `logs/wp/extensions-omnibox-range-guard-proposal/`.

Decisive instructions, read directly from the pinned arm64 executable bytes:

```text
0x43e2820: f50304aa  mov x21, x4             // retain full 64-bit disposition
0x43e2840: c8e24039  ldrb w8, [x22, #0x38]
0x43e2844: 68060036  tbz w8, #0, #0x43e2910 // wrapper engagement check first
... original ID/text conversions ...
0x43e28a0: bf3200f1  cmp x21, #0xc
0x43e28a4: 82030054  b.hs #0x43e2914        // UNSIGNED higher-or-same
0x43e28a8: c01640f9  ldr x0, [x22, #0x28]
0x43e28b4: e30315aa  mov x3, x21
0x43e28b8: ad67fc97  bl #0x42fc76c
...
0x43e2910: 200020d4  brk #1
0x43e2914: 60a12ad4  brk #0x550b
```

The declared public parameter is signed `q`, but **the actual range predicate is unsigned 64-bit `<12`**. Negative signed values have unsigned representations at least 2^63 and take `B.HS`, as do 12 and every larger positive value. It is not signed `B.GE`, a low-32-bit comparison, or a default/current-tab result for out-of-range public inputs. Valid values are exactly 0 through 11.

Ordering is concrete: the wrapper engagement bit check precedes conversion and range validation. Assuming normal argument conversion completes, the enum trap precedes loading/calling the native bridge. Thus an engaged wrapper whose native browser is dead still traps on invalid enum values; dead-browser state cannot suppress the range check. The current category previously looked up the weak browser before any enum guard, and its mapping default silently converted arbitrary values to current-tab disposition. The proposed entry CHECK restores the enum predicate before that live/dead branch.

This patch does not add or reinterpret the core wrapper's engagement model. It does not reproduce incidental Objective-C retain/string-conversion ordering before the original trap, nor claim CHECK uses the same BRK immediate. It preserves the supported scalar contract and enum-failure ordering relative to native browser liveness. Valid 0–11 dispatch and mapping stay unchanged.

## Bridge checks are separate from range validation

Native `[0x42fc76c,0x42fc818)` is 172 fully decoded bytes, SHA256 `ae5bf0a90b6d2c01545a267bbc2c5aa3ce5675b05e7140743d81e7acb453d8a1`.

- `0x42fc77c–0x42fc798` checks the +0xf0 record/flag and returns when missing/invalid.
- `0x42fc79c–0x42fc7a0` returns for a missing +0xf8 pointer.
- `0x42fc7a4–0x42fc7b0` repeats the +0xf0 checks; changed/missing state branches to `0x42fc814`, **BRK #0**. These are concrete field checks; their precise C++ weak-pointer type is not inferred here.
- After the virtual +0x78 call, `0x42fc7d0–0x42fc7dc` returns when either subsequent pointer is missing.
- `0x42fc7e0` is `cmp w21,#0xa`; `0x42fc7e4` is **B.LS** to the byte table for unsigned values 0–10. Otherwise it supplies Chromium value 1. There is no second enum range trap here; the public wrapper already restricts 0–11.

The table at `0xc9d6830` remains `09 01 03 04 01 06 05 01 08 03 03`, SHA256 `508ed213dd41ad855139d40951719da496a7c3a2282efb7b13726873d4ea55f0`; valid ordinal 11 takes the bridge default. The existing current product map and ready helper's valid-ordinal three-string listener expectations remain unchanged.

The retained readback script uses the existing `/tmp/arc-ui-capstone` decoder and the same FAT/segment addressing as existing `/tmp/arc-profile-disasm.py`. Independent scout `/root/extension_review/omnibox_semantics` reproduced the decisive branches, all range/table hashes and ordering with Xcode libLTO, CPU `apple-m1` (needed for LDAPURB). Its pinned arm64 slice SHA was `fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`. Neither decoder executes native code.

## Patch and exact temporary compiler verification

| Item | SHA256 |
| --- | --- |
| Actual browser source base | `84d532be49b5123068ed33ff340d741ef346a02234f3f58c43205b930f992b1e` |
| Proposed browser source | `ebc84f16a82b191e33f83da7b188cee175fe11ab73b71aedd595a96e31f6b35a` |
| `extensions_omnibox_range_guard.patch` | `72886519676cca645ff2a13ff8e300c8ea5b46773d37eb591be7917cc9fff2c6` |

The existing graph was queried read-only for `obj/arc/arc_core/arc_extensions_browser.o`; no build/regeneration command ran. The final exact target compiler command was retained, removing only output/dependency flags and adding `-fsyntax-only` plus a VFS overlay. It keeps the real Base warning/error/plugin/SDK/Chromium flags. Both canonical source and Chromium-symlink spellings map to the temporary proposed file with `use-external-names:false`.

- Positive exact-TU VFS compile: exit **0**, empty diagnostics.
- Separate overlay prepends only `#error ARC_EXT_OMNIBOX_RANGE_VFS_SENTINEL`; same compiler command: expected exit **1**, unique sentinel observed. This proves the compiler consumed the proposed source overlay rather than silently checking the unchanged product source.
- `git apply --check --whitespace=error`: exit **0**. Applying only in a fresh temporary tree yields exactly the proposed bytes.
- Actual browser hash was rechecked unchanged. Ready helper patch SHA remains `5faac79a9ad8c6167ad12fc37b0cf7480560f81258819aa762c27212ccec14e6`.
- Fresh read-only review `/root/extension_review/omnibox_review` confirmed the unsigned predicate, placement before `[self browser]`, unchanged valid path and explicit header; no findings.

Evidence: `logs/wp/extensions-omnibox-range-guard-proposal/validation.json`, native byte/disassembly files, reproducible readback script, exact compiler argv/query hash, both overlays, compiler logs, before/after copies and independent scout note. The first command-query capture attempted UTF-8 decoding of all dependency commands; only the final valid target command was needed and retained. That tooling correction did not execute compiler/build commands or change any source.

Parent review/application is pending. Existing valid-input omnibox probe design needs no change. No invalid-input runtime fixture, fake success, timeout response or data mask was added. No scenario diff or fresh runtime parity is claimed. This closes a bounded native scalar validation gap, consistent with the architecture's native-boundary requirement; it is not evidence of broader invalid-wrapper behavior parity.
