# Native volume bridge proposal

Proposal: `arc/browser/webextras_volume_bridge.patch`. The lead owns application
to Chromium and the maintained patch set. This proposal does not change the
package's current `setVolume:error:` implementation, Chromium sources, shared
tests, maintained patches, applications, or existing notes.

## Three-class bridge

`MediaSessionController` gains a public inline method:

```cpp
void SetVolumeMultiplier(double value) {
  OnSetVolumeMultiplier(player_id_, value);
}
```

This uses the controller's private synthetic session player ID. It neither
exposes that ID nor substitutes `MediaPlayerId::player_id` or a testing
accessor. The existing `OnSetVolumeMultiplier` validates the synthetic ID and
returns without sending when its MediaPlayer Mojo remote is unavailable.

`MediaSessionControllersManager` gains a public declaration and body:

```cpp
void MediaSessionControllersManager::SetVolumeMultiplier(double value) {
  if (!IsMediaSessionEnabled()) {
    return;
  }
  for (auto& entry : controllers_map_) {
    entry.second->SetVolumeMultiplier(value);
  }
}
```

The existing helper gates on `kMediaSessionService || kInternalMediaSession`.
All tracked controllers receive the original value, including paused players.
There is no active-session requirement, clamp, controller creation, persistent
volume state, or synthetic success/failure result. A disabled feature gate,
empty map, or unavailable remote is a no-op.

`MediaWebContentsObserver` gains a public declaration and body:

```cpp
void MediaWebContentsObserver::SetVolumeMultiplier(double value) {
  CHECK_CURRENTLY_ON(BrowserThread::UI);
  if (session_controllers_manager_) {
    session_controllers_manager_->SetVolumeMultiplier(value);
  }
}
```

The nullable manager matters because `WebContentsDestroyed()` resets it. The
bridge retains no new raw pointers and introduces no posted callback or
asynchronous lifetime. Controller dispatch uses existing asynchronous Mojo
sends; it does not run nested callbacks or wait while traversing the map.

## Package wiring and exports

After lead application, the package can use the existing public
`WebContentsImpl::media_web_contents_observer()` accessor (currently line 1435)
and call the observer's new public method on the UI thread. The wrapper must
retain its existing managed-WebContents validation and use that contents'
lifetime; it should not add an active-MediaSession precondition or clamp.
Managed contents with no controllers/remotes or disabled features still
complete successfully, following the original native behavior supplied by the
package's bounded original-binary audit.

All three existing classes already carry `CONTENT_EXPORT`. The two out-of-line
methods use those existing class export declarations; no new class, symbol
allowlist entry, build dependency, or `arc_internal.h` entry is proposed. In
non-component Chromium builds `CONTENT_EXPORT` is empty and the code links
within the same framework, as the existing classes do. Full link verification
awaits lead application/build.

## Regression test included

The existing `MediaWebContentsObserverTest` fixture binds real associated
MediaPlayer Mojo endpoints and provides metadata/play/pause helpers. The
proposal records received multiplier messages in its existing TestMediaPlayer
and adds `VolumeMultiplierIncludesPausedPlayers`:

- An empty-manager call is not retained for players created afterward.
- Distinct renderer IDs 41 and 73 are used, avoiding reliance on private
  synthetic controller IDs.
- With internal media sessions enabled and the media-session service disabled,
  one playing player and one explicitly paused player receive exactly `1.75`
  and `-0.5` in order, without clamping.
- Disabling both features with those existing controllers still present leaves
  both received-message counts unchanged after another bridge call and Mojo
  flush.

This tests delivery through the full observer/manager/controller/Mojo path;
it does not replace the implementation with mocks or inspect its private map.

## Validation and limits

- `git -C chromium/src apply --check ../../arc/browser/webextras_volume_bridge.patch`
  passed against current source.
- The controller, manager, observer, and observer unittest each passed
  `-fsyntax-only` using their respective real `out/Base` compiler commands,
  including Chromium's warning/error/plugin settings: exit 0, no diagnostics.
- Temporary VFS overlays mapped proposed headers/sources/test over the original
  paths; object/dependency output flags were removed. No source tree, build
  graph, object output, application, or shared lock was changed.
- There is no current `content_unittests` executable or built observer test
  object, so the proposed test was not executed. The lead should run
  `MediaWebContentsObserverTest.VolumeMultiplierIncludesPausedPlayers` after
  application, then validate the package wrapper against its native evidence.
