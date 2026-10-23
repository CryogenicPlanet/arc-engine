# ArcWebContentsBoostIDE

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 48 bytes; class metadata `0xe37cc58`, class_ro `0xe2a92c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_boostIDEDelegateBridge` | 8 | 16 | `{shared_ptr<BoostIDEDelegateBridge>="__ptr_"^{BoostIDEDelegateBridge}"__cntrl_"^{__shared_weak_count}}` |
| `_nativeBoostIDE` | 24 | 24 | `{optional<arc::WebContents::BoostIDE>=""(?="__null_state_"c"__val_"{BoostIDE="impl_"{shared_ptr<arc::WebContents::BoostI… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native handle at +0x18; `0x4414144` copies it and `0x44149dc` releases it and delegate bridge. `0x4414268` creates bridge with weak self but delegate is stored through slot assignment; ownership of that delegate slot is unresolved and not inferred as weak. Existing bridge updated, nil drops stored bridge.

## Threading and caller/callee relations

Native callbacks `0x4413fd8`, `0x441402c`, `0x4414080` send boostIDEDidOpen:, boostIDEWillClose: and boostIDE:didRequestExecuteCommand:jsonString:. sendResponse `0x4414488` → `0x43d35c8` → `0x68122a8` calls cr.webUIResponse with true; error `0x44145b0` → `0x43d38cc` → `0x6812404` sends false; event `0x44146d8` → `0x8371aa8` sends cr.webUIListenerCallback. The native outgoing paths construct string-valued payloads through `0xc87c40`; they do not show JSON parsing. Parsing or other interpretation by receiving JavaScript remains untraced. Callback queue and JavaScript lifecycle remain unresolved. See [revision evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/revision-evidence/analysis.md).

## Chrome / CEF route implications

Strong source-backed WebUI identification: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `content/public/browser/web_ui_message_handler.h:76`/:85/:91 identifies resolve/reject/listener contracts; binary strings and Boolean flag differentiate responses. CEF shim would need compatible trusted page/JS bridge, command dispatch and JS-allowed lifetime, rather than treating this as an arbitrary network/CDP request.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithBoostIDE:` | `@24@0:8r^v16` | [0x4414144](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414144.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77a268`; D 2127 | M; available; local_body_observed |
| (base) | `-setDelegate:` | `v24@0:8@16` | [0x4414268](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414268.c) | Creates shared BoostIDE bridge with weak self and a delegate slot, updates existing bridge or drops it for nil; installs through 0x4345824. | 0x4345824 | M `0xd77a274`; D 2128 | M; available; bridge_reviewed_partial |
| (base) | `-sendResponse:jsonString:` | `v32@0:8@16@24` | [0x4414488](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414488.c) | Converts command/request ID and payload bytes (nil to empty), checks native handler and forwards to 0x43d35c8. For a matching pending ID, string-valued payload construction through 0xc87c40 precedes 0x68122a8 and cr.webUIResponse true. | 0x1b356b8; 0x43d35c8 | M `0xd77a280`; D 2129 | M; available; bridge_reviewed_partial |
| (base) | `-sendErrorResponse:jsonString:` | `v32@0:8@16@24` | [0x44145b0](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44145b0.c) | Converts ID/payload bytes and forwards to 0x43d38cc when native handler is live. For a matching pending ID, string-valued payload construction through 0xc87c40 precedes 0x6812404 and cr.webUIResponse false. | 0x1b356b8; 0x43d38cc | M `0xd77a28c`; D 2130 | M; available; bridge_reviewed_partial |
| (base) | `-sendEvent:jsonString:` | `v32@0:8@16@24` | [0x44146d8](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44146d8.c) | Converts event name/payload bytes and forwards to 0x8371aa8; 0xc87c40 constructs string-valued arguments for cr.webUIListenerCallback. No JSON parsing appears in this native path. | 0x1b356b8; 0x8371aa8 | M `0xd77a298`; D 2131 | M; available; bridge_reviewed_partial |
| (base) | `-nativeBoostIDE` | `{optional<arc::WebContents::BoostIDE>=(?=c{BoostIDE={shared_ptr<arc::WebContents::BoostIDE::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x4414800](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414800.c) | Copies optional native shared handle from self+0x18 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77a2a4`; D 2132 | H; available; local_field_observed |
| (base) | `-boostIDEDelegateBridge` | `{shared_ptr<BoostIDEDelegateBridge>=^{BoostIDEDelegateBridge}^{__shared_weak_count}}16@0:8` | [0x4414840](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414840.c) | Returns copy of shared bridge/native handle and increments its control count; exact field offset is in the body and layout. | field/indirect dispatch; see body | M `0xd77a2b0`; D 2133 | H; available; local_field_observed |
| (base) | `-setBoostIDEDelegateBridge:` | `v32@0:8{shared_ptr<BoostIDEDelegateBridge>=^{BoostIDEDelegateBridge}^{__shared_weak_count}}16` | [0x4414860](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414860.c) | Assigns shared native bridge handle, incrementing new control count and releasing previous shared owner; identity is defined by metadata, despite shared Ghidra name. | field/indirect dispatch; see body | M `0xd77a2bc`; D 2134 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x44149dc](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/44149dc.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77a2c8`; D 2135 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4414b50](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/4414b50.c) | Initializes native storage with explicit stores: self+0x28=0; self+8=0; self+0x10=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77a2d4`; D 2136 | M; available; local_body_observed |

## Remaining consequential questions

Recover delegate-slot ownership from ARC/disassembly, native handler availability/JS-allowed conditions, receiving JavaScript interpretation of string payloads and command ID format. Verify closing while app command is in flight.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcWebContentsBoostIDE.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcWebContentsBoostIDE.md).
