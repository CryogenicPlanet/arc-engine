# Thumbnail readiness/cooldown integration preflight

Status: **PRIVATE STAGED CANDIDATE ONLY; not applied or runtime validated.**
New incremental proposal `webextras_thumbnail_cooldown_host_gate.patch` goes
AFTER frozen `webextras_thumbnail_capture.patch`24494a25. Frozen readiness
`webextras_thumbnail_readiness.patch`176d624a and both prior proposals/notes
remain unchanged. No actual Chromium/API/shared source, maintained patch,
MANIFEST, GN output, application, profile, runtime lock or board was changed.
Pixels and Root/lifecycle proposals are not part of this candidate.

## Concrete preflight result

Frozen cooldown24494a25 changes ordinary Chrome: it widens the readiness-decrease
reset from HaveFinalCapture to Cooldown-or-later, and always stops the timer.
That cannot be integrated as-is under COMMON's current host-gating condition.
The newly staged increment restores the complete original nonhost branch:
readiness<Final AND state==HaveFinalCapture, with no newly added timer Stop.
It adds state==Cooldown only when `--arcmod-host-app` is present, and limits the
new timer Stop to that same host mode. Host HaveFinalCapture still receives the
frozen proposal's timer Stop. CaptureState currently has precisely five values;
HaveFinalCapture is the only value above Cooldown. Therefore the host branch
preserves frozen cooldown behavior exactly for every current enum state.

The driver regression supplied by the frozen patch previously ran without any
host flag. The increment gives only that test a ScopedCommandLine and appends
arcmod-host-app. The fixture/global default and other driver tests are untouched.
CommandLine and ScopedCommandLine use existing public //base and test //base
support dependencies; the increment requires no BUILD.gn change.

Readiness176d624a is already correctly host-gated. Primary-main-frame filtering
precedes the host URL predicate; host matching-finish clears pending before its
URL gate; ordinary host finish waits for onload; same-document finish finalizes;
onload tests GetVisibleURL. The four chrome internal host exclusions use GURL
DomainIs. The existing nonhost same-document exclusion, unrestricted internal
URLs, unconditional first Initial and second Initial call remain byte-for-byte
in their original branches. Discard/destruction and UpdateReadiness guards are
unchanged. The proposed host same-document decrease can occur during cooldown,
so readiness must be integrated with the corrected cooldown chain, not alone.
This preflight does not reestablish the earlier native readiness audit or claim
that readiness fixes the recorded extra501x341 callback.

## Minimal applicable chain and ownership

Two dependency-valid orders were strictly checked against current C156:

1. capture24494a25 -> new cooldown_host_gate -> readiness176d624a.
2. readiness176d624a -> capture24494a25 -> new cooldown_host_gate.

Every apply/check used --whitespace=error-all. All exits were0; resulting seven
files (five changed/proposed plus two unchanged headers) are byte-identical in
both orders. The new increment is dependent on capture24494a25 and must not be
presented as a standalone patch against raw C156. No pixels patch was used.

Current patches/MANIFEST has NO row for any of the five changed/proposed files.
No current patch ownership collision exists. At actual integration, add each
exactly once under the package's `0010-webextras.patch` (or lead's chosen package
patch filename), then use the required maintained-patch regeneration:

- chrome/browser/ui/thumbnails/BUILD.gn
- chrome/browser/ui/thumbnails/thumbnail_capture_driver.cc
- chrome/browser/ui/thumbnails/thumbnail_capture_driver_unittest.cc
- chrome/browser/ui/thumbnails/thumbnail_readiness_tracker.cc
- chrome/browser/ui/thumbnails/thumbnail_readiness_tracker_unittest.cc (new)

Readiness adds //url to the thumbnails and unit_tests targets and its new test
source to unit_tests. The public //url target has no restricting visibility;
independent preflight confirmed the dependency choice. //base already exists.
No new header/public API or shared hook is required by this bounded chain.
Parent owns actual integration and must recheck MANIFEST for concurrent changes.

## Verification actually executed

Owned `webextras_thumbnail_cooldown_host_gate_test.py` extracts all10 actual
driver method bodies and the actual CaptureState enum from frozen source
snapshots. It compiles and executes those bodies with deterministic mock timer,
client, scheduler and CommandLine inputs. DCHECK is recorded nonfatally so both
old and new nonhost failure transitions can be compared without crashing the
suite. It is not a reimplementation of the state-transition decision logic.

- Positive exit0:120 original-vs-gated nonhost single-transition controls cover
  all five states, three readiness values, scheduled/can-capture/timer booleans.
  They compare client calls, state, scheduler priority, timer operations, frame
  latch, retry count and DCHECK occurrence. This also preserves the known
  ordinary-Chrome cooldown/readiness DCHECK rather than silently fixing it.
- Positive host lifecycle: a frame arrives during old cooldown; readiness drops
  midway; stop/request/start order is required; the old timer is canceled beyond
  its entire retry horizon; new final readiness must start fresh cooldown whose
  first tick cannot use the old frame; a new frame then permits final stop.
- Old-current-source host control exits73 at the required restart/cancel check.
- Frozen globally ungated cooldown nonhost control exits74 on a real observable
  divergence from current ordinary Chrome. This discriminates an accidentally
  missing host gate, rather than merely asserting the added condition's text.

Four full candidate translation units also passed real Chromium compiler flags
with -fsyntax-only and private VFS overlays: driver.cc, driver_unittest.cc,
readiness_tracker.cc and new readiness_tracker_unittest.cc, all exit0 with zero
diagnostics. Driver/test commands were freshly read with siso query commands;
this metadata query did not build. Existing target command arrays supply the
readiness/test templates. Chromium warnings/plugins/modules were retained;
object/dependency outputs were removed. Query output included unrelated
non-UTF8 text; the capture script uses replacement decoding while selecting the
exact ASCII target command. No generated command dump or unrelated content was
written into evidence.

These checks do NOT execute the proposed Chromium unit-test fixtures, real timer
runner/scheduler, renderer, capture/compression/image pipeline or app boundary.
The old new driver regression and18 readiness cases remain unexecuted in their
actual Chromium harness; out/Base/unit_tests is absent. No browser scenario or
launch-slice comparison was run for this preflight. Scenario status, uncovered
coverage and unresolved thumbnail count/pixel provenance therefore remain
unchanged. This is not an acceptance report for the work package.

## Frozen evidence and integration gates

Own evidence namespace: `logs/wp/webextras-thumbnail-host-preflight/`.
It records exact source/patch/MANIFEST inputs, private staging/composition
commands, durable base/cooldown/candidate/composed snapshots, extracted actual
code/test outcomes and complete syntax command arrays/empty diagnostics.
The manifest pins all artifacts and the three new owned proposal/test/note
files. Original source/patch inputs were rehashed after validation.

The minimal candidate is technically staged and applicable but remains held for
fresh review and parent's integration transaction. COMMON still requires actual
application inside locked.sh, host behavior gating, unique MANIFEST ownership,
regen-patches, launch-slice9/9 and no required-scenario regression, reverting and
regenerating before lock release if validation fails, and the authorized APPLIED
board report. This worker acquired no runtime/build lock and made no such claim.
Prior extra thumbnailReady1/image501x341 versus native0 remains a genuine
unresolved runtime observation; this chain must be measured without count
padding/filtering, visibility guesses, pixel inference or a tolerance change.
