# Comparator argument adoption queue addendum

Only `webextras_boundary_arguments_after_normalization.patch` (SHA87be71617d51eeab13facacb3ae1fad2ece71ba0703e5acf849a8bde1ce3f89c) is proposed for adoption, after the exact pinned six-patch normalization chain. It remains UNAPPLIED and pending fresh review/lead integration.

The fixture-only350b509a variant and its original notes/tests/evidence are preserved as historical/HOLD. Its normal report stdout/JSON is unchanged, but a confirmed malformed-fixture error adds the new option to argparse usage even when the flag is absent. That violates strict default error-output equivalence. It is not an adoption candidate and was not revised. Evidence: `logs/wp/webextras.boundary-arguments-default-error-review.json`.

The87be variant inherits the full chain's existing usage freeze. Independent constant-argv0 comparison verifies all eight default error/control cases exactly match the full-chain baseline in exit status, stdout and stderr: malformed fixture, malformed Google input, empty fixture, invalid caller fixture, unknown option, abbreviated existing option, invalid existing option value and invalid fixture image. Evidence: `logs/wp/webextras.boundary-arguments-after-normalization-default-errors.json`.

Valid default full stdout/JSON byte-equivalence and all27 tests remain as recorded in the after-normalization evidence. Normalization source/helper bytes and87be are unchanged. The two argument variants are mutually exclusive; never apply both. Do not combine historical350b with the full-chain route or duplicate any prerequisite normalization patch. No shared file, normalization rule, production driver, build, app run or acceptance state was changed.
