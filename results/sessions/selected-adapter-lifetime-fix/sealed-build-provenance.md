# Source-sealed sessions inputs during run2r build/package

**Supported:** the run2r-auth-v4 build and package completed while recorded sessions source `e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d` and sessions.gni `6f983408cd5c903507a5ae093cc6bee7700a0aa69e83d0c017581bcb3a8a9830` remained unchanged. These bytes equal the promoted selected-adapter candidate and current source. This is stronger build provenance than the earlier webextras native10 record, whose source snapshots omitted sessions. **The overall run2r pipeline FAILED with exit1 before either app/probe launch. No discard callback execution or runtime acceptance is established.**

Evidence prefix: `logs/wp/app-boundary-validation/run2r-auth-v4.prelaunch-drift.20261023T164226Z/`. The adjacent JSON hashes all archived run files plus relevant scripts and separately labels present-time observations.

## Source and inventory seal

`inputs.before.json` records1369 paths, including sessions, sessions.gni, arc_internal.h, args.gn, the build/package scripts, app/probe support and stock files. `inventories.before.json` and `inventories.after.json` are identical:126 production paths,57 API units,1109 stock paths, plus the app/UI/profile sub-inventories. These are the selected inventory sets, not every repository/native input.

The wrapper's final verifier (`logs/wp/app.run2r-auth-v4.sh:313..339`) rehashes every selected input, reenumerates those inventories and writes `inputs.changed.json`. Its sole entry is `arc/browser/webextras_native_probe.m`; sessions and GNI therefore passed this final comparison against their before hashes. There is no separate explicit after-hash map for sessions: after equality is established by the verifier's negative drift result and its pinned code. Current sessions/candidate/GNI hashes independently match those before pins at this audit.

## Build/package stages and failure boundary

The wrapper uses `set -euo pipefail`. Its lines400..410 run API generation, `ARC_BUILD_FAIL_FAST=1 ./build-babysit.sh`, package-engine, strict deep codesign verification, final-dev fingerprint, helper build and helper fingerprint in that order. The wrapper reaches the following seal stage, so each preceding command returned0; no separate per-stage exit receipt is archived.

`build.log` records preflight16:40:04Z, attempt16:40:06Z and BUILD DONE16:40:53Z. The underlying `logs/build-base.attempt-1.log` still records successful Siso completion; neither log contains an individual sessions compiler or framework linker command. `package.log` records copying the out/Base framework into ours156 and successful signing/verification; the separate strict `package-codesign.log` is empty and execution proceeds. `helper-build.log` is empty and `built-helper.json` records its built dylib.

`compiled.inputs.json` records the signed packaged framework SHA256 `28beee762b3858ea197a87736aca21b5cbbfa01ef897ca6f0e747d91568eee5c`, size641357968, mtime16:40:59.586693Z. Despite its name, that map contains app sources/args/framework; it does **not** directly pin sessions. Sessions provenance instead comes from the selected-input seal above.

The wrapper log stops at `STAGE seal-current-canonical-images-before-either-launch`: source verification at line419 rejects the changed probe source before writing `binary-pins.before.json`. No `probe` directory, `probe.log`, `probe.exit` or binary-pins receipt exists. The runner invocation is later at line439, after `probe_started=1`; it was not reached. Final verification repeats the same single drift and writes `pipeline.exit=1`. This is a failed prelaunch seal, not a passing overall experiment. The earlier profile-backup step is not an app launch.

## Additional present-time observations and limits

At this audit, sessions object mtime is16:40:16.198274Z inside the recorded build, SHA256 `50862fe23bf35909ddfab066d4ca0f88dd0e7e75fbb55a7481c681b595f10b83`. Current built framework mtime16:40:53.024841Z has SHA256 `fe72dde1ab46c9708f6db56542b242f1cc4074c29e21f7574dcefb2fb16ed953`; current signed packaged framework matches the archived28beee76 pin. These object/built-framework pins were observed now, not archived by run2r; they support temporal consistency without supplying a missing historical compiler command, object before/after seal or built-framework receipt.

The current GN/GNI mapping includes sessions source in arc_core and its generated link dependency set. Root `arc/BUILD.gn`, `chromium/src/chrome/BUILD.gn`, generated Ninja files and the full native/toolchain closure are absent from the recorded selected-input seal. Do not label this a complete source/toolchain freeze, clean rebuild of every object, per-object compiler invocation proof, exact historical built-to-signed byte transformation proof, or execution of the selected-adapter fix. No production/source/test/shared files, apps, build jobs, locks, board or credentials were changed/accessed by this audit.
