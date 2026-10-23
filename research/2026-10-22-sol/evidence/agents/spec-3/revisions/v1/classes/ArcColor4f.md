# ArcColor4f

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37ddd8`, class RO `0xe2ad500`; raw otool `objc170.txt:33648`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Stores four floats inline and copies 16 bytes in init/accessor/setter (0x443b1c8, 0x443b214, 0x443b220). Channel getters read +8/+0xc/+0x10/+0x14 (0x43dee04, 0x43dee0c, 0xc7fdb4, 0x43dee14). The Init category independently adds initWithR:g:b:a:, packing four floats then forwarding (0x4401b90). Both C++ lifecycle methods share a single-ret IMP (0x15ac0): .cxx_construct returns self with x0 unchanged and performs no stores or zeroing; .cxx_destruct has void semantics and performs no cleanup. Ghidra also erases r's return entirely: pristine `ldr s0,[x0,#8]; ret` at 0x43dee04 recovers it (evidence/arm64-leaves.json). Ghidra's color4f return-register reconstruction is degraded; retain the metadata aggregate encoding and confirm arm64 HFA conventions. No callbacks/thread dependency or color-space/range/clamping semantics were established. This is value marshalling; no browser-service route is needed for the reviewed operations.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _color4f | 8 | 16 | `{Color4f="r"f"g"f"b"f"a"f}` | 0xe2ad488 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| color4f | `T{Color4f=ffff},N,V_color4f` | 0xe2ad4b0 |
| r | `Tf,R,N` | 0xe2ad4c0 |
| g | `Tf,R,N` | 0xe2ad4d0 |
| b | `Tf,R,N` | 0xe2ad4e0 |
| a | `Tf,R,N` | 0xe2ad4f0 |

Categories in assignment: `Init` at `0xe2a5ff8`. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithColor4f:` | `@24@0:8r^{Color4f=ffff}16` | [0x443b1c8](../bodies/0443b1c8.txt) | Calls NSObject init then copies four native floats (16 bytes) into self+8. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c388`; dobjc170.txt:2909 | medium-local; available; local_review |
| (base) | `-r` | `f16@0:8` | [0x43dee04](../bodies/043dee04.txt) | Pseudocode loses the return value under incorrect void prototype. Pristine arm64 leaf: ldr s0, [x0, #8]; ret. Field identity/type independently checked against this class metadata. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c394`; dobjc170.txt:2910 | high-local; available_malformed_signature_recovered_leaf; instruction_verified |
| (base) | `-g` | `f16@0:8` | [0x43dee0c](../bodies/043dee0c.txt) | Literal return: *(float *)(param_1 + 0xc). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3a0`; dobjc170.txt:2911 | high-local; available; literal_verified |
| (base) | `-b` | `f16@0:8` | [0xc7fdb4](../bodies/00c7fdb4.txt) | Literal return: *(float *)(param_1 + 0x10). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3ac`; dobjc170.txt:2912 | high-local; available; literal_verified |
| (base) | `-a` | `f16@0:8` | [0x43dee14](../bodies/043dee14.txt) | Literal return: *(float *)(param_1 + 0x14). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3b8`; dobjc170.txt:2913 | high-local; available; literal_verified |
| (base) | `-color4f` | `{Color4f=ffff}16@0:8` | [0x443b214](../bodies/0443b214.txt) | Copies 16-byte color value from self+8; Ghidra return-register reconstruction contains undefined in_s registers and needs HFA ABI review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3c4`; dobjc170.txt:2914 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-setColor4f:` | `v32@0:8{Color4f=ffff}16` | [0x443b220](../bodies/0443b220.txt) | Copies 16-byte input color value to self+8; Ghidra exposes in_x2 instead of correctly typed aggregate argument. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3d0`; dobjc170.txt:2915 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x15ac0](../bodies/00015ac0.txt) | No-op destructor: the exact arm64 body is a single ret, with no stores or cleanup calls. This declaration has void return semantics; unchanged x0 does not create an object result. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3dc`; dobjc170.txt:2916 | high-local; available; instruction_verified |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x15ac0](../bodies/00015ac0.txt) | No-op initializer: the exact arm64 body is ret with x0 unchanged, returning self under this method encoding. It performs no stores, zeroing or field initialization. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77c3e8`; dobjc170.txt:2917 | high-local; available; instruction_verified |
| Init | `-initWithR:g:b:a:` | `@32@0:8f16f20f24f28` | [0x4401b90](../bodies/04401b90.txt) | Init category packs four float components and forwards to initWithColor4f:. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778da8`; dobjc170.txt:4958 | medium-local; available; local_review |

Unresolved engineering questions (method-specific evidence above):

- Confirm four-float aggregate return register convention.
- Confirm incoming aggregate argument register/indirection ABI.
