#!/bin/bash
# Compare standalone value transport. This is not app-scenario acceptance.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ] && [ "${1:-}" != --standalone ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked
fi
# Standalone mode only loads value classes; it never builds, packages, starts a
# browser, or opens a profile. Before/after identities reject a changed image.
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-webextras-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
clang++ -std=c++20 -fobjc-arc -framework Foundation arc/browser/webextras_values_probe.mm -o "$probe_dir/probe"
recorder="$probe_dir/ArcEngineRecorder.dylib"
mkdir -p "$E/results/webextras-values"
python3 - "$E/tests/recorder/ArcEngineRecorder.dylib" "$recorder" "$E/results/webextras-values/recorder-identity.json" <<'PY'
import json, pathlib, shutil, subprocess, sys
source, frozen, output = map(pathlib.Path, sys.argv[1:])
def identity(path):
    stat = path.stat()
    return {'realpath': str(path.resolve(strict=True)), 'size': stat.st_size,
            'mtimeNs': stat.st_mtime_ns,
            'sha256': subprocess.check_output(['shasum', '-a', '256', str(path)], text=True).split()[0]}
before = identity(source)
shutil.copyfile(source, frozen)
after, snapshot = identity(source), identity(frozen)
assert before == after and snapshot['sha256'] == before['sha256'], 'recorder changed while freezing'
output.write_text(json.dumps({'source': before, 'frozen': snapshot}, indent=2) + '\n')
PY
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"; [ "$name" != ours ] || framework="$ours"
  out="$E/results/webextras-values/$name"
  mkdir -p "$out"
  python3 - "$framework" "$out/provenance-before.json" <<'PY'
import datetime, json, pathlib, subprocess, sys
path = pathlib.Path(sys.argv[1]).resolve(strict=True)
stat = path.stat()
digest = subprocess.check_output(['shasum', '-a', '256', str(path)], text=True).split()[0]
pathlib.Path(sys.argv[2]).write_text(json.dumps({
    'capturedAt': datetime.datetime.now(datetime.timezone.utc).isoformat(),
    'path': sys.argv[1], 'realpath': str(path), 'size': stat.st_size,
    'mtimeNs': stat.st_mtime_ns, 'sha256': digest,
}, indent=2) + '\n')
PY
  "$probe_dir/probe" "$framework" > "$out/values.json"
  env ARC_RECORD_PATH="$out/record.jsonl" \
    ARC_WEBEXTRAS_RECORDER_LIBRARY="$recorder" \
    "$probe_dir/probe" "$framework" > "$out/recorded-values.json"
  python3 - "$framework" "$out" <<'PY'
import datetime, json, pathlib, subprocess, sys
path = pathlib.Path(sys.argv[1]).resolve(strict=True)
out = pathlib.Path(sys.argv[2])
stat = path.stat()
after = {
    'capturedAt': datetime.datetime.now(datetime.timezone.utc).isoformat(),
    'path': sys.argv[1], 'realpath': str(path), 'size': stat.st_size,
    'mtimeNs': stat.st_mtime_ns,
    'sha256': subprocess.check_output(['shasum', '-a', '256', str(path)], text=True).split()[0],
}
(out/'provenance-after.json').write_text(json.dumps(after, indent=2) + '\n')
before = json.loads((out/'provenance-before.json').read_text())
assert all(before[key] == after[key] for key in ('realpath', 'size', 'mtimeNs', 'sha256')), 'framework changed during probe'
PY
done
python3 - <<'PY'
import collections
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
# Caller-attributed recorders identify this standalone executable as "probe".
# The default app-only comparison would discard all of its tested calls.
records = [[json.loads(line) for line in (root/name/'record.jsonl').read_text().splitlines()]
           for name in ('original', 'ours')]
expected = {
    ('ArcAccessibilityActionData', 'initWithAction:'): 4,
    ('ArcAccessibilityActionData', 'initWithAction:value:'): 16,
    ('ArcAccessibilityActionData', 'action'): 20,
    ('ArcAccessibilityActionData', 'value'): 20,
}
expected.update({('ArcAccessibilityTreeNode', selector): 1 for selector in
                 'nodeId role name value isClickable isEditable isFocusable childTreeId children'.split()})
expected.update({('ArcBrowserSidePanelEntryInfo', selector): 1 for selector in
                 'hasActiveEntry extensionId cachedWebContents'.split()})
for name, rows in zip(('original', 'ours'), records):
    assert rows and rows[0].get('ev') == 'hooked' and rows[0].get('seq') == 0 and rows[0].get('engine_classes', 0) > 0, name
    assert sum(row.get('ev') == 'hooked' for row in rows) == 1, name
    sequence = [row['seq'] for row in rows]
    assert sequence == sorted(set(sequence)), name
    counts, stack = collections.Counter(), []
    for row in rows[1:]:
        assert row.get('thread') == 'main' and row.get('dir') == '->engine' and row.get('kind') == '-', name
        key = (row.get('cls'), row.get('sel'))
        if row.get('ev') == 'call':
            assert row.get('depth') == len(stack) and key in expected and 'args' in row, name
            counts[key] += 1
            stack.append(row)
        else:
            assert row.get('ev') == 'ret' and stack and 'ret' in row, name
            call = stack.pop()
            assert row.get('depth') == len(stack) and all(call[field] == row[field] for field in
                   ('thread', 'dir', 'kind', 'cls', 'sel', 'self')), name
    assert not stack and counts == expected, f'{name}: incomplete recorded value coverage: {counts}'
if all(any('from' in row for row in rows) for rows in records):
    raise SystemExit('JSON/metadata parity verified; recorder diff requires an explicit fixture-caller comparison mode')
with (root/'diff.txt').open('w') as output:
    subprocess.run(['python3', 'tests/recorder/diff.py', str(root/'original/record.jsonl'),
                    str(root/'ours/record.jsonl'), '--json', str(root/'diff.json')], stdout=output, check=True)
print('\n'.join((root/'diff.txt').read_text().splitlines()[:2]))
report = json.loads((root/'diff.json').read_text())
for key in ('missing','extra','return_mismatch','arg_mismatch','count_mismatch'):
    assert not report[key], key
PY
