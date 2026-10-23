# ArcTracingControllerDelegate

Trace start, completed result and abort lifecycle. Classification: `tracing_callback`.

Arc app 1.168.0 arm64 protocol descriptor `0x1043b1e80`; inherits `NSObject`. Pristine binary corroborates all 3 unique guide identities (3 physical method entries). Duplicate entries remain in the raw evidence; their encodings agree. Every identity has a matching implementation in an explicit app adopter. This establishes a declaration/implementation surface, not execution.

Engine ArcCore 1.168.0 and 1.170.0 each have 69 parsed non-Arc protocol descriptors and no Arc-named descriptor in these lists. New-engine selector sends below provide independent usage evidence. All app VAs are unslid `0x100…`; all engine VAs are unslid low Mach-O VAs. File offsets are segment-translated. No older engine code was read.

## Actual adopters

- `_TtC3ADK25TracingControllerChromium`: class `0x10447e328`, class_ro `0x10447e2c0`, direct protocols `ArcTracingControllerDelegate`.

Explicit class/category conformance is reported; dynamically created classes and inherited subclass conformances are not exhaustively enumerated. Method entry/IMP equality does not establish that a callback ran.

## High-value behavior and lifecycle

Can start/complete/abort race with stop or destruction, and who owns the result?

Per-method typed provenance and static send candidates are complete for this bounded guide surface. Payload schema, enum names, lifetime beyond the send, removal/teardown, reentrancy, ordering and callback sequence remain unresolved unless a specific finding above establishes a local operation. No runtime execution or blanket main-thread guarantee is claimed.

## Typed declaration surface

Declarations come from the pristine app protocol method entry plus its own extended-type array. Pairing uses the list-prefix/entry-index rule in pinned objc4 `fb265098298302243cd7eeaa1f63f0ba7786dd9a`, `runtime/objc-runtime-new.mm:4885–4924`; object/block annotations erase exactly to the same entry's raw encoding. This is not a dyld/otool order join. No enum names, nullability, generic element types or ownership are inferred. The implicit Objective-C receiver/selector and block receiver are omitted in the readable declarations.

| Required/optional | Typed declaration | Raw encoding | App IMP(s) | Engine static send(s) |
|---|---|---|---|---|
| required | `- (void)tracingControllerDidStartTracing:(ArcTracingController *)arg0;` | `v24@0:8@16` | `0x100530f24` (entry `0x1035e3028`) | `0x440d280` |
| required | `- (void)tracingController:(ArcTracingController *)arg0 didCompleteTracing:(NSString *)arg1;` | `v32@0:8@16@24` | `0x100531000` (entry `0x1035e3034`) | `0x440d300` |
| required | `- (void)tracingControllerDidAbortTracing:(ArcTracingController *)arg0;` | `v24@0:8@16` | `0x10053104c` (entry `0x1035e3040`) | `0x440d360` |

Exact protocol method entries, selector/type VAs, extended-array slots, all raw duplicates, app implementations and source byte ranges are in `../evidence/app168-methods.tsv`, `../evidence/typed-declarations.tsv`, `../evidence/app-contract-joins.tsv`, `../evidence/app-body-index.tsv`. Engine send instructions and selector references are in `../evidence/engine-binary-sends.tsv`; full-corpus source candidates are separately in `../evidence/engine-source-sends.tsv`. Bounded text extracts have original immutable byte ranges in `../evidence/engine-body-index.tsv` and `../evidence/focus-body-index.tsv`. `../evidence/focus-instructions.tsv` and `focus-vtables.tsv` preserve the consequential instruction/table checks. A nearest metadata start or successful pseudocode header is not a certified contiguous function boundary.

## Remaining questions

Can start/complete/abort race with stop or destruction, and who owns the result?

Further per-method questions and evidence grades are in `../callbacks.tsv`; none of these specs claims full replacement-engine semantics.
