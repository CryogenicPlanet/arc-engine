# HELD host-only volume notification producer correction

This DISTINCT incremental proposal applies after the reviewed
`webextras_volume_notification.patch` SHA
399b42ef1823a4603c5157c5f542ffedcc9d3b563b7073c8f50080507b372a81.
The optional outer-routing increment9161ec2b composes in either order with it.
No actual Chromium/API/shared file, maintained patch, manifest, GNI, app/profile,
lock, engine build, recording, existing proposal or old evidence was changed.
This is a private preflight correction for COMMON's host-scope rule.

## Minimal production change and verified flag path

Only the new `WebMediaPlayerImpl::SetVolume` notification is gated:

```
previous_volume != volume &&
base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")
```

The production file already includes base/command_line.h. No Chrome header,
feature registry, new switch, global cache, Mojo change or generator dependency
is introduced. The browser's existing
ChromeContentBrowserClient::AppendExtraCommandLineSwitches2932–2935 forwards this
exact switch only when the browser has it and child process type is renderer.
ChromeContentRendererClient::RenderFrameCreated658 already uses it. Full source
hashes and bounded excerpts are preserved in renderer-switch-proof.json. This
is current source proof of propagation, not a new renderer runtime observation.

The new client callback stays after the existing muted-status callback. Its
argument remains the changed UNMULTIPLIED double input; equal input and multiplier-
only updates do not emit it. Pipeline multiplication, watch-time reporting,
muted notification, delegate audio/metadata update, hidden-playback resume,
visibility reset, early return and final UpdatePlayState remain unchanged for
both host and ordinary rendering. The gate never returns early from SetVolume.
The entire SetVolumeMultiplier body remains byte-identical to ordinary source.

This supersedes the earlier399 notes' deliberate unconditional transport policy
only when this increment is composed. Ordinary player changes no longer reach
new DidPlayerVolumeChange, so they do not create the new downstream HTML/Mojo
volume message through that producer. Existing HTML/browser default observer
APIs and generated bindings remain as in399; no extra browser dedup or mute-to-
volume conversion is added. Alternate native player emitters remain unproven.

## Tests and checks

The two prior real WMPI notification test cases now use scoped host switches.
One new real WMPI case explicitly removes the switch, requires no new volume
callback, and checks existing muted calls and pipeline products. ScopedCommandLine
restores the prior process state. These Chromium test cases compiled, but were
NOT executed; no blink_unittests binary was built or run.

`webextras_volume_notification_host_gate_test.py` extracts the actual composed
SetVolume/SetVolumeMultiplier bodies and the complete original ordinary bodies.
Removing only the399 snapshot/notification block must reproduce the original
producer byte-for-byte. The standalone compiled fixture then executes16 ordinary
cases (empty/unrelated switches × watch/audio/hidden combinations) against the
full original source-body control, and8 host cases with exact doubles, repeated
input, multiplier-only changes, zero effective mute, unmute and double precision.
It compares pipeline/watch/client/delegate/resume/play-state traces and state;
removing only host volume events must leave the original host-side behavior.
All24 controls pass. Three perturbations compile and fail their intended runtime
assertions: unconditional emission, early-returning ordinary SetVolume, and
reporting multiplied instead of input volume. Native/Mojo/CommandLine dependencies
in this fixture are mocks, so this is source-body execution, not IPC delivery.

Only the two affected real translation units were syntax-checked with the prior
actual Chromium/macOS arm64 SDK/plugins/Werror commands and a private VFS overlay:
WMPI and WMPI unittest both exit0 with empty diagnostics. Original real-generated
Mojo bindings were reused unchanged; there was no parser/generator rerun, GN,
object build or link. Both399→host→outer and399→outer→host strictly apply and
reconstruct all13 files identically. Starting from frozen original sources,
399→host→outer independently reconstructs those same bytes.

Fresh independent code/test review is bounded-CLEAR, no P1/P2:
logs/wp/webextras-volume-host-gate/review.md SHAae4e76ec. The reviewer independently
reran24 controls and all three compiled negatives. Source/header/generated hashes,
compiler commands, extracted fixture, outputs and composition checks are frozen
under logs/wp/webextras-volume-host-gate/.

## Readiness and limits

Ready for held integration review only. This increment does not activate the
owned consumer, functional Media source, Root lifecycle, registration/delivery,
MEDIA pending accounting, volume setter bridge, outer-owner routing or any
native app callback. The missing golden volume callback remains an integration
and runtime acceptance gap. No new legacy consumer workaround or golden value
was introduced. Existing scenario/golden results are unchanged by this unapplied
producer-only host gate.
