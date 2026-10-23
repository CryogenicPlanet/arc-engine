# Private-index racy timestamp successor (proposal only)

This distinct proposal applies directly after the frozen dabc wrapper
`dabcc5255e59c45d64555f8f1adea81e6fc2f329a22349fe2738513919f5b151`
with helper absent. The 303 and deterministic-environment26 proposals, sources,
tests, freezes and reviews remain unchanged. Both are historical inputs rather
than an adoption claim for this successor. No production Git, source, tool,
index, configuration, epoch capture, lock, build or browser action occurred.

The reproduced defect is in
`logs/wp/webextras-generator-adoption-private/copied-index-racy-probe.json`:
the same raw index with its original mtime yields the dirty patch, while a
later-mtime copy can conceal same-second, same-size working edits.

[Git's racy-index documentation](https://git-scm.com/docs/racy-git.html)
explains that Git uses the index timestamp when deciding whether apparently
unchanged stat data needs a content comparison. On normal index rewrites, it
also invalidates cached size for racy entries so a later timestamp cannot hide
those edits. The fixture verifies actual Git behavior rather than assuming
that every private add-N takes the same path.

## Minimal source change

The wrapper is byte-identical to the environment26 successor. The helper differs
only by these statements immediately after its existing private `copyfile`:

```python
# The index timestamp is part of Git's racily-clean detection state.
os.utime(private, ns=(private.stat().st_atime_ns, identity[5]))
require(private.stat().st_mtime_ns == identity[5], 'private index timestamp not preserved')
```

`identity[5]` is the mtime in the already captured original real-index identity
tuple (`resolved path`, hash, device, inode, size, mtime, ctime). Existing checks
still prove the original identity before copying and prove its identity plus
copied byte hash afterward. The explicit timestamp equality fails closed if the
filesystem cannot preserve that precision. It precedes installing REPO_ENV and
every Git operation that reads the private index, including private add-N.
Only the copy receives `utime`; the real index and sources receive no repair or
restoration. Copy2 is deliberately unnecessary: no unrelated mode, access-time,
ownership or extended-attribute metadata is copied.

The fixed exact App8 environment, helper names/signatures, independent real
index discovery, known nested gitlink policy, private manifest ITA, all existing
source/index/HEAD/flag/binary guards, output preparation, ownership and rollback
semantics remain unchanged. A legitimate later private-index write by Git may
advance its mtime; the first-read equality is the required initial condition.

## Evidence and fixture boundaries

Run `python3 -B arc/browser/webextras_nested_regeneration_racy_test.py`.
The test replays all62 predecessor regression groups against copied successor
tools. Its only structural-oracle adaptation replaces the now-obsolete
"ENV is the sole helper change" check with exact source equality to environment26
plus the three-line timestamp fragment and exact App8 equality. All52 original
method bodies remain unchanged; the predecessor's five explicitly documented
private runpy fault adapters remain. The proposal filename and evidence root
are rebound to this new prefix. No frozen predecessor evidence is overwritten.

Seven additional groups use real Git with disposable outer and known nested
repositories. The four positive matrices combine new manifest leaf absent or
present (requiring private add-N) with normal copy or a simulated three-second
later copy timestamp. Each requires exact patch bytes, tracked-edit content,
nonempty output, exact original mtime at each repository's first private Git
read, and protected source/index identities. Their setup verifies actual
cached, working-file and index timestamp seconds match; up to three bounded
setup attempts are recorded. They do not move source timestamps to avoid the
racy condition. A separate Git-generated oracle with independently preserved
copy mtime checks private add-N output and the racy tracked entry's zero cached
size before producing its expected patch.

The old26 delayed-copy control returns success with an empty patch when no new
leaf exists: both tracked edits are missing. With private add-N, this real Git
refreshes/smudges those entries and old26 produces the correct patch. The test
records that rescue as a positive control, not an invented failure. The fixed
candidate must pass both branches. The remaining controls require failure
before any private Git call when mtime preservation is suppressed, and prove
the source delta is exactly the timestamp fragment.

Protected identity means SHA256, device, inode, size, mode, uid, gid, mtime and
ctime. Ordinary Git and file reads advance access-time on the fixture filesystem;
access-time is recorded separately and never restored. Early overstrict-atime
fixture failures remain in `test-access-time-overstrict.log` and
`racy-focused.log`. `racy-focused-fixed.log` preserves the initial mistaken
expectation that old26 must also fail with private add-N; final tests correctly
record its real rescue. Neither correction changed the candidate helper.

Commands, setup timestamps, first private reads, Git version, byte hashes,
access-time observations and exact old/new controls are in adjacent racy
evidence. `freeze.json` pins final sources and results. These tests qualify only
copied scripts/disposable repositories on the recorded Git implementation.
Independent review and renewed exact owner authorization remain required;
no actual tool adoption, current epoch or production patch-output claim follows.
