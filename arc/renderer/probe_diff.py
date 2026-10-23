#!/usr/bin/env python3
"""Select the explicitly injected CSS calls from an app transcript.

The full unfiltered app transcripts remain beside the selected ones. Selection
uses the fixed probe identifier, then pairs each call with its own return by
thread, depth, receiver, class and selector. No values are rewritten.
"""
import json
from pathlib import Path
import subprocess
import sys

root = Path(__file__).resolve().parents[2]
result = Path(sys.argv[1]).resolve()
for name in ('trace156', 'ours156'):
    rows = [json.loads(line) for line in (result / name / 'record.jsonl').read_text().splitlines()]
    selected = []
    pending = set()
    calls = 0
    for row in rows:
        key = tuple(row.get(k) for k in ('thread', 'depth', 'self', 'cls', 'sel'))
        args = row.get('args', [])
        if (row.get('ev') == 'call' and row.get('cls') == 'ArcWebContents'
                and row.get('sel') in ('insertStylesheet:stylesheet:', 'removeStylesheet:')
                and args and isinstance(args[0], str) and args[0].startswith('arc-renderer-probe-')):
            if key in pending:
                raise SystemExit(f'{name}: nested duplicate call identity')
            selected.append(row)
            pending.add(key)
            calls += 1
        elif row.get('ev') == 'ret' and key in pending:
            selected.append(row)
            pending.remove(key)
    if calls != 10 or pending:
        raise SystemExit(f'{name}: expected 10 complete probe calls, got {calls}, pending={pending}')
    (result / name / 'stylesheet-record.jsonl').write_text(
        ''.join(json.dumps(row) + '\n' for row in selected))
subprocess.run([sys.executable, str(root / 'tests/recorder/diff.py'),
                str(result / 'trace156/stylesheet-record.jsonl'),
                str(result / 'ours156/stylesheet-record.jsonl'),
                '--json', str(result / 'diff-stylesheets.json')],
               stdout=(result / 'diff-stylesheets.txt').open('w'), check=True)
print('\n'.join((result / 'diff-stylesheets.txt').read_text().splitlines()[:2]))
report = json.loads((result / 'diff-stylesheets.json').read_text())
if any(report[key] for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')):
    raise SystemExit('stylesheet boundary differs')
