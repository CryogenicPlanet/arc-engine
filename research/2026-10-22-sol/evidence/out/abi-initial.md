Completed [initial/report.md](/home/exedev/work/agents/abi/initial/report.md) and the requested [contract](/home/exedev/work/agents/abi/initial/contract.tsv), [call evidence](/home/exedev/work/agents/abi/initial/used-calls.tsv), and [decompile requests](/home/exedev/work/agents/abi/initial/decompile-requests.tsv).

- Confirmed **834 declarations on 63 imported classes**: 828 encodings match; six older signatures remain unresolved.
- Added app call-site evidence and limited analysis of newly arrived engine shards.
- Validation passed via `bash scripts/regenerate.sh` and the final `scripts/validate.py` run.

Behavioral compatibility remains unproven. The next specific missing body is `_CallWithEHFrame`’s target at **`0x2e0238`**; callback threading, ownership and teardown questions are recorded in the report.