# URL-only browser tab notification follow-on — STAGED ONLY

Patch core_url_only_notification.patch SHA256 4b2284a11e1065d93832c7fac07d5832c682e96a7dbf649a45b3b13ff107124d. No production/repo/shared/native edits, app/build/lock/board/credentials actions, wrapper/root allocation, registry, callback fixture or runtime. Parent/app2r source freeze remains unchanged. Lead/fresh review and full-route composition remain prerequisites to any promotion.

## Exact base and demonstrated gap

prepare.py freezes current production47534304 WC and browser/window source inputs, then strictly applies native2f423 + generic-after-progressac1b + displaye87 + observer-after-progressb222 in that order. These exact archives/hash assertions are in manifest.json; original proposals untouched. The four-part target's NativeNavigationStateChanged URL branch posts owner.url then navigationID ONLY, with no Browser notification. Its generic forwarding/observer/display files contain no additional TabsChanged producer. Browser current OnTabChangedAt is the sole updated-event forwarding path.

Chrome BrowserUiController ScheduleUIUpdate strips URL after toolbar handling127–139, returns for no remaining flags151–152, and current ArcBrowserWindow UpdateToolbar is a no-op(h113). Hence exact URL-only receipts have no guaranteed tab-change event from that path. This proposal adds a PACKAGE notification, not an original engine->app delegate callback or a fake TabStripModel event. Existing recorder does not directly hook this custom NSNotification, so no golden notification count/parity acceptance is claimed.

## Receipt phase and duplicates

Only flags==content::INVALIDATE_TYPE_URL adds work. Existing URL/navigationID property task is queued unchanged FIRST. Then QueueCoreBrowserURLOnlyNotification captures existing identities and queues a separate ordinary UI notification task. FIFO on the same UI task runner places it after the property task. It does NOT gate or cancel the original property task when native contents/browser later dies; those source-backed weak-owner property semantics are untouched.

One new updated notification is eligible PER genuine URL-only native receipt. Mixed URL|TITLE/LOAD/TAB/AUDIO or other masks remain untouched and continue existing OnTabChangedAt forwarding. No synthesized flags, KVO watcher, fake TabStrip callback, global deduplication or count normalization. Distinct actual events may overlap: an independently received title/loading/tab update can still produce its ordinary notification before/after this URL-only notification. No ordering between distinct native producers is invented. If exact ownership disappears before new notification delivery, it is suppressed rather than sent to an old or newly transferred browser. The follow-on does not complete unrelated mixed/unknown-bit notification coverage.

## Existing-only identity and lifetime checks

- New private core_browser_notifications.h declares only TWO package-local functions (under ObjC) and is added to owned core.gni header list. No public metadata/API/shared header declaration or dependency is added.
- ExistingArcBrowserHolder is factored beside its CURRENT key/holder in arc_browser.mm and reused by +wrapperForBrowser. It performs only the SAME existing static_cast<ArcBrowserWindow*>(browser->GetWindow())->GetUserData(kArcBrowserKey) lookup; it allocates nothing. This relies on the SAME hosted native-window ownership contract already required by +wrapperForBrowser; no new claim that arbitrary Chrome/foreign windows are Arc adapters, no cast of NSWindow, and no new registry. Future callers must preserve that contract.
- Receipt verifies existing canonical WC userdata and its matching native binding via a PRIVATE objc_direct bool predicate, no public webContents/browser getter or creating wrapper lookup. TabInterface::MaybeGetFromContents finds the current actual native browser; absent/detached/delete-scheduled native browser or absent canonical holder returns without posting.
- Task captures only base weak nativeWC/browser and ObjC weak canonical WC/browser. Receipt has precise strong browser only until enqueue returns, with no strong facade captured by the task. No global native/ObjC strong ownership or original-lifetime padding.
- AFTER the preexisting property setter/KVO callouts, new task checks both native WeakPtrs, !IsDeleteScheduled, ObjC weak loads into precise locals, CURRENT TabInterface owner==capturednativeBrowser, CURRENT existing holder's facade==capturedcanonicalbrowser, and CURRENT existing canonicalWC mapping/private native binding. No public getter recursion or app-boundary calls solely for validation.
- NotifyTabChange(...,@"updated") is terminal after all checks. Notification observers may synchronously transfer/destroy browser/tab/WC/holder; NO borrowed pointer dereference or further source mutation follows the callout. Local weak/native token and precise ObjC references merely release on return. Missing/stale identity suppresses ONLY this optional package notification, not mandatory accounting/callbacks elsewhere.

## Source scope and integration preserved

Only arc_browser.mm (existing holder factor + helper), arc_web_contents.mm (private predicate + exact URL-only queue call + noncreating canonical helper), NEW private header, and ownedcore.gni header list change. All current insert/remove/replace/move/activate/updated and will-close events remain byte-unchanged; sessions hooks/DidCreate gates/renderer/services/quit suppression unchanged. No logical root, eager/native ownership, public close, main bundle installation or source-token behavior added. Native patch itself is unchanged. No browser/wrapper initializer hook or new global vector exists.

This patch depends on the pending four-part target, NOT current production alone. Only helper/header behavior is new; a direct standalone apply is intentionally unsupported. Parent must compose it against its chosen complete held accounting/logical/etc route and obtain review; this task did not activate or claim that larger route.

## Checks

Strict four prerequisite apply checks/applications pass with --whitespace=error. Follow-on strict apply to a SEPARATE copy of exact four-part base reconstructs EVERY staged9 path/hash exactly. Only four owned paths differ. All relevant current production inputs are byte-unchanged after compilers, and prerequisite patch hashes remain fixed.

Actual Siso WC and Browser compile commands were queried read-only. compile.py substitutes staged inputs + stage include overlay, strips object/dependency outputs/-c, and uses -fsyntax-only with exact SDK/ARC/C++23/plugins/warnings. Both actual clang syntax checks exit0 with EMPTY diagnostics (compiler-checks.json); command/query/original/removed flags and empty logs retained. No object, GN regeneration, linked build, test fixture, native app or runtime invoked. Normal headers outside9 staged paths resolve from the current source checkout, so this is not a hermetic full-engine build. GN link/runtime/actual custom-notification observations remain unverified; fresh source review required.
