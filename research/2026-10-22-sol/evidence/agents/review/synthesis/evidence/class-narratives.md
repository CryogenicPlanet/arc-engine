
### spec-1: ArcAddressEntrySaveRequest
# ArcAddressEntrySaveRequest

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37e418`, class_ro `0xe2ae778`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Save/cancel handle for an address-entry offer, backed by an optional shared native request.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `address_entry_save_request`: `T{optional<arc::AddressEntrySaveRequest>=(?=c{AddressEntrySaveRequest={shared_ptr<arc::AddressEntrySaveRequest::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_address_entry_save_request` at `0xe2ae768`.

## Lifecycle, ownership, callbacks and threading

Initializer 0x443ebe4 copies the shared peer; 0x43df69c decrements it on destruction. save 0x443ed08 and cancel 0x443ed1c trap on an empty wrapper, then call 0x4330cdc / 0x4330d38. Those helpers send decision integers 2 / 3 to 0x7c6a394 and then call 0x7c6a4f4; the wrapper itself does not reset its optional. Do not infer one-shot behavior from the selector.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The 0x4330cdc / 0x4330d38 call chain is an autofill-decision integration gate. Exact Chromium callback enum and prompt-dismissal ownership remain unidentified; a CEF generic form-fill API is not demonstrated to reproduce it.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Does native completion consume the offer, can save/cancel repeat safely, and who dismisses/releases the app prompt?
FRESH REVIEW: Completion consumption, repeated decisions, cancellation and prompt ownership remain open.

### spec-1: ArcAdhocHistogramSample
# ArcAdhocHistogramSample

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37c258`, class_ro `0xe2a6b30`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Mutable sample record carrying unsigned type and signed 32-bit value.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `type`: `TQ,N,V_type` at `0xe2a6b10`.
- `value`: `Ti,N,V_value` at `0xe2a6b20`.

## Lifecycle, ownership, callbacks and threading

0x44034ac calls super init and stores type at +0x10 and value at +8. The independent accessors return/store those fields (0x2cb140, 0x416803c, 0x4081a30, 0x44034fc). No callback is introduced by these bodies.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This record can be represented by an adapter; the producer defines the histogram type vocabulary. Collection validation at ArcAdhocHistogramSamples 0x4403504 permits only 0..4.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover type names and metric units from the producer, without treating the local record as a metrics backend.
FRESH REVIEW: Producer type vocabulary and metric units are unknown.

### spec-1: ArcAdhocHistogramSamples
# ArcAdhocHistogramSamples

ArcCore 1.170.0 arm64. 3 declared rows; superclass `NSObject`. Class metadata `0xe37c2a8`, class_ro `0xe2a6c00`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Collection adapter from native adhoc histogram samples into ObjC records.

## State and layout

Metadata instance start 8, size 16 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `samples`: `T@"NSMutableArray",R,N,V_samples` at `0xe2a6bf0`.

## Lifecycle, ownership, callbacks and threading

0x4403504 allocates NSMutableArray and wraps each native 8-byte type/value pair in ArcAdhocHistogramSample; type >4 traps. samples 0x2d0fb40 returns the stored array, not a defensive copy; destructor 0x4166068 clears/releases _samples at self+8 by passing that slot and nil to objc_storeStrong through 0x8c7df80. There is no supplied replacement argument; its exact metadata type remains v16@0:8. The indexed body is named for an unrelated class sharing this IMP.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

No Chrome/CEF service call is present in 0x4403504; compatibility concerns here are sample ordering, numeric widths and returned collection mutability. The metrics producer remains outside this class.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Collection mutability, producer container ABI, release side effects and destruction sequence are not fully recovered.

### spec-1: ArcBackForwardListItem
# ArcBackForwardListItem

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37bc18`, class_ro `0xe2a46e8`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Shared native history-entry wrapper exposing title, URL and validity.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeBackForwardListItem`: `T{optional<arc::BackForwardListItem>=(?=c{BackForwardListItem={shared_ptr<arc::BackForwardListItem::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_nativeBackForwardListItem` at `0xe2a46a8`.
- `title`: `T@"NSString",R,N` at `0xe2a46b8`.
- `url`: `T@"NSURL",R,N` at `0xe2a46c8`.
- `isValid`: `TB,R,N` at `0xe2a46d8`.

## Lifecycle, ownership, callbacks and threading

0x43df298 copies a shared native peer into optional storage. 0x43df3bc converts UTF-16 title; 0x43df47c converts the URL through 0x43abd34. isValid 0x43df524 reads native +0x30 after checking engagement; it is not a wrapper liveness test. 0x43df57c assigns/clears the optional and 0x43df69c releases its control block.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Navigation entry data can be adapted from Chromium content APIs, but these shared Arc wrappers and native equality/invalidity semantics are an additional contract. No direct CEF equivalence has been established.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Native entry invalidation, converter semantics and aggregate calling convention remain incomplete.

### spec-1: ArcBookmarkImportTargets
# ArcBookmarkImportTargets

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37bf88`, class_ro `0xe2a56e0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Two bookmark-node destinations, one for bookmark-bar entries and one for other bookmarks.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `targetNodeForBookmarkBarBookmarks`: `T@"ArcBookmarkTreeNode",&,N,V_targetNodeForBookmarkBarBookmarks` at `0xe2a56c0`.
- `targetNodeForOtherBookmarks`: `T@"ArcBookmarkTreeNode",&,N,V_targetNodeForOtherBookmarks` at `0xe2a56d0`.

## Lifecycle, ownership, callbacks and threading

0x43ed544 stores the two supplied objects. Metadata property records 0xe2a56c0 / 0xe2a56d0 declare strong nonatomic ownership; 0x43dd5a4 clears both fields. Retain/store operations have been injected away in pseudocode. No import is started here.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

An import adapter must preserve destination identity; node creation and Chrome bookmark-model ownership lie outside these six methods. The class does not establish a persistence transaction or a CEF bookmark capability.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Import transactions and bookmark-model identity/lifetime are outside these accessors.

### spec-1: ArcBrowserContextDownloads
# ArcBrowserContextDownloads

ArcCore 1.170.0 arm64. 13 declared rows; superclass `NSObject`. Class metadata `0xe37d9c8`, class_ro `0xe2ac500`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-owned native download service wrapper with app delegate bridge.

## State and layout

Metadata instance start 8, size 56 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


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


## Remaining implementation questions

Does nil actually unregister a delegate? Recover download-created/updated callbacks, error-domain codes and ownership of downloads outside a WebContents.
FRESH REVIEW: Registration completion, download callback schemas, executor and queued work after teardown remain unknown.

### spec-1: ArcBrowserContextExtensions
# ArcBrowserContextExtensions

ArcCore 1.170.0 arm64. 25 declared rows; superclass `NSObject`. Class metadata `0xe37bfd8`, class_ro `0xe2a5950`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-scoped browser-extension facade, not the ObjC category named Extensions.

## State and layout

Metadata instance start 8, size 56 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `owner`: `T@"ArcBrowserContext",W,N,V_owner` at `0xe2a5920`.
- `extensionsDelegate`: `T{shared_ptr<(anonymous namespace)::BrowserContextExtensionsDelegateBridge>=^{BrowserContextExtensionsDelegateBridge}^{__shared_weak_count}},N,V_extensionsDelegate` at `0xe2a5930`.
- `browserContextExtensions`: `T{optional<arc::BrowserContext::Extensions>=(?=c{Extensions={shared_ptr<arc::BrowserContext::Extensions::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browserContextExtensions` at `0xe2a5940`.

## Lifecycle, ownership, callbacks and threading

0x43fc8e0 copies native optional and stores weak owner (+8); 0x43fe3c0 destroys native/bridge control blocks and weak owner. setDelegate 0x43fcbd8 holds a shared bridge with weak app delegate and owner, updates existing delegate, and explicitly clears bridge pair on nil. That differs from the Downloads category setter. isExtensionSystemReady 0x43fce00 queries a keyed service and returns false if unavailable. Install calls 0x43fd304/0x43fd46c capture completion blocks; delivery/error shaping remain open.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Operations reach profile extension services: enable/reload/uninstall call 0x6afd2fc / 0x6afdb60 / 0x6afe0e0; list/info/commands/background JS call Arc bridge helpers listed below. Pinned-order getter 0x43fdb20→0x42eb484 copies native ordered IDs; setter 0x43fdd3c→0x42eb57c deduplicates input and merges native order before deeper update. Source-backed candidate: Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c chrome/browser/ui/toolbar/toolbar_actions_model.h:124 and .cc:688,764 describe action visibility/pinned ordering; exact Arc service identity/ordinal layout is not established. No Chrome Web Store or command-system equivalence is proven for CEF.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover install completion result/error schemas, ready/loaded event ordering, extension command dispatch, policy/incognito constraints and persistence of pin ordering.
FRESH REVIEW: Native unregister completion, extension service identity, installation results and persistence are not certified.

### spec-1: ArcBrowserContextPageContentExtraction
# ArcBrowserContextPageContentExtraction

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37d8d8`, class_ro `0xe2ac170`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-scoped native extraction service wrapper and asynchronous request adapter.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


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


## Remaining implementation questions

Recover successful callback trampoline, extraction agent/IPC endpoint and time/redaction/cross-origin policy.
FRESH REVIEW: Successful extraction continuation, renderer schema/redaction and callback sequence remain unresolved.

### spec-1: ArcCommandBarRankingCandidate
# ArcCommandBarRankingCandidate

ArcCore 1.170.0 arm64. 19 declared rows; superclass `NSObject`. Class metadata `0xe37d838`, class_ro `0xe2abe90`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline ranking-input feature record, including strings, counts, engagement, match flags and title embedding.

## State and layout

Metadata instance start 8, size 184 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_candidate`: `T{optional<arc::CommandBarRankingCandidate>=(?=c{CommandBarRankingCandidate={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}iiifBBB{vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B},R,N,V_command_bar_ranking_candidate` at `0xe2abdb0`.
- `candidateID`: `T@"NSString",R,C,N` at `0xe2abdc0`.
- `archetype`: `T@"NSString",R,C,N` at `0xe2abdd0`.
- `destination`: `T@"NSString",R,C,N` at `0xe2abde0`.
- `title`: `T@"NSString",R,C,N` at `0xe2abdf0`.
- `url`: `T@"NSString",R,C,N` at `0xe2abe00`.
- `lastNavSeconds`: `Ti,R,N` at `0xe2abe10`.
- `freqEver`: `Ti,R,N` at `0xe2abe20`.
- `typedCount`: `Ti,R,N` at `0xe2abe30`.
- `siteEngagement`: `Tf,R,N` at `0xe2abe40`.
- `isExactURLMatch`: `TB,R,N` at `0xe2abe50`.
- `isBaseDomainMatch`: `TB,R,N` at `0xe2abe60`.
- `isTitlePrefix`: `TB,R,N` at `0xe2abe70`.
- `titleEmbedding`: `T@"NSArray",R,C,N` at `0xe2abe80`.

## Lifecycle, ownership, callbacks and threading

0x4434540 converts five ObjC strings (nil becomes empty), stores integer/float/boolean features and converts title embedding through 0x443181c. 0x4434900 copies native candidate using 0x42a3ea8. mutableCommandBarRankingCandidate 0x4434524 returns inline address when engaged; 0x4434af0 boxes the float-vector embedding into a new array. 0x4434c14 destroys owned candidate state through 0x42a3ff4.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is feature transport, not a ranking algorithm. CandidateID/archetype/destination/title/url are distinct strings (0x4434974..0x4434a34); lastNavSeconds/freqEver/typedCount are int, siteEngagement float and three match flags bytes (0x4434a64..0x4434adc). Preserve type distinctions when replacing the model adapter.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover archetype/destination vocabulary, count/time units, feature normalization and embedding dimension requirements.
FRESH REVIEW: No argument-to-field ABI certification; borrowed storage lifetime, units, embedding dimensions and model behavior remain open.

### spec-1: ArcCommandBarRankingInput
# ArcCommandBarRankingInput

ArcCore 1.170.0 arm64. 7 declared rows; superclass `NSObject`. Class metadata `0xe37d748`, class_ro `0xe2abaf0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline optional vector of float query-embedding components.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_input`: `T{optional<arc::CommandBarRankingInput>=(?=c{CommandBarRankingInput={vector<float, std::allocator<float>>={__vector_layout<float, std::allocator<float>>=^fQQ}}})B},R,N,V_command_bar_ranking_input` at `0xe2abad0`.
- `queryEmbedding`: `T@"NSArray",R,C,N` at `0xe2abae0`.

## Lifecycle, ownership, callbacks and threading

0x4432bfc converts an ObjC collection through 0x443181c into native float-vector storage; queryEmbedding 0x4432e20 creates NSMutableArray and boxes each float. mutableCommandBarRankingInput 0x4432be0 returns inline vector address when engaged. 0x4432f6c destroys vector allocation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The adapter preserves float order; model dimensions and input validation belong to conversion helper/model, not this getter. No stock Chromium/CEF inference API equivalence is asserted.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover vector length/dimension validation and NSNumber conversion policy in 0x443181c.
FRESH REVIEW: Conversion validation, dimensions, borrowed pointer lifetime and optional vector ABI remain open.

### spec-1: ArcCommandBarRankingResult
# ArcCommandBarRankingResult

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37d6a8`, class_ro `0xe2ab920`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline candidate-id string and float score record.

## State and layout

Metadata instance start 8, size 48 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `command_bar_ranking_result`: `T{optional<arc::CommandBarRankingResult>=(?=c{CommandBarRankingResult={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}f})B},R,N,V_command_bar_ranking_result` at `0xe2ab8f0`.
- `candidateID`: `T@"NSString",R,C,N` at `0xe2ab900`.
- `score`: `Tf,R,N` at `0xe2ab910`.

## Lifecycle, ownership, callbacks and threading

0x44326cc constructs string/score and engages optional. candidateID 0x43ed470 converts UTF-8 string; score 0x4432890 loads float +0x20. mutableCommandBarRankingResult aliases 0x442c260 used by ArcVisitCategory; identical code does not make the score an integer. Native getters/copy constructors retain their own metadata encodings.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This wrapper carries ranking output; no inference or ranking model runs in 0x44326cc. The Arc ranking producer is outside this assignment and cannot be replaced by a CEF navigation suggestion API based only on these fields.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Native aggregate ABI, borrowed pointer validity and ranking producer semantics remain incomplete.

### spec-1: ArcContentNode
# ArcContentNode

ArcCore 1.170.0 arm64. 17 declared rows; superclass `NSObject`. Class metadata `0xe37d5b8`, class_ro `0xe2ab5a0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Recursive extracted-content snapshot with node attributes and optional form-control detail.

## State and layout

Metadata instance start 8, size 448 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `content_node`: `T{optional<arc::ContentNode>=(?=c{ContentNode=i{vector<arc::ContentNode, std::allocator<arc::ContentNode>>={__vector_layout<arc::ContentNode, std::allocator<arc::ContentNode>>=^{ContentNode}QQ}}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}BiI{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<arc::FormControlInfo>=(?=c{FormControlInfo=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{vector<arc::SelectOption, std::allocator<arc::SelectOption>>={__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>=^{SelectOption}QQ}}BBB})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}})B},R,N,V_content_node` at `0xe2ab4c0`.
- `type`: `Tq,R,N` at `0xe2ab4d0`.
- `children`: `T@"NSArray",R,N,V_children` at `0xe2ab4e0`.
- `textContent`: `T@"NSString",R,C,N` at `0xe2ab4f0`.
- `hasEmphasis`: `TB,R,N` at `0xe2ab500`.
- `textSize`: `Tq,R,N` at `0xe2ab510`.
- `textColor`: `T@"NSString",R,C,N` at `0xe2ab520`.
- `linkURL`: `T@"NSString",R,C,N` at `0xe2ab530`.
- `imageAlt`: `T@"NSString",R,C,N` at `0xe2ab540`.
- `iframeOrigin`: `T@"NSString",R,C,N` at `0xe2ab550`.
- `formName`: `T@"NSString",R,C,N` at `0xe2ab560`.
- `formActionURL`: `T@"NSString",R,C,N` at `0xe2ab570`.
- `tableName`: `T@"NSString",R,C,N` at `0xe2ab580`.
- `formControlInfo`: `T@"ArcFormControlInfo",R,N,V_formControlInfo` at `0xe2ab590`.

## Lifecycle, ownership, callbacks and threading

0x4430960 destroys/replaces engaged inline native node using 0x4295208 / 0x429539c. children 0x44309e8 lazily wraps each 0x1a0-byte native child in ArcContentNode and caches a mutable array. formControlInfo 0x44311e0 lazily wraps native optional control at self+0xc8 and caches it. textContent/linkURL/imageAlt/iframeOrigin/formName/formActionURL/tableName (0x4430ac4..0x44310ec) convert optional native strings; absence yields nil. textColor 0x4430bdc instead requires the outer node optional, reads the unsigned color at self+0x60, returns nil for zero and otherwise formats an NSString with #%08X. Instructions at 0x4430c08 and 0x4430c0c/0x4430c10 recover the color vararg and format constant at 0xe244ab8 that pseudocode omits. No channel order, alpha meaning or color space is established.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This Arc tree schema is an additional extraction contract. Browser/renderer extraction and redaction semantics are not established by wrapper fields. Chromium/CEF DOM or accessibility trees must not be assumed structurally equivalent.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover node-type enum, optional-field validity by node type, cross-origin rules, redaction and producer limits.
FRESH REVIEW: Color channel/alpha/color-space meaning, producer zero interpretation, extraction policy and aggregate ABI remain unknown.

### spec-1: ArcExtensionRuntimeError
# ArcExtensionRuntimeError

ArcCore 1.170.0 arm64. 11 declared rows; superclass `NSObject`. Class metadata `0xe37d478`, class_ro `0xe2aafc0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned extension runtime-error snapshot with lazily cached stack-frame wrappers.

## State and layout

Metadata instance start 8, size 152 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `extension_runtime_error`: `T{optional<arc::ExtensionRuntimeError>=(?=c{ExtensionRuntimeError={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}Bi{vector<arc::StackFrame, std::allocator<arc::StackFrame>>={__vector_layout<arc::StackFrame, std::allocator<arc::StackFrame>>=^{StackFrame}QQ}}})B},R,N,V_extension_runtime_error` at `0xe2aaf40`.
- `extensionID`: `T@"NSString",R,C,N` at `0xe2aaf50`.
- `contextURL`: `T@"NSURL",R,N` at `0xe2aaf60`.
- `message`: `T@"NSString",R,C,N` at `0xe2aaf70`.
- `source`: `T@"NSString",R,C,N` at `0xe2aaf80`.
- `fromIncognito`: `TB,R,N` at `0xe2aaf90`.
- `severity`: `Tq,R,N` at `0xe2aafa0`.
- `stackTrace`: `T@"NSArray",R,N,V_stackTrace` at `0xe2aafb0`.

## Lifecycle, ownership, callbacks and threading

0x442eeb4 copies native error via 0x4335698 and marks engagement. extensionID is UTF-8 (0x442ef28), message/source UTF-16 (0x442ef6c/0x442ef9c); contextURL uses 0x43abd34 (0x442ef58). stackTrace 0x442eff4 wraps native 0x38-byte stack entries in ArcExtensionRuntimeErrorStackFrame and caches array. severity 0x442efe0 is a numeric field; enum names are unresolved.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This record depends on extension error production and observer delivery outside the class. No native exception is raised by reading its message/stack; Chrome extension-service integration is the route gate.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover severity enum, source/context URL semantics and delivery/retention of error notifications.
FRESH REVIEW: Severity names, producer lifetime, notification delivery and aggregate ABI remain unverified.

### spec-1: ArcExtractedPageContent
# ArcExtractedPageContent

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37d3d8`, class_ro `0xe2aad50`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Extracted page snapshot containing URL/title, extractionTime and recursive root node.

## State and layout

Metadata instance start 8, size 496 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `extracted_page_content`: `T{optional<arc::ExtractedPageContent>=(?=c{ExtractedPageContent={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}d{ContentNode=i{vector<arc::ContentNode, std::allocator<arc::ContentNode>>={__vector_layout<arc::ContentNode, std::allocator<arc::ContentNode>>=^{ContentNode}QQ}}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}BiI{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<arc::FormControlInfo>=(?=c{FormControlInfo=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{vector<arc::SelectOption, std::allocator<arc::SelectOption>>={__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>=^{SelectOption}QQ}}BBB})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}}})B},R,N,V_extracted_page_content` at `0xe2aad00`.
- `url`: `T@"NSString",R,C,N` at `0xe2aad10`.
- `title`: `T@"NSString",R,C,N` at `0xe2aad20`.
- `rootNode`: `T@"ArcContentNode",R,N,V_rootNode` at `0xe2aad30`.
- `extractionTime`: `Td,R,N` at `0xe2aad40`.

## Lifecycle, ownership, callbacks and threading

0x442e9d4 copies native page aggregate through 0x4296004 into optional at +0x10. url/title 0x442ea78/0x442eaa8 convert native UTF-8 strings. rootNode 0x442ead8 lazily creates ArcContentNode from inline self+0x48 and caches it. extractionTime 0x442eb38 reads a double without conversion; its units are not inferable from the name.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

ArcBrowserContextPageContentExtraction callback result is the likely producer relationship, but exact successful-result trampoline and full native extraction schema need further audit. No CEF extraction equivalence is claimed.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Identify extractionTime units/time basis, failure-to-nil rules, snapshot completeness and root-node limits.
FRESH REVIEW: Extraction time basis, successful producer path, snapshot completeness and limits remain open.

### spec-1: ArcFindResult
# ArcFindResult

ArcCore 1.170.0 arm64. 3 declared rows; superclass `NSObject`. Class metadata `0xe37cc08`, class_ro `0xe2a9180`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Snapshot result with signed 64-bit match count and active ordinal.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `numMatches`: `Tq,R,N,V_numMatches` at `0xe2a9160`.
- `activeMatchOrdinal`: `Tq,R,N,V_activeMatchOrdinal` at `0xe2a9170`.

## Lifecycle, ownership, callbacks and threading

0x440e120 sign-extends two native int fields into self+8/+0x10; 0x2d0fb40 and 0x2cb140 return them. ArcWebContents find callback trampoline 0x444510c allocates this object and passes it to the captured block.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The class is a result adapter; find-request sequencing and cancellation belong to ArcWebContents 0x4442354 / native 0x43770c0. No independent backend lives in this record.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Does the producer use a special ordinal for zero matches, and can multiple callbacks be delivered per request?
FRESH REVIEW: Match-ordinal sentinels, request ordering, repeated callbacks and producer lifetime remain unknown.

### spec-1: ArcFormControlInfo
# ArcFormControlInfo

ArcCore 1.170.0 arm64. 12 declared rows; superclass `NSObject`. Class metadata `0xe37cfc8`, class_ro `0xe2a9f80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned extracted form-control snapshot with lazily cached select-option objects.

## State and layout

Metadata instance start 8, size 160 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `form_control_info`: `T{optional<arc::FormControlInfo>=(?=c{FormControlInfo=i{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{optional<std::string>=(?=c{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})})B}{vector<arc::SelectOption, std::allocator<arc::SelectOption>>={__vector_layout<arc::SelectOption, std::allocator<arc::SelectOption>>=^{SelectOption}QQ}}BBB})B},R,N,V_form_control_info` at `0xe2a9ef0`.
- `controlType`: `Tq,R,N` at `0xe2a9f00`.
- `fieldName`: `T@"NSString",R,C,N` at `0xe2a9f10`.
- `fieldValue`: `T@"NSString",R,C,N` at `0xe2a9f20`.
- `placeholder`: `T@"NSString",R,C,N` at `0xe2a9f30`.
- `isChecked`: `TB,R,N` at `0xe2a9f40`.
- `isRequired`: `TB,R,N` at `0xe2a9f50`.
- `isRedacted`: `TB,R,N` at `0xe2a9f60`.
- `selectOptions`: `T@"NSArray",R,N,V_selectOptions` at `0xe2a9f70`.

## Lifecycle, ownership, callbacks and threading

0x442b9ec initializes optional native value via 0x4294be4, not an identified DOM service. fieldName/fieldValue/placeholder 0x442ba74/0x442bb64/0x442bc54 return nil for absent optional native strings. isChecked/isRequired/isRedacted 0x442bd44/0x442bd58/0x442bd6c read stored flags; redaction policy is not implemented by those loads. selectOptions 0x442bd80 lazily creates and caches ArcSelectOption objects from 0x48-byte native elements.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This extraction schema needs a renderer/IPC producer, not merely HTML form automation. No CEF exact form-content extraction schema is established. The wrapper itself exposes no callbacks or persistence writes.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Which fields are redacted, when is the snapshot taken, and does extraction include cross-origin subframes?
FRESH REVIEW: Type vocabulary, redaction policy, subframe inclusion and aggregate ABI remain unresolved.

### spec-1: ArcHSLAMatrix
# ArcHSLAMatrix

ArcCore 1.170.0 arm64. 23 declared rows; superclass `NSObject`. Class metadata `0xe37bb28`, class_ro `0xe2a4388`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline 20-float matrix value; exact ivar and class_ro metadata anchors appear below.

## State and layout

Metadata instance start 8, size 88 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `hslaMatrix`: `T{HSLAMatrix=ffffffffffffffffffff},N,V_hslaMatrix` at `0xe2a4238`.
- `m0`: `Tf,R,N` at `0xe2a4248`.
- `m1`: `Tf,R,N` at `0xe2a4258`.
- `m2`: `Tf,R,N` at `0xe2a4268`.
- `m3`: `Tf,R,N` at `0xe2a4278`.
- `m4`: `Tf,R,N` at `0xe2a4288`.
- `m5`: `Tf,R,N` at `0xe2a4298`.
- `m6`: `Tf,R,N` at `0xe2a42a8`.
- `m7`: `Tf,R,N` at `0xe2a42b8`.
- `m8`: `Tf,R,N` at `0xe2a42c8`.
- `m9`: `Tf,R,N` at `0xe2a42d8`.
- `m10`: `Tf,R,N` at `0xe2a42e8`.
- `m11`: `Tf,R,N` at `0xe2a42f8`.
- `m12`: `Tf,R,N` at `0xe2a4308`.
- `m13`: `Tf,R,N` at `0xe2a4318`.
- `m14`: `Tf,R,N` at `0xe2a4328`.
- `m15`: `Tf,R,N` at `0xe2a4338`.
- `m16`: `Tf,R,N` at `0xe2a4348`.
- `m17`: `Tf,R,N` at `0xe2a4358`.
- `m18`: `Tf,R,N` at `0xe2a4368`.
- `m19`: `Tf,R,N` at `0xe2a4378`.

## Lifecycle, ownership, callbacks and threading

0x43ded78 copies 80 bytes from the input matrix after a pointer/alignment check. m1..m19 load successive floats; m0 0x43dee04 has lost its return in pseudocode and is repaired using exact LDR S0,[X0,#8]; RET bytes in binary-checks.tsv. hslaMatrix 0x43dee9c copies 80 bytes unchanged into aggregate return storage X8, independently checked by SIMD load/store simulation (evidence/hsla-return-instructions.tsv); its pseudo-C numeric casts are artifacts. setHslaMatrix 0x43deec0 copies the aggregate fields.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

ArcWebContents applyColorFilter 0x44437c8 obtains the aggregate and forwards to native 0x4379218. Reproducing the object is straightforward; renderer-side matrix semantics and color space remain outside this local value review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate calling convention from app/native callers and renderer matrix interpretation. The 80-byte getter is a verified bitwise copy; no numeric cast should be introduced.
FRESH REVIEW: Renderer color interpretation and cross-toolchain aggregate calls remain untested.

### spec-1: ArcHistogramSample
# ArcHistogramSample

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37c1b8`, class_ro `0xe2a62a0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Snapshot of integer metric value plus three browser-state bits.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `value`: `T@"NSNumber",R,N,V_value` at `0xe2a6260`.
- `browserIsVisible`: `TB,R,N,V_browserIsVisible` at `0xe2a6270`.
- `browserInSession`: `TB,R,N,V_browserInSession` at `0xe2a6280`.
- `browserIsPlayingAudio`: `TB,R,N,V_browserIsPlayingAudio` at `0xe2a6290`.

## Lifecycle, ownership, callbacks and threading

0x4402a3c boxes an int in NSNumber and unpacks bit 0 visible, bit 1 in-session, bit 2 playing-audio into +8/+9/+10. browserIsVisible 0x40b1230 has incoherent empty-return pseudocode; actual words 0x39402000, 0xd65f03c0 prove LDRB w0,[x0,#8]; RET. 0x425b6d8 clears/releases _value (NSNumber) at self+0x10 by passing the slot and nil to objc_storeStrong through 0x8c7df80. This destructor accepts no supplied replacement argument; its exact metadata type remains v16@0:8.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This class preserves capture-time browser-state flags; producer metric sampling and submission are not implemented by the getters. No CEF or Chromium metrics service identity is asserted.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Producer sampling policy, release side effects and destruction sequence remain unknown.

### spec-1: ArcHungRendererDialog
# ArcHungRendererDialog

ArcCore 1.170.0 arm64. 7 declared rows; superclass `NSObject`. Class metadata `0xe37c348`, class_ro `0xe2a6e00`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Handle for renderer-unresponsive prompt actions.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `dialog`: `T{optional<arc::HungRendererDialog>=(?=c{HungRendererDialog={shared_ptr<arc::HungRendererDialog::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_dialog` at `0xe2a6df0`.

## Lifecycle, ownership, callbacks and threading

0x4403b80 stores an optional shared native dialog. crashProcess 0x4403ca4 calls 0x435f150, which obtains a process and forwards to 0x669e6e4 with integer 2. wait 0x4403cb8 calls 0x435f1ac, which invokes a stored refcounted callback if present. Its int-pointer self type makes +6 mean byte offset +0x18; the pseudo-C pid_t return conflicts with ObjC void metadata and is not adopted.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Chrome process termination and prompt lifecycle must be mapped explicitly; CEF unresponsive-renderer callbacks are only a possible adapter route, not demonstrated equivalence.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

What process termination result and metric side effects occur, and does waiting dismiss or rearm the prompt?
FRESH REVIEW: Recovered return type is not adopted; process reason enum, dismissal and callback executor remain unknown.

### spec-1: ArcImageRep
# ArcImageRep

ArcCore 1.170.0 arm64. 17 declared rows; superclass `NSObject`. Class metadata `0xe37c488`, class_ro `0xe2a7250`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native image value with raw pixel, codec and asynchronous decode adapters.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `imageRep`: `T{ImageRep=^^?{shared_ptr<arc::ImageRepImpl>=^{ImageRepImpl}^{__shared_weak_count}}},N,V_imageRep` at `0xe2a7210`.
- `isNull`: `TB,R,N` at `0xe2a7220`.
- `pixelWidth`: `Tq,R,N` at `0xe2a7230`.
- `pixelHeight`: `Tq,R,N` at `0xe2a7240`.

## Lifecycle, ownership, callbacks and threading

init 0x440539c constructs native empty image; 0x440549c copies supplied ImageRep through 0x43380d4. Pixel constructor 0x44054f4 forwards width/height/scale/rowBytes and NSData.bytes to 0x43378d8. dangerouslyDecodeFromData 0x4405700 returns nil on empty native optional. Async decode 0x4405930 captures block and calls 0x4337e00: null/empty data or scale<=0 invokes an empty result inline; valid input is copied and posted to an unresolved task runner, default decode budget 0x8000000 bytes when argument is zero. Callback 0x4405db4 passes nil or a new ArcImageRep.

0x4337e00 proves mixed inline-failure and task-posting behavior. It does not establish the reply sequence or a main-thread guarantee. See evidence/bridges-services.txt and callback trampoline 0x4405db4 in evidence/bridges-callbacks.txt.

## Relations and engine route

copyPixelDataBGRA8888 0x4405ad8 allocates width*height*4 mutable data, returning nil if copy helper fails. Codec calls 0x4405ba8→0x43385c0 and 0x8371eb8→0x8b0218c return NSData; toNSImage 0x4405b64 uses native image conversion. These are source/codec candidates, not exact Chromium symbol matches. CEF image APIs would require parity tests for dimensions, alpha, color, scale and failures.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Does valid async decode reply on caller sequence, how are dimensions clamped, and what are codec enums/quality limits and input-byte bounds?
FRESH REVIEW: Reply sequence, codec/size/NaN edges and teardown behavior require more evidence.

### spec-1: ArcMediaImage
# ArcMediaImage

ArcCore 1.170.0 arm64. 9 declared rows; superclass `NSObject`. Class metadata `0xe37c578`, class_ro `0xe2a7540`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native media image wrapper plus separately allocated CGSize buffer.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeMediaImage`: `T{MediaImage={shared_ptr<media_session::MediaImage>=^{MediaImage}^{__shared_weak_count}}},N,V_nativeMediaImage` at `0xe2a7500`.
- `src`: `T@"NSString",R,N` at `0xe2a7510`.
- `type`: `T@"NSString",R,N` at `0xe2a7520`.
- `sizes`: `T^{CGSize=dd},R,N,V_sizes` at `0xe2a7530`.

## Lifecycle, ownership, callbacks and threading

0x4407384 copies shared media image and allocates 16 bytes per native size, converting integer width/height to double. sizes 0x2d0fb40 returns the raw buffer pointer; this class declares no size-count method. dealloc 0x44074d0 frees that allocation before super dealloc. 0x4407624 releases shared peer storage. src 0x440751c returns UTF-8 text and type 0x44075d4 converts UTF-16 text.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Raw-pointer size enumeration and its lifetime are Arc ABI requirements. Native metadata identifies media_session::MediaImage; a CEF metadata/image adapter would still need an explicit count/buffer-ownership contract.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

How does the app determine sizes count, and can setNativeMediaImage desynchronize the cached size buffer?
FRESH REVIEW: Borrowed size buffer lifetime, count protocol and native-setter cache invalidation remain open.

### spec-1: ArcMediaPosition
# ArcMediaPosition

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37e0f8`, class_ro `0xe2ade80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared media_session::MediaPosition object with time projections.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `media_position`: `T{optional<arc::MediaPosition>=(?=c{MediaPosition={shared_ptr<media_session::MediaPosition>=^{MediaPosition}^{__shared_weak_count}}})B},R,N,V_media_position` at `0xe2ade30`.
- `playbackRate`: `Td,R,N` at `0xe2ade40`.
- `duration`: `Td,R,N` at `0xe2ade50`.
- `currentTime`: `Td,R,N` at `0xe2ade60`.
- `endOfMedia`: `TB,R,N` at `0xe2ade70`.

## Lifecycle, ownership, callbacks and threading

0x443c1dc copies a shared native position. playbackRate 0x443c300 loads native double; duration 0x443c318 divides native microseconds by 1e6, preserving signed infinity sentinels. currentTime 0x443c36c calls 0x32b5dd0 then applies the same conversion. That helper computes elapsed-time*rate, adds base position and clamps to [0,duration]; its pseudo-C return type is wrong, so source identity remains an inference. endOfMedia 0x443c3cc reads a separate flag, not currentTime==duration.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: 0x32b5dd0 corresponds to MediaPosition::GetPosition/GetPositionAtTime logic at Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c services/media_session/public/cpp/media_position.cc:43-77; private field order in media_position.h:98-110 agrees with the metadata type and observed offsets. The header :63-67 explicitly says end_of_media may be true before duration is reached. No CEF exact timing contract is established.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Confirm time source and saturated arithmetic against bounded ARM64; test paused, negative-rate, infinite-duration and end-of-media cases.
FRESH REVIEW: Exact symbol identity, time source, saturation edges and aggregate ABI remain unverified.

### spec-1: ArcMetrics
# ArcMetrics

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37d2e8`, class_ro `0xe2aa950`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-scoped site-engagement snapshot adapter.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `browser_context_metrics`: `T{optional<arc::BrowserContext::Metrics>=(?=c{Metrics={shared_ptr<arc::BrowserContext::Metrics::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browser_context_metrics` at `0xe2aa940`.

## Lifecycle, ownership, callbacks and threading

0x442dcf0 copies native metrics peer. getAllSiteEngagementDetails 0x442de14 calls 0x42efb64, wraps native URL/score pairs into ArcSiteEngagementDetails and returns array. 0x42efb64 returns empty vector if profile/service unavailable; otherwise a service lookup and 0x6f26c80 populate details.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: this resembles Chromium site_engagement::SiteEngagementService::GetAllDetails at d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/site_engagement/content/site_engagement_service.h:121,169. This is a Chrome keyed-service route requirement, not a verified symbol match. Source :69-73 requires that service on UI thread; that requirement is conditional on this identification, not proof of Arc caller-thread enforcement.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Confirm exact service getter/score selection and whether hidden/incognito profile entries are included.
FRESH REVIEW: Exact keyed service and score selection are inferred; source UI-thread requirements do not establish Arc enforcement.

### spec-1: ArcNavigation
# ArcNavigation

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37cd48`, class_ro `0xe2a96a0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native navigation-event identity wrapper and captured response-header dictionary.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `nativeNavigation`: `T{optional<arc::WebContents::Navigation>=(?=c{Navigation={shared_ptr<arc::WebContents::Navigation::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_nativeNavigation` at `0xe2a9650`.
- `url`: `T@"NSString",R,N` at `0xe2a9660`.
- `didRedirectToOtherTab`: `TB,R,N` at `0xe2a9670`.
- `responseStatusCode`: `Tq,R,N` at `0xe2a9680`.
- `responseHeaders`: `T@"NSDictionary",R,N` at `0xe2a9690`.

## Lifecycle, ownership, callbacks and threading

init: 0x4416808 copies native shared navigation and builds a dictionary from native string key/value pairs when nonempty. responseHeaders 0x43dd450 returns that captured field; url 0x4416b90, redirect flag 0x4416bbc and status 0x4416bd4 read live native fields. isEqual: 0x44169e8 compares peer pointers after class/optional checks; hash 0x4416b58 hashes 8 bytes of that pointer.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is event identity, not URL value equality. A replacement navigation observer needs distinct objects/lifetimes and header capture rules. The native producer and Chromium NavigationHandle relationship remain outside this wrapper review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Does header snapshot omit duplicates and preserve case, and can native event state change after callback?
FRESH REVIEW: Header duplicates/case, live native state, producer lifetime and aggregate ABI remain unknown.

### spec-1: ArcPermissionRequest
# ArcPermissionRequest

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37e558`, class_ro `0xe2aebe8`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Permission descriptor with native type/origin and two independently declared additionalData implementations.

## State and layout

Metadata instance start 8, size 40 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `permission_request`: `T{optional<arc::PermissionRequest>=(?=c{PermissionRequest={shared_ptr<arc::PermissionRequest::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_permission_request` at `0xe2aeba8`.
- `type`: `Tq,R,N` at `0xe2aebb8`.
- `originUrl`: `T@"NSString",R,C,N` at `0xe2aebc8`.
- `additionalData`: `T@"NSDictionary",R,N,V_additionalData` at `0xe2aebd8`.

## Lifecycle, ownership, callbacks and threading

0x443f8d4 copies shared peer into optional +0x10. type 0x443f9f8 loads native int +0x18; originUrl shares UTF-8 converter IMP 0x4416b90. Base additionalData 0x2d0fb40 returns self+8. Category Extensions additionalData 0x440a85c ignores that field, traverses native optional map at peer+0x30 when flag +0x48 is set and returns a copied dictionary with NSNumber keys and NSString values. Both identities must be retained; category runtime selection requires Objective-C load behavior validation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The permission type vocabulary and extra-data schema are Arc-specific. Generic Chromium/CEF permission callbacks cannot be assumed to supply the same dictionary keys, request identity or prompt ownership. This class declares no grant/deny method; resolution is elsewhere.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover permission-type and map-key enums, request cancellation, prompt decision consumer and category load/override behavior.
FRESH REVIEW: Runtime category selection, request/type/key enums, decision consumer and callback lifetime remain unknown.

### spec-1: ArcSecureContext
# ArcSecureContext

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37c898`, class_ro `0xe2a8118`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared security-state handle exposing a three-way Arc enum.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `context`: `T{optional<arc::SecureContext>=(?=c{SecureContext={shared_ptr<arc::SecureContext::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_context` at `0xe2a80f8`.
- `state`: `TQ,R,N` at `0xe2a8108`.

## Lifecycle, ownership, callbacks and threading

0x440aa28 copies shared native context. state 0x440ab4c calls 0x3c2b700 and maps native 0→0, 3→1, every other result→2; empty optional traps. This is a lossy mapping, not the Chromium enum verbatim.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: native results are consistent with Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/security_state/core/security_state.h:39-70 (NONE=0, SECURE=3, DANGEROUS=5). 0x3c2b700 contains multiple security-state branches; exact GetSecurityLevel identity is not fully verified. A CEF security-state adapter must preserve Arc numeric mapping and inputs.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover Arc enum names and native security input construction; do not equate all non-secure values with a single browser warning policy.
FRESH REVIEW: GetSecurityLevel identity, input state and Arc enum names are unproven.

### spec-1: ArcSessionNavigation
# ArcSessionNavigation

ArcCore 1.170.0 arm64. 12 declared rows; superclass `NSObject`. Class metadata `0xe37e1e8`, class_ro `0xe2ae150`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Read-only session-entry wrapper exposing stored navigation data.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `session_navigation`: `T{optional<arc::SessionNavigation>=(?=c{SessionNavigation={shared_ptr<arc::SessionNavigation::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_session_navigation` at `0xe2ae0c0`.
- `faviconURL`: `T@"NSURL",R,N` at `0xe2ae0d0`.
- `virtualURL`: `T@"NSURL",R,N` at `0xe2ae0e0`.
- `originalRequestURL`: `T@"NSURL",R,N` at `0xe2ae0f0`.
- `referrerURL`: `T@"NSURL",R,N` at `0xe2ae100`.
- `title`: `T@"NSString",R,C,N` at `0xe2ae110`.
- `hasPostData`: `TB,R,N` at `0xe2ae120`.
- `timestamp`: `T@"NSDate",R,N` at `0xe2ae130`.
- `isOverridingUserAgent`: `TB,R,N` at `0xe2ae140`.

## Lifecycle, ownership, callbacks and threading

0x443c968 copies shared native peer. URL getters 0x8374c34/0x8374ce8/0x8374d9c/0x8374e50 convert optional native URL strings, returning nil when absent. title 0x443ca8c reads UTF-16 data; timestamp 0x443cadc converts stored native time to NSDate since 1970 with zero/infinity handling. No disk write occurs in these getters.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed route inference only: Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/sessions/core/serialized_navigation_entry.h exposes virtual_url, title, referrer, timestamp and has_post_data; this does not identify the exact native Arc Impl or establish serialized format compatibility. A CEF in-memory navigation list alone is not proof of full Chrome session restoration.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover persisted session format, entry IDs, POST-data restoration policy and nil URL sentinel meanings.
FRESH REVIEW: Exact native Impl, persisted session format, URL sentinels and runtime time edges remain unknown.

### spec-1: ArcSiteSettingInfo
# ArcSiteSettingInfo

ArcCore 1.170.0 arm64. 13 declared rows; superclass `NSObject`. Class metadata `0xe37c9d8`, class_ro `0xe2a8548`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Mutable native site-setting descriptor, not itself a persistence operation.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `info`: `T{optional<arc::SiteSettingInfo>=(?=c{SiteSettingInfo={shared_ptr<arc::SiteSettingInfo::Impl>=^{Impl}^{__shared_weak_count}}})B},N,V_info` at `0xe2a84f8`.
- `originUrl`: `T@"NSString",&,N` at `0xe2a8508`.
- `permissionType`: `Tq,R,N` at `0xe2a8518`.
- `contentSettingsType`: `TQ,N` at `0xe2a8528`.
- `siteSetting`: `TQ,N` at `0xe2a8538`.

## Lifecycle, ownership, callbacks and threading

init 0x440b880 creates a shared native info object and initializes native settings. 0x440bc68/0x440bc8c constrain Arc contentSettingsType to 0..36. siteSetting 0x440bcb0 maps a native 0..5 value through table 0xc9d8188, returning 0 when absent/unsupported; setSiteSetting 0x440bcf8 accepts 0..4 and calls 0x4341964. setOriginUrl 0x440bb94 calls 0x4341874; originUrl 0x440bae8 traps if required origin optional is empty.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

The native values and Arc enum are not interchangeable with Chromium ContentSettingsType ordinal values. Persistence requires the consumer that commits this descriptor into a profile settings service; none is proven here. CEF settings support needs a separate exact enum/permission review.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover table 0xc9d8188, native setter mapping, origin canonicalization and the commit-to-profile caller.
FRESH REVIEW: Enum vocabulary, unsupported-state distinctions, setter mapping and profile persistence are not fully established.

### spec-1: ArcTracingConfiguration
# ArcTracingConfiguration

ArcCore 1.170.0 arm64. 11 declared rows; superclass `NSObject`. Class metadata `0xe37cac8`, class_ro `0xe2a8cf0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Value object containing file path, duration and privacy-filtering option.

## State and layout

Metadata instance start 8, size 64 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `tracingConfiguration`: `T{TracingConfiguration={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}dB},N,V_tracingConfiguration` at `0xe2a8cb0`.
- `filePath`: `T@"NSString",&,N,V_filePath` at `0xe2a8cc0`.
- `tracingDuration`: `Td,N,V_tracingDuration` at `0xe2a8cd0`.
- `privacyFilteringEnabled`: `TB,N` at `0xe2a8ce0`.

## Lifecycle, ownership, callbacks and threading

init 0x440c640 initializes empty native path, zero duration and privacy filtering enabled. setFilePath 0x440c764 updates both ObjC cache and native string; filePath 0x440c710 reconstructs the cache from native storage on read. duration getter/setter 0x440c800/0x440c80c similarly synchronize cache/native double; setTracingConfiguration 0x440c820 replaces native aggregate without updating those caches immediately. Destructor 0x440c8ac frees native string and clears cached path.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Configuration construction performs no tracing start/stop or file write in the examined bodies. The consumer (ArcTracingController, outside assignment) must map to Chromium/Perfetto policy and file lifecycle; no CEF tracing equivalence is implied.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Are zero duration and empty path sentinel defaults in controller, and when are paths opened or privacy filtering enforced?
FRESH REVIEW: Controller sentinel rules, file opening and privacy enforcement remain outside this class.

### spec-1: ArcURLLoadCancellationToken
# ArcURLLoadCancellationToken

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37cb68`, class_ro `0xe2a8ef0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Owned std::string token container; it has no declared cancel method.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `token`: `T{basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})},N,V_token` at `0xe2a8ee0`.

## Lifecycle, ownership, callbacks and threading

0x440d37c copies input string using short/long string handling; token 0xef938c returns a value copy and setToken 0x440d41c replaces string storage. 0x440d470 frees heap storage for long strings and 0x416a680 initializes empty storage. None of these five bodies sends a cancellation operation.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Cancellation routing requires the external consumer that indexes network requests by this token. CEF request cancellation is not equivalent until that identity/lifetime protocol is recovered.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Who generates the string, which service consumes it, and does destroying the token affect a request?
FRESH REVIEW: External token generation, consumers and destruction effects remain unknown.

### spec-1: ArcVisitCategory
# ArcVisitCategory

ArcCore 1.170.0 arm64. 8 declared rows; superclass `NSObject`. Class metadata `0xe37d068`, class_ro `0xe2aa180`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Inline category-id string and signed integer weight, stored in an optional native value.

## State and layout

Metadata instance start 8, size 48 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `visit_category`: `T{optional<arc::VisitCategory>=(?=c{VisitCategory={basic_string<char, std::char_traits<char>, std::allocator<char>>=(__rep={__short=[23c]b7b1}{__long=*Qb63b1})}i})B},R,N,V_visit_category` at `0xe2aa150`.
- `categoryId`: `T@"NSString",R,C,N` at `0xe2aa160`.
- `weight`: `Ti,R,N` at `0xe2aa170`.

## Lifecycle, ownership, callbacks and threading

0x442c27c constructs an engaged value from string/id and int weight; nil id becomes empty native string. 0x442c398 copies a native value. mutableVisitCategory 0x442c260 returns the address of inline storage when engaged; categoryId 0x43ed470 converts UTF-8 and weight 0x442c440 reads +0x20. 0x43ed510 frees long string storage.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

This is a value adapter for history/category producers; no ranking or persistence service is invoked by these accessors. Its mutable raw pointer and C++ aggregate return must remain private to matching native callers or be adapted explicitly.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
FRESH REVIEW: Borrowed pointer lifetime, aggregate ABI and producer category vocabulary remain unknown.

### spec-1: ArcWebContents
# ArcWebContents

ArcCore 1.170.0 arm64. 163 declared rows; superclass `NSObject`. Class metadata `0xe37e6c0`, class_ro `0xe2af8d8`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Central app-facing content wrapper, native view host, state cache and multiple delegate/service adapters.

## State and layout

Metadata instance start 8, size 304 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `web_contents`: `T{optional<arc::WebContents>=(?=c{WebContents={shared_ptr<arc::WebContents::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_web_contents` at `0xe2af548`.
- `web_contents_navigation_delegate_bridge`: `T{shared_ptr<ArcWebContentsNavigationDelegateBridge>=^{ArcWebContentsNavigationDelegateBridge}^{__shared_weak_count}},N,V_web_contents_navigation_delegate_bridge` at `0xe2af558`.
- `web_contents_ui_delegate_bridge`: `T{shared_ptr<ArcWebContentsUIDelegateBridge>=^{ArcWebContentsUIDelegateBridge}^{__shared_weak_count}},N,V_web_contents_ui_delegate_bridge` at `0xe2af568`.
- `web_contents_gesture_handler_bridge`: `T{shared_ptr<ArcWebContentsGestureHandlerBridge>=^{ArcWebContentsGestureHandlerBridge}^{__shared_weak_count}},N,V_web_contents_gesture_handler_bridge` at `0xe2af578`.
- `web_contents_actions_delegate_bridge`: `T{shared_ptr<ArcWebContentsActionsDelegateBridge>=^{ArcWebContentsActionsDelegateBridge}^{__shared_weak_count}},N,V_web_contents_actions_delegate_bridge` at `0xe2af588`.
- `web_contents_top_band_color_delegate_bridge`: `T{shared_ptr<ArcWebContentsTopBandColorDelegateBridge>=^{ArcWebContentsTopBandColorDelegateBridge}^{__shared_weak_count}},N,V_web_contents_top_band_color_delegate_bridge` at `0xe2af598`.
- `userActivityDelegate`: `T@"<ArcWebContentsUserActivityDelegate>",W,N,V_userActivityDelegate` at `0xe2af5a8`.
- `userActivityObserver`: `T{shared_ptr<arc::WebContentsObserver>=^{WebContentsObserver}^{__shared_weak_count}},N,V_userActivityObserver` at `0xe2af5b8`.
- `webContentsObserver`: `T{shared_ptr<arc::WebContentsObserver>=^{WebContentsObserver}^{__shared_weak_count}},N,V_webContentsObserver` at `0xe2af5c8`.
- `pdfController`: `T@"ArcWebContentsPDFController",&,N,V_pdfController` at `0xe2af5d8`.
- `backForwardList`: `T@"ArcBackForwardList",&,N,V_backForwardList` at `0xe2af5e8`.
- `title`: `T@"NSString",C,N,V_title` at `0xe2af5f8`.
- `url`: `T@"NSURL",&,N,V_url` at `0xe2af608`.
- `selectedText`: `T@"NSString",C,N,V_selectedText` at `0xe2af618`.
- `favicon`: `T@"NSImage",&,N,V_favicon` at `0xe2af628`.
- `isLoading`: `TB,N,V_isLoading` at `0xe2af638`.
- `loadProgress`: `Td,N,V_loadProgress` at `0xe2af648`.
- `isReadyToDisplay`: `TB,N,V_isReadyToDisplay` at `0xe2af658`.
- `navigationID`: `Ti,N,V_navigationID` at `0xe2af668`.
- `blockedResourceCount`: `Ti,N,V_blockedResourceCount` at `0xe2af678`.
- `canGoBack`: `TB,N,V_canGoBack` at `0xe2af688`.
- `canGoForward`: `TB,N,V_canGoForward` at `0xe2af698`.
- `undoableFieldActive`: `TB,N,V_undoableFieldActive` at `0xe2af6a8`.
- `freeformTextFieldActive`: `TB,N,V_freeformTextFieldActive` at `0xe2af6b8`.
- `passwordTextFieldActive`: `TB,N,V_passwordTextFieldActive` at `0xe2af6c8`.
- `hadFormOrUserEdits`: `TB,N,V_hadFormOrUserEdits` at `0xe2af6d8`.
- `hasDirtyTextField`: `TB,N,V_hasDirtyTextField` at `0xe2af6e8`.
- `hasFocus`: `TB,N,V_hasFocus` at `0xe2af6f8`.
- `inputEvent`: `T@"NSString",&,N,V_inputEvent` at `0xe2af708`.
- `tabTraits`: `T@"NSMutableIndexSet",&,N,V_tabTraits` at `0xe2af718`.
- `state`: `Tq,N,V_state` at `0xe2af728`.
- `visibility`: `Tq,N,V_visibility` at `0xe2af738`.
- `zoomLevel`: `Tf,N,V_zoomLevel` at `0xe2af748`.
- `themeColor`: `T@"ArcColor",&,N,V_themeColor` at `0xe2af758`.
- `rootBackgroundColor`: `T@"ArcColor4f",&,N,V_rootBackgroundColor` at `0xe2af768`.
- `pendingRenderFrameViewportSize`: `T{CGSize=dd},N,V_pendingRenderFrameViewportSize` at `0xe2af778`.
- `isInitialized`: `TB,R,N` at `0xe2af788`.
- `aiAgent`: `T@"ArcWebContentsAIAgent",R,N,V_aiAgent` at `0xe2af798`.
- `boostIDE`: `T@"ArcWebContentsBoostIDE",R,D,N` at `0xe2af7a8`.
- `devTools`: `T@"ArcWebContentsDevTools",R,D,N` at `0xe2af7b8`.
- `mediaSession`: `T@"ArcWebContentsMediaSession",R,D,N` at `0xe2af7c8`.
- `navigationController`: `T@"ArcWebContentsNavigationController",R,D,N` at `0xe2af7d8`.
- `thumbnailsGenerator`: `T@"ArcWebContentsThumbnailsGenerator",R,D,N` at `0xe2af7e8`.
- `sessionId`: `T@"ArcSessionID",R,D,N` at `0xe2af7f8`.
- `diagnosticId`: `T@"NSNumber",R,N` at `0xe2af808`.
- `willRunUnloadEvents`: `TB,R,N` at `0xe2af818`.
- `rootFrameAccessibilityTreeId`: `T@"NSString",R,C,N` at `0xe2af828`.
- `supportedTranslateLanguages`: `T@"NSArray",R,C,N` at `0xe2af838`.
- `deviceScaleFactor`: `Tf,R,N` at `0xe2af848`.
- `contentsMimeType`: `T@"NSString",R,C,N` at `0xe2af858`.
- `zoomFactor`: `Tf,R,N` at `0xe2af868`.
- `isScaleFactorOne`: `TB,R,N` at `0xe2af878`.
- `isScrollOffsetAtTop`: `TB,R,N` at `0xe2af888`.
- `isBlockingJavascriptPopups`: `TB,N` at `0xe2af898`.
- `javascriptPopupsDisplayedCount`: `Ti,R,N` at `0xe2af8a8`.
- `disallowDownloads`: `TB,N` at `0xe2af8b8`.
- `errorStatus`: `T@"ArcErrorStatus",R,N` at `0xe2af8c8`.

## Lifecycle, ownership, callbacks and threading

web_contents 0x4443ee0 returns an optional by value: an empty source returns empty; an engaged source copies the native peer/control pair at +0x118/+0x120 and atomically increments a nonnull control strong count at control+8. The binary checks null aggregate return storage only inside the engaged path; it does not trap merely because the source optional is empty. Return storage is written before that check, so this is not a safe null-storage fallback. Cross-toolchain aggregate calling conventions and peer invalidation remain unvalidated.

initWithWebContents 0x4440c4c copies a shared peer into optional at +0x118 (engagement +0x128) and calls 0x440e174. That initializer snapshots title/URL/loading/progress/favicon/history/zoom/state and installs a native observer with weak self. SetNavigation/UI/Actions/TopBand delegates (0x4440dec/0x444112c/0x444146c/0x44417ac) allocate shared bridges containing weak self and weak app delegate; nil paths unregister through native operations. Destructor 0x4444b54 releases native/bridge/observer control blocks and destroys weak delegate storage; this alone does not prove native close. close category 0x4410ba4 forwards timing 0; 0x4410ab4→0x437854c checks native state and may repost close while UI/navigation/observer/media wrappers remain. createPaneHostView 0x4410bb4 is a raw B veneer to 0x47390dc, allocating WebContentsPaneHostView at NSZeroRect. nativeView 0x4410bb8 ensures native host via 0x4376580 and returns native +0x288. attach 0x4410cd0 / detach 0x4410ec0 helpers 0x4739114/0x47391c8 accept only exact WebContentsPaneHostView membership. This is an Arc native host contract, not an arbitrary NSView parameter.

Weak callback captures are explicit in bridge construction (0x4440dec and peers). Completion block capture is evidenced by retainBlock injection comments and heap captures in 0x4442354/0x4442eb8/0x444302c. Some native helpers invoke failure callbacks inline; no universal asynchronous or main-thread delivery guarantee is established. close 0x437854c may post a retry via a thread-local task-runner vtable; the exact sequence is unresolved. AppKit view sends in 0x4737eb4/0x4738bb0 are UI integration evidence, not independent proof that Arc checks caller thread.

## Relations and engine route

Native bridge 0x4376580 obtains host view, creates/updates host wrapper and stores it at native +0x288. WebContentsPaneHostView attach 0x4737eb4 detaches old content, adds hosted subview and constraints, and synchronizes bounds/screen/visibility; sync 0x4738bb0 supplies screen ID, scale and refresh rate (fallback 60 Hz) to native bridge. Source-backed route comparison: Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c content/public/browser/web_contents.h:1241 and content/browser/web_contents/web_contents_view_mac.mm:138 expose native view, but do not supply the recovered Arc pane/compositor contract. CEF 426920c33ef22551e0b8f48cf343d623ac25642d include/internal/cef_types_mac.h:110-115,151-152 forces Alloy style when parent_view is provided; thus native hosting plus Chrome services is an unresolved combined route gate. find 0x4442354 captures block and uses 0x43770c0; callback 0x444510c creates ArcFindResult. Javascript overloads 0x4442eb8/0x444302c capture block, convert script/reason and call 0x4377d70; unavailable content/renderer states call an empty-string/zero result inline, while valid path forwards to frame execution with world integer 4. Callback 0x4445338 converts native string and passes second native argument through; its semantic type is not proven here. Accessors at shared IMPs are field operations, not the unrelated RTC/CPU classes named by Ghidra. Direct old-app instruction sample: app 1.168.0 BL 0x1005426b4 sends createPaneHostView; tail branches 0x1005426d4 and 0x1005426e8 send attachToPaneHostView: and detachFromPaneHostView:. App functions 0x1005426a0 / 0x1005426c4 / 0x1005426d8 and app168/ghidra/decomp/0x1005.c:53029,53041,53054 associate receiver with ADK.WebContentChromium.webView. This proves concrete sends and receiver-property flow, not runtime receiver class. See evidence/app168-embedding-calls.tsv and app168-embedding-pseudocode.txt.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover exact delegate protocol selectors/decision callback ownership, observer/KVO sequencing, close repost scheduling and pane lifetime. Validate cached setters versus engine operations (e.g. setVisibility only stores a field). Recover JS result/error serialization and isolated-world meaning; validate source-backed CEF/native-host route with actual macOS hosting tests.
FRESH REVIEW: 163 declarations are inventoried, not fully semantically recovered. Callback ABI/sequence/teardown, native host lifecycle, caller ABI and runtime compatibility remain unproven.

### spec-1: ArcWebContentsPDFController
# ArcWebContentsPDFController

ArcCore 1.170.0 arm64. 9 declared rows; superclass `NSObject`. Class metadata `0xe37e328`, class_ro `0xe2ae4f0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared PDF-control bridge with callbacks and immediate bool/NSError outcomes.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `web_contents_pdf_controller`: `T{optional<arc::WebContents::PDFController>=(?=c{PDFController={shared_ptr<arc::WebContents::PDFController::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_web_contents_pdf_controller` at `0xe2ae4e0`.

## Lifecycle, ownership, callbacks and threading

0x443df90 copies native controller peer. Methods 0x443e0b4..0x443e614 capture supplied blocks and call native helpers 0x436c99c / 0x436cb2c / 0x436ccbc / 0x436cec4 / 0x436d054, then convert immediate native status through 0x4402548. Immediate bool is not the callback answer (e.g. hasBookmarks). Helpers separately reject detached native content, missing glue and missing PDF controller.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

0x436c99c and 0x436d054 cite arc/browser/web_contents/pdf_controller_impl.cc and bridge to internal PDF controller functions (0x41a7920 / 0x41a8a58). CEF 426920c33ef22551e0b8f48cf343d623ac25642d include/cef_browser.h:546 PrintToPDF is an output API; it does not establish equivalent bookmark/text/page extraction semantics.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Recover callback argument schemas, page-index basis, partial/available-page meaning, errors and behavior during PDF document teardown.
FRESH REVIEW: Callback schemas, page conventions, malformed wrapper arguments, delivery and document teardown remain unresolved.

### spec-1: ArcWebDialog
# ArcWebDialog

ArcCore 1.170.0 arm64. 10 declared rows; superclass `NSObject`. Class metadata `0xe37ced8`, class_ro `0xe2a9c80`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Value dialog descriptor containing strings, dimensions and weak native WebContents reference.

## State and layout

Metadata instance start 8, size 88 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.


Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `dialog`: `T{WebDialog={basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{Size=ii}{Size=ii}{basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>=(__rep={__short=[11S]{__padding<1UL>=[1c]}b7b1}{__long=^SQb63b1})}{weak_ptr<arc::WebContents>=^{WebContents}^{__shared_weak_count}}},N,V_dialog` at `0xe2a9c20`.
- `accessibleTitle`: `T@"NSString",R,N` at `0xe2a9c30`.
- `preferredSize`: `T{CGSize=dd},R,N` at `0xe2a9c40`.
- `minimumSize`: `T{CGSize=dd},R,N` at `0xe2a9c50`.
- `title`: `T@"NSString",R,N` at `0xe2a9c60`.
- `webContents`: `T@"ArcWebContents",R,N` at `0xe2a9c70`.

## Lifecycle, ownership, callbacks and threading

0x441cd64 copies strings and size fields, increments weak native control count. webContents 0x441cf64 locks weak control block and allocates a fresh ArcWebContents wrapper from live native peer; expired or null peer traps rather than returning nil in visible body. dialog 0x441d120 copies whole descriptor through 0x43466c8; 0x441d2a0 destroys it through 0x434660c.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

accessibleTitle 0x441cefc and title 0x441cf40 convert UTF-16 strings; preferred/minimum sizes 0x441cf20 / 0x441cf30 unpack integer dimensions to CGSize. The app is responsible for presenting these descriptors; window/modal lifecycle and Chrome UI delegate producer are outside this review. No CEF dialog equivalent is proved.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.


## Remaining implementation questions

Does app assume wrapper identity across webContents reads, who owns the native dialog content, and can callbacks outlive dismissal?
FRESH REVIEW: Native owner/window lifecycle, weak-control synchronization and cross-toolchain calls remain untested.

### spec-2: ArcAIInferenceConfiguration
# ArcAIInferenceConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dbf8`, class_ro `0xe2ace10`; instanceStart 8, instanceSize 20 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The initializer engages an inline configuration, stores maxTokens as int32 at +8 and truncates streamAfterEveryNTokens to signed int16 at +0xc (0x443a318). Getters require the engagement byte at +0x10; an empty configuration traps rather than returning defaults (0x443a3d4, 0x443a3e8). The mutable accessor returns interior storage (0x443a2fc); the aggregate getter copies the optional (0x443a3fc).

## Lifecycle, callbacks and relations

No callback is installed here. ArcAIService consumes this configuration through its streaming adapter at 0x43d7a5c → 0x443a40c. There is no persistence operation in these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific value adapter; either backend needs the Arc ABI and integer narrowing behavior. No CEF API equivalence is established.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIInferenceConfiguration.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Confirm overflow/negative token handling at the service boundary, and whether app callers rely on the signed 16-bit truncation. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Token ranges, service consumer behavior and borrowed-pointer lifetime remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcAIInferenceModel
# ArcAIInferenceModel

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dba8`, class_ro `0xe2acd20`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


This is an engaged optional value, not the loaded model itself. init:directory: writes the model discriminator and converts the directory string (0x4439f8c); initWithAIInferenceModel: copies native string storage (0x443a094). modelType and directory check engagement (0x443a13c, 0x443a150); destruction frees an allocated string only while engaged (0x43ded20).

## Lifecycle, callbacks and relations

The aggregate getter copies the model descriptor (0x443a180). ArcAIService loadInferenceModel and streamInference consume it through the adapter 0x443a21c (callers 0x43d77b0, 0x43d7a5c). No model file is opened by the inspected model IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific descriptor; backend model loading lives beyond this class. CEF alone does not specify Arc model format.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIInferenceModel.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover modelType enum meanings and validation rules in the loader; distinguish descriptor lifetime from loaded resource lifetime. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Loader validation, model identity and resource lifetime remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcAIService
# ArcAIService

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 24 declared instance methods; no declared class methods. Categories: Extensions. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37db58`, class_ro `0xe2acc30`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithAIService: stores an engaged shared native handle, incrementing its control count (0x4439e68). ai_service copies the optional handle and increments its count (0x43df53c); .cxx_destruct releases it (0x43df69c). All service operations are in the Extensions category, distinct from the four base declarations.

## Lifecycle, callbacks and relations

Extensions marshal strings, vectors and ObjC callback objects into native operations. Streaming has separate partial-result and completion adapters, and returns the native token (0x43d7a5c → 0x43310bc → 0x429fa38). The latter installs ai.mojom.AIStreamedInferenceListener and uses ai.mojom.AIService; its early unavailable branch invokes completion and returns zero (0x429fa38). Other callback error shapes, cancellation and delivery queues remain unresolved. Both evaluateCommandBarRanking:completion: and trainCommandBarRanking:completion: explicitly warn that type propagation is not settling (0x43d9ab8, 0x43d9fd4). Their bodies are available, but displayed extra arguments/aggregate mapping are not reliable ABI evidence; no corrupt function boundary is established.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific service and Mojo protocol. Chrome or CEF hosting still needs a compatible service boundary; this analysis does not include libaperitif execution or prove it implements these endpoints.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIService.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover callback adapter vtables and exact result/error/token contracts for every operation; determine which service implementation and library version the app actually supplies. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 12, 13; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: All 20 Extensions operations and callback adapters were not freshly traced transitively; result/error/token, queue and cancellation remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcAIServiceConfiguration
# ArcAIServiceConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37b8f8`, class_ro `0xe2a3b10`; instanceStart 8, instanceSize 160 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error: rejects type -1 with NSError domain company.thebrowser.Arc, code 2 and Invalidtype text, writing the supplied error pointer (0x43dcbdc). It uses libraryPath.path, converts five strings, narrows maxRetries to a signed byte at +0x88 and stores a double at +0x90. Optional engagement is +0x98.

## Lifecycle, callbacks and relations

nativeAIServiceConfig copies an engaged configuration and setNativeAIServiceConfig: implements engaged/empty assignment (0x43dd120, 0x43dd184); destruction calls 0x42a3344 only while engaged (0x43dd3a0). No callback registration or library loading happens in these configuration IMPs. The timeout getter has a decompiler signature defect; raw instructions load d0 from +0x90 after the engagement check (0x43dd10c).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc-specific configuration ABI including narrowing and error behavior; strings describe external service deployment, without proving any particular library identity.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAIServiceConfiguration.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Determine valid service type values, whether nil error pointers are ever used on the rejection path, and allowed retry/timeout ranges. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Valid type/range policy and nil NSError-out use remain unknown; displayed ABI is not certified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcAnnotatedVisit
# ArcAnnotatedVisit

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 63 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c028`, class_ro `0xe2a5be0`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The ObjC object owns optional unique storage: init allocates a 0x150-byte flat record and sets task/root/parent IDs to -1 (0x43fed80); native-copy init uses 0x4336aac (0x43fee60); destruction calls 0x4336e98 then frees it (0x4400f18). Setters mutate this local record, with no demonstrated database write. Empty getters have individual defaults, including current NSDate for visitTime (0x43ff064).

## Lifecycle, callbacks and relations

relatedSearches returns an array of converted strings, with an empty-array fallback for absent native state; the engaged path copies its constructed array (0x43ff558).

visitTime stores Unix seconds through NSDate rather than a direct base::Time field (0x43ffac0). Redirect chains wrap ArcRedirectEntry (0x43ff0fc); categories wrap ArcVisitCategory (0x43ff438). Setters rebuild corresponding vectors (0x43ffb30, 0x44001d0, 0x4400620). Stock history::AnnotatedVisit at Chromium components/history/core/browser/history_types.h:1029 is a source-backed domain comparison, not an established layout identity.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome history integration is plausible, but this flat Arc record is a separate serialization boundary. A CEF route would need to supply the rich annotation, redirect and provenance contract independently; completeness is unproven.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAnnotatedVisit.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Follow the history producer/consumer and 0x4336aac before mapping flat fields to upstream history structs; validate empty-state and enum behavior against app use. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 3; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Most of 63 getter/setter semantics were not freshly audited statement by statement; producer mapping, conversion edge cases and freshness remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcAutocompleteResult
# ArcAutocompleteResult

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 4 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bbc8`, class_ro `0xe2a45e8`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


init:perProviderMatches: stores two ObjC collections at +8 and +0x10 after NSObject init (0x43df204). Accessors return those slots (0x43dd450, 0x420b5f8); destruction clears both (0x43dd5a4). It does not compute or merge matches.

## Lifecycle, callbacks and relations

No delegate registration occurs in this class. The exact element classes, collection mutability and copy-versus-retain policy are not fully established because retain/release fixups obscure the stores (0x43df204). It is distinct from the array constructed by autocomplete completion at 0x43eceb0.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A result container can be backend independent. Matching/ranking/provider behavior must be obtained from the producer; its class name does not prove Chrome or CEF equivalence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcAutocompleteResult.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Find the producer of this combined/per-provider result and actual app consumers; establish collection ownership and ordering. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Element classes, ordering, collection mutability and copy/retain details remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcBrowserContextAttributes
# ArcBrowserContextAttributes

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dd38`, class_ro `0xe2ad2e0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The native attributes value begins at +0x10; +8 caches an ArcColor object. Initialization delegates to native assignment at 0x441e0a0 (0x443afd0). directoryBasename and userVisibleName convert native strings (0x443ab84, 0x443aba8). userColor lazily constructs and caches ArcColor from native color storage at +0x40 (0x443b028).

## Lifecycle, callbacks and relations

Aggregate getter copies strings and color (0x443b07c); aggregate setter calls 0x441e0a0 (0x443b0f4). Destruction frees native strings and clears cached color (0x443b100). The setter only passes self+0x10 to 0x441e0a0, which updates native strings/color without touching the cached ObjC slot at self+8; an already-created userColor wrapper therefore remains unchanged along this assignment path (0x443b0f4, 0x441e0a0, 0x443b028). No disk operation is shown.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc profile presentation data. Either hosting route can marshal values, but profile identity/persistence and cached-color invalidation must match the engine contract.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcBrowserContextAttributes.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Does the app use the stale cached-color path after native reassignment, and what owns/updates the persisted profile attributes? Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: No app-use, profile persistence or cache invalidation policy beyond this assignment path is established. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcBrowserContextAutocomplete
# ArcBrowserContextAutocomplete

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 5 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bee8`, class_ro `0xe2a54b0`; instanceStart 8, instanceSize 16 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithAutocompleteController: adopts the supplied by-value native controller owner into self+8; source-zeroing is not shown on success (0x43ecb50). The zero stores belong to old-owner or failed-init cleanup, so caller-side move/ownership ABI remains unverified; destruction disposes the controller using 0x42c33cc (0x43ece68). A request converts nil text to an empty string, stores a callback adapter and forwards cursor/prevent-inline arguments (0x43ecc2c → 0x42c35e4).

## Lifecycle, callbacks and relations

0x42c35e4 replaces the saved callback, constructs an input with a literal 4 and starts a controller via 0x20cd850. Completion builds an NSArray of ArcAutocompleteMatch and invokes the captured block with an additional argument (0x43eceb0). stop checks native controller/state, directly calls 0x24e39c0 with reason value zero, then resets callback storage and indirectly cleans the saved callable (0x43ece08). Chromium components/omnibox/browser/autocomplete_controller.cc:631 and :797 provide a source-backed Start/Stop hypothesis; exact function identity remains unproven.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A Chrome omnibox service is strongly suggested by input/controller behavior; CEF replacement needs explicit provider/ranking semantics, not only URL navigation. No complete API exclusion is claimed.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcBrowserContextAutocomplete.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Identify the completion additional argument, cancellation/supersession rules, controller lifetime, and callback thread; verify cursor narrowing to int in 0x42c35e4. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 5, 6; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Caller move/owner ABI, null-owner validity, exact upstream identity, completion ordering and queue remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcBrowserContextID
# ArcBrowserContextID

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bf38`, class_ro `0xe2a55e0`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithBrowserContextID: copies a basename string and native discriminator into optional storage (0x43ed360). isIncognito returns discriminator == 1 after an engagement check (0x43ed454); directoryBasename converts the string (0x43ed470). id returns an aggregate copy rather than a unique numeric identifier (0x43ed4a0).

## Lifecycle, callbacks and relations

Destruction frees heap string storage only for an engaged value (0x43ed510); construction clears engagement (0x43ed538). No profile creation, directory traversal or callback registration is present in these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent Arc profile identity wrapper; mapping incognito identities to real browser contexts requires external service policy.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcBrowserContextID.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Determine valid discriminator values and basename canonicalization; verify whether app compares native IDs or only exposed strings/flags. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Discriminator meanings beyond one, basename policy and profile creation behavior remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcColor
# ArcColor

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 10 declared instance methods; no declared class methods. Categories: Init. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dd88`, class_ro `0xe2ad3f0`; instanceStart 8, instanceSize 12 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The four-byte field is at +8. r/g/b/a return successive bytes +8/+9/+0xa/+0xb (0x40b1230, 0x43ecb04, 0x43ecb0c, 0x440c5a4); the r IMP is also used for a Boolean in ArcWebLoginDialog, so its Ghidra name/type is not authoritative. Init category initWithR:g:b:a: packs the bytes and invokes initWithColor: (0x4401b58).

## Lifecycle, callbacks and relations

initWithColor: copies the four bytes (0x443b164); color uses the hidden aggregate return destination, obscured by Ghidra, and setColor: copies four bytes (0x443b1b0, 0x443b1bc; raw prefix evidence). Both C++ lifecycle declarations alias the same return-only IMP 0x15ac0. No callback or persistence occurs in these bodies.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A four-byte Arc value ABI. Do not map its memory byte order to a Chromium SkColor integer without checking caller packing.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcColor.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Validate endianness and alpha/premultiplication semantics at the native conversion boundary; preserve hidden return calling convention. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Consumer packing, premultiplication and complete aggregate-return ABI remain outside this sample. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcCommandBarRankingEvaluation
# ArcCommandBarRankingEvaluation

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d7e8`, class_ro `0xe2abcf0`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


An optional evaluation holds a metrics vector, count at +0x20 and duration integer at +0x24. Initializer enumerates dictionary keys, converts keys and double values, then stores counts (0x4433ba8). metrics reconstructs an NSMutableDictionary with NSNumber double values (0x4434190).

## Lifecycle, callbacks and relations

mutableCommandBarRankingEvaluation returns interior storage only if engaged (0x442c260). Aggregate copy uses 0x4434430; .cxx_destruct frees vector string allocations (0x4434298, 0x4434380). No ranking execution, callback scheduling or persistence occurs here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc AI evaluation DTO. Independent of Chromium hosting, but dictionary conversion and numeric types are part of the interface.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcCommandBarRankingEvaluation.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Check duplicate metric keys, metrics ordering assumptions and duration/count narrowing; identify how service callbacks populate this value. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Boxed FP argument ABI, metric meanings and service result production remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcCommandBarRankingExample
# ArcCommandBarRankingExample

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: Extensions. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d798`, class_ro `0xe2abbf0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


An engaged optional example owns input and candidate-vector state; selectedIndex is int32 at +0x38 (0x4433658). init:candidates:selectedIndex: converts the input, enumerates candidates into native structures, and stores the index (0x4433124); no bounds check on selectedIndex was established.

## Lifecycle, callbacks and relations

Extensions input copies native example state and constructs an ArcCommandBarRankingInput wrapper (0x4401ca0). The candidates getter shows array/candidate allocation intent, but its pseudocode has incoherent temporary cleanup/reuse and argument propagation (0x4401f34); returned contents and ownership are unresolved. Raw instructions also place cleanup-helper calls before the apparent conversion loop, so this requires coherent callee/control-flow recovery, not a guessed lifetime repair or an engine-bug diagnosis. Aggregate copies use 0x43dad44 (0x4433528, 0x443366c); destruction frees nested state (0x4433768). These are not training operations.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc ranking example DTO; both routes need conversion behavior, while model evaluation/training belongs to ArcAIService (0x43d9ab8, 0x43d9fd4).

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcCommandBarRankingExample.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover coherent candidates control flow, argument mapping and temporary ownership across 0x4401f34, native getter 0x443366c, initializer 0x4433528 and copy helper 0x43dad44; candidate contents, ordering and lifetime remain unresolved. Resolve selectedIndex validation and ownership of returned input wrappers. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 7; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Candidate contents/order, temporary ownership, constructor argument recovery and selectedIndex validation remain unknown; no engine bug or always-empty result is established. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcContentConversionService
# ArcContentConversionService

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ba88`, class_ro `0xe2a4088`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


init allocates a shared ContentConversionServiceImpl and constructs it at 0x42a6200 (0x43dd9d4). initWithImpl: stores a shared native implementation (0x43dd794); destruction releases its control block (0x43de5e8). initialize converts libraryPath.path plus identity/version/environment strings and installs a callback adapter (0x43ddb74).

## Lifecycle, callbacks and relations

HTML conversion marshals text, optional URL, enum/options, Boolean flags and a vector built by sending kind/value to untyped enumerated input elements before calling 0x42a75b4 (0x43de074). ArcMetaTagSelector is a compatible candidate shape; this path does not prove the concrete receiver class. That backend retains completion by request ID, has an unavailable early completion branch, and binds content_conversion.mojom.ContentConversionService (0x42a75b4). Conversion algorithm/library contents and completion queue are not recovered.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc service protocol, not proof of stock Chromium HTML-to-Markdown behavior. CEF can host a separate service, but matching conversion and metadata policies remains required.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcContentConversionService.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover callback result/error shape, URL transform and enum validation; determine library initialization failure behavior and helper/service ownership. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 8; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Concrete element producer/class, algorithm/library, callback outputs/queue and service ownership remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcContextMenuElementInfo
# ArcContextMenuElementInfo

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 17 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e508`, class_ro `0xe2aeac8`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initialization stores an optional shared native context record (0x443f0a0); destruction releases it (0x43df69c). x/y read native int32 at +4/+8 through the handle (0x443f1c4, 0x443f1dc); URL and text accessors convert native values, including selection text at +0x1c0 (0x443f404). Most access requires engagement.

## Lifecycle, callbacks and relations

dictionarySuggestions requires engagement and constructs/returns NSMutableArray of ObjC strings converted from native UTF-16 elements, including an empty array; it sends no copy (0x443f654). inputType maps native enum 15..23 through table 0xc9d8198, returns zero when optional input type is absent and five outside that range (0x443f844). nodeType maps native 0..6 using 0xc9d81a8, trapping outside it (0x443f88c). These are metadata adapters; no menu is displayed here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Context-menu data can be adapted by either route, but coordinate space, enum translation and top/subframe distinctions require explicit mapping. Stock Blink fields are domain context only, not proven struct identity.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcContextMenuElementInfo.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Establish coordinate units and enum names; verify suggestion lifetime and all supported native node/input kinds. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 4; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Suggestion producer/order, coordinate space, enum names and mutation expectations remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcDisplayNameOrOrigin
# ArcDisplayNameOrOrigin

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dec8`, class_ro `0xe2ad750`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The record holds a native UTF-16 string plus an isOrigin byte at +0x20. Initialization copies native string state (0x443b898), nameOrOrigin converts it through 0x1d9fb90 (0x441cefc), and isOrigin directly loads the flag (0x4081f00).

## Lifecycle, callbacks and relations

Aggregate getter copies the string and flag; setter assigns both (0x443b948, 0x443b998). Destruction frees long-string storage (0x443ba1c). There is no demonstrated origin parsing, formatting policy, delegate or persistence operation in this class.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent presentation DTO; origin/security policy must be supplied by its producer.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcDisplayNameOrOrigin.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Find who chooses display-name versus origin and whether strings are already escaped/localized. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Formatting, escaping, origin/trust policy and producer semantics remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcExtensionInfo
# ArcExtensionInfo

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 19 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d4c8`, class_ro `0xe2ab160`; instanceStart 8, instanceSize 232 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


An optional inline extension snapshot contains name/ID/description/version/keyword strings, URL, permissions, icon and scalar flags. Default init engages a default native record (0x442f174); native-copy init uses 0x4334f1c (0x442f218); destructor uses 0x4334d2c (0x442f528). Getters generally trap if empty.

## Lifecycle, callbacks and relations

permissions constructs an array of strings (0x442f330); icon returns nil for empty image state or converts it (0x442f3f0). enabled, hasAction and fromWebstore are stored flags (0x442f444, 0x442f458, 0x442f4cc). None of these IMPs installs, enables or executes an extension.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome extension data needs an actual extension-service producer for a full replacement. The DTO alone is portable; compatibility of extension execution under a CEF route remains outside this evidence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcExtensionInfo.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Find snapshot refresh points, permission text localization, icon representation and extension-service dependencies. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Extension execution, service producer, permission localization, icon ownership and snapshot freshness remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcExtensionToolbarAction
# ArcExtensionToolbarAction

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 11 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37dfb8`, class_ro `0xe2ada90`; instanceStart 8, instanceSize 120 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initialization copies the native toolbar-action record via 0x441e6d8 (0x443bc48). id is a converted native byte string (0x43e632c), accessibleName/tooltipText are converted UTF-16 (0x443bca0, 0x443bcc4), and icon is nil when the image is absent (0x443bce8).

## Lifecycle, callbacks and relations

pinnedPosition boxes a present optional integer and returns nil when absent (0x443bd08); enabled reads +0x68 (0x4243428). Aggregate assignment and lifecycle call native helpers (0x443bd4c, 0x443bd58, 0x443bd60). This object does not invoke a toolbar action or persist pinning.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Arc snapshot of Chrome-extension toolbar state; CEF feasibility depends on the producer/action API, not these getters.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcExtensionToolbarAction.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Confirm pinning-index meaning and absence semantics, snapshot ownership, and refresh after extension action changes. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Actual numeric passing/index semantics, producer/action path and snapshot lifetime remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcGPULogMessage
# ArcGPULogMessage

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e0a8`, class_ro `0xe2add70`; instanceStart 8, instanceSize 64 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


This native value stores severity int32 at +8 and two strings at +0x10/+0x28. initWithGPULogMessage: invokes assignment helper 0x441e818 (0x443c0a8); severity is a scalar read (0x443ba88); header/message convert strings (0x443ab84, 0x443aba8).

## Lifecycle, callbacks and relations

Aggregate getter copies strings and severity (0x443c100); setter reuses native assignment (0x443c17c); destructor frees strings (0x443c188). Shared IMPs with ArcBrowserContextAttributes reflect field access/layout reuse, not shared semantic classes. No log collection backend is identified here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable Arc log DTO; the GPU process log feed must be supplied separately by the host engine.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcGPULogMessage.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Establish severity enum, message source/order and callback delivery policy in the producer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Severity domain, GPU-feed source and ordering remain unknown; shared IMP name is not receiver identity. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcHistogramSamples
# ArcHistogramSamples

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 39 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c208`, class_ro `0xe2a6a30`; instanceStart 8, instanceSize 304 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithPeriodicHistogramSample: builds up to 37 ArcHistogramSample objects from present native sample fields and stores them in the 37 object slots (0x4402ad8). The individual getters are raw slot loads, many shared with unrelated classes; interpret returned object types using metadata (e.g. 0x2d0fb40, 0x31fa70).

## Lifecycle, callbacks and relations

Destructor clears the 37 slots (0x44032d8). This is a snapshot wrapper, with no collection timer, process enumeration, histogram aggregation, callback or persistence operation demonstrated in its methods. Nil represents an absent native sample branch in the initializer (0x4402ad8).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome/OS metric collection lives outside the DTO. A CEF route needs an independent source for the same process-category samples; names alone do not establish measurement units or sampling intervals.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcHistogramSamples.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover sample optionality and units from the producer and ArcHistogramSample; validate all 37 constructor-source offsets against runtime samples. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Fresh semantic sample is destructor only; the 37 initializer-source branches/units were not re-proven individually or dynamically. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcHistoryDeletion
# ArcHistoryDeletion

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d388`, class_ro `0xe2aac20`; instanceStart 8, instanceSize 144 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The engaged native deletion payload begins at +0x10 and +8 caches the ObjC deletedItems array. Initialization copies through 0x4337338 after disposing prior state (0x442e4f4). The native flags expose all-history, expiration and time-range presence; date getters convert stored seconds (0x442e568, 0x442e57c, 0x442e590, 0x442e5a4, 0x442e5c8).

## Lifecycle, callbacks and relations

deletedItems lazily wraps ArcHistoryItem values (0x442e5ec). faviconUrls/restrictUrls build sets, and deletedVisitIds boxes native IDs (0x442e6c4, 0x442e7a8, 0x442e88c). Destruction frees native state and cached array (0x442e994). This is a deletion notification payload, not evidence that reading it deletes history.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Upstream history::DeletionInfo is a domain comparison at components/history/core/browser/history_types.h:792, not a proven direct layout mapping. Chrome history observer semantics need preservation; CEF support is not established.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcHistoryDeletion.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Determine payload producer/notification timing, absent-range interpretation, cache validity and how restricted URLs/deleted IDs relate to actual history changes. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Deletion producer/timing, cached items, restricted URL relation and absent-range meaning remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcHistoryQueryOptions
# ArcHistoryQueryOptions

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 16 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c320`, class_ro `0xe2a6d30`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Construction allocates shared native options using 0x458d908; destruction releases the shared control block (0x4403adc, 0x43de5e8). searchText converts native UTF-16; the time setters convert NSDate seconds to Chromium-style microseconds with offset 0x295e9648864000 and saturation/NaN branches (0x44036c0, 0x4403830, 0x4403978).

## Lifecycle, callbacks and relations

maxResults reads native +0x28 (0x40cbfe8). Duplicate policy maps 0..2 and traps invalid getter state (0x4403a34); matchingAlgorithm returns zero for absent or native zero, while its setter only accepts zero/one (0x4403a74, 0x4403aa4). Upstream QueryOptions and meanings are in components/history/core/browser/history_types.h:362, :384, :398; Arc wraps additional searchText, so layout equivalence is not claimed.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A query descriptor, not an executed history query. Chrome history options provide a source-backed model; CEF route needs explicit query, dedup and time-bound behavior.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcHistoryQueryOptions.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Confirm all signed enum edge cases, nil/NaN dates, default begin/end bounds and actual query consumer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Signed-edge setters, date defaults/NaN handling and executed query behavior were not exhaustively re-audited. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcImportSource
# ArcImportSource

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 3 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c398`, class_ro `0xe2a6f00`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithImportSource: sign-extends sourceID from native int32 and translates the source type using a validity bitmask and table at 0xc9d8158, trapping unsupported values (0x4403e9c). sourceID/type are raw 64-bit slot reads at +8/+0x10 (0x2d0fb40, 0x2cb140).

## Lifecycle, callbacks and relations

This object copies two scalar values. It neither enumerates external profiles nor starts an importer, and has no declared destructor or callback. Its native ImportSource name does not establish identity with an upstream importer struct.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable import-source DTO, with Chrome/Safari import backends elsewhere. CEF import support cannot be concluded from this adapter.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcImportSource.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Map each accepted native source enum to its Arc-facing value and actual importer; confirm the app-facing labels. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Enum labels and backend importer identity remain unknown; no import execution established. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcKeyEventData
# ArcKeyEventData

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37bb78`, class_ro `0xe2a44e8`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initializer copies domCode and key strings, stores eventType at +0x10 and Windows virtual-key int32 at +8 (0x43def08). Accessors read +8/+0x10/+0x18/+0x20 (0x4081a30, 0x2cb140, 0x4084dc0, 0x4060580); destructor clears both strings (0x43defdc).

## Lifecycle, callbacks and relations

Explicit copy messages establish copy behavior for key/code in this initializer (0x43def08). No event injection, native key conversion, callback or scheduling occurs in this class. The eventType slot is scalar despite the shared Ghidra ID-return name.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable input-event DTO. The real keyboard dispatch path must translate it to the chosen backend and preserve physical code versus logical key.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcKeyEventData.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Determine eventType enum and keyboard-layout/modifier semantics from actual consumer; do not infer from Windows key naming. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Event consumer, layout/modifier policy and dispatch timing remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcLoginCredentials
# ArcLoginCredentials

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 7 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e378`, class_ro `0xe2ae5f0`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initialization stores an optional shared native credentials handle (0x443e874); aggregate getter copies it and destructor releases it (0x43df53c, 0x43df69c). url converts a native URL representation (0x443e998), username converts UTF-16 (0x443ea48).

## Lifecycle, callbacks and relations

password calls helper 0x6bd82ac and converts its returned data through the UTF-16 string helper (0x8374f3c). The helper was not followed; this does not establish secure memory, encryption or erase-on-release behavior. No keychain lookup, save or callback is demonstrated by these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

A credentials-access DTO. Chrome password-manager/keychain and CEF authentication workflows require separate investigation; the password getter does not identify the storage backend.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcLoginCredentials.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover 0x6bd82ac and credentials producer; confirm privacy/lifetime, origin matching and whether empty versus absent credentials are distinguishable. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: No secure-storage, encryption, erasure or complete helper contract inferred; producer/origin/lifetime remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcMetaTagSelector
# ArcMetaTagSelector

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 4 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37b998`, class_ro `0xe2a3d10`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


initWithKind:value: stores a scalar kind and explicitly copies value (0x43dd458). Getters read kind at +8 and value at +0x10 (0x2d0fb40, 0x420b5f8); destructor clears the value slot (0x425b6d8).

## Lifecycle, callbacks and relations

ArcContentConversionService enumerates untyped input objects, sends kind/value and builds a native selector vector (0x43de074). This DTO has a compatible candidate shape; no allocation/class check or producer/caller dataflow in that path proves these concrete receivers. This class does not query DOM metadata itself.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Selector DTO with a shape compatible with the Arc conversion service; concrete element-class identity requires producer/caller evidence, and exact selector kinds and matching rules remain a service contract.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcMetaTagSelector.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Resolve kind enum and matching case/namespace rules in the conversion backend. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 9; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Concrete conversion receiver dataflow, kind enum and matching policy remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcNavigationAction
# ArcNavigationAction

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 9 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c668`, class_ro `0xe2a7878`; instanceStart 8, instanceSize 96 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initializer copies native action state using 0x43452d8 (0x44084e4). url/referrer convert strings at +0x10/+0x28 (0x4408544, 0x440854c). navigationType and navigationDisposition adapt native scalar fields; disposition traps for unsupported values (0x4408554, 0x4408564).

## Lifecycle, callbacks and relations

Aggregate getter copies state via 0x4345224, setter assigns via 0x43452d8, destructor releases native string state (0x4408578, 0x440858c, 0x44085a0). This record does not itself decide navigation policy or launch a request.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Navigation callback payload can be adapted to either route, but enum mapping and the producer decision/callback timing must be preserved.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNavigationAction.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Identify the action producer and exact navigation/disposition enum names, URL/referrer normalization and any action IDs used for replies. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Enum names, actual navigation policy and producer timing remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcNetworkActivityMonitor
# ArcNetworkActivityMonitor

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c6b8`, class_ro `0xe2a7958`; instanceStart 8, instanceSize 32 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Initialization stores an engaged shared native monitor (0x44085cc). Equality compares compatible class/native pointer and hash hashes the pointer after engagement checks (0x44086f0, 0x4408860). startLogging unwraps configuration and browser context, calls 0x432c950 and converts status through 0x4402548 (0x44088ac).

## Lifecycle, callbacks and relations

The backend rejects headless contexts and already-active logging, starts a NetExport-like writer, collects prerenderInfo/extensionInfo and schedules duration in microseconds (0x432c950). isLogging reads textual states including INITIALIZING, STARTING_LOG and LOGGING (0x4408898 → 0x432c834). This differs from stock NetExportFileWriter::IsLogging, which includes STOPPING_LOG instead of INITIALIZING (components/net_log/net_export_file_writer.cc:312).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome NetExport integration is a source-backed inference from matching state strings and StartNetLog call shape (same source :157, :275). CEF logging API equivalence is unresolved; generic network interception is insufficient evidence.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNetworkActivityMonitor.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Verify stop/timer callback and file finalization, the exact NetExport writer identity, thread affinity and whether overlapping monitors share a writer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Exact upstream function identity, stop/timer/finalization and sequence/thread contract remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcNetworkLoggingConfiguration
# ArcNetworkLoggingConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 10 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e148`, class_ro `0xe2adf70`; instanceStart 8, instanceSize 40 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


A native file path string begins +8, with duration double at +0x20. Default init clears the path; no explicit duration store is visible in this initializer (0x443c3e4); filePath converts the native string (0x43e632c), setter converts input bytes (0x443c500), duration getter loads +0x20 (0xed0b1c).

## Lifecycle, callbacks and relations

Aggregate getter/copy setter duplicate string state (0x443c588, 0x443c5d8); destructor frees a heap string (0x440d470). It is consumed by startLogging at 0x44088ac; this class itself opens no output file.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Backend-independent logging options; duration interpretation and file lifecycle belong to the monitor backend at 0x432c950.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNetworkLoggingConfiguration.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Confirm allocation/default duration and allowed zero/negative/nonfinite values; establish path validation and overwrite behavior in writer. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Allocation default duration, numeric/path validation, overwrite and finalization policy remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcNotificationItem
# ArcNotificationItem

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 5 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37c730`, class_ro `0xe2a7a58`; instanceStart 8, instanceSize 24 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


title and message are object slots at +8/+0x10 (0x2d0fb40, 0x2cb140). Setters replace those slots through objc store/release fixups (0x43eae2c, 0x420b588); destructor clears both (0x43dd5a4). No initializer is declared in this class.

## Lifecycle, callbacks and relations

No notification delivery, delegate, permission check or operating-system service call is present in these methods. Copy-versus-retain setter semantics require raw callsite verification because Ghidra substitutes ownership fixups.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable content DTO; notification platform integration resides elsewhere.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcNotificationItem.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover setter ownership from machine code and identify producer/localization expectations. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Exact setter copy/retain behavior and notification producer/delivery remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcRedirectEntry
# ArcRedirectEntry

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 8 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d108`, class_ro `0xe2aa3c0`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Native-copy init engages an inline record containing URL and UTF-16 title strings plus hidden and redirect-type fields (0x442cd3c). Getters require engagement; hidden is byte +0x38, redirectType int32 +0x3c (0x442ce68, 0x442ce7c).

## Lifecycle, callbacks and relations

redirect_entry copies both strings and scalar state (0x442ce90); destruction frees long strings (0x442cf28). ArcAnnotatedVisit redirectChain creates these wrappers, and setRedirectChain: converts them back (0x43ff0fc, 0x43ffb30). No redirect is executed by this record.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Portable history-chain payload, with producer semantics tied to history. No direct upstream RedirectEntry layout match is claimed.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcRedirectEntry.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover redirect-type enum and whether URL/title correspond to pre- or post-redirect entries; validate hidden handling. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Redirect enum, producer and pre/post-redirect timing remain unknown; displayed aggregate layout is not certified ABI. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcSiteEngagementDetails
# ArcSiteEngagementDetails

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 6 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37d298`, class_ro `0xe2aa880`; instanceStart 8, instanceSize 48 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


Optional native snapshot contains a URL string at +8 and score double at +0x20; native-copy init duplicates string storage (0x442db84), URL getter converts it (0x442dc2c), score getter requires engagement (0x442dc40).

## Lifecycle, callbacks and relations

Aggregate getter copies string and score (0x442dc54); shared lifecycle IMPs clear engagement and release heap strings (0x43ed538, 0x43ed510). No engagement scoring algorithm or persistent write occurs here.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Source-backed domain comparison: components/site_engagement/content/site_engagement_score.h:29 and :148 describe upstream score state; exact backend function identity is not established. Either route needs an appropriate profile-scoped producer.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcSiteEngagementDetails.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Identify site-engagement service query, score units/range and refresh semantics. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Score algorithm/range/units, refresh timing and profile service identity remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcTabGroupProperties
# ArcTabGroupProperties

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 12 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37e288`, class_ro `0xe2ae390`; instanceStart 8, instanceSize 56 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


This is a partial-properties value: title, color and collapsed are independently optional. title returns nil when absent (0x443d9ec); color/collapsed box values when present (0x443dc04, 0x443dc88). Passing nil to setters clears presence; nonnil values are converted (0x443dad0, 0x443dc40, 0x443dcc4).

## Lifecycle, callbacks and relations

init initializes absent property values (0x443d8e0), native-copy init assigns through 0x441e8ec (0x443d994). Aggregate getter uses 0x5533b24; destructor frees an engaged title (0x8374f04, 0x443dd18). No group is mutated or persisted by these IMPs.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Compare upstream TabGroupVisualData at components/tab_groups/tab_group_visual_data.h:18, :35: it has concrete title/color/collapsed values, unlike this Arc partial-update payload. CEF group support needs explicit service mapping.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcTabGroupProperties.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Confirm nil-versus-empty title and nil-versus-false collapsed behavior in the mutation consumer; map color enum. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Mutation consumer, independent field interaction, nil-vs-empty usage and color mapping remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcURLLoadConfiguration
# ArcURLLoadConfiguration

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 29 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37cbb8`, class_ro `0xe2a9080`; instanceStart 8, instanceSize 168 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


A 160-byte native request config lives inline. payload string and payloadData share storage at +0x20; their setters replace that same buffer (0x440d76c, 0x440d834), so they are alternate views of one payload. URL/fileURL/string fields are marshaled separately (0x440d678, 0x440d8b0). headers converts native key/value entries and returns a copied dictionary, empty when no entries exist (0x440d9f8).

## Lifecycle, callbacks and relations

method maps native 1→2, 2→1, 3→3 and other values→0 using table 0xc9d8190 (0x440d71c); setter uses the same involution (0x440d744). cachePolicy normalizes to 0/1 (0x440db1c, 0x440db2c). Constructor defaults followRedirects=true, streamResponseData=false, blockLocalRequest=false and timeout=0 (0x440dd10). timeout raw instructions load d0 from +0x98 despite an empty decompiled function (0x440db0c).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

This class configures a request and does not execute it. Both routes must preserve payload aliasing, headers, timeout/cache/redirect and block-local policy in their actual loader; CEF API compatibility is not proven by field names.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcURLLoadConfiguration.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Identify loader consumer and enum names, local-request blocking rules, payload/file precedence, streaming callback behavior and zero timeout semantics. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 2; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Actual loader, conversion edge cases, payload/file precedence, timeout and policy semantics remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcWebContentsDevTools
# ArcWebContentsDevTools

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 24 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ccf8`, class_ro `0xe2a9570`; instanceStart 8, instanceSize 56 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The object retains an engaged shared native DevTools handle at +0x20 and a shared delegate bridge at +0x10 (0x4414ccc, 0x4414f60). setDelegate:nil clears the bridge; a first nonnil delegate builds a bridge with weak delegate/self references, later assignments replace the stored delegate reference (0x4414f60). Destructor releases handle/bridge and cached toolbox contents (0x4415e74).

## Lifecycle, callbacks and relations

The bridge calls webContentsDevTools:displayAs:, inspectModeDidChange:, DidClose: and NeedsFocus: (0x4415fe0, 0x441604c, 0x44160b8, 0x441610c). webContents wraps the native target; toolboxWebContents caches a wrapper only while the native toolbox is available (0x44151a0, 0x4415204). execute combines domain/name/params and converts status; backend validates JSON dictionary params, joins domain.name, sends via 0x435dd94, and retains completion by request ID (0x4415520 → 0x435b114). Existing frontend opening uses DevToolsAPI.showPanel; the no-window path calls native creation at 0xc1057c0, installs a close observer and wraps toolbox state through 0x4346100 (0x4357078). Protocol connection creation has disabled/unavailable/attach-failure branches (0x4415b60 → 0x4355df8).

Threading: notification helper 0x435c8fc copies callable state into a posted task, and 0x435ca2c invokes that callable. This supports ownership beyond return; executor identity, main-thread delivery, reentrancy and suppression/draining of already queued work remain unverified. Atomic reference counts do not establish a safe concurrent API. Candidate-backend scheduling requirements do not establish Arc scheduling.

## Backend implications

CEF provides overlapping CDP transport, method execution and message observation (CEF pin include/cef_browser.h:633, :649, :660) plus ShowDevTools (:582). This is a source-backed partial route, not native toolbox embedding equivalence. CEF macOS parent_view forces Alloy (include/internal/cef_types_mac.h:151); Arc toolbox/native pane behavior remains a route gate.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebContentsDevTools.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Verify native toolbox view ownership/display modes and callback queue; recover subscriptions, autoEnable reference counts, connection IDs and close/error ordering. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Additional analogous signature correction; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Executor identity, main-thread delivery, native pane ownership, reentrancy and queued-work suppression/draining remain unverified. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcWebContentsMediaSession
# ArcWebContentsMediaSession

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 32 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37cde8`, class_ro `0xe2a9960`; instanceStart 8, instanceSize 72 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


init:forWebContents: copies a shared native handle and stores a weak owner; destruction destroys that weak reference and releases handle/bridge/caches (0x441a1c4, 0x441b400). setDelegate: builds a shared bridge with weak delegate/owner references (0x441a4bc). position and pipController create wrapper objects from current native state, returning nil where absent (0x441b074, 0x441b254).

## Lifecycle, callbacks and relations

Constructor recovery: Ghidra renders the owner as part of the handle aggregate and leaves param_4 unused. The encoding declares a 16-byte native handle and a separate ObjC owner. Bounded raw instructions save incoming x2 as a handle address and x3 as owner; saved x2 is dereferenced for handle words and saved x3 feeds the weak-store call. Keep this local register mapping distinct from a full caller-side C++ ABI/ownership guarantee.

PiP bridge callbacks report back-to-tab/close/settings clicks and pipControllerCreated (0x441b590, 0x441b5e4, 0x441b638, 0x441b68c). Action methods forward native operations and convert status/NSError via 0x4402548. getMediaContentTypes: instead returns a copied index-set object on success and nil after NSError conversion on failure; values below four are added, their enum names remain unknown, and missing engagement traps (0x441a848). play backend rejects unavailable contents/no active session (0x441aa20 → 0x4360b58); requestPiP also rejects unsupported PiP or already-active video (0x441a6e4 → 0x436051c). Floating volume forwarding is incomplete in Ghidra output (0x441abf0).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chromium media-session behavior is a source-backed inference against content/browser/media/session/media_session_impl.cc:704 (Resume), :739 (Suspend), :1296 (SeekTo), :1412 (SetMute). CEF SetAudioMuted at include/cef_browser.h:1003 covers a narrower host operation; it does not establish action/PiP-controller parity.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebContentsMediaSession.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Verify seek/scrub units and errors, native volume argument, action availability, PiP ownership/auto-PiP behavior and callback sequence/thread. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 1, 10; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Media enum names, complete producer/error behavior, caller ABI, volume, PiP lifetime and callback queue remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcWebContentsThumbnailsGenerator
# ArcWebContentsThumbnailsGenerator

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 13 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37ce88`, class_ro `0xe2a9b60`; instanceStart 8, instanceSize 64 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The wrapper has a weak owner, optional shared native generator and vector of shared observer bridges. init stores handle/weak owner, and destruction releases observers/handle and destroys the weak owner (0x441c3c0, 0x44191d8). addObserver: deduplicates by observer identity, makes weak observer/owner references, registers the bridge natively and stores it in the vector (0x441c680 → 0x436d904).

## Lifecycle, callbacks and relations

Constructor recovery: Ghidra renders the owner as part of the handle aggregate and leaves param_4 unused. The encoding declares a 16-byte native handle and a separate ObjC owner. Bounded raw instructions save incoming x2 as a handle address and x3 as owner; saved x2 is dereferenced for handle words and saved x3 feeds the weak-store call. Keep this local register mapping distinct from a full caller-side C++ ABI/ownership guarantee.

removeObserver: unregisters/removes its matching bridge (0x441c9e4 → 0x436de50). Callback 0x441cccc emits webContents:thumbnailReady: with a converted image or nil. requestThumbnail: converts backend status; 0x436df2c checks managed contents, observer/capture state and ThumbnailTabHelper availability. 0x436d69c installs a callback into thumbnail state.

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Chrome ThumbnailTabHelper identity is supported by kDataKeyImpl in native calls 0x436df2c/0x436d69c and source chrome/browser/ui/thumbnails/thumbnail_tab_helper.h:27, :52; capture methods at .cc:242/:356 are source-backed candidate routes. CEF capture is not shown equivalent to this observer-driven Chrome helper lifecycle.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebContentsThumbnailsGenerator.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Recover image scale/crop/format and callback timing, no-observer request behavior, stale-image handling and visibility/capture policy. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.

## Revision evidence

Original review data rows 11; see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.
FRESH REVIEW: Caller ABI, capture policy, image ownership, observer timing and teardown remain unknown; no CEF lifecycle equivalence established. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-2: ArcWebLoginDialog
# ArcWebLoginDialog

ArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. 11 declared instance methods; no declared class methods. Categories: none. Metadata identity includes category and +/- kind.

## State and layout

Superclass `NSObject`; metadata class `0xe37cf28`, class_ro `0xe2a9d80`; instanceStart 8, instanceSize 136 bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).


The inline dialog value contains proxy/trust flags at +8/+9, authority UTF-16 at +0x10, and three callable objects. init copies string/callable state (0x441d2c4); setters assign that state (0x441d5cc); destruction uses 0x4346764 (0x441d734). isProxyLogin is a byte load, recovered from raw instructions despite a bad Ghidra signature (0x40b1230).

## Lifecycle, callbacks and relations

accept:with: converts nil username/password to empty native strings and invokes the callable whose entry is at +0x38, using callable storage at +0x28 (0x441d470). cancel invokes the +0x48 callable through +0x58; close separately invokes +0x68 through +0x78 (0x441d5b4, 0x441d464). No idempotence or automatic close-after-accept behavior is established. authority only converts an existing string; it does not compute trust (0x441d440).

Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.

## Backend implications

Authentication UI can use a host callback, but three separate Arc callables and displayed security flags need matching lifetime and ordering. CEF authentication parity has not been checked here.

All Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.

## Method matrix

`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/ArcWebLoginDialog.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).


## Remaining consequential questions

Trace callable producers to Chrome login handlers; determine one-shot/reentrancy, accept/cancel/close ordering, trust calculation and app presentation thread. Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.
FRESH REVIEW: Callable producers, reentrancy/one-shot semantics, trust policy and presentation thread remain unknown. Static samples only; no complete transitive behavior, replacement ABI or runtime compatibility claim.

### spec-3: ArcAIDistilBertClassifier
# ArcAIDistilBertClassifier

ArcCore **1.170.0 arm64**, unslid engine VAs. 11 declared identities. Superclass `NSObject`; class metadata `0xe37dc48`, class RO `0xe2acf30`; raw otool `objc170.txt:33194`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

This is configuration/value transport, not a classifier execution entry point in the reviewed methods. The five-argument initializer converts four strings, substitutes empty strings for nil, stores numClasses narrowed to short, and engages optional storage (0x443a46c). Getters expose string fields and signed 16-bit class count (0x443a764, 0x443a794, 0x443a7c4, 0x443a7f4, 0x443a824). mutableAIDistilBertClassifier exposes self+8 only when engaged (0x443a450). Native copy and cleanup helpers are 0x443a9fc and 0x42a4a04 as reached by 0x443a6f0/0x443a89c; their full semantics were not traced. No callback/queue contract is established. Replacement work is preserving value conversion, optional presence and narrowing; actual model-loading service behavior remains outside this class review.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `120`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Validate numClasses narrowing/validation and later AI service loading behavior.
- Native string-copy ABI and class-count invariants.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Constructor narrowing for negative or >32767 input.
- By-value aggregate ABI and native copy helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Model loading/inference, native copy helpers and borrowed storage invalidation remain outside this sample.

### spec-3: ArcAccessibilityActionData
# ArcAccessibilityActionData

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37bad8`, class RO `0xe2a4178`; raw otool `objc170.txt:22266`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Two initializers store an action code after a four-value range check (0x43deae8, 0x43deb44); an optional string is engaged only for non-nil input in the value initializer. value deliberately collapses absent and empty string to nil (0x43dec58). actionData copies action plus optional string rather than returning an ObjC object (0x43decac); metadata supplies the aggregate ABI. Cleanup releases long-string storage (0x43ded20). No callback or thread rule was established. Action meanings and conversion to actual accessibility actions remain unresolved; preserving an integer signature alone would miss nil/empty behavior.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `48`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- What are the four action meanings?
- Enum meanings and empty-string round-trip policy?
- Enum meanings?
- Validate aggregate return ABI; action enum meanings remain open.
- Deep assignment helper and valid-action range for this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Action names, aggregate calling convention and consumer effects remain unknown.

### spec-3: ArcAccessibilityTreeNode
# ArcAccessibilityTreeNode

ArcCore **1.170.0 arm64**, unslid engine VAs. 15 declared identities. Superclass `NSObject`; class metadata `0xe37dc98`, class RO `0xe2ad0a0`; raw otool `objc170.txt:33284`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

The wrapper stores an inline native snapshot copied at initialization through 0x441daa0 (0x443ab2c), and converts native strings through 0x1c391bc for role/name/value/description/child-tree ID (0x443ab84, 0x443aba8, 0x443abcc, 0x443abf0, 0x443ac14). children returns boxed integer IDs, not recursively wrapped nodes (0x443ac38). Copy/access/destruction helpers are reached by 0x443acec, 0x443acf8 and 0x443ad04. No observation subscription or refresh mechanism was established here. A replacement must preserve snapshot fields and child-tree identifiers; integration with the live accessibility tree and update thread needs the producer's contract.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `168`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Validate copy helper and snapshot refresh policy.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Snapshot producer, copy-helper closure and update subscription/sequence are not recovered here.

### spec-3: ArcAutocompleteMatch
# ArcAutocompleteMatch

ArcCore **1.170.0 arm64**, unslid engine VAs. 27 declared identities. Superclass `NSObject`; class metadata `0xe37db08`, class RO `0xe2acb60`; raw otool `objc170.txt:32803`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization retains a native shared handle in an optional (0x4438b70), with cleanup at 0x4439d80. Most fields read native state and convert URLs or UTF-16 strings (for example 0x4438c94, 0x4438f9c). additionalData constructs an ObjC array of key/value records (0x4439348); duplicates lazily constructs/caches ArcAutocompleteMatch wrappers, and associatedKeyword wraps an optional associated match (0x44398c8, 0x4439ba8). No callback thread or native-match mutation policy is established. ArcBrowser consumes the native accessor for deletion (0x43e21d8). Chromium's AutocompleteMatch data is a source-backed comparison, not a proven binary type match: components/omnibox/browser/autocomplete_match.h:867,898,980,1083,1090. CEF navigation primitives do not by themselves reproduce ranking/provider/deletion semantics; API availability was not exhaustively audited here.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `48`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native result mutation and cache invalidation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Key vocabulary and value encoding at native producer.
- Native duplicate mutation and cache invalidation.
- Cache refresh and absence semantics.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Providers, mutation/cache invalidation and underlying native match lifetime are not established.

### spec-3: ArcBackForwardList
# ArcBackForwardList

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37bc68`, class RO `0xe2a48a8`; raw otool `objc170.txt:22728`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization retains native optional shared state and materializes backList/forwardList plus adjacent/current ArcBackForwardListItem wrappers (0x43df76c). The five ObjC getters are field reads (0x2d0fb40, 0x2cb140, 0x4084dc0, 0x4060580, 0x133ae90); infer their types from this class's metadata, not shared Ghidra names. Destruction releases native handle and ObjC fields (0x43dfc10). The native setter changes native optional state (0x43dfaf0); refreshing the materialized ObjC arrays is not shown there. No callbacks/queue were established. History navigation consumes a native item through ArcWebContentsNavigationController at 0x44180d4. CEF GetNavigationEntries (include/cef_browser.h:669) is a candidate for history enumeration, but snapshot identity/order and go-to-item parity need testing.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `72`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Ordering and invalidation of snapshot arrays; nil initialization behavior.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Backend ordering, snapshot consistency and navigation identity remain open.

### spec-3: ArcBookmarkTreeNode
# ArcBookmarkTreeNode

ArcCore **1.170.0 arm64**, unslid engine VAs. 19 declared identities. Superclass `NSObject`; class metadata `0xe37bcb8`, class RO `0xe2a4a58`; raw otool `objc170.txt:22841`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization and destruction retain/release an optional shared native handle (0x43dfd40, 0x43df69c); node accessors share an IMP with unrelated wrappers but retain their own metadata types. isValid dispatches to 0x42affa0 (0x43e05ac). liveChildren obtains current children through 0x42b00f4, creates wrappers and returns a copied array (0x43e05dc). IDs, UUID, parent ID and URL/title fields convert native values (0x43dfed8, 0x43dffa0, 0x43e0068, 0x43e01bc, 0x43e02d0). Ownership of the underlying bookmark model and validity after deletion remain open. No thread guarantee was established. Chrome bookmark-service integration is an implementation candidate, but these wrappers alone do not identify the precise service or establish a CEF route.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Bookmark invalidation after parent model mutation/shutdown.
- Native node-type enum meanings.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Confirm signed exposed -1 sentinel in arm64 ABI.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Exact invalidation events and caller behavior after deletion.
- Ordering and mutation/invalid-node behavior in deeper bookmark service.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Bookmark model lifetime and deletion races remain unknown; collection copy is not a deep-copy guarantee.

### spec-3: ArcBrowser
# ArcBrowser

ArcCore **1.170.0 arm64**, unslid engine VAs. 40 declared identities. Superclass `NSObject`; class metadata `0xe37bda8`, class RO `0xe2a4f18`; raw otool `objc170.txt:23197`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

This is an ownership and Chrome-service bridge. Init stores an optional shared native Browser, creates side-panel and creation-parameter wrappers (0x43e0c34); destruction releases those fields and delegate/native control blocks (0x43e320c). setDelegate builds weak delegate/self references or updates/clears the existing bridge (0x43e1284). Raw vtable prefix 0xda87798 links callbacks 0x43e33c4 (browser:didCreateNewWebContents:isActive:), 0x43e3458 (browser:didRestoreWebContents:withSessionId:), and extension-popup/action callbacks 0x43e3514, 0x43e35d4, 0x43e367c, 0x43e370c. This is a bounded prefix, not the full delegate protocol.

createWebContents passes native configuration through 0x42faa84, whose Arc browser_impl.cc path and tab/navigation creation calls establish a browser-layer route (0x43e154c → 0x42faa84). setWindow reaches platform association 0x43c8b18 through 0x42fa9fc (0x43e14d4); that helper keeps the initial association when already set, but the exact platform pointer/NSWindow/NSView contract still needs ABI review. takeOwnershipOfWebContents is a separate path with browser-liveness failure (0x43e1734 → 0x437b014).

Close completion storage is conditional (0x43e1190 → 0x42fa5c8). The native dead/invalid/already-closing branch invokes active completion directly before return; the live branch copies transport into pending-callback storage at +0x160/+0x168/+0x170. Pending storage identifies no dispatch queue and gives no asynchronous-only guarantee. The skip flag reaches native unload handling; live-path completion timing remains unresolved. Cookies, autocomplete, extensions, and session restore use distinct service bridges (0x43e18cc, 0x43e1f80, 0x43e23c0, 0x43e2b9c). 0x42fbd80 contains browser-deinitialized/unknown-keyword failures and captures incremental autocomplete completion. Exact invocation threads and teardown races remain unresolved. CEF supports browser creation/navigation (include/cef_browser.h:301,82), but its macOS parent_view forces Alloy (include/internal/cef_types_mac.h:110-153). Matching this native embedding contract plus Chrome service dependencies is a route gate; no drop-in CEF parity is claimed.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Session identity stability across restore.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Identifier persistence and callback effects in native setter.
- Lifetime of underlying Chrome Browser after native handle survives shutdown.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Direct completion can reenter before return; live-path completion timing, exactly-once behavior, queue and before-unload cancellation remain unproved.
- Does clearing the wrapper unregister immediately or leave an expiring native weak bridge?
- Exact platform window type, replacement behavior, and pane ownership.
- Actual NSView ownership, attachment, and Chrome Browser tab insertion rules.
- What transfers from the former owner, and which observers/delegates survive?
- Cookie partition scope, callback queue, cancellation and error delivery.
- Filter semantics and cookie partition scope.
- Cookie acceptance errors, same-site and partition-key policy.
- Which domain/path/partition matches are deleted?
- No completion barrier shown here; when are cookies durable?
- Incremental completion ordering, cancellation, and callback queue.
- Provider-specific deletion and history persistence effects.
- Native extension activation, permission and popup behavior.
- Pin-state argument, persistence and callback order.
- Context menu ownership and native window placement.
- Bounds checking, toolbar ordering and persistence.
- Disposition mapping and omnibox extension API dispatch.
- Exposed type-enum mapping.
- Wrapper equality and profile lifetime across shutdown.
- Eligibility conditions and lifetime.
- Restore completion and eligibility after service shutdown.
- Full foreign-session semantics and profile matching.
- Preference/policy source and enforcement of Apple Events JavaScript.
- Cast service availability and policy.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Live completion timing, exactly-once behavior, queue, before-unload decisions and Chrome Browser shutdown lifetime remain unknown.

### spec-3: ArcBrowserApplication
# ArcBrowserApplication

ArcCore **1.170.0 arm64**, unslid engine VAs. 61 declared identities. Superclass `NSObject`; class metadata `0xe37be48`, class RO `0xe2a51a8`; raw otool `objc170.txt:23466`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

The ObjC application wrapper obtains a shared native application, creates LocalState, and registers a bridge with weak self (0x43e6548). init forwards nil to init: (0x43e6538); initialize: converts/copies config and returns native integer status (0x43e6fc8). run reaches native main-loop/shutdown orchestration (0x43e6ec0 → 0x42b1ba8), while tryToTerminate: captures a completion transport and destroys its local copy after the native call (0x43e6ee0 → 0x42b2430). Native +0x98==0 returns 1; the first initialized termination request stores its callback at +0x150..+0x168 and initiates termination, returning 2. An already-terminating request only conditionally logs “Deferring to later” and returns 2 in this body, without storing the new callback or scheduling replay. Neither eventual delivery of that repeated completion nor public enum names follows from the log. Destruction releases native/delegate pointers and ObjC caches (0x43eae38); this does not establish when global Chromium shutdown completes.

Weak histogram and accessibility observer captures are explicit (0x43e69a8, 0x43e6d80). Adapters send periodicHistogramSamplesAvailable: (0x43eb534) and accessibilityModeChanged: (0x43eb730). Breadcrumb/crash callbacks retain blocks (0x43e6b88, 0x43e6c84); their queue and unregister race remain unknown. createBrowser/popupBrowser wrap native Browser and set delegate (0x43e7048, 0x43e7528). Profile creation checks type 0/1 and bridges immediate NSError plus a retained completion; native bridge rejects empty names/null callback (0x43e8368 → 0x42b3d74). Profile deletion (0x43e8b58) and restoration with two callbacks (0x43ea810) require deeper durability/order checks.

Tracing/network wrappers are lazy caches (0x43e7b94, 0x43e7ea4). Service access includes flags, encryption availability/key export, enterprise device ID and imports; the matrix marks unresolved backend semantics instead of inventing ownership. No credential or live profile was accessed. Four pristine-app BL instructions were independently checked in evidence/app-launch-calls.tsv (app VAs 0x1004aa8c8, 0x1004ad74c, 0x1004ad770, 0x1004ad77c). Their receiver association is older-app pseudocode inference, not runtime class proof. Declared methods and direct call evidence remain separate. CEF primitive browser creation cannot establish parity for native application/profile/Chrome services; lifecycle and service parity remain route gates.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Is nil configuration usable before initialize:?
- Repeated construction/global singleton and initialization ordering.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Scheduling thread, sampling cadence and delegate replacement while callback is in flight.
- Invocation queue and reset semantics.
- Invocation queue and reentrancy.
- Invocation thread and accessibility-mode string format.
- Exact startup phases and run-loop ownership vs app callbacks.
- Public meanings of return values 1/2, first completion delivery and exactly-once behavior, and fate of a repeated completion outside these bodies remain unresolved.
- Initialization failure/status enumeration and order relative to run.
- Window bridge contract and error handling of browser creation.
- Popup parameters and window ownership.
- Safe access before initialize and after quit; queue affinity.
- Native service lifetime and event-delivery thread.
- Profile type meanings, directory layout and callback order relative to browser construction.
- Whether lookup loads or only accesses an already loaded profile.
- Loaded-profile callback queue, nil outcome and cancellation.
- Deletion durability, active-profile restrictions and asynchronous cleanup.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- No key material was accessed; service identity/encoding and availability semantics remain unverified.
- Units are inferred as MiB from shift; policy response is deeper than setter.
- Display-ID mapping and NSError code meanings.
- Callback order, count, cancellation and restored-browser ownership.
- Immediate vs deferred shutdown and run-loop exit semantics.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Public return enum names, repeated-completion fate, first delivery and global shutdown ordering remain unknown.

### spec-3: ArcBrowserApplicationDebugProperty
# ArcBrowserApplicationDebugProperty

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37bdf8`, class RO `0xe2a5008`; raw otool `objc170.txt:23394`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

The native DebugProperty contains two inline C++ strings according to ivar metadata. Initialization and setter copy both strings (0x43e6224, 0x43e6408); name/value convert each via 0x1c391bc (0x43e632c, 0x43e6350), and destruction frees long-string storage (0x43e64e4). The native property getter copies into return storage (0x43e6374). Producer is ArcBrowserApplication debugProperties, which constructs these wrappers (0x43e8f44). No callbacks, live binding, or sequence requirement was established. A replacement can marshal two strings but must identify the debug-property producer keys and values before claiming diagnostic parity.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Producer key vocabulary, aggregate ABI, live binding and producer lifetime are not resolved.

### spec-3: ArcBrowserContextDevTools
# ArcBrowserContextDevTools

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37da18`, class RO `0xe2ac5d0`; raw otool `objc170.txt:32514`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Base metadata declares six methods; Extensions adds isEqual: and hash with distinct method identities and no same-selector base override. Initialization retains optional native DevTools shared state (0x4437200); webContents reads native +0x98 and returns nil or an ArcWebContents wrapper (0x4437348). close conditionally calls 0xc107cd4 with mode 3, but Ghidra mistakes its ABI for C close(int) (0x4437324); recheck arm64 arguments before implementing. Extensions equality/hash use extracted native identity rather than ObjC wrapper address (0x43f8f28, 0x43f91fc). Cleanup shares 0x43df69c. No thread/close-completion contract is established. DevTools frontend hosting and context identity are the route questions; no CEF frontend/Chrome DevTools parity is inferred from method names.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: `Extensions` at `0xe2a5768`. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Frontend/context lifetime after close.
- Actual C++ close mode, frontend lifetime and queue.
- Ownership/liveness of native webcontents pointer after closing DevTools.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
FRESH REVIEW: Invalid close(int) recovery remains labeled; frontend ownership, close completion and register ABI remain unknown.

### spec-3: ArcBrowserCreateParams
# ArcBrowserCreateParams

ArcCore **1.170.0 arm64**, unslid engine VAs. 12 declared identities. Superclass `NSObject`; class metadata `0xe37c078`, class RO `0xe2a5d20`; raw otool `objc170.txt:24493`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

The ObjC wrapper retains optional native shared creation parameters (0x4400f58, 0x43df69c). Native capability bytes back canResize/canMaximize/canFullscreen at native +0xa8/+0xa9/+0xaa (0x44010a4, 0x44010bc, 0x44010d4); initial bounds convert four integers to CGRect doubles (0x44010ec). initialOriginSpecified is native enum ==1 (0x4401118); pipSize traps when nested optional is absent (0x4401138), with an unusual second offset requiring ABI/disassembly confirmation. ArcBrowser constructs this wrapper (0x43e0c34). No callbacks or thread guarantee was established. CEF bounds/parent-view fields are comparison points (include/internal/cef_types_mac.h:100,110), but no mapping of resizing/fullscreen/PiP ownership is established.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Underlying creation-parameter object lifetime vs parent browser.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Coordinate space and display scaling?
- Meaning of other origin enum states.
- Validate unusual second integer offset and ABI against disassembly before implementing PiP size.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: PiP layout/calling convention, platform window/view ownership and capability consumers remain open.

### spec-3: ArcColor4f
# ArcColor4f

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37ddd8`, class RO `0xe2ad500`; raw otool `objc170.txt:33648`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Stores four floats inline and copies 16 bytes in init/accessor/setter (0x443b1c8, 0x443b214, 0x443b220). Channel getters read +8/+0xc/+0x10/+0x14 (0x43dee04, 0x43dee0c, 0xc7fdb4, 0x43dee14). The Init category independently adds initWithR:g:b:a:, packing four floats then forwarding (0x4401b90). Both C++ lifecycle methods share a single-ret IMP (0x15ac0): .cxx_construct returns self with x0 unchanged and performs no stores or zeroing; .cxx_destruct has void semantics and performs no cleanup. Ghidra also erases r's return entirely: pristine `ldr s0,[x0,#8]; ret` at 0x43dee04 recovers it (evidence/arm64-leaves.json). Ghidra's color4f return-register reconstruction is degraded; retain the metadata aggregate encoding and confirm arm64 HFA conventions. No callbacks/thread dependency or color-space/range/clamping semantics were established. This is value marshalling; no browser-service route is needed for the reviewed operations.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.



Categories in assignment: `Init` at `0xe2a5ff8`. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Confirm four-float aggregate return register convention.
- Confirm incoming aggregate argument register/indirection ABI.
FRESH REVIEW: Float aggregate input/return ABI and color-space/range semantics remain unverified.

### spec-3: ArcCommandBarRankingTensor
# ArcCommandBarRankingTensor

ArcCore **1.170.0 arm64**, unslid engine VAs. 9 declared identities. Superclass `NSObject`; class metadata `0xe37d658`, class RO `0xe2ab830`; raw otool `objc170.txt:31392`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline optional native tensor stores name, integer dimensions and float payload. Init converts shape NSNumber values with intValue and data via 0x443181c (0x4431cec); shape/data allocate boxed numeric arrays (0x4432208, 0x44322c8). Native copy and cleanup reach 0x42a5760/0x42a44a8 (0x4432164, 0x44323e4); mutableCommandBarRankingTensor exposes engaged storage (0x4431cd0). No inference execution, callback or thread contract was established in these methods. Replacement must preserve element widths and array ordering; consistency of shape product versus data length, narrowing and consumers of tensor memory remain open.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `88`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- No shape/product validation established; integer narrowing and tensor data ownership.
- Native vector-copy invariants and validation.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- By-value aggregate ABI and native vector-copy helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Shape/data consistency, native copy helper, inference consumer and mutable-storage lifetime remain unknown.

### spec-3: ArcConfirmInfobar
# ArcConfirmInfobar

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37de28`, class RO `0xe2ad5d0`; raw otool `objc170.txt:33730`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization retains optional native ConfirmInfobar with a vtable-bearing value (0x443b22c); cleanup releases its shared control block (0x443b4a8). messageText converts native UTF-16 through 0x430dbcc/0x1d9fb90 (0x443b368). accept checks engagement and calls backend vtable +0x108 if live; Ghidra incorrectly uses socket accept's C ABI (0x443b434). Callback consumption, infobar dismissal and exactly-once acceptance remain unresolved. No queue affinity was proven. Chrome infobar integration or a CEF-specific emulation needs a confirmed delegate/action contract; the message getter alone cannot specify it.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Actual receiver/register ABI, action consumption, dismissal and exactly-once semantics remain unverified.

### spec-3: ArcDevToolsConnectionDialog
# ArcDevToolsConnectionDialog

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37c168`, class RO `0xe2a6140`; raw otool `objc170.txt:24770`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline native dialog holds three callback transports, corresponding to accept/deny/disable (0x4402950, 0x440296c, 0x4402988). Each checks a cancellation flag before indirect invocation. init/setter copy via 0x4335ae8; getter copies via 0x43359c8 (0x44028f8, 0x44029b0, 0x44029a4), while destructor tears down each callback transport (0x44029bc). accept has a false C socket prototype in Ghidra: preserve metadata ABI and verify actual registers. No proof was found that any action consumes all three callbacks or guarantees single use. Persisted-disable scope, callback thread, cancellation and reentrancy are consequential unresolved requirements for remote-debugging permission UI.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `104`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Callback copy vs move, cancellation ownership and invocation sequence.
- Recover correct ObjC ABI and whether callback consumes/closes dialog.
- Exactly-once/cross-action invalidation and callback thread.
- What disable persists and for which remote-debugging scope?
- Callable-copy ownership and ABI.
- Assignment cancellation/reentrancy behavior for previous callbacks.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Mutual exclusion/consumption across actions, persisted disable policy and executor remain unknown.

### spec-3: ArcDownloadItem
# ArcDownloadItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 21 declared identities. Superclass `NSObject`; class metadata `0xe37d518`, class RO `0xe2ab250`; raw otool `objc170.txt:30874`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native shared item (0x442f548), with shared destructor 0x440a40c; methods require optional engagement and check a live backend through virtual dispatch. completedBytes returns zero when backend is missing (0x442f66c). currentState maps native 0..3 to exposed 1..4, returning 0 for no live item (0x442fc60). remainingTimeInSeconds returns -1 on status failure (0x442fcd0). pause/cancel/resume/remove dispatch through native vtable slots, including forwarding cancel's boolean (0x442fd80, 0x442fdd8, 0x442fe38, 0x442fe90). Underlying download survival after profile shutdown, callback update producer and thread affinity are not established.

Chromium components/download/public/common/download_item.h:198-215,272,481-502 supplies source-backed candidate methods for these operations, not a proven vtable layout match. CEF feasibility requires preserving item identity/lifetime, state mapping, destination path and webContents association (0x442fa70); primitive download controls alone do not establish parity.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Item validity after native DownloadManager/profile shutdown.
- Slot association with received-byte count and native download lifetime.
- Slot association with total-byte count and unknown-total sentinel.
- Exact speed units/window and native backend identity.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Association lifetime after tab close.
- Named enum states require source/vtable confirmation.
- Unknown vs interrupted item conventions and native duration rounding.
- Pause state update timing and native callback thread.
- Confirm user_cancel meaning against source/vtable and persistence.
- Resume user flag and interrupted-download constraints.
- File deletion vs history removal and object invalidation.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Source API comparison does not resolve the binary vtable, native item lifetime or shutdown behavior.

### spec-3: ArcErrorStatus
# ArcErrorStatus

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37df18`, class RO `0xe2ad840`; raw otool `objc170.txt:33906`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

An eight-byte inline native value is copied in initialization/getter/setter (0x443ba3c, 0x443ba90, 0x443ba9c); two 32-bit fields back processTerminationStatus and crashedErrorCode (0x443ba88, 0x189a538). Lifecycle IMP 0x15ac0 is a single ret: .cxx_construct returns self with x0 unchanged and performs no stores or zeroing; .cxx_destruct has void semantics and performs no cleanup. Ghidra misrepresents aggregate return state at 0x443ba90, so metadata remains the ABI anchor. No callbacks/queue or ownership of external resources were established. Preserve value bits and independently resolve status/error enums at the producer; names alone cannot define crash handling.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Status/error enum meanings are defined by producer, not this copy.
- Aggregate ABI confirmation.
FRESH REVIEW: Producer enum meanings, initial value before explicit initialization and aggregate ABI remain open.

### spec-3: ArcFreezingVoteToken
# ArcFreezingVoteToken

ArcCore **1.170.0 arm64**, unslid engine VAs. 5 declared identities. Superclass `NSObject`; class metadata `0xe37e3c8`, class RO `0xe2ae6a8`; raw otool `objc170.txt:35072`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

This is move-owned native state: initialization moves a unique_ptr into self+8 and clears the input pointer (0x443eb10). clearVote calls 0x4335e08 only for non-null engaged token; that helper clears vote/liveness references and unregisters from a collection (0x443eb94 → 0x4335e08). Destruction clears pointer, calls 0x43363b4 and frees allocation (0x443ebb0). freezing_vote_token returns the address of wrapper storage, not a new owned token (0x283ebf0). Which performance-manager vote policy/sequence is involved remains unresolved; a generic browser freeze API cannot be assumed equivalent. Repeated clearing and destruction after graph shutdown are critical validation cases.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- No public copy operation shown.
- Vote type enumeration, graph sequence and repeated-clear guarantees.
- Graph sequence requirements at destruction.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Unregistration helper closure, graph shutdown, borrowed pointer invalidation and required sequence remain open.

### spec-3: ArcHistoryItem
# ArcHistoryItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 12 declared identities. Superclass `NSObject`; class metadata `0xe37d338`, class RO `0xe2aaaa0`; raw otool `objc170.txt:30271`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native HistoryItem shared handle (0x442dfd8); cleanup/accessor share 0x43df69c/0x43df53c. historyID is formatted via 0xc248b4; URLs/title convert native strings (0x442e0fc, 0x442e1a0, 0x442e250, 0x442e318). Counts read native fields (0x442e3d8, 0x442e3f0). lastVisitTime converts native microsecond Windows epoch with saturation/special handling (epoch identification is source-backed inference from base/time/time.h:537-546 and the literal offset); visitTime can be absent (0x442e408, 0x442e4a8). No callback or thread policy was established. A history query producer and persistence semantics are outside this value wrapper, so neither Chrome-history identity nor CEF service parity is claimed.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Whether query result is stable snapshot or mutable shared backend.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: This is an epoch inference, not a binary type match; persistence/producer and native lifetime remain unverified.

### spec-3: ArcHtmlToMarkdownResult
# ArcHtmlToMarkdownResult

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37ba38`, class RO `0xe2a3fd0`; raw otool `objc170.txt:22092`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initialization copies body/title/references/selectorsApplied/metaTags and stores percentage double (0x43dd5d4). Getter IMP aliases must be interpreted with this class's layout: +8 body, +0x10 title, +0x18 references, +0x20 selectorsApplied, +0x28 percentage, +0x30 metaTags (0x43dd450, 0x420b5f8, 0x420cb48, 0x41615f8, 0x416804c, 0x420c3e0). Destructor clears five ObjC fields (0x43dd740). Copy does not prove deep copy of collection elements. No conversion engine, callback or thread guarantee is shown here. Preserve schema and nil/copy behavior; the HTML extraction/Markdown producer remains a separate route question.

The getter rows retain Ghidra’s objc_retainAutoreleaseReturnValue annotations; those object returns do not establish independent copies or ownership beyond normal return handling. See [revision notes](../revision-evidence/notes.md) for the bounded annotation check.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
FRESH REVIEW: Return handling establishes no independent copy; extraction producer, deep element copying and threading remain outside scope.

### spec-3: ArcImage
# ArcImage

ArcCore **1.170.0 arm64**, unslid engine VAs. 13 declared identities. Superclass `NSObject`; class metadata `0xe37c438`, class RO `0xe2a7150`; raw otool `objc170.txt:25812`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Native image init/copy/destruction use shared image representation storage (0x440494c, 0x4404a7c, 0x4405068). nsImage converts through 0x2a02ccc (0x4404fec); isNull checks representation state (0x440500c). Class methods have separate + identity. dangerouslyDecodeFromData:scale: is a synchronous-looking native decoder bridge over NSData bytes/length (0x4404ad4 → 0x4337b04). Full decode retains completion and forwards dimensions/maxDecodedBytes (0x4404e6c → 0x4338920 → 0x4337e00). Short decode shares IMP with ArcImageRep and supplies three zero limits (0x4404fd0). The bounded 0x4337e00 body copies valid input bytes into native allocation before task dispatch and invokes completion directly for null bytes, zero length or nonpositive scale. Decoder helper closure, pixel formats, wider resource-limit behavior, completion executor and teardown lifetime remain unresolved. UI image identity and scale conversion need validation independently of the browser backend.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- NSImage creation/caching and representation ownership.
- Logical scale/rounding in native image helper.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
- Decode format support, allocation limits, malformed image failure and sync decoder isolation.
- Decoder and completion executors, wider decoder closure, malformed-input outcomes and callback lifetime after teardown remain unresolved.
- Meaning of zero allocation limit lies in decoder bridge.
FRESH REVIEW: Ultimate executor/decoder closure, malformed-image behavior, resource limits and teardown lifetime remain unknown.

### spec-3: ArcKeyboardAccelerator
# ArcKeyboardAccelerator

ArcCore **1.170.0 arm64**, unslid engine VAs. 3 declared identities. Superclass `NSObject`; class metadata `0xe37c4d8`, class RO `0xe2a7350`; raw otool `objc170.txt:26014`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init calls NSObject init and stores native accelerator fields (0x4405fc8). key and modifiers share raw field-load IMPs with unrelated classes: self+8 and self+0x10 respectively (0x2d0fb40, 0x2cb140). This class metadata determines their actual scalar encodings, not Ghidra's ID return type. No owned ObjC field cleanup, callbacks or thread semantics were established. Key enum and modifier-mask conversion at menu/keyboard consumers remain the route questions; these methods alone do not define native event synthesis.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Key enum and modifier-mask mapping at consumers.
FRESH REVIEW: Key/modifier enum meanings and native event/menu consumers are not established.

### spec-3: ArcMemoryDumpProvider
# ArcMemoryDumpProvider

ArcCore **1.170.0 arm64**, unslid engine VAs. 4 declared identities. Superclass `NSObject`; class metadata `0xe37c618`, class RO `0xe2a7728`; raw otool `objc170.txt:26322`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

initWithName:dataProvider: retains block and creates native RAII provider (0x44077b4 → 0x432b4b8). Bridge prefixes provider name with bcny- and checks runtime state; identification of TLS/task-runner-looking state as the registration task runner is a source-backed inference. Callback adapter 0x4407b5c creates a completion block around the native dump callback and invokes the ObjC provider block. dealloc clears owned pointer, unregisters/destroys provider and then calls super; .cxx_destruct has a second null-guarded cleanup path (0x4407a04, 0x4407ac4). Do not translate both into unconditional duplicate destruction.

Candidate source identification is MemoryDumpManager registration/unregistration, supported by lifecycle shape and pinned base/trace_event/memory_dump_manager.h:80-106: manager does not own provider; task runner determines callback affinity. This source comparison does not prove which registration overload/sequence Arc uses. Outstanding completion lifetime, destruction sequence and whether callback can arrive after dealloc remain unresolved. A CEF route would need separate diagnostics integration; no public API-absence claim is made.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Exact registration sequence/task runner and asynchronous data-provider completion lifetime.
- Outstanding dump completion races and required destruction sequence.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Registration identity remains a candidate; outstanding completion and teardown races are not established.

### spec-3: ArcMenuModel
# ArcMenuModel

ArcCore **1.170.0 arm64**, unslid engine VAs. 7 declared identities. Superclass `NSObject`; class metadata `0xe37e5f8`, class RO `0xe2aef38`; raw otool `objc170.txt:35701`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native shared menu; items lazily materializes/caches ArcMenuItemModel wrappers from 0x432c5e4 (0x4440638, 0x444075c). Shared destructor releases cached ObjC collection and native handle (0x440a40c). menuWillShow/Close indirectly dispatch slots +0x100/+0x108 (0x4440a3c, 0x4440a5c). Their identification with ui::MenuModel hooks is a source-backed inference from pinned ui/base/models/menu_model.h:222,226 and call context; the binary vtable type has not been independently resolved. No callbacks/queue contract or item-cache invalidation was established. A replacement must preserve show/close lifecycle and dynamic-item changes, not only enumerate a static menu.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Dynamic native menu lifetime and cached items.
- Mutation and cache-refresh contract between menu opens.
- MenuWillShow identity is source-backed inference; dynamic menu/cache refresh order.
- MenuWillClose identity is source-backed inference; destruction after close.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Vtable identity, dynamic invalidation and show/close ordering are not established.

### spec-3: ArcNomicEmbedTextItem
# ArcNomicEmbedTextItem

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37d1f8`, class RO `0xe2aa690`; raw otool `objc170.txt:29991`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Two native C++ strings represent text and task; init converts nil inputs to empty strings (0x442d568). Native-copy init copies both strings (0x442d6f8); getters convert them through 0x1c391bc (0x442d10c, 0x442d7fc), and native getter/destructor share IMPs with PasteAndGoClassification because layouts match (0x442d150, 0x442d244). Metadata preserves distinct aggregate identities. mutableNomicEmbedTextItem returns engaged storage address (0x442d54c). No model execution, callbacks or threading policy was established. The engineering task here is value marshalling and lifetime; embedding task vocabulary and inference service are separate questions.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- No embedding inference is run by this value constructor.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Aggregate ABI must use this row encoding.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Task vocabulary, native copy helper closure, service behavior and mutable pointer lifetime remain open.

### spec-3: ArcNotification
# ArcNotification

ArcCore **1.170.0 arm64**, unslid engine VAs. 34 declared identities. Superclass `NSObject`; class metadata `0xe37c7a8`, class RO `0xe2a7e28`; raw otool `objc170.txt:26698`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native Notification handle (0x4408b0c); 0x440a40c releases native state and cached iconURL. id and browserContextQualifiedID are distinct selectors sharing the same string getter IMP 0x4408d64; do not invent different semantics from names. items/buttons construct ObjC snapshots, including text-input button flags (0x44094bc, 0x4409688), while category builds action/category objects through deeper native/platform helpers (0x4409d38). didReceiveResponse calls 0x473f138 to decode a response into tagged output storage, then directly dispatches tag 0 to didClose:YES, tag 2 to didClickSettings, and tag 1 to didClick:input: with an optional button index (otherwise -1) and nil input (0x440a1f8). The parser receives no three handlers; unknown action identifiers leave the initial close tag 0. Binary output storage uses x8 and the response uses x0; Ghidra’s displayed parameter list is not an ABI; didClose/click/settings reach native notification callbacks 0x433edd0, 0x433ee60, 0x433ef10 through 0x440a0fc, 0x440a114, 0x440a1e4. The displayed parser checks dismiss/default and close/settings/button identifiers; whether its unknown-action close fallback and nil text input are intended remains unverified. Notification-center ownership, full action schema and callback thread remain unresolved. Browser context association is wrapped at 0x44092d4. CEF route viability needs native macOS notification/action parity, not only web Notification rendering.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native notification lifetime after delivery/response.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- Do clients rely on identical identifiers across context scopes?
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Ordering and optional empty-field behavior.
- Input-button ordering and response-action identifiers.
- Category ID stability and UNNotificationAction option mapping.
- Whether nil text input and unknown-action close fallback are intended; platform action schema, native callback lifetime/thread and close-by-user semantics remain unverified.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- No ObjC cached-field refresh or underlying service lifetime guarantee is established by this setter.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Intended text-input/fallback behavior, full platform schema, native callback lifetime and delivery thread remain unknown.

### spec-3: ArcNotificationButton
# ArcNotificationButton

ArcCore **1.170.0 arm64**, unslid engine VAs. 9 declared identities. Superclass `NSObject`; class metadata `0xe37c780`, class RO `0xe2a7bb8`; raw otool `objc170.txt:26602`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Stored fields title/icon/placeholder are returned from +0x10/+0x18/+0x20 (0x2cb140, 0x4084dc0, 0x4060580). Setters use objc_storeStrong, not copy (0x420b588, 0x4401164, 0x4408ac4); isTextInput is scalar +8 (0x40b1230, 0x43e0930). Destructor clears three object fields (0x4408ad0). ArcNotification creates/fills these wrappers in buttons (0x4409688). No callback/queue contract was established. Preserve this data transport and distinguish mutable object retention from copy; platform action parsing remains in the notification class.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
FRESH REVIEW: Platform response producer and caller mutation/lifetime policies are not established by field accessors.

### spec-3: ArcOverscrollBehavior
# ArcOverscrollBehavior

ArcCore **1.170.0 arm64**, unslid engine VAs. 5 declared identities. Superclass `NSObject`; class metadata `0xe37c7f8`, class RO `0xe2a7f28`; raw otool `objc170.txt:26901`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

init calls NSObject init then stores both directional values as 0 (0x440a7b4). x/y are scalar loads at +8/+0x10 and setters directly replace them (0x2d0fb40, 0x2cb140, 0x440a7f0, 0x416803c). Shared IMP Ghidra names/types are not this class's API types. No callbacks, persistence or thread policy was established. Meaning of enum 0 and conversion to renderer/native overscroll policy must be recovered at the consuming method; no CEF mapping is asserted from these scalar fields.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `24`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Overscroll enum meanings.
FRESH REVIEW: Enum meaning and renderer/native consumer behavior remain unknown.

### spec-3: ArcOverscrollParams
# ArcOverscrollParams

ArcCore **1.170.0 arm64**, unslid engine VAs. 4 declared identities. Superclass `NSObject`; class metadata `0xe37c848`, class RO `0xe2a7ff8`; raw otool `objc170.txt:26970`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

init allocates ArcOverscrollBehavior and stores it strongly (0x440a7f8). Getter/setter/destructor access/replace/release the object field at +8 (0x2d0fb40, 0x43eae2c, 0x4166068). This retains the supplied behavior object, with no copy shown. No callbacks or thread policy was established. The consuming web-contents/renderer bridge, enum meanings and effect of mutating the shared behavior after application remain open route questions.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `16`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Behavior effect and mutation semantics at consumer.
FRESH REVIEW: Post-application mutation effects and renderer policy/sequence remain unresolved.

### spec-3: ArcPasteAndGoClassification
# ArcPasteAndGoClassification

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37d158`, class RO `0xe2aa4b0`; raw otool `objc170.txt:29847`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Native optional value contains two C++ strings, copied in init (0x442d008) and destroyed at 0x442d244. menuTitle converts first string (0x442d10c); destinationURL passes second to URL converter 0x43abd34 (0x442d13c). Native getter shares IMP/type-recovery artifacts with NomicEmbedTextItem but metadata encodings remain separate (0x442d150). This wrapper does not classify input in the reviewed methods. No callback/thread contract was established. Locate the classification producer and URL normalization/error policy before choosing Chrome omnibox integration or CEF-side emulation.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.
- URL validity and conversion failure/nil policy in deeper converter.
- Aggregate ABI must use this row encoding.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Classification producer, URL policy and native string copy-helper closure are not reconstructed.

### spec-3: ArcSelectOption
# ArcSelectOption

ArcCore **1.170.0 arm64**, unslid engine VAs. 8 declared identities. Superclass `NSObject`; class metadata `0xe37d018`, class RO `0xe2aa090`; raw otool `objc170.txt:29519`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline optional native SelectOption stores two optional strings and two booleans. Init uses native copy helper 0x83694e8 and destructor frees long-string storage (0x442bee4, 0x442c1e8). value/text return nil for absent native string, while present empty strings pass through string conversion (0x442bf94, 0x442c084); optional wrapper itself must be engaged. isSelected/isDisabled read +0x48/+0x49 with engagement guard (0x442c174, 0x442c188). No callbacks or thread guarantee was established. Select-control identity, action routing and popup ownership must be found at producer/consumer; this class specifies only option transport.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `88`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native helper copy invariants.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Copy helper and aggregate ABI.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Aggregate ABI, native copy helper and option producer/control action remain open.

### spec-3: ArcSessionTab
# ArcSessionTab

ArcCore **1.170.0 arm64**, unslid engine VAs. 11 declared identities. Superclass `NSObject`; class metadata `0xe37e238`, class RO `0xe2ae290`; raw otool `objc170.txt:34697`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native SessionTab shared handle (0x443cb9c); navigation array is cached ObjC state at +8 (0x443cde8). setCurrentNavigationIndex traps for negative/out-of-range index (0x443ccf8). setNavigations stores supplied array strongly, extracts native navigation handles and replaces native vector through 0x4340c10 (0x443d0c8); current-index adjustment was not established. groupId returns nil if absent or uppercase 128-bit hex string (0x443d7b4). lastActiveTime converts Windows-epoch microseconds with special values (source-backed epoch inference from base/time/time.h:537-546 and literal offset) (0x443cd44). Pinned components/sessions/core/session_types.h:44-49,77,86,100 provides a source-backed SessionTab comparison, not verified layout equivalence. Persistence is not implemented by this wrapper; foreign-tab restore consumes it in ArcBrowser 0x43e2c10. CEF history enumeration alone does not establish session metadata/restore parity. Threading and native-session cache invalidation remain open.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Cached-navigation consistency if native session changes.
- Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.
- Interaction with simultaneous setNavigations:.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Cache invalidation if underlying native session changes externally.
- Whether current index is adjusted; partial mutation/error behavior for invalid members.
- Token half ordering cross-check against Chromium serialization.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Session type identification is provisional; persistence, current-index/cache invalidation and native lifetime remain open.

### spec-3: ArcTabGroupInfo
# ArcTabGroupInfo

ArcCore **1.170.0 arm64**, unslid engine VAs. 14 declared identities. Superclass `NSObject`; class metadata `0xe37bd58`, class RO `0xe2a4d78`; raw otool `objc170.txt:23072`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initializer establishes defaults and an ObjC tab-session-ID collection (0x43e0970); groupId/title/tabSessionIds setters use nonatomic copy (0x43e0938, 0x43e09fc, 0x43e0a0c), while extensionGroupId/color/collapsed are scalar fields (0x43e09f4, 0x43e0a04, 0x43e0930). Destructor releases stored collections/strings (0x43e0a14). Shared field-load IMPs have different scalar/object types on other classes; this metadata is authoritative. No callback/queue or persistence was established. Replacement needs producer-defined defaults, color enum and session-ID ordering; live Chrome tab-group service integration remains outside this value class.

The setter rows explicitly retain objc_setProperty_nonatomic_copy annotations erased in the C assignments; collection copy does not establish deep copying of elements. See [revision notes](../revision-evidence/notes.md) for the bounded annotation check.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `56`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Color enum meanings and extension-ID sentinel.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
FRESH REVIEW: No deep element copy is established; enum meanings, producer defaults and session-ID ordering remain open.

### spec-3: ArcTabGroupResult
# ArcTabGroupResult

ArcCore **1.170.0 arm64**, unslid engine VAs. 10 declared identities. Superclass `NSObject`; class metadata `0xe37bd08`, class RO `0xe2a4bb8`; raw otool `objc170.txt:22973`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Initializer calls NSObject init and establishes scalar defaults (0x43e08e4). success and extensionGroupId are direct scalar fields; error/groupId setters use nonatomic copy (0x43e0930, 0x4168044, 0x43e0938, 0x420b594). Destructor clears two ObjC string fields (0x43e0940). It transports operation result rather than exposing a callback implementation in the reviewed methods. No threading rule was established. Error-string vocabulary and native/extension group-ID correspondence must come from the producer before implementing tab-group command parity.

The error/groupId setter rows explicitly retain objc_setProperty_nonatomic_copy annotations erased in the C assignments. See [revision notes](../revision-evidence/notes.md) for the bounded annotation check.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `40`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Producer error/group string defaults and meaning of extension-ID sentinel.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
FRESH REVIEW: Error vocabulary, group-ID producer and command-result semantics remain unproved.

### spec-3: ArcWebContentsAIAgent
# ArcWebContentsAIAgent

ArcCore **1.170.0 arm64**, unslid engine VAs. 6 declared identities. Superclass `NSObject`; class metadata `0xe37e648`, class RO `0xe2af008`; raw otool `objc170.txt:35777`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Init retains optional native shared agent handle and cleanup releases it (0x4440a7c, 0x43df69c). sendTextInput converts text and reaches a macOS native-view path (0x4440ba0 → 0x4354de4 → 0x473dff4); the last helper iterates NSString code units, synthesizes NSEvent key events and sends keyDown:. sendKeyPress similarly reaches enum-selected synthetic event (0x4440c34 → 0x4354ea0 → 0x473e20c). It is not established as a CDP or renderer-Mojo method. Weak native-view references are handled in the bridge. No dispatch to main/UI thread was established; native AppKit operation alone is not proof caller affinity is enforced. Exact key enum, surrogate/IME behavior, focus, modifiers/keyCode and view lifetime are critical. CEF would need compatible native event/hosting behavior; public key injection existence would not prove this contract.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Native view and web-contents lifetime vs agent shared handle.
- Unicode surrogate/IME behavior, focused view identity, event keyCode and thread affinity.
- Exact enum/string mapping and modifier/keyCode semantics.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.
FRESH REVIEW: Deeper key/IME/focus behavior, weak native view transport and lifetime remain outside this fresh sample.

### spec-3: ArcWebContentsConfiguration
# ArcWebContentsConfiguration

ArcCore **1.170.0 arm64**, unslid engine VAs. 26 declared identities. Superclass `NSObject`; class metadata `0xe37ccd0`, class RO `0xe2a9440`; raw otool `objc170.txt:28377`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Inline native config supplies direct flag bytes; constructor zeroes flags but sets initiallyHidden true (0x4414cb0). Getters/setters touch packed fields; forced size is optional int32 width/height converted to CGSize or CGSizeZero (0x4414bf0). Small/nonpositive dimensions clear it; conversion guard traps on some out-of-range inputs (0x4414c18), with NaN and rounding requiring instruction confirmation. nativeWebContentsConfiguration/setter copy packed bytes (0x4414c88, 0x4414c9c); .cxx_destruct is no-op (0xfbc0). ArcBrowser createWebContents consumes this value (0x43e154c → 0x42faa84). No callbacks/queue rule is shown. Hidden/foreground/background, PDF, downloads, autoplay and context-menu flags need consumer tracing to establish actual enforcement. CEF macOS hidden/parent_view fields (include/internal/cef_types_mac.h:105-153) are candidate comparisons, not a complete mapping.

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `32`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- NaN/overflow and rounding semantics require arm64 instruction confirmation.
- Packed native layout/ABI and unnamed byte +0xb.
- Packed native layout/ABI and consumer enforcement.
- Unnamed byte at +0xb needs native configuration cross-check.
FRESH REVIEW: Packed aggregate ABI, NaN/rounding of forced-size conversion and consumer enforcement remain unresolved.

### spec-3: ArcWebContentsNavigationController
# ArcWebContentsNavigationController

ArcCore **1.170.0 arm64**, unslid engine VAs. 19 declared identities. Superclass `NSObject`; class metadata `0xe37cd98`, class RO `0xe2a97d0`; raw otool `objc170.txt:28749`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.

Chromium source comparisons use commit `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c` (156.0.8078.48). CEF comparisons use `426920c33ef22551e0b8f48cf343d623ac25642d`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).

Native controller is optional shared state at +0x10 with weak owner +8 and a vector of shared observer bridges (metadata; 0x4416bec, 0x43fe218, 0x44191d8). The owner getter at 0x43fe200 performs a weak retained load and an autoreleased return; Ghidra injects both runtime operations into a misleading field-load display. The weak slot can yield nil after owner expiration; the temporary does not confer independent caller ownership. Init's by-value native/owner arguments are mis-split by Ghidra; use metadata and verify registers. addObserver deduplicates by pointer, creates weak observer/owner bridge and registers (0x4416eac → 0x4333598); removeObserver unregisters then erases/releases, with absent observer a no-op (0x4417210 → 0x4333650). Raw bridge vtable 0xda882d0 links provisional callback 0x44193c0, which wraps ArcNavigation and sends webContents:didStartProvisionalNavigation:. Four distinct later targets lack exact indexed bodies (0x44195b4, 0x44197a8, 0x441999c, 0x4419b90); their semantics are unresolved.

Short load supplies transition=1/fromAddressBar=true/typedWithHttpScheme=false (0x4417484); full load normalizes transition and returns ArcNavigation or nil on bridge failure (0x441749c → 0x4366ac0). The bridge validates destination and initiator origin, detects cleared web contents, and has separate Chrome browser-tab and standalone-webcontents branches. Back/forward/go-to-item/reload wrap native navigation result (0x44179dc, 0x4417d58, 0x44180d4, 0x441853c, 0x44188bc). Reload bridge maps bypass mode to enum 2/check-for-repost true (0x43680fc).

Pinned content/public/browser/navigation_controller.h:565-592,647-692 and page transition/reload enum headers provide source-backed comparisons, not exact binary identifications. CEF provides GoBack/GoForward/Reload/ReloadIgnoreCache (include/cef_browser.h:82-118), but those void commands do not directly implement ArcNavigation object identity and observer ordering. Callback queue, cancellation, stale history items, same-document events and post-removal delivery need behavioral tests. Parent-view forcing Alloy (include/internal/cef_types_mac.h:151) remains relevant when combining navigation with Chrome services.

Revision evidence for corrected annotations: bounded original bodies, segment-translated instructions and limits are recorded in [revision notes](../revision-evidence/notes.md).

State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.

Instance start `8`, instance size `64`. Protocol declarations: `none in class metadata`.



Categories in assignment: none. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.

The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.


Unresolved engineering questions (method-specific evidence above):

- Confirm argument registers and weak owner store in disassembly.
- Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.
- Full callback set, post-removal queued callbacks and invocation sequence.
- Whether pending notifications can outlive removal.
- Error visibility, URL fixup and observer ordering; deeper Chrome tab-navigation behavior.
- NavigationHandle and event ordering for same-document/history traversals.
- Event ordering and inability-to-go-forward behavior.
- Stale/foreign history item validation and callback order.
- Repost prompting and error delivery.
- Parity with Chromium BYPASSING_CACHE and repost UI behavior.
- Confirm aggregate ABI and native service lifetime independently of shared handle retention.
- Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.
- Confirm nontrivial native default-constructor semantics before translating layout.

- Caller retention and lifetime across teardown or concurrent owner changes remain unproved.
FRESH REVIEW: Caller retention, concurrent weak changes, teardown and four missing exact observer bodies remain unresolved.

### spec-4: ArcAutocompleteMatchAdditionalData
# ArcAutocompleteMatchAdditionalData

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 64 bytes; class metadata `0xe37dab8`, class_ro `0xe2ac8e0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Owns an inline optional pair of native byte strings. `0x4438824` converts nil key/value to empty strings; `0x442d150` copies both strings to native return storage; `0x442d54c` returns receiver-backed mutable native storage only when engaged. Initializer IMPs: `0x4438824`, `0x4438a6c`. Cleanup IMP: `0x442d244`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc value adapter: preserve string conversion, nil/empty distinctions and C++ optional ABI. Backend autocomplete key meanings are separate; no CEF equivalence is established.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Which additional-data keys are read by the app or ranking code, and may native mutable pointers outlive their wrapper?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Data keys and external mutable-pointer consumers remain untraced.

### spec-4: ArcBreadcrumb
# ArcBreadcrumb

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 136 bytes; class metadata `0xe37dce8`, class_ro `0xe2ad1c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Stores inline category/message strings, unordered string map and optional file ID/line. `0x443ad84` materializes NSDictionary; `0x443ae68` and `0x443af4c` distinguish absent optional values from present strings/numbers. `0x443af90` forwards native record assignment. Initializer IMPs: `0x443ad2c`. Cleanup IMP: `0x8374b44`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc telemetry container. Conversion can be specified independently of browser embedding; its producer/crash reporter semantics need separate tracing.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Are file IDs stable source identifiers, and do callbacks use zero- or one-based lines? No such meaning is inferred from field names.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Constructor/assignment helpers and producers are not exhaustively followed; line-number origin remains unknown.

### spec-4: ArcBrowserApplicationConfiguration
# ArcBrowserApplicationConfiguration

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 11 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 56 bytes; class metadata `0xe37be98`, class_ro `0xe2a53f8`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

`0x43eca0c` calls NSObject init and stores configuration fields, but its recovered stack-argument expressions are malformed. At entry-SP+0/+1 raw byte loads feed self+9/+10; entry-SP+8 supplies the pointer passed to the object-store call for self+0x30. This corroborates the final field stores while the C prototype remains unreliable ([instruction analysis](../revision-evidence/analysis.md)); `0x43ecb14` clears ObjC string/array slots on destruction. This is configuration storage; no profile loading or run-loop termination occurs in these methods. Shared getter `0x40b1230` is a raw ARM64 byte load despite Ghidra dropping its result; exact bytes are in accessor-instruction-checks.tsv.

## Threading and caller/callee relations

The existing APP-layer evidence has a selector-stub call from Arc 1.168.0 `0x1004ad718` in function `0x1004ad314` (initial/app168-launch-calls.tsv and app168/ghidra/decomp/0x1004.c:128994), with receiver association based on old pseudocode. This is evidence of that call site, not blanket proof of runtime receiver types or all declared calls. No scheduling appears in engine constructor `0x43eca0c`.

## Chrome / CEF route implications

Startup configuration must be consumed by the application/browser-main integration. Preserving these fields alone is insufficient to reproduce startup, helper selection, profile loading or termination. Neither CEF nor Chrome route equivalence is established by the data object.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Trace every restoration-policy enum and startup field into application initialization; validate argc/argv consistency and suite-name nil behavior. Keep the malformed recovered prototype distinct from the declared metadata encoding and raw stack stores.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Startup consumers, restoration-policy meanings, invalid argc/argv and suite-name nil semantics remain unverified; recovered C is not a callable ABI.

### spec-4: ArcBrowserContext
# ArcBrowserContext

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 60 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 96 bytes; class metadata `0xe37da68`, class_ro `0xe2ac7f0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Native context is an engaged optional shared handle at +0x30 (`0x443739c`); service accessors cache ObjC wrappers with associated-object policy 1: preferences `0x44374c0`, history `0x44376b0`, downloads `0x44378a0`, effects `0x4437a90`, extraction `0x4437c80`. Context delegate creation uses weak delegate/self (`0x43ed9a8`); bookmark observer vector owns shared bridge handles (`0x4437fe4`, `0x443808c`, `0x43f1968`, `0x43f21a8`). Destructor `0x4438580` releases those structures. Cached wrappers and native Profile shutdown are distinct lifetimes. The nil delegate path cleans up a returned temporary; stored-bridge removal and callback suppression are not established. See [revision evidence](../revision-evidence/analysis.md).

## Threading and caller/callee relations

Category overrides are essential: base id/metrics/extensions at `0x4084dc0`/`0x2d0fb40`/`0x2cb140` simply read ivars, while Extensions `0x43f541c` constructs native ID and `0x43f5648`/`0x43f598c` cache real service wrappers. Same-IMP aliases exist for changed-permissions/content-settings (`0x43ee38c`) and chat/product closure (`0x43f5cd4`). Native WebContents construction is `0x43f35bc` → `0x42d7064`; latter has the Arc browser_context_impl.cc CreateWebContents source marker. Delegate adapters `0x43f680c`/`0x43f68cc` invoke show/closed DevTools callbacks. Other vtable targets include missing exported entries `0x43f695c` and `0x43f6ae4`; their behavior is unresolved. URL loads, cookie/storage/favicon operations capture callbacks; callback queue and shutdown cancellation are unproven.

## Chrome / CEF route implications

A replacement needs one coherent profile/service identity across bookmarks, content settings, cookie/storage partitions, importers, favicon, search and WebContents. Chrome API analogue: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/browser/profiles/profile.h:68` (see source index for anchors), and targeted native bridge evidence in methods.tsv. This is a service-integration requirement inferred from calls, not proof each feature requires a private Chromium patch. CEF feasibility needs individual native service and app-call mapping; no mapping is claimed from names or absent source files.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Establish profile-detach and shutdown sequencing; callback cancellation/queues; bookmark managed-node rules; partitioned cookie/storage behavior; import rollback; actual app-called subset. Recover missing delegate adapter bodies using disassembly if needed.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Nil detachment/native registration lifetime, queued callbacks, profile shutdown and missing exact-entry delegate adapters remain unresolved.

### spec-4: ArcBrowserContextEffects
# ArcBrowserContextEffects

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 19 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37d978`, class_ro `0xe2ac3d0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Service optional lives at +0x10; wrapper initialization is `0x4436384`/`0x44364a8`. Weak owner accessors `0x43fe200`/`0x43fe218` and destructor `_objc_destroyWeak` at `0x4435e2c` establish weak owner, independently of metadata object type. `0x44365f8` and `0x443684c` share register backend `0x42e1d80` with mode 0/1 for stylesheet/script. Filter registration copies 80 bytes (`0x4436aa0`).

## Threading and caller/callee relations

Backend `0x42e1d80` and scheme setter `0x42e3140` show `../../arc/browser/browser_context/effects_impl.cc` and NotifyBoostConfigChanged task setup. Zoom registration `0x4436dec` uses log(factor)/ln(1.2); removal `0x4436ec0` restores service default. This agrees with zoom conversion API Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `third_party/blink/public/common/page/page_zoom.h:27` as a source-backed inference. No whole-service thread contract or renderer acknowledgement was recovered.

## Chrome / CEF route implications

Arc maintains per-host configuration and notifies renderer-side effects. A CEF shim would need matching frame/world/timing, persistent host rules, matrix filtering, color-scheme and zoom behavior; public API coverage has not been verified here. Chrome route still needs the Arc effects IPC/configuration contract.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover host-pattern grammar and native map semantics, renderer injection timing/world, invalid zoom inputs, persistence and update acknowledgement. Do configuration updates affect existing documents immediately?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Host matching, renderer acknowledgement/world/injection timing and persistent behavior not established.

### spec-4: ArcBrowserContextHistory
# ArcBrowserContextHistory

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 29 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 72 bytes; class metadata `0xe37d928`, class_ro `0xe2ac2d0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializers `0x4435ef8`/`0x443601c` copy native shared service at +0x30; owner weak operations are `0x43fe200`/`0x43fe218`, with `_objc_destroyWeak` in `0x443616c`. Delegate/visit observer bridges are shared handles. `0x40439c4` creates weak observer and owner captures, installs through `0x4332f08`, and changes the observer for an existing bridge when the supplied observer is nonnil. Nil takes a temporary-handle cleanup path; stored clear/unregister and callback suppression are not established. See [revision evidence](../revision-evidence/analysis.md).

## Threading and caller/callee relations

Wrapper operations convert URLs, NSDate epoch seconds and native annotated visits before forwarding (e.g. `0x40435ac`, `0x4043c88`, `0x4043e04`). `0x4044764` → `0x43332d0` → `0x42ee818` and originator deletion `0x4044a1c` → `0x4333408` → `0x42eeb5c` show a bounded two-bridge closure, not the whole DB stack. Visit callback `0x4045110` wraps ArcAnnotatedVisit and sends browserContext:didVisitURL:. These facts establish callback payload adaptation; the delivery queue, synchronization and cancellation guarantees remain unresolved.

## Chrome / CEF route implications

Needs history query/deletion/expiration, annotated-visit insertion/retrieval and sync originator/cache GUID behavior with app-compatible callback payloads. Source analogue Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/history/core/browser/history_service.h:94` is an inference from service shape, not a proven symbol match. CEF history navigation is not yet demonstrated to cover this DB/service contract.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover DB task sequencing, deletion interval inclusivity, sync-originator IDs, top-site ranking, observer ordering and nil/delegate removal behavior. No claim that these are all app-used methods.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Native observer ownership, nil detachment, DB intervals, delivery queue, synchronization and cancellation remain unproved.

### spec-4: ArcBrowserContextPreferences
# ArcBrowserContextPreferences

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 42 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 64 bytes; class metadata `0xe37d888`, class_ro `0xe2ac070`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializers `0x4434cc4`/`0x4434de8` copy optional service at +0x10; weak owner is established by `0x43fe200`, `0x43fe218` and destructor `0x44191d8`. `0x4434f38` creates weak owner/observer bridge and stores shared handle in vector; `0x4435280` unregisters and removes by observer identity. Callbacks `0x43fc79c`, `0x43fc7f0`, `0x43fc844` report preference updates, search-engine updates and added engine.

## Threading and caller/callee relations

Boolean keys are explicitly present: download.prompt_for_download (`0x4435570`), search.suggest_enabled (`0x44355e0`/`0x4435624`), credentials_enable_service (`0x4435694`/`0x44356d8`/`0x443571c`), autofill.credit_card_enabled (`0x443581c`/`0x4435860`/`0x44358a4`) and omnibox.keyword_space_triggering_enabled (`0x4435b04`/`0x4435b48`). Modifiability calls differ from value reads. Notification enable getter `0x443578c` tests native setting !=2, while setter `0x44357cc` writes 3/2. Search-engine bridges `0x42f3024`/`0x42f3334` queue work around service loading; no general callback thread guarantee follows.

## Chrome / CEF route implications

Source-backed key identification: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/common/pref_names.h:377` and :419; Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/password_manager/core/common/password_manager_pref_names.h:47`; Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/autofill/core/common/autofill_prefs.h:95`; Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/omnibox/browser/omnibox_pref_names.h:71`. PrefService distinguishes value/modifiability (Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/prefs/pref_service.h:159`, :234, :239). TemplateURLService operations are at Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/search_engines/template_url_service.h:264`/:275/:362. These support Chrome-service hypotheses; exact symbol matching and CEF equivalents remain unproven.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Validate policy precedence, native detached/error behavior, observer reentrancy, search-model load state, activation/default restrictions, download safety and path persistence.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Observer registration/removal and concurrent/reentrant vector mutation are separate from assignment; policy/service loading and persistence not end-to-end verified.

### spec-4: ArcBrowserSidePanel
# ArcBrowserSidePanel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 13 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37c118`, class_ro `0xe2a5f70`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializer `0x44013f0` retains native shared panel; delegate shared bridge is at +0x18 and destructor `0x4401808` releases both. `0x44011d8` initializes weak delegate/self references; nil drops wrapper bridge. Title getter `0x44014fc` converts native title and `0x4401520` returns nil or a newly constructed ArcWebContents wrapper.

## Threading and caller/callee relations

Closing uses two distinct calls: `0x4401170` requests closure through `0x43c3588`; `0x44011a4` informs native panel after app closure via `0x43c1fe8`. Callback adapters `0x4401984`, `0x4401a10`, `0x4401aa0`, `0x4401b04` emit didShow, didUpdateTitle, willClose:withAnimation: and willUnhide. Session query `0x4401570` → `0x4346bd8` yields active-entry flag, extension ID and optional cached contents. Native liveness checks are visible; callback queue, animation ownership and reparenting are unresolved.

## Chrome / CEF route implications

Requires native WebContents sharing, per-tab entry/cache identity and extension-associated panel state. Chrome analogue is Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/browser/ui/side_panel/side_panel_registry.h:25` (candidate service, not proven function identification). CEF route must reproduce this panel lifecycle in addition to native view hosting; a generic child browser alone does not establish equivalence.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Do callbacks remain possible after setDelegate:nil? Who calls didFinishClosing and when can cached contents be reattached? Recover session ID mapping, native view contract and destruction during animation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Native view reparenting, animation ownership, callback suppression and cached content lifetime remain unknown.

### spec-4: ArcBrowserSidePanelEntryInfo
# ArcBrowserSidePanelEntryInfo

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c0f0`, class_ro `0xe2a5e50`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Simple ObjC result record: active flag, extension-ID string and cached ArcWebContents. Setters use normal field/strong-store operations (`0x43e0930`, `0x420b588`, `0x4401164`), destructor clears object slots (`0x43e0940`). Producer `ArcBrowserSidePanel` `0x4401570` fills these fields. Initializer IMPs: . Cleanup IMP: `0x43e0940`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Result adapter for native panel queries; cachedWebContents identity and actual view ownership belong to native panel/controller behavior, not this data record.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Determine whether retaining this result may keep a cached WebContents alive after tab/panel closes.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Retaining this result may extend object lifetimes; native contents/view ownership is not proved by strong ObjC slots.

### spec-4: ArcCommandBarRankingModel
# ArcCommandBarRankingModel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 48 bytes; class metadata `0xe37d6f8`, class_ro `0xe2aba10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional directory byte string and integer version; `0x4432940` populates descriptor, `0x442c454` copies it, `0x442c260` returns borrowed mutable storage. This class does not itself load or execute a ranking model. Initializer IMPs: `0x4432940`, `0x4432a5c`. Cleanup IMP: `0x43ed510`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc model-descriptor ABI; no browser engine dependency is established by this container. Loading/training integration belongs to its consumers.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Model layout/version compatibility, filesystem ownership and whether directory must be absolute remain unresolved.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No model-file loading/training proof; path, version and external mutable-pointer lifetime remain open.

### spec-4: ArcCommandBarRankingTrainingResult
# ArcCommandBarRankingTrainingResult

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 18 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 128 bytes; class metadata `0xe37d608`, class_ro `0xe2ab730`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional result stores outcomes, failure reason, tensor delta vector, model name/version/sample count, loss vector, float norms/ratio and duration. `0x4431310` performs conversion/storage, `0x4431b28` boxes losses. Category delta `0x4402480` invokes tensor converter `0x44022d8`, but Ghidra places cleanup `0x42a43ac` before vector reads; recovered lifetime semantics are suspect and deliberately unresolved. Initializer IMPs: `0x4431310`, `0x4431a34`. Cleanup IMP: `0x4431cb0`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc data interchange, not model execution. Preserving aggregate layout and array/tensor conversion is independent of CEF navigation APIs; training producer behavior is outside this class.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Resolve delta cleanup ordering using coherent disassembly. Validate tensor shape/data deep copy, failure enum values and producer guarantees.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Accept as incomplete; do not derive UAF or functional output from malformed recovered ordering.

### spec-4: ArcCookie
# ArcCookie

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 26 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37d568`, class_ro `0xe2ab3c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native Impl; `0x442ffa0` creates native cookie and `0x4430144` shares a supplied handle. String getters/setters operate on Impl fields; flags at +0x60..+0x63, expiration double +0x68 and SameSite int +0x70 (`0x4430870`..`0x4430948`). Mutations are direct record changes, not demonstrated cookie-store writes. Initializer IMPs: `0x442ffa0`, `0x4430144`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Must map Arc cookie fields to the chosen backend and preserve expiry units/SameSite/hostOnly/session distinctions. Store operations occur on ArcBrowserContext (`0x43f2954`, `0x43f2aec`, `0x43f2ca8`); CEF cookie-manager semantic coverage is unverified here.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Establish expiration unit and epoch, cookie flag defaults, SameSite enum, partition keys and whether wrapper mutations ever persist automatically.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Most flag/string accessors sampled; expiry epoch, partition keys, enum semantics and cookie-store persistence remain unverified.

### spec-4: ArcCpuInfo
# ArcCpuInfo

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 29 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 112 bytes; class metadata `0xe37ca78`, class_ro `0xe2a8bb0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

`0x440c34c` snapshots native CPU data and strings. On this arm64 slice many x86 feature flags are zeroed; armPartNumber and armImplementer are explicitly initialized to zero despite informative getter names. The shared armImplementer getter `0x40b1230` needs metadata type and raw LDRB interpretation. Initializer IMPs: `0x440c34c`. Cleanup IMP: `0x440c604`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Diagnostic snapshot can be implemented without browser services, but preserve observed arm64 representation rather than synthesizing hardware information from method names.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Verify actual hardware queries, supported ARM feature extraction, selector-to-ivar mismatch (armImplementer loads byte at +8/armPartNumber_, armPartNumber loads +0x68/armImplementer_) and stale snapshot expectations.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Remaining trivial feature getters are mechanically checked/sampled; native CPU provider internals and cross-architecture semantics not reviewed.

### spec-4: ArcCrashKeysReporter
# ArcCrashKeysReporter

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 1 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37dea0`, class_ro `0xe2ad660`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

No instance ivars. Class method `0x443b578` lazily creates process-global reporter storage and installs an observer callback with explicit weak capture; nil installs an empty callback. Adapter `0x443b78c` converts native name/value and invokes onCrashKeyEmitted:value:.

## Threading and caller/callee relations

Global reporter setter is the only declared method. Weak capture construction is explicit; reporter producer sources, cross-thread synchronization and whether initial keys replay are unresolved (`0x443b578`, `0x443b78c`). No all-thread safety claim follows from the singleton guard.

## Chrome / CEF route implications

Arc crash-key observation bridge. It must remain compatible with emitted key/value data independently of backend crash reporter choice; this review does not establish Sentry semantics or CEF coverage.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Find crash-key emission sources and observer replacement ordering; establish callback queue, replay and process lifetime.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Emission sources, sequencing and callback thread remain unproved.

### spec-4: ArcCreditCardInfoSaveRequest
# ArcCreditCardInfoSaveRequest

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37e468`, class_ro `0xe2ae848`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializer `0x443ed30` stores shared native optional; generic getter/destructor/constructor at `0x43df53c`/`0x43df69c`/`0x43df760` must be interpreted with this class metadata. Save `0x443ee54` → `0x433487c` accesses native controller and reports outcome 1; cancel `0x443ee68` reports outcome 2 after liveness checks.

## Threading and caller/callee relations

Native request guards trap for disengaged state; controller liveness and shared request lifetime are distinct. No callback or main-thread assertion is declared here. Callback consumption and repeat-call safety are not proven by `0x433487c`/`0x7c7dca0`.

## Chrome / CEF route implications

Chrome autofill/save-card integration candidate based on controller-call shape, not yet symbol-identified. Requires resolving persistence, local/upload save and callback ownership; a UI stub that only returns would miss native state transitions. CEF coverage is open.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Pin enum outcome meaning and concrete controller class, then validate one-shot behavior, closed dialog handling and profile storage selection.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Concrete Chrome controller name, one-shot consumption and local/upload persistence are explicitly unproved.

### spec-4: ArcExtensionCommand
# ArcExtensionCommand

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 120 bytes; class metadata `0xe37df68`, class_ro `0xe2ad960`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline native strings, optional shortcut and Boolean global flag. `0x443baa8` and `0x443bc14` use native copy/assignment helper; `0x443bb24` preserves nil for absent shortcut; `0x41544d0` reads global flag. This class contains no command execution method. Initializer IMPs: `0x443baa8`. Cleanup IMP: `0x443bc20`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc value adapter to extension command service. Keyboard dispatch, global registration and permissions require separate service tracing; no CEF extension feature equivalence is implied.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover shortcut syntax, command identifiers, global registration semantics and native copy-helper ownership.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Constructor/copy/destructor helpers are sampled, not fully expanded; command dispatch/registration is outside this record.

### spec-4: ArcExtensionRuntimeErrorStackFrame
# ArcExtensionRuntimeErrorStackFrame

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 72 bytes; class metadata `0xe37d428`, class_ro `0xe2aae60`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional native StackFrame with two unsigned 32-bit numbers and two UTF-16 strings. `0x442ec50` copies data/strings, `0x442ed38`/`0x442ed4c` read numeric fields, `0x442ed60`/`0x442ed90` convert strings and `0x442edc0` returns a native copy. Initializer IMPs: `0x442ec50`. Cleanup IMP: `0x442ee58`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Data adapter for extension runtime error reporting; browser/extension execution is not performed by this class.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Preserve unsigned widths and optional ABI; line/column origin and source URL normalization are not established.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Simple numeric/string getters sampled; line/column origins and URL normalization remain unproved.

### spec-4: ArcExternalProtocolDialog
# ArcExternalProtocolDialog

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 9 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 160 bytes; class metadata `0xe37e008`, class_ro `0xe2adb80`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional contains UTF-16 program/origin and three callback function objects; initializer `0x443bda8` and getter `0x443bec8` copy through `0x4335c68`, destructor `0x443bf10` calls `0x4335bdc`. Accept `0x443be94`, cancel `0x443beb0`, close `0x443be7c` invoke different stored callbacks. No callback consumption/reset is visible in these small IMPs.

## Threading and caller/callee relations

The Boolean supplied to accept: is forwarded unchanged. close has malformed Ghidra receiver/return type; metadata remains ABI evidence. No callback scheduling is present in these IMPs, so backend/app thread contract is unresolved. Separate callback invocations do not prove mutually exclusive or idempotent actions.

## Chrome / CEF route implications

Embedding must bridge external-protocol UI decisions to native launch/policy behavior and preserve three separate callbacks. Source/API matching and CEF request-handler route remain unverified; no external application launch was performed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Determine accept Boolean meaning, remember-choice persistence, invocation order, single-shot semantics and callback lifetime after native dialog close.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No single-shot/reset/idempotence/queue guarantee inferred; copy/destructor callback helper internals not exhaustively expanded.

### spec-4: ArcFindTextWithDirectiveResult
# ArcFindTextWithDirectiveResult

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 40 bytes; class metadata `0xe37e058`, class_ro `0xe2adc70`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline native record with byte-string directive and Boolean result; `0x443bf24` copies native input, `0x443bfd4` returns native record and `0x443c024` assigns it. `0x43e632c` converts directive; shared scalar getter `0x4081f00` reads result using metadata identity. Initializer IMPs: `0x443bf24`. Cleanup IMP: `0x440d470`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Result adapter. Text-fragment search algorithm, renderer execution and directive escaping occur in producer methods outside this assignment.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

What does false mean (no match, invalid directive or execution failure), and what encoding/escaping is used for directive?

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Trivial accessors sampled; escaping, false-result meaning and renderer producer remain unknown.

### spec-4: ArcImportSourceProfile
# ArcImportSourceProfile

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 14 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c3e8`, class_ro `0xe2a7040`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared import profile; `0x440403c` creates Safari-export source configuration, converting path and profile/display names. Type uses lookup `0x44045a8`; categories are masked to 0xfff (`0x4404770`); constraints returns native type==3 (`0x440484c`). `0x44044b0` captures completion for available-extension enumeration through `0x433ae6c`. Initializer IMPs: `0x4403f18`, `0x440403c`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Chrome/importer-service integration is indicated by native profile and import flow in ArcBrowserContext `0x43f3874`; source profile itself is data/configuration. CEF route needs independent importer support and data migration contracts.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover source type table, category bitset/constraints, Safari archive format, extension enumeration callback queue and exported-path validation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Other string accessors sampled; importer/archive/extension enumeration semantics and delivery queue unverified.

### spec-4: ArcLocalState
# ArcLocalState

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 12 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c528`, class_ro `0xe2a7420`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializer `0x440601c` copies optional shared native local-state handle; generic cleanup `0x43df69c` releases it. Preference callback registration `0x4406140` → `0x4325ac8` returns unsigned NSNumber token; remove `0x4406368` uses unsignedIntValue. Callback adapter `0x4407194` supplies converted preference-name string. Error pointer recovery in registration pseudocode is malformed; type encoding is authoritative ABI metadata.

## Threading and caller/callee relations

Native bridge `0x4325ac8` explicitly reports detached object, and typed getter backend `0x4327790` distinguishes missing preference and wrong type. Conversion `0x4406e44` maps bool/int/double/string/dictionary/list and NSNull recursively. Native policy getter `0x4327f3c` maps numeric selector to preference name, not a proven general policy service query. Callback queue, registrar thread and token invalidation remain unresolved.

## Chrome / CEF route implications

Strong source-backed candidate: PrefService FindPreference and typed getters (Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/prefs/pref_service.h:234`/:239/:240/:253/:258), PrefChangeRegistrar Add/Remove (Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/prefs/pref_change_registrar.h:55`/:63). This is browser-wide local state, distinct from profile preferences. CEF per-request-context preference access must be compared for registered keys, policy and callback semantics.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Disassemble registration error-pointer ABI; identify policy selector table, error domain/codes and callback cancellation semantics. Confirm recursive value conversion, token reuse and local-state shutdown.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No end-to-end registrar/token invalidation, numeric policy-table semantics or NSError ABI execution validation.

### spec-4: ArcLoginCredentialsSaveRequest
# ArcLoginCredentialsSaveRequest

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37e4b8`, class_ro `0xe2ae928`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializer `0x443eefc` copies optional shared request handle. `0x443f020` reads native update flag +0x2c. save `0x443f038` calls `0x7d64988` only if manager exists, cancel `0x443f058` calls `0x7d641d0` only for updates, and neverAskAgain `0x443f080` calls `0x7d64b14` if manager exists. Shared request destructor is `0x43df69c`.

## Threading and caller/callee relations

The observable asymmetry is consequential: cancel for a new-save request has no native manager call in this IMP. Native manager methods update state and virtual callbacks (`0x7d64988`, `0x7d64b14`), but their precise identity and full persistence behavior are not established. No callback queue or general concurrency guarantee is recovered.

## Chrome / CEF route implications

Password-manager UI request bridge; storage selection, blocked-origin persistence and update behavior must integrate with chosen backend. Concrete manager naming remains a source-backed hypothesis until independently anchored; CEF public API equivalence has not been demonstrated.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Validate one-shot operations, manager disappearance, account versus profile storage, never-ask scope and new-save cancellation lifecycle.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Concrete manager identity, blocked-origin/storage persistence and one-shot handling remain unproved.

### spec-4: ArcMemoryDumpMetric
# ArcMemoryDumpMetric

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 5 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c5c8`, class_ro `0xe2a7670`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Three scalar/object ivars; `0x44076e8` populates name, units and unsigned 64-bit value. Getters share IMPs with unrelated classes (`0x2d0fb40`, `0x2cb140`, `0x4084dc0`); raw load widths and this metadata preserve meaning. Destructor `0x43dd5a4` clears object fields. Initializer IMPs: `0x44076e8`. Cleanup IMP: `0x43dd5a4`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Diagnostic payload independent of Chromium scheduling; units/value interpretation belongs to dump producer.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Define accepted unit strings and whether values ever encode signed measurements or sentinel values.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Trivial getters sampled/mechanically checked; units/sentinels come from producers not reviewed here.

### spec-4: ArcMenuItemModel
# ArcMenuItemModel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 24 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 80 bytes; class metadata `0xe37e5a8`, class_ro `0xe2aee38`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native item wraps a menu-model pointer plus index. `0x443fb34`, `0x443fb68`, `0x443fc3c` through `0x443fcbc`, `0x443fcdc`, `0x443ff68` dispatch to model vtable at that index. `0x443fd88` lazily caches submenu strongly; delegate setter `0x443ff88` captures weak model/delegate and registers bridge. Initializer IMPs: `0x443fa10`. Cleanup IMP: `0x44404a4`.

## Threading and caller/callee relations

`0x443ff88` installs a native bridge; its nil path clears native delegate slots through `0x432bf9c(native,0,0)`. The wrapper getter supplies a temporary that is released; no wrapper stored-bridge clear is visible. Retained lifetime/reuse and queued callback effects remain unknown ([instruction analysis](../revision-evidence/analysis.md)). Bridge callbacks `0x4445404` and `0x4445458` emit changed/removed notifications. Index-based virtual calls and cached submenu imply a consequential invalidation boundary; no synchronization or post-removal validity is established. Category overrides are independently retained in the matrix and category-overrides.json.

## Chrome / CEF route implications

Source-backed ui::MenuModel candidate: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `ui/base/models/menu_model.h:104`/:124/:173/:187/:190/:194/:199/:211/:214. Vtable offsets are evidence, not a complete proved class match. Extensions overrides hasAccelerator/accelerator/icon with native queries (`0x44080f8`, `0x44081f0`, `0x4408318`), whereas base IMPs read ivars. Chrome-native menu state and app callback adaptation must be preserved; CEF menu equivalence needs separate comparison.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Callbacks `0x4445404`/`0x4445458` send menuItemModelChanged:/Removed:. Determine index invalidation after removal, callback thread/reentrancy, event flags and submenu lifetime. Category hasAccelerator uses a recovered secondary return register; confirm disassembly.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Wrapper bridge retention/reuse, complete native cleanup and queued/in-flight callback effects remain unknown; three helper instruction words undecoded.

### spec-4: ArcMetaTagEntry
# ArcMetaTagEntry

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 4 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 24 bytes; class metadata `0xe37b9e8`, class_ro `0xe2a3e10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Simple key/value NSString holder. Initializer `0x43dd4e8` populates object fields and destructor `0x43dd5a4` clears them; getters `0x43dd450`/`0x420b5f8` are local slot access. Initializer IMPs: `0x43dd4e8`. Cleanup IMP: `0x43dd5a4`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc data adapter; DOM extraction and meta-tag matching are outside this container.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Are key/value optional, copied or retained exactly, and does producer normalize key case? Follow constructor ARC helpers if mutable strings matter.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Trivial accessors sampled/mechanically checked; mutable string/producer normalization and DOM semantics unverified.

### spec-4: ArcMostVisitedURL
# ArcMostVisitedURL

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 7 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 64 bytes; class metadata `0xe37d248`, class_ro `0xe2aa790`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional URL byte string plus UTF-16 title. `0x442d974` deep-copies long strings, `0x442da54` converts URL representation and `0x442d10c` returns URL spec NSString; `0x442da68` converts title. Initializer IMPs: `0x442d974`. Cleanup IMP: `0x442db28`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Value adapter for history/top-sites outputs; algorithm and caching belong to native producer, not the wrapper.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

URL canonicalization and invalid URL handling, title encoding errors and ranking/caching inputs remain unresolved.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Getter conversion helpers sampled; no top-site ranking/cache or canonicalization equivalence established.

### spec-4: ArcNomicEmbedTextModel
# ArcNomicEmbedTextModel

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 48 bytes; class metadata `0xe37d1a8`, class_ro `0xe2aa5a0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional directory byte string plus version integer. `0x442d2ac` stores descriptor; native getter `0x442c454` deep-copies directory when long; mutable getter `0x442c260` borrows internal storage. Shared IMPs also serve ranking-model descriptor; identity is independently verified. Initializer IMPs: `0x442d2ac`, `0x442d3c8`. Cleanup IMP: `0x43ed510`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Arc model descriptor. No model execution, download or browser-engine call is shown in these methods; any CEF integration question belongs to consuming AI service.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Identify model format/versioning, directory normalization, download lifetime and mutable-pointer consumers.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No model execution/downloader evidence; path/version and external consumer lifetime remain unknown.

### spec-4: ArcPerformanceController
# ArcPerformanceController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37e1c0`, class_ro `0xe2ae000`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

No instance state. All six methods are class methods, sharing process singleton guarded by `0xe4a2f20` and initialized via `0x433d1bc`. Battery active/temporary-disabled bytes are read at provider+0x28/+0x29 (`0x443c65c`, `0x443c6c8`). Observer setter `0x443c7ac` holds weak callback capture; `0x443c924` invokes onBatterySaverModeChanged:.

## Threading and caller/callee relations

setHighEfficiencyModeEnabled: `0x8374b4c` → `0x7b02940` writes exact key performance_tuning.high_efficiency_mode.state with integer 2/0. Temporary disable `0x8374bc0` → `0x7afc3a8` updates byte only on change. Low-battery simulation `0x443c734` forwards unsigned duration to `0x4208bd4`; units are unresolved. Singleton construction guard does not prove thread safety of observer replacement or provider mutations.

## Chrome / CEF route implications

Source-backed memory-saver key Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/performance_manager/public/user_tuning/prefs.h:35` and battery-saver interface implementation Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `chrome/browser/performance_manager/user_tuning/battery_saver_mode_manager.cc:562` support Chrome performance-manager candidate mapping. Arc state numbering and provider behavior require specific validation; CEF route needs equivalent global policy controls, not only page throttling.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Validate duration units, provider-specific behavior, policy precedence, low-battery timer restoration, callback queue and observer initial-state replay.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Construction guard is not general thread safety; timer units, provider shutdown and initial replay remain open.

### spec-4: ArcReference
# ArcReference

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 4 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 24 bytes; class metadata `0xe37b948`, class_ro `0xe2a3c10`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Stores referenceText NSString and signed 64-bit minimum-character count. `0x43dd3c0` initializes fields, `0x43dd450` returns text and shared raw getter `0x2cb140` returns count bits using this metadata ABI. Initializer IMPs: `0x43dd3c0`. Cleanup IMP: `0x4166068`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Small Arc data record independent of browser services. Matching/extraction algorithm using this reference is not implemented by the container.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Meaning of negative/zero count, Unicode character counting and text normalization must come from consumers.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Trivial getter/destructor sampling; negative count, Unicode and consumer behavior remain unknown.

### spec-4: ArcSearchEngine
# ArcSearchEngine

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 14 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37d0b8`, class_ro `0xe2aa2b0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native search-engine record; `0x442c50c` builds from name/keyword/URL/favicon, `0x442c8ac` copies shared handle. Name/keyword getters convert UTF-16 (`0x442c9d0`, `0x442ca98`), URL uses byte string (`0x442cb60`); `0x442cd1c` tests prepopulate ID>0. Category equality compares native pointers after optional checks (`0x43ecf8c`); hash calls `0x4340180` to combine hashes from native string fields +0x3b8/+0x3d0 (`0x43ed260`). String field names and exact upstream hash symbol remain unproved. See [revision evidence](../revision-evidence/analysis.md). Initializer IMPs: `0x442c50c`, `0x442c8ac`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Native data fits Chromium TemplateURL family; source analogue Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/search_engines/template_url_service.h:264` and Arc preferences bridges `0x43fa828`, `0x43fab90`, `0x43faf00`, `0x43fb268` establish service operations. Preserve search template substitutions/status and provider policy; CEF generic URL loading alone does not establish this contract.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover status enum, template validation/substitution, favicon invalidity fallback and identity/equality across service reload.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Native string field names and exact upstream hash symbol, mutation stability and service-reload lifetimes remain unproved.

### spec-4: ArcSessionID
# ArcSessionID

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 8 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 16 bytes; class metadata `0xe37c8e8`, class_ro `0xe2a81f8`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Inline optional native signed 32-bit ID. Constructor `0x440ab88` narrows long-long to int; `0x440abb4` marks optional engaged. Native optional getter `0x2d0fb40` is an eight-byte raw load; this is aggregate bits, despite Ghidra naming an unrelated ObjC object getter. No destructor is declared.

## Threading and caller/callee relations

isEqual: `0x440ac14` compares type, engagement and native integer. identifier and hash share `0x440acd8`, sign-extending ID; `0x440acf4` assigns native optional by value. No scheduling is visible and no concurrent setter/getter guarantee is inferred.

## Chrome / CEF route implications

Native session ID adapter. Preserve 32-bit narrowing, optional ABI and hash/equality consistency. Source family Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:185` declares `SessionID window_id` in session data; no numeric correspondence with CEF browser IDs is assumed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Which ID values are sentinels, can ID be disengaged in app paths, and how are IDs mapped across restoration? Test negative/out-of-range values explicitly.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Sentinels/restore mapping, old-engine ambiguity and numeric correspondence to CEF IDs remain unproved.

### spec-4: ArcSessionTabGroup
# ArcSessionTabGroup

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 9 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c938`, class_ro `0xe2a8308`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native group record; `0x440ad10` retains native handle. `0x440ae34` converts group ID string, title getter aliases cookie-value string copy `0x440ae60`, color `0x440af20` reads int +0x30 and collapsed `0x440af38` reads byte +0x34. Initializer IMPs: `0x440ad10`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Source-backed candidate structure Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:123`/:133/:135 associates ID and visual data. Preserve serialized group identity and public color enum; this wrapper does not restore tabs. CEF restoration needs separate session data reconstruction.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Map color/group-ID serialization, optional ownership and group order; identify snapshot versus live mutation.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Pinned session structure is an analogue, not layout identity; original native title encoding differs from assuming the upstream struct wholesale.

### spec-4: ArcSessionWindow
# ArcSessionWindow

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 11 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37c988`, class_ro `0xe2a8438`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native session-window handle; `0x440af50` retains it. `0x440b0a8` and `0x440b464` wrap native tab/group collections; `0x440b36c` reads selected index. `0x440b388` converts epoch-microsecond time to NSDate, including zero/infinity sentinels. Type/show-state use mappings (`0x440b074`, `0x440b430`). Initializer IMPs: `0x440af50`. Cleanup IMP: `0x43df69c`.

## Threading and caller/callee relations

The method matrix records local getters/copies and exact bridge calls. No callback registration or scheduling contract is established by these container methods; no thread-affinity or concurrent-mutation guarantee follows. Producers/consumers outside this class remain the boundary of this review.

## Chrome / CEF route implications

Source-backed structure candidate Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/sessions/core/session_types.h:176`/:203/:216/:223 supports fields, not a proved full layout identity. Replacement must preserve session restoration data, group identity, timestamp conversion and selected-tab semantics in either route.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover enum conversion tables, timestamp sentinel boundaries, tab/group order and whether native record updates are visible through existing wrappers.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Tab/group arrays and restore behavior sampled only; enum tables, live mutation and ordering need further validation.

### spec-4: ArcSpake2Context
# ArcSpake2Context

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 5 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 16 bytes; class metadata `0xe37ca28`, class_ro `0xe2a8600`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Unique native pointer at +8. `0x440bd1c` passes role!=0 and NSData names to `0x4341ab0` → `0x4461340`; generation `0x440be50` → `0x4341c4c` → `0x44614ac` uses 32-byte message buffer; process `0x440c0b8` → `0x4341d34` → `0x44617b4` uses 64-byte key buffer. Destructor `0x440c320` invokes `0x4341b9c` and frees native allocation.

## Threading and caller/callee relations

These wrappers return NSData or NSError on invalid context/native failure. Source-backed inference identifies BoringSSL SPAKE2: Chromium DEPS at Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `DEPS:360` pins BoringSSL `2ea3200c3a3ec176613ab7a0b9a4ffc6db846120`; fetched `specs/sources/boringssl/curve25519.h:157-215` specifies one exchange per context, one generation, then processing. Role normalization and matching function signatures/buffer constants corroborate inference; it is not a mechanical binary match. Calls have no scheduling visible; concurrency on one context is unverified.

## Chrome / CEF route implications

Cryptographic library adapter can be separate from Chromium/CEF navigation. Preserve pinned SPAKE2 protocol behavior and external key-confirmation usage. Source header :195-205 describes output as keying material for confirmation/HKDF; processing alone is not proof of authenticated peer confirmation.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Use interoperability vectors to verify role/name/password binding, wrong password/peer message, call ordering, repeated operations, NSError mapping and zeroization. Do not infer protocol security from wrapper signatures alone.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No crypto vectors/runtime/peer confirmation proof; deeper crypto bodies and zeroization not fully reviewed.

### spec-4: ArcTracingController
# ArcTracingController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 13 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 56 bytes; class metadata `0xe37cb18`, class_ro `0xe2a8e20`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared controller at +0x20 and shared delegate bridge +0x10; `0x440c8fc` copies native handle, `0x440cb90` creates weak delegate/self references. Destructor `0x440ced8` releases both. isTracing `0x440cda8` locks Impl mutex +0xc0 and reads active-session +0xb8 before caching flag at self+8.

## Threading and caller/callee relations

Start `0x440ce18` → `0x434a700` configures systrace/categories/histogram list and schedules custom memory-pressure/system/swap/browser CPU tasks at nominal native delays 500000/1000000/5000000/500000. The Arc tracing_controller_impl.cc source markers identify those collection tasks. No time unit is inferred solely from constants. Stop `0x440cec4` → `0x434b768` differs from abort `0x440ceb0` → `0x434b818`; abort explicitly posts DidAbortTracing. Delegate adapters `0x440d248`, `0x440d29c`, `0x440d328` report start, completed text and abort. Getter mutex does not establish the callback queue or whole-object thread safety.

## Chrome / CEF route implications

Chrome tracing plus Arc metric collection/output contract must be reproduced. Native config names/histograms at `0x434a700` show more than a simple start/stop toggle. CEF tracing API equivalence needs checks for categories, data destination, custom metrics, errors and delegate event order; no final route decision is claimed.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Verify start Boolean return register (native Ghidra prototype is void), output artifact semantics, delegate callback queue, simultaneous start/stop/abort, timer units and cleanup after consumer disappearance.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Native start void prototype/Boolean return and delay units remain unresolved; no whole-object thread safety or event ordering proof.

### spec-4: ArcURLUtilities
# ArcURLUtilities

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 2 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37e300`, class_ro `0xe2ae420`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

No instance ivars; both methods are class methods. `0x443dd58` converts host and returns nil for empty native registrable-domain result; `0x443de84` converts formatted display host. Native bridges `0x4345480`/`0x434563c` distinguish empty/IP-like hosts and canonicalization paths.

## Threading and caller/callee relations

Registry helper call `0x4345480` → `0xda58a0` and IDN conversion `0x434563c` → `0x788520` are bounded source-backed hypotheses. Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `net/base/registry_controlled_domains/registry_controlled_domain.h:162`/:193 (fetched under specs/sources) defines registrable-domain behavior/private filters; Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/url_formatter/url_formatter.h:178`/:185 defines IDN display conversion and fallback. No locale/main-thread assumption follows from these synchronous wrappers.

## Chrome / CEF route implications

Can use pinned Chromium registry/IDN utilities in a Chrome route or reproduce identical public-suffix/IDN behavior in a shim. Return nil versus empty and private-registry filtering are compatibility gates; CEF URL utilities are not yet compared.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Test empty, IP/IPv6, trailing dots, private registries, unknown suffixes, Unicode/punycode spoof checks and invalid hosts against original engine.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: No whole-domain/IDN test vectors, locale/spoof-policy equivalence or exact helper symbol identification.

### spec-4: ArcWebContentsBoostIDE
# ArcWebContentsBoostIDE

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 10 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 48 bytes; class metadata `0xe37cc58`, class_ro `0xe2a92c0`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Optional shared native handle at +0x18; `0x4414144` copies it and `0x44149dc` releases it and delegate bridge. `0x4414268` creates bridge with weak self but delegate is stored through slot assignment; ownership of that delegate slot is unresolved and not inferred as weak. Existing bridge updated, nil drops stored bridge.

## Threading and caller/callee relations

Native callbacks `0x4413fd8`, `0x441402c`, `0x4414080` send boostIDEDidOpen:, boostIDEWillClose: and boostIDE:didRequestExecuteCommand:jsonString:. sendResponse `0x4414488` → `0x43d35c8` → `0x68122a8` calls cr.webUIResponse with true; error `0x44145b0` → `0x43d38cc` → `0x6812404` sends false; event `0x44146d8` → `0x8371aa8` sends cr.webUIListenerCallback. The native outgoing paths construct string-valued payloads through `0xc87c40`; they do not show JSON parsing. Parsing or other interpretation by receiving JavaScript remains untraced. Callback queue and JavaScript lifecycle remain unresolved. See [revision evidence](../revision-evidence/analysis.md).

## Chrome / CEF route implications

Strong source-backed WebUI identification: Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `content/public/browser/web_ui_message_handler.h:76`/:85/:91 identifies resolve/reject/listener contracts; binary strings and Boolean flag differentiate responses. CEF shim would need compatible trusted page/JS bridge, command dispatch and JS-allowed lifetime, rather than treating this as an arbitrary network/CDP request.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Recover delegate-slot ownership from ARC/disassembly, native handler availability/JS-allowed conditions, receiving JavaScript interpretation of string payloads and command ID format. Verify closing while app command is in flight.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Delegate-slot ownership, receiving JavaScript, downstream serialization and end-to-end JS-allowed/pending-ID lifetimes remain open.

### spec-4: ArcWebContentsMediaSessionPiPController
# ArcWebContentsMediaSessionPiPController

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 6 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 32 bytes; class metadata `0xe37ce38`, class_ro `0xe2a9a30`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.


## Lifecycle, ownership and callbacks

Initializer `0x441c1f8` copies native shared optional at +8; generic destructor `0x43df69c` decrements its control block. videoView `0x441c31c` returns nil for disengaged/absent native object, otherwise returns view through native pointer chain. Returned-view retain/reparent ownership is not inferred from the getter.

## Threading and caller/callee relations

sourceElementBounds `0x441c37c` requires engaged wrapper, reads native integer x/y/w/h, converts to CGRect doubles, and gives zeros if native object is absent. No scheduling, coordinate conversion or scale adjustment is shown. The view and geometry depend on nested native controller state; thread affinity and invalidation remain unresolved.

## Chrome / CEF route implications

A route gate for native macOS embedding: replacement must provide usable native video view plus matching geometry and lifecycle. This wrapper does not prove that CEF child-view hosting or CEF PiP offers that exact contract; identify native view implementation and original-engine reparenting behavior first.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).


## Remaining consequential questions

Determine actual NSView subclass, layer/backing ownership, bounds coordinate space and scale, view migration during PiP, destroyed source behavior and required main-thread operations.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.
FRESH REVIEW: Borrowed view retain/reparenting, native class, scale/coordinate space and required thread remain unproved.