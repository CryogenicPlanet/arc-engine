# Deterministic Git environment successor (proposal only)

This is a distinct two-file proposal directly after frozen dabc
`dabcc5255e59c45d64555f8f1adea81e6fc2f329a22349fe2738513919f5b151`
and helper absent. It does not modify or supersede the bytes of the reviewed
303 proposal, its tests, or its freezes. No production generator, Git command,
index, source, config, lock, build, or browser was used for this preparation.
Owner adoption and independent review remain required.

The prior evidence at
`logs/wp/webextras-standalone-generator-adoption-plan/environment-probe.json`
shows why 303 is insufficient: it removes caller `GIT_*` suppression while
retaining HOME/XDG. Disposable HOME configuration and attributes select
different xfuncname hunk labels for the same source bytes. This successor uses
the parent's prescribed exact App8 environment for wrapper discovery and every
helper Git subprocess:

```
PATH=/usr/bin:/bin
LANG=C
LC_ALL=C
GIT_OPTIONAL_LOCKS=0
GIT_CONFIG_NOSYSTEM=1
GIT_CONFIG_SYSTEM=/dev/null
GIT_CONFIG_GLOBAL=/dev/null
GIT_ATTR_NOSYSTEM=1
```

The wrapper fixes PATH before locating its physical root and uses `/usr/bin/env
-i` with those eight assignments both for Git root discovery and helper launch.
The helper uses a literal identical dictionary. Its private copied-index
commands alone append the internally selected `GIT_INDEX_FILE`. Caller HOME,
XDG, Git config/attribute suppression and executable selection do not enter
these commands. Repository-local configuration remains available to Git, as in
the App8 baseline; this proposal does not claim isolation from local config or
tracked repository attributes. The wrapper now deliberately uses the system
Python resolved through `/usr/bin:/bin`, rather than a caller Python on PATH.

Relative to frozen 303, only the wrapper environment setup/discovery/launch and
helper ENV assignment change. The regression checks compare the entire helper
AST after removing the ENV assignment. All public helper names/signatures,
known nested-repository routing, independent private index copies/manifest ITA,
HEAD/flag/binary/source guards, output preparation/publication/rollback and
source/index preservation remain unchanged. There is no sync-chromium change.

## Isolated regression replay

Run `python3 -B arc/browser/webextras_nested_regeneration_environment_test.py`.
Every execution uses copied tools and disposable repositories. The new test
source loads all 52 frozen303 test method bodies unchanged, rebinding their
paths to private copies. Frozen source fixtures, the original sync script
`5973cb9d53fde94f969362b033fbb645fe840e63283a1983829e4621db720923`,
the old test/utility source, dabc baseline and 303 copies are pinned in
`logs/wp/webextras-nested-regeneration-environment/input-pins.json`.

The original suite's live-production input guard is intentionally replaced by
private-copy preservation plus read-only preservation of those frozen inputs.
It does not read current production tool/index state. Its temporary directories
are constrained beneath one owned disposable root.

Five existing real drift injections depended on caller PATH selecting a fake
Git executable. The candidate correctly ignores that PATH. A test-only copied
runpy adapter intercepts only those five explicit private fake-Git subprocess
sites while preserving the candidate's exact environment passed to Git. It
does not modify the candidate, relax its environment, or affect normal wrapper
or environment tests. The original assertions still require the injected index
byte/inode, source byte/mode and destination changes to occur and remain
preserved. Adapter uses and commands are recorded separately.

Ten additional groups cover exactApp8 dictionary/unchanged helper AST,
copied-dabc byte equivalence, HOME and XDG configuration, explicit global and
HOME/XDG default attributes, hostile caller config and PATH, root-discovery
worktree poisoning, and reproduced old303 HOME/default-attribute counterexamples.
Default-attribute controls put driver definitions in disposable local config,
so global-config suppression alone cannot make those controls vacuous. Both
wrapper and direct helper are exercised for the successor. All source and
real-disposable-index physical identities are checked before/after these runs.

An early replay used the historical speculative sync postimage by mistake;
`test-wrong-sync-fixture.log` preserves that failed harness run. The final suite
uses the pinned original sync preimage above. No candidate change was needed
for that harness correction.

Final results, strict reconstruction, shell/Python parsing, source preservation
and hashes are recorded in the adjacent environment evidence directory and
`freeze.json`. These are private fixture results, not an actual integration,
production patch oracle, runtime result, or shared-tool owner grant.
