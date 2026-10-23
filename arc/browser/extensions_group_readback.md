# Applied group read-result correction; runtime pending

The parent extensions worker applied this reviewed patch after validation5.
Actual compiler checks are being repeated before the next runtime batch. The
temporary pre-application evidence and dependency description remain below.

`extensions_group_readback.patch` applies **after** the refreshed
`extensions_delegate_dispatch.patch`, including the parent's ungroup capture fix.
It changes only `arc/browser/extensions_groups.mm`. The temporary base was built
by applying that dependency to the preserved pre-dispatch files and verified
byte-for-byte against the parent-applied dispatch result. This child made no
actual product source edits, app launches, builds, or shared-lock operations.

Public `tabGroups.get` now serializes the matching GetAll result directly, after
all allowed browsers reply. It matches the public numeric ID even when the app's
native string group ID is empty. The additional direct `getTabGroupWithId` request
is removed. No eligible hosted browsers retain the existing Chromium numeric-ID
error; a completed lookup with no match returns the native error exactly:
`No group with specified id`.

Successful update and move always request GetAll/local lookup from the captured
**original source browser**, using the **original requested numeric group ID**.
Neither `has_callback()` nor `result.groupId` controls this readback. Move's app
mutation routing remains unchanged, including its destination-delegate caveat;
this correction concerns the separately proven source-browser result path.

App completion values are copied into immutable C++ snapshots before any reply
task is posted, matching the native ObjC adapters. Numeric IDs, copied strings,
color and collapsed state can no longer change if the app reuses a DTO after
invoking completion. Mutation success/error values are likewise captured before
posting; no extra public Arc value objects are allocated.

Native result behavior differs by operation and is preserved:

| Condition after successful mutation | Update | Move |
| --- | --- | --- |
| Lookup finds the numeric ID | Serialize found info with original window ID | Same |
| Lookup misses, or source expired before readback starts | Default object described below | Successful empty argument list |

The update default is `{id: originalID, windowId: originalWindowID,
collapsed: false, color: "", shared: false}`, with `title` omitted. It is generated
using the current Chromium `api::tab_groups::TabGroup` constructor and `ToValue()`;
its `Color::kNone` deliberately serializes to the empty string. Regular found-info
serialization retains the existing grey fallback for invalid native colors and
now reads the color scalar once before validating its enum range.

Mutation failures preserve an app-provided nonempty error. Empty or absent errors
use the native strings `Failed to update tab group` or `Failed to move tab group`,
without punctuation. Failure is handled before the source-liveness fallback.

## Native evidence and timing

Addresses are unslid ARM64 static evidence from the disposable original engine
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
No runtime result is claimed.

- Public Get's hosted-browser filter at `4259840` checks root-profile
  `IsSameOrParent` (`4259874–88`) and, without incognito permission, exact profile
  identity (`425988c–b8`). It requires the hosted Arc bridge (`42598c0–dc`) and
  captures weak browser/window ID (`42598f8–9920`). The proposal keeps the current
  `Allowed` profile/incognito and extension-window visibility checks.
- Run `8089b00` skips expired weak browsers (`8089f54–fa8`) and calls `4301230`
  at `808a034`. That helper requests GetAll and matches only numeric ID
  (`4308d58–6c`), then posts its optional-info reply (`4308f10–24`). It does not
  invoke the direct-get adapter at slot `90`.
- Get's per-browser callback `4259b90` stores a matching value and decrements the
  outstanding count (`4259c1c–70`). Aggregate `4256cd8` waits for zero
  (`4256d34–38`), then serializes found info (`4256f38–fc8`) or emits the exact
  missing-group error (`4256ebc–6f20`, string `d35a83c`). With no eligible hosted
  browsers the native Run uses the Chromium numeric-ID fallback (`808a114`,
  format string `c9a8ec0`).
- Update calls `4302fe4` at `4257850`, with completion `4257ec0`. On success it
  tests the original source weak browser (`4257ef0–f20`) and calls `4301230` at
  `4257f8c`, with original numeric ID (`4257f28`). If the source expired it
  serializes the default ID/window object (`4257fd0–8018`). Readback callback
  `808a848` has the same optional-miss default (`808a868–8b4`).
- Move calls `43038dc` at `4258964`, with completion `4258fac`. On success it
  checks the original source (`4258fdc–9088`) then calls `4301230` at `42590ec`
  with original numeric ID (`4259090`). Missing source uses empty success
  (`4258fec–9004`); optional-miss callback `4259338` does the same
  (`4259358–78`). Helper `2fe2e10:2e44–58` sets an engaged empty result list and
  success, rather than an error.
- Update and move result callbacks do not recheck source liveness after lookup.
  They use the captured original window ID. The proposal similarly caches the
  window ID before enqueue and removes browser dereferences from serialization.
  Browser closure after the lookup cannot pass null to `GetWindowId`.
- The original group's request/reply task boundaries remain: posted mutation
  dispatch → app callback → posted mutation reply → posted GetAll dispatch → app
  callback → posted lookup reply → serialization. Public Get has one posted
  GetAll request/reply per browser, then serialization in the final reply task.
  No new serialization-only task is introduced.

Native generated serializer `beab7ac:800–828` indexes the color table only after
checking enum `<10`; table `e1e8f40[0]` points to the empty string. Current generated
`chromium/src/out/Base/gen/chrome/common/extensions/api/tab_groups.cc:101` sets
false/zero defaults; `ToString(Color::kNone)` returns `""`, and `ToValue()` omits an
absent title. This avoids passing an invalid color through a CHECK/NOTREACHED path.

## Missing target versus a later delegate clear

A missing target at readback enqueue is a proven optional-miss path, distinct
from cancellation of an already queued request. Native `4301230` checks its
current delegate control at `4301250–60`; absent/expired ownership branches to
`43016bc`, constructs a disengaged optional (`43016c8–cc`), and directly invokes
its callback at `43016dc`, unless that callback is already cancelled. The
proposal's ReadBack enqueue-false update-default/move-empty branch preserves
this result. The exact bounded instructions are saved as
`native-nil-targets.txt` beside the other evidence.

After a request was successfully queued, a later nil clear instead expires the
captured original weak bridge; `42c0840` skips its payload. No readback fallback
is invoked for that dropped queued task, and pending-request retention remains.

A separate existing dispatch-helper limitation remains outside this correction:
generic `Request` responds with the unavailable error when enqueue returns false.
Native GetAll instead immediately supplies an empty vector (`4300f18–38`), and
native create supplies a failure result with `No browser delegate`
(`4300228–278`, literal `d37f393`). Their operation-specific enqueue-false results
and synchronous fallback timing need a separate correction; this proposal does
not claim that generic helper path is native-clean.

## Lifetime and validation

The refreshed dependency copies `batch.ids` into a regular strong `session_ids`
local before queued ungroup dispatch. ObjC blocks capture reference-typed loop
locals by reference, so the old `batch.ids` capture was unsafe. This proposal adds
no escaping loop references: lookup records are consumed synchronously; queued
blocks capture regular source, integer window/ID, enum, and owning callback locals.
Pending-request retention, duplicate completion guards, weak profile scope,
request epoch invalidation, and existing permission behavior are preserved.

Evidence is in `/tmp/arc-extensions-group-readback-v1/`: dependency-applied
`before/`, combined `after/`, independently applied copies, exact Base compiler
argv with output/dependency flags removed, compiler logs, both-path VFS overlays,
negative sentinel logs, source hashes, and `native-readback.txt`.

The final groups TU passes its exact Base compiler command with the combined
VFS overlay: exit 0, no diagnostics. The deliberate source-overlay sentinel
returns exit 1 with the expected unique error. Strict sequential apply and
whitespace checks pass for both patches; applied copies match the proposed
sources byte-for-byte, and current product hashes remain unchanged by this
child. Two final independent native/lifetime reviews report no introduced findings.
They explicitly checked all escaping ObjC block captures and immutable payloads. Runtime callback order, fallback
values, and paired recorder parity remain **unexecuted**. No scenario is claimed
diff-clean by this proposal.
