# ArcBrowserContextPreferencesObserver

Preference/search-engine updates and search-engine addition origin. Classification: `preferences_observer`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b0e10`; inherits `NSObject`. Pristine binary corroborates all 3 unique guide identities (4 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK28BrowserContextDelegateBridge`: class `0x10447da70`, class_ro `0x10447da10`, direct protocols `ArcBrowserContextPreferencesObserver;ArcBrowserContextDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Two app IMPs have no exact exported app body: `browserContextPreferencesDidUpdate:` at `0x1004e82ec` and `browserContextPreferencesDidUpdateSearchEngines:` at `0x1004e82e8`. Independent branch decoding shows both are veneers to shared app implementation `0x1004e530c`, whose exact body is retained. Their entries are branches, not RET-only leaves; substantive handling by the shared routine is not established here. Metadata identities remain separate despite the shared target.

The third unique method carries an added-search-engine object and a signed integer origin. Duplicate protocol rows have identical raw/extended encodings. Search-engine identity, origin enum names, update ordering and observer teardown are unresolved.

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)browserContextPreferencesDidUpdate:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e82ec` (entry `0x1035ff3e8`) | `0x43fc7d4` |
| required | `- (void)browserContextPreferencesDidUpdateSearchEngines:(ArcBrowserContext *)arg0;` | `v24@0:8@16` | `0x1004e82e8` (entry `0x1035ff3f4`) | `0x43fc828` |
| required | `- (void)browserContextPreferences:(ArcBrowserContext *)arg0 didAddSearchEngine:(ArcSearchEngine *)arg1 by:(int64_t)arg2;` | `v40@0:8@16@24q32` | `0x1004e54d4` (entry `0x1035ff400`) | `0x43fc8b4` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

What distinguishes update/search-engine callbacks, and which origin enum values are valid?

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
