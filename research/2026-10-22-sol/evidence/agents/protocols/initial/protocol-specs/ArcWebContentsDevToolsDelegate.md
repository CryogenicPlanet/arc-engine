# ArcWebContentsDevToolsDelegate

DevTools frontend display, close, focus, confirmation and inspect mode. Classification: `devtools_ui_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b20e0`; inherits `NSObject`. Pristine binary corroborates all 5 unique guide identities (5 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK18WebContentChromium`: class `0x10447e518`, class_ro `0x10447e4b0`, direct protocols `ArcWebContentsTopBandColorDelegate;ArcWebContentsThumbnailsGeneratorObserver;ArcWebContentsUIDelegate;ArcWebContentsNavigationObserver;ArcWebContentsNavigationDelegate;ArcWebContentsMediaSessionDelegate;ArcWebContentsDevToolsDelegate;ArcWebContentsBoostIDEDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

This is the DevTools frontend/UI callback interface, distinct from the command/event values and raw protocol connection blocks. Short bridges `0x4415fe0`, `0x441604c`, `0x44160b8`, `0x441610c` cover display, inspect change, close and focus. Weak loads are visible in the actual bridge instructions/source candidates.

The separate raw transport API `0x4415b60` has a retained callback adapter at headerless `0x4416690`: the completed ABI report corroborates message-or-error delivery. That block signature, 8 MiB transport limit and queued close notifications must not be attributed to these five protocol methods. Scheduling identity and frontend view ownership remain unresolved.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)webContentsDevTools:(ArcWebContentsDevTools *)arg0 displayAs:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x100534748` (entry `0x1035ff304`) | `0x441602c` |
| required | `- (void)webContentsDevToolsDidClose:(ArcWebContentsDevTools *)arg0;` | `v24@0:8@16` | `0x1005348ac` (entry `0x1035ff31c`) | `0x44160f0` |
| required | `- (void)webContentsDevToolsNeedsFocus:(ArcWebContentsDevTools *)arg0;` | `v24@0:8@16` | `0x100534804` (entry `0x1035ff310`) | `0x4416144` |
| required | `- (void)webContentsDevTools:(ArcWebContentsDevTools *)arg0 needsConfirmation:(NSString *)arg1 completion:(void (^)(BOOL))arg2;` | `v40@0:8@16@24@?32` | `0x100534954` (entry `0x1035ff328`) | `0x441622c` |
| required | `- (void)webContentsDevTools:(ArcWebContentsDevTools *)arg0 inspectModeDidChange:(uint64_t)arg1;` | `v32@0:8@16Q24` | `0x10053468c` (entry `0x1035ff2f8`) | `0x4416098` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
