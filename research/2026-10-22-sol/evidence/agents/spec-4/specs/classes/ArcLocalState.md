# ArcLocalState

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 12 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c528`, class_ro `0xe2a7420`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_localState` | 8 | 24 | `{optional<arc::LocalState>=""(?="__null_state_"c"__val_"{LocalState="impl_"{shared_ptr<arc::LocalState::Impl>="__ptr_"^{… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Initializer `0x440601c` copies optional shared native local-state handle; generic cleanup `0x43df69c` releases it. Preference callback registration `0x4406140` → `0x4325ac8` returns unsigned NSNumber token; remove `0x4406368` uses unsignedIntValue. Callback adapter `0x4407194` supplies converted preference-name string. Error pointer recovery in registration pseudocode is malformed; type encoding is authoritative ABI metadata.

## Threading and caller/callee relations

Native bridge `0x4325ac8` explicitly reports detached object, and typed getter backend `0x4327790` distinguishes missing preference and wrong type. Conversion `0x4406e44` maps bool/int/double/string/dictionary/list and NSNull recursively. Native policy getter `0x4327f3c` maps numeric selector to preference name, not a proven general policy service query. Callback queue, registrar thread and token invalidation remain unresolved.

## Chrome / CEF route implications

Strong source-backed candidate: PrefService FindPreference and typed getters (Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/prefs/pref_service.h:234`/:239/:240/:253/:258), PrefChangeRegistrar Add/Remove (Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/prefs/pref_change_registrar.h:55`/:63). This is browser-wide local state, distinct from profile preferences. CEF per-request-context preference access must be compared for registered keys, policy and callback semantics.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithLocalState:` | `@24@0:8r^v16` | [0x440601c](../bodies/440601c.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd7794a8`; D 1835 | M; available; local_body_observed |
| (base) | `-addPrefChangeCallback:callback:error:` | `@40@0:8@16@?24^@32` | [0x4406140](../bodies/4406140.c) | Converts preference name, captures callback through 0x4407194 and calls 0x4325ac8; returns NSNumber token on success, NSError on failure. Ghidra error-pointer argument recovery is malformed. | 0x1b356b8; 0x4325ac8; 0x44025f4 | M `0xd7794b4`; D 1836 | M; available_prototype_malformed; bridge_reviewed_partial |
| (base) | `-removePrefChangeCallback:error:` | `B32@0:8@16^@24` | [0x4406368](../bodies/4406368.c) | Reads NSNumber token as unsignedInt, calls 0x4325ce8 and returns success or NSError. | 0x4325ce8; 0x44025f4 | M `0xd7794c0`; D 1837 | M; available; bridge_reviewed_partial |
| (base) | `-getBoolean:error:` | `@32@0:8@16^@24` | [0x4406454](../bodies/4406454.c) | Converts preference path and calls 0x4325da4; boxes Boolean result or translates native error with 0x44025f4. | 0x1b356b8; 0x4325da4; 0x44025f4 | M `0xd7794cc`; D 1838 | M; available; bridge_reviewed_partial |
| (base) | `-getString:error:` | `@32@0:8@16^@24` | [0x44065c8](../bodies/44065c8.c) | Converts preference path, calls 0x4325e5c and returns converted string on success or NSError on failure. | 0x1b356b8; 0x1c391bc; 0x4325e5c; 0x44025f4 | M `0xd7794d8`; D 1839 | M; available; bridge_reviewed_partial |
| (base) | `-getInteger:error:` | `@32@0:8@16^@24` | [0x4406798](../bodies/4406798.c) | Converts preference path, calls 0x4325f14 and boxes signed 32-bit value as NSNumber; translates failure. | 0x1b356b8; 0x4325f14; 0x44025f4 | M `0xd7794e4`; D 1840 | M; available; bridge_reviewed_partial |
| (base) | `-getDictionary:error:` | `@32@0:8@16^@24` | [0x4406910](../bodies/4406910.c) | Calls 0x4325fcc and recursively converts native dictionary values via 0x4406e44 into NSMutableDictionary. | 0x1b356b8; 0x1c391bc; 0x4325fcc; 0x433ba7c; 0x44025f4; 0x4406e44 | M `0xd7794f0`; D 1841 | M; available; bridge_reviewed_partial |
| (base) | `-getList:error:` | `@32@0:8@16^@24` | [0x4406c00](../bodies/4406c00.c) | Calls 0x4326084 and recursively converts native list values through 0x4406e44 into NSMutableArray. | 0x1b356b8; 0x4326084; 0x433b47c; 0x44025f4; 0x4406e44; 0x4b37ee8 | M `0xd7794fc`; D 1842 | M; available; bridge_reviewed_partial |
| (base) | `-getBrowserPolicy:error:` | `@32@0:8Q16^@24` | [0x4407084](../bodies/4407084.c) | Calls 0x432631c with numeric policy selector and converts returned native value via 0x4406e44; backend looks up a mapped preference and reports missing preference. | 0x432631c; 0x433b0ec; 0x44025f4; 0x4406e44 | M `0xd779508`; D 1843 | M; available; bridge_reviewed_partial |
| (base) | `-localState` | `{optional<arc::LocalState>=(?=c{LocalState={shared_ptr<arc::LocalState::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd779514`; D 1844 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd779520`; D 1845 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77952c`; D 1846 | M; available; local_body_observed |

## Remaining consequential questions

Disassemble registration error-pointer ABI; identify policy selector table, error domain/codes and callback cancellation semantics. Confirm recursive value conversion, token reuse and local-state shutdown.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
