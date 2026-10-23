# ArcWebContentsNavigationObserver

Provisional/committed/in-page navigation and failure notifications. Classification: `navigation_observer`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b2310`; inherits `NSObject`. Pristine binary corroborates all 8 unique guide identities (11 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK18WebContentChromium`: class `0x10447e518`, class_ro `0x10447e4b0`, direct protocols `ArcWebContentsTopBandColorDelegate;ArcWebContentsThumbnailsGeneratorObserver;ArcWebContentsUIDelegate;ArcWebContentsNavigationObserver;ArcWebContentsNavigationDelegate;ArcWebContentsMediaSessionDelegate;ArcWebContentsDevToolsDelegate;ArcWebContentsBoostIDEDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

The navigation observer has eight unique methods, separate from the synchronous navigation-policy delegate. The binary/source send evidence identifies distinct provisional/commit/finish/failure and in-page selectors; it does not establish their full ordering or exactly-once behavior.

The completed class review shows add/remove at `0x4416eac/0x4417210` deduplicate and unregister weak bridges. Weak loads and local sends cannot establish suppression of already queued work after removal or observer destruction.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)webContents:(ArcWebContents *)arg0 didStartProvisionalNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x1005359f8` (entry `0x1035ff19c`) | `0x441945c` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didCommitNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535a60` (entry `0x1035ff1a8`) | `0x4419650` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didFinishNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535b74` (entry `0x1035ff1b4`) | `0x4419844` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didReceiveServerRedirectForProvisionalNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535bdc` (entry `0x1035ff1c0`) | Uncorroborated in bounded scan |
| required | `- (void)webContents:(ArcWebContents *)arg0 didFailNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535c54` (entry `0x1035ff1cc`) | `0x4419a38` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didFailProvisionalNavigation:(ArcNavigation *)arg1 withError:(NSError *)arg2;` | `v40@0:8@16@24@32` | `0x100535c64` (entry `0x1035ff1d8`) | `0x4419c78` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didStartInPageNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535d60` (entry `0x1035ff1e4`) | `0x4419e78` |
| required | `- (void)webContents:(ArcWebContents *)arg0 didFinishInPageNavigation:(ArcNavigation *)arg1;` | `v32@0:8@16@24` | `0x100535e50` (entry `0x1035ff1f0`) | `0x441a06c` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
