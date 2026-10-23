# Private pre-lock checks for the reviewed initial frontend pipeline

The precise frontend preflight is available and has passed with the actual GN compiler,
TypeScript7.0.2: baseline and reviewed staged output each passed four declaration actions,
seven no-emit semantic checks and one compatibility-JS syntax check (24 commands, exit0,
empty diagnostics). Two deliberate API-declaration removals made the corresponding
actual consumer check fail; the private declaration was restored byte-for-byte.
No actual source, GN, build/output, generated declaration, resource, package, app, index,
lock or board was changed. Private outputs are under the temporary directory recorded
in `logs/wp/webextras-frontend-prelock/receipt.json`; owned evidence is in that log directory.

This supersedes using the earlier55→55 TypeScript6 limited probe as build equivalence.
The current frontend node_modules compiler is6.0.2, while GN's mac-arm64 compiler and
its `--version` are7.0.2. The earlier probe was valid comparative evidence, but is not
the exact production type checker or option set.

## Exact frontend recipe

`logs/wp/webextras-frontend-prelock/probe.py` is the concrete private-only recipe.
Every exact command argv/cwd, input hash and result is in `receipt.json`. No Ninja/Siso
build, GN regen, stamp writer, run_with_restat or normal compiler output path is invoked.
The recipe is:

1. Capture stable frontend source inputs from `front_end` and `extension-api`, generated
   `.d.ts`/`*-tsconfig{,.ref}.json`/package descriptors from
   `out/Base/gen/third_party/devtools-frontend/src`, and the build/frontend package.json
   descriptors. Mirror the directory depth under a fresh private `src`; symlink only
   the existing node_modules dependency tree for reads. No emitted path points to it.
   Keep generated extension-api declarations/configs: models/bindings references them.
2. Make baseline and staged mirrors. Install only the five reviewed staged frontend
   leaves from the initial5 + defdeb5f host-gate result into the staged mirror. The
   source chain is dcbf→d929→2fae and owned a2a9→ed9a8, plus defdeb5f on shared output;
   the owned ObjC++ changes do not enter these TS commands. No held artifact is edited.
3. Use actual `chromium/src/third_party/typescript/mac-arm64/src/lib/tsc`. For each of
   `core/host/host_base`, `core/host/host_stub`, `panels/elements/elements`, `ui/legacy/ui`,
   derive the original source/global-definition list from its captured generated
   `*-tsconfig.json`. Execute the same declaration action used by typescript.gni,
   redirecting all locations to the private mirror:

```
<actual-tsc7> --ignoreConfig --declaration --emitDeclarationOnly --isolatedDeclarations --noCheck --noResolve --outDir <private-generated-module-dir> --rootDir <private-source-module-dir> <captured-private-source/global-definition-files...>
```

4. Run the existing generated config for each of these seven module descriptors,
   copied at its original relative location in the private gen tree:

| Descriptor beneath generated `front_end` | Why included |
|---|---|
| `core/host/host_base-tsconfig.json` | changed Host API; browser foundation |
| `core/host/host_base_node_typecheck-tsconfig.json` | mandatory Node foundation check |
| `core/host/host_stub-tsconfig.json` | changed implementation of Host API |
| `core/host/host-tsconfig.json` | existing Host instance consumes changed API/stub |
| `core/host/host_node_typecheck-tsconfig.json` | mandatory Node foundation check |
| `panels/elements/elements-tsconfig.json` | changed inspect producer |
| `ui/legacy/ui-tsconfig.json` | changed dock producer and Root import |

```
<actual-tsc7> --noEmit --incremental false -p <private-generated-descriptor>
```

No `-b` is used. Generated normal configs already use composite=false,
declaration=false, incremental=false, skipLibCheck=true, empty default types/typeRoots,
moduleResolution=bundler and rootDirs mapping source plus generated declarations.
Browser/UI targets have their actual DOM libraries; foundation Node checks retain their
actual Node-only config. They are not replaced by a handwritten broad tsconfig.

**The private declaration actions are essential.** Project references intentionally use
`disableSourceOfProjectReferenceRedirect=true`. Merely replacing the four source `.ts`
files while keeping the old generated Host API/stub `.d.ts` either produces a stale-API
false failure or misses the intended dependency shape. Both baseline and staged mirrors
refresh those declarations before consumers. Existing unrelated generated declarations
are used as captured dependencies; this is not a fresh full-graph build.

5. Compatibility JS is a copy-to-gen input (`front_end:embedder-scripts`), not a TS
   module. Its preflight is syntax parsing only:

```
/opt/homebrew/bin/node --check <private-front_end/devtools_compatibility.js>
```

The two independent private controls remove inspectModeChanged/requestDockPosition
from the staged generated API declaration. Actual elements/UI semantic checks then fail
with the expected missing-property diagnostics (TS2551/TS2339), showing the added calls
are typed through the refreshed API. `api-negative-controls.json` retains the commands'
results; the declaration was restored and its hash verified.

## Minimal native TU recipe using existing build flags

The native units are `devtools_window.cc`, `devtools_ui_bindings.cc`,
`devtools_embedder_message_dispatcher.cc`, and owned `arc_webextras_devtools.mm`.
Exact genuine original and syntax argv from the reviewed host-gate check are retained in
`logs/wp/webextras-devtools-host-gate/syntax/{devtools_window,devtools_ui_bindings,devtools_embedder_message_dispatcher,companion}.{original-command,syntax-command}.json`.
All four previously passed with the real build flags and private VFS overlay, zero
exit/diagnostics. This feasibility refresh does not relabel those historical commands
as a newly queried Sessions-provisional build graph or claim a fresh native run.

For a current pre-lock recheck, copy the relevant current source bases, strictly compose
only the reviewed chain in private, and make a VFS overlay for those source/header paths.
Capture the current arc_internal.h; add the one permitted declaration privately only if
still absent. Preserve concurrent shared declarations instead of substituting the old
whole header. Execute the original compiler argv directly from `out/Base`, removing
`-c`, `-o <...>`, dependency-output arguments `-MF <...>` and `-MD`/`-MMD`, then add
`-fsyntax-only -Wno-unused-command-line-argument -ivfsoverlay <private-overlay.json>`.
Keep actual explicit module map/PCM inputs as read-only dependencies. Redirect
`-fcrash-diagnostics-dir` and `-fmodules-cache-path` to fresh private directories; retain
`-fno-implicit-modules -fno-implicit-module-maps` so missing modules fail rather than
regenerate into mutable shared output. Check direct compiler exit and all diagnostics;
`check-file.sh`'s filtered success message alone is not the exit-status receipt.

The existing explicit system PCM is
`out/Base/obj/build/modules/system/module.pcm`, with module map
`out/Base/gen/build/modules/module.modulemap`. Compiler is the recorded Chromium
`third_party/llvm-build/Release+Asserts/bin/clang++`. If current graph flags, generated
headers/PCM or staged contexts differ, recapture and recheck rather than assert byte
identity with this earlier invocation. None of these commands may run the real regen
script or build a missing generated input during preflight.

## Build dependency and remaining checks

Primary source definitions are `scripts/build/typescript/typescript.gni`,
`ts_type_check.gni`, `typescript_vars.gni`, `ninja/devtools_module.gni`, and the three
module BUILD.gn files. Foundation targets include both browser and Node checks.
`devtools_entrypoint.gni` splits entrypoint declaration/type checks from esbuild bundling;
private type success does not produce the real runtime bundle. `bundle.gni` invokes
esbuild.js using generated bundled_entrypoints.json, source and generated roots, a
mutable depfile/output and minify flags determined by the actual build. Root frontend
BUILD.gn then compresses and generates GRD inputs; content GRIT packs them into the
framework resource chain. The actual locked build/package and reviewed resource checker
must still establish fresh bundled bytes and packaged-resource identity.

Unavailable from these checks: a clean build of every frontend target, native linking,
GN/dependency regeneration, esbuild/resource/compression success in the actual output,
app behavior or boundary parity. The seven targeted configs are clean, unlike the old
broad comparative probe, but are not a claim that all frontend tests are clean.
Snapshots were stable before/after this private run. They must be recaptured against
the actual integration slot's bases after other provisional integrations settle.
