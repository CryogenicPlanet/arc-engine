# Host thumbnail readiness proposal

Status: **UNAPPLIED; fresh review and lead integration required.**

Proposal: `arc/browser/webextras_thumbnail_readiness.patch`.
SHA256: `176d624a1424fc0e397e45b0f810ef12e5da82bf6281dbc105ba3a981533161a`.
No Chromium source, maintained patch, shared test, production wrapper, app,
profile, build graph, or runtime lock was modified. Staging and syntax checks
used `/tmp/webextras-readiness-proposal`; the only workspace writes are the
owned proposal/notes and owned validation logs.

This restores a recovered native readiness policy. It does not suppress a
callback by count, scenario name, page visibility, bitmap size or assumed image
content. The extra501x341 callback remains a real unresolved runtime difference
until a paired scenario measures the integrated change. Its pixels were not
recorded; an old NewTab image has not been established.

The detailed bounded native evidence and current raw sequence table are in
[webextras_thumbnail_delivery_audit_notes.md](webextras_thumbnail_delivery_audit_notes.md),
section "Current required-scenario follow-up". The direct original enum-label
ToString/table proof establishes values4/6 as the two same-document cases.

## Proposed behavior

Only `--arcmod-host-app` uses the restored native policy:

- Start still requires a primary main frame. For Arc host mode it accepts
  same-document starts, and ignores URLs whose scheme is chrome and whose
  DomainIs matches start-page, assistant, native-bookmarks or skills-manager.
  Ignoring an internal start leaves an older pending navigation and readiness
  untouched. This is the public GURL hostname-boundary predicate, not a prefix.
- A matching finish clears its tracked navigation before testing the current
  request URL. Internal redirected finishes remain excluded, with the pending
  handle already consumed. A same-document finish becomes final unless already
  beyond initial readiness; an ordinary finish does not enable initial capture.
  No committed/error-page gate is invented.
- Onload checks WebContents GetVisibleURL, including pending virtual URLs, and
  excludes those same internal hosts. It does not use GetLastCommittedURL.
  Other onloads use the existing final-readiness path, including its destruction
  guard and unchanged duplicate-final behavior.
- The existing discard and destruction routes remain intact: discard resets
  readiness without clearing the pending handle; destruction clears pending and
  resets readiness. Existing UpdateReadiness handles IsBeingDestroyed.

Outside Arc host mode the original current implementation remains intact,
including its unconditional UpdateInitial before the readiness comparison and
its second UpdateInitial call. This proposal does not repair or simplify that
nonhost baseline. There is no change to ThumbnailImage token invalidation,
compression/decode, subscription, pixel conversion, or accepted notification
queueing.

The patch touches three proposed Chromium files:

1. `chrome/browser/ui/thumbnails/thumbnail_readiness_tracker.cc`: host detection,
   recovered URL predicate and the host branches above. No header API change.
2. `.../thumbnail_readiness_tracker_unittest.cc`: 18 focused transition cases.
3. `.../BUILD.gn`: adds the new test source and explicit url deps for its source
   and test target. The test is appended with an independent `sources +=` line
   after the existing list so it composes in either order with the already
   proposed pixel test's insertion into that list.

## Tests supplied

The new fixture uses the real ThumbnailReadinessTracker, content test harness,
MockNavigationHandle and GURL. A TestWebContents subclass controls the public
visible-URL and being-destroyed inputs, independently from its committed URL.
It does not expose private tracker fields or require renderer/network/window
work. Assertions check emitted readiness transitions, not copied implementation
state. Cases cover:

1. Ordinary host finish stays not-ready until onload; repeated onload deduplicates.
2. Same-document host start invalidates and finish becomes final.
3. Excluded start preserves a previously tracked navigation.
4. Excluded start preserves existing ready state.
5. Redirected excluded finish clears pending before its URL gate.
6. Only the most recent tracked handle can finish.
7. All four internal hosts, including a subdomain/trailing-dot boundary case.
8. Scheme and hostname-boundary negatives; permitted pages still capture onload.
9. Visible-vs-committed URL distinction in both directions.
10. Subframe start cannot create a pending navigation.
11. No added HasCommitted/IsErrorPage guard.
12. Being-destroyed contents cannot become final.
13. Discard resets readiness while preserving pending identity.
14. Actual test-WebContents destruction resets readiness.
15. Destruction notification clears a pending navigation.
16. Nonhost initial/final transitions and same-document exclusion.
17. Nonhost internal-page capture remains enabled.
18. Nonhost finish-after-onload preserves the existing final-to-initial transition.

These are proposed Chromium unit tests, **not executed tests**. The executable
`chromium/src/out/Base/unit_tests` is absent. No GN regeneration, unit-test build,
link, browser scenario, or native engine run was performed by this worker.

## Validation actually performed

- `git -C chromium/src apply --check --whitespace=error-all
  ../../arc/browser/webextras_thumbnail_readiness.patch`: exit0.
- The complete proposed production source passes `-fsyntax-only` with its actual
  `out/Base` command from siso query commands: exit0, no diagnostics. VFS overlays
  replace only the staged source; object/dependency output flags are removed.
- The complete new test passes `-fsyntax-only` with the existing thumbnails
  unit-test target's real compiler command (thumbnail_image_unittest.o), replacing
  only its input and removing object/dependency output flags: exit0, no diagnostics.
  Chromium warning/error settings and compiler plugins were retained.
- In independent temporary trees, every patch passes strict whitespace/apply
  checks and applies in each sequence: readiness+cooldown; cooldown+readiness;
  pixels+readiness+cooldown; readiness+pixels+cooldown. Cooldown reverse-check
  then passes. In all four compositions, the resulting tracker/test bytes equal
  the exact syntax-checked staged bytes.
- Actual shared source/header/BUILD remain byte-identical to the copied baseline.
  Existing production and probe source freeze was respected.

Evidence:

- `logs/wp/webextras.thumbnail.readiness.source.check.log`
- `logs/wp/webextras.thumbnail.readiness.test.check.log`
- `logs/wp/webextras.thumbnail.readiness.apply-checks.json`
- `logs/wp/webextras.thumbnail.readiness.validation.json`
- Staged exact compiler argument arrays:
  `/tmp/webextras-readiness-proposal/{source,test}-syntax-command.json`.

Baseline hashes:

| File | SHA256 |
|---|---|
| tracker.cc |4e7bcf0bca5f30221b868bca5b2760c5b0481127764786e652cb3488afb1f622|
| tracker.h |9ce063cc0c471bf06dd607bc705fb2a815b2e6376afa4c61a787a068ca464acb|
| BUILD.gn |7ca674ccb19e6f8cb049ad4ca8315259ab737cd0e53c9b80c2678deacc6d1045|

Syntax-checked staged hashes:

| File | SHA256 |
|---|---|
| tracker.cc |beeb766631c5bcb513cbf3ca9d0b2916df43291f2b27724c8848a0a775daab1c|
| new tracker_unittest.cc |dba2b802690aa3b99091b8f239c8bed162091fcb260d36a36930b73fd0dd0bc0|
| BUILD.gn |a05e876cb15a4ce44657f02237c07a450552d4abff73e59537e61a92ae73615f|

## Integration requirements and remaining limits

Apply/review the existing `webextras_thumbnail_capture.patch` cooldown/navigation
correction with this proposal before host runtime validation. This restored
policy deliberately resets readiness on same-document starts, which can occur
while a capture is cooling down. The existing C156 driver has an independently
proved invalid cooldown-state path on a readiness decrease. Do not activate this
proposal alone and interpret that known driver failure as a callback result.
The pixel proposal remains separate: it repairs proved native full-bitmap
behavior, not the count policy. All three compose cleanly as described above.

Lead should run `ThumbnailReadinessTrackerTest.*` and the existing thumbnail
capture-driver regressions, then rebuild/package with reviewed maintained
patches and repeat the existing required scenario pair with exact provenance.
A no-loss producer trace and callback clock sidecar can then locate remaining
capture/compression/accepted-JPEG differences. Existing CDP tracing cannot
ensure coverage before its first endpoint connection, and no readiness/token
transition is directly traced. Pixel content must be inspected or fingerprinted
before attributing an image to an older page. No acceptance claim is made from
syntax checks or from the existence of these test cases.
