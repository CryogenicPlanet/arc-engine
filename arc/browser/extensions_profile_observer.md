# Early regular-profile extension observer proposal (UNAPPLIED)

Review-ready request only. Product/shared/helper sources, GN files, runtime
artifacts, apps and profiles were not changed. The main proposal is
[`extensions_profile_observer.patch`](extensions_profile_observer.patch), SHA256
`4223beeeda7b61d7f15d87a0b0867e5828a15671996d2a113dd7b65fc3060535`.
All copied sources, exact commands, compiler results, sentinels, composition
inputs and hashes are in `logs/wp/extensions-profile-observer-proposal/v1/`.
No engine build/package, app/scenario run, lock or board action was performed.
The only executed programs were explicitly requested, isolated link-and-call
probes that contain an empty/default installer and a counter; they load no Arc
app, Chromium framework, profiles or delegate code.

## Concrete integration requested from lead

The five-file patch supplies these focused changes:

| File | Proposed change |
|---|---|
| `chrome/browser/arcmod/arcmod_hooks.h` | Empty-by-default `RepeatingCallback<void(Profile*)>` with inline NoDestructor storage: `GetExtensionProfileWillInitializeHook`. |
| `chrome/browser/extensions/chrome_extension_system.cc` | Synchronously deliver the bound hook after `LoadErrorReporter::Init`, before content-verifier/service-worker/user-script/ExtensionService construction, gated on `arcmod-host-app`. |
| `arc/api/arc_internal.h` | Lead-owned cross-package declaration `arc::InstallExtensionProfileInitializationHook()`. |
| `arc/api/arc_browser_application.mm` | Standing weak no-op for package-omitted builds and installer call immediately before `ChromeMainInitializeHostedBrowser`. |
| `arc/api/arc_extensions_context.mm` | Strong installer plus profile-owned persistent regular-profile observer; facade binds only its weak callback target and shares pin suppression. |

The Chromium portions are requests for a maintained lead-owned patch (the
existing `patches/0004-arcmod-hooks.patch` mechanism), not permission to edit the
Chromium checkout directly. No GN change is necessary. The installer merely
stores a callback before ChromeMain; it does not read CommandLine, request a
UI task runner, touch a profile/facade or invoke app code before Chrome globals
exist. Chrome's delivery site performs the host-switch check. Ordinary Chrome
retains its empty-hook path and no direct `//arc` dependency.

For a standard original regular macOS profile, the hook runs after eager keyed
services and reporter initialization but before extension loads. The existing
ToolbarActionsModel has queued its OnReady work without attaching its registry
observer. No nested loop or initialization reentry is introduced, so Arc's
registry observer attaches before that deferred toolbar observer. This ordering
is a source-backed proposal, not an engine-runtime observation.

## State, delivery and shutdown

Profile user data owns the native observer independently of delegates. This is
an implementation adaptation, not proof of native one-observer-per-Profile
ownership. Publication precedes observer attachment; explicit unattached,
attaching, attached and shutdown phases make reentrant ensure calls reuse the
same state. The facade retains only a weak native reference. Profile and ObjC
context/delegate references remain weak.

Each actual lifecycle/error/pin event copies the current weak delivery target
when it queues its existing snapshot payload. Events with no target are not
buffered or replayed to a future target. Nonnil delegate replacement updates
the existing bridge. Nil unbinds the bridge and explicitly clears its delegate
before releasing it, including when an executing callback still holds that
bridge during app reentry. Native observation remains attached for regular
profiles. An old facade cannot unbind a newer facade's target. The observer and
facade use the same shared suppression object during setPinnedExtensionOrder;
the pin algorithm itself is unchanged by this proposal.

An independent delivery token is weakly captured by queued callbacks. Shutdown
revokes it before detaching dependencies, even if the facade and its callback
bridge remain externally retained. ProfileObserver::OnProfileWillBeDestroyed
fires while keyed services are valid, before their two-phase teardown; registry,
error-console and toolbar shutdown notifications also revoke. Detach alone is
not relied on. Shutdown invalidates ready tasks and cannot be resurrected by
later bind/ensure calls. Both the ErrorConsole creation path and the ensure
helper check a preexisting weak reference before reading state after potential
reentrancy/deletion.

## Noncreating lookup proof and scope

`GetServiceForBrowserContext(..., false)` is protected here; calling it from the
owned facade would be invalid. The proposal uses the public virtual
`KeyedServiceBaseFactory::IsServiceCreated` instead. Its implementation is only
`mapping_.find(context)` and a nonnull service check
(`keyed_service_templated_factory.cc:274–278`). It neither creates a mapping nor
constructs a service. Once this gate passes, typed Get runs synchronously on
the UI sequence with no intervening user code.

The mapping equality is bounded to admitted original regular macOS profiles:

- `Profile::IsRegularProfile` is the kRegular metrics type (`profile.cc:417–420`),
  mutually exclusive with guest/system/incognito/isolated types (`profile.h:394–404`).
  Ensure also explicitly rejects IsOffTheRecord and ShutdownStarted.
- `ProfileImpl::GetOriginalProfile()` returns this (`profile_impl.cc:1120–1125`).
- Registry's factory redirects through ChromeExtensionsBrowserClient to the
  original profile (`extension_registry_factory.cc:45–50`,
  `chrome_extensions_browser_client.cc:340–349`). This yields the same key.
- Toolbar and system factory kOwnInstance selection returns the input profile
  (`toolbar_actions_model_factory.cc:35`, `chrome_extension_system_factory.cc:141–153`,
  `profile_selections.cc:119–133`). Both eager factory services therefore use
  that same key. No new factory API or dependency-graph mutation is proposed.

Missing or testing-null registry/system/toolbar leaves state unattached; it does
not force-create eager dependencies and claims no early ordering. A later facade
initialization/delegate installation retries attachment. The global reporter
is touched only at the selected hook or after actual system readiness proves
its initialization. ErrorConsole's reviewed lazy constructor may use existing
registry/prefs; a null console also leaves attachment retryable. This does not
establish behavior for every custom testing graph or recover events missed
before a successful late attachment. There is no extension_service() access,
internal-extension load, Chrome Init recursion, readiness Signal, synthetic
callback, observer-list reorder, sorting/coalescing or delayed payload repair
inside ensure/attachment. Existing facade-owned internal extension loading is
left in its existing path.

OTR, guest and system contexts retain the delegate-owned late observer path.
The hook does not create any new subscription for them or infer fanout from
redirected registry/error/system services. Nil still destroys their observer.
The shared shutdown-token safety applies, but this is not a recovery of native
OTR ownership, callback routing or early toolbar insertion.

Global system-ready ordering remains separate: the retained ready().Post task
captures the target when its Ready method runs, not necessarily when Signal
occurs. Toolbar's system-ready task was registered earlier. A delegate attached
between Signal and Ready could receive that notification. Native evidence must
resolve that queue boundary; this proposal makes no readiness-parity claim and
adds no callback merely upon delegate binding.

## App pending integration composition

The app proposal input is `logs/wp/app.pending-integration.patch`, SHA256
`e50fcdf625ee3f1a64192916f55501727155b3c186128791d4b8ff143d01e5f9`. Do not apply two overlapping bootstrap patches and
silently discard either side. The explicit alternate extension patch is:

`logs/wp/extensions-profile-observer-proposal/v1/extensions_profile_observer.after-app.patch`

SHA256 `118c8bf32177b8cf1c3adc0011683367af1d5c8f1551d3f6183db32bd061c3f1`.

Apply the existing app proposal first, then this alternate extension patch
instead of the main extension patch. Copied application of both overlap hunks
and alternate patch passed with fuzz0. Exact final bytes are under `after-app/`
and in `app-composition.json`; they were compared byte-for-byte against the
copied patched result. This preserves the app's disable-field-trial-config
default with both explicit user override checks, app browser-process hook,
existing callback removal and single shared header import. The extension
installer runs after app-hook binding and immediately before ChromeMain.
The app proposal itself was not edited. Its unrelated files/hunks were not
retested or changed by this work.

## Pin/icon composition

The main proposal retains all current actual pin and icon behavior. Composition
inputs were pinned separately, not refreshed:

- Icon proposal SHA256 `2c3e180885f559c0532b366e99b6a46b0f985b7372e4a36216c06c1d603ca2f9`.
- Pin proposal SHA256 `ec73a0ff215ff9388e341ef8b4d285220f6aec7185acbbfa7fde0f4941aa5aef`.

The pin Context hunks apply unchanged with fuzz0 after the new observer refactor.
Icon/profile application in either order has **two include-only hunk conflicts**:
the icon proposal removes SupportsUserData as its old cache disappears, while
the profile service now directly requires it; the action/registry imports also
share hunk context. Lead resolution is the union of required profile imports,
retain `base/supports_user_data.h`, add the real `extension_action.h`, and remove
`extension_icon_image.h`. All semantic icon hunks and all pin setter hunks were
preserved unchanged in the copied composition. Its Context SHA256 is
`1963b24c89a03378b887f90c3db56e6a6e41e2b23c8a4cce5319aff7fb879246`.

`composition.json` retains the unfuzzed conflicts/rejects;
`composition-resolution.json` describes the explicit include resolution. The
resolved Context copied source is under `compose-profile-icon-pin/`. This full
Context, including the persistent observer, native-icon producer and pin
semantics, also compiles against the app-composed shared headers. Browser and
native icon/pin producer hunks are outside this proposal's changed files and
were not refreshed or claimed as newly verified here.

## Verification and exact source hashes

Every changed TU used its own current `siso query commands` command, retaining
all original warnings, Werror, optimization and Chromium Clang plugin flags.
Only dependency/object/serialized-diagnostic outputs were removed. Syntax-only
checks used VFS overlays with use-external-names:false, covering canonical and
Chromium arc-symlink spellings. No graph regeneration or engine build ran.

| Copied variant / TU | Syntax status | Negative VFS check |
|---|---|---|
| Main Context | exit0, empty diagnostics | unique CONTEXT sentinel, expected exit1 |
| Main bootstrap | exit0, empty diagnostics | unique APP sentinel, expected exit1 |
| Main ChromeExtensionSystem | exit0, empty diagnostics | unique NATIVE sentinel, expected exit1 |
| After-app Context/bootstrap/native | exit0, empty diagnostics each | unique after-app sentinels, expected exit1 each |
| Profile+pin+icon Context with app headers | exit0, empty diagnostics | unique all-proposals Context sentinel, expected exit1 |

`git apply --check --whitespace=error` passes for the main patch against the
frozen current originals. The alternate after-app patch was applied only to
copied app-applied originals, with fuzz0 and exact final byte comparison.

The optional-package linker/call test uses the current bootstrap flags for the
two small probe TUs, including its actual optimization flags. With no provider,
the weak default runs and counter remains0; with the strong provider, counter
becomes1. Both executable links and both calls exit0, with empty diagnostics.
Recorded symbols distinguish the weak default and strong implementation. This
is linkage evidence only, not an engine lifecycle test.

| Source | Current actual SHA256 | Main proposed SHA256 |
|---|---|---|
| `arc/api/arc_extensions_context.mm` | `475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d` | `f17b61b06eec5d43fc55511f7f84b7e8c67d5b78394439545cf0f8c632f952b6` |
| `arc/api/arc_internal.h` | `70495f85414c996dc97262cc9670a9464451e9e215600c934cc71e46269a62a1` | `7a7e50cf663b5f6a63fad3cfc88bb329998368dead5663f7b590cb24bc0f84b9` |
| `arc/api/arc_browser_application.mm` | `d6bae43313c72ff2803dbe3037d96b3c4c886d3484c6482a9dd862cb7e85cb55` | `bcc0badc8e5619d01c08f82a113c2478286475d2c976a91c35cb810b952c5a2b` |
| `chromium/src/chrome/browser/arcmod/arcmod_hooks.h` | `a6bb3a03f67ee0e66394ccad379916cce571cfddcfa0e880d76ac8a6ff6b14d2` | `4fdbb329b14d412a9b075e111afad9660109cd0c60276d4e7aa358e9ad97e482` |
| `chromium/src/chrome/browser/extensions/chrome_extension_system.cc` | `047ac0c8ee4dfe66c2dfb045861140607a96e3dd676206c6a6ab470979cddde8` | `ffa54f4ed6fb4a0ce288401b9095855d3dc9bae1dc477d80f7e860b5e98b1c5a` |

`source-sha256.json` retains all source pairs; `app-composition.json` retains
alternate final hashes. `freeze-check.json` verifies all five actual sources
still match their captured baselines. Writes were limited to this proposal
patch/document and new log/evidence copies. Fresh read-only review found no
remaining concrete defect in lifecycle guards, mapping equality, nil/shutdown
revocation or optional linkage/app composition. The reviewer did not mutate
sources or run engine/runtime checks.

## Remaining request and acceptance status

Lead must review/select the regular-profile hook and its maintained Chromium
patch integration, the profile-user-data ownership adaptation, and the bounded
legacy OTR policy. Native OTR fanout and global system-ready queue-boundary
semantics remain precise unresolved questions. They are not silently replaced
by a guessed implementation.

After any approved activation, verify one observer construction before toolbar
registry attachment, delegate nil/reattach/replacement without reordering,
retained-facade queued deliveries after profile shutdown, missing/null factory
behavior, and the real paired fixture callback/payload order including setter
suppression and intermediate pin states. None of those engine-runtime tests or
scenario diffs were run for this proposal. Package acceptance remains
UNEXECUTED for this change. No new architecture/spec contradiction was found;
implementation ownership and the explicitly uncovered boundaries remain limits.


## Addendum: independent init/network-only profile variant (UNAPPLIED)

Use this route when selecting the app's new **init/network-only** stage. It is
independent of the held field-trial default. The earlier app.pending/after-app
composition above is historical for this selection and must not be substituted:
the old after-app copy retains the held default.

New variant:
`logs/wp/extensions-profile-observer-proposal/v2-init-network/extensions_profile_observer.after-init-network.patch`

SHA256 `8ecc477e99111637a6d901428110ab21ed3ecc368cb0c8fbe264bd868caea1f3`.

This variant is applied **after**
`logs/wp/app.run6.init-network.patch`, SHA256
`a9a8a25b04c93b4fac309c657554ac79354cf178367d8186432f4f3773713dcf`.
That app stage replaces only the corresponding initialization/network hunks of
stable208; it is not additive to those same hunks. Apply the new extension variant
instead of the main profile patch or historical after-app variant on this base.
Neither app patch was modified by this work. Do not apply the full stable208
artifact as part of this independent route.

The variant has exactly the same semantic added/deleted lines as main profile
patch4223. Only overlapping bootstrap insertion context is resolved: import
arc_internal once, retain one optional-package weak extension installer default,
and invoke the real installer after binding the app initialization hook and
immediately before ChromeMainInitializeHostedBrowser. The copied app hook and
removal of the late synthetic init/network task are preserved. The profile
observer implementation, hook API and shutdown/weak-target behavior remain
those of the original main proposal.

The complete argument-building policy source is **byte-identical** to actual
bootstrap SHA256
`d6bae43313c72ff2803dbe3037d96b3c4c886d3484c6482a9dd862cb7e85cb55`.
There is no field-trial switch/default in the resulting bootstrap, and no
field-trial, feature or argument-policy change in the extension variant.
The historical after-app bootstrap was never used as a construction input.
`policy-and-linkage-static.json` and exact copied application record this.

### Optional histogram-startup composition

The independent histogram-startup patch, SHA256
`a266382ca353eeffbfdfe442f4f660d84f470e0bc5fe5eca2f4bf0f2ab074ff6`, also composes without changing its
bytes. These three tested orders apply with strict whitespace checks:

1. Init/network, new profile variant, histogram startup.
2. Histogram startup, init/network, new profile variant.
3. Init/network, histogram startup, new profile variant.

All seven final files are byte-identical across those orders. There is exactly
one namespaced extension weak default in the bootstrap, one plain shared
extension installer declaration, one global Mac histogram declaration and one
global Mac weak/noinline histogram default. Both hook getters occur once.
The histogram Mac/host guards and existing app startup phase remain unchanged.
No weak fallback or non-inline startup-function definition was added to a shared header. Histogram startup
is optional for the independent init/network plus profile route.

The final hooks header without histogram has SHA256
`f95bccd5e84c29dcfb3c189ff15991cbc4464eb96362d60372ac81951572777e`;
with histogram it is
`0c3b67234cf9dc2dd9cfa2a35dad479a750eaab5fd12de29aefcad4d134d8482`.
`histogram-composition.json` retains all commands and seven-file hashes.

### New copied-source verification and preserved history

Each copied TU used its own freshly queried current Base command, retaining
warnings, Werror, optimization and Clang plugin flags. Object/dependency outputs
were removed, syntax-only and a VFS overlay were added, and both canonical and
Chromium arc-symlink spellings were mapped with use-external-names:false.

| Variant | TUs | Syntax checks | Negative VFS sentinels |
|---|---|---|---|
| Init/network + profile | Context, bootstrap, extension system | all3 exit0, empty diagnostics | all3 expected exit1 |
| Plus histogram startup | Those3, BrowserProcessImpl, ChromeBrowserMain | all5 exit0, empty diagnostics | all5 expected exit1 |

Exact init/network application followed by the new profile variant yields the
same bytes as the saved `after-init-network/` copies. This is strict copied
patch/application and compiler evidence, not a framework build/link or runtime
acceptance result. Main profile patch remains SHA256
`4223beeeda7b61d7f15d87a0b0867e5828a15671996d2a113dd7b65fc3060535`. The historical after-app variant remains
SHA256 `118c8bf32177b8cf1c3adc0011683367af1d5c8f1551d3f6183db32bd061c3f1`. All142 files in v1 evidence
are unchanged. The earlier bytes of this document are preserved verbatim, with
their original SHA2565ceda1b5 in the new before-addendum copy; this addendum is
the only main-document change.

The new evidence is under
`logs/wp/extensions-profile-observer-proposal/v2-init-network`. Actual source
content fingerprints include current browser ebc84f16, context475fedd7,
bootstrapd6bae433 and internal header70495f85. Source mtimes are recorded
separately. No actual product/shared/helper source, GNI, runtime artifact,
app/profile, lock or board action occurred. No app/golden promotion, build,
package, executable linkage probe or runtime scenario ran for this addendum.

| Final copied source without optional histogram | SHA256 |
|---|---|
| `arc/api/arc_extensions_context.mm` | `f17b61b06eec5d43fc55511f7f84b7e8c67d5b78394439545cf0f8c632f952b6` |
| `arc/api/arc_internal.h` | `7a7e50cf663b5f6a63fad3cfc88bb329998368dead5663f7b590cb24bc0f84b9` |
| `arc/api/arc_browser_application.mm` | `e6b5798b0ab7128c9c31f1a31ab9699f6bfdfb8039a3a1e6cf4673ab7dd95c25` |
| `chromium/src/chrome/browser/arcmod/arcmod_hooks.h` | `f95bccd5e84c29dcfb3c189ff15991cbc4464eb96362d60372ac81951572777e` |
| `chromium/src/chrome/browser/extensions/chrome_extension_system.cc` | `ffa54f4ed6fb4a0ce288401b9095855d3dc9bae1dc477d80f7e860b5e98b1c5a` |
| `chromium/src/chrome/browser/browser_process_impl.cc` | `0a9efbdc8382fcb341124fe627adfcbc9f34acdb6d0cd4517f9a37395f054eaa` |
| `chromium/src/chrome/browser/chrome_browser_main.cc` | `6386e69814d99b3ca0f3f5d778635f5225c8a4d0810b892f552170add01f0dc3` |

Fresh independent read-only review found no material findings in exact
application, unchanged argv policy, semantic equality to the main profile delta,
singleton declarations/defaults or three-order histogram composition. All
previously documented OTR ownership/fanout, global-ready queue-boundary and
profile-shutdown runtime verification limits remain open. This selection does
not establish lifecycle parity or activate either proposal. Shared Chromium
hunks still require lead-maintained patches and integrated runtime validation.
