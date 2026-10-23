# ArcSiteSettingInfo

ArcCore 1.170.0 arm64. 13 declared rows; superclass `NSObject`. Class metadata `0xe37c9d8`, class_ro `0xe2a8548`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Mutable native site-setting descriptor, not itself a persistence operation.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_info` | 0x8 | 24 | `{optional<arc::SiteSettingInfo>=""(?="__null_state_"c"__val_"{SiteSettingInfo="impl_"{shared_ptr<arc::SiteSettingInfo::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2a84d0` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `info`: `T{optional<arc::SiteSettingInfo>=(?=c{SiteSettingInfo={shared_ptr<arc::SiteSettingInfo::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_info` at `0xe2a84f8`.
- `originUrl`: `T@"NSString",&,N` at `0xe2a8508`.
- `permissionType`: `Tq,R,N` at `0xe2a8518`.
- `contentSettingsType`: `TQ,N` at `0xe2a8528`.
- `siteSetting`: `TQ,N` at `0xe2a8538`.

## Lifecycle, ownership, callbacks and threading

init 0x440b880 creates a shared native info object and initializes native settings. 0x440bc68/0x440bc8c constrain Arc contentSettingsType to 0..36. siteSetting 0x440bcb0 maps a native 0..5 value through table 0xc9d8188, returning 0 when absent/unsupported; setSiteSetting 0x440bcf8 accepts 0..4 and calls 0x4341964. setOriginUrl 0x440bb94 calls 0x4341874; originUrl 0x440bae8 traps if required origin optional is empty.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The native values and Arc enum are not interchangeable with Chromium ContentSettingsType ordinal values. Persistence requires the consumer that commits this descriptor into a profile settings service; none is proven here. CEF settings support needs a separate exact enum/permission review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -init | `@16@0:8` | 0x440b880 | Calls super init; constructs shared native info state, initializes type and native settings aggregate, then installs engaged optional. | 0x8790,0xbd3c0b4,0x4331f00,0x5810 | [0x440b880](../bodies/0x440b880.c); record 0xd779bd8; dyld L2003 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -initWithSiteSettingInfo: | `@24@0:8r^v16` | 0x440b9c4 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x440b9c4](../bodies/0x440b9c4.c); record 0xd779be4; dyld L2004 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -originUrl | `@16@0:8` | 0x440bae8 | Converts stored/native UTF-8 string data to NSString via 0x1c391bc. Traps if outer optional is empty. | 0x7d0cf8,0x1c391bc,0x5810 | [0x440bae8](../bodies/0x440bae8.c); record 0xd779bf0; dyld L2005 | medium / local_verified / available | Native string validity and invalid-encoding behavior need runtime validation. |
| (base) | -setOriginUrl: | `v24@0:8@16` | 0x440bb94 | Converts supplied string (nil becomes empty native string) and calls 0x4341874 on engaged peer. | 0x1b356b8,0x4341874,0x5810 | [0x440bb94](../bodies/0x440bb94.c); record 0xd779bfc; dyld L2006 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -permissionType | `q16@0:8` | 0x440bc28 | Maps native type through 0x4332684, 0x2122528 and 0x433f850; returns fallback integer 28 when no mapping is present. | 0x4332684,0x2122528,0x433f850 | [0x440bc28](../bodies/0x440bc28.c); record 0xd779c08; dyld L2007 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -contentSettingsType | `Q16@0:8` | 0x440bc68 | Requires engaged optional; returns native integer only when <37, otherwise traps. | No direct anonymous call recorded / indirect possible | [0x440bc68](../bodies/0x440bc68.c); record 0xd779c14; dyld L2008 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -setContentSettingsType: | `v24@0:8Q16` | 0x440bc8c | Requires engaged optional; accepts integer <37 and stores native integer, otherwise traps. | No direct anonymous call recorded / indirect possible | [0x440bc8c](../bodies/0x440bc8c.c); record 0xd779c20; dyld L2009 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -siteSetting | `Q16@0:8` | 0x440bcb0 | Returns 0 when native setting is absent/unsupported; otherwise maps native 0..5 through byte table 0xc9d8188. | No direct anonymous call recorded / indirect possible | [0x440bcb0](../bodies/0x440bcb0.c); record 0xd779c2c; dyld L2010 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -setSiteSetting: | `v24@0:8Q16` | 0x440bcf8 | Requires engaged optional; accepts 0..4 and calls 0x4341964; out-of-range input traps. | 0x4341964 | [0x440bcf8](../bodies/0x440bcf8.c); record 0xd779c38; dyld L2011 | medium / reviewed_partial / available | Recover enum/table meanings and persistence consumer; no profile write is proven. |
| (base) | -info | `{optional<arc::SiteSettingInfo>=(?=c{SiteSettingInfo={shared_ptr<arc::SiteSettingInfo::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../bodies/0x43df53c.c); record 0xd779c44; dyld L2012 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -setInfo: | `v40@0:8{optional<arc::SiteSettingInfo>=(?=c{SiteSettingInfo={shared_ptr<arc::SiteSettingInfo::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | 0x43df57c | Assigns optional shared peer: copy/replace when engaged, increments new control count, releases old control block; clears engagement for empty input. | 0x4331e20,0x5810 | [0x43df57c](../bodies/0x43df57c.c); record 0xd779c50; dyld L2013 | medium / local_verified / available | Validate aggregate input ABI and detached peer handling. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../bodies/0x43df69c.c); record 0xd779c5c; dyld L2014 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../bodies/0x43df760.c); record 0xd779c68; dyld L2015 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover table 0xc9d8188, native setter mapping, origin canonicalization and the commit-to-profile caller.
