Completed all **37 class specifications and 491 declared method rows**, preserving category and class/instance identity.

- [Report and findings](/home/exedev/work/agents/spec-3/specs/report.md)
- [Method inventory](/home/exedev/work/agents/spec-3/specs/methods.tsv)
- [Coverage reconciliation](/home/exedev/work/agents/spec-3/specs/coverage.json)
- [Exact follow-up address requests](/home/exedev/work/agents/spec-3/specs/evidence/address-requests.json)

Validation passed: **503 byte-exact body snapshots**, 11 pinned source files, and no missing declared IMP bodies. Exact run:

```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash tools/reproduce.sh > specs/evidence/validation-run.log 2>&1
```

Native embedding, profile services, navigation identity, and callback lifecycle remain compatibility gates. **79 rows remain behavior-unresolved**; runtime compatibility is unverified. Next priorities are the four missing navigation callback targets and ownership, cancellation, and callback-thread guarantees.