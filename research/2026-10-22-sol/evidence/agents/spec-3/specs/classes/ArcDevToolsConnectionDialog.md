# ArcDevToolsConnectionDialog

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37c168`, class RO `0xe2a6140`; raw otool `objc170.txt:24770`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline native dialog holds three callback transports, corresponding to accept/deny/disable (0x4402950, 0x440296c, 0x4402988). Each checks a cancellation flag before indirect invocation. init/setter copy via 0x4335ae8; getter copies via 0x43359c8 (0x44028f8, 0x44029b0, 0x44029a4), while destructor tears down each callback transport (0x44029bc). accept has a false C socket prototype in Ghidra: preserve metadata ABI and verify actual registers. No proof was found that any action consumes all three callbacks or guarantees single use. Persisted-disable scope, callback thread, cancellation and reentrancy are consequential unresolved requirements for remote-debugging permission UI.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `104`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _dialog | 8 | 96 | `{DevToolsConnectionDialog="accepted"{function<void ()>="__f_"{__policy_func<void ()>="__buf_"(__policy_storage="__small"[16c]"__large"^v)"__func_"^?"__policy_"^{__policy}}}"denied"{function<void ()>="__f_"{__policy_func<void ()>="__buf_"(__policy_storage="__small"[16c]"__large"^v)"__func_"^?"__policy_"^{__policy}}}"disabled"{function<void ()>="__f_"{__policy_func<void ()>="__buf_"(__policy_storage="__small"[16c]"__large"^v)"__func_"^?"__policy_"^{__policy}}}}` | 0xe2a6108 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| dialog | `T{DevToolsConnectionDialog={function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}},N,V_dialog` | 0xe2a6130 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithDevToolsConnectionDialog:` | `@24@0:8r^v16` | [0x44028f8](../bodies/044028f8.txt) | Calls NSObject init then copies native callback-bearing dialog into self+8 through 0x4335ae8. | 0x4335ae8 | metadata `0xd778df0`; dobjc170.txt:1677 | medium-partial; available; partial_review |
| (base) | `-accept` | `v16@0:8` | [0x4402950](../bodies/04402950.txt) | Tests accept callback cancellation flag, then indirectly invokes stored callable at self+0x18 on self+8. Ghidra wrongly recovered a socket accept prototype. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778dfc`; dobjc170.txt:1678 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-deny` | `v16@0:8` | [0x440296c](../bodies/0440296c.txt) | Tests deny callback cancellation flag and indirectly invokes callable at self+0x38 on self+0x28. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778e08`; dobjc170.txt:1679 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-disable` | `v16@0:8` | [0x4402988](../bodies/04402988.txt) | Tests disable callback cancellation flag and indirectly invokes callable at self+0x58 on self+0x48. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778e14`; dobjc170.txt:1680 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `-dialog` | `{DevToolsConnectionDialog={function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}}16@0:8` | [0x44029a4](../bodies/044029a4.txt) | Copies native dialog from self+8 via 0x43359c8 into native aggregate return storage. | 0x43359c8 | metadata `0xd778e20`; dobjc170.txt:1681 | medium-partial; available; partial_review |
| (base) | `-setDialog:` | `v112@0:8{DevToolsConnectionDialog={function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}{function<void ()>={__policy_func<void ()>=(__policy_storage=[16c]^v)^?^{__policy}}}}16` | [0x44029b0](../bodies/044029b0.txt) | Assigns native callback-bearing dialog into self+8 via 0x4335ae8. | 0x4335ae8 | metadata `0xd778e2c`; dobjc170.txt:1682 | medium-partial; available; partial_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x44029bc](../bodies/044029bc.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778e38`; dobjc170.txt:1683 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4402a1c](../bodies/04402a1c.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd778e44`; dobjc170.txt:1684 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Callback copy vs move, cancellation ownership and invocation sequence.
- Recover correct ObjC ABI and whether callback consumes/closes dialog.
- Exactly-once/cross-action invalidation and callback thread.
- What disable persists and for which remote-debugging scope?
- Callable-copy ownership and ABI.
- Assignment cancellation/reentrancy behavior for previous callbacks.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
