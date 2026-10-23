# HostView fullscreen observation and attachment — source audit

2026-10-23. Read-only native/current-source audit; only these /tmp notes and
bounded evidence traces were written. No code/proposal/production/native/shared
changes, builds, app execution, locks, or board activity. Attachment remains HOLD.

Evidence:
- `/tmp/core-host-fullscreen-native.txt`: bounded native disassembly, selector
  cells, ivar offsets, feature29 name/initializer records.
- `/tmp/core-host-fullscreen-imports.txt`: exact AppKit notification imports and
  weak-storage/retain/release imports from dyld_info.
- `/tmp/core-host-fullscreen-methods.txt`: native ObjC method identities.
- `/tmp/core-host-fullscreen-retain-sdk.txt`: local SDK retain/release contract.

Correction notice: an intermediate message/draft incorrectly called browser
+268 a retained weak-storage handle. That claim is SUPERSEDED and false. The
transport passes weak storage, but attachment resolves it to NSWindow BEFORE
calling the binder. The corrected conversion/public-setter proof is below.

## Notification identities and native effects

| Notification | Selector / IMP | Host-client slot | Impl effect |
| --- | --- | --- | --- |
| NSWindowWillEnterFullScreenNotification | onWindowWillEnterFullscreen: /473af50 | +30 | 43988a4 sets Impl270=true; RWH slot1d8 sets RWH641=true |
| NSWindowDidEnterFullScreenNotification | onWindowDidEnterFullscreen: /473afec | +38 | 4398900 clears Impl270; RWH slot1e0 clears RWH641 |
| NSWindowWillExitFullScreenNotification | onWindowWillExitFullscreen: /473b088 | +40 when attached | same set-true method |
| NSWindowDidExitFullScreenNotification | onWindowDidExitFullscreen: /473b158 | +48 when attached | same clear method |

Imported notification GOT cells are respectively d8255c8/d8255c0/d8255b8/
d8255b0. Selector cells e293828/e293768/e293830/e293770 resolve the exact
selectors above. Native host-client tableda81918 slots+30/+40 point43988a4,
+38/+48 point4398900. Those adjusted-this receivers are Impl+68, so receiver
+208 is actual Impl270. RWH setters4169390/41693a0 only set/clear its byte641;
the separate CALayer delivery path41683dc consumes that byte.

Every handler first requires its C++ weak host-client token to be live and its
target nonnull, then calls shouldHandleWindowNotifications. It revalidates the
token before native dispatch. Notification argument is retained/released for
the call, but the handler does NOT inspect notification.object. Object filtering
comes from registration, not an added current-window comparison in the handler.

Will/DidEnter dispatch +30/+38 after those gates. Will/DidExit additionally read
CURRENT [self window]. If nonnull, dispatch +40/+48. If nil, WillExit sets
didStartExitingFullscreen=true; DidExit clears it. They do not dispatch native
set/clear in those nil-window cases.

## Feature29 exact identity and sampling

Feature29 is kDisableCompositorRecycling. Consumer-verified name-table record
dff3858 contains pointerd46257e/length27/enum29; initializer recorddff3640 maps
enum29 to44d9170. It constructs false/type1 at44d9184/918c and applies JSON key
`disableCompositorRecycling` (d462376,length26). This is a DEFAULT, not proof of
the running value: public runtime overrides remain possible.

This uses the independently verified record schemas/consumers documented in
`arc/common/renderer_arc36_relay_evidence.md`: names24-byte records with enum+16,
initializers16-byte records with enum+0/function+8. No neighbor-name inference.
Current app-owned ArcRuntimeFeatureEnabled(29) exists inarc/api/arc_app_flags.mm.

viewDidMoveToWindow473a9d8 samples feature29 ONCE at method start and keeps the
value for its branch. shouldHandleWindowNotifications473aefc samples it on EACH
notification: false returns true; true returns whether current self.window is
nonnull. Do not force false, cache the value for the observer lifetime, or assume
that a runtime change rebuilds existing registrations: no such relay is shown.

## Observation lifetime, default false path and override true path

On EVERY viewDidMoveToWindow with a nonnull current window:
1. defaultCenter removeObserver:self (all prior registrations).
2. Add all FOUR notifications above with object:[self window], using a retained
   local current-window result for each add. No object:nil subscription.
3. Under false only, perform the deferred-exit repair below if its gates pass.
4. Store current [self window] into weak lastWindow.
5. Continue into the existing host-client outer attachment callback.

The class persists no strong NSWindow for these registrations. lastWindow is
an ObjC WEAK ivar atoffset48: getter473c3d0 calls objc_loadWeakRetained;
setter473c3f0 calls objc_storeWeak; .cxx_destruct473c450 calls objc_destroyWeak.
The BOOL didStartExitingFullscreen is byte9. Host-client C++ weak token is at56.
The class uses selector registration, not a retained block-observer token.
This describes explicit native storage; it does not infer NSNotificationCenter
internals from this binary.

On a move to NIL window, weak-load lastWindow:
- If lastWindow is nil/expired: removeObserver:self (all). No explicit flag
  reset/storeWeak(nil) is executed on this branch.
- If lastWindow is live AND feature29=false: remove only WillEnter and DidEnter,
  specifying object:lastWindow. KEEP WillExit/DidExit for that old window and
  KEEP weak lastWindow. This allows detached exit notifications to update the
  deferred-exit BOOL because shouldHandle returns true under false.
- If lastWindow is live AND feature29=true: removeObserver:self (all), clear
  didStartExitingFullscreen, and storeWeak(lastWindow,nil). Detached notification
  handlers also independently return because shouldHandle requires window.

Nil moves dispatch the existing client+18 unparented callback, not the +10
state-zero attachment callback. dealloc473a5fc removes ALL observations;
.cxx_destruct releases the weak host-client storage and destroys weak lastWindow.

## Deferred-exit repair before attachment

Only when feature29=false, didStartExitingFullscreen=true, and weak client is
live/nonnull does the nonnull-window move enter473aba0..ac04:
- Clear didStartExitingFullscreen BEFORE comparing windows.
- Weak-load lastWindow and compare with CURRENT [self window].
- If different, do not dispatch fullscreen repair.
- If SAME, synchronously dispatch client+40 at473ae44 (sets Impl270/RWH641).
- Then re-read current window.styleMask; if FullScreen bit14 is CLEAR, dispatch
  client+48 at473ae94 (clears Impl270/RWH641). If bit14 is SET, leave it set.
- Update weak lastWindow and proceed to ordinary outer attachment.

If the weak client was absent/expired, this repair branch is skipped, including
its BOOL clear. These are exact source gates, not an unconditional reset.

The later native attachment method43981ac independently reads Impl270. False
skips its cleanup branch; true queries incoming window.styleMask bit14. An
already-fullscreen window skips cleanup; otherwise clear Impl270 and RWH641
before continuing. This ordering can set/clear flags multiple times on one
reattachment and cannot be reconstructed solely from current styleMask.

## Corrected browser binding / PUBLIC setter proof

Host.lastWindow is weak; browser+268 is a DIFFERENT lifetime. Full call chain:
1. Sender473ad40 allocates8-byte storage and objc_storeWeak(window) at473ad54.
2. Attachment4398328 loads that storage;439832c ->84edd34 ->c4a5b14 ->GOTd824118
   resolves `objc_loadWeakRetained`, returning ACTUAL NSWindow. It stores the
   NSWindow in stack+8 at4398334, retains it4398338, releases the temporary ObjC
   return4398340, and passes &stackNSWindow to binder43c8b18 at439834c.
3. Binder reads *argument and stores that SAME NSWindow pointer in+268 at
   43c8b4c/50, then retains it43c8b54. It is not storing the sender's weak cell.

Independent PUBLIC path: ArcBrowser setWindow:43e14d4 receives the ObjC window
argument inx2, keeps it inx19, and passes its address to42fa9fc at43e151c. That
function resolves the native window adapter, copies *argument tostack+8, and
calls the SAME binder43c8b18 at42faa5c. Golden calls identify the incoming object
as NSKVONotifying_Arc.BrowserWindow, not a transport-storage object. Thus the
public setter shares the original binder's nonnull-existing-window early return.
CreateBrowser:forWindow: initialization has NOT been traced here; do not assume
that current constructor/adapter setup is interchangeable with this setter.

Retain classification is based on complete import chains, not a guessed name:
`c2265a0 ->c4a7d40 ->GOTd826120` binds libSystem/_os_retain;
`8f1b544 ->c4a7d34 ->GOTd826118` binds libSystem/_os_release. Local SDK
os/object.h281..325 specifies that os_retain/release increment/decrement the
reference count and on the modern ObjC runtime exactly equal retain/release
messages. Combined with the actual NSWindow argument above, +268 holds a
RETAINED WINDOW reference, not weak-window storage. This is static ABI/source
classification; no live retain-count probe was run.

Native getter43cc638 (vtablecellda86c80) reads the retained window at+268,
creates fresh8-byte weak storage, and objc_storeWeak's that WINDOW into it at
43cc674. It returns this weak handle inx8's output. Thus an outward-facing weak
NativeWindow return does NOT imply that the adapter's internal+268 is weak.
43c8c40 is NOT this getter; it forwards the retained field to47343e0, and its
precise frame-operation name is not needed/proven here.

Release evidence: teardown body43c82c4 reaches43c8598..59c, loading+268 and
calling the release chain. Method43c9580 releases it43c9598, writeszero43c959c,
then releases/clears the separate+270 registration. The clearing callback is now independently proved below. Consequently
do NOT describe binding as permanent for the entire object lifetime, introduce
an unconditional ever-bound BOOL, or ignore explicit clear/reset. Nil input
also leaves+268zero when initially empty; existing nonnull+268 still wins the
early return. Native setter behavior is conditionally once-set until clearing.

Current ArcBrowserWindow.native_window_ is gfx::NativeWindow, which inherits
WeakNSWindow (native_ui_types.h162); current SetNativeWindow simply replaces it
(arc_browser_window.mm376). This is an independently source-backed ownership/
setter gap, but this audit proposes NO change. Parent can stage a separately
reviewed adapter-held-window design after resolving exact creation and clearing
paths. A blanket change to every current SetNativeWindow call is not justified.

## Implementability boundary — no new generic native task justified

Owned code can REPRESENT this fullscreen event/weak-window protocol and logical
Impl270 bookkeeping on the existing UI stack, using real AppKit notifications
and the existing genuine outer-view transport. No source shows a generic task
before these native flag updates or attachment operations. The native state
setter queues app observer delivery afterward. Existing transport contract
requires queueing APP/KVO work; it does not require deferring non-app native
bookkeeping. The held helper's extra non-nestable native-operation task remains
incorrect for exact original timing.

This is NOT authority to activate. Remaining boundaries:
- Complete logical state290 writers/identity must survive native WC replacement
  and include PM lifecycle, direct freeze, discard replacement, and attachment.
- Impl270 and RWH641 are distinct lifetimes. Current RWH CALayer delivery has
  no recovered suppression counterpart; owned flag bookkeeping alone does not
  implement that native display effect.
- Global candidate-browser selection/filter/order and weak owner-browser binding
  must match the source. A registry of only wrapped browsers is not proven to
  equal the original native collection or its additional candidate filter.
- Adapter-held retained-window lifetime and conditional once-binding/clear must
  match the PUBLIC setter and the native creation/clearing paths. General
  new-window binding/transfer and state-one reload remain held.
- Current transport is Chromium outer WebContentsViewCocoa AFTER its existing
  visibility update; original sender is custom WebContentsHostView and registers/
  repairs notifications before native attachment. Reentrant visibility ordering
  and custom-host vs current-outer event correspondence are not runtime-proven.
- Transport installation only at in-process CreateView leaves the previously
  documented remote/migration-back gap. No approximate queue compensates for it.

Thus the protocol is source-representable without adding a generic task, but
the complete attachment/model/display implementation is still HOLD. No source
gate was relaxed and no runtime parity claim is made.


## Resolved exact window clear phase

Fresh review proves NativeWindowTrackerObject init4735278 registers
NSWindowWillCloseNotification with object=its separately retained window. The
notification callback47355ac removes registrations, then checks native weak
validity before the inlined clear4735710..5750: release/zero adapter+268,
move/zero/release tracker registration+270, then release/zero tracker window+8
at47356d4..dc. Primary43c9580 and secondary43c95e4 are equivalent clear methods;
the notification callback inlines that operation. This is synchronous WillClose
clearing, preceding BrowserDidClose; nil public setWindow does not clear an
existing binding. The tracker retains its own window across adapter-window
release, so callback/reentrant lifetime must be preserved.

Evidence and independent review: logs/wp/core.host-window-clear-audit/. No
production window binding change or runtime parity claim follows from this
source-only finding.


## Creation and destruction scope resolved

Bounded public43e7048/native42b32d0/browser42f8300/factory3510610/adapter35116fc
trace proves forWindow is identifier string metadata, adapter window268/tracker270
start zero, and public delegate assignment follows factory and wrapper init. No
creation-time NSWindow lookup/binder appears in this bounded chain; keep existing
empty-adapter creation unchanged. This is not a recursive audit of all Chrome
feature constructors.

Native adapter destruction43c82c4 invalidates callback receiver/token43c8434..8464
before releasing tracker270 at43c8578..8594, then releasing actual window268 at
43c8598..859c. This differs from live WillClose clearing and must be preserved
independently. Raw evidence and independent source note are archived under
logs/wp/core.browser-window-creation-audit/.


Nil-input correction: the earlier excerpt ended at43c8c2c; the next instruction
43c8c30 branches BACK to43c8b5c with x19=0 and continues tracker creation.
NativeWindowTrackerObject init4735278 has no nil-window guard, so object:nil
subscriptions are global within the app. An initial nil setter therefore leaves
actual window268 zero but DOES install/replace its tracker. The corrected owned
implementation creates that tracker for nil too; no tracker-identity guard is
added. Earlier claims that initial nil creates no tracker are superseded.
Exact raw proof: logs/wp/core.window-binding/native-nil-binding.txt.
