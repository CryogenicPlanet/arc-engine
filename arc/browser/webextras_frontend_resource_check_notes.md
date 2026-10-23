# Private post-build frontend resource provenance helper

Status: helper proposal; fresh review required before use. Only private synthetic fixtures
were checked. No actual generated bundle, intermediate/built/packaged PAK was hashed or
modified by this work; no build, app, profile, lock acquisition/queue or executor change.
Metadata discovery confirmed the named files currently exist, without claiming their
current content or freshness.

## Exact caller interface

Run only beneath the future integration transaction's ancestor lock, after its build and
package phases, before runtime and again after each scenario when required by the parent:

```
python3 arc/browser/webextras_frontend_resource_check.py --expect-host-gate present --output /Users/cryogenicplanet/general/arc-engine/logs/wp/<fresh-receipt-directory>
```

`--expect-host-gate` is required: candidate initial frontend integration uses `present`;
rollback to the pre-integration frontend uses `absent`. A later batch whose baseline
already includes the gate can seal `present` for both forward and rollback validation.
Mode is an explicit sealed transaction input, never inferred from whichever bytes happen
to exist. In `absent` mode the same complete exact-byte proof applies, but `arcmodHost`
must be absent from all three decoded/generated bundles; unrelated upstream hook name
absence is not guessed. `present` requires `arcmodHost` in all three, inspectModeChanged
in compatibility/elements and requestDockPosition in compatibility/legacy.

There is no `--locked` flag and no source-path override. The helper verifies that the
actual shared lock owner is an ancestor; it never acquires or releases it. The output
must be a fresh subdirectory of this checkout's `logs/wp`. Existing output is rejected;
failure leaves diagnostic inputs but never writes a passing `receipt.json`. Success
returns0, prints `{"status":"RESOURCE_BYTES_VERIFIED","receipt":"<output>/receipt.json"}`,
and writes that receipt with:

- `passed: true`, `expectHostGate: "present"|"absent"`;
- `builtPakSHA256`, `runtimePakSHA256` (packaged ours156), and
  `builtPackagedPakExactMatch: true`; both hashes must equal the scenario runner's sealed
  before/after packaged `resources.sha256`;
- exactly three `resources[]` records with textual/numeric/canonical IDs, exact raw and
  decoded SHA256/size, actual GRD attributes, compression layers, and booleans
  `generatedExactMatch`, `grdInputExactMatch`, `intermediateExactMatch` all true;
- fixed-input identities (`realpath`, dev/ino, size, mtime/ctime, SHA256), and archived
  artifact hashes. Caller must check mode, process0, true fields, exact resource set and
  both PAK hashes; a status string alone is insufficient.

Recon owns the surrounding scenario-runner/transaction checker interface; this helper
adds no executor, builds or scenario actions.

## Primary source derivation and fixed paths

All build paths below are relative to `chromium/src/out/Base`:

| Resource name | Generated uncompressed file | Current discovery metadata ID |
|---|---|---|
| DEVTOOLS_COMPATIBILITY_JS | `gen/third_party/devtools-frontend/src/front_end/devtools_compatibility.js` | 52490 |
| PANELS_ELEMENTS_ELEMENTS_JS | `gen/third_party/devtools-frontend/src/front_end/panels/elements/elements.js` | 52853 |
| UI_LEGACY_LEGACY_JS | `gen/third_party/devtools-frontend/src/front_end/ui/legacy/legacy.js` | 53015 |

IDs are discovered afresh from
`gen/content/browser/devtools/devtools_resources.pak.info`, not hardcoded by the checker.
The generated GRD is
`gen/third_party/devtools-frontend/src/front_end/devtools_resources.grd`. Each selected
include must have the exact expected resource_path, BINDATA, compress=false and file
that is either the exact generated file or that file plus `.compressed`. No traversing,
absolute, alternate or unknown GRD input is accepted.

Primary build/package sources establish this chain:

1. `build/config/devtools.gni` sets `devtools_grd_location`; the frontend root BUILD.gn
   generates that GRD. `front_end/BUILD.gn` copies `devtools_compatibility.js` through
   copy_to_gen_with_grd. The elements/legacy entrypoints use
   `scripts/build/ninja/devtools_entrypoint.gni`'s `$target_gen_dir/$_file_name.js`.
2. `third_party/devtools-frontend/src/BUILD.gn` builds/compresses listed GRD inputs for
   non-debug builds. `scripts/build/compress_files.js` uses Node Brotli with GRIT's
   two-byte magic plus six-byte little-endian uncompressed size. Its generator
   `scripts/build/generate_devtools_grd.py` points the GRD at `.compressed`, while
   debug builds use uncompressed inputs; the checker supports both exact declarations.
3. `content/browser/devtools/BUILD.gn` packs the generated GRD into
   `gen/content/browser/devtools/devtools_resources.pak`. Chrome resources BUILD.gn
   includes it in dev_ui_resources.pak; `chrome/chrome_paks.gni` includes that in
   resources.pak. `tools/grit/repack.gni` copies repacked data into bundle_resources_dir.
4. `chrome/BUILD.gn` sets ArcCore/A; `build/config/mac/rules.gni` places resources beneath
   framework Versions/A/Resources. The built PAK checked is
   `Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/Resources/resources.pak`.
   `package-engine.sh` passes this app's entire framework to
   `../arc/tools/build-mod.ts`, whose engine branch copies it into the agent bundle.
   The packaged PAK checked is
   `../arc/build/ArcModAgent-ours156.app/Contents/Frameworks/ArcCore.framework/Versions/A/Resources/resources.pak`
   relative to the engine checkout.

No assumption is made that out/Base/resources.pak.info exists. The intermediate
DevTools `.info` supplies the selected names/IDs and its source-GRD field must match the
fixed generated GRD path.

## Exact checks, decoding and source policy

The helper compiles only fixed actual primary definitions from freshly read
`tools/grit/grit/format/data_pack.py`: WrongFileVersion, DataPackSizes, DataPackContents,
ReadDataPackFromString, ReadGrdInfo and GrdInfoItem. It does not import the Grit package
or consult its `.pyc`. The exact primary source and extracted executed definitions are
archived; BROTLI_CONST is obtained by literal AST evaluation of the actual constants.py.
This is the official PAK parser body, not an independent PAK format implementation.

Built and packaged whole PAK bytes/hashes must match. Each selected raw resource must
match the intermediate PAK resource and the exact actual GRD input. Only then decode
at most three gzip/GRIT-Brotli layers and compare bytes with the actual generated
uncompressed JS. The receipt archives raw payloads, decoded JS, generated JS, GRD/info,
primary parser/constants source and every compression layer's input/output hashes,
sizes and declared Brotli size. Marker checks supplement exact equality; they do not
replace it or claim to prove all frontend control flow.

Input limits: PAK/Node binary256MiB, metadata4MiB, each JS/compressed layer32MiB.
Gzip uses bounded zlib output and rejects truncation, trailing/concatenated streams and
oversized output. Brotli validates its GRIT length header, then uses the fixed absolute
`/opt/homebrew/bin/node` target's standard bounded brotliDecompressSync (32MiB limit,
20-second subprocess timeout, no shell). Decoded size must equal the header. Unknown
noncompressed content is compared exactly with generated bytes. An unsupported future
codec/layout fails closed for review; no alternate source or global executable is
silently tried.

Every read is stat-stable; all inputs are rehashed after decoding/archive. The helper,
primary parser/constants and actual resolved Node executable are included. This seals
those reader sources; it is not a claim to reproduce Python/Node shared libraries or
the entire build dependency graph. The parent's transaction must phase-seal the source,
GN/generator/compiler inputs before build, plus helper/parser/codec inputs before this
check, and retain this receipt alongside packaged binary/resource before/after identities.
A byte-matching generated file alone does not prove all compiler inputs were current.

## Private validation

`python3 arc/browser/webextras_frontend_resource_check_test.py` uses only temporary
synthetic files and the official PAK writer definition compiled from source. It executes
the helper source directly, without an import cache. Tests cover plain/gzip/Brotli/mixed
layers, official alias handling, both expected-gate modes, package/generated/intermediate
mismatch, exact decoded-versus-generated mismatch, missing markers/metadata, duplicate
IDs, wrong generator provenance, unsafe GRD path, invalid Brotli sizes/header, excessive
layers, concatenated gzip, actual gzip/Brotli output bounds and mid-check input drift.
No actual framework/generated-resource content is read by these tests.

This proves a bounded resource provenance checker. Actual resource verification,
frontend integration, build/package success, app behavior and boundary parity remain
unexecuted and require the reviewed parent transaction.
