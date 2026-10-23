#!/bin/bash
# Compare the bounded AI DTO surface on the original and freshly built engines.
# This does not establish inference/ranking service or app-scenario acceptance.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked
fi
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-ai-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
clang++ -fobjc-arc -framework Foundation arc/browser/ai_values_probe.mm \
  -o "$probe_dir/probe"
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
rm -f "$E/results/ai-values/diff.txt" "$E/results/ai-values/diff.json"
for name in original ours; do
  framework="$original"
  [ "$name" != ours ] || framework="$ours"
  out="$E/results/ai-values/$name"
  mkdir -p "$out"
  "$probe_dir/probe" "$framework" --observe-unresolved > "$out/values.json"
  env ARC_RECORD_PATH="$out/record.jsonl" \
    ARC_AI_RECORDER_LIBRARY="$E/tests/recorder/ArcEngineRecorder.dylib" \
    "$probe_dir/probe" "$framework" --observe-unresolved > "$out/recorded-values.json"
done
python3 - <<'PY'
import json
import subprocess
from pathlib import Path

root = Path('results/ai-values')
surface = json.loads((root / 'original' / 'values.json').read_text())['metadata']
expected = {(name, selector) for name, item in surface.items()
            for selector in item['methods']}
for name in ('original', 'ours'):
    rows = [json.loads(line) for line in
            (root / name / 'record.jsonl').read_text().splitlines()]
    calls = [r for r in rows if r.get('ev') == 'call']
    classes = {r['cls'] for r in calls}
    observed = {(r['cls'], r['sel']) for r in calls}
    if len(classes) < 14:
        raise SystemExit(f'{name}: recorder coverage missing: {len(classes)} classes')
    if expected - observed:
        raise SystemExit(f'{name}: unrecorded public methods: {sorted(expected - observed)}')
    print(f'{name}: {len(calls)} calls across {len(classes)} classes recorded')
for filename in ('values.json', 'recorded-values.json'):
    original = json.loads((root / 'original' / filename).read_text())
    ours = json.loads((root / 'ours' / filename).read_text())
    if original != ours:
        keys = sorted(k for k in original.keys() | ours.keys()
                      if original.get(k) != ours.get(k))
        raise SystemExit(f'{filename}: mismatching sections: {keys}')
    print(f'{filename}: exact values and public metadata match')
for name in ('original', 'ours'):
    raw = json.loads((root / name / 'values.json').read_text())
    recorded = json.loads((root / name / 'recorded-values.json').read_text())
    if raw != recorded:
        raise SystemExit(f'{name}: recorder changed observed values or metadata')
with (root / 'diff.txt').open('w') as output:
    subprocess.run(['python3', 'tests/recorder/diff.py',
                    str(root / 'original' / 'record.jsonl'),
                    str(root / 'ours' / 'record.jsonl'),
                    '--json', str(root / 'diff.json')], stdout=output, check=True)
print('\n'.join((root / 'diff.txt').read_text().splitlines()[:2]))
report = json.loads((root / 'diff.json').read_text())
for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch'):
    if report[key]:
        raise SystemExit(f'boundary diff has {key}: inspect results/ai-values/diff.json')
if report['callback_sequence']['first_divergence'] is not None:
    raise SystemExit('callback sequence differs')
PY
