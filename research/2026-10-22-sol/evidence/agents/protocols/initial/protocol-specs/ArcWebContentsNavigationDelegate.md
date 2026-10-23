# ArcWebContentsNavigationDelegate

App returns navigation disposition immediately. Classification: `synchronous_policy_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b22b0`; inherits `NSObject`. Pristine binary corroborates all 1 unique guide identities (2 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK18WebContentChromium`: class `0x10447e518`, class_ro `0x10447e4b0`, direct protocols `ArcWebContentsTopBandColorDelegate;ArcWebContentsThumbnailsGeneratorObserver;ArcWebContentsUIDelegate;ArcWebContentsNavigationObserver;ArcWebContentsNavigationDelegate;ArcWebContentsMediaSessionDelegate;ArcWebContentsDevToolsDelegate;ArcWebContentsBoostIDEDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Bridge `0x4445a58` wraps `ArcNavigationAction`, loads weak receiver/owner and sends `webContents:decidePolicyForNavigationAction:` at `0x4445abc`. `cmp x0,#12` at `0x4445ac0` followed by unsigned `b.hs` rejects values outside 0–11. The return is consumed immediately, so this boundary is synchronous; the metadata signed int64 return is not an asynchronous completion.

App implementation `0x100535984` forwards to `0x10053605c`; `0x100536028` supplies a mapping table on some paths. Policy ordinal names are unresolved. Chromium enum resemblance is not sufficient to name them.

Setter `0x4440dec` holds weak self/delegate in a shared bridge. Nil calls native `0x4375b40` with zero registration arguments. Clearing native registration does not prove destruction of the wrapper-held bridge or cancellation of tasks already carrying it.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (int64_t)webContents:(ArcWebContents *)arg0 decidePolicyForNavigationAction:(ArcNavigationAction *)arg1;` | `q32@0:8@16@24` | `0x100535984` (entry `0x1035ff1fc`) | `0x4445abc` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Resolve payload/enum schemas, nil/default/error behavior, owner/delegate retention, callback lifetime, cancellation/teardown, ordering, reentrancy and execution sequence against a future authorized behavioral harness.

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
