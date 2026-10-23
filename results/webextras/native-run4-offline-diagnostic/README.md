# Native4 offline diagnostic

Held comparator and driver wiring were applied only in the temporary source tree listed in input-manifest.json. Active source files, shared comparator and drivers were untouched. This is diagnostic evidence, not runtime acceptance.

The frozen comparator capability canary passed. All raw call/return caller attribution and ancestry scans passed for both recordings; each has only the enclosing ArcBrowserApplication.run lifecycle call left open. Every action restores its ambient stack and validates public offsets, receiver identity, forwarding and actual callback ancestry. Both sides contain exactly 13 fixture calls and 13 returns across 11 actions.

| Action | Fixture pairs each | Methods differing | Projected events original/ours | Public shape equal | Outcome/timeline equal |
|---|---:|---:|---:|---|---|
| console-open | 1 | 10 | 140/160 | False | False |
| frontend | 1 | 25 | 52/2 | False | True |
| console-close | 1 | 9 | 48/54 | False | True |
| panel-closed | 1 | 0 | 2/2 | True | True |
| execute-evaluate | 1 | 0 | 2/2 | True | True |
| execute-malformed | 1 | 0 | 2/2 | True | True |
| execute-unknown | 1 | 1 | 2/8 | False | True |
| subscription | 3 | 0 | 6/6 | True | True |
| media-types | 1 | 0 | 2/2 | True | True |
| pip-request-error | 1 | 0 | 2/2 | True | True |
| pip-exit-error | 1 | 0 | 2/2 | True | True |

Seven actions have zero official comparator differences, matching projected public shapes, and matching outcome/timeline payloads: panel-closed, execute-evaluate, execute-malformed, subscription, media-types, pip-request-error, pip-exit-error.

Console-open retains the real display mode and timing mismatch: original mode3 after return, ours mode4 before return. It also differs in public WebContents calls and nativeView return class (WebContentsHostView versus WebContentsViewCocoa).

Frontend retains 25 original engine set* calls missing from that action in ours. Those are selected public events, not suppressible helper differences; initialization occurs in different action windows.

Console-close has public setter/getter differences and one original crash-key callback absent in ours. Execute-unknown has three additional ambient crash-key callbacks in ours. These remain flagged by the existing official callback policy.

The metadata projection removes helper/getter interleavings only. It preserves the four differing action shapes; no event suppression or alternate normalization was introduced. Full per-action first-two comparator lines and actual admitted-image counts are in diagnostic-summary.json and each *.diff.txt/.json. Exact path-level projected and semantic differences are in validation-and-projection.json / diagnostic-summary.json.

Production run4 summary remains complete=false/parity=false pending shared explicit fixture-image comparison adoption; the diagnostic does not override it.

Frozen raw inputs, metadata/callback table, comparator, helpers and held patches are archived with SHA256/identity manifests. Executed patched comparator and driver copies were isolated under /tmp.
