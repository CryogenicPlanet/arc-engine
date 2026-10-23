#!/usr/bin/env python3
"""Apply review data rows 1–4 to the seeded revision only."""
import collections
import csv
import json
from pathlib import Path

R = Path(__file__).resolve().parents[1]
ORIGINAL = Path('/home/exedev/work/agents/spec-1/specs')
REVIEW = Path('/home/exedev/work/agents/spec-4/classreview')
with (R/'methods.tsv').open(newline='') as f:
    reader = csv.DictReader(f, delimiter='\t'); fields = reader.fieldnames; rows = list(reader)
with (REVIEW/'findings.tsv').open(newline='') as f:
    findings = list(csv.DictReader(f, delimiter='\t'))
assert len(findings) == 4
changes = [
    dict(behavior='Returns native optional by value: empty input stays empty; engaged input copies peer/control from self+0x118/+0x120 and increments the nonnull control strong count. The engaged-path trap tests null aggregate return storage, not empty input.',
         unresolved_question='Aggregate return ABI and native peer invalidation require caller/producer validation; the null-storage check follows initial stores and is not a supported error-handling contract.'),
    dict(behavior='Traps if the outer native optional is empty. Returns nil for zero native color at self+0x60; otherwise returns NSString formatted with #%08X and the unsigned 32-bit color. Binary instructions recover the omitted variadic argument.',
         unresolved_question='Channel order, alpha meaning, color space and producer interpretation of zero remain unknown.',
         body_status='available_incoherent_vararg_repaired_by_binary'),
    dict(behavior='Clears/releases the stored samples object at self+8 through objc_storeStrong(slot, nil); accepts no caller-supplied replacement value.',
         backend_callees='objc_storeStrong via 0x8c7df80 -> 0xc4a6dc8',
         unresolved_question='Effects of releasing the stored collection and destruction-thread requirements outside this wrapper remain unverified.'),
    dict(behavior='Clears/releases the stored NSNumber at self+0x10 through objc_storeStrong(slot, nil); accepts no caller-supplied replacement value.',
         backend_callees='objc_storeStrong via 0x8c7df80 -> 0xc4a6dc8',
         unresolved_question='Effects of releasing the stored NSNumber and destruction-thread requirements outside this wrapper remain unverified.'),
]
corrections = []
for n, (finding, change) in enumerate(zip(findings, changes), 1):
    matching = [r for r in rows if r['class_name'] == finding['class'] and r['selector'] == finding['selector']]
    assert len(matching) == 1
    row = matching[0]
    before = row.copy()
    # Corrected local behavior remains supported; local_verified is not ABI certification.
    row.update(change)
    row['evidence'] += '; revision-evidence/finding-instructions.json; revision-evidence/finding-checks.json (ArcCore 1.170.0 arm64 unslid VAs)'
    path = R/'classes'/f"{row['class_name']}.md"
    lines = path.read_text().splitlines()
    count = 0
    for j, line in enumerate(lines):
        if not line.startswith('| '): continue
        cols = [c.strip() for c in line.split('|')[1:-1]]
        if len(cols) != 9 or cols[1] != row['kind']+row['selector'] or cols[3] != row['imp_va']: continue
        assert cols[0] == (row['category'] or '(base)')
        assert cols[4] == before['behavior']
        cols[4] = row['behavior']
        cols[5] = row['backend_callees'] or 'No direct anonymous call recorded / indirect possible'
        cols[6] += '; [revision instruction checks](../revision-evidence/finding-instructions.json); [body/import checks](../revision-evidence/finding-checks.json)'
        cols[7] = f"{row['confidence']} / {row['semantic_status']} / {row['body_status']}"
        cols[8] = row['unresolved_question']
        lines[j] = '| ' + ' | '.join(cols) + ' |'
        count += 1
    assert count == 1
    text = '\n'.join(lines)+'\n'
    if n == 1:
        text = text.replace('initWithWebContents 0x4440c4c copies',
            'web_contents 0x4443ee0 returns an optional by value: an empty source returns empty; an engaged source copies the native peer/control pair at +0x118/+0x120 and atomically increments a nonnull control strong count at control+8. The binary checks null aggregate return storage only inside the engaged path; it does not trap merely because the source optional is empty. Return storage is written before that check, so this is not a safe null-storage fallback. Cross-toolchain aggregate calling conventions and peer invalidation remain unvalidated.\n\ninitWithWebContents 0x4440c4c copies')
    elif n == 2:
        old = 'Text/link/image/iframe/form/table getters (0x4430ac4..0x44310ec) convert optional native strings; absence yields nil.'
        new = 'textContent/linkURL/imageAlt/iframeOrigin/formName/formActionURL/tableName (0x4430ac4..0x44310ec) convert optional native strings; absence yields nil. textColor 0x4430bdc instead requires the outer node optional, reads the unsigned color at self+0x60, returns nil for zero and otherwise formats an NSString with #%08X. Instructions at 0x4430c08 and 0x4430c0c/0x4430c10 recover the color vararg and format constant at 0xe244ab8 that pseudocode omits. No channel order, alpha meaning or color space is established.'
        assert old in text; text = text.replace(old, new)
    elif n == 3:
        old = 'destructor 0x4166068 clears its object field (ARC operations are injected).'
        new = 'destructor 0x4166068 clears/releases _samples at self+8 by passing that slot and nil to objc_storeStrong through 0x8c7df80. There is no supplied replacement argument; its exact metadata type remains v16@0:8. The indexed body is named for an unrelated class sharing this IMP.'
        assert old in text; text = text.replace(old, new)
    else:
        old = '0x425b6d8 clears the NSNumber field.'
        new = '0x425b6d8 clears/releases _value (NSNumber) at self+0x10 by passing the slot and nil to objc_storeStrong through 0x8c7df80. This destructor accepts no supplied replacement argument; its exact metadata type remains v16@0:8.'
        assert old in text; text = text.replace(old, new)
    path.write_text(text)
    corrections.append(dict(finding_row=n, class_name=row['class_name'], selector=row['selector'],
        disposition='corrected', before=before['behavior'], after=row['behavior'],
        evidence=f"ArcCore 1.170.0 arm64 unslid {row['imp_va']}; bodies/{row['imp_va']}.c; revision-evidence/finding-instructions.json; revision-evidence/finding-checks.json; original finding data row {n}",
        remaining_unknown=row['unresolved_question']))

with (R/'methods.tsv').open('w', newline='') as f:
    w = csv.DictWriter(f, fields, delimiter='\t'); w.writeheader(); w.writerows(rows)
with (R/'corrections.tsv').open('w', newline='') as f:
    w = csv.DictWriter(f, ['finding_row','class_name','selector','disposition','before','after','evidence','remaining_unknown'], delimiter='\t')
    w.writeheader(); w.writerows(corrections)

coverage = json.loads((R/'coverage.json').read_text())
coverage['body_status'] = dict(collections.Counter(r['body_status'] for r in rows))
coverage['semantic_status'] = dict(collections.Counter(r['semantic_status'] for r in rows))
coverage['rows_with_open_question'] = sum(bool(r['unresolved_question']) for r in rows)
for name, c in coverage['by_class'].items():
    subset = [r for r in rows if r['class_name'] == name]
    c['body_status'] = dict(collections.Counter(r['body_status'] for r in subset))
    c['semantic_status'] = dict(collections.Counter(r['semantic_status'] for r in subset))
coverage['cef_commit'] = '426920c33ef22551e0b8f48cf343d623ac25642d'
coverage['revision_review'] = dict(review='/home/exedev/work/agents/spec-4/classreview/findings.tsv',
    finding_rows=4, corrected_rows=4, unchanged_dispositions=0,
    changed_method_annotation_rows=4, changed_class_files=4,
    extra_method_annotation_changes=0, analogous_rows_examined=86, analogous_distinct_bodies=61)
(R/'coverage.json').write_text(json.dumps(coverage, indent=2)+'\n')
print('Corrected four original finding data rows; retained local_verified for their corrected local observations.')
