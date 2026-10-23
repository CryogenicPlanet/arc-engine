# Proposed opt-in public media clock probe

Historical design: implementation uses parent-approved explicit DOM-fed public
service-position inputs because C156 excludes no-audio implicit players. See
[implementation notes](webextras_media_clock_probe_notes.md) for current scope.

Design only. No probe/production/held pipeline, saved result, app, build, lock or
board change. Extend the existing owned `webextras_native_probe.m/.ts` behind an
explicit `--media-clock` option; do not duplicate its process/injection/cleanup
runner. Default11 operations/13 public call pairs and fixture behavior stay
unchanged. The opt-in gets a new isolated output directory and explicit expected
coverage derived from its selected phase list. Parent reviews before editing.

## Source facts that fix the stimuli

- `content/browser/media/session/media_session_impl.cc:641` first takes the
  routed MediaSessionService's explicit position, if present. Only otherwise,
  with exactly one normal player and no one-shot players, does it use that
  player's position. `MediaSessionController::GetPosition`185 and state-change
  receipt229 expose/update its cached native position. Routing1770 onward
  admits playing OR paused players, and can fall back to an active service.
- Blink `modules/mediasession/media_session.cc:250` accepts an empty dictionary
  to CLEAR explicit position; setPositionState with playbackRate0 throws at299.
  `setPlaybackState`164 calls RecalculatePositionState;382..404 publishes
  effective rate0 for PAUSED while remembering the declared nonzero rate.
  Switching to PLAYING resumes the declared rate after rebasing its position.
  Do not pass0 as the public setPositionState playbackRate to manufacture pause.
- `MediaSessionServiceImpl::SetPositionState`81 saves it and rebuilds browser
  aggregation; this is asynchronous renderer→browser IPC. Runtime.evaluate
  completion, DOM pause/seeked and two animation frames are NOT browser Mojo
  flush barriers. Never use private FlushForTesting or a C++ DTO constructor.
- `services/media_session/public/cpp/media_position.cc:43` computes currentTime
  from monotonic elapsed time × rate plus base position and clamps to[0,duration].
  end_of_media is a distinct member; do not derive it from time==duration.
  `base/time/time_apple.mm:83` uses mach_absolute_time for TimeTicks, providing a
  same-process monotonic clock for getter brackets without calling private ABI.
- Exact public metadata: session.position returns@; ArcMediaPosition
  playbackRate/duration/currentTime returnd; endOfMedia returnsB. Read only those
  five public getters. The existing ArcMediaPosition spec names helper32b5dd0
  (not prompt33b5dd0) as the inferred GetPosition counterpart; no new binary
  identity or private ABI claim is needed by this probe.

## Smallest owned extension

Add only opt-in operations to the existing native helper:

1. `clock-snapshot`: use the already passively captured media receiver tied to
   the fixture owner/URL; bracket position and the returned DTO's four getters.
   Retain that returned DTO strongly under a per-phase nonce/token. Validate
   actual ArcMediaPosition runtime class and exact getter ABIs before typed
   objc_msgSend double/BOOL calls. Return object/receiver fingerprints plus
   bracketed scalar values, never a fabricated DTO/native pointer.
2. `clock-held-time`: reread currentTime on that EXACT retained DTO, with its own
   monotonic bracket. Preserve its fingerprint/token in the response. It adds
   one explicit public call, not a dummy fresh position getter. Retain no extra
   app delegate and do not replace the app's media delegate.
3. `clock-release`: release only helper-owned DTO state after a phase, with no
   public engine invocation. Reject a stale/missing token; do not silently
   substitute a newly fetched DTO. Bound phases, samples and retained objects.

For each call save raw mach_absolute_time ticks and mach_timebase_info, public
call/return recorder byte offsets, exact class/selector/type/receiver and value.
Keep existing balanced inner/outer stacks, frozen recorder identity, fixture
caller attribution, forwarding and source/framework/helper identity checks.
Update coverage for these opt-in operations explicitly: a fresh snapshot has5
public pairs; each held-time sample has1. Default operation validation must
remain unchanged. First snapshot currentTime is q0; two subsequent held samples
make7 pairs per measurement phase before any explicitly configured warmup
snapshots. If the fixed two-warmup-snapshot plan below is used, each ordinary
phase has3×5+2=17 public pairs. Derive total coverage from that frozen schedule;
do not hardcode the old13-pair count for opt-in.

TS requests are serialized. Get a DOM snapshot before sending each helper
request and another only after its matching response. Store CDP send/receive
brackets, renderer performance.now, fixture phase nonce, video.currentTime,
paused/ended/playbackRate/duration/readyState/seeking/document.visibilityState,
mediaSession.playbackState
and the explicit setPositionState inputs last issued by the fixture. Sequence
ordering provides pre/post observations without equating renderer and native
clock origins. Native getter brackets use native mach ticks, not Date/NSDate.

Use existing exclusive recorder copy, trusted fixture selection, dedicated
profile/app/CDP endpoint, request nonce/atomic response and helper PID-birth
cleanup. Fail on wrong fixture, stale response, ABI mismatch, missing receiver,
nil DTO, unexpected extra call, absent sample, nonfinite finite-fixture values,
or any recorder/identity error. A failed serialization is an error, never an
empty successful sample. No DYLD library should inject a second copy of the
same interposer or replace caller labels. The active/default runner is untouched
until parent authorizes implementation and a later isolated run.

## Controlled fixture and phase order

Prepare ONE generated, finite, seekable local WebM (e.g.6–8s VP9 color/test
frames without audio), finalized/remuxed to contain finite duration. Pin its
bytes/hash and serve the SAME file to both apps from the isolated localhost
fixture server with ordinary seek/range behavior. Verify codec support,
loadedmetadata, finite duration and seekable ranges before measurement.
The existing700ms looping MediaRecorder fixture is unsuitable: wrapping can hide
advancement, and unfinalized WebM duration may be Infinity. Do not regenerate
nonidentical media separately per app or reuse results from that looping fixture.
No external media/account/player/iframe, PiP, live stream, audio focus switching,
second player or renderer hidden-tab throttling should confound this lane.

A. Real player fallback (clear setPositionState({}) first; one video, loop=false):

- Warm it once with actual video.play(), then pause and seek to1.25s. Await
  DOM seeked/paused state, then bounded browser convergence. Fresh DTO should
  converge to actual finite video duration, rate0 and DOM's stable seek value.
  Two held currentTime reads about400ms apart must remain fixed.
- Seek away from boundaries (e.g.0.5s), set video.playbackRate=1 and play. Require
  actual DOM advancement and not paused/seeking/ended. Take q0 and two held DTO
  readings about400ms apart while leaving player/session state unchanged.
- Pause/seek back to an interior point, set rate2 and genuinely play; repeat.
  Keep all samples away from0, clip end, seek, wrap and stalls. Recheck DOM after
  every sample; crossing a boundary invalidates that phase, not a passing clock.
- Pause and seek a DISTINCT interior value (e.g.2.25s), then a fresh snapshot and
  held reads. This defeats a constant chosen to pass the first paused phase.

B. Explicit public Media Session position (same actual video remains paused):

- Set playbackState='paused', then setPositionState({duration:20,position:7.25,
  playbackRate:1.5}). This is intentionally distinct from DOM video time/duration.
  After convergence fresh DTO must reflect the explicit service override:
  duration20, effective rate0, fixed currentTime7.25. DOM stays paused at2.25.
- Set playbackState='playing' without claiming the paused video itself plays.
  The declared Media Session clock should now advance at1.5. Use the same held
  DTO delta test, then pause the declared clock and acquire a fresh fixed DTO.
  The real-playing-player requirement was already tested in laneA; laneB proves
  public declared-clock precedence, not decoder playback.
- Optional saturation phase: declared duration2, position1.9, rate2, playing.
  A later fresh DTO/currentTime must clamp at2. Record endOfMedia independently;
  do not expect it to become true merely because extrapolation reached2.

Each state change starts a NEW phase/held DTO; do not assume an old DTO mutates
when DOM state changes. The decisive held samples run while fixture state is
constant. Keep early IPC transients, and use a FIXED sample schedule identically in both
engines, not stop-on-first-success polling. For example, two explicit warmup
clock-snapshot requests at0/250ms plus the measurement snapshot at500ms are all
recorded and counted; the latter becomes the retained DTO for q0 and held reads.
Freeze this schedule in the probe manifest. Readiness requires the final sample
(and any chosen consecutive sample rule) to satisfy the phase predicate; failure
stays failure/inconclusive. If a2s convergence budget is needed, predeclare an
identical larger finite sample schedule for both engines before running, instead
of silently adding data-dependent calls. DOM preparation may use bounded DOM-only
polling, recorded outside the public engine call schedule. A nil/wrong-type DTO,
missing sample, stale response or wrong final rate is a visible error; it cannot
be made successful by omitting the row or continuing unlogged until it matches.
Initial original behavior must be measured before claiming the C156-based laneB
expectations are an established Arc contract.

## Clock criterion and comparison policy

For held samples q1 at native bracket[a1,b1] and q2 at[a2,b2], fixed positive
rate r and no saturation, the permitted elapsed interval is
[a2-b1,b2-a1]. Compare q2-q1 with r times that interval, retaining the signed
residual interval. Derive a tiny representation allowance from the verified
native clock/time-unit quantization and JSON precision; do not widen it after
seeing an engine difference. Require well-separated samples (about400ms) so a
constant cannot pass a broad dispatch bracket. For r0, require fixed time within
that representation allowance. Rate2 and1.5 catch a fake generic wall clock
that advances at1 regardless of the DTO's rate. The two paused seek values and
declared duration/position override catch fake or stale starting values.

DOM pre/post ranges are supporting evidence, not an arbitrary strict
zero-latency browser-position equality assertion: player position publication,
decoder clock and Mojo transport may lag. Report freshness/convergence and raw
DOM/public residuals separately. The held-object slope is a same-process test
that avoids conflating IPC latency with elapsed-clock correctness.

Preserve every raw currentTime value, clock and recorder row. Evaluate original
and ours independently against these predicates, then compare stable selectors,
classes, receiver relationships, counts/order, nil/error behavior, playbackRate,
duration and endOfMedia strictly where the phase supplies a proven expectation.
Different playing values across separately scheduled runs are not automatically
a failure of the clock, but a successful relational probe is also not an
existing boundary-diff normalization rule.

Do not change the accepted media scenario's0.322324/0.422503 values, round them,
replace them with a sentinel, normalize all doubles, or grant a class-wide
currentTime exemption. Only after BOTH frozen implementations demonstrate the
clock relation and phase/DOM facts should a narrow timing policy be requested:
exact ArcMediaPosition.currentTime occurrences in proven active/unclamped phases,
with relational evidence retained and paused/static/clamp rows still exact.
Duration/rate/end/nil/callback/order differences remain visible. Existing
runtime-only PASS flags and old saved traces do not supply this new proof.

Implementation touches only the two owned probe files and optional owned fixture
helper/test assets. Tests should reject a forged constant, wrong-rate clock,
stale phase/DTO token, missing sample, overwide bracket, boundary crossing and
forged raw recorder offsets, alongside one source-backed valid synthetic clock.
No app/build/runtime step is authorized by this plan itself.
