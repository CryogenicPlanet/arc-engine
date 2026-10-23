# Held boundary argument comparison after the full normalization chain

UNAPPLIED, mutually exclusive alternative to `webextras_boundary_arguments.patch` (SHA350b509a). Apply exactly one argument proposal. This alternative applies **after** all six existing normalization patches in the pinned order: fixture→epoch→SessionID→receivers→Google-query→profile-download. It does not include or replace those prerequisites. The original350b variant, its notes and22-test source are unchanged.

The reconstructed full-chain baseline comparator SHA256 is `c7659039f9ee152515d02d0ebca38c8fae0232650fd9b69e7b213c5de81fd3ec`; after this incremental patch it is `14671717f532c81807404cfa26cd3b5a7d1622ca3e48326ce363f6ff7a294caa`. Strict `git apply --check`/application reconstructs the staged bytes exactly. Exact prerequisite hashes and helper bytes are retained in `logs/wp/webextras-boundary-arguments-after-normalization/initial-manifest.json`. No actual shared source, prerequisite, allowlist, production driver or runtime was changed.

The new flag has the identical semantics and capability metadata documented in `webextras_boundary_arguments_notes.md`: independent first normalized return/argument mismatch per selected metadata boundary method or existing engine-to-app callback; existing whole-method literal-`"<ptr>"` argument skip; valid caller strings on every call/return and at least one call per input; no fixture-image requirement for callback-only spans. Syntactically valid unselected caller images keep their existing exclusion. This is caller-field/selection validation, not proof of semantic origin or ancestry. Driver callback/forwarding/identity/public-work gates remain separate.

All existing normalization functions, the raw normalization prepass and the comparison transformation region are unchanged. Download, Google and receiver helper files are byte-identical. The only extra integration work threads `require_callers` through the initial loads, Google pair-rejection fallback loads and receiver-align `partial(load,...)` reloads. This prevents normalization reloads from silently losing the new opt-in caller requirement. Existing Google rejection and mismatch exit policies remain in effect; the argument option does not turn a rejected scope into acceptance.

Without the new flag, normal reports retain identical first two lines, full stdout and JSON bytes. The inherited22 tests compare the full-chain baseline against this variant, including both archived real-recording regressions. Five additional controls exercise existing epoch raw-key proof, SessionID aliases, download raw identity proof and aliases, successful Google+receiver reloads, and Google rejection fallback while the argument gate is enabled. The successful combined scope also reproduces its baseline full report after removing only the new capability key; callback/return normalization does not change.

Run:

```
python3 -B arc/browser/webextras_boundary_arguments_after_normalization_test.py
```

All27 tests pass: the original22 test bodies are inherited unchanged, plus the five controls. The first additional combined-scope test initially failed because the generic synthetic helper overwrote positive fixture sequence IDs with seq0. Only that test serialization was corrected to preserve the existing source-sensitive fixtures; comparator code was unchanged. Both logs are retained. No semantic normalization conflict was observed after the strict rebase.

The option remains unsupported in the actual shared comparator. Adoption still requires fresh review, lead application of exactly one variant, and the driver's deliberately different setter-argument canary with exact capability metadata and mismatch count. No build, app run, activation or boundary acceptance is claimed.
