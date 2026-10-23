# ArcNotification

ArcCore **1.170.0 arm64**, unslid engine VAs. 34 declared identities. Superclass `NSObject`; class metadata `0xe37c7a8`, class RO `0xe2a7e28`; raw otool `objc170.txt:26698`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native Notification handle (0x4408b0c); 0x440a40c releases native state and cached iconURL. id and browserContextQualifiedID are distinct selectors sharing the same string getter IMP 0x4408d64; do not invent different semantics from names. items/buttons construct ObjC snapshots, including text-input button flags (0x44094bc, 0x4409688), while category builds action/category objects through deeper native/platform helpers (0x4409d38). didReceiveResponse delegates to a platform-response adapter with close/settings/click handlers (0x440a1f8); didClose/click/settings reach native notification callbacks 0x433edd0, 0x433ee60, 0x433ef10 through 0x440a0fc, 0x440a114, 0x440a1e4. The response parser and notification-center ownership, action identifiers and callback thread remain unresolved. Browser context association is wrapped at 0x44092d4. CEF route viability needs native macOS notification/action parity, not only web Notification rendering.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.

| Ivar | Offset | Size | Type | Metadata record VA |
|---|---:|---:|---|---|
| _iconURL | 8 | 8 | `@"NSURL"` | 0xe2a7c50 |
| _notification | 16 | 24 | `{optional<arc::Notification>=""(?="__null_state_"c"__val_"{Notification="impl_"{shared_ptr<arc::Notification::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | 0xe2a7c70 |

| Property | Raw attributes | Metadata record VA |
|---|---|---|
| notification | `T{optional<arc::Notification>=(?=c{Notification={shared_ptr<arc::Notification::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_notification` | 0xe2a7c98 |
| iconURL | `T@"NSURL",&,N,V_iconURL` | 0xe2a7ca8 |
| type | `Tq,R,N` | 0xe2a7cb8 |
| title | `T@"NSString",R,N` | 0xe2a7cc8 |
| id | `T@"NSString",R,N` | 0xe2a7cd8 |
| browserContextLocalID | `T@"NSString",R,N` | 0xe2a7ce8 |
| browserContextQualifiedID | `T@"NSString",R,N` | 0xe2a7cf8 |
| message | `T@"NSString",R,N` | 0xe2a7d08 |
| contextMessage | `T@"NSString",R,N` | 0xe2a7d18 |
| origin | `T@"NSURL",R,N` | 0xe2a7d28 |
| documentURL | `T@"NSURL",R,N` | 0xe2a7d38 |
| priority | `Tq,R,N` | 0xe2a7d48 |
| progress | `Ti,R,N` | 0xe2a7d58 |
| progressStatus | `T@"NSString",R,N` | 0xe2a7d68 |
| renotify | `TB,R,N` | 0xe2a7d78 |
| permanent | `TB,R,N` | 0xe2a7d88 |
| items | `T@"NSArray",R,N` | 0xe2a7d98 |
| icon | `T@"NSImage",R,N` | 0xe2a7da8 |
| image | `T@"NSImage",R,N` | 0xe2a7db8 |
| smallImage | `T@"NSImage",R,N` | 0xe2a7dc8 |
| buttons | `T@"NSArray",R,N` | 0xe2a7dd8 |
| showSnoozeButton | `TB,R,N` | 0xe2a7de8 |
| showSettingsButton | `TB,R,N` | 0xe2a7df8 |
| category | `T@"NSString",R,N` | 0xe2a7e08 |
| browserContext | `T@"ArcBrowserContext",R,N` | 0xe2a7e18 |

Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.

| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |
|---|---|---|---|---|---|---|---|
| (base) | `-initWithNotification:` | `@24@0:8r^v16` | [0x4408b0c](../bodies/04408b0c.txt) | Calls NSObject init; retains native shared notification at self+0x10/+0x18 and engages +0x20. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797f0`; dobjc170.txt:1912 | medium-local; available; local_review |
| (base) | `-type` | `q16@0:8` | [0x4408c30](../bodies/04408c30.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7797fc`; dobjc170.txt:1913 | low-unresolved; available; unresolved |
| (base) | `-priority` | `q16@0:8` | [0x4408c68](../bodies/04408c68.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779808`; dobjc170.txt:1914 | low-unresolved; available; unresolved |
| (base) | `-title` | `@16@0:8` | [0x4408ca4](../bodies/04408ca4.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd779814`; dobjc170.txt:1915 | medium-local; available; conversion_partial |
| (base) | `-id` | `@16@0:8` | [0x4408d64](../bodies/04408d64.txt) | Both id and browserContextQualifiedID share this IMP; both obtain same string via 0x433e240. | 0x433e240,0x1c391bc | metadata `0xd779820`; dobjc170.txt:1916 | medium-local; available; local_review |
| (base) | `-browserContextLocalID` | `@16@0:8` | [0x4408e0c](../bodies/04408e0c.txt) | Converts native field-derived string data through 0x1c391bc (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x60a60,0x1c391bc | metadata `0xd77982c`; dobjc170.txt:1917 | medium-local; available; conversion_partial |
| (base) | `-browserContextQualifiedID` | `@16@0:8` | [0x4408d64](../bodies/04408d64.txt) | Both id and browserContextQualifiedID share this IMP; both obtain same string via 0x433e240. | 0x433e240,0x1c391bc | metadata `0xd779838`; dobjc170.txt:1918 | medium-local; available; local_review |
| (base) | `-message` | `@16@0:8` | [0x4408ecc](../bodies/04408ecc.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd779844`; dobjc170.txt:1919 | medium-local; available; conversion_partial |
| (base) | `-contextMessage` | `@16@0:8` | [0x4408f8c](../bodies/04408f8c.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x433e440,0x1d9fb90 | metadata `0xd779850`; dobjc170.txt:1920 | medium-local; available; conversion_partial |
| (base) | `-progressStatus` | `@16@0:8` | [0x440902c](../bodies/0440902c.txt) | Converts native field-derived string data through 0x1d9fb90 (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body. | 0x54420,0x1d9fb90 | metadata `0xd77985c`; dobjc170.txt:1921 | medium-local; available; conversion_partial |
| (base) | `-origin` | `@16@0:8` | [0x44090f4](../bodies/044090f4.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x7cf864, 0x60a60, 0x43abd34. | 0x7cf864,0x60a60,0x43abd34 | metadata `0xd779868`; dobjc170.txt:1922 | low-unresolved; available; unresolved |
| (base) | `-documentURL` | `@16@0:8` | [0x44091a4](../bodies/044091a4.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x7cf864, 0x60a60, 0x43abd34. | 0x7cf864,0x60a60,0x43abd34 | metadata `0xd779874`; dobjc170.txt:1923 | low-unresolved; available_with_decompiler_limits; unresolved |
| (base) | `-progress` | `i16@0:8` | [0x4409274](../bodies/04409274.txt) | Observed return expression: *(int *)(*(long *)(param_1 + 0x10) + 0x288). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779880`; dobjc170.txt:1924 | medium-local; available; literal_partial |
| (base) | `-renotify` | `B16@0:8` | [0x440928c](../bodies/0440928c.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x2e0). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77988c`; dobjc170.txt:1925 | medium-local; available; literal_partial |
| (base) | `-permanent` | `B16@0:8` | [0x44092a4](../bodies/044092a4.txt) | Behavior unresolved beyond local wrapper evidence. Native field or indirect-dispatch operations require further review. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779898`; dobjc170.txt:1926 | low-unresolved; available; unresolved |
| (base) | `-browserContext` | `@16@0:8` | [0x44092d4](../bodies/044092d4.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x433eb60. Observed selector operands: initWithBrowserContext:. | 0x433eb60 | metadata `0xd7798a4`; dobjc170.txt:1927 | low-unresolved; available; unresolved |
| (base) | `-showSnoozeButton` | `B16@0:8` | [0x4409484](../bodies/04409484.txt) | Observed return expression: (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x314). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7798b0`; dobjc170.txt:1928 | medium-local; available; literal_partial |
| (base) | `-showSettingsButton` | `B16@0:8` | [0x440949c](../bodies/0440949c.txt) | Observed return expression: (bool)(*(int *)(*(long *)(param_1 + 0x10) + 0x310) != 0). Guard/trap and native optional details are in exact body. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd7798bc`; dobjc170.txt:1929 | medium-local; available; literal_partial |
| (base) | `-items` | `@16@0:8` | [0x44094bc](../bodies/044094bc.txt) | Builds notification item array by calling native item accessor, creating ObjC items and setting title/message; returns copy. | 0x433e630,0x440a4e8,0x1d9fb90 | metadata `0xd7798c8`; dobjc170.txt:1930 | medium-partial; available; partial_review |
| (base) | `-buttons` | `@16@0:8` | [0x4409688](../bodies/04409688.txt) | Builds button array with title, NSImage icon, placeholder and isTextInput; returns copied array. | 0x433e888,0x433e338,0x1d9fb90,0x2a02ccc | metadata `0xd7798d4`; dobjc170.txt:1931 | medium-partial; available; partial_review |
| (base) | `-icon` | `@16@0:8` | [0x44099e8](../bodies/044099e8.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x433e540, 0x2a02ccc. | 0x433e540,0x2a02ccc | metadata `0xd7798e0`; dobjc170.txt:1932 | low-unresolved; available; unresolved |
| (base) | `-image` | `@16@0:8` | [0x4409b00](../bodies/04409b00.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x4338dd8, 0x2a02ccc. | 0x4338dd8,0x2a02ccc | metadata `0xd7798ec`; dobjc170.txt:1933 | low-unresolved; available; unresolved |
| (base) | `-smallImage` | `@16@0:8` | [0x4409c1c](../bodies/04409c1c.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x4338dd8, 0x2a02ccc. | 0x4338dd8,0x2a02ccc | metadata `0xd7798f8`; dobjc170.txt:1934 | low-unresolved; available; unresolved |
| (base) | `-category` | `@16@0:8` | [0x4409d38](../bodies/04409d38.txt) | Constructs notification category/actions using native buttons and identifiers; several deeper helpers and platform behavior remain unresolved. | 0x433e888,0x54420,0x577fc24,0x440a5f0,0x433e338,0x433e240,0x473ed28,0x577ee50 | metadata `0xd779904`; dobjc170.txt:1935 | medium-partial; available; partial_review |
| (base) | `-didClose:` | `v20@0:8B16` | [0x440a0fc](../bodies/0440a0fc.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x433edd0. | 0x433edd0 | metadata `0xd779910`; dobjc170.txt:1936 | low-unresolved; available; unresolved |
| (base) | `-didClick:input:` | `v28@0:8i16@20` | [0x440a114](../bodies/0440a114.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x1b356b8, 0x433ee60. | 0x1b356b8,0x433ee60 | metadata `0xd77991c`; dobjc170.txt:1937 | low-unresolved; available; unresolved |
| (base) | `-didClickSettings` | `v16@0:8` | [0x440a1e4](../bodies/0440a1e4.txt) | Behavior unresolved beyond local wrapper evidence. Direct bridge/helper calls: 0x433ef10. | 0x433ef10 | metadata `0xd779928`; dobjc170.txt:1938 | low-unresolved; available; unresolved |
| (base) | `-didReceiveResponse:` | `v24@0:8@16` | [0x440a1f8](../bodies/0440a1f8.txt) | Passes UNNotificationResponse to 0x473f138 with three handlers that route to didClose:, didClickSettings, and didClick:input:. | 0x473f138 | metadata `0xd779934`; dobjc170.txt:1939 | medium-partial; available; partial_review |
| (base) | `-notification` | `{optional<arc::Notification>=(?=c{Notification={shared_ptr<arc::Notification::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | [0x440a2ac](../bodies/0440a2ac.txt) | Copies optional native shared handle from self+0x10 into aggregate return storage, increments non-null control block; source engagement is at self+0x20. Ghidra display type may name a different IMP alias. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779940`; dobjc170.txt:1940 | medium-local; available; local_review |
| (base) | `-setNotification:` | `v40@0:8{optional<arc::Notification>=(?=c{Notification={shared_ptr<arc::Notification::Impl>=^{Impl}^{__shared_weak_count}}})B}16` | [0x440a2ec](../bodies/0440a2ec.txt) | Replaces optional native shared handle at self+0x10, retaining/releasing control blocks as presence changes; engagement byte self+0x20. | 0x4331e20 | metadata `0xd77994c`; dobjc170.txt:1941 | medium-local; available; local_review |
| (base) | `-iconURL` | `@16@0:8` | [0x2d0fb40](../bodies/02d0fb40.txt) | Literal return: *(ID *)(param_1 + 8). Use this row metadata type; shared Ghidra name/type may differ. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779958`; dobjc170.txt:1942 | high-local; available; literal_verified |
| (base) | `-setIconURL:` | `v24@0:8@16` | [0x43eae2c](../bodies/043eae2c.txt) | Literal store: *(ID *)(param_1 + 8) = param_3; Runtime annotation identifies objc_storeStrong. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779964`; dobjc170.txt:1943 | high-local; available; literal_verified |
| (base) | `-.cxx_destruct` | `v16@0:8` | [0x440a40c](../bodies/0440a40c.txt) | Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd779970`; dobjc170.txt:1944 | medium-local; available; literal_partial |
| (base) | `-.cxx_construct` | `@16@0:8` | [0x440a4dc](../bodies/0440a4dc.txt) | Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference. | No direct FUN call; see body for field/ObjC/indirect operations | metadata `0xd77997c`; dobjc170.txt:1945 | medium-local; available; literal_partial |

Unresolved engineering questions (method-specific evidence above):

- Native notification lifetime after delivery/response.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Do clients rely on identical identifiers across context scopes?
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Ordering and optional empty-field behavior.
- Input-button ordering and response-action identifiers.
- Category ID stability and UNNotificationAction option mapping.
- Action identifiers, text input and close-by-user mapping.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
