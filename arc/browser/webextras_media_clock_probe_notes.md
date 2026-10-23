# Opt-in public media clock probe — review state

Implemented only in the owned test helper/driver, with optional wrapper mode.
The parent experimentally ran native-run6 and diagnostic native-run7: both engines
completed the default11 actions/13 pairs, then failed first-phase DOM preparation
before any clock attempt. Run7 retained metadata-timeout exceptions and hidden,
readyState0/networkState2/no-error video state, with no video HTTP request. This
fits C156's hidden-first-media load deferral; it is not decoder failure proof.
Run7 ours also hit a tab-strip reentrancy FATAL during Browser.close cleanup.
The visible-setup/runtime-log revision below has not been run in either app. Production,
shared patches, held integration proposals and existing results remain untouched.

## Invocation and fixed evidence

After parent review, use a fresh namespace in the verified build/package/probe
slot, for example `bash arc/browser/webextras_native_current_run.sh --run-number 8 --media-clock`.
Archived native-run6/native-run7 must remain immutable.
Do not run the driver directly outside its existing ancestor-lock check. Default
wrapper invocation still means run4 and refuses its archived artifacts; default
driver remains eleven operations/thirteen fixture public pairs on the inert page.

Clock mode adds six fixed phases, three position snapshots and two held-currentTime
reads per phase. Each snapshot invokes exactly position/playbackRate/duration/
currentTime/endOfMedia (5 pairs); a held read invokes only currentTime (1 pair).
Total: 41 measured actions, 115 public call/return pairs. The separate explicit
release acknowledgment has zero engine calls and cannot substitute an empty
measured action. All three receipt attempts are saved, even if the final fixed
attempt has not converged; there is no stop-on-success public polling.

All six phases validate the PUBLIC SERVICE-POSITION clock. First four use real
muted, non-looping video: paused seeks1.25 and2.25, playing rates1 and2 afterseek0.5.
After actual play/seek, they report the exact DOM-read position/duration/rate through
public JS setPositionState. Every receipt saves performance.now before the DOM read,
the actual DOM values, the exact object issued, and before/after setter timestamps.
Playing start input is read after play rather than assumed to remain0.5. Effective
paused rate0 comes from playbackState, while the issued rate remains nonzero1.
Final independent phases leave the real video paused2.25, then issue
setPositionState(duration20,position7.25,playbackRate1.5) with playbackState
paused/playing, distinguishing service input from the actual paused video.
Every phase first clears with an empty dictionary; rate0 is never passed to the
setter. All original-engine service-position outcomes remain provisional until
measured; C156 source alone is not original contract proof. Implicit player fallback
is explicitly UNCOVERED. The required no-audio fixture cannot enter C156's normal
player list without PiP: IsMediaSessionNeeded274..292 and AddOrRemovePlayer294..317.
Service fallback routing1814 onward works without a normal player; Impl645 prefers
its explicit position. This source admission fact is why a DOM-fed public service
stimulus is necessary rather than treating nil player positions as a clock bug.

The actual public DTO result is held strongly in the native helper. Snapshot IDs
become held tokens. A stale/released/wrong-owner token fails before any getter.
Recorder position return identity must equal every DTO getter receiver, including
subsequent held requests. Exact method encodings/classes, caller image, empty args,
counts, offsets, matched returns, ambient stacks and raw caller labels are checked.
High precision sidecar doubles must agree with the recorder's source `%g` numeric
rounding; every raw recorder return is retained and compared.

Native monotonic brackets retain uint64 mach ticks as decimal strings, with integer
timebase. Only relative tick differences become JS numbers. CurrentTime deltas
must fit rate × [secondBefore−firstAfter, secondAfter−firstBefore], expanded by
**4 microseconds × (1 + abs(rate))**, fixed before the first runtime. C156
base/time/time_apple.mm40,83..87 converts mach ticks to integer microseconds;
services/media_session/public/cpp/media_position.cc43..77 computes integer-TimeDelta
elapsed × rate and clamps. The allowance covers four endpoint quantizations;
there is no empirical jitter allowance. Getter brackets over25ms and held intervals
under200ms fail. Samples are nominally400ms apart, with two250ms warmup intervals.
Native and host process.hrtime brackets plus renderer performance.now are saved;
clock origins are never equated. DOM must remain in its intended paused/playing,
nonseeking, nonended interior state. Paused public anchors are derived from the exact issued input receipt, with
duration/rate/end strict. Measured/held values must remain inside their reported
duration and at/after the issued base; the two seek values and independent7.25
position catch constants. All DOM observations must preserve the same input receipt.

This is a functional clock check, not a dynamic comparison allowlist. Raw currentTime
values remain in boundary and sidecar comparison, so two correct clocks can still
produce parity=false. No timing differences have been normalized or accepted.

## Fixture provenance and isolation

Owned finalized webextras_media_clock_fixture.webm is8.208s, VP9 video only,
201593bytes, SHA256fa0a68bc0d09eadfa7d1b199673cd865cc075d813b79cb1866f90af75916ee54.
It was generated once by remuxing three repetitions of Chromium's video-only
media/test/data/bear-vp9.webm (source SHA and exact ffmpeg9.0.2 command are in the
adjacent JSON), without audio or encoding changes. The source asset is2.703s;
no suitable existing ≥6s single-video WebM was found. The resulting HTMLvideo does
not loop. Runtime never regenerates this file.

Before either launch, one exclusive readonly fixture copy/provenance snapshot is
saved under --out, hash checked against the fixed policy, and the exact verified
Buffer is served to both engines at a nonce-localhost URL with bounded range/HEAD
support. Frozen fixture and metadata identities are rechecked before each app.
Clock wrapper source inventories include both fixture files (16 vs default14),
record probeMode, and reject mode/source drift across build/package/run.
Existing isolated app/profile/endpoint/process-birth cleanup, immutable recorder,
frozen comparator/metadata and unsupported-comparator fail-closed behavior remain.

## Verification and limits

`logs/wp/webextras-media-clock-probe-review/` contains the prior real standalone ARC clang
-Wall/-Wextra/-Werror helper compilation, Bun bundle, ffprobe and test logs/manifest.
The current fourteen clock regression groups exercise actual native helper routines against Foundation
mocks, the actual public JS producer/receipt, all six phase schedules, same-held-token slope, constants/wrong rates/stale
receivers, ABI/order/caller/offset/return tampering and the saved original11 replay.
Ten existing wiring tests run on current driver behavior, including all negative,
projection and22-ack unsupported-comparator cases. Historical adoption is recognized
only after strictly reversing the owned clock-v1 extension patch to the exact prior
adopted driver SHA1a31eb68758d66f50e608479b164ac7ff9f825555af300c0d39512f2c10d929f,
verifying the old wiring patch, then restoring the current bytes. Arbitrary future
or partial drift is rejected. The historical wiring patch is unchanged.
Three wrapper extraction groups cover strict flags/default forwarding, the actual
14/16-file inventories, mode/asset/metadata/source drift, and same-slot order.
`logs/wp/webextras-media-clock-diagnostics-review/` records the run7 diagnostic
revision. `logs/wp/webextras-media-clock-visible-review/` records current source
hashes, Bun build and fourteen clock/ten wiring/three wrapper checks.
These are standalone/mocked checks. They establish neither live Chromium media
publication timing nor original laneB behavior, app runtime parity or acceptance.

The unchanged default DevTools open/close lane can leave an undocked window
affecting focus. Clock setup now requires observed actual DOM visibility and all
clock before/after snapshots must remain visible/hidden=false; otherwise the phase
is explicitly inconclusive. This does not claim physical screen visibility or a
successful decode before it is observed. An authentic nil initial warmup DTO makes
strict coverage incomplete; it is nonconvergence/inconclusive evidence, not
independently an engine bug. Playing slope plus interior checks prove elapsed rate
but do not establish absolute alignment with the asynchronous DOM player position.

The DOM-fed public service stimulus was parent-approved after this source admission
audit. It leaves implicit player fallback uncovered and does not add audio or PiP.

## Diagnostic-only follow-up to native-run6

The phase record is now saved with status=preparing and zero attempts BEFORE
awaiting the first preparation. Every clock Runtime.evaluate saves its exact
expression, phase/label, raw result/exceptionDetails or transport error and host
monotonic brackets to clock-evaluations.jsonl before asserting anything. Returned
exceptions still fail; there is no retry or fabricated success value. JS setup
records progress before/after creation, metadata, play, clear, seek, playbackState
and setPositionState, and stores caught name/message/stack/stage before rethrowing.
The video, ordering of stimulus calls, fixed samples, effective rates and all
quantization tolerances are unchanged.

Each failed phase attempts ONE read-only diagnostic evaluation, even if that query
also fails. It records body/API presence, video src/currentSrc (only the exact
owned video URL is exposed; other URLs are redacted), media error/code/message,
ready/network state, dimensions, duration/currentTime, muted/paused/seeking/ended,
visibility/security, VP9 canPlayType and up to16 resource entries for the exact
owned video. It does not fetch another resource, retry preparation, play/pause or
call an engine public getter. The result/error is saved before the original failure
propagates. Resource/server diagnostics remain insufficient to identify a decoder
bug without the actual browser error.

Optional media-clock-http.json logs at most256 nonce-fixture requests with only
agent/resource label, method, validated range (invalid contents redacted), status,
body byte count and monotonic receipt time. It never logs arbitrary URLs, queries,
Authorization/Cookie or other headers; overflow is explicit. Default mode writes
no diagnostic HTTP log and returns the same response. Tests exercise exact current
producer/helper bodies: initial create/async metadata/play success and stage error,
raw exception/transport persistence, phase prewrite/one-shot failed diagnostic,
readonly DOM redaction and bounded HTTP response identity. No clock policy or
comparison allowlist has changed.

## Genuine owned-target activation and runtime cleanup failures

Before any video is created, clock mode saves clock-activation.json, validates the
owned endpoint PID and exact fixture URL/nonce, and sends exactly ONE genuine
Page.bringToFront to that already-owned target. The request, acknowledgment and
host brackets are retained. Every request/sample revalidates the PID; every DOM
sample checks the URL/nonce. Actual document.visibilityState/hidden samples have
a5000ms acceptance deadline after acknowledgment, a bounded pending-RPC wait and
a late-response check, at nominal150ms intervals. Remaining time is recomputed
after synchronous PID verification before starting a sample. Process inspection
is synchronous/noninterruptible, including the evaluator's own identity check;
this is not a hard wall-time interruption promise for an unresponsive OS check.
A late result is never accepted. Only visible with
hidden=false permits video creation. Wrong identity or persistent hidden state
is explicitly inconclusive; no video phase begins. No visibility/focus emulation,
private media flag, global/OS input or hasPlayedBefore preseed is used. All clock
DOM snapshots now save hidden too, and lost visibility makes the phase inconclusive.
The media asset, API inputs, rates, timing slope tolerance and public coverage are
unchanged. Default11/13 does not activate a target.

Source basis: components/no_state_prefetch/renderer/no_state_prefetch_utils.cc113
can defer hidden, never-played media; its observer resumes on genuine visibility
or a real prior-play result. WebMediaPlayerImpl::Load745 calls this deferral before
DoLoad/network. The run7 no-video-request facts match that source path, without a
captured renderer stack proving its dynamic branch. Waiting longer or calling a
fake visibility override would not establish the intended public stimulus.

After owned cleanup and cdp.close, EVERY run (including default mode) scans its
actual app.log for anchored FATAL/DFATAL or Check failed lines. runtimeStatus stores
bounded exact matching lines, total count, log status and natural/forced cleanup.
Any fatal adds a runtime acceptance error even when process disappearance previously
produced a Browser.close label. Any forced SIGTERM adds an error and is never natural;
a launched run with a missing log or unverified cleanup is also an error. A clean
scan is labeled no-fatal-observed, not an exit-code or complete engine correctness
claim. The scanner does not change shutdown or repair the demonstrated core reentry.

Tests use the exact run7 tab_strip_model.cc142 fatal line, clean/quoted noise,
check-only lines and forced/missing-log cases against the actual helper. Activation
extracts actual production code with mock control/identity/visibility calls to reject
wrong PID/URL, changed nonce, persistent hidden, verification crossing the deadline
and late visible samples, and verifies
exactly one owned-target command with a fixed5s bound. Existing source/default replay,
raw tamper, fixed clock math and unsupported-comparator tests remain. Prior11 inputs
were copied and hash-verified before editing under
logs/wp/webextras-native7-failed-inputs/manifest.json. No new app/run has occurred.


## Separate declared service-only mode (run8 preserved)

`--media-service-clock` is a separate opt-in mode, mutually exclusive with
`--media-clock` (duplicates are rejected too). Default11 actions/13 public pairs
and the existing video lane retain their producer/activation/validator behavior.
The run8 video lane remains inconclusive: the one genuine Page.bringToFront did
not change the actual hidden document, so it never created video or collected
clock samples. This new mode does not repair or reinterpret that observation.
The exact11 run8 inputs were copied and verified before this change at
`logs/wp/webextras-native8-service-lane-inputs/manifest.json`.

The new lane uses ONLY public `navigator.mediaSession.playbackState` followed by
`setPositionState({duration:20,position,playbackRate})`. It creates no video,
audio, canvas or decoder, requests no media asset, activates no target and uses
no visibility/focus emulation or new native ABI. The inert owned fixture remains
identified by the same exact URL, nonce, endpoint PID and passive native wrapper
registration. Actual visibility/hidden is recorded; hidden is allowed here.
It tests declared SERVICE-position DTO clocks only. It supplies no evidence of
implicit player fallback, DOM/player alignment, override precedence against real
video, decoder operation or successful activation/visibility.

| Phase | Public playbackState | Issued position | Issued playbackRate | Expected DTO rate |
| --- | --- | ---: | ---: | ---: |
| paused-one | paused | 1.25 | 1 | 0 |
| playing-one | playing | 1.25 | 1 | 1 |
| playing-two | playing | 1.25 | 2 | 2 |
| paused-two | paused | 2.25 | 1 | 0 |
| declared-paused | paused | 7.25 | 1.5 | 0 |
| declared-playing | playing | 7.25 | 1.5 | 1.5 |

Each phase records its exact issued input, public playbackState, performance.now
before/after the setter, actual API/URL/nonce/visibility observation, progress and
all raw Runtime.evaluate results/exceptions/transport failures before assertion.
There is no input rate0: paused effective0 is produced by public playbackState.
The six phases reuse the unchanged native helper: three complete public position
snapshots (five getters each), two currentTime reads on the SAME retained third
DTO, then explicit release. Thus successful evidence is41 action spans and115
admitted public call/return pairs including the unchanged baseline11/13.
No receiver, ABI, raw caller/offset, frozen comparator, callback or count checks
are relaxed. Raw numeric currentTime is preserved and strict cross-engine
comparison remains strict; this is not a timing-normalization allowlist.

Final fixed snapshot convergence requires duration20, exact effective rate and
endOfMedia=false. Measured/held values must be finite, above the issued lower
base and strictly inside(0,20), excluding clamp-based false slope evidence.
Paused values equal their distinct exact issued base within the predeclared4us.
The SAME retained DTO must obey all three mach-bracket slope comparisons, still
with the original fixed4us*(1+abs(rate)) endpoint quantization allowance.
Warmups are saved diagnostics, not a hidden success poll. A genuine nil position
receipt is explicitly inconclusive; raw coverage remains incomplete and cannot
pass. Wrong class/ABI/owner/receiver or malformed traces remain errors. If the
fixed attempts leave no held DTO, the attempted release is recorded as such;
this does not synthesize a successful release or missing public pairs.

Absolute alignment of a PLAYING DTO's offset to setter/DOM time remains
UNCOVERED: the held slope and lower/interior checks do not prove that alignment.
The paused distinct-base checks anchor the three paused phases. Host RPC brackets
are diagnostic only; no hrtime/mach origin equality or original IPC timestamp
preservation is assumed. Original-native service behavior stays provisional
until genuinely measured, and failure/nonconvergence is diagnostic evidence,
not by itself an engine defect. Default DevTools focus/undocked-close and real
native cleanup limitations are unchanged. Fatal/Check-failed scanning and forced
SIGTERM rejection apply to every mode.

The fresh-build wrapper accepts/forwards the strict new flag under the same
verified ancestor lock. Both opt-in modes retain a16-file source inventory;
service mode labels `mediaResourceUsed:false` and freezes the unused video asset
and JSON only as inventory. It never loads/serves them. Default remains14 files.
Mode changes or any inventoried source/asset drift are rejected before/after
build/package/probe. No app or runtime run is part of these standalone checks.

Tests execute the actual service JS with throwing media-element/DOM mocks, all
six exact inputs and setter ordering, wrong URL/nonce/missing API rejection,
hidden observations, exact receipt caching and failure stages. They execute the
actual service measurement branch for six phases/30 clock actions/102 public
pairs plus six releases, asserting66 raw evaluations and no activation/resource
stimulus. Relational negative controls reject constant clocks, rate/duration/end
mismatches, stale tokens/replaced DTOs/owners, changed receipts/identity/state,
reordered attempts, paused wrong bases and out-of-range values. Existing saved
default11 replay, raw receiver/caller/ABI/offset tampering, fatal/forced cleanup,
old video stimulus and visibility tests remain. Frozen run8 video producer,
validator, phase definitions and activation helper bodies are byte-compared in
tests. The exact current clock extension still reverses to the frozen previously
adopted historical driver before the10 wiring behavioral/adoption tests run;
partial/arbitrary drift is not accepted.


The separate opt-in genuine frontend diagnostic is documented in
`webextras_devtools_frontend_probe_notes.md`; it adds no public fixture pairs and
is mutually exclusive with both clocks. Existing clock behavior/policy is unchanged.
