# ArcWebContentsAIAgent

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37e648`, class RO `0xe2af008`; raw otool `objc170.txt:35777`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native shared agent handle and cleanup releases it (0x4440a7c, 0x43df69c). sendTextInput converts text and reaches a macOS native-view path (0x4440ba0 → 0x4354de4 → 0x473dff4); the last helper iterates NSString code units, synthesizes NSEvent key events and sends keyDown:. sendKeyPress similarly reaches enum-selected synthetic event (0x4440c34 → 0x4354ea0 → 0x473e20c). It is not established as a CDP or renderer-Mojo method. Weak native-view references are handled in the bridge. No dispatch to main/UI thread was established; native AppKit operation alone is not proof caller affinity is enforced. Exact key enum, surrogate/IME behavior, focus, modifiers/keyCode and view lifetime are critical. CEF would need compatible native event/hosting behavior; public key injection existence would not prove this contract.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _web_contents_ai_agent | 8 | 24 | `{optional<arc::WebContents::AIAgent>=""(?="__null_state_"c"__val_"{AIAgent="impl_"{shared_ptr<arc::WebContents::AIAgent::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2aefd0 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| web_contents_ai_agent | `T{optional<arc::WebContents::AIAgent>=(?=c{AIAgent={shared_ptr<arc::WebContents::AIAgent::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_web_contents_ai_agent` | 0xe2aeff8 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithWebContentsAIAgent:` | `@24@0:8r^v16` | [0x4440a7c](../bodies/04440a7c.txt) | Calls NSObject init; retains native shared agent at self+8/+0x10 and engages +0x18. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cf1c`; dobjc170.txt:3192 | medium-local; available; local_review |
| (base) | `-sendTextInput:` | `v24@0:8@16` | [0x4440ba0](../bodies/04440ba0.txt) | Converts string and forwards through 0x4354de4 to 0x473dff4, which synthesizes one NSEvent keyDown per NSString code unit into native view. | 0x1b356b8,0x4354de4 | metadata `0xd77cf28`; dobjc170.txt:3193 | medium-partial; available; partial_review |
| (base) | `-sendKeyPress:` | `v24@0:8q16` | [0x4440c34](../bodies/04440c34.txt) | Forwards key enum to 0x4354ea0 then 0x473e20c; native helper selects a string for enum 2..5 (fallback otherwise), synthesizes NSEvent and sends keyDown:. | 0x4354ea0 | metadata `0xd77cf34`; dobjc170.txt:3194 | medium-partial; available; partial_review |
| (base) | `-web_contents_ai_agent` | `{optional<arc::WebContents::AIAgent>=(?=c{AIAgent={shared_ptr<arc::WebContents::AIAgent::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x43df53c](../bodies/043df53c.txt) | Copies optional native shared handle from self+8 into aggregate return storage, increments non-null control block; source engagement is at self+0x18. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cf40`; dobjc170.txt:3195 | medium-local; available; local_review |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x43df69c](../bodies/043df69c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cf4c`; dobjc170.txt:3196 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x43df760](../bodies/043df760.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77cf58`; dobjc170.txt:3197 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native view and web-contents lifetime vs agent shared handle.
- Unicode surrogate/IME behavior, focused view identity, event keyCode and thread affinity.
- Exact enum/string mapping and modifier/keyCode semantics.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
