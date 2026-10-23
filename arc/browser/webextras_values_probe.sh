#!/bin/bash
# Compare standalone value transport. This is not app-scenario acceptance.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked
fi
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-webextras-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
clang++ -std=c++20 -fobjc-arc -framework Foundation arc/browser/webextras_values_probe.mm -o "$probe_dir/probe"
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"; [ "$name" != ours ] || framework="$ours"
  out="$E/results/webextras-values/$name"
  mkdir -p "$out"
  "$probe_dir/probe" "$framework" > "$out/values.json"
  env ARC_RECORD_PATH="$out/record.jsonl" \
    ARC_WEBEXTRAS_RECORDER_LIBRARY="$E/tests/recorder/ArcEngineRecorder.dylib" \
    "$probe_dir/probe" "$framework" > "$out/recorded-values.json"
done
python3 - <<'PY'
import json
import subprocess
from pathlib import Path
root = Path('results/webextras-values')
for filename in ('values.json', 'recorded-values.json'):
    original = json.loads((root/'original'/filename).read_text())
    ours = json.loads((root/'ours'/filename).read_text())
    if original != ours:
        raise SystemExit(f'{filename}: mismatching sections: ' + str([k for k in original if original[k] != ours.get(k)]))
    print(filename + ': values and public method encodings match')
for name in ('original','ours'):
    assert json.loads((root/name/'values.json').read_text()) == json.loads((root/name/'recorded-values.json').read_text()), name
with (root/'diff.txt').open('w') as output:
    subprocess.run(['python3', 'tests/recorder/diff.py', str(root/'original/record.jsonl'),
                    str(root/'ours/record.jsonl'), '--json', str(root/'diff.json')], stdout=output, check=True)
print('\n'.join((root/'diff.txt').read_text().splitlines()[:2]))
report = json.loads((root/'diff.json').read_text())
for key in ('missing','extra','return_mismatch','arg_mismatch','count_mismatch'):
    assert not report[key], key
PY
