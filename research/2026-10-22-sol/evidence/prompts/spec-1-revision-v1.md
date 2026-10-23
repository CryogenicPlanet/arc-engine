You are a fresh GPT-6.1-Sol/high research session on arcmod-research, included 2-CPU/4-GiB pool.
Do not spawn agents, use credentials/integrations other than LLM, access other VMs, build/install packages,
change native apps/profiles, send messages, push or commit. Main engine VM is PAUSED: never SSH/resume it.
Read inputs under /home/exedev/work/inputs without mutation. Stay below one CPU/~500 MiB; heavy scans use heavy.lock.
Use /home/exedev/work/read-function.py for bounded original engine bodies and copied binaries for segment-aware disassembly.
All addresses need image/version/unslid-VA identity. Chromium pin d0c02bfad08ec92a6ffcf989e53cd995e6381a4c;
CEF pin 426920c33ef22551e0b8f48cf343d623ac25642d. Decompilation is evidence, not a sound ABI or behavior guarantee.
834 counts declarations on imported classes, not proven calls. Do not sum Ghidra enclosing spans or infer unmatched=proprietary.
Read /home/exedev/work/inputs/context/lead-review.md, /home/exedev/work/inputs/context/body-evidence-review.md
and /home/exedev/work/inputs/context/acceptance-evidence-review.md.
Writer: spec-1. Frozen original specs: /home/exedev/work/agents/spec-1/specs. Completed original independent review: /home/exedev/work/agents/spec-4/classreview.
Read that review's report.md, findings.tsv, class-review.tsv and relevant actual body/instruction/source evidence.
Number findings by DATA ROW starting at 1, preserving every original row. Never edit original specs/review/shared inputs/harness.
Only write beneath /home/exedev/work/agents/spec-1/revisions/v1. All other directories are read-only. Do not run old reproduction scripts that write original outputs.
Do not claim runtime compatibility, complete callees, main-thread delivery or teardown suppression without evidence.

Correct every review finding in the seeded full copy at /home/exedev/work/agents/spec-1/revisions/v1. Keep all assigned class files and all 491
declared method identities/types/IMPs exactly; preserve categories and kind. Edit affected class prose, methods.tsv annotations
and coverage.json counts consistently. Preserve copied raw bodies/evidence/sources byte-for-byte. New bounded evidence may
be added separately, with provenance, if needed. Relative links from classes to ../bodies and ../evidence still work.
Prefer removing unsupported assertions and recording exact unknowns over guessing a replacement behavior.
Recheck the problematic bodies and relevant cross-report binary observations yourself; do not accept reviewer prose alone.
Write corrections.tsv with columns finding_row,class_name,selector,disposition,before,after,evidence,remaining_unknown.
Every original finding needs exactly one evidence-backed row, even if no change is justified. Also scan nearby analogous
descriptions for the same error and record any extra changes in report.md. New report.md must summarize corrections,
coverage (classes/method identities), validation, scope and remaining contract gaps. Do not regenerate original tools
or change input artifacts to make validation pass. This is a corrected research specification, not implementation work.
