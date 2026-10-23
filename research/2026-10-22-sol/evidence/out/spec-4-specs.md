Completed **38 class specs and 491 method rows**, covering 418 unique IMPs.

Artifacts: [report](specs/report.md), [methods](specs/methods.tsv), [coverage](specs/coverage.json), [class specs](specs/classes/).

Validation passed: exact metadata reconciliation and 505 indexed body comparisons. Run from `/home/exedev/work/agents/spec-4`:

```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash specs/scripts/reproduce.sh
```

Key findings concern category overrides, profile services, native side-panel/PiP hosting and BoostIDE’s WebUI callbacks. Runtime equivalence remains untested. Next priorities are resolving delta cleanup ordering at `0x4402480`, native view ownership and callback shutdown/threading contracts.