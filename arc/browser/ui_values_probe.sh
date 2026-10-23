#!/bin/bash
# Bounded ObjC DTO/POD behavior and public metadata comparison. No build or
# browser launch. Does not establish app scenario or native service acceptance.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ] && [ "${1:-}" != --compile-only ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked "$@"
fi
[ "${1:-}" != --locked ] || shift
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-ui-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
clang++ -std=c++17 -Wall -Wextra -Werror -fobjc-arc -framework Cocoa \
  arc/browser/ui_values_probe.mm -o "$probe_dir/probe"
if [ "${1:-}" = --compile-only ]; then
  echo 'UI probe compiled successfully; no engine loaded.'
  exit 0
fi
if [ $# -gt 1 ] || { [ $# -eq 1 ] && [ "$1" != --record ]; }; then
  echo "usage: $0 [--compile-only|--record]" >&2
  exit 2
fi
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"
  [ "$name" != ours ] || framework="$ours"
  out="$E/results/ui-values/$name"
  mkdir -p "$out"
  "$probe_dir/probe" "$framework" > "$out/values.json"
  if [ "${1:-}" = --record ]; then
    env ARC_RECORD_PATH="$out/record.jsonl" \
      ARC_UI_RECORDER_LIBRARY="$E/tests/recorder/ArcEngineRecorder.dylib" \
      "$probe_dir/probe" "$framework" > "$out/recorded-values.json"
  fi
done
python3 - "${1:-}" <<'PY'
import difflib
import json
import subprocess
import sys
from pathlib import Path

root = Path('results/ui-values')
original = json.loads((root / 'original/values.json').read_text())
ours = json.loads((root / 'ours/values.json').read_text())
differences = []
for section in ('values', 'metadata'):
    if original[section] != ours[section]:
        before = json.dumps(original[section], indent=2, sort_keys=True).splitlines()
        after = json.dumps(ours[section], indent=2, sort_keys=True).splitlines()
        differences.extend(difflib.unified_diff(before, after,
                           fromfile=f'original/{section}', tofile=f'ours/{section}'))
summary = [f'Values: {"MATCH" if original["values"] == ours["values"] else "DIFFER"}',
           f'Public metadata: {"MATCH" if original["metadata"] == ours["metadata"] else "DIFFER"}']
(root / 'comparison.txt').write_text('\n'.join(summary + differences) + '\n')
print('\n'.join(summary))
if sys.argv[1] == '--record':
    for name, expected in (('original', original), ('ours', ours)):
        recorded = json.loads((root / name / 'recorded-values.json').read_text())
        if expected != recorded:
            differences.append(f'{name}: recorder altered values or metadata')
        calls = [json.loads(line) for line in (root / name / 'record.jsonl').read_text().splitlines()]
        observed = {row.get('cls') for row in calls if row.get('ev') == 'call'}
        required = {'ArcNotificationButton', 'ArcNotificationItem', 'ArcKeyboardAccelerator'}
        if not required <= observed:
            differences.append(f'{name}: missing recorder classes {sorted(required-observed)}')
    with (root / 'diff.txt').open('w') as output:
        subprocess.run(['python3', 'tests/recorder/diff.py',
                        str(root / 'original/record.jsonl'), str(root / 'ours/record.jsonl'),
                        '--json', str(root / 'diff.json')], stdout=output, check=True)
    print('\n'.join((root / 'diff.txt').read_text().splitlines()[:2]))
    report = json.loads((root / 'diff.json').read_text())
    for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch'):
        if report[key]:
            differences.append(f'boundary diff has {key}')
    if report['callback_sequence']['first_divergence'] is not None:
        differences.append('callback sequence differs')
if differences:
    print('Inspect results/ui-values/comparison.txt and optional recorder diff.')
    raise SystemExit(1)
PY
