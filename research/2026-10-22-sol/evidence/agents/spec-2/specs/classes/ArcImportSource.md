# ArcImportSource

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 3 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c398`, class_ro `0xe2a6f00`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_sourceID` | `q` | 0xe2a6e98 |
| 0x10 | 8 | `_type` | `q` | 0xe2a6eb8 |

initWithImportSource: sign-extends sourceID from native int32 and translates the source type using a validity bitmask and table at 0xc9d8158, trapping unsupported values (0x4403e9c). sourceID/type are raw 64-bit slot reads at +8/+0x10 (0x2d0fb40, 0x2cb140).

## Lifecycle, callbacks and relations

This object copies two scalar values. It neither enumerates external profiles nor starts an importer, and has no declared destructor or callback. Its native ImportSource name does not establish identity with an upstream importer struct.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable import-source DTO, with Chrome/Safari import backends elsewhere. CEF import support cannot be concluded from this adapter.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcImportSource.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithImportSource: | `@24@0:8r^{ImportSource=ii}16` | 0x4403e9c | Copies signed source ID and maps valid native enum indices via table 0xc9d8158 with validity mask 0x7f1d; unsupported values trap. | local state / indirect operation; see body | IMP 0x4403e9c; record 0xd779218; dyld L1775; otool L25662 | medium / available / bounded_review |
| base | - sourceID | `q16@0:8` | 0x2d0fb40 | Reads 64-bit register value from self+0x8 (_sourceID); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2d0fb40; record 0xd779224; dyld L1776; otool L21978 | high / available / local_verified |
| base | - type | `q16@0:8` | 0x2cb140 | Reads 64-bit register value from self+0x10 (_type); exact load+RET. Interpret result using this metadata encoding. | scalar field; machine words in imp-prefixes.json | IMP 0x2cb140; record 0xd779230; dyld L1777; otool L21914 | high / available / local_verified |

## Remaining consequential questions

Map each accepted native source enum to its Arc-facing value and actual importer; confirm the app-facing labels. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
