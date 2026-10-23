# ArcSessionTab nonempty navigation replacement audit

Read-only static audit, 2026-10-23. Fresh independent review corroborated the bounded result. No application, runtime probe, build, source/shared/test edit, private-ABI DTO fixture, board access, or credential access. The abandoned created-close preparation was not resumed and produced no probe.

## Result

One bounded publication-order mismatch remains in current `arc/api/arc_sessions_restore.mm:541`: the owned setter publishes its retained `_navigations` array only AFTER count/reserve/enumeration. Original strongly stores the supplied array BEFORE first enumeration. For an NSArray implementation that reenters the tab's navigation getter during count/fast enumeration, original exposes the new input array while current code can expose the old cache or lazily materialize the old native vector. This is a statically supported conditional difference; no such runtime fixture or ordinary app occurrence was measured.

The faithful correction is: strongly publish the supplied array, enumerate/convert the supplied parameter WITHOUT asking it for count or reserving from count, CHECK_GT(entries.size(), 0u) on the actually collected entries, then replace the native vector. This preserves the native nonempty invariant at its original semantic boundary, after conversion. No implementation change was made here. See count-callouts.md for the exact absence of incoming NSArray count calls.

No selected-index/clamping bug was found. Both retain the exact array without copying and separately snapshot/copy native navigation values.

## Original nonempty setter and array semantics

Original public `setNavigations:` IMP443d0c8:

```asm
443d0e8 mov x19,x2               ; incoming array
443d0ec mov x20,x0               ; ArcSessionTab
443d100 mov x0,x2
443d104 bl 800484c              ; retain input
443d108 add x0,x20,#8           ; _navigations slot
443d10c mov x1,x19
443d110 bl 8c7df80              ; objc_storeStrong BEFORE enumeration
...
443d138 add x2,sp,#0x30
443d13c add x3,sp,#0x90
443d140 mov x1,x21              ; countByEnumeratingWithState:objects:count:
443d144 mov w4,#0x10
443d148 bl 80045ec              ; first fast-enumeration message
...
443d19c add x0,x20,#0x10        ; native SessionTab peer
443d1a0 mov x1,sp               ; collected navigation handle vector
443d1a4 bl 4340c10              ; native value-vector replacement
```

The exact class metadata declares navigations strong (`&,N`), not copy, at `arc/tools/arccore-1.170.0-objc.tsv:2831`; setter/getter records2840/2841. Store veneer8c7df80 branches to import stubc4a6dc8, whose `_objc_storeStrong` identity is independently recorded in `research/2026-10-22-sol/evidence/agents/spec-1/revisions/v1/revision-evidence/finding-checks.json:44-50` and matching methods.tsv rows. This avoids treating decompiler retain/store fixups alone as proof.

Getter443cde8 loads ObjC self+8 at443ce0c, and when nonnull directly returns that cached object via443ce30. There is no array copy in this path. A mutable array supplied to the setter remains the same retained object returned by subsequent getters. Its later mutations do not automatically mutate the already-copied native vector; this split cache/snapshot model exists in original and should not be "repaired" by copying the array or binding its mutations to native history.

Per-element443d23c..443d258 sends the native session_navigation getter and requires engaged peer, then443d274..443d284 transfers the shared navigation handle into a temporary collection. Backend4340c84..4340ca0 extracts each source SerializedNavigationEntry and copies its value into a newly built vector (copy helper6c7effc; allocation/copy-growth helper4340f60). After construction,4340ce8..4340cf8 destroys old native navigation values and replaces only the vector storage/length/capacity triple at native SessionTab+0xa8/+0xb0/+0xb8. Current entries.push_back(*chromeNavigation) followed by vector move has the same value-snapshot purpose. No live linkage from input NSArray to native vector is established.

## Selected index is unchanged

Original selected index getter443ccdc follows its SessionTab peer and loads native Tab+0xc at443ccec. Its separate explicit setter443ccf8 checks nonnegative and below current native vector length then stores at443cd20.

Neither the inspected public navigation-array setter nor backend4340c10 stores to Tab+0xc or calls that index setter. Backend commits ONLY navigation vector+0xa8/+0xb0/+0xb8. Therefore replacing a nonempty history does not itself normalize/clamp selected index in this bounded original route. For example, a formerly valid selected index2 can remain2 after a one-entry replacement; that is an explanation of the static field updates, not an executed case.

Current owned `setNavigations:` lines530..542 likewise never modifies `_tab->current_navigation_index`. This matches original. Do not add a clamp to this setter based on vector length. `restoreForeignTab:` separately uses `normalized_navigation_index()` as defined by Chromium `components/sessions/core/session_types.h:48` when restoring; that later consumer normalization is not setter behavior.

## Comparison with current owned implementation

-530..533: validates peer and sends incoming NSArray count to check nonempty. Original instead validates the ACTUALLY COLLECTED native handle-vector count after enumeration.
-534..540: sends a second NSArray count for reserve, then value-copies incoming navigation records. Neither incoming count send occurs in original.
-541: strongly assigns the exact input array, matching final identity/retention but publishing it too late relative to enumeration.
-542: moves the built values into the native vector; leaves selected index unchanged.

Moving cache publication earlier and removing the two extra incoming count callouts is the supported minimal sequence correction. Keep the nonempty check on entries.size() after conversion. Ordinary Foundation NSArray calls that do not reenter and whose count matches enumeration have matching final array/native-vector/index results. No runtime regression or broad producer/foreign-restore acceptance is claimed.

## Inputs and limitations

Original backend source evidence: `research/2026-10-22-sol/evidence/agents/spec-2/classreview/backend/04340c10.c`; ObjC wrapper body: `research/2026-10-22-sol/evidence/agents/spec-3/revisions/v1/bodies/0443d0c8.txt`. Raw exact ranges are saved in `original-disassembly.txt`. Earlier empty guard evidence is in `results/sessions/navigation-empty-guard/`; it established the nonempty requirement, not a complete nonempty publication-order audit.

Full indirect destructor/reentry behavior, malformed-member ABI, impossible allocation-size limits, concurrent mutation races, and all external native peer mutations are outside this bounded audit. No setter was invoked and no native DTO handle was fabricated. The index conclusion is based on the shown field/call dataflow, not an invented observed trace. Hashes and the current source snapshot identity are in manifest.json.
