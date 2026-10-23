#!/usr/bin/env python3
"""Compare genuine original/ours feature observations without rewriting values.

The harness summary covers stylesheet snapshots only. This separately checks
every optional feature observation file present in either run and rejects
incomplete runs, so stylesheet parity cannot mask a feature failure.
"""
import json
from pathlib import Path
import sys

root = Path(sys.argv[1]).resolve()
summary = json.loads((root / 'summary.json').read_text())
if not summary.get('complete') or summary.get('parity') is not True:
    raise SystemExit('incomplete run or stylesheet mismatch')
checked = []
differences = []
for filename in ('effects-observations.json', 'adblock-observations.json',
                 'scheme-observations.json'):
    paths = [root / name / filename for name in ('trace156', 'ours156')]
    if not any(path.exists() for path in paths):
        continue
    if not all(path.exists() for path in paths):
        raise SystemExit(f'incomplete feature output: {filename}')
    original, ours = [json.loads(path.read_text()) for path in paths]
    if not original or len(original) != len(ours):
        differences.append({'file': filename, 'original': original, 'ours': ours})
        continue
    for expected, actual in zip(original, ours):
        if expected != actual:
            differences.append({'file': filename, 'step': expected.get('step'),
                                'original': expected, 'ours': actual})
    checked.append({'file': filename, 'observations': len(original)})
if not checked:
    raise SystemExit('no optional features observed')
report = {'checked': checked, 'differences': differences}
(root / 'feature-comparison.json').write_text(json.dumps(report, indent=2) + '\n')
if differences:
    raise SystemExit('feature observations differ; inspect feature-comparison.json')
print('PASS: exact feature observations: ' + ', '.join(
    f'{item["file"]} ({item["observations"]})' for item in checked))
