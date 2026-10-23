# ArcBrowserContextPageContentExtraction

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37d8d8`, class_ro `0xe2ac170`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-scoped native extraction service wrapper and asynchronous request adapter.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_owner` | 0x8 | 8 | `@"ArcBrowserContext"` | `0xe2ac108` |
| `_browser_context_page_content_extraction` | 0x10 | 24 | `{optional<arc::BrowserContext::PageContentExtraction>=""(?="__null_state_"c"__val_"{PageContentExtraction="impl_"{shared_ptr<arc::BrowserContext::PageContentExtraction::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ac128` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `browser_context_page_content_extraction`: `T{optional<arc::BrowserContext::PageContentExtraction>=(?=c{PageContentExtraction={shared_ptr<arc::BrowserContext::PageContentExtraction::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browser_context_page_content_extraction` at `0xe2ac150`.
- `owner`: `T@"ArcBrowserContext",W,N,V_owner` at `0xe2ac160`.

## Lifecycle, ownership, callbacks and threading

0x4435bb8 / 0x4435cdc copy native optional at +0x10; second stores owner +8, declared weak in metadata. 0x4435e2c destroys native peer and weak owner. getExtractedContentForWebContents:completion: category 0x43f9a98 reads both optional handles, captures block, and calls 0x42f0254. That helper invokes an empty result callback for unavailable profile/contents and otherwise sets up native extraction continuation. Its CoreServices-string-plus-object pointer expressions are suspicious decompiler artifacts and are not an accepted layout contract.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The result schema is ArcExtractedPageContent / ArcContentNode / ArcFormControlInfo. Renderer/IPC and profile extraction service behavior remain unresolved; CEF DOM/accessibility access is not proven equivalent. Exact native producer and redaction rules are the route gates.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithBrowserContextPageContentExtraction: | `@24@0:8r^v16` | 0x4435bb8 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x4435bb8](../bodies/0x4435bb8.c); record 0xd77bad4; dyld L2702 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -initWithBrowserContextPageContentExtraction:forBrowserContext: | `@32@0:8r^v16@24` | 0x4435cdc | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x4435cdc](../bodies/0x4435cdc.c); record 0xd77bae0; dyld L2703 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -browser_context_page_content_extraction | `{optional<arc::BrowserContext::PageContentExtraction>=(?=c{PageContentExtraction={shared_ptr<arc::BrowserContext::PageContentExtraction::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x440a2ac | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x440a2ac](../bodies/0x440a2ac.c); record 0xd77baec; dyld L2704 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -owner | `@16@0:8` | 0x43fe200 | Returns native/inline field. | No direct anonymous call recorded / indirect possible | [0x43fe200](../bodies/0x43fe200.c); record 0xd77baf8; dyld L2705 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -setOwner: | `v24@0:8@16` | 0x43fe218 | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x43fe218](../bodies/0x43fe218.c); record 0xd77bb04; dyld L2706 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4435e2c | Destroys backing storage with shared-count decrement and last-owner virtual destruction; explicitly destroys weak ObjC storage. ARC injection may hide retain/release operations. | 0x5810 | [0x4435e2c](../bodies/0x4435e2c.c); record 0xd77bb10; dyld L2707 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x440a4dc | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x440a4dc](../bodies/0x440a4dc.c); record 0xd77bb1c; dyld L2708 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |
| Extensions | -isEqual: | `B24@0:8@16` | 0x43f96c8 | Compares native service handle identity after class/optional checks. | 0x5810 ; ObjC selectors isKindOfClass: | [0x43f96c8](../bodies/0x43f96c8.c); record 0xd7786e4; dyld L4933 | medium / reviewed_partial / available | Validate empty optional/detached service equality and hash consistency. |
| Extensions | -hash | `Q16@0:8` | 0x43f999c | Requires engaged native service and hashes its pointer via 0x18f3c. | 0x4b08a00,0x18f3c,0x5810 | [0x43f999c](../bodies/0x43f999c.c); record 0xd7786f0; dyld L4934 | medium / reviewed_partial / available | Validate empty optional/detached service equality and hash consistency. |
| Extensions | -getExtractedContentForWebContents:completion: | `v32@0:8@16@?24` | 0x43f9a98 | Extracts engaged service and content handles, captures completion block and calls 0x42f0254; unavailable native profile/contents invokes empty result. Deeper pointer expressions are suspect. | 0x4b08a00,0x8790,0x42f0254,0x5810 | [0x43f9a98](../bodies/0x43f9a98.c); record 0xd7786fc; dyld L4935; evidence/bridges-services.txt:0x42f0254 | medium / reviewed_partial / available | Recover success result transformation, callback lifetime/thread and actual extraction implementation. |

## Remaining implementation questions

Recover successful callback trampoline, extraction agent/IPC endpoint and time/redaction/cross-origin policy.
