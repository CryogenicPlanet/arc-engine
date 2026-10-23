# ArcSearchEngine

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 14 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37d0b8`, class_ro `0xe2aa2b0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| `_search_engine_template_url` | 8 | 24 | `{optional<arc::SearchEngine>=""(?="__null_state_"c"__val_"{SearchEngine="impl_"{shared_ptr<arc::SearchEngine::Impl>="__p… (full encoding in layout.json)` |

## Lifecycle, ownership and callbacks

Optional shared native search-engine record; `0x442c50c` builds from name/keyword/URL/favicon, `0x442c8ac` copies shared handle. Name/keyword getters convert UTF-16 (`0x442c9d0`, `0x442ca98`), URL uses byte string (`0x442cb60`); `0x442cd1c` tests prepopulate ID>0. Category equality/hash use native record identity, not a proven fieldwise comparison. Initializer IMPs: `0x442c50c`, `0x442c8ac`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Native data fits Chromium TemplateURL family; source analogue Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/search_engines/template_url_service.h:264` and Arc preferences bridges `0x43fa828`, `0x43fab90`, `0x43faf00`, `0x43fb268` establish service operations. Preserve search template substitutions/status and provider policy; CEF generic URL loading alone does not establish this contract.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `-mutableSearchEngineTemplateURL` | `^v16@0:8` | [0x442c4f0](../bodies/442c4f0.c) | Returns pointer to inline native value at self+8 when optional is engaged; disengaged path calls 0x4b08a00. | field/indirect dispatch; see body | M `0xd77abac`; D 2344 | H; available; local_field_observed |
| (base) | `-init:keyword:url:faviconUrl:` | `@48@0:8@16@24@32@40` | [0x442c50c](../bodies/442c50c.c) | Converts display name/keyword to UTF-16 and URL/favicon to byte strings; constructs native SearchEngine via 0x433ffd8 and stores shared optional handle. | 0x1b356b8; 0x433ffd8; 0x83b1228 | M `0xd77abb8`; D 2345 | M; available; bridge_reviewed_partial |
| (base) | `-initWithSearchEngineTemplateURL:` | `@24@0:8r^v16` | [0x442c8ac](../bodies/442c8ac.c) | Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count. | field/indirect dispatch; see body | M `0xd77abc4`; D 2346 | M; available; local_body_observed |
| (base) | `-name` | `@16@0:8` | [0x442c9d0](../bodies/442c9d0.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x54420; 0x1d9fb90 | M `0xd77abd0`; D 2347 | M; available; local_body_observed |
| (base) | `-keyword` | `@16@0:8` | [0x442ca98](../bodies/442ca98.c) | Converts native UTF-16 string to NSString through 0x1d9fb90; optional state and small/long string branches are visible in body. | 0x54420; 0x1d9fb90 | M `0xd77abdc`; D 2348 | M; available; local_body_observed |
| (base) | `-url` | `@16@0:8` | [0x442cb60](../bodies/442cb60.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x1c391bc | M `0xd77abe8`; D 2349 | M; available; local_body_observed |
| (base) | `-faviconUrl` | `@16@0:8` | [0x442cc28](../bodies/442cc28.c) | Converts native byte string to NSString through 0x1c391bc; optional state and small/long string branches are visible in body. | 0x60a60; 0x7cf864; 0x1c391bc | M `0xd77abf4`; D 2350 | M; available; local_body_observed |
| (base) | `-status` | `q16@0:8` | [0x442cd04](../bodies/442cd04.c) | Reads native field and returns expression (long)*(int *)(*(long *)(param_1 + 8) + 0x3ac). Optional guards/traps in body must be preserved. | field/indirect dispatch; see body | M `0xd77ac00`; D 2351 | H; available; local_field_observed |
| (base) | `-isPrepopulated` | `B16@0:8` | [0x442cd1c](../bodies/442cd1c.c) | Returns whether native prepopulate ID at Impl+0x354 is positive. | field/indirect dispatch; see body | M `0xd77ac0c`; D 2352 | M; available; bridge_reviewed_partial |
| (base) | `-search_engine_template_url` | `{optional<arc::SearchEngine>=(?=c{SearchEngine={shared_ptr<arc::SearchEngine::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/43df53c.c) | Copies optional native shared handle from self+0x8 into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged. | field/indirect dispatch; see body | M `0xd77ac18`; D 2353 | H; available; local_field_observed |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/43df69c.c) | Performs C++/ObjC cleanup of native storage. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown. | field/indirect dispatch; see body | M `0xd77ac24`; D 2354 | M; available; local_body_observed |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/43df760.c) | Initializes native storage with explicit stores: self+8=0; self+0x18=0. | field/indirect dispatch; see body | M `0xd77ac30`; D 2355 | M; available; local_body_observed |
| Extensions | `-isEqual:` | `B24@0:8@16` | [0x43ecf8c](../bodies/43ecf8c.c) | Checks receiver identity, nil and class compatibility; compares native optional engagement and underlying identity/value after extracting both handles. | field/indirect dispatch; see body | M `0xd7783e4`; D 4877 | M; available; local_body_observed |
| Extensions | `-hash` | `Q16@0:8` | [0x43ed260](../bodies/43ed260.c) | Reads native preference value at key search_engine_template_url. Profile/native handle checks are visible in body. | 0x4340180 | M `0xd7783f0`; D 4878 | M; available; local_body_observed |

## Remaining consequential questions

Recover status enum, template validation/substitution, favicon invalidity fallback and identity/equality across service reload.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
