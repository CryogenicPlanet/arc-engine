# ArcPerformanceController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37e1c0`, class_ro `0xe2ae000`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| No declared ivars | — | — | — |

## Lifecycle, ownership and callbacks

No instance state. All six methods are class methods, sharing process singleton guarded by `0xe4a2f20` and initialized via `0x433d1bc`. Battery active/temporary-disabled bytes are read at provider+0x28/+0x29 (`0x443c65c`, `0x443c6c8`). Observer setter `0x443c7ac` holds weak callback capture; `0x443c924` invokes onBatterySaverModeChanged:.

## Threading and caller/callee relations

setHighEfficiencyModeEnabled: `0x8374b4c` → `0x7b02940` writes exact key performance_tuning.high_efficiency_mode.state with integer 2/0. Temporary disable `0x8374bc0` → `0x7afc3a8` updates byte only on change. Low-battery simulation `0x443c734` forwards unsigned duration to `0x4208bd4`; units are unresolved. Singleton construction guard does not prove thread safety of observer replacement or provider mutations.

## Chrome / CEF route implications

Source-backed memory-saver key Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/performance_manager/public/user_tuning/prefs.h:35` and battery-saver interface implementation Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/browser/performance_manager/user_tuning/battery_saver_mode_manager.cc:562` support Chrome performance-manager candidate mapping. Arc state numbering and provider behavior require specific validation; CEF route needs equivalent global policy controls, not only page throttling.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `+isBatterySaverModeActive` | `B16@0:8` | [0x443c65c](../bodies/443c65c.c) | Ensures shared controller singleton exists and returns native battery-saver active byte at provider+0x28, false if provider is absent. | 0x433d1bc | M `0xd77c844`; D 3026 | M; available; bridge_reviewed_partial |
| (base) | `+setHighEfficiencyModeEnabled:` | `v20@0:8B16` | [0x8374b4c](../bodies/8374b4c.c) | Ensures controller singleton and calls 0x7b02940, which stores memory-saver preference state 2 for enabled and 0 for disabled. | 0x433d1bc; 0x7b02940; 0xc44b610; 0xc44b760 | M `0xd77c850`; D 3027 | M; available; bridge_reviewed_partial |
| (base) | `+temporarilyDisableBatterySaverMode:` | `v20@0:8B16` | [0x8374bc0](../bodies/8374bc0.c) | Ensures controller singleton and calls 0x7afc3a8; sets temporary-disable byte at provider+0x29 only on change and invokes update. | 0x433d1bc; 0x7afc3a8; 0xc44b610; 0xc44b760 | M `0xd77c85c`; D 3028 | M; available; bridge_reviewed_partial |
| (base) | `+isBatterySaverModeTemporarilyDisabled` | `B16@0:8` | [0x443c6c8](../bodies/443c6c8.c) | Ensures singleton and reads temporary-disable byte at provider+0x29, false if absent. | 0x433d1bc | M `0xd77c868`; D 3029 | M; available; bridge_reviewed_partial |
| (base) | `+simulateLowBatteryStateForDuration:` | `v20@0:8I16` | [0x443c734](../bodies/443c734.c) | Forwards unsigned duration to 0x4208bd4 on battery provider after ensuring singleton. | 0x4208bd4; 0x433d1bc | M `0xd77c874`; D 3030 | M; available; bridge_reviewed_partial |
| (base) | `+setArcPerformanceObserver:` | `v24@0:8@16` | [0x443c7ac](../bodies/443c7ac.c) | Installs singleton observer callback with weak ObjC capture or empty callback for nil; adapter 0x443c924 invokes onBatterySaverModeChanged:. | 0x433d1bc; 0x433dcf0 | M `0xd77c880`; D 3031 | M; available; bridge_reviewed_partial |

## Remaining consequential questions

Validate duration units, provider-specific behavior, policy precedence, low-battery timer restoration, callback queue and observer initial-state replay.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
