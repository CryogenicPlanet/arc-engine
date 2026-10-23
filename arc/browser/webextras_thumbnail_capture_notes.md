# Thumbnail capture readiness regression proposal

Proposal: `arc/browser/webextras_thumbnail_capture.patch`. The lead owns applying
it to Chromium and the maintained patch set. Only this proposal and these new
notes were written; Chromium sources, shared tests, applications, existing
notes, and maintained patches remain untouched.

## Observed failure and cause

`results/webextras/native-run2/ours156/app.log:16` records a fatal DCHECK in
`ThumbnailCaptureDriver::UpdateCaptureState`: state `kCooldown` (3) reached the
assertion requiring `kCapturing` (2), with readiness
`kReadyForInitialCapture` (1). The stack passes through
`ThumbnailReadinessTracker::DidFinishNavigation`.

A readiness decrease invalidates the existing page's final capture. The driver
already handles this after `kHaveFinalCapture`, but not while the old page is
still in `kCooldown`. A scheduled capture can remain permitted when readiness
changes from final to initial, so the cooldown state reaches the final DCHECK.
Its retaining timer also still belongs to the old capture.

## Proposed behavior

Extend the existing readiness-regression invalidation branch from
`capture_state_ == kHaveFinalCapture` to `capture_state_ >= kCooldown`, and call
`cooldown_timer_.Stop()` before stopping and resetting capture.

The existing scheduler/capture path then requests and starts capture for the
new page when permitted and able. Once that page reaches final readiness,
`StartCooldown()` starts its own timer and clears the previous frame/retry
state. The DCHECK remains in place. No readiness notification, image callback,
or legitimate new-page capture is suppressed.

## Regression test included in the proposal

`ThumbnailCaptureDriverTest.RestartsCaptureOnReadinessDecreaseInCooldown` uses
the existing strict client mock, stub scheduler, and mock-time environment:

1. Start capture, enter final cooldown, and receive an old-page frame.
2. Halfway through cooldown, regress readiness to initial while scheduled.
   Require `StopCapture`, `RequestCapture`, and `StartCapture` in order.
3. Verify the canceled timer leaves no pending main-thread task, then advance
   beyond its entire retry horizon. Capture must remain at low priority and
   must not stop.
4. Reach final readiness again. A fresh cooldown must remain active through
   its first timer tick despite the old page's frame. A newly received frame
   must then let the next tick stop capture and clear scheduling priority.

The unpatched source reaches its reported DCHECK at step 2. Merely removing
that assertion would still fail the required capture restart and timer checks.
Resetting capture without canceling the old timer would fail the pending-task
assertion. The final phase verifies that ordinary final capture still works.

## Validation and limits

- `git -C chromium/src apply --check ../../arc/browser/webextras_thumbnail_capture.patch`
  passed against current sources.
- Patched `thumbnail_capture_driver.cc` and its patched unittest both passed
  `-fsyntax-only` with their respective real `out/Base` compiler commands,
  including Chromium's warning/error/plugin settings: exit 0, no diagnostics.
  Temporary VFS overlays supplied the proposed source/test contents; object
  and dependency output flags were removed.
- No shared build, application run, or lock acquisition was performed. The
  current output has no `unit_tests` executable or built test-support/gtest
  libraries, so the regression test was not executed. After application, the
  lead should run the complete `ThumbnailCaptureDriverTest.*` suite and repeat
  the owned-app comparison that produced the crash.
