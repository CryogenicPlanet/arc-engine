"""Apply the reviewed annotation corrections to this seeded revision only.
This is not an original renderer or evidence collector.
"""
import csv
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ORIGINAL = Path('/home/exedev/work/agents/spec-2/specs')
REVIEW = Path('/home/exedev/work/agents/spec-1/classreview')
assert ROOT == Path('/home/exedev/work/agents/spec-2/revisions/v1')
with (ORIGINAL/'methods.tsv').open() as f:
    reader = csv.DictReader(f, delimiter='\t')
    fields = reader.fieldnames
    original = list(reader)
rows = [dict(r) for r in original]
bykey = {(r['class_name'],r['selector']):r for r in rows}
findings = list(csv.DictReader((REVIEW/'findings.tsv').open(), delimiter='\t'))
corrections = []

def update(cls, sel, behavior, question=None, body_status=None, semantic_status=None,
           confidence=None, extra_evidence=None):
    r = bykey[cls,sel]
    r['behavior'] = behavior
    if question: r['unresolved_question'] = question
    if body_status: r['body_status'] = body_status
    if semantic_status: r['semantic_status'] = semantic_status
    if confidence: r['confidence'] = confidence
    if extra_evidence:
        r['evidence'] += '; revision-evidence/' + extra_evidence
    return r

descriptions = [
    ('Requires engaged native session or traps. Builds NSMutableIndexSet from native result values below 4 and returns its copy on success, including an empty set; on failure calls 0x4402548 for NSError conversion and returns nil. Result is an object, not BOOL.',
     'Content-type enum names, producer meaning of values, invalid-value filtering and NSError edge cases remain unverified.'),
    ('Returns a copied dictionary snapshot. Iterates native linked header entries, converts each key/value byte string with 0x1c391bc and sends setValue:forKey: to NSMutableDictionary; no entries produces an empty copied dictionary.',
     'Duplicate-key/case handling, invalid-string conversion and snapshot freshness remain unverified.'),
    ('Returns an array of strings: absent native optional returns the empty NSArray singleton; engaged state enumerates 24-byte native string entries, converts each via 0x1c391bc, adds it to NSMutableArray and returns a copy. Invalid nonzero engagement can trap.',
     'Producer meaning, invalid-string behavior and snapshot freshness remain unverified.'),
    ('Requires engaged native state or traps. Gets a temporary suggestion vector via 0x43342a4, converts each UTF-16 element with 0x1d9fb90 and returns the constructed NSMutableArray, including an empty array; no copy is sent.',
     'Suggestion producer, ordering, invalid-string behavior and caller mutation expectations remain unverified.'),
    ('Adopts the supplied by-value native controller owner into self+8 and disposes the previous owner via 0x42c33cc and 0x5810. The successful path does not zero the incoming source; zero stores are old-owner or failed-init cleanup. Caller-side move and full ownership ABI remain unverified.',
     'Caller-side move/aggregate ABI, controller construction/observer registration and external lifetime remain unverified.'),
    ('Loads the controller through its stored owner; if native controller and state at +0x70 are nonnull, directly calls 0x24e39c0(controller+0x30 value, 0), then resets saved completion storage. The later indirect operation cleans the saved callable; exact upstream Stop identity remains provisional.',
     'Whether pending completion fires during stop, null-owner behavior, exact native stop reason and upstream identity remain unverified.'),
    ('Incoherent pseudocode: apparent temporary-vector cleanup/count reset precedes array construction/reuse; candidate initializer arguments contain object-to-bool casts and in_stack values. Array and ArcCommandBarRankingCandidate allocations show conversion intent only. Array contents, argument mapping and lifetime are unresolved; no actual engine use-after-free, empty-result or corrupt-boundary conclusion follows.',
     'Recover coherent control flow, temporary ownership and constructor argument mapping across 0x443366c, 0x4433528 and 0x43dad44 before asserting array contents, ordering or wrapper lifetime.'),
    ('Marshals HTML, optional URL, options/flags and a native vector from kind/value messages to untyped enumerated input elements; captures completion and calls 0x42a75b4, reaching content_conversion.mojom.ContentConversionService. ArcMetaTagSelector has a compatible candidate shape; this path allocates/checks no such class and does not establish concrete receiver identity.',
     'Producer/caller dataflow establishing element class, exact option enums, result/error adapter, algorithm/library and callback queue remain unverified.'),
    ('After NSObject init, stores scalar kind and an explicit copy of value. Its kind/value shape is compatible with conversion input, but the untyped enumeration at 0x43de074 does not establish that its receivers are ArcMetaTagSelector instances.',
     'Concrete conversion-element receiver class requires producer/caller dataflow; selector kind enum and service matching rules remain unknown.'),
    ('Copies native shared-handle state and stores a weak owner after NSObject init. Ghidra signature/argument-propagation artifact: unused param_4 and owner rendered from the aggregate. Raw entry saves x2 as the handle address and x3 as owner; handle words are loaded through saved x2 and saved x3 feeds the weak-store call. This is local register evidence, not a complete C++ ABI guarantee.',
     'Caller-side aggregate passing, native copy/move ownership, defaults and input validation remain unverified.'),
    ('Copies native shared-handle state and stores a weak owner after NSObject init. Ghidra signature/argument-propagation artifact: unused param_4 and owner rendered from the aggregate. Raw entry saves x2 as the handle address and x3 as owner; handle words are loaded through saved x2 and saved x3 feeds the weak-store call. This is local register evidence, not a complete C++ ABI guarantee.',
     'Caller-side aggregate passing, native copy/move ownership, defaults and input validation remain unverified.'),
    ('Enumerates input examples, copies the native service handle and captures completion before forwarding to 0x429d6b8; collection helpers include 0x43dab74, 0x43daef4 and 0x43db22c. Ghidra explicitly warns Type propagation algorithm not settling; displayed extra arguments and aggregate semantics are untrusted. Export remains available; function-boundary corruption is not established.',
     'Argument/aggregate recovery, exact result/error adapter, callback ownership/once-versus-repeated behavior, queue and unavailable/cancellation behavior remain unresolved.'),
    ('Enumerates input examples, copies the native service handle and captures completion before forwarding to 0x429e884; collection helpers include 0x43dab74, 0x43daef4 and 0x43db22c. Ghidra explicitly warns Type propagation algorithm not settling; displayed extra arguments and aggregate semantics are untrusted. Export remains available; function-boundary corruption is not established.',
     'Argument/aggregate recovery, exact result/error adapter, callback ownership/once-versus-repeated behavior, queue and unavailable/cancellation behavior remain unresolved.'),
]
assert len(findings) == len(descriptions) == 13
for number,(finding,(behavior,question)) in enumerate(zip(findings,descriptions),1):
    key = finding['class'],finding['selector']
    before = bykey[key]['behavior']
    extra = 'rechecked-bodies.txt'
    if number in (5,6,7,10,11): extra += '; revision-evidence/instructions.txt'
    if number == 7: extra += '; revision-evidence/related-bodies.txt'
    r = update(*key, behavior, question,
               body_status='available_signature_artifact' if number in (10,11,12,13) else
                           'available_incoherent_pseudocode' if number==7 else None,
               semantic_status='unresolved' if number==7 else None,
               confidence='low' if number in (7,12,13) else None,
               extra_evidence=extra)
    corrections.append(dict(finding_row=number,class_name=key[0],selector=key[1],
                            disposition='corrected; semantic contract unresolved' if number==7 else 'corrected',
                            before=finding['claim']+' Original annotation: '+before,
                            after=r['behavior'],
                            evidence=finding['exact evidence']+'; '+r['evidence'],
                            remaining_unknown=r['unresolved_question']))

# Additional signature artifact corroborated in the cross-report binary check.
r = update('ArcWebContentsDevTools','openProtocolConnection:error:',
           'Captures notification sink; 0x4355df8 can reject disabled/unavailable/attach-failed state; returns boxed connection ID on success or nil after error conversion. Ghidra adds a spurious parameter: raw 0x4415b74 saves the NSError-out argument from x3 and 0x4415c18 forwards it to 0x4402548.',
           'Protocol message/result adapter, callback delivery sequence, cancellation/drain ordering and native frontend lifetime remain unverified.',
           body_status='available_signature_artifact',
           extra_evidence='crosscheck-bodies.txt; revision-evidence/instructions.txt')

for cls in sorted({r['class_name'] for r in rows}):
    # Seed every class from the immutable original, then change annotations only.
    text = (ORIGINAL/'classes'/f'{cls}.md').read_text()
    lines = text.splitlines()
    for i,line in enumerate(lines):
        if not line.startswith('| ') or line.startswith('| Category '): continue
        cells = [c.strip() for c in line.split('|')[1:-1]]
        if len(cells)!=8 or not cells[1].startswith(('- ','+ ')): continue
        selector = cells[1][2:]
        r = bykey[cls,selector]
        cells[4] = r['behavior']
        cells[7] = f"{r['confidence']} / {r['body_status']} / {r['semantic_status']}"
        lines[i] = '| ' + ' | '.join(cells) + ' |'
    text = '\n'.join(lines)+'\n'
    if cls == 'ArcBrowserContextAutocomplete':
        text = text.replace('initWithAutocompleteController: moves a pointer from the supplied controller owner and nulls that source (0x43ecb50);',
                            'initWithAutocompleteController: adopts the supplied by-value native controller owner into self+8; source-zeroing is not shown on success (0x43ecb50). The zero stores belong to old-owner or failed-init cleanup, so caller-side move/ownership ABI remains unverified;')
        text = text.replace('stop checks controller state, invokes a virtual stop-like operation, and clears callback storage (0x43ece08).',
                            'stop checks native controller/state, directly calls 0x24e39c0 with reason value zero, then resets callback storage and indirectly cleans the saved callable (0x43ece08).')
    if cls == 'ArcCommandBarRankingExample':
        text = text.replace('Extensions category input and candidates copy the native example and construct ArcCommandBarRankingInput/ArcCommandBarRankingCandidate wrappers (0x4401ca0, 0x4401f34).',
                            'Extensions input copies native example state and constructs an ArcCommandBarRankingInput wrapper (0x4401ca0). The candidates getter shows array/candidate allocation intent, but its pseudocode has incoherent temporary cleanup/reuse and argument propagation (0x4401f34); returned contents and ownership are unresolved. Raw instructions also place cleanup-helper calls before the apparent conversion loop, so this requires coherent callee/control-flow recovery, not a guessed lifetime repair or an engine-bug diagnosis.')
        text = text.replace('Resolve candidate ordering and selectedIndex validation; verify independent ownership of returned input/candidate wrappers.',
                            'Recover coherent candidates control flow, argument mapping and temporary ownership across 0x4401f34, native getter 0x443366c, initializer 0x4433528 and copy helper 0x43dad44; candidate contents, ordering and lifetime remain unresolved. Resolve selectedIndex validation and ownership of returned input wrappers.')
    if cls == 'ArcContentConversionService':
        text = text.replace('a vector built from ArcMetaTagSelector kind/value before calling 0x42a75b4 (0x43de074).',
                            'a vector built by sending kind/value to untyped enumerated input elements before calling 0x42a75b4 (0x43de074). ArcMetaTagSelector is a compatible candidate shape; this path does not prove the concrete receiver class.')
    if cls == 'ArcMetaTagSelector':
        text = text.replace('ArcContentConversionService conversion enumerates these objects, sends kind/value and builds a native selector vector (0x43de074).',
                            'ArcContentConversionService enumerates untyped input objects, sends kind/value and builds a native selector vector (0x43de074). This DTO has a compatible candidate shape; no allocation/class check or producer/caller dataflow in that path proves these concrete receivers.')
        text = text.replace('Portable selector DTO for the Arc conversion service;',
                            'Selector DTO with a shape compatible with the Arc conversion service; concrete element-class identity requires producer/caller evidence, and')
    if cls == 'ArcContextMenuElementInfo':
        text = text.replace('dictionarySuggestions constructs a new array of native UTF-16 strings (0x443f654).',
                            'dictionarySuggestions requires engagement and constructs/returns NSMutableArray of ObjC strings converted from native UTF-16 elements, including an empty array; it sends no copy (0x443f654).')
    if cls == 'ArcURLLoadConfiguration':
        text = text.replace('URL/fileURL/string fields and headers are marshaled separately (0x440d678, 0x440d8b0, 0x440d9f8).',
                            'URL/fileURL/string fields are marshaled separately (0x440d678, 0x440d8b0). headers converts native key/value entries and returns a copied dictionary, empty when no entries exist (0x440d9f8).')
    if cls == 'ArcAnnotatedVisit':
        text = text.replace('## Lifecycle, callbacks and relations\n',
                            '## Lifecycle, callbacks and relations\n\nrelatedSearches returns an array of converted strings, with an empty-array fallback for absent native state; the engaged path copies its constructed array (0x43ff558).\n')
    if cls in ('ArcWebContentsMediaSession','ArcWebContentsThumbnailsGenerator'):
        note = ('\nConstructor recovery: Ghidra renders the owner as part of the handle aggregate and leaves param_4 unused. '
                'The encoding declares a 16-byte native handle and a separate ObjC owner. Bounded raw instructions '
                'save incoming x2 as a handle address and x3 as owner; saved x2 is dereferenced for handle words and '
                'saved x3 feeds the weak-store call. Keep this local register mapping distinct from a full caller-side '
                'C++ ABI/ownership guarantee.\n')
        text = text.replace('## Lifecycle, callbacks and relations\n',
                            '## Lifecycle, callbacks and relations\n'+note)
    if cls == 'ArcWebContentsMediaSession':
        text = text.replace('Action methods forward native operations and convert status/NSError via 0x4402548.',
                            'Action methods forward native operations and convert status/NSError via 0x4402548. getMediaContentTypes: instead returns a copied index-set object on success and nil after NSError conversion on failure; values below four are added, their enum names remain unknown, and missing engagement traps (0x441a848).')
    if cls == 'ArcAIService':
        text = text.replace('Other callback error shapes, cancellation and delivery queues remain unresolved.',
                            'Other callback error shapes, cancellation and delivery queues remain unresolved. Both evaluateCommandBarRanking:completion: and trainCommandBarRanking:completion: explicitly warn that type propagation is not settling (0x43d9ab8, 0x43d9fd4). Their bodies are available, but displayed extra arguments/aggregate mapping are not reliable ABI evidence; no corrupt function boundary is established.')
    if cls == 'ArcWebContentsDevTools':
        text = text.replace('Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.',
                            'Threading: notification helper 0x435c8fc copies callable state into a posted task, and 0x435ca2c invokes that callable. This supports ownership beyond return; executor identity, main-thread delivery, reentrancy and suppression/draining of already queued work remain unverified. Atomic reference counts do not establish a safe concurrent API. Candidate-backend scheduling requirements do not establish Arc scheduling.')
    affected = [c for c in corrections if c['class_name']==cls]
    if affected or cls == 'ArcWebContentsDevTools':
        nums = ', '.join(str(c['finding_row']) for c in affected)
        text += ('\n## Revision evidence\n\n'+
                 (f'Original review data rows {nums}; ' if nums else 'Additional analogous signature correction; ')+
                 'see [correction ledger](../corrections.tsv) and [independent recheck notes](../revision-evidence/notes.md). '
                 'The copied raw evidence remains the original snapshot; revised quality labels and contract limits are in this matrix and methods.tsv.\n')
    # Avoid gratuitous formatting edits to unaffected class files.
    if affected or cls == 'ArcWebContentsDevTools':
        (ROOT/'classes'/f'{cls}.md').write_text(text)

with (ROOT/'methods.tsv').open('w', newline='') as f:
    w=csv.DictWriter(f, fields, delimiter='\t');w.writeheader();w.writerows(rows)
with (ROOT/'corrections.tsv').open('w', newline='') as f:
    w=csv.DictWriter(f,list(corrections[0]),delimiter='\t');w.writeheader();w.writerows(corrections)
coverage=json.loads((ORIGINAL/'coverage.json').read_text())
coverage['body_status_counts']=dict(Counter(r['body_status'] for r in rows))
coverage['semantic_status_counts']=dict(Counter(r['semantic_status'] for r in rows))
coverage['unresolved_rows']=sum(bool(r['unresolved_question']) for r in rows)
coverage['explicitly_semantically_unresolved_rows']=sum(r['semantic_status']=='unresolved' for r in rows)
coverage['body_incoherent_semantics']=1
coverage['signature_or_type_artifact_rows']=sum(r['body_status']=='available_signature_artifact' for r in rows)
coverage['body_quality_flagged_rows']=sum(r['body_status']!='available' for r in rows)
coverage['body_malformed_established_definition']='Established corrupt function boundary; remains zero. Incoherent candidates semantics is counted separately and is not an engine-bug diagnosis.'
coverage['body_status_definitions']={
    'available':'Successful bounded export; no complete semantics/boundary certification.',
    'available_signature_artifact':'Successful export with explicit signature, argument or type-propagation artifact.',
    'available_incoherent_pseudocode':'Successful export with incoherent temporary lifetime/arguments; contents and ownership remain semantically unresolved.'}
coverage['revision']={'writer':'spec-2','version':'v1','original_findings':13,'ledger_rows':13,
                      'extra_method_annotations':1,'extra_class':'ArcWebContentsDevTools',
                      'identity_and_raw_evidence_preserved':True,
                      'validation':'revision-evidence/validation.json'}
for cls,item in coverage['per_class'].items():
    cr=[r for r in rows if r['class_name']==cls]
    item['semantic_status_counts']=dict(Counter(r['semantic_status'] for r in cr))
    item['body_status_counts']=dict(Counter(r['body_status'] for r in cr))
(ROOT/'coverage.json').write_text(json.dumps(coverage,indent=2)+'\n')
print(json.dumps({k:coverage[k] for k in ['body_status_counts','semantic_status_counts','unresolved_rows']},indent=2))
