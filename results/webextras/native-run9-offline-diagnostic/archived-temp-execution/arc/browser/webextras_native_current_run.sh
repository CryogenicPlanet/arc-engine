#!/bin/bash
# One fresh build/package/probe slot. Only disposable ArcModAgent apps are used.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"
locked=0
if [ "${1:-}" = --locked ]; then
  locked=1
  shift
fi
run_number=4
media_clock=0
media_service_clock=0
seen_run_number=0
while [ "$#" -gt 0 ]; do
  case "$1" in
    --run-number)
      [ "$seen_run_number" -eq 0 ] && [ "$#" -ge 2 ] && [[ "$2" =~ ^[1-9][0-9]*$ ]] || { echo "invalid or repeated --run-number" >&2; exit 2; }
      run_number="$2"; seen_run_number=1; shift 2 ;;
    --media-clock)
      [ "$media_clock" -eq 0 ] || { echo "repeated --media-clock" >&2; exit 2; }
      media_clock=1; shift ;;
    --media-service-clock)
      [ "$media_service_clock" -eq 0 ] || { echo "repeated --media-service-clock" >&2; exit 2; }
      media_service_clock=1; shift ;;
    *) echo "usage: $0 [--run-number N] [--media-clock | --media-service-clock]" >&2; exit 2 ;;
  esac
done
[ "$media_clock" -eq 0 ] || [ "$media_service_clock" -eq 0 ] || { echo "clock modes are mutually exclusive" >&2; exit 2; }
clock_args=()
[ "$media_clock" -eq 0 ] || clock_args=(--media-clock)
[ "$media_service_clock" -eq 0 ] || clock_args=(--media-service-clock)
if [ "$locked" -ne 1 ]; then
  exec "$ROOT/arc/tools/locked.sh" bash "$0" --locked --run-number "$run_number" ${clock_args[@]+"${clock_args[@]}"}
fi
python3 - "$ROOT" <<'PY_LOCK'
import os, pathlib, subprocess, sys
owner = int((pathlib.Path(sys.argv[1])/'logs/arccore-build.lock/pid').read_text())
ancestor = os.getppid()
for _ in range(20):
    if ancestor == owner:
        break
    assert ancestor > 1, '--locked requires an ancestor holding the shared lock'
    ancestor = int(subprocess.check_output(['ps', '-p', str(ancestor), '-o', 'ppid='], text=True))
assert ancestor == owner, '--locked requires an ancestor holding the shared lock'
PY_LOCK
stem="$ROOT/logs/wp/webextras.native.run$run_number"
output="$ROOT/results/webextras/native-run$run_number"
[ ! -e "$output" ] && [ ! -e "$stem.source-before.json" ] || { echo "Refusing prior native-run$run_number artifacts" >&2; exit 2; }
source_snapshot() {
  python3 - "$ROOT" "$stem" "$1" "$media_clock" "$media_service_clock" <<'PY'
import datetime, hashlib, json, pathlib, sys
root, stem, phase = pathlib.Path(sys.argv[1]), sys.argv[2], sys.argv[3]
assert not (sys.argv[4] == '1' and sys.argv[5] == '1'), 'clock modes are mutually exclusive'
mode = 'media-clock' if sys.argv[4] == '1' else 'media-service-clock' if sys.argv[5] == '1' else 'default'
paths = sorted(root.glob('arc/api/arc_webextras_*.mm')) + [root/path for path in (
    'arc/browser/webextras_media.h', 'arc/browser/webextras_values.h',
    'arc/wp/webextras.gni', 'arc/browser/webextras_native_probe.m',
    'arc/browser/webextras_native_probe.ts', 'arc/browser/webextras_native_probe.sh',
    'arc/browser/webextras_fixture_inputs.py', 'arc/browser/webextras_fixture_projection.ts',
    'arc/browser/webextras_native_current_run.sh')]
# Both opt-in lanes freeze the same16 inventory; service-only does not load/serve these video assets.
if mode != 'default':
    paths += [root/path for path in ('arc/browser/webextras_media_clock_fixture.webm',
                                    'arc/browser/webextras_media_clock_fixture.json')]
files = {}
for path in paths:
    before = path.stat()
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    after = path.stat()
    assert (before.st_ino, before.st_size, before.st_mtime_ns, before.st_ctime_ns) == (
        after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns), 'source changed during snapshot'
    files[str(path.relative_to(root))] = {'sha256': digest, 'size': after.st_size,
                                        'mtimeNs': after.st_mtime_ns}
state = {'capturedAt': datetime.datetime.now(datetime.timezone.utc).isoformat(),
         'phase': phase, 'probeMode': mode, 'mediaResourceUsed': mode == 'media-clock', 'scope': 'owned production and native probe source only', 'files': files}
pathlib.Path(stem+'.source-'+phase+'.json').write_text(json.dumps(state, indent=2)+'\n')
if phase != 'before':
    original = json.loads(pathlib.Path(stem+'.source-before.json').read_text())
    assert original['probeMode'] == mode and original['files'] == files, 'owned production/probe source drifted during build/package/run'
PY
}
source_snapshot before
python3 arc/tools/gen_api_table.py > "$stem.api-table.log" 2>&1
ARC_BUILD_FAIL_FAST=1 ./build-babysit.sh > "$stem.engine-build.log" 2>&1
./package-engine.sh ours156 > "$stem.package.log" 2>&1
bash arc/browser/webextras_native_probe.sh build > "$stem.probe-build.log" 2>&1
source_snapshot after-build
set +e
bash arc/browser/webextras_native_probe.sh run --out "$output" ${clock_args[@]+"${clock_args[@]}"} > "$stem.log" 2>&1
probe_status=$?
set -e
source_snapshot after-run
exit "$probe_status"
