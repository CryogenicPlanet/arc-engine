# Held DevTools integration companion

Unapplied proposal: `arc/browser/webextras_devtools_integration_held.patch`.
It changes only `arc/api/arc_webextras_devtools.mm`. Do not apply during an
active build/package/run slot. No shared source, maintained patch, application,
probe or lock was modified for this companion.

## Prerequisites and base

- The lead must adopt the header/call-site integration in
  `arc/browser/webextras_devtools_hooks.patch`, including the actual focus hook
  in `DevToolsWindow::ActivateWindow` and its reentry guards.
- The lead must adopt the `ArcRuntimeFeatureEnabled(NSUInteger)` declaration
  from `logs/wp/app.pending-integration.patch` into `arc/api/arc_internal.h`.
  The current definition already exists in `arc/api/arc_app_flags.mm`.
- Base production SHA256:
  `cef1ff1c919943659096e7218cd4b056ad3f986bcf39f364e906d7f1c53342c0`.
  This includes the new shared delegate bridge, asynchronous display, weak
  native registration alias and synchronous close routing. Rebase if the
  production initializer, registration helpers or declarations change.

## Production display mapping limits

The base production change captures display mode at receipt: undocked is1;
for a docked callback it uses `DevToolsSettings::GetDockSide` on the actual
frontend Profile and maps none1/left2/right3/bottom4, with enum fallback0.
Chromium's helper defaults missing or unrecognized preferences to right, so
this API does not prove native unknown-text→0 behavior or that preferences
always equal the live dock side. Host `can_dock` preservation remains a shared
integration gap. These cases need actual runtime comparison; this companion
does not add a new preference store or infer a frontend side.

## Exact feature gate

`openProtocolConnection:error:` checks native feature44 before obtaining the
owner WebContents, allocating a protocol client or attaching to a native host.
When disabled it returns nil and, when supplied, sets NSError domain
`company.thebrowser.Arc`, code6, description
`DevTools protocol connections are disabled`.

Original `0x4355e1c..e24` queries44 and branches to the disabled result before
the owner checks beginning `0x4355e28`. The proposal calls the app-owned C++
helper directly: it adds no public Objective-C feature-getter boundary call,
local fallback/default, or duplicate feature store. Other raw send/close
behavior remains unchanged.

## Focus adapter and registration lifetime

The stateless `GetDevToolsFocusHook` callback looks up the service using the
actual inspected WebContents supplied by native `ActivateWindow`. It calls an
`objc_direct` helper that queues notification on Chromium's UI task runner;
there is no app callback inside the native hook. Normal native activation is
left to the shared hook implementation.

Delivery uses the production registration model rather than capturing the
current app delegate at receipt:

- The service owns its current `WebextrasDevToolsDelegateBridge` strongly.
  The bridge's app delegate and service owner are weak.
- A new non-nil registration updates the native weak-registration alias; a
  later non-nil assignment mutates the same bridge's weak delegate.
- Nil drops only the current strong bridge. Already queued tasks retain their
  old bridge strongly. The native weak alias can still accept a new receipt
  while those tasks keep that bridge alive; after its last strong task drains,
  the weak alias expires. Nil followed by a new delegate creates a new bridge
  and updates the alias without redirecting old queued captures.
- Focus copies the selected bridge into the posted task, then loads its weak
  delegate/service at delivery. It permits a nil service argument, matching
  the native Objective-C bridge's lack of an owner-presence branch. No
  frontend identity, navigation generation or closed-state filter is added.

Evidence: `setDelegate:` at `0x4414f60`, existing non-nil weak store
`0x4414f88..f98`, nil pair clear/release `0x4414fb4..4ffc`, new bridge creation
`0x4415028..5074` and native setter call `0x44150a4..50bc`. The managed setter
`0x4332f08` reaches `0x42ede24`, which installs the weak registration pair at
`0x42ede50`; the nil branch does not call it.

Focus producer `0x4359dd0` weak-locks registration, then calls async helper
`0x42d4298` with virtual member slot0x28 (`0x4359e40..e50`). That helper copies
the strong registration pair and increments its control count at
`0x42d437c..4394`, then posts a guarded zero-delay task at `0x42d4478`.
The member resolves through bridge address point `0xda88250` +0x28 to
`0x441610c`, which weak-loads the delegate and service at `0x4416120/612c`
and sends `webContentsDevToolsNeedsFocus:` at `0x4416144`.

The explicit `native_lifetime` argument is the current owner-WebContents
WeakPtr proxy chosen for production. It is not proof of the original native
Impl cancellation lifetime. Ordinary owner destruction cancels delivery;
discard/rebind behavior and independent nativeImpl lifetime remain held gaps.
The native TLS runner is the same observed asynchronous delegate mechanism;
its precise symbolic identity is not independently recovered here.

## Inspect-mode audit: intentionally not wired

No frontend-protocol hook registration is added by this companion. It does
not infer inspect success from `openInspectView` or generate guessed enums.
A fresh bounded disassembly establishes the native string conversion at
`0x4359a74..9c2c`:

| Input string | App value | Evidence |
| --- | --- | --- |
| `searchForNode` | 1 | 13-byte comparison via literal0xd381242;0x4359adc,0x4359c28 |
| `searchForUAShadowDOM` | 2 | 20-byte integer comparisons0x4359af0..9b2c;0x4359c1c |
| `captureAreaScreenshot` | 3 | 21-byte comparisons0x4359b6c..9bb4 |
| `showDistances` | 4 | 13-byte comparisons0x4359bd0..9c14 |
| all other strings, including `none` | 0 | fallback0x4359b30 |

The converted enum is copied at `0x435974c`; producer0x4359c98 invokes
async helper0x4359648 with member slot0x18, whose task post is0x4359830.
Bridge0x441604c retains weak delegate/service at delivery and maps native1..4
unchanged, others0. Accepted devtools launch2 observes value1; other modes
have static conversion proof only. The precise frontend-message trigger,
malformed/missing parameter behavior, target/session routing and callback
conditions still need a bounded call-chain or runtime audit before wiring
`GetDevToolsFrontendMessageHook`. This is an explicit remaining integration
gap, not a success/no-op implementation.

## Validation

The proposal applies cleanly with `git apply --check --whitespace=error`.
Compiler validation stages only the proposed production file and the two
prerequisite headers in a temporary VFS overlay. The actual Chromium macOS
arm64 compiler command is reused; object/dependency output flags are removed.
No shared prerequisite is applied to the working tree. This is syntax-only
validation, not linking, feature-gate runtime coverage or a focus scenario.

Production display and close extraction regressions cover the shared bridge
and queue lifetime rules independently. Their queue/WebContents mocks do not
establish Chromium runner integration, nativeImpl proxy equivalence or hosted
undocked behavior. The companion must remain held until prerequisite adoption
and an isolated comparison run verify the new boundaries.

Final local checks: production and staged companion each exited0 with no
compiler diagnostics. The staged companion used the temporary VFS proof at
`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-devtools-held-v7xvllg3`
(`check.json`, `final-held.json`). Production proof is under
`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-async-display-compile-ich1w5ag`
(`check.json`, `final-production.json`). Display10 and close10 extracted-code
regression groups passed; reverting either receipt or close to the current
strong bridge/current delegate was rejected by a targeted negative control.
No focus feature-gate runtime pass is claimed.
