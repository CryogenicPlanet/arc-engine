# ArcWebContentsPDFController

ArcCore 1.170.0 arm64. 9 declared rows; superclass `NSObject`. Class metadata `0xe37e328`, class_ro `0xe2ae4f0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Optional shared PDF-control bridge with callbacks and immediate bool/NSError outcomes.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_web_contents_pdf_controller` | 0x8 | 24 | `{optional<arc::WebContents::PDFController>=""(?="__null_state_"c"__val_"{PDFController="impl_"{shared_ptr<arc::WebContents::PDFController::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2ae4b8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `web_contents_pdf_controller`: `T{optional<arc::WebContents::PDFController>=(?=c{PDFController={shared_ptr<arc::WebContents::PDFController::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_web_contents_pdf_controller` at `0xe2ae4e0`.

## Lifecycle, ownership, callbacks and threading

0x443df90 copies native controller peer. Methods 0x443e0b4..0x443e614 capture supplied blocks and call native helpers 0x436c99c / 0x436cb2c / 0x436ccbc / 0x436cec4 / 0x436d054, then convert immediate native status through 0x4402548. Immediate bool is not the callback answer (e.g. hasBookmarks). Helpers separately reject detached native content, missing glue and missing PDF controller.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

0x436c99c and 0x436d054 cite arc/browser/web_contents/pdf_controller_impl.cc and bridge to internal PDF controller functions (0x41a7920 / 0x41a8a58). CEF 426920c33ef22551e0b8f48cf343d623ac25642d include/cef_browser.h:546 PrintToPDF is an output API; it does not establish equivalent bookmark/text/page extraction semantics.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithWebContentsPDFController: | `@24@0:8r^v16` | 0x443df90 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x443df90](../bodies/0x443df90.c); record 0xd77ca70; dyld L3079 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -hasBookmarks:error: | `B32@0:8@?16^@24` | 0x443e0b4 | Requires engaged native wrapper, captures callback, calls 0x436c99c and converts immediate status to BOOL/NSError via 0x4402548; callback carries actual query result. | 0x8790,0x436c99c,0x4402548,0x4b35898 | [0x443e0b4](../bodies/0x443e0b4.c); record 0xd77ca7c; dyld L3080; evidence/bridges-services.txt | medium / reviewed_partial / available | Callback schema/page index, ownership, delivery thread and cancellation remain unresolved. |
| (base) | -getBookmarks:error: | `B32@0:8@?16^@24` | 0x443e20c | Requires engaged native wrapper, captures callback, calls 0x436cb2c and converts immediate status to BOOL/NSError via 0x4402548; callback carries actual query result. | 0x8790,0x436cb2c,0x4402548,0x4b35898 | [0x443e20c](../bodies/0x443e20c.c); record 0xd77ca88; dyld L3081; evidence/bridges-services.txt | medium / reviewed_partial / available | Callback schema/page index, ownership, delivery thread and cancellation remain unresolved. |
| (base) | -availablePages:error: | `B32@0:8@?16^@24` | 0x443e364 | Requires engaged native wrapper, captures callback, calls 0x436ccbc and converts immediate status to BOOL/NSError via 0x4402548; callback carries actual query result. | 0x8790,0x436ccbc,0x4402548,0x4b35898 | [0x443e364](../bodies/0x443e364.c); record 0xd77ca94; dyld L3082; evidence/bridges-services.txt | medium / reviewed_partial / available | Callback schema/page index, ownership, delivery thread and cancellation remain unresolved. |
| (base) | -visiblePages:error: | `B32@0:8@?16^@24` | 0x443e4bc | Requires engaged native wrapper, captures callback, calls 0x436cec4 and converts immediate status to BOOL/NSError via 0x4402548; callback carries actual query result. | 0x8790,0x436cec4,0x4402548,0x4b35898 | [0x443e4bc](../bodies/0x443e4bc.c); record 0xd77caa0; dyld L3083; evidence/bridges-services.txt | medium / reviewed_partial / available | Callback schema/page index, ownership, delivery thread and cancellation remain unresolved. |
| (base) | -getAvailableTextOn:callback:error: | `B36@0:8i16@?20^@28` | 0x443e614 | Requires engaged native wrapper, captures callback, calls 0x436d054 and converts immediate status to BOOL/NSError via 0x4402548; callback carries actual query result. | 0x8790,0x436d054,0x4402548,0x4b35898 | [0x443e614](../bodies/0x443e614.c); record 0xd77caac; dyld L3084; evidence/bridges-services.txt | medium / reviewed_partial / available | Callback schema/page index, ownership, delivery thread and cancellation remain unresolved. |
| (base) | -web_contents_pdf_controller | `{optional<arc::WebContents::PDFController>=(?=c{PDFController={shared_ptr<arc::WebContents::PDFController::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../bodies/0x43df53c.c); record 0xd77cab8; dyld L3085 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../bodies/0x43df69c.c); record 0xd77cac4; dyld L3086 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../bodies/0x43df760.c); record 0xd77cad0; dyld L3087 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Recover callback argument schemas, page-index basis, partial/available-page meaning, errors and behavior during PDF document teardown.
