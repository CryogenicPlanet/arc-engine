# ArcFreezingVoteToken

ArcCore **1.170.0 arm64**, unslid engine VAs. 5 declared identities. Superclass `NSObject`; class metadata `0xe37e3c8`, class RO `0xe2ae6a8`; raw otool `objc170.txt:35072`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/evidence/sources.json).

This is move-owned native state: initialization moves a unique_ptr into self+8 and clears the input pointer (0x443eb10). clearVote calls 0x4335e08 only for non-null engaged token; that helper clears vote/liveness references and unregisters from a collection (0x443eb94 → 0x4335e08). Destruction clears pointer, calls 0x43363b4 and frees allocation (0x443ebb0). freezing_vote_token returns the address of wrapper storage, not a new owned token (0x283ebf0). Which performance-manager vote policy/sequence is involved remains unresolved; a generic browser freeze API cannot be assumed equivalent. Repeated clearing and destruction after graph shutdown are critical validation cases.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _freezing_vote_token | 8 | 16 | `{optional<arc::FreezingVoteToken>=""(?="__null_state_"c"__val_"{FreezingVoteToken="impl_"{unique_ptr<arc::FreezingVoteToken::Impl, std::default_delete<arc::FreezingVoteToken::Impl>>="__ptr_"^{Impl}}})"__engaged_"B}` | 0xe2ae688 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| None declared | — | — |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-freezing_vote_token` | `r^v16@0:8` | [0x283ebf0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0283ebf0.txt) | Literal return: (void *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cb40`; dobjc170.txt:3099 | high-local; available; literal_verified |
| (base) | `-initWithFreezingVoteToken:` | `@24@0:8^v16` | [0x443eb10](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443eb10.txt) | Moves native unique_ptr into self+8, clearing source pointer; engages wrapper flag at +0x10; replaces/destroys prior token. | 0x43363b4 | metadata `0xd77cb4c`; dobjc170.txt:3100 | medium-local; available; local_review |
| (base) | `-clearVote` | `v16@0:8` | [0x443eb94](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443eb94.txt) | Requires engaged optional; if token pointer non-null calls 0x4335e08, which clears vote/liveness fields and unregisters. | 0x4335e08 | metadata `0xd77cb58`; dobjc170.txt:3101 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x443ebb0](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443ebb0.txt) | If native token engaged and non-null, clears pointer, destroys via 0x43363b4, frees 0x58-byte allocation. | 0x43363b4 | metadata `0xd77cb64`; dobjc170.txt:3102 | medium-partial; available; partial_review |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4400f4c](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/04400f4c.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cb70`; dobjc170.txt:3103 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- No public copy operation shown.
- Vote type enumeration, graph sequence and repeated-clear guarantees.
- Graph sequence requirements at destruction.
- Confirm nontrivial native default-constructor semantics before translating layout.


Original reviewed source: [ArcFreezingVoteToken.md](../research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/classes/ArcFreezingVoteToken.md).
