# Arc engine reviewed evidence stage

This is a newly staged package, pending lead review and publication. Its paths
mirror the intended arc-engine repository: specs/<Class>.md and
research/2026-10-22-sol/evidence/. No final destination was modified.

All 146 class documents cover 1,964 exact declared identities, including category,
kind, encoding and IMP. Coverage is distinct from behavioral/runtime equivalence.
The readiness manifest, eight correction/review completion proofs, completed
global synthesis with frozen input/output/final-output proof, frozen inputs,
original drafts/reviews, corrected revisions and role evidence are retained.
Raw TSV/JSON provenance and original Markdown are byte-exact. Only specs/*.md are
presentation copies with relocated links. Address-named bodies retain their
writer, stage and revision namespaces; equal addresses are never deduplicated.

relocation-manifest.json records original Linux paths/hashes and output hashes,
including explicit omissions and link issues. BUNDLE-README.md and SHA256SUMS
live inside research/2026-10-22-sol/evidence/. Checksum paths are relative to the
repository root. Verification covers exactly specs/Arc*.md and every file under
that evidence directory except SHA256SUMS itself. It ignores unrelated repository
files, including the repository README.md and optional specs/README.md index.
The same verify command works in a new stage or the existing repository after
the lead publishes these two trees. Hash consistency is not an independent
authenticity signature.

Offline verification (Python 3.9+):
    python3 research/2026-10-22-sol/evidence/_package_tools/package-evidence.py verify .

The quality warning table is losslessly compressed. To inspect without mutating
the package:
    gzip -dc research/2026-10-22-sol/evidence/agents/quality/initial/warning-bodies.tsv.gz > /tmp/warning-bodies.tsv
Its original SHA256 is recorded and checked on decompression. The large CEF
build-index catalog is omitted with its original SHA256 and reason recorded.

This package supports offline integrity and declared-coverage checks. It is not a
self-contained rerun environment: original scripts use /home/exedev/work Linux
paths, pristine image binaries, full decompile shards/index, source trees, and
some network fetches. Large binaries, full corpus, SQLite caches, Ghidra projects,
profiles and credentials are excluded.
Bounded protocol-audit instruction excerpts under agents/protocols/initial/bodies/
are retained byte-for-byte with SHA256; only engine/app VA filenames up to 4096
bytes each qualify. These are evidence snippets, not complete executable images.
Two exact zero-byte reviewer artifacts, agents/review/9 and
agents/spec-1/revision-reviews/v1/9, are retained and hashed as original files.
Only those paths qualify, and verification requires that both remain empty.
Chromium source links are pinned upstream
when retained provenance establishes the pin; other absent links are reported in
link-issues.json. Original historical Markdown can have Linux paths and broken
relative links. Missing raw-link dependencies are recorded in raw-link-issues.json,
never silently changed in machine evidence.

The verifier checks local presentation links; unresolved links explicitly listed
in the manifest are reported as limitations. Raw Markdown is provenance, not a
relocated presentation. Review every reported unresolved link before publishing.
