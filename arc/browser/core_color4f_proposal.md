ArcColor4f leaf proposal — lead ownership/application required
============================================================

Artifact: /tmp/core-color4f-stage/core_color4f.patch
New source: arc/api/arc_app_color.mm (staged only).
Incremental app source list: arc/wp/app.gni (staged only).
No production, shared/generated file, GNI, native source, app, lock, or board changed.
No current WP assigns ArcColor4f; the lead must assign/apply this app-package leaf.

MANDATORY INTEGRATION PREREQUISITE
----------------------------------
Current production arc/api/arc_api_classes.inc:5 still contains
ARC_CLASS(ArcColor4f). arc_api_stubs.mm:167..174 emits a primary class/metaclass
for that row. Linking this new primary without regenerating the shell inventory
would create duplicate class/metaclass symbols. The two-file patch ALONE is NOT
ready to link against today's generated classes file.

Lead application order:
1. Assign/apply this new source and app.gni addition.
2. Run python3 arc/tools/gen_api_table.py, as the lead owning generated files.
3. Confirm regenerated arc_api_classes.inc omits ARC_CLASS(ArcColor4f), then build.

The generator's primary-implementation scan at gen_api_table.py:37..45 recognizes
this exact standalone @implementation ArcColor4f line and excludes it. There is no
automatic GN regeneration action established here. The script has no output-path
option; it writes files relative to its own location, so it was NOT run against
production. No generated-file diff is included. Its table still has applicable
scalar selectors, but runtime class_addMethod does not overwrite real methods.

Source and metadata
-------------------
Read specs/ArcColor4f.md in full, exact TSV2636..2652, and original bounded bodies
0443b1c8/0443b214/0443b220/04401b90/00015ac0. inputs.json freezes all evidence,
production app.gni baseline, staged source/list, and standalone verifier hashes.
Native file-only disassembly is in native-evidence.txt. No original framework was
loaded or executed for these checks.

Four float32 fields r/g/b/a are stored inline at object8/12/16/20. Struct Color4f
is16 bytes/alignment4; NSObject instance24, _color4f offset8. Pointer initializer
calls NSObject init and copies the input value. Getters read fields unchanged;
nonatomic native property copies the whole value. No null-pointer fallback,
clamping, finite/range check, color-space conversion, or default normalization is
added. The Init category packs four float arguments and messages
initWithColor4f:, preserving that selector call shape.

IMPORTANT ABI CORRECTION TO THE TASK'S HFA ASSUMPTION
----------------------------------------------------
Original property methods are NOT arm64 HFA calls:
443b214: ldur q0,[x0,#8]; str q0,[x8]; ret  (indirect result through x8)
443b220: ldr q0,[x2]; stur q0,[x0,#8]; ret  (indirect by-value input through x2)
The four scalar Init args DO use s0..s3, then stack packing and pointer forwarding.
Metadata {Color4f=ffff} alone cannot distinguish a trivial HFA struct from a
nontrivial-for-calls C++ value. A plain four-float C struct would have matching
metadata yet the WRONG native property ABI.

The minimal empty C++ ctor/dtor reproduces indirect aggregate passing without
inventing stores/cleanup. Explicit default copy/assignment preserve four fields.
The compiler emits .cxx_construct returning self and .cxx_destruct, both single
ret, matching original shared15ac0 and exact TSV encodings. This follows the same
existing source pattern as app_values::ErrorStatus in arc_app_values.mm:9..20.
Standalone optimized assembly EXACTLY matches both original property leaves and
both no-op lifecycle leaves; see standalone.s. No explicit Objective-C methods
with invented dot-prefixed selectors are added.

Validation and limits
---------------------
1. Actual Chromium Browser command via siso query, substituted staged source and
   include overlay, outputs/dependency generation removed, -fsyntax-only added:
   actual compiler exit0, no diagnostics; checker process exit0. check.log and
   /tmp/core_check_color4f.py preserve this check. No build or lock acquired.
2. git apply --check on frozen production app.gni/new-source baseline: exit0,
   apply-check.log. Current production app.gni hash still matches inputs.json.
3. Foundation-only arm64 standalone executable, xcrun clang++ -std=c++20
   -fobjc-arc -O2 -Wall -Wextra -Werror -framework Foundation: compile0/run0.
   verify_color4f.mm uses the EXACT staged class body; ONLY arc_internal import is
   replaced by Foundation import plus identical ARC_EXPORT macro, then test code
   is appended. Generation asserted that reversing only that import shim restores
   source byte-for-byte. verify_main.inc is the appended verifier.
4. Standalone runtime verifies all10 method identities/encodings, all5 property
   attributes, ivar encoding, NSObject superclass, class size24 and ivar offset8.
   Tests pointer/property copy independence, scalar getters/Init forwarding, and
   direct runtime IMP aggregate calls. Five input vectors include signedzero,
   subnormals, largest finite values, infinities, out-of-range values, quiet and
   signaling NaN payloads; every float32 bit pattern survives unchanged.
5. otool dependencies are Foundation/objc/c++/System only. Runtime dyld-image checks
   assert no ArcCore or AppKit framework loaded. standalone-run.log records PASS.

This is candidate-only ABI/value verification, not paired runtime parity against
original Arc, not framework linking, and not a display integration/scenario run.
Lead-generated shell removal, ownership assignment, full framework build, and
actual metadata/display boundary verification remain outstanding. Review must
include the generated-class prerequisite, not only the standalone class body.

Fresh independent review
------------------------
core_review found no concrete P1/P2 in the value leaf: types, metadata, indirect
ABI, float bits/copy and Init forwarding match cited evidence. Reviewer identified
the duplicate generated-shell prerequisite; the follow-on documentation check
confirmed the mandatory regeneration/application order is now explicit. No full
framework linkage or original paired runtime was claimed by that review.
