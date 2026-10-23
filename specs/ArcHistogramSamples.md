# ArcHistogramSamples

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 39 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c208`, class_ro `0xe2a6a30`; instanceStart 8, instanceSize 304 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_browserMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6338 |
| 0x10 | 8 | `_totalMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6358 |
| 0x18 | 8 | `_averageTotalCpuWhileInBackgroundPercent` | `@"ArcHistogramSample"` | 0xe2a6378 |
| 0x20 | 8 | `_averageTotalCpuPercent` | `@"ArcHistogramSample"` | 0xe2a6398 |
| 0x28 | 8 | `_averageBrowserCpuPercent` | `@"ArcHistogramSample"` | 0xe2a63b8 |
| 0x30 | 8 | `_idleWakeupsTotalWhileInBackground` | `@"ArcHistogramSample"` | 0xe2a63d8 |
| 0x38 | 8 | `_idleWakeupsTotal` | `@"ArcHistogramSample"` | 0xe2a63f8 |
| 0x40 | 8 | `_batteryDischargeRateNavigation` | `@"ArcHistogramSample"` | 0xe2a6418 |
| 0x48 | 8 | `_batteryDischargeRateWhileInBackground` | `@"ArcHistogramSample"` | 0xe2a6438 |
| 0x50 | 8 | `_batteryDischargeRate` | `@"ArcHistogramSample"` | 0xe2a6458 |
| 0x58 | 8 | `_memoryPressureWindowDurationCriticalToModerate` | `@"ArcHistogramSample"` | 0xe2a6478 |
| 0x60 | 8 | `_memoryPressureWindowDurationCriticalToNone` | `@"ArcHistogramSample"` | 0xe2a6498 |
| 0x68 | 8 | `_memoryPressureWindowDurationModerateToCritical` | `@"ArcHistogramSample"` | 0xe2a64b8 |
| 0x70 | 8 | `_memoryPressureWindowDurationModerateToNone` | `@"ArcHistogramSample"` | 0xe2a64d8 |
| 0x78 | 8 | `_averageGpuProcessCpuPercent` | `@"ArcHistogramSample"` | 0xe2a64f8 |
| 0x80 | 8 | `_averageNetworkProcessCpuPercent` | `@"ArcHistogramSample"` | 0xe2a6518 |
| 0x88 | 8 | `_averageExtensionEventProcessCpuPercent` | `@"ArcHistogramSample"` | 0xe2a6538 |
| 0x90 | 8 | `_averageExtensionPersistentProcessCpuPercent` | `@"ArcHistogramSample"` | 0xe2a6558 |
| 0x98 | 8 | `_averageRendererProcessCpuPercent` | `@"ArcHistogramSample"` | 0xe2a6578 |
| 0xa0 | 8 | `_averageTotalCpuDuringFullscreenVideoPercent` | `@"ArcHistogramSample"` | 0xe2a6598 |
| 0xa8 | 8 | `_averageTotalCpuWithInteractionPercent` | `@"ArcHistogramSample"` | 0xe2a65b8 |
| 0xb0 | 8 | `_averageTotalCpuWithNavigationPercent` | `@"ArcHistogramSample"` | 0xe2a65d8 |
| 0xb8 | 8 | `_averageTotalCpuWhenIdleForegroundPercent` | `@"ArcHistogramSample"` | 0xe2a65f8 |
| 0xc0 | 8 | `_gpuMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6618 |
| 0xc8 | 8 | `_rendererMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6638 |
| 0xd0 | 8 | `_totalRendererMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6658 |
| 0xd8 | 8 | `_totalSharedMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6678 |
| 0xe0 | 8 | `_totalPrivateSwapFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6698 |
| 0xe8 | 8 | `_totalResidentSetMb` | `@"ArcHistogramSample"` | 0xe2a66b8 |
| 0xf0 | 8 | `_totalGpuMappedMemoryMb` | `@"ArcHistogramSample"` | 0xe2a66d8 |
| 0xf8 | 8 | `_totalTileMemoryMb` | `@"ArcHistogramSample"` | 0xe2a66f8 |
| 0x100 | 8 | `_networkServiceMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6718 |
| 0x108 | 8 | `_extensionMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6738 |
| 0x110 | 8 | `_aiServiceMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6758 |
| 0x118 | 8 | `_audioServiceMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6778 |
| 0x120 | 8 | `_utilityMemoryFootprintMb` | `@"ArcHistogramSample"` | 0xe2a6798 |
| 0x128 | 8 | `_averageAIServiceCpuPercent` | `@"ArcHistogramSample"` | 0xe2a67b8 |

initWithPeriodicHistogramSample: builds up to 37 ArcHistogramSample objects from present native sample fields and stores them in the 37 object slots (0x4402ad8). The individual getters are raw slot loads, many shared with unrelated classes; interpret returned object types using metadata (e.g. 0x2d0fb40, 0x31fa70).

## Lifecycle, callbacks and relations

Destructor clears the 37 slots (0x44032d8). This is a snapshot wrapper, with no collection timer, process enumeration, histogram aggregation, callback or persistence operation demonstrated in its methods. Nil represents an absent native sample branch in the initializer (0x4402ad8).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome/OS metric collection lives outside the DTO. A CEF route needs an independent source for the same process-category samples; names alone do not establish measurement units or sampling intervals.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcHistogramSamples.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithPeriodicHistogramSample: | `@24@0:8r^v16` | 0x4402ad8 | Tests native per-field presence and constructs up to 37 ArcHistogramSample wrappers in object slots; no metrics collection here. | local state / indirect operation; see body | IMP 0x4402ad8; record 0xd778ea8; dyld L1695; otool L24945 | medium / available / bounded_review |
| base | - browserMemoryFootprintMb | `@16@0:8` | 0x2d0fb40 | Reads 64-bit register value from self+0x8 (_browserMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2d0fb40; record 0xd778eb4; dyld L1696; otool L12036 | high / available / local_verified |
| base | - totalMemoryFootprintMb | `@16@0:8` | 0x2cb140 | Reads 64-bit register value from self+0x10 (_totalMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2cb140; record 0xd778ec0; dyld L1697; otool L12039 | high / available / local_verified |
| base | - averageTotalCpuWhileInBackgroundPercent | `@16@0:8` | 0x4084dc0 | Reads 64-bit register value from self+0x18 (_averageTotalCpuWhileInBackgroundPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4084dc0; record 0xd778ecc; dyld L1698; otool L13108 | high / available / local_verified |
| base | - averageTotalCpuPercent | `@16@0:8` | 0x4060580 | Reads 64-bit register value from self+0x20 (_averageTotalCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4060580; record 0xd778ed8; dyld L1699; otool L12042 | high / available / local_verified |
| base | - averageBrowserCpuPercent | `@16@0:8` | 0x133ae90 | Reads 64-bit register value from self+0x28 (_averageBrowserCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x133ae90; record 0xd778ee4; dyld L1700; otool L12045 | high / available / local_verified |
| base | - idleWakeupsTotalWhileInBackground | `@16@0:8` | 0x2131e0 | Reads 64-bit register value from self+0x30 (_idleWakeupsTotalWhileInBackground); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2131e0; record 0xd778ef0; dyld L1701; otool L12048 | high / available / local_verified |
| base | - idleWakeupsTotal | `@16@0:8` | 0x327c740 | Reads 64-bit register value from self+0x38 (_idleWakeupsTotal); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x327c740; record 0xd778efc; dyld L1702; otool L12051 | high / available / local_verified |
| base | - batteryDischargeRateNavigation | `@16@0:8` | 0x3547bf0 | Reads 64-bit register value from self+0x40 (_batteryDischargeRateNavigation); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x3547bf0; record 0xd778f08; dyld L1703; otool L12054 | high / available / local_verified |
| base | - batteryDischargeRateWhileInBackground | `@16@0:8` | 0x4080f00 | Reads 64-bit register value from self+0x48 (_batteryDischargeRateWhileInBackground); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4080f00; record 0xd778f14; dyld L1704; otool L12057 | high / available / local_verified |
| base | - batteryDischargeRate | `@16@0:8` | 0x4080f20 | Reads 64-bit register value from self+0x50 (_batteryDischargeRate); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4080f20; record 0xd778f20; dyld L1705; otool L12060 | high / available / local_verified |
| base | - memoryPressureWindowDurationCriticalToModerate | `@16@0:8` | 0x4085370 | Reads 64-bit register value from self+0x58 (_memoryPressureWindowDurationCriticalToModerate); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4085370; record 0xd778f2c; dyld L1706; otool L12063 | high / available / local_verified |
| base | - memoryPressureWindowDurationCriticalToNone | `@16@0:8` | 0x35f0300 | Reads 64-bit register value from self+0x60 (_memoryPressureWindowDurationCriticalToNone); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x35f0300; record 0xd778f38; dyld L1707; otool L12066 | high / available / local_verified |
| base | - memoryPressureWindowDurationModerateToCritical | `@16@0:8` | 0xf90b90 | Reads 64-bit register value from self+0x68 (_memoryPressureWindowDurationModerateToCritical); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0xf90b90; record 0xd778f44; dyld L1708; otool L12069 | high / available / local_verified |
| base | - memoryPressureWindowDurationModerateToNone | `@16@0:8` | 0x408dad8 | Reads 64-bit register value from self+0x70 (_memoryPressureWindowDurationModerateToNone); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x408dad8; record 0xd778f50; dyld L1709; otool L12072 | high / available / local_verified |
| base | - averageGpuProcessCpuPercent | `@16@0:8` | 0x373be80 | Reads 64-bit register value from self+0x78 (_averageGpuProcessCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x373be80; record 0xd778f5c; dyld L1710; otool L12075 | high / available / local_verified |
| base | - averageNetworkProcessCpuPercent | `@16@0:8` | 0x4085a40 | Reads 64-bit register value from self+0x80 (_averageNetworkProcessCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4085a40; record 0xd778f68; dyld L1711; otool L12078 | high / available / local_verified |
| base | - averageExtensionEventProcessCpuPercent | `@16@0:8` | 0x4084d88 | Reads 64-bit register value from self+0x88 (_averageExtensionEventProcessCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4084d88; record 0xd778f74; dyld L1712; otool L12081 | high / available / local_verified |
| base | - averageExtensionPersistentProcessCpuPercent | `@16@0:8` | 0x403cb0 | Reads 64-bit register value from self+0x90 (_averageExtensionPersistentProcessCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x403cb0; record 0xd778f80; dyld L1713; otool L24999 | high / available / local_verified |
| base | - averageRendererProcessCpuPercent | `@16@0:8` | 0x4080f28 | Reads 64-bit register value from self+0x98 (_averageRendererProcessCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4080f28; record 0xd778f8c; dyld L1714; otool L25002 | high / available / local_verified |
| base | - averageTotalCpuDuringFullscreenVideoPercent | `@16@0:8` | 0x414f3e0 | Reads 64-bit register value from self+0xa0 (_averageTotalCpuDuringFullscreenVideoPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x414f3e0; record 0xd778f98; dyld L1715; otool L25005 | high / available / local_verified |
| base | - averageTotalCpuWithInteractionPercent | `@16@0:8` | 0x40cc040 | Reads 64-bit register value from self+0xa8 (_averageTotalCpuWithInteractionPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x40cc040; record 0xd778fa4; dyld L1716; otool L25008 | high / available / local_verified |
| base | - averageTotalCpuWithNavigationPercent | `@16@0:8` | 0x413dde0 | Reads 64-bit register value from self+0xb0 (_averageTotalCpuWithNavigationPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x413dde0; record 0xd778fb0; dyld L1717; otool L25011 | high / available / local_verified |
| base | - averageTotalCpuWhenIdleForegroundPercent | `@16@0:8` | 0x4084380 | Reads 64-bit register value from self+0xb8 (_averageTotalCpuWhenIdleForegroundPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4084380; record 0xd778fbc; dyld L1718; otool L25014 | high / available / local_verified |
| base | - gpuMemoryFootprintMb | `@16@0:8` | 0x414f55c | Reads 64-bit register value from self+0xc0 (_gpuMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x414f55c; record 0xd778fc8; dyld L1719; otool L25017 | high / available / local_verified |
| base | - rendererMemoryFootprintMb | `@16@0:8` | 0x413ddc0 | Reads 64-bit register value from self+0xc8 (_rendererMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x413ddc0; record 0xd778fd4; dyld L1720; otool L25020 | high / available / local_verified |
| base | - totalRendererMemoryFootprintMb | `@16@0:8` | 0x419a380 | Reads 64-bit register value from self+0xd0 (_totalRendererMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x419a380; record 0xd778fe0; dyld L1721; otool L25023 | high / available / local_verified |
| base | - totalSharedMemoryFootprintMb | `@16@0:8` | 0x44032d0 | Reads 64-bit register value from self+0xd8 (_totalSharedMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x44032d0; record 0xd778fec; dyld L1722; otool L25026 | high / available / local_verified |
| base | - totalPrivateSwapFootprintMb | `@16@0:8` | 0x410d384 | Reads 64-bit register value from self+0xe0 (_totalPrivateSwapFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x410d384; record 0xd778ff8; dyld L1723; otool L25029 | high / available / local_verified |
| base | - totalResidentSetMb | `@16@0:8` | 0x25bf698 | Reads 64-bit register value from self+0xe8 (_totalResidentSetMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x25bf698; record 0xd779004; dyld L1724; otool L25032 | high / available / local_verified |
| base | - totalGpuMappedMemoryMb | `@16@0:8` | 0x4096808 | Reads 64-bit register value from self+0xf0 (_totalGpuMappedMemoryMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4096808; record 0xd779010; dyld L1725; otool L25035 | high / available / local_verified |
| base | - totalTileMemoryMb | `@16@0:8` | 0x40a7080 | Reads 64-bit register value from self+0xf8 (_totalTileMemoryMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x40a7080; record 0xd77901c; dyld L1726; otool L25038 | high / available / local_verified |
| base | - networkServiceMemoryFootprintMb | `@16@0:8` | 0x4084000 | Reads 64-bit register value from self+0x100 (_networkServiceMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4084000; record 0xd779028; dyld L1727; otool L25041 | high / available / local_verified |
| base | - extensionMemoryFootprintMb | `@16@0:8` | 0x1a4a1c0 | Reads 64-bit register value from self+0x108 (_extensionMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x1a4a1c0; record 0xd779034; dyld L1728; otool L25044 | high / available / local_verified |
| base | - aiServiceMemoryFootprintMb | `@16@0:8` | 0x4150e98 | Reads 64-bit register value from self+0x110 (_aiServiceMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4150e98; record 0xd779040; dyld L1729; otool L25047 | high / available / local_verified |
| base | - audioServiceMemoryFootprintMb | `@16@0:8` | 0x414f400 | Reads 64-bit register value from self+0x118 (_audioServiceMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x414f400; record 0xd77904c; dyld L1730; otool L25050 | high / available / local_verified |
| base | - utilityMemoryFootprintMb | `@16@0:8` | 0x4150790 | Reads 64-bit register value from self+0x120 (_utilityMemoryFootprintMb); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x4150790; record 0xd779058; dyld L1731; otool L25053 | high / available / local_verified |
| base | - averageAIServiceCpuPercent | `@16@0:8` | 0x31fa70 | Reads 64-bit register value from self+0x128 (_averageAIServiceCpuPercent); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x31fa70; record 0xd779064; dyld L1732; otool L25056 | high / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x44032d8 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x128, 0x120, 0x118, 0x110, 0x108, 0x100, 0xf8, 0xf0. | local state / indirect operation; see body | IMP 0x44032d8; record 0xd779070; dyld L1733; otool L25059 | medium / available / local_observation |

## Remaining consequential questions

Recover sample optionality and units from the producer and ArcHistogramSample; validate all 37 constructor-source offsets against runtime samples. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.


Original reviewed source: [ArcHistogramSamples.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcHistogramSamples.md).
