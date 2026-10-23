# Boundary callback argument correction review

Verdict: **no P1/P2 blocker found in the corrected route: six pinned normalization prerequisites → 87be7161 → f2b3d542 callback correction**. Both argument patches are required. This is source/comparator review readiness for lead integration, not activation or runtime/package acceptance.

This explicitly supersedes the adoption-readiness verdict in my earlier `webextras_boundary_arguments_review.md` (SHA510506d463119a233cac7040bafb80536d02d26ef139163d6d3eeefee011399c). That review missed a real P2 regression:87be alone suppresses pre-existing engine-to-app argument evidence whenever all method calls contain literal `"<ptr>"`. Its passing27 controls did not cover that callback case. The old review and all frozen evidence are preserved unchanged;87be alone is not adoption-ready. Historical350b remains HOLD due default argparse-error usage drift and must not be combined with this route.

Exact frozen identities:

- Incremental patch `f2b3d542ab7c0246aea1215771209464946bee73b1cb8b83bcc1944630dea092`.
- Notes `6266dce06717b9faba11f7e3f27340a3eebf33a59da03dda8924d85ee8722f77`; test `cec30ce6f3c8f1663c70bf25da3080c8e6d9c47dfe8d2a6a9232c34a87d29bdd`.
- Freeze `32a37e6d32c01b11932a8a23e8f5816bec4c5f910674de7b90e1d1d005c63882`; artifact manifest `f35160852bf33e784d24a92178a788053032877a82489dd4d297e1cc9bc6d68a`.
- Defective87be baseline comparator `14671717f532c81807404cfa26cd3b5a7d1622ca3e48326ce363f6ff7a294caa`; corrected comparator `24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf`.

The small source delta at staged `tests/recorder/diff.py:356` moves `key[0] == "engine->app"` outside the literal-pointer exclusion. Existing callbacks therefore remain eligible regardless of pointer-looking strings anywhere in their arguments. The existing whole-method literal-pointer exclusion remains only on additional metadata-bound call-into-engine coverage. It introduces no type inference, selector/value exception, normalizer or allowlist. Private helper admission is unchanged.

The opt-in argument loop still runs independently after the first-return loop and appends only its first difference before breaking. Default callback argument comparison remains disabled only when the replacement opt-in lane is active, preventing duplicate reports; default return-first precedence is unchanged. Missing/extra/count logic and the ordered callback selector stream are untouched. Caller validation, all Google/receiver loader propagation, normalization and capability metadata are unchanged. The patch contains no CLI/parser change; default malformed-input paths are unaffected by source inspection.

Fresh verification:

- All3 owned outputs and33 correction evidence files match the freeze/manifest, and all31 earlier87be evidence files still match their old manifest. Download/Google/receiver helper bytes and the normalization block are identical. A fresh TEMP strict check/apply of this correction on frozen87be reproduces the corrected comparator bytes exactly.
- A fresh actual TEMP-reconstructed module/CLI suite passes32 tests: inherited27 unchanged plus the five new cases. These cover exact filename/timeout regression, nested app getter, nested literal-pointer payload, independent callback argument/return mismatch and exactly one first argument mismatch across repeated callbacks. Existing counts, constructor/private-helper exclusions, empty/malformed attribution, normalization reloads and archived default stdout/JSON equivalence still pass.
- Independently reran actual CLI commands on the frozen four-row synthetic fixture containing matched valid DownloadItem GUID proof and the real selector `browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:`. Both sides use literal filename `"<ptr>"`; only timeout15→16 differs. Defective87be default reports1 argument mismatch; defective87be opt-in reports0; corrected default and corrected opt-in each report1. All return-mismatch lists are empty. Default baseline/corrected full stdout and JSON bytes are equal.
- An additional independent raw replay inserts an attributed nested app `ArcWebContents.isReadyToDisplay` getter returning true versus false inside that callback. Corrected opt-in reports exactly1 callback argument mismatch AND1 nested getter return mismatch. Neither hides the other. These are comparator counterexamples; no native runtime literal filename or current app occurrence is claimed.

Own fresh commands, raw nested inputs, stdout/stderr, JSON reports and32-test output are retained under `logs/wp/webextras-boundary-arguments-callback-correction-review/`. Its manifest SHA256 is `3a90bce8beedd5fe5db467c35d6c9917033c8028de3c04a85a7ba48ab5cb765f`.

Lead/driver requirements remain: apply exactly the pinned full chain plus87be and this incremental correction, then require BOTH a deliberately different setter-argument canary and a literal-pointer callback canary, each with exact capability metadata and expected one argument mismatch. A setter-only canary accepts defective87be. Exit0 and capability metadata alone prove neither useful public work nor origin/ancestry/forwarding; existing independent driver gates remain necessary. No shared/proposal source, old review/evidence, active driver or normalizer was edited; no engine build, app run, lock or board action occurred.
