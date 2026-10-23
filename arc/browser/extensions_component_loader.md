# Deferred normal-loader companion for Arc's internal component

**Proposal only.** `extensions_component_loader.patch` is unapplied. It must
wait for the lead to integrate and apply the existing exact-ID hosted allowlist
proposal, `arc/browser/extensions_component_allowlist.patch`, through the
maintained Chromium patches. Do not apply this owned companion first. The
current native allowlist rejects this ID and reaches `NOTREACHED()`; the
companion deliberately uses that normal allowlist instead of bypassing it.

## Proposed change

Only `arc/api/arc_extensions_internal.mm` changes:

- Obtain `extensions::ComponentLoader::Get(profile)` and use its `Exists` for
  duplicate registration instead of checking the installed registry.
- Pass the unchanged embedded stock manifest to the public production
  `Add(std::string_view, FilePath)` overload, with the stock relative root
  `arc_internal_extension`.
- Remove the manual JSON parse, `Extension::Create`, and direct registrar
  call. Normal ComponentLoader still reaches the same `REQUIRE_KEY`, component
  manifest location, and production registrar when it loads the component.
- Reject a destroyed weak profile or a profile whose `ShutdownStarted()` is
  true before looking up the loader. Keep the existing host switch, original
  profile selection, per-profile userdata marker, WeakPtr capture, and
  ExtensionSystem readiness dispatch unchanged.
- Log an unexpected registration result without adding app callbacks or a
  substitute listener. No resources, public values, Snapshot/model algorithms,
  global settings, or GN dependencies change.

There is no `set_ignore_allowlist_for_testing`, `AddOrReplace`, private
`skip_allowlist` overload, test background-extension switch, or new broad
allowlist. The separate prerequisite admits only
`ldciclpbkaobfbicjebamaapbfplfjen` under `--arcmod-host-app`; ordinary Chromium
allowlist behavior remains intact.

## Native assets and API evidence

Original ArcCore `0x4311964` reads resource 14558 at `0x43119a4-19a8`, substitutes
`<APP_NAME>` with Arc or Dia at `0x43119b8-19e8`, constructs the relative
`arc_internal_extension` root at `0x4311a2c-1a3c`, and calls the normal loader
Add path at `0x4311a58` with the skip-allowlist argument zero (`0x4311a54`).
This companion preserves the existing Arc-substituted manifest, public key,
version 0.0.10, MV3 service worker, permissions, and packaged assets.

Current `chrome/browser/extensions/component_loader.h:86-96` exposes both
the production string and parsed-manifest overloads. The selected string
overload in `component_loader.cc:355-389` parses and retains the manifest and
root, generates the ID, performs the ordinary ID allowlist check, registers
the component, and loads it immediately when ExtensionSystem is ready. Add's
returned ID proves registration, not that later extension creation or worker
startup succeeded. `Load` still logs extension creation errors.

The relative path matches the native loader contract:

- `ComponentExtensionInfo` at `component_loader.cc:227-235` resolves relative
  roots under `chrome::DIR_RESOURCES`.
- On Mac, `chrome/common/chrome_paths.cc:264-268` resolves that directory to
  `base::apple::FrameworkBundlePath()/Resources`.
- Existing `arc/browser/extensions_internal/BUILD.gn` packages `background.js`
  and `manifest.json` under that bundle's `arc_internal_extension` directory.
  The previous code derived the same resource directory from NSBundle; the
  proposal delegates this path handling to the normal native loader.

## Bookkeeping and lifetime

Direct registrar insertion leaves the component absent from ComponentLoader's
`component_extensions_` collection. The normal Add path retains the original
manifest, root, and ID. This supplies the native behavior for:

- `Exists` (`component_loader.cc:496`) and registered-ID enumeration;
- `LoadAll` and `Reload` (`component_loader.cc:406`), which recreate registered
  components from retained data;
- normal component removal/unload;
- `ChromeContentBrowserClientExtensionsPart::IsBuiltinComponent`, whose
  ordinary extension-origin branch returns `ComponentLoader::Exists`
  (`chrome_content_browser_client_extensions_part.cc:735-737`).

The factory redirects regular/guest OTR requests to their original profile and
depends on ExtensionRegistrar and ExtensionSystem
(`component_loader_factory.cc:36-55`). Existing EnsureInternalExtension already
selects the original profile and stores the once marker there. A queued
closure captures only `base::WeakPtr<Profile>`; it does not retain the profile,
loader, or native extension. Install obtains the loader at execution time and
does not retain it across asynchronous work.

The added shutdown guard matters because `ComponentLoader::Shutdown` nulls
its profile and ExtensionSystem pointers (`component_loader.cc:298-301`),
while a live Profile weak pointer alone does not prove services are active.
`Profile::MaybeSendDestroyedNotification` invokes `NotifyWillBeDestroyed`
(`profile.cc:484-499`), which supplies the public `ShutdownStarted` state.
The existing once marker remains once-only even if registration fails; no
automatic retry, loader adoption, or live migration of a component previously
inserted directly in the same process is claimed.

## Validation without applying to product source

Temporary evidence directory:

`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-component-loader-4m_fzg6o`

- `git apply --check --whitespace=error` passes against the actual current TU.
- The patch applies to a temporary copied original; the result matches the
  proposed copy byte-for-byte.
- The proposed TU passes Clang `-fsyntax-only` with its actual current Base
  compiler command for `obj/arc/arc_core/arc_extensions_internal.o`: exit 0,
  no diagnostics. Compiler warning/error and plugin flags were retained;
  object/dependency output flags were stripped.
- A VFS overlay with `use-external-names: false` maps both the repository source
  and Chromium symlink spelling to the proposed temporary source. A negative
  overlay produces `ARC_COMPONENT_LOADER_VFS_SENTINEL` and exit 1, proving the
  source-routing check reads the temporary file.
- Exact argv, overlay, compiler logs, negative sentinel and source hashes are
  retained in that directory. An initial command-query decoding failure was
  corrected before any compiler invocation; it changed no product file.

Actual-source SHA256 before and after:
`cf61b5c5baeb28691229b5f4c61ed60a0f2993d3a9c383be1952e2d305793db3`.
Proposed-copy SHA256:
`dc63b7f5bd8095fb3fbe23a54178e2a7def8c8d1930ef1ef313b5cebd7b0b0f3`.

No actual product/shared source, packaged resource, runtime helper, compiled
artifact, profile, or app was modified. No build graph command, app run, or
shared lock was used. Syntax and static bookkeeping evidence establish no
runtime parity claim.

Fresh read-only review by `/root/extension_scout/component_loader_review` found
no blocking issue in the patch or note. It independently checked the loader
API, path resolution, shutdown/lifetime protection, exact allowlist dependency,
bookkeeping, apply-check and retained syntax/VFS evidence. Its one citation
correction was incorporated above.

Request to lead: apply the exact hosted-ID allowlist first, then integrate this
companion. A later real run must verify successful registry load and real MV3
listener startup, loader Exists/builtin classification and reload behavior,
hidden-component behavior, and the existing launch-slice/extensions boundary
transcripts. Do not fabricate group-ID callbacks or normalize callback counts.
