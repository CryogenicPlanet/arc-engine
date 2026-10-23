#!/usr/bin/env python3
"""Lead: disjoint class assignments and self-contained Sol writer/reviewer prompts."""
import csv
import json
from pathlib import Path

root=Path(__file__).resolve().parent
snapshot=root/'collected/agents/arc-layer/initial'
classes={row['class_name']:row for row in csv.DictReader((snapshot/'objc-classes.tsv').open(),delimiter='\t') if row['class_name'].startswith('Arc')}
methods={name:[] for name in classes}
for row in csv.DictReader((snapshot/'objc-methods.tsv').open(),delimiter='\t'):
    if row['class_name'] in methods and not row['protocol']:
        methods[row['class_name']].append(row)
assert len(classes)==146
groups={f'spec-{n}':[] for n in range(1,5)}
weights={name:0 for name in groups}
for name in sorted(classes,key=lambda n:(-len(methods[n]),n)):
    group=min(groups,key=lambda n:(weights[n],n))
    groups[group].append(name)
    weights[group]+=max(len(methods[name]),1)
assignment={'scope':'All Arc-prefixed ObjC metadata classes; not a claim all are exported or called',
            'class_count':len(classes),'method_rows':sum(map(len,methods.values())),
            'groups':{group:{'classes':sorted(names),'method_rows':sum(len(methods[n]) for n in names)} for group,names in groups.items()}}
(root/'class-assignments.json').write_text(json.dumps(assignment,indent=2)+'\n')
common=(root/'prompts/common.md').read_text()
for role in ('abi','cef','arc-layer','matcher'):
    (root/f'prompts/{role}-decompile.md').write_text(common+f'\n\nYour role is {role}.\n\n'+(root/'prompts/decompile.task.md').read_text())

spec_task='''
Your stage is specs. Read inputs/context/ENGINE.md sections 2 and 5.3 and inputs/context/lead-review.md. Your exact disjoint assignment is below and in inputs/context/class-assignments.json. Read raw ObjC metadata and initial Arc inventory under ../arc-layer/initial. Revalidate the assigned class/method inventory; include category and class-vs-instance identity.

Write one `specs/classes/<Class>.md` for EVERY assigned class, `specs/methods.tsv` with one row for every assigned declared method, `specs/coverage.json`, and `specs/report.md`. This is a research specification, not implementation code. Do not create class specs outside your assignment.

Each class spec should explain state/layout, lifecycle and ownership, callbacks/delegates, threading evidence, important caller/callee relations and Chrome/CEF route implications. Include a compact method matrix: category, +/- selector, type encoding, IMP VA, behavior, backend/callees, evidence and confidence/status. Every behavioral claim must cite exact ArcCore 1.170.0 function VAs, and any source identification must cite Chromium commit/file/line or an explicit source-backed inference. Metadata-only facts need metadata anchors. Do not invent ownership or thread semantics from signatures or a getter's name. Trivial accessors may have concise field-level descriptions. Mark unresolved/failed/thunk/malformed bodies explicitly and explain which consequential questions remain. An unknown row is preferable to a fabricated spec.

Use the indexed read-function.py helper and immutable complete corpus to obtain exact IMP bodies without scanning 1.1 GB repeatedly. Follow a bounded number of significant C++ bridge callees for complex methods, using Arc-layer-functions.tsv and the primary source mirror to check hypotheses. Focus semantic depth on lifecycle, view/embedding, persistence, callbacks, service access and nontrivial behavior; record limits on deeper closure rather than claiming exhaustive 567K-function understanding. Check category overrides and shared IMP aliases independently by metadata identity. Do not rely on giant malformed function bounds. No full mechanical matching, no Ghidra jobs or builds.

methods.tsv must include class_name,category,kind,selector,type_encoding,imp_va,body_status,behavior,evidence,confidence,unresolved_question. Coverage must reconcile exact method identities and assigned class count with metadata, report body-available/failed/missing/unresolved counts, and distinguish method coverage from semantic verification. Cite no report-wide size/effort estimate from summed Ghidra bounds. Keep class specs useful to an engineer implementing a replacement.
'''
review_task='''
Your stage is classreview, a FRESH independent CLI session. Review every class spec in the assigned OTHER writer's `specs/classes/` directory against the actual engine pseudocode, metadata and exact upstream source. Do not edit the writer's files. Read ../quality/initial/report.md, inputs/context/lead-review.md and the writer's methods.tsv and coverage.json. Verify every class exists, every declared method identity appears once, every cited VA resolves to the correct image/body, and confidence/unknown labels are honest. Check high-impact behavioral/ownership/thread claims against bodies and bridge callees; if you sample trivial accessors, report that sampling explicitly. Review each class, not just the summary.

Write classreview/report.md, classreview/class-review.tsv (columns class_name,result,evidence,limits; one row per assigned class), and classreview/findings.tsv (severity,class,selector,claim,exact evidence,required correction). Reproduce coverage checks. Flag unproved Chrome names, selector-only receiver assumptions, category conflation, borrowed object lifetimes and malformed Ghidra bodies. A class may remain incomplete with clearly stated unknowns; don't require guesses to fill gaps. Your output is a review; the parent resolves material findings before publication.
'''
for n in range(1,5):
    role=f'spec-{n}'
    details=json.dumps(assignment['groups'][role],indent=2)
    (root/f'prompts/{role}-specs.md').write_text(common+f'\n\nYour role is {role}.\n'+spec_task+'\nAssigned classes:\n'+details+'\n')
    other=f'spec-{n%4+1}'
    (root/f'prompts/{role}-classreview.md').write_text(common+f'\n\nYour role is {role}; review ../{other}/specs/classes/.\n'+review_task+'\nAssigned classes:\n'+json.dumps(assignment['groups'][other],indent=2)+'\n')

quality='''
Your role is quality, stage initial. Independently audit the ENTIRE copied engine export and the handoff before the full report is written. Read lead-review.md. Confirm the 199 unique manifests, 567959 C headers, 79 failure placeholders and 607294 TSV entries. Classify failures using their actual error strings, not the unsupported assumption all 79 timed out. Measure malformed/overlapping/huge Ghidra bounds and their effect on Arc-seed/closure claims, without treating pseudocode or TSV byte sums as actual code coverage.

Independently audit inputs/claude-handoff/scripts/arc_layer.py and results/arc-layer-functions.tsv: seed/closure counting, missing address fields, thunks/shared aliases, parser limitations, corrupted bodies beyond FUN_0421f778, and why a bounded closure is neither all proprietary code nor a rewrite-size measure. Reconcile metadata/import/export class and method counts, including 146 vs 142 vs exported symbols, and known unknown old-version encodings.

Do a bounded structural triage of the small libaperitif.dylib copy: hashes, Mach-O architectures/load dependencies, exports/imports, important strings and any clearly supported role. It was NOT part of the Ghidra decompile. If semantics require additional decompile, say so. The helper app executables have not been copied/analyzed: preserve that scope gap. Do not run code from the dylib, build anything, or use Ghidra. No full binary matching. Use one CPU and stream scans under heavy.lock.

Write initial/report.md, initial/coverage.json, initial/failures.tsv (columns va,error,category,evidence; exactly one row for each actual failed C-header entry), initial/quality-gates.tsv, and reproducible small tools. Give exact evidence and do not turn consistency checks into behavioral assurance. Review the coverage expectations for all class specs and give the synthesis reviewer a concrete list of claims that must remain provisional.
'''
(root/'prompts/quality-initial.md').write_text(common+'\n\n'+quality)
synthesis=(root/'prompts/review-synthesis.task.md').read_text()+'''

The user expanded the deliverable to a FULL report and per-class specs. Also inspect ../quality/initial/report.md, all four ../spec-N/specs/classes/ groups and their independent classreview artifacts. Reconcile all 146 metadata classes and all declared-method rows, while keeping exported/imported/reachable sets separate. Review all material class-review findings and identify anything the parent must correct before publication. Use the complete 199-shard corpus and source mirror; do not assume all pseudocode is semantically sound. Include a clear coverage appendix, Aperitif/helper/older-engine limits, observed ABI/bootstrap/view/callback/service architecture, source-backed Arc/Chromium boundaries, CEF route gates, maintenance implications and staged validation/implementation plan. Provide synthesis/engine-feasibility.md as the comprehensive top-level report and synthesis/spec-index.tsv (class_name,writer,reviewer,status,path) linking all classes to writer/reviewer outputs. Distinguish verified observations, source-backed inferences and unresolved behavior. Avoid unsupported full-rewrite time/code-size estimates. No native changes or pushes.
'''
(root/'prompts/review-synthesis.md').write_text(common+'\n\n'+synthesis+'\n\n'+(root/'prompts/review-final-gates.md').read_text())
print(json.dumps({'class_count':len(classes),'method_rows':assignment['method_rows'],'workloads':weights}))
