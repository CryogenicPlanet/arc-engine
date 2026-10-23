# Webextras discard attachment audit (HELD, no hook activated)

This is a proposed cross-package contract for the lead/core. There is no production
rebind implementation or shared declaration in this package. Do not enable a
partial discard handoff based on this note.

## Proposed declaration and ordering

**Superseded as a complete contract by the recorded follow-up below.** A single
callback-free cutover cannot preserve the measured synchronous PiP destruction.
The lead/core must choose separate retirement and replacement-attachment phases
before any hook is activated; the signature below is historical proposal input.

```objc
namespace arc {
void WebExtrasWebContentsRebound(
    ArcWebContents* owner,
    content::WebContents* old_contents,
    content::WebContents* new_contents,
    NSArray* existing_services);
}
```

The lead chooses the shared declaration in `arc/api/arc_internal.h`. Core passes
its existing `_services.allValues` snapshot, held strongly with `owner`, rather
than invoking the public lazy `devTools`, `mediaSession` or `thumbnailsGenerator`
getters. A no-op applies for identical native pointers. The call is synchronous
on UI, with both native objects alive. Canonical mapping and the wrapper's native
pointer must already identify the replacement; its tab/helper associations must
exist. No callback/event-loop gap may precede removal of old attachment mappings.
Perform attachment changes before replacement notifications/state publication.
The replacement's own native SessionID is retained; service shell/delegate
identity is preserved.

The aggregator can dispatch to private package subhooks in the existing owned
translation units. The PDF observer is native userdata, rather than an object in
`_services`; its attachment must also be handled directly. No service should be
created merely because another service exists.

## Attachment changes and unresolved behavior

### Media

`arc_webextras_media.mm` caches its Mojo/WebContents observer in the existing
service. Replace that observer/receiver with an attachment to the new native
MediaSession; action/getter methods already resolve native contents through the
weak owner. Preserve cached state comparison values pending genuine new native
updates. `MediaSession::AddObserver` sends initial native info/position/actions;
no artificial app callback or state replay should be inserted.

An attached old observer's `WebContentsDestroyed` calls `nativePiPDestroyed`;
resetting the observer before destruction detaches it without that callback.
Native overlay destruction independently calls `nativePiPDestroyed` and clears
the owner's shared `videoView` association. Thus an old overlay must not clear a
replacement overlay's association. Active-PiP discard eligibility and whether a
successful ordinary discard emits a PiP-destroyed callback still need native
recorded evidence. Do not guess/reset `_pipDestroyed` or suppress/replay callbacks.

### Thumbnails

Replace the old image/subscription with the replacement tab helper's image while
preserving the existing ordered weak registration tokens. Already queued image
deliveries are guarded by registrations, not by a native attachment pointer;
removal/re-add cancellation semantics must remain intact. Their precise behavior
across discard is not recorded yet.

C156 `ThumbnailTabHelper::AboutToBeDiscarded` transfers compressed data through
`DiscardedTabThumbnailData`; the new helper consumes it. Its constructor explicitly
receives the incoming contents before the tab swaps. Do not move the old helper
or invent/copy pixels outside that existing native transfer. Wait until the new
TabInterface association and helper are available before subscribing.

The old image can safely outlive its helper: `ThumbnailImage::Delegate` destruction
clears the image's delegate pointer and final unsubscribe checks that pointer.
Clear service members before destroying the old subscription, as normal teardown
already does, to preserve reentrant subscription setup.

### DevTools

Move/recreate only the inspected-contents userdata binding. Do not silently
reattach raw hosts, reuse connection IDs against a different target, replay pending
commands, or retarget the frontend observer. C156's page live-state decorator
tracks all attached page agent hosts; discard eligibility protects those pages
unless `kAllowDevtoolsConnectedDiscard` is enabled. The native successful-discard
probe detached its fixture debugger; its attached-debugger case returned NO.
This supports ordinary no-live-client eligibility, not a universal claim that
all forced replacements are impossible. A nonstandard replacement with a live
frontend/protocol client needs an explicit contract.

### PDF

Recreate the native attachment; `WebContentsUserData` holds a private **const**
pointer to its original contents. Moving the old userdata and merely calling
`Observe(new_contents)` leaves that pointer wrong.

Recreation currently cancels weak-observer queued readiness delivery. Original
PDF notifications capture a persistent native controller peer and guard the native
UI wrapper/registration lifetime, with no recovered navigation-generation check.
Whether accepted receipts survive underlying-content discard requires separating
attachment lifetime from shell/delivery lifetime or native evidence establishing
cancellation. Do not silently discard or retarget queued receipts. Retained public
PDF controller wrappers currently resolve through the weak shell owner; exact
cross-discard helper/result semantics remain unproven. See
`webextras_native_notes.md` for bounded producer/capture/guard evidence.

### Boost

No persistent native attachment is currently wired. Preserve shell/delegate
identity. Do not replay old WebUI requests into a replacement document.

## Native evidence limit and required next measurement

`results/sessions/discard-original-run2` contains app logs and probe JSON, **no
boundary recorder transcript**. Its driver injected only the discard helper,
without `ARC_RECORD_PATH` or ArcEngineRecorder. It establishes the same retained
ArcWebContents shell, native SessionID `402077452 -> 402077453` observable before
finalization, and one asynchronous main-thread finalization callback. It cannot
establish absence/count/order of media, PiP, thumbnail, PDF or DevTools callbacks.

A future owner-controlled recorded discard probe should preserve existing service
identities/delegates and measure callbacks/queued work around a successful
replacement. Until then, hold activation of behavior-dependent service migration.
No app launch, shared source edit, hook activation or runtime parity claim was
made to produce this audit.

## Recorded original follow-up: discard-recorded-original-run1

This newer measurement closes only the ordinary, inactive-page media/PiP gap
above. It does not change the historical limitation of `discard-original-run2`.
Evidence is in `results/sessions/discard-recorded-original-run1/record.jsonl`
(7,632 rows), `recorder-gates.json`, `selected-boundary-window.json`,
`observations.json`, and `result.json`. The recorder gate passes. The selected
shell is `<ArcWebContents#137>`, its app delegate is
`<ADK.WebContentChromium#145>`, and its media service is
`<ArcWebContentsMediaSession#147>`. This is an inert HTML fixture, with its own
page debugger detached before successful discard; it does not exercise active
PiP/audio, thumbnails, PDF readiness, or live DevTools protocol clients.

### Measured order and identity

- Initial `mediaSession` call/return 3669/3670 returns service #147; its
  `setDelegate:` call/return 3671/3672 installs delegate #145. There is no second
  `setDelegate:` on this service in the full transcript.
- `sessionId.identifier` return 6233 is `2138339481`. `discard` enters at 6236,
  main thread, depth 1. The genuine engine-to-app
  `webContentsPiPControllerDestroyed:` enters at 6237, main thread, depth 2,
  with shell #137, and returns at 6276. This notification therefore occurs
  synchronously inside the public discard call even for this inactive fixture.
- The first observed use of the replacement numeric SessionID is the engine's
  `browser:extensionGroupIdForTabWithSessionId:` call 6277, argument
  `2138339482`, after the PiP callback returns. `discard` returns at 6279.
  The selected shell's `sessionId.identifier` returns the same new ID at 6293.
  No selected-shell SessionID/native-state getter occurs inside 6237..6276;
  these rows do **not** prove which native contents the shell exposes during
  the PiP callback, or exactly when the replacement SessionID was allocated.
- `mediaSession` call/return 6360/6361 returns the **same service #147**.
  Its next public operation is `exitPictureInPicture:error:` at 6362/6363,
  arguments `[false, "<ptr>"]`, returning false. It is not a media-content-types
  query, and the pointer encoding does not reveal the NSError contents.
- Forwarded `webContents:finalizeDiscardRequest:` enters/returns at 6384/6385,
  main thread, depth 1, arguments shell #137 and true. Its caller label is the
  discard probe because that probe forwards the actual app delegate callback.
  The probe observations establish one asynchronous finalization, the same
  retained shell pointer, and replacement ID `2138339482` before forwarding.
- The selected window ends at 6385. It contains only the PiP-destroyed callback
  and finalize callback among these selected media/discard notifications.
  **Full raw rows 6402/6403 additionally contain genuine engine-to-app
  `webContents:didChangeAvailableMediaActions:`**, main thread, depth 1,
  arguments shell #137 and `<NSMutableIndexSet#232>`, after finalization returns.
  Its contents are not serialized, so this is not proof of an empty action set.
  The earlier initial action notification is 4252/4253. No selected media
  session-state, position, camera, or microphone callback appears in the full
  recorded run. This bounded census is evidence about the initialized media
  delegate, not absence evidence for unobserved services.

The 6402 action notification precedes the next selected provisional-navigation
callback at 6556. The recorder has no wall-clock timestamps for joining rows to
the driver's CDP reload request, so its exact placement relative to that request
and its native producer are not established. Do not label it a proven
`AddObserver` initial delivery or suppress it on the basis of the shorter slice.
DevTools/Boost getters are observed before discard, but no post-discard getter
proves their service identities in this recording. No thumbnail/PDF getter is
observed for this shell.

### Attachment implications and remaining handoff contract

The current media observer detaches with `Observe(nullptr)` before calling
`nativePiPDestroyed` from `WebContentsDestroyed`, because the app callback may
clear the delegate and delete that observer. Resetting `_observer` at cutover
instead would detach it without emitting the measured 6237 event. Leaving it
cached after old destruction is also insufficient: the old Mojo receiver stays
with that observer, and non-null `_observer` prevents `setDelegate:` from
attaching to the replacement. A successful-discard handoff must preserve the
same service/delegate, retire the old native attachment with reentry-safe
ownership, preserve the synchronous destruction notification, and attach a new
receiver to the replacement; it cannot be a callback-free reset alone.

C156 `MediaSessionImpl::AddObserver` sends genuine initial info, metadata,
images, position and actions over Mojo before registering the observer. Allow
those real native events rather than adding manual action/state notifications.
The trace above establishes an after-finalize action event but does not prove
the internal registration/dispatch point. Cached comparison values, PiP
destruction latch behavior on a second discard without an intervening PiP
creation, old-overlay destruction ordering, and which attachment public calls
inside the destruction callback must target still require an explicit lead/core
contract or further native evidence. In particular an old overlay must not
clear a newly installed shell `videoView` association.

The next concrete core contract should identify an old-attachment retirement
phase (including whether shell lookup still resolves old or already resolves
new during callbacks), and a new-attachment phase after the new tab/helper
associations are installed, before successful finalization. Both phases retain
the shell and existing-service snapshot strongly, run synchronously on UI, and
perform no lazy public service getter calls. Reentry must be allowed to clear
delegates/drop services; no stale observer/service member may be used after an
app callback. The exact phase locations are not proved by recorder ordering
alone, so this note still does not authorize hook activation.

For C156 source context only, `TabLifecycleUnit::FinishDiscard` in
`chrome/browser/resource_coordinator/tab_lifecycle_unit.cc` creates the incoming
contents, calls old `AboutToBeDiscarded(new)`, copies history, swaps through
`DiscardWebContents`, and only then destroys the returned old contents before
`NotifyWasDiscarded` and tab-state publication. That supplies a possible
old/new-live handoff interval, not proof that original Arc uses the same chain.
`discard_eligibility_policy.cc` protects audible/recently audible pages, active
PiP, PDF MIME contents, and attached DevTools unless
`kAllowDevtoolsConnectedDiscard` is enabled. These guards narrow ordinary C156
coverage; they do not settle forced replacement behavior or queued thumbnail/PDF
delivery. The earlier PDF userdata/queued-receipt and thumbnail registration
limitations remain in force.

Only this findings appendix was changed. No app, build, lock, shared/production
source, rebind hook, or runtime parity claim was introduced.

The saved driver phase timestamps are discard `2026-10-23T12:34:09.182Z`,
history-after-discard `12:34:10.136Z`, and reload-discarded-fixture
`12:34:10.243Z`. `app.log` has only startup messages through `12:34:07` and no
discard/reload marker. The raw schema has no timestamp or CDP-request marker;
there is no recorded public reload/navigation entry between discard 6236 and
action notification 6402. The first later selected engine `setUrl:` is 6474/6475,
followed by status-text callback 6548, provisional navigation 6556, and render
view ready 6558. Those generic UI/navigation callbacks are present and must not
be described as an absence of all service callbacks. The restricted absence
claim above concerns only the selected media state/position/camera/microphone
selectors; PDF/thumbnail delivery is unmeasured because those services were not
observed for this shell.

## Static retirement binding follow-up: sessions PiP adjunct

The previous recording-only uncertainty about the native association at PiP
callback entry is narrowed by
`results/sessions/native-evidence/discard-impl/arccore-sessions-discard-impl-pip-evidence.md`
and its paired `arccore-sessions-discard-impl-pip-disassembly.txt`. Both files'
SHA256 values match that directory's `manifest.json` on this audit. This is
bounded original-native dataflow evidence, not an additional runtime run or a
complete getter/reentry audit.

Detach4372b6c takes the old observation engagement from Impl+c0 into x24;
4372cc0..4372cd0 passes `(x24 != nil)` and new-observation nil to media helper
435f9c8 through Impl+2c8. With the previous native observation engaged/valid,
435fb30..435fb78 looks up old-WC media userdata key c9d6f08 and synchronously
calls its media peer vtable+8. The media peer constructor435f34c..435f360
installs da80620; slot da80628 is43639e8. That producer clears its PiP-controller
peer at4363a5c, captures the outer shared WebContents peer through42c8400,
and calls4355b30 at4363a84 with virtual member slot30 and literal
`arc::WebContents::MediaSession::Delegate::PiPControllerDestroyed`.
4355cb0..4355cc4 locks/dispatches the delegate synchronously. Media delegate
bridge da88368+30=441b718 weak-loads the app delegate/owner and directly sends
`webContentsPiPControllerDestroyed:` at441b750. There is no queue between this
retirement route and the app callback. Inside43639e8 the cleared PiP-controller
peer value is not tested for nonnil before selecting the notification; the
outer shared-peer/observation/delegate conditions still apply.

Crucially, the **same native Impl+130 still contains oldWC at callback entry**
on this local route, absent earlier external/reentrant mutation. Removal of the
old Arc shared-Impl userdata key e4a2f38 is later at4372ee0; clearing
Impl+128/+130 is later at4373054..4373058. Replacement attachment stores newWC
to Impl+130 at437039c only after detach returns through43980b0..43980c4.
This agrees with recorded synchronous PiP callback6237..6276 and supplies native
binding dataflow that those recorder rows alone did not show. The required
retirement callback therefore precedes canonical native-pointer replacement;
it must not be emitted from a callback-free new-pointer cutover.

This is **not** a claim that the old service graph is fully live: other services
have already begun detachment before media, and the PiP-controller peer itself
has been cleared before the callback. Do not promise that every getter/helper
works during this callback, or that arbitrary delegate reentry leaves the
remaining detach sequence valid. Existing media `nativePiPDestroyed` clears its
cached PiP wrapper before notifying, which agrees with this local peer clear;
its service-wide latch still needs an explicit per-attachment lifetime decision
when implementing repeated retirements. Preserve the existing detach-before-
callback observer safety and strongly held shell/service snapshot. The new
native attachment must still wait for replacement tab/helper associations.
Thumbnail/PDF accepted queued-delivery semantics and live DevTools replacement
remain unresolved as above. No hook is activated by this stronger entry-binding
proof.

