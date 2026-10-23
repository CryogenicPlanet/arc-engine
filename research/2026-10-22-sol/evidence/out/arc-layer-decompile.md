Completed [decompile/report.md](/home/exedev/work/agents/arc-layer/decompile/report.md).

- Traced bootstrap, profiles, content creation, native hosting and IPC paths.
- Audited the 5,846-row handoff; no code-size estimate is justified.
- Recorded [13 recovery targets](/home/exedev/work/agents/arc-layer/decompile/additional-shards.tsv) and unresolved contract questions.

Validation **PASS**: 199 shards, 67 exact body copies, eight new app sends and four source-backed identities. Initial evidence remains intact.

Reproduce with `flock /home/exedev/work/heavy.lock bash decompile/tools/reproduce.sh`.

Runtime ABI, callback ownership and full coverage remain unproven.