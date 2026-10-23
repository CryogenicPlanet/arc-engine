# ArcHistogramSamplesDelegate

Periodic and ad hoc histogram sample deliveries. Classification: `diagnostic_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b2a80`; inherits `NSObject`. Pristine binary corroborates all 2 unique guide identities (2 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK30HistogramSamplesDelegateBridge`: class `0x10447ea78`, class_ro `0x10447ea18`, direct protocols `ArcHistogramSamplesDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Which snapshot schema, sampling interval, ownership and sequence are required?

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)periodicHistogramSamplesAvailable:(ArcHistogramSamples *)arg0;` | `v24@0:8@16` | `0x1005490b8` (entry `0x1035ff828`) | `0x43eb584` |
| required | `- (void)adhocHistogramSamplesAvailable:(ArcAdhocHistogramSamples *)arg0;` | `v24@0:8@16` | `0x10054915c` (entry `0x1035ff834`) | `0x43eb654` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Which snapshot schema, sampling interval, ownership and sequence are required?

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
