Delivered [report.md](/home/exedev/work/agents/matcher/initial/report.md) and corrected tools.

- Ran the matcher: 512 unknown pairs remain ambiguous; no validated exact/high-confidence matches.
- Overlapping Ghidra spans block reliable body attribution. Named controls expose false positives from short signatures.
- All 15 regression checks pass; peak RSS was 250 MiB.

Artifacts: [matches.tsv](/home/exedev/work/agents/matcher/initial/matches.tsv), [validation.json](/home/exedev/work/agents/matcher/initial/validation.json), [56 prioritized requests](/home/exedev/work/agents/matcher/initial/decompile-requests.tsv).

Validation: `python3 tools/test_matcher.py --out initial`; exact scan commands are recorded in the report.

Next: obtain actual Ghidra body ranges for the ten oversized spans and the requested engine shards. Current mismatches cannot establish Arc modifications or maintenance cost.