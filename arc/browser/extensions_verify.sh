#!/bin/bash
# Package-owned verification batch. Invoke only through arc/tools/locked.sh.
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"
OUT="results/extensions-validation4"
if [ -e "$OUT" ]; then
  echo "refusing to overwrite $OUT" >&2
  exit 2
fi
mkdir -p "$OUT"
python3 arc/tools/gen_api_table.py || exit $?
ARC_BUILD_FAIL_FAST=1 ./build-babysit.sh > "$OUT/build.log" 2>&1 || exit $?
./package-engine.sh ours156 > "$OUT/package.log" 2>&1 || exit $?
arc/browser/extensions_service_probe.sh run --out "$OUT/service" --agents trace156,ours156 --port 9458 > "$OUT/service.log" 2>&1
service_status=$?
echo "service exit $service_status"
scenario_status=0
for scenario in launch-slice extensions-page-actions; do
  # Only this disposable app's data is reset, after checking that it has exited.
  python3 - <<'PY' || exit $?
from pathlib import Path
import shutil, subprocess
root = Path.cwd()
exe = str((root / '../arc/build/ArcModAgent-ours156.app/Contents/MacOS/Arc').resolve())
commands = subprocess.check_output(['ps', '-axo', 'command='], text=True).splitlines()
assert not any(c == exe or c.startswith(exe + ' ') for c in commands), 'ours156 remains running; refusing data reset'
data = Path.home() / 'Library/Application Support/ArcModAgent-ours156'
if data.exists():
    shutil.rmtree(data)
PY
  bun tests/launch-slice/run.ts --app "$ROOT/../arc/build/ArcModAgent-ours156.app" --out "$OUT/$scenario" --record --scenario "$scenario" > "$OUT/$scenario.log" 2>&1
  status=$?
  echo "$scenario harness exit $status"
  if [ "$status" -ne 0 ]; then scenario_status=$status; fi
  for golden in results/golden/arc-1.170-"$scenario"/launch-*/record.jsonl; do
    launch="$(basename "$(dirname "$golden")")"
    ours="$OUT/$scenario/$launch/record.jsonl"
    if [ ! -f "$ours" ]; then
      echo "$scenario $launch: missing transcript"
      scenario_status=1
      continue
    fi
    python3 tests/recorder/diff.py "$golden" "$ours" --json "$OUT/$scenario/diff-$launch.json" > "$OUT/$scenario/diff-$launch.txt" || scenario_status=1
    echo "$scenario $launch"
    head -n 2 "$OUT/$scenario/diff-$launch.txt"
  done
done
if [ "$service_status" -ne 0 ] || [ "$scenario_status" -ne 0 ]; then exit 1; fi
