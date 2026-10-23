# Completed current batch: independent evidence review

No outstanding P1/P2 fixture or evidence issue found. The previously identified filtered-return presentation gap was corrected by the parent and independently rechecked: selected events now include paired callback returns, explicit `finalizeReturn` equals raw6385, and scope states the selected-WC-plus-callback-return filter. No raw/runtime evidence changed. This review retains the raw rows in [verified-boundary.json](verified-boundary.json).

The unchanged canonical original close is synchronous: **call6136 → callback6138 → return6489**. Replacement is asynchronous: **call6380 → return6381 → callback6428** (callback return6773). Each callback references the exact closed WC, occurs once, and runs on main. The replacement scenario aborts before restore; its missing restore rows are a coverage cutoff, not independent proof of a restore failure.

The supplemental recorded original discard confirms **discard6236 → PiP-controller-destroyed6237 → return6279 → favicon6304 → traits6310 → state(1)6316 → finalize(true)6384 → callback return6385**, all main. Same selected recorder identity `ArcWebContents#137` and native probe pointer are retained, while public live session ID changes **2138339481 → 2138339482**. Both post-discard and post-reload histories equal the original exact A/B/C URLs and titles, current index1 (B). Cleanup reports graceful exit, no TERM or cleanup error.

Cached media wrapper `#147` returns before discard at3670 and after at6361. Navigation wrapper `#148` returns at3676 and later7591. Those observations establish getter wrapper identity only. Later navigation timing is not attributed precisely to discard/reload by raw sequence. There is no selected-wrapper PDF/thumbnail getter coverage and no service-absence claim.

The probe wraps the actual finalize implementation and forwards once through the standard recorder; the raw callback caller image is explicitly `arccore-sessions-discard-probe.dylib`, not native engine. The original discard result is **supplemental**, not a canonical golden or paired service parity result. Frozen input hashes and original framework before/after hashes match; full raw rows and checks are in [review.json](review.json).

No apps, builds, network, credentials, production files, or other evidence files were changed during this review. Only this directory was written.
