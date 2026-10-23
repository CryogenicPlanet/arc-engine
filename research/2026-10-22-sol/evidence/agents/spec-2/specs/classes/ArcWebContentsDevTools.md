# ArcWebContentsDevTools

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 24 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ccf8`, class_ro `0xe2a9570`; instanceStart 8, instanceSize 56 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_toolboxWebContents` | `@"ArcWebContents"` | 0xe2a94d8 |
| 0x10 | 16 | `_bridge` | `{shared_ptr<(anonymous namespace)::ArcWebContentsDevToolsDelegateBridge>="__ptr_"^{ArcWebContentsDevToolsDelegateBridge}"__cntrl_"…` | 0xe2a94f8 |
| 0x20 | 24 | `_nativeDevTools` | `{optional<arc::WebContents::DevTools>=""(?="__null_state_"c"__val_"{DevTools="impl_"{shared_ptr<arc::WebContents::DevTools::Impl>=…` | 0xe2a9518 |

The object retains an engaged shared native DevTools handle at +0x20 and a shared delegate bridge at +0x10 (0x4414ccc, 0x4414f60). setDelegate:nil clears the bridge; a first nonnil delegate builds a bridge with weak delegate/self references, later assignments replace the stored delegate reference (0x4414f60). Destructor releases handle/bridge and cached toolbox contents (0x4415e74).

## Lifecycle, callbacks and relations

The bridge calls webContentsDevTools:displayAs:, inspectModeDidChange:, DidClose: and NeedsFocus: (0x4415fe0, 0x441604c, 0x44160b8, 0x441610c). webContents wraps the native target; toolboxWebContents caches a wrapper only while the native toolbox is available (0x44151a0, 0x4415204). execute combines domain/name/params and converts status; backend validates JSON dictionary params, joins domain.name, sends via 0x435dd94, and retains completion by request ID (0x4415520 → 0x435b114). Existing frontend opening uses DevToolsAPI.showPanel; the no-window path calls native creation at 0xc1057c0, installs a close observer and wraps toolbox state through 0x4346100 (0x4357078). Protocol connection creation has disabled/unavailable/attach-failure branches (0x4415b60 → 0x4355df8).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

CEF provides overlapping CDP transport, method execution and message observation (CEF pin include/cef_browser.h:633, :649, :660) plus ShowDevTools (:582). This is a source-backed partial route, not native toolbox embedding equivalence. CEF macOS parent_view forces Alloy (include/internal/cef_types_mac.h:151); Arc toolbox/native pane behavior remains a route gate.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebContentsDevTools.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init: | `@32@0:8{DevTools={shared_ptr<arc::WebContents::DevTools::Impl>=^{Impl}^{__shared_weak_count}}}16` | 0x4414ccc | Initializes object/native state after NSObject init; conversion/helper calls and assignment branches are preserved in exact body. | local state / indirect operation; see body | IMP 0x4414ccc; record 0xd77a428; dyld L2167; otool L28541 | medium / available / local_observation |
| base | - isEqual: | `B24@0:8@16` | 0x4414df0 | Checks same-object identity, compatible ObjC class, then compares unwrapped native handle pointers; no content equality established. | local state / indirect operation; see body | IMP 0x4414df0; record 0xd77a434; dyld L2168; otool L28544 | medium / available / bounded_review |
| base | - hash | `Q16@0:8` | 0x43fcba0 | Requires engaged native handle and hashes its pointer through 0x18f3c. | 0x00018f3c, 0x04b08a00 | IMP 0x43fcba0; record 0xd77a440; dyld L2169; otool L24086 | medium / available / bounded_review |
| base | - setDelegate: | `v24@0:8@16` | 0x4414f60 | Nil clears shared bridge; existing bridge updates delegate; first bridge uses objc_initWeak for delegate/self and registers through 0x4332f08. | 0x04332f08 | IMP 0x4414f60; record 0xd77a44c; dyld L2170; otool L28550 | medium / available / bounded_review |
| base | - open | `v16@0:8` | 0x4415178 | Calls 0x4356fdc with native handle; that helper opens with panel string elements. | 0x04356fdc | IMP 0x4415178; record 0xd77a458; dyld L2171; otool L28553 | medium / available / bounded_review |
| base | - close | `v16@0:8` | 0x441518c | Requires engaged handle and calls native close bridge 0x4357fd0. | 0x04357fd0 | IMP 0x441518c; record 0xd77a464; dyld L2172; otool L28556 | medium / available / bounded_review |
| base | - webContents | `@16@0:8` | 0x44151a0 | Requires engaged native handle; creates ArcWebContents wrapper for target native state. | local state / indirect operation; see body | IMP 0x44151a0; record 0xd77a470; dyld L2173; otool L28559 | medium / available / bounded_review |
| base | - toolboxWebContents | `@16@0:8` | 0x4415204 | Returns nil when native toolbox absent; otherwise validates cached wrapper native handle and creates/reuses ArcWebContents wrapper. | local state / indirect operation; see body | IMP 0x4415204; record 0xd77a47c; dyld L2174; otool L28562 | medium / available / bounded_review |
| base | - inspectElementAt: | `v32@0:8{CGPoint=dd}16` | 0x4415370 | Forwards supplied point to 0x43584f8, which checks live target/frame state and invokes an indirect inspection operation. | 0x043584f8 | IMP 0x4415370; record 0xd77a488; dyld L2175; otool L28565 | medium / available / bounded_review |
| base | - openInspectView | `v16@0:8` | 0x441538c | Calls 0x4357738, opens elements panel if needed and schedules a native inspect action. | 0x04357738 | IMP 0x441538c; record 0xd77a494; dyld L2176; otool L28568 | medium / available / bounded_review |
| base | - openConsoleView | `v16@0:8` | 0x44153a0 | Calls 0x4357aa0, which forwards console panel string to 0x4357078. | 0x04357aa0 | IMP 0x44153a0; record 0xd77a4a0; dyld L2177; otool L28571 | medium / available / bounded_review |
| base | - openNetworkView | `v16@0:8` | 0x44153b4 | Calls 0x4357b40, which forwards network panel string to 0x4357078. | 0x04357b40 | IMP 0x44153b4; record 0xd77a4ac; dyld L2178; otool L28574 | medium / available / bounded_review |
| base | - currentPanel:error: | `B32@0:8@?16^@24` | 0x44153c8 | Captures result callback, invokes 0x4357be0 and converts immediate status; backend rejects unopened window and requests DevToolsAPI.currentPanel. | 0x04357be0, 0x04402548, 0x04b35898 | IMP 0x44153c8; record 0xd77a4b8; dyld L2179; otool L28577 | medium / available / bounded_review |
| base | - execute:completion:error: | `B40@0:8@16@?24^@32` | 0x4415520 | Extracts command domain/name/params, leaves native params string empty when absent, captures completion; 0x435b114 validates dictionary params, sends domain.name through 0x435dd94 and stores callback by request ID. | 0x01b356b8, 0x0435b114, 0x04402548, 0x04b35898, 0x04052260 | IMP 0x4415520; record 0xd77a4c4; dyld L2180; otool L28580 | medium / available / bounded_review |
| base | - subscribe:autoEnable:notification: | `@36@0:8@16B24@?28` | 0x4415838 | Extracts event domain/name, captures notification and autoEnable Boolean, delegates registration to 0x435b784; boxes subscription ID. | 0x01b356b8, 0x0435b784, 0x04b35898 | IMP 0x4415838; record 0xd77a4d0; dyld L2181; otool L28583 | medium / available / bounded_review |
| base | - unsubscribe: | `B24@0:8@16` | 0x4415ad8 | Unboxes intValue subscription ID and calls 0x435be6c; returns success status Boolean. | 0x0435be6c, 0x04b35898 | IMP 0x4415ad8; record 0xd77a4dc; dyld L2182; otool L28586 | medium / available / bounded_review |
| base | - openProtocolConnection:error: | `@32@0:8@?16^@24` | 0x4415b60 | Captures notification sink; 0x4355df8 can reject disabled/unavailable/attach-failed state; boxes connection ID and converts error. | 0x04355df8, 0x04402548, 0x04b35898, 0x04b37e18 | IMP 0x4415b60; record 0xd77a4e8; dyld L2183; otool L28589 | medium / available / bounded_review |
| base | - sendProtocolMessage:connectionID:error: | `B40@0:8@16@24^@32` | 0x4415cf8 | Converts message to native string, unboxes connectionID intValue, invokes 0x43564d8 and converts status to BOOL/NSError. | 0x01b356b8, 0x043564d8, 0x04402548, 0x04b35898 | IMP 0x4415cf8; record 0xd77a4f4; dyld L2184; otool L28592 | medium / available / bounded_review |
| base | - closeProtocolConnection: | `v24@0:8@16` | 0x4415e20 | Requires engaged handle; unboxes intValue ID and forwards close to 0x4356754. | 0x04356754 | IMP 0x4415e20; record 0xd77a500; dyld L2185; otool L28595 | medium / available / bounded_review |
| base | - nativeDevTools | `{optional<arc::WebContents::DevTools>=(?=c{DevTools={shared_ptr<arc::WebContents::DevTools::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43fe184 | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x30, 0x28, 0x20. Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x43fe184; record 0xd77a50c; dyld L2186; otool L28598 | medium / available / local_observation |
| base | - bridge | `{shared_ptr<(anonymous namespace)::ArcWebContentsDevToolsDelegateBridge>=^{ArcWebContentsDevToolsDelegateBridge}^{__shared_weak_count}}16@0:8` | 0x43fe224 | Local field operation: plVar1 = (long *)(*(long *)(param_1 + 0x18) + 8);; return sVar2; | local state / indirect operation; see body | IMP 0x43fe224; record 0xd77a518; dyld L2187; otool L28601 | medium / available / local_verified |
| base | - setBridge: | `v32@0:8{shared_ptr<(anonymous namespace)::ArcWebContentsDevToolsDelegateBridge>=^{ArcWebContentsDevToolsDelegateBridge}^{__shared_weak_count}}16` | 0x43fe244 | Assigns/replaces local native or ObjC state; self offsets in exact body. Calls local state / indirect operation; see body. | local state / indirect operation; see body | IMP 0x43fe244; record 0xd77a524; dyld L2188; otool L28604 | medium / available / local_observation |
| base | - .cxx_destruct | `v16@0:8` | 0x4415e74 | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x30, 0x28, 0x18, 0x8. | local state / indirect operation; see body | IMP 0x4415e74; record 0xd77a530; dyld L2189; otool L28607 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x43fe528 | Initializes native storage/engagement fields at self offsets 0x30, 0x10, 0x18, 0x20; returns self. | local state / indirect operation; see body | IMP 0x43fe528; record 0xd77a53c; dyld L2190; otool L24152 | medium / available / local_observation |

## Remaining consequential questions

Verify native toolbox view ownership/display modes and callback queue; recover subscriptions, autoEnable reference counts, connection IDs and close/error ordering. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
