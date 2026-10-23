Delivered [report](/home/exedev/work/agents/cef/initial/report.md), [selector mapping](/home/exedev/work/agents/cef/initial/cef-mapping.tsv), [counts](/home/exedev/work/agents/cef/initial/category-counts.tsv), and [decompile requests](/home/exedev/work/agents/cef/initial/decompile-requests.tsv).

All 834 declared methods across 63 imported classes are classified: 11 direct CEF mappings, 139 partial, 317 Arc wrapper/value contracts, and 367 unresolved. No Chrome-internal dependency is yet confirmed. The pinned CEF source targets Chromium 156.0.8078.48 exactly.

Arrived engine excerpts confirm concrete native-pane hosting requirements. Backend and callback semantics still gate route selection; app selector calls lack receiver proof. No rewrite estimate is justified.

Validation passed in 9.73 seconds, using one CPU and approximately 73.4 MiB RSS:

```bash
/usr/bin/time -v bash initial/tools/run_local.sh \
  > initial/evidence/reproduce.log \
  2> initial/evidence/reproduce-resources.txt
```

Next: resolve profile, extension-install, password-save, and autocomplete targets listed in [followups](/home/exedev/work/agents/cef/initial/decompile-followups.tsv).