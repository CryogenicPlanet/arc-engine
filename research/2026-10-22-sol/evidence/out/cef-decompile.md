Completed [decompile/report.md](/home/exedev/work/agents/cef/decompile/report.md).

Evidence favors investigating an in-tree Chromium bridge: observed paths reach Chrome profiles, omnibox, extensions, password saving and native compositor internals. CEF equivalence remains unproven.

Updated mappings and reproducible tools are under `decompile/`. Five unresolved callback/installer targets are listed in [additional-shards.tsv](/home/exedev/work/agents/cef/decompile/additional-shards.tsv).

Validation: `bash decompile/tools/run_local.sh` passed—199 shard hashes, 100 extracted bodies, 4.98 seconds, about 72 MiB RSS. No runtime compatibility tests were performed.