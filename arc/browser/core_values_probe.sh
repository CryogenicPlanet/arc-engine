#!/bin/bash
# Compare core values on completed frameworks, without starting a browser.
# This only compiles a standalone probe; it does not build or package ArcCore.
set -euo pipefail
engine_root="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$engine_root"
probe_dir="$(mktemp -d /tmp/arccore-core-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
python3 - "$probe_dir" <<'BUILD'
import os, shlex, subprocess, sys
from pathlib import Path
root = Path.cwd()
cwd = root / 'chromium/src/out/Base'
tmp = Path(sys.argv[1])
env = {**os.environ, 'PATH': str(root/'depot_tools') + ':' + os.environ['PATH']}
p = subprocess.Popen(['siso', 'query', 'commands', '-C', '.',
                      'obj/arc/arc_core/arc_navigation.o'], cwd=cwd, env=env,
                     stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
command = None
for line in p.stdout:
    text = line.decode(errors='replace')
    if ' -c ../../arc/api/arc_navigation.mm ' in text:
        command = shlex.split(text)
        break
p.stdout.close()
p.terminate()
p.wait()
if not command:
    raise SystemExit('arc_navigation compile command not found')
args = []
i = 0
while i < len(command):
    value = command[i]
    if value in ('-o', '-MF'):
        i += 2
        continue
    if value == '-MMD':
        i += 1
        continue
    args.append(value.replace('../../arc/api/arc_navigation.mm',
                              '../../arc/browser/core_values_probe.mm'))
    i += 1
subprocess.run(args + ['-o', str(tmp/'probe.o')], cwd=cwd, check=True)
subprocess.run([command[0], '-fuse-ld=lld', '-isysroot',
                'sdk/xcode_links/MacOSX26.0.sdk', '-nostdlib++', str(tmp/'probe.o'),
                str(cwd/'obj/buildtools/third_party/libc++/libc++.a'),
                str(cwd/'obj/buildtools/third_party/libc++abi/libc++abi.a'),
                '-framework', 'AppKit', '-framework', 'Foundation',
                '-o', str(tmp/'probe')], cwd=cwd, check=True)
BUILD
original="$engine_root/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$engine_root/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"
  [ "$name" != ours ] || framework="$ours"
  out="$engine_root/results/core-values/$name"
  mkdir -p "$out"
  python3 - "$probe_dir/probe" "$framework" "$out" <<'PROBE'
import datetime, hashlib, json, os, subprocess, sys
from pathlib import Path
probe, framework, output = sys.argv[1:]
binary, out = Path(framework), Path(output)
def fingerprint():
    digest = hashlib.sha256()
    with binary.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            digest.update(block)
    return {'sha256': digest.hexdigest(), 'size': binary.stat().st_size,
            'mtime_ns': binary.stat().st_mtime_ns}
before = fingerprint()
started = datetime.datetime.now(datetime.timezone.utc).isoformat()
with (out/'values.json').open('w') as stream:
    subprocess.run([probe, framework], stdout=stream, check=True)
environment = {**os.environ, 'ARC_RECORD_PATH': str(out/'record.jsonl'),
               'ARC_CORE_VALUES_RECORDER_LIBRARY':
               str(Path.cwd()/'tests/recorder/ArcEngineRecorder.dylib')}
with (out/'recorded-values.json').open('w') as stream:
    subprocess.run([probe, framework], stdout=stream, env=environment, check=True)
after = fingerprint()
stable = before == after
(out/'provenance.json').write_text(json.dumps({
    'framework': str(binary.resolve()), 'started_utc': started,
    'before': before, 'after': after, 'stable': stable}, indent=2) + '\n')
if not stable:
    raise SystemExit('Framework changed during probe; results require a retry')
PROBE
done
python3 - <<'COMPARE'
import json, subprocess
from pathlib import Path
root = Path('results/core-values')
for filename in ('values.json', 'recorded-values.json'):
    original = json.loads((root/'original'/filename).read_text())
    ours = json.loads((root/'ours'/filename).read_text())
    if original != ours:
        for key in sorted(original.keys() | ours.keys()):
            if original.get(key) != ours.get(key):
                print('DIFFERENCE', key, original.get(key), ours.get(key))
        raise SystemExit(1)
    print(filename + ': exact match')
with (root/'diff.txt').open('w') as output:
    subprocess.run(['python3', 'tests/recorder/diff.py',
                    str(root/'original/record.jsonl'),
                    str(root/'ours/record.jsonl'), '--json', str(root/'diff.json')],
                   stdout=output, check=True)
print('\n'.join((root/'diff.txt').read_text().splitlines()[:2]))
COMPARE
