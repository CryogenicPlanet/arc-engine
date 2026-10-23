# Allowed boundary differences (ours vs Arc's engine)

Each entry: what differs in `tests/recorder/diff.py` output, and why it is not a behavior difference for the
app. Only the lead adds entries; packages request them in their reports.

| Difference | Reason |
|---|---|
| Engine-internal constructors taking a native pointer (`initWith…:` with `"<ptr>"` args) are absent | Arc's engine builds wrappers from its C++ objects; ours builds them its own way. The diff already reports these as `internal`. |
| `-[ArcBrowser context]` (and other getters returning ArcBrowserContext) returns the same object each call; Arc's returns a fresh wrapper | We keep one wrapper per profile (ARCHITECTURE.md, Identity). `isEqual:`/`hash` compare the profile ID, so app comparisons are unchanged, and per-profile service objects stay consistent. Compare these returns by `-id`, not by object number. |
| Download items, web contents and other per-object wrappers: one canonical wrapper per native object; Arc's engine sometimes returns a fresh wrapper per event | Same identity rule as profiles (ARCHITECTURE.md). Compare by the object's stable identity (download GUID/id, session id), not by object number. |
| Development builds hit DCHECKs that Arc's release engine never ran (e.g. history expiry); scenarios that need those paths are judged on the release build (`out/Release`) | Arc shipped release builds; DCHECK-only aborts are not app-visible behavior. Host-only DCHECK tolerances go in patch 0006 when a dev-build run is needed. |
| `nativeView` / pane host returns: ours is Chrome's `WebContentsViewCocoa` (direct native view, ARCHITECTURE.md); Arc's is its own `WebContentsHostView` | The app uses it as an `NSView` in its hierarchy. Compare these returns as "an NSView", not by class tag. Revisit only if a scenario shows a hosting behavior difference (focus, resize, attach/detach). |
| PiP controller and DevTools frontend wrappers: canonical on ours, fresh on Arc's | Covered by the canonical-identity rule; compare by the underlying object (same web contents / same PiP session). |
| Volatile values normalized by the comparator: positive session IDs (bijective renumbering per run), epoch/date values, Google favicon CDN shard and cache-age headers, timed media getters (`ArcMediaPosition.currentTime` etc. within the elapsed-time bound) | Values that legitimately change per run or per server response. Normalization must be typed and exact-field (no broad regexes), keep malformed values different, and keep the raw transcripts. |

| `ArcWebContents.inputEvent` node ids (`{"input":{"id":N,…}}`, `{"click":{"id":N}}`) differ in value | N is the target's Blink DOM node id, drawn from one per-renderer-process counter that other renderer features (paint timing, layout shift, inspector, autofill) also draw from, so absolute values depend on which features ran first. The app can only rely on one stable id per element within a page, which holds on both engines. Compare the event kind, value and id equality pattern within a page, not the numbers. |
