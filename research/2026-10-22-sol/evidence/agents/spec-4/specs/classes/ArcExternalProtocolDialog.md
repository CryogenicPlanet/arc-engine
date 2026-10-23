# ArcExternalProtocolDialog

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 9 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 160 bytes; class metadata `0xe37e008`, class_ro `0xe2adb80`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_external_protocol_dialog` | 8 | 152 | `{optional<arc::ExternalProtocolDialog>=""(?="__null_state_"c"__val_"{ExternalProtocolDialog="program_name"{basic_string<… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Inline optional contains UTF-16 program/origin and three callback function objects; initializer `0x443bda8` and getter `0x443bec8` copy through `0x4335c68`, destructor `0x443bf10` calls `0x4335bdc`. Accept `0x443be94`, cancel `0x443beb0`, close `0x443be7c` invoke different stored callbacks. No callback consumption/reset is visible in these small IMPs.

## Threading and caller/callee relations

The Boolean supplied to accept: is forwarded unchanged. close has malformed Ghidra receiver/return type; metadata remains ABI evidence. No callback scheduling is present in these IMPs, so backend/app thread contract is unresolved. Separate callback invocations do not prove mutually exclusive or idempotent actions.

## Chrome / CEF route implications

Embedding must bridge external-protocol UI decisions to native launch/policy behavior and preserve three separate callbacks. Source/API matching and CEF request-handler route remain unverified; no external application launch was performed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithExternalProtocolDialog:` | `@24@0:8r^v16` | [0x443bda8](../bodies/443bda8.c) | Initializes NSObject and copies inline native dialog with its strings and three callback function objects through 0x4335c68, cleaning previous optional value if engaged. | 0x4335bdc; 0x4335c68 | M `0xd77c624`; D 2974 | M; available; bridge_reviewed_partial |
| (base) | `-programName` | `@16@0:8` | [0x443be1c](../bodies/443be1c.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x1d9fb90 | M `0xd77c630`; D 2975 | M; available; local_body_observed |
| (base) | `-origin` | `@16@0:8` | [0x443be4c](../bodies/443be4c.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x1d9fb90 | M `0xd77c63c`; D 2976 | M; available; local_body_observed |
| (base) | `-close` | `v16@0:8` | [0x443be7c](../bodies/443be7c.c) | Requires engaged dialog then invokes closed callback stored at self+0x78 through target self+0x88. Ghidra truncates receiver to int; use metadata ABI. | field/indirect dispatch; see body | M `0xd77c648`; D 2977 | M; available_prototype_malformed; bridge_reviewed_partial |
| (base) | `-accept:` | `v20@0:8B16` | [0x443be94](../bodies/443be94.c) | Requires engaged dialog and invokes accepted callback at self+0x38 with supplied Boolean through target self+0x48. | field/indirect dispatch; see body | M `0xd77c654`; D 2978 | M; available; bridge_reviewed_partial |
| (base) | `-cancel` | `v16@0:8` | [0x443beb0](../bodies/443beb0.c) | Requires engaged dialog and invokes cancelled callback at self+0x58 through target self+0x68. | field/indirect dispatch; see body | M `0xd77c660`; D 2979 | M; available; bridge_reviewed_partial |
| (base) | `-external_protocol_dialog` | `{optional<arc::ExternalProtocolDialog>=(?=c{ExternalProtocolDialog={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{function<void (bool)>={__policy_func<void (bool)>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}})B}16@0:8` | [0x443bec8](../bodies/443bec8.c) | Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly. | 0x4335c68 | M `0xd77c66c`; D 2980 | M; available; local_body_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x443bf10](../bodies/443bf10.c) | Performs C++/ObjC cleanup of native storage; invokes 0x4335bdc. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | 0x4335bdc | M `0xd77c678`; D 2981 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43dd3b4](../bodies/43dd3b4.c) | Initializes native storage with explicit stores: self+8=0; self+0x98=0. | field/indirect dispatch; see body | M `0xd77c684`; D 2982 | M; available; local_body_observed |

## Remaining consequential questions

Determine accept Boolean meaning, remember-choice persistence, invocation order, single-shot semantics and callback lifetime after native dialog close.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
