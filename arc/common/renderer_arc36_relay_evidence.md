# Native TopBand runtime flag: corrected enum mapping and startup relay

Read-only audit against pristine ArcCore 1.170 image:
`/Users/cryogenicplanet/general/arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
SHA256 `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`,
501736176 bytes, arm64 slice file offset262438912, Mach-O UUID
`4c4c44a0-5555-3144-a1b3-01c201c55814`. Addresses below are unslid arm64 VAs.
Pin and bounded logs: `logs/wp/renderer-arc36-relay/`.

## Correction: TopBand is public enum31, not36

This explicitly supersedes the Arc36/name/initializer association in
renderer_contract_evidence.md:1478..1485 and prior handoff prose. The earlier
audit paired an enum at the end of one name record with the next name pointer.
There is no proven native name/config mismatch. The corrected table and its
consumer establish:

| Name-record VA | Name pointer/string | Length | Enum at record +16 |
| --- | --- | ---: | ---: |
| 0xdff38b8 | 0xd4625f6 `kEnableTabLifecycleSnapshot` | 27 | 36 |
| 0xdff38d0 | 0xd462612 `kEnableTopBandColorSampling` | 27 | 31 |
| 0xdff38e8 | 0xd46262e `kHeifImageDecoding` | 18 | 43 |

This is not an assumed record schema. Native `0x44d88cc..0x44d88d0` loads name
table base `0xdff3750`. Lookup `0x44da780` uses46 records and stride24
(`0x44da7b8..0x44da7bc`), loads string pointer/length from record +0/+8 at
`0x44da7d0`, and compares the string. Its caller loads the matched **enum from
record +16** at `0x44d8970`, then uses that enum in the ordinary boolean setter
at `0x44d8934`. Thus the enum31 and36 names are independently pinned by a real
consumer, not guessed from adjacent constants.

The initializer table is separate, starts `0xdff3480`, and uses16-byte records:
enum at +0, initializer function pointer at +8. Its iteration invokes +8 at
`0x44d869c..0x44d86a4`, loads the corresponding enum at `0x44d86a8`, and inserts
that ID/value into the runtime entries; it advances16 bytes at `0x44d8690`.
An absent enum8 means numeric enum is not the initializer array index.

| Initializer record | Enum | Initializer | Result |
| --- | ---: | --- | --- |
| 0xdff3660 | 31 | 0x44d92b8 | bool default false, then JSON key `enableTopBandColorSampling` |
| 0xdff36b0 | 36 | 0x44d8a48 | generic bool false |

At `0x44d92cc..0x44d92e0`, enum31's initializer constructs a bool variant with
value0/type1. At `0x44d92e4..0x44d92f4` it passes key `0xd4623a9`, length26,
`enableTopBandColorSampling`, to `0x44da874`. Enum36's generic initializer
constructs false/type1 at `0x44d8a58..0x44d8a68` and does not use that key.

## Public getter/setter preserve those numeric IDs

Public getter IMP `0x43e81d8` only special-cases low32 ID45 (base feature
ArcMacCoalescedResizing); all other IDs pass unchanged to `0x20cf42c`.
The latter finds the numeric ID by binary search in the runtime manager's
sorted40-byte entries (`0x20cf488..0x20cf4bc`), checks variant type1 at +32,
and returns stored bool +8 at `0x20cf4cc`. It does not remap31 to36 or the
reverse. Public boolean setter `0x43e81f4..0x43e8214` forwards its ID unchanged
to manager setter `0x44d9734`; its search compares the supplied low32 ID with
the entry's ID (`0x44d978c`, `0x44d97b0`). Equal current bool returns without
notification (`0x44d97c8..0x44d97cc`); a changed bool stores through
`0x1195640` at `0x44d9834`.

Changed-value notification then synchronously iterates the global weak-observer
list and calls observer virtual +16 with the numeric enum only
(`0x44d9984..0x44d9990`). The recovered subscriptions are:

- Ad-block observer `0x4296ee8 -> 0x4296f2c`: callback `0x42978ac` accepts11
  and returns for other enums.
- Memory observer `0x432ef1c -> 0x432ef60`: callback thunk `0x432f28c` enters
  `0x432f0d0`, which handles20..23 and returns for31/36.

All13 direct callers of singleton accessor `0x44d84a0` were inspected in this
bounded follow-up. Those were its two observed subscriptions. The observer
registration helper's other two direct callers use separate profile lists.
No live TopBand relay was found in these recovered observer registrations.
That is not proof that every possible indirect consumer is absent.

## Positive startup relay reads31 and enables the Blink runtime feature

Command-line hook `0x42abb3c` receives the child command line in x1 and keeps it
in x19. It queries switch `type` (`0xc6d7369`, length4) at `0x42abc38..0x42abc50`
and compares its length8/value with literal `renderer` at
`0x42abc68..0x42abc90`. Only the renderer branch reaches:

1. `0x42abd14`: materialize **w0=31**; `0x42abd18`: call ordinary getter
   `0x20cf42c`. False returns without adding the TopBand switch.
2. `0x42abd24`: load the name pointer from base feature descriptor `0xe2c32b8`.
   The pointer is `0xd0eb641`, exactly `TopBandColorSampling`.
3. `0x42abd2c..0x42abd34`: obtain that name's length.
4. `0x42abd38..0x42abd4c`: call command-line append helper `0x1b2e164` with
   child command line, literal `enable-blink-features` (`0xc6d6e15`, length21),
   and `TopBandColorSampling` as its value.

Helper `0x1b2e164` copies switch/value, handles switch prefixes, assigns its
switch map, appends '=' for a nonempty value and inserts the switch into argv.
Its source/ABI correspondence is CommandLine::AppendSwitchNative on POSIX
(base/command_line.cc:436..483). The positive call arguments and literal bytes
are native facts even without relying on the stripped outer method's C++ name.

The descriptor's default-state word +8 is0 (disabled). Prior independently
recovered Blink startup mapping remains valid: feature table entry934 points
to that descriptor, flag-index669 maps to runtime bool `0xe369ced`, and the
generic enabled-or-overridden base-feature updater initializes the bool.
Generic enable-blink-features processing can override the runtime flag at
renderer startup. No native live propagation is established here.

## Controls: what prior results establish and what to test next

Native controls `results/renderer-topband-native-retry1-run2` recorded callbacks:
default0, base-feature9, Arc-index36-only0, both9. The Arc-index36 conditions
verified getter36=true before the first observed context call. Those controls
varied **TabLifecycleSnapshot**, not the recovered TopBand flag31; they cannot
establish that the TopBand public relay is absent. The live36 stages were also
at scroll400 and silent, so they supply no additional TopBand relay conclusion.

The next discriminating native control is public setter/getter **31=true before
a new renderer is launched**, with no base TopBand feature override. Record
actual31/36 values and renderer command line where available; use neutral
numeric labels in probe metadata. Compare unscrolled plain-color changes and
an explicit empty/nil-producing geometry, preserving the existing controls.
Then test31=false/true with a fresh renderer versus an existing renderer to
separate the recovered startup hook from any live update. The identified
observer callbacks predict no direct live TopBand reconfiguration, but a live
absence claim requires a stimulus that emits in the matched enabled control.

Fresh-context independent review has verified the raw bytes, actual name-table
consumer, separate initializer table, public numeric ID routing and positive
startup call. It is CLEAN and finds the31-before-new-renderer control statically
justified. The preserved review is
`logs/wp/renderer-arc36-relay/independent-static-review.txt`. This is static
verification, not execution of that control. No app launch, builds, production
or proposal edits, locks, or board operations were part of this audit.
