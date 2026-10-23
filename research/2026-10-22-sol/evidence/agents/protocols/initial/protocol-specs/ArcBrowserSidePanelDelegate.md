# ArcBrowserSidePanelDelegate

Side-panel visibility, title, close animation and unhide requests. Classification: `native_ui_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b1c40`; inherits `NSObject`. Pristine binary corroborates all 4 unique guide identities (4 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK15ChromiumBrowser`: class `0x10447de98`, class_ro `0x10447de38`, direct protocols `ArcBrowserDelegate;ArcBrowserSidePanelDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

The four methods are app UI requests/notifications, including BOOL animation control. The completed side-panel class review distinguishes close request `0x4401170` from app completion `0x44011a4`; these methods cannot be reduced to a single synchronous close operation.

Native panel view ownership, which completion ends the close transition, focus transfer and AppKit thread/reentrancy constraints remain unverified.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)sidePanel:(ArcBrowserSidePanel *)arg0 didShow:(ArcWebContents *)arg1;` | `v32@0:8@16@24` | `0x100529750` (entry `0x1035fee3c`) | `0x44019e8` |
| required | `- (void)sidePanel:(ArcBrowserSidePanel *)arg0 didUpdateTitle:(NSString *)arg1;` | `v32@0:8@16@24` | `0x1005298b8` (entry `0x1035fee48`) | `0x4401a78` |
| required | `- (void)sidePanelWillClose:(ArcBrowserSidePanel *)arg0 withAnimation:(BOOL)arg1;` | `v28@0:8@16B24` | `0x10052949c` (entry `0x1035fee24`) | `0x4401ae4` |
| required | `- (void)sidePanelWillUnhide:(ArcBrowserSidePanel *)arg0;` | `v24@0:8@16` | `0x100529600` (entry `0x1035fee30`) | `0x4401b3c` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
