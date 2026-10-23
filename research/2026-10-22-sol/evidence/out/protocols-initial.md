Completed [initial/report.md](/home/exedev/work/agents/protocols/initial/report.md), all 26 protocol specs, the 200-row callback audit, reproducible tools and evidence.

- 189/195 candidate callback/service identities have static send evidence; six remain unresolved.
- Five DevTools getter identities are classified as values.
- All six older encoding unknowns resolve to matching raw encodings and relevant ObjC layouts.

Validation **PASS**: 36.20 seconds, 68,220 KiB peak RSS.

```sh
flock /home/exedev/work/heavy.lock /usr/bin/time -v bash initial/tools/reproduce.sh > initial/validation-run.log 2>&1
```

[Next requests](/home/exedev/work/agents/protocols/initial/next-requests.tsv) identify 15 boundary repairs and six unresolved selector routes. Receiver execution, callback lifetime/threading and full behavioral equivalence remain unproven.