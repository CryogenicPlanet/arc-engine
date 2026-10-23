# Post-promotion selected-adapter lifetime audit

PASS: production SHA e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d is byte-exact the reviewed /tmp candidate. Against HEAD a647, the complete change is exactly the two-line transformation recorded in owned.diff.

At126, delivery strong-locks captured weakAdapter into an objc_precise_lifetime local;128 reads that selected adapter's delegate. Its lexical lifetime includes the existing app callback131 and SessionsDiscard.pending decrement133. Current registration is never reread or retargeted, and enqueue/runner/task/weak-wrapper/pending behavior is otherwise unchanged. This matches native selected-registration ownership through its pending decrement; it does not prove a runtime failure previously occurred.

Production compiler command is now archived, targets the actual production source, uses Objective-C ARC and -fsyntax-only, and has recorded exit0 with an empty production log. Official check reports done and recorded exit0; diff check exit0 with empty log. Reviewer did not run compilers/builds/checks.

Scope remains local: precise lifetime ends before any future outer core-accounting decrement. No logical-root, setter registration, canonical rebind, service migration, shared API, ordinary-close activation or runtime acceptance is established. Only these review files were written.
