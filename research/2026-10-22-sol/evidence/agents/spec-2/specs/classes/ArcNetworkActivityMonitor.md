# ArcNetworkActivityMonitor

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c6b8`, class_ro `0xe2a7958`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 24 | `_networkActivityMonitor` | `{optional<arc::BrowserApplication::NetworkActivityMonitor>=""(?="__null_state_"c"__val_"{NetworkActivityMonitor="impl_"{shared_ptr…` | 0xe2a7910 |

Initialization stores an engaged shared native monitor (0x44085cc). Equality compares compatible class/native pointer and hash hashes the pointer after engagement checks (0x44086f0, 0x4408860). startLogging unwraps configuration and browser context, calls 0x432c950 and converts status through 0x4402548 (0x44088ac).

## Lifecycle, callbacks and relations

The backend rejects headless contexts and already-active logging, starts a NetExport-like writer, collects prerenderInfo/extensionInfo and schedules duration in microseconds (0x432c950). isLogging reads textual states including INITIALIZING, STARTING_LOG and LOGGING (0x4408898 → 0x432c834). This differs from stock NetExportFileWriter::IsLogging, which includes STOPPING_LOG instead of INITIALIZING (components/net_log/net_export_file_writer.cc:312).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome NetExport integration is a source-backed inference from matching state strings and StartNetLog call shape (same source :157, :275). CEF logging API equivalence is unresolved; generic network interception is insufficient evidence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNetworkActivityMonitor.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init: | `@32@0:8{NetworkActivityMonitor={shared_ptr<arc::BrowserApplication::NetworkActivityMonitor::Impl>=^{Impl}^{__shared_weak_count}}}16` | 0x44085cc | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x44085cc; record 0xd7796d0; dyld L1884; otool L26475 | medium / available / local_observation |
| base | - isEqual: | `B24@0:8@16` | 0x44086f0 | Checks same-object identity, compatible ObjC class, then compares unwrapped native handle pointers; no content equality established. | local state / indirect operation; see body | IMP 0x44086f0; record 0xd7796dc; dyld L1885; otool L26478 | medium / available / bounded_review |
| base | - hash | `Q16@0:8` | 0x4408860 | Requires engaged native handle and hashes its pointer through 0x18f3c. | 0x00018f3c, 0x04b08a00 | IMP 0x4408860; record 0xd7796e8; dyld L1886; otool L26481 | medium / available / bounded_review |
| base | - isLogging | `B16@0:8` | 0x4408898 | Requires engaged monitor; 0x432c834 reads writer state string and accepts INITIALIZING, STARTING_LOG or LOGGING. | 0x0432c834 | IMP 0x4408898; record 0xd7796f4; dyld L1887; otool L26484 | medium / available / bounded_review |
| base | - startLogging:with:error: | `B40@0:8@16@24^@32` | 0x44088ac | Requires engaged monitor/context, unwraps configuration and context, calls 0x432c950 then 0x4402548 for BOOL/NSError. Backend rejects headless/already-logging, starts writer and duration timer. | 0x04b08a00, 0x0432c950, 0x04402548, 0x04b35898 | IMP 0x44088ac; record 0xd779700; dyld L1888; otool L26487 | medium / available / bounded_review |
| base | - networkActivityMonitor | `{optional<arc::BrowserApplication::NetworkActivityMonitor>=(?=c{NetworkActivityMonitor={shared_ptr<arc::BrowserApplication::NetworkActivityMonitor::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x18, 0x10, 0x8. Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43df53c; record 0xd77970c; dyld L1889; otool L26490 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x43df69c | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x18, 0x10. | local state / indirect operation; see body | IMP 0x43df69c; record 0xd779718; dyld L1890; otool L22685 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43df760 | Initializes native storage/engagement fields at self offsets 0x8, 0x18; returns self. | local state / indirect operation; see body | IMP 0x43df760; record 0xd779724; dyld L1891; otool L22688 | medium / available / local_observation |

## Remaining consequential questions

Verify stop/timer callback and file finalization, the exact NetExport writer identity, thread affinity and whether overlapping monitors share a writer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
