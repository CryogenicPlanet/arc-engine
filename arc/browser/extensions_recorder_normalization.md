**SessionID proposal rebased onto lead e7ef669. Not applied; acceptance remains unclean.**

The adjacent `extensions_recorder_normalization.patch` proposes changes only to
`tests/recorder/diff.py` and `arc/DIFF-ALLOWLIST.md`. The current diff-tool base
was verified byte-for-byte against commit `e7ef669`. Neither shared file, product
source, runtime helper, app/artifact, raw result, or source-frozen batch was
modified. No engine build, app launch, or build/run lock was used.

The earlier exact-factory exception is **superseded and removed**. The patch
contains no factory constant, classification hunk, or factory allowlist row.
The lead's metadata-based `boundary_methods` and `internal_ours` handling remain
unchanged. Its caller detection/filtering, complete report generation, counts,
and full callback sequence also remain unchanged.

The only proposed semantic normalization concerns two exact recorder fields:

| Recorder identity | Field |
|---|---|
| `->engine -[ArcSessionID identifier]` | Retained return value |
| `engine->app -[ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:]` | Call argument index 1; index 0 is the ArcBrowser |

One first-seen map is shared across these two fields per transcript, **after
the existing caller filter**. When both transcripts carry attribution, the
lead's filter keeps app calls and engine setters and excludes engine getter
reads. Excluded reads allocate no SessionID aliases. When one or both
transcripts predate attribution, the existing fallback remains in effect.
The proposal neither changes those policies nor reintroduces filtered events.

Only exact JSON integers `type(value) is int` from 1 through 2147483647 become
aliases. Chromium `SessionID::id_type` is `int32_t`, with validity `value > 0`
(`chromium/src/components/sessions/core/session_id.h:21-37`). Repeated IDs share
an alias and distinct IDs never do across the two fields. Different native
numbers with the same chronological alias pattern can compare equal across
runs. Alias collapse, alias split, and cross-field identity disagreement still
differ. Extra retained first-seen IDs can shift later aliases conservatively;
there is no best-fit permutation or repair of differing call sequences.

Valid aliases and invalid literals have separate tagged JSON representations.
Zero, -1, other negatives, oversized integers, bools, floats, strings, null,
lists, and dicts retain value/type under the literal tag and cannot impersonate
aliases. Within these exact fields, invalid literals also bypass the existing
string/object volatility masks, preventing UUID or object-marker strings from
turning into wildcards. Other fields keep the original normalizer. Callback
return values (extension group IDs), unrelated arguments/numbers, classes,
selectors, method counts, and ordering remain compared.

Validation used a temporary copied tree, including the pinned runtime metadata
and callback inventory needed by the lead's `boundary_methods`. The complete
proposal passed `git apply --check --whitespace=error`, was applied only in that
copy, matched the expected source byte-for-byte, and passed `python3 -m py_compile`.
A source assertion verified the entire caller-detection/filter prefix and
`boundary_methods`/`main` tail are byte-identical to the current shared tool.

**65 synthetic cases passed: 13 expected equal and 52 expected different.**
The actual CLI JSON reports and caller-attribution output were asserted.

| Checks | Result |
|---|---|
| Repeated/distinct/shared IDs, callback-first ordering, maximum int32, unchanged sentinel patterns | Equal alias patterns pass |
| Alias collapse/split or disagreement between identifier return and callback argument | Mismatch remains |
| Changed sentinels/negatives/oversized values; bool/int, float/int, string/int, null, list/dict collisions | Type/value differences remain in both target fields |
| Numeric strings, UUID strings, object-marker strings, alias-shaped dictionaries | No alias spoofing or new wildcard |
| Other field/class/selector/direction, callback extra slot and callback return | Unrelated numeric differences remain |
| Identifier/callback count difference and foreign callback insertion | Counts/missing/full sequence remain different |
| Six factory/public-method regression pairs | Reports exactly match the current lead baseline; no proposal-specific factory handling |
| Caller attribution enabled with matching alias pattern | Pass |
| Unequal excluded engine getter reads, or an unknown-origin read | Filtered reads consume no aliases; pass |
| Attributed app identifier collapse | Still fails |
| Engine setters and real app calls with differing counts | Retained and still fail; reports unchanged from lead baseline |
| Attribution missing from only one side | Existing attribution-off fallback preserved |
| Attributed factory call from app versus engine | Existing `internal_ours` classification / caller exclusion preserved exactly |

The original v1 proposal/evidence remains historical at:

`logs/wp/extensions-normalization-proposal`

The distinct v2 snapshot is retained at:

`logs/wp/extensions-normalization-proposal/v2-e7ef669`

It includes the former proposal/doc snapshots, current and proposed tool trees,
synthetic pairs and CLI reports, current-base diagnostics, source fingerprints,
and a SHA256 manifest verifying every preexisting v1 evidence file is unchanged.
The previous v1 diagnostic totals are not the current baseline: e7ef669 already
classifies nonboundary helpers separately.

Read-only historical launch3 diagnostics were repeated using the **current lead
tool** versus the temporary SessionID-only proposal. Inputs remained the
immutable launch3 copy and original golden transcripts. These older recordings
predate caller attribution, so the existing attribution-off fallback applied;
caller-on behavior was covered by the synthetic cases above.

| Launch | Tool | Missing | Extra | Returns | Callback args | Counts | Internal | Ours internal | Total noninternal |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| 1 | Lead e7ef669 | 13 | 16 | 13 | 3 | 109 | 9 | 20 | 154 |
| 1 | Temporary SessionID proposal | 13 | 16 | 12 | 2 | 109 | 9 | 20 | 152 |
| 2 | Lead e7ef669 | 14 | 13 | 11 | 3 | 21 | 9 | 20 | 62 |
| 2 | Temporary SessionID proposal | 14 | 13 | 10 | 2 | 21 | 9 | 20 | 60 |

Exactly two report changes occur in each launch: the `ArcSessionID.identifier`
return mismatch and its corresponding SessionID-bearing callback argument
mismatch disappear. There is no factory reclassification in this proposal.
Every internal/internal_ours/missing/extra/count list and the entire callback
sequence report remain unchanged from e7ef669. Every surviving return/argument
mismatch entry remains unchanged. Core missing methods and the 13/10 group-ID
callback count difference in launch 1 remain. Full callback sequences remain
106/145 and 46/87, both diverging at index 1 amid foreign bootstrap/crash callbacks.

Raw input SHA256 values were checked before and after; no results were rewritten:

| Input | SHA256 |
|---|---|
| `results/golden/arc-1.170-launch-slice/launch-1/record.jsonl` | `e9baeeaca8b3d5b8369a4a539f62285b2596c4c73315c9fbdcf7b0f50243a9ee` |
| `results/extensions-launch3/launch-slice/launch-1/record.jsonl` | `e8c8b791f8bdb786659f88290342dfc2171da21abb82bc5b494c0206ee095557` |
| `results/golden/arc-1.170-launch-slice/launch-2/record.jsonl` | `78f4f27c5c058a8f9f2911fcdce1038d52fdc695d125dd2657ad0ac2809d20e5` |
| `results/extensions-launch3/launch-slice/launch-2/record.jsonl` | `6f6df9b9ad37fc487ca61089300e6c3386d7d19f054142247a9accdb15d22cc3` |

Unchanged current shared bases:

- `tests/recorder/diff.py`: `f8df579e6ea23b4eb531f357fdfe0460fa57b563daf44f9d6858c73abf84b184`
- `arc/DIFF-ALLOWLIST.md`: `17f7846129013795c3eefdbf2a7b51b47f4857a3046d756c5d0abfbbf6716ecd`

This is a proposed normalization change and historical diagnostic evidence.
Acceptance is not clean. The lead must review/apply the shared patch and use
current acceptance recordings; no present engine/probe runtime was exercised.

A fresh read-only reviewer reported no findings in the rebased patch,
caller-filter preservation, masking scope, documentation, or retained evidence.
The reviewer performed no edits or code/runtime checks.
