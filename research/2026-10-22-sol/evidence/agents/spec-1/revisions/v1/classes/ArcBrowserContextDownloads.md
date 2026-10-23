# ArcBrowserContextDownloads

ArcCore 1.170.0 arm64. 13 declared rows; superclass `NSObject`. Class metadata `0xe37d9c8`, class_ro `0xe2ac500`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-owned native download service wrapper with app delegate bridge.

## State and layout

Metadata instance start 8, size 56 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_owner` | 0x8 | 8 | `@"ArcBrowserContext"` | `0xe2ac468` |
| `_downloadsDelegate` | 0x10 | 16 | `{shared_ptr<BrowserContextDownloadsDelegateBridge>="__ptr_"^{BrowserContextDownloadsDelegateBridge}"__cntrl_"^{__shared_weak_count}}` | `0xe2ac488` |
| `_browser_context_downloads` | 0x20 | 24 | `{optional<arc::BrowserContext::Downloads>=""(?="__null_state_"c"__val_"{Downloads="impl_"{shared_ptr<arc::BrowserContext::Downloads::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ac4a8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `browser_context_downloads`: `T{optional<arc::BrowserContext::Downloads>=(?=c{Downloads={shared_ptr<arc::BrowserContext::Downloads::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browser_context_downloads` at `0xe2ac4d0`.
- `owner`: `T@"ArcBrowserContext",W,N,V_owner` at `0xe2ac4e0`.
- `downloadsDelegate`: `T{shared_ptr<BrowserContextDownloadsDelegateBridge>=^{BrowserContextDownloadsDelegateBridge}^{__shared_weak_count}},N,V_downloadsDelegate` at `0xe2ac4f0`.

## Lifecycle, ownership, callbacks and threading

0x4436f8c / 0x44370b0 copy optional native Downloads at +0x20; second initializer stores owner at +8. Metadata declares owner weak (property 0xe2ac4e0), and destructor 0x43fe3c0 explicitly calls objc_destroyWeak(+8). setDelegate category 0x4041fec creates a shared bridge with weak app delegate and weak owner using objc_initWeak; existing bridge path updates delegate. Nil path only releases a temporary bridge reference in visible code, so do not promise unregister-on-nil. startDownload:error: 0x40422b0 converts NSURL.absoluteString and calls 0x43328cc.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Native 0x43328cc returns “This object is detached”; deeper 0x836ba70 returns “Download manager is not available” or constructs/submits native download parameters. Both cite arc/browser/browser_context/downloads_impl.cc. CEF 426920c33ef22551e0b8f48cf343d623ac25642d include/cef_browser.h:512 offers browser-host StartDownload; that is a source-backed candidate operation, not profile-service/delegate/error-contract equivalence.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithBrowserContextDownloads: | `@24@0:8r^v16` | 0x4436f8c | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x4436f8c](../bodies/0x4436f8c.c); record 0xd77bc90; dyld L2743 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -initWithBrowserContextDownloads:forBrowserContext: | `@32@0:8r^v16@24` | 0x44370b0 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x44370b0](../bodies/0x44370b0.c); record 0xd77bc9c; dyld L2744 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -browser_context_downloads | `{optional<arc::BrowserContext::Downloads>=(?=c{Downloads={shared_ptr<arc::BrowserContext::Downloads::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43fe184 | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43fe184](../bodies/0x43fe184.c); record 0xd77bca8; dyld L2745 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -owner | `@16@0:8` | 0x43fe200 | Returns native/inline field. | No direct anonymous call recorded / indirect possible | [0x43fe200](../bodies/0x43fe200.c); record 0xd77bcb4; dyld L2746 | medium / local_verified / available | Producer semantics and enum meanings are not established by the field load. |
| (base) | -setOwner: | `v24@0:8@16` | 0x43fe218 | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x43fe218](../bodies/0x43fe218.c); record 0xd77bcc0; dyld L2747 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -downloadsDelegate | `{shared_ptr<BrowserContextDownloadsDelegateBridge>=^{BrowserContextDownloadsDelegateBridge}^{__shared_weak_count}}16@0:8` | 0x43fe224 | Returns shared bridge pair by value; increments nonnull control strong count. | No direct anonymous call recorded / indirect possible | [0x43fe224](../bodies/0x43fe224.c); record 0xd77bccc; dyld L2748 | medium / local_verified / available | Bridge event selectors and app delegate lifetime require separate audit. |
| (base) | -setDownloadsDelegate: | `v32@0:8{shared_ptr<BrowserContextDownloadsDelegateBridge>=^{BrowserContextDownloadsDelegateBridge}^{__shared_weak_count}}16` | 0x43fe244 | Replaces shared bridge pair, incrementing new control count and releasing prior shared owner. Ghidra parameter typing is imperfect. | 0x5810 | [0x43fe244](../bodies/0x43fe244.c); record 0xd77bcd8; dyld L2749 | medium / local_verified / available | Validate shared_ptr aggregate calling convention. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43fe3c0 | Destroys backing storage with shared-count decrement and last-owner virtual destruction; explicitly destroys weak ObjC storage. ARC injection may hide retain/release operations. | 0x5810 | [0x43fe3c0](../bodies/0x43fe3c0.c); record 0xd77bce4; dyld L2750 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43fe528 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43fe528](../bodies/0x43fe528.c); record 0xd77bcf0; dyld L2751 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |
| Extensions | -isEqual: | `B24@0:8@16` | 0x4041e2c | Identity fast path; class/optional checks then 0x411db00 compares native Downloads handles. | 0x411db00,0x4332750 ; ObjC selectors isKindOfClass: | [0x4041e2c](../bodies/0x4041e2c.c); record 0xd773e4c; dyld L4786 | medium / reviewed_partial / available | Inspect comparison helper; do not assume URL/content equality. |
| Extensions | -hash | `Q16@0:8` | 0x4041f6c | Requires native engagement; hashes native Downloads via 0x4332988. | 0x4332988,0x4332750,0x4b08a00 | [0x4041f6c](../bodies/0x4041f6c.c); record 0xd773e58; dyld L4787 | medium / reviewed_partial / available | Confirm hash matches equality helper and detachment behavior. |
| Extensions | -setDelegate: | `v24@0:8@16` | 0x4041fec | Creates native shared bridge with weak delegate/owner on first nonnil registration; attaches it via 0x4332814. Existing bridge updates weak delegate. Nil branch shows temporary release only. | 0x2527d18,0x8790,0x4332814,0x4332750 ; ObjC selectors setDownloadsDelegate: | [0x4041fec](../bodies/0x4041fec.c); record 0xd773e64; dyld L4788; evidence/bridges-embedding.txt | medium / reviewed_partial / available | Prove unregister-on-nil semantics, callback selectors and owner teardown. |
| Extensions | -startDownload:error: | `v32@0:8@16^@24` | 0x40422b0 | Converts NSURL.absoluteString to native string, calls 0x43328cc and converts non-success result into optional NSError out parameter. Native helper distinguishes detached object and missing download manager. | 0x3b60ee8,0x43328cc,0x5810,0x4332750,0x44025f4,0x4b35898 | [0x40422b0](../bodies/0x40422b0.c); record 0xd773e70; dyld L4789; evidence/bridges-embedding.txt | medium / reviewed_partial / available | Identify ArcErrorDomain codes, URL validation and callback relationship. |

## Remaining implementation questions

Does nil actually unregister a delegate? Recover download-created/updated callbacks, error-domain codes and ownership of downloads outside a WebContents.
