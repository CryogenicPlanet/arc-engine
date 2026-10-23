# Browser delegate request dispatch proposal (parent applied)

`extensions_delegate_dispatch.patch` is a repository-root patch against the current
frozen sources. The parent applied it after validation5 and corrected the queued ungroup capture.
This child edited only proposal artifacts. No build, app launch,
shared lock, runtime helper, fixture, or recorder result was changed for this work.

The proposed central `DispatchBrowserExtensionRequest` helper posts one UI task,
using the existing `BrowserExtensions` weak pointer and the callback epoch captured
at enqueue. It resolves the current weak ObjC delegate at delivery. Nonnil delegate
replacement keeps the epoch; nil clear advances it. Closed bridges and requests
queued before nil clear are dropped. A false enqueue result means the existing
bridge/callback target is absent; group functions defer their existing unavailable
error, and the permission hook can decline as before.

The patch uses this helper for Gather, create, add, remove, update, and move.
New-group creation loses its former outer task because the helper supplies that
same request boundary. Group identifiers are copied before enqueue so the added
hop cannot observe a later mutation of app-owned lookup values. Group replies keep their existing separate `OnUI` task,
completion guards, profile/browser checks, and pending request retention until
response or browser-context shutdown. The permission snapshot is materialized at
the event before enqueue; its once-only completion invokes the native callback
directly, with no reply task or delegate epoch cancellation after dispatch.

Move keeps its existing destination selection and its explicit runtime-evidence
caveat. This proposal does not establish cross-window destination routing.

## Exact native evidence

Read-only ARM64 inspection used the disposable original engine at
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`,
via `/tmp/arc-profile-disasm.py` with the existing `/tmp/arc-ui-capstone` library.
Retained source anchors independently identify these request templates in
`research/2026-10-22-sol/evidence/agents/matcher/decompile/closure-anchors.tsv:144–152`.
Addresses below are unslid native addresses, not runtime observations.

| Request | Proven separate request post | Actual app adapter/send |
| --- | --- | --- |
| Permission | `42ff770` posts at `42ffc54`, payload `4307c2c` | `43e3a08` / `43e3ad0` |
| Create | `43001f0` posts at `43009d0`, payload `4307f30` | slot `68`, `43e3b50` / `43e3cac` |
| Get all | `4300ab8` posts at `4300d8c`, payload `4308530` | slot `98`, `43e4b7c` / `43e4c30` |
| Remove | `4301e48` posts at `43021c4`, payload `430936c` | slot `78`, `43e4184` / `43e42b8` |
| Add after lookup | lookup reply `4309bcc` separately posts at `430a2e4`, payload `430a630` | slot `70`, `43e3e68` / `43e3fc8` |
| Update after lookup | lookup reply `430b100` separately posts at `430b9e4`, payload `430bad0` | slot `80`, `43e446c` / `43e457c` |
| Move after lookup | lookup reply `430c5cc` separately posts at `430cbfc`, payload `430cf00` | slot `88`, `43e4734` / `43e4828` |

Every listed request installs `42c1740 → 42c0840` before posting. The wrapper's
source marker is `delegate_wrapper.h:118`. The outer task captures the original
bridge pointer and weak control block: create `4300984–988`, get-all `4300d40–44`.
`42c0894–08bc` locks that exact control; an expired control branches to `42c0968`
and skips app invocation. It does not re-fetch a later browser delegate bridge.

Native `ArcBrowser::setDelegate:` at `43e1284` drops its shared bridge ownership on
nil, updates the existing ObjC weak delegate on nonnil replacement, and allocates
a fresh bridge after nil. Retained exact body:
`research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/043e1284.txt:20–66`.
The proposed epoch implements invalidation of requests still queued on the UI
thread. This is not a claim that native nil clear cancels a request whose weak
lock already succeeded: native can retain the old bridge for that active call.
The proposal likewise performs no epoch checks after beginning app dispatch.

Create's app completion adapter `43e4e5c` invokes callback `430805c`, which posts
reply task `4308140` at `4308128`; the callback runs at `430818c`. Get-all's reply
adapter `4308654` posts at `4308780`, then runs its response in `43087c0`.
Permission differs: ObjC completion `43e4e50` tail-calls captured callback
`42deb64`, whose done callback is invoked at `42debc8` without another task post.
The proposal preserves that difference rather than making every reply async.

## Deliberate bounded exclusion

`ReadGroup` is unchanged. The native direct-get ObjC adapter `43e49d8` and slot
`90` exist, but its request scheduling has not been established. Native public
get path `4301230` actually posts GetAll, selects the ID locally in `4308d1c`, and
posts the optional-info response through `4308f2c` (`4308f10–24`). It supplies no
proof for the extra direct-get request currently made by our `ReadGroup`.
Post-mutation readback behavior requires a separate bounded audit/correction;
this dispatch-only patch makes no assumption about it.

## Validation and limits

Evidence is retained separately at `/tmp/arc-extensions-delegate-dispatch-v1/`:
`before/`, `after/`, `applied/`, source hashes, `overlay.json`, each TU's exact
stripped argv and compiler stdout/stderr/result, `applycheck.json`, and bounded
native disassembly in `native-schedules.txt`. The first overlay attempt lacked
Chromium symlink aliases and is preserved under `incorrect-overlay-checks/`; its
compiler results are invalid for proposal validation. Current `overlay.json` maps
both repository paths and `chromium/src/arc` aliases with external names disabled.
The final successful commands and header sentinel use this corrected overlay.

- `git apply --check` succeeds in the temporary source tree; applying the patch
  there produces byte-identical proposed copies for all three paths.
- Proposed `arc/api/arc_extensions_browser.mm`, proposed
  `arc/browser/extensions_groups.mm`, and header consumer
  `arc/api/arc_browser.mm` each pass their own exact real Base Clang
  command with `-fsyntax-only`: exit 0, no diagnostics. Output/dependency flags
  are stripped. All three commands consume the same proposed header VFS overlay.
- Product source hashes are checked against the captured starting copies.
- Strict patch whitespace/apply checks pass. Repository-wide `git diff --check`
  reports pre-existing trailing whitespace in foreign core-navigation and
  webextras proposal patches; those files were not edited.
- A deliberate temporary header `#error` fails the core browser consumer with the
  expected unique sentinel, proving that command reads the proposed header VFS.
- Fresh independent proposal review fixed the mutable group-ID capture risk.
  Parent review subsequently caught an escaping ObjC loop-reference capture in
  queued ungroup: the block must capture a copied regular `session_ids` local,
  never `batch.ids` through the reference-typed loop variable. The refreshed
  patch includes the parent correction; all escaping captures were re-audited.

Runtime timing, clear/replacement behavior, and original/current recorder parity
remain **unexecuted** for this proposal. Existing scenario differences are not
accepted as clean by these static checks. The parent applied this patch after the frozen validation batch. The appropriate
real paired scenario must still verify its behavior.

Original pre-dispatch source SHA-256 (historical v1 base):

| Path | SHA-256 |
| --- | --- |
| `arc/browser/extensions_bridge.h` | `ca3958062677e582bdf98798f61bbf95011fc9c50c2fde6e2f777efcaaac87e5` |
| `arc/api/arc_extensions_browser.mm` | `b3aabc4d7fd31bc0fb92c72632752205574f1f16efbb8dd621329a3696950b58` |
| `arc/browser/extensions_groups.mm` | `6e52d7060590040cc24a70a9fc2d8f099a17c19a1cf5848944967c20a0ed494b` |

## Refreshed capture correction

The parent-applied ungroup correction is included in the adjacent patch. Original
v1 evidence remains unchanged; refreshed temporary copies and exact groups-TU
compiler evidence are under `/tmp/arc-extensions-delegate-dispatch-v2-ungroup-capture/`.
The refreshed staged result is also the explicit base dependency for the separate
read-result proposal. Runtime parity is still unexecuted for this artifact.

## Remaining pre-enqueue fallback distinction

Subsequent native audit distinguishes an already queued request cancelled by nil
clear from a target absent before enqueue. The former skips its weak task. The
latter has operation-specific native results: GetAll supplies an empty vector
(`4300f18–38`); create supplies failure `No browser delegate` (`4300228–278`).
The current generic Request helper's unavailable-error fallback is not exact for
those paths. This remains a separate correction; the dispatch artifact's static
validation does not establish native parity there. Native readback helper
`4301230:43016bc–dc` immediately supplies a missing optional on pre-enqueue target
absence; the separate read-result proposal preserves that proven fallback.
