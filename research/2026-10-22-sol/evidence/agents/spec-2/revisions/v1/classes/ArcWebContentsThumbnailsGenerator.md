# ArcWebContentsThumbnailsGenerator

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ce88`, class_ro `0xe2a9b60`; instanceStart 8, instanceSize 64 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_owner` | `@"ArcWebContents"` | 0xe2a9ac8 |
| 0x10 | 24 | `_thumbnailsGenerator` | `{optional<arc::WebContents::ThumbnailsGenerator>=""(?="__null_state_"c"__val_"{ThumbnailsGenerator="impl_"{shared_ptr<arc::WebCont…` | 0xe2a9ae8 |
| 0x28 | 24 | `_observers` | `{vector<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>, std::allocator<std::shared_ptr<(anon…` | 0xe2a9b08 |

The wrapper has a weak owner, optional shared native generator and vector of shared observer bridges. init stores handle/weak owner, and destruction releases observers/handle and destroys the weak owner (0x441c3c0, 0x44191d8). addObserver: deduplicates by observer identity, makes weak observer/owner references, registers the bridge natively and stores it in the vector (0x441c680 → 0x436d904).

## Lifecycle, callbacks and relations

Constructor recovery: Ghidra renders the owner as part of the handle aggregate and leaves param_4 unused. The encoding declares a 16-byte native handle and a separate ObjC owner. Bounded raw instructions save incoming x2 as a handle address and x3 as owner; saved x2 is dereferenced for handle words and saved x3 feeds the weak-store call. Keep this local register mapping distinct from a full caller-side C++ ABI/ownership guarantee.

removeObserver: unregisters/removes its matching bridge (0x441c9e4 → 0x436de50). Callback 0x441cccc emits webContents:thumbnailReady: with a converted image or nil. requestThumbnail: converts backend status; 0x436df2c checks managed contents, observer/capture state and ThumbnailTabHelper availability. 0x436d69c installs a callback into thumbnail state.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome ThumbnailTabHelper identity is supported by kDataKeyImpl in native calls 0x436df2c/0x436d69c and source chrome/browser/ui/thumbnails/thumbnail_tab_helper.h:27, :52; capture methods at .cc:242/:356 are source-backed candidate routes. CEF capture is not shown equivalent to this observer-driven Chrome helper lifecycle.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebContentsThumbnailsGenerator.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - init:forWebContents: | `@40@0:8{ThumbnailsGenerator={shared_ptr<arc::WebContents::ThumbnailsGenerator::Impl>=^{Impl}^{__shared_weak_count}}}16@32` | 0x441c3c0 | Copies native shared-handle state and stores a weak owner after NSObject init. Ghidra signature/argument-propagation artifact: unused param_4 and owner rendered from the aggregate. Raw entry saves x2 as the handle address and x3 as owner; handle words are loaded through saved x2 and saved x3 feeds the weak-store call. This is local register evidence, not a complete C++ ABI guarantee. | local state / indirect operation; see body | IMP 0x441c3c0; record 0xd77a894; dyld L2268; otool L29122 | medium / available_signature_artifact / local_observation |
| base | - isEqual: | `B24@0:8@16` | 0x441c510 | Checks same-object identity, compatible ObjC class, then compares unwrapped native handle pointers; no content equality established. | local state / indirect operation; see body | IMP 0x441c510; record 0xd77a8a0; dyld L2269; otool L29125 | medium / available / bounded_review |
| base | - hash | `Q16@0:8` | 0x4416b58 | Requires engaged native handle and hashes its pointer through 0x18f3c. | 0x00018f3c, 0x04b08a00 | IMP 0x4416b58; record 0xd77a8ac; dyld L2270; otool L28681 | medium / available / bounded_review |
| base | - addObserver: | `v24@0:8@16` | 0x441c680 | Deduplicates observer identity, constructs shared bridge with weak observer/owner, registers via 0x436d904 and appends bridge to vector. | 0x0436d904, 0x040416c0, 0x04b087f0 | IMP 0x441c680; record 0xd77a8b8; dyld L2271; otool L29131 | medium / available / bounded_review |
| base | - removeObserver: | `v24@0:8@16` | 0x441c9e4 | Finds observer bridge by identity, invokes native unregister helper 0x436de50 and removes/releases matching vector entry. | 0x0436de50 | IMP 0x441c9e4; record 0xd77a8c4; dyld L2272; otool L29134 | medium / available / bounded_review |
| base | - requestThumbnail: | `B24@0:8^@16` | 0x441cc58 | Requires engaged handle; 0x436df2c checks contents/helper/capture state, optionally requests capture and converts result to BOOL/NSError. | 0x0436df2c, 0x04402548, 0x04b35898 | IMP 0x441cc58; record 0xd77a8d0; dyld L2273; otool L29137 | medium / available / bounded_review |
| base | - thumbnailsGenerator | `{optional<arc::WebContents::ThumbnailsGenerator>=(?=c{ThumbnailsGenerator={shared_ptr<arc::WebContents::ThumbnailsGenerator::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x440a2ac | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x20, 0x18, 0x10. Engagement guard/trap is visible. | local state / indirect operation; see body | IMP 0x440a2ac; record 0xd77a8dc; dyld L2274; otool L29140 | medium / available / local_observation |
| base | - observers | `{vector<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>, std::allocator<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>>>={__vector_layout<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>, std::allocator<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>>>=^vQQ}}16@0:8` | 0x4418c3c | Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets 0x30, 0x28. Engagement guard/trap is visible. | 0x040416c0 | IMP 0x4418c3c; record 0xd77a8e8; dyld L2275; otool L29143 | medium / available / local_observation |
| base | - setObservers: | `v40@0:8{vector<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>, std::allocator<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>>>={__vector_layout<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>, std::allocator<std::shared_ptr<(anonymous namespace)::WebContentsThumbnailsGeneratorObserverBridge>>>=^vQQ}}16` | 0x4418ce4 | Assigns/replaces local native or ObjC state; self offsets 0x28, 0x38, 0x30. Calls 0x040416c0. Engagement guard/trap is visible. | 0x040416c0 | IMP 0x4418ce4; record 0xd77a8f4; dyld L2276; otool L29146 | medium / available / local_observation |
| base | - owner | `@16@0:8` | 0x43fe200 | Local field operation: return *(ID *)(param_1 + 8); | local state / indirect operation; see body | IMP 0x43fe200; record 0xd77a900; dyld L2277; otool L24137 | medium / available / local_verified |
| base | - setOwner: | `v24@0:8@16` | 0x43fe218 | Local field operation: *(ID *)(param_1 + 8) = param_3; | local state / indirect operation; see body | IMP 0x43fe218; record 0xd77a90c; dyld L2278; otool L24140 | medium / available / local_verified |
| base | - .cxx_destruct | `v16@0:8` | 0x44191d8 | Releases shared control-block state and invokes terminal virtual destruction when counts reach zero; self offsets 0x28, 0x30, 0x38, 0x20, 0x18, 0x8. | local state / indirect operation; see body | IMP 0x44191d8; record 0xd77a918; dyld L2279; otool L28816 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x44193ac | Initializes native storage/engagement fields at self offsets 0x10, 0x20, 0x30, 0x38, 0x28; returns self. | local state / indirect operation; see body | IMP 0x44193ac; record 0xd77a924; dyld L2280; otool L28819 | medium / available / local_observation |

## Remaining consequential questions

Recover image scale/crop/format and callback timing, no-observer request behavior, stale-image handling and visibility/capture policy. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 11; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
