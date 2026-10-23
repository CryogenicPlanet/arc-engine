Completed [decompile/report.md](/home/exedev/work/agents/abi/decompile/report.md) with 14 findings covering unwind behavior, delegate removal, callback ownership, navigation policy, filename selection and DevTools.

Evidence exposed closure-counting defects and nine additional suspect metadata regions. Six missing callback/helper headers need repaired function boundaries; requests are in [additional-shards.tsv](/home/exedev/work/agents/abi/decompile/additional-shards.tsv).

Validation **PASS**: 199 shard hashes, 567,959 headers, 36 selected bodies and 1,228 instruction bytes. Reproduce with:

```sh
flock -w 60 /home/exedev/work/heavy.lock /usr/bin/time -v bash decompile/tools/regenerate.sh
```

Runtime equivalence, timeout behavior, exactly-once completion and callback thread identity remain unresolved. No implementation changes were made.