# ArcDownloadItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 21 declared identities. Superclass `NSObject`; class metadata `0xe37d518`, class RO `0xe2ab250`; raw otool `objc170.txt:30874`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

Init retains optional native shared item (0x442f548), with shared destructor 0x440a40c; methods require optional engagement and check a live backend through virtual dispatch. completedBytes returns zero when backend is missing (0x442f66c). currentState maps native 0..3 to exposed 1..4, returning 0 for no live item (0x442fc60). remainingTimeInSeconds returns -1 on status failure (0x442fcd0). pause/cancel/resume/remove dispatch through native vtable slots, including forwarding cancel's boolean (0x442fd80, 0x442fdd8, 0x442fe38, 0x442fe90). Underlying download survival after profile shutdown, callback update producer and thread affinity are not established.

Chromium components/download/public/common/download_item.h:198-215,272,481-502 supplies source-backed candidate methods for these operations, not a proven vtable layout match. CEF feasibility requires preserving item identity/lifetime, state mapping, destination path and webContents association (0x442fa70); primitive download controls alone do not establish parity.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _webContents | 8 | 8 | `@"ArcWebContents"` | 0xe2ab1f8 |
| _download_item | 16 | 24 | `{optional<arc::DownloadItem>=""(?="__null_state_"c"__val_"{DownloadItem="impl_"{shared_ptr<arc::DownloadItem::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2ab218 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| download_item | `T{optional<arc::DownloadItem>=(?=c{DownloadItem={shared_ptr<arc::DownloadItem::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_download_item` | 0xe2ab240 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithDownloadItem:` | `@24@0:8r^v16` | [0x442f548](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f548.txt) | Calls NSObject init; retains native shared handle at self+0x10/+0x18 and engages +0x20. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b240`; dobjc170.txt:2503 | medium-local; available; local_review |
| (base) | `-completedBytes` | `q16@0:8` | [0x442f66c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f66c.txt) | Requires optional engagement; returns zero if native backend is missing/dead, otherwise invokes backend vtable slot +0x28. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b24c`; dobjc170.txt:2504 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-totalBytes` | `q16@0:8` | [0x442f6c8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f6c8.txt) | Requires optional engagement; returns zero if native backend missing/dead, otherwise invokes backend vtable slot +0x30. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b258`; dobjc170.txt:2505 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-currentSpeed` | `q16@0:8` | [0x442f724](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f724.txt) | Requires optional engagement; returns zero if native backend missing/dead, otherwise re-queries live item and reads speed field +0x430 only when flag +0x3a4 low bit is clear. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b264`; dobjc170.txt:2506 | medium-partial; available; partial_review |
| (base) | `-targetPath` | `@16@0:8` | [0x442f788](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f788.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x60a60,0x1c391bc | metadata `0xd77b270`; dobjc170.txt:2507 | medium-local; available; conversion_partial |
| (base) | `-sourceURL` | `@16@0:8` | [0x442f8b0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f8b0.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x430dfbc,0x1c391bc | metadata `0xd77b27c`; dobjc170.txt:2508 | medium-local; available; conversion_partial |
| (base) | `-id` | `@16@0:8` | [0x442f968](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442f968.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x430e358,0x1c391bc | metadata `0xd77b288`; dobjc170.txt:2509 | medium-local; available; conversion_partial |
| (base) | `-startTime` | `@16@0:8` | [0x442fa20](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fa20.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x430def0. Observed selector operands: dateWithTimeIntervalSince1970:. | 0x430def0 | metadata `0xd77b294`; dobjc170.txt:2510 | low-unresolved; available; unresolved |
| (base) | `-webContents` | `@16@0:8` | [0x442fa70](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fa70.txt) | Returns cached ObjC wrapper if present; otherwise calls 0x430e1e8, wraps and caches ArcWebContents when native optional is present, nil otherwise. | 0x430e1e8 | metadata `0xd77b2a0`; dobjc170.txt:2511 | medium-partial; available; partial_review |
| (base) | `-currentState` | `q16@0:8` | [0x442fc60](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fc60.txt) | Requires native optional; missing/dead underlying item returns 0, otherwise native states 0..3 map to 1..4, out-of-range traps. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2ac`; dobjc170.txt:2512 | medium-local; available; local_review |
| (base) | `-remainingTimeInSeconds` | `q16@0:8` | [0x442fcd0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fcd0.txt) | Calls remaining-time status helper 0x430ddd0; success returns seconds result, failure releases status and returns -1. | 0x4b37104,0x4b37cec,0x430ddd0 | metadata `0xd77b2b8`; dobjc170.txt:2513 | medium-partial; available; partial_review |
| (base) | `-isPaused` | `B16@0:8` | [0x442fd60](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fd60.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2c4`; dobjc170.txt:2514 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-pause` | `v16@0:8` | [0x442fd80](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fd80.txt) | Requires engaged native optional; live backend dispatches vtable +0x170, dead backend does nothing. Ghidra uses invalid pause C prototype. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2d0`; dobjc170.txt:2515 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-cancel:` | `v20@0:8B16` | [0x442fdd8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fdd8.txt) | Requires engaged native optional; live backend dispatches vtable +0x180 with supplied boolean; dead backend is no-op. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2dc`; dobjc170.txt:2516 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-resume` | `v16@0:8` | [0x442fe38](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fe38.txt) | Requires engaged native optional; live backend dispatches vtable +0x178, dead backend is no-op. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2e8`; dobjc170.txt:2517 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-remove` | `v16@0:8` | [0x442fe90](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fe90.txt) | Requires engaged native optional; live backend dispatches remove operation at vtable +0x188. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b2f4`; dobjc170.txt:2518 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-isDone` | `B16@0:8` | [0x442fee8](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442fee8.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b300`; dobjc170.txt:2519 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-canResume` | `B16@0:8` | [0x442ff44](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0442ff44.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b30c`; dobjc170.txt:2520 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-download_item` | `{optional<arc::DownloadItem>=(?=c{DownloadItem={shared_ptr<arc::DownloadItem::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x440a2ac](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0440a2ac.txt) | Copies optional native shared handle from self+0x10 into aggregate return storage, increments non-null control block; source engagement is at self+0x20. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b318`; dobjc170.txt:2521 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440a40c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0440a40c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b324`; dobjc170.txt:2522 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440a4dc](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0440a4dc.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77b330`; dobjc170.txt:2523 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Item validity after native DownloadManager/profile shutdown.
- Slot association with received-byte count and native download lifetime.
- Slot association with total-byte count and unknown-total sentinel.
- Exact speed units/window and native backend identity.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Association lifetime after tab close.
- Named enum states require source/vtable confirmation.
- Unknown vs interrupted item conventions and native duration rounding.
- Pause state update timing and native callback thread.
- Confirm user_cancel meaning against source/vtable and persistence.
- Resume user flag and interrupted-download constraints.
- File deletion vs history removal and object invalidation.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcDownloadItem.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcDownloadItem.md).
