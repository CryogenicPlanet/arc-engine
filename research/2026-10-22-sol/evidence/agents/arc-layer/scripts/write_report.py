#!/usr/bin/env python3
"""Render the bounded research report from generated evidence tables."""
import csv
import json
from pathlib import Path

O=Path('initial')
def load(n):return list(csv.DictReader(open(O/n),delimiter='\t'))
counts=json.loads((O/'counts.json').read_text())
xc=json.loads((O/'xref-counts.json').read_text())
app=json.loads((O/'app-launch-counts.json').read_text())
validation=json.loads((O/'validation.json').read_text())
subs=load('subsystems.tsv');ss=load('arc-strings.tsv');requests=load('decompile-requests.tsv');gaps=load('decompile-gap-requests.tsv')
arc_cpp_strings=len({r['text'] for r in ss if 'arc::' in r['text']})
table='\n'.join(f"| {r['subsystem']} | {r['named_function_entries']} | {r['named_reported_bytes']} | {r['arc_source_paths']} | {r['triage_priority']} |" for r in subs)
gaplist='\n'.join(f"- `{r['address']}` → `{r['expected_shard']}` (instruction-range hint): {r['reason'].split(': ',1)[-1]}" for r in gaps)
overlap=sorted({r['text'] for r in ss if r['kind']=='historical_string' and r['present_as_exact_printable_run_in_stock']=='True'})
maxrss=max(int(l.split(':',1)[1]) for name in ['inventory','export','trace','app-launch','validation'] for l in (O/f'{name}-resource.log').read_text().splitlines() if 'Maximum resident set size' in l)
parts=f"""# Arc engine inventory: initial research

ArcCore 1.170.0 contains **146 Arc-prefixed ObjC classes, 16 Arc categories and 1,964 declared Arc method rows**. The saved engine table has **1,531 Arc-named function entries** across **144 class scopes and 16 category scopes**. Of these entries, **1,520 are primary ObjC IMP anchors and 11 are additional branch veneers**. This is a lower-bound discovery inventory of the named Arc interface, not an inventory of all custom native code or a rewrite estimate. Named interface wrappers can share stock implementations.

The useful next step is targeted launch-path decompilation: **{xc['decompile_requests']} exact function-entry requests**, plus **{len(gaps)} instruction-only metadata gap requests**. Engine pseudocode is absent from these inputs; no engine semantics have been inferred from unseen pseudocode.

## Evidence and address basis

All evidence inputs are beneath `/home/exedev/work/inputs`; all outputs and scripts are in `/home/exedev/work/agents/arc-layer`. No product implementation, Ghidra run, remote access, agent delegation, package installation or external messaging was performed. Public Apple source was fetched only to document pointer layout.

| Image | Version basis | SHA-256 |
|---|---|---|
| `binaries/ArcCore-1.170.0.arm64` | Supplied ArcCore 1.170.0 / Chromium 156.0.8078.48 identity | `{counts['engine_sha256']}` |
| `binaries/ChromiumFramework-156.arm64` | Supplied same-version stock build; different optimization/link settings | `{xc['stock_sha256']}` |
| `binaries/Arc-1.168.0.arm64` | Pristine compatibility-target app | `{app['app_sha256']}` |

The function-table SHA-256 is `{counts['sha256']['metadata/decomp/arccore-1.170.0-functions.tsv']}`. `counts.json` pins the dump/helper/evidence files separately; `app-launch-counts.json` pins the old app pseudocode and function table. These are artifact revisions, not an inferred Chromium source checkout revision. No upstream source diff was available or claimed.

Engine addresses are unslid low Mach-O VAs; app addresses are `0x100...`. File offsets are translated through `LC_SEGMENT_64` mappings in `segments.tsv` and `sections.tsv`. Engine `__TEXT` and `__DATA_CONST` happen to have equal VAs and offsets; that equality is not used as an assumption. Later segments differ. The app selector-ref VA `0x104462d78`, for example, is resolved through the app's mappings, not used as a file offset.

Both binaries use chained fixups: engine `DYLD_CHAINED_PTR_64` (2), app `DYLD_CHAINED_PTR_64_OFFSET` (6). Raw pointer chain bits must be decoded. The parser checks the format, resolves local pointers and imported class bindings, and decodes small/relative method entries. Primary source: Apple dyld revision **fd8d0c4d52320ebf64db34f3cb280310d905c5ae**, [`include/mach-o/fixup-chains.h`](https://github.com/apple-oss-distributions/dyld/blob/fd8d0c4d52320ebf64db34f3cb280310d905c5ae/include/mach-o/fixup-chains.h#L161), especially pointer formats (line 98), 64-bit rebases (161), binds (218), imports (307). A pinned local copy and source URL are `fixup-chains.h` and `primary-sources.tsv`.

## What the counts mean

| Measure | Reproduced result |
|---|---:|
| Saved engine function entries, including named thunks | 607,294 |
| Function names beginning `thunk_` | 35,485 |
| Arc-named entries with ordinary class scope | 1,390 |
| Arc-named entries with category scope | 141 |
| Arc-named entries total / distinct qualified name strings | 1,531 / 1,520 |
| Arc class scopes with named entries / metadata classes | 144 / 146 |
| Arc category scopes / metadata categories | 16 / 16 |
| Named `arc::` C++ function entries in saved table | 0 |
| Arc base-method declarations / category-method declarations | 1,821 / 143 |
| Unique implementation VAs used by declared Arc methods | 1,566 |
| Those IMPs with an Arc-named table anchor / another class's anchor | 1,520 / 46 |
| All engine ObjC classes / categories / distinct protocols | 325 / 32 / 69 |
| Engine protocols named with `Arc` prefix | 0 |

`objc-methods.tsv` records the actual `+`/`-` kind, class, category, selector, type encoding, method-record VA and IMP. The function-table kind `objc_class_method_anchor` means a name in an ObjC class scope; it does **not** mean every row is a `+` class method. Methods are matched independently by explicit **class, category, kind, selector and IMP**, never by dump order. All **4,314** dyld implementation tuples match the independently parsed binary metadata. Class, category and protocol identity sets also match. Protocol requirement rows are kept separately by their `protocol` field; they are not implementation functions.

`objc-arc-imp-anchors.tsv` covers all 1,566 Arc IMPs, including the 46 whose saved name belongs to another class. Shared trivial accessors/destructors explain examples such as `RTCEncodedImage::encodeStartMs` and `WebNNMLFeatureProvider::featureValues`; the alias alone does not establish custom semantics. `ArcBrowserSidePanelEntryInfo` and `ArcNotificationItem` have metadata but no own named class scope. The older 1.168 dump also has 146 Arc classes, 16 categories and 1,964 method rows; equal counts are not ABI compatibility proof.

There are **12** syntactically verified branch/veneer patterns among Arc-named entries: the 11 additional veneers and the primary category IMP `ArcWebContents(Extensions)::createPaneHostView` at `0x4410bb4`, which branches directly to `FUN_047390dc` at `0x47390dc` (`0x47.c`). The latter is explicitly requested. The table does not export Ghidra's `isThunk` bit; name-prefixed thunks and instruction patterns are reported separately. Historical figures **142 / 1,524** are not reproduced by the current saved table under these definitions.

The third column is preserved as `reported_body_bytes`, but **every one of the 607,294 rows equals `end_bound - entry`**. The supplied historical `context/vm/ghidra/ExportFunctions.java` also writes that span, despite the input description calling this body bytes. The whole-table sum is **858,548,775**, with overlapping, very large bounds (for example `FUN_002b7940`, `0x2b7940..0x8b5e074`). Therefore these are extent statistics with provenance uncertainty, not a contiguous byte map or exclusive code sizes. Arc-named reported extents sum to **288,288 bytes (281.53 KiB)**, including 108 bytes of additional veneers. This small wrapper-oriented number cannot size the full Arc layer. Exact Ghidra body ranges are still required.

## Declared app surface versus actual sends

`app168-imported-class-declared-methods.tsv` reproduces **834 declaration rows on 63 imported classes**: 786 base rows and 48 category rows, or **830 unique (class, kind, selector) combinations**. Four combinations occur in both base/category metadata: `ArcBrowserContext`'s `metrics`, `extensions`, `id`, and `ArcPermissionRequest`'s `additionalData`. This count says which methods are available on imported classes; it does not say which methods the app calls.

`app168-launch-calls.tsv` separately verifies **seven actual app `BL` instructions and decoded ObjC selector stubs** in two saved app functions. The old app pseudocode associates receivers with the Arc classes; runtime receiver type and callback ownership remain unproven. Concrete examples:

- App `ADK.ADKApplication.arcBrowserApplication.getter`, entry `0x1004aa890`: call `0x1004aa8c8` (file `0x4aa8c8`) → stub `0x1035bed60` → `init:`. Receiver association: `ArcBrowserApplication`; old app `app168/ghidra/decomp/0x1004.c:126553`.
- App `ADK.ADKApplication.method_instance`, entry `0x1004ad314`: `0x1004ad718` → configuration initializer stub `0x1035beda0`; `0x1004ad77c` → `initialize:` stub `0x1035c1c80`. Context: the same old app shard, line 128994 onward.
- That second function also directly sends `setNotificationCenter:`, `setHistogramSamplesDelegate:`, `setArcPerformanceObserver:` and `setArcCrashKeysObserver:`. These observer APIs warrant early investigation even if their runtime work is optional.

The engine export trie independently matches all **259** supplied export names: 146 ObjC class symbols, 105 metaclass symbols, seven function exports and one nonfunction symbol. The supplied 1.168/1.169/1.170 export-name sets are identical; all **67** app imports (63 classes and four C symbols) are present. `exported-interface.tsv` preserves exported VAs separately from repeated Ghidra name anchors. The imported C boundary includes `_ArcErrorDomain` at `0xdf6bb28`, `_CallWithEHFrame` at `0x441d770`, `_arcCategoryEnabledFlag` at `0x440d23c`, and `_emitTraceEvent` at `0x440d04c`. The three functions receive additional P1 decompile requests and are excluded from the 1,531 ObjC-scope/namespace count. Export-name equality is only a link-surface check.
+
+This is a targeted launch sample, not an exhaustive app method-call census. Objective-C dispatch and shared selector stubs prevent inferring receiver classes from selector presence alone.

## Subsystems and priority

Rows are assigned by the explicit ordered keyword rules in `inventory.py::subsystem`; this is a reproducible triage partition, not a verified architecture graph. Namespace strings, class scopes and file paths are different evidence types. Category `(Extensions)` means added ObjC API; it does not automatically mean browser extensions. Counts below are additive only for the named entries and path partition. Reference-derived function candidates may appear in several subsystems.

| Subsystem | Named entries | Reported extent bytes | Arc source paths | Inspect priority |
|---|---:|---:|---:|---|
{table}

**P0** means inspect lifecycle/profile/browser/content ownership and view creation first. **P1** means inspect UI hooks, IPC, extensions, networking, blocking/media, security and startup observer registration. **P2** marks service/workflow candidates such as AI, content conversion and importing, plus unclassified evidence. These are research priorities, not proof that a subsystem is required at launch or safe to omit. A full Arc-compatible app interface may need all app-reachable behavior.

The native-view bridge has precise named anchors: category `createPaneHostView` (`0x4410bb4`), `nativeView` (`0x4410bb8`), `attachToPaneHostView:` (`0x4410cd0`), and `detachFromPaneHostView:` (`0x4410ec0`). Browser/application roots include `ArcBrowserApplication::init:` (`0x43e6548`), `initialize:` (`0x43e6fc8`), `createBrowser:withDelegate:forWindow:` (`0x43e7048`), and context creation (`0x43e8368`). Context and content initializers are `0x443739c` and `0x4440c4c`. These are `0x43.c`/`0x44.c` requests, with the pane-host branch target in `0x47.c`.

Native implementation candidates are independently evidenced by exact source-string instruction references. Examples (all engine VAs; offsets are recorded in `string-xrefs.tsv`):

| Compiled source path | Candidate function entry | Exact referencing instruction | String VA |
|---|---|---|---|
| `arc/browser/arc_browser_runner.cc` | `0x42aac34` | `0x42aad10` | `0xd37da58` |
| `arc/browser/browser_application_impl.cc` | `0x42b0e64` | `0x42b1188` | `0xd37dc17` |
| `arc/browser/browser_context/browser_context_impl.cc` | `0x42c8aa0` | `0x42c8b84` | `0xd37e5a1` |
| `arc/browser/browser_impl.cc` | `0x42f89b0` | `0x42f8d3c` | `0xd37f245` |
| `arc/browser/web_contents/web_contents_impl.cc` | `0x4374a1c` | `0x4374a98` | `0xd381cc0` |

The UI path inventory includes permission prompts, JavaScript dialogs, autofill/password prompts, WebAuthn, exclusive access, window integration, extension prompts and PiP. Paths prove the compiled layer exists; they do not establish whether a hook implements functionality, forwards it, or deliberately returns an unsupported result.

## Strings, IPC and unnamed candidates

The binary scan reproduces **7,823 unique embedded relative source paths**, including **111 under `arc/`**. It finds all 200 historical selected strings and {arc_cpp_strings} distinct retained strings containing `arc::`; these are data strings, not recovered C++ namespace/function symbols. Four exact IPC identities are present and absent as exact printable runs from the stock baseline: `arc.mojom.AdBlockerAgent` (`0xd37db87`), `EffectsAgent` (`0xd37ed3d`), `RenderProcessAgent` (`0xd38319c`) and `RenderFrameAgent` (`0xd3831b9`). Registration, schema versions, message layouts and required startup routing remain unknown.

**Seven of the historical 200 strings also occur in stock**, leaving 193 absent from this particular stock image. The overlaps are Android/ChromeOS ARC names: {', '.join('`'+s+'`' for s in overlap)}. Absence from this differently built stock binary is only a search filter; it is not proof of proprietary code or an upstream change.

The limited ARM64 decoder finds **1,528 adjacent `ADRP+ADD` instruction pairs**, covering all 111 Arc paths and pointing to **562 distinct nearest-entry function candidates**, of which **554 have `FUN_`/`thunk_FUN_` names**. `string-xrefs.tsv` pins both instruction words, instruction/string VAs and mapped offsets. `unnamed-candidates.tsv` supplies exact entry/shard requests and evidence anchors. Attribution uses nearest preceding entry only when its saved end bound covers the instruction; it is explicitly **not body-range verified**.

There are **393 references without that bounded attribution**, including 307 source-path references. They are preserved in `unattributed-xrefs.tsv`; no earlier giant function extent is used to force a match. Six paths have no attributable function at all. Resolve exact containing bodies at these instruction addresses before assuming a decompile shard:

{gaplist}

`decompile-gap-requests.tsv` labels these addresses as instructions. Its `expected_shard` is the requested `address >> 20` hint and may differ from the shard of the actual containing function. Ordinary `decompile-requests.tsv` rows are real saved function entries and use the confirmed function-entry formula `0x%x.c`.

## Reproduction, validation and limits

Exact validation run, from `/home/exedev/work/agents/arc-layer`:

```sh
flock /home/exedev/work/heavy.lock bash scripts/reproduce.sh
```

The stdlib Python tools run serially under the shared heavy lock, stream big files in one-MiB blocks, and use one CPU. `/usr/bin/time -v` logs are preserved. Maximum measured RSS was **{maxrss:,} KiB**. Validation result: **{validation['status']}** — {validation['named_function_rows']} named rows, {validation['string_occurrence_rows']} retained string occurrences, {validation['exact_instruction_xrefs']} exact instruction references, {validation['decompile_request_rows']} function-entry requests, seven app call sites, 1,566 Arc IMP anchors and 4,314 explicit dyld/binary method tuple matches. Validation checks bytes, VA/offset translations, decoded targets, saved entry boundaries, shard names, partitions and input hashes. `validation.json` is machine-readable.

Primary deliverables: `report.md`, `arc-functions.tsv`, `subsystems.tsv`, `decompile-requests.tsv`. Supporting artifacts: `counts.json`, `xref-counts.json`, the `objc-*.tsv` tables, `arc-strings.tsv`, `all-source-paths.txt`, `string-xrefs.tsv`, `unnamed-candidates.tsv`, `app168-launch-calls.tsv`, `decompile-gap-requests.tsv`, and validation/resource logs. Reproducible tools are in `../scripts/`.

The xref decoder intentionally covers only adjacent 64-bit `ADRP+ADD`; it misses separated pairs, CFString indirection, literals, jump tables and indirect calls. Syntactic instruction matches do not prove execution. Exact body gaps, tail calls, inlining, ICF and compiler configuration remain unresolved. ArcCore has no `LC_FUNCTION_STARTS`; this work never uses the failed `engine_match.py` prototype. The function table has no reliable original source namespace map. No absence claim about changed Chromium files is justified by compiled-path absence, and unmatched stock functions are not automatically Arc code. The old app pseudocode is used only for app receiver context. No dynamic startup, macOS runtime validation, ABI/callback lifetime proof, service schema recovery or complete custom-code measurement was performed.

## Next specific questions

1. In `0x43.c`/`0x44.c`, what do application configuration, `initialize:`, browser/context creation, content delegates and native view attachment actually require, in what order and on which threads? Which objects own the C++ peers and completion blocks?
2. In `0x42.c`, recover the implementation candidates above and then follow their exact calls into Chromium. Are any initialization requirements hidden in feature config, profile loading, import migration or service startup?
3. Supply exact Ghidra body-range mappings for the six instruction gaps, plus decompiled `0x47390dc` in `0x47.c`. What is the pane host view contract and destruction/attachment lifetime?
4. Recover the four Arc Mojo endpoint schemas, registration sites and renderer/utility startup reachability. Can AI/content conversion/importing be deferred while preserving startup callbacks and app-visible errors?
5. Reconcile selector signatures with app-side call sites and observed callback behavior before treating equal metadata counts as compatibility. Determine which UI hooks are intentionally unsupported and which forward into Chrome internals.
"""
(O/'report.md').write_text(parts)
print('wrote initial/report.md')
