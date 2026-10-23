# Distinct native-sanity resource checker correction

Status: PRIVATE aggregate replacement, pending fresh review. Preserves the original
`webextras_frontend_resource_check.py` ca14402b, its test/notes, e80d7cc1 freeze and all
review evidence unchanged. The original helper is HOLD: its permissive Grit reader
accepted encoding255 and a terminal offset beyond EOF when intermediate/built/packaged
PAKs were identically mutated. Those exact independent findings are recorded in
`logs/wp/webextras-frontend-resource-review-worker/independent-negatives.json`.

Use this distinct helper only after review:

```
python3 arc/browser/webextras_frontend_resource_checked.py --expect-host-gate present --output /Users/cryogenicplanet/general/arc-engine/logs/wp/<fresh-receipt-directory>
```

The CLI and all earlier fixed-source/ancestor-lock/output restrictions are preserved.
`--expect-host-gate present|absent` remains required. Forward frontend integration uses
present; rollback to old source uses absent. This changes no executor, shared source,
build graph, runtime, package or resource file. Only private synthetic fixtures have
been read or hashed; no actual generated/frontend/framework PAK content was checked.

## Minimal validation before the official parser

The aggregate adds one bounded header/table validator before calling the official Grit
PAK reader on any intermediate, built or packaged PAK. It does not replace the official
reader or add a second payload/resource parser. The existing exact generated/GRD-input/
intermediate/built/package comparisons, bounded compression decoding, input stability
checks and artifact receipts remain unchanged.

The native reference is exact current Chromium156 `ui/base/resource/data_pack.cc`
LoadImpl and SanityCheckFileAndRegisterResources, packed Entry/Alias in data_pack.h,
and the encoding enum in resource_handle.h. Their exact source hashes are enforced and
archived; a changed reference source fails for review. On the fixed V5 build scope:

- require a complete12-byte V5 header, encoding BINARY/UTF8/UTF16 (0/1/2);
- require header + (resource_count+1)*6 + alias_count*4 to fit in the file;
- require every entry offset, including the terminal entry, at or before EOF;
- require offsets to be nondecreasing, including the final adjacent pair;
- require every alias index strictly less than resource_count.

These are the actual native loader checks for V5. Counts are uint16 and arithmetic is
bounded before constructing the tables or invoking the Grit parser. Equal offsets and
zero-sized resources remain valid. No invented check requires a zero terminal ID,
zero padding, minimum payload offset, terminal offset exactly at EOF, or absence of
trailing bytes. Native acceptance of these fields is exercised independently.

**Explicit version scope:** C156's native loader supports V4 and V5; both current
reviewed writers emit V5. This checker intentionally supports generated V5 only.
V4 is rejected as an unsupported checker policy, not represented as corrupt or natively
rejected. The tests demonstrate that the native extracted loader accepts a valid V4
fixture while this checker refuses it. Whole-PAK gzip is likewise outside the fixed
build scope; gzip/Brotli resource-payload layers remain supported as before.

## Separate writer/native-lookup determinism policy

Native SanityCheck does NOT validate ID ordering or uniqueness. LookupEntryById uses
bsearch over the canonical table and then aliases; Grit builds a Python dictionary and
can choose different bytes for unsorted/duplicate/colliding IDs. Consequently the
aggregate additionally requires strictly increasing canonical IDs (excluding the
terminal sentinel), strictly increasing alias IDs, and disjoint canonical/alias IDs.
These are explicit official-writer/native-lookup-equivalence requirements, not claimed
native loader rejection conditions. Both native WritePack (map keys and sorted table)
and Grit WriteDataPackToString emit this invariant. Zero is not banned as a real ID;
the fixed-width uint16 representation supplies the numeric range.

A private unsorted fixture preserves each Grit dictionary resource's correct bytes by
reordering its payloads and table entries together. The actual extracted native loader
accepts it, but actual native LookupEntryById misses52853. The new policy rejects this
before any dictionary-selected payload can be called the native-selected resource.
Duplicate canonical/alias IDs and alias/canonical collisions are separately rejected
with policy-labeled errors. Valid aliases continue to pass all exact-byte tests.

## Receipt and parent integration interface

All previous receipt fields remain: `passed`, `expectHostGate`, `builtPakSHA256`,
`runtimePakSHA256`, `builtPackagedPakExactMatch`, three exact resource records, input
identities, compression layers and artifact hashes. Added `nativePackSanity` has exactly
`intermediate`, `built`, `packaged`, each with version5, encoding, counts,
`nativeLoaderChecksPassed:true`, `writerLookupDeterminismPolicyPassed:true`.
Parent/recon must seal this new helper path/hash and require these fields in addition
to the existing exact mode/resource set/PAK-hash and phase-seal gates. An old ca144
receipt without this correction is insufficient. Runner before/after packaged
resources.sha256 must equal both receipt PAK hashes. No passing receipt is published
when sanity/policy validation fails.

The original primary GN/package path derivation remains in
`webextras_frontend_resource_check_notes.md`; this correction changes no paths/IDs,
compression format, parser extraction mechanism, source policy or archive format.
Original source/fixture receipts remain historical evidence, not live resource claims.

## Private tests and limits

`python3 arc/browser/webextras_frontend_resource_checked_test.py` directly compiles
helper source without import caches and replays all24 original exact-resource tests
against this aggregate. It compiles the actual C156 LoadImpl, SanityCheck and lookup/
comparator bodies in a small private data-source/result fixture, with packed table
layouts matching the pinned native header. This is an extracted-body unit fixture,
not a Chromium build or native runtime test.

Additional controls cover both reviewed false passes, truncated headers/tables,
unknown versions, extreme counts, all-offset EOF bounds, decreasing offsets, alias
index bounds, ID-order/duplicate/collision policies, native-accepted field variations,
and explicit V4 scope. Native rejected cases and native-accepted writer-policy failures
are recorded separately. The independently preserved reviewer fixtures are also replayed
against the correction in separate evidence, without modifying them.

This validates loader-relevant V5 structure and deterministic native resource selection
before the exact byte provenance proof. It does not prove JavaScript validity or
execution, full DataPack support, a clean frontend build, runtime behavior, app boundary
parity or integration readiness without the reviewed parent transaction.
