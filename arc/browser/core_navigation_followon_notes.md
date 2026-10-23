# Navigation follow-on audit (read-only, 2026-10-23)

This audit makes no production source or runtime claim. Production was frozen for
run6/shared integration while this evidence was collected. Addresses below are
unslid arm64 addresses in
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.

## Corrections to older stage notes

`/tmp/core-navigation-values-stage/NOTES.md` contains two overstatements:

1. **0x436a748 is successful frame-load completion; 0x436ade4 is frame-load
   failure.** The old notes called the latter finish-load. The native observer
   vtable at 0xda80780 has these adjacent entries at +0x100/+0x108. The former
   posts observer member +0x20 (0x436aa6c), whose ObjC bridge 0x44197a8 loads the
   selector at 0xd75ee54: `webContents:didFinishNavigation:`. The latter posts
   +0x30 (0x436b108), whose bridge 0x441999c loads 0xd75ee75:
   `webContents:didFailNavigation:`. Failure supplies ArcNavigation only, with
   no NSError argument. See `/tmp/core-nav-finish-callback-mapping.txt` and
   `/tmp/core-nav-finish-bridge-selectors.txt`.
2. **The command-path redirect flag compares post-Navigate source_contents,
   not the actual destination output.** Exact compared native Params field is
   +0x268; the separate destination output is +0x260. See below. HTTP server
   redirect does not establish this flag either.

Older evidence was left intact so the correction is explicit.

## Pending committed navigation FIFO

Original `DidFinishNavigation` producer 0x43695b0 obtains the shared navigation
value from the exact NavigationHandle-pointer map entry. Its normal branch posts
commit at 0x4369c74–0x4369c88, then appends that shared pair to the pending deque
at 0x43696d4–0x43696f0 and removes the handle-map entry at
0x43696f4–0x4369700. This is the same mutable value used by earlier start/commit
wrappers, not a newly reconstructed URL-only navigation. Its response state has
already been captured if a response-header object exists. The exceptional/error
branch 0x4369a90 onward dispatches via 0x436b480 and removes its map entry without
appending it. The same-document branch dispatches observer member +0x48 at
0x43698ec/0x4369954 and also bypasses the normal commit/FIFO append. Do not map
raw request-state numeric constants to current Chromium enum names without
version-specific proof.

Both frame-load producers have the following observable mechanics:

- 0x436a764 and 0x436ae00 load frame+0x178 and return when nonzero.
- They weak-lock the native ArcWebContents owner and return if unavailable.
- They test the callback GURL for `chrome-error` (literal 0xc6d7922, length12)
  and return without consuming the FIFO for that scheme.
- An empty FIFO logs `Unknown navigation finished. Ignoring` and returns.
- Otherwise they retain and pop the **oldest** shared pair, before observer
  delivery: success 0x436a8d8–0x436a9dc; failure
  0x436af74–0x436b078.
- They overwrite that pair's URL from the callback GURL: success
  0x436a9e0–0x436aa0c; failure 0x436b07c–0x436b0a8.
- They post successful finish (+0x20) or failure (+0x30), using the same generic
  asynchronous per-registration delivery path as other navigation callbacks.

There is **no matching** of the deque front to a NavigationHandle, navigation ID,
callback URL, or current primary frame pointer. There is also no generation
filter in these frame-load producers. A later start does not select a different
FIFO element.

Frame eligibility is a parent-null check, rather than the current implementation's
`frame == WebContents::GetPrimaryMainFrame()` comparison. The original native
getter 0x4150800 is exactly `ldr x0,[x0,#0x178]; ret`; nearby ancestor traversal
0x4150820–0x4150848 repeatedly follows this field. This supports GetParent/main
frame provenance, with a bounded limitation: no named original symbol or
constructor/type-identity proof for that RFH vtable was recovered in this pass.
Do not silently upgrade this to IsInPrimaryMainFrame, GetParentOrOuterDocument,
or an assumption about prerender/fenced/inner contents. Exact instructions are
in `/tmp/core-nav-frame-parent-getter.txt`. Current Chromium public API has
`RenderFrameHost::GetParent()` backed directly by `parent_`
(`render_frame_host_impl.cc:3266`); WCO DidFinishLoad and DidFailLoad signatures
are in `web_contents_observer.h:502–519`.

Evidence: `/tmp/core-nav-dispatch-audit.txt`,
`/tmp/core-nav-fifo-enqueue-crosstab-full.txt`,
`/tmp/core-nav-finish-callback-mapping.txt`.

## Current gaps and an isolated implementation boundary

Current `arc/api/arc_navigation.mm:415` has one `_documentNavigation`.
`didStartNavigationSnapshot:inPage:` clears it at line503; a successful commit
replaces it at line562; `didFinishDocumentLoad` copies and clears it at
lines569–572. Thus starting B after committed A can discard A's pending finish,
and B's commit can overwrite A before A's frame callback. The WC observer at
`arc/api/arc_web_contents.mm:362–365` discards callback GURL and accepts only the
current primary RFH. It has no DidFailLoad override. These differ from the proven
FIFO mechanics, but accepted goldens do not exercise competing pending loads.

A bounded follow-on can be confined to arc_navigation.mm, its private owned
header, and the WC observer adapter: store pending shared values in FIFO; append
on the established normal cross-document commit path; stop clearing pending
committed values on another start/failure; forward validated GURL on both frame
load callbacks; ignore chrome-error before pop; pop first, mutate its shared URL,
then use existing posted observer delivery for finish or failure. Never retain a
raw frame/handle. Preserve event-time registration capture and distinct delivery
wrappers. No new public selectors are needed; direct helpers/C++ entry points
suffice. Parent should settle the parent-null eligibility proof before widening
beyond the existing primary-frame filter.

**BFCache remains a specific adaptation gap.** Current lines564–565 immediately
call the no-URL finish helper after a restored commit. No BFCache-specific direct
finish dispatch was found in the bounded original DidFinishNavigation producer;
its examined normal branch appends to FIFO. Current Chromium's only additional
explicit RFH finish replay found here is prerender activation
(`render_frame_host_impl.cc:6869–6883`), not BFCache restoration. Do not blindly
call a FIFO-pop helper from the existing restored fast path: an older outstanding
entry could be consumed. Either prove original restoration delivery/native
configuration or keep this compatibility behavior separately scoped, document
its identity choice, and validate it with an explicit paired restoration probe.
The serialized golden back/forward callbacks alone do not prove BFCache usage.

This note does not propose changing the existing error-state classification,
missed-start replay, or BFCache behavior based on guessed flags.

## Command preallocation and cross-tab flag

Navigation value constructor 0x4345994 initializes shared mutable URL storage,
redirect=false, empty response headers and status0. Original `load` creates it at
0x4366f44 (browser-managed path) or 0x4367014 (standalone path), stores the pair in
controller optional+0x48/+0x50 with engaged byte+0x58, then invokes Chromium.
DidStartNavigation 0x43683f0 reuses that optional when present, otherwise creates a
new value at 0x4368608, then inserts it into the handle map. `load` returns that
preallocated optional at 0x4367210–0x4367238, independently of whether a native
NavigationHandle is returned synchronously.

Reload likewise allocates at 0x43681a8–0x43681b4, stores the optional at
0x4368218–0x4368228, invokes native Reload at 0x4368238–0x4368240 and returns the
value at 0x4368244–0x436826c. Current load lines603–604 requires a returned native
handle; reload lines633–635 requires synchronous DidStartNavigation observation.
This is an uncovered return/identity gap. A preallocation change needs exact
pending-command cancellation/replacement and reentrancy behavior as a separate
scope; merely creating a dummy value on the return path does not preserve shared
identity with queued callbacks.

The proven command true writer is 0x4367204. The browser-managed load sets
NavigateParams+0x268 (`sp+0x2e8`) to the requested native WC at 0x4366fd8, calls
Navigate 0x3870000 at 0x43671dc, then compares that field to requested WC at
0x43671e4–0x43671f0. Difference sets the pending value's redirect byte+0x18.
Original Navigate fills +0x268 from selected browser contents when initially NULL
at 0x387034c–0x38703a8, clears it in the profile-boundary branch at
0x38707d4–0x3870824, and writes its separate actual destination output at
+0x260 at 0x3871000. Those operations match current
`NavigateParams::source_contents` versus `navigated_or_inserted_contents`:
`chrome/browser/ui/navigator/browser_navigator_params.h:158–177`,
`browser_navigator.cc:710–715,849–854,1095`.

Consequently, a future adapter must reproduce the original **post-Navigate
source_contents comparison** if it uses this Chrome path; deriving the flag from
actual destination inequality or HTTP redirect is unsupported. Current content
controller-only commands do not expose that producer. No immediate native/shared
hook is justified solely by these goldens.

Evidence: `/tmp/core-nav-values-load.txt`,
`/tmp/core-nav-preallocation-constructor.txt`,
`/tmp/core-nav-reload-preallocate.txt`,
`/tmp/core-nav-fifo-enqueue-crosstab-full.txt`.

## Accepted golden reachability

Coverage audit excluded `.candidate` and `.recorder-v1` directories:21 accepted
canonical directories/25 record.jsonl files. None calls either load form,
goBack/goForward/goToBackForwardListItem:, reload/reloadWithoutCache, or any
ArcNavigation property getter. Therefore returned command value identity,
preallocation, mutable callback URL and redirect=true are not directly covered.

The recorded callbacks include42 provisional starts,41 commits,40 finishes,
1 provisional failure and5 in-page start/finish pairs. No WC receives a new
provisional start callback before its prior finish/failure. The HTTP redirect
in tabs-navigation has two commits for one start/finish (sequences7644,
7646/7890,7936), not two competing FIFO entries. Tabs navigation is initiated by
JS location/history/reload (`tests/scenarios/tabs-navigation.ts:10–18`), so it
exercises native observer delivery but not the public command methods above.
No currently accepted trace establishes a FIFO/callback-URL mismatch. This is a
source-proven uncovered compatibility follow-on, not a demonstrated run6 fix.

Reachability refreshed after new accepted scenarios:24 canonical scenarios and
29 rawrecordings, using scoreboard.ts's exact name/all-pass/accepted selection.
ArcWebContentsNavigationController still receives only addObserver:/removeObserver:
in those recordings; no public command selector or ArcNavigation getter appears.
The new source-only hash inventory is
logs/wp/core.accepted-navigation-coverage.20261023T1325Z.json. This does not expand
runtime parity coverage or remove the uncovered command/FIFO obligations.
