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
strict_sequences = {}
for name in ('trace156', 'ours156'):
    rows = [json.loads(line) for line in (result / name / 'record.jsonl').read_text().splitlines()]
    selected = []
    pending = {}
    ordered_calls = []
    receivers = set()
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
            call = {k: row.get(k) for k in ('dir', 'kind', 'cls', 'sel', 'thread', 'args')}
            if call['thread'] != 'main':
                raise SystemExit(f'{name}: CSS call left the main thread')
            ordered_calls.append(call)
            pending[key] = call
            receivers.add(row.get('self'))
            calls += 1
        elif row.get('ev') == 'ret' and key in pending:
            selected.append(row)
            pending.pop(key)['ret'] = row.get('ret')
    if calls != 10 or pending or len(receivers) != 1:
        raise SystemExit(f'{name}: expected 10 complete probe calls, got {calls}, pending={pending}')
    strict_sequences[name] = ordered_calls
    (result / name / 'stylesheet-record.jsonl').write_text(
        ''.join(json.dumps(row) + '\n' for row in selected))
# Compare the known test-peer calls directly as well. The shared caller-aware
# diff can omit injected images; preserving its report alone could hide all ten
# calls. No arguments, return values, ordering or thread names are rewritten.
strict = {'calls_per_engine': {name: len(rows) for name, rows in strict_sequences.items()},
          'equal': strict_sequences['trace156'] == strict_sequences['ours156'],
          'sequences': strict_sequences}
(result / 'stylesheet-strict-comparison.json').write_text(json.dumps(strict, indent=2) + '\n')
if not strict['equal']:
    raise SystemExit('raw stylesheet arguments, returns, order or thread differ')
subprocess.run([sys.executable, str(root / 'tests/recorder/diff.py'),
                str(result / 'trace156/stylesheet-record.jsonl'),
                str(result / 'ours156/stylesheet-record.jsonl'),
                '--json', str(result / 'diff-stylesheets.json')],
               stdout=(result / 'diff-stylesheets.txt').open('w'), check=True)
print('\n'.join((result / 'diff-stylesheets.txt').read_text().splitlines()[:2]))
report = json.loads((result / 'diff-stylesheets.json').read_text())
if any(report[key] for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')):
    raise SystemExit('stylesheet boundary differs')
