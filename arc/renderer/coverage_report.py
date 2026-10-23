#!/usr/bin/env python3
"""Snapshot accepted golden coverage and current shared renderer diff entries.

This reads existing recorder evidence only. Shared ours artifacts can be
replaced by other workers; hashes and timestamps identify the bytes inspected.
It does not promote candidates or classify remaining differences as allowed.
"""
import collections
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
CLASSES = {
    'ArcBrowserContextEffects', 'ArcHSLAMatrix',
    'ArcBrowserContextPageContentExtraction', 'ArcExtractedPageContent',
    'ArcContentNode', 'ArcMetaTagEntry', 'ArcMetaTagSelector',
    'ArcHtmlToMarkdownResult',
}
SELECTORS = {
    'ArcBrowserApplication': {
        'setBlockLists:', 'setExcludedDomains:',
    },
    'ArcBrowserContext': {'getAdBlockDebugInfo:completion:'},
    'ArcWebContents': {
        'insertStylesheet:stylesheet:', 'removeStylesheet:',
        'applyColorFilter:', 'removeColorFilter', 'applyColorScheme:',
        'removeColorScheme', 'blockedResourceCount', 'setBlockedResourceCount:',
        'setTopBandColorDelegate:',
    },
}


def owned(cls, selector):
    return (cls in CLASSES or selector in SELECTORS.get(cls, set()) or
            'didSampleTopBandColor:' in selector)


def snapshot(path):
    before = path.stat()
    data = path.read_bytes()
    after = path.stat()
    if (before.st_mtime_ns, before.st_size) != (after.st_mtime_ns, after.st_size):
        raise RuntimeError(f'artifact changed during read: {path}')
    return data, {'path': str(path.relative_to(ROOT)),
                  'sha256': hashlib.sha256(data).hexdigest(),
                  'mtime_ns': after.st_mtime_ns}


report = {'goldens': {}, 'shared_diffs': {}}
for path in sorted((ROOT / 'results/golden').glob('arc-1.170-*/launch-*/record.jsonl')):
    if any('candidate' in part.lower() for part in path.parts):
        continue
    data, provenance = snapshot(path)
    calls = collections.Counter()
    callers = collections.Counter()
    for line in data.splitlines():
        row = json.loads(line)
        if row.get('ev') == 'call' and owned(row.get('cls'), row.get('sel', '')):
            calls[row['cls'] + ' ' + row['sel']] += 1
            callers[row.get('from', 'unattributed')] += 1
    report['goldens'][provenance['path']] = {**provenance, 'renderer_calls': dict(calls),
                                            'raw_callers': dict(callers)}

for path in sorted((ROOT / 'results/ours').glob('*/diff-launch-*.json')):
    try:
        data, provenance = snapshot(path)
        diff = json.loads(data)
        text_path = path.with_suffix('.txt')
        diff_text, text_provenance = snapshot(text_path)
    except FileNotFoundError:
        continue  # Another worker may be replacing this shared scenario.
    remaining = {}
    for kind, entries in diff.items():
        if not isinstance(entries, list):
            continue
        selected = []
        for entry in entries:
            method = entry.get('method', '') if isinstance(entry, dict) else ''
            match = re.search(r'[-+]([\w.]+) (.+)$', method)
            if match and owned(*match.groups()):
                selected.append(entry)
        if selected:
            remaining[kind] = selected
    report['shared_diffs'][provenance['path']] = {
        **provenance, 'text_provenance': text_provenance,
        'first_two_lines': diff_text.decode().splitlines()[:2],
        'renderer_differences': remaining,
    }

output = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / 'logs/wp/renderer.coverage.run2.json'
output.parent.mkdir(parents=True, exist_ok=True)
output.write_text(json.dumps(report, indent=2) + '\n')
print(f'{len(report["goldens"])} accepted launch transcripts; '
      f'{len(report["shared_diffs"])} shared diff snapshots; {output}')
