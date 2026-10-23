Applied by the extensions worker after validation5. Actual compilation is being repeated; runtime remains pending. The proposal evidence below is the preserved pre-application snapshot.

# Unapplied group missing-target preflight correction

`extensions_group_preflight.patch` is based on the current parent-applied dispatch
and read-result corrections. It proposes changes only to
`arc/browser/extensions_groups.mm`; this child has not edited actual product
sources, launched apps, built an engine, or taken a shared lock.

The generic request helper now distinguishes these cases:

| Case | Proposed result |
| --- | --- |
| GetAll has no target before enqueue | Synchronous empty contribution to the existing all-browser aggregation |
| Create/add/update/move/remove has no target before enqueue | Synchronous native error `No browser delegate` |
| A queued request loses its target after enqueue | Existing weak-task cancellation; no fallback response is manufactured |
| An actual GetAll app completion supplies nil NSArray | Empty successful contribution through the existing posted reply task |

No fallback allocates a fake `ArcTabGroupResult` or sends an app callback. Native
mutation preflight results contain `success=false`, the literal error, empty
native group string, and numeric ID `-1`; their API error paths consume only the
failure/error values, so the proposal reports that native error directly.

GetAll aggregation is shared between real replies and preflight fallback. Each
browser decrements the outstanding count exactly once. A missing target cannot
finish the aggregate before other browsers reply. The former nil-array-to-closed
error is removed; the separate source-closed check on actual replies is retained.
All immutable reply snapshots remain in place. Ungroup now sends one request
through the first input tab's browser, after validating every requested tab. Its
copied session-ID array preserves input order and duplicates, matching native.

Synchronous fallback can complete while `Run()` is still executing. The six
hosted API entry points therefore use
`did_respond() ? AlreadyResponded() : RespondLater()`. This follows the current
Chromium debug contract and preserves the empty native ResponseAction instead
of adding an artificial reply task.

## Exact native evidence

Static unslid ARM64 evidence uses the disposable original engine at
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
Bounded disassembly is retained in the temporary evidence directory below.

- GetAll entry `4300ab8` tests its delegate control; missing/expired ownership
  takes `4300f18`, builds an empty vector (`4300f24–28`), and directly invokes
  the caller at `4300f38`.
- Create `4300228–278`, add `4302d58–da8`, update `430302c–307c`, move
  `4303e78–ec8`, and remove `4302454–24a4` construct the same false/error result
  and directly invoke their supplied callback. Literal `d37f393` is exactly
  `No browser delegate` (19 bytes).
- This is also proven after lookup, immediately before the actual mutation:
  add `4309c58–80 → 4309e50–ea0`, update
  `430b1c4–1ec → 430b1f0–240`, and move
  `430c654–67c → 430c800–850` recheck the source/browser/delegate control and
  directly return that same error if absent.
- Once the request was queued, the task instead locks its captured original
  weak control in `42c0894–08bc`. Expiry branches to `42c0968` and skips the app
  payload. The proposed helper's missing-target block is not invoked there.
- Actual app GetAll completion adapter `43e5518` sends NSArray `count` at
  `43e5570`. Nil returns zero and takes `43e5588 → 43e5650`; nil fast enumeration
  returns zero at `43e55e0`. The adapter constructs the empty vector at
  `43e5664–674` and directly invokes its captured callback at `43e5708`. The
  existing native GetAll reply wrapper then supplies its usual posted reply.
- Native API return sites `8089e0c–14`, `4255dd0–d4`, `4256744–48`, and
  `41ce544–54c` construct an empty ResponseAction. There is no visible
  `did_respond` branch at these sites. The official binary cannot establish
  whether the source spelled `AlreadyResponded()` or `RespondLater()`:
  current `extensions/browser/extension_function.cc:674–683` constructs the
  same empty action for both; `AlreadyResponded()` adds a DCHECK.
  `extension_function.h:533–554` explicitly documents the conditional pattern
  for helpers with synchronous completions. The proposal uses it for the
  current debug build, without claiming a native source spelling.

ReadBack's existing optional-miss preflight fallback is unchanged and separately
proven by `4301250–60 → 43016bc–dc`. Permission-hook initial guards and permission
completion behavior are outside this group-only correction.

## One native ungroup request

The proposal replaces per-browser fanout and selector prevalidation with one
remove request through the first input tab's browser, after all inputs pass the
existing tab/profile/editability checks. `Tabs` gains an ordered-output option
used only by Ungroup: it preserves duplicates. Grouping's existing deduplication
behavior is unchanged. The complete ordered array is copied into a regular
strong local before enqueue; no queued block captures a loop reference.

Exact native Run `41ce2c0` validates each tab (`41ce3f4–4cc`) and chooses the first
browser only while its browser variable is null (`41ce43c–458`). It reads each
validated tab's native SessionID (`41ce47c`) and unconditionally appends it
(`41ce49c–4b4`); there is no membership test or deduplication branch. Only after
all inputs are processed does it resolve the hosted bridge (`41ce4d0–4f0`) and
call Remove once at `41ce52c` with the complete vector.

Completion `41ce8a0` returns empty success or the supplied nonempty error; empty
errors use `Failed to ungroup tabs` (`41ce958–9a8`). It does not recheck browser
liveness after the app replied. The proposal preserves that behavior while
retaining the request's weak browser/epoch guard and the weak profile guard in
`Success`. A nil clear after enqueue still cancels without calling fallback.
The earlier ungroup loop-reference UAF correction remains historical evidence;
this single-request path likewise captures only the copied strong ID array.

## Validation and evidence

Evidence is retained at `/tmp/arc-extensions-group-preflight-v1/`: captured
`before/`, proposed `after/`, independently applied copies, exact stripped Base
compiler argv, VFS mappings for both real repository and `chromium/src/arc`
symlink aliases, compiler logs, deliberate negative-overlay logs, source hashes,
and `native-preflight.txt`.

The proposed groups TU passes its exact Base Clang command through the VFS
overlay: exit 0, no diagnostics. A deliberate source-overlay sentinel returns
exit 1 with its unique expected error, proving routing through both path aliases.
Strict patch apply/whitespace checks pass; applied files match the proposed copies
byte-for-byte, and all captured current product hashes remain unchanged by this
child. Two fresh independent reviews report no introduced defects or native
overclaims, including a dedicated capture/lifetime review.

Runtime
preflight timing, nil-completion behavior, cancellation, and paired recorder
parity remain **unexecuted**. No scenario is claimed diff-clean by this proposal.

Base groups SHA-256:
`ab585069543a58c0e81e09a9b3763bb560352e8f56ffcd7981431dedd6e17f65`.
