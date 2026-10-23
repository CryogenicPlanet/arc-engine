# Session DTO ABI and surface audit

No concrete Cocoa selector/encoding gap was found by source inspection. No source change is indicated by this bounded audit. This is not runtime metadata, native payload interchangeability or DTO behavior acceptance.

## Exact initializer distinction

| Class | Original selector / encoding | Current source |
|---|---|---|
| ArcSessionNavigation | initWithSessionNavigation: — @24@0:8r^v16 (TSV2814) | const void* at mm192/541; same source-level signature |
| ArcSessionTab | initWithSessionTab: — @24@0:8r^v16 (TSV2835) | const void* at mm563/577; same source-level signature |
| ArcSessionWindow | initWithSessionWindow: — @32@0:8{SessionWindow={shared_ptr<arc::SessionWindow::Impl>=^{Impl}^{__shared_weak_count}}}16 (TSV1169) | own objc_direct initWithChromeSessionWindow: at mm647/662 |
| ArcSessionTabGroup | initWithSessionTabGroup: — @32@0:8{SessionTabGroup={shared_ptr<arc::SessionTabGroup::Impl>=^{Impl}^{__shared_weak_count}}}16 (TSV1151) | own objc_direct initWithChromeSessionTabGroup: at mm623/634 |

The premise that Navigation/Tab have by-value C++ initializer encodings is incorrect. Their const-void pointer encoding is explicit in both exact TSV and specs. Original indexed bodies 443c968/443cb9c copy the supplied two-pointer native shared-handle data into engaged optional storage and retain its control. Current source copies its own shared_ptr<SerializedNavigationEntry>/shared_ptr<SessionTab> snapshots (mm541–546/577–579), produced by its own getter paths at mm598–599/675–676. Encoding equality does not establish compatibility with original private arc::Impl payloads, original layouts or producer objects. The pointer initializers themselves are not excluded by the generator's C++-signature filter.

## Cocoa surface and stub coexistence

All 27 non-C++ metadata selectors have corresponding source signatures: Navigation9 (8 getters+initializer), Tab8 (5 getters+2 setters+initializer), Window6 getters, Group4 getters. The ordinary 25 accessors/mutators use the required @, B, i, q or void argument/return shapes, including Tab's int currentNavigationIndex/setter, strong navigations/setter, and Window/Group NSInteger enum getters. Exact original rows are copied in original-metadata.txt; audit.json lists each encoding. No public accessor is left solely to a generated stub by the inspected source.

ARCHITECTURE.md30–33 requires public selectors/encodings to match, then explicitly says methods with C++ types in their encoding are ArcCore-internal and need not be reproduced: own internal accessors are allowed. This covers Window/Group's original aggregate initializer and the four native optional-handle getters plus Window/Group native setters. It does not prove interchangeability of pointer-encoded private payloads. Current direct Chrome accessors/constructors avoid claiming that private Arc ABI. Current C++ storage/ivar layouts differ; no app-side private-ivar compatibility is established or required by an exercised row here.

Generator35–41 excludes the four source-owned primary classes from shell declarations; all four are absent from current arc_api_classes.inc. Generator62–65 excludes .cxx methods and C++-encoded methods from the table entirely: original Window/Group initializers and native handle getters/setters are not installed as fallback stubs. The table still includes all27 ordinary selectors, exactly matching the TSV-filtered subset (arc_api_table.inc1099–1125). Installation calls class_addMethod at arc_api_stubs.mm194; it leaves existing implementations intact and does not compare, repair or overwrite their encodings. .cxx lifecycle is compiler-generated for owned storage, not a claim to reproduce original private layouts.

## Coverage and limits

The exact searched corpus is 44 finalized non-candidate results/golden/*/launch-*/record.jsonl paths and four historical owned v4 records. Each has zero rows containing any of the four DTO class names, including argument/return object strings. Paths, SHA256 and sizes are pinned in audit.json. This describes the searched files, not an independently verified accepted-scoreboard cohort. No runtime DTO accessor, initializer or pointer-payload behavior is thereby proved. C++ methods are additionally skipped by the standard recorder (ArcEngineRecorder.m218–220), so zero rows cannot establish absence of internal C++ activity.

Current restoreForeignTab: consumes the owned direct chromeSessionTab accessor (mm1129–1132); this source route is not evidence that a real app-produced original private DTO payload is interchangeable. Window's direct snapshot initializer has no inspected in-tree caller beyond its declaration/implementation; no producer activation recommendation follows.

Smallest reviewable option: retain the current adapters and matched Cocoa signatures; no ABI refactor follows from the mistaken by-value premise. If a genuine future native snapshot producer supplies DTOs, wire it through owned typed/direct accessors and separately verify the Cocoa getters and restoreForeignTab with that real producer. Do not fabricate original private peer layouts, reuse an original arc::Impl pointer as a Chromium snapshot or rename pointer constructors merely to imply ABI compatibility. No such work is implemented or recommended for activation here.

Independent focused source/generator review by full_probe_review agrees: no concrete public ABI defect, with pointer-payload and runtime-encoding limits retained. Only this new evidence directory was written. No apps, builds/compiler/graph queries, Git/index, queue, Chirp, credentials or source edits occurred.
