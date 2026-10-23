# ArcBrowserContextAutocomplete

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 5 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bee8`, class_ro `0xe2a54b0`; instanceStart 8, instanceSize 16 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/layouts.json).

| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |
|---:|---:|---|---|---|
| 0x8 | 8 | `_controller` | `{unique_ptr<arc::ArcAutocompleteController, std::default_delete<arc::ArcAutocompleteController>>="__ptr_"^{ArcAutocompleteControll…` | 0xe2a5490 |

initWithAutocompleteController: adopts the supplied by-value native controller owner into self+8; source-zeroing is not shown on success (0x43ecb50). The zero stores belong to old-owner or failed-init cleanup, so caller-side move/ownership ABI remains unverified; destruction disposes the controller using 0x42c33cc (0x43ece68). A request converts nil text to an empty string, stores a callback adapter and forwards cursor/prevent-inline arguments (0x43ecc2c → 0x42c35e4).

## Lifecycle, callbacks and relations

0x42c35e4 replaces the saved callback, constructs an input with a literal 4 and starts a controller via 0x20cd850. Completion builds an NSArray of ArcAutocompleteMatch and invokes the captured block with an additional argument (0x43eceb0). stop checks native controller/state, directly calls 0x24e39c0 with reason value zero, then resets callback storage and indirectly cleans the saved callable (0x43ece08). Chromium components/omnibox/browser/autocomplete_controller.cc:631 and :797 provide a source-backed Start/Stop hypothesis; exact function identity remains unproven.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A Chrome omnibox service is strongly suggested by input/controller behavior; CEF replacement needs explicit provider/ranking semantics, not only URL navigation. No complete API exclusion is claimed.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/ArcBrowserContextAutocomplete.txt), [indexed machine-readable bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/methods-bodies.json), [method rows](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/methods.tsv), [bridge bodies](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/evidence/bridges.json).

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| base | - initWithAutocompleteController: | `@24@0:8{unique_ptr<arc::ArcAutocompleteController, std::default_delete<arc::ArcAutocompleteController>>=^{ArcAutocompleteController}}16` | 0x43ecb50 | Adopts the supplied by-value native controller owner into self+8 and disposes the previous owner via 0x42c33cc and 0x5810. The successful path does not zero the incoming source; zero stores are old-owner or failed-init cleanup. Caller-side move and full ownership ABI remain unverified. | 0x042c33cc | IMP 0x43ecb50; record 0xd7783a0; dyld L1524; otool L23880 | medium / available / bounded_review |
| base | - autocomplete:cursorPosition:preventInlineAutocomplete:completion: | `v44@0:8@16q24B32@?36` | 0x43ecc2c | Converts nil text to empty native string, captures completion, and forwards cursor/prevent-inline to 0x42c35e4 when controller is nonnull; completion adapter 0x43eceb0 builds ArcAutocompleteMatch array. | 0x01b356b8, 0x042c35e4 | IMP 0x43ecc2c; record 0xd7783ac; dyld L1525; otool L23883 | medium / available / bounded_review |
| base | - stop | `v16@0:8` | 0x43ece08 | Loads the controller through its stored owner; if native controller and state at +0x70 are nonnull, directly calls 0x24e39c0(controller+0x30 value, 0), then resets saved completion storage. The later indirect operation cleans the saved callable; exact upstream Stop identity remains provisional. | 0x024e39c0 | IMP 0x43ece08; record 0xd7783b8; dyld L1526; otool L23886 | medium / available / bounded_review |
| base | - .cxx_destruct | `v16@0:8` | 0x43ece68 | Cleans local native/object state using shown stores and cleanup helpers; self offsets 0x8. | 0x042c33cc | IMP 0x43ece68; record 0xd7783c4; dyld L1527; otool L23889 | medium / available / local_observation |
| base | - .cxx_construct | `@16@0:8` | 0x407ab00 | Initializes native storage/engagement fields at self offsets 0x8; returns self. | local state / indirect operation; see body | IMP 0x407ab00; record 0xd7783d0; dyld L1528; otool L31 | medium / available / local_observation |

## Remaining consequential questions

Identify the completion additional argument, cancellation/supersession rules, controller lifetime, and callback thread; verify cursor narrowing to int in 0x42c35e4. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 5, 6; see [correction ledger](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/corrections.tsv) and [independent recheck notes](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.


Original reviewed source: [ArcBrowserContextAutocomplete.md](../research/2026-10-22-sol/evidence/agents/spec-2/revisions/v1/classes/ArcBrowserContextAutocomplete.md).
