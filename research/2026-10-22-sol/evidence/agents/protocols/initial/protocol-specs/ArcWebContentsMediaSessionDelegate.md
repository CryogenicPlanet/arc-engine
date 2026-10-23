# ArcWebContentsMediaSessionDelegate

Media session state/actions/position, device state and native PiP UI. Classification: `media_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b2178`; inherits `NSObject`. Pristine binary corroborates all 20 unique guide identities (25 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK18WebContentChromium`: class `0x10447e518`, class_ro `0x10447e4b0`, direct protocols `ArcWebContentsTopBandColorDelegate;ArcWebContentsThumbnailsGeneratorObserver;ArcWebContentsUIDelegate;ArcWebContentsNavigationObserver;ArcWebContentsNavigationDelegate;ArcWebContentsMediaSessionDelegate;ArcWebContentsDevToolsDelegate;ArcWebContentsBoostIDEDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Bridge `0x441c08c` allocates `ArcMediaPosition`, initializes it from native position state, loads weak target/owner and sends `webContents:didChangeMediaPosition:`. The bounded instruction/body evidence corroborates this local conversion, not position time units or update ordering.

The matrix preserves integer session/camera/microphone states, BOOL feature flags, media actions, native PiP sizes, native controller objects and title/icon values as distinct types. Native PiP view parenting, unavailable-session behavior and callback ordering need the media class findings plus runtime validation; no universal main-thread rule follows.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)webContentsDidClickPipBackToTabButton:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x10053513c` (entry `0x1035ff298`) | `0x441b5c8` |
| required | `- (void)webContentsDidClickPipCloseButton:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x1005351e4` (entry `0x1035ff2a4`) | `0x441b61c` |
| required | `- (void)webContentsDidClickPipSettingsButton:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x10053528c` (entry `0x1035ff2b0`) | `0x441b670` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeVolume:(double)arg1;` | `v32@0:8@16d24` | `0x1005355d4` (entry `0x1035ff2ec`) | `0x441bf3c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeMediaSessionState:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x100535334` (entry `0x1035ff2bc`) | `0x441bfa8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeAvailableMediaActions:(NSIndexSet *)arg1;` | `v32@0:8@16@24` | `0x100535388` (entry `0x1035ff2c8`) | `0x441c05c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeMediaPosition:(ArcMediaPosition *)arg1;` | `v32@0:8@16@24` | `0x100535040` (entry `0x1035ff280`) | `0x441c0f0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeCameraState:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x100535444` (entry `0x1035ff2d4`) | `0x441c164` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didChangeMicrophoneState:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x10053550c` (entry `0x1035ff2e0`) | `0x441c1d4` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipControllerCreated:(ArcWebContentsMediaSessionPiPController *)arg1;` | `v32@0:8@16@24` | `0x100534aa0` (entry `0x1035ff208`) | `0x441b6f0` |
| required | `- (void)webContentsPiPControllerDestroyed:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100534b64` (entry `0x1035ff214`) | `0x441b750` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipVideoResolutionChanged:(CGSize)arg1;` | `v40@0:8@16{CGSize=dd}24` | `0x100534c0c` (entry `0x1035ff220`) | `0x441b7b8` |
| required | `- (void)webContentsShowPiP:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100534ccc` (entry `0x1035ff22c`) | `0x441b810` |
| required | `- (void)webContentsHidePiP:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100534d74` (entry `0x1035ff238`) | `0x441b864` |
| required | `- (void)webContentsClosePiP:(ArcWebContents *)arg0;` | `v24@0:8@16` | `0x100534e1c` (entry `0x1035ff244`) | `0x441b8b8` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipPlaybackStateChanged:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x100534ec4` (entry `0x1035ff250`) | `0x441b920` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipFeatureStateChanged:(uint64_t)arg1 turnedOn:(BOOL)arg2;` | `v36@0:8@16Q24B32` | `0x100534f18` (entry `0x1035ff25c`) | `0x441b998` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipMediaPositionChanged:(ArcMediaPosition *)arg1;` | `v32@0:8@16@24` | `0x100534f78` (entry `0x1035ff268`) | `0x441ba20` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipSourceTitleChanged:(NSString *)arg1;` | `v32@0:8@16@24` | `0x10053503c` (entry `0x1035ff274`) | `0x441bab0` |
| required | `- (void)webContents:(ArcWebContents *)arg0 pipFaviconImagesChanged:(NSArray *)arg1;` | `v32@0:8@16@24` | `0x100535138` (entry `0x1035ff28c`) | `0x441bb80` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
