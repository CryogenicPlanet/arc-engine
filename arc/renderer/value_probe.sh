#!/bin/bash
# Bounded value ABI and boundary comparison; no browser/profile initialization.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked
fi
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-renderer-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
clang++ -fobjc-arc -framework Foundation arc/renderer/value_probe.mm -o "$probe_dir/probe"
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"
  [ "$name" != ours ] || framework="$ours"
  out="$E/results/renderer-values/$name"
  mkdir -p "$out"
  env ARC_RECORD_PATH="$out/record.jsonl" \
    ARC_RENDERER_RECORDER_LIBRARY="$E/tests/recorder/ArcEngineRecorder.dylib" \
    "$probe_dir/probe" "$framework" > "$out/values.txt"
done
python3 tests/recorder/diff.py results/renderer-values/original/record.jsonl \
  results/renderer-values/ours/record.jsonl --json results/renderer-values/diff.json \
  > results/renderer-values/diff.txt
head -2 results/renderer-values/diff.txt
python3 - <<'PY'
import json
from pathlib import Path
root = Path('results/renderer-values')
def surface(name):
    return sorted(line for line in (root / name / 'values.txt').read_text().splitlines()
                  if '\t' in line and not any(part in line for part in
                      ('.cxx_', '\tinitWithArcBrowserContext:', 'optional<')))
if surface('original') != surface('ours'):
    raise SystemExit('public method/property metadata differs; inspect values.txt')
report = json.loads((root / 'diff.json').read_text())
for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch'):
    if report[key]:
        raise SystemExit(f'boundary diff has {key}: inspect diff.json')
for name in ('original', 'ours'):
    rows = [json.loads(line) for line in (root / name / 'record.jsonl').read_text().splitlines()]
    calls = [r for r in rows if r.get('ev') == 'call']
    if len({r['cls'] for r in calls}) != 4:
        raise SystemExit(f'{name}: incomplete recorder coverage')
    print(f'{name}: {len(calls)} calls across four value classes')
print('PASS: exact public method/property metadata and recorded value boundary')
PY
