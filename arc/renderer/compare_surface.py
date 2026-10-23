#!/usr/bin/env python3
"""Compare eight classes' runtime surface; this does not prove implementation."""
import json
from pathlib import Path
import sys

root = Path(sys.argv[1]).resolve()


def public_surface(name):
    rows = set()
    for line in (root / (name + '.tsv')).read_text().splitlines():
        parts = line.split('\t')
        if ('.cxx_' in line or 'optional<' in line or
                '\tinitWithArcBrowserContext:' in line):
            continue
        if parts[0] not in ('C', 'M', 'R'):
            raise SystemExit(f'{name}: invalid surface row')
        rows.add(line)
    if sum(row.startswith('C\t') for row in rows) != 8:
        raise SystemExit(f'{name}: expected eight runtime classes')
    return rows


original, ours = [public_surface(name) for name in ('original', 'ours')]
report = {'missing': sorted(original - ours), 'extra': sorted(ours - original),
          'original_rows': len(original), 'ours_rows': len(ours)}
(root / 'comparison.json').write_text(json.dumps(report, indent=2) + '\n')
if report['missing'] or report['extra']:
    raise SystemExit('public method/property/class metadata differs; inspect comparison.json')
print(f'PASS: {len(original)} public surface rows across eight classes; '
      'implementation coverage is separate')
