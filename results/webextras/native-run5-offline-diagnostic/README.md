# Native5 offline diagnostic

Offline evidence only. The proven explicit-fixture comparator from native4 was copied and executed in a new temporary tree. Current driver validators/projection and all comparator metadata, callback inputs, helpers, raw recordings and observations were frozen at load. Production/shared/proposal files were untouched.

Capability canary passed. All full-raw caller identities and matched call/return ancestry passed (original6626 rows; ours6684 rows). Only the enclosing ArcBrowserApplication.run lifecycle call remains open on each side. All11 action offsets, expected selector counts, receiver checks, callback forwarding and actual ancestry checks pass. Each engine has exactly13 fixture calls and13 returns: subscription3 pairs, each other action1 pair. All outcome validators pass.

| Action | Fixture pairs each | Methods differing | Projected events original/ours | Public shape equal | Outcome/timeline equal |
|---|---:|---:|---:|---|---|
| console-open | 1 | 62 | 168/206 | False | False |
| frontend | 1 | 25 | 52/2 | False | True |
| console-close | 1 | 3 | 48/42 | False | True |
| panel-closed | 1 | 0 | 2/2 | True | True |
| execute-evaluate | 1 | 0 | 2/2 | True | True |
| execute-malformed | 1 | 0 | 2/2 | True | True |
| execute-unknown | 1 | 0 | 2/2 | True | True |
| subscription | 3 | 0 | 6/6 | True | True |
| media-types | 1 | 0 | 2/2 | False | True |
| pip-request-error | 1 | 0 | 2/2 | False | True |
| pip-exit-error | 1 | 0 | 2/2 | False | True |

Counts: official comparator clean8/mismatching3 actions; projected public shape clean5/mismatching6; normalized outcome/timeline payload clean10/mismatching1. Nothing was renumbered per action or suppressed.

The single outcome/payload mismatch is console-open display mode: original3, ours1. Both now forward the display callback after public return, so timing agrees.

Console-open also differs in public boundary state/returns and initialization: readiness true/false, native view class WebContentsHostView/WebContentsViewCocoa, viewport 698x716/0x0, background ArcColor4f/null, visibility0/2. It has6 missing,32 extra,5 return,1 callback-argument and18 count mismatches (62total); native crash-key and thumbnail callbacks appear only in original in this action. These are retained public differences.

Frontend has25 original public engine set* calls absent in ours during this action. The projected event counts are52/2; wrapper initialization occurs earlier in ours. Public setter placement is not a removable private-helper difference.

Console-close differs by3 methods: original isReadyToDisplay getter/setter and one crash-key callback. Its forwarding, close timing and outcomes agree. Execute-unknown is fully clean here, unlike native4 ambient crash-key variation.

Media-types and both PiP error actions have zero official comparator/outcome differences. Their only projected differences are the two receiver fields per action: original MediaSession#1 versus ours#2. A selected frontend mediaSession/setDelegate call during ours console-open introduced another MediaSession receiver before these fixture actions. All3 fixture calls remain on the same raw receiver within each engine (original#128, ours#91). The global projection deliberately preserves this ordinal difference; it is not silently reset per action and alone does not show that the fixture owner changed.

Private helpers, engine getters and native pointer constructors are projected only under the existing frozen metadata/caller policy. Selected app getters, engine setters and all app callbacks remain visible. Exact per-action first-two comparator lines and admitted counts are in diagnostic-summary.json and *.diff.txt/.json; full public shapes and all path-level differences are preserved.

All16 original/frozen input SHA256 checks passed at completion. Executed driver/projection hashes match the frozen active sources; comparator capability and frozen input manifest are archived. Shared comparator remains unsupported/pending; native5 production complete/parity remain false. No acceptance claim.
