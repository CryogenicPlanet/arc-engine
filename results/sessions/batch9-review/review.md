# Batch9 independent review

No new fixture P1/P2 found. Fresh build/package and all12 before/after hash gates pass. Default discard passes; the single-flag diagnostic repeats its missing-finalize failure with fresh disposable cache and profile.

Both runs have exactly one cache-isolation marker, profile-isolation marker, profile RESTORED marker and CACHE_RESTORED-original marker, with no refusal marker. Recorded cache/profile backup paths are now absent; original paths are real directories. New wrappers' unchanged source hashes establish that all-main/helper guards and original-inode checks ran before success markers. No independent pre-run inode sidecar was recorded. No ours156 main/helper remains at review.

Default: publicID204560291 remains initialized on the same wrapper; finalize returns YES once asynchronously on main. Exact A/B/C URLs/titles and B selected survive discard and reload.

Diagnostic: --disable-field-trial-config is supplied exactly once; selected wrapper becomes uninitialized during discard, genuine close callbacks occur inside the request, and complete raw transcript has zero finalize callbacks. Sidecar times out with actualDiscarded=null; history/reload checks are not reached. Both cleanups acknowledge exact PID/nonce AppKit shutdown and parent apps exit without TERM.

Neither app log has the batch8 wrong-cache-format errors. This controls a disclosed cache confound; effective feature/backend state, native pointer identity, causal mechanism and parity remain unproven. See review.json for exact raw request/callback pairs, hashes and recorded backup paths.
