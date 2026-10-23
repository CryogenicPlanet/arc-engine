# ArcImage

ArcCore **1.170.0 arm64**, unslid engine VAs. 13 declared identities. Superclass `NSObject`; class metadata `0xe37c438`, class RO `0xe2a7150`; raw otool `objc170.txt:25812`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Native image init/copy/destruction use shared image representation storage (0x440494c, 0x4404a7c, 0x4405068). nsImage converts through 0x2a02ccc (0x4404fec); isNull checks representation state (0x440500c). Class methods have separate + identity. dangerouslyDecodeFromData:scale: is a synchronous-looking native decoder bridge over NSData bytes/length (0x4404ad4 → 0x4337b04). Full decode retains completion and forwards dimensions/maxDecodedBytes (0x4404e6c → 0x4338920 → 0x4337e00). Short decode shares IMP with ArcImageRep and supplies three zero limits (0x4404fd0). The bounded 0x4337e00 body copies valid input bytes into native allocation before task dispatch and invokes completion directly for null bytes, zero length or nonpositive scale. Decoder helper closure, pixel formats, wider resource-limit behavior, completion executor and teardown lifetime remain unresolved. UI image identity and scale conversion need validation independently of the browser backend.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _image | 8 | 24 | `{Image="_vptr$Image"^^?"impl_"{shared_ptr<arc::ImageImpl>="__ptr_"^{ImageImpl}"__cntrl_"^{__shared_weak_count}}}` | 0xe2a70d8 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| image | `T{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}},N,V_image` | 0xe2a7100 |
| nsImage | `T@"NSImage",R,N` | 0xe2a7110 |
| isNull | `TB,R,N` | 0xe2a7120 |
| logicalWidth | `Tq,R,N` | 0xe2a7130 |
| logicalHeight | `Tq,R,N` | 0xe2a7140 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-init` | `@16@0:8` | [0x440494c](../bodies/0440494c.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x43381c0. | 0x43381c0 | metadata `0xd779320`; dobjc170.txt:1796 | low-unresolved; available; unresolved |
| (base) | `-initWithArcImage:` | `@24@0:8r^v16` | [0x4404a7c](../bodies/04404a7c.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x43380d4. | 0x43380d4 | metadata `0xd77932c`; dobjc170.txt:1797 | low-unresolved; available; unresolved |
| (base) | `-nsImage` | `@16@0:8` | [0x4404fec](../bodies/04404fec.txt) | Returns nil if image state/representation is absent; live representation converts through 0x2a02ccc. | 0x2a02ccc | metadata `0xd779338`; dobjc170.txt:1798 | medium-partial; available; partial_review |
| (base) | `-isNull` | `B16@0:8` | [0x440500c](../bodies/0440500c.txt) | Returns true if image shared state or its native representation pointer is null, otherwise false. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779344`; dobjc170.txt:1799 | medium-local; available; local_review |
| (base) | `-logicalWidth` | `q16@0:8` | [0x8371e80](../bodies/08371e80.txt) | Calls 0x8aed380 on native image state and sign-extends resulting int width. | 0x8aed380 | metadata `0xd779350`; dobjc170.txt:1800 | medium-partial; available; partial_review |
| (base) | `-logicalHeight` | `q16@0:8` | [0x8371e9c](../bodies/08371e9c.txt) | Calls 0x8aed3bc on native image state and sign-extends resulting int height. | 0x8aed3bc | metadata `0xd77935c`; dobjc170.txt:1801 | medium-partial; available; partial_review |
| (base) | `-image` | `{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}}16@0:8` | [0x4405030](../bodies/04405030.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779368`; dobjc170.txt:1802 | low-unresolved; available; unresolved |
| (base) | `-setImage:` | `v40@0:8{Image=^^?{shared_ptr<arc::ImageImpl>=^{ImageImpl}^{__shared_weak_count}}}16` | [0x440505c](../bodies/0440505c.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x43380d4. | 0x43380d4 | metadata `0xd779374`; dobjc170.txt:1803 | low-unresolved; available; unresolved |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x4405068](../bodies/04405068.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779380`; dobjc170.txt:1804 | medium-local; available_with_decompiler_limits; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x4405130](../bodies/04405130.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77938c`; dobjc170.txt:1805 | medium-local; available; literal_partial |
| (base) | `+dangerouslyDecodeFromData:scale:` | `@28@0:8@16f24` | [0x4404ad4](../bodies/04404ad4.txt) | Passes NSData bytes/length and scale through 0x4337b04; wraps native image when result is present. | 0x4337b04,0x43386f8 | metadata `0xd7792f4`; dobjc170.txt:1806 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `+decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion:` | `v60@0:8@16f24q28q36Q44@?52` | [0x4404e6c](../bodies/04404e6c.txt) | Passes NSData bytes/length, scale, desired dimensions, maxDecodedBytes and captured completion through 0x4338920 to 0x4337e00. That helper directly invokes completion for null bytes, zero length or nonpositive scale; on its valid path it allocates and copies input bytes before task dispatch. | 0x4338920 | metadata `0xd779300`; dobjc170.txt:1807 | medium-partial; available_with_decompiler_limits; partial_review |
| (base) | `+decodeFromData:scale:completion:` | `v36@0:8@16f24@?28` | [0x4404fd0](../bodies/04404fd0.txt) | Shared IMP with ArcImageRep; forwards to full decode selector using desiredWidth=0, desiredHeight=0, maxDecodedBytes=0. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77930c`; dobjc170.txt:1808 | medium-local; available; local_review |

Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- NSImage creation/caching and representation ownership.
- Logical scale/rounding in native image helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
- Decode format support, allocation limits, malformed image failure and sync decoder isolation.
- Decoder and completion executors, wider decoder closure, malformed-input outcomes and callback lifetime after teardown remain unresolved.
- Meaning of zero allocation limit lies in decoder bridge.
