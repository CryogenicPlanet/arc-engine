#!/usr/bin/env python3
"""Build reviewer-owned navigation; never rewrites selected source specs/reviews."""
import csv
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
AGENTS = ROOT.parents[1]

def rows(path):
    with Path(path).open(newline='') as f:
        return list(csv.DictReader(f, delimiter='\t'))

def write_tsv(path, fields, data):
    with path.open('w', newline='') as f:
        w = csv.DictWriter(f, fields, delimiter='\t', lineterminator='\n')
        w.writeheader()
        w.writerows(data)

def esc(s):
    return str(s).replace('|', '\\|').replace('\n', ' ')

specs = rows(ROOT / 'spec-index.tsv')
assert len(specs) == len({r['class_name'] for r in specs}) == 146
assert sum(int(r['declared_methods']) for r in specs) == 1964
reviews = {}
for n in range(1, 5):
    for r in rows(AGENTS / f'spec-{n}/revision-reviews/v1/class-review.tsv'):
        reviews[r['class_name']] = r
assert set(reviews) == {r['class_name'] for r in specs}

out = ['# Coverage appendix: selected class revisions and protocol surface', '',
       'Selection: `/home/exedev/work/CLASS-REVISIONS-READY`, SHA-256 '
       '`58b7e65868a774783345bc65391f16d6d983e61f648a2e604a5ec9d2d69afc8e`. '
       'All class links below select v1 revisions. Reviews are fresh independent review artifacts stored beneath each writer group; '
       'the storage path does not identify a human/model author. Historical reviewer paths are separately recorded in '
       '[spec-index.tsv](spec-index.tsv). Review acceptance means bounded static research, with the stated unknowns retained.', '',
       'All 146 classes / 1,964 declaration rows reconcile mechanically. Globally there are 1,566 IMPs; '
       '1,565 have successful exact indexed bodies and the remaining pane-host entry is a verified branch veneer. '
       'Body presence and local review labels do not certify semantics. There are 1,670 rows with open questions and '
       '97 explicitly unresolved rows. Different groups use different local semantic vocabularies; their labels are not comparable percentages.', '',
       '| Writer | Classes | Rows | Base/category | Instance/class | Group distinct IMPs | Open-question rows | Explicit unresolved |',
       '|---|---:|---:|---|---|---:|---:|---:|']
for r in rows(ROOT / 'evidence/groups.tsv'):
    out.append(f"| {r['writer']} | {r['classes']} | {r['methods']} | {r['base']}/{r['category']} | {r['instance']}/{r['class_methods']} | {r['distinct_imps']} | {r['open_questions']} | {r['explicit_unresolved']} |")
out += ['', 'Group distinct-IMP counts overlap and must not be added. Every matrix identity includes class, category, method kind and selector.', '',
        '## All selected classes', '',
        '| Class / selected spec | Writer | Declarations | Fresh review | Synthesis qualification / retained review limits |',
        '|---|---|---:|---|---|']
for r in specs:
    review = reviews[r['class_name']]
    lim = review['limits']
    if r['class_name'] == 'ArcSessionID':
        lim += ' SYNTHESIS CORRECTION: old raw encoding ambiguity is closed by the later protocol audit and 12 parent entry checks; sentinels, restore behavior and CEF ID correspondence remain open.'
    out.append(f"| [{r['class_name']}]({r['path']}) | {r['writer']} | {r['declared_methods']} | [{r['status']}]({r['review_path']}) | {esc(lim)} |")

p_root = AGENTS / 'protocols/initial'
protocols = rows(p_root / 'protocols.tsv')
callbacks = rows(p_root / 'callbacks.tsv')
assert len(protocols) == 26 and len(callbacks) == 200
by_protocol = defaultdict(list)
for r in callbacks:
    by_protocol[r['protocol_name']].append(r)
p_index = []
out += ['', '## All 26 protocol specs', '',
        'This guide surface contains 235 physical entries, 200 unique protocol/kind/selector identities and 198 selectors. '
        'Two DevTools protocols describe five value getters. The other 24 contain 195 candidate callback/app-service identities, '
        'of which 189 have static-send support. Combined evidence has 195 send VAs / 192 selectors supporting 194 identities. '
        'These are metadata/static-send counts, with dynamic receiver and execution limits.', '',
        '| Protocol / spec | Purpose | Identities | Static-send-supported identities | App adopters |',
        '|---|---|---:|---:|---|']
for p in protocols:
    name = p['protocol_name']
    rs = by_protocol[name]
    supported = sum('binary_static_selector_sends:' in r['engine_invocation_evidence'] for r in rs)
    path = p_root / 'protocol-specs' / (name + '.md')
    assert path.is_file()
    d = dict(protocol_name=name, purpose=p['purpose_classification'], identities=len(rs),
             static_send_supported_identities=supported,
             classifications=';'.join(sorted({r['classification'] for r in rs})),
             path=str(path), callbacks_path=str(p_root / 'callbacks.tsv'),
             combined_send_evidence=str(p_root / 'evidence/engine-all-static-sends.tsv'),
             adopter_evidence=p['adopter_classes'], limits=p['unresolved_limits'])
    p_index.append(d)
    out.append(f"| [{name}]({path}) | {esc(p['purpose_classification'])} | {len(rs)} | {supported} | {esc(p['adopter_classes'])} |")
assert sum(int(r['static_send_supported_identities']) for r in p_index) == 194
write_tsv(ROOT / 'protocol-index.tsv', list(p_index[0]), p_index)
out += ['', 'Use the combined send table linked in protocol-index.tsv for '
        '`webContents:updateDevToolsToolbox:` at engine VA `0x4447edc`; the earlier narrow table omits this focus-only row. '
        'Use the termination correction in engine-feasibility.md when reading the historical protocol report. '
        'All six formerly uncertain old/current raw encodings agree, including signed `q` for `ArcSessionID.identifier`; '
        'this is not native C++ ABI or behavioral equality.', '',
        '## Review dispositions', '',
        'All 32 original class-review finding rows are mapped, without dropping historical findings, in '
        '[correction-dispositions.tsv](evidence/correction-dispositions.tsv): eight resolved and 24 resolved_with_limits. '
        'The fresh finding tables are header-only. Selected class docs, correction ledgers and fresh review limits '
        'remain the authorities; historical evidence copies may retain superseded language. '
        'Synthesis findings, including the stale ArcSessionID review limit, are in [findings.tsv](findings.tsv).', '']
(ROOT / 'coverage-appendix.md').write_text('\n'.join(out))
print('PASS: 146 selected classes; 1964 rows; 26 protocols; 200 identities; 194 static-send-supported identities')
