# ArcBrowserSidePanel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 13 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37c118`, class_ro `0xe2a5f70`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_nativeSidePanel` | 8 | 16 | `{SidePanel="impl_"{shared_ptr<arc::SidePanel::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}}` |
| `_sidePanelDelegate` | 24 | 16 | `{shared_ptr<(anonymous namespace)::SidePanelDelegateBridge>="__ptr_"^{SidePanelDelegateBridge}"__cntrl_"^{__shared_weak_count}}` |

## Lifecycle, ownership and callbacks

Initializer `0x44013f0` retains native shared panel; delegate shared bridge is at +0x18 and destructor `0x4401808` releases both. `0x44011d8` initializes weak delegate/self references; nil drops wrapper bridge. Title getter `0x44014fc` converts native title and `0x4401520` returns nil or a newly constructed ArcWebContents wrapper.

## Threading and caller/callee relations

Closing uses two distinct calls: `0x4401170` requests closure through `0x43c3588`; `0x44011a4` informs native panel after app closure via `0x43c1fe8`. Callback adapters `0x4401984`, `0x4401a10`, `0x4401aa0`, `0x4401b04` emit didShow, didUpdateTitle, willClose:withAnimation: and willUnhide. Session query `0x4401570` → `0x4346bd8` yields active-entry flag, extension ID and optional cached contents. Native liveness checks are visible; callback queue, animation ownership and reparenting are unresolved.

## Chrome / CEF route implications

Requires native WebContents sharing, per-tab entry/cache identity and extension-associated panel state. Chrome analogue is Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/browser/ui/side_panel/side_panel_registry.h:25` (candidate service, not proven function identification). CEF route must reproduce this panel lifecycle in addition to native view hosting; a generic child browser alone does not establish equivalence.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-requestClose` | `v16@0:8` | [0x4401170](../bodies/4401170.c) | Checks native side-panel/browser liveness and calls 0x43c3588 to request closure. | 0x31dab4; 0x43c3588 | M `0xd778cf0`; D 1662 | M; available; bridge_reviewed_partial |
| (base) | `-didFinishClosing` | `v16@0:8` | [0x44011a4](../bodies/44011a4.c) | Checks same native side-panel/browser liveness and calls 0x43c1fe8 after app closing completes. | 0x31dab4; 0x43c1fe8 | M `0xd778cfc`; D 1663 | M; available; bridge_reviewed_partial |
| (base) | `-setDelegate:` | `v24@0:8@16` | [0x44011d8](../bodies/44011d8.c) | Nil drops the shared delegate bridge; otherwise creates bridge with weak delegate/self, updates existing delegate or installs new native bridge through 0x43417bc. | 0x43417bc | M `0xd778d08`; D 1664 | M; available; bridge_reviewed_partial |
| (base) | `-initWithSidePanel:` | `@24@0:8r^v16` | [0x44013f0](../bodies/44013f0.c) | Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body. | field/indirect dispatch; see body | M `0xd778d14`; D 1665 | M; available; local_body_observed |
| (base) | `-title` | `@16@0:8` | [0x44014fc](../bodies/44014fc.c) | Converts native title from side-panel Impl to NSString. | 0x1d9fb90 | M `0xd778d20`; D 1666 | M; available; bridge_reviewed_partial |
| (base) | `-webContents` | `@16@0:8` | [0x4401520](../bodies/4401520.c) | Returns nil if Impl+0x60 is absent; otherwise constructs a new ArcWebContents wrapper for that native handle. | field/indirect dispatch; see body | M `0xd778d2c`; D 1667 | M; available; bridge_reviewed_partial |
| (base) | `-getEntryInfoForTabWithSessionId:` | `@24@0:8q16` | [0x4401570](../bodies/4401570.c) | Creates ArcBrowserSidePanelEntryInfo and fills active-entry flag, extension ID and optional cached ArcWebContents from 0x4346bd8 for the supplied session ID. | 0x1fee4; 0x1c391bc; 0x4346bd8 | M `0xd778d38`; D 1668 | M; available; bridge_reviewed_partial |
| (base) | `-nativeSidePanel` | `{SidePanel={shared_ptr<arc::SidePanel::Impl>=^{Impl}^{__shared_weak_count}}}16@0:8` | [0x4401710](../bodies/4401710.c) | Copies native side-panel shared handle from self+8 into native return storage, incrementing its control count. Shared Ghidra name HistoryQueryOptions is not this metadata identity. | field/indirect dispatch; see body | M `0xd778d44`; D 1669 | M; available; bridge_reviewed_partial |
| (base) | `-setNativeSidePanel:` | `v32@0:8{SidePanel={shared_ptr<arc::SidePanel::Impl>=^{Impl}^{__shared_weak_count}}}16` | [0x4401730](../bodies/4401730.c) | Assigns new native shared side-panel handle at self+8, retaining new control owner and releasing previous control block. | field/indirect dispatch; see body | M `0xd778d50`; D 1670 | M; available; bridge_reviewed_partial |
| (base) | `-sidePanelDelegate` | `{shared_ptr<(anonymous namespace)::SidePanelDelegateBridge>=^{SidePanelDelegateBridge}^{__shared_weak_count}}16@0:8` | [0x43e3070](../bodies/43e3070.c) | Returns copy of shared bridge/native handle and increments its control count; exact field offset is in the body and layout. | field/indirect dispatch; see body | M `0xd778d5c`; D 1671 | H; available; local_field_observed |
| (base) | `-setSidePanelDelegate:` | `v32@0:8{shared_ptr<(anonymous namespace)::SidePanelDelegateBridge>=^{SidePanelDelegateBridge}^{__shared_weak_count}}16` | [0x43e3090](../bodies/43e3090.c) | Assigns shared native bridge handle, incrementing new control count and releasing previous shared owner; identity is defined by metadata, despite shared Ghidra name. | field/indirect dispatch; see body | M `0xd778d68`; D 1672 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4401808](../bodies/4401808.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd778d74`; D 1673 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4401974](../bodies/4401974.c) | Initializes native storage with explicit stores: self+0x20=0; self+0x18=0; self+0x10=0; self+8=0. | field/indirect dispatch; see body | M `0xd778d80`; D 1674 | M; available; local_body_observed |

## Remaining consequential questions

Do callbacks remain possible after setDelegate:nil? Who calls didFinishClosing and when can cached contents be reattached? Recover session ID mapping, native view contract and destruction during animation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
