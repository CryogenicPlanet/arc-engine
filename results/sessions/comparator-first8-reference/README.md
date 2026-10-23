# Accepted first-eight comparator: historical sessions references

**Offline comparison only. No new engine, app, build or runtime.** Four immutable v4 raw pairs are compared with default CLI using a private copied accepted comparator bundle: diff24437bed + omnibox_receivers3af958f6 + google_q67fb19cf + download_identity5318e64f. All helpers are copied/pinned before execution. No fixture admission, omnibox options or boundary-argument opt-in is used; no allowlist/source/shared output is changed.

| Pair | Saved f8df methods differing | Accepted24437 | Callbacks golden/ours |
| --- | ---: | ---: | --- |
| launch-slice1 |58 |56 |106/133 |
| launch-slice2 |45 |43 |47/63 |
| sessions-public-api1 |101 |101 |122/123 |
| sessions-tab-restart1 |97 |97 |158/178 |

The four new default first-two-lines are in `diff-first-two-lines.txt`; complete TXT/JSON plus interpreter/import receipts are under `accepted-first8/`. Four additional old-f8 default controls use the same copied metadata and raw pairs. Every old control reproduces the original v4 TXT **and JSON byte-for-byte**. All eight CLI exits are0: that establishes successful offline tool execution, not parity.

## Exact change in sessions-related rows

Only two report sections differ for each pair, and every changed entry is either `ArcSessionID.identifier` return or `ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:` argument1. All other report sections, method counts, missing/extra rows and callback sequence are identical in value. Every callback sequence still diverges first at1.

For each launch pair, the previous call0 numeric identifier return mismatch and call0 group-query argument mismatch disappear. Total differences decrease by2. The raw integers and calls remain preserved.

For public, those same call0 entries are replaced by later structural differences: identifier call6 has aliases `{native_session_id:3}` versus2, and group-query call14 has3 versus2. For restart launch1, identifier call9 has4 versus2 and group-query call14 has3 versus2. Indices are zero-based. Totals therefore remain101/97; these are NOT clean canonical diffs. Exact old/new entries and corresponding raw seq/thread/depth/getter returns are in `analysis.json`.

The accepted comparator uses a transcript-local bijection only for positive int32 SessionID integers in those exact typed positions. Invalid values remain typed literals; this does not discard counts, callback order, close timing, getter/primitive facts or other scalar fields. Download identity is also default-active in the accepted bundle, so the experiment is attributed to the complete accepted comparator; for these specific pairs its only observed report changes are the two typed SessionID-related entry families above. Optional receiver/Google modules were not imported; copied download helper source import is evidenced in each accepted command's private Python import trace.

## Preserved runtime limitations and provenance

Raw hashes match the prior canonical audit; frozen golden hashes match v4's inputs.expected.json. Original v4 reports, steps, coverage, sidecars, input/source receipts and framework receipts are copied, not overwritten. The strict canonical first-close failures remain unchanged historical evidence: ours closes return before genuine callbacks, both scenarios stop at their first close assertion, and no ours eligibility/restore API or actual restart launch2 is reached. New normalization does not repair that contract or establish restored histories/canonical acceptance.

The v4 runtime source was178091e6; this is retained provenance, not a claim about a newly built/current engine. Its built/signed framework receipts remain historical7536f5e2/dc446842. Comparator/metadata/allowlist/architecture copies are new reference inputs. V4 did not independently pin the metadata TSV or callback inventory. We freeze current accepted metadata9ba3d475/52b3687b (matching the prior independent accepted-comparator reference), and old-f8 reproduction proves effective output equivalence for these four pairs under it. It does NOT reconstruct the historical metadata hash seal or a full native/toolchain closure.

Every selected origin and private copy is rehashed before each CLI and after execution. No input drift was detected; no private bytecode directory is created. `frozen-inputs.json` preserves source/copy hashes/inodes, `execution.json` exact8 commands/interpreter-flags/cwd/import/output receipts (the full inherited environment is not sealed), and `manifest.json` output hashes. Existing v4/canonical review artifacts and all shared files remain unchanged. No apps, compiler/build, queue/lock, Git/index, Chirp, opt-in or allowlist edit occurred.
