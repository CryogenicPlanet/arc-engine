# Thumbnail delivery audit: public guards and the remaining startup callback

Read-only production audit, 2026-10-23. Only this new note was written. No
production source, proposed patch, Chromium source, app, profile, build, lock,
shared test, existing result, or board state was changed. The parent's native5
source freeze remains intact. Original addresses are unslid arm64 VAs in
`/tmp/arccore-webextras.arm64`, inspected with bounded llvm-objdump ranges.

## Conclusion

The extra launch-slice callback is real, but existing recordings do not expose
its native producer or capture state. No safe owned behavior change is proven.
Public registration, subscription, explicit request, and callback handling must
not be changed merely to suppress this callback. The existing uncropped-pixels
and navigation-cooldown proposals address separately proved Chromium differences;
neither proves why this particular callback exists only in ours.

A tempting duplicate-add discrepancy is **not** a public mismatch. The original
has TWO deduplication layers. Auditing only the C++ layer gives the wrong answer.
Current `arc_webextras_capture.mm:117-128` correctly returns before native work on
a repeated public registration of the same observer.

## Complete duplicate-add path

Original Objective-C `-addObserver:` starts at `0x441c680`:

1. `0x441c6a8-0x441c6e8` scans the wrapper's bridge vector, weak-loads each
   observer (`0x441c6c8`), compares that observer to the incoming ObjC identity
   (`0x441c6d4`), and stops on a match (`0x441c6d8`).
2. `0x441c6f4-0x441c700` compares the found bridge iterator with vector end.
   If a match exists, the equal-to-end branch is NOT taken: execution runs
   `0x441c704-0x441c71c`, releases the incoming reference and returns. There is
   no native generator request on this public duplicate path.
3. Only an absent observer reaches `0x441c720`, allocates a new weak bridge,
   checks the native optional handle at `0x441c790-0x441c794`, and calls the
   C++ registration function at `0x441c7b4 -> 0x436d904`. It then stores the
   bridge in the wrapper vector.

The inner C++ function `0x436d904` independently deduplicates its native weak
registrations. Its `existing` branch at `0x436dafc -> 0x436db50` and new-insert
branch both converge on `0x436db7c-0x436db80 -> 0x436d69c`. That helper creates a
subscription if necessary and requests the stored image even if a subscription
already exists. This inner behavior is unreachable through a repeated public
ObjC registration because the outer wrapper has already returned. Do not move
ours' stored-image request above its public identity guard.

## Subscription and request contract

The new-public-observer native route calls `0x436d69c`:

- Native managed-contents pointer at `this+0x18` must exist (`0x436d6b0-6b4`).
  Tab-interface/helper lookups at `0x436d6b8-6d0` may return no helper; this
  registration helper returns quietly, without producing a public NSError.
- Stored subscription `this+0x40` is checked at `0x436d6d8-6dc`. When present,
  skip subscription construction and proceed to the image request at
  `0x436d808-844`.
- When absent, helper image `+0x68` must exist. `0x436d74c-750` subscribes through
  `0x731be94`; `0x436d798-804` installs the uncompressed callback `0x436dff4`.
- The existing image is requested via `0x731c7a8` at `0x436d840` (or the
  null-image tail route at `0x436d820`). The image function checks compressed
  data at `0x731c7c4-7c8`; with no stored data it returns false/no image, rather
  than manufacturing a callback. With data it schedules decode and delivery.

Original `ThumbnailImage::Subscribe` equivalent `0x731be94` appends a native
subscription; only first subscriber (`0x731bf40-748`) notifies the helper's
observed=true route at `0x731bf74 -> 0x731dd68`. That route updates thumbnail
visibility and calls LoadIfNecessary only when observed and not discarded
(`0x731dd88-0x731ddb0`). This agrees with current `thumbnail_image.cc:60-71`
and `thumbnail_tab_helper.cc:195-201`. Merely adding an observer can therefore
start scheduled capture. An explicit public request is not required.

Original public `-requestThumbnail:` `0x441cc58` requires an engaged native
optional (`0x441cc68-6c`), then calls `0x436df2c` and the status-to-BOOL/NSError
adapter `0x4402548`. The native request checks, in this order:

1. Managed contents exists (`0x436df40-44`); otherwise tagged status 0x25,
   mapped to NSError code 6, managed-webcontents error.
2. Native observer count `+0x70` and subscription `+0x40` are nonzero
   (`0x436df48-54`); otherwise success/no request.
3. Tab-interface/helper lookup succeeds (`0x436df58-70`); otherwise status0x25,
   code6, helper-unavailable error.
4. Visibility is VISIBLE (enum2, `0x436df74-80`): invoke tab-background capture
   `0x436dfa0 -> 0x731d500`. Otherwise request an existing thumbnail through
   `0x436dfa8 -> 0x731c7a8`. Return success.

Current owned request implementation at `arc_webextras_capture.mm:146-155`
matches these observable routes. `CaptureThumbnailOnTabBackgrounded` has its
own readiness, view, surface-availability, and nonempty-size guards, both in
native `0x731d518-59c` and current `thumbnail_tab_helper.cc:242` onward.

## Exact saved-record evidence

`results/webextras/launch-slice-peer-run4` is a paired immutable snapshot.
All sequence numbers below are raw recorder `seq`, not comparator projections.

Golden launch1:

- ArcWebContents#118 / ThumbnailsGenerator#135 / ADK observer#126:
  `thumbnailsGenerator` call3399/ret3400, `addObserver:` call3401/ret3404,
  `nativeView` call3405. No public `requestThumbnail:` appears.
- `webContentsClosingImminent:withSessionID:` callback5280 precedes
  `removeObserver:` call5716/ret5717. No thumbnail callback occurs.
- A DIFFERENT WC#194/service#210/observer#202 registers at6145/ret6146 and
  accesses nativeView6147. This is not a duplicate registration.

Ours launch1:

- WC#99 / service#116 / observer#107: getter3579/ret3584,
  `addObserver:`3585/ret3590, `nativeView`3591. No public request appears.
- The sole thumbnail callback is5524/ret5525, args WC#99, image(501x341),
  main-thread depth1, from engine. It occurs BEFORE closing-imminent callback
  5532 and BEFORE removeObserver6060/ret6061. It is not a queued callback
  delivered after removal or after the recorder's closing notification.
- The last recorded viewport setter before it is5188, size1042x716; the latest
  loading=false setter is5416. Those values do not expose capture source size,
  device scale, content rectangle, surface ID, or image assignment token.

Launch2: golden WC#97/service#114/observer#105 add2941/ret2944; ours
WC#78/service#95/observer#86 add3217/ret3222. Each has zero thumbnail callbacks.

Current canonical original goldens were also scanned by exact
(service receiver, observer argument) identity with removeObserver invalidating
membership. launch-slice, media, media-pip, pdf, and devtools contain **no public
duplicate adds without an intervening remove** and **no public thumbnail
requests**. Golden devtools launch2 nevertheless emits two automatic thumbnail
callbacks: seq4056 image512x352, seq4298 image427x352. This confirms that no-public-
request is not a valid reason to suppress automatic notification. Other listed
original launches have zero callbacks. The same identity scan on saved owned
media-run1, media-pip-peer-run3, pdf-run2, and native-run3 found no duplicate adds.

## What the recordings cannot attribute

Current capture flow can generate a valid automatic image from observation:
Subscribe -> observed=true -> scheduling priority when ready -> RequestCapture
(IncrementCapturerCount with stay_hidden=true) -> StartVideoCapture -> compositor
frame -> StoreThumbnail(kVideoFrame) -> AssignSkBitmap -> compression -> current-
token validation -> decode -> current-token validation -> subscriber callback ->
owned ordered weak-registration UI task -> app notification.

`ThumbnailCaptureDriver::UpdateSchedulingPriority` gates on readiness and
thumbnail-observed state (`thumbnail_capture_driver.cc:120-161`), not a public
request, page activation, or visible window. Its public UpdatePageVisibility
method has no production call site in the current thumbnails directory.
Adding a new wrapper-level visibility gate would change this native-supported
observation contract and is not justified by this diff.

Native has the same broad stale-token behavior: bitmap assignment stores a new
random token (`0x731c118-11c`); JPEG assignment compares it (`0x731c494-4a0`);
uncompressed delivery compares it again (`0x731c6ec-6f8`). Thus rapid navigations
can legitimately invalidate native work before a public callback. Existing
records contain only the ObjC boundary, not these producer transitions. They
cannot tell whether golden had no source frame, lost a scheduled capture, had
an image invalidated during navigation, or simply had different completion
ordering. Nor do they prove our callback was produced by a background video
frame rather than a preexisting/copy-from-surface image.

The independently proved native full-bitmap versus C156 cropped-bitmap
mechanism remains in `webextras_thumbnail_pixels_notes.md`. Its proposal may
correct pixels/dimensions, but does not establish actual frame geometry or
callback-count causality for501x341. The independently proved cooldown navigation
state bug remains in `webextras_thumbnail_capture.patch`; it prevents a fatal
state transition, not an established public count mismatch.

Next useful diagnostic is a matched native/C156 producer trace, using isolated
scenario profiles, of (1) first subscription/observed toggle and helper identity,
(2) readiness and scheduling/can-capture transitions, (3) source view bounds,
device scale, surface availability and compositor content rectangle, (4) capture
type/frame ID and bitmap dimensions at StoreThumbnail, (5) assignment/decode token
accept-or-drop, and (6) registration-task enqueue/delivery/cancel. Correlate those
with existing recorder sequence boundaries. Shared C156 instrumentation belongs
to lead; bounded original hooks require its own isolated native run. Until such
evidence exists, classify the extra callback as an unresolved real upstream
capture/readiness/view-lifecycle difference, not an accepted count exception and
not a proven owned callback-emission defect.

## Existing tracing feasibility (read-only follow-up)

Browser-CDP tracing is a feasible **partial** diagnostic with existing source
instrumentation. It does not require a new engine switch, private native hook,
or artificial thumbnail request. Runtime support and event delivery have NOT
been exercised in this audit. Binary strings prove instrumentation/protocol
names are present, not that a tracing service is healthy or that a particular
run emitted an event.

### Available producer events and fields

Current `background_thumbnail_video_capturer.cc` and `thumbnail_image.cc`:

| Category | Event | Existing payload / meaning |
|---|---|---|
| ui | Tab.Preview.VideoCapture | Begin/end on a per-capture track; starts after source view and nonempty bounds passed the helper, and the background capturer accepted Start. No WC pointer, bounds, scale, readiness, or observer payload. |
| ui | Tab.Preview.VideoCaptureFrameReceived | Instant with `frame_number`, a zero-based counter reset on capturer Start; emitted after shared-memory mapping/allocation-size checks. |
| ui | Tab.Preview.ProcessVideoCaptureFrame | Scoped event with process-scoped flow ID created for this browser-side frame. |
| ui | Tab.Preview.CompressJPEGWithFlow | Worker compression with that same flow ID. |
| ui | Tab.Preview.JPEGReceivedOnUIThreadWithFlow | UI-thread event terminating that flow, emitted only after AssignJPEGData's current-token comparison succeeds. |
| ui | Tab.Preview.CompressJPEG | Compression without a video frame ID, e.g. copy-from-surface path. |
| ui | Tab.Preview.JPEGReceivedOnUIThread | Accepted JPEG assignment without a video frame ID. |

Source anchors are background capturer lines64,92,133,138 and image lines155,
159,224,228. The original binary contains all seven exact event cstrings:
`0xd291731`, `0xd29174a`, `0xd29170c`, `0xd29188a`, `0xd29183c`,
`0xd2918ab`, `0xd291867`, respectively. Category `ui` is present at0xd2393d5;
`frame_number` at0xd291770. The original bounded producer/compression/assignment
bodies inspected above reference these same nearby literals. This is strong
static support for matching this event family, with runtime emission pending.

Current `components/viz/service/frame_sinks/video_capture/
frame_sink_video_capturer_impl.cc` supplies category `gpu.capture`:

| Event | Source line | Fields |
|---|---:|---|
| SetResolvedTarget | 259 | current/new FrameSinkId strings, or None |
| SetFormat | 318 | format |
| SetMinCapturePeriod | 351 | min_capture_period |
| SetMinSizeChangePeriod | 366 | min_size_change_period |
| SetResolutionConstraints | 389 | min_size, max_size |
| SetAutoThrottlingEnabled | 409 | autothrottling_enabled |
| Start | 449 | buffer_format_preference |
| FrameSinkVideoCapturerImpl::Start | 459/531 | Per-instance begin/end; pixel_format_, buffer_format_preference_ |
| FpsRateLimited / NoConsumer / NoRegionProperties | 836/850/860 | trigger |
| DroppingFrameWithUncontainedRegion | 879 | root_render_pass_size, render_pass_subrect_in_root_space |
| UsingResurrectedFrame | 911 | No additional fields |
| ReservingVideoFrame | 914 | root_render_pass_size, render_pass_subrect |
| PipelineLimited / NearlyPipelineLimited | 931/957 | trigger, utilization_pct |
| FrameInUse | 990/1551/1678 | frame_number, utilization_pct on frame-specific track |
| ContentRectDeterminedForCapture | 1011 | content_rect, source_size |
| Capture | 1015/1549/1566 | Begin: frame_number, trigger. End: result; success also timestamp_micros |

`timestamp_micros` is the **media frame timestamp relative to first captured
frame**, not UTC and not trace `ts`. FrameInUse/Capture track identities derive
from capture-begin/reference TimeTicks (source177-185). GPU capture frame_number
is its own counter; it is not the browser ui frame_number or image assignment
token. Do not equate those counters or assume an exact GPU-to-ui join. The ui
Process/Compress/JPEG flow does provide a direct browser-side frame chain.

Original contains `gpu.capture` at0xd238c05 and these exact distinctive event
cstrings: SetResolvedTarget0xd5848b4, SetMinCapturePeriod0xd5848d0,
SetResolutionConstraints0xd58495c, FrameSinkVideoCapturerImpl::Start0xd5849c7,
FpsRateLimited0xd584a11, NoConsumer0xd584a20, NoRegionProperties0xd584a2b,
DroppingFrameWithUncontainedRegion0xd584a3e, UsingResurrectedFrame0xd584a99,
ReservingVideoFrame0xd584aaf, PipelineLimited0xd584ad7,
NearlyPipelineLimited0xd584af7, FrameInUse0xd584b0d,
ContentRectDeterminedForCapture0xd584b18. Field literals source_size0xd584b38,
timestamp_micros0xd584b44, content_rect0xd48acd5, root_render_pass_size0xd584a61,
render_pass_subrect0xd584ac3 are also present. These static checks do not yet
verify every original event's exact runtime argument schema; preserve raw args
rather than assuming current C156 fields on original events.

Copy-from-surface has a weaker existing trace:

- `viz/CopyOutputRequest` begin on request-pointer track at
  `components/viz/common/frame_sinks/copy_output_request.cc:63`; its SendResult
  ends the track with `success` and `has_provided_task_runner` at134-137.
  Original exact event/field strings are0xd48a63b and0xd48a64d.
- `viz/SkiaOutputSurfaceImplOnGpu::CopyOutput` scoped event at
  display_embedder/skia_output_surface_impl_on_gpu.cc:1643; original event
  string0xd581a2d. It has no requested size/result dimensions in this event.
- `viz/DelegatedFrameHost::EmbedSurface` at delegated_frame_host.cc:285 exposes
  surface_id/is_first_surface_activation. This can help correlate view surface
  activity, but is not a thumbnail-capture ownership event.
- No TRACE_EVENT occurs in the Mac CopyFromSurface wrapper, delegated
  copy-result callback, ThumbnailTabHelper StoreThumbnail functions, or
  ThumbnailCaptureDriver/ThumbnailReadinessTracker. CopyOutputRequest events
  can belong to unrelated clients; their existence alone cannot attribute a
  thumbnail copy without additional association evidence.

The category `navigation` adds NavigationRequest lifetime/commit events. Current
NavigationRequest::WriteIntoTrace serializes navigation_id, frame-tree-node and
sometimes render-frame-host, URL, committed/error flags, net_error, state and
navigation_type (navigation_request.cc:10103 onward). Modern fields may be
structured proto fields, not identical JSON debug keys. Original contains
NavigationRequest::NavigationRequest, NavigationRequest::DidCommitNavigation,
StartToCommit, navigation_id and frame_tree_node_id strings. Use the raw emitted
schema and CDP Page navigation events/fixture URLs for correlation; do not
invent a common navigation ID field when an original event omits it.

### What remains invisible

Existing trace events do not expose first subscription/last unsubscribe,
observer identity, helper identity, thumbnail readiness, driver scheduling or
cooldown state, AssignSkBitmap's random token, ClearData, compression-token
rejection, decode-token rejection, per-registration task enqueue/cancel, or the
final public thumbnailReady callback. Source has no tracing macro on these
routes. Native literal absence for ThumbnailCaptureDriver/ReadinessTracker is
consistent with this gap but does not prove no indirectly named native event.

Consequently a complete, no-loss trace may locate the earliest visibly missing
stage (no capture lifetime; started but no mapped frame; frame compression
without accepted JPEG assignment; accepted JPEG without public callback). It
cannot by itself prove the uninstrumented state transition causing the gap.
A compression event without accepted-JPEG event is not automatically proof of
a stale token: an outstanding task at trace end or another failure remains
possible. Likewise a trace with no frames is not evidence the renderer was
incapable of generating one before tracing started.

### CDP collection and clocks

Current browser endpoint creates a trusted TracingHandler with root_session=null
(browser_devtools_agent_host.cc:258). Unlike page-target sessions, this avoids a
page process filter and can include browser plus GPU process events. Use the
actual browser WebSocket returned by /json/version and an independent browser
CDP session. Do not route tracing through this package's public DevTools wrapper,
which would change the recorded ArcCore calls. Tracing is browser-global: if
already active, fail this diagnostic rather than stopping another session.

Current public protocol `Tracing.pdl` supports start/end/getCategories,
recordClockSyncMarker and ReturnAsStream; IO.read/IO.close drains the returned
stream after Tracing.tracingComplete. Original binary contains Tracing.start,
Tracing.getCategories, Tracing.tracingComplete, unprefixed recordClockSyncMarker,
syncId, streamFormat, streamCompression and dataLossOccurred. Full `Tracing.end`
not appearing as a standalone cstring is not evidence it is unsupported; generated
dispatch need not keep full method names in that form. Live protocol checks and
responses, including unknown-method errors, remain required. No live checks were
performed here. Retain exact command/response/errors as diagnostic evidence.

A minimal compatible start request can use ONLY categories/options:

```
Tracing.start {
  categories: "-*,ui,gpu.capture,viz,navigation,__metadata",
  options: "record-until-full",
  transferMode: "ReturnAsStream",
  streamFormat: "json"
}
```

Alternatively use traceConfig includedCategories for those exact categories,
recordMode recordUntilFull and an explicit bounded buffer; do not combine it
with categories/options (handler rejects that combination). Avoid screenshots,
CPU sampling, systrace, memory dumps, wildcard categories and console output.
Check dataLossOccurred=false, record all pids/tids, counts and trace extent, and
preserve the complete raw trace. Do not silently fall back to an empty trace or
ReportEvents without recording the actual supported mode. ReturnAsStream and
JSON suitability still require runtime confirmation on original.

Trace JSON `ts` is monotonic microseconds, generally with pid/tid and duration or
begin/end phases. It is not UTC. `Tracing.recordClockSyncMarker` emits the
`__metadata/clock_sync` receiver event with matching sync_id; source
tracing_handler.cc:1146 and perfetto track_event_legacy.h:938-941. Bracket each
marker CDP round trip in the controlling driver with local monotonic and UTC
before/after measurements and save them as a sidecar. A unique sync ID provides
a trace timestamp bounded by that local interval; retain the uncertainty rather
than treating a midpoint as exact. Repeat at start/end to expose clock drift or
wall-clock adjustment. A callback sidecar needs the same local monotonic clock
(or an explicit conversion), not just Date.now UTC, to join precisely. Browser
trace clock snapshots may supply more precise clock mappings, but do not assume
the legacy JSON export exposes them. Record existing fixture step and callback
monotonic/UTC timestamps without changing callback/control semantics.

### Minimum matched diagnostic scope and irreducible startup limit

1. Keep recorder bytes, engine/profile snapshots and the existing launch-slice
   control sequence unchanged. Use separate fresh disposable profiles for original
   and ours, with the same locked-desktop condition. Tag outputs as diagnostics,
   never replacement canonical goldens. Snapshot engine/recorder identities.
2. Connect browser CDP at the existing first-available endpoint, start the same
   exact trace on both engines, and wait for the start response before the next
   existing scenario action. Record time brackets and failures. No new thumbnail
   request, tab activation, window focus, sleep, synthetic page or engine flag.
3. Record through the existing navigation/back/storage and first target-close
   boundary that brackets the observed callback, then clock marker/end/drain
   before Browser.close. Browser exit can destroy the stream and tracingComplete
   transport; collect before it. Preserve existing final app/helper termination
   checks separately. The tracing round trips inherently perturb timing, so
   compare traced pairs and retain the original untraced evidence.
4. Inspect capture starts, frames, flows, accepted JPEGs, GPU result/geometry,
   navigation boundaries and callback sidecars; distinguish observed paths from
   no-loss absence. Never infer fields or identities not emitted.

Without a startup trace flag or pre-start injection, browser CDP cannot guarantee
coverage BEFORE endpoint discovery. Initial service registration, observation,
first readiness, or even a capture start may already have happened. That is an
irreducible limit for reproducing a startup-only0-versus1 mismatch under this
constraint. Starting at first CDP readiness still captures subsequent existing
navigations and the observed pre-close callback window, but missing early
VideoCapture events are inconclusive. Creating an extra tab after tracing would
solve trace coverage for that tab while changing the tested stimulus and might
not exercise the same app-owned wrapper path; it is a separate experiment,
not the minimum matched original scenario. No trace implementation was added.

Inventory correction after exact source recheck: the current
`viz/DelegatedFrameHost::EmbedSurface` payload is `surface_id` and
`deadline_policy` (delegated_frame_host.cc:285-287), NOT
`is_first_surface_activation`. Use these exact source fields; the earlier table
sentence above should not be used as a schema claim. No producer-source claim
or proposed collection action changes.
