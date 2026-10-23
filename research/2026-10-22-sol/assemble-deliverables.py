#!/usr/bin/env python3
"""Prepare reviewable sibling Arc/Arc-engine deliverables AFTER genuine packaging.

Usage: assemble-deliverables.py --package /NEW_WORKSPACE/arc-engine

Offline, stdlib only. Never publishes, overwrites, manufactures readiness, or
modifies evidence/** or specs/Arc*.md. Exit 2 means staged links need parent review;
exit 1 means a prerequisite/invariant failed (partial new outputs are retained).
"""
import argparse
import csv
import importlib.util
import io
import json
from pathlib import Path
import posixpath
import re
import sys
from urllib.parse import quote, unquote

RAW_REPORT = 'agents/review/synthesis/engine-feasibility.md'
PROTOCOL_BASE = 'agents/protocols/initial'
REVIEW_JSON = 'research/2026-10-22-sol/deliverable-stage-review.json'
FINAL_ROOT = Path('/Users/cryogenicplanet/general')
EXTRA_METHOD_FIELDS = ('image', 'source_writer', 'source_methods', 'source_class_spec')


def helper():
    # Use the trusted checkout helper, not executable code supplied by a package.
    path = Path(__file__).with_name('package-evidence.py')
    spec = importlib.util.spec_from_file_location('arc_evidence_package', path)
    module = importlib.util.module_from_spec(spec)
    previous = sys.dont_write_bytecode
    sys.dont_write_bytecode = True
    try:
        spec.loader.exec_module(module)
    finally:
        sys.dont_write_bytecode = previous
    return module


def table(store, name):
    reader = csv.DictReader(io.StringIO(store.read(name).decode()), delimiter='\t')
    fields = reader.fieldnames
    if not fields or len(fields) != len(set(fields)):
        raise ValueError('Missing/duplicate TSV schema: ' + name)
    rows = list(reader)
    if any(None in row or any(value is None for value in row.values()) for row in rows):
        raise ValueError('Malformed TSV row: ' + name)
    return fields, rows


def tsv(fields, rows):
    stream = io.StringIO(newline='')
    writer = csv.DictWriter(stream, fields, delimiter='\t', lineterminator='\n')
    writer.writeheader()
    writer.writerows(rows)
    return stream.getvalue()


def relative_link(source, target):
    # Both arguments must be absolute: never let the invocation CWD affect links.
    if not source.is_absolute() or not target.is_absolute():
        raise ValueError('Link endpoints must be absolute')
    return quote(posixpath.relpath(target.as_posix(), source.parent.as_posix()), safe='/.-_')


def md_cell(value):
    return str(value).replace('|', '\\|').replace('\n', '<br>')


def markdown_link(label, source, target):
    return '[' + md_cell(label) + '](' + relative_link(source, target) + ')'


def method_index(api, store, ready, raw_mapping):
    fields, combined, classes = [], [], {}
    for item in sorted(ready['selected'], key=lambda row: row['writer']):
        writer = item['writer']
        base = f'agents/{writer}/revisions/v1'
        name = base + '/methods.tsv'
        original_fields, records = table(store, name)
        api.require(not set(original_fields).intersection(EXTRA_METHOD_FIELDS),
                    'Reserved combined-method column already exists: ' + name)
        for field in original_fields:
            if field not in fields:
                fields.append(field)
        coverage = api.load(store, base + '/coverage.json')
        image = coverage.get('image')
        api.require(isinstance(image, str) and image.strip(), 'Missing source image: ' + base)
        for class_name in item['classes']:
            classes[class_name] = {'writer': writer, 'count': 0,
                                  'review': f'agents/{writer}/revision-reviews/v1/report.md',
                                  'review_table': f'agents/{writer}/revision-reviews/v1/class-review.tsv'}
        for row in records:
            api.require(row['class_name'] in item['classes'], 'Method assigned to wrong writer')
            classes[row['class_name']]['count'] += 1
            combined.append({**row, 'image': image, 'source_writer': writer,
                             'source_methods': raw_mapping[name]['output_path'],
                             'source_class_spec': raw_mapping[base + '/classes/' + row['class_name'] + '.md']['output_path']})
    api.require(len(classes) == ready['classes'] and len(combined) == ready['declared_methods'],
                'Combined class/method count differs from verified readiness')
    api.require(len(api.identities(combined, 'imp_va')) == ready['declared_methods'],
                'Combined methods duplicate exact identities')
    # Explicitly compare again, including category, encoding and numeric IMP.
    metadata = api.rows(store, 'agents/arc-layer/initial/objc-methods.tsv')
    expected = api.identities([r for r in metadata if r['class_name'] in classes and not r['protocol']], 'imp')
    api.require(api.identities(combined, 'imp_va') == expected, 'Combined identities differ from raw metadata')
    return fields + list(EXTRA_METHOD_FIELDS), combined, classes


def protocol_index(api, store, raw_mapping):
    source = PROTOCOL_BASE + '/protocols.tsv'
    fields, protocols = table(store, source)
    _, callbacks = table(store, PROTOCOL_BASE + '/callbacks.tsv')
    coverage = api.load(store, PROTOCOL_BASE + '/coverage.json')
    names = [r['protocol_name'] for r in protocols]
    api.require(len(names) == len(set(names)), 'Duplicate protocol')
    identities = {(r['protocol_name'], r['kind'], r['selector']) for r in callbacks}
    api.require(len(identities) == len(callbacks), 'Duplicate callback identity')
    api.require({r['protocol_name'] for r in callbacks} == set(names), 'Protocol/callback coverage mismatch')
    candidates = [r for r in callbacks if r['classification'] != 'non_callback_value_protocol']
    values = [r for r in callbacks if r['classification'] == 'non_callback_value_protocol']
    static = [r for r in candidates if 'binary_static_selector_sends:' in r['engine_invocation_evidence']]
    totals = {'protocols': len(protocols), 'identities': len(callbacks),
              'candidate_identities': len(candidates), 'static_candidate_identities': len(static),
              'value_protocols': len({r['protocol_name'] for r in values}), 'value_identities': len(values)}
    for key, expected in [('protocol_specs', totals['protocols']), ('callback_rows', totals['identities']),
                          ('candidate_callback_or_app_service_identities', totals['candidate_identities']),
                          ('candidate_callback_identities_with_binary_selector_send', totals['static_candidate_identities']),
                          ('non_callback_value_protocols', totals['value_protocols']),
                          ('non_callback_value_identities', totals['value_identities'])]:
        api.require(coverage.get(key) == expected, 'Protocol census disagrees with coverage: ' + key)
    # These are verified scope requirements, not substituted counts.
    api.require((totals['protocols'], totals['identities'], totals['candidate_identities'],
                 totals['static_candidate_identities']) == (26, 200, 195, 189),
                'Unexpected protocol audit scope; review assembler before use')
    added = ['declared_identities', 'candidate_identities', 'static_candidate_identities',
             'source_protocol_spec', 'source_callback_census']
    api.require(not set(fields).intersection(added), 'Reserved protocol index column collision')
    result = []
    for row in protocols:
        name = row['protocol_name']
        api.require(re.fullmatch(r'Arc\w+', name), 'Unsafe protocol name')
        members = [r for r in callbacks if r['protocol_name'] == name]
        candidate_members = [r for r in candidates if r['protocol_name'] == name]
        static_members = [r for r in static if r['protocol_name'] == name]
        spec = PROTOCOL_BASE + '/protocol-specs/' + name + '.md'
        api.require(spec in raw_mapping, 'Missing retained protocol spec: ' + name)
        result.append({**row, 'declared_identities': str(len(members)),
                       'candidate_identities': str(len(candidate_members)),
                       'static_candidate_identities': str(len(static_members)),
                       'source_protocol_spec': raw_mapping[spec]['output_path'],
                       'source_callback_census': raw_mapping[PROTOCOL_BASE + '/callbacks.tsv']['output_path']})
    return fields + added, result, totals


def readme(api, root, mapping, classes, protocols, totals):
    source = root / 'specs/README.md'
    link = lambda label, name: markdown_link(label, source, root / name)
    rawlink = lambda label, name: link(label, mapping[name]['output_path'])
    lines = ['# Arc engine class and protocol research index', '',
             '**Staged for parent review; not published.**', '',
             f"The verified readiness selection covers {len(classes)} Arc-prefixed metadata classes and "
             f"{sum(c['count'] for c in classes.values()):,} exact declared method identities. "
             'These are bounded static research specifications. Declared coverage does not establish '
             'complete behavior recovery, receiver reachability, full native ABI compatibility, '
             'implementation readiness, or runtime acceptance. No runtime native changes were performed by this research.', '',
             markdown_link('Main feasibility report', source, root.parent / 'arc/notes/engine-feasibility.md')
             + '; ' + link('combined methods', 'specs/methods.tsv') + '; '
             + link('protocol index', 'specs/protocol-index.tsv') + '.', '',
             rawlink('Readiness selection', 'CLASS-REVISIONS-READY') + '; '
             + rawlink('global synthesis', RAW_REPORT) + '; '
             + rawlink('global completion proof', 'review-synthesis.completion.json') + '.', '',
             'The methods table preserves the union of the four original schemas and every original value, '
             'including evidence strings and source-specific confidence/semantic labels. Those labels are '
             'not a common score and must not be ranked or aggregated across writers. Added image strings '
             'come verbatim from each writer coverage.json. Added source paths are repository-root-relative; '
             'source_methods and source_class_spec identify the immutable corrected source table and class document.', '',
             '| Class | Declared methods | Corrected writer | Fresh revision review |',
             '|---|---:|---|---|']
    for name, info in sorted(classes.items()):
        lines.append('| ' + link(name, 'specs/' + name + '.md') + ' | ' + str(info['count'])
                     + ' | ' + info['writer'] + ' | ' + rawlink('report', info['review'])
                     + '; ' + rawlink('class records', info['review_table']) + ' |')
    lines += ['', '## Protocols and callback census', '',
              f"The retained audit covers {totals['protocols']} protocols and {totals['identities']} "
              f"protocol/kind/selector identities: {totals['candidate_identities']} candidate callback or app-service "
              f"identities, of which {totals['static_candidate_identities']} have bounded static binary selector-send "
              f"evidence. The other {totals['value_identities']} identities belong to {totals['value_protocols']} "
              'value protocols; they are not callbacks. Static send evidence does not prove the dynamic receiver '
              'or execution, callback ordering, ownership, thread guarantees, or complete semantics.', '',
              rawlink('Protocol report', PROTOCOL_BASE + '/report.md') + '; '
              + rawlink('typed identity census', PROTOCOL_BASE + '/callbacks.tsv') + '; '
              + rawlink('coverage definitions', PROTOCOL_BASE + '/coverage.json') + '.', '',
              '| Protocol | Purpose | Retained specification |', '|---|---|---|']
    for row in sorted(protocols, key=lambda row: row['protocol_name']):
        name = row['protocol_name']
        lines.append('| ' + name + ' | ' + md_cell(row['purpose_classification']) + ' | '
                     + rawlink('spec', PROTOCOL_BASE + '/protocol-specs/' + name + '.md') + ' |')
    lines += ['', '## Review and reproduction limits', '',
              'Original source records, hashes, corrected artifacts and fresh reviews remain in '
              + link('immutable evidence', api.EVIDENCE + '/BUNDLE-README.md') + '. '
              'Large binaries, the full corpus/index, Ghidra project and source/build trees are outside '
              'this compact package. Verification checks retained integrity and exact declared coverage; '
              'it is not a self-contained research rerun or behavioral acceptance test. The staging-only '
              + link('review inventory', REVIEW_JSON) + ' records unresolved/broken links for parent review.', '']
    return '\n'.join(lines)


def handoff(root, class_count, method_count):
    report = FINAL_ROOT / 'arc/notes/engine-feasibility.md'
    engine = FINAL_ROOT / 'arc-engine'
    evidence = engine / 'research/2026-10-22-sol/evidence'
    return f'''# Claude engine handoff — staged for parent review

The prompt below names the intended final repository paths. Parent review and
publication of both sibling trees must happen before it is handed to Claude.
This assembler does not publish files or authorize implementation/capacity changes.

```text
Continue Arc engine planning from the finished, parent-reviewed research at:
- Main report: {report}
- Class/protocol index: {engine / 'specs/README.md'}
- Exact method index: {engine / 'specs/methods.tsv'}
- Protocol index: {engine / 'specs/protocol-index.tsv'}
- Retained evidence and provenance: {evidence}
- Offline package verifier: {evidence / '_package_tools/package-evidence.py'}

First read the live /Users/cryogenicplanet/general/arc/README.md,
/Users/cryogenicplanet/general/arc/notes/runtime-status.md and relevant
/Users/cryogenicplanet/general/arc/notes/HANDOFF.md sections, then live
{engine / 'ENGINE.md'} and {engine / 'NEXT.md'}.
Read the report, index and cited evidence before selecting work. The static
research covers {class_count} classes and {method_count:,} declared identities;
declared coverage is not full method behavior recovery, implementation readiness,
native ABI equivalence or runtime acceptance. No runtime native changes were
performed by this research. Preserve source-specific confidence definitions.

Use the report-supported comparison of possible engine routes and its explicit
unknowns to propose one narrow, testable next milestone and its acceptance gates.
Do not rerun the full pilot or turn untested compatibility assumptions into facts.
Do not begin implementation merely because declared coverage is complete.

arcmod-engine is PAUSED; its disk/project is preserved. Never SSH to it, resume,
restart or resize it without Rahul's new explicit approval. Needed evidence is
already on the included pooled arcmod-research VM under ~/work, including the
corpus index and read-function.py byte-range reader. Read the live handoff for
exact locations. No new paid capacity. Original evidence, completed jobs and
frozen inputs are immutable; do not duplicate launches or remove done markers.
All research, spec and reviewer subagents must use gpt-6.1-sol at high reasoning.

For shared coordination, fetch https://chirp.cryo.wtf/init and follow its live
protocol. Use Claude's own enrolled credentials and identity; never read or use
Codex credentials in ~/.chirp/codex/. Check relevant board context before posting.
Preserve the original app/profile and ArcMod account-free behavior, service
filtering, single Little Arc, and native ownership/calling conventions. Follow
the live repository instructions for any later approved change and validation.
```
'''


def check_links(api, workspace, files):
    workspace = workspace.resolve()
    planned = {path.resolve() for path in files}
    issues = []
    for source, content in files.items():
        def check(value):
            if not value or value.startswith('#') or api.external_link(value):
                return value
            path, _ = api.split_destination(value)
            path = unquote(path)
            candidate = source.parent / path
            resolved = candidate.resolve()
            reason = None
            if path.startswith('/'):
                reason = 'absolute local link is not portable'
            elif not resolved.is_relative_to(workspace):
                reason = 'local link escapes sibling workspace'
            elif resolved not in planned and not resolved.exists():
                reason = 'missing local target'
            elif candidate.is_symlink():
                reason = 'linked target is not a regular staged target'
            if reason:
                issues.append({'source': str(source.relative_to(workspace)),
                               'destination': value, 'reason': reason})
            return value
        api.markdown_links(content, check)
    return issues


def assemble(package):
    api = helper()
    api.require(package.is_absolute() and package.name == 'arc-engine',
                '--package must be an absolute /NEW_WORKSPACE/arc-engine path')
    api.require(not package.is_symlink(), 'Linked package root refused')
    root = package.resolve(strict=True)
    source_root = Path(__file__).resolve().parents[2]
    api.require(root.name == 'arc-engine' and root != source_root,
                'Use a new sibling-workspace stage, not the source repository')
    workspace = root.parent
    arc = workspace / 'arc'
    for directory in (root, arc, arc / 'notes', root / 'specs', root / 'research/2026-10-22-sol'):
        api.require(not directory.is_symlink(), 'Linked output directory refused: ' + str(directory))
    api.require(not (root / '.git').exists() and not (arc / '.git').exists(),
                'Refusing repository checkout destinations; use a new workspace stage')
    targets = [arc / 'notes/engine-feasibility.md', arc / 'notes/engine-claude-handoff.md',
               root / 'specs/README.md', root / 'specs/methods.tsv', root / 'specs/protocol-index.tsv',
               root / REVIEW_JSON]
    for target in targets:
        api.require(not target.exists() and not target.is_symlink(), 'Refusing existing target: ' + str(target))
    before = api.verify(root)  # No stage mutation occurs before genuine verification.
    manifest = json.loads((root / api.EVIDENCE / 'relocation-manifest.json').read_text())
    raw = [record for record in manifest['files'] if record['transform'] != 'presentation-markdown']
    mapping = {record['source_relative']: record for record in raw}
    store = api.Packaged(root, raw)
    ready = api.validate_ready(store)
    fields, methods, classes = method_index(api, store, ready, mapping)
    protocol_fields, protocols, totals = protocol_index(api, store, mapping)
    # The helper requires matching coordinate systems: absolute output and absolute
    # relocation targets. Raw canonical source names retain the Linux namespace.
    absolute_mapping = {name: {**record, 'output_path': str(root / record['output_path'])
                              if record['output_path'] else None}
                        for name, record in mapping.items()}
    rebase_issues = []
    report_path, handoff_path, index_path, methods_path, protocols_path, review_path = targets
    report = api.rebase_markdown(store.read(RAW_REPORT).decode(), RAW_REPORT, str(report_path),
                                 absolute_mapping, manifest['chromium_pin'], rebase_issues)
    report += ('\n\nResearch provenance and index: '
               + markdown_link(f'{len(classes)} class specifications', report_path, index_path)
               + '; ' + markdown_link('raw global synthesis', report_path, root / mapping[RAW_REPORT]['output_path'])
               + '; ' + markdown_link('frozen completion proof', report_path,
                                      root / mapping['review-synthesis.completion.json']['output_path'])
               + '; ' + markdown_link('readiness selection', report_path,
                                      root / mapping['CLASS-REVISIONS-READY']['output_path'])
               + '. This is bounded static research, not runtime acceptance or a claim that '
               'every method behavior is fully recovered. Staged for parent review; not published.\n')
    outputs = {report_path: report, handoff_path: handoff(root, len(classes), len(methods)),
               index_path: readme(api, root, mapping, classes, protocols, totals),
               methods_path: tsv(fields, methods), protocols_path: tsv(protocol_fields, protocols)}
    # The review JSON is a future local target referenced by the generated README.
    markdown_files = {path: text for path, text in outputs.items() if path.suffix == '.md'}
    markdown_files[review_path] = ''
    for path in (root / 'specs').glob('Arc*.md'):
        markdown_files[path] = path.read_text()
    broken = check_links(api, workspace, markdown_files)
    for records, path_fields in ((methods, ('source_methods', 'source_class_spec')),
                                 (protocols, ('source_protocol_spec', 'source_callback_census'))):
        for row in records:
            for field in path_fields:
                name = api.relative(row[field])
                target = root / name
                api.require(target.is_file() and not target.is_symlink(), 'Missing index source: ' + name)
    existing = {'presentation': manifest['link_issues'], 'raw': manifest['raw_link_issues']}
    needs_review = bool(rebase_issues or broken or any(existing.values()))
    review = {'version': 1, 'state': 'pending-parent-review', 'published': False,
              'scope': 'bounded static research; integrity and declared coverage only; no runtime acceptance',
              'package': str(root), 'workspace': str(workspace), 'verify_before': before,
              'counts': {'classes': len(classes), 'declared_methods': len(methods), **totals},
              'report_source': RAW_REPORT,
              'report_source_sha256': mapping[RAW_REPORT]['source_sha256'],
              'report_rebase_issues': rebase_issues, 'broken_local_links': broken,
              'existing_package_link_issues': existing,
              'review_required': True, 'link_review_required': needs_review,
              'intended_final_root': str(FINAL_ROOT),
              'generated_files': {str(path.relative_to(workspace)): api.sha_bytes(text.encode())
                                  for path, text in outputs.items()}}
    try:
        for path, content in outputs.items():
            path.parent.mkdir(parents=True, exist_ok=True)
            with path.open('x', encoding='utf-8', newline='') as stream:
                stream.write(content)
        for path, content in outputs.items():
            api.require(path.read_text(encoding='utf-8') == content,
                        'Written deliverable differs from planned content: ' + str(path))
        after = api.verify(root)
        api.require(after == before, 'Immutable package verification result changed during assembly')
        review['verify_after'] = after
        with review_path.open('x', encoding='utf-8') as stream:
            json.dump(review, stream, indent=2, sort_keys=True)
            stream.write('\n')
    except Exception:
        print('Partial staged outputs retained for parent inspection: ' + str(workspace), file=sys.stderr)
        raise
    print(json.dumps({'state': 'pending-parent-review', 'published': False,
                      'workspace': str(workspace), 'review_inventory': str(review_path),
                      'counts': review['counts'], 'package_verified_before_and_after': True,
                      'report_rebase_issues': len(rebase_issues), 'broken_local_links': len(broken),
                      'package_presentation_issues': len(existing['presentation']),
                      'package_raw_issues': len(existing['raw']), 'link_review_required': needs_review}, indent=2))
    return 2 if needs_review else 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--package', type=Path, required=True)
    args = parser.parse_args()
    try:
        return assemble(args.package)
    except (ValueError, OSError, KeyError, TypeError, csv.Error) as error:
        print('Assembly refused/failed: ' + str(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
