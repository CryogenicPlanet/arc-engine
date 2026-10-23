# Shared cosmetic/debug cache and debug-agent selection

Bounded read-only audit, 2026-10-23, of the pristine arm64 ArcCore image used by
`renderer_contract_evidence.py`. No runtime launches, debugger attachment,
builds, locks, or production changes. This file records fresh verification of
the specific Success-versus-Cached question.

## Finding

**Native frame cosmetic application and native debug use the same caching
evaluator on their process's AdBlockerAgent.** There is no demonstrated
non-caching frame path, separate debug cache, or cache-mode parameter. The
replacement's shared evaluator/cache is correct on this point.

The remaining observed Success-versus-Cached difference cannot justify changing
cache semantics or normalizing those values. Native debug chooses the oldest
surviving registered process agent; current unapplied early-startup hooks mean
ours can have a different usable set of process agents. Actual selected process
identities/frame ownership were not recorded by this read-only audit.

## Only two callers enter the same evaluator

Scanning all direct arm64 B/BL references to **0x43a51f4** finds exactly:

```text
0x43a4ef4 BL -> 0x43a51f4   frame cosmetic application
0x43a714c BL -> 0x43a51f4   debug request
```

Both call sites supply the same argument shape:

| Register | Frame caller | Debug caller |
|---|---|---|
| x0 | retained process agent (x20) | retained process agent (x20) |
| x1 | document origin aggregate | origin parsed from supplied debug URL |
| x2 | frame document GURL | GURL parsed from supplied debug URL |
| x3 | writable CSS output string | writable CSS output string |
| x8 | result aggregate return storage | result aggregate return storage |

Frame setup is `0x43a4ee0-0x43a4ef4`; debug setup is
`0x43a7138-0x43a714c`. There is no extra boolean or invocation-mode parameter.
Evaluator prologue saves x0 as its agent (x28), x1 as origin, x2 as URL, and x3
as CSS output (`0x43a5220-0x43a522c`). It does not distinguish the callers.

Frame **0x43a4bdc** has an early return when its WebLocalFrame is absent and
skips evaluation when the extracted document host length is not positive
(`0x43a4eb8-0x43a4ec0`). A live matching HTTP fixture frame reaches the common
evaluator. Returned state0/1 inserts CSS through the shared user-sheet helper
at `0x43a501c`; states above1 remove the `blocklist` sheet.

## Common cache lookup, insertion, and hit result

Inside the evaluator:

```asm
0x43a543c add x0, x28, #152    ; this agent's cache object
0x43a5440 add x1, sp, #224     ; host key
0x43a5444 bl 0x836fc70         ; cache lookup
0x43a5448 cmp x27, x0         ; compare end sentinel (agent+184)
0x43a544c b.ne ...            ; cache-hit branch
```

Successful nonempty selector assembly uses the same agent cache:

```asm
0x43a5df4 cbz x23, ...        ; no selector -> do not insert
0x43a5df8 add x0, sp, #288
0x43a5dfc add x1, sp, #152     ; host
0x43a5e00 bl 0x30d5b40        ; build cache entry from host + CSS output
0x43a5e04 add x0, x28, #152   ; SAME cache object
0x43a5e08 add x1, sp, #288
0x43a5e0c bl 0x836fe14        ; insert/update
```

This insertion path is inside the common function, without a frame/debug
caller predicate. Cache-hit code at `0x43a64d4-0x43a655c` copies cached CSS to
the provided CSS output and writes result state1 with the full requested host
string. Fresh nonempty assembly writes state0/Success. Empty results do not
populate the cache.

Cosmetic-rules receipt clears the cache before enumerating frames
(`0x43a6c2c-0x43a6c88`). Exclusion receipt does the same
(`0x43a6f58-0x43a6fb4`). Their frame visitor **0x43a7778** calls
**0x43a4bdc** at `0x43a7788`. Native document-start callback **0x43a7fb4**
applies Boosts first, then tailcalls the same frame cosmetic helper at
`0x43a7ff8`.

Consequently a received update followed by successful cosmetic application in
process P warms P's cache for that host before a later debug query to P.
Conversely, clearing P's cache with no matching current frame in P leaves that
host uncached, even if process Q visibly reapplies the same CSS to its page.
The first later debug query to P can then return Success with identical CSS.

## Debug routing is process-registration order, not page ownership

Browser profile service debug method **0x47314c4**:

```asm
0x47314dc ldr x8, [x0, #152]  ; bound-agent map size
0x47314e0 cbz x8, ...         ; No agent is bound to profile
0x47314e8 ldr x8, [x0, #136]  ; ordered map's leftmost node
0x47314ec ldr x21, [x8, #56]  ; selected Mojo proxy
...
0x4731560 b 0x441f82c         ; send debug URL to that agent
```

There is no lookup by debug URL, current tab, fixture frame, active page,
or browser-local evaluator. Registration **0x4731028** assigns an increasing
counter from service+128 at `0x4731164-0x473116c`, stores that old counter in
map-node+32 at `0x4731258`, and its proxy at node+56 at `0x4731264`.
Disconnect removes the corresponding map entry through `0x4731720`, including
map-size decrement and proxy destruction at `0x473181c-0x473185c`.

The registration call is native **RenderProcessWillLaunch**, before process
creation, as separately established at `0x42ac400`. No frame or Ping readiness
acknowledgment determines the native map key.

## Current integration confound and evidentiary limit

As read in this audit, actual Chromium source lacks the proposed calls to
`EnsureRenderProcessAgent` from `ChromeContentRendererClient::RenderThreadStarted`
and to `arc::RendererProcessWillLaunch` from the browser's corresponding hook.
Those additions exist in the pending proposal, rather than current applied
source. Our process agent instead installs from `CreateRenderFrameAgent`.
Browser fallback binding waits for a frame's `SetRendererHost` acknowledgment
(`arc/api/arc_renderer_web_contents.mm:128-132`).

Our registry correctly reserves launch order and selects the smallest
`bound_sequence`, but RequestDebug considers only bound/connected entries
(`arc/api/arc_renderer_process.mm:127-143`). A process without a frame handshake
therefore cannot be selected in that fallback. Native can register/select a
process without a matching fixture frame. App/core process creation and
navigation lifetimes can further change which process remains the oldest.

This is a concrete difference in eligible destination sets, **not proof of the
actual destination in the paired run**. The long-run observations provide no
selected-agent PID/registration key/frame-ownership mapping. Initial debug
readiness polls themselves can warm an otherwise frame-less agent's cache, so
initial Cached followed by post-update Success is consistent with this model.
It is not proof of a non-caching frame evaluator.

No cache implementation change is justified by this audit. To establish a
specific remaining bug, a later authorized comparison must identify selected
native/ours debug agents and the fixture-owning process, then compare message
receipt/frame application/query order for the same host and process. No such
runtime work was performed here.

Reproduction:

```sh
python3 arc/common/renderer_contract_evidence.py --calls-to 0x43a51f4
python3 arc/common/renderer_contract_evidence.py 0x43a4ee0:0x18 0x43a7138:0x18 0x43a543c:0x14 0x43a5dec:0x24 0x43a64d4:0x8c
python3 arc/common/renderer_contract_evidence.py 0x43a7778:0x20 0x43a7fb4:0x48
python3 arc/common/renderer_contract_evidence.py 0x47314c4:0xa0 0x4731160:0x10 0x4731258:0x44
```
