Completed all 34 class specs and 491 method rows.

- [Report](specs/report.md)
- [Method evidence](specs/methods.tsv)
- [Coverage](specs/coverage.json)

Key findings: native embedding requires Arc’s exact pane-host contract; two category overrides change behavior; three decompiler artifacts were repaired with ARM64 checks.

Validation passed: 491 metadata records, 435 indexed bodies, and three app-call samples. Peak RSS: 45,144 KiB.

Exact run:
```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash scripts/reproduce.sh > specs/validation-run.log 2> specs/validation-resource.log
```

Sixteen methods remain semantically unresolved. Runtime equivalence remains untested. Next priorities are delegate callback ownership, native-host teardown, and C++ aggregate ABI compatibility.