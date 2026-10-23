# Latest canonical sessions-public-api review

Known P2 close-timing mismatch reproduced; no new fixture P1/P2. Both raw SHA256 values and all supplied triple rows match exactly.

- Accepted original:22/22 pass, accepted=true/errors=[], raw6136 close→6138 genuine callback→6483 callback return→6489 close return.
- Latest ours:10 pass/1 fail/1 inconclusive, accepted=false/errors=[], raw6434 close→6437 return→6484 genuine main callback→6829 callback return. Same selected WC; callback occurs exactly once. Immediate sidecar closedCount=0.
- Both cleanup steps pass ordinary Browser.close with browser+9 helper exits. Ours PID82484, original PID37402. No fallback cleanup is credited.
- Ours has zero public restore requests; fixture aborts at the synchronous-close assertion. Missing restore in standard diff is cutoff coverage, not an observed failed restore.
- Standard diff:93 differing methods(6 missing,1 extra,14 returns,7 callback args,65 counts); callback sequence122 original vs120 ours, first divergence1. Verbatim first two lines are adjacent.

Source-freeze limit: transcripts are SHA-pinned, but this latest artifact tree contains no per-run before/after framework/source freeze manifest. Do not attribute this observation to an exact global revision or current on-disk source. No acceptance promotion, app/build action, source edit, new probe, or optional created-close retry occurred.
