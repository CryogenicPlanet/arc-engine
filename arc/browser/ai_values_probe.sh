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
for name in original ours; do
  framework="$original"
  [ "$name" != ours ] || framework="$ours"
  out="$E/results/ai-values/$name"
  mkdir -p "$out"
  "$probe_dir/probe" "$framework" --observe-unresolved > "$out/values.json"
  env ARC_RECORD_PATH="$out/record.jsonl" \
    DYLD_INSERT_LIBRARIES="$E/tests/recorder/ArcEngineRecorder.dylib" \
    "$probe_dir/probe" "$framework" --observe-unresolved > "$out/recorded-values.json"
done
python3 tests/recorder/diff.py results/ai-values/original/record.jsonl \
  results/ai-values/ours/record.jsonl --json results/ai-values/diff.json \
  > results/ai-values/diff.txt
head -2 results/ai-values/diff.txt
python3 - <<'PY'
import json
from pathlib import Path

root = Path('results/ai-values')
for filename in ('values.json', 'recorded-values.json'):
    original = json.loads((root / 'original' / filename).read_text())
    ours = json.loads((root / 'ours' / filename).read_text())
    if original != ours:
        keys = sorted(k for k in original.keys() | ours.keys()
                      if original.get(k) != ours.get(k))
        raise SystemExit(f'{filename}: mismatching sections: {keys}')
    print(f'{filename}: exact values and public metadata match')
report = json.loads((root / 'diff.json').read_text())
for key in ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch'):
    if report[key]:
        raise SystemExit(f'boundary diff has {key}: inspect results/ai-values/diff.json')
if report['callback_sequence']['first_divergence'] is not None:
    raise SystemExit('callback sequence differs')
PY
