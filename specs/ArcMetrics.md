# ArcMetrics

ArcCore 1.170.0 arm64. 5 declared rows; superclass `NSObject`. Class metadata `0xe37d2e8`, class_ro `0xe2aa950`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Profile-scoped site-engagement snapshot adapter.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_browser_context_metrics` | 0x8 | 24 | `{optional<arc::BrowserContext::Metrics>=""(?="__null_state_"c"__val_"{Metrics="impl_"{shared_ptr<arc::BrowserContext::Metrics::Impl>="__ptr_"^{Impl}"__cntrl_"^{__shared_weak_count}}})"__engaged_"B}` | `0xe2aa918` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `browser_context_metrics`: `T{optional<arc::BrowserContext::Metrics>=(?=c{Metrics={shared_ptr<arc::BrowserContext::Metrics::Impl>=^{Impl}^{__shared_weak_count}}})B},R,N,V_browser_context_metrics` at `0xe2aa940`.

## Lifecycle, ownership, callbacks and threading

0x442dcf0 copies native metrics peer. getAllSiteEngagementDetails 0x442de14 calls 0x42efb64, wraps native URL/score pairs into ArcSiteEngagementDetails and returns array. 0x42efb64 returns empty vector if profile/service unavailable; otherwise a service lookup and 0x6f26c80 populate details.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

Source-backed inference: this resembles Chromium site_engagement::SiteEngagementService::GetAllDetails at d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/site_engagement/content/site_engagement_service.h:121,169. This is a Chrome keyed-service route requirement, not a verified symbol match. Source :69-73 requires that service on UI thread; that requirement is conditional on this identification, not proof of Arc caller-thread enforcement.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithBrowserContextMetrics: | `@24@0:8r^v16` | 0x442dcf0 | Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails. | 0x5810 | [0x442dcf0](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442dcf0.c); record 0xd77ae78; dyld L2413 | high / local_verified / available | Creation/detachment of the native Impl lies outside this wrapper. |
| (base) | -getAllSiteEngagementDetails | `@16@0:8` | 0x442de14 | Requires engaged peer; gets URL/score vector from 0x42efb64 and wraps each element in ArcSiteEngagementDetails. Native helper returns empty vector when profile/service missing. | 0x42efb64,0x5810 ; ObjC selectors arrayWithCapacity:,initWithSiteEngagementDetails:,addObject: | [0x442de14](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x442de14.c); record 0xd77ae84; dyld L2414; evidence/bridges-deep.txt | medium / reviewed_partial / available | Identify exact service and score choice; distinguish empty result from unavailable service. |
| (base) | -browser_context_metrics | `{optional<arc::BrowserContext::Metrics>=(?=c{Metrics={shared_ptr<arc::BrowserContext::Metrics::Impl>=^{Impl}^{__shared_weak_count}}})B}16@0:8` | 0x43df53c | Returns native optional by value; empty stays empty, engaged shared peer increments its control count. | No direct anonymous call recorded / indirect possible | [0x43df53c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df53c.c); record 0xd77ae90; dyld L2415 | high / local_verified / available | C++ optional return/storage ABI needs cross-toolchain validation. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43df69c | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x43df69c](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df69c.c); record 0xd77ae9c; dyld L2416 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x43df760 | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | No direct anonymous call recorded / indirect possible | [0x43df760](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/bodies/0x43df760.c); record 0xd77aea8; dyld L2417 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |

## Remaining implementation questions

Confirm exact service getter/score selection and whether hidden/incognito profile entries are included.


Original reviewed source: [ArcMetrics.md](../research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/classes/ArcMetrics.md).
